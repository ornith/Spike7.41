// =================================================================================
//	CCombEvents.h				�1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CCombEvents.cp	

#pragma once

#include <LWindow.h>
#include <LListener.h>


class CCombEvents : public LWindow, public LListener{
public:
	enum { class_ID = 'CCEv' };

								CCombEvents();
								CCombEvents( const SWindowInfo &inWindowInfo );
								CCombEvents( ResIDT inWINDid,
									UInt32 inAttributes, LCommander *inSuperCommander );
								CCombEvents( LStream *inStream );
								~CCombEvents();

	virtual void				ListenToMessage( MessageT inMessage, void *ioParam );
	
protected:
	virtual void				FinishCreateSelf();
};
