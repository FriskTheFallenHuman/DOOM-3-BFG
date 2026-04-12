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

#include "renderer/tr_local.h"
#include "../win32/win_local.h"
#include "sdl_input.h"
#include "../sys_public.h"

// those are needed for moving/resizing windows
extern idCVar r_windowX;
extern idCVar r_windowY;
extern idCVar r_windowWidth;
extern idCVar r_windowHeight;

static uint32 s_userEventType = 0;

/*
=================
EnsureUserEventRegistered
=================
*/
void EnsureUserEventRegistered() {
	if ( s_userEventType == 0 ) {
		s_userEventType = SDL_RegisterEvents( 1 );
		if ( s_userEventType == (uint32)-1 ) {
			common->Error( "SDL_RegisterEvents failed: %s", SDL_GetError() );
		}
	}
}

/*
=================
PushConsoleEvent
=================
*/
static void PushConsoleEvent( const char *s ) {
	EnsureUserEventRegistered();

	char  *b;
	size_t len = strlen( s ) + 1;
	b = (char *)Mem_Alloc( len, TAG_EVENTS );
	strcpy( b, s );

	SDL_Event event = {};
	event.type = s_userEventType;
	event.user.code = SE_CONSOLE;
	event.user.data1 = (void *)(uintptr_t)len;
	event.user.data2 = b;

	SDL_PushEvent( &event );
}

/*
================
Sys_GetEvent
================
*/
sysEvent_t Sys_GetEvent() {
	SDL_Event  ev;
	sysEvent_t res = {};
	int        key;

	static const sysEvent_t res_none = { SE_NONE, 0, 0, 0, NULL };

	// Drain the multi-byte SDL_EVENT_TEXT_INPUT string one char at a time
	static char  *s     = NULL;
	static size_t s_pos = 0;

	if ( s ) {
		res.evType  = SE_CHAR;
		res.evValue = (unsigned char)s[s_pos];
		s_pos++;
		if ( !s[s_pos] ) {
			free( s );
			s     = NULL;
			s_pos = 0;
		}
		return res;
	}

	// Fake a "mousewheel released" event so menu scrolling stops after one step
	static int mwheelRel = 0;
	if ( mwheelRel ) {
		res.evType   = SE_KEY;
		res.evValue  = mwheelRel;
		res.evValue2 = 0;
		mwheelRel    = 0;
		return res;
	}

	// Drain a single pending SE_CHAR (e.g. backspace)
	static byte c = 0;
	if ( c ) {
		res.evType  = SE_CHAR;
		res.evValue = c;
		c = 0;
		return res;
	}

	if ( !SDL_PollEvent( &ev ) ) {
		return res_none;
	}

	switch ( ev.type ) {

	case SDL_EVENT_WINDOW_FOCUS_GAINED: {
		// Clear modifier state so stale ALT from alt-tab doesn't toggle fullscreen
		SDL_Keymod currentmod = SDL_GetModState();
		SDL_Keymod newmod     = SDL_KMOD_NONE;
		if ( currentmod & SDL_KMOD_CAPS )
			newmod = (SDL_Keymod)( newmod | SDL_KMOD_CAPS );
		SDL_SetModState( newmod );

		// start playing the game sound world
		soundSystem->SetMute( false );

		// resume the game if we were paused
		common->SetPaused( false );
		
		// fordward to the engine that we are requesting focus
		common->SetFocus( true );
		return res_none;
	}

	case SDL_EVENT_WINDOW_FOCUS_LOST:
		// stop playing the game sound world
		soundSystem->SetMute( true );

		// pause the game if we were not paused
		common->SetPaused( true );
		
		// fordward to the engine that we are not requesting focus
		common->SetFocus( false );
		return res_none;

	case SDL_EVENT_WINDOW_RESIZED: {
		int w = ev.window.data1;
		int h = ev.window.data2;
		r_windowWidth.SetInteger( w );
		r_windowHeight.SetInteger( h );
		glConfig.nativeScreenWidth  = w;
		glConfig.nativeScreenHeight = h;
		return res_none;
	}

	case SDL_EVENT_WINDOW_MOVED: {
		int x = ev.window.data1;
		int y = ev.window.data2;
		r_windowX.SetInteger( x );
		r_windowY.SetInteger( y );
		return res_none;
	}

	case SDL_EVENT_KEY_DOWN:
		// Alt+Enter: toggle fullscreen
		if ( ev.key.key == SDLK_RETURN && ( ev.key.mod & SDL_KMOD_ALT ) ) {
			int fullscreen = renderSystem->IsFullScreen() ? 0 : -2;
			cvarSystem->SetCVarInteger( "r_fullscreen", fullscreen );
			PushConsoleEvent( "vid_restart" );
			return res_none;
		}

		// Ctrl+G: toggle mouse grab
		if ( ev.key.key == SDLK_G && ( ev.key.mod & SDL_KMOD_CTRL ) ) {
			bool grab = cvarSystem->GetCVarBool( "in_nograb" );
			cvarSystem->SetCVarBool( "in_nograb", !grab );
			return res_none;
		}
		// fall through

	case SDL_EVENT_KEY_UP: {
		bool isChar;
		if ( ev.key.scancode == SDL_SCANCODE_GRAVE ) {
			key = K_GRAVE;
			c   = K_BACKSPACE; // bad hack to clear console input line
		} else {
			key = KeyToEngineKey( ev.key.key, isChar );

			if ( key == 0 ) {
				if ( ev.type == SDL_EVENT_KEY_DOWN ) {
					common->Warning( "unmapped SDL key %d scancode %d",
									 ev.key.key, ev.key.scancode );
				}
				return res_none;
			}
		}

		res.evType = SE_KEY;
		res.evValue = key;
		res.evValue2 = ev.key.down ? 1 : 0;

		kbd_polls.Append( kbd_poll_t( key, ev.key.down ) );

		if ( key == K_BACKSPACE && ev.key.down ) {
			c = key;
		}

		return res;
	}

	case SDL_EVENT_TEXT_INPUT:
		if ( ev.text.text && *ev.text.text ) {
			if ( !ev.text.text[1] ) {
				c = (unsigned char)*ev.text.text;
			} else {
				s = strdup( ev.text.text );
			}
		}
		return res_none;

	case SDL_EVENT_MOUSE_MOTION:
		if ( game && game->Shell_IsActive() ) {
			res.evType = SE_MOUSE_ABSOLUTE;
			res.evValue = (int)ev.motion.x;
			res.evValue2 = (int)ev.motion.y;
		} else {
			res.evType = SE_MOUSE;
			res.evValue = (int)ev.motion.xrel;
			res.evValue2 = (int)ev.motion.yrel;
		}

		mouse_polls.Append( mouse_poll_t( M_DELTAX, (int)ev.motion.xrel ) );
		mouse_polls.Append( mouse_poll_t( M_DELTAY, (int)ev.motion.yrel ) );
		return res;

	case SDL_EVENT_MOUSE_WHEEL: {
		int ticks = (int)ev.wheel.y;
		if ( ticks == 0 ) {
			return res_none;
		}

		res.evType = SE_KEY;
		res.evValue = ( ticks > 0 ) ? K_MWHEELUP : K_MWHEELDOWN;
		mouse_polls.Append( mouse_poll_t( M_DELTAZ, ( ticks > 0 ) ? 1 : -1 ) );

		mwheelRel = res.evValue;
		res.evValue2 = 1;
		return res;
	}

	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
		res.evType = SE_KEY;

		switch ( ev.button.button ) {
		case SDL_BUTTON_LEFT:
			res.evValue = K_MOUSE1;
			mouse_polls.Append( mouse_poll_t( M_ACTION1, ev.button.down ? 1 : 0 ) );
			break;
		case SDL_BUTTON_MIDDLE:
			res.evValue = K_MOUSE3;
			mouse_polls.Append( mouse_poll_t( M_ACTION3, ev.button.down ? 1 : 0 ) );
			break;
		case SDL_BUTTON_RIGHT:
			res.evValue = K_MOUSE2;
			mouse_polls.Append( mouse_poll_t( M_ACTION2, ev.button.down ? 1 : 0 ) );
			break;
		case SDL_BUTTON_X1:
			res.evValue = K_MOUSE4;
			mouse_polls.Append( mouse_poll_t( M_ACTION4, ev.button.down ? 1 : 0 ) );
			break;
		case SDL_BUTTON_X2:
			res.evValue = K_MOUSE5;
			mouse_polls.Append( mouse_poll_t( M_ACTION5, ev.button.down ? 1 : 0 ) );
			break;
		default:
			return res_none;
		}

		res.evValue2 = ev.button.down ? 1 : 0;
		return res;

	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
	case SDL_EVENT_QUIT:
		soundSystem->SetMute( true );
		PushConsoleEvent( "quit" );
		return res_none;

    case SDL_EVENT_MOUSE_ADDED:
    case SDL_EVENT_MOUSE_REMOVED:
    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
    case SDL_EVENT_JOYSTICK_ADDED:
    case SDL_EVENT_JOYSTICK_REMOVED:
    case SDL_EVENT_GAMEPAD_ADDED:
    case SDL_EVENT_GAMEPAD_REMOVED:
        return res_none;

    case SDL_EVENT_WINDOW_SHOWN:
    case SDL_EVENT_WINDOW_HIDDEN:
    case SDL_EVENT_WINDOW_EXPOSED:
    case SDL_EVENT_WINDOW_MINIMIZED:
    case SDL_EVENT_WINDOW_MAXIMIZED:
    case SDL_EVENT_WINDOW_RESTORED:
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        return res_none;

    // System events we don't care about
    case SDL_EVENT_CLIPBOARD_UPDATE:
    case SDL_EVENT_KEYMAP_CHANGED:
        return res_none;

	default:
		if ( ev.type == s_userEventType ) {
			switch ( ev.user.code ) {
			case SE_CONSOLE:
				res.evType = SE_CONSOLE;
				res.evPtrLength = (intptr_t)ev.user.data1;
				res.evPtr = ev.user.data2;
				return res;
			default:
				common->Warning( "unknown user event %u", ev.user.code );
				return res_none;
			}
		}

		common->Warning( "unknown event %u", ev.type );
		return res_none;
	}
}


/*
================
Sys_ClearEvents
================
*/
void Sys_ClearEvents() {
	SDL_Event ev;
	while ( SDL_PollEvent( &ev ) )
		;

	kbd_polls.SetNum( 0 );
	mouse_polls.SetNum( 0 );
}

/*
================
Sys_GenerateEvents
================
*/
void Sys_GenerateEvents() {
	char *s = Sys_ConsoleInput();
	if ( s ) {
		PushConsoleEvent( s );
	}

	// grab or release the mouse cursor if necessary
	if ( inputDevice ) {
		inputDevice->Frame();
	}

	// pump the message loop
	SDL_PumpEvents();
}