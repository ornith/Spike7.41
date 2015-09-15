// =================================================================================
//	CBigView.cp  					©1995-1998 Metrowerks Inc. All rights reserved.
// =================================================================================

#include <UDrawingState.h>
#include <LString.h>
#include "CTempView.h"
#include "CSpike6App.h"
#include "CSettingsWindow.h"
#include "CSpikeTrainWindow.h"
#include <math.h>
#include <LStaticText.h>
#include <LEditText.h>

Float64 CTempView::sxmin=0;
Float64 CTempView::symin=0;
Float64 CTempView::sxmax=0;
Float64 CTempView::symax=0;
Float64 CTempView::sxscale=0;
Float64 CTempView::syscale=0;
SInt32 CTempView::sxwindmin=0;
SInt32 CTempView::sywindmin=0;
SInt32 CTempView::sxwindmax=0;
SInt32 CTempView::sywindmax=0;

// ---------------------------------------------------------------------------------
//		¥ CTempView
// ---------------------------------------------------------------------------------

CTempView::CTempView()
{
mSelection = Rect_0000;
mFirstRound = true;
}


// ---------------------------------------------------------------------------------
//		¥ CTempView(SPaneInfo&, SViewInfo&)
// ---------------------------------------------------------------------------------

CTempView::CTempView(
	const SPaneInfo	&inPaneInfo,
	const SViewInfo &inViewInfo )
		: LView( inPaneInfo, inViewInfo )
{
mSelection = Rect_0000;
mFirstRound = true;
}


// ---------------------------------------------------------------------------------
//		¥ CTempView(const CTempView&)
// ---------------------------------------------------------------------------------

CTempView::CTempView(
	const CTempView	&inOriginal )
		: LView( inOriginal )
{
mSelection = Rect_0000;
mFirstRound = true;
}


// ---------------------------------------------------------------------------------
//		¥ CTempView(LStream*)
// ---------------------------------------------------------------------------------

CTempView::CTempView(
	LStream	*inStream )
		: LView( inStream )
{
mSelection = Rect_0000;
mFirstRound = true;
}


// ---------------------------------------------------------------------------------
//		¥ ~CTempView
// ---------------------------------------------------------------------------------

CTempView::~CTempView()
{
}

// ---------------------------------------------------------------------------
//	¥ ConstrainMouse												  [public]
// ---------------------------------------------------------------------------
//	Override to constrain the mouse within the frame of this view

void
CTempView::ConstrainMouse(
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
//	¥ Click
// ---------------------------------------------------------------------------
//	Handle a click inside a View

void
CTempView::Click(
	SMouseDownEvent	&inMouseDown)
{
									// Check if a SubPane of this View
									//   is hit
	LPane	*clickedPane = FindSubPaneHitBy(inMouseDown.wherePort.h,
											inMouseDown.wherePort.v);
											
	if (clickedPane != nil) {		// SubPane is hit, let it respond to
									//   the Click
		clickedPane->Click(inMouseDown);
		
	} else {						// No SubPane hit. Inherited function
		LPane::Click(inMouseDown);	//   will process click on this View
	}
}

// ---------------------------------------------------------------------------
//	¥ ClickSelf													   [protected]
// ---------------------------------------------------------------------------

void
CTempView::ClickSelf(
	const SMouseDownEvent&	 inMouseDown)
{
	//FocusDraw();
	
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
				DrawSelf();
		}

	Draw(nil);					// Draw now to frame the selection
	
	StartIdling();				// Request idle time to animate selection
}

// ---------------------------------------------------------------------------
//	¥ GetTargetView													  [public]
// ---------------------------------------------------------------------------
//	Required override for LMarqueeReceiver

const LView*
CTempView::GetTargetView() const
{
	return this;
}



// ---------------------------------------------------------------------------------
//		¥ DrawSelf
// ---------------------------------------------------------------------------------

void
CTempView::DrawSelf()

{
LStr255 Lstr;

if (CSpike6App::tRecord==nil){return;}
// Set the font and size.
TextFont( kFontIDGeneva );
TextSize( 9 );
SInt32 xwindmax,xwindmin,ywindmax,ywindmin;
SInt32 j, lastspike,hpos,vpos,xoffset,yoffset;
Float64 yscale, xscale, xx;
templrecrd Recd;
Rect theFrame;

StColorPenState::Normalize();		// Draw in black and white
CalcLocalFrameRect(theFrame);
::FrameRect(&theFrame);
::EraseRect(&theFrame);
xoffset=5;
yoffset=5;
xwindmax = theFrame.right-xoffset;
xwindmin = theFrame.left+xoffset;
ywindmin = theFrame.bottom-15-yoffset;//15 for scale drawing
ywindmax = theFrame.top+yoffset;
lastspike = CSpike6App::tRecord->GetCount();

if (not ::EmptyRect(&mSelection)) {//draw the expanded picture. Expansion only in X-direction
	mMarchingAnts.UseAntsPattern();		//   marching ants pattern
	::FrameRect(&mSelection);
	mXmin=(mSelection.left-sxwindmin)/sxscale+sxmin;
	mXmax=(mSelection.right-sxwindmin)/sxscale+sxmin;
	if (mXmax>sxmax){mXmax=sxmax;}
	if (mXmin<sxmin){mXmin=sxmin;}
	mSelection = Rect_0000;
	StopIdling();
	}
else {//if (mFirstRound){//this leads to regular drawing of firing patterns
	mXmin=CSpike6App::SpikeParams.strt;
	mXmax=CSpike6App::SpikeParams.stp;
	}
mYmin=-32767;
mYmax=32768;

StColorPenState::Normalize();		// Draw in black and white

//FindOccurrences i.e. locations of spikes
Float64 roomperunit;
xx=1;
xscale=(xwindmax-xwindmin)/(mXmax-mXmin);
SInt32 increm;
//draw scale
increm=round(xwindmax-xwindmin)/10;
hpos=xwindmin;
vpos=ywindmin+5;
::MoveTo(hpos,vpos);
::LineTo(hpos,vpos+10);
for(j=hpos;j<=xwindmax;j+=increm){
	::MoveTo(j,vpos);
	::LineTo(j,vpos+10);
	}
hpos=xwindmin;
::MoveTo(hpos,vpos);
hpos=xwindmax;
::LineTo(hpos,vpos);
hpos=xwindmin;
roomperunit=(Float64)(ywindmin-ywindmax)/(Float64)(CSpike6App::snTemplates);
yscale=(Float64)roomperunit/(Float64)65536;
for (j=1;j<=lastspike;j++){
	CSpike6App::tRecord->FetchItemAt(j,&Recd);
	if((Recd.timestamp>=mXmin)&&(Recd.timestamp<=mXmax)){
		hpos=xwindmin+xscale*(Recd.timestamp-mXmin);
		vpos=ywindmax;
		vpos+= (Recd.templatenum)*roomperunit;
		vpos+= 0.5*roomperunit;
		vpos-= yscale*(Recd.plusval);
		::MoveTo (hpos,vpos);
		vpos+=yscale*(Recd.plusval-Recd.minval);
		::LineTo (hpos,vpos);
		}
		
	}
LEditText *theCaption=dynamic_cast<LEditText*>(mSuperView->FindPaneByID(3));
Lstr.Assign(mXmin,"\p######.###");
theCaption->SetText(Lstr);
theCaption=dynamic_cast<LEditText*>(mSuperView->FindPaneByID(4));
Lstr.Assign(mXmax, "\p######.###");
theCaption->SetText(Lstr);
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

// ---------------------------------------------------------------------------
//	¥ SpendTime														  [public]
// ---------------------------------------------------------------------------

void
CTempView::SpendTime(
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

