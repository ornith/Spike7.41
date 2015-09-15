// =================================================================================
//	CSumWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CSumWindow.h	
#include "CSpike6App.h"
#include <LString.h>
#include <UReanimator.h>
#include <UWindows.h>
#include <LArray.h>
#include <Math.h>
#include <LStdControl.h>
#include <LPopupButton.h>
#include <LWindow.h>
#include <LPane.h>
#include <LProgressBar.h>
#include <LLittleArrows.h>
#include <LScrollerView.h>
#include <LString.h>
#include <LStaticText.h>
#include <UNavServicesDialogs.h>
#include <UStandardDialogs.h>
#include <FixMath.h>

#include "CSumWindow.h"
#include "CSumView.h"
#include "SpikeConstants.h"

Float64 BinWidth=0.020;
SInt32 NumBins=64;
SInt32 CSumWindow::amp=1; 
bool CSumWindow::DrawAll=false; 
//locals

// ---------------------------------------------------------------------------------
//		¥ CProgressWindow
// ---------------------------------------------------------------------------------

CSumWindow::CSumWindow()
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CSumWindow::CSumWindow(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo )
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CSumWindow::CSumWindow(
	ResIDT		inWINDid,
	UInt32		inAttributes,
	LCommander	*inSuperCommander )
		: LWindow( inWINDid, inAttributes, inSuperCommander )
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(LStream *)
// ---------------------------------------------------------------------------------

CSumWindow::CSumWindow(
	LStream *inStream )
		: LWindow( inStream )
{
}


// ---------------------------------------------------------------------------------
//		¥ ~CProgressWindow
// ---------------------------------------------------------------------------------

CSumWindow::~CSumWindow()
{
;
}


// ---------------------------------------------------------------------------------
//		¥ FinishCreateSelf
// ---------------------------------------------------------------------------------

void
CSumWindow::FinishCreateSelf()
{
	// Link the window (the listener) with its controls (the broadcasters).
	UReanimator::LinkListenerToControls( this, this, rRidL_SumWindow );
	mSumView = dynamic_cast<CSumView *> (FindPaneByID( 1 ));
	ThrowIfNil_( mSumView );
	mTitle=dynamic_cast<LStaticText*>(FindPaneByID(2));
	if (mTitle != nil) {
			mTitle->SetDescriptor(CSpike6App::sFileName);
	}  
	MakeHistograms();
	SInt32 count=CSpike6App::theWaves->GetCount();
	if (count!=0){
		MakeAvgs();
		}
	else{
		//fill the relevant file with zeroes
		;
		}
}




// ---------------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------------


void
CSumWindow::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
SInt16 mitem;
LPopupButton	*thePopup;
	switch ( inMessage ) {


		case 7003:
		{
		thePopup=dynamic_cast<LPopupButton*>
										(FindPaneByID(3));
		
		mitem=thePopup->GetCurrentMenuItem();
		if (mitem==1){
			BinWidth=0.001;
			}
		else if (mitem==2){
			BinWidth=0.002;
			}
		else if (mitem==3){
			BinWidth=0.005;
			}
		else if (mitem==4){
			BinWidth=0.010;
			}
		else if (mitem==5){
			BinWidth=0.020;
			}
		else if (mitem==6){
			BinWidth=0.050;
			}
		else if (mitem==7){
			BinWidth=0.100;
			}
		else if (mitem==8){
			BinWidth=0.200;
			}
		else if (mitem==9){
			BinWidth=0.500;
			}
		//teken opnieuw
		MakeHistograms();
		mSumView = dynamic_cast<CSumView *> (FindPaneByID( 1 ));
		ThrowIfNil_( mSumView );
		mSumView->Draw(nil);
		break;
		}
		case 7004:
		{
		thePopup=dynamic_cast<LPopupButton*>
										(FindPaneByID(4));
		
		mitem=thePopup->GetCurrentMenuItem();
		if (mitem==1){
			amp=1;
			}
		else if (mitem==2){
			amp=2;
			}
		else if (mitem==3){
			amp=3;
			}
		else if (mitem==4){
			amp=4;
			}
		else if (mitem==5){
			amp=5;
			}
		//teken opnieuw
		MakeHistograms();
		mSumView = dynamic_cast<CSumView *> (FindPaneByID( 1 ));
		ThrowIfNil_( mSumView );
		mSumView->Draw(nil);
		break;
		}
		case 7005:
		{
		if (!DrawAll){
			DrawAll=true;
			}
		else{
			DrawAll=false;
			//MakeAvgs();
			}
		//teken opnieuw
		MakeHistograms();
		mSumView = dynamic_cast<CSumView *> (FindPaneByID( 1 ));
		ThrowIfNil_( mSumView );
		mSumView->Draw(nil);
		break;
		}
	}
}

void
CSumWindow::MakeHistograms(){
	CSpike6App::templrecrd myrec;
	CSpike6App::histo myhist;
	delete CSpike6App::theHistogram;
	CSpike6App::theHistogram=nil;
	CSpike6App::theHistogram = new LArray(sizeof(CSpike6App::histo));
	SInt32 count=0,binnum,ccl,mval=0,cnt;
	UInt32 sp;
	Float64 maxdist=BinWidth*(Float64)NumBins, lasttime[128];
	for (sp=0; sp<=63; sp++){//set up a help array that contains the last occurrence
		lasttime[sp]=0.0;
		}
	for (binnum=0; binnum<= 63; binnum++){
		myhist.histdata[binnum]=0;
		}	
	for (ccl=1; ccl<=CSpike6App::snTemplates+1; ccl++){
		CSpike6App::theHistogram->AddItem(&myhist);
		}
	cnt=CSpike6App::tRecord->GetCount();
	for (sp=1; sp<=cnt; sp++){
		CSpike6App::tRecord->FetchItemAt(sp,&myrec);
		CSpike6App::theHistogram->FetchItemAt(myrec.templatenum,&myhist);
		if (lasttime[myrec.templatenum-1]!=0){
			binnum=(SInt32)trunc(NumBins*(myrec.timestamp-lasttime[myrec.templatenum-1])/maxdist);
			if (binnum==0){
				mval+=1;
				}
			if (binnum<=(NumBins-1)){
				count=myhist.histdata[binnum];
				count+=1;
				myhist.histdata[binnum]=count;
				CSpike6App::theHistogram->AssignItemsAt(1,myrec.templatenum,&myhist);
				}
			lasttime[myrec.templatenum-1]=myrec.timestamp;
			}
		else { //if lasttime is zero, so this is the first occurrence
			lasttime[myrec.templatenum-1]=myrec.timestamp;
			}
		}
}

void
CSumWindow::MakeAvgs(){
	SInt32 sum[129][65];//sum[0] have N. 
	UInt32 sp, i, count;
	CSpike6App::templrecrd myrec;
	CSpike6App::waveform wvfrm;
	
	delete CSpike6App::theAvgs;
	CSpike6App::theAvgs=nil;
	CSpike6App::theAvgs = new LArray(sizeof(CSpike6App::waveform));//note: we will only use the .wavform part of the struct, not .timestamp
	
	for (sp=0; sp<=128; sp++){
		for (i=0; i<=65; i++){
			sum[sp][i]=0.0;
			}
		}
	count=CSpike6App::tRecord->GetCount();
	count=CSpike6App::theWaves->GetCount();
	for (sp=1; sp<=count; sp++){
		CSpike6App::tRecord->FetchItemAt(sp,&myrec);
		CSpike6App::theWaves->FetchItemAt(sp,&wvfrm);
		sum[myrec.templatenum][64]+=1;
		for (i=0;i<=63;i++){
			sum[myrec.templatenum][i]=sum[myrec.templatenum][i] + wvfrm.wavform[i];//summate
			}
		}
	//now we have the sums of all poits in waveform. We put the averages in the static array theAvgs.
	for (sp=1; sp<=CSpike6App::snTemplates; sp++){
		for (i=0; i<=63; i++){
			wvfrm.wavform[i]=(SInt16)((long double)sum[sp][i]/(long double)sum[sp][64]);
			}
		CSpike6App::theAvgs->AddItem(&wvfrm);
		}
	}

