// =================================================================================
//	CMatrixWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CMatrixWindow.h	
#include "CSpike6App.h"
#include "SpikeConstants.h"

#include <UNavServicesDialogs.h>
#include "Analyze1.h"

#include <LString.h>
#include <UReanimator.h>
#include <Math.h>
#include <LPushButton.h>
#include <LPopupButton.h>
#include <LEditText.h>
#include <LMenu.h>
#include <LPopupButton.h>

#include "CMatrixTable.h"
#include "CMatrixWindow.h"
//#include "CBatchView.h"
//#include "SongUtils.h"

#include <gsl/gsl_cdf.h>


//#include <gsl/gsl_permutation.h>
SInt32 CMatrixWindow::typematrix=1;


// ---------------------------------------------------------------------------------
//		¥ CMatrixWindow
// ---------------------------------------------------------------------------------

CMatrixWindow::CMatrixWindow()
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CMatrixWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CMatrixWindow::CMatrixWindow(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CMatrixWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CMatrixWindow::CMatrixWindow(
	ResIDT		inWINDid,
	UInt32		inAttributes,
	LCommander	*inSuperCommander )
		: LWindow( inWINDid, inAttributes, inSuperCommander )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CMatrixWindow(LStream *)
// ---------------------------------------------------------------------------------

CMatrixWindow::CMatrixWindow(
	LStream *inStream )
		: LWindow( inStream )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CMatrixWindow
// ---------------------------------------------------------------------------------

CMatrixWindow::~CMatrixWindow()
{
;
}


// ---------------------------------------------------------------------------------
//		¥ CMatrixWindow
// ---------------------------------------------------------------------------------

void
CMatrixWindow::FinishCreateSelf()
{
	UReanimator::LinkListenerToControls( this, this, rRidL_MatrixWindow );	
	SetDefaultButton(2);
	typematrix = 1;
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
CMatrixWindow::HandleKeyPress(
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
// ---------------------------------------------------------------------------
//	¥ SetDefaultButton
// ---------------------------------------------------------------------------
//	Specify the PaneID of the default button of the window. The default
//	button must be derived from LControl.

void
CMatrixWindow::SetDefaultButton(
								  PaneIDT	inButtonID)
{
	LPushButton	*theButton = dynamic_cast<LPushButton*>(FindPaneByID(mDefaultButtonID));
	if (theButton!=nil){
		theButton->SetDefaultButton(FALSE);
	}
	
	mDefaultButtonID = inButtonID;	
	
	theButton = dynamic_cast<LPushButton*>(FindPaneByID(mDefaultButtonID));
	if (theButton != nil) {
		theButton->SetDefaultButton(TRUE);
	}
	// Create new default outline
}


// ---------------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------------



void
CMatrixWindow::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
	
	LStr255 Lstr;
	Ptr	thePtr;
	SInt16 mitem;
	switch ( inMessage ) {
		case 4202:{
			UInt8 writeflag=typematrix+4;
			Str63 mstr="\pVocalMatrix.txt";
			FSSpec OutFileSpec;
			bool replac =true;
			LFile *theOutFile;
			SInt32 datLength;
			if (UNavServicesDialogs::AskSaveFile(mstr,FOUR_CHAR_CODE('TEXT'),OutFileSpec,replac,
												 kNavDefaultNavDlogOptions + kNavSelectAllReadableItem)){
				theOutFile=new LFile(OutFileSpec);
				thePtr=WritePlotData(&datLength, writeflag);
				if (!(replac)){
					theOutFile->CreateNewDataFile(
												  FOUR_CHAR_CODE('atmD'),FOUR_CHAR_CODE('TEXT'),nil);
				}
				theOutFile->OpenDataFork(fsRdWrPerm);
				theOutFile->WriteDataFork(thePtr,datLength);
				theOutFile->CloseDataFork();
				DisposePtr(thePtr);
				delete theOutFile;
				theOutFile=nil;
			}		
			Str63 nstr="\pMatrixCounts.txt";
			if (UNavServicesDialogs::AskSaveFile(nstr,FOUR_CHAR_CODE('TEXT'),OutFileSpec,replac,
												 kNavDefaultNavDlogOptions + kNavSelectAllReadableItem)){
				theOutFile=new LFile(OutFileSpec);
				thePtr=WritePlotData(&datLength, 9);
				if (!(replac)){
					theOutFile->CreateNewDataFile(
												  FOUR_CHAR_CODE('atmD'),FOUR_CHAR_CODE('TEXT'),nil);
				}
				theOutFile->OpenDataFork(fsRdWrPerm);
				theOutFile->WriteDataFork(thePtr,datLength);
				theOutFile->CloseDataFork();
				DisposePtr(thePtr);
				delete theOutFile;
				theOutFile=nil;
			}	
		break;
		}
			
		case 4203:{
			LPopupButton *thePopupMenu = dynamic_cast<LPopupButton*>(FindPaneByID(3));
			mitem=thePopupMenu->GetValue();
			if (mitem == 1){
				typematrix= 1; //all
			}
			else if (mitem == 2){
				typematrix = 2; //left				
			}
			else if (mitem == 3){
				typematrix = 3; //right
			}
			else if (mitem == 4){
				typematrix = 4;
			}
			/*else if (mitem == 5){
				typematrix = 5;
			}*/
			CMatrixTable *theMatrixTable = dynamic_cast<CMatrixTable *> (FindPaneByID(1));
			ThrowIfNil_(theMatrixTable);
			theMatrixTable->Draw(nil);
			break;
		}
	}
}

