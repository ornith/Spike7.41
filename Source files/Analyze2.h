#pragma once

#include <LArray.h>
#include <gsl/gsl_rng.h>
#include "CSpike6App.h"
//void InitRand();
void MakeIntvArray();//used by KW,KS,Median test, permutation test
void MakeIntvArray2();//used by return plots
void MakeIntvArray3();//for permutation test
void RankIntervals(Float64 *Array1, Float64 *Array2);

void RunBasicStats(Float64 *avpt, Float64 *sdevpt,  Float64 *avpt2, Float64 *sdevpt2,
		SInt32 *intvls1, SInt32 *intvls2);
void RunBasicStatsNew(Float64 *avpt, Float64 *sdevpt, Float64 *avpt2, Float64 *sdevpt2, 
					  Float64 *avpt3, Float64 *sdevpt3,
					  SInt32 *intvls1, SInt32 *intvls2, SInt32 *intvls3);
void DoKruskal(Float64 *Array1, Float64 *Array2);
void DoMedianTest(Float64 *Array1, Float64 *Array2);
void DoKolmogorov(Float64 *Array1, Float64 *Array2);

void RunKWStat();

void RunStatPermutation();
void DoStatPermutation();

CSpike6App::BinRecord PermuteList(UInt32 SoMany);

Float64 probks(Float64 alam);

void RunKSStat();
Float64 RunKSStatProb();
void RunBreakpoint();

void RunACF();
void RunPACF();
Float64 RunPHI();
Float64 RunTheta();
void ReconstructAR();
void ReconstructMA();

void RunReturn();
void RunRankedReturn();
void DoKSReturn();

void Run3iReturn();
void RunR3iReturn();

void RunCumPSTH();
void RunPSTH();
void RunCumPSTH3();

void InitCumPSTH();
void shufflespiketrain();
void DoCumPSTHnew();
void FinishCumPSTH();
void FinishHistogram();

Boolean FindLinesB (SInt32 *SError);

void sortFloat64(double * indat, size_t arsize);
UInt32 GetIndex (Float64 *inarray, Float64 val, size_t arsize);
Float64 GetRank (Float64 *inarray, Float64 val, size_t arsize);

void  MakeGenStats();