// =================================================================================
//	CBigView.cp  					©1995-1998 Metrowerks Inc. All rights reserved.
// =================================================================================

#include <UDrawingState.h>
#include <LString.h>
#include "CBig3.h"
#include "SpikeConstants.h"
#include "CStatsWindow.h"

Float64 CBig3::sxmin=0;
Float64 CBig3::symin=0;
Float64 CBig3::sxmax=0;
Float64 CBig3::symax=0;
Float64 CBig3::sxscale=0;
Float64 CBig3::syscale=0;
SInt32 CBig3::sxwindmin=0;
SInt32 CBig3::sywindmin=0;
SInt32 CBig3::sxwindmax=0;
SInt32 CBig3::sywindmax=0;

// ---------------------------------------------------------------------------------
//		¥ CBig3
// ---------------------------------------------------------------------------------

CBig3::CBig3()
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ CBig3(SPaneInfo&, SViewInfo&)
// ---------------------------------------------------------------------------------

CBig3::CBig3(
	const SPaneInfo	&inPaneInfo,
	const SViewInfo &inViewInfo )
		: LView( inPaneInfo, inViewInfo )
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ CBig3(const CBig3&)
// ---------------------------------------------------------------------------------

CBig3::CBig3(
	const CBig3	&inOriginal )
		: LView( inOriginal )
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ CBig3(LStream*)
// ---------------------------------------------------------------------------------

CBig3::CBig3(
	LStream	*inStream )
		: LView( inStream )
{
mSelection = Rect_0000;
}


// ---------------------------------------------------------------------------------
//		¥ ~CBig3
// ---------------------------------------------------------------------------------

CBig3::~CBig3()
{
}

// ---------------------------------------------------------------------------
//	¥ ConstrainMouse												  [public]
// ---------------------------------------------------------------------------
//	Override to constrain the mouse within the frame of this view

void
CBig3::ConstrainMouse(
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
CBig3::ClickSelf(
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
CBig3::GetTargetView() const
{
	return this;
}

// ---------------------------------------------------------------------------------
//		¥ DrawSelf
// ---------------------------------------------------------------------------------

void
CBig3::DrawSelf()
{
	
// Set the font and size.
TextFont( kFontIDGeneva );
TextSize( 9 );
SInt32 xwindmax,xwindmin,ywindmax,ywindmin;
SInt32 i,hpos,vpos, vpos3, vpos4,xoffset,yoffset;
Float64 yscale, xscale, xx, yy, minx, maxx,miny,maxy;
Rect kk;
Rect theFrame;
CalcLocalFrameRect( theFrame );
::EraseRect(&theFrame);
UInt32 cnt1=0;
UInt32 cnt2=0;
UInt32 cnt3=0;
UInt32 cnt4=0;
LArray *mArray1=nil;
LArray *mArray2=nil;
LArray *mArray3=nil;
LArray *mArray4=nil;
if (CStatsWindow::stDrawCol1 != nil){
	mArray1=CStatsWindow::stDrawCol1;
	cnt1=mArray1->GetCount();
	}
if (CStatsWindow::stDrawCol2 != nil){
	mArray2=CStatsWindow::stDrawCol2;
	cnt2=mArray2->GetCount();
	}

if (CStatsWindow::stDrawCol3 != nil){
	mArray3=CStatsWindow::stDrawCol3;
	cnt3=mArray3->GetCount();
	}
if (CStatsWindow::stDrawCol4 != nil){
	mArray4=CStatsWindow::stDrawCol4;
	cnt4=mArray4->GetCount();
	}

//if ((cnt3 > 0)&&(cnt4>0)){ then draw upper and lower confidence limits
	
//if (cnt1>cnt2){cnt1=cnt2;}
// Calculate the frame rect (local coordinates).
if ((mArray1 != nil) && (mArray2 != nil))
	{
		if (not ::EmptyRect(&mSelection)) {		// Frame selection with a
		mMarchingAnts.UseAntsPattern();		//   marching ants pattern
		::FrameRect(&mSelection);
		mXmin=(mSelection.left-CBig3::sxwindmin)/CBig3::sxscale + CBig3::sxmin;
		mXmax=(mSelection.right-CBig3::sxwindmin)/CBig3::sxscale + CBig3::sxmin;
		mYmin=(CBig3::sywindmin-mSelection.bottom)/CBig3::syscale +CBig3::symin;
		mYmax=(CBig3::sywindmin-mSelection.top)/CBig3::syscale +CBig3::symin;
		if (mXmax>sxmax){mXmax=sxmax;}
		if (mYmax>symax){mYmax=symax;}
		if (mXmin<sxmin){mXmin=sxmin;}
		if (mYmin<symin){mYmin=symin;}
		mSelection = Rect_0000;
		StopIdling();
		}
	else //find regular min and max.
		{
		minx=100000000;
   		maxx=-100000000;
    	miny=100000000;
    	maxy=-100000000;
    	for (i=1;i<= cnt1; i++)
    		{
    		mArray1->FetchItemAt(i, &xx);
    		if (xx<minx) {minx=xx;}
			if (xx>maxx) {maxx=xx;}
			}
		for (i=1;i<= cnt2; i++)
    		{
			mArray2->FetchItemAt(i, &yy);
    		if (yy<miny) {miny=yy;}
			if (yy>maxy) {maxy=yy;}
    		}
    	mXmin=minx;
		mXmax=maxx;
		mYmin=miny;
		mYmax=maxy;
		sxmin=mXmin;
		symin=mYmin;
		sxmax=mXmax;
		symax=mYmax;
    	}
     //if we have a plus and minus 95% confidence interval, Drawcols 3 and 4 exist, then
    //maxy and miny may be in one of these arrays, so
    if (cnt3>0){
    	for (i=1;i<= cnt3; i++)
    		{
    		mArray3->FetchItemAt(i, &xx);//lower confidence limit)
    		if (xx<miny) {miny=xx;}
    		mArray4->FetchItemAt(i, &yy);
    		if (yy>maxy) {maxy=yy;}
    		}
    	mYmin=miny - 0.05*maxy;//to keep things visible
		mYmax=maxy + 0.05*maxy;
		sxmin=mXmin;
		symin=mYmin;
    	}

	StColorPenState::Normalize();		// Draw in black and white
	::EraseRect(&theFrame);
	if ((cnt1 != 0)&&(cnt2 != 0))
		{
		xwindmax = theFrame.right-15;
		xoffset=40;
		yoffset=30;
		xwindmin = theFrame.left+xoffset;
		ywindmin = theFrame.bottom-yoffset;
		ywindmax = theFrame.top+10;
		//Draw axes
    	::MoveTo(xwindmin,ywindmin);
    	::LineTo(xwindmin,ywindmax);
    	::MoveTo(xwindmin,ywindmin);
    	::LineTo(xwindmax,ywindmin);
    	//needs to be elaborated
    	//get maximum and minimum plotvalues (are statics in CStatWindow)
    	/*draw only when there is valid data*/
    	if ((mXmax>mXmin)and(mYmax>mYmin))
    		{
    		xscale=(xwindmax-xwindmin)/(mXmax-mXmin);
   			yscale=(ywindmin-ywindmax)/(mYmax-mYmin);
   			 //draw gridlines always with respect to the plotframe!!! The legend is adjusted
    		Pattern	grayPat,blackPat;
    		hpos = (SInt16)(xwindmin + xwindmax)/2;//(SInt16)((xwindmin + xwindmax)/2)
    		vpos = ywindmin;
    		MoveTo(hpos,vpos);    		
			UQDGlobals::GetGrayPat(&grayPat);
			PenPat(&grayPat);
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
			//end of drawing gridlines. Pen is back to normal
   	 		//move to first datapoint
    		mArray1->FetchItemAt(1,&xx);
    		mArray2->FetchItemAt(1,&yy);
    		hpos=xscale*(xx-mXmin)+xwindmin;
    		vpos=ywindmin-yscale*(yy-mYmin);
    		::MoveTo(hpos,vpos);
    		for (i=2;i<=cnt1; i++)
    			{
    			mArray1->FetchItemAt(i,&xx);
    			mArray2->FetchItemAt(i,&yy);
    			hpos=xscale*(xx-mXmin)+xwindmin;
    			vpos=ywindmin-yscale*(yy-mYmin);
    			if (((hpos<=xwindmax)&&(hpos>=xwindmin))&&((vpos>=ywindmax)&&(vpos<=ywindmin)))
    				{
    				if (!CStatsWindow::HistORCumPSTHFlag){
    					::SetRect(&kk, hpos-1, vpos-1, hpos + 1, vpos + 1);
						::FrameRect(&kk);
						}
					else{//if a histogram is drawn
    					::LineTo(hpos,vpos);
    					::MoveTo(hpos,vpos);
    					}
    				}
    			}
			 if ((cnt4>0)){//when confidence limits have to be drawn
				ForeColor(redColor);
			  	mArray3->FetchItemAt(1,&yy);
    			vpos3=ywindmin-yscale*(yy-mYmin);
    			mArray1->FetchItemAt(1,&xx);
    			//mArray1->FetchItemAt(1,&xx);
    			hpos=xscale*(xx-mXmin)+xwindmin;
    			MoveTo(hpos, vpos3);
			  	for (i=2; i<=cnt3; i++){
    				mArray3->FetchItemAt(i,&yy);
    				vpos3=ywindmin-yscale*(yy-mYmin);
    				mArray1->FetchItemAt(i,&xx);
    				//mArray1->FetchItemAt(i,&xx);
    				hpos=xscale*(xx-mXmin)+xwindmin;
    				if (((hpos<=xwindmax)&&(hpos>=xwindmin))&&((vpos3>=ywindmax)&&(vpos3<=ywindmin)))
    					{
    					LineTo(hpos, vpos3);
						MoveTo(hpos,vpos3);
    					}
    				}
    			mArray4->FetchItemAt(1,&yy);
    			vpos4=ywindmin-yscale*(yy-mYmin);
    			mArray1->FetchItemAt(1,&xx);
    			//mArray1->FetchItemAt(1,&xx);
    			hpos=xscale*(xx-mXmin)+xwindmin;
    			MoveTo(hpos, vpos4);
			  	for (i=2; i<=cnt3; i++){
    				mArray4->FetchItemAt(i,&yy);
    				vpos4=ywindmin-yscale*(yy-mYmin);
    				mArray1->FetchItemAt(i,&xx);
    				//mArray1->FetchItemAt(i,&xx);
    				hpos=xscale*(xx-mXmin)+xwindmin;
    				if (((hpos<=xwindmax)&&(hpos>=xwindmin))&&((vpos3>=ywindmax)&&(vpos3<=ywindmin)))
    					{
    					LineTo(hpos, vpos4);
						MoveTo(hpos,vpos4);
    					}
    				}
    			
    			}
			ForeColor(blackColor);
    		MoveTo(xwindmin-10,ywindmin+15);
			//LStr255 str(mXmin,"\p#####.#");
			Str255 str;
			CFStringRef myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.1f"),mXmin);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
    		DrawString(str);
			CFRelease(myString);
    		MoveTo(xwindmax-20,ywindmin+15);
    		//str.Assign(mXmax, "\p#####.#");
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.1f"),mXmax);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
    		DrawString(str);
			CFRelease(myString);
    		MoveTo(xwindmin-39,ywindmin+5);
    		//str.Assign(mYmin,"\p#####.#");
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.1f"),mYmin);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
    		DrawString(str);
    		MoveTo(xwindmin-39,ywindmax+5);
    		//str.Assign(mYmax, "\p#####.#");
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.1f"),mYmax);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
			DrawString(str);
			MoveTo(xwindmin+25,ywindmin+25);
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.4f"),CStatsWindow::ksstat);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
			DrawString(str);
			MoveTo(xwindmin+85,ywindmin+25);
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.4f"),CStatsWindow::critval1);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
			DrawString(str);
			MoveTo(xwindmin+145,ywindmin+25);
			myString=CFStringCreateWithFormat(NULL,NULL,CFSTR("%2.4f"),CStatsWindow::critval2);
			CFStringGetPascalString(myString, str, 256, CFStringGetSystemEncoding());
			DrawString(str);
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
}

// ---------------------------------------------------------------------------
//	¥ SpendTime														  [public]
// ---------------------------------------------------------------------------

void
CBig3::SpendTime(
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
