// =================================================================================
//	CProgressWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CProgressWindow.h	

#include <LString.h>
#include <UReanimator.h>
#include <UWindows.h>
#include <UCursor.h>
#include <LArray.h>
#include <String.h>
#include <Math.h>
//#include <LActiveScroller.h>
//#include <LCaption.h>
#include <LDialogBox.h>
#include <LEditText.h>
//#include <LGroupBox.h>
#include <LListBox.h>
#include <LScroller.h>
#include <LStdControl.h>
#include <LPushButton.h>
#include <LPopUpButton.h>
#include <LTable.h>
#include <LTextButton.h>
#include <LTextEditView.h>
#include <LWindow.h>
#include <LString.h>
#include <LStaticText.h>
#include <LPane.h>
#include <UNavServicesDialogs.h>
#include <TextUtils.h>

#include "SpikeConstants.h"
#include "CSettingsWindow.h"
#include "CSpike6App.h"

#include "CBig1.h"
#include "CViewInfoPane.h"
#include "CViewInfoPane1.h"
#include "CViewInfoPane2.h"
#include "CSpikeTrainWindow.h"
#include "CCombEvents.h"
#include "CMatrixWindow.h"



#include "CAvgTable.h"
#include "CTimView.h"
#include "CWavPlot.h"
#include "CTempView.h"

#include "Analyze1.h"
#include "Analyze2.h"

#include "SwapUtils.h"

//SInt32 nSpikes;
Boolean	CSettingsWindow::mHasChanged;
Boolean CSettingsWindow::DrawDots;
Boolean CSettingsWindow::DrawLines;
Boolean CSettingsWindow::ZeroYOrigin;
Boolean CSettingsWindow::ZeroXOrigin;
Float64 CSettingsWindow::uplim=-10.0;//1% confidence limits
Float64 CSettingsWindow::lolim=-10.0;
Float64 CSettingsWindow::uplim05=-10.0;//5% confidence limits
Float64 CSettingsWindow::lolim05=-10.0;


Boolean CSettingsWindow::TimeCourseFlag=FALSE;
Boolean CSettingsWindow::HistTimeCourseFlag=FALSE;
UInt8 CSettingsWindow::ExportMeanSD=0;
LArray *CSettingsWindow::DrawCol1;
LArray *CSettingsWindow::DrawCol2;
LArray *CSettingsWindow::DrawCol3;//3 and 4 are the arrays having
LArray *CSettingsWindow::DrawCol4;//4 is y
LArray *CSettingsWindow::DrawCol5;
LArray *CSettingsWindow::DrawCol6;
LArray *CSettingsWindow::DrawCol7;
LArray *CSettingsWindow::DrawCol8;
LArray *CSettingsWindow::DrawCol9;


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow
// ---------------------------------------------------------------------------------

CSettingsWindow::CSettingsWindow()
{
;//TimeCourseFlag=;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CSettingsWindow::CSettingsWindow(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo )
{
;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CSettingsWindow::CSettingsWindow(
	ResIDT		inWINDid,
	UInt32		inAttributes,
	LCommander	*inSuperCommander )
		: LWindow( inWINDid, inAttributes, inSuperCommander )
{
;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(LStream *)
// ---------------------------------------------------------------------------------

CSettingsWindow::CSettingsWindow(
	LStream *inStream )
		: LWindow( inStream )
{
;
}


// ---------------------------------------------------------------------------------
//		¥ ~CProgressWindow
// ---------------------------------------------------------------------------------

CSettingsWindow::~CSettingsWindow()
{
//mark snTemplates as zero;
//CSpike6App::snTemplates=0;
	delete DrawCol1;
	DrawCol1 =nil;
	delete DrawCol2;
	DrawCol2 =nil;
	delete DrawCol3;
	DrawCol3 =nil;
	delete DrawCol4;
	DrawCol4 =nil;
	delete DrawCol5;
	DrawCol5 =nil;
	delete DrawCol6;
	DrawCol6 =nil;
	delete DrawCol7;
	DrawCol7 =nil;
	delete DrawCol8;
	DrawCol8 =nil;
	delete DrawCol9;
	DrawCol9 =nil;

}


// ---------------------------------------------------------------------------------
//		¥ FinishCreateSelf
// ---------------------------------------------------------------------------------

void
CSettingsWindow::FinishCreateSelf()
{
	Float64 temp=0.0;
	CSpike6App::templrecrd theTRec;
	LPaintAttachment *theAttach;
	//RGBColor color_Black = {0,0,0};
	theAttach = new LPaintAttachment(nil,nil,nil,true);
	AddAttachment(theAttach, nil, true);
	/*CCombEvents *theEventsWindow = dynamic_cast<CCombEvents *>(LWindow::FindWindowByID(rPPob_Combination));
	if (theEventsWindow !=nil)
	{
		//delete theEventsWindow;
		theEventsWindow=nil;
	}*/
	//MakeRawStats();
	// Link the window (the listener) with its controls (the broadcasters).
	UReanimator::LinkListenerToControls( this, this, rRidL_SettingWindow );
	SetDefaultButton(kRedoButt);
	SetDescriptorForPaneID(kFileCaption, CSpike6App::sFileName);
	CSpike6App::tRecord->FetchItemAt(1, &theTRec);//get first timestamp
	temp=theTRec.timestamp;
	LStr255	theTitle(temp,"\p#########.#####");
	SetDescriptorForPaneID(kStrt,theTitle);
	SInt32 n1=CSpike6App::tRecord->GetCount();
	CSpike6App::tRecord->FetchItemAt(n1,&theTRec);//get last timestamp
	temp=theTRec.timestamp;
	theTitle.Assign(temp,"\p#########.#####");
	SetDescriptorForPaneID( kEnd,theTitle);
	CSpike6App::SpikeParams.maxbns=100;
	DrawLines=TRUE;
	DrawDots=FALSE;
	ZeroXOrigin=FALSE;
	ZeroYOrigin=FALSE;
	mHasChanged=FALSE;
	mLastButton=nil;
	if (CSpike6App::SpikeParams.BinAvailable){
		LControl	*theButton = dynamic_cast<LControl*>(FindPaneByID(kWaveViewButton));
		theButton->Enable();
		;//enable showwaveformbutton
		}
	//ApplyForeAndBackColors();
	
}



// ---------------------------------------------------------------------------
//	¥ SetDefaultButton
// ---------------------------------------------------------------------------
//	Specify the PaneID of the default button of the window. The default
//	button must be derived from LControl.

void
CSettingsWindow::SetDefaultButton(
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
//		¥ FindCommandStatus
// ---------------------------------------------------------------------------------

void		
CSettingsWindow::FindCommandStatus(
								CommandT			inCommand,
								Boolean				&outEnabled,
								Boolean				&outUsesMark,
								UInt16				&outMark,
								Str255				outName)
{
switch (inCommand) {
		case 2001:
		case 2002:
        case 3037:
		outEnabled = true;
		break;

		default:
			PP_PowerPlant::LWindow::FindCommandStatus(inCommand, outEnabled,
												outUsesMark, outMark, outName);
		break;
		}
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
CSettingsWindow::HandleKeyPress(
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
CSettingsWindow::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
		
	LStr255 Lstr;
	Boolean isok, allspikes=FALSE;
	TimeCourseFlag=FALSE;
	SInt32 myer;
	CViewInfoPane 	*theInfoPane;
    CViewInfoPane2 	*theInfoPane2;
	CBig1		*theBigView;
	CSpikeTrainWindow *theTrainWindow = nil;
	FSSpec OutFileSpec;
	Ptr	thePtr;
	LFile *theOutFile;
	Str63 mstr="\pmyFile.txt";
	bool replac =true;
	UInt8 writeflag=1;
	SInt32 datLength;


switch ( inMessage ) 
	{
	case 3002:	//make cumulative plot (one template only)
		{			
		UCursor::SetWatch();
		TimeCourseFlag=FALSE;
		PrepareArrays();		
		GetSettings();
		isok=FindLinesA(&myer);
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			SInt32 cnt,cnt1;
			cnt=::TickCount();
			MakeCumPlot();
			cnt1=::TickCount();
			cnt=cnt1-cnt;
			DrawDots=FALSE;
			DrawLines=TRUE;
			ZeroXOrigin=FALSE;
			ZeroYOrigin=FALSE;
			theBigView = dynamic_cast<CBig1 *> (FindPaneByID( kDrawing1 ));
			ThrowIfNil_( theBigView );
			theBigView->Draw(nil);
			// Get the view info pane.
			theInfoPane = dynamic_cast<CViewInfoPane *> (FindPaneByID( kViewInfoPane1 ));
			ThrowIfNil_( theInfoPane );
			// Register the big view with the view info pane.
			theInfoPane->RegisterView( theBigView );
			}
		/*LStdButton *mypaneptr=dynamic_cast<LStdButton *> (FindPaneByID( kCumButt ));
		mypaneptr->SimulateHotSpotClick(1);*/
		MakeTimeCourse();
		mLastButton=kCumButt;
		}
	break;
	
	case 3003:	
	/*Make Interval histogram. Also for 2 templates, then intervals are 1-2 intervals,
	1 being the source template and 2 the target*/
		{
		UCursor::SetWatch();
		HistTimeCourseFlag=TRUE;
		TimeCourseFlag=FALSE;
		PrepareArrays();		
		GetSettings();		
		isok=FindLinesA(&myer);
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			SInt32 cnt,cnt1;
			cnt=::TickCount();
			MakeIntvHisto();
			cnt1=::TickCount();
			cnt=cnt1-cnt;
			DrawDots=FALSE;
			DrawLines=TRUE;
			ZeroXOrigin=FALSE;
			ZeroYOrigin=FALSE;
			theBigView = dynamic_cast<CBig1 *> (FindPaneByID( kDrawing1 ));
			ThrowIfNil_( theBigView );
			theBigView->Draw(nil);
            // Get the view info pane.
            theInfoPane = dynamic_cast<CViewInfoPane *> (FindPaneByID( kViewInfoPane1 ));
            ThrowIfNil_( theInfoPane );
            // Register the big view with the view info pane.
            theInfoPane->RegisterView( theBigView );
            //MakeTimeCourse();
            CTimView *theTimView = dynamic_cast<CTimView *>(FindPaneByID( kTimView ));
            ThrowIfNil_( theTimView );
            theTimView->Draw(nil);
            theInfoPane2 = dynamic_cast<CViewInfoPane2 *> (FindPaneByID( 28 ));
            ThrowIfNil_( theInfoPane );
            // Register the big view with the view info pane.
            theInfoPane2->RegisterView( theTimView );

			}
		
		mLastButton=kHistButt;
		}
	break;
	
	case 3004:
	/*Make All point density histogram. Also for 2 templates, then intervals 
	are 1-2 intervals, 1 being the source template and 2 the target*/
		{
		UCursor::SetWatch();
		HistTimeCourseFlag=FALSE;
		TimeCourseFlag=TRUE;
		PrepareArrays();
		GetSettings();		
		isok=FindLinesA(&myer);		
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			AllDensityGSL(1);
			DrawDots=FALSE;
			DrawLines=TRUE;
			ZeroXOrigin=FALSE;
			ZeroYOrigin=TRUE;
			theBigView = dynamic_cast<CBig1 *> (FindPaneByID( kDrawing1 ));
			ThrowIfNil_( theBigView );
			theBigView->Draw(nil);
			// Get the view info pane.
			theInfoPane = dynamic_cast<CViewInfoPane *> (FindPaneByID( kViewInfoPane1 ));
			ThrowIfNil_( theInfoPane );
			// Register the big view with the view info pane.
			theInfoPane->RegisterView( theBigView );
			MakeTimeCourse();

			}
		mLastButton=kDensButt;
		}
	break;
	
	case 3005:	//Events by time. One template only
		{
		UCursor::SetWatch();
		PrepareArrays();
		GetSettings();		
		isok=FindLinesA(&myer);		
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			EventsXTime();
			DrawDots=TRUE;
			DrawLines=FALSE;
			ZeroXOrigin=FALSE;
			ZeroYOrigin=FALSE;
			theBigView = dynamic_cast<CBig1 *> (FindPaneByID( kDrawing1 ));
			ThrowIfNil_( theBigView );
			theBigView->Draw(nil);
			// Get the view info pane.
			theInfoPane = dynamic_cast<CViewInfoPane *> (FindPaneByID( kViewInfoPane1 ));
			ThrowIfNil_( theInfoPane );
			// Register the big view with the view info pane.
			theInfoPane->RegisterView( theBigView );
			}
		//MakeTimeCourse();
		mLastButton=kEvtButt;
		}
	break;
	
	case 3006:
		{
		LumpUnits();
		}
	break;
	
	case 3007: //make probability plot
		{
		UCursor::SetWatch();
		LPopupButton	*thePopup=(LPopupButton	*)FindPaneByID(kSetDistrPopup );
		Assert_( thePopup != nil );

		Str255	theMenuText;
		::GetMenuItemText( thePopup->GetMacMenuH(),
		thePopup->GetValue(), theMenuText );

		// Get the size referred to by the menu item.
		char itis[255];
		char *mp;
		mp=(char*)theMenuText;
		
		itis[1]=*(mp+1);
		UInt32 DistriFlag=0;
		if (itis[1]=='n'){DistriFlag=1;}
		else if (itis[1]=='e'){DistriFlag=2;}
		else if (itis[1]=='l'){DistriFlag=4;}
		else {DistriFlag=0;}

		PrepareArrays();
		GetSettings();		
		isok=FindLinesA(&myer);
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			ProbPlot(DistriFlag);
			Float64 theStat=RunKSStatProb();
			LStr255 str(theStat,"\p####.#####");
			SetDescriptorForPaneID(kKSStat, str);
			DrawDots=TRUE;
			DrawLines=FALSE;
			ZeroXOrigin=FALSE;
			ZeroYOrigin=FALSE;
			theBigView = dynamic_cast<CBig1 *> (FindPaneByID( kDrawing1 ));
			ThrowIfNil_( theBigView );
			theBigView->Draw(nil);
			// Get the view info pane.
			theInfoPane = dynamic_cast<CViewInfoPane *> (FindPaneByID( kViewInfoPane1 ));
			ThrowIfNil_( theInfoPane );
			// Register the big view with the view info pane.
			theInfoPane->RegisterView( theBigView );
			}
		MakeTimeCourse();
		mLastButton=kProbButt;
		}
	break;
	
	case 3008://make histogram using ALL templates (to determine acquisition dead time)
		{
			UCursor::SetWatch();
			PrepareArrays();	
			GetSettings();
			MakeAllTemplatesHisto();
			DrawDots=FALSE;
			DrawLines=TRUE;
			ZeroXOrigin=FALSE;
			ZeroYOrigin=FALSE;
			theBigView = dynamic_cast<CBig1 *> (FindPaneByID( kDrawing1 ));
			ThrowIfNil_( theBigView );
			theBigView->Draw(nil);
			// Get the view info pane.
			theInfoPane = dynamic_cast<CViewInfoPane *> (FindPaneByID( kViewInfoPane1 ));
			ThrowIfNil_( theInfoPane );
			// Register the big view with the view info pane.
			theInfoPane->RegisterView( theBigView );
			mLastButton=kAllTemplHist;
		}
	break;
	
	case 3009://summarize all crosscorrelations between clusters
		{
			delete DrawCol5;
			DrawCol5 =nil;
			DrawCol5 = new LArray(sizeof(Float64));
			ThrowIfNil_(DrawCol5);
			delete DrawCol6;
			DrawCol6 =nil;
			DrawCol6 = new LArray(sizeof(Float64));
			ThrowIfNil_(DrawCol6);
			delete DrawCol7;
			DrawCol7 =nil;
			DrawCol7 = new LArray(sizeof(Float64));
			ThrowIfNil_(DrawCol7);
			delete DrawCol8;
			DrawCol8 =nil;
			DrawCol8 = new LArray(sizeof(Float64));
			ThrowIfNil_(DrawCol8);
			delete DrawCol9;
			DrawCol9 =nil;
			DrawCol9 = new LArray(sizeof(Float64));
			ThrowIfNil_(DrawCol9);
			
			GetSettings();
            allspikes=FALSE;
			AllAllDensity();
			CMatrixWindow *theMatrixWindow = nil;
			theMatrixWindow = dynamic_cast<CMatrixWindow *>(LWindow::CreateWindow(rPPob_MatrixWindow, this));
			ThrowIfNil_( theMatrixWindow );
			theMatrixWindow->Show();
		}
	break;
    case 3037://summarize all possible PSTHs between clusters
        {
            
			GetSettings();
            SInt16 ntemplates=CSpike6App::snTemplates, nbins=CSpike6App::SpikeParams.maxbns;
            SInt32 i=0, arsze=0.5*(ntemplates*ntemplates-ntemplates)+1;
            Float64 binwidth=CSpike6App::SpikeParams.BWdth;
            SInt32 **difarr=AllPSTH(ntemplates,binwidth,nbins);
            LStr255 mstr;
            mstr.Assign("\pmyarrayA.txt");
			Write2DArray(difarr, arsze,2*nbins+2,&mstr);
            
        }
    break;
	
                         case 3010:
		{
		if (mLastButton != 0)
			{
			LPushButton *mypaneptr=dynamic_cast<LPushButton *> (FindPaneByID(mLastButton));
			mypaneptr->SimulateHotSpotClick(1);
			}
		}
	break;
	
	case 3011://plot waveforms
		{
		GetSettings();
		isok=FindLinesA(&myer);
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			if  (CSpike6App::SpikeParams.templ2==0) {
				CSpike6App::SpikeParams.templ2=CSpike6App::SpikeParams.templ1;
				}
			LWindow *theWaveWindow=nil;
			theWaveWindow =dynamic_cast<LWindow *>(LWindow::FindWindowByID(rPPob_WaveForm1));
			if (CSpike6App::SpikeParams.BinAvailable){
				delete CSpike6App::sWavs;
				CSpike6App::sWavs=nil;
				CSpike6App::sWavs=new LArray(sizeof(SInt32));
				ThrowIfNil_(CSpike6App::sWavs);
				FindOccurs();
				if (theWaveWindow != nil)
					{
					theWaveWindow->Select();
					theWaveWindow->Draw(nil);
					}
				else
					{
					theWaveWindow = LWindow::CreateWindow( rPPob_WaveForm1, this );
					ThrowIfNil_( theWaveWindow );
					theWaveWindow->Show();
					}
				}
			}
	break;
	}

	case 3012: //show the firing patterns
		{	
		GetSettings();
		isok=FindLinesA(&myer);
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			theTrainWindow=dynamic_cast<CSpikeTrainWindow *>(LWindow::FindWindowByID(rPPob_SpikeTrainWindow));
			if (theTrainWindow !=nil)
				{
				theTrainWindow->Select();
				}
			else
				{
				theTrainWindow=dynamic_cast<CSpikeTrainWindow *>(LWindow::CreateWindow(rPPob_SpikeTrainWindow,this));
				ThrowIfNil_( theTrainWindow );
				theTrainWindow->Show();
				}
			}
		break;
		}
		
	case 3013://export the data used for plot
		{
		writeflag=1;
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
			}		
		DisposePtr(thePtr);
		delete theOutFile;
		theOutFile=nil;
		break;
		}
		
		
    case 3014:
	/*Make cumulative density plot. For 2 templates*/
		{
		TimeCourseFlag=true;
		PrepareArrays();
		GetSettings();		
		isok=FindLinesA(&myer);		
		if (isok==FALSE)//usually some adjustments to values are necessary
			{
			HandleError(myer);
			ResetSettings();
			}
		else
			{
			//AllDensity();
			CumPSTH(1);
			DrawDots=FALSE;
			DrawLines=TRUE;
			ZeroXOrigin=FALSE;
			ZeroYOrigin=FALSE;
			theBigView = dynamic_cast<CBig1 *> (FindPaneByID( kDrawing1 ));
			ThrowIfNil_( theBigView );
			theBigView->Draw(nil);
			// Get the view info pane.
			theInfoPane = dynamic_cast<CViewInfoPane *> (FindPaneByID( kViewInfoPane1 ));
			ThrowIfNil_( theInfoPane );
			// Register the big view with the view info pane.
			theInfoPane->RegisterView( theBigView );
			MakeTimeCourse();
			}
		mLastButton=kCSumButt;
        break;
		}
	
    case 3031://export the data of the rasterplot (TimView)
		{
            if (ExportMeanSD==0){
                writeflag=2;
            }
            else if(ExportMeanSD==1){
                writeflag = 10;
            }
            else if(ExportMeanSD==2){
                writeflag = 11;
            }
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
            
            break;
		}
            
    case 3035://export the data of the rasterplot (TimView)
		{
        if ( *(SInt32 *) ioParam == Button_On ) 
            {
                ExportMeanSD = 0;
            }
        break;
        }
    case 3034://export the data of the rasterplot (TimView)
		{
            if ( *(SInt32 *) ioParam == Button_On ) 
            {
                ExportMeanSD = 1;
            }
            break;
        }
    case 3036://export the data of the rasterplot (TimView)
		{
            if ( *(SInt32 *) ioParam == Button_On ) 
            {
                ExportMeanSD = 2;
            }
            break;
        }
	}
}


	
//
// prepare arrays
//

void
CSettingsWindow::PrepareArrays()
	{
	delete DrawCol1;
	DrawCol1 =nil;
	DrawCol1 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol1);
	delete DrawCol2;
	DrawCol2 =nil;
	DrawCol2 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol2);
	delete DrawCol3;
	DrawCol3 =nil;
	DrawCol3 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol3);
	delete DrawCol4;
	DrawCol4 =nil;
	DrawCol4 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol4);
	delete DrawCol5;
	DrawCol5 =nil;
	DrawCol5 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol5);
	delete DrawCol6;
	DrawCol6 =nil;
	DrawCol6 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol6);
	delete DrawCol7;
	DrawCol7 =nil;
	DrawCol7 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol7);
	delete DrawCol8;
	DrawCol8 =nil;
	DrawCol8 = new LArray(sizeof(Float64));
	ThrowIfNil_(DrawCol8);
		
	CSettingsWindow::uplim=-10.0;//1% confidence limits
	CSettingsWindow::lolim=-10.0;

	}
// ---------------------------------------------------------------------------------
//		¥ RenumberTemplates
// ---------------------------------------------------------------------------------

void
CSettingsWindow::RenumberTemplates(SInt32 ttempl2)//ttempl2 is the one that was removed.
	{
	CSpike6App::templrecrd theTRec;
	SInt32 j, nlines;
	nlines=CSpike6App::tRecord->GetCount();
	for (j=1; j<=nlines; j++){
		CSpike6App::tRecord->FetchItemAt(j,&theTRec);
		if (theTRec.templatenum>ttempl2){
			theTRec.templatenum-=1;
			CSpike6App::tRecord->AssignItemsAt(1,j,&theTRec);
			}
		}
	CSpike6App::snTemplates-=1;
	}	
// ---------------------------------------------------------------------------------
//		¥ LumpUnits
// ---------------------------------------------------------------------------------

void
CSettingsWindow::LumpUnits()
	{
	Str255 str;
	SInt32 ttempl1, ttempl2;
	TableIndexT rows,cols;
	CSpike6App::templrecrd theTRec;
	GetDescriptorForPaneID(kTempl1, str);
	StringToNum(str,&ttempl1);
	GetDescriptorForPaneID(kTempl2, str);
	StringToNum(str,&ttempl2);
	if (( ttempl1 != 0 ) && (ttempl2 !=0 ))
		{
		SInt32 i, n1;
		n1 = CSpike6App::tRecord->GetCount();
		//LArray *WorkArray = new LArray(sizeof(Float64));
		for (i=1; i<=n1; i++)
			{
			CSpike6App::tRecord->FetchItemAt(i, &theTRec);
			if (theTRec.templatenum==ttempl2)
				{
			theTRec.templatenum=ttempl1;
			CSpike6App::tRecord->AssignItemsAt(1,i,&theTRec);
				}
			}
		RenumberTemplates(ttempl2);
		UpdateRawStats();
		mHasChanged=TRUE;
		//delete WorkArray;
		}
	CAvgTable *theTable=dynamic_cast<CAvgTable *>(FindPaneByID(kAvgTable));
	theTable->GetTableSize(rows,cols);
	rows-=1;
	theTable->RemoveRows(1,rows);
	theTable->Draw(nil);
	}


// ---------------------------------------------------------------------------------
//		¥ UpdateRawStats
// ---------------------------------------------------------------------------------
	
void
CSettingsWindow::UpdateRawStats() // After lumping the averages etc need to be updated.
	{
	SInt32 i,j,vall, noccur,ttemp,temp;
	delete CSpike6App::sAvgCol1;
	CSpike6App::sAvgCol1=nil;
	delete CSpike6App::sAvgCol2;
	CSpike6App::sAvgCol2=nil;
	delete CSpike6App::sAvgCol3;
	CSpike6App::sAvgCol3=nil;
	delete CSpike6App::sAvgCol4;
	CSpike6App::sAvgCol4=nil;
	CSpike6App::sAvgCol1 = new LArray(sizeof(SInt32));	//template #
	ThrowIfNil_(CSpike6App::sAvgCol1);
	CSpike6App::sAvgCol2 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol2);					//occurrences
	CSpike6App::sAvgCol3 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol3);					//plussize
	CSpike6App::sAvgCol4 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol4);					//minsize

	j=0;		
	for (i=1; i<=600; i++){//maximum number of templates is 601!
		CSpike6App::sAvgCol1->AddItem(&i,sizeof(SInt32));
		CSpike6App::sAvgCol2->AddItem(&j,sizeof(SInt32));
		CSpike6App::sAvgCol3->AddItem(&j,sizeof(SInt32));
		CSpike6App::sAvgCol4->AddItem(&j,sizeof(SInt32));
		}			
	CSpike6App::templrecrd theTRec;
	//look for highest template number this yields AvDim3
	noccur=CSpike6App::tRecord->GetCount();
	for (i=1; i<=noccur; i++){//accumulate the totals
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		CSpike6App::sAvgCol2->FetchItemAt(theTRec.templatenum,&vall);
		vall+=1;
		CSpike6App::sAvgCol2->AssignItemsAt(1,theTRec.templatenum,&vall);
		CSpike6App::sAvgCol3->FetchItemAt(theTRec.templatenum,&vall);
		vall+=theTRec.plusval;
		CSpike6App::sAvgCol3->AssignItemsAt(1,theTRec.templatenum,&vall);
		CSpike6App::sAvgCol4->FetchItemAt(theTRec.templatenum,&vall);
		vall+=1;
		CSpike6App::sAvgCol4->AssignItemsAt(1,theTRec.templatenum,&vall);
		}
	for (i=1;i<=128;i++){//find out how many templates there are
		CSpike6App::sAvgCol2->FetchItemAt(i,&vall);
		if (vall==0){
			break;
			}
		}
	CSpike6App::sAvgCol1->RemoveItemsAt(i,128);//truncates the array to where it contains useful data.
	CSpike6App::sAvgCol2->RemoveItemsAt(i,128);
	CSpike6App::sAvgCol3->RemoveItemsAt(i,128);
	CSpike6App::sAvgCol4->RemoveItemsAt(i,128);
	ttemp=CSpike6App::sAvgCol1->GetCount();
	for(i=1;i<=ttemp; i++){
		CSpike6App::sAvgCol2->FetchItemAt(i,&temp);
		CSpike6App::sAvgCol3->FetchItemAt(i,&vall);
		vall=round((Float64)vall/(Float64)temp);
		CSpike6App::sAvgCol3->AssignItemsAt(1,i,&vall);
		CSpike6App::sAvgCol4->FetchItemAt(i,&vall);
		vall=round((Float64)vall/(Float64)temp);
		CSpike6App::sAvgCol4->AssignItemsAt(1,i,&vall);
		}
	CSpike6App::sAvDim2=4;
	CSpike6App::sAvDim3=CSpike6App::sAvgCol1->GetCount();
	}



// ---------------------------------------------------------------------------------
//		¥ ResetSettings
// ---------------------------------------------------------------------------------

	
void CSettingsWindow::ResetSettings()
	{
	LStr255 Lstr;
	Lstr.Assign(CSpike6App::SpikeParams.templ1);
	SetDescriptorForPaneID(kTempl1, Lstr);
	Lstr.Assign(CSpike6App::SpikeParams.templ2);
	SetDescriptorForPaneID(kTempl2, Lstr);
	Lstr.Assign(CSpike6App::SpikeParams.strt,"\p##########.#####");
	SetDescriptorForPaneID(kStrt, Lstr);
	Lstr.Assign(CSpike6App::SpikeParams.stp,"\p##########.#####");
	SetDescriptorForPaneID(kEnd, Lstr);
	Lstr.Assign(CSpike6App::SpikeParams.maxbns);
	SetDescriptorForPaneID(kNBINS, Lstr);
	}
	
	
// ---------------------------------------------------------------------------
//	¥ MakeTimeCourse													  [public]
// ---------------------------------------------------------------------------
	void
	CSettingsWindow::MakeTimeCourse()
	{
	CTimView *theTimView = dynamic_cast<CTimView *>(FindPaneByID( kTimView ));
	ThrowIfNil_( theTimView );
	theTimView->Draw(nil);
	CViewInfoPane2 	*theInfoPane = dynamic_cast<CViewInfoPane2 *> (FindPaneByID( kViewInfoPane2 ));
	ThrowIfNil_( theInfoPane );
	// Register the big view with the view info pane.
	theInfoPane->RegisterView( theTimView );

	}
	
// ---------------------------------------------------------------------------------
//		¥ GetSettings
// ---------------------------------------------------------------------------------


void
	CSettingsWindow::GetSettings()
	{
	Str255 str;
	LStr255 Lstr;
	//LStr255 Lstr;
	GetDescriptorForPaneID(kTempl1, str);
	StringToNum(str,&CSpike6App::SpikeParams.templ1);
	GetDescriptorForPaneID(kTempl2, str);
	StringToNum(str,&CSpike6App::SpikeParams.templ2);
	GetDescriptorForPaneID(kStrt, Lstr);
	Float64 myFloat64=FloatFromString(&Lstr);
	CSpike6App::SpikeParams.strt=myFloat64;
	GetDescriptorForPaneID(kEnd, Lstr);
	myFloat64=FloatFromString(&Lstr);
	CSpike6App::SpikeParams.stp=myFloat64;
	GetDescriptorForPaneID(kBWidth, Lstr);
	myFloat64=FloatFromString(&Lstr);
	CSpike6App::SpikeParams.BWdth=myFloat64;
	GetDescriptorForPaneID(kLag, Lstr);
	myFloat64=FloatFromString(&Lstr);
	CSpike6App::SpikeParams.ExpoLag=myFloat64;
	GetDescriptorForPaneID(kNBINS, Lstr);
	SInt16 myInt=(SInt16)Lstr.operator SInt32();
	//myFloat64=FloatFromString(&Lstr);
	CSpike6App::SpikeParams.maxbns=myInt;
	}
// ---------------------------------------------------------------------------
//	HandleError(ernum)
// ---------------------------------------------------------------------------

void
CSettingsWindow::HandleError(SInt32 errnum){
	//theDialog->Show();
	
	LStr255 mystring;
	if (errnum==101){
		mystring.Assign("\pTemplate number does not occur in this file;\rit is set to 0");
		}
	else if (errnum==102){
		mystring="\ptemplate 1 was not specified.\rtemplate2 was specified.\rtemplate1 is set to template2";
		}
	else if (errnum==103){
		mystring="\ptemplate 1 was not specified.\r It is set to 0";
		}
	else if (errnum==104){
		mystring="\pStop value too high. \rSet at end of file";
		}
	else if (errnum==105){
		mystring="\pStart value too low.\rSet at start of file";
		}
	else if (errnum==106){
		mystring="\pbreakpoint out of bounds\rSet at zero";
		}
	else if (errnum==107){
		mystring="\pno occurrence in designated period(s)";
		}
	else if (errnum==108){
		mystring="\ptemplate does not occur";
		}
	else {
		mystring="\punspecified error";
		}
	StDialogHandler	theHandler(rPPob_1btnAlert, this);
	LWindow*		theDialog = theHandler.GetDialog();
	Assert_(theDialog != nil);
	theDialog->Show();
	MessageT theMessage;
	LStaticText	*theCaption;
	Rect frame;
	theCaption = dynamic_cast<LStaticText*>(theDialog->FindPaneByID(1));
	theCaption->SetText(mystring);
	theCaption->FocusDraw();
	theCaption->CalcLocalFrameRect(frame);
	theCaption->ApplyForeAndBackColors();
	::EraseRect(&frame);
	theCaption->Draw(nil);
	do {
		theMessage = theHandler.DoDialog();
		} while ( theMessage != msg_OK && theMessage != msg_Cancel );
}
// ---------------------------------------------------------------------------
//	Write2DArray
// ---------------------------------------------------------------------------
//	Writes Drawcol1 and drawcol 2 into ASCII file for plotting
void
CSettingsWindow::Write2DArray(SInt32 **theArray, UInt32 sze1, UInt32 sze2, LStr255 *inString){//writes 2d array
	
	char *cstr;
	LStr255 mStr; 
	UInt32 length;
	LArray *theCharray = new LArray(sizeof(char));
	UInt32 i,j,k,m,maxline1=sze1,maxline2=sze2;
	static char tabchar[]="\t";
	static char returnchar[]="\n";
	SInt32 myvar;
	m=0;
	for(i=0;i<maxline2;i++){
		for (j=0; j<maxline1;j++){
			myvar = theArray[j][i];
			mStr.Assign(myvar);
			cstr= mStr.PToCStr(mStr);
			if(j==maxline1-1){
				cstr=strcat(cstr,returnchar);
			} else {
				cstr=strcat(cstr,tabchar);
			}
			length=strlen(const_cast <char *>(cstr));
			for (k=0;k<=length-1;k++){
				theCharray->AddItem(&cstr[k]);
				m+=1;
			}
		}
	}
	SaveArray(theCharray, inString);
}

// ---------------------------------------------------------------------------
//	SaveArray
// ---------------------------------------------------------------------------

void		
CSettingsWindow::SaveArray(LArray *inArray, LStr255 *inString){
	SInt32 count;
	if (inArray != nil){
		count = inArray->GetCount();
		if ( count > 0 ){
			ConstStringPtr OutFileName;
			OutFileName=inString->operator ConstStringPtr();
			bool replac = false;
			FSSpec OutFileSpec;
			if (UNavServicesDialogs::AskSaveFile(OutFileName,FOUR_CHAR_CODE('data'),OutFileSpec,replac,
                                                 kNavDefaultNavDlogOptions+ kNavSelectAllReadableItem )){//+ kNavSelectAllReadableItem
				LFile *theOutFile=new LFile(OutFileSpec);
				Handle mymacbs2handle=inArray->GetItemsHandle();
				SInt32 datLength=GetHandleSize(mymacbs2handle);
				char *thePtr=*mymacbs2handle;
				if (!(replac)){
					OSType		OutCreator=FOUR_CHAR_CODE('atmt');
					theOutFile->CreateNewDataFile(OutCreator, FOUR_CHAR_CODE('data'), nil);
                }
				theOutFile->OpenDataFork(fsRdWrPerm);
				theOutFile->WriteDataFork(thePtr,datLength);
				theOutFile->CloseDataFork();
            }
        }
    }
}
