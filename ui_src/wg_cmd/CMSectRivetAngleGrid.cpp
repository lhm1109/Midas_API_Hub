// CMSectRivetAngleGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectRivetAngleGrid.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#define D_COL_NUM  6

CCMSectRivetAngleGrid::CCMSectRivetAngleGrid()
{
	m_bSymmetric = FALSE;
}

CCMSectRivetAngleGrid::~CCMSectRivetAngleGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMSectRivetAngleGrid::SetData2Grid(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos, BOOL bSymmetric, CArray<int, int>& aAngleIndex)
{
	BOOL bOldLock = LockUpdate(TRUE);

	m_bSymmetric = bSymmetric;

	T_SECT_RIVET_POS_D RivetPos;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D> aRivetSort;

	std::map<int, int> mapUseAngle;

	for (int i = 0; i < aRivetPos.GetSize(); i++)
	{
		//aRivetSort.Add(aRivetPos[i]);
		mapUseAngle[aRivetPos[i].nAngle] = aRivetPos[i].nAngle;
	}

	for (int i = 0; i < aAngleIndex.GetSize(); i++)
	{
		if (mapUseAngle.find(aAngleIndex[i]) == mapUseAngle.end())
		{
			RivetPos.Initialize();

			if (aAngleIndex[i] == 0)
			{
				RivetPos.nComponent = RIVET_TL;
				RivetPos.nAngle = aAngleIndex[i];
				RivetPos.nRefPos = 1;
				aRivetSort.Add(RivetPos);

				RivetPos.nComponent = RIVET_WEB;
				RivetPos.nAngle = aAngleIndex[i];
				RivetPos.nRefPos = 0;
				aRivetSort.Add(RivetPos);
			}
			else if (aAngleIndex[i] == 1)
			{
				RivetPos.nComponent = RIVET_BL;
				RivetPos.nAngle = aAngleIndex[i];
				RivetPos.nRefPos = 1;
				aRivetSort.Add(RivetPos);

				RivetPos.nComponent = RIVET_WEB;
				RivetPos.nAngle = aAngleIndex[i];
				RivetPos.nRefPos = 1;
				aRivetSort.Add(RivetPos);
			}
			else if (aAngleIndex[i] == 2)
			{
				RivetPos.nComponent = RIVET_TL;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);
			}
			else if (aAngleIndex[i] == 3)
			{
				RivetPos.nComponent = RIVET_BL;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);
			}
			else if (aAngleIndex[i] == 4)
			{
				RivetPos.nComponent = RIVET_TR;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);

				RivetPos.nComponent = RIVET_WEB;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);
			}
			else if (aAngleIndex[i] == 5)
			{
				RivetPos.nComponent = RIVET_BR;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);

				RivetPos.nComponent = RIVET_WEB;
				RivetPos.nRefPos = 1;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);
			}
			else if (aAngleIndex[i] == 6)
			{
				RivetPos.nComponent = RIVET_TR;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);
			}
			else if (aAngleIndex[i] == 7)
			{
				RivetPos.nComponent = RIVET_BR;
				RivetPos.nAngle = aAngleIndex[i];
				aRivetSort.Add(RivetPos);
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			for (int j = 0; j < aRivetPos.GetSize(); j++)
			{
				if (aAngleIndex[i] == aRivetPos[j].nAngle)
				{
					aRivetSort.Add(aRivetPos[j]);
				}
			}
		}
	}

	SortRivetArray(aRivetSort);

	std::map<int, T_SECT_RIVET_POS_D> mapUseRevet;
	std::map<int, T_SECT_RIVET_POS_D> mapUseRevetW;
	for (int i = 0; i < aRivetSort.GetSize(); i++)
	{
		if (aRivetSort[i].nComponent == RIVET_WEB)
			mapUseRevetW[aRivetSort[i].nAngle] = aRivetSort[i];
		else
			mapUseRevet[aRivetSort[i].nAngle] = aRivetSort[i];
	}

	int nNum = aRivetSort.GetSize();

	SetRowCount(nNum);

	m_mapRowdata.RemoveAll();
	m_mapRowdata.InitHashTable(11);
	for (int i = 0; i < nNum; i++)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		RivetPos = aRivetSort[i];

		BOOL bRightPart = FALSE;
		DataToString(bSymmetric, mapUseRevet, mapUseRevetW, RivetPos, bRightPart, aValue);
		
		BOOL bSymRow = (bSymmetric && bRightPart)? TRUE : FALSE;
		if (RivetPos.nComponent == RIVET_WEB && bRightPart) bSymRow = TRUE;
		SetDataToRow(i + 1, bSymRow, aValue);

		m_mapRowdata.SetAt(i+1, RivetPos);
	}

	SetRowHeight(1, nNum, globalUtils.ScaleByDPI(18));

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

BOOL CCMSectRivetAngleGrid::GetGrid2Data(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos)
{
	int nCount = GetRowCount();
	if (nCount == 0)
	{
		aRivetPos.RemoveAll();
		return TRUE;
	}

	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);
	
	aRivetPos.RemoveAll();

	T_SECT_RIVET_POS_D RivetPos;

	for (int i = 0; i < nCount; ++i)
	{
		if (!GetRowToData(i + 1, aValue)) return FALSE;

		StringToData(aValue, RivetPos);

		aRivetPos.Add(RivetPos);
	}
	return TRUE;
}

void CCMSectRivetAngleGrid::SetDataToRow(ROWCOL nRow, BOOL bSymRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i = 0; i < nColCount; ++i)
	{
		SetValueRange(CGXRange(nRow, i + 1), raValue[i]);

		if (i == 2 || i == 3)
		{
			if (bSymRow)
			{
				SetStyleRange(CGXRange(nRow, i + 1), CGXStyle().SetEnabled(FALSE));
			}
			else
			{
				if (raValue[i] == _T(""))
					SetStyleRange(CGXRange(nRow, i + 1), CGXStyle().SetEnabled(FALSE));
				else
					SetStyleRange(CGXRange(nRow, i + 1), CGXStyle().SetEnabled(TRUE));
			}
		}

		if (i == 4 || i == 5)
		{
			if (bSymRow)
			{
				SetStyleRange(CGXRange(nRow, i + 1), CGXStyle().SetEnabled(FALSE));
			}
			else
			{
				SetStyleRange(CGXRange(nRow, i + 1), CGXStyle().SetEnabled(TRUE));
			}
		}
	}
}

BOOL CCMSectRivetAngleGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i = 0; i < nColCount; ++i)
		raValue[i] = GetEditingValue(nRow, i + 1);

	return TRUE;
}

void CCMSectRivetAngleGrid::StringToData(IN CStringArray& aValue, OUT T_SECT_RIVET_POS_D& RivetPos)
{
	RivetPos.Initialize();

	if      (aValue[0] == _T("Top Left Flange"))     RivetPos.nComponent = RIVET_TL;
	else if (aValue[0] == _T("Top Right Flange"))    RivetPos.nComponent = RIVET_TR;
	else if (aValue[0] == _T("Web"))                 RivetPos.nComponent = RIVET_WEB;
	else if (aValue[0] == _T("Bottom Left Flange"))  RivetPos.nComponent = RIVET_BL;
	else if (aValue[0] == _T("Bottom Right Flange")) RivetPos.nComponent = RIVET_BR;
	else                                             RivetPos.nComponent = RIVET_TL;

	if      (aValue[1] == _T("Angle 1")) RivetPos.nAngle = 0;
	else if (aValue[1] == _T("Angle 2")) RivetPos.nAngle = 1;
	else if (aValue[1] == _T("Angle 3")) RivetPos.nAngle = 2;
	else if (aValue[1] == _T("Angle 4")) RivetPos.nAngle = 3;
	else if (aValue[1] == _T("Angle 5")) RivetPos.nAngle = 4;
	else if (aValue[1] == _T("Angle 6")) RivetPos.nAngle = 5;
	else if (aValue[1] == _T("Angle 7")) RivetPos.nAngle = 6;
	else if (aValue[1] == _T("Angle 8")) RivetPos.nAngle = 7;
	else RivetPos.nAngle = 0;

	if (RivetPos.nComponent == RIVET_TL || RivetPos.nComponent == RIVET_TR ||
		RivetPos.nComponent == RIVET_BL || RivetPos.nComponent == RIVET_BR)
	{
		if      (aValue[2] == _T("Left"))  RivetPos.nRefPos = 0;
		else if (aValue[2] == _T("Right")) RivetPos.nRefPos = 1;
		else RivetPos.nRefPos = 0;
	}
	else if (RivetPos.nComponent == RIVET_WEB)
	{
		if      (aValue[3] == _T("Top"))    RivetPos.nRefPos = 0;
		else if (aValue[3] == _T("Bottom")) RivetPos.nRefPos = 1;
		else RivetPos.nRefPos = 0;
	}
	else
	{
		RivetPos.nRefPos = 0;
	}

	RivetPos.dDist = _tstof(aValue[4]);
	RivetPos.dDia = _tstof(aValue[5]);
}

void CCMSectRivetAngleGrid::DataToString(BOOL bSymmetric, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevet, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevetW, IN T_SECT_RIVET_POS_D& RivetPos, 
										 BOOL& bRightPart, OUT CStringArray& aValue)
{
	if      (RivetPos.nComponent == RIVET_TL ) aValue[0] = _T("Top Left Flange");
	else if (RivetPos.nComponent == RIVET_TR ) aValue[0] = _T("Top Right Flange");
	else if (RivetPos.nComponent == RIVET_WEB) aValue[0] = _T("Web");
	else if (RivetPos.nComponent == RIVET_BL ) aValue[0] = _T("Bottom Left Flange");
	else if (RivetPos.nComponent == RIVET_BR ) aValue[0] = _T("Bottom Right Flange");
	else                                       aValue[0] = _T("");

	if      (RivetPos.nAngle == 0) { aValue[1] = _T("Angle 1");}
	else if (RivetPos.nAngle == 1) { aValue[1] = _T("Angle 2");}
	else if (RivetPos.nAngle == 2) { aValue[1] = _T("Angle 3");}
	else if (RivetPos.nAngle == 3) { aValue[1] = _T("Angle 4");}
	else if (RivetPos.nAngle == 4) { aValue[1] = _T("Angle 5"); bRightPart = TRUE; }
	else if (RivetPos.nAngle == 5) { aValue[1] = _T("Angle 6"); bRightPart = TRUE; }
	else if (RivetPos.nAngle == 6) { aValue[1] = _T("Angle 7"); bRightPart = TRUE; }
	else if (RivetPos.nAngle == 7) { aValue[1] = _T("Angle 8"); bRightPart = TRUE; }
	else                           { aValue[1] = _T("");       }

	if (RivetPos.nComponent == RIVET_TL || RivetPos.nComponent == RIVET_TR ||
		RivetPos.nComponent == RIVET_BL || RivetPos.nComponent == RIVET_BR)
	{
		if (bSymmetric && bRightPart)
		{
			int nAngle = 0;
			if      (RivetPos.nAngle == 4) { nAngle = 0;}
			else if (RivetPos.nAngle == 5) { nAngle = 1;}
			else if (RivetPos.nAngle == 6) { nAngle = 2;}
			else if (RivetPos.nAngle == 7) { nAngle = 3;}

			T_SECT_RIVET_POS_D Temp = mapUseRevet[nAngle];

			RivetPos.nRefPos = (Temp.nRefPos==0)? 1 : 0;
			RivetPos.dDist   = Temp.dDist;
			RivetPos.dDia    = Temp.dDia;
		}

		if      (RivetPos.nRefPos == 0) aValue[2] = _T("Left");
		else if (RivetPos.nRefPos == 1) aValue[2] = _T("Right");
		else                            aValue[2] = _T("");
	}
	else if (RivetPos.nComponent == RIVET_WEB)
	{
		if (bSymmetric && bRightPart)
		{
			int nAngle = 0;
			if      (RivetPos.nAngle == 4) { nAngle = 0;}
			else if (RivetPos.nAngle == 5) { nAngle = 1;}
			else if (RivetPos.nAngle == 6) { nAngle = 2;}
			else if (RivetPos.nAngle == 7) { nAngle = 3;}

			T_SECT_RIVET_POS_D Temp = mapUseRevetW[nAngle];

			RivetPos.nRefPos = Temp.nRefPos;
			RivetPos.dDist   = Temp.dDist;
			RivetPos.dDia    = Temp.dDia;
		}

		if      (RivetPos.nRefPos == 0) aValue[3] = _T("Top");
		else if (RivetPos.nRefPos == 1) aValue[3] = _T("Bottom");
		else                            aValue[3] = _T("");
	}
	else
	{
		aValue[2] = _T("");
		aValue[3] = _T("");
	}

	aValue[4].Format(_T("%g"), RivetPos.dDist);
	aValue[5].Format(_T("%g"), RivetPos.dDia);
}

CString CCMSectRivetAngleGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString strNum(_T(""));
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
				bGet = pControl->GetValue(strNum);
		}
	}
	if (!bGet) strNum = GetValueRowCol(nRow, nCol);

	return strNum;
}

void CCMSectRivetAngleGrid::SortRivetArray(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D>& aArray)
{
	const std::pair<int, int> sortOrder[] = {
		{0, 2}, {0, 0}, {1, 6}, {1, 4}, {2, 0},
		{2, 1}, {3, 3}, {3, 1}, {4, 7}, {4, 5}
	};

	std::map<std::pair<int, int>, int> orderMap;
	for (int i = 0; i < sizeof(sortOrder) / sizeof(sortOrder[0]); ++i) 
	{
		orderMap[sortOrder[i]] = i;
	}

	int nSize = (int)aArray.GetSize();
	for (int i = 0; i < nSize - 1; ++i) 
	{
		for (int j = 0; j < nSize - i - 1; ++j) 
		{
			auto key1 = std::make_pair(aArray[j].nComponent, aArray[j].nAngle);
			auto key2 = std::make_pair(aArray[j + 1].nComponent, aArray[j + 1].nAngle);

			int order1 = orderMap.count(key1) ? orderMap[key1] : INT_MAX;
			int order2 = orderMap.count(key2) ? orderMap[key2] : INT_MAX;

			if (order1 > order2) 
			{
				T_SECT_RIVET_POS_D temp = aArray[j];
				aArray[j] = aArray[j + 1];
				aArray[j + 1] = temp;
			}
		}
	}
}

void CCMSectRivetAngleGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	CString strValue = GetEditingValue(nRow, nCol);

	CStringArray CurValue;
	CurValue.SetSize(D_COL_NUM);
	GetRowToData(nRow, CurValue);

	T_SECT_RIVET_POS_D RivetPos;
	StringToData(CurValue, RivetPos);

	T_SECT_RIVET_POS_D RivetTraget;
	int nRowTarget = 0;
	POSITION pos = m_mapRowdata.GetStartPosition();
	if(RivetPos.nComponent == RIVET_TL && RivetPos.nAngle == 0 && m_bSymmetric)    // _T("Top Left Flange") && _T("Angle 1")
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if(RivetTraget.nComponent == RIVET_TR && RivetTraget.nAngle == 4)      // _T("Top Right Flange")) && _T("Angle 5")
			{
				// aValue[2]
				if      (strValue == _T("Left"))  strValue = _T("Right");
				else if (strValue == _T("Right")) strValue = _T("Left");

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}
	else if (RivetPos.nComponent == RIVET_BL && RivetPos.nAngle == 1 && m_bSymmetric)  // _T("Bottom Left Flange") && _T("Angle 2")
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if (RivetTraget.nComponent == RIVET_BR && RivetTraget.nAngle == 5)        // _T("Bottom Right Flange")) && _T("Angle 6")
			{
				// aValue[2]
				if      (strValue == _T("Left"))  strValue = _T("Right");
				else if (strValue == _T("Right")) strValue = _T("Left");

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}
	else if (RivetPos.nComponent == RIVET_WEB && RivetPos.nAngle == 0)   // _T("Web") && Angle 1
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if (RivetTraget.nComponent == RIVET_WEB && RivetTraget.nAngle == 4) //_T("Angle 5")
			{
				if(nRowTarget == nRow) continue;

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}
	else if (RivetPos.nComponent == RIVET_WEB && RivetPos.nAngle == 1)   // _T("Web") && Angle 2
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if (RivetTraget.nComponent == RIVET_WEB && RivetTraget.nAngle == 5) //_T("Angle 6")
			{
				if (nRowTarget == nRow) continue;

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}

}

BEGIN_MESSAGE_MAP(CCMSectRivetAngleGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectRivetAngleGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleGrid message handlers

void CCMSectRivetAngleGrid::Initialize()
{
	CGXGridWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	// Wheel Mouse 지원
	EnableMouseWheel();

	SetColCount(D_COL_NUM);

	// Header
	HideCols(0, 0);
	SetFrozenCols(0, 0);

	// Component
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
	);

	// Angle
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
	);

	CString csChoiceList;

	csChoiceList.Empty();

	csChoiceList += _T("Left");
	csChoiceList += _T("\n");

	csChoiceList += _T("Right");
	csChoiceList += _T("\n");
	
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		.SetEnabled(TRUE)
	);

	csChoiceList.Empty();

	csChoiceList += _T("Top");
	csChoiceList += _T("\n");

	csChoiceList += _T("Bottom");
	csChoiceList += _T("\n");

	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		.SetEnabled(TRUE)
	);

	SetStyleRange(CGXRange().SetCols(5,6), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(TRUE)
	);

	SetRowHeight(0, 0, globalUtils.ScaleByDPI(30));

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);

	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	// 표준 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	int aWidth[] = { 115, 50, 60, 60, 60, 60 };

	for (int i = 0; i < D_COL_NUM; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}

	CString csUnit;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);

	CString strDist = _T("dR\n");
	CString strDia = _T("Diameter\n");

	SetValueRange(CGXRange(0, 1), _T("Component"));
	SetValueRange(CGXRange(0, 2), _T("Angle"));
	SetValueRange(CGXRange(0, 3), _T("Ref.Pos\n(horizontal)"));
	SetValueRange(CGXRange(0, 4), _T("Ref.Pos\n(vertical)"));
	SetValueRange(CGXRange(0, 5), strDist + _T("(") + csUnit + _T(")"));
	SetValueRange(CGXRange(0, 6), strDia + _T("(") + csUnit + _T(")"));

	LockUpdate(FALSE);
}

BOOL CCMSectRivetAngleGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol))
		ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bCurrentRow = (nRow == ncRow);
	bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.
	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
	{
		bEnabled = style.GetEnabled();
	}
	else // Column & Row style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
		GetRowStyle(nRow, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);
	bHeaderRow = IsFrozenRow(nRow);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(COLORREF(RGB(192, 220, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(::GetSysColor(COLOR_BTNFACE))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else style                      // Disabled Cell
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(255, 251, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(192, 220, 192)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(192, 220, 192)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));

	return TRUE;
}

BOOL CCMSectRivetAngleGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CGXGridWnd::OnStartEditing(nRow, nCol);
}

void CCMSectRivetAngleGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
 
	//if (m_bSymmetric)
	{
		ChangeColumnStatus(nRow, nCol);
	}
}

BOOL CCMSectRivetAngleGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bResult = CGXGridWnd::OnEndEditing(nRow, nCol);

	return bResult;
}

BOOL CCMSectRivetAngleGrid::ProcessKeys(CWnd* pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow = 1, nCol = 1;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if (nMessage == WM_KEYDOWN)
	{
		switch (nChar)
		{
		case VK_TAB:
		{
			BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
			if (bShift)
			{
				if (SetLeftValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
			}

			if (!bShift)
			{
				if (SetRightValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
			}
		}
		break;
		default:
			break;
		}
	}

	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

GT_PROC_RETURN:
	if (nMessage == WM_KEYDOWN && (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_LEFT))
	{
		Redraw();
	}

	return TRUE;
}

BOOL CCMSectRivetAngleGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{

	long i = nRow, j = nCol + 1; // 현재 행 다음 열
	int nColCount = GetColCount();
	if (j > nColCount) { ++i; j = 1; }

	BOOL bCovered;/* = FALSE;*/
	while (i <= GetRowCount())
	{
		bCovered = FALSE;

		while (j <= nColCount)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if (!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if ((cvr.top <= i && i <= cvr.bottom) && (cvr.left <= j && j <= cvr.right))
				{
					if (nRow == cvr.top) i = cvr.bottom;
					j = cvr.right + 1;
					bCovered = TRUE;
					continue;
				}
			}

			if (IsRowHidden(i)) break;
			if (IsColHidden(j)) { ++j; continue; }

			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
			BOOL bReadOnly = style.GetReadOnly();

			if (bEnable && !bReadOnly)
			{
				if (SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			//bCovered = TRUE;
			bCovered = FALSE;

			++j;  // 다음 열을 조사
		}
		if (j > nColCount) { ++i; j = 1; }
	}
	return FALSE;
}

BOOL CCMSectRivetAngleGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i = nRow, j = nCol - 1; // 현재 행 이전 열
	int nColCount = GetColCount();
	if (nCol == 0 || j < 1) { --i; j = nColCount; }

	BOOL bCovered = FALSE;
	while (i >= 1)
	{
		bCovered = FALSE;
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if (!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if ((cvr.top <= i && i <= cvr.bottom) && (cvr.left <= j && j <= cvr.right))
				{
					i = cvr.bottom;
					j = cvr.left - 1;
					bCovered = TRUE;
					continue;
				}
			}

			if (IsRowHidden(i)) break;
			if (IsColHidden(j)) { --j; continue; }

			BOOL bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
			BOOL bReadOnly = style.GetReadOnly();

			if (bEnable && !bReadOnly)
			{
				if (GetCoveredCellsRowCol(i, j, cvr))
				{
					if ((cvr.top <= i && i <= cvr.bottom) && (cvr.left <= j && j <= cvr.right))
					{
						j = cvr.left;
					}
				}
				if (SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			bCovered = TRUE;
			--j;  // 이전 열을 조사
		}
		if (nCol == 0 || j < 1) { --i; j = nColCount; }
	}
	return FALSE;
}
