// =================================================================================//	CViewInfoPane3.cp 				�1995-1998 Metrowerks Inc. All rights reserved.// =================================================================================#include <LView.h>#include <LWindow.h>#include <UDesktop.h>#include <UDrawingState.h>#include "CViewInfoPane3.h"#include "CBig3.h"// ---------------------------------------------------------------------------------//		� CViewInfoPane3(LStream*)// ---------------------------------------------------------------------------------CViewInfoPane3::CViewInfoPane3(	LStream	*inStream )		: LPane( inStream ){	// Initialize the view and last point.	mView = nil;	mLastPt.h = mLastPt.v = 0;}// ---------------------------------------------------------------------------------//		� ~CViewInfoPane3// ---------------------------------------------------------------------------------CViewInfoPane3::~CViewInfoPane3(){}// ---------------------------------------------------------------------------------//		� RegisterView// ---------------------------------------------------------------------------------voidCViewInfoPane3::RegisterView(	LView	*inView ){	// Store the view and start idling.	mView = inView;	StartIdling();}// ---------------------------------------------------------------------------------//		� SpendTime// ---------------------------------------------------------------------------------voidCViewInfoPane3::SpendTime(	const EventRecord &inMacEvent ){#pragma unused( inMacEvent )	// Fetch the top window if there is one.	LWindow	*theWindow = UDesktop::FetchTopRegular();		if ( theWindow != nil ) {			// Another pane may have changed the origin.		// Since GetMouse gets coordinates in port coordinates, we		// have to reset the window origin.		::SetOrigin( 0, 0 );			// Get the mouse coordinates.		Point	thePoint;		::GetMouse( &thePoint );				// Tell LView to purge it's cache of the currently focused		// view since we changed the origin ourselves.		LView::OutOfFocus( nil );			if ( !::EqualPt( thePoint, mLastPt ) ) {					// Update the last point.			mLastPt = thePoint;						// Find the deepest pane containing the port coordinates.			LPane	*thePane;			thePane = theWindow->FindDeepSubPaneContaining( thePoint.h, thePoint.v );						if ( thePane != nil && thePane == mView ) {							// Draw the cursor location in local,				// port, image, and global coordinates.				if ( FocusDraw() ) {									// Get the frame rect.					Rect	theFrame;					CalcLocalFrameRect( theFrame );										// Inset it to account for the border.					::InsetRect( &theFrame, 1, 1 );										// Setup the clip region.					StClipRgnState	theClipState( theFrame );										// Erase the pane.					::EraseRect( &theFrame );										// Setup the text characteristics.					StTextState::Normalize();					::TextFont( kFontIDGeneva );					::TextSize( 9 );										// Setup the pen position.					Point	thePenPos = topLeft(theFrame);					thePenPos.h += 4;					thePenPos.v += 12;										// Get the point in local coordinates.					mView->PortToLocalPoint( thePoint );										//Point myPoint;					Float64 hor, ver;					hor=(Float64)(thePoint.h-CBig3::sxwindmin)/(CBig3::sxwindmax-CBig3::sxwindmin)*(CBig3::sxmax-CBig3::sxmin) + CBig3::sxmin;					ver=(CBig3::sywindmin-thePoint.v)/CBig3::syscale +CBig3::symin;					// Draw the view info.					//OurDrawTypes::ShowDataPoints( mView, myPoint, thePenPos );					::MoveTo(thePenPos.h, thePenPos.v);					::ForeColor(redColor);					LStr255 str(hor,"\p######.###");    				::DrawString(str);					::DrawString("\p, ");					LStr255 str1(ver,"\p######.###");					::DrawString(str1);				}			} else {							// Erase the info.				if ( FocusDraw() ) {									// Get the frame rect.					Rect	theFrame;					CalcLocalFrameRect( theFrame );										// Inset it to account for the border.					::InsetRect( &theFrame, 1, 1 );										// Erase the pane.					::EraseRect( &theFrame );				}			}				}			}}// ---------------------------------------------------------------------------------//		� DrawSelf// ---------------------------------------------------------------------------------voidCViewInfoPane3::DrawSelf(){	// Calculate the frame rect.	Rect	theFrame;	CalcLocalFrameRect( theFrame );		// Erase the pane.	::EraseRect( &theFrame );		// Draw a frame around the pane.	::FrameRect( &theFrame );}