// =================================================================================
//	CSpike6App.h	 				©1996-1998 Metrowerks Inc. All rights reserved.
// =================================================================================

#pragma once

//#include <PP_Prefix.h>
#include <LDocApplication.h>
#include <LString.h>
//#include <PP_Resources.h>
//#include <UDesktop.h>

class CScrollWheelHandler;
class LString;
class CSpike6App : public PP_PowerPlant::LDocApplication {

public:
							CSpike6App();			// constructor registers PPobs
	virtual 				~CSpike6App();		// stub destructor

	virtual Boolean		ObeyCommand(PP_PowerPlant::CommandT inCommand, void* ioParam);	
	
	
	// this overriding method returns the status of menu items
	virtual void		FindCommandStatus(PP_PowerPlant::CommandT inCommand,
									Boolean &outEnabled, Boolean &outUsesMark,
									PP_PowerPlant::UInt16 &outMark, Str255 outName);

	virtual void Initialize();
	static LArray *sAvgCol1;
	static LArray *sAvgCol2;
	static LArray *sAvgCol3;
	static LArray *sAvgCol4;
	static LArray *sWavs;
	static LArray *theWaves;
	static LArray *tRecord;
	static LArray *bRecord;
	static LArray *theHistogram;
	static LArray *theAvgs;
	static SInt32 sMaxPlus;
	static SInt32 sMaxMin;
	
	static Handle BinFile;
	static Boolean endianflag;
	typedef struct waveform{SInt16 wavform[64];
					Float64 timestamp;};
	//typedef struct waveform waveform;

	typedef struct templrecrd{
		Float64 timestamp;
		SInt16 templatenum;
		SInt16 plusval;
		SInt16 minval;};
	//typedef struct templrecrd templrecrd;
	typedef struct BinRecord{Float64 bins[500];};
	//typedef struct BinRecord BinRecord;
	typedef struct BinRecordLong{Float64 bins[1000];};
	//typedef struct BinRecordLong BinRecordLong;

	//LArray *s1Array;
	//LArray *s2Array;
	//LArray *s3Array;
	//LArray *s4Array;

	static UInt32 	sAvDim2,sAvDim3,sdim1,sdim2,sdim3,sFType;
	static UInt32 	sdrawdim2, sdrawdim3;
	static SInt32	snTemplates;
	static Str63	sFileName;
	static LStr255 *FileNameBase;
	
	typedef struct 	{
					SInt32 templ1;
					SInt32 ntempl1;//occurrences within selected time period
					SInt32 templ2;
					SInt32 ntempl2;//occurrences within selected time period
					SInt32 minlin;
					SInt32 maxlin;
					SInt32 brklin;
					SInt32 brklin2;
					Float64 strt;
					Float64 stp;
					Float64 brkpt;
					Float64 brkpt2;
					Float64 BWdth;
					Float64 ExpoLag;
					SInt32 maxbns;
					Boolean BinAvailable;//true if the .bin file is available for plotting waveforms
					} SpParams;
	static SpParams SpikeParams;
	
	typedef struct histo{
	SInt32 histdata[64];
	};
	//typedef histo histo;
	
protected:

	LWindow *mSettWindow;
	LWindow	*theMonteWindow;
	LWindow	*mStatsWindow;
	LWindow	*mSpikeGenerator;
	LWindow	*mSumWindow;
	
	virtual void			StartUp();				// override startup functions
	
	// this overriding method is called when the FileMenu ->Open items is selected.
	virtual void			OpenDocument( FSSpec *inMacFSSpec );
	
	// this method creates a new instance of a document
	//virtual PP_PowerPlant::LModelObject *	MakeNewDocument();
	
	// this method displays the StandardFile Open dialog
	virtual void			ChooseDocument();
	
	// this method prints the open document
	virtual void			PrintDocument( FSSpec *inMacFSSpec );
	void	SaveFile();
	void	OpenFile( FSSpec &inFileSpec );
	void	MakeSpikeArray(char *Datpt);
	void	MakeSpikeArrayFST(char *Datpt);
	void 	MakeArraysFromBS2(char *Datpt);
	void 	MakeArraysFromMacBS2(char *Datpt);
	//void	MakemacBS2();

	//void	MakeRawStats();
	
	void	SetUpAnalysis();
	
	void	DoFileIOAlert();
	
	void	CloseAllWindows();
	
private:
	StDeleter<CScrollWheelHandler> mScrollWheelHandlerOwner;

};
