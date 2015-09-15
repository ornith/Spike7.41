// =================================================================================
//	CTimView.cp  					©1995-1998 Metrowerks Inc. All rights reserved.
// =================================================================================

#include <UDrawingState.h>

#include "CTimView.h"
#include "CSpike6App.h"
#include "CSettingsWindow.h"
#include <math.h>

Float64 CTimView::sxmin=0;
Float64 CTimView::symin=0;
Float64 CTimView::sxmax=0;
Float64 CTimView::symax=0;
Float64 CTimView::sxscale=0;
Float64 CTimView::syscale=0;
SInt32 CTimView::sxwindmin=0;
SInt32 CTimView::sywindmin=0;
SInt32 CTimView::sxwindmax=0;
SInt32 CTimView::sywindmax=0;


// ---------------------------------------------------------------------------------
//		¥ CTimView
// ---------------------------------------------------------------------------------

CTimView::CTimView()
{
}


// ---------------------------------------------------------------------------------
//		¥ CTimView(SPaneInfo&, SViewInfo&)
// ---------------------------------------------------------------------------------

CTimView::CTimView(
	const SPaneInfo	&inPaneInfo,
	const SViewInfo &inViewInfo )
		: LView( inPaneInfo, inViewInfo )
{
}


// ---------------------------------------------------------------------------------
//		¥ CTimView(const CTimView&)
// ---------------------------------------------------------------------------------

CTimView::CTimView(
	const CTimView	&inOriginal )
		: LView( inOriginal )
{
}


// ---------------------------------------------------------------------------------
//		¥ CTimView(LStream*)
// ---------------------------------------------------------------------------------

CTimView::CTimView(
	LStream	*inStream )
		: LView( inStream )
{
}


// ---------------------------------------------------------------------------------
//		¥ ~CTimView
// ---------------------------------------------------------------------------------

CTimView::~CTimView()
{
}


// ---------------------------------------------------------------------------------
//		¥ DrawSelf
// ---------------------------------------------------------------------------------

void
CTimView::DrawSelf()
{
Rect	theFrame;
CalcLocalFrameRect( theFrame );
::EraseRect(&theFrame);
CSpike6App::templrecrd theTRec;
if ((CSpike6App::SpikeParams.templ1 > -1)&&((CSettingsWindow::TimeCourseFlag)||(CSettingsWindow::HistTimeCourseFlag))&&(CSettingsWindow::DrawCol3!=nil))
	{
	// Save and normalize the text state.
	StTextState	theTextState;
	StTextState::Normalize();
	
	// Set the font and size.
	TextFont( kFontIDGeneva );
	TextSize( 9 );

	// Calculate the frame rect (local coordinates).
	// resize the image
	SPoint16	theTopPos, theBotPos;
	SInt16 hpos, vpos ;
	theTopPos.h=theFrame.left +50;
	theBotPos.h=theFrame.right -15;
	SInt16 horsze=theBotPos.h-theTopPos.h;
	SInt16 midpt=50+round(horsze/2);
	SInt32 templ=0;
	SInt32 i=0, j=0, k=0;
	CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1, &i);
	// i is total occurrences of selected template1
	//for proper scaling, get #occurrences in selected time frame
	Float64 scope=CSpike6App::SpikeParams.BWdth * CSpike6App::SpikeParams.maxbns/1000;
	Float64 scfact=1.0;
	if(CSettingsWindow::TimeCourseFlag){
		scfact=(theBotPos.h-midpt)/scope;
	} else if (CSettingsWindow::HistTimeCourseFlag) {
		scfact=(theBotPos.h-theTopPos.h)/scope;
		midpt=50;
	}
	Float64 vscfact = 260.0/(CSpike6App::SpikeParams.stp-CSpike6App::SpikeParams.strt);
	hpos=10;
	vpos=10;
	Rect kk;
	Float64 intval, val, valu;
	i=0;
	//CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
	vpos = 0;
	hpos=midpt;
	MoveTo(hpos,vpos);
	Pattern	grayPat,blackPat;
	UQDGlobals::GetGrayPat(&grayPat);
	::PenPat(&grayPat);
	vpos = 260;
    LineTo(hpos,vpos);
    UQDGlobals::GetBlackPat(&blackPat);
    PenPat(&blackPat);
	for (j=CSpike6App::SpikeParams.minlin; j<=CSpike6App::SpikeParams.maxlin; j++)
		{
		//if (theImagePos.h>=100){theImagePos.h=1;}
		CSpike6App::tRecord->FetchItemAt(j,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ1)
			{
			val=theTRec.timestamp;
			vpos = round(vscfact*(val-CSpike6App::SpikeParams.strt));
			intval = 0;
			hpos=midpt;
			::ForeColor (redColor);
			::MoveTo(hpos , vpos );
			::SetRect(&kk, hpos-2, vpos-2, hpos + 2, vpos + 2);
			::FrameRect(&kk);
			::ForeColor (blackColor);
			i+=1;
			if (CSettingsWindow::TimeCourseFlag){
				for (k=j-1; k>=CSpike6App::SpikeParams.minlin; k--)
					{
						CSpike6App::tRecord->FetchItemAt(k,&theTRec);
						valu=theTRec.timestamp;
						intval=valu-val; //should be negative
						if (intval< -scope)
						{break;}
						templ=theTRec.templatenum;
						//if (templ == CSpike6App::SpikeParams.templ2)
						if (templ == CSpike6App::SpikeParams.templ2)
						{
                            CSettingsWindow::DrawCol4->AddItem(&val);
                            CSettingsWindow::DrawCol3->AddItem(&intval);
							//draw point
							hpos = midpt + round(scfact * intval); 
							//vpos = round(vscfact*i);
							::MoveTo(hpos , vpos );
							::SetRect(&kk, hpos-1, vpos-1, hpos + 1, vpos + 1);
							::FrameRect(&kk);
						} 
                        
						if (intval< -scope)
						{break;}
					}
				}
			for (k=j+1; k<=CSpike6App::SpikeParams.maxlin; k++)
				{
				CSpike6App::tRecord->FetchItemAt(k,&theTRec);
				valu=theTRec.timestamp;
				intval=valu-val; //should be negative
				if (intval > scope)
					{break;}
				templ=theTRec.templatenum;
				if (templ == CSpike6App::SpikeParams.templ2)
					{
					CSettingsWindow::DrawCol4->AddItem(&val);
					CSettingsWindow::DrawCol3->AddItem(&intval);
					//draw point
					hpos = midpt + round(scfact * intval); 
					//vpos = round(vscfact*i);
					::MoveTo( hpos, vpos );
					::SetRect(&kk, hpos-1, vpos-1, hpos + 1, vpos + 1);
					::FrameRect(&kk);
					if (CSettingsWindow::HistTimeCourseFlag){
						break;
						}
					}
				if (intval > scope)
					{break;}
				}
			}
		}
	sxmin=-scope;
	sxmax=scope;
	symin=CSpike6App::SpikeParams.strt;
	symax=CSpike6App::SpikeParams.stp;
	sxwindmin=theFrame.left+50;
	sxwindmax=theFrame.right-15;
	sywindmin=theFrame.top;
	sywindmax=theFrame.bottom;
	sxscale=scfact;
	syscale=vscfact;
	}
}
