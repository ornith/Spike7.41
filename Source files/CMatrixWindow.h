// =================================================================================
//	CMatrixWindow.h				�1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CProgressWindow.cp	

#pragma once

#include <LWindow.h>
#include <LListener.h>

class CMatrixWindow : public LWindow, public LListener {
public:
	enum { class_ID = 'MatW' };

								CMatrixWindow();
								CMatrixWindow( const SWindowInfo &inWindowInfo );
								CMatrixWindow( ResIDT inWINDid,
									UInt32 inAttributes, LCommander *inSuperCommander );
								CMatrixWindow( LStream *inStream );
								~CMatrixWindow();
								
	virtual void				ListenToMessage( MessageT inMessage, void *ioParam );	
	void  	SetDefaultButton(PaneIDT	inButtonID);
	Boolean	HandleKeyPress(const EventRecord	&inKeyEvent);	
	
	static SInt32 typematrix;

protected:
	PaneIDT				mDefaultButtonID;
	LDefaultOutline		*mDefaultOutline;

	virtual void		FinishCreateSelf();
};
