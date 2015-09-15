#include "SwapUtils.h"
#include "SpikeConstants.h"
#include "CSpike6App.h"
// ---------------------------------------------------------------------------
//	SwapTRec
// ---------------------------------------------------------------------------
//	if the current machine is little endian, this routine swaps big to little when reading. 
//  and little to big when writing. All spike files are big endian
//  
void 
SwapTRec(LArray *myArr){
	CSpike6App::templrecrd tRec;
	SInt32 i, count=myArr->GetCount();
	SInt16 yy;
	short mymsg;
	Float64 timev;
	LStr255 *mystring;
	CFByteOrder myOrder=CFByteOrderGetCurrent();//gets the byte order
	if (myOrder==CFByteOrderLittleEndian){
		for (i=1; i<=count; i++){
			myArr->FetchItemAt(i,&tRec);
			timev=tRec.timestamp;
			tRec.timestamp=SwapIntelIEEEFloat64A(timev);
			yy=tRec.templatenum;
			tRec.templatenum=CFSwapInt16 (yy);
			yy=tRec.plusval;
			tRec.plusval=CFSwapInt16 (yy);
			yy=tRec.minval;
			tRec.minval=CFSwapInt16 (yy);
			myArr->AssignItemsAt(1, i, &tRec, sizeof(tRec));
			//mystring=new LStr255("\pSwapped");
			//mymsg=DoShowSwapMessage(mystring);
			}
		}
	else if (myOrder==CFByteOrderUnknown){
		mystring=new LStr255("\pByte Order Problem");
		mymsg=DoShowSwapMessage(mystring);
		}
	//if the machine is already big endian (PPC) do nothing
}

// ---------------------------------------------------------------------------
//	SwapWaves
// ---------------------------------------------------------------------------
//	if the current machine is little endian, this routine swaps big to little when reading. 
//  and little to big when writing. All spike files are big endian
//  
void 
SwapWaves(LArray *myArr){
	CSpike6App::waveform wvfrm;
	SInt32 i,j, count=myArr->GetCount();
	SInt16 yy;
	short mymsg;
	Float64 timev;
	LStr255 *mystring;
	CFByteOrder myOrder=CFByteOrderGetCurrent();//gets the byte order
	if (myOrder==CFByteOrderLittleEndian){//if the current machine is little endian swap, if etc!!!!
		for (i=1; i<=count; i++){
			myArr->FetchItemAt(i,&wvfrm);
			timev=wvfrm.timestamp;
			wvfrm.timestamp=SwapIntelIEEEFloat64A(timev);
			for (j=0;j<=63; j++){
				yy=wvfrm.wavform[j];
				wvfrm.wavform[j]=CFSwapInt16 (yy);
				}
			myArr->AssignItemsAt(1, i, &wvfrm, sizeof(wvfrm));
			}
		}
	else if (myOrder==CFByteOrderUnknown){
		mystring=new LStr255("\pByte Order Problem");
		mymsg=DoShowSwapMessage(mystring);
		}
	//if the machine is already big endian (PPC) do nothing
}


					
short 
DoShowSwapMessage(LStr255 *mystring){
	short msg;				
	StDialogHandler	theHandler(rPPob_1btnAlert, nil);//error dialog handler
	LWindow*	theDialog = theHandler.GetDialog();//the error dialog
	Assert_(theDialog != nil);
	theDialog->Show();
	MessageT theMessage;
	LStaticText	*theCaption;
	Rect frame;
	theCaption = dynamic_cast<LStaticText*>(theDialog->FindPaneByID(1));
	theCaption->SetText(*mystring);
	theCaption->FocusDraw();
	theCaption->CalcLocalFrameRect(frame);
	theCaption->ApplyForeAndBackColors();
	::EraseRect(&frame);
	theCaption->Draw(nil);
	do {
		theMessage = theHandler.DoDialog();
	} while ( theMessage != msg_OK && theMessage != msg_Cancel );
	if (theMessage==msg_OK){
		msg=1;
		}
	else if (theMessage==msg_Cancel){
		msg=0;
		}
	return msg;
}

///-------------------------
//	MoreByteSwapping
///-------------------------

Float64 
SwapIntelIEEEFloat64A(Float64 fixMe)
	{
	Float64 result;
	
			union 
				{
				UInt64 hex;
				Float64 myfloat;
				}
			Converter;
			Converter.myfloat=fixMe;
			Converter.hex = EndianS64_LtoB(Converter.hex);
			result=Converter.myfloat;
			/*UInt64 mantist = Converter.fields.mantiss;
			Float64 mantissa = (Float64)(mantist | (1<<52)) / (Float64)(1<<52);
			result = mantissa*exp2(Converter.fields.exponent-1023);
			if (Converter.fields.sign)
				{result = -result;}*/
	return result;
}
///-------------------------
//	FloatFromString
///-------------------------


Float64 
FloatFromString(LStr255 *instring){
	SInt32 whole, part, n;
	LStr255 *strng1, *strng2;
	Float64 myFloat64, decfloat;
	UInt8 ll=8, le, loc, mm;
	strng1=new LStr255(*instring);
	le=strng1->Length();
	loc=strng1->Find(".", 1);
	if (loc==0){//if there is no decimal point make a float out of what we have
		whole=strng1->operator SInt32();
		myFloat64=(Float64)whole;
		}
	else {
		mm=le-loc+1;//length of part behind decimal point including decimal point
		strng1->Remove(loc,mm);
		whole=strng1->operator SInt32();//part before decimal point
		strng2=new LStr255(*instring);
		strng2->Remove(1,loc); 
		part=strng2->operator SInt32();//part after decimal point
		ll=strng2->Length();
		decfloat=10.0;
		for (n=2; n<=ll; n++) {
			decfloat*=10.0;
			}
		decfloat=(Float64)part/decfloat;
		myFloat64=(Float64)whole + decfloat;
		}
	return myFloat64;
}