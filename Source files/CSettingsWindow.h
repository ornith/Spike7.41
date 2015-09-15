// =================================================================================//	CProgressWindow.h				�1995-1998  Metrowerks Inc. All rights reserved.// =================================================================================//	CProgressWindow.cp	#pragma once#include <LWindow.h>#include <LListener.h>class CSettingsWindow : public LWindow, public LListener{public:	enum { class_ID = 'SetW' };								CSettingsWindow();								CSettingsWindow( const SWindowInfo &inWindowInfo );								CSettingsWindow( ResIDT inWINDid,									UInt32 inAttributes, LCommander *inSuperCommander );								CSettingsWindow( LStream *inStream );								~CSettingsWindow();									virtual void		FindCommandStatus(								CommandT			inCommand,								Boolean				&outEnabled,								Boolean				&outUsesMark,								UInt16				&outMark,								Str255				outName);	virtual void				ListenToMessage( MessageT inMessage, void *ioParam );	virtual Boolean		HandleKeyPress(const EventRecord	&inKeyEvent);		void  	SetDefaultButton(PaneIDT	inButtonID);	void	HandleError(SInt32 ernum);			static LArray *DrawCol1;	static LArray *DrawCol2;	static LArray *DrawCol3;	static LArray *DrawCol4;	static LArray *DrawCol5;	static LArray *DrawCol6;	static LArray *DrawCol7;	static LArray *DrawCol8;	static LArray *DrawCol9;			static Boolean	mHasChanged, DrawLines, DrawDots, ZeroXOrigin, ZeroYOrigin, TimeCourseFlag, HistTimeCourseFlag, ExportMeanSD;	static Float64 uplim, lolim, uplim05, lolim05;    protected:	PaneIDT			mDefaultButtonID;	//LDefaultOutline	*mDefaultOutline;	PaneIDT			mLastButton;	virtual void	FinishCreateSelf();	void			PrepareArrays();	void			LumpUnits();	void			UpdateRawStats();	void 			ResetSettings();	void			MakeTimeCourse();	void			GetSettings();	void			RenumberTemplates(SInt32 ttempl2);    void            Write2DArray(SInt32 **theArray, UInt32 sze1, UInt32 sze2, LStr255 *inString);    void            SaveArray(LArray *inArray, LStr255 *inString);	//char *			WritePlotData(SInt32 *dLength, bool myflag);};