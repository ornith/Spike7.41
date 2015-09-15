// =================================================================================
//	CProgressWindow.cp				©1995-1998  Metrowerks Inc. All rights reserved.
// =================================================================================
//	CProgressWindow.h	

#include <LString.h>
#include <UReanimator.h>
#include <UWindows.h>
#include <LArray.h>

#include "SpikeConstants.h"
#include "CSpike6App.h"
#include "CCombEvents.h"
#include "CSettingsWindow.h"
#include "Analyze1.h"
#include "Analyze2.h"

bool AllEvents = TRUE;//if true, all events following the source event are inserted as a new cluster
					//if false only the first one following
bool replacing = TRUE;//if false, the combined unit is added to the file. When TRUE, the new unit replaces the old 
					//source unit
// ---------------------------------------------------------------------------------
//		• CProgressWindow
// ---------------------------------------------------------------------------------

CCombEvents::CCombEvents()
{
}


// ---------------------------------------------------------------------------------
//		• CProgressWindow(const SWindowInfo&)
// ---------------------------------------------------------------------------------

CCombEvents::CCombEvents(
	const SWindowInfo &inWindowInfo )
		: LWindow( inWindowInfo )
{
}


// ---------------------------------------------------------------------------------
//		• CProgressWindow(ResIDT, Uint32, LCommander *)
// ---------------------------------------------------------------------------------

CCombEvents::CCombEvents(
	ResIDT		inWINDid,
	UInt32		inAttributes,
	LCommander	*inSuperCommander )
		: LWindow( inWINDid, inAttributes, inSuperCommander )
{
}


// ---------------------------------------------------------------------------------
//		• CProgressWindow(LStream *)
// ---------------------------------------------------------------------------------

CCombEvents::CCombEvents(
	LStream *inStream )
		: LWindow( inStream)
{
}


// ---------------------------------------------------------------------------------
//		• ~CProgressWindow
// ---------------------------------------------------------------------------------

CCombEvents::~CCombEvents()
{
}


// ---------------------------------------------------------------------------------
//		• FinishCreateSelf
// ---------------------------------------------------------------------------------

void
CCombEvents::FinishCreateSelf()
{
	// Link the window (the listener) with its controls (the broadcasters).
	UReanimator::LinkListenerToControls( this, this, rRidL_Combination );
	AllEvents=TRUE;
	replacing=TRUE;
	//InitRand(); 


}


// ---------------------------------------------------------------------------------
//		• ListenToMessage
// ---------------------------------------------------------------------------------

void
CCombEvents::ListenToMessage( 
	MessageT	inMessage,
	void		*ioParam )
{
#pragma unused( ioParam )
	switch ( inMessage ) {
		case 3503:
			//get values
			{
			Str255 str;
			LStr255 Lstr;
            bool thetruth;
			Float64 mydist=0.0,dif=0.0;
			SInt16  templ1,templ2;
			SInt32  i,j,count=CSpike6App::tRecord->GetCount(), count1;
			CSpike6App::templrecrd rec1, rec2, newrec;
			newrec.templatenum=CSpike6App::snTemplates+1;
			newrec.plusval=10000;
			newrec.minval=-10000;
			LArray *newrecs = new LArray(sizeof(CSpike6App::templrecrd));
			LArray *insertions = new LArray(sizeof(SInt32));
			
			GetDescriptorForPaneID(1, str);
			CFStringRef myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
			templ1=CFStringGetIntValue(myString);
			GetDescriptorForPaneID(2, str);
			myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
			templ2=CFStringGetIntValue(myString);//get time
			GetDescriptorForPaneID(3, str);
			myString=CFStringCreateWithPascalString(kCFAllocatorDefault, str, CFStringGetSystemEncoding());
			mydist=CFStringGetDoubleValue(myString);
			mydist=mydist/1000.0;
			thetruth=AllEvents;
			//AllEvents=FALSE;
			if (!AllEvents){
				for (i=1; i<=count-1;i++){
					CSpike6App::tRecord->FetchItemAt(i,&rec1);
					if(rec1.templatenum == templ1){//the source cluster
                        if(mydist>0){
                            for (j=i+1;j<=count;j++){
                                CSpike6App::tRecord->FetchItemAt(j,&rec2);
                                if (rec2.templatenum == templ2){//the target cluster
                                    dif=-rec1.timestamp;//to start getting the difference
                                    dif+=rec2.timestamp;//the difference
                                    if (dif <= mydist) {
                                        newrec.timestamp=rec1.timestamp;
                                        newrecs->AddItem(&newrec,sizeof(CSpike6App::templrecrd));
                                        insertions->AddItem(&i);
                                    }
								break;
                                }
							}
						} else {//if mydist <= 0
                            for (j=i-1;j>0;j--){
                                CSpike6App::tRecord->FetchItemAt(j,&rec2);
                                if (rec2.templatenum == templ2){//the target cluster
                                    dif=-rec1.timestamp;//to get the difference
                                    dif+=rec2.timestamp;//the real difference
                                    if (dif >= mydist) {
                                        newrec.timestamp=rec1.timestamp;
                                        newrecs->AddItem(&newrec,sizeof(CSpike6App::templrecrd));
                                        insertions->AddItem(&i);
                                    }
                                    break;
                                }
							}
                        }
					}
				}
			} else {
                if (mydist >= 0){//we record those events: templ1 that are followed by at least one other event (templ2: answer) within the specified time
                    for (i=1; i<=count-1;i++){
                        CSpike6App::tRecord->FetchItemAt(i,&rec1);
                        if(rec1.templatenum == templ1){
                            for (j=i+1;j<=count;j++){
                                CSpike6App::tRecord->FetchItemAt(j,&rec2);
                                if (rec2.templatenum == templ2){
                                    dif=-rec1.timestamp;//to get the difference
                                    dif+=rec2.timestamp;//the real difference
                                    if (dif <= mydist) {
                                        newrec.timestamp=rec1.timestamp;
                                        newrecs->AddItem(&newrec,sizeof(CSpike6App::templrecrd));
                                        insertions->AddItem(&i);
                                    } else {
                                        break;
                                    }
								}
							}
						}
					}
				} else { //if mydist is smaller than 0, we select cases of templ1 that have been preceded by templ2
                    for (i=2; i<=count-1;i++){
                        CSpike6App::tRecord->FetchItemAt(i,&rec1);
                        if(rec1.templatenum == templ1){
                            for (j=i-1;j>0;j--){
                                CSpike6App::tRecord->FetchItemAt(j,&rec2);
                                dif=rec2.timestamp-rec1.timestamp;//to get the difference
                                //dif+=rec1.timestamp;//the real difference
                                if (dif < mydist){
                                    break;
                                }
                                if (rec2.templatenum == templ2){

                                    if (dif >= mydist) {
                                        newrec.timestamp=rec1.timestamp;
                                        newrecs->AddItem(&newrec,sizeof(CSpike6App::templrecrd));
                                        insertions->AddItem(&i);
                                    } else {
                                        break;
                                    }
								}
							}
						}
					}
                }
			}
		
			count1=insertions->GetCount();//insertions should be just as large as newrecs
			if (replacing){
				for(i=count1; i>=1; i--){
					newrecs->FetchItemAt(i,&newrec);
					insertions->FetchItemAt(i,&j);//get the index where the record should be inserted
					CSpike6App::tRecord->AssignItemsAt(1,j,&newrec);
					}
			}
			else {
				for(i=count1; i>=1; i--){
					newrecs->FetchItemAt(i,&newrec);
					insertions->FetchItemAt(i,&j);//get the index where the record should be inserted
					CSpike6App::tRecord->InsertItemsAt(1,j,&newrec);
				}
			}
			count=CSpike6App::tRecord->GetCount();
			MakeRawStats();
			CSettingsWindow *theSettWindow = dynamic_cast<CSettingsWindow *>(LWindow::FindWindowByID(rPPob_SettingWindow));
			if (theSettWindow !=nil)
			{
				delete theSettWindow;
				theSettWindow=nil;
			}
			theSettWindow =dynamic_cast<CSettingsWindow *>(LWindow::CreateWindow( rPPob_SettingWindow, this ));
			delete newrecs;
			delete insertions;
			newrecs = NULL;
			insertions = NULL;
			
			//MakeGenStats();
			//CSpike6App::sdim2=2;
			//CSpike6App::sdim3=CSpike6App::tRecord->GetCount();
			}
			break;
			
		case 3505:
		{
			if (AllEvents){
				AllEvents=FALSE;
			} else {
				AllEvents=TRUE;
			}
		}
		break;
			
		case 3506:
		{
			if (replacing){
				replacing=FALSE;
			} else {
				replacing=TRUE;
			}
		}
			break;
		}
}
