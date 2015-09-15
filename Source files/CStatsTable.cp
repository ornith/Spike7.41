// ===========================================================================//	CAvgTable.cp					�1998 Metrowerks Inc. All rights reserved.// ===========================================================================#include "CStatsTable.h"#include "CSpike6App.h"#include "CStatsWindow.h"// ---------------------------------------------------------------------------//		� CStatsTable// ---------------------------------------------------------------------------CStatsTable::CStatsTable(	LStream		*inStream)		: LTable(inStream){SetCellDataSize(sizeof(Float64));Float64 mynum=0.0;//now fill the table with data from avgarraySInt32 i;TableCellT cellmy;		//CDatDocument::sAvgCol2->FetchItemAt(1, &mynum);SInt32 rowstoadd, colstoadd;colstoadd = 2;rowstoadd = CStatsWindow::maxorder-1; //10th order is highestInsertCols(colstoadd,1,&mynum);InsertRows(rowstoadd,1,&mynum);for (i=1;i<=mRows;i++)	{	cellmy.col = i;	cellmy.row = 1;	SetCellData(cellmy, &i);	CSpike6App::sAvgCol2->FetchItemAt(i, &mynum);	cellmy.row = 2;	SetCellData(cellmy, &mynum);	}}// ---------------------------------------------------------------------------//		� ~CStatsTable// ---------------------------------------------------------------------------CStatsTable::~CStatsTable(){}//     *ClickSelf// ---------------------------------------------------------------------------//		� DrawCell// ---------------------------------------------------------------------------voidCStatsTable::DrawCell(	const TableCellT	&inCell)	{	Float64 mynum=0;	SInt32 i=0;	Rect	cellFrame;	if (FetchLocalCellFrame(inCell, cellFrame)) 		{		TextFont(applFont);		TextSize(9);		MoveTo(cellFrame.left + 4, cellFrame.bottom - 4);		Str31	str;		PP_PowerPlant::LStr255	theTitle;		if (inCell.col==1)			{			for (i=1; i<=CStatsWindow::maxorder; i++)				{				if (inCell.row==i)					{					NumToString(i, str);					DrawString(str);					}				}			}		else if (inCell.col==2)			{			for (i=1; i<=CStatsWindow::maxorder; i++)				{				if (inCell.row==i)					{					CStatsWindow::sACFray->FetchItemAt(i, &mynum);					theTitle.Assign(mynum, "\p####.###");					DrawString(theTitle);					}				}			}		else if (inCell.col==3)			{			for (i=1; i<=CStatsWindow::maxorder; i++)				{				if (inCell.row==i)					{					CStatsWindow::sPACFray->FetchItemAt(i, &mynum);					theTitle.Assign(mynum, "\p####.###");					DrawString(theTitle);					}				}			}		}	}	