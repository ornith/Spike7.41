#include <LArray.h>void AllAllDensity();void AllDensityGSL(SInt16 colstosaveflag);void AllDensity(SInt16 colstosaveflag);void CumPSTH(SInt16 colstosaveflag);void MakeCumPlot ();void MakeAllTemplatesHisto();void MakeIntvHisto();void EventsXTime();void ProbPlot(UInt32 flag);//Boolean FindLines (SInt32 *templ1,SInt32 *templ2, Float64 *strt, Float64 *brkpt, Float64 *stp,//	SInt32 *minln, SInt32 *brkln, SInt32 *maxln);Boolean FindLinesA (SInt32 *SError);void FindOccurs();//void swap_bytes (void *ptr, int len);Float64 SwapIntelIEEEFloat64(UInt64 fixMe);void MakeCumulativeNormal();Float64 NormEstimate(Float64 zzscore);void SortFloat64Array(LArray* inArray);void  MakeRawStats();char *WritePlotData(SInt32 *dLength, UInt8 myFlag);SInt32 ** AllPSTH(SInt16 ntemplates, Float64 binwidth, SInt32 nbins);