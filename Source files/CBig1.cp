// =================================================================================
//	CBig1.cp  					©1995-1998 Metrowerks Inc. All rights reserved.
// =================================================================================

#include <UDrawingState.h>
#include <LString.h>
#include "CBig1.h"
#include <math.h>
#include "CSettingsWindow.h"
#include "CSpike6App.h"
#include <LStaticText.h>
#include <UModalDialogs.h>
Float64 CBig1::sxmin=0;
Float64 CBig1::symin=0;
Float64 CBig1::sxmax=0;
Float64 CBig1::symax=0;
Float64 CBig1::sxscale=0;
Float64 CBig1::syscale=0;
SInt32 CBig1::sxwindmin=0;
SInt32 CBig1::sywindmin=0;
SInt32 CBig1::sxwindmax=0;
SInt32 CBig1::sywindmax=0;

// ---------------------------------------------------------------------------------
//		¥ CBig1
// ---------------------------------------------------------------------------------

CBig1::CBig1()
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ CBig1(SPaneInfo&, SViewInfo&)
// ---------------------------------------------------------------------------------

CBig1::CBig1(
	const SPaneInfo	&inPaneInfo,
	const SViewInfo &inViewInfo )
		: LView( inPaneInfo, inViewInfo )
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ CBig1(const CBig1&)
// ---------------------------------------------------------------------------------

CBig1::CBig1(
	const CBig1	&inOriginal )
		: LView( inOriginal )
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ CBig1(LStream*)
// ---------------------------------------------------------------------------------

CBig1::CBig1(
	LStream	*inStream )
		: LView( inStream )
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ ~CBig1
// ---------------------------------------------------------------------------------

CBig1::~CBig1()
{
}

// ---------------------------------------------------------------------------
//	¥ ConstrainMouse												  [public]
// ---------------------------------------------------------------------------
//	Override to constrain the mouse within the frame of this view

void
CBig1::ConstrainMouse(
	const LMarqueeTask&	/* inMarqueeTask */,
	Point&				ioMouseLocal,
	UInt16				/* inModifiers */)
{
	Rect	frame;
	CalcLocalFrameRect(frame);
	
	::InsetRect(&frame, 1, 1);	// Account for 1-pixel border which
								//   we draw around the view
	
		// We could use the Toolbox routine ::PinRect(), but
		// that routine has a documented bug when the point is
		// exactly on the lower right corner. So we do it ourself
		// by testing each edge.
	
	if (ioMouseLocal.h < frame.left) {
		ioMouseLocal.h = frame.left;
		
	} else if (ioMouseLocal.h > frame.right - 1) {
		ioMouseLocal.h = frame.right - 1;
	}
	
	if (ioMouseLocal.v < frame.top) {
		ioMouseLocal.v = frame.top;
	
	} else if (ioMouseLocal.v > frame.bottom - 1) {
		ioMouseLocal.v = frame.bottom - 1;
	}
}

// ---------------------------------------------------------------------------
//	¥ ClickSelf													   [protected]
// ---------------------------------------------------------------------------

void
CBig1::ClickSelf(
	const SMouseDownEvent&	 inMouseDown)
{
	FocusDraw();
	
	if ( not ::EmptyRect(&mSelection) ) {		// Erase existing selection
		::EraseRect(&mSelection);
		mSelection = Rect_0000;
		StopIdling();
	}
	
		// Create LMarquee object on the stack
	
	LMarqueeTask theMarquee(*this, inMouseDown);
	
		// DoMarquee() will handle all mouse tracking. Look at the
		// LMarqueeReceiver class for details on which functions to
		// override in order to provide visual feedback while tracking.
	
	theMarquee.DoMarquee();
	
		// Finished tracking. Ask the MarqueeTask what rectangle
		// the user selected.
	
	theMarquee.GetMarqueeRect(mSelection);
		//now what does this mean in terms of x and y values?
		
	
		// If the selection rectangle is really small, assume the
		// user just clicked in the view. We'll interpret this to
		// mean there is no selection.
	
	if ( ((mSelection.right - mSelection.left) < 2) &&
			((mSelection.bottom - mSelection.top < 2)) ) {

		mSelection = Rect_0000;
	}
	
	Draw(nil);					// Draw now to frame the selection
	
	StartIdling();				// Request idle time to animate selection
}

// ---------------------------------------------------------------------------
//	¥ GetTargetView													  [public]
// ---------------------------------------------------------------------------
//	Required override for LMarqueeReceiver

const LView*
CBig1::GetTargetView() const
{
	return this;
}



// ---------------------------------------------------------------------------------
//		¥ DrawSelf
// ---------------------------------------------------------------------------------

void
CBig1::DrawSelf()

{
SInt32 xwindmax,xwindmin,ywindmax,ywindmin;
SInt32 i,hpos,vpos,xoffset,yoffset;
Float64 yscale, xscale, xx, yy, est, minx, maxx,miny,maxy;

if ((CSettingsWindow::DrawCol1 != nil) && (CSettingsWindow::DrawCol2 != nil))
{
SInt32 n1=CSettingsWindow::DrawCol1->GetCount();
SInt32 n2=CSettingsWindow::DrawCol2->GetCount();
SInt32 n3=CSettingsWindow::DrawCol3->GetCount();
	if (not ::EmptyRect(&mSelection)) {		
		// Frame selection with a marching ants pattern
		mMarchingAnts.UseAntsPattern();		   
		::FrameRect(&mSelection);
		mXmin=(mSelection.left-CBig1::sxwindmin)/CBig1::sxscale + CBig1::sxmin;
		mXmax=(mSelection.right-CBig1::sxwindmin)/CBig1::sxscale + CBig1::sxmin;
		mYmin=(CBig1::sywindmin-mSelection.bottom)/CBig1::syscale +CBig1::symin;
		mYmax=(CBig1::sywindmin-mSelection.top)/CBig1::syscale +CBig1::symin;
		if (mXmax>sxmax){mXmax=sxmax;}
		if (mYmax>symax){mYmax=symax;}
		if (mXmin<sxmin){mXmin=sxmin;}
		if (mYmin<symin){mYmin=symin;}
		mSelection = Rect_0000;
		StopIdling();
		}
	else //find regular min and max in DrawCol1
		{
		minx=100000000;
   		maxx=-100000000;
    	miny=100000000;
    	maxy=-100000000;
    	for (i=1;i<= n1; i++)
    		{
    		CSettingsWindow::DrawCol1->FetchItemAt(i, &xx);
    		if (xx<minx) {minx=xx;}else if (xx>maxx) {maxx=xx;}
    		CSettingsWindow::DrawCol2->FetchItemAt(i, &yy);
    		if (yy<miny) {miny=yy;}else if (yy>maxy) {maxy=yy;}
    		}
    	mXmin=minx;
		mXmax=maxx;
		mYmin=miny;
		//mYmax=maxy + 0.05*maxy;
		if(maxy < 10){
			mYmax = maxy +1;
		} else if (maxy < 100){
			mYmax = trunc(maxy / 10)*10 + 10;
		} else if (maxy < 1000){
			mYmax = trunc(maxy / 100)*100 + 100;
		}else if (maxy < 10000){
			mYmax = trunc(maxy / 1000)*1000 + 1000;
		} else if (maxy < 100000){
			mYmax = trunc(maxy / 10000)*10000 + 10000;
		} else if (maxy < 1000000){
			mYmax = trunc(maxy / 100000)*100000 + 100000;
		} else {
			mYmax= maxy;
		}
		sxmin=mXmin;
		symin=mYmin;
		sxmax=mXmax;
		symax=mYmax;
    	}
    if(CSettingsWindow::ZeroXOrigin){
    	mXmin=0;
	}
	if(CSettingsWindow::ZeroYOrigin){
    	mYmin=0;
    	}
	StColorPenState::Normalize();		// Draw in black and white
	
if ((n1 != 0)&&(n2 != 0))
	{
	// Set the font and size.
	TextFont( kFontIDGeneva );
	TextSize( 9 );
	// Calculate the frame rect (local coordinates).
	Rect	theFrame;
	CalcLocalFrameRect( theFrame );
	::EraseRect(&theFrame);
	xwindmax = theFrame.right-15;
	xoffset=50;
	yoffset=25;
	xwindmin = theFrame.left+xoffset;
	ywindmin = theFrame.bottom-yoffset;
	ywindmax = theFrame.top+10;
	//Draw axes
    MoveTo(xwindmin,ywindmin);
    LineTo(xwindmin,ywindmax);
    MoveTo(xwindmin,ywindmin);
    LineTo(xwindmax,ywindmin);
    
    //needs to be elaborated
    //find maximum and minimum plotvalues (are in sDrawArray)
    xscale=(xwindmax-xwindmin)/(mXmax-mXmin);
    yscale=(ywindmin-ywindmax)/(mYmax-mYmin);
    //draw gridlines always with respect to the plotframe!!! The legend is adjusted
    Pattern	grayPat,blackPat;
    hpos = (SInt16)(xwindmin + xwindmax)/2;//(SInt16)((xwindmin + xwindmax)/2)
    vpos = ywindmin;
    MoveTo(hpos,vpos);    		
	//UQDGlobals::GetGrayPat(&grayPat);
	//PenPat(&grayPat);
	vpos = ywindmax;
   	LineTo(hpos,vpos);
   	hpos = xwindmin;
   	vpos = (SInt16)(ywindmin + ywindmax)/2;
   	MoveTo (hpos,vpos);
   	hpos = xwindmax;
   	LineTo (hpos,vpos);
   	hpos=xwindmin;
   	vpos=ywindmax;
   	MoveTo(hpos,vpos);
   	hpos=xwindmax;
   	LineTo(hpos,vpos);
   	vpos=ywindmin;
   	LineTo(hpos,vpos);
    UQDGlobals::GetBlackPat(&blackPat);
	PenPat(&blackPat);
	//draw what is in drawcols 1 and 2
	hpos=10000000;
	vpos=10000000;
    //move to first datapoint within the selection
   SInt32 k=0;
   Rect kk;
    while ((not ((hpos<=xwindmax)&&(hpos>=xwindmin)&&(vpos>=ywindmax)&&(vpos<=ywindmin)))&&(k<n1))
    	{
    	k+=1;
    	CSettingsWindow::DrawCol1->FetchItemAt(k, &xx);
    	CSettingsWindow::DrawCol2->FetchItemAt(k, &yy);
    	hpos=xscale*(xx-mXmin)+xwindmin;
   		vpos=ywindmin-yscale*(yy-mYmin);
   		}
    ::MoveTo(hpos,vpos);
    for (i=k;i<=n1; i++)
    	{
    	CSettingsWindow::DrawCol1->FetchItemAt(i, &xx);
    	CSettingsWindow::DrawCol2->FetchItemAt(i, &yy);
    	hpos=xscale*(xx-mXmin)+xwindmin;
    	vpos=ywindmin-round(yscale*(yy-mYmin));
    	if (((hpos<=xwindmax)&&(hpos>=xwindmin))&&((vpos>=ywindmax)&&(vpos<=ywindmin)))
    		{
    		if(CSettingsWindow::DrawLines)
    			{
    			::LineTo(hpos,vpos);
    			::MoveTo(hpos,vpos);
            }
    		else if (CSettingsWindow::DrawDots)
    			{
    			::SetRect(&kk, hpos-1, vpos-1, hpos + 1, vpos + 1);
				::FrameRect(&kk);
            }
        }
    	else
    		{
    		::MoveTo(hpos,vpos);
        }
    }
   	if(n3 != 0)
    	{
    	k=0;
    	hpos=10000000;
		vpos=10000000;
    	while ((not ((hpos<=xwindmax)&&(hpos>=xwindmin)&&(vpos>=ywindmax)&&(vpos<=ywindmin)))&&(k<n1))
    		{
    		k+=1;
    		CSettingsWindow::DrawCol1->FetchItemAt(k, &xx);
   			CSettingsWindow::DrawCol3->FetchItemAt(k, &est);
   			hpos=xscale*(xx-mXmin)+xwindmin;
   			vpos=ywindmin-yscale*(est-mYmin);
   			}
    	::MoveTo(hpos,vpos);
    	for (i=k; i<=n1; i++)
    		{
    		CSettingsWindow::DrawCol1->FetchItemAt(i, &xx);
    		hpos=xscale*(xx-mXmin)+xwindmin;
    		CSettingsWindow::DrawCol3->FetchItemAt(i, &est);
    		vpos=ywindmin-yscale*(est-mYmin);//we assume that maxy more or less equals maxest
    		if (((hpos<=xwindmax)&&(hpos>=xwindmin))&&((vpos>=ywindmax)&&(vpos<=ywindmin)))//check whether datapoints are within bounds
    			{
				::LineTo(hpos,vpos);
    			}
    		}
		}
		xx=CSettingsWindow::uplim;
		xx=CSettingsWindow::lolim;
	//draw the upper and lower limit lines
		if (CSettingsWindow::uplim>=0){
			::ForeColor (redColor);
			hpos=xwindmin;
			vpos=ywindmin-yscale*(CSettingsWindow::uplim-mYmin);
			::MoveTo(hpos,vpos);
			hpos=xwindmax;
			::LineTo(hpos,vpos);
		}
		if (CSettingsWindow::lolim>=0){
			::ForeColor (redColor);
			hpos=xwindmin;
			vpos=ywindmin-yscale*(CSettingsWindow::lolim-mYmin);
			::MoveTo(hpos,vpos);
			hpos=xwindmax;
			::LineTo(hpos,vpos);
		}
	::ForeColor (blackColor);
	LStr255	theTitle(mXmin,"\p######.#####");
	MoveTo(xwindmin-10,ywindmin+12);
	DrawString(theTitle);
	//SetDescriptorForPaneID(50,theTitle);
	theTitle.Assign(mXmax,"\p######.#####");
	MoveTo(xwindmax-20,ywindmin+12);
	DrawString(theTitle);
	//SetDescriptorForPaneID(52,theTitle);
	xx = (mXmin + mXmax)/2;
	theTitle.Assign(xx,"\p######.#####");
	MoveTo(round((xwindmax+xwindmin)/2.0)-5,ywindmin+12);
	DrawString(theTitle);
	//SetDescriptorForPaneID(51,theTitle);
	theTitle.Assign(mYmin,"\p######.#");
	MoveTo(xwindmin-45,ywindmin+5);
	DrawString(theTitle);
	//SetDescriptorForPaneID(53,theTitle);
	yy = (mYmin + mYmax)/2;
	theTitle.Assign(yy,"\p######.#");
	MoveTo(xwindmin-45,round((ywindmin-ywindmax)/2)+12);
	DrawString(theTitle);
	//SetDescriptorForPaneID(54,theTitle);
	theTitle.Assign(mYmax,"\p######.#");
	MoveTo(xwindmin-45,ywindmax+5);
	DrawString(theTitle);
	//SetDescriptorForPaneID(55,theTitle);
	theTitle.Assign(CSpike6App::SpikeParams.templ1,"\p####");
	theTitle.Append("\p - ");
	LStr255 helpTitle(CSpike6App::SpikeParams.templ2,"\p####");
	theTitle.Append(helpTitle);
	MoveTo(xwindmin + 5, ywindmin+23);
	DrawString(theTitle);
	theTitle.Assign("\punits:");
	MoveTo(xwindmin - 25, ywindmin+23);
	DrawString(theTitle);
	theTitle.Assign(CSpike6App::SpikeParams.BWdth,"\p#####.###");
	MoveTo(xwindmin + 95, ywindmin+23);
	DrawString(theTitle);
	theTitle.Assign("\pbinwidth:");
	MoveTo(xwindmin + 45, ywindmin+23);
	DrawString(theTitle);
	theTitle.Assign("\pmsec");
	MoveTo(xwindmin + 130, ywindmin+23);
	DrawString(theTitle);
		
	//SetDescriptorForPaneID(56,theTitle);

    sxmin=mXmin;
	symin=mYmin;
	sxmax=mXmax;
	symax=mYmax;
	sxscale=xscale;
	syscale=yscale;
	sxwindmin=xwindmin;
	sywindmin=ywindmin;
	sxwindmax=xwindmax;
	sywindmax=ywindmax;
	}
}
}

// ---------------------------------------------------------------------------
//	¥ SpendTime														  [public]
// ---------------------------------------------------------------------------

void
CBig1::SpendTime(
	const EventRecord&	/*  inMacEvent */)
{
		// Animate the selection if we are in an active window. Selection
		// appears to animate because the LMarchingAnts class rotates the
		// pen pattern each time we draw.

	if ( not ::EmptyRect(&mSelection)  &&
		 IsActive()  &&
		 FocusExposed() ) {	
	
		StColorPenState::Normalize();			// Draw in black and white
		
			// BeginMarch() returns true if we should draw now. If it returns
			// false, it means that not enough time has passed since the last
			// time we drew. 
		
		if ( mMarchingAnts.BeginMarch() ) {
		
				// At this point, LMarchingAnts has set the pen pattern to
				// the ants pattern. Any subsequent QuickDraw calls will use
				// this pattern to draw. This is most suitable for the
				// various FrameXXX() QuickDraw functions.
		
			::FrameRect(&mSelection);
			
				// Now me must call EndMarch(), which rotates the ants
				// pattern so that the next time we draw the "ants" will
				// be in a different position.
			
			mMarchingAnts.EndMarch();
		}
	}
}
