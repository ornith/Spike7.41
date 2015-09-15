// =================================================================================
//	CProgressWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CProgressWindow.h	

#include <LString.h>
#include <UReanimator.h>
#include <UWindows.h>
#include <LArray.h>
//#include <LStdControl.h>
#include <String.h>
#include <Math.h>
#include <LPopupButton.h>
#include "SpikeConstants.h"
#include "CSpike6App.h"
#include "CSpikeGenWindow.h"
#include "CSettingsWindow.h"
#include "Analyze2.h"
#include "gsl/gsl_randist.h"
//#include "MonteCarlo.h"  //used ONLY for InitRand
//#include "UltraU.h" //used for all other randomization
//#include "URandomLib.h"
SInt32 nSpikes, ARMAFlag=0;
SInt32	theOrder, theSlope=0, theDirection=1;
Boolean unif=FALSE, norm=TRUE, logn=FALSE, expo=FALSE, cnst=FALSE, autcorr=FALSE;
// ---------------------------------------------------------------------------------
//		¥ CProgressWindow
// ---------------------------------------------------------------------------------

CSpikeGenWindow::CSpikeGenWindow()
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CSpikeGenWindow::CSpikeGenWindow(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo )
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CSpikeGenWindow::CSpikeGenWindow(
	ResIDT		inWINDid,
	UInt32		inAttributes,
	LCommander	*inSuperCommander )
		: LWindow( inWINDid, inAttributes, inSuperCommander )
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(LStream *)
// ---------------------------------------------------------------------------------

CSpikeGenWindow::CSpikeGenWindow(
	LStream *inStream )
		: LWindow( inStream )
{
}


// ---------------------------------------------------------------------------------
//		¥ ~CProgressWindow
// ---------------------------------------------------------------------------------

CSpikeGenWindow::~CSpikeGenWindow()
{
//delete SimArray;
}


// ---------------------------------------------------------------------------------
//		¥ FinishCreateSelf
// ---------------------------------------------------------------------------------

void
CSpikeGenWindow::FinishCreateSelf()
{
	// Link the window (the listener) with its controls (the broadcasters).
	UReanimator::LinkListenerToControls( this, this, rRidL_GeneratorWindow );
	//InitRand(); 
	nSpikes = 100;
	expo=FALSE;
	norm=TRUE;
	logn=false;
	unif=FALSE;
	cnst=FALSE;
	theDirection=1;
	PrepArrays();
	const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
			
}

void		
CSpikeGenWindow::FindCommandStatus(
								CommandT			inCommand,
								Boolean				&outEnabled,
								Boolean				&outUsesMark,
								UInt16				&outMark,
								Str255				outName)
{
switch (inCommand) {
		case 2001:
		case 2002:
		outEnabled = false;
		break;

		default:
			PP_PowerPlant::LWindow::FindCommandStatus(inCommand, outEnabled,
												outUsesMark, outMark, outName);
		break;
		}
}
 
// ---------------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------------

void
CSpikeGenWindow::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
SInt16 mitem;
LPopupButton*	thePopupMenu;
	switch ( inMessage ) {
		
	case 4101:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			nSpikes=100;
			}
		}
	break;
	
	case 4102:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			nSpikes=1000;
			}
		}
	break;
	
	case 4103:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			nSpikes=10000;
			
			}
		}
	break;

	case 4107:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			if (expo)
				{
				expo=FALSE;
				}
			else
				{
				unif=FALSE;
				norm=FALSE;
				logn=FALSE;
				expo=TRUE;
				cnst=FALSE;
				}
			}
		}
	break;
	
	
	case 4108:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			if (norm)
				{
				norm=FALSE;
				}
			else
				{
				unif=FALSE;
				norm=TRUE;
				logn=FALSE;
				expo=FALSE;
				cnst=FALSE;
				}
			}
		}
	break;
	
	case 4109:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			if (logn)
				{
				logn=FALSE;
				}
			else
				{
				unif=FALSE;
				norm=FALSE;
				logn=TRUE;
				expo=FALSE;
				cnst=FALSE;
				}
			}
		}
	break;

	case 4110:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			if (unif)
				{
				unif=FALSE;
				}
			else
				{
				unif=TRUE;
				norm=FALSE;
				logn=FALSE;
				expo=FALSE;
				cnst=FALSE;
				}
			}
		}
	break;
	
	case 4111:
		{
		if ( *(UInt32 *) ioParam == Button_On ) 
			{
			if (cnst)
				{
				cnst=FALSE;
				}
			else
				{
				cnst=TRUE;
				unif=FALSE;
				norm=FALSE;
				logn=FALSE;
				expo=FALSE;
				}
			}
		}
	break;

	case 4112:
		{
		thePopupMenu = dynamic_cast<LPopupButton*>
						(FindPaneByID(9));
		mitem=thePopupMenu->GetValue();
		if (mitem==2){
			ARMAFlag=1; //do autoregression
			}
		else if (mitem==3){
			ARMAFlag=2; //do moving average
			}
		else if (mitem==1){
			ARMAFlag=0; //leave the data alone
			}
		}
	break;

	/*case 4113: //sets sign of slope of trend
		{
		if (theDirection==1)
			{theDirection=-1;}else{theDirection=+1;}
		}
	break;*/
	
	case 4121: //responds to "construct" and makes one run
		{
		
		SInt32 cnt;
		cnt=::TickCount();
		
			//DoSimulation();
		LStdPopupMenu	*thePopup=(LStdPopupMenu	*)FindPaneByID(kSetOrderPopup );
		Assert_( thePopup != nil );
		Str255	theMenuText;
		::GetMenuItemText( thePopup->GetMacMenuH(),
		thePopup->GetValue(), theMenuText );
		// Get the size referred to by the menu item.
		::StringToNum( theMenuText, &theOrder );
		Assert_( thePopup2 != nil );
		theSlope=0.0;
		if (ARMAFlag==0){
			MakeTrain();
			}
		else if (ARMAFlag==1){
			MakeTrainAR();
			}
		else if (ARMAFlag==2){
			MakeTrainMA();
			}
		MakeGenStats();
		CSpike6App::snTemplates=1;
		CSpike6App::sdim2=2;
		CSpike6App::sdim3=CSpike6App::tRecord->GetCount();
		LString::CopyPStr("\psimulated train",CSpike6App::sFileName, sizeof(Str63));
		CSettingsWindow  *theSettWindow = dynamic_cast<CSettingsWindow *>(LWindow::CreateWindow( rPPob_SettingWindow, this ));
		ThrowIfNil_( theSettWindow );
		theSettWindow->Show();
		delete CSpike6App::FileNameBase;
		CSpike6App::FileNameBase=nil;
		CSpike6App::FileNameBase=new LStr255 (CSpike6App::sFileName);
		SetDescriptorForPaneID( kConstructButton, "\pDone" );
		LPane *theButt=dynamic_cast<LPane *> (FindPaneByID( kConstructButton ));
		theButt->Disable();
		//DoClose();
		}
	break;
	}
}

//
// prepare arrays and globals
//
void
CSpikeGenWindow::PrepArrays()
	{
	delete CSpike6App::tRecord;
	CSpike6App::tRecord=nil;
	CSpike6App::tRecord=new LArray(sizeof (CSpike6App::templrecrd));
	CSpike6App::sdim2=4;
	}

	//
	// generate spike train. for autocorrelation, moving average is used
	//
	
void
CSpikeGenWindow::MakeTrainMA()//always normally distributed train. Will change this
	{
	CSpike6App::templrecrd theTRec;
	float tim, intval, oldintval, avrg, sigm, increment;//fact is proportion avg(2)/avg(1) or N1/N2
	Float64 theD;
	SInt32 i, size=10000;
	SInt32 Templ=1;
	tim=0;
	intval=0;
	oldintval=1;
	const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	if (theDirection==1)
		{
		increment=(float)theSlope/((float)(nSpikes-1));
		}
	else if (theDirection==-1)
		{
		increment=1/(theSlope*((float)(nSpikes-1)));
		}
	LArray *IntvArray1=nil;
	IntvArray1=new LArray(sizeof(float));
	ThrowIfNil_(IntvArray1);
	/*	Critical!  We MUST supply two ulong seeds > 0.  There is, intentionally, no
	default.  Ultra_Init() MUST be the first library function called. */
	avrg=0;
	sigm=0.25;
	Float64 theAvg=2.0, coeff=0.9, dev=0.0;
	Float64 olddev=gsl_ran_gaussian(r, sigm) + avrg;

	for (i=1; i<=nSpikes-1; i++)
		{
		avrg=0;
		sigm=0.25;
		//dev=PRNG.Normal(avrg,sigm);
		dev=gsl_ran_gaussian(r, sigm) + avrg;
		intval=theAvg - olddev*coeff + dev;
		olddev=dev;
		theD=(Float64)tim;
		theTRec.timestamp=theD;
		theTRec.templatenum=Templ;
		theTRec.plusval=size;
		theTRec.minval=-size;
		CSpike6App::tRecord->AddItem(&theTRec,sizeof(CSpike6App::templrecrd));
		tim += intval;
		}
	theD=(Float64)tim;
	theTRec.timestamp=theD;
	theTRec.templatenum=Templ;
	theTRec.plusval=size;
	theTRec.minval=-size;
	CSpike6App::tRecord->AddItem(&theTRec,sizeof(CSpike6App::templrecrd));
	delete(IntvArray1);
	IntvArray1=nil;
	}

	//
	// generate spike train. for autocorrelation, autoregression is used
	//
	
void
CSpikeGenWindow::MakeTrainAR()
	{
	CSpike6App::templrecrd theTRec;
	float tim, intval, oldintval, avrg, sigm, increment;//fact is proportion avg(2)/avg(1) or N1/N2
	Float64 theD;
	SInt32 i, size=10000;
	avrg=1.0;//all series have average 1 
	sigm=0.25*avrg;
	SInt32 Templ=1;
	tim=0;
	intval=0;
	oldintval=1;
	const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);

	if (theDirection==1)
		{
		increment=(float)theSlope/((float)(nSpikes-1));
		}
	else if (theDirection==-1)
		{
		increment=1/(theSlope*((float)(nSpikes-1)));
		}
	LArray *IntvArray1=nil;
	IntvArray1=new LArray(sizeof(float));
	ThrowIfNil_(IntvArray1);
	/*	Critical!  We MUST supply two ulong seeds > 0.  There is, intentionally, no
	default.  Ultra_Init() MUST be the first library function called. */
	avrg=0;
	sigm=0.25;
	Float64 theAvg=2.0, coeff=0.8, dev=0.0;
	//Float64 oudintval=PRNG.Normal(0,sigm)
	Float64 oudintval=gsl_ran_gaussian(r, sigm);
	avrg=0;
	sigm=0.25;
	for (i=1; i<=nSpikes-1; i++)
		{
		//dev=PRNG.Normal(avrg,sigm);
		dev=gsl_ran_gaussian(r, sigm) + avrg;
		intval=coeff*oudintval + dev;
		oudintval=intval;
		theD=(Float64)tim;
		theTRec.timestamp=theD;
		theTRec.templatenum=Templ;
		theTRec.plusval=size;
		theTRec.minval=-size;
		CSpike6App::tRecord->AddItem(&theTRec,sizeof(CSpike6App::templrecrd));
		tim += intval+theAvg;
		}
	theD=(Float64)tim;
	theTRec.timestamp=theD;
	theTRec.templatenum=Templ;
	theTRec.plusval=size;
	theTRec.minval=-size;
	CSpike6App::tRecord->AddItem(&theTRec,sizeof(CSpike6App::templrecrd));
	delete(IntvArray1);
	IntvArray1=nil;
	}

	//
	// generate spike train without autocorrelation
	//
	
void
CSpikeGenWindow::MakeTrain()//currently the only one that does all distributions (april 2001)
	{
	
	CSpike6App::templrecrd theTRec;
	delete(CSpike6App::tRecord);
	CSpike6App::tRecord=new LArray(sizeof(CSpike6App::templrecrd));

	float tim=0.0, intval, oldintval, avrg, sigm, increment, totdur;//fact is proportion avg(2)/avg(1) or N1/N2
	Float64 theD;
	SInt32 i, size=10000;
	//avrg=2.0;//all series have average 1 
	Str255 str;
	GetDescriptorForPaneID(13, str);
	StringToNum(str,&i);
	avrg=(float)i/1000;
	//sigm=0.25;
	SInt32 Templ=1;
	tim=0;
	intval=0;
	oldintval=1;
	
	
	gsl_rng *r;
	const gsl_rng_type *T;//type of generator to use
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	UInt32 myseed=TickCount();
	gsl_rng_set(r,myseed);


	if (theDirection==1)
		{
		increment=(float)theSlope/((float)(nSpikes-1));
		}
	else if (theDirection==-1)
		{
		increment=1/(theSlope*((float)(nSpikes-1)));
		}
	LArray *IntvArray1=nil;
	IntvArray1=new LArray(sizeof(float));
	ThrowIfNil_(IntvArray1);
	//make it so that all spiketrains have appoximately the same duration
	//avrg=avrg*10000.0/(float)nSpikes;// this is to make the duration of all series the same
	totdur=avrg*(float)nSpikes;//for use with uniform distribution
	sigm=0.25*avrg; //coeff of variation is 25%
	/*	Critical!  We MUST supply two ulong seeds > 0.  There is, intentionally, no
	default.  Ultra_Init() MUST be the first library function called. */
	if (logn){
		avrg=log(avrg);
		}
	for (i=1; i<=nSpikes; i++)
		{
		if (unif){
			intval = 2 * gsl_rng_uniform(r);
			}
		else if (norm) 
			{intval = gsl_ran_gaussian(r, sigm) + avrg;}
		else if (logn) 
			{
				sigm=1.0;
			intval = gsl_ran_lognormal(r, .24, sigm);
			//intval=3;
			//intval=exp(intval);//pow(10,intval);//
			}
		else if (expo) 
			{intval=gsl_ran_exponential(r, avrg);}
		else if (cnst)
			{intval=avrg;}
		if (theSlope != 0.0)
			{
			intval=intval*(1.0+(float)i*increment);
			}
		tim+=intval;
		theD=(Float64)tim;
		theTRec.timestamp=theD;
		theTRec.templatenum=Templ;
		theTRec.plusval=size;
		theTRec.minval=-size;
		CSpike6App::tRecord->AddItem(&theTRec,sizeof(CSpike6App::templrecrd));
		IntvArray1->AddItem(&intval); //interval(temporary)
		//tim += intval;
		}
	theD=(Float64)tim;
	theTRec.timestamp=theD;
	theTRec.templatenum=Templ;
	theTRec.plusval=size;
	theTRec.minval=-size;
	CSpike6App::tRecord->AddItem(&theTRec,sizeof(CSpike6App::templrecrd));
	CSpike6App::tRecord->RemoveItemsAt(1,1);//remove the first zero value
	delete(IntvArray1);
	IntvArray1=nil;
	}
	
/*void  CSpikeGenWindow::MakeGenStats()
//makes number of occurrences of each template&average&stdev&minimum intervals}
{	
	CSpike6App::templrecrd theTRec;
	SInt32 i,j,maxdata,ttemp, temp,noccur, vall, val, sum ;
	//String(255) ThermTitle;
	delete CSpike6App::sAvgCol1;
	CSpike6App::sAvgCol1=nil;
	delete CSpike6App::sAvgCol2;
	CSpike6App::sAvgCol2=nil;
	delete CSpike6App::sAvgCol3;
	CSpike6App::sAvgCol3=nil;
	delete CSpike6App::sAvgCol4;
	CSpike6App::sAvgCol4=nil;
	CSpike6App::sAvgCol1 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol1);
	CSpike6App::sAvgCol2 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol2);
	CSpike6App::sAvgCol3 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol3);
	CSpike6App::sAvgCol4 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol4);
	CSpike6App::sAvDim2 = 3;
	//CSpike6App::sAvDim2 = 2;

	//look for highest template number this yields AvDim3
	maxdata = CSpike6App::tRecord->GetCount();
	ttemp=0;
	for(i=1; i<=maxdata-1;i++)
		{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		temp=theTRec.templatenum;
		if (temp > ttemp)
			{
			ttemp = temp;
			}
		}
	SInt32 Templates = (SInt32)ttemp;
	ttemp=0;
	val=0;	
	for (j=1;j<=Templates;j++)
		{
		vall=j;
		CSpike6App::sAvgCol1->AddItem(&vall, sizeof(SInt32));
		CSpike6App::sAvgCol2->AddItem(&ttemp, sizeof(SInt32));
		CSpike6App::sAvgCol3->AddItem(&val, sizeof(SInt32));
		CSpike6App::sAvgCol4->AddItem(&val, sizeof(SInt32));
		}
	
	//template#, #templates
	//j is the template number; i the line number
	noccur=0;
	sum=0;
	SInt32 min=0;
	for (i=1; i<=maxdata; i++)
		{
		noccur=0;
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		temp=theTRec.templatenum;
		j = temp;
		CSpike6App::sAvgCol2->FetchItemAt(j, &vall);
		noccur=vall + 1; 
		CSpike6App::sAvgCol2->AssignItemsAt(1,j, &noccur);
		val=theTRec.plusval;
		sum+=val;
		CSpike6App::sAvgCol3->AssignItemsAt(1,j, &sum);
		val=theTRec.minval;
		min+=val;
		CSpike6App::sAvgCol4->AssignItemsAt(1,j, &min);
		}
	//ttemp=CSpike6App::tRecord->GetCount();
	ttemp=CSpike6App::sAvgCol2->GetCount();
	for (i=1; i<=ttemp; i++)
		{
		CSpike6App::sAvgCol2->FetchItemAt(i, &noccur);
		CSpike6App::sAvgCol3->FetchItemAt(i, &val);
		sum=round((Float64)val/((Float64)noccur));
		CSpike6App::sAvgCol3->AssignItemsAt(1,i,&sum);
		CSpike6App::sAvgCol4->FetchItemAt(i, &val);
		sum=round((Float64)val/((Float64)noccur));
		CSpike6App::sAvgCol4->AssignItemsAt(1,i,&sum);

		}
};*/
