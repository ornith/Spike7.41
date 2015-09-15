// =================================================================================
//	CViewInfoPane.cp 				©1995-1998 Metrowerks Inc. All rights reserved.
// =================================================================================

#include <LView.h>
#include <LWindow.h>
#include <UDesktop.h>
#include <UDrawingState.h>

#include <Math.h>
#include "CViewInfoPane1.h"
#include "CTempView.h"
#include "CSpike6App.h"

// ---------------------------------------------------------------------------------
//		¥ CViewInfoPane(LStream*)
// ---------------------------------------------------------------------------------

CViewInfoPane1::CViewInfoPane1(
	LStream	*inStream )
		: LPane( inStream )
{
	// Initialize the view and last point.
	mView = nil;
	mLastPt.h = mLastPt.v = 0;
}


// ---------------------------------------------------------------------------------
//		¥ ~CViewInfoPane
// ---------------------------------------------------------------------------------

CViewInfoPane1::~CViewInfoPane1()
{
}


// ---------------------------------------------------------------------------------
//		¥ RegisterView
// ---------------------------------------------------------------------------------

void
CViewInfoPane1::RegisterView(
	LView	*inView )
{
	// Store the view and start idling.
	mView = inView;
	StartIdling();
}


// ---------------------------------------------------------------------------------
//		¥ SpendTime
// ---------------------------------------------------------------------------------

void
CViewInfoPane1::SpendTime(
	const EventRecord &inMacEvent )
{
#pragma unused( inMacEvent )

	// Fetch the top window if there is one.
	LWindow	*theWindow = UDesktop::FetchTopRegular();
	
	if ( theWindow != nil ) {
	
		// Another pane may have changed the origin.
		// Since GetMouse gets coordinates in port coordinates, we
		// have to reset the window origin.
		::SetOrigin( 0, 0 );
	
		// Get the mouse coordinates.
		Point	thePoint;
		::GetMouse( &thePoint );
		
		// Tell LView to purge it's cache of the currently focused
		// view since we changed the origin ourselves.
		LView::OutOfFocus( nil );
	
		if ( !::EqualPt( thePoint, mLastPt ) ) {
		
			// Update the last point.
			mLastPt = thePoint;
			
			// Find the deepest pane containing the port coordinates.
			LPane	*thePane;
			thePane = theWindow->FindDeepSubPaneContaining( thePoint.h, thePoint.v );
			
			if ( thePane != nil && thePane == mView ) {
			
				// Draw the cursor location in local,
				// port, image, and global coordinates.
				if ( FocusDraw() ) {
				
					// Get the frame rect.
					Rect	theFrame;
					CalcLocalFrameRect( theFrame );
					
					// Inset it to account for the border.
					::InsetRect( &theFrame, 1, 1 );
					
					// Setup the clip region.
					StClipRgnState	theClipState( theFrame );
					
					// Erase the pane.
					::EraseRect( &theFrame );
					
					// Setup the text characteristics.
					StTextState::Normalize();
					::TextFont( kFontIDGeneva );
					::TextSize( 9 );
					
					// Setup the pen position.
					Point	thePenPos = topLeft(theFrame);
					thePenPos.h += 4;
					thePenPos.v += 12;
					
					// Get the point in local coordinates.
					mView->PortToLocalPoint( thePoint );
					
					Float64 hor, ver;
					hor=(Float64)(thePoint.h-CTempView::sxwindmin)/(CTempView::sxwindmax-CTempView::sxwindmin)*(CTempView::sxmax-CTempView::sxmin) + CTempView::sxmin;
					ver = ((Float64)(CTempView::sywindmin - CTempView::sywindmax))/((Float64)CSpike6App::snTemplates);
					ver = trunc(((Float64)thePoint.v-5.0)/ver);
					SInt32 vert=round(ver);
					// Draw the view info.
					//OurDrawTypes::ShowDataPoints( mView, myPoint, thePenPos );
					::MoveTo(thePenPos.h, thePenPos.v);
					::ForeColor(redColor);
					LStr255 str(hor,"\p########.###");
    				::DrawString(str);
					::DrawString("\p, ");
					LStr255 str1(vert,"\p######");
					::DrawString(str1);
				}

			} else {
			
				// Erase the info.
				if ( FocusDraw() ) {
				
					// Get the frame rect.
					Rect	theFrame;
					CalcLocalFrameRect( theFrame );
					
					// Inset it to account for the border.
					::InsetRect( &theFrame, 1, 1 );
					
					// Erase the pane.
					::EraseRect( &theFrame );

				}

			}
		
		}
		
	}
}


// ---------------------------------------------------------------------------------
//		¥ DrawSelf
// ---------------------------------------------------------------------------------

void
CViewInfoPane1::DrawSelf()
{
	// Calculate the frame rect.
	Rect	theFrame;
	CalcLocalFrameRect( theFrame );
	
	// Erase the pane.
	::EraseRect( &theFrame );
	
	// Draw a frame around the pane.
	::FrameRect( &theFrame );
}
