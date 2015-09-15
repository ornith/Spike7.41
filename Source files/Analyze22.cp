#include "Analyze2.h"
#include "Analyze1.h"
#include "CStatsWindow.h"
#include "CSettingsWindow.h"
#include "CSpike6App.h"
#include "MonteCarlo.h"
#include "SpikeConstants.h"
//#include "URandomLib.h"
#include <LProgressBar.h>
#include <UCursor.h>
#include <time.h>
#include <math.h>

#include <gsl_sort_double.h>

#define EPS1 0.001
#define EPS2 1.0e-8

SInt32 Intvals1, Intvals2;
//LArray 	*IntvArray1;
//LArray	*IntvArray2;
LArray 	*HelpCol1;
LArray 	*HelpCol2;

struct Strct1 {Float64 intval; UInt32 rank; UInt32 orde;};
typedef struct Strct1 Strct1;
struct Strct2 {UInt32 orde; UInt32 rank;};
typedef struct Strct2 Strct2;

void MakeIntvArray()//makes one! array of intervals to be used in KW and Median tests 
{
CSpike6App::templrecrd theTRec;
SInt32 	i, templ, linlin;
Float64	tim,intv, oldval1;
//make an array of intervals with only target spikes starting at minlin ending at maxlin (is divided up during ranking)
CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
oldval1=theTRec.timestamp;
Intvals1 = 0;
for (i=CSpike6App::SpikeParams.minlin+1;i<=CSpike6App::SpikeParams.brklin-1;i++) //minlin+1 because oldval1 is already known
	{
	//find source template#
	CSpike6App::tRecord->FetchItemAt(i,&theTRec);
	templ=theTRec.templatenum;
	if (templ==CSpike6App::SpikeParams.templ1)
		{
		tim=theTRec.timestamp;
		intv=tim-oldval1;
		HelpCol1->AddItem(&intv,sizeof(Float64));
		oldval1=tim;
		Intvals1 += 1;
		}
	}
//if brklin == 0 (no breakpoint), we have to start at the minlin
if (CSpike6App::SpikeParams.brklin==0){
	linlin=CSpike6App::SpikeParams.minlin;
	}
else {
	linlin = CSpike6App::SpikeParams.brklin;
	}
CSpike6App::tRecord->FetchItemAt(linlin,&theTRec);
oldval1=theTRec.timestamp;
Intvals2 = 0;
for (i=linlin+1;i<=CSpike6App::SpikeParams.maxlin;i++) //brklin+1 because oldval1 is already known
	{
	//find source template#
	CSpike6App::tRecord->FetchItemAt(i,&theTRec);
	templ=theTRec.templatenum;
	if (templ==CSpike6App::SpikeParams.templ1)
		{
		tim=theTRec.timestamp;
		intv=tim-oldval1;
		HelpCol1->AddItem(&intv,sizeof(Float64));
		oldval1=tim;
		Intvals2 += 1;
		}
	}
CStatsWindow::Intervals1=Intvals1;
CStatsWindow::Intervals2=Intvals2;
}

void MakeIntvArray2() //makes two arrays of intervals (before and after breakpoint)
{
SInt32 	i, templ;
Float64	tim,intv, oldval1,oldval2;
CSpike6App::templrecrd theTRec;


//make an array of intervals with only target spikes starting at minlin 
//where the first occurrence of the target spike is.
CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
oldval1=theTRec.timestamp;

if (CSpike6App::SpikeParams.brklin !=0.0)
	{
	Intvals1 = 0;
	for (i=CSpike6App::SpikeParams.minlin+1;i<CSpike6App::SpikeParams.brklin;i++) //minlin+1 because oldval1 is already known
		{
		//find source template#
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ1)
			{
			tim=theTRec.timestamp;
			intv=tim-oldval1;
			HelpCol1->AddItem(&intv,sizeof(Float64));
			oldval1=tim;
			Intvals1 += 1;
			}
		}
	CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.brklin,&theTRec);
	oldval2=theTRec.timestamp;
	Intvals2 = 0;
	if (CSpike6App::SpikeParams.brklin != 0.0)
		{
		if (CSpike6App::SpikeParams.brklin < 1000000)
			{
			for (i=CSpike6App::SpikeParams.brklin+1;i<=CSpike6App::SpikeParams.maxlin;i++) //minlin+1 because oldval2 is already known
				{
				//find source template#
				CSpike6App::tRecord->FetchItemAt(i,&theTRec);
				templ=theTRec.templatenum;
				if (templ==CSpike6App::SpikeParams.templ1)
					{
					tim=theTRec.timestamp;
					intv=tim-oldval2;
					HelpCol2->AddItem(&intv,sizeof(Float64));
					oldval2=tim;
					Intvals2 += 1;
					}
				}
			}
		}
	}
else	//if there is no breakpoint, then put all intervals in Helpcol1
	{
	Intvals1 = 0;
	
	for (i=CSpike6App::SpikeParams.minlin+1;i<=CSpike6App::SpikeParams.maxlin;i++) //minlin+1 because oldval1 is already known
		{
		//find source template#
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ1)
			{
			tim=theTRec.timestamp;
			intv=tim-oldval1;
			HelpCol1->AddItem(&intv,sizeof(Float64));
			oldval1=tim;
			Intvals1 += 1;
			}
		}
	}
CStatsWindow::Intervals1=Intvals1;
CStatsWindow::Intervals2=Intvals2;
}

void MakeIntvArray3()
{
CSpike6App::templrecrd theTRec;
SInt32 	i, templ;
Float64	tim,intv, oldval1;
//make an array of intervals with only target spikes starting at minlin ending at maxlin (is divided up during ranking)
CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
oldval1=theTRec.timestamp;
Intvals1 = 0;
for (i=CSpike6App::SpikeParams.minlin+1;i<=CSpike6App::SpikeParams.brklin-1;i++) //minlin+1 because oldval1 is already known
	{
	//find source template#
	CSpike6App::tRecord->FetchItemAt(i,&theTRec);
	templ=theTRec.templatenum;
	if (templ==CSpike6App::SpikeParams.templ1)
		{
		tim=theTRec.timestamp;
		intv=tim-oldval1;
		if (tim > CSpike6App::SpikeParams.brkpt){ 
			break;
			}
		HelpCol1->AddItem(&intv,sizeof(Float64));
		oldval1=tim;
		Intvals1 += 1;
		}
	}
Intvals2 = 0;
CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.brklin,&theTRec);
oldval1=theTRec.timestamp;
for (i=CSpike6App::SpikeParams.brklin + 1;i<=CSpike6App::SpikeParams.maxlin;i++) 
	{
	//find source template#
	CSpike6App::tRecord->FetchItemAt(i,&theTRec);
	templ=theTRec.templatenum;
	if (templ==CSpike6App::SpikeParams.templ1)
		{
		tim=theTRec.timestamp;
		intv=tim-oldval1;
		HelpCol2->AddItem(&intv,sizeof(Float64));
		oldval1=tim;
		Intvals2 += 1;
		}
	}
CStatsWindow::Intervals1=Intvals1;
CStatsWindow::Intervals2=Intvals2;

}



void RankIntervals(Float64 *Array1, Float64 *Array2) //makes two arrays with intervals sorted according to length, determines the medians in each
	{
	SInt32 i,j,cnt;
	Float64 val,valu;
	size_t arsize;
	j=0;
	cnt=HelpCol1->GetCount();
	for (i=1; i<=cnt; i++)
		{
		HelpCol1->FetchItemAt(i, &val);
		//IntvArray1->AddItem(&val,sizeof(Float64));
		Array1[j]=val;
		j+=1;
		}
	arsize = j;
	sortFloat64(Array1, arsize); 
	j=0;
	cnt=HelpCol2->GetCount();
	for (i=1; i<=cnt ;i++)
		{
		HelpCol2->FetchItemAt(i, &val);
		//IntvArray2->AddItem(&val,sizeof(Float64));
		Array2[j]=val;
		j+=1;
		}
	arsize = j;
	sortFloat64(Array2, arsize); 
	cnt=trunc((float)Intvals1/2.0);
	if((Intvals1-2*cnt)==1){//when intvals1 is even take the average of the two middle intervals
		//IntvArray1->FetchItemAt(cnt,&val);
		//IntvArray1->FetchItemAt(cnt,&valu);
		val=Array1[cnt-1];
		valu=Array1[cnt-1];
		val+=valu;
		val=val/2;
		}
	else {//when Intvals is uneven, take the center interval
		//cnt+=1;
		//IntvArray1->FetchItemAt(cnt,&val);
		val=Array1[cnt];
		}
	CStatsWindow::sMedian1=val;
	cnt=trunc((float)Intvals2/2.0);
	if((Intvals2-2*cnt)==1){//when intvals1 is even take the average of the two middle intervals
		//IntvArray2->FetchItemAt(cnt,&val);
		//IntvArray2->FetchItemAt(cnt,&valu);
		val=Array2[cnt-1];
		valu=Array2[cnt-1];
		val+=valu;
		val=val/2;
		}
	else {//when Intvals is uneven, take the center interval
		//cnt+=1;
		//IntvArray2->FetchItemAt(cnt,&val);
		val=Array2[cnt];
		}
	CStatsWindow::sMedian2=val;
	}
	
void RunBreakpoint(){
CSpike6App::templrecrd theTRec;

	//first:make array with only the occurrences of the spikes, as well as two rank number columns
SInt32 	i, j, templ, count, ranknum, ranksum1, ranksum2;
Float64	tim,oldtim,intv, oldval1;
HelpCol1 = new LArray(sizeof(Float64));
HelpCol2 = new LArray(sizeof(Float64));
LArray *HelpCol3 = new LArray(sizeof(SInt32));
//make two arrays of intervals (HelpCol1 and 2)
CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
oldval1=theTRec.timestamp;

Intvals1 = 0;
for (i=CSpike6App::SpikeParams.minlin+1;i<=CSpike6App::SpikeParams.maxlin;i++) //minlin+1 because oldval1 is already known
	{
	//find source template#
	CSpike6App::tRecord->FetchItemAt(i,&theTRec);
	templ=theTRec.templatenum;
	if (templ==CSpike6App::SpikeParams.templ1)
		{
		tim=theTRec.timestamp;
		intv=tim-oldval1;
		HelpCol1->AddItem(&intv,sizeof(Float64));
		HelpCol2->AddItem(&intv,sizeof(Float64));
		oldval1=tim;
		Intvals1 += 1;
		}
	}
//now we sort Helpcol1
HelpCol1->Sort();//sort intervals low to high
tim=CSpike6App::SpikeParams.strt;
CStatsWindow::stDrawCol1->AddItem(&tim,sizeof(Float64));
oldtim=tim;
count=HelpCol1->GetCount();
for (i=1;i<=count;i++){
	HelpCol2->FetchItemAt(i,&intv);//get intervals in order of appearance
	tim=oldtim+intv;
	CStatsWindow::stDrawCol1->AddItem(&tim,sizeof(Float64));
	oldtim=tim;
	j=HelpCol1->FetchIndexOf(&intv, sizeof(Float64));
	HelpCol3->AddItem(&j, sizeof(SInt32));//put the rank number of this interval in HelpCol3
	//for (j=1; j<=count; j++){
		//HelpCol1->FetchItemAt(j,&oldval1);//find the rank order (j) of this interval (location in sorted array)
		//if (fabs(oldval1-intv)<0.00000001){
		//	HelpCol3->AddItem(&j, sizeof(SInt32));//put the rank number of this interval in HelpCol3
		//	break;
		//	}
		//}
	}
ranksum2=0;
for (i=1;i<=count;i++){
	ranksum2+=i;
	}
SInt32 cnt2;
double KWStatt=0.0;
double v1,v2,n1,n2;
Float64 myKW=0.0;
ranksum1=0;
for(i=1;i<=10;i++){
	HelpCol3->FetchItemAt(i,&ranknum);
	ranksum1+=ranknum;
	ranksum2-=ranknum;
	}
for (i= 11; i<=count-10; i++){
	HelpCol3->FetchItemAt(i,&ranknum);
	ranksum1+=ranknum;
	ranksum2-=ranknum;
	v1=(double)ranksum1;
	v2=(double)ranksum2;
	cnt2=count-i;
	n1=(double)i;
	n2=(double)cnt2;
	KWStatt=(v1*v1/n1 + v2*v2/n2);
	KWStatt=KWStatt/((n1+n2)*(n1+n2+1));
	KWStatt=KWStatt*12.0;
	KWStatt=KWStatt-3*(n1+n2+1);
	myKW=(Float64)KWStatt;
	CStatsWindow::stDrawCol2->AddItem(&myKW,sizeof(Float64));
	}
delete (HelpCol1);
HelpCol1=nil;
delete (HelpCol2);
HelpCol2=nil;
delete (HelpCol3);
HelpCol3=nil;


}
	
void DoStatPermutation()
/*
does permutations and counts how many times the given outcome or worse occurs.
it uses simCol2 without the interval containing the central value because 
you would not use it normally and it is not used in all other tests on
intervals.
*/
	{
	SInt32 i, j, locat, Intvals, occurs=0, rounds=10000;
	Float64 totval, val,pcenterval;
	SInt32 used=-100;
	pcenterval=CSpike6App::SpikeParams.brkpt;
	SInt32 totintvals=Intvals1+Intvals2;
	Float64 expduration = (CSpike6App::SpikeParams.brkpt-CSpike6App::SpikeParams.strt);//(CSpike6App::SpikeParams.stp-CSpike6App::SpikeParams.strt);
	//SInt32 expected=round(proportion*totintvals);
	const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	UInt32 myseed=TickCount();
	gsl_rng_set(r,myseed);
	i=0;
	for (i=1;i<=rounds; i++)
		{
		used=-100.0;
		totval	= 0;
		Intvals	= 0;
		LArray *usd;
		usd = new LArray(sizeof(SInt32));
		ThrowIfNil_(usd);
		for (j=1; j<=totintvals; j++)//
			{
			usd->AddItem(&used, sizeof(SInt32));
			}
		//get Intvals1 intervals and see whether they add up to the measured time
		//if the intervals had been randomly distributed, they would add up
		//to the measured time in 50% of the runs
		for (j=1; j<=Intvals1; j++) {
			used=1;
			while (used>0){//find next unused interval 
				//locat = round(fabs(totintvals*PRNG.Uniform_0_1()))+1;
				locat = round(fabs(gsl_rng_uniform(r)*totintvals)) + 1;
				if (locat <= totintvals) 
					{
					usd->FetchItemAt(locat, &used);
					}
				}
			//we found an unused interval
			HelpCol1->FetchItemAt(locat, &val);//get the interval
			totval+=val;
			Intvals+=1;
			used=100;
			usd->AssignItemsAt(1,locat,&used);
			}
		if (totval <= expduration){
			occurs+=1;
			}
		delete(usd);
		usd=nil;
		}
	CStatsWindow::PermProb=(Float64)occurs/(Float64)rounds;
	}

void DoKruskal(Float64 *Array1, Float64 *Array2)
	{
	//SInt32 i=1,j=1,k;
	SInt32 i=0,j=0,k;
	Float64 val,valu,tStat,tStat1=0;
	Float64 RankS1=0;
	Float64 RankS2=0;
	Float64 mmedian=0.0;
	Float64 half=(Intvals1+Intvals2 - 1)/2;
	half = round(half);

	k=1;
	while ((i<Intvals1) && (j<Intvals2))
		{
		//IntvArray1->FetchItemAt(i,&val);
		val=Array1[i];
		//IntvArray2->FetchItemAt(j,&valu);
		valu=Array2[j];
		if(val<=valu)
			{
			if(k==half)
				{
				mmedian=val;
				}
			if(k==(half+1))
				{
				mmedian=(mmedian+val)/2;
				}
			RankS1+=k;
			k+=1;
			i+=1;
			}
		else if (val>valu)
			{
			if(k==half)
				{
				mmedian=valu;
				}
			if(k==(half+1))
				{
				mmedian=(mmedian+valu)/2;
				}

			RankS2+=k;
			k+=1;
			j+=1;
			}
		}
	if (i>=Intvals1)
		{
		while (j<Intvals2)
			{
			if(k==half)
				{
				mmedian=valu;
				}
			if(k==(half+1))
				{
				mmedian=(mmedian+valu)/2;
				}
			RankS2+=k;
			k+=1;
			j+=1;
			}
		}
	else if (j>=Intvals2)
		{
		while (i<Intvals1)
			{
			if(k==half)
				{
				mmedian=valu;
				}
			if(k==(half+1))
				{
				mmedian=(mmedian+valu)/2;
				}
			RankS1+=k;
			k+=1;
			i+=1;
			}
		}
	tStat=RankS1/(Float64)Intvals1;
	tStat=tStat*RankS1;
	tStat1=RankS2/(Float64)Intvals2;
	tStat1=tStat1*RankS2;
	tStat=tStat+tStat1;
	tStat=12.0*tStat/(((Float64)Intvals1+(Float64)Intvals2)*((Float64)Intvals1+(Float64)Intvals2+1.0));
	tStat1=3.0*((Float64)Intvals1+(Float64)Intvals2+1.0);
	tStat=tStat-tStat1;
	CStatsWindow::sMedian=mmedian;
	CStatsWindow::sKWStat=tStat;
	}
	
	
Float64 probks(Float64 alam)
	{
	SInt32 j;
	Float64 a2,fac=2.0,sum=0.0,term,termbf=0.0;
	
	a2=-2.0*alam*alam;
	for (j=1;j<=100;j++)
		{
		term=fac*exp(a2*j*j);
		sum+=term;
		if (fabs(term)<=EPS1*termbf||fabs(term)<=EPS2*sum) return sum;
		fac=-fac;
		termbf=fabs(term);
		}
	return 1.0;
	}

void DoKolmogorov(Float64 *Array1, Float64 *Array2)
	{
	//SInt32 j1=1,j2=1;
	SInt32 j1=0,j2=0;
	Float64 dt,en1,en2,en,fn1=0.0,fn2=0.0;
	Float64 prob, val1, val2, KSStat;
	
	en1=Intvals1;
	en2=Intvals2;
	KSStat=0.0;
	//while (j1 <= en1 && j2 <= en2)
	while (j1 < en1 && j2 < en2)
		{
		val1=Array1[j1];
		val2=Array2[j2];
		if ((val1) <= (val2)) {
			fn1=j1++/en1;
		}
		if (val2<=val1) {
			fn2=j2++/en2;
		}
		if ((dt=fabs(fn2-fn1)) > KSStat) {
			KSStat=dt;
		}
	} 
	en=sqrt(en1*en2/(en1+en2));
	prob=probks((en+0.12+0.11/en)*KSStat);
	CStatsWindow::sKSStat=prob;
}

void DoMedianTest(Float64 *Array1, Float64 *Array2) //needs MakeKWStat to be run first (for making median)
	{
	SInt32 i;
	Float64 val=0, mStat,mmedian=CStatsWindow::sMedian;
	Float64 expgtm1,expgtm2,expltm1,expltm2, NN;
	Float64 ltm1,ltm2, gtm1,gtm2;
	i=0;
	while ((val<=mmedian)&&(i<=Intvals1))
		{
		//i+=1;
		//IntvArray1->FetchItemAt(i, &val);
		val=Array1[i];
		i+=1;
		}
	ltm1=i-1;
	gtm1=Intvals1-ltm1;
	i=0;
	val=0;
	while ((val<=mmedian)&&(i<Intvals2))//!!!!!!!<=
		{
		//i+=1;
		//IntvArray2->FetchItemAt(i, &val);
		val=Array2[i];
		i+=1;
		}
	ltm2=i-1;
	gtm2=Intvals2-ltm2;
	//actual test will be done now
	
	NN=ltm1+ltm2+gtm1+gtm2;
	expltm1=(ltm1+ltm2)*(ltm1+gtm1)/NN;
	expltm2=(ltm1+ltm2)*(ltm2+gtm2)/NN;
	expgtm1=(gtm1+gtm2)*(ltm1+gtm1)/NN;
	expgtm2=(gtm1+gtm2)*(ltm2+gtm2)/NN;
	mStat=(ltm1-expltm1)*(ltm1-expltm1)/expltm1;
	mStat+=(ltm2-expltm2)*(ltm2-expltm2)/expltm2;
	mStat+=(gtm1-expgtm1)*(gtm1-expgtm1)/expgtm1;
	mStat+=(gtm2-expgtm2)*(gtm2-expgtm2)/expgtm2;
	CStatsWindow::sMedianStat=mStat;
	}

void RunStatPermutation()
	{
	//InitRand();
	HelpCol1 = new LArray(sizeof(Float64));
	HelpCol2 = new LArray(sizeof(Float64));
	MakeIntvArray3();
	DoStatPermutation();
	delete (HelpCol1);
	HelpCol1=nil;
	delete (HelpCol2);
	HelpCol2=nil;
	}
	
	
void RunKWStat()
	{
	HelpCol1 = new LArray(sizeof(Float64));
	HelpCol2 = new LArray(sizeof(Float64));
	MakeIntvArray3();
	
	size_t arsize1 = HelpCol1->GetCount();
	void * np1 = malloc (arsize1 * sizeof(Float64));
	Float64 *IntvArray1 = (Float64 *)np1;
	size_t arsize2 = HelpCol2->GetCount();
	void * np2 = malloc (arsize2 * sizeof(Float64));
	Float64 *IntvArray2 = (Float64 *)np2;
	void *mr1 = malloc (arsize1 * sizeof(Float64));
	Float64 *mar1 = (Float64 *)mr1;
	void *mr2 = malloc (arsize2 * sizeof(Float64));
	Float64 *mar2 = (Float64 *)mr2;

	/*IntvArray1 = new LArray(sizeof(Float64));
	IntvArray1->SetKeepSorted(TRUE);
	IntvArray2 = new LArray(sizeof(Float64));
	IntvArray2->SetKeepSorted(TRUE);*/
	
	//testing....
	UInt32 i;
	//Float64 mar1[100],mar2[100];
	for (i=0; i<arsize1; i++){
		mar1[i]=IntvArray1[i];
		}
	for (i=0; i<arsize2; i++){
		mar2[i]=IntvArray2[i];
		}
	RankIntervals(IntvArray1, IntvArray2);
	for (i=0; i<arsize1; i++){
		mar1[i]=IntvArray1[i];
		}
	for (i=0; i<arsize2; i++){
		mar2[i]=IntvArray2[i];
		}
	DoKruskal(IntvArray1, IntvArray2);
	for (i=0; i<arsize1; i++){
		mar1[i]=IntvArray1[i];
		}
	for (i=0; i<arsize2; i++){
		mar2[i]=IntvArray2[i];
		}
	DoMedianTest(IntvArray1, IntvArray2);
	/*delete (IntvArray1);
	IntvArray1=nil;
	delete (IntvArray2);
	IntvArray2=nil;*/
	delete (HelpCol1);
	HelpCol1=nil;
	delete (HelpCol2);
	HelpCol2=nil;
	}
	
void RunKSStat()
	{
	HelpCol1 = new LArray(sizeof(Float64));
	HelpCol2 = new LArray(sizeof(Float64));
	
	MakeIntvArray();
	
	size_t arsize1 = HelpCol1->GetCount();
	void * np1 = malloc (arsize1 * sizeof(Float64));
	Float64 *IntvArray1 = (Float64 *)np1;
	size_t arsize2 = HelpCol2->GetCount();
	void * np2 = malloc (arsize2 * sizeof(Float64));
	Float64 *IntvArray2 = (Float64 *)np2;

	RankIntervals(IntvArray1, IntvArray2);
	DoKolmogorov(IntvArray1, IntvArray2);
	delete (IntvArray1);
	IntvArray1=nil;
	delete (IntvArray2);
	IntvArray2=nil;
	delete (HelpCol1);
	HelpCol1=nil;
	}

Float64 RunKSStatProb()
	{
	SInt32 count=CSettingsWindow::DrawCol2->GetCount();
	SInt32 i,j,nsmaller=0;	
	Float64 prob, KSStat=0.0, val1, val2, dt;
	for (i=1;i<=count; i++)
		{
		CSettingsWindow::DrawCol3->FetchItemAt(i,&val2);
		for(j=1;j<=count;j++){
			CSettingsWindow::DrawCol2->FetchItemAt(j,&val1);
			if(val1>val2){
				nsmaller = j-1;
				break;
			}
		}
		dt=fabs((Float64)i-(Float64)nsmaller)/(Float64)count;		
		if (dt > KSStat) {
			KSStat=dt;
		}
	}
	Float64 en=sqrt(count*count/(2*count));
	prob=probks((en+0.12+0.11/en)*KSStat);
	return(prob);
	}

void RunACF()
	{
	HelpCol1 = new LArray(sizeof(Float64));//gets intervals
	MakeIntvArray();
	Float64 val0,valj,sval, sum0=0.0,ssum0=0.0;
	Float64 val, c0;
	SInt32 i,j, max;
	LArray *sums;
	sums=new LArray(sizeof(Float64));
	//fill arrays with zeroes
	val=0.0;
	for (i=1; i<=CStatsWindow::maxorder;i++)
		{
		sums->AddItem(&val, sizeof(Float64));
		}
	//in 1: order; in n: order-(n-1)
	max = HelpCol1->GetCount();
		for (i = 1; i<= max; i++)
		{
		HelpCol1->FetchItemAt(i,&val0);
		sum0 += val0; //sum of observations
		ssum0 += val0*val0;//sum of squared observations
		}
	c0=ssum0-sum0*sum0/max;
	Float64 avg0=sum0/max;
	for (i=1; i<max; i++)//step from first measurement until the next-to last
		{
		HelpCol1->FetchItemAt(i,&val0);
		for (j=1;j<=CStatsWindow::maxorder; j++)//step forward with j=lag 1...ord
			{
			if ((i+j)>max) {break;}
			HelpCol1->FetchItemAt(i+j,&valj);
			sval=(val0-avg0)*(valj-avg0);
			sums->FetchItemAt(j,&val);
			val+=sval;
			sums->AssignItemsAt(1,j,&val, sizeof(Float64));
			}
		}
		
	//make correlation coefficients
	
	Float64 rxy, sum;
	for (i=1; i<=CStatsWindow::maxorder; i++)
		{
		sums->FetchItemAt(i, &sum);
		rxy=sum/c0;
		CStatsWindow::sACFray->AssignItemsAt(1,i,&rxy, sizeof(Float64));
		}
	delete (sums);
	sums=nil;
	//RunPACF();
	delete (HelpCol1);
	HelpCol1=nil;
	}

void RunPACF()//called from ACF; Helpcol is already in place
	{
	Float64 phi[241], rhoA, rhoB, rhoC, sumA, sumB;
	SInt32 j,p, mxorder=CStatsWindow::maxorder;
	for (j=0; j<=241;j++)
		{phi[j]=0.0000;}
	CStatsWindow::sACFray->FetchItemAt(1,&rhoA);
	CStatsWindow::sACFray->FetchItemAt(2,&rhoB);
	phi[16]=rhoA;
	phi[31]=rhoA*(1-rhoB)/(1-rhoA*rhoA);
	phi[32]=(rhoB-rhoA*rhoA)/(1-rhoA*rhoA);
	for (p=2;p<mxorder;p++)
		{
		CStatsWindow::sACFray->FetchItemAt(p+1,&rhoA);
		sumA=0.0;
		sumB=0.0;
		for (j=1;j<=p;j++)
			{
			CStatsWindow::sACFray->FetchItemAt(p+1-j,&rhoB);
			sumA+=phi[p*mxorder+j]*rhoB;
			CStatsWindow::sACFray->FetchItemAt(j,&rhoC);
			sumB+=phi[p*mxorder+j]*rhoC;
			}
		phi[(p+1)*mxorder+p+1]=(rhoA-sumA)/(1-sumB);
		for (j=1;j<=p;j++)
			{
			phi[(p+1)*mxorder+j]=phi[p*mxorder+j]-phi[(p+1)*mxorder+(p+1)]*phi[p*mxorder+(p-j+1)];
			}
		}
	Float64 pxy=0;
	for (p=1;p<=mxorder;p++)
		{
		pxy=phi[mxorder*p+p];
		CStatsWindow::sPACFray->AssignItemsAt(1,p,&pxy, sizeof(Float64));
		}
	}
	
	
Float64 RunPHI(){//estimates the AR-coefficient from the data (only for first-order)
	RunACF();
	SInt32 nn, i;
	Float64 val, inverts[25];
	nn=CStatsWindow::sACFray->GetCount();
	//transform regression coefficients to linear with order
	for (i=1; i<=nn;i++){
		CStatsWindow::sACFray->FetchItemAt(i, &val);
		inverts[i]=log(1/(fabs(val)));
		}
	Float64 sumx=0.0, sumy=0.0, sumxx=0.0, sumxy=0.0;
	for (i=1; i<=nn; i++){
		sumxy+=(Float64)i*inverts[i];
		sumxx+=i*i;
		sumy+=inverts[i];
		sumx+=i;
		}
	val=(sumxy-sumx*sumy/(Float64)nn)/(sumxx-sumy*sumy/(Float64)nn);
	val=powf(10,val);
	val=1/val;
	return val;
}

Float64 RunTheta(){
	RunPACF();
	SInt32 nn, i;
	Float64 val, rt1,rt2,inverts[25];
	nn=CStatsWindow::sPACFray->GetCount();
	//transform regression coefficients to linear with order
	for (i=1; i<=nn;i++){
		CStatsWindow::sPACFray->FetchItemAt(i, &val);
		inverts[i]=log(1/(fabs(val)));
		}
	CStatsWindow::sPACFray->FetchItemAt(1, &val);
	if ((1-4*val*val) >=0){
		rt1=(-1+sqrt(1-4*val*val))/(2*val);
		rt2=(-1-sqrt(1-4*val*val))/(2*val);
		val=rt1;
		}
	else {
		rt1=0.0;
		rt2=0.0;
		val=rt1;
		}
	return val;
	}

void ReconstructAR(){//does not yet use 
	Float64 avrg, stdev, avrg2, avrg3, stdev2, stdev3, myPhi, aa, intval, lastintval, sum;
	SInt32 i, nn, Templ, interv1, interv2, interv3;
	RunBasicStatsNew(&avrg, &stdev,&avrg2, &stdev2, &avrg3, &stdev3, &interv1, &interv2, &interv3);
	myPhi=RunPHI();//runs acf, which makes helpcol1 and deletes it again
	//myPhi=0.800;
	HelpCol1 = new LArray(sizeof(Float64));//gets intervals
	HelpCol2 = new LArray(sizeof(Float64));//stores A's
	MakeIntvArray();
	Templ=CSpike6App::sAvgCol1->GetCount()+1;
	nn=HelpCol1->GetCount();
	HelpCol1->FetchItemAt(nn,&intval);
	intval-=avrg;
	sum=0;
	for (i=nn-1; i>=1; i--){
		HelpCol1->FetchItemAt(i,&lastintval);
		lastintval-=avrg;
		aa=intval-myPhi*lastintval;
		sum+=aa;
		HelpCol2->AddItem(&aa, sizeof(Float64));
		intval=lastintval;
		}
	CSpike6App::templrecrd theTRec;	
	CSpike6App::tRecord->FetchItemAt(1,&theTRec);
	sum=theTRec.timestamp;	
	delete CSpike6App::tRecord;
	CSpike6App::tRecord=nil;
	CSpike6App::tRecord = new LArray(sizeof(Float64));
	CSpike6App::sdim2=3;
	Templ=1;
	nn=HelpCol2->GetCount();
	for (i=1; i<=nn; i++){
		theTRec.timestamp=sum;
		theTRec.templatenum=Templ;
		HelpCol2->FetchItemAt(i,&intval);
		intval+=avrg;//since only the a's are in helpcol2 we need to bring them to average levels
		sum+=intval;
		}
	
	delete (HelpCol1);
	HelpCol1=nil;

	}


void ReconstructMA(){
	;
	}
	
void RunReturn()
	{
	SInt32 i;
	Float64 val=0.0;
	HelpCol1 = new LArray(sizeof(Float64));
	if (CSpike6App::SpikeParams.brklin != 0.0)
		{
		HelpCol2 = new LArray(sizeof(Float64));
		}
	MakeIntvArray2();
	SInt32 max=HelpCol1->GetCount();
	HelpCol1->FetchItemAt(1,&val);
	CStatsWindow::stDrawCol1->AddItem(&val, sizeof(Float64));
	for (i=2; i<=max-1; i++)
		{
		HelpCol1->FetchItemAt(i,&val);
		CStatsWindow::stDrawCol1->AddItem(&val, sizeof(Float64));
		CStatsWindow::stDrawCol2->AddItem(&val, sizeof(Float64));
		}
	delete (HelpCol1);
	HelpCol1=nil;
	//after breakpoint
	if (CSpike6App::SpikeParams.brklin != 0.0)
		{
		max=HelpCol2->GetCount();
		HelpCol2->FetchItemAt(1,&val);
		CStatsWindow::stDrawCol1a->AddItem(&val, sizeof(Float64));
		for (i=2; i<=max-1; i++)
			{
			HelpCol2->FetchItemAt(i,&val);
			CStatsWindow::stDrawCol1a->AddItem(&val, sizeof(Float64));
			CStatsWindow::stDrawCol2a->AddItem(&val, sizeof(Float64));
			}
		delete (HelpCol2);
		HelpCol2=nil;
		}
	}
	
void RunRankedReturn()
	{
	UInt32 i;
	ArrayIndexT j;
	Float64 jj;
	Float64 val=0.0;
		
	HelpCol1 = new LArray(sizeof(Float64));
	if (CSpike6App::SpikeParams.brklin != 0.0) 
		{
		HelpCol2 = new LArray(sizeof(Float64));
		}
	MakeIntvArray2();//yields helpcol1 (and 2 if there is a breakpoint). These are unsorted
	size_t arsize1 = HelpCol1->GetCount();
	void * np1 = malloc (arsize1 * sizeof(Float64));
	Float64 *IntvArray1 = (Float64 *)np1;
	SInt32 max=HelpCol1->GetCount();
	j=0;
	for (i=1; i<=max; i++) //create a second array with intervals sorted small to large
		{
		HelpCol1->FetchItemAt(i,&val);
		//IntvArray1->AddItem(&val);
		IntvArray1[j]=val;
		j+=1;
		}
	sortFloat64(IntvArray1, arsize1);
	HelpCol1->FetchItemAt(1,&val);			//get first value
	jj=GetRank(IntvArray1, val, arsize1)+1;
	CStatsWindow::stDrawCol1->AddItem(&jj, sizeof(Float64));
	for (i=2; i<=max; i++) // get rank order of ith interval and put it in correct places
		{
		HelpCol1->FetchItemAt(i,&val);			//get ith value
		jj=GetRank(IntvArray1, val,arsize1) + 1;
		CStatsWindow::stDrawCol1->AddItem(&jj, sizeof(Float64));
		CStatsWindow::stDrawCol2->AddItem(&jj, sizeof(Float64));
		}
	delete (HelpCol1);
	HelpCol1=nil;

	//after breakpoint
	
	if (CSpike6App::SpikeParams.brklin!=0)
		{
		max=HelpCol2->GetCount();
		size_t arsize2 = HelpCol2->GetCount();
		void * np2 = malloc (arsize2 * sizeof(Float64));
		Float64 *IntvArray2 = (Float64 *)np2;
		j=0;
		for (i=1; i<=max; i++) //create a second array with intervals sorted small to large
			{
			HelpCol2->FetchItemAt(i,&val);
			IntvArray2[j]=val;
            j+=1;
		}
		sortFloat64(IntvArray2, arsize2);
		HelpCol2->FetchItemAt(1,&val);			//get first value
		jj=GetRank(IntvArray2, val,arsize2) +1;
		CStatsWindow::stDrawCol1a->AddItem(&jj, sizeof(Float64));
		for (i=2; i<=max; i++) // put rank order in place of interval value in helpcol1
			{
			HelpCol2->FetchItemAt(i,&val);			//get ith value
			jj=GetRank(IntvArray2, val,arsize2);
			CStatsWindow::stDrawCol1a->AddItem(&jj, sizeof(Float64));
			CStatsWindow::stDrawCol2a->AddItem(&jj, sizeof(Float64));
			}
		delete (HelpCol2);
		HelpCol2=nil;
		}
	DoKSReturn();
	}
	
	
void DoKSReturn(){//compare 2 dimensional cumulative distr. with 2D uniform.
//test statistic is largest difference between the experimental and the theoretical distribution, divided by N 
//Rhoades et al. (1996)
//"Distribution-free graphical and statistical analysis of serial dependence in neuronal spike trains"
SInt32 max1,max2, max3, max4, i, j, max, xcoord, ycoord;
Float64 xval,yval,val,valu,dif,maxdif,cnt;

	max1=CStatsWindow::stDrawCol1->GetCount();
	max2=CStatsWindow::stDrawCol2->GetCount();
	if(max1>max2){max=max2;}else if(max2>max1){max=max1;}else {max=max1;}
	Float64 **rankarr = (Float64 **)malloc (10*sizeof(Float64 *));//a 10x10 array to house the observed values
	for (i=0; i < 10; i++){
		rankarr[i]=(Float64 *)malloc(10*sizeof(Float64));
	}
	Float64 **theoryarr = (Float64 **)malloc (10*sizeof(Float64 *));//a 10x10 array to house the uniformly distributed values
	for (i=0; i < 10; i++){
		theoryarr[i]=(Float64 *)malloc(10*sizeof(Float64));
	}
	//sweep the data arrays
	for (i=0; i <10; i++){
		for (j=0;j<10;j++){
			rankarr[i][j]=0.0;
			theoryarr[i][j]=0.0;
		}
	}//arrays sweeped
	
	//fill the uniform array with uniformly distributed ata
	//version with random assigment of the counts in each bin 
	/*gsl_rng *r;
	const gsl_rng_type *T;//type of generator to use
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	UInt32 myseed=TickCount();
	gsl_rng_set(r,myseed);
	val = 2 * max * gsl_rng_uniform(r)/100;//gsl_rng_uniform is 0-1, avg 0.5. To get avg = observed we need 2*max/100*/
	
	/*for (i=0; i <10; i++){
		for (j=0;j<10;j++){
			val = 2 * max * gsl_rng_uniform(r)/100;
			theoryarr[i][j]=val;
		}
	}*/
	max3=CStatsWindow::stDrawCol1->GetCount();
	max4=CStatsWindow::stDrawCol2->GetCount();
	if (max3 != 0){
	;
	}
	//this is for testing the counts against the uniform distribution
	val=(Float64)max/100.0;
	for (i=0; i <10; i++){
		for (j=0;j<10;j++){
			theoryarr[i][j]=val;
		}
	}
	for (i = 1; i <=max; i++){
		CStatsWindow::stDrawCol1->FetchItemAt(i, &xval);
		CStatsWindow::stDrawCol2->FetchItemAt(i, &yval);
		xcoord=trunc(((xval-1)/max1)*10);
		ycoord=trunc(((yval-1)/max2)*10);
		if((ycoord<10)&&(xcoord<10)){
			rankarr[xcoord][ycoord] += 1;
		}
	}
	cnt=(Float64)max/100.0;
	val=0.0;
	valu=0.0;
	maxdif=0.0;
	for (i=0; i<=9;i++){
		for(j=0;j<=9;j++){
			val+=rankarr[i][j];
			valu+=theoryarr[i][j];
			dif=fabs(val-valu);
			if(dif>maxdif){
				maxdif=dif;
			}
		}
	}
	maxdif=maxdif/max;//this is the Ôjoint interval Kolmogorov-SmirnovÕ (JIKS) statistic.
	CStatsWindow::ksstat=maxdif;
	//Float64 en=sqrt(max)*maxdif;
	//Float64 prob=probks(en);
	CStatsWindow::critval1=-0.5*log10((Float64)max)+0.19089;
	val=log10((Float64)max);
	val=CStatsWindow::critval1;

	CStatsWindow::critval1=pow(10.0,CStatsWindow::critval1);
	val=CStatsWindow::critval1;

	CStatsWindow::critval2=-.5*log10((Float64)max)+0.28675;
	valu=CStatsWindow::critval2;

	CStatsWindow::critval2=pow(10.0,CStatsWindow::critval2);
	valu=CStatsWindow::critval2;

	val=CStatsWindow::critval1;
	valu=CStatsWindow::critval2;
	free(rankarr);
	free(theoryarr);

	}
	
void Run3iReturn()
	{
	SInt32 i;
	Float64 val=0.0, val1=0.0,val2=0.0,val3=0.0;
	HelpCol1 = new LArray(sizeof(Float64));
	if (CSpike6App::SpikeParams.brklin != 0.0) 
		{
		HelpCol2 = new LArray(sizeof(Float64));
		}
	MakeIntvArray();
	SInt32 max=HelpCol1->GetCount();
	HelpCol1->FetchItemAt(1,&val1);
	HelpCol1->FetchItemAt(2,&val2);
	for (i=3; i<=max; i++) 
		{
		HelpCol1->FetchItemAt(i,&val3);
		val=val2-val1;
		CStatsWindow::stDrawCol1->AddItem(&val, sizeof(Float64));
		val=val2-val3;
		CStatsWindow::stDrawCol2->AddItem(&val, sizeof(Float64));
		val1=val2;
		val2=val3;
		}

	delete (HelpCol1);
	HelpCol1=nil;
	//after breakpoint
	if (CSpike6App::SpikeParams.brklin!=0)
		{
		max=HelpCol2->GetCount();
		HelpCol2->FetchItemAt(1,&val1);
		HelpCol2->FetchItemAt(2,&val2);
		for (i=3; i<=max; i++) 
			{
			HelpCol2->FetchItemAt(i,&val3);
			val=val2-val1;
			CStatsWindow::stDrawCol1a->AddItem(&val, sizeof(Float64));
			val=val2-val3;
			CStatsWindow::stDrawCol2a->AddItem(&val, sizeof(Float64));
			val1=val2;
			val2=val3;
			}
		delete (HelpCol2);
		HelpCol2=nil;
		}
	}

void RunR3iReturn()
	{
	SInt32 i,j;
	Float64 val=0.0, val1=0.0,val2=0.0,val3=0.0, jj=0.0;
	HelpCol1 = new LArray(sizeof(Float64));
	//IntvArray1 = new LArray(sizeof(Float64));
	//IntvArray1->SetKeepSorted(TRUE);
	if (CSpike6App::SpikeParams.brklin != 0.0) 
		{
		HelpCol2 = new LArray(sizeof(Float64));
		//IntvArray2 = new LArray(sizeof(Float64));
		//IntvArray2->SetKeepSorted(TRUE);
		}
	MakeIntvArray();
	
	size_t arsize1 = HelpCol1->GetCount();
	void * np1 = malloc (arsize1 * sizeof(Float64));
	Float64 *IntvArray1 = (Float64 *)np1;
	
	SInt32 max=HelpCol1->GetCount();
	j=0;
	for (i=1; i<=max; i++) //create an array with intervals sorted small to large
		{
		HelpCol1->FetchItemAt(i,&val);
		//IntvArray1->AddItem(&val);
		IntvArray1[j]=val;
		j+=1;
		}
	sortFloat64(IntvArray1, arsize1);
	for (i=1; i<=max; i++) // put rank order in place of interval value in helpcol1
		{
		HelpCol1->FetchItemAt(i,&val);			//get ith value
		j=GetIndex(IntvArray1, val, arsize1)+1;//IntvArray1->FetchIndexOf(&val,sizeof(Float64));	//get rank number of ith value from sorted array
		jj=j;
		HelpCol1->AssignItemsAt(1,i,&jj,sizeof(Float64));	//put rank number in ith place, where the value was taken from
		}

	//after breakpoint
	if (CSpike6App::SpikeParams.brklin!=0)
		{
		max=HelpCol2->GetCount();
		size_t arsize2 = HelpCol2->GetCount();
		void * np2 = malloc (arsize2 * sizeof(Float64));
		Float64 *IntvArray2 = (Float64 *)np2;
		j=0;
		for (i=1; i<=max; i++) //create a second array with intervals sorted small to large
			{
			HelpCol2->FetchItemAt(i,&val);
			IntvArray2[j]=val;//->AddItem(&val);
			j+=1;
			}
		for (i=1; i<=max; i++) // put rank order in place of interval value in helpcol1
			{
			HelpCol2->FetchItemAt(i,&val);			//get ith value
			//j=IntvArray2->FetchIndexOf(&val,sizeof(Float64));	//get rank number of ith value from sorted array
			j=GetIndex(IntvArray2, val,arsize2) + 1;
			jj=j;
			HelpCol2->AssignItemsAt(1,i,&jj,sizeof(Float64));	//put rank number in ith place, where the value was taken from
			}
		}
	max=HelpCol1->GetCount();
	HelpCol1->FetchItemAt(1,&val1);
	HelpCol1->FetchItemAt(2,&val2);
	for (i=3; i<=max; i++) 
		{
		HelpCol1->FetchItemAt(i,&val3);
		val=val2-val1;
		CStatsWindow::stDrawCol1->AddItem(&val, sizeof(Float64));
		val=val3-val2;
		CStatsWindow::stDrawCol2->AddItem(&val, sizeof(Float64));
		val1=val2;
		val2=val3;
		}
	delete (HelpCol1);
	HelpCol1=nil;
	//after breakpoint
	if (CSpike6App::SpikeParams.brklin!=0)
		{
		max=HelpCol2->GetCount();
		HelpCol2->FetchItemAt(1,&val1);
		HelpCol2->FetchItemAt(2,&val2);
		for (i=3; i<=max; i++) 
			{
			HelpCol2->FetchItemAt(i,&val3);
			val=val2-val1;
			CStatsWindow::stDrawCol1a->AddItem(&val, sizeof(Float64));
			val=val3-val2;
			CStatsWindow::stDrawCol2a->AddItem(&val, sizeof(Float64));
			val1=val2;
			val2=val3;
			}
		delete (HelpCol2);
		HelpCol2=nil;
		}
	}

void RunBasicStatsNew(Float64 *avpt1, Float64 *sdevpt1, Float64 *avpt2, Float64 *sdevpt2, Float64 *avpt3, Float64 *sdevpt3,
					  SInt32 *intvls1, SInt32 *intvls2, SInt32 *intvls3){
	SInt32 	i, start, nn=0, count, templ;
	Float64	intv, val,oldval=0,sumx=0.0,sumxx=0.0,break1=CSpike6App::SpikeParams.brkpt,break2=CSpike6App::SpikeParams.brkpt2;
	//Float64 start=CSpike6App::SpikeParams.strt, stop=CSpike6App::SpikeParams.stp;
	SInt32 strtcnt=CSpike6App::SpikeParams.minlin, stpcnt=CSpike6App::SpikeParams.maxlin; //where to start and where to stop
	count=CSpike6App::tRecord->GetCount();
	CSpike6App::templrecrd rec;
	
	templ=CSpike6App::SpikeParams.templ1;
	if ((break1==0)&&(break2==0)){//when there is no first breakpoint
		for(i=strtcnt;i<=stpcnt;i++){
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if(rec.templatenum==templ){
				oldval=rec.timestamp;
				start=i;
				break;
			}
		}
		for(i=start;i<=stpcnt;i++){
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if(rec.templatenum==templ){
				val=rec.timestamp;
				intv=val-oldval;
				sumx+=intv;
				sumxx+=intv*intv;
				nn+=1;
				oldval=val;
			}
		}
		nn-=1;//nn was number of occurrences; is now number of intervals(1 smaller)
		if(nn>1){
			*avpt1=sumx/(Float64)nn;
			*sdevpt1=sqrt((sumxx-sumx*sumx/(Float64)nn)/((Float64)nn-1));
			*intvls1=nn;
		}else {
			*avpt1=0;
			*sdevpt1=0;
			*intvls1=1;
		}
		sumx=0.0;
		sumxx=0.0;
		nn=0;
		
	}
	else if(break1!=0.0){
		for(i=strtcnt;i<=stpcnt;i++){//find first occurrence op templ-before breakpoint-get time
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if(rec.templatenum==templ){
				oldval=rec.timestamp;
				start=i;
				break;
			}
		}//what if there was no occurrence?
		for (i=start;i<=stpcnt;i++){
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if(rec.timestamp>break1){
				start=i+1;
				break;//go to second part of the data after break1
			}
			if(rec.templatenum==templ){
				val=rec.timestamp;
				intv=val-oldval;
				sumx+=intv;
				sumxx+=intv*intv;
				nn+=1;
				oldval=val;
			}
		}
		nn-=1;
		if (nn>1){
			*avpt1=sumx/(Float64)nn;
			*sdevpt1=sqrt((sumxx-sumx*sumx/(Float64)nn)/((Float64)nn-1));
			*intvls1=nn;
		} else {
			*avpt1=0;
			*sdevpt1=0;
			*intvls1=1;
		}
		sumx=0.0;
		sumxx=0.0;
		nn=0;		
		
		//second section starts at start
		
		for(i=start;i<=stpcnt;i++){//find first occurrence op templ-get time
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if(rec.templatenum==templ){
				oldval=rec.timestamp;
				start=i;
				break;
			}
		}
		for (i=start;i<=stpcnt;i++){
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if((rec.timestamp>break2)&&(break2 != 0.0)){
				start=i;
				break;//go to second part of the data after break1
			}
			if(rec.templatenum==templ){
				val=rec.timestamp;
				intv=val-oldval;
				sumx+=intv;
				sumxx+=intv*intv;
				nn+=1;
				oldval=val;
			}
		}
		nn-=1;
		if (nn>1){
			*avpt2=sumx/(Float64)nn;
			*sdevpt2=sqrt((sumxx-sumx*sumx/(Float64)nn)/((Float64)nn-1));
			*intvls2=nn;
		} else {
			*avpt2=0;
			*sdevpt2=0;
			*intvls2=1;
		}
		sumx=0.0;
		sumxx=0.0;
		nn=0;
		start=i;
		
		//third section starts at start
		
		for(i=start;i<=stpcnt;i++){//find first occurrence op templ-get time
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if(rec.templatenum==templ){
				oldval=rec.timestamp;
				start=i;
				break;
			}
		}
		for (i=start;i<=stpcnt;i++){
			CSpike6App::tRecord->FetchItemAt(i,&rec);
			if(rec.templatenum==templ){
				val=rec.timestamp;
				intv=val-oldval;
				sumx+=intv;
				sumxx+=intv*intv;
				nn+=1;
				oldval=val;
			}
		}
		nn-=1;
		if (nn>1){
			*avpt3=sumx/(Float64)nn;
			*sdevpt3=sqrt((sumxx-sumx*sumx/(Float64)nn)/((Float64)nn-1));
			*intvls3=nn;
		} else {
			*avpt3=0;
			*sdevpt3=0;
			*intvls3=1;
		}
		sumx=0.0;
		sumxx=0.0;
		nn=0;
	}
}
	
CSpike6App::BinRecord PermuteList(UInt32 SoMany){
		Float64 val, valu;
		UInt32 i, j;
		CSpike6App::BinRecord blist;
		LArray *mylist, *helplist;
		mylist=new LArray(sizeof(Float64));
		ThrowIfNil_(mylist);
		helplist=new LArray(sizeof(Float64));
		ThrowIfNil_(helplist);
		const gsl_rng_type *T;//type of generator to use
		gsl_rng *r;
		gsl_rng_env_setup();
		T=gsl_rng_default;
		r= gsl_rng_alloc (T);
		for (i=1; i<=SoMany; i++){
			//val=10000.00*PRNG.Uniform_0_1();//PRNG.Normal();
			val = 10000.00*(gsl_rng_uniform(r));
			mylist->AddItem(&val);
			helplist->AddItem(&val);
			}
		helplist->Sort();
		for (i=1; i<=SoMany; i++){
			helplist->FetchItemAt(i,&val);
			for (j=1; j<=SoMany; j++){
				mylist->FetchItemAt(j, &valu);
				if ((fabs(val-valu))<0.00001){
					blist.bins[i-1]=j;
					}
				}
			}
		delete mylist;
		delete helplist;
		mylist=nil;
		helplist=nil;
		return blist;
		}
		
	void RunCumPSTH()
		{
		SInt32 i, j, maxruns=1000, count, halfcount, locat;
		CSpike6App::BinRecord brec, permRec;
		Float64 val, valu, average=0.0, lastval, tlastval, binval;
		//SInt32 used=-100;
		//AllDensity(2);//this results in the alldensity(not)histogram of the original data. and Is put into stDrawCol1 and 2
		AllDensityGSL(2);
		//the histogram data of the original spike train are stored in stDrawcol2a.
		count=CStatsWindow::stDrawCol2->GetCount();
		halfcount=round((Float64)count/2.0);
		LArray *LastHalf=new LArray(sizeof(Float64)); 
		for (i=halfcount+1;i<=count;i++){
			CStatsWindow::stDrawCol2->FetchItemAt(i,&val);
			LastHalf->AddItem(&val);
			}
		for (i=1; i<=halfcount; i++){
			CStatsWindow::stDrawCol2->FetchItemAt(i, &val);
			average+=val;
			}
		average=average/(Float64)halfcount;
		CStatsWindow::stDrawCol2->FetchItemAt(1, &val);
		val-=average;
		CStatsWindow::stDrawCol2->AssignItemsAt(1,1,&val);//puts ORIGINAL cumPSTH into stDrawCol2.
		for (i=2; i<=count; i++){
			CStatsWindow::stDrawCol2->FetchItemAt(i, &valu);
			val +=(valu-average);
			if (i== halfcount){
				lastval=val;//keep this for producing the randomized cumPSTH's
				}
			CStatsWindow::stDrawCol2->AssignItemsAt(1,i,&val);
			}
		//now we randomise the intervals maxruns times and generate a cumPSTH everytime
		//first, maka a help array to keep track of the locations that have been used already
		LArray *hlp;//a helpcolumn to determine the 5 and 95 % confIntervals
		hlp = new LArray(sizeof(Float64));
		ThrowIfNil_(hlp);
		binval=0.00;
		for (i=1; i<=maxruns; i++){
			hlp->AddItem(&binval);
			}
		delete CSpike6App::bRecord;
		CSpike6App::bRecord=nil;
		CSpike6App::bRecord=new LArray(sizeof(CSpike6App::BinRecord));
		ThrowIfNil_(CSpike6App::bRecord);
		//we already know lastval i.e. the value at the end of the pre-stimulus period
		for (i=1; i<=maxruns;i++){
			//get permuted list
			tlastval=lastval;
			permRec=PermuteList(halfcount);
			for(j=0; j<=halfcount-1; j++){//put the known content of each bin in stDrawcol2a into a brec.
				locat=permRec.bins[j];
				LastHalf->FetchItemAt(locat,&val);
				binval=tlastval + val - average;
				brec.bins[j]=binval;
				tlastval=binval;
				}
			CSpike6App::bRecord->AddItem(&brec);//puts a permuted value of the original spiketrain into the BinRecord
			}
		//convert the bRec data into 
		count=CSpike6App::bRecord->GetCount();
		//for each of the 100 columns in bRecord (bins[0]-bins[99], sort the data low to high, determine the 5% level and the 95%level. 
		//Put these levels in stDrawCol3 and -4. 
		for (i=0; i<=99; i++){
			for (j=1; j<=count; j++){
				CSpike6App::bRecord->FetchItemAt(j,&brec);
				val=brec.bins[i] +10000;//note, this may be too small (10000)
				hlp->AssignItemsAt(1, j, &val);
				}
			hlp->Sort();
			locat=round(0.05*(Float64)count);
			hlp->FetchItemAt(locat, &val);
			val-=10000;
			CStatsWindow::stDrawCol3->AddItem(&val);//lower confidence limit
			locat=round(0.95*(Float64)count);
			hlp->FetchItemAt(locat, &val);
			val-=10000;
			CStatsWindow::stDrawCol4->AddItem(&val);//upper confidence limit
			}
		delete hlp;
		delete LastHalf;
		hlp=nil;
		LastHalf=nil;
		}
		
//-------------------------------------------------------------------------------------------------------------------------
//		InitCumPSTH()
//-------------------------------------------------------------------------------------------------------------------------
	
void InitCumPSTH()//initialize the process BOTH histogram assessment AND CumPSTH randomization
//PThelp contains the intervals instead of spike times
{
	//first,clear the drawing arrays stDrawcol3 and stDrawcol4
	
	SInt32 i, histmax;
	Float64 val, valu, tim, oldtim, totview;
	Float64 bwdt=CSpike6App::SpikeParams.BWdth/1000.0;
	SInt32 maxbins=2*CSpike6App::SpikeParams.maxbns;
	SInt32 halfbins=maxbins/2; 
	//produce a help array containing the times of occurrence of the target template
	CStatsWindow::PThelp=new LArray(sizeof(Float64)); 
	CStatsWindow::PTintervals=new LArray(sizeof(Float64)); 

	ThrowIfNil_(CStatsWindow::PThelp);
	SInt32 tot=0, templ;
	CSpike6App::templrecrd theTRec;
	CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
	oldtim = theTRec.timestamp;
	CStatsWindow::PThelp->AddItem(&oldtim, sizeof(Float64));
	for (i = CSpike6App::SpikeParams.minlin+1; i <= CSpike6App::SpikeParams.maxlin; i++)
	{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ2)
		{
			tim=theTRec.timestamp;
			val=tim-oldtim;
			CStatsWindow::PThelp->AddItem(&tim, sizeof(Float64));
			CStatsWindow::PTintervals->AddItem(&val, sizeof(Float64));
			oldtim=tim;
		}
	}
	
	//PThelp contains a list of spike occurrences of the chosen template within the specified stretch of data		
	//PTintervals is a list of intervals to be shuffled with each randomization
	
	
	//Get parameters for randomization procedure. Stimulus must be placed within the selected stretch of data
	//the locations of the simulated stimuli must be such that the view in each direction does not
	//stretch beyond the time span of PTHelp
	CStatsWindow::PTcount=CStatsWindow::PThelp->GetCount();
	CStatsWindow::PThelp->FetchItemAt(1,&valu);
	CStatsWindow::PThelp->FetchItemAt(CStatsWindow::PTcount,&val);
	totview=CSpike6App::SpikeParams.BWdth*halfbins/1000.00;//get the totview in seconds
	CStatsWindow::PTmintime=valu;
	CStatsWindow::PTmaxtime=val - totview;
	CStatsWindow::PTspan=CStatsWindow::PTmaxtime-CStatsWindow::PTmintime;
	//determine the number of stimuli within the span of allowable values (PTspan); collect stimulus times
	tot=0;
	CStatsWindow::PTstims=new LArray(sizeof(Float64));//PTstims contains the stimulus times
	for (i = CSpike6App::SpikeParams.minlin; i <= CSpike6App::SpikeParams.maxlin; i++){
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ1){//if it is a stimulus
			// is it within PTspan ?
			if ((theTRec.timestamp>=CStatsWindow::PTmintime)&&(theTRec.timestamp<=CStatsWindow::PTmaxtime)){
				tot +=1;
				tim=theTRec.timestamp;
				CStatsWindow::PTstims->AddItem(&tim);//put the stimulus in help
			}
		}
	}
	CStatsWindow::PTmaxstim=tot;//is the total number of stimuli within our scope(=number of entries in help)
	delete CSpike6App::bRecord;
	CSpike6App::bRecord=nil;
	CSpike6App::bRecord=new LArray(sizeof(CSpike6App::BinRecordLong));//bRecord contains the histogram data
	//now make the original cumPSTH
	CSpike6App::BinRecordLong brec;
	for (i=0;i<=(maxbins-1);i++){//fill brecord with zeroes
		brec.bins[i]=0.0;
	}
	SInt32 stimcount=CStatsWindow::PTstims->GetCount();//stimcount=CSpike6App::PTmaxstim=tot. Unclutter???
	SInt32 count=CStatsWindow::PThelp->GetCount(), j, k, binnum;//count=number of target spikes
	SInt32 lastline=1;//time where we started the binning the last time (saves having to go through the whole pthelp
	//with every stimulus
	Float64 targettime, intv;
	CStatsWindow::PTstims->FetchItemAt(1,&tim);//fetch time of first stimulus
	for (i=1; i<=stimcount; i++){
		CStatsWindow::PTstims->FetchItemAt(i,&tim);//fetch time of stimulus
		for (j=lastline; j<=count; j++){//starting at where we were, run through the spikes until we are out of the window
			CStatsWindow::PThelp->FetchItemAt(j,&targettime);
			if (targettime>=tim){
				lastline = j;//this could go later to improve efficiency
				for (k=j-1; k >= 1; k--){//looking back from the stimulus time (tim)
					CStatsWindow::PThelp->FetchItemAt(k, &targettime);
					intv=tim-targettime;
					binnum = halfbins-1 - trunc(intv / bwdt);
					if (binnum < 0){
						break;
					}
					brec.bins[binnum]+=1;
				}
				for (k=j; k <= count; k++){//looking forward from stimulus time
					CStatsWindow::PThelp->FetchItemAt(k, &targettime);
					intv=targettime-tim;
					binnum = halfbins + trunc(intv / bwdt);
					if (binnum > maxbins-1){
						break;
					}
					brec.bins[binnum]+=1;
				}
				break;
			}
		}
	}
	//now, if the stimulus equals the target ie we do the cumpsth with itself,
	//remove all cases in bin 100 where stimulus equalled target.
	//i.e. subtract N-stimuli from the brec.binnum array at location 100.
	if (CSpike6App::SpikeParams.templ1==CSpike6App::SpikeParams.templ2){
		brec.bins[halfbins]-=stimcount;
	}
	//make it a cumpsth, if the HistORCumPSTHFlag is false, make the histogram if HistORCumPSTHFlag is true
	Float64 lastval=0.0;
	if (!CStatsWindow::HistORCumPSTHFlag){// cumpsth from brec.bins
		valu=0.0;
		val=0.0;
		for (j=0; j<halfbins; j++){
			val+=brec.bins[j];
		}
		val=val/((Float64)halfbins);//average number of targets before occurrence of the stimulus
		//ensures that at bin=halfbins, the sum of the bins is zero
		lastval=0.0;
		for (j=0; j<maxbins; j++){
			valu=brec.bins[j]-val + lastval;
			CStatsWindow::stDrawCol2->AddItem(&valu);
			lastval=valu;
		}
	}
	else {//this is the alldensity histogram
		valu=0.0;
		CStatsWindow::stDrawCol2->AddItem(&valu);
		for (j=0; j<=(maxbins-1); j++){
			valu=brec.bins[j];
			CStatsWindow::stDrawCol2->AddItem(&valu);
			CStatsWindow::stDrawCol2->AddItem(&valu);
		}
		valu=0.0;
		CStatsWindow::stDrawCol2->AddItem(&valu);
	}
	count=CStatsWindow::stDrawCol2->GetCount();
	val=-totview;
	//make the x-axis data for the psth
	if (!CStatsWindow::HistORCumPSTHFlag){
		for (i=1; i<=count; i++){
			CStatsWindow::stDrawCol1->AddItem(&val);
			val+=CSpike6App::SpikeParams.BWdth/1000.00;
		}
	}
	else{//it is the psth, make it a real histogram
		histmax=round(count/2.0);
		for (i=1; i<=histmax; i++){
			CStatsWindow::stDrawCol1->AddItem(&val);
			CStatsWindow::stDrawCol1->AddItem(&val);
			val+=CSpike6App::SpikeParams.BWdth/1000.00;
		}
	}
}
// arrays PTHelp (spiketimes); PTintervals (spike intervals); help (stimulus times) are ready and the histogram is made
//-------------------------------------------------------------------------------------------------------------------------
//		shufflespiketrain()
//-------------------------------------------------------------------------------------------------------------------------
	

void shufflespiketrain(){//shuffle the intervals, PTHelp out(shuffled)
		//produce intervals
	SInt32 i, k, count=CStatsWindow::PTintervals->GetCount();
	Float64 val, timeval;
	//CStatsWindow::PThelp->FetchItemAt(1, &starttime);
	//CStatsWindow::PThelp->FetchItemAt(count, &stoptime);
	delete CStatsWindow::PThelp;
	CStatsWindow::PThelp=nil;
	CStatsWindow::PThelp = new LArray(sizeof(Float64));
	const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	UInt32 myseed=TickCount();
	gsl_rng_set(r,myseed);
	gsl_permutation * p = gsl_permutation_alloc (count);
	gsl_permutation_init (p);
	gsl_ran_shuffle (r, p->data, count, sizeof(size_t));
	timeval = CStatsWindow::PTmintime;
	CStatsWindow::PThelp->AddItem(&timeval, sizeof(Float64));
	for (i=0; i < count;i++){
		k = gsl_permutation_get (p, i);//p should be a list of addresses in PThelp to read
		CStatsWindow::PTintervals->FetchItemAt(k, &val);//fetch interval
		timeval += val;
		CStatsWindow::PThelp->AddItem(&timeval, sizeof(Float64));		
	}
	gsl_permutation_free (p);
}

//-------------------------------------------------------------------------------------------------------------------------
//		DoCumPSTHnew()
//-------------------------------------------------------------------------------------------------------------------------


void DoCumPSTHnew(){//shuffle the spike intervals (instead of the stimulus occurrences)
	CSpike6App::BinRecordLong brec;
	SInt32 i, j, k, binnum, count=CStatsWindow::PThelp->GetCount(), lastline=1, stimcount=CStatsWindow::PTstims->GetCount();
	Float64 tim, val, valu, intv, bwdt=CSpike6App::SpikeParams.BWdth/1000.0;
	SInt32 maxbins=2*CSpike6App::SpikeParams.maxbns;
	for (i=0;i<=maxbins-1;i++){
		brec.bins[i]=0.0;
	}
//first shuffle PTHelp i.e. the spikes of the selected cluster
	
	shufflespiketrain();//shuffles the spikes by shuffling the intervals
	for(i=1; i<=stimcount; i++){
		//val = newhulp[i];
		CStatsWindow::PTstims->FetchItemAt(i, &val);//get time of stimulus
		for (j=lastline; j<=count; j++){
			CStatsWindow::PThelp->FetchItemAt(j,&valu);
			if (valu>=val){
				//make the histogram
				for (k=j-1; k >= 1; k--){
					CStatsWindow::PThelp->FetchItemAt(k, &tim);
					intv=val-tim;
					binnum = CSpike6App::SpikeParams.maxbns-1 - trunc(intv / bwdt);
					if (binnum < 0){
						lastline=k;
						break;
					}
					brec.bins[binnum]+=1;
				}
				for (k=j; k <= count; k++){
					CStatsWindow::PThelp->FetchItemAt(k, &tim);
					intv=tim-val;
					binnum = CSpike6App::SpikeParams.maxbns + trunc(intv / bwdt);
					if (binnum > maxbins-1){
						break;
					}
					brec.bins[binnum]+=1;
				}
				break;
			}
		}
	}
	CSpike6App::bRecord->AddItem(&brec);
}
//-------------------------------------------------------------------------------------------------------------------------
//		FinishCumPSTH()
//-------------------------------------------------------------------------------------------------------------------------
	
	void FinishCumPSTH()//finishes both histogram and cumpsth
		{
		UCursor::SetWatch();
		//make the brecs into cumulative histograms
		CSpike6App::BinRecordLong brec, binrec;
		SInt32 i, j, count=CSpike6App::bRecord->GetCount(), locat;
			for (i=0; i < 1000; i++){
				binrec.bins[i]=0.0;
			}
			
		Float64 val, valu, lastval, tot=0.0, totval, bigtotval;
		SInt32 maxbins=2*CSpike6App::SpikeParams.maxbns;
		SInt32 halfbins=maxbins/2; 
		if (!CStatsWindow::HistORCumPSTHFlag){ //this needs to be done only if cumpsth is done, for histogram, it is skipped
			for (i=1; i<=count; i++){//for the number of brecords i.e. the number of runs
				CSpike6App::bRecord->FetchItemAt(i,&brec);
				val=0.0;
				valu=0.0;
				lastval=0.0;
				for (j=0; j<halfbins; j++){
					val+=brec.bins[j];
					}
				val=val/((Float64)halfbins);
			for (j=0; j<maxbins; j++){
					valu=brec.bins[j]-val + lastval;
					brec.bins[j]=valu;
					lastval=valu;
					}
				CSpike6App::bRecord->AssignItemsAt(1,i,&brec);
				}
			}
		
		//convert the bRec data into 
		count=CSpike6App::bRecord->GetCount();
		size_t arsize = count;
		void * np = malloc (arsize * sizeof(Float64));
		Float64 *hlp = (Float64 *)np;
		for (j=0; j<count; j++){
			hlp[j]=0.0;
			}
		if (!CStatsWindow::HistORCumPSTHFlag){//if the cumpsth is done, the sorting, etc is only done after the halfbins
			val=0.0;
			for (j=1; j<=halfbins; j++){
				CStatsWindow::stDrawCol3->AddItem(&val);
				CStatsWindow::stDrawCol4->AddItem(&val);
				}
			
			//for each of the columns in bRecord (bins[0]-bins[maxbins-1], sort the data low to high, determine the 5% level and the 95%level. 
			//Put these levels in stDrawCol3 and -4
			for (i=halfbins+1; i<maxbins; i++){	
				for (j=1; j<=count; j++){
					CSpike6App::bRecord->FetchItemAt(j,&brec);
					val=brec.bins[i] + 10000.0;// 10000 to keep all the values positive. note, this may be too small (10000)
					hlp[j-1]=val;
					binrec.bins[j]+=brec.bins[j];
					}
				sortFloat64(hlp,arsize);
				locat=round(CStatsWindow::PValue*(Float64)count);
				val=hlp[locat-1];
				val-=10000.0;
				CStatsWindow::stDrawCol3->AddItem(&val);//lower confidence limit
				locat=round((1.0-CStatsWindow::PValue)*(Float64)count);
				val=hlp[locat-1];
				val-=10000.0;
				CStatsWindow::stDrawCol4->AddItem(&val);//upper confidence limit
				}
			}
		else {//if the psth is done, all the bins are looked at, also we want to make it into a real histogram
			//start with a zero in the first bin
			val=0.0;
			CStatsWindow::stDrawCol3->AddItem(&val);
			CStatsWindow::stDrawCol4->AddItem(&val);
			bigtotval=0.0;
			for (i=0; i<=maxbins-1; i++){
				tot=0;
				for (j=1; j<=count; j++){
					CSpike6App::bRecord->FetchItemAt(j,&brec);
					val=brec.bins[i] + 10000.0;// 10000 to keep all the values positive. note, this may be too small (10000)
					hlp[j-1]=val;
					tot+=val;
					binrec.bins[j]=brec.bins[i];
					}
				totval=0.0;
				for (j=1; j<=count; j++){
					totval+=binrec.bins[j];
				}
				bigtotval+=totval;
				sortFloat64(hlp,arsize);
				locat=round(CStatsWindow::PValue*(Float64)count);
				val=hlp[locat-1];
				val-=10000.0;
				CStatsWindow::stDrawCol3->AddItem(&val);//lower confidence limit
				CStatsWindow::stDrawCol3->AddItem(&val);//lower confidence limit
				locat=round((1.0-CStatsWindow::PValue)*(Float64)count);
				val=hlp[locat-1];
				val-=10000.0;
				CStatsWindow::stDrawCol4->AddItem(&val);//upper confidence limit
				CStatsWindow::stDrawCol4->AddItem(&val);//upper confidence limit
				}
			bigtotval=bigtotval/1000;
			bigtotval=bigtotval/400;
			val=0;
			CStatsWindow::stDrawCol3->AddItem(&val);
			CStatsWindow::stDrawCol4->AddItem(&val);
			}
		delete CStatsWindow::PThelp;
		CStatsWindow::PThelp=nil;
		}






Boolean FindLinesB (SInt32 *SError)
	{
	SInt32 i=0;
	//{find first line after strt AND first line after break (if a break exists) 
	//where templ1 fired
	SInt32 minlin=1000001; //to intercept cases where target template does not occur
	SInt32 maxlin=1000001;
	SInt32 brklin=1000001;
	SInt32 brklin2=1000001;
	Boolean foundmin=FALSE;
	Boolean foundmax=FALSE;
	Boolean foundbrk=FALSE;
	Boolean foundbrk2=FALSE;

	Boolean templtimeok=TRUE;
	Float64 val,start,stop;
	SInt32 templ, val1, val2, occurs1, occurs2,count;
	
		if (CSpike6App::SpikeParams.templ1 > -1){
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1+1, & occurs1);
		if (occurs1 == 0){
			*SError = 108;
			templtimeok=FALSE;
			}
		}
	if (CSpike6App::SpikeParams.templ2 > -1){
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ2+1, & occurs2);
		if (occurs1 == 0){
			*SError = 108;
			templtimeok=FALSE;
			}
		}

	CSpike6App::templrecrd theTRec;
	//check whether Templates are entered correctly
	if (CSpike6App::SpikeParams.templ1 > CSpike6App::snTemplates)
		{
		CSpike6App::SpikeParams.templ1 = 0;
		*SError=101;
		templtimeok=FALSE;
		}
	else if (CSpike6App::SpikeParams.templ1 < 0)
		{
		if	(!(CSpike6App::SpikeParams.templ2 == -1))
			{
			CSpike6App::SpikeParams.templ1 = CSpike6App::SpikeParams.templ2;
			CSpike6App::SpikeParams.templ2 = -1;
			*SError=102;

			templtimeok=FALSE;
			}
		else {
			CSpike6App::SpikeParams.templ1=0;
			*SError=103;
			templtimeok=FALSE;
			}
		}
	else {
		//{get highest time value}
		count=CSpike6App::tRecord->GetCount();
		CSpike6App::tRecord->FetchItemAt(count, &theTRec);
		val=theTRec.timestamp;
		stop=CSpike6App::SpikeParams.stp;
		val1=round(10*stop);
		val2=round(10*val);
		if (val2 < val1)
			{ 
			CSpike6App::SpikeParams.stp=val;
			*SError=104;
			templtimeok=FALSE;
			}
		i=1;
		CSpike6App::tRecord->FetchItemAt(i, &theTRec);
		val=theTRec.timestamp;
		start=CSpike6App::SpikeParams.strt;
		val1=round(10*start);
		val2=round(10*val);
		if (val2 > val1)
			{
			CSpike6App::SpikeParams.strt=val;
			*SError=105;
			templtimeok=FALSE;
			}
		}
	if (templtimeok)
		{
		//find first line after strt
		i=0;
		val=-1;
		while (round(10*val)<round(10*start))
			{
			i+=1;
			CSpike6App::tRecord->FetchItemAt(i, &theTRec);
			val=theTRec.timestamp;
			}
			//now i is the first line with value at or just after the given startpoint
		i=i-1;
		while ((foundmin==FALSE)&&(val < stop))
			{
			i+=1;
			CSpike6App::tRecord->FetchItemAt(i, &theTRec);
			templ=theTRec.templatenum; //is it the target spike?
			if ((templ==CSpike6App::SpikeParams.templ1)||(templ==CSpike6App::SpikeParams.templ2))
				{
				val=theTRec.timestamp;
				if (foundmin==FALSE)
					{
					minlin=i;
					foundmin=TRUE;
					}
				}
			}
		if(val >= stop) {foundmin=FALSE;}
		//look for line with last firing of templ1
		i=CSpike6App::sdim3 + 1;
		val=10000000.00;//should be larger than largest time value in the file
		while (val>stop)
			{
			i=i-1;
			CSpike6App::tRecord->FetchItemAt(i, &theTRec);
			val=theTRec.timestamp;
			}
		i=i+1;
		while ((foundmax==FALSE)&&(i > minlin))
			{
			i-=1;
			CSpike6App::tRecord->FetchItemAt(i, &theTRec);
			templ=theTRec.templatenum; //is it the target spike?
			if ((templ==CSpike6App::SpikeParams.templ1)||(templ==CSpike6App::SpikeParams.templ2))
				{
				foundmax=TRUE;
				maxlin=i;
				}
			}
		//find breakpoint, if one is specified (brkpt != 0.0)
		Float64 breakpt=CSpike6App::SpikeParams.brkpt;
		if (breakpt != 0.0)
			{
			if ((breakpt > stop)||(breakpt < start))
				{
				CSpike6App::SpikeParams.brkpt=0.0;
				*SError=106;
				return(FALSE);
				}
			CSpike6App::tRecord->FetchItemAt(minlin,&theTRec);
			val=theTRec.timestamp;
			if (val >= breakpt) {foundmin=FALSE;}
			CSpike6App::tRecord->FetchItemAt(maxlin,&theTRec);
			val=theTRec.timestamp;
			if (val <= breakpt) {foundmax=FALSE;}
		//look for line with first firing of templ1 after breakpoint
			i=minlin;
			while ((foundbrk == FALSE) && (i < maxlin))
				{
				i+=1;
				CSpike6App::tRecord->FetchItemAt(i,&theTRec);
				templ=theTRec.templatenum; //is it the target spike?
				if (templ==CSpike6App::SpikeParams.templ1)
					{
					val=theTRec.timestamp;
					if (val>breakpt)
						{
						brklin=i;
						foundbrk=TRUE;
						}
					}
				}
			}
		else 
			{
			foundbrk=TRUE;
			brklin=0;
			}//because there is no breakpoint, do not influence outcome of the following
		Float64 breakpt2=CSpike6App::SpikeParams.brkpt2;
		if (breakpt2 != 0.0)
			{
			if ((breakpt2 > stop)||(breakpt2 < start))
				{
				CSpike6App::SpikeParams.brkpt2=0.0;
				*SError=106;
				return(FALSE);
			}
			CSpike6App::tRecord->FetchItemAt(minlin,&theTRec);
			val=theTRec.timestamp;
			if (val >= breakpt2) {foundmin=FALSE;}
			CSpike6App::tRecord->FetchItemAt(maxlin,&theTRec);
			val=theTRec.timestamp;
			if (val <= breakpt2) {foundmax=FALSE;}
			//look for line with first firing of templ1 after breakpoint2
			i=minlin;//is not very efficient, but we can spare the time
			while ((foundbrk2 == FALSE) && (i < maxlin))
				{
				i+=1;
				CSpike6App::tRecord->FetchItemAt(i,&theTRec);
				templ=theTRec.templatenum; //is it the target spike?
				if (templ==CSpike6App::SpikeParams.templ1)
					{
					val=theTRec.timestamp;
					if (val>breakpt2)
						{
						brklin2=i;
						foundbrk2=TRUE;
						}
					}
				}
			}
		else 
			{
				foundbrk2=TRUE;
				brklin2=0;
			}//because there is no breakpoint, do not influence outcome of the following
			
		//end of finding first, break, and last line
		if ((foundmin==TRUE)&&(foundmax==TRUE)&&(foundbrk==TRUE)&&(foundbrk2==TRUE))
			{
			CSpike6App::SpikeParams.minlin=minlin;
			CSpike6App::SpikeParams.maxlin=maxlin;
			CSpike6App::SpikeParams.brklin=brklin;
			CSpike6App::SpikeParams.brklin2=brklin2;
			return(TRUE);
			}
		else
			{
			*SError=107;
			return(FALSE);
			}
		} 
	else {return(FALSE);}
	}
	
void sortFloat64(double * indat, size_t arsize){
	gsl_sort (indat, 1, arsize);
	}

UInt32 GetIndex (Float64 *inarray, Float64 val, size_t arsize){//gets the index of a value in the array
	UInt32 j;
	Float64 valu;
	for (j=0; j<arsize; j++){
		valu=inarray[j];
		if ((fabs(valu-val))<0.00000001){break;}
		}
	return j;
	}
Float64 GetRank (Float64 *inarray, Float64 val, size_t arsize){//gets the index of a value in the array for rank-ordering
	Float64 rank;
	SInt32 j;
	Float64 valu;
	for (j=0; j<arsize; j++){
		valu=inarray[j];
		if ((fabs(valu-val))<0.00000001){break;}
	}
	rank=(Float64)j;
	for(j=arsize-1;j>=0;j--){
		valu=inarray[j];
		if ((fabs(valu-val))<0.00000001){break;}
	}
	rank=(rank + (Float64)j)/2.0;
	return rank;
}



void  MakeGenStats()
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
};
