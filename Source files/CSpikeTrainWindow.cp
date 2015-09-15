// =================================================================================
//	CSpikeTrainWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CSpikeTrainWindow.h	
#include "CSpike6App.h"
#include "CSettingsWindow.h"
#include <LString.h>
#include <UReanimator.h>
#include <Math.h>
#include <LPushButton.h>
#include <LEditText.h>
#include "CSpikeTrainWindow.h"
#include "CAllView.h"
#include "CTempView.h"
//#include "ClusterConstants.h"
#include "CViewInfoPane1.h"


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow
// ---------------------------------------------------------------------------------

CSpikeTrainWindow::CSpikeTrainWindow()
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CSpikeTrainWindow::CSpikeTrainWindow(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CSpikeTrainWindow::CSpikeTrainWindow(
	ResIDT		inWINDid,
	UInt32		inAttributes,
	LCommander	*inSuperCommander )
		: LWindow( inWINDid, inAttributes, inSuperCommander )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(LStream *)
// ---------------------------------------------------------------------------------

CSpikeTrainWindow::CSpikeTrainWindow(
	LStream *inStream )
		: LWindow( inStream )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ ~CProgressWindow
// ---------------------------------------------------------------------------------

CSpikeTrainWindow::~CSpikeTrainWindow()
{
;
}


// ---------------------------------------------------------------------------------
//		¥ FinishCreateSelf
// ---------------------------------------------------------------------------------

void
CSpikeTrainWindow::FinishCreateSelf()
{
	mDefaultButtonID=5;
	// Link the window (the listener) with its controls (the broadcasters).
	UReanimator::LinkListenerToControls( this, this, 131 );
	CTempView *theTempView= dynamic_cast<CTempView *> (FindPaneByID( 2 ));
	CViewInfoPane1 *theInfoPane1;
	theInfoPane1 = dynamic_cast<CViewInfoPane1 *> (FindPaneByID( 5 ));
	ThrowIfNil_( theInfoPane1 );
				// Register the big view with the view info pane.
	theInfoPane1->RegisterView( theTempView );

    
}

// ---------------------------------------------------------------------------
//	¥ HandleKeyPress
// ---------------------------------------------------------------------------
//	DialogBox handles keyboard equivalents for hitting the Default and
//	Cancel Buttons.
//
//		Default Button: Enter, Return
//		Cancel Button:  Escape, Command-Period

Boolean
CSpikeTrainWindow::HandleKeyPress(
	const EventRecord	&inKeyEvent)
{
	Boolean		keyHandled	= false;
	LControl*	keyButton	= nil;
	UInt16		theChar		= (UInt16) (inKeyEvent.message & charCodeMask);
	
	if ( (theChar == char_Enter) || (theChar == char_Return) ) {
			keyButton = dynamic_cast<LControl*>(FindPaneByID(mDefaultButtonID));
			
	}/* else if ( UKeyFilters::IsEscapeKey(inKeyEvent) ||
				UKeyFilters::IsCmdPeriod(inKeyEvent) ) {
				
			keyButton = dynamic_cast<LControl*>(FindPaneByID(mCancelButtonID));
	}*/
			
	if (keyButton != nil) {
		keyButton->SimulateHotSpotClick(kControlButtonPart);
		keyHandled = true;
		
	} else {
		keyHandled = LWindow::HandleKeyPress(inKeyEvent);
	}
	
	return keyHandled;
}



// ---------------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------------


void
CSpikeTrainWindow::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
	CSpike6App::templrecrd Recd;
	Float64 val;
	SInt32 lastspike = CSpike6App::tRecord->GetCount();
	LStr255 Lstr;
	
	switch ( inMessage ) {


		case 7001://redraw the template view
			{
			CTempView *theTempView= dynamic_cast<CTempView *> (FindPaneByID( 2 ));
			theTempView->Draw(nil);
			break;
			}
		case 7002://reset the start and stopvalues and draw
			{
			LEditText *theCaption=dynamic_cast<LEditText *> (FindPaneByID(3));
			CSpike6App::tRecord->FetchItemAt(1,&Recd);
			val=Recd.timestamp; 
			Lstr.Assign(val,"\p######.###");
			theCaption->SetText(Lstr);
			CSpike6App::tRecord->FetchItemAt(lastspike,&Recd);
			val=Recd.timestamp;
			theCaption=dynamic_cast<LEditText *> (FindPaneByID(4));
			Lstr.Assign(val,"\p######.###");
			theCaption->SetText(Lstr);
			break;
			}

	}
}

