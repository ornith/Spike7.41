// =================================================================================//	CProgressWindow.h				�1995-1998  Metrowerks Inc. All rights reserved.// =================================================================================//	CProgressWindow.cp	#pragma once#include <LWindow.h>#include <LListener.h>#include <LPeriodical.h>class CMonteWindow : public LWindow, public LListener, public LPeriodical {public:	enum { class_ID = 'MonW' };								CMonteWindow();								CMonteWindow( const SWindowInfo &inWindowInfo );								CMonteWindow( ResIDT inWINDid,									UInt32 inAttributes, LCommander *inSuperCommander );								CMonteWindow( LStream *inStream );								~CMonteWindow();	virtual void				ListenToMessage( MessageT inMessage, void *ioParam );	virtual	void				SpendTime( const EventRecord &inMacEvent );	virtual void				SetBusyState( Boolean inBusy );	virtual Boolean				GetBusyState() const {return mBusy;};			static LArray 				*simCol1;	static LArray 				*simCol2;	static LArray 				*simCol3;	static LArray				*RXY1;	static LArray				*RXY2;	static LArray				*BreakArray2;	static SInt32				sDrawDim, order, ARMAFlag1;					PP_PowerPlant::LStr255 		Fl2Str(Float64 inval, SInt32 decplac, SInt32 totplac);	static SInt32				nIntervals, nRuns, cSpikes, cRuns, sN1, sN2, sSum1, sSum2, theOrd;	static Float64				KWStat, KSProb, PermProb, MedianStat;	static Float64				Average1, Average2, Stdev1, Stdev2, SumAvg1, SumSd1,SumAvg2, SumSd2;	static Boolean				DrawFlag1, DrawFlag2, DrawFlag3;	static Boolean				unifs, norms, logns, expos;protected:	Boolean						mBusy;	virtual void				FinishCreateSelf();	void 						DrawStats();	void						PrepArrays();};