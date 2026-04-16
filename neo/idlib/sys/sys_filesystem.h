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
#ifndef __SYS_FILESYSTEM_H__
#define __SYS_FILESYSTEM_H__

// creates a directory at the specified path, including any parent directories that don't exist.
void			Sys_Mkdir( const char *path );

// removes the directory at the specified path.
bool			Sys_Rmdir( const char *path );

// removes the file at the specified path.
void			Sys_RemoveFile( const char *path );

// returns true if the file exists and is writable, false if it doesn't exist or is read only
bool			Sys_IsFileWritable( const char *path );

enum sysFolder_t {
	FOLDER_ERROR	= -1,
	FOLDER_NO		= 0,
	FOLDER_YES		= 1
};

// returns FOLDER_YES if the specified path is a folder
sysFolder_t		Sys_IsFolder( const char *path );

// is the file located on HDD?
// (or other device with slow seeking)
bool			Sys_IsFileOnHdd( const char *path );

// returns true if the file was successfully renamed, false if it failed
bool			Sys_RenameFile( const char *filePath, const char *newName );

// This really isn't the right place to have this, but since this is the 'top level' include
// and has a function signature with 'FILE' in it, it kinda needs to be here =/
typedef HANDLE idFileHandle;

// returns the file time stamp, or 0 if it failed
ID_TIME_T		Sys_FileTimeStamp( idFileHandle fp );

// Opens a file directly through the OS, bypassing the idFileSystem.
idFileHandle	Sys_OpenOSFile( const char *fileName, int mode );

// Closes the file handle opened by Sys_OpenOSFile. 
// This is not the same as idFile::Close, which is used for files opened through the idFileSystem.
void			Sys_CloseOSFile( idFileHandle fp );

// returns the length of the file, or -1 if it failed
int				Sys_DirectFileLength( idFileHandle o );

// use fs_debug to verbose Sys_ListFiles
// returns -1 if directory was not found (the list is cleared)
int				Sys_ListFiles( const char * directory, const char * extension, idList<class idStr> & list );

// returns the path to the executable, including the executable name
const char *	Sys_EXEPath();

// returns the current working directory
const char *	Sys_CWD();

#endif /* !__SYS_FILESYSTEM_H__ */