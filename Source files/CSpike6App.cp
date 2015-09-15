// =================================================================================
//	CSpike6App.cp					©1996-1998 Metrowerks Inc. All rights reserved.
// =================================================================================
//	This file contains the starter code for a document PowerPlant project
//
//  The Application is derived from LDocApplication.  New Windows are managed
//  by CDatDocument which is derived from LSingleDoc.  Each document contains
//  a smart text view that remembers if it has changed since it was last saved.


//7.42beta 23.09.2014 Added an alternative to allalldensity where less that one event per bin is accepted (see 7.33)
//7.41  20.06.2013 In AllAllDensities, the significance of the relationship is tested with chi-squared of
//  central and control bins (4 in all. When chisquared exceeds 7.82 (5% level), we take the measurement
//  in the matrix, else it is set to zero
//7.4	11.04.2013 Bug fixes.
//7.39  26.02.2013 Fixed the plotting of the Ranked Return Plot. Added in CSettingsWindow (and Analyze11)the option to save the raster plot data both as mean and standard deviation of the bins and as the actual raster.
//7.38  28.02.2013 Fixed the saving of the raster plot data. Only the positive intervals were saved, now both negative and positive intervals are saved.
//7.37  16.10.2012 Repaired calculations of the correlation matrices. 
//		The central bins are 71 - 79 (Nleft) and 82 - 90 (Nright). 9 bins on each side
//   	The control bins are 1 - 9 (Cleft) and 152 - 160 (Cright).
//7.36  18.09.2012 Repaired bug in "Combination that caused the wrong unit to be saved.
//      
//7.35  13.09.2012 Made "Combination" so, that templ1 is acually removed and put into new cluster when it is followed (preceded) by templ2
//7.34  20.08.2012 Fixed bug concerning the counts entered in the counts matrix

//7.33  13.08.2012 Changed the way the correlation matrix between events is calculated (in AllAllDensities)from PSTHs. 
//      Calculation is now based on 2*80 bins, each 50 msec. 
//      The central bins are 71 - 79 (Nleft) and 82 - 90 (Nright).
//      The control bins are 1 - 9 (Cleft) and 152 - 160 (Cright).
//      Data are considered valid only if each bin contains on average 1 or more events (total > 180 events).
//      General response strength: 
//      ((Nleft + Nright)-(Cleft + Cright))/((Nleft + Nright)+(Cleft + Cright)).
//      Right-looking strength: (Nright - Cleft)/(Nright + Cleft). Left-looking: (Nleft - Cright)/(Nleft + Cright). Left and right looking matrices
//      are mirror images. Only righ-looking is shown in the Matrixwindow. Finally we have directionality:
//      (Nright - Nleft)/(Nright + Nleft)
//      all indices are * 100, varying from + 1 to -1 for perfect positive to perfect negative correspondence 

//7.32  05.06.2012 Removed directional2 from the matrix options. Added saving of the  
//  number of events in each cell of the matrix after the selected matrix is saved.

//7.31  02.04.2012 Use last and first 48 bins to estimate baseline, to enhance symmetry

//7.3   17.01.2012 Better calculations of directional effects and cross correlations in AllAllDensities
//          The Combination window takes negative time values to select events that are preceded by something

//7.26  21.12.2011 Changed AllAllDensities, to have the option to look at directional effects. Fixed bug in saveastext (Analyze1.1) that was a result of this change

//7.25  22.11.2011 Changed output of All Densities so that all viable combinations yield a number

//7.24  19.10.2011 Added option in Settingswindow to produce a matrix of all density combinations. 
//      Resolved issue in the Summary Window where template numbers were not aligned with graphs
//      Made wave drawing window larger to make copies at better resolution.

//7.23  20.09.2011 Added option in "combination" to replace the occurrences of old unit that satisfy the criteria with 
//      the occurrences of the new ones.

//7.22  01.08.2011 Fixed bug in the generation of "combination" units

//7.21  Changed "combination" window. When the checkbox "all events" is checked, the events that occur within the 
//      specified time from the source template, are recorded as a new template. When it is unchecked, the source events that
//      are followed by the target event are saved

//7.20  June 2011. added time course description to the histogram option

//7.19  small fixes of findlines in analyze1

//7.18 various fixes. The combination tool for templates is fixed.

//7.17  added "accessories" menu. Methods to find occasions where two events occur within a specified time. 
//		These are stored as an additional unit/cluster/template.

//7.16  Histogram works again for spike pairs. Alldens and Cusum in stats rewritten. 

//7.15  The max and min value in the plots made by CSettingWindow have been adjusted for easier comparison and counting

//7.14  In the time course plot of the AllDensity, added plotting of the source events in red

//7.13  Fixed distribution plot. Added function SortFloatLArray(xx) to circumvent problems with LArray sorting.
//		Fixed Spike train generator which had various bugs. Added possibility to set average ISI

//7.11  Changed the way the views in settingwindow and statswindow are shown (scales, mainly)

//7.1   Made changes so that the cluater numbering of syllables conforms that of Sound Explorer. Fixed some bugs too.

//7.082 fixed the spiketrain window to give correct reading of the unit number pointed at.

//7.081 increased size of myptr in writeplotdata in CSettingsWindow.cp to accommodate large datasets.

//7.07      7.3.2007 Implemented gsl-histogram routines. Use of scroll wheel implemented. Spike7.04

// Nov 2006-Jan 2007 ported to xcode. Spike7.02-7.03

// 31.08.06 Ported to XCode started version 7

// Implemented GSL routines for randomization in Analyze 2.2, MonteCarlo and SpikeGenWindow

// Debugged Analyze 2.2: K-W, K-S and Permutation test; RankedReturn, 3iRR

// June 28 2004 Made scale indication for histograms in settingswindow.

//this is a carbonized version of Spike4

#include "CSpike6App.h"

#include <LGrowZone.h>
#include <PP_Messages.h>
#include <PP_Resources.h>
#include <UDrawingState.h>
#include <UMemoryMgr.h>
#include <URegistrar.h>
#include <UEnvironment.h>
//#include <UAttachments.h>
//#include <LDocument.h>
#include <UNavServicesDialogs.h>

#include <UStandardDialogs.h>
#if PP_StdDialogs_Option == PP_StdDialogs_Conditional
	#include <UConditionalDialogs.h>
#endif
#include <UControlRegistry.h>
#include <LGroupBox.h>
#include <Math.h>

#include "endian.h"

#include "Analyze1.h"
#include "CAvgTable.h"
#include "CBig1.h"
#include "CTimView.h"
#include "CViewInfoPane.h"
#include "CViewInfoPane1.h"
#include "CViewInfoPane2.h"
#include "CViewInfoPane3.h"

#include "CSettingsWindow.h"
#include "CStatsWindow.h"
#include "CStatsTable.h"
#include "CBig2.h"
#include "CBig3.h"
#include "CBig4.h"
#include "CWavPlot.h"
#include "CMonteWindow.h"
#include "CSpikeGenWindow.h"
#include "CAllView.h"
#include "CTempView.h"
#include "MonteCarlo.h"
#include "CSumView.h"
#include "CSumWindow.h"
#include "CSpikeTrainWindow.h"
#include "CCombEvents.h"

#include "CMatrixWindow.h"
#include "CMatrixTable.h"

#include "CScrollWheelHandler.h"
#include <LEventHandler.h>

#include "SwapUtils.h"

#include "SpikeConstants.h"

LArray *CSpike6App::sAvgCol1=nil;
LArray *CSpike6App::sAvgCol2=nil;
LArray *CSpike6App::sAvgCol3=nil;
LArray *CSpike6App::sAvgCol4=nil;
LArray *CSpike6App::sWavs=nil;
LArray *CSpike6App::theWaves=nil;
LArray *CSpike6App::tRecord=nil;
LArray *CSpike6App::bRecord=nil;
LArray *CSpike6App::theHistogram;
LArray *CSpike6App::theAvgs=nil;

LStr255 *CSpike6App::FileNameBase=nil;

SInt32	CSpike6App::sMaxPlus=0.0;
SInt32	CSpike6App::sMaxMin=0.0;
Str63 	CSpike6App::sFileName;
UInt32 	CSpike6App::sdim2=0.0;
UInt32 	CSpike6App::sdim3=0.0;
UInt32	CSpike6App::sAvDim2=0;
UInt32 	CSpike6App::sAvDim3=0;
UInt32 	CSpike6App::sdrawdim2=0;
UInt32 	CSpike6App::sdrawdim3=0;
UInt32 	CSpike6App::sFType=0;
SInt32 	CSpike6App::snTemplates=0;
Handle 	CSpike6App::BinFile=nil;
Boolean CSpike6App::endianflag=nil;

CSpike6App::SpParams CSpike6App::SpikeParams;

struct CSpike6App::waveform;
struct CSpike6App::templrecrd;
struct CSpike6App::histo;

pascal Boolean mymacbs2FilterProc(AEDesc* theItemAEDesc, void* info,
                                void* callBackUD,
                                NavFilterModes filterModes);
pascal Boolean mymacbin2FilterProc(AEDesc* theItemAEDesc, void* info,
                                void* callBackUD,
                                NavFilterModes filterModes);



// =================================================================================
//		¥ Main Program
// ===========================================================================

int main()
{
								
	SetDebugThrow_(debugAction_Alert);	// Set Debugging options
	SetDebugSignal_(debugAction_Alert);

	InitializeHeap(3);					// Initialize Memory Manager
														// Parameter is number of Master Pointer
														// blocks to allocate
	
	UQDGlobals::InitializeToolbox();	// Initialize standard Toolbox managers
	
	new LGrowZone(20000);				// Install a GrowZone function to catch
														// low memory situations.

	CSpike6App	theApp;								// create instance of your application
	theApp.Run();
	
	return 0;
}


// ---------------------------------------------------------------------------------
//		¥ CSpike6App
// ---------------------------------------------------------------------------------
//	Constructor

CSpike6App::CSpike6App()
{
	if (UEnvironment::HasFeature(env_HasAppearance)) {
		::RegisterAppearanceClient();
	}
	#if PP_Target_Carbon
									// AM always present on Carbon
		UControlRegistry::RegisterAMClasses();
		
	#else
	
		UControlRegistry::RegisterClasses();
		
	#endif
	RegisterClass_(LTabGroup);
	RegisterClass_(LView);
	RegisterClass_(LWindow);
	RegisterClass_(LRadioGroup);
	RegisterClass_(CAvgTable);
	RegisterClass_(CStatsTable);
	RegisterClass_(CBig1);
	RegisterClass_(CTimView);
	RegisterClass_(CBig2);
	RegisterClass_(CBig3);
	RegisterClass_(CBig4);
	RegisterClass_(CWavPlot);
	RegisterClass_(CViewInfoPane);
	RegisterClass_(CViewInfoPane1);
	RegisterClass_(CViewInfoPane2);
	RegisterClass_(CViewInfoPane3);
	RegisterClass_(CSettingsWindow);
	RegisterClass_(CStatsWindow);
	RegisterClass_(CMonteWindow);
	RegisterClass_(CAllView);
	RegisterClass_(CTempView);
	RegisterClass_(CSpikeGenWindow);
	RegisterClass_(CSpikeTrainWindow);
	RegisterClass_(CSumView);
	RegisterClass_(CSumWindow);
	RegisterClass_(CCombEvents);
	RegisterClass_(CMatrixWindow);
	RegisterClass_(CMatrixTable);

}


// ---------------------------------------------------------------------------------
//		¥ ~CSpike6App
// ---------------------------------------------------------------------------------
//	Destructor

CSpike6App::~CSpike6App()
{
	PP_PowerPlant::PP_StandardDialogs::Unload();	// Clean up after std dialogs
}


// ---------------------------------------------------------------------------------
//		¥ StartUp
// ---------------------------------------------------------------------------------
//	This function lets you do something when the application starts up
//	without a document. For example, you could issue your own new command.

void
CSpike6App::StartUp()
{
ObeyCommand(cmd_Open, nil);

}

void
CSpike6App::Initialize()
{
	LDocApplication::Initialize();
	/*if (CFM_AddressIsResolved_(InstallStandardEventHandler))
		{
				//AddAttachment(new CScrollWheelAttachment);
		}*/
	mScrollWheelHandlerOwner.Adopt(new CScrollWheelHandler);

}

// ---------------------------------------------------------------------------
//		¥ ObeyCommand
// ---------------------------------------------------------------------------
//	This method lets the application respond to commands like Menu commands

Boolean CSpike6App::ObeyCommand
(
	PP_PowerPlant::CommandT	inCommand,
	void					*ioParam)
{
	Boolean		cmdHandled = true;
	CSettingsWindow  *theSettWindow=nil;
	CStatsWindow  *theStatsWindow=nil;
	CCombEvents *theCombWindow=nil;
	LColorEraseAttachment *theAttach;
	switch (inCommand) {
		case 2001:{
		theSettWindow = dynamic_cast<CSettingsWindow *>(LWindow::FindWindowByID(rPPob_SettingWindow));
		if (theSettWindow !=nil)
			{
			mSettWindow->Select();
			}
		else
			{
			mSettWindow = LWindow::CreateWindow( rPPob_SettingWindow, this );
			ThrowIfNil_( mSettWindow );
			theAttach = new LColorEraseAttachment();
			AddAttachment(theAttach, nil, true);
				
			mSettWindow->Show();
			}
			break;
			}
			
		case 2002:{
		CStatsWindow  *theStatsWindow=nil;
		theStatsWindow = dynamic_cast<CStatsWindow *>(LWindow::FindWindowByID(rPPob_StatsWindow));

		if (theStatsWindow != nil)
			{
			mStatsWindow->Select();
			}
		else
			{
			mStatsWindow = LWindow::CreateWindow( rPPob_StatsWindow, this );
			ThrowIfNil_( mStatsWindow );
			mStatsWindow->Show();
			}
			break;
			}
		case 2003:{
		CSumWindow  *theSumWindow=nil;
		theSumWindow = dynamic_cast<CSumWindow *>(LWindow::FindWindowByID(rPPob_SumWindow));

		if (theSumWindow != nil)
			{
			mSumWindow->Select();
			}
		else
			{
			mSumWindow = LWindow::CreateWindow( rPPob_SumWindow, this );
			ThrowIfNil_( mSumWindow );
			mSumWindow->Show();
			}
			break;
			}

		case 2011:{
			theSettWindow = dynamic_cast<CSettingsWindow *>(LWindow::FindWindowByID(rPPob_SettingWindow));
			if (theSettWindow !=nil)
				{
				//mSettWindow->Select();
				theSettWindow->DoClose();
				}
			theStatsWindow = dynamic_cast<CStatsWindow *>(LWindow::FindWindowByID(rPPob_StatsWindow));
			if (theStatsWindow !=nil)
				{
				theStatsWindow->DoClose();
				}
			CSpikeGenWindow *theSpikeGenerator;
			theSpikeGenerator = dynamic_cast<CSpikeGenWindow *>(LWindow::FindWindowByID(rPPob_GeneratorWindow));
			if (theSpikeGenerator !=nil)
				{
				mSpikeGenerator->DoClose();
				}
			mSpikeGenerator = LWindow::CreateWindow( rPPob_GeneratorWindow, this );
			ThrowIfNil_(mSpikeGenerator);
			mSpikeGenerator->Show();
			break;
			}
			
		case 2012:{
			theMonteWindow = LWindow::CreateWindow( rPPob_MonteWindow, this );
			ThrowIfNil_( theMonteWindow );
			theMonteWindow->Show();
			break;
		}
		case 2013:{//combine templates that occur within a specified window
			theCombWindow = dynamic_cast<CCombEvents *>(LWindow::FindWindowByID( rPPob_Combination));
			if (theCombWindow != nil){
				theCombWindow->Select();
			} else {
				theCombWindow = dynamic_cast<CCombEvents *>(LWindow::CreateWindow( rPPob_Combination, this ));
				ThrowIfNil_( theCombWindow );
				theCombWindow->Show();
			}
			break;
		}
		case cmd_Save:{
			SaveFile();
			break;
		}
		default:{
			cmdHandled = PP_PowerPlant::LDocApplication::ObeyCommand(inCommand, ioParam);
			break;
		}
	}
	
	
	return cmdHandled;
}

// ---------------------------------------------------------------------------
//		¥ FindCommandStatus
// ---------------------------------------------------------------------------
//	This method enables menu items.

void
CSpike6App::FindCommandStatus(
	PP_PowerPlant::CommandT	inCommand,
	Boolean					&outEnabled,
	Boolean					&outUsesMark,
	PP_PowerPlant::UInt16	&outMark,
	Str255					outName)
{

	switch (inCommand) {
	
		// Return menu item status according to command messages.
		case PP_PowerPlant::cmd_New:
			outEnabled = false;
			break;
		case PP_PowerPlant::cmd_Open:
		case PP_PowerPlant::cmd_Save:
			outEnabled = true;
			break;
		case 2001:
		case 2002:
		
			if (tRecord!=nil){
				outEnabled = true;
				}
			else{
				outEnabled = false;
				}
		break;
		case 2003:
			if ((tRecord!=nil)&&(theWaves != nil)){
				outEnabled = true;
				}
			else{
				outEnabled = false;
				}
		break;
		case 2011:
		case 2012:
		case 2013:
		outEnabled = true;
		break;

		// Any that you don't handle, such as cmd_About and cmd_Quit,
		// will be passed up to LApplication
	default:
		PP_PowerPlant::LDocApplication::FindCommandStatus(inCommand, outEnabled,
												outUsesMark, outMark, outName);
		break;
	}
}

// SaveFile

void
CSpike6App::SaveFile()
{
	//now ask savefile etc.
	Handle myspikehandle=tRecord->GetItemsHandle();
	SInt32 datLength=GetHandleSize(myspikehandle);
	//Str63 mstr="\pmyFile.macbs2";
	//ConstStringPtr OutFileName=mstr;
	FileNameBase->Append("\p.LMP.macbs2");
	bool replac =true;
	FSSpec OutFileSpec;
	if (UNavServicesDialogs::AskSaveFile(*FileNameBase,FOUR_CHAR_CODE('data'),OutFileSpec,replac,
		kNavDefaultNavDlogOptions + kNavSelectAllReadableItem)){
		LFile *	theOutFile=new LFile(OutFileSpec);
		SwapTRec(tRecord);
		char *thePtr=*myspikehandle;
		if (!(replac)){
			OSType		OutCreator=FOUR_CHAR_CODE('atmD');
			theOutFile->CreateNewDataFile(OutCreator, FOUR_CHAR_CODE('atm6'), nil);
			}
		theOutFile->OpenDataFork(fsRdWrPerm);
		theOutFile->WriteDataFork(thePtr,datLength);
		theOutFile->CloseDataFork();
		SwapTRec(tRecord);
		//DisposeHandle(myspikehandle);
		}
	
	}

// ---------------------------------------------------------------------------------
//		¥ OpenDocument
// ---------------------------------------------------------------------------------
// This method is called when a file is chosen from the StandardFile Open Dialog
// File_Menu->Open item.

void
CSpike6App::OpenDocument(
	FSSpec	*inMacFSSpec )
{
OpenFile(*inMacFSSpec);	
}




// ---------------------------------------------------------------------------------
//		¥ ChooseDocument
// ---------------------------------------------------------------------------------
// This method uses the PowerPlant Standard Dialogs to let the user choose a
// document to open.

void
CSpike6App::ChooseDocument()
{
	CloseAllWindows();
	PP_StandardDialogs::LFileChooser	chooser;
	
		// Open any/all TEXT files
	
	NavDialogCreationOptions*	options = chooser.GetDialogOptions();
	if (options != nil) {
		options->optionFlags =	kNavDefaultNavDlogOptions;
										+ kNavSelectAllReadableItem;
	}
	chooser.SetObjectFilterProc(mymacbs2FilterProc);
	if (chooser.AskOpenFile(LFileTypeList(FOUR_CHAR_CODE(fileTypes_All)))) {
		AEDescList		docList;
		chooser.GetFileDescList(docList);
		SendAEOpenDocList(docList);
	}
}


// ---------------------------------------------------------------------------------
//		¥ PrintDocument
// ---------------------------------------------------------------------------------
// This method is called when the FileMenu->Print item is chosen.

void
CSpike6App::PrintDocument(
	FSSpec	*inMacFSSpec )
{
;	// Create a new document using the file spec.
}
// ---------------------------------------------------------------------------------
//		¥ OpenFile
// ---------------------------------------------------------------------------------

void 
          
{
	char *pntwo;
	SInt32 *numm;
	char *flta;	
	SInt32 hsze=0;
	//mFile = nil;
	LStr255 mystring;
	Boolean rdflag=false;
	
	
	LFile *theFile=new LFile(inFileSpec);
	delete tRecord;
	tRecord=nil;
	
	
	//get filename
	FSSpec *FilSpec=&inFileSpec;
	LString::CopyPStr(FilSpec->name,CSpike6App::sFileName, sizeof(Str63));
	//const unsigned char *st="\p.BS2";
	LStr255 *st=new LStr255 (CSpike6App::sFileName);
	FileNameBase=new LStr255(CSpike6App::sFileName);
	UInt8 ll=1,mm=1;
	mm=FileNameBase->Length();
	UInt8 startpos=FileNameBase->ReverseFind(".",ll);
	mm=mm-startpos+1;
	FileNameBase->Remove(startpos,mm);

	// Open the data fork.
	theFile->OpenDataFork( fsRdWrPerm );
	
	// Read the entire file contents and close the file.
	Handle theDatH=theFile->ReadDataFork();
	
	theFile->CloseDataFork();
	

	pntwo = *theDatH;
	numm = (SInt32*)pntwo;
		
	if ((st->EndsWith("\p.BS2"))||(st->EndsWith("\p.bs2")))//if name ends with ".BS2 or .bs2//pay attention!!!
		{
		rdflag=true;
		hsze=::GetHandleSize(theDatH);
		CSpike6App::sdim3 = hsze/14;
		CSpike6App::sdim2 = 4;
		flta = (char *)(*theDatH);
		tRecord=new LArray(sizeof(templrecrd),theDatH);
		MakeArraysFromBS2(flta);
		MakeRawStats();
		::DisposeHandle( theDatH );	
		theDatH=nil;
		}
	else if (st->EndsWith("\p.macbs2"))
		{
		rdflag=true;
		hsze=::GetHandleSize(theDatH);
		CSpike6App::sdim3 = hsze/16; //16 instead of 14 because of the size of the struct that was used in princcomp!!!!
		CSpike6App::sdim2 = 4;
		tRecord=new LArray(sizeof(templrecrd),theDatH);
		SwapTRec(tRecord);
		flta = (char *)(*theDatH);
		sdim3=tRecord->GetCount();//array is now made
		MakeRawStats();
		//MakeArraysFromMacBS2(flta);
		//::DisposeHandle( theDatH );	
		//theDatH=nil;
		}
	else 
		{
		rdflag=false;
		DoFileIOAlert();
		return;
		}
	delete theFile;
	theFile=nil;
	
	///Hier gaan we de .BIN-file lezen
	PP_StandardDialogs::LFileChooser	chooser;
	
		// Open any files
	
	NavDialogCreationOptions*	options = chooser.GetDialogOptions();
	if (options != nil) {
		options->optionFlags =	kNavDefaultNavDlogOptions
										+ kNavSelectAllReadableItem;
	}
	chooser.SetObjectFilterProc(mymacbin2FilterProc);
	if (chooser.AskOpenFile(LFileTypeList(FOUR_CHAR_CODE(fileTypes_All)))) {
		::DisposeHandle(BinFile);
		BinFile=nil;
		SpikeParams.BinAvailable=true;
		FSSpec filspec;
		waveform wvfrm;
		chooser.GetFileSpec(1,filspec);
		Str63 binfilename;
		LString::CopyPStr(filspec.name,binfilename, sizeof(Str63));
		LStr255 *filestring=new LStr255(binfilename);
		if ((filestring->EndsWith("\p.BIN"))||
					(filestring->EndsWith("\p.bin"))||
					(filestring->EndsWith("\p.macbin2"))){	
					delete theWaves;
			theWaves=nil;	
			theFile=new LFile(filspec);
			// Open the data fork.
			theFile->OpenDataFork( fsRdWrPerm );
			// Read the entire file contents and close the file.
			BinFile=theFile->ReadDataFork();
			theFile->CloseDataFork();
			if (filestring->EndsWith("\p.macbin2")){
				endianflag=false;
				theWaves= new LArray(sizeof(waveform),BinFile);
				SwapWaves(theWaves);
				theWaves->FetchItemAt(1,&wvfrm);
				//theWaves->Sort();
				}//do not convert
			else {
				endianflag=true;
				pntwo = *BinFile;
				}		
		//::DisposeHandle( theDatH );	
			delete theFile;
			theFile=nil;
			}
		else {
			SpikeParams.BinAvailable=false;
			DoFileIOAlert();
			}
		}
	mSettWindow = LWindow::CreateWindow( rPPob_SettingWindow, this );
	ThrowIfNil_( mSettWindow );
	mSettWindow->Show();
}


// ---------------------------------------------------------------------------------
// MakeArraysFromBS2 spike sorting array exported from rene's analysis
// ---------------------------------------------------------------------------------
void CSpike6App::MakeArraysFromBS2(char *Datpt)
{
templrecrd theTRec;
ArrayIndexT locc=1;
SInt32 k;
Float64 timev;
UInt16 *templp;
UInt16 *plusp;
UInt16 *minp;
UInt64 *unumm;
UInt64 uinumm;
unumm=(UInt64 *)Datpt;
templp=(UInt16 *)(Datpt+8);
plusp=(UInt16 *)(Datpt+10);
minp=(UInt16 *)(Datpt+12);
SInt16 templv, plusv, minv;
SInt32 incr, incr1, indx,ttemplv,tplusv,tminv;
incr=0;
incr1=0;
for (k=0; k<= (sdim3-1);k++)
	{
	indx=k+1;
	uinumm = *(UInt64*)(Datpt +incr1);
	timev=(Float64)uinumm;//SwapIntelIEEEFloat64(uinumm);
	theTRec.timestamp=timev;
	//locc=sSpikeCol1->AddItem(&timev,sizeof(Float64));
	if (locc==0)
		{locc+=1;}
	templv=*(UInt16*)(Datpt + incr1 + 8);
	//templv=EndianS16_LtoB(templv);
	ttemplv=(SInt32)templv;
	theTRec.templatenum=ttemplv;
	//locc=sSpikeCol2->AddItem(&ttemplv, sizeof(SInt32));
	plusv=*(UInt16*)(Datpt + incr1 + 10);
	//plusv=EndianS16_LtoB(plusv);
	tplusv=(SInt32)plusv;
	theTRec.plusval=tplusv;
	//locc=sSpikeCol3->AddItem(&tplusv,sizeof(SInt32));
	minv=*(UInt16*)(Datpt + incr1 + 12);
	//minv=EndianS16_LtoB(minv);
	tminv=(SInt32)minv;
	theTRec.minval=tminv;
	//locc=sSpikeCol4->AddItem(&tminv,sizeof(SInt32));
	tRecord->AddItem(&theTRec, sizeof(templrecrd));
	incr+=7;
	incr1+=14;
	}
tRecord->Sort();
MakeRawStats();
}

// ---------------------------------------------------------------------------------
//     * MakeAverages
// ---------------------------------------------------------------------------------

/*void  CSpike6App::MakeRawStats()
//makes number of occurrences of each template&average&stdev&minimum intervals}
{	
	SInt32 i,j, noccur, vall;
	SInt32 ttemp, temp;
	//String(255) ThermTitle;
	delete sAvgCol1;
	sAvgCol1=nil;
	delete sAvgCol2;
	sAvgCol2=nil;
	delete sAvgCol3;
	sAvgCol3=nil;
	delete sAvgCol4;
	sAvgCol4=nil;
	sAvgCol1 = new LArray(sizeof(SInt32));	//template #
	ThrowIfNil_(sAvgCol1);
	sAvgCol2 = new LArray(sizeof(SInt32));
	ThrowIfNil_(sAvgCol2);					//occurrences
	sAvgCol3 = new LArray(sizeof(SInt32));
	ThrowIfNil_(sAvgCol3);					//plussize
	sAvgCol4 = new LArray(sizeof(SInt32));
	ThrowIfNil_(sAvgCol4);					//minsize

	j=0;		
	for (i=0; i<=127; i++){//maximum number of templates is 128!
		sAvgCol1->AddItem(&i,sizeof(SInt32));//template number
		sAvgCol2->AddItem(&j,sizeof(SInt32));//#occurrences
		sAvgCol3->AddItem(&j,sizeof(SInt32));//average plusval
		sAvgCol4->AddItem(&j,sizeof(SInt32));//average minval
		}			
	templrecrd theTRec;
	//look for highest template number this yields AvDim3
	noccur=tRecord->GetCount();
	for (i=1; i<=noccur; i++){//accumulate the totals
		tRecord->FetchItemAt(i,&theTRec);
		sAvgCol2->FetchItemAt(theTRec.templatenum+1,&vall);
		vall+=1;
		sAvgCol2->AssignItemsAt(1,theTRec.templatenum+1,&vall);
		sAvgCol3->FetchItemAt(theTRec.templatenum+1,&vall);
		vall+=theTRec.plusval;
		sAvgCol3->AssignItemsAt(1,theTRec.templatenum+1,&vall);
		sAvgCol4->FetchItemAt(theTRec.templatenum+1,&vall);
		vall+=theTRec.minval;
		sAvgCol4->AssignItemsAt(1,theTRec.templatenum+1,&vall);
		}
	for (i=1;i<=128;i++){//find out how many templates there are
		sAvgCol2->FetchItemAt(i,&vall);
		if (vall==0){
			break;
			}
		}
	sAvgCol1->RemoveItemsAt(128,i);//truncates the array to where it contains useful data.
	sAvgCol2->RemoveItemsAt(128,i);
	sAvgCol3->RemoveItemsAt(128,i);
	sAvgCol4->RemoveItemsAt(128,i);*/
	/*ttemp=sAvgCol1->GetCount();
	for(i=1;i<=ttemp; i++){
		sAvgCol2->FetchItemAt(i,&temp);
		sAvgCol3->FetchItemAt(i,&vall);
		vall=round((Float64)vall/(Float64)temp);
		sAvgCol3->AssignItemsAt(1,i,&vall);
		sAvgCol4->FetchItemAt(i,&vall);
		vall=round((Float64)vall/(Float64)temp);
		sAvgCol4->AssignItemsAt(1,i,&vall);
		}
	sAvDim2=4;
	sAvDim3=sAvgCol1->GetCount();
	for (i=1;i<=12;i++){
		sAvgCol1->FetchItemAt(i,&temp);
		sAvgCol2->FetchItemAt(i,&temp);
		sAvgCol3->FetchItemAt(i,&temp);
		sAvgCol4->FetchItemAt(i,&temp);
		}
	i = 128;
	bool going = true;
	while (going){
		sAvgCol2->FetchItemAt(i, &temp);
		if (temp != 0){
			snTemplates = i;
			going = false;
			}
		i -= 1;
		}
	snTemplates = i + 1;
	//snTemplates=sAvDim3;
	}*/
	

void
CSpike6App::DoFileIOAlert(){

	StDialogHandler	theHandler(rPPob_1btnAlert, this);
	LWindow*		theDialog = theHandler.GetDialog();
	Assert_(theDialog != nil);
	theDialog->Show();
	MessageT theMessage;
	LStaticText	*theCaption;
	Rect frame;
	LStr255 mystring;
	mystring.Assign("\pNot a readable file.");
	theCaption = dynamic_cast<LStaticText*>(theDialog->FindPaneByID(1));
	theCaption->SetText(mystring);
	theCaption->FocusDraw();
	theCaption->CalcLocalFrameRect(frame);
	theCaption->ApplyForeAndBackColors();
	::EraseRect(&frame);
	theCaption->Draw(nil);
	do {
		theMessage = theHandler.DoDialog();
		} while ( theMessage != msg_OK && theMessage != msg_Cancel );
}

void
CSpike6App::CloseAllWindows(){
	CStatsWindow *theStatsWindow = dynamic_cast<CStatsWindow *>(LWindow::FindWindowByID(rPPob_StatsWindow));
	if (theStatsWindow !=nil)
		{
		delete theStatsWindow;
		theStatsWindow=nil;
		}
	CSettingsWindow *theSettWindow = dynamic_cast<CSettingsWindow *>(LWindow::FindWindowByID(rPPob_SettingWindow));
	if (theSettWindow !=nil)
		{
		delete theSettWindow;
		theSettWindow=nil;
		}
	CSumWindow *theSumWindow = dynamic_cast<CSumWindow *>(LWindow::FindWindowByID(rPPob_SumWindow));
	if (theSumWindow !=nil)
		{
		delete theSumWindow;
		theSumWindow=nil;
		}
	LWindow *theWaveWindow = dynamic_cast<LWindow *>(LWindow::FindWindowByID(rPPob_WaveForm1));
	if (theWaveWindow !=nil)
		{
		delete theWaveWindow;
		theWaveWindow=nil;
		}
	LWindow *thePatternWindow = dynamic_cast<CSumWindow *>(LWindow::FindWindowByID(134));
	if (thePatternWindow !=nil)
		{
		delete thePatternWindow;
		thePatternWindow=nil;
		}
	CMonteWindow *theMonteWindow = dynamic_cast<CMonteWindow *>(LWindow::FindWindowByID(rPPob_MonteWindow));
	if (theMonteWindow !=nil)
		{
		delete theMonteWindow;
		theMonteWindow=nil;
		}
	CSpikeGenWindow *theGeneratorWindow = dynamic_cast<CSpikeGenWindow *>(LWindow::FindWindowByID(rPPob_GeneratorWindow));
	if (theGeneratorWindow !=nil)
		{
		delete theGeneratorWindow;
		theGeneratorWindow=nil;
		}
	CCombEvents *theCombWindow = dynamic_cast<CCombEvents *>(LWindow::FindWindowByID(rPPob_Combination));
	if (theCombWindow !=nil)
	{
		delete theCombWindow;
		theCombWindow=nil;
	}
	CMatrixWindow *theMatrixWindow = dynamic_cast<CMatrixWindow *>(LWindow::FindWindowByID(rPPob_MatrixWindow));
	if (theMatrixWindow !=nil)
	{
		delete theMatrixWindow;
		theMatrixWindow=nil;
	}
}

/*void
CSpike6App::MakemacBS2(){

templrecrd tmplrcrd;
SInt32 i,j,val,templnum,templ1,kk,curtemplnum,noccurs,noccurs1;
//rename the template numbers.
curtemplnum=1;
kk=1;//location of first nonzero template
for(j=1;j<=snTemplates; j++){
	sAvgCol2->FetchItemAt(j,&noccurs);
	if(noccurs==0){
		for (kk=j;kk<=snTemplates;kk++){//search for the next template that has occurrences
			sAvgCol2->FetchItemAt(kk, &noccurs1);
			if (noccurs1 != 0){//this is the first that is nozero, so it must get the templatenumber j
				sAvgCol1->FetchItemAt(kk, &templnum);
				curtemplnum=j;
				for (i=1;i<=sdim3;i++){
					theRecords->FetchItemAt(i,&tmplrcrd);
					if(tmplrcrd.templatenum==templnum){//get the templatenumber and replace it by what it should be
						tmplrcrd.templatenum=curtemplnum;
						theRecords->AssignItemsAt(1,i,&tmplrcrd);
						}
					}
				break; //no need to look further
				}
			
			}
			
		}
	}
;
}*/

// ---------------------------------------------------------------------------
//	¥ mymacbs2FilterProc								[protected]
// ---------------------------------------------------------------------------
//	
//NavServFileFilterProc
pascal Boolean mymacbs2FilterProc(AEDesc* theItemAEDesc, void* info,
                                void* callBackUD,
                                NavFilterModes filterModes)
{
#pragma unused (callBackUD, filterModes)
   Boolean display = false;
   NavFileOrFolderInfo* theInfo = (NavFileOrFolderInfo*)info;
   //if ( theItemAEDesc->descriptorType == typeFSS )
   //{
      if ( theInfo->isFolder ) // show folders and volumes
      {
         return true; 
      }
   //}
   StAEDescriptor   aSpecDesc;
   if (::AECoerceDesc(theItemAEDesc, typeFSS, aSpecDesc) == noErr) {
      FSSpec   spec;
   OSErr   err = ::AEGetDescData(aSpecDesc, &spec, sizeof(FSSpec));
      ThrowIfOSErr_(err);
      {
         FInfo   finderInfo;
         ::FSpGetFInfo( &spec, &finderInfo );
         LStr255   tempString( spec.name );
         if (tempString.EndsWith( "\p.macbs2" ))
         {
            display = true;
         }
        /* if ( finderInfo.fdType == 'MPG3' || 
                                 finderInfo.fdType == 'MP3 ' )
         {
            display = true;
         }*/
      }
      
   }
   return display;
}

// ---------------------------------------------------------------------------
//	¥ mymacbin2FilterProc								[protected]
// ---------------------------------------------------------------------------
//	
//NavServFileFilterProc
pascal Boolean mymacbin2FilterProc(AEDesc* theItemAEDesc, void* info,
                                void* callBackUD,
                                NavFilterModes filterModes)
{
#pragma unused (callBackUD, filterModes)
   Boolean display = false;
   NavFileOrFolderInfo* theInfo = (NavFileOrFolderInfo*)info;
   //if ( theItemAEDesc->descriptorType == typeFSS )
   //{
      if ( theInfo->isFolder ) // show folders and volumes
      {
         return true; 
      }
   //}
   StAEDescriptor   aSpecDesc;
   if (::AECoerceDesc(theItemAEDesc, typeFSS, aSpecDesc) == noErr) {
      FSSpec   spec;
   OSErr   err = ::AEGetDescData(aSpecDesc, &spec, sizeof(FSSpec));
      ThrowIfOSErr_(err);
      {
         FInfo   finderInfo;
         ::FSpGetFInfo( &spec, &finderInfo );
         LStr255   tempString( spec.name );
         if (tempString.EndsWith( "\p.macbin2" ))
         {
            display = true;
         }
        /* if ( finderInfo.fdType == 'MPG3' || 
                                 finderInfo.fdType == 'MP3 ' )
         {
            display = true;
         }*/
      }
      
   }
   return display;
}



