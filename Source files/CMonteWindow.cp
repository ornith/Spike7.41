// =================================================================================
//	CProgressWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CProgressWindow.h	

#include <LString.h>
#include <UReanimator.h>
#include <UWindows.h>
#include <LArray.h>
#include <String.h>
#include <LString.h>
#include <LPopupButton.h>

#include "SpikeConstants.h"
#include "CBig2.h"
#include "CMonteWindow.h"
#include "MonteCarlo.h" //the actual arithmetic unit

SInt32 CMonteWindow::sDrawDim=0;
LArray * CMonteWindow::simCol1=nil;
LArray * CMonteWindow::simCol2=nil;
LArray * CMonteWindow::simCol3=nil;
LArray * CMonteWindow::RXY1=nil;
LArray * CMonteWindow::RXY2=nil;//immediate R's
LArray * CMonteWindow::BreakArray2=nil;
SInt32 CMonteWindow::nIntervals;
SInt32 CMonteWindow::nRuns;
SInt32 CMonteWindow::cRuns;
SInt32 CMonteWindow::sN1;
SInt32 CMonteWindow::sN2;
SInt32 CMonteWindow::sSum1;
SInt32 CMonteWindow::sSum2;
SInt32 CMonteWindow::order;
Float64 CMonteWindow::KWStat;
Float64 CMonteWindow::KSProb;
Float64 CMonteWindow::PermProb;
Float64 CMonteWindow::MedianStat;
Float64 CMonteWindow::Average1;
Float64 CMonteWindow::Average2;
Float64 CMonteWindow::Stdev1;
Float64 CMonteWindow::Stdev2;
Float64 CMonteWindow::SumAvg1=0.0;
Float64 CMonteWindow::SumSd1=0.0;
Float64 CMonteWindow::SumAvg2=0.0;
Float64 CMonteWindow::SumSd2=0.0;

Boolean CMonteWindow::DrawFlag1=true; //draw spike Train
Boolean CMonteWindow::DrawFlag2=FALSE; //draw Breakpoint, 
Boolean CMonteWindow::DrawFlag3=FALSE; //draw RXY
Float64 KW5=0,KW10=0,KS5=0,KS10=0, Perm5=0, Perm10=0, Med5=0, Med10=0, Flag1=0,Flag2=0,Flag3=0;
SInt32 bothKSKW, justKS, justKW, noneKSKW;
//SInt32	CMonteWindow::theOrd;
Boolean CMonteWindow::unifs=FALSE, CMonteWindow::norms=FALSE;
Boolean CMonteWindow::logns=FALSE, CMonteWindow::expos=TRUE; 
Boolean autcorrs=FALSE;
SInt32 	CMonteWindow::ARMAFlag1=0;

// ---------------------------------------------------------------------------------
//		¥ CProgressWindow
// ---------------------------------------------------------------------------------

CMonteWindow::CMonteWindow()
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CMonteWindow::CMonteWindow(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo ), mBusy( false )
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CMonteWindow::CMonteWindow(
	ResIDT		inWINDid,
	UInt32		inAttributes,
	LCommander	*inSuperCommander )
		: LWindow( inWINDid, inAttributes, inSuperCommander ), mBusy( false )
{
}


// ---------------------------------------------------------------------------------
//		¥ CProgressWindow(LStream *)
// ---------------------------------------------------------------------------------

CMonteWindow::CMonteWindow(
	LStream *inStream )
		: LWindow( inStream ), mBusy( false )
{
}


// ---------------------------------------------------------------------------------
//		¥ ~CProgressWindow
// ---------------------------------------------------------------------------------

CMonteWindow::~CMonteWindow()
{
//delete SimArray;LArray * CMonteWindow::simCol1=nil;
	delete simCol2;
	simCol2 =nil;
	delete simCol3;
	simCol3 =nil;
	delete RXY1;
	RXY1 =nil;
	delete RXY2;
	RXY2 =nil;
	delete BreakArray2;
	BreakArray2 =nil;
}


// ---------------------------------------------------------------------------------
//		¥ FinishCreateSelf
// ---------------------------------------------------------------------------------

void
CMonteWindow::FinishCreateSelf()
{
	// Link the window (the listener) with its controls (the broadcasters).
	UReanimator::LinkListenerToControls( this, this, rRidL_MonteWindow );
	

	//InitRand(); 
	nIntervals = 101;
	nRuns = 100;
	cRuns =0;
	order=15;
	DrawFlag1=FALSE;
	DrawFlag2=FALSE;
	DrawFlag3=FALSE;
	PrepArrays();
	KW5=0;
	KW10=0;
	KS5=0;
	KS10=0;
	Med5=0;
	Med10=0;
	justKS=0;
	bothKSKW=0;
	justKW=0;
	noneKSKW=0;
	Perm10=0;
	Perm5=0;
	expos=TRUE;
	norms=FALSE;
	logns=FALSE;
	unifs=FALSE;


}


// ---------------------------------------------------------------------------------
//		¥ ListenToMessage
// ---------------------------------------------------------------------------------

void
CMonteWindow::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
SInt16 mitem;
LPopupButton*	thePopupMenu;

	switch ( inMessage ) {
		case 4001:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			nIntervals=101;
			PrepArrays();
			}
		}
	break;
	case 4002:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			nIntervals=1001;
			PrepArrays();			
			}
		}
	break;
	case 4003:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			nIntervals=10001;
			PrepArrays();
			}
		}
	break;
	case 4004:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			nRuns=100;
			}
		}
	break;
	case 4005:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			nRuns=1000;
			}
		}
	break;
	case 4006:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			nRuns=10000;
			}
		}
	break;
	
	case 4007:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{if (DrawFlag1)
				{
				DrawFlag1=FALSE;
				}
			else
				{
				DrawFlag1=TRUE;
				DrawFlag2=FALSE;
				DrawFlag3=FALSE;
				}
			if (cRuns==nRuns)
				{
				if ((DrawFlag1)||(DrawFlag2)||(DrawFlag3))
					{
					//use drawarray for drawing!! put simCols in DrawArrays
					CBig2	*theIntvPlot = dynamic_cast<CBig2 *> (FindPaneByID( kDrawit ));
					ThrowIfNil_( theIntvPlot );
					theIntvPlot->Draw(nil);
					}
				}
			}
		}
	break;
	
	case 4008:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			if (DrawFlag2)
				{
				DrawFlag2=FALSE;
				}
			else
				{
				DrawFlag1=FALSE;
				DrawFlag2=TRUE;
				DrawFlag3=FALSE;
				}
			if (cRuns==nRuns)
				{
				if ((DrawFlag1)||(DrawFlag2)||(DrawFlag3))
					{
					//use drawarray for drawing!! put simCols in DrawArrays
					CBig2	*theIntvPlot = dynamic_cast<CBig2 *> (FindPaneByID( kDrawit ));
					ThrowIfNil_( theIntvPlot );
					theIntvPlot->Draw(nil);
					}

				}
			}
		}
	break;
	
	case 4010:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			if (DrawFlag3)
				{
				DrawFlag3=FALSE;
				}
			else
				{
				DrawFlag1=FALSE;
				DrawFlag2=FALSE;
				DrawFlag3=TRUE;
				}
			if (cRuns==nRuns)
				{
				if ((DrawFlag1)||(DrawFlag2)||(DrawFlag3))
					{
					//use drawarray for drawing!! put simCols in DrawArrays
					CBig2	*theIntvPlot = dynamic_cast<CBig2 *> (FindPaneByID( kDrawit ));
					ThrowIfNil_( theIntvPlot );
					theIntvPlot->Draw(nil);
					}

				}
			
			}
		}
	break;

	case 4009:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			DrawFlag1=FALSE;
			DrawFlag2=FALSE;
			DrawFlag3=FALSE;
			}
		}
	break;
	
		case 4013:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			if (expos)
				{
				expos=FALSE;
				}
			else
				{
				unifs=FALSE;
				norms=FALSE;
				logns=FALSE;
				expos=TRUE;
				}
			}
		}
	break;
	
	
	case 4014:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			if (norms)
				{
				norms=FALSE;
				}
			else
				{
				unifs=FALSE;
				norms=TRUE;
				logns=FALSE;
				expos=FALSE;
				}
			}
		}
	break;
	
	case 4015:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			if (logns)
				{
				logns=FALSE;
				}
			else
				{
				unifs=FALSE;
				norms=FALSE;
				logns=TRUE;
				expos=FALSE;
				}
			}
		}
	break;

	case 4016:
		{
		if ( *(SInt32 *) ioParam == Button_On ) 
			{
			if (unifs)
				{
				unifs=FALSE;
				}
			else
				{
				unifs=TRUE;
				norms=FALSE;
				logns=FALSE;
				expos=FALSE;
				}
			}
		}
	break;
	
case 4017:
		{
		thePopupMenu = dynamic_cast<LPopupButton*>
						(FindPaneByID(38));
		mitem=thePopupMenu->GetValue();
		if (mitem==2){
			ARMAFlag1=1; //do autoregression
			}
		else if (mitem==3){
			ARMAFlag1=2; //do moving average
			}
		else if (mitem==1){
			ARMAFlag1=0; //leave the data alone
			}
		}
	break;


	
	case 4011: //do monte carlo simulation
		{
		if (cRuns<nRuns)
			{
			SetBusyState( !GetBusyState() );
			}
		}
	break;
	
	case 4012: //responds to "next" and makes one run
		{
		
		if (cRuns < nRuns)
			{
			SInt32 cnt,cnt1;
			cnt=::TickCount();

 			DoSimulation();
			cnt1=::TickCount();
			cnt=cnt1-cnt;
			cRuns = cRuns + 1;
			if ((DrawFlag1)||(DrawFlag2)||(DrawFlag3))
				{
				//use drawarray for drawing!! put simCols in DrawArrays
				CBig2	*theIntvPlot = dynamic_cast<CBig2 *> (FindPaneByID( kDrawit ));
				ThrowIfNil_( theIntvPlot );
				theIntvPlot->Draw(nil);
				}
			DrawStats();
			} 
		else //if we have no more runs to do
			{
			SetDescriptorForPaneID( kRunButton, "\pDone" );
			LPane *theButt=dynamic_cast<LPane *> (FindPaneByID( kRunButton ));
			theButt->Disable();
			theButt=dynamic_cast<LPane *> (FindPaneByID( kNextButton ));
			theButt->Disable();

			CBig2	*theIntvPlot = dynamic_cast<CBig2 *> (FindPaneByID( kDrawit ));
			ThrowIfNil_( theIntvPlot );
			theIntvPlot->Draw(nil);
			}
		}
	break;
	}
}

//SpendTime Runs a simulated train of intervals and draws it

void CMonteWindow::SpendTime(const EventRecord &inMacEvent )
{
#pragma unused( inMacEvent )
if (cRuns < nRuns)
		{
		delete simCol1;//holds times
		delete simCol2;//holds intervals
		delete simCol3;
		simCol1 = new LArray(sizeof(Float64));
		ThrowIfNil_(simCol1);
		simCol2 = new LArray(sizeof(Float64));
		ThrowIfNil_(simCol2);
		simCol3 = new LArray(sizeof(Float64));
		ThrowIfNil_(simCol3);

		DoSimulation();
		
		if ((DrawFlag1)||(DrawFlag2)||(DrawFlag3))
			{
			CBig2	*theIntvPlot = dynamic_cast<CBig2 *> (FindPaneByID( kDrawit ));
			ThrowIfNil_( theIntvPlot );
			theIntvPlot->Draw(nil);
			}

		cRuns = cRuns + 1;
		DrawStats();
		} else {
		SetDescriptorForPaneID( kRunButton, "\pDone" );
		LPane *theButt=dynamic_cast<LPane *> (FindPaneByID( kNextButton ));
		theButt->Disable();
		theButt=dynamic_cast<LPane *> (FindPaneByID( kRunButton ));
		theButt->Disable();
			sDrawDim = order+3;
			CBig2	*theIntvPlot = dynamic_cast<CBig2 *> (FindPaneByID( kDrawit ));
			ThrowIfNil_( theIntvPlot );
			theIntvPlot->Draw(nil);
		SumAvg1=SumAvg1/cRuns;
		SumSd1=SumSd1/cRuns;
		SumAvg2=SumAvg2/cRuns;
		SumSd2=SumSd2/cRuns;

		StopIdling();
		}
}
// ---------------------------------------------------------------------------------
//		¥ SetBusyState
// ---------------------------------------------------------------------------------

void
CMonteWindow::SetBusyState(
	Boolean inBusy )
{
	// Set the busy state.
mBusy = inBusy;
if ( mBusy ) {
	
	StartIdling();	//generate random intervals
		
	// Set the button title.
	SetDescriptorForPaneID( kRunButton, "\pStop" );
	LPane *theButt=dynamic_cast<LPane *> (FindPaneByID( kNextButton ));
	theButt->Disable();
	StartIdling();	//generate random intervals
	} else {
	
	StopIdling();
		
	// Set the button title.
	SetDescriptorForPaneID( kRunButton, "\pResume" );
	LPane *theButt=dynamic_cast<LPane *> (FindPaneByID( kNextButton ));
	theButt->Enable();
	}
}

//---------------------------------------------------------------------
void CMonteWindow::DrawStats()
	{
	Float64 PCT=0;
	
	SumAvg1+=Average1;
	SumSd1+=Stdev1;
	SumAvg2+=Average2;
	SumSd2+=Stdev2;
	Str255 str;
	NumToString(cRuns, str);
	SetDescriptorForPaneID( kRunsDone,str);	
	LStr255	theTitle(Average1,"\p#####.####");
	SetDescriptorForPaneID( kP1Avg,theTitle);
	theTitle.Assign(Average2, "\p#####.####");
	SetDescriptorForPaneID( kP2Avg,theTitle);
	theTitle.Assign(Stdev1, "\p#####.####");
	SetDescriptorForPaneID( kP1Stdev,theTitle);
	theTitle.Assign(Stdev2, "\p#####.####");
	SetDescriptorForPaneID( kP2Stdev,theTitle);
	theTitle.Assign(KWStat, "\p#####.####");
	SetDescriptorForPaneID( kKWStat,theTitle);
	theTitle.Assign(MedianStat, "\p#####.####");
	SetDescriptorForPaneID( kMedianProb,theTitle);
	theTitle.Assign(KSProb, "\p#####.####");
	SetDescriptorForPaneID( kKSProb,theTitle);
	if ((KWStat >= 3.841) && (KSProb <=0.05)) 
		{
		bothKSKW+=1;
		NumToString(bothKSKW, str);
		SetDescriptorForPaneID( kbothKSKW,str);
		}
	else if (KWStat >= 3.841)
		{justKW+=1;
		NumToString(justKW, str);
		SetDescriptorForPaneID( kjustKW,str);}
	else if (KSProb <=0.05)
		{justKS+=1;
		NumToString(justKS, str);
		SetDescriptorForPaneID( kjustKS,str);}
	else
		{noneKSKW+=1;
		NumToString(noneKSKW, str);
		SetDescriptorForPaneID( knoneKSKW,str);}
	if (KWStat >= 3.841)
		{
		KW5+=1;
		}
	PCT=KW5/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kKWProb5,theTitle);
	if (KWStat >= 6.635)
		{
		KW10+=1;
		}
	PCT=KW10/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kKWProb10,theTitle);
	
	if (MedianStat >= 3.841)
		{
		Med5+=1;
		}
	PCT=Med5/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kMedianProb5,theTitle);
	
	if (MedianStat >= 6.635)
		{
		Med10+=1;
		}
	PCT=Med10/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kMedianProb10,theTitle);
	
	if (KSProb <= 0.05)
		{
		KS5+=1;
		}
	PCT=KS5/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kKSProb5,theTitle);
	
	if (KSProb <= 0.01)
		{
		KS10+=1;
		}
	PCT=KS10/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kKSProb10,theTitle);
	theTitle.Assign(PermProb, "\p####.####");
	SetDescriptorForPaneID( kPermProb,theTitle);
	if (PermProb<=0.05)
		{
		Perm5 += 1;
		if (sN1<sN2){Flag1+=1;}
		else if (sN1>sN2){Flag2+=1;}
		else {Flag3+=1;}
		}
	PCT=Perm5/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kPermProb5,theTitle);
	if (PermProb<=0.01)
		{
		Perm10 += 1;
		}
	PCT=Perm10/cRuns;
	theTitle.Assign(PCT, "\p####.####");
	SetDescriptorForPaneID( kPermProb10,theTitle);
	
	}
//---------------------------------------------------------------------
// Prepare Arrays
//---------------------------------------------------------------------
void
CMonteWindow::PrepArrays()	
	{
	RXY1 = new LArray(sizeof(Float64));
	ThrowIfNil_(RXY1);
	RXY2 = new LArray(sizeof(Float64));
	ThrowIfNil_(RXY2);
	BreakArray2 = new LArray(sizeof(Float64));
	ThrowIfNil_(BreakArray2);
	Float64 vval=0.0;
	SInt32 i;
	for (i=1; i<=nIntervals; i++)
		{
		BreakArray2->AddItem(&vval);
		}
	for (i=1; i<=order+3; i++)
		{
		RXY2->AddItem(&vval);
		}
	Float64 valu;
	valu=1;
	RXY1->AddItem(&valu);
	RXY1->AddItem(&valu);
	RXY1->AddItem(&valu);
	for (i=4; i<=order+3; i++)
		{
		valu=i-3;
		RXY1->AddItem(&valu);
		}
	valu=-1;
	RXY2->InsertItemsAt(1,1,&valu);
	valu=1;
	RXY2->InsertItemsAt(1,2,&valu);
	valu=0;
	RXY2->InsertItemsAt(1,3,&valu);
	}
