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
#ifndef __XA_SOUNDHARDWARE_H__
#define __XA_SOUNDHARDWARE_H__

class idSoundSample_XAudio2;
class idSoundVoice_XAudio2;

/*
================================================
idSoundEngineCallback
================================================
*/
class idSoundEngineCallback : public IXAudio2EngineCallback {
public:
	idSoundHardware_XAudio2 * hardware;

private:
		// Called by XAudio2 just before an audio processing pass begins.
	STDMETHOD_( void, OnProcessingPassStart ) ( THIS ) {}

	// Called just after an audio processing pass ends.
	STDMETHOD_( void, OnProcessingPassEnd ) ( THIS ) {}

	// Called in the event of a critical system error which requires XAudio2
	// to be closed down and restarted.  The error code is given in Error.
	STDMETHOD_( void, OnCriticalError ) ( THIS_ HRESULT Error );
};

/*
================================================
idSoundHardware_XAudio2
================================================
*/

class idSoundHardware_XAudio2 : public idSoundHardware {
public:
					idSoundHardware_XAudio2();

	virtual void	Init();
	virtual void	Shutdown();
	virtual void	ShutdownReverbSystem();

	virtual void	Update();
	virtual void	UpdateEAXEffect( idSoundEffect * effect );

	virtual idSoundVoice *	AllocateVoice( const idSoundSample * leadinSample, const idSoundSample * loopingSample, const int channel );
	virtual void	FreeVoice( idSoundVoice * voice );

	virtual int		GetNumZombieVoices() const { return zombieVoices.Num(); }
	virtual int		GetNumFreeVoices() const { return freeVoices.Num(); }

	virtual bool	IsReverbSupported() { return hasReverb; }
	virtual bool	ParseEAXEffects( idLexer & src, idToken name, idToken token, idSoundEffect * effect );

	IXAudio2 *					GetIXAudio2() const { return pXAudio2; };
	IXAudio2MasteringVoice *	GetMasterVoice() const { return pMasterVoice; };
	IXAudio2SubmixVoice *		GetSubMixVoice() const { return pSubmixVoice; };

protected:
	friend class idSoundSample_XAudio2;
	friend class idSoundVoice_XAudio2;

private:
	IXAudio2 * pXAudio2;
	IXAudio2MasteringVoice * pMasterVoice;
	IXAudio2SubmixVoice * pSubmixVoice;

	idSoundEngineCallback	soundEngineCallback;

	bool	hasReverb;
	XAUDIO2FX_REVERB_PARAMETERS EAX;

	// Can't stop and start a voice on the same frame, so we have to double this to handle the worst case scenario of stopping all voices and starting a full new set
	idStaticList<idSoundVoice_XAudio2, MAX_HARDWARE_VOICES * 2 > voices;
	idStaticList<idSoundVoice_XAudio2 *, MAX_HARDWARE_VOICES * 2 > zombieVoices;
	idStaticList<idSoundVoice_XAudio2 *, MAX_HARDWARE_VOICES * 2 > freeVoices;
};

#endif
