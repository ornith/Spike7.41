// =================================================================================
//	CProgressWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CProgressWindow.h	
/*
message numbers of CStatsWindow

message	ID	action

5001	17	KRUSKAL_WALLIS statistic
5002	18	Kolmogorov-Smirnoff statistic
5003	15	Permutation test
5004	32	remove AR or MA
5005	31	save data
5006		-----unused-----
5007	14	Do breakpoint Analysis. Currently this only works well with 1 template only in sSpikeCol
5008	43	Set number of runs
5009	44	Set Desired P-value
5010	16	make ACF-plot
5011		-----unused-----
5012	20	make Return plot
5013	21	make Ranked Return plot
5014	22	make 3-i-Return plot
5015	42	respond to STOP-command. only for permutation tests
5016	40	do Histogram-analysis
5017	41	do cumPSTH-analysis. Randomization test
5018		-----unused-----
5019		-----unused-----
5020	24	REDO last analysis
5021	28	make averages, stadard deviations and medians
*/
/*
pane ID's of CStatsWindow

ID	PaneDesc
1	Lscrollerview
2	LTable
3	LEditText
4	LEditText
5	LEditText
6	LEditText
7	LEditText
8	LEditText
9	LEditText
10	LStaticText
11	LEditField
12	Big3
13	LTable
14	LPushButton
15	LPushButton
16	LPushButton
17	LPushButton
18	LPushButton
19	LPopupButton
20	LPushButton
21	LPushButton
22	LPushButton
23	LProgressBar
24	LPushButton		REDO
25	Big4
26	LStaticText
27	LStaticText
28	LPushButton
29	LStaticText
30	LStaticText
31	LPushButton
32	LPushButton
33	LStaticText
34	LStaticText
35	LStaticText
36	LStaticText
37	LStaticText
38	LStaticText	
39	LStaticText
40	LPushButton
41	LPushButton
42	LPushButton
43	LPopupButton	nRuns
44	LPopupButton	pValue
45	LEditText		maxbns;max number of bins to use



*/
#include <UReanimator.h>
#include <UWindows.h>
#include <UCursor.h>
#include <LArray.h>
#include <String.h>
#include <LString.h>
#include <LStdControl.h>
#include <LEditText.h>
#include <Math.h>

#include <UNavServicesDialogs.h>
//#include <LActiveScroller.h>
#include <LPushButton.h>
#include <LPopupButton.h>
#include <LEditText.h>
#include <LProgressBar.h>
#include <LTable.h>
#include <LWindow.h>
#include "Analyze1.h"
#include "Analyze2.h"
#include "SpikeConstants.h"
#include "CSpike6App.h"
#include "CStatsWindow.h"
#include "CStatsTable.h"
#include "CBig3.h"
#include "CBig4.h"
#include "CViewInfoPane3.h"

#include "SwapUtils.h"

LArray *CStatsWindow::stDrawCol1=nil;//all the x-values (if breakpoint, then x-values before the break)
LArray *CStatsWindow::stDrawCol2=nil;//all the y-values (if breakpoint, then y-values before the break)
LArray *CStatsWindow::stDrawCol3=nil;//contains lower confidence limits
LArray *CStatsWindow::stDrawCol4=nil;//contains upper confidence limits
LArray *CStatsWindow::stDrawCol1a=nil;//x-values after the breakpoint
LArray *CStatsWindow::stDrawCol2a=nil;//yvalues after the breakpoint
LArray *CStatsWindow::sACFray=nil;
LArray *CStatsWindow::sPACFray=nil;
LArray *CStatsWindow::PThelp=nil;
LArray *CStatsWindow::PTintervals=nil;
LArray *CStatsWindow::PTstims=nil;

Float64 CStatsWindow::stMinx=1000000;
Float64 CStatsWindow::stMaxx=-1000000;
Float64 CStatsWindow::stMiny=1000000;
Float64 CStatsWindow::stMaxy=-1000000;
Float64 CStatsWindow::stMinx2=1000000;
Float64 CStatsWindow::stMaxx2=-1000000;
Float64 CStatsWindow::stMiny2=1000000;
Float64 CStatsWindow::stMaxy2=-1000000;
Float64 CStatsWindow::sMedianStat=0;
Float64 CStatsWindow::sKWStat=0;
Float64 CStatsWindow::sKSStat=0;
Float64 CStatsWindow::sMedian=0;
Float64 CStatsWindow::sMedian1=0;
Float64 CStatsWindow::sMedian2=0;
Float64 CStatsWindow::PermProb=0;
SInt32	CStatsWindow::Intervals1=0;
SInt32	CStatsWindow::Intervals2=0;
SInt32 	CStatsWindow::maxorder=15;
SInt32 	CStatsWindow::PTcount=0;
SInt32 	CStatsWindow::PTmaxstim=20;
Boolean CStatsWindow::KeepRepeating=TRUE;
Float64 CStatsWindow::PTspan=0;
Float64 CStatsWindow::PTmintime=0;
Float64 CStatsWindow::PTmaxtime=0;
SInt32	CStatsWindow::MaxRuns=1000;
Float64	CStatsWindow::PValue=0.05;
Float64	CStatsWindow::critval1=0.05;
Float64	CStatsWindow::critval2=0.01;
Float64	CStatsWindow::ksstat=0.0;
SInt32 RunsDone=0;
Boolean CStatsWindow::HistORCumPSTHFlag=FALSE; //when flag is false, do CumPSTH, else Histogram
Boolean CStatsWindow::jrisflag=FALSE;

// ---------------------------------------------------------------------------------
//		¥ CProgressWindow
// ---------------------------------------------------------------------------------

CStatsWindow::CStatsWindow()
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CStatsWindow::CStatsWindow(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CStatsWindow::CStatsWindow(
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

CStatsWindow::CStatsWindow(
	LStream *inStream )
		: LWindow( inStream )
{
mDefaultOutline	 = nil;
}


// ---------------------------------------------------------------------------------
//		¥ ~CProgressWindow
// ---------------------------------------------------------------------------------

CStatsWindow::~CStatsWindow()
{
//delete SimArray;
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
}


// ---------------------------------------------------------------------------------
//		¥ FinishCreateSelf
// ---------------------------------------------------------------------------------

void
CStatsWindow::FinishCreateSelf()
{
	CSpike6App::templrecrd theTRec;
	// Link the window (the listener) with its controls (the broadcasters).
	UReanimator::LinkListenerToControls( this, this, rRidL_StatsWindow );
	SetDefaultButton(ksRedoButt);
	Float64 temp=0.0;
	SetDescriptorForPaneID(kFileCaptionb, CSpike6App::sFileName);
	CSpike6App::tRecord->FetchItemAt(1, &theTRec);
	temp=theTRec.timestamp;//get first timestamp
	LStr255	theTitle(temp,"\p########.#####");
	SetDescriptorForPaneID(kStrtb,theTitle);
	SInt32 n1=CSpike6App::tRecord->GetCount();
	CSpike6App::tRecord->FetchItemAt(n1, &theTRec);
	temp=theTRec.timestamp;//get last timestamp
	theTitle.Assign(temp, "\p########.#####");
	SetDescriptorForPaneID( kEndb,theTitle);
	delete sACFray;
	sACFray=nil;
	sACFray = new LArray(sizeof(Float64));
	ThrowIfNil_(sACFray);
	delete sPACFray;
	sPACFray=nil;
	sPACFray = new LArray(sizeof(Float64));
	ThrowIfNil_(sPACFray);
	SInt32 i=0;
	Float64 myval=0.0;
	for (i=1; i<=maxorder; i++)
		{
		sACFray->AddItem(&myval, sizeof(Float64));
		sPACFray->AddItem(&myval, sizeof(Float64));
		}
	mLastButton=nil;
}

// ---------------------------------------------------------------------------
//	¥ SetDefaultButton
// ---------------------------------------------------------------------------
//	Specify the PaneID of the default button of the window. The default
//	button must be derived from LControl.

void
CStatsWindow::SetDefaultButton(
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
CStatsWindow::FindCommandStatus(
								CommandT			inCommand,
								Boolean				&outEnabled,
								Boolean				&outUsesMark,
								UInt16				&outMark,
								Str255				outName)
{
switch (inCommand) {
		case 2001:
		case 2002:
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
CStatsWindow::HandleKeyPress(
	const EventRecord	&inKeyEvent)
{
	Boolean		keyHandled	= false;
	LControl*	keyButton	= nil;
	UInt16		theChar		= (UInt16) (inKeyEvent.message & charCodeMask);
	
	if ( (theChar == char_Enter) || (theChar == char_Return) ) {
			keyButton = dynamic_cast<LControl*>(FindPaneByID(mDefaultButtonID));
			
	} else if (theChar =='.'){
		SetBusyState(FALSE);
		}
		
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
CStatsWindow::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
Float64 avrg=0.0, stdev=0.0, avrg2=0.0, stdev2=0.0, avrg3=0.0,stdev3=0.0;
SInt32 interv1=0, interv2=0, interv3=0;
LStr255 Lstr;
SInt32 myer;
SInt32 i;
Boolean isok;
Boolean norml,norm2;
LWindow *theResultsWindow;
LEditText *mypaneptr=nil;
CBig3	*theReturnPlot;
CBig4	*theReturnPlotA=nil;
CViewInfoPane3 	*theInfoPane;
SInt16 mitem;
LStdPopupMenu*	thePopupMenu;
LPopupButton	*thePopupBut;
	switch ( inMessage ) {
		
	case 5001: //KRUSKAL_WALLIS statistic
		{
		UCursor::SetWatch();
		GetStatSettings();
		sMedian=0;
		mLastButton=kKWbt;
		isok=FindLinesB(&myer);
		if (CSpike6App::SpikeParams.brkpt==0.0){
			myer=109;
			HandleError(myer);
			break;
			}
		
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			RunKWStat();//ALSO RUNS MEDIAN TEST!!!!!!!!!
			theResultsWindow=LWindow::CreateWindow(rPPob_TestStatistics,this);
			ThrowIfNil_(theResultsWindow);
			TextFont( kFontIDGeneva );
			TextSize( 9 );
			Lstr.Assign("Kruskal-Wallis");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kTestType ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(CStatsWindow::sKWStat,"\p#####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kTestStat ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign("3.841");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kpt5Value ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign("6.635");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kpt1Value ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(Intervals1);
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kNPeriod1 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(Intervals2);
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kNPeriod2 ));
			mypaneptr->SetText(Lstr);		
			Lstr.Assign(CSpike6App::SpikeParams.strt,"\p######.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kStart ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(CSpike6App::SpikeParams.stp,"\p#######.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kStop ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(CSpike6App::SpikeParams.brkpt,"\p#######.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kBreak ));
			mypaneptr->SetText(Lstr);
			RunBasicStatsNew(&avrg, &stdev, &avrg2, &stdev2, &avrg3, &stdev3,  &interv1, &interv2,&interv3);
			Lstr.Assign(avrg,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 11 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(stdev,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 12 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(avrg2,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 13 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(stdev2,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 14 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(sMedian1,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 15 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(sMedian2,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 16 ));
			mypaneptr->SetText(Lstr);

		theResultsWindow->Show();

			}
		}
	break;
	
	case 5002: //Kolmogorov-Smirnoff statistic
		{
		
		UCursor::SetWatch();
		GetStatSettings();
		mLastButton=kKSbt;		
		isok=FindLinesB(&myer);
		if (CSpike6App::SpikeParams.brkpt==0.0){
			myer=109;
			HandleError(myer);
			break;
			}
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			RunKSStat();
			theResultsWindow=LWindow::CreateWindow(rPPob_TestStatistics,this);
			ThrowIfNil_(theResultsWindow);
			Lstr.Assign("Kolmogorov-Smirn");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kTestType ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(CStatsWindow::sKSStat,"\p#.#########");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kTestStat ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign("N/A");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kpt5Value ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign("N/A");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kpt1Value ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(Intervals1);
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kNPeriod1 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(Intervals2);
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kNPeriod2 ));
			mypaneptr->SetText(Lstr);		
			Lstr.Assign(CSpike6App::SpikeParams.strt,"\p######.#####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kStart ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(CSpike6App::SpikeParams.stp,"\p#######.#####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kStop ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(CSpike6App::SpikeParams.brkpt,"\p#######.#####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kBreak ));
			mypaneptr->SetText(Lstr);
				RunBasicStatsNew(&avrg, &stdev, &avrg2, &stdev2,&avrg3, &stdev3,  &interv1, &interv2,&interv3);
			Lstr.Assign(avrg,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 11 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(stdev,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 12 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(avrg2,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 13 ));
			mypaneptr->SetText(Lstr);
			Lstr.Assign(stdev2,"\p####.####");
			mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( 14 ));
			mypaneptr->SetText(Lstr);
			theResultsWindow->Show();
			}
		}
	break;
	
	case 5003: //Permutation test
		{
		UCursor::SetWatch();
		GetStatSettings();
		mLastButton=kPermutbt;
		
		isok=FindLinesB(&myer);
		if (CSpike6App::SpikeParams.brkpt==0.0){
			myer=109;
			HandleError(myer);
			break;
			}
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			RunStatPermutation();
			theResultsWindow=LWindow::CreateWindow(rPPob_TestStatistics,this);
		ThrowIfNil_(theResultsWindow);
		Lstr.Assign("Permutation (10000)");
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kTestType ));
		mypaneptr->SetText(Lstr);
		Lstr.Assign(CStatsWindow::PermProb,"\p#####.####");
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kTestStat ));
		mypaneptr->SetText(Lstr);
		Lstr.Assign("N/A");
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kpt5Value ));
		mypaneptr->SetText(Lstr);
		Lstr.Assign("N/A");
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kpt1Value ));
		mypaneptr->SetText(Lstr);
		Lstr.Assign(Intervals1);
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kNPeriod1 ));
		mypaneptr->SetText(Lstr);
		Lstr.Assign(Intervals2);
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kNPeriod2 ));
		mypaneptr->SetText(Lstr);		
		Lstr.Assign(CSpike6App::SpikeParams.strt,"\p######.#####");
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kStart ));
		mypaneptr->SetText(Lstr);
		Lstr.Assign(CSpike6App::SpikeParams.stp,"\p#######.#####");
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kStop ));
		mypaneptr->SetText(Lstr);
		Lstr.Assign(CSpike6App::SpikeParams.brkpt,"\p#######.#####");
		mypaneptr=dynamic_cast<LEditText *> (theResultsWindow->FindPaneByID( kBreak ));
		mypaneptr->SetText(Lstr);
		theResultsWindow->Show();

			}
		}
	break;
	
	case 5004://remove AR or MA
		{
		UCursor::SetWatch();
		GetStatSettings();
		thePopupMenu = dynamic_cast<LStdPopupMenu*>
						(FindPaneByID(kARMAmenu));
		mitem=thePopupMenu->GetValue();
		if (mitem==2){ //do autoregression
			isok=FindLinesB(&myer);
			if(isok){
				ReconstructAR();
				}
			else {
				HandleError(myer);
				ResetStatSettings();
				}
			}
		else if (mitem==3){//do moving average
			isok=FindLinesB(&myer);
			if(isok){
				ReconstructMA();
				}
			else {
				HandleError(myer);
				ResetStatSettings();
				}
			}
		mLastButton=kARMAmenu;
		break;
		}
		
	case 5005: //save data
		{
		FSSpec OutFileSpec;
		Ptr thePtr;
		LFile *theOutFile;
		Str63 mstr="\pmyFile.txt";
		bool replac =false;
		if (UNavServicesDialogs::AskSaveFile(mstr,FOUR_CHAR_CODE('TEXT'),OutFileSpec,replac,
		kNavDefaultNavDlogOptions + kNavSelectAllReadableItem)){
			theOutFile=new LFile(OutFileSpec);
			SInt32 datLength;
			thePtr=WriteStatPlotData(&datLength);
			if (!(replac)){
				theOutFile->CreateNewDataFile(
				FOUR_CHAR_CODE('atmD'),FOUR_CHAR_CODE('TEXT'),nil);
				}
			theOutFile->OpenDataFork(fsRdWrPerm);
			theOutFile->WriteDataFork(thePtr,datLength);
			theOutFile->CloseDataFork();
			}
		DisposePtr(thePtr);
		break;
		}

	case 5006://
		{
		;
		break;
		}		
	case 5007://Do breakpoint Analysis. Currently this only works well with 1 template only in sSpikeCol.
		{
		UCursor::SetWatch();
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		stDrawCol1 = new LArray(sizeof(Float64));
		stDrawCol2 = new LArray(sizeof(Float64));
		stDrawCol3 = new LArray(sizeof(Float64));
		stDrawCol4 = new LArray(sizeof(Float64));
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
		stDrawCol1a = new LArray(sizeof(Float64));
		stDrawCol2a = new LArray(sizeof(Float64));
		GetStatSettings();
		//this is just to get around FindLines complaining about there being no breakpoint
		CSpike6App::SpikeParams.brkpt=CSpike6App::SpikeParams.strt+
				(CSpike6App::SpikeParams.stp-CSpike6App::SpikeParams.strt)/2;		
		isok=FindLinesB(&myer);
		if (isok){
			RunBreakpoint();
			CSpike6App::SpikeParams.brkpt=0.0;
			}
		else
			{
			HandleError(myer);
			ResetStatSettings();
			}
		theReturnPlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
		ThrowIfNil_( theReturnPlot );
		theReturnPlot->Draw(nil);
		theInfoPane = dynamic_cast<CViewInfoPane3 *> (FindPaneByID( kViewInfoPane3 ));
		ThrowIfNil_( theInfoPane );
		// Register the big view with the view info pane.
		theInfoPane->RegisterView( theReturnPlot );
		theReturnPlotA = dynamic_cast<CBig4 *> (FindPaneByID( kDrawing3 ));
		ThrowIfNil_( theReturnPlotA );
		theReturnPlotA->Draw(nil);
		mLastButton=kFindBreakbt;
		
		break;
		}	
	
	case 5008: //set number of runs
		{
		thePopupBut = dynamic_cast<LPopupButton*>
						(FindPaneByID(43));
		mitem=thePopupBut->GetCurrentMenuItem();
		if (mitem==1){
			MaxRuns=100;
			}
		else if (mitem==2){
			MaxRuns=500;
			}
		else if (mitem==3){
			MaxRuns=1000;
			}
		else if (mitem==4){
			MaxRuns=5000;
			}
		else if (mitem==5){
			MaxRuns=10000;
			}
		break;
		}
	case 5009:
		{
		thePopupBut = dynamic_cast<LPopupButton*>
						(FindPaneByID(44));
		mitem=thePopupBut->GetCurrentMenuItem();
		if (mitem==1){
			PValue=0.10;
			}
		else if (mitem==2){
			PValue=0.05;
			}
		else if (mitem==3){
			PValue=0.01;
			}
		break;
		}
	case 5010: //make ACF-plot
		{
		UCursor::SetWatch();
		delete sACFray;
		sACFray=nil;
		sACFray = new LArray(sizeof(Float64));
		ThrowIfNil_(sACFray);
		delete sPACFray;
		sPACFray=nil;
		sPACFray = new LArray(sizeof(Float64));
		ThrowIfNil_(sPACFray);
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		stDrawCol1 = new LArray(sizeof(Float64));
		stDrawCol2 = new LArray(sizeof(Float64));
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
		stDrawCol1a = new LArray(sizeof(Float64));
		stDrawCol2a = new LArray(sizeof(Float64));
		Float64 myval=0.0;
		for (i=1; i<=maxorder; i++)
			{
			sACFray->AddItem(&myval, sizeof(Float64));
			sPACFray->AddItem(&myval, sizeof(Float64));
			}
		GetStatSettings();
		isok=FindLinesB(&myer);
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			Float64 tbrk=CSpike6App::SpikeParams.brklin;
			CSpike6App::SpikeParams.brklin=0;
			RunACF();
			RunPACF();//should accommodate brkpt in the future
			CSpike6App::SpikeParams.brklin=tbrk; //restores breakline
			//to control drawing (max correlation in plot should be +/- 1.0
			myval=-1.0;
			stDrawCol2->AddItem(&myval, sizeof(Float64));
			stDrawCol2a->AddItem(&myval, sizeof(Float64));
			myval=1.0;
			stDrawCol2->AddItem(&myval, sizeof(Float64));
			stDrawCol2a->AddItem(&myval, sizeof(Float64));
			myval=0.0;
			stDrawCol1->AddItem(&myval,sizeof(Float64));
			stDrawCol1->AddItem(&myval,sizeof(Float64));
			stDrawCol1a->AddItem(&myval,sizeof(Float64));
			stDrawCol1a->AddItem(&myval,sizeof(Float64));
			//end of controlling drawing
			for (i=1;i<=maxorder;i++){
				sACFray->FetchItemAt(i,&myval);
				stDrawCol2->AddItem(&myval, sizeof(Float64));
				myval=(Float64)i;
				stDrawCol1a->AddItem(&myval,sizeof(Float64));
				sPACFray->FetchItemAt(i,&myval);
				stDrawCol2a->AddItem(&myval, sizeof(Float64));
				myval=(Float64)i;
				stDrawCol1->AddItem(&myval,sizeof(Float64));
				}
			theReturnPlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
			ThrowIfNil_( theReturnPlot );
			theReturnPlot->Draw(nil);
			theReturnPlotA = dynamic_cast<CBig4 *> (FindPaneByID( kDrawing3 ));
			ThrowIfNil_( theReturnPlotA );
			theReturnPlotA->Draw(nil);
			CStatsTable *theStatsTable=dynamic_cast<CStatsTable *> (FindPaneByID( kStatTable ));
			ThrowIfNil_( theStatsTable );
			theStatsTable->Draw(nil);
			}
		mLastButton=kACFbt;
		}
	break;



	case 5012: //make Return plot
		{
		UCursor::SetWatch();
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		stDrawCol1 = new LArray(sizeof(Float64));
		stDrawCol2 = new LArray(sizeof(Float64));
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
		stDrawCol1a = new LArray(sizeof(Float64));
		stDrawCol2a = new LArray(sizeof(Float64));
		GetStatSettings();
		isok=FindLinesB(&myer);
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			RunReturn();
			norml=TRUE;
			norm2=FALSE;
			FindMinMax(norml,norm2);
			theReturnPlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
			ThrowIfNil_( theReturnPlot );
			theReturnPlot->Draw(nil);
			if (CSpike6App::SpikeParams.brkpt !=0)
				{
				theReturnPlotA = dynamic_cast<CBig4 *> (FindPaneByID( kDrawing3 ));
				ThrowIfNil_( theReturnPlotA );
				theReturnPlotA->Draw(nil);
				}
			}
		mLastButton=kReturnbt;
		}
	break;
	
	case 5013: //make Ranked Return plot
		{
		UCursor::SetWatch();
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		stDrawCol1 = new LArray(sizeof(Float64));
		stDrawCol2 = new LArray(sizeof(Float64));
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
		stDrawCol1a = new LArray(sizeof(Float64));
		stDrawCol2a = new LArray(sizeof(Float64));
		SInt32 count;
		GetStatSettings();
        HistORCumPSTHFlag = FALSE;
		isok=FindLinesB(&myer);
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			RunRankedReturn();
			norml=FALSE;		
			norm2=FALSE;
			FindMinMax(norml,norm2);
			theReturnPlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
			ThrowIfNil_( theReturnPlot );
			theReturnPlot->Draw(nil);
			count=stDrawCol1a->GetCount();
			if ((CSpike6App::SpikeParams.brkpt !=0)||(count != 0))
				{
				theReturnPlotA = dynamic_cast<CBig4 *> (FindPaneByID( kDrawing3 ));
				ThrowIfNil_( theReturnPlotA );
				theReturnPlotA->Draw(nil);
				}
			}
		mLastButton=kRReturnbt;
		}
	break;

	case 5014: //make 3-i-Return plot
		{
		UCursor::SetWatch();
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		stDrawCol1 = new LArray(sizeof(Float64));
		stDrawCol2 = new LArray(sizeof(Float64));
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
		stDrawCol1a = new LArray(sizeof(Float64));
		stDrawCol2a = new LArray(sizeof(Float64));
		GetStatSettings();
		isok=FindLinesB(&myer);
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			Run3iReturn();
			norml=TRUE;//before and after same scale
			norm2=TRUE;
			FindMinMax(norml,norm2);
			theReturnPlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
			ThrowIfNil_( theReturnPlot );
			theReturnPlot->Draw(nil);
			if (CSpike6App::SpikeParams.brkpt !=0)
				{
				theReturnPlotA = dynamic_cast<CBig4 *> (FindPaneByID( kDrawing3 ));
				ThrowIfNil_( theReturnPlotA );
				theReturnPlotA->Draw(nil);
				}
			
			}
			mLastButton=kP3ibt;
		}
	break;

	case 5015: //respond to STOP-command
		{
		//KeepRepeating=FALSE;
		//SetBusyState(FALSE);
		//StopIdling();
		RunsDone=MaxRuns + 1;
		break;
		}
		
	case 5016: //do Histogram-analysis.
		{
		UCursor::SetWatch();
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		stDrawCol1 = new LArray(sizeof(Float64));
		stDrawCol2 = new LArray(sizeof(Float64));
		stDrawCol3 = new LArray(sizeof(Float64));
		stDrawCol4 = new LArray(sizeof(Float64));
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
		stDrawCol1a = new LArray(sizeof(Float64));
		stDrawCol2a = new LArray(sizeof(Float64));
		GetStatSettings();
		//MaxRuns = 1000;
		HistORCumPSTHFlag=true;
		LProgressBar *theProgBar=dynamic_cast<LProgressBar *>(FindPaneByID(kProgBar));
		theProgBar->SetMinValue(1);
		theProgBar->SetMaxValue(MaxRuns);
		isok=FindLinesB(&myer);
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			
			InitCumPSTH();//init histogram this routine is also for that
			SetBusyState( TRUE);
			//RunPSTH();
			theReturnPlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
			ThrowIfNil_( theReturnPlot );
			theReturnPlot->Draw(nil);
			}
		mLastButton=kPSTH2bt;
		break;
		}

		case 5017: //do cumPSTH-analysis. Randomization test
		{
		UCursor::SetWatch();
		delete stDrawCol1;
		delete stDrawCol2;
		delete stDrawCol3;
		delete stDrawCol4;
		stDrawCol1=nil;
		stDrawCol2=nil;
		stDrawCol3=nil;
		stDrawCol4=nil;
		stDrawCol1 = new LArray(sizeof(Float64));
		stDrawCol2 = new LArray(sizeof(Float64));
		stDrawCol3 = new LArray(sizeof(Float64));
		stDrawCol4 = new LArray(sizeof(Float64));
		delete stDrawCol1a;
		delete stDrawCol2a;
		stDrawCol1a=nil;
		stDrawCol2a=nil;
		stDrawCol1a = new LArray(sizeof(Float64));
		stDrawCol2a = new LArray(sizeof(Float64));
		GetStatSettings();
		//make progress bar and possibly other items. Clicking the progress bar causes execution to stop
		LProgressBar *theProgBar=dynamic_cast<LProgressBar *>(FindPaneByID(kProgBar));
		theProgBar->SetMinValue(1);
		theProgBar->SetMaxValue(MaxRuns);
		HistORCumPSTHFlag = FALSE;
		isok=FindLinesB(&myer);
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			InitCumPSTH();
			SetBusyState( TRUE);
			//RunCumPSTH3();
			//theReturnPlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
			//ThrowIfNil_( theReturnPlot );
			//theReturnPlot->Draw(nil);
			}
		mLastButton=kPSTH3bt;
		break;
		}

	case 5020:
		{
		if (mLastButton != nil)
			{
			LPushButton *mybutptr=dynamic_cast<LPushButton *> (FindPaneByID(mLastButton));
			mybutptr->SimulateHotSpotClick(1);
			}
		break;
		}
		
	case 5021://make averages, (estimate phi and theta) and median
		{
			GetStatSettings();
		Float64 tim1=0.0,tim2=0.0,tim3=0.0,start,end,break1=CSpike6App::SpikeParams.brkpt,break2=CSpike6App::SpikeParams.brkpt2;
		SInt32 count=CSpike6App::tRecord->GetCount();
		CSpike6App::templrecrd theTRec;
		CSpike6App::tRecord->FetchItemAt(1, &theTRec);
		start=theTRec.timestamp;//get first timestamp
		CSpike6App::tRecord->FetchItemAt(count, &theTRec);
		end=theTRec.timestamp;//get last timestamp
		if((break1<break2)&&(break1<0.0001)){//when breakpoint2 is entered but no breakpoint1
			tim1=break2;
			break2=break1;
			break1=tim1;
			CSpike6App::SpikeParams.brkpt=break1;
			CSpike6App::SpikeParams.brkpt2=break2;
		}
		if(break1 > 0.0){
			tim1=break1-start;
			tim2=end-break1;//will be overwritten when breakpt2 is present
		} else if(break1 == 0.0){
			tim1=end-start;
		}
		if(break2 > 0.0){
			tim2=break2-break1;
			tim3=end-break2;
		} //else{
			//tim2=end-CSpike6App::SpikeParams.brkpt;
		//}

		
		mLastButton=kStatbt;
		isok=FindLinesB(&myer);
		if (isok==FALSE)
			{
			HandleError(myer);
			ResetStatSettings();
			}
		else
			{
			RunBasicStatsNew(&avrg, &stdev, &avrg2, &stdev2,&avrg3, &stdev3,  &interv1, &interv2,&interv3);
			norml=FALSE;		
			norm2=FALSE;
			}
		LStr255 Lstr;
		Str255 str;
		LStaticText *myfieldptr=nil;
		CFStringRef myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"),avrg);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(kAvg, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( kAvg ));
		myfieldptr->Refresh();
		
		//Lstr.Assign(stdev,"\p####.##");
		//SetDescriptorForPaneID(kStdev, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"),stdev);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(kStdev, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( kStdev ));
		myfieldptr->Refresh();
				
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"),tim1);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(37, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 37 ));
		myfieldptr->Refresh();
		
		//Lstr.Assign(tim2,"\p#####.##");
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"),tim2);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(38, str);
		//SetDescriptorForPaneID(38, Lstr);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 38 ));
		myfieldptr->Refresh();
			
		//Lstr.Assign(tim3,"\p#####.##");
		//SetDescriptorForPaneID(48, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"),tim3);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(48, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 48 ));
		myfieldptr->Refresh();
			
		//Lstr.Assign(avrg2,"\p####.##");
		//SetDescriptorForPaneID(33, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"),avrg2);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(33, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 33 ));
		myfieldptr->Refresh();
		
		//Lstr.Assign(stdev2,"\p####.##");
		//SetDescriptorForPaneID(34, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), stdev2);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(34, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 34 ));
		myfieldptr->Refresh();

			
			
		//Lstr.Assign(interv1,"\p#####");
		//SetDescriptorForPaneID(29, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), (Float64)interv1);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(29, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 29 ));
		myfieldptr->Refresh();
		
		//Lstr.Assign(interv2,"\p#####");
		//SetDescriptorForPaneID(35, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), (Float64)interv2);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(35, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 35 ));
		myfieldptr->Refresh();
		
		//Lstr.Assign(interv3,"\p#####");
		//SetDescriptorForPaneID(47, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), (Float64)interv3);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(47, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 47 ));
		myfieldptr->Refresh();
		
			
		tim1=(Float64)interv1*3600.0/tim1;
		//Lstr.Assign(tim1,"\p####.##");
		//SetDescriptorForPaneID(30, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), tim1);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(30, str);
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 30 ));
		myfieldptr->Refresh();
		if(tim2!=0.0){
			tim2=(Float64)interv2*3600.0/tim2;
			//Lstr.Assign(tim2,"\p####.##");
			//SetDescriptorForPaneID(36, Lstr);
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), tim2);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
			SetDescriptorForPaneID(36, str);			
			myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID(36 ));
			myfieldptr->Refresh();
		}
		if(tim3!=0.0){
			tim3=(Float64)interv3*3600.0/tim3;
			//Lstr.Assign(tim3,"\p####.##");
			//SetDescriptorForPaneID(49, Lstr);
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), tim3);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
			SetDescriptorForPaneID(49, str);						
			myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID(49 ));
			myfieldptr->Refresh();
		}
		//Lstr.Assign(avrg3,"\p####.##");
		//SetDescriptorForPaneID(50, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), avrg3);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(50, str);		
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 50 ));
		myfieldptr->Refresh();
		//Lstr.Assign(stdev3,"\p####.##");
		//SetDescriptorForPaneID(46, Lstr);
		myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.2f"), stdev3);
		CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
		SetDescriptorForPaneID(46, str);		
		myfieldptr=dynamic_cast<LStaticText *> (FindPaneByID( 46 ));
		myfieldptr->Refresh();
			
		mLastButton=kStatbt;
		}
	break;
	}
}

void CStatsWindow::FindMinMax(Boolean normalize, Boolean normLR)
{
SInt32 i=0, cnt1, cnt2;
Float64 xx, yy, minx, maxx,miny,maxy, minx2, maxx2,miny2,maxy2;
minx=100000000;
maxx=-100000000;
miny=100000000;
maxy=-100000000;
cnt1=stDrawCol1->GetCount();
for (i=1;i<= cnt1; i++)
   	{
   	stDrawCol1->FetchItemAt(i,&xx);
   	if (xx<minx) {minx=xx;}
   	if (xx>maxx) {maxx=xx;}
   	stDrawCol2->FetchItemAt(i,&yy);
   	if (yy<miny) {miny=yy;}
    if (yy>maxy) {maxy=yy;}
   	}
stMaxx=maxx;
stMinx=minx;
stMaxy=maxy;
stMiny=miny;
if ((stDrawCol1a != nil) && (normalize == TRUE))
	{
	cnt2=stDrawCol1a->GetCount();
	for (i=1;i<= cnt2; i++)
   		{
   		stDrawCol1a->FetchItemAt(i,&xx);
   		if (xx<minx) {minx=xx;}
   		if (xx>maxx) {maxx=xx;}
   		stDrawCol2a->FetchItemAt(i,&yy);
   		if (yy<miny) {miny=yy;}
    	if (yy>maxy) {maxy=yy;}
   		}
	if (normLR==TRUE)
		{
		if (fabs(minx)<fabs(maxx))
			{
			minx=-maxx;
			}
		else
			{
			maxx=-minx;
			}
		if (fabs(miny)<fabs(maxy))
			{
			miny=-maxy;
			}
		else
			{
			maxy=-miny;
			}
		}
   	stMaxx=maxx;
	stMinx=minx;
	stMaxy=maxy;
	stMiny=miny;
	stMaxx2=maxx;
	stMinx2=minx;
	stMaxy2=maxy;
	stMiny2=miny;
	}
else if ((stDrawCol1a != nil) && (normalize == FALSE))
	{
	cnt2=stDrawCol1a->GetCount();
	minx2=100000000;
	maxx2=-100000000;
	miny2=100000000;
	maxy2=-100000000;
	for (i=1;i<= cnt2; i++)
   		{
   		stDrawCol1a->FetchItemAt(i,&xx);
   		if (xx<minx2) {minx2=xx;}
   		if (xx>maxx2) {maxx2=xx;}
   		stDrawCol2a->FetchItemAt(i,&yy);
   		if (yy<miny2) {miny2=yy;}
    	if (yy>maxy2) {maxy2=yy;}
   		}
	if (normLR==TRUE)
		{
		if (fabs(minx2)<fabs(maxx2))
			{
			minx2=-maxx2;
			}
		else
			{
			maxx2=-minx2;
			}
		}
	stMaxx2=maxx2;
	stMinx2=minx2;
	stMaxy2=maxy2;
	stMiny2=miny2;
	}
}
// ---------------------------------------------------------------------------------
//		¥ ResetStatSettings
// ---------------------------------------------------------------------------------

void CStatsWindow::ResetStatSettings()
	{
	LStr255 Lstr;
	Lstr.Assign(CSpike6App::SpikeParams.templ1);
	SetDescriptorForPaneID(kTempl1b, Lstr);
	//mypaneptr=dynamic_cast<LEditText *> (FindPaneByID( kTempl1b ));
	//mypaneptr->Refresh();
	Lstr.Assign(CSpike6App::SpikeParams.templ2);
	SetDescriptorForPaneID(kTempl2b, Lstr);
	Lstr.Assign(CSpike6App::SpikeParams.maxbns);
	SetDescriptorForPaneID(kNBINSb, Lstr);
	//mypaneptr=dynamic_cast<LEditText *> (FindPaneByID( kTempl2b ));
	//mypaneptr->Refresh();
	Lstr.Assign(CSpike6App::SpikeParams.strt,"\p######.#####");
	SetDescriptorForPaneID(kStrtb, Lstr);
	//mypaneptr=dynamic_cast<LEditText *> (FindPaneByID( kStrtb ));
	//mypaneptr->Refresh();
	Lstr.Assign(CSpike6App::SpikeParams.stp,"\p#######.#####");
	SetDescriptorForPaneID(kEndb, Lstr);
	//mypaneptr=dynamic_cast<LEditText *> (FindPaneByID( kEndb ));
	//mypaneptr->Refresh();
	Lstr.Assign(CSpike6App::SpikeParams.brkpt,"\p#######.#####");
	SetDescriptorForPaneID(kBrk1, Lstr);
	Lstr.Assign(CSpike6App::SpikeParams.maxbns);
	SetDescriptorForPaneID(kNBINSb, Lstr);
	//mypaneptr=dynamic_cast<LEditText *> (FindPaneByID( kBrk1 ));
	//mypaneptr->Refresh();
	}
// ---------------------------------------------------------------------------------
//		¥ GetStatSettings
// ---------------------------------------------------------------------------------


void
	CStatsWindow::GetStatSettings()
	{
	Str255 str;
	LStr255 Lstr;
	CFStringRef myString=NULL;
	GetDescriptorForPaneID(kTempl1, str);
	myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
	CSpike6App::SpikeParams.templ1=CFStringGetIntValue(myString);
	
	GetDescriptorForPaneID(kTempl2, str);
	myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
	CSpike6App::SpikeParams.templ2=CFStringGetIntValue(myString);
	
	GetDescriptorForPaneID(kNBINSb, str);
	myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
	CSpike6App::SpikeParams.maxbns=CFStringGetIntValue(myString);
	
	GetDescriptorForPaneID(kStrtb, str);
	myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
	Float64 myFloat64=CFStringGetDoubleValue(myString);
	CSpike6App::SpikeParams.strt=myFloat64;

	GetDescriptorForPaneID(kBrk1, str);	
	myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
	myFloat64=CFStringGetDoubleValue(myString);
	CSpike6App::SpikeParams.brkpt=myFloat64;
	
		GetDescriptorForPaneID(kBrk2, str);
		myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
		myFloat64=CFStringGetDoubleValue(myString);
		CSpike6App::SpikeParams.brkpt2=myFloat64;

		GetDescriptorForPaneID(kEndb, str);
		myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
		myFloat64=CFStringGetDoubleValue(myString);
		CSpike6App::SpikeParams.stp=myFloat64;
	//CSpike6App::SpikeParams.stp=static_cast<double>(Lstr);
	
		GetDescriptorForPaneID(kBWidthb, str);
		myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
		myFloat64=CFStringGetDoubleValue(myString);
		CSpike6App::SpikeParams.BWdth=myFloat64;
	//CSpike6App::SpikeParams.BWdth=static_cast<double>(Lstr);
	
		GetDescriptorForPaneID(kNBINSb, str);
		myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
		CSpike6App::SpikeParams.maxbns=CFStringGetIntValue(myString);

	CStatsWindow::sMedianStat=0;
	CStatsWindow::sKWStat=0;
	CStatsWindow::sKSStat=0;
	CStatsWindow::sMedian=0;
	CStatsWindow::PermProb=0;
	CStatsWindow::Intervals1=0;
	CStatsWindow::Intervals2=0;
	CStatsWindow::jrisflag=FALSE;

	}
	
	// ---------------------------------------------------------------------------
//	WriteStatPlotData
// ---------------------------------------------------------------------------

char *
CStatsWindow::WriteStatPlotData(SInt32 *dLength){
	
char *myptr=NewPtrClear(1000000);
char *cstr;
LStr255 mStr;
UInt32 length;
SInt32 i,j,k,maxline=stDrawCol1->GetCount();//, maxline1 = stDrawCol3->GetCount();
SInt32 maxline2=stDrawCol1a->GetCount();
static char tabchar[]=" ";//"\t";
static char returnchar[]="\n";//"\r";
Float64 floatval;
Boolean Drawthree=false;
if (stDrawCol3 != nil){
	Drawthree = true;
	}
j=0;
for(k=1;k<=maxline;k++){
	stDrawCol1->FetchItemAt(k,&floatval);
	mStr.Assign(floatval,"\p#######.#####");
	//mptr=mStr.operator StringPtr();
	cstr= mStr.PToCStr(mStr);
	cstr=strcat(cstr,tabchar);
	length=strlen(const_cast <char *>(cstr));
	for (i=0;i<=length;i++){
		myptr[j]=cstr[i];
		j+=1;
		}
	j-=1;
	stDrawCol2->FetchItemAt(k,&floatval);
	mStr.Assign(floatval,"\p#######.#####");
	cstr= mStr.PToCStr(mStr);
	if (Drawthree) {
		cstr=strcat(cstr,tabchar);
		}
	else {
		cstr=strcat(cstr,returnchar);
		}
	length=strlen(const_cast <char *>(cstr));
	for (i=0;i<=length;i++){
		myptr[j]=cstr[i];
		j+=1;
		}
	j-=1;
	if (Drawthree)
		{
		stDrawCol3->FetchItemAt(k,&floatval);
		mStr.Assign(floatval,"\p#######.#####");
		cstr= mStr.PToCStr(mStr);
		cstr=strcat(cstr,tabchar);
		length=strlen(const_cast <char *>(cstr));
		for (i=0;i<=length;i++){
			myptr[j]=cstr[i];
			j+=1;
			}
		j-=1;
		stDrawCol4->FetchItemAt(k,&floatval);
		mStr.Assign(floatval,"\p#######.#####");
		cstr= mStr.PToCStr(mStr);
		cstr=strcat(cstr,returnchar);
		length=strlen(const_cast <char *>(cstr));
		for (i=0;i<=length;i++){
			myptr[j]=cstr[i];
			j+=1;
			}
		j-=1;
		}
	}
*dLength=j;
if(maxline2 !=0){
	 j=0;
	for(k=1;k<=maxline2;k++){
		stDrawCol1a->FetchItemAt(k,&floatval);
		mStr.Assign(floatval,"\p#######.#####");
		//mptr=mStr.operator StringPtr();
		cstr= mStr.PToCStr(mStr);
		cstr=strcat(cstr,tabchar);
		length=strlen(const_cast <char *>(cstr));
		for (i=0;i<=length;i++){
			myptr[j]=cstr[i];
			j+=1;
			}
		j-=1;
		stDrawCol2a->FetchItemAt(k,&floatval);
		mStr.Assign(floatval,"\p#######.#####");
		cstr= mStr.PToCStr(mStr);
		cstr=strcat(cstr,returnchar);
		length=strlen(const_cast <char *>(cstr));
		for (i=0;i<=length;i++){
			myptr[j]=cstr[i];
			j+=1;
			}
		j-=1;
		}
	*dLength+=j;
	}
return myptr;
}
// ---------------------------------------------------------------------------
//	HandleError(ernum)
// ---------------------------------------------------------------------------

void
CStatsWindow::HandleError(SInt32 errnum){
	
	LStr255 mystring;
	if (errnum==101){
		mystring.Assign("\pTemplate number does not occur in this file;\rit is set to 1");
		}
	else if (errnum==102){
		mystring="\ptemplate 1 was not specified.\rtemplate2 was specified.\rtemplate1 is set to template2";
		}
	else if (errnum==103){
		mystring="\ptemplate 1 was not specified.\r It is set to 1";
		}
	else if (errnum==104){
		mystring="\pstop value too high. \rSet at end of file";
		}
	else if (errnum==105){
		mystring="\pstart value too low.\rSet at start of file";
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
	else if (errnum==109){
		mystring="\pno breakpoint has been specified";
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
// ---------------------------------------------------------------------------------
//		¥ SetBusyState
// ---------------------------------------------------------------------------------

void
CStatsWindow::SetBusyState(
	Boolean inBusy )
{
	// Set the busy state.
mBusy = inBusy;
if ( mBusy ) {
	StartIdling();	//generate random intervals
	} else {
	StopIdling();
	}
}

// ---------------------------------------------------------------------------------
//		¥ SpendTime
// ---------------------------------------------------------------------------------

void
CStatsWindow::SpendTime(const EventRecord &inMacEvent){
	#pragma unused( inMacEvent )
	LProgressBar *theProgBar=dynamic_cast<LProgressBar *>(FindPaneByID(kProgBar));
	SInt32 i;
	if (RunsDone == 0){
		theProgBar->SetMinValue(0);
		theProgBar->SetMaxValue(MaxRuns);
		}
	if (RunsDone < MaxRuns){
		for (i=1; i<=20; i++){
			DoCumPSTHnew();//do one randomization run
			RunsDone +=1;
			}
		theProgBar->SetValue(RunsDone);
		}
	else if (RunsDone >= MaxRuns){//when all iterations have been done, finish up and draw
		FinishCumPSTH();
		CBig3	*thePlot = dynamic_cast<CBig3 *> (FindPaneByID( kDrawing2 ));
		ThrowIfNil_( thePlot );
		thePlot->Draw(nil);
		RunsDone = 0;
		theProgBar->SetValue(RunsDone);
		StopIdling();
		}
	}

// ---------------------------------------------------------------------------
//	SetProgressBar
// ---------------------------------------------------------------------------

Boolean
CStatsWindow::SetProgressBar(SInt32 Round) {
	Boolean Go_on = TRUE; 
	LProgressBar *theProgBar=dynamic_cast<LProgressBar *>(FindPaneByID(kProgBar));
	theProgBar->SetValue(Round);
	if (!KeepRepeating){Go_on = FALSE;}
	return (Go_on);
}