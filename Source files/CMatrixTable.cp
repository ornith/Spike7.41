// ===========================================================================
//	CMatrixTable.cp					©1998 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "CMatrixTable.h"
#include "CMatrixWindow.h"

#include "CSettingsWindow.h"
#include "CSpike6App.h"

//LArray *theArray = nil;
// ---------------------------------------------------------------------------
//		¥ CMatrixTable
// ---------------------------------------------------------------------------

CMatrixTable::CMatrixTable(
	LStream		*inStream)
		: LTable(inStream)
{
SetCellDataSize(sizeof(SInt32));
SInt32 mynum = 0;
Float64 myval=0;
//now fill the table with data from avgarray
SInt32 i, j, k=1;
TableCellT cellmy;		
LArray *theArray = nil;
	if (CMatrixWindow::typematrix == 1){
		theArray = CSettingsWindow::DrawCol5;
	}else if (CMatrixWindow::typematrix == 2){
		theArray = CSettingsWindow::DrawCol6;
	}else if (CMatrixWindow::typematrix == 3){
		theArray = CSettingsWindow::DrawCol7;
	}else if (CMatrixWindow::typematrix == 4){
		theArray = CSettingsWindow::DrawCol8;
	}else if (CMatrixWindow::typematrix == 5){
		theArray = CSettingsWindow::DrawCol9;
	}
SInt32 rowstoadd, colstoadd,templs=theArray->GetCount();
templs=round(sqrt((Float64)templs));
colstoadd = templs-1;// + 2;
rowstoadd = templs-1;// + 2;
InsertCols(colstoadd,1,&mynum);
InsertRows(rowstoadd,1,&mynum);
for (i=1;i<=mRows;i++)
	{
	for (j = 1; j <= mRows; j++){
		theArray->FetchItemAt(k, &myval);
		cellmy.col = i;
		cellmy.row = j;
		SetCellData(cellmy, &myval);
		k+=1;
		}
	}
	//delete theArray;
	//theArray = nil;
}


// ---------------------------------------------------------------------------
//		¥ ~CMatrixTable
// ---------------------------------------------------------------------------

CMatrixTable::~CMatrixTable()
{
}

// ---------------------------------------------------------------------------
//		¥ DrawCell
// ---------------------------------------------------------------------------

void
CMatrixTable::DrawCell(
	const TableCellT	&inCell)
	{
	SInt32 mynum=100, i;
	Float64 myval=100.0;
	Rect	cellFrame;
	LArray *theArray = nil;
	if (CMatrixWindow::typematrix == 1){
		theArray = CSettingsWindow::DrawCol5;
	}else if (CMatrixWindow::typematrix == 2){
		theArray = CSettingsWindow::DrawCol6;
	}else if (CMatrixWindow::typematrix == 3){
		theArray = CSettingsWindow::DrawCol7;
	}else if (CMatrixWindow::typematrix == 4){
		theArray = CSettingsWindow::DrawCol8;
	}else if (CMatrixWindow::typematrix == 5){
		theArray = CSettingsWindow::DrawCol9;
	}
		
	if (FetchLocalCellFrame(inCell, cellFrame)) 
		{
		::EraseRect(&cellFrame);
		TextFont(applFont);
		TextSize(9);
		MoveTo(cellFrame.left + 4, cellFrame.bottom - 4);
		Str31	str;
		i = (inCell.row - 1) * mCols + inCell.col;

		theArray->FetchItemAt(i, &myval);
		mynum=(SInt32)myval;
		if (mynum == 0){
			NumToString(mynum, str);
			str[1] = '.';
			DrawString(str);
			}
		else {
			NumToString(mynum, str);
			DrawString(str);
			}
		}
	}