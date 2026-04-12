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

#include "../win32/win_local.h"
#include "../renderer/tr_local.h"
#include "sdl_input.h"

// the rest of the engine will only reference the "inputDevice" variable, while all local aspects stay hidden
idInputDevicesSDL inputDeviceLocal;
idInputDevices *inputDevice = &inputDeviceLocal;	// statically pointed at an idInputDevicesSDL

// the rest of the engine will only reference the "joystick" variable, while all local aspects stay hidden
idJoystickSDL joystickLocal;
idJoystick *joystick = &joystickLocal;	// statically pointed at an idJoystickSDL


/*
================================================================================================

	Input Devices SDL

================================================================================================
*/

idList<kbd_poll_t>   kbd_polls;
idList<mouse_poll_t> mouse_polls;

extern void EnsureUserEventRegistered();

/*
=================
KeyToEngineKey
=================
*/
int KeyToEngineKey( SDL_Keycode key, bool &isChar ) {
	isChar = false;

	if ( key >= SDLK_SPACE && key < SDLK_DELETE ) {
		isChar = true;
	}

	switch ( key ) {
		case SDLK_ESCAPE:       return K_ESCAPE;
		case SDLK_SPACE:        return K_SPACE;

		case SDLK_0:            return K_0;
		case SDLK_1:            return K_1;
		case SDLK_2:            return K_2;
		case SDLK_3:            return K_3;
		case SDLK_4:            return K_4;
		case SDLK_5:            return K_5;
		case SDLK_6:            return K_6;
		case SDLK_7:            return K_7;
		case SDLK_8:            return K_8;
		case SDLK_9:            return K_9;

		case SDLK_UNDERSCORE:   return K_UNDERLINE;
		case SDLK_MINUS:        return K_MINUS;
		case SDLK_COMMA:        return K_COMMA;
		case SDLK_COLON:        return K_COLON;
		case SDLK_SEMICOLON:    return K_SEMICOLON;
		case SDLK_PERIOD:       return K_PERIOD;
		case SDLK_AT:           return K_AT;
		case SDLK_EQUALS:       return K_EQUALS;

		case SDLK_A:            return K_A;
		case SDLK_B:            return K_B;
		case SDLK_C:            return K_C;
		case SDLK_D:            return K_D;
		case SDLK_E:            return K_E;
		case SDLK_F:            return K_F;
		case SDLK_G:            return K_G;
		case SDLK_H:            return K_H;
		case SDLK_I:            return K_I;
		case SDLK_J:            return K_J;
		case SDLK_K:            return K_K;
		case SDLK_L:            return K_L;
		case SDLK_M:            return K_M;
		case SDLK_N:            return K_N;
		case SDLK_O:            return K_O;
		case SDLK_P:            return K_P;
		case SDLK_Q:            return K_Q;
		case SDLK_R:            return K_R;
		case SDLK_S:            return K_S;
		case SDLK_T:            return K_T;
		case SDLK_U:            return K_U;
		case SDLK_V:            return K_V;
		case SDLK_W:            return K_W;
		case SDLK_X:            return K_X;
		case SDLK_Y:            return K_Y;
		case SDLK_Z:            return K_Z;

		case SDLK_RETURN:       return K_ENTER;
		case SDLK_BACKSPACE:    return K_BACKSPACE;
		case SDLK_PAUSE:        return K_PAUSE;
		case SDLK_TAB:          return K_TAB;
		case SDLK_CAPSLOCK:     return K_CAPSLOCK;
		case SDLK_SCROLLLOCK:   return K_SCROLL;
		case SDLK_POWER:        return K_POWER;

		case SDLK_UP:           return K_UPARROW;
		case SDLK_DOWN:         return K_DOWNARROW;
		case SDLK_LEFT:         return K_LEFTARROW;
		case SDLK_RIGHT:        return K_RIGHTARROW;

		case SDLK_LGUI:         return K_LWIN;
		case SDLK_RGUI:         return K_RWIN;

		case SDLK_LALT:         return K_LALT;
		case SDLK_RALT:         return K_RALT;
		case SDLK_RCTRL:        return K_RCTRL;
		case SDLK_LCTRL:        return K_LCTRL;
		case SDLK_RSHIFT:       return K_RSHIFT;
		case SDLK_LSHIFT:       return K_LSHIFT;

		case SDLK_INSERT:       return K_INS;
		case SDLK_DELETE:       return K_DEL;
		case SDLK_PAGEDOWN:     return K_PGDN;
		case SDLK_PAGEUP:       return K_PGUP;
		case SDLK_HOME:         return K_HOME;
		case SDLK_END:          return K_END;

		case SDLK_F1:           return K_F1;
		case SDLK_F2:           return K_F2;
		case SDLK_F3:           return K_F3;
		case SDLK_F4:           return K_F4;
		case SDLK_F5:           return K_F5;
		case SDLK_F6:           return K_F6;
		case SDLK_F7:           return K_F7;
		case SDLK_F8:           return K_F8;
		case SDLK_F9:           return K_F9;
		case SDLK_F10:          return K_F10;
		case SDLK_F11:          return K_F11;
		case SDLK_F12:          return K_F12;
		case SDLK_F13:          return K_F13;
		case SDLK_F14:          return K_F14;
		case SDLK_F15:          return K_F15;

		case SDLK_KP_7:         return K_KP_7;
		case SDLK_KP_8:         return K_KP_8;
		case SDLK_KP_9:         return K_KP_9;
		case SDLK_KP_4:         return K_KP_4;
		case SDLK_KP_5:         return K_KP_5;
		case SDLK_KP_6:         return K_KP_6;
		case SDLK_KP_1:         return K_KP_1;
		case SDLK_KP_2:         return K_KP_2;
		case SDLK_KP_3:         return K_KP_3;
		case SDLK_KP_ENTER:     return K_KP_ENTER;
		case SDLK_KP_0:         return K_KP_0;
		case SDLK_KP_PERIOD:    return K_KP_DOT;
		case SDLK_KP_DIVIDE:    return K_KP_SLASH;
		case SDLK_KP_MINUS:     return K_KP_MINUS;
		case SDLK_KP_PLUS:      return K_KP_PLUS;
		case SDLK_NUMLOCKCLEAR: return K_NUMLOCK;
		case SDLK_KP_MULTIPLY:  return K_KP_STAR;
		case SDLK_KP_EQUALS:    return K_KP_EQUALS;

		case SDLK_PRINTSCREEN:  return K_PRINTSCREEN;
	}

	return 0;
}

/*
=================
idInputDevicesSDL::Init
=================
*/
void idInputDevicesSDL::Init() {
	kbd_polls.SetGranularity( 64 );
	mouse_polls.SetGranularity( 64 );

	EnsureUserEventRegistered();
}

/*
=================
idInputDevicesSDL::Shutdown
=================
*/
void idInputDevicesSDL::Shutdown() {
	kbd_polls.Clear();
	mouse_polls.Clear();
}

/*
===============
idInputDevicesSDL::GrabMouseCursor
===============
*/
void idInputDevicesSDL::GrabMouseCursor( bool grabIt ) {
	int flags = grabIt ? (GRAB_ENABLE | GRAB_SETSTATE) : GRAB_SETSTATE;
	GLimp_GrabInput( flags );
}

/*
================
idInputDevicesSDL::PollKeyboardInputEvents
================
*/
int idInputDevicesSDL::PollKeyboardInputEvents() {
	return kbd_polls.Num();
}

/*
================
idInputDevicesSDL::ReturnKeyboardInputEvent
================
*/
int idInputDevicesSDL::ReturnKeyboardInputEvent( const int n, int &key, bool &state ) {
	if ( n >= kbd_polls.Num() ) {
		return 0;
	}
	key = kbd_polls[n].key;
	state = kbd_polls[n].state;
	return 1;
}

/*
================
idInputDevicesSDL::EndKeyboardInputEvents
================
*/
void idInputDevicesSDL::EndKeyboardInputEvents() {
	kbd_polls.SetNum( 0 );
}

/*
================
Sys_PollMouseInputEvents
================
*/
int idInputDevicesSDL::PollMouseInputEvents( int mouseEvents[MAX_MOUSE_EVENTS][2] ) {
	int numEvents = mouse_polls.Num();
	if ( numEvents > MAX_MOUSE_EVENTS ) {
		numEvents = MAX_MOUSE_EVENTS;
	}

	for ( int i = 0; i < numEvents; i++ ) {
		mouseEvents[i][0] = mouse_polls[i].action;
		mouseEvents[i][1] = mouse_polls[i].value;
	}

	mouse_polls.SetNum( 0 );
	return numEvents;
}

/*
================================================================================================

	Joystick SDL

================================================================================================
*/

void idJoystickSDL::SetRumble( int device, int low, int hi ) {
	// TODO
}

int idJoystickSDL::PollInputEvents( int deviceNum ) {
	// TODO
	return 0;
}

int idJoystickSDL::ReturnInputEvent( const int n, int &action, int &value ) {
	// TODO
	return 0;
}

void idJoystickSDL::EndInputEvents() {
}