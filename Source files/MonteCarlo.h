//#include "UltraU.h"
//#include "URandomLib.h"
//#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

//void InitRand();
void RunSim ();
void RunSimMA();
void RunSimAR();
void RankIntvals();
void MakeKWStat();
Float64 probksm(Float64 alam);
void kstwo();
void DoSimulation ();
void AutoCorr ();
void DoMedian();
void DoPermutation();
void FakeSim();
void DoBreakpoint ();
void MakeAverages ();