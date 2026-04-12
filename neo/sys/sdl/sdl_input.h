/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

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

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __SDL_INPUT_H__
#define __SDL_INPUT_H__

#include "precompiled.h"
#include "../sys_public.h"

/*
================================================================================================

	Input Devices SDL

================================================================================================
*/

struct kbd_poll_t {
	int  key;
	bool state;

	kbd_poll_t() {}
	kbd_poll_t( int k, bool s ) : key( k ), state( s ) {}
};

struct mouse_poll_t {
	int action;
	int value;

	mouse_poll_t() {}
	mouse_poll_t( int a, int v ) : action( a ), value( v ) {}
};

struct consoleKeyMapping {
	const char *langName;
	unsigned char key;
	unsigned char keyShifted;
};

extern idList<kbd_poll_t>   kbd_polls;
extern idList<mouse_poll_t> mouse_polls;

extern int KeyToEngineKey( SDL_Keycode key, bool &isChar );

class idInputDevicesSDL : public idInputDevices  {
public:
					idInputDevicesSDL() {};

	// input is tied to windows, so it needs to be started up and shut down whenever
	// the main window is recreated
	virtual void	Init();
	virtual void	Shutdown();
	virtual void	Frame() {}

	// keyboard input polling
	virtual int		PollKeyboardInputEvents();
	virtual int		ReturnKeyboardInputEvent( const int n, int &ch, bool &state );
	virtual void	EndKeyboardInputEvents();

	// mouse input polling
	virtual int		PollMouseInputEvents( int mouseEvents[MAX_MOUSE_EVENTS][2] );

	// when the console is down, or the game is about to perform a lengthy
	// operation like map loading, the system can release the mouse cursor
	// when in windowed mode
	virtual void	GrabMouseCursor( bool grabIt );

	virtual void	DeactivateMouseIfWindowed() {};
	virtual void	DeactivateMouse() {};
	virtual void	ActivateMouse() {};
};

/*
================================================================================================

	Joystick SDL

================================================================================================
*/

class idJoystickSDL : public idJoystick {
public:
					idJoystickSDL() {}

	virtual bool	Init() { return true; }
	virtual void	SetRumble( int deviceNum, int rumbleLow, int rumbleHigh );
	virtual int		PollInputEvents( int inputDeviceNum );
	virtual int		ReturnInputEvent( const int n, int &action, int &value );
	virtual void	EndInputEvents();
};

#endif
