/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include <errno.h>
#include <float.h>
#include <fcntl.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include <mapi.h>
#include <shellapi.h>
#include <shlobj.h>

#ifndef __MRC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <SDL3/SDL_Main.h>

#undef snprintf
#include "../external/subprocess/subprocess.h"

#include "../sys_local.h"
#include "win_local.h"
#include "../../renderer/tr_local.h"

idCVar Win32Vars_t::win_outputEditString( "win_outputEditString", "1", CVAR_SYSTEM | CVAR_BOOL, "" );
idCVar Win32Vars_t::win_viewlog( "win_viewlog", "0", CVAR_SYSTEM | CVAR_INTEGER, "" );
idCVar Win32Vars_t::win_timerUpdate( "win_timerUpdate", "0", CVAR_SYSTEM | CVAR_BOOL, "allows the game to be updated while dragging the window" );

Win32Vars_t win32 = {};

static char		sys_cmdline[MAX_STRING_CHARS];

static HANDLE hProcessMutex;

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
	#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

/*
This class contains code adapted from Blat Blatnik's precise_sleep.c sample
(retrieved 2024-04-10), which is under the Unlicense license:
   https://github.com/blat-blatnik/Snippets/blob/main/precise_sleep.c
   https://github.com/blat-blatnik/Snippets/blob/main/LICENSE
*/
class idTimeHiRes {

public:
				idTimeHiRes();
	void		Init();
	void		Shutdown();
	int64       ClockCount();
	double		ClockCountToMilliseconds( int64 count );
	void		Sleep( double seconds );

private:
	HANDLE Timer;
	int SchedulerPeriodMs;
	int64 QpcPerSecond;
};

idTimeHiRes::idTimeHiRes() {
	Timer = NULL;
	SchedulerPeriodMs = 0;
	QpcPerSecond = 0;
}

void idTimeHiRes::Init() {
	if ( !Timer ) {
		Timer = CreateWaitableTimerEx(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
	}

	TIMECAPS timeCaps;
	bool capsObtained = false;
	if ( timeGetDevCaps(&timeCaps, sizeof(timeCaps)) == MMSYSERR_NOERROR ) {
		SchedulerPeriodMs = (int)timeCaps.wPeriodMin;
		if ( SchedulerPeriodMs > 0 ) {
			timeBeginPeriod((UINT)SchedulerPeriodMs);
			capsObtained = true;
		}
	}
	if ( !capsObtained ) {
		SchedulerPeriodMs = 0;
	}

	LARGE_INTEGER qpf;
	QueryPerformanceFrequency(&qpf);
	QpcPerSecond = qpf.QuadPart;
}

void idTimeHiRes::Shutdown() {
	if ( Timer ) {
		CloseHandle(Timer);
		Timer = NULL;
	}
	if ( SchedulerPeriodMs > 0 ) {
		timeEndPeriod((UINT)SchedulerPeriodMs);
	}
	SchedulerPeriodMs = 0;
}

int64 idTimeHiRes::ClockCount() {
	if ( !idLib::IsMainThread() ) {
		return 0;
	}
	LARGE_INTEGER qpc;
	QueryPerformanceCounter(&qpc);
	int64 count = qpc.QuadPart;
	return count;
}

double idTimeHiRes::ClockCountToMilliseconds( int64 count ) {
	if ( !idLib::IsMainThread() ) {
		return 0.0;
	}
	double msec = ((double)count / (double)QpcPerSecond) * 1000.0;
	return msec;
}

void idTimeHiRes::Sleep( double milliseconds ) {
	if ( !idLib::IsMainThread() ) {
		return;
	}
	double seconds = milliseconds * 0.001;

	LARGE_INTEGER qpc;
	QueryPerformanceCounter(&qpc);
	int64 targetQpc = qpc.QuadPart + (int64)(seconds * (double)QpcPerSecond);

	if ( SchedulerPeriodMs > 0 ) {
		// Try using a high resolution timer first.
		if ( Timer ) {
			const double TOLERANCE = 0.001'02;
			int64 maxTicks = (int64)SchedulerPeriodMs * 9'500;
			// Break sleep up into parts that are lower than scheduler period.
			for (;;) {
				double remainingSeconds = (double)(targetQpc - qpc.QuadPart) / (double)QpcPerSecond;
				int64 sleepTicks = (int64)((remainingSeconds - TOLERANCE) * 10'000'000.0); // 100ns intervals
				if ( sleepTicks <= 0 )
					break;

				LARGE_INTEGER due;
				due.QuadPart = -(sleepTicks > maxTicks ? maxTicks : sleepTicks);
				SetWaitableTimerEx(Timer, &due, 0, NULL, NULL, NULL, 0);
				WaitForSingleObject(Timer, INFINITE);
				QueryPerformanceCounter(&qpc);
			}
		} else { // Fallback to Sleep.
			const double TOLERANCE = 0.000'02;
			double sleepMs = (seconds - TOLERANCE) * 1000.0 - (double)SchedulerPeriodMs; // Sleep for 1 scheduler period less than requested.
			int sleepSlices = (int)(sleepMs / (double)SchedulerPeriodMs);
			if ( sleepSlices > 0 )
				Sys_Sleep((DWORD)sleepSlices * (DWORD)SchedulerPeriodMs);
			QueryPerformanceCounter(&qpc);
		}
	}

	// Spin for any remaining time.
	while ( qpc.QuadPart < targetQpc ) {
		YieldProcessor();
		QueryPerformanceCounter(&qpc);
	}
}

idTimeHiRes sysTimeHiRes;

/*
=============
Sys_Error

Show the early console as an error dialog
=============
*/
void Sys_Error( const char *error, ... ) {
	va_list		argptr;
	char		text[4096];

	va_start( argptr, error );
	vsprintf( text, error, argptr );
	va_end( argptr);

#ifdef _DEBUG
	Conbuf_AppendText( text );
	Conbuf_AppendText( "\n" );

	Sys_ShowConsole();
#endif

	sysTimeHiRes.Shutdown();

	if ( inputDevice ) {
		inputDevice->Shutdown();
	}

	renderSystem->Shutdown();

#ifdef _DEBUG
	Sys_DestroyConsole();
#endif

	if ( SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Fatal Error", text, win32.window ) ) {
		common->Quit(); // notfy common system that we are quitting
		exit (1);
	}
}

/*
========================
SplitCmdLine

Splits a flat command string into an argv array.
Returns the array (caller must free it).
========================
*/
static const char **SplitCmdLine( const char *exePath, const char *cmdline ) {
	// copy so we can tokenize it
	static char buf[1024];
	idStr::Copynz( buf, cmdline, sizeof(buf) );

	// count tokens
	int count = 1; // for exePath
	char *p = buf;
	bool inToken = false;
	while ( *p ) {
		if ( *p == ' ' ) { inToken = false; }
		else if ( !inToken ) { inToken = true; count++; }
		p++;
	}

	const char **argv = (const char **)Mem_Alloc( sizeof(char*) * (count + 1), TAG_CRAP );
	argv[0] = exePath;

	// reset and fill
	idStr::Copynz( buf, cmdline, sizeof(buf) );
	p = buf;
	int i = 1;
	inToken = false;
	while ( *p ) {
		if ( *p == ' ' ) {
			*p = '\0';
			inToken = false;
		} else if ( !inToken ) {
			inToken = true;
			argv[i++] = p;
		}
		p++;
	}
	argv[i] = NULL;
	return argv;
}

/*
========================
Sys_Launch
========================
*/
void Sys_Launch( const char * path, idCmdArgs & args,  void * data, unsigned int dataSize ) {
	const char **argv = SplitCmdLine( Sys_EXEPath(), (const char *)data );

	struct subprocess_s proc;
	if ( subprocess_create( argv, subprocess_option_no_window | subprocess_option_inherit_environment, &proc ) != 0 ) {
		idLib::Error( "Could not start process: '%s'", Sys_EXEPath() );
		Mem_Free( argv );
		return;
	}

	Mem_Free( argv );
	cmdSystem->AppendCommandText( "quit\n" );
}

/*
========================
Sys_GetCmdLine
========================
*/
const char * Sys_GetCmdLine() {
	return sys_cmdline;
}

/*
========================
Sys_ReLaunch
========================
*/
void Sys_ReLaunch( void * data, const unsigned int dataSize ) {
	const char **argv = SplitCmdLine( Sys_EXEPath(), (const char *)data );

#ifdef _WIN32
	// release the mutex so the new instance can acquire it
	CloseHandle( hProcessMutex );
#endif

	struct subprocess_s proc;
	if ( subprocess_create( argv, subprocess_option_no_window | subprocess_option_inherit_environment, &proc ) != 0 ) {
		idLib::Error( "Could not relaunch process: '%s'", Sys_EXEPath() );
		Mem_Free( argv );
		return;
	}

	Mem_Free( argv );
	cmdSystem->AppendCommandText( "quit\n" );
}

/*
==============
Sys_Quit
==============
*/
void Sys_Quit() {
	sysTimeHiRes.Shutdown();
	if ( inputDevice ) {
		inputDevice->Shutdown();
	}
	Sys_DestroyConsole();
	SDL_Quit();
	ExitProcess( 0 );
}


/*
==============
Sys_Printf
==============
*/
#define MAXPRINTMSG 4096
void Sys_Printf( const char *fmt, ... ) {
	char		msg[MAXPRINTMSG];

	va_list argptr;
	va_start(argptr, fmt);
	idStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	va_end(argptr);
	msg[sizeof(msg)-1] = '\0';

	SDL_Log( "%s", msg );

	if ( win32.win_outputEditString.GetBool() && idLib::IsMainThread() ) {
		Conbuf_AppendText( msg );
	}
}

/*
==============
Sys_DebugPrintf
==============
*/
#define MAXPRINTMSG 4096
void Sys_DebugPrintf( const char *fmt, ... ) {
	char msg[MAXPRINTMSG];

	va_list argptr;
	va_start( argptr, fmt );
	idStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	msg[ sizeof(msg)-1 ] = '\0';
	va_end( argptr );

	SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "%s", msg );
}

/*
==============
Sys_DebugVPrintf
==============
*/
void Sys_DebugVPrintf( const char *fmt, va_list arg ) {
	char msg[MAXPRINTMSG];

	idStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, arg );
	msg[ sizeof(msg)-1 ] = '\0';

	SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "%s", msg );
}

/*
==============
Sys_Sleep
==============
*/
void Sys_Sleep( int msec ) {
	SDL_Delay( msec );
}

/*
==============
Sys_EnableThreadAffinity
==============
*/
void Sys_EnableThreadAffinity( bool enable ) {
	static bool isEnabled = false;
	static DWORD_PTR previousAffinityMask = 0;

	if ( !idLib::IsMainThread() ) {
		return;
	}
	if ( enable ) {
		if ( !isEnabled ) {
			isEnabled = true;
			HANDLE hThread = GetCurrentThread();
			previousAffinityMask = SetThreadAffinityMask(hThread, 0x1); // previousAffinityMask will be 0 if this call fails
			Sys_Sleep(0);
		}
	} else {
		if ( isEnabled ) {
			isEnabled = false;
			if ( previousAffinityMask ) {
				HANDLE hThread = GetCurrentThread();
				SetThreadAffinityMask(hThread, previousAffinityMask);
				previousAffinityMask = 0;
				Sys_Sleep(0);
			}
		}
	}
}

/*
==============
Sys_HiResClockCount
==============
*/
int64 Sys_HiResClockCount() {
	return sysTimeHiRes.ClockCount();
}

/*
==============
Sys_HiResClockCountToMilliseconds
==============
*/
double Sys_HiResClockCountToMilliseconds( int64 count ) {
	return sysTimeHiRes.ClockCountToMilliseconds( count );
}

/*
==============
Sys_HiResClockCountToMilliseconds
==============
*/
void Sys_SleepHiRes( double milliseconds ) {
	sysTimeHiRes.Sleep( milliseconds );
}

/*
==============
Sys_ShowWindow
==============
*/
void Sys_ShowWindow( bool show ) {
	if ( show ) {
		SDL_ShowWindow( win32.window );
	} else {
		SDL_HideWindow( win32.window );
	}
}

/*
==============
Sys_IsWindowVisible
==============
*/
bool Sys_IsWindowVisible() {
	SDL_WindowFlags flags = SDL_GetWindowFlags( win32.window );
	return ( flags & ( SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED ) ) == 0;
}

/*
==============
Sys_Mkdir
==============
*/
void Sys_Mkdir( const char *path ) {
	::CreateDirectory( path, NULL );
}

/*
=================
Sys_FileTimeStamp
=================
*/
ID_TIME_T Sys_FileTimeStamp( idFileHandle fp ) {
	FILETIME writeTime;
	GetFileTime( fp, NULL, NULL, &writeTime );

	/*
		FILETIME = number of 100-nanosecond ticks since midnight
		1 Jan 1601 UTC. time_t = number of 1-second ticks since
		midnight 1 Jan 1970 UTC. To translate, we subtract a
		FILETIME representation of midnight, 1 Jan 1970 from the
		time in question and divide by the number of 100-ns ticks
		in one second.
	*/

	SYSTEMTIME base_st = {
		1970,   // wYear
		1,      // wMonth
		0,      // wDayOfWeek
		1,      // wDay
		0,      // wHour
		0,      // wMinute
		0,      // wSecond
		0       // wMilliseconds
	};

	FILETIME base_ft;
	SystemTimeToFileTime( &base_st, &base_ft );

	LARGE_INTEGER itime;
	itime.QuadPart = reinterpret_cast<LARGE_INTEGER&>( writeTime ).QuadPart;
	itime.QuadPart -= reinterpret_cast<LARGE_INTEGER&>( base_ft ).QuadPart;
	itime.QuadPart /= 10000000LL;
	return itime.QuadPart;
}

/*
========================
Sys_Rmdir
========================
*/
bool Sys_Rmdir( const char *path ) {
	return ::RemoveDirectory( path ) != 0;
}

/*
========================
Sys_RemoveFile
========================
*/
void Sys_RemoveFile( const char *path ) {
	::DeleteFile( path );
}

/*
========================
Sys_IsFileWritable
========================
*/
bool Sys_IsFileWritable( const char *path ) {
	struct _stat st;
	if ( _stat( path, &st ) == -1 ) {
		return true;
	}
	return ( st.st_mode & S_IWRITE ) != 0;
}

/*
========================
Sys_OpenOSFile
========================
*/
idFileHandle Sys_OpenOSFile( const char * fileName, int mode ) {
	idFileHandle fp;


	DWORD dwAccess = 0;
	DWORD dwShare = 0;
	DWORD dwCreate = 0;
	DWORD dwFlags = 0;

	if ( mode == FS_WRITE ) {
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwShare = FILE_SHARE_READ;
		dwCreate = CREATE_ALWAYS;
		dwFlags = FILE_ATTRIBUTE_NORMAL;
	} else if ( mode == FS_READ ) {
		dwAccess = GENERIC_READ;
		dwShare = FILE_SHARE_READ;
		dwCreate = OPEN_EXISTING;
		dwFlags = FILE_ATTRIBUTE_NORMAL;
	} else if ( mode == FS_APPEND ) {
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		dwShare = FILE_SHARE_READ;
		dwCreate = OPEN_ALWAYS;
		dwFlags = FILE_ATTRIBUTE_NORMAL;
					}

	fp = CreateFile( fileName, dwAccess, dwShare, NULL, dwCreate, dwFlags, NULL );
	if ( fp == INVALID_HANDLE_VALUE ) {
		return NULL;
				}
	return fp;
}

/*
========================
Sys_CloseOSFile
========================
*/
void Sys_CloseOSFile( idFileHandle fp ) {
	if ( fp != NULL ) {
		::CloseHandle( fp );
	}
}

/*
========================
Sys_DirectFileLength
========================
*/
int Sys_DirectFileLength( idFileHandle o ) {
	return GetFileSize( o, NULL );
}

/*
========================
Sys_IsFolder
========================
*/
sysFolder_t Sys_IsFolder( const char *path ) {
	struct _stat buffer;
	if ( _stat( path, &buffer ) < 0 ) {
		return FOLDER_ERROR;
	}
	return ( buffer.st_mode & _S_IFDIR ) != 0 ? FOLDER_YES : FOLDER_NO;
}

/*
==============
Sys_Cwd
==============
*/
const char *Sys_Cwd() {
	static char cwd[MAX_OSPATH];

	_getcwd( cwd, sizeof( cwd ) - 1 );
	cwd[MAX_OSPATH-1] = 0;

	return cwd;
}

/*
==============
Sys_DefaultBasePath
==============
*/
const char *Sys_DefaultBasePath() {
	return Sys_Cwd();
}

// Vista shit
typedef HRESULT (WINAPI * SHGetKnownFolderPath_t)( const GUID & rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath );
// NOTE: FOLIDERID_SavedGames is already exported from in shell32.dll in Windows 7.  We can only detect
// the compiler version, but that doesn't doesn't tell us which version of the OS we're linking against.
// This GUID value should never change, so we name it something other than FOLDERID_SavedGames to get
// around this problem.
const GUID FOLDERID_SavedGames_IdTech5 = { 0x4c5c32ff, 0xbb9d, 0x43b0, { 0xb5, 0xb4, 0x2d, 0x72, 0xe5, 0x4e, 0xaa, 0xa4 } };

/*
==============
Sys_DefaultSavePath
==============
*/
const char *Sys_DefaultSavePath() {
	static char savePath[ MAX_PATH ];
	memset( savePath, 0, MAX_PATH );

	HMODULE hShell = LoadLibrary( "shell32.dll" );
	if ( hShell ) {
		SHGetKnownFolderPath_t SHGetKnownFolderPath = (SHGetKnownFolderPath_t)GetProcAddress( hShell, "SHGetKnownFolderPath" );
		if ( SHGetKnownFolderPath ) {
			wchar_t * path;
			if ( SUCCEEDED( SHGetKnownFolderPath( FOLDERID_SavedGames_IdTech5, CSIDL_FLAG_CREATE | CSIDL_FLAG_PER_USER_INIT, 0, &path ) ) ) {
				if ( wcstombs( savePath, path, MAX_PATH ) > MAX_PATH ) {
					savePath[0] = 0;
				}
				CoTaskMemFree( path );
			}
		}
		FreeLibrary( hShell );
	}

	if ( savePath[0] == 0 ) {
		SHGetFolderPath( NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, savePath );
		strcat( savePath, "\\My Games" );
	}

	strcat( savePath, SAVE_PATH );

	return savePath;
}

/*
==============
Sys_EXEPath
==============
*/
const char *Sys_EXEPath() {
	static char exe[ MAX_OSPATH ];
	GetModuleFileName( NULL, exe, sizeof( exe ) - 1 );
	return exe;
}

/*
==============
Sys_ListFiles
==============
*/
int Sys_ListFiles( const char *directory, const char *extension, idStrList &list ) {
	idStr		search;
	struct _finddata_t findinfo;
	intptr_t	findhandle;
	int			flag;

	if ( !extension) {
		extension = "";
	}

	// passing a slash as extension will find directories
	if ( extension[0] == '/' && extension[1] == 0 ) {
		extension = "";
		flag = 0;
	} else {
		flag = _A_SUBDIR;
	}

	sprintf( search, "%s\\*%s", directory, extension );

	// search
	list.Clear();

	findhandle = _findfirst( search, &findinfo );
	if ( findhandle == -1 ) {
		return -1;
	}

	do {
		if ( flag ^ ( findinfo.attrib & _A_SUBDIR ) ) {
			list.Append( findinfo.name );
		}
	} while ( _findnext( findhandle, &findinfo ) != -1 );

	_findclose( findhandle );

	return list.Num();
}


/*
===========
Sys_GetClipboardData
===========
*/
char *Sys_GetClipboardData() {
	char *txt = SDL_GetClipboardText();
	if ( txt == NULL || txt[0] == '\0' ) {
		SDL_free( txt );
		return NULL;
	}
	char *ret = Mem_CopyString( txt );
	SDL_free( txt );
	return ret;
}

/*
===============
Sys_SetClipboardData
===============
*/
void Sys_SetClipboardData( const char *string ) {
	SDL_SetClipboardText( string );
}

/*
========================
ExecOutputFn
========================
*/
static void ExecOutputFn( const char * text ) {
	idLib::Printf( text );
}


/*
========================
Sys_Exec
 
if waitMsec is INFINITE, completely block until the process exits
If waitMsec is -1, don't wait for the process to exit
Other waitMsec values will allow the workFn to be called at those intervals.
========================
*/
bool Sys_Exec( const char *appPath, const char *workingPath, const char *args,
			   execProcessWorkFunction_t workFn, execOutputFunction_t outputFn,
			   const int waitMS, unsigned int &exitCode ) {
 
	exitCode = 0;
 
	if ( outputFn != NULL ) {
		outputFn( va( "^2Executing Process: ^7%s\n^2working path: ^7%s\n^2args: ^7%s\n",
					  appPath, workingPath, args ) );
	} else {
		outputFn = ExecOutputFn;
	}
 
	const char **argv = SplitCmdLine( appPath, args ? args : "" );
 
	int options = subprocess_option_combined_stdout_stderr
				| subprocess_option_enable_async
				| subprocess_option_no_window;
 
	// subprocess doesn't support setting a working directory natively,
	// so we temporarily chdir if a working path is provided.
	char prevDir[MAX_OSPATH] = {};
	if ( workingPath && workingPath[0] ) {
#ifdef _WIN32
		_getcwd( prevDir, sizeof( prevDir ) );
		_chdir( workingPath );
#else
		getcwd( prevDir, sizeof( prevDir ) );
		chdir( workingPath );
#endif
	}
 
	struct subprocess_s proc;
	int ret = subprocess_create( argv, options, &proc );
 
	// Restore working directory immediately after spawn
	if ( prevDir[0] ) {
#ifdef _WIN32
		_chdir( prevDir );
#else
		chdir( prevDir );
#endif
	}
 
	Mem_Free( argv );
 
	if ( ret != 0 ) {
		outputFn( va( "Sys_Exec: subprocess_create failed for '%s'\n", appPath ) );
		return false;
	}
 
	if ( waitMS == -1 ) {
		// Fire and forget — don't wait for the process to exit
		return true;
	}
 
	char buffer[4096];
 
	if ( waitMS == (int)0xFFFFFFFF /* INFINITE */ ) {
		// Block until done, reading all output via subprocess_read_stdout
		// Returns 0 only when the process has completed and all output is drained
		unsigned bytesRead;
		while ( ( bytesRead = subprocess_read_stdout( &proc, buffer, sizeof( buffer ) - 1 ) ) > 0 ) {
			// Strip \r for cross-platform consistency
			int len = 0;
			for ( unsigned i = 0; i < bytesRead; i++ ) {
				if ( buffer[i] != '\r' ) {
					buffer[len++] = buffer[i];
				}
			}
			buffer[len] = '\0';
			if ( len > 0 ) {
				outputFn( buffer );
			}
		}
	} else {
		// Poll at waitMS intervals using subprocess_alive, reading output between sleeps
		while ( subprocess_alive( &proc ) ) {
			unsigned bytesRead = subprocess_read_stdout( &proc, buffer, sizeof( buffer ) - 1 );
			if ( bytesRead > 0 ) {
				int len = 0;
				for ( unsigned i = 0; i < bytesRead; i++ ) {
					if ( buffer[i] != '\r' ) {
						buffer[len++] = buffer[i];
					}
				}
				buffer[len] = '\0';
				if ( len > 0 ) {
					outputFn( buffer );
				}
			}
 
			if ( workFn != NULL && !workFn() ) {
				subprocess_terminate( &proc );
				break;
			}
 
			Sys_Sleep( waitMS > 0 ? waitMS : 4 );
		}
 
		// Drain any remaining output after process exits
		unsigned bytesRead;
		while ( ( bytesRead = subprocess_read_stdout( &proc, buffer, sizeof( buffer ) - 1 ) ) > 0 ) {
			int len = 0;
			for ( unsigned i = 0; i < bytesRead; i++ ) {
				if ( buffer[i] != '\r' ) buffer[len++] = buffer[i];
			}
			buffer[len] = '\0';
			if ( len > 0 ) outputFn( buffer );
		}
	}
 
	int procExitCode = 0;
	subprocess_join( &proc, &procExitCode );
	subprocess_destroy( &proc );
 
	exitCode = (unsigned int)procExitCode;
	return true;
}

/*
========================================================================

DLL Loading

========================================================================
*/

/*
=====================
Sys_DLL_Load
=====================
*/
uintptr_t Sys_DLL_Load( const char *dllName ) {
	SDL_SharedObject *libHandle = SDL_LoadObject( dllName );
	if ( !libHandle ) {
		const char *err = SDL_GetError();
		if ( err && *err ) {
			common->Warning( "SDL_LoadObject( \"%s\" ) Failed! %s", dllName, err );
		}
		return 0;
	}
	return (uintptr_t)libHandle;
}

/*
=====================
Sys_DLL_GetProcAddress
=====================
*/
void *Sys_DLL_GetProcAddress( uintptr_t dllHandle, const char *procName ) {
	void *adr = SDL_LoadFunction( (SDL_SharedObject *)dllHandle, procName );
	if ( !adr ) {
		const char *err = SDL_GetError();
		if ( err && *err ) {
			common->Warning( "SDL_LoadFunction( %zu, \"%s\" ) Failed! %s",
							 (size_t)dllHandle, procName, err );
		}
	}
	return adr;
}

/*
=====================
Sys_DLL_Unload
=====================
*/
void Sys_DLL_Unload( uintptr_t dllHandle ) {
	if ( !dllHandle ) {
		return;
	}
	SDL_UnloadObject( (SDL_SharedObject *)dllHandle );
}

/*
================
Sys_Init

The cvar system must already be setup
================
*/

void Sys_Init() {
	int cpuFlags = Sys_GetProcessorId();

	CoInitialize( NULL );

	//
	// Windows version
	//
	win32.osversion.dwOSVersionInfoSize = sizeof( win32.osversion );

	if ( !GetVersionEx( (LPOSVERSIONINFO)&win32.osversion ) )
		Sys_Error( "Couldn't get OS info" );

	if ( win32.osversion.dwMajorVersion < 7 ) {
		Sys_Error( GAME_NAME " requires Windows 7 SP1 or greater" );
	}

	if ( win32.osversion.dwMajorVersion >= 6 && win32.osversion.dwMinorVersion == 1 && ( win32.osversion.dwBuildNumber == 7600 || win32.osversion.dwBuildNumber == 7601 ) ) {
		common->Printf( "Detected: Windows 7 %u.%u (Build: %u)\n", win32.osversion.dwMajorVersion, win32.osversion.dwMinorVersion, win32.osversion.dwBuildNumber );
	} else if ( win32.osversion.dwMajorVersion >= 6 && win32.osversion.dwMinorVersion == 2 && ( win32.osversion.dwBuildNumber == 9200 ) ) {
		common->Printf( "Detected: Windows 8 %u.%u (Build: %u)\n", win32.osversion.dwMajorVersion, win32.osversion.dwMinorVersion, win32.osversion.dwBuildNumber );
	} else if ( win32.osversion.dwMajorVersion >= 6 && win32.osversion.dwMinorVersion == 3 && ( win32.osversion.dwBuildNumber == 9200 || win32.osversion.dwBuildNumber == 9600 ) ) {
		common->Printf( "Detected: Windows 8.1 %u.%u (Build: %u)\n", win32.osversion.dwMajorVersion, win32.osversion.dwMinorVersion, win32.osversion.dwBuildNumber );
	} else if ( win32.osversion.dwMajorVersion >= 10 && win32.osversion.dwBuildNumber < 21996 ) {
		common->Printf( "Detected: Windows 10 %u.%u (Build: %u)\n", win32.osversion.dwMajorVersion, win32.osversion.dwMinorVersion, win32.osversion.dwBuildNumber );
	} else if ( win32.osversion.dwMajorVersion >= 10 && win32.osversion.dwBuildNumber >= 21996 ) {
		common->Printf( "Detected: Windows 11 %u.%u (Build: %u)\n", win32.osversion.dwMajorVersion, win32.osversion.dwMinorVersion, win32.osversion.dwBuildNumber );
	} else {
		common->Printf( "Detected: Windows (Proton/Wine) %u.%u (Build: %u)", win32.osversion.dwMajorVersion, win32.osversion.dwMinorVersion, win32.osversion.dwBuildNumber );
	}

	//
	// CPU type
	//
	common->Printf( "CPU Name: %s\n", Sys_GetProcessorString() );
	common->Printf( "%1.0f CPU MHz ", Sys_ClockTicksPerSecond() / 1000000.0f );

	common->Printf( "%d MB System Memory\n", Sys_GetSystemRam() );
	if ( ( cpuFlags & CPUID_SSE ) == 0 ) {
		common->Error( "SSE not supported!" );
	}

	//
	// Joystick
	//
	if ( joystick ) {
		joystick->Init();
	}
}

/*
================
Sys_Shutdown
================
*/
void Sys_Shutdown() {
	for ( int i = 0; i < MAX_CRITICAL_SECTIONS; i++ ) {
		Sys_MutexDestroy( win32.criticalSections[i] );
	}

	CoUninitialize();
}

/*
====================
Win_Frame
====================
*/
void Win_Frame() {
	// if "viewlog" has been modified, show or hide the log console
	if ( win32.win_viewlog.IsModified() ) {
		win32.win_viewlog.ClearModified();
		if ( win32.win_viewlog.GetBool() ) {
			Sys_ShowConsole();
		} else {
			Sys_HideConsole();
		}
	}
}

/*
==================
WinMain
==================
*/
int main( int argc, char **argv ) {
	//::SetCursor( NULL );

	Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );

	// combine the args into a windows-style command line
	sys_cmdline[0] = 0;
	for ( int i = 1 ; i < argc ; i++ ) {
		strcat( sys_cmdline, argv[i] );
		if ( i < argc - 1 ) {
			strcat( sys_cmdline, " " );
		}
	}

	// We need to create the mutexes before anything else
	for ( int i = 0; i < MAX_CRITICAL_SECTIONS; i++ ) {
		Sys_MutexCreate( win32.criticalSections[i] );
	}

	// done before Com/Sys_Init since we need this for error output
	Sys_CreateConsole();

	// Register the unhandled exception
	LONG CALLBACK Sys_UnhandledExceptionFilter( EXCEPTION_POINTERS * exceptionInfo );
	SetUnhandledExceptionFilter( Sys_UnhandledExceptionFilter );

	// no abort/retry/fail errors
	SetErrorMode( SEM_FAILCRITICALERRORS );

	// make sure the timer is high precision, otherwise
	// NT gets 18ms resolution
	sysTimeHiRes.Init();

	// get the initial time base
	Sys_Milliseconds();

#ifdef DEBUG
	// disable the painfully slow MS heap check every 1024 allocs
	_CrtSetDbgFlag( 0 );
#endif

	common->Init( 0, NULL, sys_cmdline );

	// hide or show the early console as necessary
#ifndef _DEBUG
	if( win32.win_viewlog.GetInteger() ) {
		Sys_ShowConsole();
	} else {
		Sys_HideConsole();
	}
#else
	Sys_ShowConsole();
#endif

#ifdef SET_THREAD_AFFINITY
	// give the main thread an affinity for the first cpu
	SetThreadAffinityMask( GetCurrentThread(), 1 );
#endif

	//::SetFocus( win32.hWnd );

	// main game loop
	while( 1 ) {

		Win_Frame();

#ifdef DEBUG
		Sys_MemFrame();
#endif

		// run the game
		common->Frame();
	}

	// never gets here
	return 0;
}

/*
==================
idSysLocal::OpenURL
==================
*/
void idSysLocal::OpenURL( const char *url, bool doexit ) {
	static bool doexit_spamguard = false;

	if (doexit_spamguard) {
		common->DPrintf( "OpenURL: already in an exit sequence, ignoring %s\n", url );
		return;
	}

	common->Printf("Open URL: %s\n", url);

	if ( !SDL_OpenURL( url ) ) {
		common->Error( "Could not open url: '%s' — %s", url, SDL_GetError() );
		return;
	}

	if ( doexit ) {
		doexit_spamguard = true;
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
	}
}

/*
==================
idSysLocal::StartProcess
==================
*/
void idSysLocal::StartProcess( const char *exePath, bool doexit ) {
	const char **argv = SplitCmdLine( Sys_EXEPath(), (const char *)exePath );

	struct subprocess_s proc;
	if ( subprocess_create( argv, subprocess_option_no_window | subprocess_option_inherit_environment, &proc ) != 0 ) {
		idLib::Error( "Could not start process: '%s'", Sys_EXEPath() );
		Mem_Free( argv );
		return;
	}

	Mem_Free( argv );
	if ( doexit ) {
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
	}
}