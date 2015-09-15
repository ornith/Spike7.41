#include "Analyze1.h"
#include "Analyze2.h"

#include "CSpike6App.h"
#include "CSettingsWindow.h"
#include "CStatsWindow.h"
#include "CMatrixWindow.h"
#include <math.h>
#include <LStaticText.h>
#include "endian.h"
#include "SpikeConstants.h"
#include <LString.h>
#include "gsl/gsl_histogram.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_cdf.h"
LArray *zscore=nil;
LArray *prop=nil;
Float64 older, newer;
//================================================================================
void MakeCumulativeNormal()
	{
	Float64 pp;
	SInt32 zz;
	zscore=new LArray(sizeof(SInt32));
	prop=new LArray(sizeof(Float64));
zz=-30; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0013498980; prop->AddItem(&pp,sizeof(Float64));
zz=-29; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0018658133; prop->AddItem(&pp,sizeof(Float64));
zz=-28; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0025551303; prop->AddItem(&pp,sizeof(Float64));
zz=-27; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0034669738; prop->AddItem(&pp,sizeof(Float64));
zz=-26; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0046611880; prop->AddItem(&pp,sizeof(Float64));
zz=-25; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0062096653; prop->AddItem(&pp,sizeof(Float64));
zz=-24; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0081975359; prop->AddItem(&pp,sizeof(Float64));
zz=-23; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0107241100; prop->AddItem(&pp,sizeof(Float64));
zz=-22; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0139034480; prop->AddItem(&pp,sizeof(Float64));
zz=-21; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0178644210; prop->AddItem(&pp,sizeof(Float64));
zz=-20; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0227501320; prop->AddItem(&pp,sizeof(Float64));
zz=-19; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0287165600; prop->AddItem(&pp,sizeof(Float64));
zz=-18; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0359303190; prop->AddItem(&pp,sizeof(Float64));
zz=-17; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0445654630; prop->AddItem(&pp,sizeof(Float64));
zz=-16; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0547992920; prop->AddItem(&pp,sizeof(Float64));
zz=-15; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0668072010; prop->AddItem(&pp,sizeof(Float64));
zz=-14; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0807566590; prop->AddItem(&pp,sizeof(Float64));
zz=-13; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.0968004850; prop->AddItem(&pp,sizeof(Float64));
zz=-12; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.1150696700; prop->AddItem(&pp,sizeof(Float64));
zz=-11; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.1356660600; prop->AddItem(&pp,sizeof(Float64));
zz=-10; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.1586552500; prop->AddItem(&pp,sizeof(Float64));
zz=-9; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.1840601300; prop->AddItem(&pp,sizeof(Float64));
zz=-8; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.2118554000; prop->AddItem(&pp,sizeof(Float64));
zz=-7; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.2419636500; prop->AddItem(&pp,sizeof(Float64));
zz=-6; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.2742531200; prop->AddItem(&pp,sizeof(Float64));
zz=-5; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.3085375400; prop->AddItem(&pp,sizeof(Float64));
zz=-4; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.3445782600; prop->AddItem(&pp,sizeof(Float64));
zz=-3; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.3820885800; prop->AddItem(&pp,sizeof(Float64));
zz=-2; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.4207402900; prop->AddItem(&pp,sizeof(Float64));
zz=-1; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.4601721600; prop->AddItem(&pp,sizeof(Float64));
zz=0; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.5000000000; prop->AddItem(&pp,sizeof(Float64));
zz=1; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.5398278400; prop->AddItem(&pp,sizeof(Float64));
zz=2; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.5792597100; prop->AddItem(&pp,sizeof(Float64));
zz=3; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.6179114200; prop->AddItem(&pp,sizeof(Float64));
zz=4; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.6554217400; prop->AddItem(&pp,sizeof(Float64));
zz=5; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.6914624600; prop->AddItem(&pp,sizeof(Float64));
zz=6; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.7257468800; prop->AddItem(&pp,sizeof(Float64));
zz=7; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.7580363500; prop->AddItem(&pp,sizeof(Float64));
zz=8; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.7881446000; prop->AddItem(&pp,sizeof(Float64));
zz=9; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.8159398700; prop->AddItem(&pp,sizeof(Float64));
zz=10; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.8413447500; prop->AddItem(&pp,sizeof(Float64));
zz=11; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.8643339400; prop->AddItem(&pp,sizeof(Float64));
zz=12; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.8849303300; prop->AddItem(&pp,sizeof(Float64));
zz=13; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9031995200; prop->AddItem(&pp,sizeof(Float64));
zz=14; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9192433400; prop->AddItem(&pp,sizeof(Float64));
zz=15; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9331928000; prop->AddItem(&pp,sizeof(Float64));
zz=16; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9452007100; prop->AddItem(&pp,sizeof(Float64));
zz=17; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9554345400; prop->AddItem(&pp,sizeof(Float64));
zz=18; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9640696800; prop->AddItem(&pp,sizeof(Float64));
zz=19; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9712834400; prop->AddItem(&pp,sizeof(Float64));
zz=20; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9772498700; prop->AddItem(&pp,sizeof(Float64));
zz=21; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9821355800; prop->AddItem(&pp,sizeof(Float64));
zz=22; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9860965500; prop->AddItem(&pp,sizeof(Float64));
zz=23; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9892758900; prop->AddItem(&pp,sizeof(Float64));
zz=24; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9918024600; prop->AddItem(&pp,sizeof(Float64));
zz=25; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9937903300; prop->AddItem(&pp,sizeof(Float64));
zz=26; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9953388100; prop->AddItem(&pp,sizeof(Float64));
zz=27; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9965330300; prop->AddItem(&pp,sizeof(Float64));
zz=28; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9974448700; prop->AddItem(&pp,sizeof(Float64));
zz=29; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9981341900; prop->AddItem(&pp,sizeof(Float64));
zz=30; zscore->AddItem(&zz, sizeof(SInt32)); pp=0.9986501000; prop->AddItem(&pp,sizeof(Float64));
}
void MakeCumPlot ()
{
Float64 j,valu;
Boolean Rerr;
SInt32 i, templ;
SInt32 templ1=CSpike6App::SpikeParams.templ1;
SInt32 minlin=CSpike6App::SpikeParams.minlin;
SInt32 maxlin=CSpike6App::SpikeParams.maxlin;
Float64 strt=CSpike6App::SpikeParams.strt;
Float64 stp=CSpike6App::SpikeParams.stp;
//find location of requested start
//put the requested start of the plot in first position there are zero occurrences
CSettingsWindow::DrawCol1->AddItem(&strt, sizeof(Float64));
valu=0;
CSettingsWindow::DrawCol2->AddItem(&valu,sizeof(Float64));
//start writing in drawarray at position 2
j=0;
CSpike6App::templrecrd theTRec;
for (i=minlin;i<=maxlin;i++)
	{
	Rerr=CSpike6App::tRecord->FetchItemAt(i,&theTRec);
	templ=theTRec.templatenum;
	if (templ==templ1)
		{
		j+=1.0;
		valu=theTRec.timestamp;
		CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
		CSettingsWindow::DrawCol2->AddItem(&j,sizeof(Float64));
		}
	}
CSettingsWindow::DrawCol1->AddItem(&stp, sizeof(Float64));
//j=j-1;
CSettingsWindow::DrawCol2->AddItem(&j, sizeof(Float64));
CSpike6App::sdrawdim3=(UInt32)(j+1);
}

//================================================================================

void MakeAllTemplatesHisto()
{
SInt32 i,tot,tot1,binnum;
SInt32 firstline=1, lastline=CSpike6App::tRecord->GetCount();
Float64 bmsec,totview,val,valu,maxintv,tmp;
Float64 k;
LArray *WorkArray = new LArray(sizeof(Float64));
ThrowIfNil_(WorkArray);
val=0.0;
for (i=1; i<=CSpike6App::SpikeParams.maxbns; i++)
	{
	WorkArray->AddItem(&val, sizeof(Float64));
	}//fill workID with zeroes
//use all templates
bmsec = CSpike6App::SpikeParams.BWdth / 1000;
totview = bmsec * CSpike6App::SpikeParams.maxbns;
//{get highest time value}

//{filling the bins of densarray}
tot1=0;
tot = 0;
maxintv = bmsec * CSpike6App::SpikeParams.maxbns;
CSpike6App::templrecrd theTRec,theTRec2;
for (i = firstline; i<= lastline - 1; i++)
	{
	CSpike6App::tRecord->FetchItemAt(i,&theTRec);
	CSpike6App::tRecord->FetchItemAt(i+1,&theTRec2);
	tmp = theTRec2.timestamp-theTRec.timestamp;
	if (tmp <= totview) 
		{
		binnum = 1 + (CSpike6App::SpikeParams.maxbns * tmp) / maxintv;
		WorkArray->FetchItemAt(binnum, &valu);
		//valu=*nmbr;
		valu = valu + 1;
		WorkArray->AssignItemsAt(1,binnum, &valu,sizeof(Float64));
		tot = tot + 1;
		}
	}
//Bins2Draw(WorkArray);	
k=1;
valu = 0;
val = 0.0;
CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
k+=1.0;
CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));

for (i=1;i<=CSpike6App::SpikeParams.maxbns;i++)
	{	
	WorkArray->FetchItemAt(i,&val);
	CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
	CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
	k+=1.0;
	valu=valu + bmsec;//Float64(i);
	CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
	CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
	k+=1.0;
	}
val=0.0;
CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
delete WorkArray;
WorkArray=nil;
}

//================================================================================
void MakeIntvHisto()
{
SInt32 i,j, kk, tot,tot1,binnum,templ,spikecount=CSpike6App::tRecord->GetCount();
Float64 bmsec,totview,val,valu,maxintv,tim,tmp;
Float64 k, number;
LArray *WorkArray = new LArray(sizeof(Float64));
ThrowIfNil_(WorkArray);
val=0;
for (i=1; i<=CSpike6App::SpikeParams.maxbns; i++)
	{
	WorkArray->AddItem(&val, sizeof(Float64));
}//fill workID with zeroes
//determine which templates
	
if ((CSpike6App::SpikeParams.templ1 > -1) && (CSpike6App::SpikeParams.templ2 < 0)) 
	{CSpike6App::SpikeParams.templ2 = CSpike6App::SpikeParams.templ1;}
if ((CSpike6App::SpikeParams.templ2 > -1) && (CSpike6App::SpikeParams.templ1 < 0)) 
	{CSpike6App::SpikeParams.templ1 = CSpike6App::SpikeParams.templ2;}
bmsec = CSpike6App::SpikeParams.BWdth / 1000.0;
totview = CSpike6App::SpikeParams.BWdth* (Float64)CSpike6App::SpikeParams.maxbns/1000.0;//bmsec * (Float64)CSpike6App::SpikeParams.maxbns;
//{get highest time value}

//{filling the bins of densarray}
tot1=0;
tot = 0;
maxintv = totview;//bmsec * (Float64)CSpike6App::SpikeParams.maxbns;
CSpike6App::templrecrd theTRec;
bool going = true;
i = CSpike6App::SpikeParams.minlin;
while (going){//go find the first legal occurrence of the source spike
	CSpike6App::tRecord->FetchItemAt(i, &theTRec); //get template number of source spike}
	templ=theTRec.templatenum;
	if ((templ==CSpike6App::SpikeParams.templ1)||(i == CSpike6App::SpikeParams.maxlin)) {
		going = false;
	}
	i+= 1;
}
tim = theTRec.timestamp;//the time of the first source spike
	//we found the first occurrence of our template (on line i-1)
i-=1;
for (j=i;j<=CSpike6App::SpikeParams.maxlin;j++){
	CSpike6App::tRecord->FetchItemAt(j, &theTRec); //get template number of source spike}
	templ=theTRec.templatenum;
	if (templ==CSpike6App::SpikeParams.templ1){
		tim = theTRec.timestamp;//the time of the source spike
		going = true;
		kk = j;
		while (going){
			kk+=1;
			CSpike6App::tRecord->FetchItemAt(kk, &theTRec); //get template number of target spike}
			templ=theTRec.templatenum;
			tmp = theTRec.timestamp - tim;//the interval
			if (tmp<=maxintv){
				if (templ==CSpike6App::SpikeParams.templ2){
					binnum = 1.0 + trunc((Float64)CSpike6App::SpikeParams.maxbns * tmp / maxintv);
					WorkArray->FetchItemAt(binnum, &number);
					number +=1;
					WorkArray->AssignItemsAt(1, binnum, &number, sizeof(Float64));
					tot = tot + 1;
					going = false;
				}
			} else {
				going = false;
			}
			if(kk-1 == spikecount){
				going = false;
			}
		}
	}	
}
	//Bins2Draw(WorkArray);	
k=1;
valu = 0;
val = 0.0;
CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
k+=1.0;
CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));

for (i=1;i<=CSpike6App::SpikeParams.maxbns;i++)
	{	
	WorkArray->FetchItemAt(i,&val);
	CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
	CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
	k+=1.0;
	valu=valu + bmsec;//Float64(i);
	CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
	CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
	k+=1.0;
	}
valu=0.0;
CSettingsWindow::DrawCol2->AddItem(&valu, sizeof(Float64));
delete WorkArray;
WorkArray=nil;
}
//========================================================
//AllPSTH
//========================================================

SInt32 ** AllPSTH(SInt16 ntemplates, Float64 binwidth, SInt32 nbins)
{
    SInt32 i, j, t1, t2, count=CSpike6App::tRecord->GetCount(),arsze=0.5*(ntemplates*ntemplates-ntemplates)+1;
    //arsze+1 because the first location has the times
    SInt32  mybin, myplace;
    Float64 myval1, myval2, mydif,scope=binwidth*nbins/1000;
    binwidth=binwidth/1000;
    SInt32 **difarr = (SInt32 **)malloc (arsze*sizeof(SInt32 *));
    for (i=0; i < arsze; i++){
        difarr[i]=(SInt32 *)malloc((2*nbins+2)*sizeof(SInt32));
    }
    for (i=0; i < arsze; i++){
        for(j=0;j<2*nbins+2;j++){
            difarr[i][j]=0.0;
        }
    }
    CSpike6App::templrecrd theTRec;
    for (i = 1; i <=count;i++){
        CSpike6App::tRecord->FetchItemAt(i,&theTRec);
        t1 = theTRec.templatenum + 1;
        myval1 = theTRec.timestamp;
        for (j=i-1;j>=1;j--){
            CSpike6App::tRecord->FetchItemAt(j,&theTRec);
            t2 = theTRec.templatenum+1;
            if (t2 > t1){
                myval2 = theTRec.timestamp;
                mydif=myval1-myval2;
                if (mydif > scope){
                    break;
                } else {
                    mybin = nbins - trunc(mydif/binwidth)-1;
                    //myplace=(t1-1)*ntemplates-(t1-1)*(1+t1)/2 + t2-t1;
                    myplace=(t1-1)*ntemplates+(ntemplates-(ntemplates-t2))-t1*(t1+1)/2;
                    difarr[myplace][mybin]+=1;
                }
            }
        }
        for (j=i+1;j<=count;j++){
            CSpike6App::tRecord->FetchItemAt(j,&theTRec);
            t2 = theTRec.templatenum + 1;
            if (t2 > t1){
                myval2 = theTRec.timestamp;
                mydif=myval2-myval1;
                if (mydif > scope){
                    break;
                } else {
                    mybin = nbins + trunc(mydif/binwidth);
                    myplace=(t1-1)*ntemplates+(ntemplates-(ntemplates-t2))-t1*(t1+1)/2;//-(t1-1)*(1+t1)/2 + t2-t1;
                    difarr[myplace][mybin]+=1;
                    difarr[myplace][2*nbins]=t1-1;
                    difarr[myplace][2*nbins+1]=t2-1;
                }
            }
        }
        //now put the cnt in the right place in difarr
        
    }   
    mybin=(SInt32)(-nbins*binwidth*1000 + binwidth*1000/2);
    for(i=0;i<2*nbins;i++){
        difarr[0][i]=mybin;
        mybin+=binwidth*1000;
    }
    /*mybin=difarr[1][2*nbins];
    mybin=difarr[1][2*nbins+1];*/
    return difarr;
}


//========================================================
//    AllDensityGSL
//===========================================================================
//new alldensityprocedure that makes use of the gsl_histogram library routines

void AllDensityGSL(SInt16 colstosaveflag)
	{
	CSpike6App::templrecrd theTRec;
	SInt32 i=0, j=0, k=0, templ=0, binnum,mycount;
	int mm;
	size_t nbins=2*CSpike6App::SpikeParams.maxbns, nn;
	
	double val, valu, intval;
	Float64 myval, mycnt, scope=CSpike6App::SpikeParams.BWdth * CSpike6App::SpikeParams.maxbns/1000,bwdt=CSpike6App::SpikeParams.BWdth/1000;
	
	double minval, maxval, mval;
	maxval = CSpike6App::SpikeParams.BWdth * CSpike6App::SpikeParams.maxbns/1000;
	minval = -maxval;
		
	gsl_histogram *h = gsl_histogram_alloc (nbins);
	mm=gsl_histogram_set_ranges_uniform (h, minval, maxval);
	
	//determine which templates}
	
	if (((CSpike6App::SpikeParams.templ1 > -1) && (CSpike6App::SpikeParams.templ2 < 0))||
		(CSpike6App::SpikeParams.templ1==CSpike6App::SpikeParams.templ2))
		{
		CSpike6App::SpikeParams.templ2 = CSpike6App::SpikeParams.templ1;
    }
	for (j=CSpike6App::SpikeParams.minlin; j<=CSpike6App::SpikeParams.maxlin; j++)
		{
		CSpike6App::tRecord->FetchItemAt(j,&theTRec);
		templ=theTRec.templatenum;
        mycount=0;
		if (templ==CSpike6App::SpikeParams.templ1)
			{
            mycount+=1;
			i+=1;
			val=theTRec.timestamp;
			for (k=j+1; k<=CSpike6App::SpikeParams.maxlin; k++)
				{
				CSpike6App::tRecord->FetchItemAt(k,&theTRec);
				valu=theTRec.timestamp;
				intval=valu-val;
				if (intval > scope){
					break;
                }
                if (theTRec.templatenum ==CSpike6App::SpikeParams.templ2){
                    binnum = CSpike6App::SpikeParams.maxbns + 1 + trunc(intval / bwdt);
					mm=gsl_histogram_increment (h, intval);
                }
            }
			for (k=j-1; k>=CSpike6App::SpikeParams.minlin; k--)
				{
				CSpike6App::tRecord->FetchItemAt(k,&theTRec);
				valu=theTRec.timestamp;
				intval=valu-val;
				if (intval < -scope){
					break;
                }
				if (theTRec.templatenum ==CSpike6App::SpikeParams.templ2){
                    binnum = CSpike6App::SpikeParams.maxbns - trunc(intval / bwdt);
					mm=gsl_histogram_increment (h, intval);
                }
            }
        }
    }
	bwdt = (maxval - minval)/nbins;
	valu =  minval;
    //Drawcol4 and Drawcol5 show count + resp count - sd of the bincounts
	if (colstosaveflag==1){
		for (i=1;i<=nbins;i++)
			{	
			nn = (size_t)i-1;
			mval = gsl_histogram_get(h, nn);
			mm=gsl_histogram_get_range(h, nn, &minval, &maxval);
			val = (Float64)mval;
			CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
			CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
			k+=1;
			CSettingsWindow::DrawCol1->AddItem(&minval, sizeof(Float64));
			CSettingsWindow::DrawCol1->AddItem(&maxval, sizeof(Float64));
			k+=1;
			}
		}
	else if (colstosaveflag==2){
		for (i=1;i<=nbins;i++)
			{	
			nn=(size_t)i-1;
			mval = gsl_histogram_get(h, nn);
			val = (Float64)mval;
			CStatsWindow::stDrawCol2->AddItem(&val, sizeof(Float64));
			mm=gsl_histogram_get_range(h, nn, &minval, &maxval);
			CStatsWindow::stDrawCol1->AddItem(&minval, sizeof(Float64));
			valu +=bwdt;
			}
		}
	//make confidence limits (Abeles, 1982)
		SInt32 firstbins, lastbins,cnt=0;
		Float64 avg=0, prob, probsum=0.0;
		firstbins=round(nbins/4.0);
		lastbins=firstbins*3;
		for (i=1; i<=firstbins; i++){//get the counts in the lower and higher 25% of the bins
			nn = (size_t)i-1;
			mval = gsl_histogram_get(h, nn);
			avg+=mval;
			cnt+=1;
		}
		avg=avg/(Float64)cnt;//this is the lambda in the poisson calculation.
		//if (avg <=30){
		bool fnd = false;
		for(i=0;i<=20000; i++){
			prob=gsl_ran_poisson_pdf(i,avg);
			probsum+=prob;
			if((probsum>0.01)&&(!fnd)){
				CSettingsWindow::lolim=(Float64)i-1.0;
				if(CSettingsWindow::lolim < 0){CSettingsWindow::lolim = 0.0;}
				fnd = true;
			}
			if ((probsum>0.05)&&(fnd)){
				CSettingsWindow::lolim05=(Float64)i-1.0;
				if(CSettingsWindow::lolim05 < 0){CSettingsWindow::lolim05 = 0.0;}
				break;
			}
		}
		fnd = false;
		for(j=i+1; j<=20000;j++){
			prob=gsl_ran_poisson_pdf(j,avg);
			probsum+=prob;
			if ((probsum>0.95)&&(!fnd)){
				CSettingsWindow::uplim05=(Float64)j;
				fnd=true;
			}
			if(probsum>0.99){
				CSettingsWindow::uplim=(Float64)j;
				break;
			}
		}
	gsl_histogram_free(h);
	}

/*void AllAllDensity(bool useallspikes)
	{
		CSpike6App::SpikeParams.minlin = 1;
		CSpike6App::SpikeParams.maxlin = CSpike6App::tRecord->GetCount();
		SInt32 ntemplates = CSpike6App::sAvgCol2->GetCount(),toptempl=0,cntrwel=0,cntrniet=0,cntralle=0,cntrnulls=0,cntrnull2=0;
		LArray *mycounts = new LArray(sizeof(SInt32));
		Float64 leftIndex, rightIndex,totIndex,directIndex,chisquare,expect,tot;
		SInt32 i, j, k, n1=0, n2=0, cnt;
        SInt32 tempbins=CSpike6App::SpikeParams.maxbns;
        Float64 tempbwdth=CSpike6App::SpikeParams.BWdth;
        CSpike6App::SpikeParams.maxbns=80;//bin 71-79 and 82 - 90 used for N1 and N2
        //bin1-9 and 152 - 160 used for C1 and C2
        CSpike6App::SpikeParams.BWdth=50;
		SInt32 n3=CSpike6App::SpikeParams.maxbns-round(500/CSpike6App::SpikeParams.BWdth);
		Float64 theCount=0, theLeftCount2=0, theRightCount2 = 0, theLeftNullCount=0, theRightNullCount=0;
		//n3 is the first bin to look at in the result array. the last is maxbns +  n3
		//the array goes from -maxbns to + maxbins. The area where we calculate stuff runs from
		//n3 to maxbns + n3, i.e. -0.5 to +0.5 sec from the origin. The baseline stretch of data 
		//runs from -200 to -175 bins
		//find the maximum template number
		for (i=1;i<=ntemplates;i++){
			CSpike6App::sAvgCol2->FetchItemAt(i, &n1);
			if (n1 != 0) {
			toptempl=i;
			}
		}
		ntemplates = toptempl;
		Float64 myscope = CSpike6App::SpikeParams.BWdth*CSpike6App::SpikeParams.maxbns;
		Float64 mycount;
		//myscope: how much time are we looking at Check whether the choice of bins is realistic, i.e.
		//the scope is not too small (2*500 msec either way)
		if (myscope >= 1000){
			//i=0;
			//mycounts->InsertItemsAt(1,1,&i);//upper left cell has nothing in it
			//determine which will be the first bin to look at
			for (i = 1; i <= ntemplates; i++){
                cntralle+=1;
				CSpike6App::sAvgCol2->FetchItemAt(i, &n1);
				CSpike6App::SpikeParams.templ1 = i-1;
				mycounts->AddItem(&CSpike6App::SpikeParams.templ1);
				if (n1 != 0){
					for (j = 1; j <= ntemplates; j++){
						CSpike6App::sAvgCol2->FetchItemAt(j, &n2);
						if (n2 != 0){
                            

							//make alldensityGSL
                            
							delete CStatsWindow::stDrawCol1;
							CStatsWindow::stDrawCol1 =nil;
							CStatsWindow::stDrawCol1 = new LArray(sizeof(Float64));
							delete CStatsWindow::stDrawCol2;
							CStatsWindow::stDrawCol2 =nil;
							CStatsWindow::stDrawCol2 = new LArray(sizeof(Float64));
							CSpike6App::SpikeParams.templ2 = j-1;
							AllDensityGSL(2);
                            
                            //
                            cnt=CStatsWindow::stDrawCol1->GetCount();
							theCount = 0.0;
                            for (k=1; k<=160; k++){//total events in plot
                                CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
								theCount += (Float64)mycount;
                            }
                            if ((theCount >= cnt)||(useallspikes)){
                                cntrwel+=1;
                                theLeftNullCount = 0.0;
                                for (k = 1; k <= 9; k++){
                                    CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                    theLeftNullCount += (Float64)mycount;
                                }
                                theRightNullCount = 0.0;
                                for (k = 152; k <= 160; k++){
                                    CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                    theRightNullCount += (Float64)mycount;
                                }
                                theLeftCount2 = 0.0;
                                for (k = 71; k <= 79; k++){
                                    CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                    theLeftCount2 += (Float64)mycount;
                                }
                                theRightCount2 = 0.0;
                                for (k = 82; k<=90;k++){
                                    CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                    theRightCount2 += (Float64)mycount;
                                }
                                
                                expect=(theLeftNullCount+theRightNullCount+theLeftCount2+theRightCount2)/4.0;
                                chisquare=((theLeftNullCount-expect)*(theLeftNullCount-expect)+(theRightNullCount-expect)*(theRightNullCount-expect)+(theLeftCount2-expect)*(theLeftCount2-expect)+(theRightCount2-expect)*(theRightCount2-expect))/expect;
                                if (chisquare > 7.81){
                                    chisquare=1.0;
                                }
                                else {
                                    chisquare=0.0;
                                }
                                if (chisquare >0.0){
                                    leftIndex = 100*(theLeftCount2 - theRightNullCount)/(theLeftCount2 + theRightNullCount);
                                    rightIndex = 100*(theRightCount2 - theLeftNullCount)/(theRightCount2 + theLeftNullCount);
                                    totIndex = 100*((theLeftCount2+theRightCount2)-(theRightNullCount + theLeftNullCount))/((theLeftCount2+theRightCount2)+(theRightNullCount + theLeftNullCount));
                                    directIndex=100*((theRightCount2 - theLeftCount2)/(theRightCount2 + theLeftCount2));
                                    CSettingsWindow::DrawCol6->AddItem(&leftIndex);
                                    CSettingsWindow::DrawCol7->AddItem(&rightIndex);
                                    CSettingsWindow::DrawCol5->AddItem(&totIndex);
                                    CSettingsWindow::DrawCol8->AddItem(&directIndex);
                                    //CSettingsWindow::DrawCol9->AddItem(&theCount);
                                    CSettingsWindow::DrawCol9->AddItem(&chisquare);
                                }
                                else {
                                    theCount = 0;
                                    CSettingsWindow::DrawCol6->AddItem(&theCount);
                                    CSettingsWindow::DrawCol7->AddItem(&theCount);
                                    CSettingsWindow::DrawCol5->AddItem(&theCount);
                                    CSettingsWindow::DrawCol8->AddItem(&theCount);
                                    CSettingsWindow::DrawCol9->AddItem(&theCount);
                                }
                            }
                            else {
                                cntrniet+=1;
                                //CSettingsWindow::DrawCol9->AddItem(&theCount);
                                theCount = 0;
                                CSettingsWindow::DrawCol6->AddItem(&theCount);
                                CSettingsWindow::DrawCol7->AddItem(&theCount);
                                CSettingsWindow::DrawCol5->AddItem(&theCount);
                                CSettingsWindow::DrawCol8->AddItem(&theCount);
                                CSettingsWindow::DrawCol9->AddItem(&theCount);
                            }
						}
						else {
                            cntrnulls +=1;
							theCount = 0;
							CSettingsWindow::DrawCol6->AddItem(&theCount);
							CSettingsWindow::DrawCol7->AddItem(&theCount);
							CSettingsWindow::DrawCol5->AddItem(&theCount);
							CSettingsWindow::DrawCol8->AddItem(&theCount);
							CSettingsWindow::DrawCol9->AddItem(&theCount);
						}
					}
				}
				else {
                    cntrnull2+=1;
					for (j = 1; j<=ntemplates;j++){
						theCount=0;
						CSettingsWindow::DrawCol6->AddItem(&theCount);
						CSettingsWindow::DrawCol7->AddItem(&theCount);
						CSettingsWindow::DrawCol5->AddItem(&theCount);
						CSettingsWindow::DrawCol8->AddItem(&theCount);
						CSettingsWindow::DrawCol9->AddItem(&theCount);
					}

				}
			}
		}
		
		theCount=CSettingsWindow::DrawCol9->GetCount();
	}*/
//===================================================

//milena's version
///this version only takes into account two periods: 0.5 sec before, 0-0.5 sec after the replay

//====================================================
void AllAllDensity()
{
    CSpike6App::SpikeParams.minlin = 1;
    CSpike6App::SpikeParams.maxlin = CSpike6App::tRecord->GetCount();
    SInt32 ntemplates = CSpike6App::sAvgCol2->GetCount(),toptempl=0,cntrwel=0,cntrniet=0,cntralle=0,cntrnulls=0,cntrnull2=0;
    LArray *mycounts = new LArray(sizeof(SInt32));
    Float64 leftIndex, rightIndex,totIndex,directIndex,chisquare,expect;
    SInt32 i, j, k, n1=0, n2=0, cnt;
    CSpike6App::SpikeParams.maxbns=80;//bin 71-79 and 82 - 90 used for N1 and N2
    //bin1-9 and 152 - 160 used for C1 and C2
    CSpike6App::SpikeParams.BWdth=50;
    Float64 theCount=0, theLeftCount2=0, theRightCount2 = 0, theLeftNullCount=0, theRightNullCount=0;
    //n3 is the first bin to look at in the result array. the last is maxbns +  n3
    //the array goes from -maxbns to + maxbins. The area where we calculate stuff runs from
    //n3 to maxbns + n3, i.e. -0.5 to +0.5 sec from the origin. The baseline stretch of data 
    //runs from -200 to -175 bins
    //find the maximum template number
    for (i=1;i<=ntemplates;i++){
        CSpike6App::sAvgCol2->FetchItemAt(i, &n1);
        if (n1 != 0) {
			toptempl=i;
        }
    }
    ntemplates = toptempl;
    Float64 myscope = CSpike6App::SpikeParams.BWdth*CSpike6App::SpikeParams.maxbns;
    Float64 mycount;
    //myscope: how much time are we looking at Check whether the choice of bins is realistic, i.e.
    //the scope is not too small (2*500 msec either way)
    if (myscope >= 500){
        //i=0;
        //mycounts->InsertItemsAt(1,1,&i);//upper left cell has nothing in it
        //determine which will be the first bin to look at
        for (i = 1; i <= ntemplates; i++){
            cntralle+=1;
            CSpike6App::sAvgCol2->FetchItemAt(i, &n1);
            CSpike6App::SpikeParams.templ1 = i-1;
            mycounts->AddItem(&CSpike6App::SpikeParams.templ1);
            if (n1 != 0){
                for (j = 1; j <= ntemplates; j++){
                    CSpike6App::sAvgCol2->FetchItemAt(j, &n2);
                    if (n2 != 0){              
                        delete CStatsWindow::stDrawCol1;
                        CStatsWindow::stDrawCol1 =nil;
                        CStatsWindow::stDrawCol1 = new LArray(sizeof(Float64));
                        delete CStatsWindow::stDrawCol2;
                        CStatsWindow::stDrawCol2 =nil;
                        CStatsWindow::stDrawCol2 = new LArray(sizeof(Float64));
                        CSpike6App::SpikeParams.templ2 = j-1;
                        AllDensityGSL(2);
                        cnt=CStatsWindow::stDrawCol1->GetCount();
                        theCount = 0.0;
                        for (k=1; k<=160; k++){//total events in plot
                            CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                            theCount += (Float64)mycount;
                        }
                        if ((theCount >= cnt)){
                            cntrwel+=1;
                            theLeftCount2 = 0.0;
                            for (k = 71; k <= 79; k++){
                                CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                theLeftCount2 += (Float64)mycount;
                            }
                            theRightCount2 = 0.0;
                            for (k = 82; k<=90 ;k++){
                                CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                theRightCount2 += (Float64)mycount;
                            }
                            theLeftNullCount = 0.0;
                            for (k = 1; k <= 9; k++){
                                CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                theLeftNullCount += (Float64)mycount;
                            }
                            theRightNullCount = 0.0;
                            for (k = 152; k<=160 ;k++){
                                CStatsWindow::stDrawCol2->FetchItemAt(k, &mycount);
                                theRightNullCount += (Float64)mycount;
                            }
                            expect=(theLeftCount2+theRightCount2)/2.0;
                            if (expect==0){
                                chisquare=0;
                            } else {
                                chisquare=((theLeftCount2-expect)*(theLeftCount2-expect)+(theRightCount2-expect)*(theRightCount2-expect))/expect;
                            }
                            leftIndex = 100*(theLeftCount2 - theRightNullCount)/(theLeftCount2 + theRightNullCount);
                            rightIndex = 100*(theRightCount2 - theLeftNullCount)/(theRightCount2 + theLeftNullCount);
                            totIndex = 100*((theLeftCount2+theRightCount2)-(theRightNullCount + theLeftNullCount))/((theLeftCount2+theRightCount2)+(theRightNullCount + theLeftNullCount));
                            if ((theLeftCount2==0)&&(theRightCount2==0)){
                                directIndex=0;
                                //chisquare=0;
                            } else {
                            directIndex=100*((theRightCount2 - theLeftCount2)/(theRightCount2 + theLeftCount2));
                            }
                            CSettingsWindow::DrawCol8->AddItem(&directIndex);
                            CSettingsWindow::DrawCol9->AddItem(&theCount);
                                //CSettingsWindow::DrawCol9->AddItem(&chisquare);
                                //theCount = 0;
                            CSettingsWindow::DrawCol6->AddItem(&leftIndex);
                            CSettingsWindow::DrawCol7->AddItem(&rightIndex);
                            CSettingsWindow::DrawCol5->AddItem(&totIndex);
                               
                           // }
                        }
                        else {
                            cntrniet+=1;
                            theCount = 0;
                            CSettingsWindow::DrawCol6->AddItem(&theCount);
                            CSettingsWindow::DrawCol7->AddItem(&theCount);
                            CSettingsWindow::DrawCol5->AddItem(&theCount);
                            CSettingsWindow::DrawCol8->AddItem(&theCount);
                            CSettingsWindow::DrawCol9->AddItem(&theCount);
                        }
                    }
                    else {
                        cntrnulls +=1;
                        theCount = 0;
                        CSettingsWindow::DrawCol6->AddItem(&theCount);
                        CSettingsWindow::DrawCol7->AddItem(&theCount);
                        CSettingsWindow::DrawCol5->AddItem(&theCount);
                        CSettingsWindow::DrawCol8->AddItem(&theCount);
                        CSettingsWindow::DrawCol9->AddItem(&theCount);
                    }
                }
            }
            else {
                cntrnull2+=1;
                for (j = 1; j<=ntemplates;j++){
                    theCount=0;
                    CSettingsWindow::DrawCol6->AddItem(&theCount);
                    CSettingsWindow::DrawCol7->AddItem(&theCount);
                    CSettingsWindow::DrawCol5->AddItem(&theCount);
                    CSettingsWindow::DrawCol8->AddItem(&theCount);
                    CSettingsWindow::DrawCol9->AddItem(&theCount);
                }
                
            }
        }
    }
    
    theCount=CSettingsWindow::DrawCol9->GetCount();
}
//========================================================
//    AllDensity
//===========================================================================

// 9/10/2002 Changed procedure such that first the span of the view forward and backwards from the source spike is 
//always larger than the total view. This because if we use spikes at the very beginning or end of the file, 
//asymmetries arise because from the target spike we may look further back than forward and vice versa.

void AllDensity(SInt16 colstosaveflag)
	{
	SInt32 	i, j, tot=0, binnum,templ,itemcnt,mmaxlin,mminlin;
	Float64	tim,tim1,totview, val, valu, myval, bwdt,intv;
	Float64  k;
	//SInt32 halfbins=round(CSpike6App::SpikeParams.maxbns/2);
	CSpike6App::templrecrd theTRec;
	
//determine which templates}
	
	if (((CSpike6App::SpikeParams.templ1 != 0) && (CSpike6App::SpikeParams.templ2 == 0))||
		(CSpike6App::SpikeParams.templ1==CSpike6App::SpikeParams.templ2))
		{
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1, &itemcnt);
		CSpike6App::SpikeParams.templ2 = CSpike6App::SpikeParams.templ1;
		}
	else if ((CSpike6App::SpikeParams.templ2 != 0) and (CSpike6App::SpikeParams.templ1 != 0))
		{
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1, &itemcnt);
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ2, &i);
		itemcnt+=i;
		}
		
	LArray *WorkArray = new LArray(sizeof(Float64));
	ThrowIfNil_(WorkArray);
	LArray *HelpCol1 = new LArray(itemcnt,sizeof(Float64));
	ThrowIfNil_(HelpCol1);
	LArray *HelpCol2 = new LArray(itemcnt,sizeof(SInt32));
	ThrowIfNil_(HelpCol2);
	LArray *sumArray = new LArray(itemcnt,sizeof(Float64));
	ThrowIfNil_(sumArray);
	LArray *sumsumArray = new LArray(itemcnt,sizeof(Float64));
	ThrowIfNil_(sumsumArray);
	LArray *helpArray = new LArray(itemcnt,sizeof(Float64));
	ThrowIfNil_(helpArray);
	Float64 diftime;
	val=0.0;
	for (i = 1; i<= 2 * CSpike6App::SpikeParams.maxbns;i++)
		{
		WorkArray->AddItem(&val, sizeof(Float64));
		sumArray->AddItem(&val, sizeof(Float64));
		sumsumArray->AddItem(&val, sizeof(Float64));	
		helpArray->AddItem(&val, sizeof(Float64));	
	}//fill workID with zeroes
		
	totview = CSpike6App::SpikeParams.BWdth * CSpike6App::SpikeParams.maxbns/1000.0;
	bwdt = CSpike6App::SpikeParams.BWdth/1000.0;
			
	//make an array with only source and target spikes times in helpcol1, template #s in helpcol2
	
	j=1;
	for (i=CSpike6App::SpikeParams.minlin;i<=CSpike6App::SpikeParams.maxlin;i++)
		{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if ((templ==CSpike6App::SpikeParams.templ1)||(templ==CSpike6App::SpikeParams.templ2))
			{
			HelpCol2->AddItem(&templ,sizeof(SInt32));
			tim=theTRec.timestamp;
			HelpCol1->AddItem(&tim,sizeof(Float64));
			j+=1;
			}
		}
	
	//determine where to start(line where (timestamp-totview)>=0.0
	
	SInt32 cnt=HelpCol1->GetCount();
	for (i=1; i<=cnt;i++){
		HelpCol1->FetchItemAt(i,&tim);
		diftime=tim-totview;
		if (diftime>=CSpike6App::SpikeParams.strt){
			break;
			}
		}
	mminlin=i;
	for (i=cnt; i>=1;i--){
		HelpCol1->FetchItemAt(i,&tim);
		diftime=tim +totview;
		if (diftime<CSpike6App::SpikeParams.stp){
			break;
			}
		}
	mmaxlin=i+1;

	
	//make bins and put counts in workarray
	
	//mmaxlin=j-1;
	//mminlin=1;
	for (i=mminlin; i<=mmaxlin; i++)
		{
		HelpCol2->FetchItemAt(i,&templ);
		if (templ==CSpike6App::SpikeParams.templ1)
			{
			HelpCol1->FetchItemAt(i,&tim1);
			intv=0;
			for (j=i-1;j>=1;j--)//looking back
				{
				HelpCol2->FetchItemAt(j,&templ);
				if (templ==CSpike6App::SpikeParams.templ2)
					{
					HelpCol1->FetchItemAt(j,&tim);
					intv=tim1-tim;
					if (intv >= totview)
						{
						break;
						}
					binnum = CSpike6App::SpikeParams.maxbns - trunc(intv / bwdt);
					//if(binnum==80){
					//	tim=tim1;
					//	}
					WorkArray->FetchItemAt(binnum, &valu);
					helpArray->FetchItemAt(binnum,&myval);
					valu = valu + 1;
					myval+=1;
					WorkArray->AssignItemsAt(1,binnum, &valu,sizeof(Float64));
					helpArray->AssignItemsAt(1,binnum,&myval);
					tot = tot + 1;
					}
				}
			intv=0;
			for (j=i+1;j<=cnt;j++)//looking forward
				{
				HelpCol2->FetchItemAt(j,&templ);
				if (templ==CSpike6App::SpikeParams.templ2)
					{
					HelpCol1->FetchItemAt(j,&tim);
					intv=tim-tim1;
					if (intv >=totview)	
						{
						break;
						}
					binnum = CSpike6App::SpikeParams.maxbns+1 + trunc(intv / bwdt);
					WorkArray->FetchItemAt(binnum, &valu);
					helpArray->FetchItemAt(binnum,&myval);
					valu = valu + 1;
					myval+=1;
					WorkArray->AssignItemsAt(1,binnum, &valu,sizeof(Float64));
					helpArray->AssignItemsAt(1,binnum,&myval);
					tot = tot + 1;
					}
				}
			for (k = 1;k <= 2*CSpike6App::SpikeParams.maxbns; k++){
				helpArray->FetchItemAt(k,&myval);
				sumArray->FetchItemAt(k,&valu);
				valu+=myval;
				sumArray->AssignItemsAt(1,k,&valu);
				sumsumArray->FetchItemAt(k,&valu);
				valu+=myval*myval;
				sumsumArray->AssignItemsAt(1,k,&valu);
				myval=0.0;
				helpArray->AssignItemsAt(1,k,&myval);
				}	
			}
		}
		
	// transfer binnumbers and counts to DrawCol1 and 2
	k=1;
	valu =  - CSpike6App::SpikeParams.maxbns * bwdt;
	if (colstosaveflag==1){
		CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
		val = 0;
		CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
		k+=1.0;
		CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
		for (i=1;i<=2*CSpike6App::SpikeParams.maxbns;i++)
			{	
			WorkArray->FetchItemAt(i,&val);
			CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
			CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
			k+=1.0;
			
			CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
			CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
			valu +=bwdt;
			k+=1.0;
			}
		val=0.0;
		CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
		}
	else if (colstosaveflag==2){
		for (i=1;i<=2*CSpike6App::SpikeParams.maxbns;i++)
			{	
			WorkArray->FetchItemAt(i,&val);
			CStatsWindow::stDrawCol2->AddItem(&val, sizeof(Float64));
			
			CStatsWindow::stDrawCol1->AddItem(&valu, sizeof(Float64));
			valu +=bwdt;
			}
		}

	delete WorkArray;
	WorkArray=nil;
	delete sumArray;
	sumArray=nil;
	delete sumsumArray;
	sumsumArray=nil;
	delete helpArray;
	helpArray=nil;
	delete HelpCol1;
	HelpCol1=nil;
	delete HelpCol2;
	HelpCol2=nil;
	}

//========================================================
//    CumPSTH //uses lots of code from AllDensity
//===========================================================================

void CumPSTH(SInt16 colstosaveflag)
	{
	SInt32 	i, j, tot=0, binnum, templ,itemcnt;
	Float64	tim,tim1,tim2,totview, val, valu,bwdt,intv;
	Float64 k;
	//SInt32 halfbins=round(CSpike6App::SpikeParams.maxbns/2);
	CSpike6App::templrecrd theTRec;
	
//determine which templates}
		templ=CSpike6App::SpikeParams.templ2;
	if ((CSpike6App::SpikeParams.templ1 != 0) && (CSpike6App::SpikeParams.templ2 < 0))
		{
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1, &itemcnt);
		CSpike6App::SpikeParams.templ2 = CSpike6App::SpikeParams.templ1;
		}
	if ((CSpike6App::SpikeParams.templ2 != 0) and (CSpike6App::SpikeParams.templ1 != 0))
	{
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1, &itemcnt);
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ2, &i);
		itemcnt+=i;
	}
		templ=CSpike6App::SpikeParams.templ2;

	LArray *WorkArray = new LArray(sizeof(Float64));
	ThrowIfNil_(WorkArray);
	LArray *HelpCol1 = new LArray(itemcnt,sizeof(Float64));
	ThrowIfNil_(HelpCol1);
	LArray *HelpCol2 = new LArray(itemcnt,sizeof(Float64));
	ThrowIfNil_(HelpCol2);

	val=0;
	for (i = 1; i<= 2 * CSpike6App::SpikeParams.maxbns;i++)
		{WorkArray->AddItem(&val, sizeof(Float64));}//fill workID with zeroes
	totview = CSpike6App::SpikeParams.BWdth * CSpike6App::SpikeParams.maxbns/1000;
	bwdt = CSpike6App::SpikeParams.BWdth/1000;
	

	//make an array with only source and target spikes times of stimulus in helpcol1, target times in helpcol2
	CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
	tim1=theTRec.timestamp;//lowest timestamp
	tim1=tim1 + totview;//lowest timestamp for stimulus
	CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.maxlin,&theTRec);
	tim2 = theTRec.timestamp;//highest timestamp
	tim2 = tim2 - totview;//highest timestamp for stimulus
	j=1;
	for (i=CSpike6App::SpikeParams.minlin;i<=CSpike6App::SpikeParams.maxlin;i++)
		{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ1)//is it a stimulus?
			{
			tim=theTRec.timestamp;
			if ((tim >= tim1)&&(tim <= tim2)){
				//tim-=0.3;
				HelpCol1->AddItem(&tim,sizeof(Float64));//the stimulus is within range of proper times
				}
			}
		else if (templ==CSpike6App::SpikeParams.templ2)//is it a target spike?
			{
			tim=theTRec.timestamp;
			HelpCol2->AddItem(&tim,sizeof(Float64));
			}
		}
	
	//make bins and put counts in workarray
	tim1=0.0;
	tim2=0.0;
	SInt32 mmaxlin1=HelpCol1->GetCount();//number of stimuli
	SInt32 mmaxlin2=HelpCol2->GetCount();//number of target spikes
	for (i=1; i<=mmaxlin1; i++)
		{
		HelpCol1->FetchItemAt(i,&tim1);//fetch time of stimulus
			{
			intv=0;
			for (j= 1;j<= mmaxlin2;j++)
				{
				HelpCol2->FetchItemAt(j,&tim2);
				intv=tim1-tim2;
				if ((intv > 0)&&(intv <= totview))
					{
					binnum = CSpike6App::SpikeParams.maxbns - trunc(intv / bwdt);
					WorkArray->FetchItemAt(binnum, &valu);
					valu = valu + 1;
					WorkArray->AssignItemsAt(1,binnum, &valu,sizeof(Float64));
					tot = tot + 1;
					}
				else if ((intv <= 0 )&&(intv > -totview))
					{
					binnum=CSpike6App::SpikeParams.maxbns +1 + trunc((tim2-tim1)/bwdt);
					WorkArray->FetchItemAt(binnum, &valu);
					valu = valu + 1;
					WorkArray->AssignItemsAt(1,binnum, &valu,sizeof(Float64));
					tot = tot + 1;
					}
				/*else if (binnum > 2*CSpike6App::SpikeParams.maxbns)
					{
					break;
					}*/
				}
			}
		}

	//NOW WE CAN PRODUCE A CUMULATIVE PSTH ACCORDING TO ELLAWAY(1978)
	//First get the average value of the first 50 bins(pre-stimulus)
		
	itemcnt=WorkArray->GetCount();
	itemcnt=round(itemcnt/2);
	valu=0.0;
	for (i=1; i<=itemcnt;i++){
		WorkArray->FetchItemAt(i,&val);
		valu+=val;
		}
	valu=valu/(Float64)itemcnt;//valu is average
	itemcnt=itemcnt*2;
	intv=0.0;
	for (i=1; i<=itemcnt; i++){
		WorkArray->FetchItemAt(i,&val);
		intv+=val-valu;
		WorkArray->AssignItemsAt(1,i,&intv);
		}
	// transfer sums and times to DrawCol1 and 2
	k=1;
	valu = -totview;// - CSpike6App::SpikeParams.maxbns * bwdt;
	if (colstosaveflag==1){
		for (i=1;i<=2*CSpike6App::SpikeParams.maxbns;i++)
			{	
			WorkArray->FetchItemAt(i,&val);
			CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
			
			CSettingsWindow::DrawCol1->AddItem(&valu, sizeof(Float64));
			valu +=bwdt;
			}
		}
	else if (colstosaveflag==2){
		for (i=1;i<=2*CSpike6App::SpikeParams.maxbns;i++)
			{	
			WorkArray->FetchItemAt(i,&val);
			CStatsWindow::stDrawCol2->AddItem(&val, sizeof(Float64));
			valu +=bwdt;
			CStatsWindow::stDrawCol1->AddItem(&valu, sizeof(Float64));
			}
		}
	delete WorkArray;
	WorkArray=nil;
	delete HelpCol1;
	HelpCol1=nil;
	delete HelpCol2;
	HelpCol2=nil;
	}


//================================================================================
void EventsXTime()
	{
	SInt32 i, tot=0, templ;
	Float64 val, tim, freq;
	CSpike6App::templrecrd theTRec;
	//prepare drawarray val is needed for making intervals
	CSettingsWindow::DrawCol1->AddItem(&CSpike6App::SpikeParams.strt, sizeof(Float64));//start drawing at strt
	val=0.0;
	CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
	CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
	val=theTRec.timestamp;
	for (i = CSpike6App::SpikeParams.minlin+1; i <= CSpike6App::SpikeParams.maxlin; i++)
		{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ1)
			{
			tim=theTRec.timestamp;
			CSettingsWindow::DrawCol1->AddItem(&tim, sizeof(Float64));
			val=tim-val;
			freq=1/val;
			//freq=val;
			CSettingsWindow::DrawCol2->AddItem(&freq, sizeof(Float64));
			val=tim;
			tot = tot + 1;
			}
		}
	//CSettingsWindow::DrawCol1->AddItem(&CSpike6App::SpikeParams.stp, sizeof(Float64));//stop drawing at stp
	//val=0.0;
	//CSettingsWindow::DrawCol2->AddItem(&val, sizeof(Float64));
}

Float64 NormEstimate(Float64 zzscore)//finds estimated area under the normal curve
	{
	Float64 val, valu, valout;
	SInt32 zz=0;
	older=newer;
	if (zzscore < -3.0)
		{
		valout = 0.0;
		}
	else if (zzscore > 3.0)
		{
		valout = 1.0;
		}
	else
		{
		if (zzscore<0.0)
			{
			zz=trunc(10*zzscore)-1;
			}
		else
			{
			zz=trunc(10*zzscore);
			}
		ArrayIndexT indx;
		indx=zscore->FetchIndexOf(&zz); //finds location of lower zzscore
		prop->FetchItemAt(indx, &val);
		indx+= 1;
		prop->FetchItemAt(indx, &valu);
		valout=val + (10*zzscore-zz)*(valu-val);
		}
	newer = valout;
	if(newer<older)
		{
		older=0;
		}
	return valout;
	}
	
void ProbPlot(UInt32 flag)
	{
	SInt32 i, count, templ, NN=(CSpike6App::SpikeParams.maxlin-CSpike6App::SpikeParams.minlin);
	Float64 tim,intval,avg=0.0,stdev=0.0,val,oldtim, valu;
	CSpike6App::templrecrd theTRec;
	CSpike6App::tRecord->FetchItemAt(CSpike6App::SpikeParams.minlin,&theTRec);
	oldtim=theTRec.timestamp;
	count=0;
		//fill drawcol1 with the intervals and accumulate the sum and count
	for (i=CSpike6App::SpikeParams.minlin+1; i<=CSpike6App::SpikeParams.maxlin; i++)
		{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;
		if (templ==CSpike6App::SpikeParams.templ1)
			{
			tim=theTRec.timestamp;
			intval=tim-oldtim;
			if (flag==4){
				intval=log(intval);
			}
			avg+=intval;
			stdev+=intval*intval;
			CSettingsWindow::DrawCol1->AddItem(&intval,sizeof(Float64));
			oldtim=tim;
			count+=1;
			}
		}
	SortFloat64Array(CSettingsWindow::DrawCol1);
	stdev=stdev-avg*avg/((Float64)count);
	stdev=sqrt(stdev/((Float64)count-1));
	avg=avg/((Float64)count);
	NN=CSettingsWindow::DrawCol1->GetCount();
	NN=CSettingsWindow::DrawCol2->GetCount();
	NN=CSettingsWindow::DrawCol3->GetCount();

	if ((flag ==1)||(flag==4)) //do normal estimates(or logged lognormal estimates
		{
		count=CSettingsWindow::DrawCol1->GetCount();
		for (i=1; i<=count; i++)
			{
			val=((Float64)i-0.5)/(Float64)count;//(percentile)
			val  = gsl_cdf_gaussian_Pinv(val, stdev);//bij deze val hoort bovenstaande percentiel
			val+= avg	;//expected real value
			CSettingsWindow::DrawCol2->AddItem(&val,sizeof(Float64));
			CSettingsWindow::DrawCol1->FetchItemAt(i, &valu);
			CSettingsWindow::DrawCol3->AddItem(&valu);//line x=y
			}
		}
	else if (flag==2) //do exponential estimates
		{
		count=CSettingsWindow::DrawCol1->GetCount();
			for (i=1; i<=count; i++){
				val=((Float64)i-0.5)/(Float64)count;//(percentile)
				val = gsl_cdf_exponential_Pinv(val,avg);//expected z
				CSettingsWindow::DrawCol2->AddItem(&val,sizeof(Float64));
				CSettingsWindow::DrawCol1->FetchItemAt(i, &val);
				CSettingsWindow::DrawCol3->AddItem(&val);//line x=y
			}
		}
	//CSettingsWindow::DrawCol1->SetKeepSorted(false);
		NN=CSettingsWindow::DrawCol1->GetCount();
		NN=CSettingsWindow::DrawCol2->GetCount();
		NN=CSettingsWindow::DrawCol3->GetCount();
		
		CSettingsWindow::DrawDots=true;
	}

Boolean FindLinesA (SInt32 *SError)
	{
	SInt32 i=0;
	//{find first line after strt AND first line after break (if a break exists) 
	//where templ1 fired
	SInt32 minlin=1000001; //to catch cases where target template does not occur
	SInt32 maxlin=1000001;
	SInt32 brklin=1000001;
	Boolean foundmin=FALSE;
	Boolean foundmax=FALSE;
	Boolean templtimeok=TRUE;
	Float64 val,start,stop;
	SInt32 templ, val1,val2, occurs1, occurs2,count;
	//get the number of occurrence 
	if (CSpike6App::SpikeParams.templ1 > -1){//check whether the template exists
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1+1, & occurs1);
		if (occurs1 == 0){
			*SError = 108;
			templtimeok=FALSE;
			}
		}
	if (CSpike6App::SpikeParams.templ2 > -1){//check whether the template exists
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ2+1, & occurs2);
		if (occurs1 == 0){
			*SError = 108;
			templtimeok=FALSE;
			}
		}
	CSpike6App::templrecrd theTRec;
	//check whether Templates are entered correctly
	if (CSpike6App::SpikeParams.templ1 > CSpike6App::snTemplates)//wrong template number
		{
		CSpike6App::SpikeParams.templ1 = 0;
		*SError=101;
		templtimeok=FALSE;
		}
	else if (CSpike6App::SpikeParams.templ1 < 0)
		{
		if	(!(CSpike6App::SpikeParams.templ2 ==-1))
			{
			CSpike6App::SpikeParams.templ1 = CSpike6App::SpikeParams.templ2;
			CSpike6App::SpikeParams.templ2 = -1;
			*SError=102;
			templtimeok=FALSE;
			}
		else {//if templ2 is also -1?????????
			CSpike6App::SpikeParams.templ1=0;
			*SError=103;
			templtimeok=FALSE;
			}
		}
	//else {	//{get highest time value}
	count=CSpike6App::tRecord->GetCount();
	CSpike6App::tRecord->FetchItemAt(count, &theTRec);
	val=theTRec.timestamp;
	stop=CSpike6App::SpikeParams.stp;
	val1=round(100000*stop);
	val2=round(100000*val);
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
	if (templtimeok)
		{
		//find first line after strt
		i=0;
		val=-1;
		while (trunc(10*val)<trunc(10*start))
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
		//end of finding first and last line
		}
	if ((foundmin==TRUE)&&(foundmax==TRUE))//&&(foundbrk==TRUE))
			{
			CSpike6App::SpikeParams.minlin=minlin;
			CSpike6App::SpikeParams.maxlin=maxlin;
			CSpike6App::SpikeParams.brklin=brklin;
			return(TRUE);
			}
	else
			{
			*SError=107;
			return(FALSE);
			}
		}

	
///-------------------------
//	FindOccurs 
//  Utility for showing the waveforms in .BIN file. Finds locations of spikes in file
///-------------------------
void FindOccurs()
{
UInt32 i;
SInt32 templ,templ1,templ2,val;
templ1=CSpike6App::SpikeParams.templ1;
templ2=CSpike6App::SpikeParams.templ2;
SInt32 max=0, min=0;
CSpike6App::templrecrd theTRec;
if(templ1==templ2)
	{
	for (i=CSpike6App::SpikeParams.minlin; i<=CSpike6App::SpikeParams.maxlin; i++)
		{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;//is it the target spike?
		if (templ==templ1)
			{
			CSpike6App::sWavs->AddItem(&i,sizeof(SInt32));
			val=theTRec.plusval;
			if (val>max){max=val;}
			val=theTRec.minval;
			if (val<min){min=val;}
			}
		}
	}
else //if templ1 ­ templ2
	{
	for (i=CSpike6App::SpikeParams.minlin; i<=CSpike6App::SpikeParams.maxlin; i++)
		{
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		templ=theTRec.templatenum;//is it the target spike?
		if ((templ==templ1)||(templ==templ2))
			{
			CSpike6App::sWavs->AddItem(&i,sizeof(SInt32));
			val=theTRec.plusval;
			if (val>max){max=val;}
			val=theTRec.minval;
			if (val<min){min=val;}
			}
		}
	}
CSpike6App::sMaxPlus=max;
CSpike6App::sMaxMin=min;
}


// ---------------------------------------------------------------------------------
//     * MakeAverages
// ---------------------------------------------------------------------------------

void  MakeRawStats()
//makes number of occurrences of each template&average&stdev&minimum intervals}
{	
	SInt32 i,j, noccur, vall;
	SInt32 ttemp, temp;
	Float64 timeval;
	//String(255) ThermTitle;
	delete CSpike6App::sAvgCol1;
	CSpike6App::sAvgCol1=nil;
	delete CSpike6App::sAvgCol2;
	CSpike6App::sAvgCol2=nil;
	delete CSpike6App::sAvgCol3;
	CSpike6App::sAvgCol3=nil;
	delete CSpike6App::sAvgCol4;
	CSpike6App::sAvgCol4=nil;
	CSpike6App::sAvgCol1 = new LArray(sizeof(SInt32));	//template #
	ThrowIfNil_(CSpike6App::sAvgCol1);
	CSpike6App::sAvgCol2 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol2);					//occurrences
	CSpike6App::sAvgCol3 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol3);					//plussize
	CSpike6App::sAvgCol4 = new LArray(sizeof(SInt32));
	ThrowIfNil_(CSpike6App::sAvgCol4);					//minsize
	
	j=0;		
	for (i=0; i<=600; i++){//maximum number of templates is 601!
		CSpike6App::sAvgCol1->AddItem(&i,sizeof(SInt32));//template number (0-127) i.e. 128 templates
		CSpike6App::sAvgCol2->AddItem(&j,sizeof(SInt32));//#occurrences
		CSpike6App::sAvgCol3->AddItem(&j,sizeof(SInt32));//average plusval
		CSpike6App::sAvgCol4->AddItem(&j,sizeof(SInt32));//average minval
	}			
	CSpike6App::templrecrd theTRec;
	//look for highest template number this yields AvDim3
	noccur=CSpike6App::tRecord->GetCount();
	for (i=1; i<=noccur; i++){//accumulate the totals
		CSpike6App::tRecord->FetchItemAt(i,&theTRec);
		timeval=theTRec.timestamp;
		CSpike6App::sAvgCol2->FetchItemAt(theTRec.templatenum+1,&vall);
		vall+=1;
		CSpike6App::sAvgCol2->AssignItemsAt(1,theTRec.templatenum+1,&vall);
		CSpike6App::sAvgCol3->FetchItemAt(theTRec.templatenum+1,&vall);
		vall+=theTRec.plusval;
		CSpike6App::sAvgCol3->AssignItemsAt(1,theTRec.templatenum+1,&vall);
		CSpike6App::sAvgCol4->FetchItemAt(theTRec.templatenum+1,&vall);
		vall+=theTRec.minval;
		CSpike6App::sAvgCol4->AssignItemsAt(1,theTRec.templatenum+1,&vall);
	}
	ttemp=CSpike6App::sAvgCol1->GetCount();
	for(i=1;i<=ttemp; i++){//make avere of plus and min totals (col3 and 4)
		CSpike6App::sAvgCol2->FetchItemAt(i,&temp);
		CSpike6App::sAvgCol3->FetchItemAt(i,&vall);
		vall=round((Float64)vall/(Float64)temp);
		CSpike6App::sAvgCol3->AssignItemsAt(1,i,&vall);
		CSpike6App::sAvgCol4->FetchItemAt(i,&vall);
		vall=round((Float64)vall/(Float64)temp);
		CSpike6App::sAvgCol4->AssignItemsAt(1,i,&vall);
	}
	CSpike6App::sAvDim2=4;
	CSpike6App::sAvDim3=CSpike6App::sAvgCol1->GetCount();
	i = 601;
	bool going = true;
	while (going){
		CSpike6App::sAvgCol2->FetchItemAt(i, &temp);//get the number of occurrences for this template
		if (temp != 0){
			CSpike6App::snTemplates = i;
			going = false;
		}
		i -= 1;
	}
	CSpike6App::snTemplates = i + 1;
}

///-------------------------
//	ByteSwapping
///-------------------------

Float64 SwapIntelIEEEFloat64(UInt64 fixMe)
	{
	Float64 result;
	switch(fixMe)
		{case 0:
			result = 0.0;
			break;
			
		case 0x803f:
			result = 1.0;
			break;
			
		default:
			{
			union 
				{
				UInt64 hex;
				Float64 myfloat;
				}
			Converter;
			Converter.hex = EndianS64_LtoB(fixMe);
			result=Converter.myfloat;
			}
		break;
	}
	return result;
}

///-------------------------
//	SortFloat64Array
///-------------------------
void SortFloat64Array(LArray* inArray){
	Handle theHandle = inArray->GetItemsHandle();
	size_t arsize = inArray->GetCount();
	char *theChar = *theHandle;
	Float64* myArr = (Float64 *)theChar;
	sortFloat64(myArr, arsize);
}

// ---------------------------------------------------------------------------
//	WritePlotData
// ---------------------------------------------------------------------------
//	Writes Drawcol1 and drawcol 2 into ASCII file for plotting
Ptr
WritePlotData(SInt32 *dLength, UInt8 myFlag){
	
	Ptr myptr=NewPtrClear(100000000);
	char *cstr;
	LStr255 mStr;
	UInt32 length;
	SInt32 i, j, k, ll, maxline, ntemplates = CSpike6App::snTemplates;
    Float64 myval, yourval, ourval, oldval, vall, binw=CSpike6App::SpikeParams.BWdth/1000;
	SInt32 nn,binnum,oldbinnum, bincount=0,sourcecount=0,emptycount;
	LArray *sumarr = new LArray(sizeof(Float64));
	LArray *sumsumarr = new LArray(sizeof(Float64));
    LArray *emptybincounts = new LArray(sizeof(SInt32));
    LArray *prearr=new LArray(sizeof(SInt32));
    LArray *timarr=new LArray(sizeof(Float64));
    LArray *postarr=new LArray(sizeof(SInt32));
	static char tabchar[]="\t";
	static char returnchar[]="\n";
	Float64 floatval;
	SInt32 myvar;
	LArray *theArr1;
	LArray *theArr2;
	if (myFlag==1){//if we ask to write the histogram etc data
		theArr1=CSettingsWindow::DrawCol1;
		theArr2=CSettingsWindow::DrawCol2;
		maxline=CSettingsWindow::DrawCol1->GetCount();
	}
	else if (myFlag==2){//write the rasterplot data
		theArr1=CSettingsWindow::DrawCol3;
		theArr2=CSettingsWindow::DrawCol4;
		maxline=CSettingsWindow::DrawCol3->GetCount();
	}
	else if (myFlag==5){//write the matrix data
		theArr1=CSettingsWindow::DrawCol5;
		theArr2=nil;
		maxline=CSettingsWindow::DrawCol5->GetCount();
		ll=0;
	}
	else if (myFlag==6){//write the matrix data
		theArr1=CSettingsWindow::DrawCol6;
		theArr2=nil;
		maxline=CSettingsWindow::DrawCol6->GetCount();
		ll=0;
	}
	else if (myFlag==7){//write the matrix data
		theArr1=CSettingsWindow::DrawCol7;
		theArr2=nil;
		maxline=CSettingsWindow::DrawCol7->GetCount();
		ll=0;
	}
	else if (myFlag==8){//write the matrix data
		theArr1=CSettingsWindow::DrawCol8;
		theArr2=nil;
		maxline=CSettingsWindow::DrawCol8->GetCount();
		ll=0;
	}
	else if (myFlag==9){//write the matrix data
		theArr1=CSettingsWindow::DrawCol9;
		theArr2=nil;
		maxline=CSettingsWindow::DrawCol9->GetCount();
		ll=0;
	}
    else if (myFlag == 10){
        SInt32 binzero=0;
		LArray *theArr3=CSettingsWindow::DrawCol3;
		LArray *theArr4=CSettingsWindow::DrawCol4;
		maxline=CSettingsWindow::DrawCol3->GetCount();
		//--------------
		//make sd data
		nn = 2*CSpike6App::SpikeParams.maxbns;
		vall=0.0;
		for (k=1;k<=nn;k++){
			sumarr->AddItem(&vall);
			sumsumarr->AddItem(&vall);
		}
		theArr4->FetchItemAt(1, &oldval); //source spike times
		theArr3->FetchItemAt(1, &yourval);//target spike times
		oldbinnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw);
		bincount=0;
		for (k=1; k <= maxline; k++){
			theArr4->FetchItemAt(k, &myval);//gets the time of the source spike
			if (myval != oldval){
				sourcecount+=1;
				oldval=myval;
			}
			theArr3->FetchItemAt(k, &yourval);//gets the dt of a target spike
            if(yourval<0){
                binnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw);
                binzero=0;
            } else {
                binnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw)+1;
                binzero=0;
            }
			if (binnum == oldbinnum){//when more than one spike is in this bin
				bincount+=1;
			} else {//add the bincount to sumarr and the squared bincount into sumsumarr
                //once i am here, I have found a spike, so binzero=0
				sumarr->FetchItemAt(oldbinnum, &ourval);
				ourval += bincount;
				sumarr->AssignItemsAt(1,oldbinnum,&ourval);
				sumsumarr->FetchItemAt(oldbinnum, &ourval);
				ourval += bincount*bincount;
				sumsumarr->AssignItemsAt(1,oldbinnum,&ourval);
				bincount=1;
                binzero=1;
				oldbinnum=binnum;
			}
		}
		//delete(theArr1);
		//theArr1=nil;
		//delete(theArr2);
		//theArr2=nil;
		theArr1=new LArray(sizeof(Float64));
		theArr2=new LArray(sizeof(Float64));
		CSpike6App::sAvgCol2->FetchItemAt(CSpike6App::SpikeParams.templ1+1,&sourcecount);
		for (k=1;k<=nn;k++){
			sumarr->FetchItemAt(k,&myval);
			vall=myval/sourcecount;//average
			theArr1->AddItem(&vall);
			myval=vall*myval;//(·x)^2/N
			sumsumarr->FetchItemAt(k,&yourval);
			if (sourcecount != 0){
				ourval=sqrt((yourval-myval)/(sourcecount-1));
			} else {
				ourval=0.0;
			}
			theArr2->AddItem(&ourval);
		}
		maxline=theArr1->GetCount();
		//ll=0;
	}//end of when flag=10
    else if (myFlag==11){
        SInt32 postbincount=0.0;
        Float64 prebincount=0.0;
        LArray *theArr3=CSettingsWindow::DrawCol3;
		LArray *theArr4=CSettingsWindow::DrawCol4;
		maxline=CSettingsWindow::DrawCol3->GetCount();
        nn = 2*CSpike6App::SpikeParams.maxbns;
		vall=0.0;
		/*for (k=1;k<=nn;k++){
			sumarr->AddItem(&vall);
			sumsumarr->AddItem(&vall);
            emptybincounts->AddItem(&vall);
		}*/
		theArr4->FetchItemAt(1, &oldval); //source spike times
        //oldval=0.0;
		theArr3->FetchItemAt(1, &yourval);//target spike times
		//oldbinnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw);
		bincount=0;
        UInt32 prebin=CSpike6App::SpikeParams.maxbns;
        UInt32 postbin=CSpike6App::SpikeParams.maxbns+1;
		for (k=1; k <= maxline-1; k++){
			//gets the time of the source spike
			//if (myval != oldval){
				//sourcecount+=1;
				//oldval=myval;
			//}
			theArr3->FetchItemAt(k, &yourval);//gets the dt of a target spike
            if(yourval<0){
                binnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw);
            } else {
                binnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw)+1;
            }
            if (binnum == prebin){
                prebincount += 1;
            }
            if (binnum == postbin){
                postbincount += 1;
            }
 
            theArr4->FetchItemAt(k+1, &myval);
            if (myval != oldval){
				sumarr->AddItem(&prebincount);
				sumsumarr->AddItem(&oldval);
                emptybincounts->AddItem(&postbincount);
                prebincount=0;
				postbincount=0;
                sourcecount+=1;
				oldval=myval;
				//oldbinnum=binnum;
			}
		}
        //get the very last numbers
        theArr3->FetchItemAt(k, &yourval);//gets the dt of a target spike
        if(yourval<0){
            binnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw);
        } else {
            binnum=CSpike6App::SpikeParams.maxbns+trunc(yourval/binw)+1;
        }
        if (binnum == prebin){
            prebincount += 1;
        }
        if (binnum == postbin){
            postbincount += 1;
        }
        sumarr->AddItem(&prebincount);
        sumsumarr->AddItem(&oldval);
        emptybincounts->AddItem(&postbincount);
        CSpike6App::templrecrd theTRec;
        j=1;
        nn=CSpike6App::tRecord->GetCount();
        //CSpike6App::SpikeParams.templ1;
        for (i = 1; i <= nn; i++){
            CSpike6App::tRecord->FetchItemAt(i,&theTRec);
            if (theTRec.templatenum==CSpike6App::SpikeParams.templ1){
                sumsumarr->FetchItemAt(j,&oldval);
                if (fabs(oldval-theTRec.timestamp)<0.0001){
                    sumarr->FetchItemAt(j,&myval);
                    postbincount=round(myval);
                    prearr->AddItem(&postbincount);//SInt32
                    timarr->AddItem(&oldval);//Float64
                    emptybincounts->FetchItemAt(j,&prebincount);//SInt32
                    postarr->AddItem(&prebincount);//SInt32
                    j+=1;//we are at the next occurrence
                } else {
                    prebincount=0;
                    prearr->AddItem(&prebincount);//SInt32
                    postarr->AddItem(&prebincount);//SInt32
                    oldval=theTRec.timestamp;
                    timarr->AddItem(&oldval);//Float64
                }
            }
        }
    }
   // prearr->FetchItemAt(361,&prebincount);
	j=0;
    if(myFlag==11){
        nn=timarr->GetCount();
        for(k=1;k<=nn;k++){
            prearr->FetchItemAt(k,&emptycount);
            mStr.Assign(emptycount,"\p#######");
            cstr= mStr.PToCStr(mStr);
            cstr=strcat(cstr,tabchar);
            ll+=1;
            length=strlen(const_cast <char *>(cstr));
            for (i=0;i<=length;i++){
                myptr[j]=cstr[i];
                j+=1;
                if (j>=10000000){
                    myvar=1;
                }
            }
            j-=1;
            timarr->FetchItemAt(k,&myval);
            mStr.Assign(myval,"\p#######.####");
            cstr= mStr.PToCStr(mStr);
            cstr=strcat(cstr,tabchar);
            ll+=1;
            length=strlen(const_cast <char *>(cstr));
            for (i=0;i<=length;i++){
                myptr[j]=cstr[i];
                j+=1;
                if (j>=10000000){
                    myvar=1;
                }
            }
            j-=1;
            postarr->FetchItemAt(k,&emptycount);
            mStr.Assign(emptycount,"\p#######");
            cstr= mStr.PToCStr(mStr);
			cstr=strcat(cstr,returnchar);
			length=strlen(const_cast <char *>(cstr));
			for (i=0;i<=length;i++){
				myptr[j]=cstr[i];
				j+=1;
				if (j>=10000000){
					myvar=1;
				}
			}
			j-=1;
        }    
    } 
    else {
	for(k=1;k<=maxline;k++){
		theArr1->FetchItemAt(k,&floatval);
		mStr.Assign(floatval,"\p#######.#########");
		//mptr=mStr.operator StringPtr();
		cstr= mStr.PToCStr(mStr);
		if((myFlag>=3)&&(myFlag<10)){
			if(ll==ntemplates-1){
				cstr=strcat(cstr,returnchar);
				ll=0;
			} else {
				cstr=strcat(cstr,tabchar);
				ll+=1;
			}
		} else {
			cstr=strcat(cstr,tabchar);
		}
		length=strlen(const_cast <char *>(cstr));
		for (i=0;i<=length;i++){
			myptr[j]=cstr[i];
			j+=1;
			if (j>=10000000){
				myvar=1;
			}
		}
		j-=1;
		if((myFlag == 1)||(myFlag == 2)||(myFlag==10)){
			theArr2->FetchItemAt(k,&floatval);
			mStr.Assign(floatval,"\p#######.#########");
			cstr= mStr.PToCStr(mStr);
            cstr=strcat(cstr,returnchar);
			length=strlen(const_cast <char *>(cstr));
			for (i=0;i<=length;i++){
				myptr[j]=cstr[i];
				j+=1;
				if (j>=10000000){
					myvar=1;
				}
			}
			j-=1;
		}
	}
    }
    if((myFlag==10)||(myFlag==11)){
        delete(sumarr);
        sumarr=nil;
        delete(sumsumarr);
        sumsumarr=nil;
        delete(emptybincounts);
        emptybincounts=nil;
    }
    if(myFlag==11){
        delete(prearr);
        prearr=nil;
        delete(postarr);
        postarr=nil;
        delete(timarr);
        timarr=nil;
    }
	*dLength=j; 
	return myptr;
}
