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

#ifndef __COMMON_DIALOG_H__
#define __COMMON_DIALOG_H__

static const int	MAX_DIALOGS			= 4;		// maximum dialogs that can be open at one time
static const int	PC_KEYBOARD_WAIT	= 20000;

/*
================================================
Dialog box message types
================================================
*/
enum gameDialogMessages_t {
	GDM_INVALID,
	GDM_SWAP_DISKS_TO1,
	GDM_SWAP_DISKS_TO2,
	GDM_SWAP_DISKS_TO3,
	GDM_NO_GAMER_PROFILE,
	GDM_PLAY_ONLINE_NO_PROFILE,
	GDM_LEADERBOARD_ONLINE_NO_PROFILE,
	GDM_NO_STORAGE_SELECTED,
	GDM_ONLINE_INCORRECT_PERMISSIONS,
	GDM_SP_QUIT_SAVE,
	GDM_SP_RESTART_SAVE,
	GDM_SP_SIGNIN_CHANGE,
	GDM_SERVER_NOT_AVAILABLE,
	GDM_CONNECTION_LOST_HOST,
	GDM_CONNECTION_LOST,
	GDM_OPPONENT_CONNECTION_LOST,
	GDM_HOST_CONNECTION_LOST,
	GDM_HOST_CONNECTION_LOST_STATS,
	GDM_FAILED_TO_LOAD_RANKINGS,
	GDM_HOST_QUIT,
	GDM_BECAME_HOST_PARTY,			// Became host of party
	GDM_NEW_HOST_PARTY,				// Someone else became host of party
	GDM_LOBBY_BECAME_HOST_GAME,		// In lobby, you became game host
	GDM_LOBBY_NEW_HOST_GAME,		// In lobby, new game host was chosen (not you)
	GDM_NEW_HOST_GAME,				// Host left/DC'd, someone else is new host, unranked game
	GDM_NEW_HOST_GAME_STATS_DROPPED,// Host left/DC'd, someone else is new host, ranked game so stats were dropped
	GDM_BECAME_HOST_GAME,				// Host left/DC'd, you became host, unranked game
	GDM_BECAME_HOST_GAME_STATS_DROPPED, // Host left/DC'd, you became host, ranked game so stats were dropped
	GDM_LOBBY_DISBANDED,
	GDM_LEAVE_WITH_PARTY,
	GDM_LEAVE_LOBBY_RET_MAIN,
	GDM_LEAVE_LOBBY_RET_NEW_PARTY,
	GDM_MIGRATING,
	GDM_OPPONENT_LEFT,
	GDM_NO_MATCHES_FOUND,
	GDM_INVALID_INVITE,
	GDM_KICKED,
	GDM_BANNED,
	GDM_SAVING,
	GDM_OVERWRITE_SAVE,
	GDM_LOAD_REQUEST,
	GDM_AUTOSAVE_DISABLED_STORAGE_REMOVED,
	GDM_STORAGE_INVALID,
	GDM_STORAGE_REMOVED,
	GDM_CONNECTING,
	GDM_REFRESHING,
	GDM_DELETE_SAVE,
	GDM_DELETING,
	GDM_BINDING_ALREDY_SET,
	GDM_CANNOT_BIND,
	GDM_OVERLAY_DISABLED,
	GDM_DIRECT_MAP_CHANGE,
	GDM_DELETE_AUTOSAVE,
	GDM_GAME_ERROR,
	GDM_MULTI_RETRY,
	GDM_MULTI_SELF_DESTRUCT,
	GDM_MULTI_VDM_QUIT,
	GDM_MULTI_COOP_QUIT,
	GDM_LOADING_PROFILE,
	GDM_STORAGE_REQUIRED,
	GDM_INSUFFICENT_STORAGE_SPACE,
	GDM_PARTNER_LEFT,
	GDM_RESTORE_CORRUPT_SAVEGAME,
	GDM_UNRECOVERABLE_SAVEGAME,
	GDM_PROFILE_SAVE_ERROR,
	GDM_LOBBY_FULL,
	GDM_QUIT_GAME,
	GDM_CONNECTION_PROBLEMS,
	GDM_VOICE_RESTRICTED,
	GDM_LOAD_DAMAGED_FILE,
	GDM_MUST_SIGNIN,
	GDM_CONNECTION_LOST_NO_LEADERBOARD,
	GDM_SP_SIGNIN_CHANGE_POST,
	GDM_MIGRATING_WAITING,
	GDM_MIGRATING_RELAUNCHING,
	GDM_MIGRATING_FAILED_CONNECTION,
	GDM_MIGRATING_FAILED_CONNECTION_STATS,
	GDM_MIGRATING_FAILED_DISBANDED,
	GDM_MIGRATING_FAILED_DISBANDED_STATS,
	GDM_MIGRATING_FAILED_PARTNER_LEFT,
	GDM_HOST_RETURNED_TO_LOBBY,
	GDM_HOST_RETURNED_TO_LOBBY_STATS_DROPPED,
	GDM_FAILED_JOIN_LOCAL_SESSION,
	GDM_DELETE_CORRUPT_SAVEGAME,
	GDM_LEAVE_INCOMPLETE_INSTANCE,
	GDM_UNBIND_CONFIRM,
	GDM_BINDINGS_RESTORE,
	GDM_NEW_HOST,
	GDM_CONFIRM_VIDEO_CHANGES,
	GDM_UNABLE_TO_USE_SELECTED_STORAGE_DEVICE,
	GDM_ERROR_LOADING_SAVEGAME,
	GDM_ERROR_SAVING_SAVEGAME,
	GDM_DISCARD_CHANGES,
	GDM_LEAVE_LOBBY,
	GDM_LEAVE_LOBBY_AND_TEAM,
	GDM_CONTROLLER_DISCONNECTED_0,
	GDM_CONTROLLER_DISCONNECTED_1,
	GDM_CONTROLLER_DISCONNECTED_2,
	GDM_CONTROLLER_DISCONNECTED_3,
	GDM_CONTROLLER_DISCONNECTED_4,
	GDM_CONTROLLER_DISCONNECTED_5,
	GDM_CONTROLLER_DISCONNECTED_6,
	GDM_DLC_ERROR_REMOVED,
	GDM_DLC_ERROR_CORRUPT,
	GDM_DLC_ERROR_MISSING,
	GDM_DLC_ERROR_MISSING_GENERIC,
	GDM_DISC_SWAP,
	GDM_NEEDS_INSTALL,
	GDM_NO_SAVEGAMES_AVAILABLE,
	GDM_ERROR_JOIN_TWO_PROFILES_ONE_BOX,
	GDM_WARNING_PLAYING_COOP_SOLO,
	GDM_MULTI_COOP_QUIT_LOSE_LEADERBOARDS,
	GDM_CORRUPT_CONTINUE,
	GDM_MULTI_VDM_QUIT_LOSE_LEADERBOARDS,
	GDM_WARNING_PLAYING_VDM_SOLO,
	GDM_NO_GUEST_SUPPORT,
	GDM_DISC_SWAP_CONFIRMATION,
	GDM_ERROR_LOADING_PROFILE,
	GDM_CANNOT_INVITE_LOBBY_FULL,
	GDM_WARNING_FOR_NEW_DEVICE_ABOUT_TO_LOSE_PROGRESS,
	GDM_DISCONNECTED,
	GDM_INCOMPATIBLE_NEWER_SAVE,
	GDM_ACHIEVEMENTS_DISABLED_DUE_TO_CHEATING,
	GDM_INCOMPATIBLE_POINTER_SIZE,
	GDM_TEXTUREDETAIL_RESTARTREQUIRED,
	GDM_TEXTUREDETAIL_INSUFFICIENT_CPU,
	GDM_CHECKPOINT_SAVE,
	GDM_CALCULATING_BENCHMARK,
	GDM_DISPLAY_BENCHMARK,
	GDM_DISPLAY_CHANGE_FAILED,
	GDM_GPU_TRANSCODE_FAILED,
	GDM_OUT_OF_MEMORY,
	GDM_CORRUPT_PROFILE,
	GDM_PROFILE_TOO_OUT_OF_DATE_DEVELOPMENT_ONLY,
	GDM_SP_LOAD_SAVE,
	GDM_INSTALLING_TROPHIES,
	GDM_XBOX_DEPLOYMENT_TYPE_FAIL,
	GDM_SAVEGAME_WRONG_LANGUAGE,
	GDM_GAME_RESTART_REQUIRED,
	GDM_MAX
};

static const char * dialogStateToString[ GDM_MAX + 1 ] = {
	ASSERT_ENUM_STRING( GDM_INVALID, 0 ),
	ASSERT_ENUM_STRING( GDM_SWAP_DISKS_TO1, 1 ),
	ASSERT_ENUM_STRING( GDM_SWAP_DISKS_TO2, 2 ),
	ASSERT_ENUM_STRING( GDM_SWAP_DISKS_TO3, 3 ),
	ASSERT_ENUM_STRING( GDM_NO_GAMER_PROFILE, 4 ),
	ASSERT_ENUM_STRING( GDM_PLAY_ONLINE_NO_PROFILE, 5 ),
	ASSERT_ENUM_STRING( GDM_LEADERBOARD_ONLINE_NO_PROFILE, 6 ),
	ASSERT_ENUM_STRING( GDM_NO_STORAGE_SELECTED, 7 ),
	ASSERT_ENUM_STRING( GDM_ONLINE_INCORRECT_PERMISSIONS, 8 ),
	ASSERT_ENUM_STRING( GDM_SP_QUIT_SAVE, 9 ),
	ASSERT_ENUM_STRING( GDM_SP_RESTART_SAVE, 10 ),
	ASSERT_ENUM_STRING( GDM_SP_SIGNIN_CHANGE, 11 ),
	ASSERT_ENUM_STRING( GDM_SERVER_NOT_AVAILABLE, 12 ),
	ASSERT_ENUM_STRING( GDM_CONNECTION_LOST_HOST, 13 ),
	ASSERT_ENUM_STRING( GDM_CONNECTION_LOST, 14 ),
	ASSERT_ENUM_STRING( GDM_OPPONENT_CONNECTION_LOST, 15 ),
	ASSERT_ENUM_STRING( GDM_HOST_CONNECTION_LOST, 16 ),
	ASSERT_ENUM_STRING( GDM_HOST_CONNECTION_LOST_STATS, 17 ),
	ASSERT_ENUM_STRING( GDM_FAILED_TO_LOAD_RANKINGS, 18 ),
	ASSERT_ENUM_STRING( GDM_HOST_QUIT, 19 ),
	ASSERT_ENUM_STRING( GDM_BECAME_HOST_PARTY, 20 ),
	ASSERT_ENUM_STRING( GDM_NEW_HOST_PARTY, 21 ),
	ASSERT_ENUM_STRING( GDM_LOBBY_BECAME_HOST_GAME, 22 ),
	ASSERT_ENUM_STRING( GDM_LOBBY_NEW_HOST_GAME, 23 ),
	ASSERT_ENUM_STRING( GDM_NEW_HOST_GAME, 24 ),
	ASSERT_ENUM_STRING( GDM_NEW_HOST_GAME_STATS_DROPPED, 25 ),
	ASSERT_ENUM_STRING( GDM_BECAME_HOST_GAME, 26 ),
	ASSERT_ENUM_STRING( GDM_BECAME_HOST_GAME_STATS_DROPPED, 27 ),
	ASSERT_ENUM_STRING( GDM_LOBBY_DISBANDED, 28 ),
	ASSERT_ENUM_STRING( GDM_LEAVE_WITH_PARTY, 29 ),
	ASSERT_ENUM_STRING( GDM_LEAVE_LOBBY_RET_MAIN, 30 ),
	ASSERT_ENUM_STRING( GDM_LEAVE_LOBBY_RET_NEW_PARTY, 31 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING, 32 ),
	ASSERT_ENUM_STRING( GDM_OPPONENT_LEFT, 33 ),
	ASSERT_ENUM_STRING( GDM_NO_MATCHES_FOUND, 34 ),
	ASSERT_ENUM_STRING( GDM_INVALID_INVITE, 35 ),
	ASSERT_ENUM_STRING( GDM_KICKED, 36 ),
	ASSERT_ENUM_STRING( GDM_BANNED, 37 ),
	ASSERT_ENUM_STRING( GDM_SAVING, 38 ),
	ASSERT_ENUM_STRING( GDM_OVERWRITE_SAVE, 39 ),
	ASSERT_ENUM_STRING( GDM_LOAD_REQUEST, 40 ),
	ASSERT_ENUM_STRING( GDM_AUTOSAVE_DISABLED_STORAGE_REMOVED, 41 ),
	ASSERT_ENUM_STRING( GDM_STORAGE_INVALID, 42 ),
	ASSERT_ENUM_STRING( GDM_STORAGE_REMOVED, 43 ),
	ASSERT_ENUM_STRING( GDM_CONNECTING, 44 ),
	ASSERT_ENUM_STRING( GDM_REFRESHING, 45 ),
	ASSERT_ENUM_STRING( GDM_DELETE_SAVE, 46 ),
	ASSERT_ENUM_STRING( GDM_DELETING, 47 ),
	ASSERT_ENUM_STRING( GDM_BINDING_ALREDY_SET, 48 ),
	ASSERT_ENUM_STRING( GDM_CANNOT_BIND, 49 ),
	ASSERT_ENUM_STRING( GDM_OVERLAY_DISABLED, 50 ),
	ASSERT_ENUM_STRING( GDM_DIRECT_MAP_CHANGE, 51 ),
	ASSERT_ENUM_STRING( GDM_DELETE_AUTOSAVE, 52 ),
	ASSERT_ENUM_STRING( GDM_GAME_ERROR, 53 ),
	ASSERT_ENUM_STRING( GDM_MULTI_RETRY, 54 ),
	ASSERT_ENUM_STRING( GDM_MULTI_SELF_DESTRUCT, 55 ),
	ASSERT_ENUM_STRING( GDM_MULTI_VDM_QUIT, 56 ),
	ASSERT_ENUM_STRING( GDM_MULTI_COOP_QUIT, 57 ),
	ASSERT_ENUM_STRING( GDM_LOADING_PROFILE, 58 ),
	ASSERT_ENUM_STRING( GDM_STORAGE_REQUIRED, 59 ),
	ASSERT_ENUM_STRING( GDM_INSUFFICENT_STORAGE_SPACE, 60 ),
	ASSERT_ENUM_STRING( GDM_PARTNER_LEFT, 61 ),
	ASSERT_ENUM_STRING( GDM_RESTORE_CORRUPT_SAVEGAME, 62 ),
	ASSERT_ENUM_STRING( GDM_UNRECOVERABLE_SAVEGAME, 63 ),
	ASSERT_ENUM_STRING( GDM_PROFILE_SAVE_ERROR, 64 ),
	ASSERT_ENUM_STRING( GDM_LOBBY_FULL, 65 ),
	ASSERT_ENUM_STRING( GDM_QUIT_GAME, 66 ),
	ASSERT_ENUM_STRING( GDM_CONNECTION_PROBLEMS, 67 ),
	ASSERT_ENUM_STRING( GDM_VOICE_RESTRICTED, 68 ),
	ASSERT_ENUM_STRING( GDM_LOAD_DAMAGED_FILE, 69 ),
	ASSERT_ENUM_STRING( GDM_MUST_SIGNIN, 70 ),
	ASSERT_ENUM_STRING( GDM_CONNECTION_LOST_NO_LEADERBOARD, 71 ),
	ASSERT_ENUM_STRING( GDM_SP_SIGNIN_CHANGE_POST, 72 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING_WAITING, 73 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING_RELAUNCHING, 74 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING_FAILED_CONNECTION, 75 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING_FAILED_CONNECTION_STATS, 76 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING_FAILED_DISBANDED, 77 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING_FAILED_DISBANDED_STATS, 78 ),
	ASSERT_ENUM_STRING( GDM_MIGRATING_FAILED_PARTNER_LEFT, 79 ),
	ASSERT_ENUM_STRING( GDM_HOST_RETURNED_TO_LOBBY, 80 ),
	ASSERT_ENUM_STRING( GDM_HOST_RETURNED_TO_LOBBY_STATS_DROPPED, 81 ),
	ASSERT_ENUM_STRING( GDM_FAILED_JOIN_LOCAL_SESSION, 82 ),
	ASSERT_ENUM_STRING( GDM_DELETE_CORRUPT_SAVEGAME, 83 ),
	ASSERT_ENUM_STRING( GDM_LEAVE_INCOMPLETE_INSTANCE, 84 ),
	ASSERT_ENUM_STRING( GDM_UNBIND_CONFIRM, 85 ),
	ASSERT_ENUM_STRING( GDM_BINDINGS_RESTORE, 86 ),
	ASSERT_ENUM_STRING( GDM_NEW_HOST, 87 ),
	ASSERT_ENUM_STRING( GDM_CONFIRM_VIDEO_CHANGES, 88 ),
	ASSERT_ENUM_STRING( GDM_UNABLE_TO_USE_SELECTED_STORAGE_DEVICE, 89 ),
	ASSERT_ENUM_STRING( GDM_ERROR_LOADING_SAVEGAME, 90 ),
	ASSERT_ENUM_STRING( GDM_ERROR_SAVING_SAVEGAME, 91 ),
	ASSERT_ENUM_STRING( GDM_DISCARD_CHANGES, 92 ),
	ASSERT_ENUM_STRING( GDM_LEAVE_LOBBY, 93 ),
	ASSERT_ENUM_STRING( GDM_LEAVE_LOBBY_AND_TEAM, 94 ),
	ASSERT_ENUM_STRING( GDM_CONTROLLER_DISCONNECTED_0, 95 ),
	ASSERT_ENUM_STRING( GDM_CONTROLLER_DISCONNECTED_1, 96 ),
	ASSERT_ENUM_STRING( GDM_CONTROLLER_DISCONNECTED_2, 97 ),
	ASSERT_ENUM_STRING( GDM_CONTROLLER_DISCONNECTED_3, 98 ),
	ASSERT_ENUM_STRING( GDM_CONTROLLER_DISCONNECTED_4, 99 ),
	ASSERT_ENUM_STRING( GDM_CONTROLLER_DISCONNECTED_5, 100 ),
	ASSERT_ENUM_STRING( GDM_CONTROLLER_DISCONNECTED_6, 101 ),
	ASSERT_ENUM_STRING( GDM_DLC_ERROR_REMOVED, 102 ),
	ASSERT_ENUM_STRING( GDM_DLC_ERROR_CORRUPT, 103 ),
	ASSERT_ENUM_STRING( GDM_DLC_ERROR_MISSING, 104 ),
	ASSERT_ENUM_STRING( GDM_DLC_ERROR_MISSING_GENERIC, 105 ),
	ASSERT_ENUM_STRING( GDM_DISC_SWAP, 106 ),
	ASSERT_ENUM_STRING( GDM_NEEDS_INSTALL, 107 ),
	ASSERT_ENUM_STRING( GDM_NO_SAVEGAMES_AVAILABLE, 108 ),
	ASSERT_ENUM_STRING( GDM_ERROR_JOIN_TWO_PROFILES_ONE_BOX, 109 ),
	ASSERT_ENUM_STRING( GDM_WARNING_PLAYING_COOP_SOLO, 110 ),
	ASSERT_ENUM_STRING( GDM_MULTI_COOP_QUIT_LOSE_LEADERBOARDS, 111 ),
	ASSERT_ENUM_STRING( GDM_CORRUPT_CONTINUE, 112 ),
	ASSERT_ENUM_STRING( GDM_MULTI_VDM_QUIT_LOSE_LEADERBOARDS, 113 ),
	ASSERT_ENUM_STRING( GDM_WARNING_PLAYING_VDM_SOLO, 114 ),
	ASSERT_ENUM_STRING( GDM_NO_GUEST_SUPPORT, 115 ),
	ASSERT_ENUM_STRING( GDM_DISC_SWAP_CONFIRMATION, 116 ),
	ASSERT_ENUM_STRING( GDM_ERROR_LOADING_PROFILE, 117 ),
	ASSERT_ENUM_STRING( GDM_CANNOT_INVITE_LOBBY_FULL, 118 ),
	ASSERT_ENUM_STRING( GDM_WARNING_FOR_NEW_DEVICE_ABOUT_TO_LOSE_PROGRESS, 119 ),
	ASSERT_ENUM_STRING( GDM_DISCONNECTED, 120 ),
	ASSERT_ENUM_STRING( GDM_INCOMPATIBLE_NEWER_SAVE, 121 ),
	ASSERT_ENUM_STRING( GDM_ACHIEVEMENTS_DISABLED_DUE_TO_CHEATING, 122 ),
	ASSERT_ENUM_STRING( GDM_INCOMPATIBLE_POINTER_SIZE, 123 ),
	ASSERT_ENUM_STRING( GDM_TEXTUREDETAIL_RESTARTREQUIRED, 124 ),
	ASSERT_ENUM_STRING( GDM_TEXTUREDETAIL_INSUFFICIENT_CPU, 125 ),
	ASSERT_ENUM_STRING( GDM_CHECKPOINT_SAVE, 126 ),
	ASSERT_ENUM_STRING( GDM_CALCULATING_BENCHMARK, 127 ),
	ASSERT_ENUM_STRING( GDM_DISPLAY_BENCHMARK, 128 ),
	ASSERT_ENUM_STRING( GDM_DISPLAY_CHANGE_FAILED, 129 ),
	ASSERT_ENUM_STRING( GDM_GPU_TRANSCODE_FAILED, 130 ),
	ASSERT_ENUM_STRING( GDM_OUT_OF_MEMORY, 131 ),
	ASSERT_ENUM_STRING( GDM_CORRUPT_PROFILE, 132 ),
	ASSERT_ENUM_STRING( GDM_PROFILE_TOO_OUT_OF_DATE_DEVELOPMENT_ONLY, 133 ),
	ASSERT_ENUM_STRING( GDM_SP_LOAD_SAVE, 134 ),
	ASSERT_ENUM_STRING( GDM_INSTALLING_TROPHIES, 135 ),
	ASSERT_ENUM_STRING( GDM_XBOX_DEPLOYMENT_TYPE_FAIL, 136 ),
	ASSERT_ENUM_STRING( GDM_SAVEGAME_WRONG_LANGUAGE, 137 ),
	ASSERT_ENUM_STRING( GDM_GAME_RESTART_REQUIRED, 138 ),
	ASSERT_ENUM_STRING( GDM_MAX, 139 )
};

/*
================================================
Dialog box types
================================================
*/
enum dialogType_t {
	DIALOG_INVALID = -1,
	DIALOG_ACCEPT,
	DIALOG_CONTINUE,
	DIALOG_ACCEPT_CANCEL,
	DIALOG_YES_NO,
	DIALOG_CANCEL,
	DIALOG_WAIT,
	DIALOG_WAIT_BLACKOUT,
	DIALOG_WAIT_CANCEL,
	DIALOG_DYNAMIC,
	DIALOG_QUICK_SAVE,
	DIALOG_TIMER_ACCEPT_REVERT,
	DIALOG_CRAWL_SAVE,
	DIALOG_CONTINUE_LARGE,
	DIALOG_BENCHMARK,
};

/*
================================================
idDialogCallback

Generic engine-level callback interface for dialog buttons.
================================================
*/
class idDialogCallback {
public:
    idDialogCallback() : refCount( 0 ) {}
    virtual ~idDialogCallback() {}

    void    AddRef()  { refCount++; }
    void    Release() { if ( --refCount <= 0 ) { delete this; } }

    virtual void Call() = 0;

private:
    int refCount;
};

/*
================================================
idDialogInfo
================================================
*/
class idDialogInfo {
public:
	idDialogInfo() {
		msg					= GDM_INVALID;
		type				= DIALOG_ACCEPT;
		acceptCB			= NULL;
		cancelCB			= NULL;
		altCBOne			= NULL;
		altCBTwo			= NULL;
		showing				= false;
		clear				= false;
		waitClear			= false;
		pause				= false;
		startTime			= 0;
		killTime			= 0;
		leaveOnClear		= false;
		renderDuringLoad	= false;
	}
	gameDialogMessages_t	msg;
	dialogType_t			type;
    idDialogCallback *		acceptCB;
    idDialogCallback *		cancelCB;
    idDialogCallback *		altCBOne;
    idDialogCallback *		altCBTwo;
	bool					showing;
	bool					clear;
	bool					waitClear;
	bool					pause;
	bool					forcePause;
	bool					leaveOnClear;
	bool					renderDuringLoad;
	int						startTime;
	int						killTime;
	idStrStatic< 256 >		overrideMsg;

	idStrId					txt1;
	idStrId					txt2;
	idStrId					txt3;
	idStrId					txt4;
};

/*
==============================================================

  Common Dialog

==============================================================
*/

class idCommonDialog {
public:
					idCommonDialog() {
						dialogPause = false;
						dialogInUse = false;
						dialogShowingSaveIndicatorRequested  = false;
						startSaveTime = 0;
						stopSaveTime = 0;
					}

	virtual         ~idCommonDialog() {}

	virtual void    Init()     = 0;
	virtual void    Shutdown() = 0;
			void    Restart();

	virtual void    Render( bool loading );

	void    AddDialog( gameDialogMessages_t msg, dialogType_t type, idDialogCallback * acceptCallback, idDialogCallback * cancelCallback, bool pause, const char * location = NULL, int lineNumber = 0, bool leaveOnMapHeapReset = false, bool waitOnAtlas = false, bool renderDuringLoad = false );
	void    AddDynamicDialog( gameDialogMessages_t msg, const idStaticList< idDialogCallback *, 4 > & callbacks, const idStaticList< idStrId, 4 > & optionText, bool pause, idStrStatic< 256 > overrideMsg, bool leaveOnMapHeapReset = false, bool waitOnAtlas = false, bool renderDuringLoad = false );

	void    AddDialogIntVal( const char * name, int val );

	void    ClearDialog( gameDialogMessages_t msg,
						 const char * location = NULL,
						 int lineNumber = 0 );
	void    ClearDialogs( bool forceClear = false );
	void    ClearAllDialogHack();

	bool    HasDialogMsg( gameDialogMessages_t msg, bool * isNowActive );
	bool    HasAnyActiveDialog() const;
	bool    IsDialogPausing() const { return dialogPause; }

	virtual void    ShowSaveIndicator( bool show )           = 0;
	virtual bool    HandleDialogEvent( const sysEvent_t * sev ) = 0;
	virtual bool    IsDialogActive() const                   = 0;

	virtual idStr   GetDialogMsg( gameDialogMessages_t msg,
								  idStr & outMessage,
								  idStr & outTitle );

protected:
	virtual bool    IsRendererLoaded()   const = 0;
	virtual bool    IsRendererActive()   const = 0;
	virtual void    ActivateRenderer( bool active ) = 0;

	virtual bool    IsSaveIndicatorActive() const = 0;
	virtual void    RenderDialog( int timeMicroseconds ) = 0;
	virtual void    RenderSaveIndicator( int timeMicroseconds ) = 0;

	virtual void    SetRendererGlobalInt( const char * name, int val )         = 0;
	virtual void    SetRendererGlobalString( const char * name, const char * val ) = 0;

    void    AddRefCallback( idDialogCallback * cb )  { if ( cb ) cb->AddRef(); }
    void    ReleaseCallback( idDialogCallback * cb ) { if ( cb ) cb->Release(); }

    void    InvokeCallback( idDialogCallback * cb )  { if ( cb ) cb->Call(); }

	virtual void    BindDialogToRenderer( const idDialogInfo & info ) = 0;

	void    AddDialogInternal( idDialogInfo & info );
	void    ShowDialog( const idDialogInfo & info );
	void    ShowNextDialog();
	void    ActivateDialog( bool activate );
	void    RemoveWaitDialogs();
	void    ReleaseCallBacks( int index );

	static bool DialogMsgShouldWait( gameDialogMessages_t msg );

protected:
	bool    dialogPause;
	bool    dialogInUse;
	bool    dialogShowingSaveIndicatorRequested;

	int     startSaveTime;
	int     stopSaveTime;

	idStaticList< idDialogInfo, MAX_DIALOGS >   messageList;
};

/*
========================
idCommonDialog::DialogMsgShouldWait

There are a few dialog types that should pause so the user has the ability to read what's going on
========================
*/
ID_INLINE bool idCommonDialog::DialogMsgShouldWait( gameDialogMessages_t msg ) {
	switch ( msg ) {
		case GDM_SAVING:
		case GDM_LOADING_PROFILE:
		case GDM_INSTALLING_TROPHIES:
		case GDM_REFRESHING:
			return true;
		default:
			return false;
	}
}

/*
================================================
idCommonDialog::ClearDialogs
================================================
*/
ID_INLINE void idCommonDialog::ClearDialogs( bool forceClear ) {
	bool topMessageCleared = false;
	for ( int index = 0; index < messageList.Num(); ++index ) {
		if ( !messageList[index].leaveOnClear || forceClear ) {
			ReleaseCallBacks( index );
			messageList.RemoveIndex( index );

			if ( index == 0 ) {
				topMessageCleared = true;
			}
			index--;
		}
	}

	if ( topMessageCleared ) {
		ActivateDialog( false );
	}
}

/*
========================
idCommonDialog::HasAnyActiveDialog
========================
*/
ID_INLINE bool idCommonDialog::HasAnyActiveDialog() const {
	return ( messageList.Num() > 0 ) && ( !messageList[0].clear );
}

/*
================================================
idCommonDialog::AddDialogIntVal
================================================
*/
ID_INLINE void idCommonDialog::AddDialogIntVal( const char * name, int val ) {
	SetRendererGlobalInt( name, val );
}

/*
================================================
idCommonDialog::AddDialog
================================================
*/
ID_INLINE void idCommonDialog::AddDialog( gameDialogMessages_t msg, dialogType_t type, idDialogCallback * acceptCallback,
								idDialogCallback * cancelCallback, bool pause, const char * location, int lineNumber,
								bool leaveOnMapHeapReset, bool waitOnAtlas, bool renderDuringLoad ) {

	keyBindMgr->ClearStates();

	sys->ClearEvents();

	idLib::PrintfIf( cvarSystem->GetCVarBool("popupDialog_debug"), "[%s] msg: %s, pause: %d from: %s:%d\n", __FUNCTION__, dialogStateToString[msg], pause, location == NULL ? "NULL" : location, lineNumber );

	if ( !IsRendererLoaded() ) {
		return;
	}

	idDialogInfo info;
	info.msg = msg;
	info.type = type;
	info.acceptCB = acceptCallback;
	info.cancelCB = cancelCallback;
	info.clear = false;
	info.pause = pause;
	info.startTime = sys->Milliseconds();
	info.killTime = 0;
	info.leaveOnClear = leaveOnMapHeapReset;
	info.renderDuringLoad = renderDuringLoad;

	AddDialogInternal( info );
}

/*
========================
idCommonDialog::AddDynamicDialog
========================
*/
ID_INLINE void idCommonDialog::AddDynamicDialog( gameDialogMessages_t msg, const idStaticList< idDialogCallback *, 4 > & callbacks,
										const idStaticList< idStrId, 4 > & optionText, bool pause, idStrStatic< 256 > overrideMsg,
										bool leaveOnMapHeapReset, bool waitOnAtlas, bool renderDuringLoad ) {
	if ( !IsRendererLoaded() ) {

		return;
	}

	idDialogInfo info;
	info.msg = msg;
	info.overrideMsg = overrideMsg;
	info.type = DIALOG_DYNAMIC;
	info.pause = pause;
	info.leaveOnClear = leaveOnMapHeapReset;
	info.acceptCB = 0 < callbacks.Num() ? callbacks[0] : NULL;
	info.cancelCB = 1 < callbacks.Num() ? callbacks[1] : NULL;
	info.altCBOne = 2 < callbacks.Num() ? callbacks[2] : NULL;
	info.altCBTwo = 3 < callbacks.Num() ? callbacks[3] : NULL;
	info.txt1 = 0 < optionText.Num() ? optionText[0] : idStrId();
	info.txt2 = 1 < optionText.Num() ? optionText[1] : idStrId();
	info.txt3 = 2 < optionText.Num() ? optionText[2] : idStrId();
	info.txt4 = 3 < optionText.Num() ? optionText[3] : idStrId();
	info.renderDuringLoad = renderDuringLoad;

	info.clear = false;
	info.startTime = sys->Milliseconds();
	info.killTime = 0;

	AddDialogInternal( info );
}

/*
========================
idCommonDialog::AddDialogInternal
========================
*/
ID_INLINE void idCommonDialog::AddDialogInternal( idDialogInfo & info ) {

	// don't add the dialog if it's already in the list, we never want to show a duplicate dialog
	if ( HasDialogMsg( info.msg, NULL ) ) {
		return;
	}

	// Remove the delete confirmation if we remove the device and ask for a storage confirmation
	if ( info.msg == GDM_STORAGE_REQUIRED ) {
		if ( HasDialogMsg( GDM_DELETE_SAVE, NULL ) ) {
			ClearDialog( GDM_DELETE_SAVE, NULL, 0 );
		}
		if ( HasDialogMsg( GDM_DELETE_AUTOSAVE, NULL ) ) {
			ClearDialog( GDM_DELETE_AUTOSAVE, NULL, 0 );
		}
		if ( HasDialogMsg( GDM_LOAD_DAMAGED_FILE, NULL ) ) {
			ClearDialog( GDM_LOAD_DAMAGED_FILE, NULL, 0 );
		}
	}

	if ( info.acceptCB != NULL ) {
		AddRefCallback(info.acceptCB);
	}

	if ( info.cancelCB != NULL ) {
		AddRefCallback(info.cancelCB);
	}

	if ( info.altCBOne != NULL ) {
		AddRefCallback(info.altCBOne);
	}

	if ( info.altCBTwo != NULL ) {
		AddRefCallback(info.altCBTwo);
	}

	if ( messageList.Num() == 0 ) {
		messageList.Append( info );
	} else {

		// attempting to add another one beyond our set max. take off the oldest
		// one from the list in order to not crash, but this really isn't a good
		// thing to be happening...
		if ( !verify( messageList.Num() < MAX_DIALOGS ) ) {
			ReleaseCallBacks( MAX_DIALOGS - 1 );
			messageList.RemoveIndex( MAX_DIALOGS - 1 );
		}

		if ( messageList.Num() > 0 ) {
			idLib::PrintfIf( cvarSystem->GetCVarBool("popupDialog_debug"), "[%s] msg: %s new dialog added over old\n", __FUNCTION__, dialogStateToString[info.msg] );

			ActivateRenderer( false );
			messageList.Insert( info, 0 );
		}
	}

	if ( info.type == DIALOG_QUICK_SAVE || info.type == DIALOG_CRAWL_SAVE || messageList[0].msg == GDM_CALCULATING_BENCHMARK ) {
		ShowNextDialog();
	}
}

/*
========================
idCommonDialog::ActivateDialog
========================
*/
ID_INLINE void idCommonDialog::ActivateDialog( bool activate ) {
	dialogInUse = activate;
	ActivateRenderer( activate );
}

/*
================================================
idCommonDialog::ShowDialog
================================================
*/
ID_INLINE void idCommonDialog::ShowDialog( const idDialogInfo & info ) {
	idLib::PrintfIf( cvarSystem->GetCVarBool("popupDialog_debug"), "[%s] msg: %s, m.clear = %d, m.waitClear = %d, m.killTime = %d\n",
		__FUNCTION__, dialogStateToString[info.msg], info.clear, info.waitClear, info.killTime );

	// here instead of add dialog to make sure we meet the TCR, otherwise it has a chance to be visible for less than 1 second
	if ( DialogMsgShouldWait( info.msg ) && !dialogInUse ) {
		startSaveTime = sys->Milliseconds();
		stopSaveTime = 0;
	}

	if ( IsDialogActive() ) {
		ActivateRenderer( false );
	}

	// Delegate all renderer binding to the subclass
	BindDialogToRenderer( info );

	ActivateDialog( true );
}

/*
================================================
idCommonDialog::ShowNextDialog
================================================
*/
ID_INLINE void idCommonDialog::ShowNextDialog() {
	for ( int index = 0; index < messageList.Num(); ++index ) {
		if ( !messageList[index].clear ) {
			ShowDialog(messageList[index]);
			break;
		}
	}
}

/*
========================
idCommonDialog::RemoveWaitDialogs

From TCR# 047
Games must display a message during storage writes for the following conditions and the respective amount of time:
- Writes longer than one second require the standard message be displayed for three seconds.
- Writes longer than three seconds require the standard message be displayed for the length of the write.
- Writes that last one second or less require the shorter message be displayed for one second or the standard message for three seconds.

========================
*/
ID_INLINE void idCommonDialog::RemoveWaitDialogs() {
	bool topMessageCleared = false;
	for ( int index = 0; index < messageList.Num(); ++index ) {
		if ( DialogMsgShouldWait( messageList[index].msg ) ) {
			if ( sys->Milliseconds() >= messageList[index].killTime && messageList[index].waitClear ) {
				messageList[index].clear = true;
				messageList[index].waitClear = false;
				if ( index == 0 ) {
					topMessageCleared = true;
				}
			}
		}
	}

	if ( topMessageCleared && messageList.Num() > 0 ) {
		ActivateDialog( false );
	}
}

/*
================================================
idCommonDialog::ClearAllDialogHack
================================================
*/
ID_INLINE void idCommonDialog::ClearAllDialogHack() {
	for ( int index = 0; index < messageList.Num(); ++index ) {
		messageList[index].clear = true;
		messageList[index].waitClear = false;
	}
}

/*
================================================
idCommonDialog::HasDialogMsg
================================================
*/
ID_INLINE bool idCommonDialog::HasDialogMsg( gameDialogMessages_t msg, bool * isNowActive ) {
	for ( int index = 0; index < messageList.Num(); ++index ) {
		idDialogInfo & info = messageList[index];

		if ( info.msg == msg && !info.clear ) {
			if ( isNowActive != NULL ) {
				*isNowActive = ( index == 0 );
			}
			return true;
		}
	}

	if ( isNowActive != NULL ) {
		*isNowActive = false;
	}

	return false;
}

/*
================================================
idCommonDialog::ClearDialog
================================================
*/
ID_INLINE void idCommonDialog::ClearDialog( gameDialogMessages_t msg, const char * location, int lineNumber ) {
	bool topMessageCleared = false;

	for ( int index = 0; index < messageList.Num(); ++index ) {
		idDialogInfo & info = messageList[index];

		if ( info.msg != msg || info.clear ) {
			continue;
		}

		if ( DialogMsgShouldWait( info.msg ) ) {
			// you can have 2 saving dialogs simultaneously, if you clear back-to-back, we need to let the 2nd dialog
			// get the clear message
			if ( messageList[index].waitClear ) {
				continue;
			}

			int timeShown = sys->Milliseconds() - messageList[index].startTime;

			// for the time being always use the long saves
			if ( timeShown < cvarSystem->GetCVarInteger("dialog_saveClearLevel") ) {
				messageList[index].killTime = sys->Milliseconds() + ( cvarSystem->GetCVarInteger("dialog_saveClearLevel") - timeShown );
				messageList[index].waitClear = true;
			} else {
				messageList[index].clear = true;
				if ( index == 0 ) {
					topMessageCleared = true;
				}
			}
		} else {
			messageList[index].clear = true;
			if ( index == 0 ) {
				topMessageCleared = true;
			}
		}
		assert( info.msg >= GDM_INVALID && info.msg < GDM_MAX );	// not sure why /analyze complains about this
		idLib::PrintfIf( cvarSystem->GetCVarBool("popupDialog_debug"), "[%s] msg: %s, from: %s:%d, topMessageCleared = %d, m.clear = %d, m.waitClear = %d, m.killTime = %d\n",
			__FUNCTION__, dialogStateToString[info.msg], location == NULL ? "NULL" : location, lineNumber,
			topMessageCleared, messageList[index].clear,
			messageList[index].waitClear, messageList[index].killTime );
		break;
	}

	if ( topMessageCleared && messageList.Num() > 0 ) {
		ActivateDialog( false );
	}
}

/*
================================================
idCommonDialog::ReleaseCallBacks
================================================
*/
ID_INLINE void idCommonDialog::ReleaseCallBacks( int index ) {
    if ( index >= messageList.Num() ) {
        return;
    }
    idDialogInfo & info = messageList[ index ];

    if ( info.acceptCB ) {
		info.acceptCB->Release();
		info.acceptCB = NULL;
	}
    if ( info.cancelCB ) {
		info.cancelCB->Release();
		info.cancelCB = NULL;
	}
    if ( info.altCBOne ) {
		info.altCBOne->Release();
		info.altCBOne = NULL;
	}
    if ( info.altCBTwo ) {
		info.altCBTwo->Release();
		info.altCBTwo = NULL;
	}
}

/*
================================================
idCommonDialog::Render
================================================
*/
ID_INLINE void idCommonDialog::Render( bool loading ) {

	dialogPause = false;

	if ( !IsRendererLoaded() ) {
		return;
	}

	RemoveWaitDialogs();

	bool pauseCheck = false;
	for ( int index = 0; index < messageList.Num(); ++index ) {
		if ( messageList[index].clear ) {
			idLib::PrintfIf( cvarSystem->GetCVarBool("popupDialog_debug"), "[%s] removing %s\n", __FUNCTION__, dialogStateToString[messageList[index].msg] );
			ReleaseCallBacks( index );
			messageList.RemoveIndex( index );
			index--;
		} else {
			if ( messageList[index].pause && !pauseCheck ) {
				pauseCheck = true;
			}
		}
	}

	dialogPause = pauseCheck;

	if ( messageList.Num() > 0 && !IsRendererActive() ) {
		ShowNextDialog();
	}

	if ( messageList.Num() == 0 && IsRendererActive() ) {
		ActivateRenderer( false );
	}

	// Decrement the time remaining on the save indicator or turn it off
	if ( !dialogShowingSaveIndicatorRequested && IsSaveIndicatorActive() ) {
		ShowSaveIndicator( false );
	}

	// Handle countdown timer for DIALOG_TIMER_ACCEPT_REVERT
	if ( messageList.Num() > 0 &&
		 messageList[0].type == DIALOG_TIMER_ACCEPT_REVERT ) {
		int startTime     = messageList[0].startTime;
		int endTime       = startTime + PC_KEYBOARD_WAIT;
		int timeRemaining = ( endTime - sys->Milliseconds() ) / 1000;

		if ( timeRemaining <= 0 ) {
			if ( messageList[0].cancelCB != NULL ) {
				InvokeCallback(messageList[0].cancelCB);

			}
			messageList[0].clear = true;
		} else {
			idStrId txtTime = idStrId( "#str_time_remaining" );
			SetRendererGlobalString( "countdownInfo", va( txtTime.GetLocalizedString(), timeRemaining ) );
		}
	}

	if ( messageList.Num() > 0 && loading && !messageList[0].renderDuringLoad ) {
		return;
	}

	int timeMicroseconds = sys->Microseconds();

	if ( IsRendererActive() ) {
		RenderDialog( timeMicroseconds );
	}

	if ( IsSaveIndicatorActive() ) {
		RenderSaveIndicator( timeMicroseconds );
	}
}

/*
========================
idCommonDialog::Restart
========================
*/
ID_INLINE void idCommonDialog::Restart() {
	Shutdown();
	Init();
}

/*
================================================
idCommonDialog::GetDialogMsg
================================================
*/
ID_INLINE idStr idCommonDialog::GetDialogMsg( gameDialogMessages_t msg, idStr & message, idStr & title ) {

		message = "#str_dlg_pc_";

	switch ( msg ) {
		case GDM_SWAP_DISKS_TO1: {
			message.Append( "switch_disc_to_1" );
			break;
		}
		case GDM_SWAP_DISKS_TO2: {
			message.Append( "switch_disc_to_2" );
			break;
		}
		case GDM_SWAP_DISKS_TO3: {
			message.Append( "switch_disc_to_3" );
			break;
		}
		case GDM_NO_GAMER_PROFILE: {
			message.Append( "signin_request" );
			break;
		}
		case GDM_PLAY_ONLINE_NO_PROFILE: {
			message.Append( "online_signin_request" );
			break;
		}
		case GDM_LEADERBOARD_ONLINE_NO_PROFILE: {
			message.Append( "online_signing_request_leaderboards" );
			break;
		}
		case GDM_NO_STORAGE_SELECTED: {
			message.Append( "storage_device_selection_request" );
			break;
		}
		case GDM_ONLINE_INCORRECT_PERMISSIONS: {
			message.Append( "incorrect_online_permissions" );
			break;
		}
		case GDM_SP_QUIT_SAVE: {
			title = idLocalization::GetString( "#str_04215" );
			title.ToUpper();
			if ( cvarSystem->GetCVarBool("g_demoMode") ) {
				message = "#str_04145";
			} else {
				message = "#str_dlg_quit_progress_lost";
			}
			break;
		}
		case GDM_SP_LOAD_SAVE: {
			title = idLocalization::GetString( "#str_02187" );
			title.ToUpper();
			message = "#str_dlg_360_load_request";
			break;
		}
		case GDM_SP_RESTART_SAVE: {
			title = idLocalization::GetString( "#str_04271" );
			title.ToUpper();
			message = "#str_dlg_restart_progress_lost";
			break;
		}
		case GDM_SP_SIGNIN_CHANGE: {
			message = "#str_dlg_signin_changed";
			break;
		}
		case GDM_SERVER_NOT_AVAILABLE: {
			message.Append( "game_server_unavailable" );
			break;
		}
		case GDM_CONNECTION_LOST_HOST: {
			message = "#str_dlg_opponent_connection_lost_ranking_not_counted";
			break;
		}
		case GDM_CONNECTION_LOST: {
			message.Append( "online_connection_lost_main_menu_return" );
			break;
		}
		case GDM_OPPONENT_CONNECTION_LOST: {
			message = "#str_dlg_opponent_connection_lost";
			break;
		}
		case GDM_HOST_CONNECTION_LOST: {
			message = "#str_dlg_host_connection_lost";
			break;
		}
		case GDM_HOST_CONNECTION_LOST_STATS: {
			message = "#str_dlg_host_connection_lost_ranking_not_counted";
			break;
		}
		case GDM_FAILED_TO_LOAD_RANKINGS: {
			message = "#str_dlg_ranking_load_failed";
			break;
		}
		case GDM_HOST_QUIT: {
			message = "#str_dlg_host_quit";
			break;
		}
		case GDM_OPPONENT_LEFT: {
			message = "#str_dlg_opponent_left";
			break;
		}
		case GDM_PARTNER_LEFT: {
			message = "#str_dlg_partner_left";
			break;
		}
		case GDM_NO_MATCHES_FOUND: {
			message = "#str_dlg_matches_not_found";
			break;
		}
		case GDM_INVALID_INVITE: {
			message = "#str_dlg_invalid_game";
			break;
		}
		case GDM_KICKED: {
			message = "#str_dlg_kicked";
			break;
		}
		case GDM_BANNED: {
			message = "#str_dlg_banned";
			break;
		}
		case GDM_SAVING: {
			title = "#str_save_dialog_heading";
			message.Append( "saving" );
			break;
		}
		case GDM_GAME_ERROR: {
			title = "#str_01754";
			break;
		}
		case GDM_OVERWRITE_SAVE: {
			title = "#str_02306";
			message = "#str_dlg_overwrite_save";
			break;
		}
		case GDM_LOAD_REQUEST: {
			message.Append( "load_request" );
			break;
		}
		case GDM_AUTOSAVE_DISABLED_STORAGE_REMOVED: {
			message.Append( "storage_removed_autosave_disabled" );
			break;
		}
		case GDM_STORAGE_INVALID: {
			message.Append( "storage_not_available" );
			break;
		}
		case GDM_CONNECTING: {
			message = "#str_dlg_connecting";
			break;
		}
		case GDM_REFRESHING: {
			title = "#str_01694";
			message = "#str_dlg_refreshing";
			break;
		}
		case GDM_DELETE_SAVE: {
			title = "#str_02313";
			message.Append( "delete_save" );
			break;
		}
		case GDM_DELETING: {
			message.Append( "deleting" );
			break;
		}
		case GDM_BINDING_ALREDY_SET: {
			message.Append( "bind_exists" );
			break;
		}
		case GDM_CANNOT_BIND: {
			message.Append( "cannont_bind" );
			break;
		}
		case GDM_OVERLAY_DISABLED: {
			message.Append( "overlay_disabled" );
			break;
		}
		case GDM_BECAME_HOST_PARTY: {
			message = "#str_dlg_became_host_party";
			break;
		}
		case GDM_NEW_HOST_PARTY: {
			message = "#str_dlg_new_host_party";
			break;
		}
		case GDM_LOBBY_BECAME_HOST_GAME: {
			message = "#str_dlg_lobby_became_host_game";
			break;
		}
		case GDM_LOBBY_NEW_HOST_GAME: {
			message.Append( "lobby_new_host_game" );
			break;
		}
		case GDM_NEW_HOST_GAME: {
			message = "#str_dlg_new_host_game";
			break;
		}
		case GDM_NEW_HOST_GAME_STATS_DROPPED: {
			message = "#str_dlg_new_host_game_stats_dropped";
			break;
		}
		case GDM_BECAME_HOST_GAME: {
			message.Append( "became_host_game" );
			break;
		}
		case GDM_BECAME_HOST_GAME_STATS_DROPPED: {
			message = "#str_dlg_became_host_game_stats_dropped";
			break;
		}
		case GDM_LOBBY_DISBANDED: {
			message.Append( "lobby_disbanded" );
			break;
		}
		case GDM_LEAVE_WITH_PARTY: {
			message = "#str_dlg_leave_with_party";
			break;
		}
		case GDM_LEAVE_LOBBY_RET_MAIN: {
			message.Append( "leave_lobby_ret_main" );
			break;
		}
		case GDM_LEAVE_LOBBY_RET_NEW_PARTY: {
			message.Append( "leave_lobby_ret_new_party" );
			break;
		}
		case GDM_MIGRATING: {
			message = "#str_online_host_migration";
			break;
		}
		case GDM_MIGRATING_WAITING: {
			message = "#str_online_host_migration_waiting";
			break;
		}
		case GDM_MIGRATING_RELAUNCHING: {
			message = "#str_online_host_migration_relaunching";
			break;
		}
		case GDM_DIRECT_MAP_CHANGE: {
			message = "#str_dlg_direct_map_change";
			break;
		}
		case GDM_DELETE_AUTOSAVE: {
			message.Append( "delete_autosave" );
			break;
		}
		case GDM_MULTI_RETRY: {
			message = "#str_online_confirm_retry";
			break;
		}
		case GDM_MULTI_SELF_DESTRUCT: {
			message = "#str_online_confirm_suicide";
			break;
		}
		case GDM_MULTI_VDM_QUIT: {
			message = "#str_online_confirm_quit_generic";
			break;
		}
		case GDM_MULTI_COOP_QUIT: {
			message = "#str_online_confirm_coop_quit_game_generic";
			break;
		}
		case GDM_LOADING_PROFILE: {
			message = "#str_dlg_loading_profile";
			title = "#str_dlg_updating_profile";
			break;
		}
		case GDM_STORAGE_REQUIRED: {
			message.Append( "storage_required" );
			break;
		}
		case GDM_INSUFFICENT_STORAGE_SPACE: {
			message = "#str_dlg_insufficient_space";
			break;
		}
		case GDM_RESTORE_CORRUPT_SAVEGAME: {
			message = "#str_dlg_restore_corrupt_savegame";
			break;
		}
		case GDM_UNRECOVERABLE_SAVEGAME: {
			message = "#str_dlg_unrecoverable_savegame";
			break;
		}
		case GDM_PROFILE_SAVE_ERROR: {
			message.Append( "profile_save_error" );
			break;
		}
		case GDM_LOBBY_FULL: {
			message.Append( "lobby_full" );
			break;
		}
		case GDM_QUIT_GAME: {
			title = "#str_01975";	// EXIT GAME
			message = "#str_dlg_confirm_quit";
			break;
							}
		case GDM_CONNECTION_PROBLEMS: {
			message = "#str_online_connection_problems";
			break;
		}
		case GDM_VOICE_RESTRICTED: {
			message.Append( "voice_restricted" );
			break;
		}
		case GDM_MUST_SIGNIN: {
			message.Append( "must_signin" );
			break;
		}
		case GDM_LOAD_DAMAGED_FILE: {
			message = "#str_dlg_corrupt_save_file";
			break;
		}
		case GDM_DLC_ERROR_REMOVED: {
			message.Append( "dlc_error_content_removed" );
			break;
		}
		case GDM_DLC_ERROR_CORRUPT: {
			message.Append( "dlc_error_content_corrupt" );
			break;
		}
		case GDM_DLC_ERROR_MISSING: {
			message.Append( "dlc_error_content_missing" );
			break;
		}
		case GDM_DLC_ERROR_MISSING_GENERIC: {
			message.Append( "dlc_error_content_missing_generic" );
			break;
		}
		case GDM_CONNECTION_LOST_NO_LEADERBOARD: {
			message.Append( "online_connection_lost_no_leaderboard" );
			break;
		}
		case GDM_SP_SIGNIN_CHANGE_POST: {
			message.Append( "signin_changed_post" );
			break;
		}
		case GDM_MIGRATING_FAILED_CONNECTION: {
			message = "#str_online_host_migration_failed";
			break;
		}
		case GDM_MIGRATING_FAILED_CONNECTION_STATS: {
			message = "#str_online_host_migration_failed_stats";
			break;
		}
		case GDM_MIGRATING_FAILED_DISBANDED: {
			message = "#str_online_host_migration_failed_disbanded";
			break;
		}
		case GDM_MIGRATING_FAILED_DISBANDED_STATS: {
			message = "#str_online_host_migration_failed_disbanded_stats";
			break;
		}
		case GDM_MIGRATING_FAILED_PARTNER_LEFT: {
			message = "#str_online_host_migration_failed_partner_left";
			break;
		}
		case GDM_FAILED_JOIN_LOCAL_SESSION: {
			message = "#str_dlg_failed_join_local_session";
			break;
		}
		case GDM_DELETE_CORRUPT_SAVEGAME:
			message = "#str_dlg_delete_corrupt_savegame";
			break;
		case GDM_LEAVE_INCOMPLETE_INSTANCE:
			message = "#str_dlg_leave_incomplete_instance";
			break;
		case GDM_UNBIND_CONFIRM:
			message = "#str_dlg_bind_unbind";
			break;
		case GDM_BINDINGS_RESTORE:
			message = "#str_dlg_bind_restore";
			break;
		case GDM_HOST_RETURNED_TO_LOBBY: {
			message.Append( "host_quit_to_lobby" );
			break;
		}
		case GDM_HOST_RETURNED_TO_LOBBY_STATS_DROPPED: {
			message.Append( "host_quit_to_lobby_stats_dropped" );
			break;
		}
		case GDM_NEW_HOST: {
			message.Append( "new_host" );
			break;
		}
		case GDM_DISC_SWAP: {
			message = "#str_dlg_disc_swap";
			break;
		}
		case GDM_NO_SAVEGAMES_AVAILABLE: {
			message = "#str_dlg_no_savegames_available";
			break;
		}
		case GDM_CONFIRM_VIDEO_CHANGES: {
			message = "#str_dlg_confirm_display_changes";
			break;
			}
		case GDM_UNABLE_TO_USE_SELECTED_STORAGE_DEVICE: {
			message.Append( "unable_to_use_selected_storage_device" );
			break;
		}
		case GDM_ERROR_LOADING_SAVEGAME: {
			message = "#str_dlg_error_loading_savegame";
			break;
		}
		case GDM_ERROR_SAVING_SAVEGAME: {
			message = "#str_dlg_error_saving_savegame";
			break;
		}
		case GDM_DISCARD_CHANGES: {
			message = "#str_dlg_confirm_discard";
			break;
		}
		case GDM_LEAVE_LOBBY: {
			message = "#str_online_leave_game_lobby_alt_02";
			break;
		}
		case GDM_LEAVE_LOBBY_AND_TEAM: {
			message = "#str_online_party_leave_game";
			break;
		}
		case GDM_CONTROLLER_DISCONNECTED_0:
		case GDM_CONTROLLER_DISCONNECTED_1:
		case GDM_CONTROLLER_DISCONNECTED_2:
		case GDM_CONTROLLER_DISCONNECTED_3:
		case GDM_CONTROLLER_DISCONNECTED_4:
		case GDM_CONTROLLER_DISCONNECTED_5:
		case GDM_CONTROLLER_DISCONNECTED_6:{
			message = "#str_dlg_reconnect_controller";
			break;
		}
		case GDM_NEEDS_INSTALL: {
			message = "#str_dlg_game_install_message";
			break;
		}
		case GDM_ERROR_JOIN_TWO_PROFILES_ONE_BOX: {
			message.Append( "online_join_error_two_profiles_one_box" );
			break;
		}
		case GDM_WARNING_PLAYING_COOP_SOLO: {
			message = "#str_online_lotw_solo_warning_alt_05";
			break;
		}
		case GDM_MULTI_COOP_QUIT_LOSE_LEADERBOARDS: {
			message = "#str_online_confirm_coop_quit_game";
			break;
		}
		case GDM_CORRUPT_CONTINUE: {
			message = "#str_corrupt_save_load";
			break;
		}
		case GDM_MULTI_VDM_QUIT_LOSE_LEADERBOARDS: {
			message = "#str_online_confirm_quit_game";
			break;
		}
		case GDM_WARNING_PLAYING_VDM_SOLO: {
			message = "#str_online_cr_custom_game_no_stats";
			break;
		}
		case GDM_NO_GUEST_SUPPORT: {
			message = "#str_dlg_ps3_incorrect_online_permissions";
			break;
		}
		case GDM_DISC_SWAP_CONFIRMATION: {
			message = "#str_dlg_disc_swap_confirmation";
			break;
		}
		case GDM_ERROR_LOADING_PROFILE: {
			message.Append( "error_loading_profile" );
			break;
		}
		case GDM_CANNOT_INVITE_LOBBY_FULL: {
			message = "#str_online_join_error_full";
			break;
		 }
		case GDM_WARNING_FOR_NEW_DEVICE_ABOUT_TO_LOSE_PROGRESS: {
			message = "#str_dlg_360_new_device_selected";
			break;
		}
		case GDM_DISCONNECTED: {
			message = "#str_online_connection_error_03";
			break;
		}
		case GDM_INCOMPATIBLE_NEWER_SAVE: {
			message = "#str_dlg_newer_incompatible_savegame";
			break;
		}
		case GDM_ACHIEVEMENTS_DISABLED_DUE_TO_CHEATING: {
			message = "#str_dlg_achievements_disabled_due_to_cheating";
			break;
		}
		case GDM_INCOMPATIBLE_POINTER_SIZE: {
			message = "#str_dlg_pointer_size_mismatch";
			break;
		}
		case GDM_TEXTUREDETAIL_RESTARTREQUIRED: {
			message = "#str_swf_texture_restart";
			break;
		}
		case GDM_TEXTUREDETAIL_INSUFFICIENT_CPU: {
			message = "#str_swf_insufficient_cores";
			break;
		}
		case GDM_CALCULATING_BENCHMARK: {
			message = "#str_swf_calc_benchmark";
			break;
		}
		case GDM_DISPLAY_BENCHMARK: {
			message = "BENCHMARK SCORE = ";
			break;
		}
		case GDM_DISPLAY_CHANGE_FAILED: {
			message = "#str_swf_display_changes_failed";
			break;
		}
		case GDM_GPU_TRANSCODE_FAILED: {
			message = "#str_swf_gpu_transcode_failed";
			break;
		}
		case GDM_OUT_OF_MEMORY: {
			message = "#str_swf_failed_level_load";
			break;
		}
		case GDM_CORRUPT_PROFILE: {
			message = "#str_dlg_corrupt_profile";
			break;
		}
		case GDM_PROFILE_TOO_OUT_OF_DATE_DEVELOPMENT_ONLY: {
			message = "#str_dlg_profile_too_out_of_date_development_only";
			break;
		}
		case GDM_INSTALLING_TROPHIES: {
			title = "#str_dlg_ps3_trophy_install_heading";
			message = "#str_dlg_ps3_trophy_installing";
			break;
		}
		case GDM_XBOX_DEPLOYMENT_TYPE_FAIL: {
			message = "#str_dlg_360_installed_continue";
			break;
		}
		case GDM_GAME_RESTART_REQUIRED: {
			message = "#str_dlg_game_restart_required";
			break;
		}
		default: {
			message = "MESSAGE TYPE NOT DEFINED";
			break;
		}
	}

	return message;
}

#endif // __COMMON_DIALOG_H__
