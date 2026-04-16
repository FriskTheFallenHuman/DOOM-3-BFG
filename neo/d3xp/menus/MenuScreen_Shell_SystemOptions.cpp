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
#include "../Game_local.h"

const static int NUM_SYSTEM_OPTIONS_OPTIONS = 8;

/*
========================
idMenuScreen_Shell_SystemOptions::Initialize
========================
*/
void idMenuScreen_Shell_SystemOptions::Initialize( idMenuHandler * data ) {
	idMenuScreen::Initialize( data );

	if ( data != NULL ) {
		menuGUI = data->GetGUI();
	}

	SetSpritePath( "menuSystemOptions" );

	options = new (TAG_SWF) idMenuWidget_DynamicList();
	options->SetNumVisibleOptions( NUM_SYSTEM_OPTIONS_OPTIONS );
	options->SetSpritePath( GetSpritePath(), "info", "options" );
	options->SetWrappingAllowed( true );
	options->SetControlList( true );
	options->Initialize( data );

	btnBack = new (TAG_SWF) idMenuWidget_Button();
	btnBack->Initialize( data );
	btnBack->SetLabel( "#str_swf_settings" );
	btnBack->SetSpritePath( GetSpritePath(), "info", "btnBack" );
	btnBack->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_GO_BACK );

	AddChild( options );
	AddChild( btnBack );

	idMenuWidget_ControlButton * control;
	control = new (TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_02154" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN );
	options->AddChild( control );

	control = new (TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_swf_framerate" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FRAMERATE );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FRAMERATE );
	options->AddChild( control );

	control = new (TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_04126" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_VSYNC );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_VSYNC );
	options->AddChild( control );

	control = new (TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_04128" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_ANTIALIASING );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_ANTIALIASING );
	options->AddChild( control );

	control = new (TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_swf_motionblur" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_MOTIONBLUR );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_MOTIONBLUR );
	options->AddChild( control );

	control = new (TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_swf_lodbias" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_LODBIAS );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_LODBIAS );
	options->AddChild( control );

	control = new (TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_02155" );	// Brightness
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_BRIGHTNESS );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_BRIGHTNESS );
	options->AddChild( control );

	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER, WIDGET_EVENT_SCROLL_DOWN ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER, WIDGET_EVENT_SCROLL_UP ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_RELEASE ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_DOWN_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_RELEASE ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_UP_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_LSTICK ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER, WIDGET_EVENT_SCROLL_DOWN_LSTICK ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_LSTICK ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER, WIDGET_EVENT_SCROLL_UP_LSTICK ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE ).Set( new (TAG_SWF) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE ) );
}

/*
========================
idMenuScreen_Shell_SystemOptions::Update
========================
*/
void idMenuScreen_Shell_SystemOptions::Update() {

	if ( menuData != NULL ) {
		idMenuWidget_CommandBar * cmdBar = menuData->GetCmdBar();
		if ( cmdBar != NULL ) {
			cmdBar->ClearAllButtons();
			idMenuWidget_CommandBar::buttonInfo_t * buttonInfo;
			buttonInfo = cmdBar->GetButton( idMenuWidget_CommandBar::BUTTON_JOY2 );
			if ( menuData->GetPlatform() != 2 ) {
				buttonInfo->label = "#str_00395";
			}
			buttonInfo->action.Set( WIDGET_ACTION_GO_BACK );

			buttonInfo = cmdBar->GetButton( idMenuWidget_CommandBar::BUTTON_JOY1 );
			buttonInfo->action.Set( WIDGET_ACTION_PRESS_FOCUSED );
		}
	}

	idSWFScriptObject & root = GetSWFObject()->GetRootObject();
	if ( BindSprite( root ) ) {
		idSWFTextInstance * heading = GetSprite()->GetScriptObject()->GetNestedText( "info", "txtHeading" );
		if ( heading != NULL ) {
			heading->SetText( "#str_00183" );	// FULLSCREEN
			heading->SetStrokeInfo( true, 0.75f, 1.75f );
		}

		idSWFSpriteInstance * gradient = GetSprite()->GetScriptObject()->GetNestedSprite( "info", "gradient" );
		if ( gradient != NULL && heading != NULL ) {
			gradient->SetXPos( heading->GetTextLength() );
		}
	}

	if ( btnBack != NULL ) {
		btnBack->BindSprite( root );
	}

	idMenuScreen::Update();
}

/*
========================
idMenuScreen_Shell_SystemOptions::ShowScreen
========================
*/
void idMenuScreen_Shell_SystemOptions::ShowScreen( const mainMenuTransition_t transitionType ) {

	systemData.LoadData();

	idMenuScreen::ShowScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SystemOptions::HideScreen
========================
*/
void idMenuScreen_Shell_SystemOptions::HideScreen( const mainMenuTransition_t transitionType ) {

	if ( systemData.IsRestartRequired() ) {
		class idDialogRestartCallback : public idDialogCallback {
		public:
			idDialogRestartCallback( gameDialogMessages_t _msg, bool _restart ) {
				msg = _msg;
				restart = _restart;
			}
			void Call() override {
				common->Dialog().ClearDialog( msg );
				if ( restart ) {
					idStr cmdLine = sys->GetCmdLine();
					if ( cmdLine.Find( "com_skipIntroVideos" ) < 0 ) {
						cmdLine.Append( " +set com_skipIntroVideos 1" );
					}
					sys->ReLaunch( (void*)cmdLine.c_str(), cmdLine.Length() );
				}
			}
		private:
			gameDialogMessages_t msg;
			bool restart;
		};
		idStaticList<idDialogCallback *, 4> callbacks;
		idStaticList<idStrId, 4> optionText;
		callbacks.Append( new idDialogRestartCallback( GDM_GAME_RESTART_REQUIRED, false ) );
		callbacks.Append( new idDialogRestartCallback( GDM_GAME_RESTART_REQUIRED, true ) );
		optionText.Append( idStrId( "#str_00100113" ) ); // Continue
		optionText.Append( idStrId( "#str_02487" ) ); // Restart Now
		ADD_DYNAMIC_DIALOG( GDM_GAME_RESTART_REQUIRED, callbacks, optionText, true, idStr() );
	}

	if ( systemData.IsDataChanged() ) {
		systemData.CommitData();
	}

	idMenuScreen::HideScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SystemOptions::HandleAction h
========================
*/
bool idMenuScreen_Shell_SystemOptions::HandleAction( idWidgetAction & action, const idWidgetEvent & event, idMenuWidget * widget, bool forceHandled ) {

	if ( menuData == NULL ) {
		return true;
	}

	if ( menuData->ActiveScreen() != SHELL_AREA_SYSTEM_OPTIONS ) {
		return false;
	}

	widgetAction_t actionType = action.GetType();
	const idSWFParmList & parms = action.GetParms();

	switch ( actionType ) {
		case WIDGET_ACTION_GO_BACK: {
			if ( menuData != NULL ) {
				menuData->SetNextScreen( SHELL_AREA_SETTINGS, MENU_TRANSITION_SIMPLE );
			}
			return true;
		}
		case WIDGET_ACTION_ADJUST_FIELD:
			if ( widget->GetDataSourceFieldIndex() == idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN ) {
				menuData->SetNextScreen( SHELL_AREA_RESOLUTION, MENU_TRANSITION_SIMPLE );
				return true;
			}
			break;
		case WIDGET_ACTION_COMMAND: {

			if ( options == NULL ) {
				return true;
			}

			int selectionIndex = options->GetFocusIndex();
			if ( parms.Num() > 0 ) {
				selectionIndex = parms[0].ToInteger();
			}

			if ( options && selectionIndex != options->GetFocusIndex() ) {
				options->SetViewIndex( options->GetViewOffset() + selectionIndex );
				options->SetFocusIndex( selectionIndex );
			}

			switch ( parms[0].ToInteger() ) {
				case idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN: {
					menuData->SetNextScreen( SHELL_AREA_RESOLUTION, MENU_TRANSITION_SIMPLE );
					return true;
				}
				default: {
					systemData.AdjustField( parms[0].ToInteger(), 1 );
					options->Update();
				}
			}

			return true;
		}
		case WIDGET_ACTION_START_REPEATER: {

			if ( options == NULL ) {
				return true;
			}

			if ( parms.Num() == 4 ) {
				int selectionIndex = parms[3].ToInteger();
				if ( selectionIndex != options->GetFocusIndex() ) {
					options->SetViewIndex( options->GetViewOffset() + selectionIndex );
					options->SetFocusIndex( selectionIndex );
				}
			}
			break;
		}
	}

	return idMenuWidget::HandleAction( action, event, widget, forceHandled );
}

/////////////////////////////////
// SCREEN SETTINGS
/////////////////////////////////

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::idMenuDataSource_SystemSettings
========================
*/
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::idMenuDataSource_SystemSettings() {
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::LoadData
========================
*/
void idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::LoadData() {
	originalFramerate = cvarSystem->GetCVarInteger( "com_engineHz" );
	originalAntialias = cvarSystem->GetCVarInteger( "r_multiSamples" );
	originalMotionBlur = cvarSystem->GetCVarInteger( "r_motionBlur" );
	originalVsync = cvarSystem->GetCVarInteger( "r_swapInterval" );
	originalBrightness = cvarSystem->GetCVarFloat( "r_lightScale" );

	const int fullscreen = cvarSystem->GetCVarInteger( "r_fullscreen" );
	if ( fullscreen > 0 ) {
		renderSystem->GetModeListForDisplay( fullscreen - 1, modeList );
	} else {
		modeList.Clear();
	}
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsRestartRequired
========================
*/
bool idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsRestartRequired() const {
	if ( originalAntialias != cvarSystem->GetCVarInteger( "r_multiSamples" ) ) {
		return true;
	}
	if ( originalFramerate != cvarSystem->GetCVarInteger( "com_engineHz" ) ) {
		return true;
	}
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::CommitData
========================
*/
void idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::CommitData() {
	cvarSystem->SetModifiedFlags( CVAR_ARCHIVE );
}

/*
========================
AdjustOption
Given a current value in an array of possible values, returns the next n value
========================
*/
int AdjustOption( int currentValue, const int values[], int numValues, int adjustment ) {
	int index = 0;
	for ( int i = 0; i < numValues; i++ ) {
		if ( currentValue == values[i] ) {
			index = i;
			break;
		}
	}
	index += adjustment;
	while ( index < 0 ) {
		index += numValues;
	}
	index %= numValues;
	return values[index];
}

/*
========================
LinearAdjust
Linearly converts a float from one scale to another
========================
*/
float LinearAdjust( float input, float currentMin, float currentMax, float desiredMin, float desiredMax ) {
	return ( ( input - currentMin ) / ( currentMax - currentMin ) ) * ( desiredMax - desiredMin ) + desiredMin;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::AdjustField
========================
*/
void idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::AdjustField( const int fieldIndex, const int adjustAmount ) {
	switch ( fieldIndex ) {
		case SYSTEM_FIELD_FRAMERATE: {
			static const int numValues = 2;
			static const int values[numValues] = { 60, 120 };
			cvarSystem->SetCVarInteger( "com_engineHz", AdjustOption( cvarSystem->GetCVarInteger( "com_engineHz" ), values, numValues, adjustAmount ) );
			break;
		}
		case SYSTEM_FIELD_VSYNC: {
			static const int numValues = 3;
			static const int values[numValues] = { 0, 1, 2 };
			cvarSystem->SetCVarInteger( "r_swapInterval", AdjustOption( cvarSystem->GetCVarInteger( "r_swapInterval" ), values, numValues, adjustAmount ) );
			break;
		}
		case SYSTEM_FIELD_ANTIALIASING: {
			static const int numValues = 5;
			static const int values[numValues] = { 0, 2, 4, 8, 16 };
			cvarSystem->SetCVarInteger( "r_multiSamples", AdjustOption( cvarSystem->GetCVarInteger( "r_multiSamples" ), values, numValues, adjustAmount ) );
			break;
		}
		case SYSTEM_FIELD_MOTIONBLUR: {
			static const int numValues = 5;
			static const int values[numValues] = { 0, 2, 3, 4, 5 };
			cvarSystem->SetCVarInteger( "r_motionBlur", AdjustOption( cvarSystem->GetCVarInteger( "r_motionBlur" ), values, numValues, adjustAmount ) );
			break;
		}
		case SYSTEM_FIELD_LODBIAS: {
			const float percent = LinearAdjust( cvarSystem->GetCVarFloat( "r_lodBias" ), -1.0f, 1.0f, 0.0f, 100.0f );
			const float adjusted = percent + (float)adjustAmount * 5.0f;
			const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
			cvarSystem->SetCVarFloat( "r_lodBias", LinearAdjust( clamped, 0.0f, 100.0f, -1.0f, 1.0f ) );
			break;
		}
		case SYSTEM_FIELD_BRIGHTNESS: {
			const float percent = LinearAdjust( cvarSystem->GetCVarFloat( "r_lightScale" ), 2.0f, 4.0f, 0.0f, 100.0f );
			const float adjusted = percent + (float)adjustAmount;
			const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
			cvarSystem->SetCVarFloat( "r_lightScale", LinearAdjust( clamped, 0.0f, 100.0f, 2.0f, 4.0f ) );
			break;
		}
	}
	cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::GetField
========================
*/
idSWFScriptVar idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::GetField( const int fieldIndex ) const {
	switch ( fieldIndex ) {
		case SYSTEM_FIELD_FULLSCREEN: {
			const int fullscreen = cvarSystem->GetCVarInteger( "r_fullscreen" );
			const int vidmode = cvarSystem->GetCVarInteger( "r_vidMode" );
			if ( fullscreen == 0 ) {
				return "#str_swf_disabled";
			}
			if ( fullscreen < 0 || vidmode < 0 || vidmode >= modeList.Num() ) {
				return "???";
			}
			if ( modeList[vidmode].displayHz == 60 ) {
				return va( "%4i x %4i", modeList[vidmode].width, modeList[vidmode].height );
			} else {
				return va( "%4i x %4i @ %dhz", modeList[vidmode].width, modeList[vidmode].height, modeList[vidmode].displayHz );
			}
		}
		case SYSTEM_FIELD_FRAMERATE:
			return va( "%d FPS", cvarSystem->GetCVarInteger( "com_engineHz" ) );
		case SYSTEM_FIELD_VSYNC:
			if ( cvarSystem->GetCVarInteger( "r_swapInterval" ) == 1 ) {
				return "#str_swf_smart";
			} else if ( cvarSystem->GetCVarInteger( "r_swapInterval" ) == 2 ) {
				return "#str_swf_enabled";
			} else {
				return "#str_swf_disabled";
			}
		case SYSTEM_FIELD_ANTIALIASING:
			if ( cvarSystem->GetCVarInteger( "r_multiSamples" ) == 0 ) {
				return "#str_swf_disabled";
			}
			return va( "%dx", cvarSystem->GetCVarInteger( "r_multiSamples" ) );
		case SYSTEM_FIELD_MOTIONBLUR:
			if ( cvarSystem->GetCVarInteger( "r_motionBlur" ) == 0 ) {
				return "#str_swf_disabled";
			}
			return va( "%dx", idMath::IPow( 2, cvarSystem->GetCVarInteger( "r_motionBlur" ) ) );
		case SYSTEM_FIELD_LODBIAS:
			return LinearAdjust( cvarSystem->GetCVarFloat( "r_lodBias" ), -1.0f, 1.0f, 0.0f, 100.0f );
		case SYSTEM_FIELD_BRIGHTNESS:
			return LinearAdjust( cvarSystem->GetCVarFloat( "r_lightScale" ), 2.0f, 4.0f, 0.0f, 100.0f );
	}
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsDataChanged
========================
*/
bool idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsDataChanged() const {
	if ( originalFramerate != cvarSystem->GetCVarInteger( "com_engineHz" ) ) {
		return true;
	}
	if ( originalAntialias != cvarSystem->GetCVarInteger( "r_multiSamples" ) ) {
		return true;
	}
	if ( originalMotionBlur != cvarSystem->GetCVarInteger( "r_motionBlur" ) ) {
		return true;
	}
	if ( originalVsync != cvarSystem->GetCVarInteger( "r_swapInterval" ) ) {
		return true;
	}
	if ( originalBrightness != cvarSystem->GetCVarFloat( "r_lightScale" ) ) {
		return true;
	}
	return false;
}
