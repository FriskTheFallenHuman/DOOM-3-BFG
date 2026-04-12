/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU
General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "precompiled.h"
#pragma hdrstop

#include "../renderer/tr_local.h"
#include "../win32/win_local.h"

idCVar in_nograb( "in_nograb", "0", CVAR_SYSTEM | CVAR_NOCHEAT, "prevents input grabbing" );

static bool         grabbed = false;

/*
========================
GLimp_GetSDLDisplayID

Maps a 1-based parms.fullScreen index to an SDL_DisplayID.
Returns 0 on failure.
========================
*/
static SDL_DisplayID GLimp_GetSDLDisplayID( int oneBasedIndex ) {
	int count = 0;
	SDL_DisplayID *displays = SDL_GetDisplays( &count );
	if ( !displays || oneBasedIndex < 1 || oneBasedIndex > count ) {
		SDL_free( displays );
		return 0;
	}
	SDL_DisplayID id = displays[oneBasedIndex - 1];
	SDL_free( displays );
	return id;
}

/*
========================
GLimp_GetDisplayCount
========================
*/
static int GLimp_GetDisplayCount() {
	int count = 0;
	SDL_DisplayID *displays = SDL_GetDisplays( &count );
	SDL_free( displays );
	return count;
}


/*
===================
GLimp_PreInit

R_GetModeListForDisplay is called before GLimp_Init(), but SDL needs
SDL_Init() first.
===================
*/
void GLimp_PreInit() {
	if ( !SDL_Init( SDL_INIT_VIDEO ) ) {
		common->Error( "Error while initializing SDL: %s", SDL_GetError() );
	}
}

/*
===================
GLimp_TestSwapBuffers
===================
*/
void GLimp_TestSwapBuffers( const idCmdArgs &args ) {
	idLib::Printf( "GLimp_TimeSwapBuffers\n" );
	static const int MAX_FRAMES = 5;
	uint64 timestamps[MAX_FRAMES];
	qglDisable( GL_SCISSOR_TEST );
 
	for ( int swapInterval = 2; swapInterval >= -1; swapInterval-- ) {
		SDL_GL_SetSwapInterval( swapInterval );
		for ( int i = 0; i < MAX_FRAMES; i++ ) {
			if ( swapInterval == -1 ) Sys_Sleep( 16 );
			qglClearColor( (i&1)?0.0f:1.0f, (i&1)?1.0f:0.0f, 0, 1 );
			qglClear( GL_COLOR_BUFFER_BIT );
			SDL_GL_SwapWindow( win32.window );
			qglFinish();
			timestamps[i] = Sys_Microseconds();
		}
		idLib::Printf( "\nswapinterval %i\n", swapInterval );
		for ( int i = 1; i < MAX_FRAMES; i++ ) {
			idLib::Printf( "%i microseconds\n", (int)(timestamps[i] - timestamps[i-1]) );
		}
	}
}

/*
===================
GLimp_Init
===================
*/
bool GLimp_Init( glimpParms_t parms ) {
	cmdSystem->AddCommand( "testSwapBuffers", GLimp_TestSwapBuffers, CMD_FL_SYSTEM, "Times swapbuffer options" );

	common->Printf( "Initializing OpenGL subsystem\n" );

	SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	int colorbits   = 24;
	int depthbits   = 24;
	int stencilbits = 8;

	for ( int i = 0; i < 16; i++ ) {
		if ( (i % 4) == 0 && i ) {
			switch ( i / 4 ) {
				case 2:
					if ( colorbits == 24 )
						colorbits = 16;
					break;
				case 1:
					if ( depthbits == 24 )
						depthbits = 16;
					else if ( depthbits == 16 )
						depthbits = 8;
				case 3:
					if ( stencilbits == 24 )
						stencilbits = 16;
					else if ( stencilbits == 16 )
						stencilbits = 8;
					break;
			}
		}

		int tcolorbits = colorbits;
		int tdepthbits = depthbits;
		int tstencilbits = stencilbits;

		if ( (i % 4) == 3 && tcolorbits == 24 )
			tcolorbits = 16;

		if ( (i % 4) == 2 ) {
			if ( tdepthbits == 24 )
				tdepthbits = 16;
			else if ( tdepthbits == 16 )
				tdepthbits = 8;
		}
		if ( (i % 4) == 1 ) {
			if ( tstencilbits == 24 )
				tstencilbits = 16;
			else if ( tstencilbits == 16 )
				tstencilbits = 8;
			else
				tstencilbits = 0;
		}

		int channelcolorbits = ( tcolorbits == 24 ) ? 8 : 4;

		SDL_GL_SetAttribute( SDL_GL_RED_SIZE,     channelcolorbits );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE,   channelcolorbits );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE,    channelcolorbits );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   tdepthbits );
		SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, tstencilbits );
		SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE,   channelcolorbits );

		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, parms.multiSamples ? 1 : 0 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, parms.multiSamples );

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE );

		if ( r_debugContext.GetBool() ) {
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );
		}

		win32.window = SDL_CreateWindow( GAME_NAME, parms.width, parms.height, flags );
		if ( !win32.window ) {
			common->DPrintf( "Couldn't set GL mode %d/%d/%d: %s",
							 channelcolorbits, tdepthbits, tstencilbits, SDL_GetError() );
			continue;
		}

		// Position & fullscreen AFTER creation
		if ( parms.fullScreen > 0 ) {
			SDL_DisplayID displayId = GLimp_GetSDLDisplayID( parms.fullScreen );
			if ( !displayId ) {
				common->Warning( "Couldn't find display %i, using primary", parms.fullScreen );
				displayId = GLimp_GetSDLDisplayID( 1 );
			}
			if ( displayId ) {
				SDL_Rect bounds = {};
				if ( SDL_GetDisplayBounds( displayId, &bounds ) ) {
					int x = bounds.x + ( bounds.w - parms.width )  / 2;
					int y = bounds.y + ( bounds.h - parms.height ) / 2;
					SDL_SetWindowPosition( win32.window, x, y );
				}
			}

			if ( !SDL_SetWindowFullscreen( win32.window, true ) ) {
				common->Warning( "Couldn't set fullscreen: %s", SDL_GetError() );
			}
		}

		win32.glContext = SDL_GL_CreateContext( win32.window );
		if ( !win32.glContext ) {
			common->DPrintf( "Couldn't create GL context: %s", SDL_GetError() );
			SDL_DestroyWindow( win32.window );
			win32.window = NULL;
			continue;
		}

		if ( !SDL_GL_SetSwapInterval( r_swapInterval.GetInteger() ) ) {
			common->Warning( "SDL_GL_SetSwapInterval not supported" );
		}

		SDL_GetWindowSize( win32.window, &glConfig.nativeScreenWidth, &glConfig.nativeScreenHeight );
		glConfig.isFullscreen = ( SDL_GetWindowFlags( win32.window ) & SDL_WINDOW_FULLSCREEN ) != 0;

		common->Printf( "Using %d color bits, %d depth, %d stencil display\n",
						channelcolorbits, tdepthbits, tstencilbits );

		glConfig.colorBits = tcolorbits;
		glConfig.depthBits = tdepthbits;
		glConfig.stencilBits = tstencilbits;
		glConfig.displayFrequency = 60;
		glConfig.multisamples = parms.multiSamples;
		glConfig.pixelAspect = 1.0f;

		// Detect adaptive vsync (tear control)
		glConfig.swapControlTearAvailable = ( SDL_GL_SetSwapInterval( -1 ) == 0 );
		r_swapInterval.SetModified();

		break;
	}

	if ( !win32.window ) {
		common->Printf( "No usable GL mode found: %s", SDL_GetError() );
		return false;
	}

	SDL_HideCursor();
	SDL_StartTextInput( win32.window );

	return true;
}

static int ScreenParmsHandleDisplayIndex( glimpParms_t parms ) {
	int displayIdx;

	if ( parms.fullScreen > 0 ) {
		displayIdx = parms.fullScreen - 1;
	} else {
		// -2 = use current display
		SDL_DisplayID currentDisp = SDL_GetDisplayForWindow( win32.window );
		int count = 0;
		SDL_DisplayID *displays = SDL_GetDisplays( &count );
		displayIdx = 0;
		if ( displays ) {
			for ( int i = 0; i < count; i++ ) {
				if ( displays[i] == currentDisp ) {
					displayIdx = i;
					break;
				}
			}
			SDL_free( displays );
		}
	}

	int displayCount = GLimp_GetDisplayCount();
	if ( parms.fullScreen > displayCount ) {
		common->Warning( "Can't set fullscreen mode to display number %i, only %i display(s) available!",
						 parms.fullScreen, displayCount );
		return -1;
	}

	if ( parms.fullScreen != glConfig.isFullscreen ) {
		if ( glConfig.isFullscreen ) {
			// Go windowed first before moving to another monitor (SDL oddity)
			SDL_SetWindowFullscreen( win32.window, false );
		}

		// Move to centre of target display
		int count = 0;
		SDL_DisplayID *displays = SDL_GetDisplays( &count );
		if ( displays && displayIdx < count ) {
			SDL_Rect bounds = {};
			if ( SDL_GetDisplayBounds( displays[displayIdx], &bounds ) ) {
				int x = bounds.x + ( bounds.w - glConfig.nativeScreenWidth )  / 2;
				int y = bounds.y + ( bounds.h - glConfig.nativeScreenHeight ) / 2;
				SDL_SetWindowPosition( win32.window, x, y );
			}
		}
		SDL_free( displays );
	}

	return displayIdx;
}

static bool SetScreenParmsFullscreen( glimpParms_t parms ) {
	int displayIdx = ScreenParmsHandleDisplayIndex( parms );
	if ( displayIdx < 0 ) {
		return false;
	}

	int count = 0;
	SDL_DisplayID *displays = SDL_GetDisplays( &count );
	if ( !displays || displayIdx >= count ) {
		SDL_free( displays );
		return false;
	}
	SDL_DisplayID targetDisplay = displays[displayIdx];
	SDL_free( displays );

	const SDL_DisplayMode *currentMode = SDL_GetCurrentDisplayMode( targetDisplay );
	if ( !currentMode ) {
		common->Warning( "Couldn't get current display mode: %s", SDL_GetError() );
		return false;
	}

	// Find the best matching fullscreen mode for the requested resolution.
	int modeCount = 0;
	SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes( targetDisplay, &modeCount );
	const SDL_DisplayMode *bestMode = NULL;

	if ( modes ) {
		for ( int m = 0; m < modeCount; m++ ) {
			if ( modes[m]->w == parms.width && modes[m]->h == parms.height ) {
				if ( !bestMode ) {
					bestMode = modes[m]; // first size match
				}
				if ( parms.displayHz == 0 || (int)modes[m]->refresh_rate == parms.displayHz ) {
					bestMode = modes[m]; // exact match
					break;
				}
			}
		}
		SDL_free( modes );
	}

	// NULL = SDL picks the desktop mode; always succeeds
	if ( !SDL_SetWindowFullscreenMode( win32.window, bestMode ) ) {
		common->Warning( "Couldn't set window fullscreen mode, reason: %s", SDL_GetError() );
		return false;
	}

	if ( !( SDL_GetWindowFlags( win32.window ) & SDL_WINDOW_FULLSCREEN ) ) {
		if ( !SDL_SetWindowFullscreen( win32.window, true ) ) {
			common->Warning( "Couldn't switch to fullscreen mode, reason: %s", SDL_GetError() );
			return false;
		}
	}

	return true;
}

static bool SetScreenParmsWindowed( glimpParms_t parms ) {
	SDL_SetWindowSize( win32.window, parms.width, parms.height );
	SDL_SetWindowPosition( win32.window, parms.x, parms.y );

	if ( SDL_GetWindowFlags( win32.window ) & SDL_WINDOW_FULLSCREEN ) {
		if ( !SDL_SetWindowFullscreen( win32.window, false ) ) {
			common->Warning( "Couldn't switch to windowed mode, reason: %s", SDL_GetError() );
			return false;
		}
	}
	return true;
}

/*
===================
GLimp_SetScreenParms
===================
*/
bool GLimp_SetScreenParms( glimpParms_t parms ) {
	if ( parms.fullScreen > 0 || parms.fullScreen == -2 ) {
		if ( !SetScreenParmsFullscreen( parms ) ) {
			return false;
		}
	} else if ( parms.fullScreen == 0 ) {
		if ( !SetScreenParmsWindowed( parms ) ) {
			return false;
		}
	} else {
		common->Warning( "GLimp_SetScreenParms: unhandled fullScreen value %d", parms.fullScreen );
		return false;
	}

	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, parms.multiSamples ? 1 : 0 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, parms.multiSamples );

	glConfig.isFullscreen = parms.fullScreen;
	glConfig.nativeScreenWidth = parms.width;
	glConfig.nativeScreenHeight = parms.height;
	glConfig.displayFrequency = parms.displayHz;
	glConfig.multisamples = parms.multiSamples;

	return true;
}

/*
===================
GLimp_Shutdown
===================
*/
void GLimp_Shutdown() {
	common->Printf( "Shutting down OpenGL subsystem\n" );

	SDL_StopTextInput( win32.window );

	if ( win32.glContext ) {
		SDL_GL_DestroyContext( win32.glContext );
		win32.glContext = NULL;
	}

	if ( win32.window ) {
		SDL_DestroyWindow( win32.window );
		win32.window = NULL;
	}
}

/*
===================
GLimp_SwapBuffers
===================
*/
void GLimp_SwapBuffers() {
	if ( r_swapInterval.IsModified() ) {
		r_swapInterval.ClearModified();
		int interval = 0;
		if ( r_swapInterval.GetInteger() == 1 )
			interval = glConfig.swapControlTearAvailable ? -1 : 1;
		else if ( r_swapInterval.GetInteger() == 2 )
			interval = 1;
		SDL_GL_SetSwapInterval( interval );
	}
	SDL_GL_SwapWindow( win32.window );
}

/*
=================
GLimp_SetGamma
=================
*/
void GLimp_SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] ) {
}

/*
=================
GLimp_GrabInput
=================
*/
void GLimp_GrabInput( int flags ) {
	bool grab = ( flags & GRAB_ENABLE ) != 0;

	if ( grab && ( flags & GRAB_REENABLE ) ) {
		grab = false;
	}

	if ( flags & GRAB_SETSTATE ) {
		grabbed = grab;
	}

	if ( in_nograb.GetBool() ) {
		grab = false;
	}

	if ( !win32.window ) {
		common->Warning( "GLimp_GrabInput called without window" );
		return;
	}

	SDL_SetWindowRelativeMouseMode( win32.window, ( flags & GRAB_ENABLE ) != 0 );
	SDL_SetWindowMouseGrab( win32.window, grab );
}

/*
====================
DumpAllDisplayDevices
====================
*/
void DumpAllDisplayDevices() {
	int count = 0;
	SDL_DisplayID *displays = SDL_GetDisplays( &count );
	if ( !displays ) return;

	common->Printf( "\n" );
	for ( int i = 0; i < count; i++ ) {
		SDL_DisplayID id   = displays[i];
		const char   *name = SDL_GetDisplayName( id );
		SDL_Rect      bounds = {};
		SDL_GetDisplayBounds( id, &bounds );
		const SDL_DisplayMode *cur = SDL_GetCurrentDisplayMode( id );

		common->Printf( "display device: %i\n", i );
		common->Printf( "  Name  : %s\n", name ? name : "unknown" );
		common->Printf( "  Bounds: %d,%d %dx%d\n", bounds.x, bounds.y, bounds.w, bounds.h );
		if ( cur ) {
			common->Printf( "  Mode  : %dx%d @ %.1f Hz\n", cur->w, cur->h, cur->refresh_rate );
		}
	}
	common->Printf( "\n" );
	SDL_free( displays );
}


class idSort_VidMode : public idSort_Quick<vidMode_t, idSort_VidMode> {
public:
	int Compare( const vidMode_t &a, const vidMode_t &b ) const {
		int hd = a.height - b.height;
		int wd = a.width - b.width;
		int fd = a.displayHz - b.displayHz;
		return ( hd != 0 ) ? hd : ( wd != 0 ) ? wd : fd;
	}
};

/*
====================
R_GetModeListForDisplay
====================
*/
bool R_GetModeListForDisplay( const int requestedDisplayNum, idList<vidMode_t> &modeList ) {
	assert( requestedDisplayNum >= 0 );
	modeList.Clear();

	int count = 0;
	SDL_DisplayID *displays = SDL_GetDisplays( &count );

	if ( !displays || requestedDisplayNum >= count ) {
		SDL_free( displays );
		return false;
	}

	SDL_DisplayID targetDisplay = displays[requestedDisplayNum];
	SDL_free( displays );

	int modeCount = 0;
	SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes( targetDisplay, &modeCount );
	if ( !modes ) {
		return false;
	}

	for ( int m = 0; m < modeCount; m++ ) {
		const SDL_DisplayMode *mode = modes[m];

		if ( mode->h < 720 )
			continue;

		int hz = (int)mode->refresh_rate;
		if ( hz != 60 && hz != 120 )
			continue;

		vidMode_t vm = {};
		vm.width = mode->w;
		vm.height = mode->h;
		vm.displayHz = hz;
		modeList.AddUnique( vm );
	}
	SDL_free( modes );

	if ( modeList.Num() == 0 ) {
		return false;
	}

	modeList.SortWithTemplate( idSort_VidMode() );
	return true;
}

/*
====================
GLimp_ActivateContext
====================
*/
void GLimp_ActivateContext() {
	if ( !SDL_GL_MakeCurrent( win32.window, win32.glContext ) ) {
		common->Printf( "GLimp_ActivateContext: %s\n", SDL_GetError() );
	}
}

/*
====================
GLimp_DeactivateContext
====================
*/
void GLimp_DeactivateContext() {
	qglFinish();
	SDL_GL_MakeCurrent( win32.window, NULL );
}

/*
====================
GLimp_ExtensionPointer
====================
*/
GLExtension_t GLimp_ExtensionPointer( const char *name ) {
	SDL_FunctionPointer proc = SDL_GL_GetProcAddress( name );
	if ( !proc ) {
		common->Printf( "Couldn't find proc address for: %s\n", name );
	}
	return (GLExtension_t)proc;
}