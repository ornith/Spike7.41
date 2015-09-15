#include "MonteCarlo.h"
//#include "CSpikeApp.h"
//#include "URandomLib.h"
#include "CMonteWindow.h"
#include <time.h>
#include <math.h>

#define EPS1 0.001
#define EPS2 1.0e-8

LArray *IntvArr1 = nil;
LArray *IntvArr2 = nil;
LArray *OrigStor = nil;
Float64 avg, median=0, centerval;
Float64 ltm1,ltm2, gtm1,gtm2;
/*void InitRand ()
	{
	Ultra_seed1 = clock(); Ultra_seed2 = 87654321;
	Ultra_Init();
	};*/
	
void RunSim ()
	{
	Float64 tim, intval, avrg1, avrg2, sigm1,sigm2,val, valu;//fact is proportion avg(2)/avg(1) or N1/N2
	SInt32 i, n1, n2;
	avrg1=2.0;
	//avrg2=avrg1;
	sigm1=0.25;
	sigm2=sigm1;
	avrg2=avrg1;//+2*avrg1;
	n1=avrg2/(avrg1+avrg2)*CMonteWindow::nIntervals;
	n2=CMonteWindow::nIntervals-n1;
	tim=0;
	intval=0;
	gsl_rng *r;
	const gsl_rng_type *T;//type of generator to use
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	UInt32 myseed=TickCount();
	gsl_rng_set(r,myseed);

	for (i=1; i<=n1; i++)
		{
		if (CMonteWindow::unifs) 
			//{intval=(Float64)PRNG.Uniform_0_1();}
			{intval = gsl_rng_uniform(r);}
		else if (CMonteWindow::norms) 
			//{intval=(Float64)PRNG.Normal(avrg1,sigm1);}//Ultra_norm(avrg1,sigm1);}
			{intval = gsl_ran_gaussian(r, sigm1) + avrg1;}
		else if (CMonteWindow::logns) 
			{
			//intval=(Float64)PRNG.Normal(avrg1,sigm1);
			intval = gsl_ran_gaussian(r, sigm1) + avrg1;
			intval=log(intval+1);
			}
		else if (CMonteWindow::expos) 
			//{intval=(Float64)PRNG.Expo(avrg1);}
			{intval=gsl_ran_exponential(r, avrg1);}
		CMonteWindow::simCol1->AddItem(&tim);//timestamp
		CMonteWindow::simCol2->AddItem(&intval); //interval
		tim += intval;
		}
	for (i=n1+1; i<=CMonteWindow::nIntervals; i++)
		{
		if (CMonteWindow::unifs) 
			//{intval=(Float64)PRNG.Uniform_0_1();}
			{intval = gsl_rng_uniform(r);}

		else if (CMonteWindow::norms) 
			//{intval=(Float64)PRNG.Normal(avrg2,sigm2);}
			{intval = gsl_ran_gaussian(r, sigm2) + avrg2;}

		else if (CMonteWindow::logns) 
			{
			//intval=(Float64)PRNG.Normal(avrg2,sigm2);
			intval = gsl_ran_gaussian(r, sigm2) + avrg2;
			intval=log(intval+1);
			}
		else if (CMonteWindow::expos) 
			//{intval=(Float64)PRNG.Expo(avrg2);}
			{intval=gsl_ran_exponential(r, avrg2);}
		CMonteWindow::simCol1->AddItem(&tim);//timestamp//timestamp
		CMonteWindow::simCol2->AddItem(&intval); //interval
		tim += intval;
		}
	//find middle interval
	CMonteWindow::simCol1->FetchItemAt(1,&val);
	CMonteWindow::simCol1->FetchItemAt(CMonteWindow::nIntervals,&valu);
	centerval = (val+valu)/2;
	}
	
	
	
void RunSimAR(){
	Float64 tim, intval, theIntval,theAvg1=2.0, theAvg2, n1, n2, avrg, sigm;//fact is proportion avg(2)/avg(1) or N1/N2
	SInt32 i;
	SInt32 Templ=1;
	theAvg2=theAvg1+0.5;
	n1=round(n1=theAvg2/(theAvg1+theAvg2)*CMonteWindow::nIntervals);
	n2=CMonteWindow::nIntervals-n1;
	tim=0;
	intval=0;
		const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	avrg=0;
	sigm=0.25;
	//Float64  oudintval=PRNG.Normal(0,sigm);
	Float64  oudintval=gsl_ran_gaussian(r, sigm);;
	Float64 coeff=0.5, dev=0.0;
	avrg=0;
	sigm=0.25;
	for (i=1; i<=n1; i++)
		{
		//dev=PRNG.Normal(avrg,sigm);
		dev = gsl_ran_gaussian(r, sigm) + avrg;
		intval= coeff*oudintval + dev;
		oudintval=intval;
		theIntval=intval+theAvg1;
		CMonteWindow::simCol1->AddItem(&tim);//timestamp
		CMonteWindow::simCol2->AddItem(&theIntval); //Template number
		tim += theIntval;
		
		}
	for (i=n1+1; i<=CMonteWindow::nIntervals; i++){
		//dev=PRNG.Normal(avrg,sigm);
		dev = gsl_ran_gaussian(r, sigm) + avrg;
		intval=coeff*oudintval + dev;
		oudintval=intval;
		theIntval=intval+theAvg2;
		CMonteWindow::simCol1->AddItem(&tim);//timestamp
		CMonteWindow::simCol2->AddItem(&theIntval); //Template number
		tim += theIntval;
		}
	CMonteWindow::simCol1->AddItem(&tim);
	CMonteWindow::simCol2->AddItem(&Templ);
	//find middle interval
	Float64 val, valu;
	CMonteWindow::simCol1->FetchItemAt(1,&val);
	CMonteWindow::simCol1->FetchItemAt(CMonteWindow::nIntervals,&valu);
	centerval = (val+valu)/2;

}

void RunSimMA(){
	Float64 tim, intval, oldintval, n1, n2, avrg, sigm;//fact is proportion avg(2)/avg(1) or N1/N2
	SInt32 i;
	n1=round(CMonteWindow::nIntervals/2);
	n2=CMonteWindow::nIntervals-n1;
	tim=0;
	intval=0;
	oldintval=1;
	const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);

	/*	Critical!  We MUST supply two ulong seeds > 0.  There is, intentionally, no
	default.  Ultra_Init() MUST be the first library function called. */
	avrg=0;
	sigm=0.25;
	Float64 theAvg=2.0, olddev=gsl_ran_gaussian(r, sigm)+avrg, coeff=-0.95, dev=0.0;

	for (i=1; i<=n1; i++)
		{
		//dev=PRNG.Normal(avrg,sigm);
		dev = gsl_ran_gaussian(r, sigm) + avrg;
		intval=theAvg - olddev*coeff + dev;
		olddev=dev;
		CMonteWindow::simCol1->AddItem(&tim);//timestamp
		CMonteWindow::simCol2->AddItem(&intval); //Template number
		tim += intval;
		}
	for (i=n1+1; i<=CMonteWindow::nIntervals; i++){
		//dev=PRNG.Normal(avrg,sigm);
		dev = gsl_ran_gaussian(r, sigm) + avrg;
		intval=theAvg - olddev*coeff + dev;
		olddev=dev;
		CMonteWindow::simCol1->AddItem(&tim);//timestamp
		CMonteWindow::simCol2->AddItem(&intval); //Template number
		tim += intval;
		}	
		//find middle interval
	Float64 val, valu;
	CMonteWindow::simCol1->FetchItemAt(1,&val);
	CMonteWindow::simCol1->FetchItemAt(CMonteWindow::nIntervals,&valu);
	centerval = (val+valu)/2;
}


void FakeSim()//produce spike train with one single interval length
	{
	SInt32 i, nn;
	Float64 val;
	nn=100;
	for (i=1; i<=nn; i++)
		{
		val=i;
		CMonteWindow::simCol2->AssignItemsAt(1,i,&val); //interval
		}
	CMonteWindow::nIntervals=nn;
	}
	
void MakeAverages ()
	{
	SInt32 i=1;
	Float64 sum=0, qsum=0.0, val;
	for (i=1; i<=CMonteWindow::sN1; i++)
		{
		CMonteWindow::simCol2->FetchItemAt(i,&val);
		sum=sum+val;
		qsum=qsum+val*val;
		}
	CMonteWindow::Stdev1=sqrt((qsum-sum*sum/(i-1))/(i-2));
	CMonteWindow::Average1=sum/(i-1);
	sum=0.0;
	qsum=0.0;
	Float64 nn=0.0;
	for (i=CMonteWindow::sN1+2; i<=CMonteWindow::nIntervals; i++)
		{
		CMonteWindow::simCol2->FetchItemAt(i,&val);
		sum=sum+val;
		qsum=qsum+val*val;
		nn+=1;
		}
	CMonteWindow::Stdev2=sqrt((qsum-sum*sum/(CMonteWindow::sN2))/(CMonteWindow::sN2-1));
	CMonteWindow::Average2=sum/(CMonteWindow::sN2);
	}
		
void AutoCorr ()
	{
	Float64 val0,sval0, sum0=0.0,ssum0=0.0;
	Float64 val, valu, xval;
	SInt32 i,j,k, max, ord;
	
	ord=CMonteWindow::order;
	LArray *CrossProducts;
	LArray *sums;
	LArray *ssums;
	//FakeSim();
	CrossProducts=new LArray(sizeof(Float64));
	sums=new LArray(sizeof(Float64));
	ssums=new LArray(sizeof(Float64));
	
	//fill arrays with zeroes
	val=0.0;
	for (i=1; i<=ord;i++)
		{
		CrossProducts->AddItem(&val, sizeof(Float64));
		sums->AddItem(&val, sizeof(Float64));
		ssums->AddItem(&val, sizeof(Float64));
		}
		
	//in 1: order; in n: order-(n-1)
	
	for (i = ord+1; i<= CMonteWindow::nIntervals; i++)
		{
		CMonteWindow::simCol2->FetchItemAt(i,&val0);
		sum0 += val0; //sum of observations
		sval0 = val0*val0;
		ssum0 += sval0; //sum of squared observations
		for (j=1; j<=ord;j++)
			{
			CrossProducts->FetchItemAt(j, &xval);
			CMonteWindow::simCol2->FetchItemAt(i-j,&val);
			xval=xval+val*val0;
			CrossProducts->AssignItemsAt(1, j, &xval, sizeof(Float64));
			}
		}
	
	max = CMonteWindow::nIntervals;
	
	//adjust sums for order columns
	
	for (j=1; j<=ord; j++)
		{
		for (k=j; k<=ord; k++)
			{
			CMonteWindow::simCol2->FetchItemAt(max-j+1, &val);
			sums->FetchItemAt(k, &valu);
			valu=valu-val;
			sums->AssignItemsAt(1,k,&valu);
			ssums->FetchItemAt(k, &valu);
			valu=valu-val*val;
			ssums->AssignItemsAt(1,k,&valu);
			CMonteWindow::simCol2->FetchItemAt(ord+1-j, &val);
			sums->FetchItemAt(k, &valu);
			valu=valu+val;
			sums->AssignItemsAt(1,k,&valu);
			ssums->FetchItemAt(k, &valu);
			valu=valu+val*val;
			ssums->AssignItemsAt(1,k,&valu);
			}
		}
	for (k=1; k<=ord; k++)
		{
		sums->FetchItemAt(k, &valu);
		valu = sum0 + valu;
		sums->AssignItemsAt(1,k,&valu);
		ssums->FetchItemAt(k, &valu);
		valu = ssum0 + valu;
		ssums->AssignItemsAt(1,k,&valu);
		}
	
	//make correlation coefficients
	
	Float64 sx, sy, sxy, rxy, NN, sum, ssum, xsum;
	NN=max-ord;
	sx= ssum0-sum0*sum0/NN;
	for (i=1; i<=ord; i++)
		{
		sums->FetchItemAt(i, &sum);
		ssums->FetchItemAt(i, &ssum);
		sy=ssum-sum*sum/NN;
		CrossProducts->FetchItemAt(i,&xsum);
		sxy=xsum-sum0*sum/NN;
		rxy=sxy/sqrt(sx*sy);
		CMonteWindow::RXY2->FetchItemAt(i+3,&val);
		val=((CMonteWindow::cRuns)*val+rxy)/(CMonteWindow::cRuns+1);
		CMonteWindow::RXY2->AssignItemsAt(1,i+3,&val, sizeof(Float64));
		}
	delete (sums);
	sums=nil;
	delete (ssums);
	ssums=nil;
	delete (CrossProducts);
	CrossProducts=nil;
	}
	
	
	
void RankIntvals()// makes two arrays with sorted intervals
	{
	SInt32 i,j,n1, n2;
	Float64 val, valu;//current value in array, last low value used, lowest in last run
	//make interval arrays for each group
	
	
	//fill first intervalarray
	CMonteWindow::simCol1->FetchItemAt(1,&val);
	avg=0;
	i=1;
	while (val <= centerval)
		{
		CMonteWindow::simCol2->FetchItemAt(i,&valu);
		IntvArr1->InsertItemsAt(1,i, &valu, sizeof(Float64));
		avg+=valu;
		i+=1;
		CMonteWindow::simCol1->FetchItemAt(i,&val);
		}
	n1 = i-1; 
	n2 = CMonteWindow::nIntervals -i;
	//fill second intervalarray
	for (j=1; j<=n2; j++)
		{
		CMonteWindow::simCol2->FetchItemAt(j+i,&valu);
		IntvArr2->InsertItemsAt(1,j, &valu, sizeof(Float64));
		avg+=valu;
		}
	avg=avg/(n1+n2);
	CMonteWindow::sN1=n1;
	CMonteWindow::sN2=n2;

	}
	
void MakeKWStat() // divides intervalseries in two equal parts and determines number of intervals 
// in each part & rank number in each
// also finds median (avg of interval 50 and 51)
	{
	SInt32 i=1;
	SInt32 j=1;
	SInt32 k;
	SInt32 n1,n2,half;
	Float64 val,valu,tStat1;
	Float64 RankS1=0;
	Float64 RankS2=0;
	Float64 tStat=0;
	//RankIntvals();
	val=(CMonteWindow::nIntervals - 1)/2;
	half=round(val);
	n1=CMonteWindow::sN1;
	n2=CMonteWindow::sN2;
	k=1;
	while ((i<=n1) && (j<=n2))
		{
		IntvArr1->FetchItemAt(i,&val);
		IntvArr2->FetchItemAt(j,&valu);
		//get the median
		if(val<=valu)
			{
			if(k==half)
				{
				median=val;
				}
			if(k==(half+1))
				{
				median=(median+val)/2;
				}
			RankS1+=k;
			k+=1;
			i+=1;
			}
		else if (val>valu)
			{
			if(k==half)
				{
				median=valu;
				}
			if(k==(half+1))
				{
				median=(median+valu)/2;
				}
			RankS2+=k;
			k+=1;
			j+=1;
			}
		}
	if (i>n1)
		{
		while (j<=n2)
			{
			if(k==half)
				{
				median=valu;
				}
			if(k==(half+1))
				{
				median=(median+valu)/2;
				}
			RankS2+=k;
			k+=1;
			j+=1;
			}
		}
	else if (j>n2)
		{
		while (i<=n1)
			{
			if(k==half)
				{
				median=valu;
				}
			if(k==(half+1))
				{
				median=(median+valu)/2;
				}
			RankS1+=k;
			k+=1;
			i+=1;
			}
		}

	if(RankS1==RankS2)
		{
		CMonteWindow::KWStat=0;
		}
	else
		{
		tStat=RankS1/n1;
		tStat=tStat*RankS1;
		tStat1=RankS2/n2;
		tStat1=tStat1*RankS2;
		tStat=tStat+tStat1;
		tStat=tStat/((n1+n2)*(n1+n2+1));
		tStat=12*tStat;
		tStat=tStat-3*(n1+n2+1);
		CMonteWindow::KWStat=tStat;
		}
	}
void DoPermutation()
/*
does permutations and counts how many times the given outcome or worse occurs.
it uses simCol2 without the interval containing the central value because 
you would not use it normally and it is not used in all other tests on
//
intervals.
*/
	{
	SInt32 i, j, locat, Intvals, occurs=0, rounds=100;
	Float64 totval, val;
	SInt32 used=-100;
	const gsl_rng_type *T;//type of generator to use
	gsl_rng *r;
	gsl_rng_env_setup();
	T=gsl_rng_default;
	r= gsl_rng_alloc (T);
	i=0;
	for (i=1;i<=rounds; i++)
		{
		used=-100.0;
		totval	= 0;
		Intvals	= 0;
		LArray *usd;
		usd = new LArray(sizeof(SInt32));
		ThrowIfNil_(usd);
		for (j=1; j<=CMonteWindow::nIntervals; j++)
			{
			usd->AddItem(&used, sizeof(SInt32));
			}
		while (totval<=centerval)
			{
			//locat = round(fabs(CMonteWindow::nIntervals*PRNG.Uniform_0_1()))+1;
			locat = round(fabs(CMonteWindow::nIntervals*gsl_rng_uniform(r)))+1;
			if ((locat <= CMonteWindow::nIntervals) && !(locat == (CMonteWindow::sN1 + 1)))
				{
				usd->FetchItemAt(locat, &used);
				if (used < 0) //if the value has not already been used
					{
					CMonteWindow::simCol2->FetchItemAt(locat, &val);
					totval+=val;
					Intvals+=1;
					used=100;
					usd->AssignItemsAt(1,locat,&used);
					}
				}
			}
		if (CMonteWindow::sN1 <= CMonteWindow::sN2)
			{
			if ((Intvals <= CMonteWindow::sN1) || (Intvals >= CMonteWindow::sN2))
				{
				occurs=occurs+1;
				}
			}
		else 
			{
			if ((Intvals >= CMonteWindow::sN1) || (Intvals <= CMonteWindow::sN2))
				{
				occurs += 1;
				}
			}
		delete(usd);
		usd=nil;
		}
	CMonteWindow::PermProb=(Float64)occurs/(Float64)rounds;
	}
	
	
void DoMedian() //needs MakeKWStat to be run first (for making median)
	{
	SInt32 i;
	Float64 val=0, mStat;
	Float64 expgtm1,expgtm2,expltm1,expltm2, NN;
	i=0;
	while ((val<=median)&&(i<=CMonteWindow::sN1))
		{
		i+=1;
		IntvArr1->FetchItemAt(i, &val);
		}
	ltm1=i-1;
	gtm1=CMonteWindow::sN1-ltm1;
	i=0;
	val=0;
	while ((val<=median)&&(i<=CMonteWindow::sN2))
		{
		i+=1;
		IntvArr2->FetchItemAt(i, &val);
		}
	ltm2=i-1;
	gtm2=CMonteWindow::sN2-ltm2;
	//actual test has to be done now
	
	NN=ltm1+ltm2+gtm1+gtm2;
	expltm1=(ltm1+ltm2)*(ltm1+gtm1)/NN;
	expltm2=(ltm1+ltm2)*(ltm2+gtm2)/NN;
	expgtm1=(gtm1+gtm2)*(ltm1+gtm1)/NN;
	expgtm2=(gtm1+gtm2)*(ltm2+gtm2)/NN;
	mStat=(ltm1-expltm1)*(ltm1-expltm1)/expltm1;
	mStat+=(ltm2-expltm2)*(ltm2-expltm2)/expltm2;
	mStat+=(gtm1-expgtm1)*(gtm1-expgtm1)/expgtm1;
	mStat+=(gtm2-expgtm2)*(gtm2-expgtm2)/expgtm2;
	CMonteWindow::MedianStat=mStat;
	}
	
Float64 probksm(Float64 alam)
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
	
void kstwo()
	{
	SInt32 j1=1,j2=1;
	Float64 dt,en1,en2,en,fn1=0.0,fn2=0.0;
	Float64 prob, val1, val2, KSStat;
	
	en1=CMonteWindow::sN1;
	en2=CMonteWindow::sN2;
	KSStat=0.0;
	while (j1 <= en1 && j2 <= en2)
		{
		IntvArr1->FetchItemAt(j1,&val1);
		IntvArr2->FetchItemAt(j2,&val2);
		if ((val1) <= (val2)) {fn1=j1++/en1;}
		if (val2<=val1) {fn2=j2++/en2;}
		if ((dt=fabs(fn2-fn1)) > KSStat) {KSStat=dt;}
		} 
	en=sqrt(en1*en2/(en1+en2));
	Float64 blam=(en+0.12+0.11/en)*KSStat;
	prob=probksm(blam);
	CMonteWindow::KSProb = prob;
	}
	
	
void DoBreakpoint ()
	{
	SInt32 i,j,m, n1, n2;
	Float64 val, valu;//current value in array, last low value used, lowest in last run
	
	for (i=10; i<=CMonteWindow::nIntervals-10; i++)
		{
		LArray *IntvArr3 = new LArray(sizeof(Float64));
		IntvArr3->SetKeepSorted(TRUE);
		LArray *IntvArr4 = new LArray(sizeof(Float64));
		IntvArr4->SetKeepSorted(TRUE);
		//fill first intervalarray
		for (j=1; j<=i;j++)
			{
			CMonteWindow::simCol2->FetchItemAt(j,&val);
			IntvArr3->InsertItemsAt(1,j, &val, sizeof(Float64));
			}
		n1=i; //this is not really half, note!!!!
		m=1;
		for (j=i+2;j<=CMonteWindow::nIntervals;j++)
			{
			CMonteWindow::simCol2->FetchItemAt(j,&val);
			IntvArr4->InsertItemsAt(1,m, &val, sizeof(Float64));
			m+=1;
			}
		n2=j-2-n1;
		//now we have two sorted arrays
		m=1;
		j=1;
		SInt32 k;
		Float64 tStat1;
		Float64 RankS1=0;
		Float64 RankS2=0;
		Float64 tStat=0;
		k=1;
		while ((m<=n1) && (j<=n2))
			{
			IntvArr3->FetchItemAt(m,&val);
			IntvArr4->FetchItemAt(j,&valu);
			if(val<=valu)
				{
				RankS1+=k;
				k+=1;
				m+=1;
				}
			else if (val>valu)
				{
				RankS2+=k;
				k+=1;
				j+=1;
				}
			}
		if (m>n1)
			{
			while (j<=n2)
				{
				RankS2+=k;
				k+=1;
				j+=1;
				}
			}
		else if (j>n2)
			{
			while (m<=n1)
				{
				RankS1+=k;
				k+=1;
				m+=1;
				}
			}

		if(RankS1==RankS2)
			{
			tStat=0;
			}
		else
			{
			tStat=RankS1/n1;
			tStat=tStat*RankS1;
			tStat1=RankS2/n2;
			tStat1=tStat1*RankS2;
			tStat=tStat+tStat1;
			tStat=tStat/((n1+n2)*(n1+n2+1));
			tStat=12*tStat;
			tStat=tStat-3*(n1+n2+1);
			CMonteWindow::BreakArray2->FetchItemAt(i,&tStat1);
			tStat=tStat1+tStat;
			CMonteWindow::BreakArray2->AssignItemsAt(1,i,&tStat, sizeof(Float64));
			}
		delete(IntvArr3);
		IntvArr3=nil;
		delete(IntvArr4);
		IntvArr4=nil;
		}
	}
	

void DoSimulation ()
	{
	if (CMonteWindow::ARMAFlag1==0){
		RunSim();
		}
	else if (CMonteWindow::ARMAFlag1==1){
		RunSimAR();
		}
	else if (CMonteWindow::ARMAFlag1==2){
		RunSimMA();
		}
	AutoCorr();
	IntvArr1 = new LArray(sizeof(Float64));
	IntvArr1->SetKeepSorted(TRUE);
	IntvArr2 = new LArray(sizeof(Float64));
	IntvArr2->SetKeepSorted(TRUE);
	RankIntvals();
	MakeAverages();
	MakeKWStat();
	DoMedian();
	//DoPermutation();
	//DoBreakpoint ();
	kstwo();
	delete (IntvArr1);
	IntvArr1=nil;
	delete (IntvArr2);
	IntvArr2=nil;
	}	
	
	
	
	
	