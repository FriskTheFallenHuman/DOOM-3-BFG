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

#ifndef __KEYINPUT_H__
#define __KEYINPUT_H__

struct keyBindings_t {
	idStr keyboard;
	idStr mouse;
	idStr gamepad;
};

class idSerializer;

// Converts from a USB HID code to a K_ code
int Key_CovertHIDCode( int hid );

class idKeyInput {
public:
	virtual						~idKeyInput() {}

	virtual void				Init() = 0;
	virtual void				Shutdown() = 0;

	virtual void				PreliminaryKeyEvent( int keyNum, bool down ) = 0;
	virtual bool				IsDown( int keyNum ) = 0;
	virtual int					GetUsercmdAction( int keyNum ) = 0;
	virtual bool				GetOverstrikeMode() = 0;
	virtual void				SetOverstrikeMode( bool state ) = 0;
	virtual void				ClearStates() = 0;

	virtual keyNum_t			StringToKeyNum( const char * str ) = 0;		// This is used by the "bind" command
	virtual const char *		KeyNumToString( keyNum_t keyNum ) = 0;		// This is the inverse of StringToKeyNum, used for config files
	virtual const char *		LocalizedKeyName( keyNum_t keyNum ) = 0;	// This returns text suitable to print on screen

	virtual void				SetBinding( int keyNum, const char *binding ) = 0;
	virtual const char *		GetBinding( int keyNum ) = 0;
	virtual bool				UnbindBinding( const char *bind ) = 0;
	virtual int					NumBinds( const char *binding ) = 0;
	virtual bool				ExecKeyBinding( int keyNum ) = 0;
	virtual const char *		KeysFromBinding( const char *bind ) = 0;
	virtual const char *		BindingFromKey( const char *key ) = 0;
	virtual bool				KeyIsBoundTo( int keyNum, const char *binding ) = 0;
	virtual void				WriteBindings( idFile *f ) = 0;
	virtual keyBindings_t		KeyBindingsFromBinding( const char * bind, bool firstOnly = false, bool localized = false ) = 0;
};

extern idKeyInput *keyBindMgr;

class idKey {
public:
					idKey() { down = false; repeats = 0; usercmdAction = 0; }
	bool			down;
	int				repeats;		// if > 1, it is autorepeating
	idStr			binding;
	int				usercmdAction;	// for testing by the asyncronous usercmd generation
};

extern idKey *keys;

#endif /* !__KEYINPUT_H__ */
