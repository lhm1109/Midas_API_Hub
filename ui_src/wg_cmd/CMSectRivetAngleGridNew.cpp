// CMSectRivetAngleGridNew.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectRivetAngleGridNew.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_CompFunc.h"

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CCMSectRivetAngleGridNew::m_aCurDefVal[] = {_T("1"), _T("Top Left Flange"), _T("Angle 1"), _T("Left"), _T("Top"), _T("0.0"), _T("0.0")};
CCMSectRivetAngleGridNew::CCMSectRivetAngleGridNew()
{
	CString strRef = _LS(IDS_CMD_SECT_RIVET_Ref);
	CString strHor = _LS(IDS_CMD_SECT_RIVET_horizontal);
	CString strVer = _LS(IDS_CMD_SECT_RIVET_vertical);

	CString strRef_H = strRef + _T("\n") + strHor;
	CString strRef_V = strRef + _T("\n") + strVer;

	AddColInfo(_T("key"),                        t_real,   0); // dummy
	AddColInfo(_LS(IDS_CMD_SECT_RIVET_User),     t_string, 0);
	AddColInfo(_LS(IDS_CMD_SECT_RIVET_Component),t_string, 0);
	AddColInfo(_LS(IDS_CMD_SECT_RIVET_Angle),    t_string, 0);
	AddColInfo(strRef_H,                         t_string, 0);
	AddColInfo(strRef_V,                         t_string, 0);
	AddColInfo(_T("dR"),                         t_real,   0);
	AddColInfo(_LS(IDS_CMD_SECT_RIVET_Diameter), t_real,   0);
	m_aPrimaryKey.Add(0);

	CString aCurDefVal[] = { _T("1"), _LS(IDS_CMD_SECT_RIVET_Top_Left_Flange), _LS(IDS_CMD_SECT_RIVET_None), _LS(IDS_CMD_SECT_RIVET_Left), _LS(IDS_CMD_SECT_RIVET_Top), _T("0.0"), _T("0.0")};
	for (int i = 0; i < sizeof(aCurDefVal) / sizeof(CString); i++)
		m_aCurDefVal[i] = aCurDefVal[i];

	m_aData.RemoveAll();
}

CCMSectRivetAngleGridNew::~CCMSectRivetAngleGridNew()
{
}

BEGIN_MESSAGE_MAP(CCMSectRivetAngleGridNew, CTBCommon)
	//{{AFX_MSG_MAP(CCMSectRivetAngleGridNew)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMSectRivetAngleGridNew::SetData2Grid(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos, BOOL bSymmetric, CArray<int, int>& aAngleIndex)
{
	BOOL bOldLock = LockUpdate(TRUE);

	m_bSymmetric = bSymmetric;

	T_SECT_RIVET_POS_D RivetPos;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D> aRivetSort;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D> aRivetSortUser;

	std::map<int, int> mapUseAngle;

	for (int i = 0; i < aRivetPos.GetSize(); i++)
	{
		///
		if (aRivetPos[i].bUser)
		{
			aRivetSortUser.Add(aRivetPos[i]);
			continue;
		}
		///

		mapUseAngle[aRivetPos[i].nAngle] = aRivetPos[i].nAngle;
	}

	m_mapUsecAngle.RemoveAll();
	m_mapUsecAngle.InitHashTable(11);

	for (int i = 0; i < aAngleIndex.GetSize(); i++)
	{
		///
		m_mapUsecAngle.SetAt(aAngleIndex[i], aAngleIndex[i]);
		///

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
				///
				if (aRivetPos[j].bUser) continue;
				///

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
	m_aData.RemoveAll();

	m_mapRowdata.RemoveAll();
	m_mapRowdata.InitHashTable(11);
	for (int i = 0; i < nNum; i++)
	{
		CStringArray aValue;
		aValue.SetSize(D_DATA_NUM);

		RivetPos = aRivetSort[i];

		BOOL bRightPart = FALSE;
		DataToString(bSymmetric, mapUseRevet, mapUseRevetW, RivetPos, bRightPart, aValue);
		
		BOOL bSymRow = (bSymmetric && bRightPart)? TRUE : FALSE;
		if (RivetPos.nComponent == RIVET_WEB && bRightPart) bSymRow = TRUE;

		RIVET_POS_Temp RivetPosTemp(RivetPos, bSymRow);
		m_mapRowdata.SetAt(i+1, RivetPosTemp);
		m_aData.Add(RivetPosTemp);
	}

	///
	if (aRivetSortUser.GetSize() > 0)
	{
		for (int i = 0; i < aRivetSortUser.GetSize(); i++)
		{
			RIVET_POS_Temp RivetPosTemp(aRivetSortUser[i], FALSE);
			m_aData.Add(RivetPosTemp);
		}
	}
	///

 	LockUpdate(bOldLock);
 	if (!bOldLock) Redraw();

	MakeItemEx();
}

void CCMSectRivetAngleGridNew::DataToString(BOOL bSymmetric, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevet, std::map<int, T_SECT_RIVET_POS_D>& mapUseRevetW, IN T_SECT_RIVET_POS_D& RivetPos, 
										 BOOL& bRightPart, OUT CStringArray& aValue)
{
	aValue[0] = (RivetPos.bUser) ? _T("1") : _T("0");

	if      (RivetPos.nComponent == RIVET_TL ) aValue[1] = _LS(IDS_CMD_SECT_RIVET_Top_Left_Flange);  //_LS(IDS_CMD_SECT_RIVET_Web)
	else if (RivetPos.nComponent == RIVET_TR ) aValue[1] = _LS(IDS_CMD_SECT_RIVET_Top_Right_Flange);
	else if (RivetPos.nComponent == RIVET_WEB) aValue[1] = _LS(IDS_CMD_SECT_RIVET_Web);
	else if (RivetPos.nComponent == RIVET_BL ) aValue[1] = _LS(IDS_CMD_SECT_RIVET_Bottom_Left_Flange);
	else if (RivetPos.nComponent == RIVET_BR ) aValue[1] = _LS(IDS_CMD_SECT_RIVET_Bottom_Right_Flange);
	else                                       aValue[1] = _T("");

	if      (RivetPos.nAngle == 0) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_1);}  
	else if (RivetPos.nAngle == 1) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_2);}
	else if (RivetPos.nAngle == 2) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_3);}
	else if (RivetPos.nAngle == 3) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_4);}
	else if (RivetPos.nAngle == 4) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_5); bRightPart = TRUE; }
	else if (RivetPos.nAngle == 5) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_6); bRightPart = TRUE; }
	else if (RivetPos.nAngle == 6) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_7); bRightPart = TRUE; }
	else if (RivetPos.nAngle == 7) { aValue[2] = _LS(IDS_CMD_SECT_RIVET_Angle_8); bRightPart = TRUE; }
	else                           { aValue[2] = _LS(IDS_CMD_SECT_RIVET_None);       }

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

		if      (RivetPos.nRefPos == 0) aValue[3] = _LS(IDS_CMD_SECT_RIVET_Left);
		else if (RivetPos.nRefPos == 1) aValue[3] = _LS(IDS_CMD_SECT_RIVET_Right);   // _LS(IDS_CMD_SECT_RIVET_Bottom)
		else                            aValue[3] = _T("-");
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

		if      (RivetPos.nRefPos == 0) aValue[4] = _LS(IDS_CMD_SECT_RIVET_Top);
		else if (RivetPos.nRefPos == 1) aValue[4] = _LS(IDS_CMD_SECT_RIVET_Bottom);
		else                            aValue[4] = _T("-");
	}
	else
	{
		aValue[3] = _T("");
		aValue[4] = _T("");
	}

	aValue[5].Format(_T("%g"), RivetPos.dDist);
	aValue[6].Format(_T("%g"), RivetPos.dDia);
}


BOOL CCMSectRivetAngleGridNew::GetGrid2Data(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aRivetPos)
{
	int nCount = GetRowCount();

	aRivetPos.RemoveAll();

	T_SECT_RIVET_POS_D RivetPos;

	for (int i = 0; i < nCount; ++i)
	{
		double key;
		T_SECT_RIVET_POS_D RivetPos;
		if(!GetValue(i+1/*nRow*/, key, RivetPos))
			continue;

		aRivetPos.Add(RivetPos);
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleGridNew message handlers

void CCMSectRivetAngleGridNew::Initialize()
{
	T_UNIT_SYSTEM us;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE); // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	int nColCount = GetInitialColCount();
	SetColCount(nColCount);
	SetHeaderTitle(us, TRUE);

#ifdef _RUS
	SetRowHeightByDPI(0, 0, 30);
#else
	SetRowHeightByDPI(0, 0, 30);
#endif

	// key column(dummy)
	SetStyleRange(CGXRange().SetCols(1), CGXStyle() 
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED));

	// User 
	// Check Box Column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("1"))
		.SetEnabled(FALSE)
	);

	// Component
	CString csChoiceList;
	csChoiceList.Empty();
	csChoiceList +=_LS(IDS_CMD_SECT_RIVET_Top_Left_Flange);
	csChoiceList += _T("\n");
	csChoiceList +=_LS(IDS_CMD_SECT_RIVET_Top_Right_Flange);
	csChoiceList += _T("\n");
	csChoiceList +=_LS(IDS_CMD_SECT_RIVET_Web);
	csChoiceList += _T("\n");
	csChoiceList +=_LS(IDS_CMD_SECT_RIVET_Bottom_Left_Flange);
	csChoiceList += _T("\n");
	csChoiceList +=_LS(IDS_CMD_SECT_RIVET_Bottom_Right_Flange);
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

	// Angle
	CString strN;
	csChoiceList.Empty();
	for (int i = 0; i < 8; i++)
	{
		strN.Format(_T(" %d"), i+1);
		csChoiceList += _LS(IDS_CMD_SECT_RIVET_Angle) + strN;
		csChoiceList += _T("\n");
	}
	csChoiceList += _LS(IDS_CMD_SECT_RIVET_None);
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

	// Ref.Pos(hor)
	csChoiceList.Empty();
	csChoiceList += _LS(IDS_CMD_SECT_RIVET_Left);
	csChoiceList += _T("\n");
	csChoiceList += _LS(IDS_CMD_SECT_RIVET_Right);
	csChoiceList += _T("\n");
	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
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

	// Ref.Pos(ver)
	csChoiceList.Empty();
	csChoiceList += _LS(IDS_CMD_SECT_RIVET_Top);
	csChoiceList += _T("\n");
	csChoiceList += _LS(IDS_CMD_SECT_RIVET_Bottom);
	csChoiceList += _T("\n");
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
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

	// R, Diameter,
	SetStyleRange(CGXRange().SetCols(7, 8), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(TRUE)
	);

	HideCols(0, 0., TRUE); // key column(dummy)
//	HideCols(1, 1, TRUE); // User

	// 표준 스타일 변경
// 	ChangeStandardStyle(CGXStyle()
// 		.SetFont(CGXFont_GC().SetSize(9))
// 		.SetVerticalAlignment(DT_VCENTER)
// 		.SetAllowEnter(FALSE));
// 	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
// 	ChangeRowHeaderStyle(CGXStyle()
// 		.SetFont(CGXFont_GC().SetBold(FALSE))
// 		.SetHorizontalAlignment(DT_RIGHT)
// 		.SetControl(GX_IDS_CTRL_HEADER));
//  	GetParam()->SetNumberedRowHeaders(TRUE);
 
 	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_ALWAYS);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType  = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_BOTH, gxnEnabled | gxnEnhanced);

	LockUpdate(FALSE);

	SetExpressionWOEQ();
}

BOOL CCMSectRivetAngleGridNew::InsertRecordByKey(const double& key, const T_SECT_RIVET_POS_D& data)
{
	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data);
}

BOOL CCMSectRivetAngleGridNew::DeleteRecordByKey(const double& key, const T_SECT_RIVET_POS_D& data)
{
	// 레코드를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	// 삭제한다.
	return DeleteRecord(nRow);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleGridNew Overridable Functions
void CCMSectRivetAngleGridNew::OnChangedSelectionPublic(long* keys, int nSize)
{
	return;
}

long CCMSectRivetAngleGridNew::GetSelectedKeys(CArray<long, long>& caKey)
{
	return 0;
}

void CCMSectRivetAngleGridNew::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnModifyCell(nRow, nCol);

	if (nRow <= 0) return;

	ChangeColumnStatus(nRow, nCol);
}

BOOL CCMSectRivetAngleGridNew::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_ULS(Program Error!));
		return FALSE;
	}

	int nErrCode;
	if (nColID == D_REVIT_COL_DR || nColID == D_REVIT_COL_DIA)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch (nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_SW__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_SW__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_SW__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
		if (_tstof(value) < 0.0)
		{
			SetWarningText(_LS(IDS_SW_ERROR___NOT_ALLOW_NEGATIVE_VAL));
			return FALSE;
		}
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMSectRivetAngleGridNew::ValidateRecord(CStringArray& value, CRowColArray& aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_SW_ERROR___HAS_NOT_BEEN_ENTERED);
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);

	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CCMSectRivetAngleGridNew::ValidateAndMakeRecord(CStringArray& value, CRowColArray& aCols, double& key, T_SECT_RIVET_POS_D& data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 1; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1; // col id i에 해당하는 value가 저장된 위치
		ConvStrToData(i, value[nPos], data);
	}

	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	T_SECT_RIVET_POS_D data;

	GetValue(nRow, key, data);
	data.bUser = TRUE; // 직접 추가되는 경우

	if (!CheckValue(data)) return FALSE;

	return AddData(key, data);
}

BOOL CCMSectRivetAngleGridNew::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_SECT_RIVET_POS_D*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMSectRivetAngleGridNew::DeleteFromDB(ROWCOL nRow)
{
	RIVET_POS_Temp RivetPosTemp;
	if (m_mapRowdata.Lookup(nRow, RivetPosTemp))
	{
		return FALSE;
	}

	double key;
	T_SECT_RIVET_POS_D data;

	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CCMSectRivetAngleGridNew::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<double, double> awKeys;

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_tstof(value));
	}

	return DelData(awKeys);
}

BOOL CCMSectRivetAngleGridNew::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	T_SECT_RIVET_POS_D data;

	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	if (!CheckValue(data)) return FALSE;

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data);
}

BOOL CCMSectRivetAngleGridNew::ModifyToDB(CStringArray& raOldKey,
	CArray<void*, void*>& raKey,
	CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_SECT_RIVET_POS_D*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMSectRivetAngleGridNew::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CCMSectRivetAngleGridNew::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMSectRivetAngleGridNew::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMSectRivetAngleGridNew::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_SECT_RIVET_POS_D* pData;
	pKey = new double;
	pData = new T_SECT_RIVET_POS_D;
	pData->Initialize();
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMSectRivetAngleGridNew::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_SECT_RIVET_POS_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_SECT_RIVET_POS_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMSectRivetAngleGridNew::GetCountData()
{
	return m_aData.GetSize();
}

void CCMSectRivetAngleGridNew::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMSectRivetAngleGridNew::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	T_SECT_RIVET_POS_D Data;

	long nCount;
	for (nCount = 0; nCount < nNumData; nCount++)
	{
		Key = (double)nCount + 1;
		Data = m_aData[nCount].Data;

		SetValue(GetParam()->GetData(), nCount + 1, Key, Data, awCols);
	} // end of loop nCount
}

void CCMSectRivetAngleGridNew::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_SECT_RIVET_POS_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		ConvStrToData(i, EditingRecord[i], Data);
	}

	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleGridNew Implementation
BOOL CCMSectRivetAngleGridNew::CheckValue(T_SECT_RIVET_POS_D& RivetPos)
{
	BOOL bOK = FALSE;
	int nAngle = RivetPos.nAngle;
	if (RivetPos.nComponent == RIVET_TL)
	{
		if (nAngle == 0 || nAngle == 2)
			bOK = TRUE;
	}
	else if (RivetPos.nComponent == RIVET_TR)
	{
		if (nAngle == 4 || nAngle == 6)
			bOK = TRUE;
	}
	else if (RivetPos.nComponent == RIVET_WEB)
	{
		if (nAngle == 0 || nAngle == 1 || nAngle == 4 || nAngle == 5)
			bOK = TRUE;
	}
	else if (RivetPos.nComponent == RIVET_BL)
	{
		if (nAngle == 1 || nAngle == 3)
			bOK = TRUE;
	}
	else if (RivetPos.nComponent == RIVET_BR)
	{
		if (nAngle == 5 || nAngle == 7)
			bOK = TRUE;
	}
	else
		ASSERT(0);

	int nAngleTemp=0;
	if (!m_mapUsecAngle.Lookup(nAngle, nAngleTemp))
		bOK = FALSE;

	if (nAngle == 100)
		bOK = TRUE;

	if (!bOK)
	{
		CString msg;
		msg = _LS(IDS_CMD_SECT_RIVET_Component_ERROR); // [Error] The component did not have proper Angle information entered
		AfxMessageBox(msg);
	}

	return bOK;
}

void CCMSectRivetAngleGridNew::ConvRusToEng(CString& value)
{
	value.Replace(_T("Уголок"), _T("Angle")); 
}

void CCMSectRivetAngleGridNew::ConvEngToRus(CString& value)
{
	value.Replace(_T("Angle"), _T("Уголок"));
}

BOOL CCMSectRivetAngleGridNew::GetValue(ROWCOL nRow, double& key, T_SECT_RIVET_POS_D& data)
{
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴

#ifdef _RUS
		ConvRusToEng(str);
#endif

		VERIFY(ConvertFormulaToValue(str));

#ifdef _RUS
		ConvEngToRus(str);
#endif

		if (i == 0)
		{
			key = (double)nRow;
		}
		else 
			ConvStrToData(i, str, data);
	}

	if (data.nComponent == -1)
		return FALSE;

	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::SetValue(ROWCOL nRow, const double& key, const T_SECT_RIVET_POS_D& data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));

	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMSectRivetAngleGridNew::SetValue(CGXData* pData, ROWCOL nRow, const double& key, const T_SECT_RIVET_POS_D& data, CRowColArray& awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) str.Format(DATA_FMT, key);
		else        ConvDataToStr(i, data, str);

		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL bSymRow = FALSE;
	RIVET_POS_Temp RivetPosTemp;
	if (m_mapRowdata.Lookup(nRow, RivetPosTemp))
	{
		bSymRow = RivetPosTemp.bSymRow;
	}

	ROWCOL nCol;
	CStringArray raValue;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		VERIFY(ConvertFormulaToValue(str));
		if (i == 0)
		{
			str.Format(DATA_FMT, key);
		}
		raValue.Add(str);
	}

	SetDataToRow(nRow, bSymRow, raValue);
	return TRUE;
}

void CCMSectRivetAngleGridNew::SetDataToRow(ROWCOL nRow, BOOL bSymRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i = 0; i < nColCount; ++i)
	{
		int nColID = i+1;

		SetValueRange(CGXRange(nRow, nColID), raValue[i]);

		if (i == 0 || i == 1) // Key, User
		{
			// Skip
		}
		else if (i == 2 || i == 3) // Comp, Angle
		{
			BOOL bUser = (raValue[1] == _T("1"))? TRUE : FALSE;
			if (bUser) // User
			{
				SetStyleRange(CGXRange(nRow, nColID), CGXStyle().SetEnabled(TRUE));
			}
			else
			{
				SetStyleRange(CGXRange(nRow, nColID), CGXStyle().SetEnabled(FALSE));
			}
		}
		else if (i == 4 || i == 5) // hor, Ver
		{
			if (bSymRow)
			{
				SetStyleRange(CGXRange(nRow, nColID), CGXStyle().SetEnabled(FALSE));
			}
			else
			{
				if (raValue[i] == _T("-"))
					SetStyleRange(CGXRange(nRow, nColID), CGXStyle().SetEnabled(FALSE));
				else
					SetStyleRange(CGXRange(nRow, nColID), CGXStyle().SetEnabled(TRUE));
			}
		}
		else if (i == 6 || i == 7) //dR, Dia
		{
			if (bSymRow)
			{
				SetStyleRange(CGXRange(nRow, nColID), CGXStyle().SetEnabled(FALSE));
			}
			else
			{
				SetStyleRange(CGXRange(nRow, nColID), CGXStyle().SetEnabled(TRUE));
			}
		}

	}
}

void CCMSectRivetAngleGridNew::ConvStrToData(int i, CString& value, T_SECT_RIVET_POS_D& RivetPos)
{
	if      (i == 1) RivetPos.bUser = (value == _T("1")) ? TRUE : FALSE;
	else if (i == 2)
	{
		if      (value == _LS(IDS_CMD_SECT_RIVET_Top_Left_Flange))     RivetPos.nComponent = RIVET_TL;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Top_Right_Flange))    RivetPos.nComponent = RIVET_TR;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Web))                 RivetPos.nComponent = RIVET_WEB;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Bottom_Left_Flange))  RivetPos.nComponent = RIVET_BL;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Bottom_Right_Flange)) RivetPos.nComponent = RIVET_BR;
		else                                                           RivetPos.nComponent = -1;
	}
	else if (i == 3)
	{
		if      (value == _LS(IDS_CMD_SECT_RIVET_Angle_1)) RivetPos.nAngle = 0;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Angle_2)) RivetPos.nAngle = 1;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Angle_3)) RivetPos.nAngle = 2;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Angle_4)) RivetPos.nAngle = 3;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Angle_5)) RivetPos.nAngle = 4;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Angle_6)) RivetPos.nAngle = 5;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Angle_7)) RivetPos.nAngle = 6;
		else if (value == _LS(IDS_CMD_SECT_RIVET_Angle_8)) RivetPos.nAngle = 7;
		else                                               RivetPos.nAngle = 100;
	}
	else if (i == 4)
	{
		if (RivetPos.nComponent == RIVET_TL || RivetPos.nComponent == RIVET_TR ||
			RivetPos.nComponent == RIVET_BL || RivetPos.nComponent == RIVET_BR)
		{
			if      (value == _LS(IDS_CMD_SECT_RIVET_Left))  RivetPos.nRefPos = 0;
			else if (value == _LS(IDS_CMD_SECT_RIVET_Right)) RivetPos.nRefPos = 1;
			else                                             RivetPos.nRefPos = 0;
		}
		else 
			RivetPos.nRefPos = 0;
	}
	else if (i == 5)
	{
		if (RivetPos.nComponent == RIVET_WEB)
		{
			if      (value == _LS(IDS_CMD_SECT_RIVET_Top))    RivetPos.nRefPos = 0;
			else if (value == _LS(IDS_CMD_SECT_RIVET_Bottom)) RivetPos.nRefPos = 1;
			else                                              RivetPos.nRefPos = 0;
		}
	}
	else if (i == 6) RivetPos.dDist    = _tstof(value);
	else if (i == 7) RivetPos.dDia     = _tstof(value);
	else ASSERT(0);
}

void CCMSectRivetAngleGridNew::ConvDataToStr(int i,/*col*/ const T_SECT_RIVET_POS_D& RivetPos, CString& value)
{
	BOOL bRightPart = FALSE;
	if (i == 1)
	{
		value = (RivetPos.bUser) ? _T("1") : _T("0");
	}
	else if (i == 2)
	{
		if      (RivetPos.nComponent == RIVET_TL ) value = _LS(IDS_CMD_SECT_RIVET_Top_Left_Flange);
		else if (RivetPos.nComponent == RIVET_TR ) value = _LS(IDS_CMD_SECT_RIVET_Top_Right_Flange);
		else if (RivetPos.nComponent == RIVET_WEB) value = _LS(IDS_CMD_SECT_RIVET_Web);
		else if (RivetPos.nComponent == RIVET_BL ) value = _LS(IDS_CMD_SECT_RIVET_Bottom_Left_Flange);
		else if (RivetPos.nComponent == RIVET_BR ) value = _LS(IDS_CMD_SECT_RIVET_Bottom_Right_Flange);
		else                                       value = _T("");
	}
	else if (i == 3)
	{
		if      (RivetPos.nAngle == 0) { value = _LS(IDS_CMD_SECT_RIVET_Angle_1);}
		else if (RivetPos.nAngle == 1) { value = _LS(IDS_CMD_SECT_RIVET_Angle_2);}
		else if (RivetPos.nAngle == 2) { value = _LS(IDS_CMD_SECT_RIVET_Angle_3);}
		else if (RivetPos.nAngle == 3) { value = _LS(IDS_CMD_SECT_RIVET_Angle_4);}
		else if (RivetPos.nAngle == 4) { value = _LS(IDS_CMD_SECT_RIVET_Angle_5);/* bRightPart = TRUE;*/ }
		else if (RivetPos.nAngle == 5) { value = _LS(IDS_CMD_SECT_RIVET_Angle_6);/* bRightPart = TRUE;*/ }
		else if (RivetPos.nAngle == 6) { value = _LS(IDS_CMD_SECT_RIVET_Angle_7);/* bRightPart = TRUE;*/ }
		else if (RivetPos.nAngle == 7) { value = _LS(IDS_CMD_SECT_RIVET_Angle_8);/* bRightPart = TRUE;*/ }
		else                           { value = _LS(IDS_CMD_SECT_RIVET_None);       }

	}
	else if (i == 4)
	{
		if (RivetPos.nComponent == RIVET_TL || RivetPos.nComponent == RIVET_TR ||
			RivetPos.nComponent == RIVET_BL || RivetPos.nComponent == RIVET_BR)
		{
			if      (RivetPos.nRefPos == 0) value = _LS(IDS_CMD_SECT_RIVET_Left);
			else if (RivetPos.nRefPos == 1) value = _LS(IDS_CMD_SECT_RIVET_Right);
			else                            value = _T("-");
		}
		else
			value = _T("-");
	}
	else if (i == 5)
	{
		if (RivetPos.nComponent == RIVET_WEB)
		{
			if      (RivetPos.nRefPos == 0) value = _LS(IDS_CMD_SECT_RIVET_Top);
			else if (RivetPos.nRefPos == 1) value = _LS(IDS_CMD_SECT_RIVET_Bottom);
			else                            value = _T("-");
		}
		else
			value = _T("-");
	}
	else if (i == 6) 
		value.Format(_T("%g"), RivetPos.dDist);
	else if (i == 7) 
		value.Format(_T("%g"), RivetPos.dDia);
	else 
		ASSERT(0);
}

void CCMSectRivetAngleGridNew::MakeSearchKey(const double& key, const T_SECT_RIVET_POS_D& data, CStringArray& aKey)
{
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		int nColID = m_aSortInfo[i].nRC;

		if (nColID == 0) aKey[i].Format(DATA_FMT, key);
		else             ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMSectRivetAngleGridNew::SetHeaderTitle(T_UNIT_SYSTEM& us, BOOL bInit)
{
	BOOL bOldLock = LockUpdate(TRUE);

	if (bInit) SetColWidthByDPI(0, 0, 30);

	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; ++i)
	{
		CString strTitle = m_aTitle[i];
		if (i == 6 || i == 7)
		{
			CString strUnit = _T("");
			CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit);

			strTitle += _T("\n(") + strUnit + _T(")");
		}
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(strTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));

		if (bInit)
		{
			switch (i)
			{
			case 0: SetColWidthByDPI(i + 1, i + 1,   0); break; // key
			case 1: SetColWidthByDPI(i + 1, i + 1,  30); break; // User
			case 2: SetColWidthByDPI(i + 1, i + 1,  80); break; // Component
			case 3: SetColWidthByDPI(i + 1, i + 1,  60); break; // Angle
			case 4: SetColWidthByDPI(i + 1, i + 1,  67); break; // Hor.
			case 5: SetColWidthByDPI(i + 1, i + 1,  60); break; // Ver.
			case 6: SetColWidthByDPI(i + 1, i + 1,  50); break; // R
			case 7: SetColWidthByDPI(i + 1, i + 1,  60); break; // Dia.
			default: ASSERT(0); break;
			}
		}
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

BOOL CCMSectRivetAngleGridNew::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_aData.GetSize();
	// 단위변환에 대한  거더길이의 오차
	double dTol = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 0.5); // 0.5 mm

	CString csVal;
	double dblVal;
	double TempKey;

	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		TempKey = i+1;
		csVal.Format(_T("%g"), TempKey);
		dblVal = _tstof(csVal);
		nCompResult = CCompFunc::FastCompDouble(key, dblVal);

		nCompResult = CCompFunc::CompRealTol(key, dblVal, dTol);
		if      (nCompResult > 0) continue; // key > value
		else if (nCompResult < 0) break;    // key < value
		else                                // key == value
		{
			nPos = i;
			return TRUE;;
		}
	}
	nPos = i;
	return FALSE;
}


// 이 함수는 수정하지 않고 CTBBrowserWndEx 에서 그대로 퍼왔음
BOOL CCMSectRivetAngleGridNew::FindRecordByKey(CStringArray& aKey, ROWCOL& nRow)
{
	ROWCOL nStart, nEnd, nCurrent, nRow1;
	nStart = 1;
	nEnd = GetRowCount() - 1;

	// sorting key에 대한 column 번호를 모두 찾는다.
	int nNumKeyCol = m_aSortInfo.GetSize();
	VERIFY(nNumKeyCol != 0);
	VERIFY(aKey.GetSize() == nNumKeyCol);

	CRowColArray awCols;
	awCols.SetSize(nNumKeyCol);
	for (int i = 0; i < nNumKeyCol; i++)
		GetColumnNoFor(m_aSortInfo[i].nRC, awCols[i]);

	// 추가된 레코드가 없으면 정상 호출
	if (m_nEditMode != addnew)
		return FindRecordByKey(nStart, nEnd, awCols, aKey, nRow);

	// 추가된 레코드가 있으면 추가된 레코드 위치를 경계로 분할해서 두 번 호출
	nCurrent = GetBrowseParam()->m_nCurrentRow;

	// 제일 첫번째가 addnew이면 nCurrent+1 부터 끝까지 찾는 것과 동일
	if (nCurrent == nStart) return FindRecordByKey(nCurrent + 1, nEnd, awCols, aKey, nRow);

	// addnew 레코드가 레코드 끝에 있을 때는 nCurrent가 nEnd가 되며 여기서
	// 찾게 되면 리턴이 되고 그렇지 않으면 뒷 부분으로 간다.
	// 이 부분은 addnew 레코드의 앞부분이기도 하다.
	if (FindRecordByKey(nStart, nCurrent - 1, awCols, aKey, nRow)) return TRUE;

	// addnew 레코드가 끝에 있는 경우 검색이 끝난 경우이므로 FALSE 리턴
	if (nCurrent == nEnd) return FALSE;

	// addnew 레코드가 중간에 있는 경우이다. 이제 addnew 레코드의 뒷부분을 찾아본다.
	// 이전에 찾은 nRow의 값을 저장한다. 이것은 못 찾은 경우 검색키보다 바로 큰 값의
	// 위치를 리턴하기 위해 필요하다.
	nRow1 = nRow;
	if (FindRecordByKey(nCurrent + 1, nEnd, awCols, aKey, nRow)) return TRUE;

	// 검색키보다 큰 값 중 가장 가까운 위치를 계산해서 리턴한다.
	if (nRow1 < nCurrent) nRow = nRow1; // 이 경우는 앞 부분에 있는 경우
	return FALSE;
}

// 이 함수는 CTBBrowserWndEx 에서 그대로 퍼오고
// case t_real 에 대한 톨러런스만 변경 ㅜㅜ
BOOL CCMSectRivetAngleGridNew::FindRecordByKey(ROWCOL nStart, ROWCOL nEnd, CRowColArray& awCols, CStringArray& aKey, ROWCOL& nRow)
{
	double dTol = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 0.5); // 0.5 mm (case t_real: 에서의 톨러런스를 고치기 위해서 함수 재정의..)

	// 바이너리 탐색으로 찾는다.
	ROWCOL nMid;
	CString value;            // 비교 대상 string 값
	long nKey, nCandidate;    // 정수 키 값 및 비교 대상 값
	double dKey, dCandidate;  // 실수 키 값 및 비교 대상 값
	int nCompResult;       // 후보가 검색 키보다 크면 양수, 같으면 0, 작으면 음수
	BOOL bAscending;          // 비교 칼럼의 sorting이 ascending이면 1, 아니면 0

	// 레코드가 없을 때는 바로 리턴한다.
	if (nEnd < nStart) { nRow = nStart;  return FALSE; }

	int nNumKeyCol = awCols.GetSize();

	while (nEnd >= nStart)
	{
		// 중간값을 구한다.
		nMid = (nStart + nEnd) >> 1;

		// sorting column에 대해 모두 값을 비교한다.
		for (int i = 0; i < nNumKeyCol; i++)
		{
			// 편집 모드일 수 있으므로 편집 전의 값을 가져온다.
			value = GetValueRowColBeforeEdit(nMid, awCols[i]);
			// 타입에 따라 값을 비교한다.
			switch (m_aType[m_aSortInfo[i].nRC])
			{
			case t_string:
				// 대소문자 구분 없음
				// Grid Library내에서 _tcscoll을 사용한다.
				// _tcscoll과 CompareNoCase의 Return값이 틀리므로 반드시 _tcscoll을
				// 사용해야 한다.
				//nCompResult = value.CompareNoCase(aKey[i]);
				nCompResult = _tcscoll(value, aKey[i]);
				break;
			case t_integer:
				nCandidate = _ttol(value);
				nKey = _ttol(aKey[i]);
				nCompResult = nCandidate - nKey;
				break;
			case t_real:
				dCandidate = _tstof(value);
				dKey = _tstof(aKey[i]);
				if (fabs(dCandidate - dKey) < dTol) nCompResult = 0;
				else if (dCandidate > dKey) nCompResult = 1;
				else nCompResult = -1;
				break;
			default:
				ASSERT(0);  // 에러...
				break;
			}
			if (nCompResult) // 크거나 작으면 더이상의 키 값 비교를 하지 않는다.
			{
				bAscending = (m_aSortInfo[i].sortOrder == CGXSortInfo::ascending);
				// BOOL값은 0 아니면 1만 가지는 것으로 한다.
				ASSERT(bAscending == 0 || bAscending == 1);
				ASSERT(!bAscending == 0 || !bAscending == 1);
				break;
			}
		}

		if (nCompResult > 0)
		{
			if (bAscending) nEnd = nMid - 1;
			else nStart = nMid + 1;
		}
		else if (nCompResult < 0)
		{
			if (bAscending) nStart = nMid + 1;
			else nEnd = nMid - 1;
		}
		else { nRow = nMid; return TRUE; }
	}

	// 못 찾으면 첫번째로 큰 값(ascending, 작은값:descending)의 위치
	if (nCompResult > 0) nRow = nMid + !bAscending;
	else nRow = nMid + bAscending;

	return FALSE;
}

BOOL CCMSectRivetAngleGridNew::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
{
	CArray<double, double> aTempKey;

	int nSize = aKey.GetSize();
	aTempKey.Copy(aKey);
	BOOL bSame;
	int nPos = 0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aTempKey[i], nPos, nStart);
		if (!bSame)
		{
			errorKey = aTempKey[i];
			return FALSE;
		}
		nPos = nStart;
	}
	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::AddData(double key, T_SECT_RIVET_POS_D data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aData;
	aKey.Add(key);
	aData.Add(data);
	RIVET_POS_Temp prof;
	prof.Data= data;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_SW_ERROR___ALREADY_EXIST_s_g___), m_aTitle[2], key);
		AfxMessageBox(msg);
		return FALSE;
	}

	m_aData.InsertAt(nPos, prof);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();

	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::AddData(CArray<double, double>& aKey, CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aData, BOOL bNotify)
{
	CArray<double, double> aOldKey;

	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);

	BOOL bSame;
	int nPos = 0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aOldKey[i], nPos, nStart);
		if (!bSame && i > 0) bSame = (aOldKey[i] - aOldKey[i - 1] <= EPSILON);

		if (bSame)
		{
			CString msg;
			msg.Format(_LS(IDS_SW_ERROR___ALREADY_EXIST_s_g___), m_aTitle[i], aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		RIVET_POS_Temp prof;
		prof.Data= aData[i];
		m_aData.InsertAt(nPos, prof);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aData;
	T_SECT_RIVET_POS_D data; data.Initialize();
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_SW_ERROR___ALREADY_EXIST_s_g___), m_aTitle[2], key);
		AfxMessageBox(msg);
		return FALSE;
	}

	m_aData.RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();

	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::DelData(CArray<double, double>& aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aData;
	int nSize = aKey.GetSize();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		T_SECT_RIVET_POS_D data; data.Initialize();
		aData.Add(data);
	}

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_SW_ERROR___ALREADY_EXIST_s_g___), m_aTitle[i], errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_aData.GetSize();
	int j = nSize2 - 1;

	double TempKey;
	double dblDiff;
	for (int i = nSize - 1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			TempKey = j + 1;
			dblDiff = aOldKey[i] - TempKey;
			if (dblDiff >= -EPSILON && dblDiff <= EPSILON)
			{
				m_aData.RemoveAt(j);
				j--;
				break;
			}
		}
	}
	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();

	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::ModifyData(double oldKey, double key, T_SECT_RIVET_POS_D data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	oldKey = key; // RivetAngleGrid에서는 현재 key로 Modigy 하자.
	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_SW_ERROR___ALREADY_EXIST_s_g___), m_aTitle[2], oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (oldKey == key)
	{
		m_aData[nPos].Data = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_SW_ERROR___ALREADY_EXIST_s_g___), m_aTitle[2], key);
			AfxMessageBox(msg);
			return FALSE;
		}
		RIVET_POS_Temp prof;
		prof.Data = data;
		m_aData.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_aData.InsertAt(nPos2, prof);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();

	return TRUE;
}

BOOL CCMSectRivetAngleGridNew::ModifyData(CArray<double, double>& aOldKey, CArray<double, double>& aKey,
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aData)
{
	CArray<RIVET_POS_Temp, RIVET_POS_Temp&> aOrgData;
	aOrgData.Copy(m_aData);

	if (!DelData(aOldKey, FALSE))
		return FALSE;

	if (!AddData(aKey, aData, FALSE))
	{
		m_aData.Copy(aOrgData);
		return FALSE;
	}
	DBChanged();

	return TRUE;
}

void CCMSectRivetAngleGridNew::UpdateBuffer(int nCmd, CArray<double, double>& aOldKey, CArray<double, double>& aKey,
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&>& aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch (nCmd)
		{
		case(0):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			InsertRecordByKey(aKey[i], aData[i]);
			break;
		case(1):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			DeleteRecordByKey(aKey[i], aData[i]);
			break;
		case(2):
			DeleteRecordByKey(aOldKey[i], aData[i]);
			InsertRecordByKey(aKey[i], aData[i]);
			break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_nEditMode = CTBBrowserWnd::noMode;
		if (GetRowCount() < ncRow) ncRow = GetRowCount();

		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetLeftValidCell(ncRow, ncCol);
		m_nEditMode = nOldMode;
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMSectRivetAngleGridNew::DBChanged()
{
	/*
		if(m_pParent->GetSafeHwnd() == 0) return;
		if(!IsWindow(m_pParent->GetSafeHwnd())) return;

		m_pParent->GridDataChanged();
	*/
}

void CCMSectRivetAngleGridNew::SetHunchInfo(BOOL bHunch)
{
	BOOL bOldLock = LockUpdate(TRUE);

	HideCols(8, 8, bHunch);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

//////////////////////////////////////////////////////////////////////////
void CCMSectRivetAngleGridNew::SortRivetArray(CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D>& aArray)
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

void CCMSectRivetAngleGridNew::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	RIVET_POS_Temp RivetPosTemp;
	if (!m_mapRowdata.Lookup(nRow, RivetPosTemp))
	{
		ChangeColumnStatusUser(nRow, nCol);
		return;
	}

	CString strValue = GetValueRowCol(nRow, nCol);

	double key;
	T_SECT_RIVET_POS_D RivetPos;
	GetValue(nRow, key, RivetPos);

	RIVET_POS_Temp RivetTraget;
	int nRowTarget = 0;
	POSITION pos = m_mapRowdata.GetStartPosition();
	if (RivetPos.nComponent == RIVET_TL && RivetPos.nAngle == 0 && m_bSymmetric)    // _LS(IDS_CMD_SECT_RIVET_Top_Left_Flange) && _LS(IDS_CMD_SECT_RIVET_Angle_1)
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if (RivetTraget.Data.nComponent == RIVET_TR && RivetTraget.Data.nAngle == 4)      // _LS(IDS_CMD_SECT_RIVET_Top_Right_Flange)) && _LS(IDS_CMD_SECT_RIVET_Angle_5)
			{
				// aValue[2]
				if      (strValue == _LS(IDS_CMD_SECT_RIVET_Left))  strValue = _LS(IDS_CMD_SECT_RIVET_Right);
				else if (strValue == _LS(IDS_CMD_SECT_RIVET_Right)) strValue = _LS(IDS_CMD_SECT_RIVET_Left);

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}
	else if (RivetPos.nComponent == RIVET_BL && RivetPos.nAngle == 1 && m_bSymmetric)  // _LS(IDS_CMD_SECT_RIVET_Bottom_Left_Flange) && _LS(IDS_CMD_SECT_RIVET_Angle_2)
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if (RivetTraget.Data.nComponent == RIVET_BR && RivetTraget.Data.nAngle == 5)        // _LS(IDS_CMD_SECT_RIVET_Bottom_Right_Flange)) && _LS(IDS_CMD_SECT_RIVET_Angle_6)
			{
				// aValue[2]
				if      (strValue == _LS(IDS_CMD_SECT_RIVET_Left))  strValue = _LS(IDS_CMD_SECT_RIVET_Right);
				else if (strValue == _LS(IDS_CMD_SECT_RIVET_Right)) strValue = _LS(IDS_CMD_SECT_RIVET_Left);

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}
	else if (RivetPos.nComponent == RIVET_WEB && RivetPos.nAngle == 0)   // _LS(IDS_CMD_SECT_RIVET_Web) && Angle 1
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if (RivetTraget.Data.nComponent == RIVET_WEB && RivetTraget.Data.nAngle == 4) //_LS(IDS_CMD_SECT_RIVET_Angle_5)
			{
				if (nRowTarget == nRow) continue;

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}
	else if (RivetPos.nComponent == RIVET_WEB && RivetPos.nAngle == 1)   // _LS(IDS_CMD_SECT_RIVET_Web) && Angle 2
	{
		while (pos != NULL)
		{
			m_mapRowdata.GetNextAssoc(pos, nRowTarget, RivetTraget);
			if (RivetTraget.Data.nComponent == RIVET_WEB && RivetTraget.Data.nAngle == 5) //_LS(IDS_CMD_SECT_RIVET_Angle_6)
			{
				if (nRowTarget == nRow) continue;

				SetValueRange(CGXRange(nRowTarget, nCol), strValue);
				break;
			}
		}
	}
}

void CCMSectRivetAngleGridNew::ChangeColumnStatusUser(ROWCOL nRow, ROWCOL nCol)
{
	CString strValue = GetValueRowCol(nRow, nCol);

	if (nCol != D_REVIT_COL_COM+1) return;

	double key;
	T_SECT_RIVET_POS_D RivetPos;
	GetValue(nRow, key, RivetPos);

	SetValue(nRow, key, RivetPos);
}

// 항상 FALSE를 리턴해야 한다.
// 왜냐 하면 Grid가 Paste 작업을 해서는 안 되기 때문
BOOL CCMSectRivetAngleGridNew::OnPasteFromClipboard(const CGXRange& range)
{
	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴

	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CGXGridWnd x;
	int nPasteMode;
	int nPastedCols;

	// 임시 GridWnd에 Paste 작업을 한다.
	if (!PasteFromClipboard(range, GetDefValueArray(), GetKeyColID(),
		x, nPasteMode, nPastedCols))
	{
		x.DestroyWindow();
		DisplayWarningText();
		return FALSE;
	}
	CStringArray value;
	value.SetSize(GetColCount());
	CArray<void*, void*> aKey;
	CArray<void*, void*> aData;
	CRowColArray aCols;
	aCols.SetSize(GetColCount());
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
		VERIFY(GetColumnNoFor(i, aCols[i]));

	// DB 수정이 되지 않는 경우이므로 Copy되는 Field Validation만 수행한다.
	if (nPasteMode == TB_PASTE_MODE_ONMODIFY)
	{
		int nColID;
		for (int j = range.left; j < range.left + nPastedCols; j++)
		{
			VERIFY(GetColumnIDFor(j, nColID));
			value[j - 1] = x.GetValueRowCol(1, j);
			if (!ValidateField(value[j - 1], nColID))
			{
				x.DestroyWindow();
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	else // DB에 추가 또는 수정이 일어나는 경우이므로 Record Validation을 한다.
	{
		int nxRowCount = x.GetRowCount();
		for (int i = 1; i <= nxRowCount; i++)
		{
			// Table에서 레코드 값을 가져온다.
			int nxColCount = x.GetColCount();
			for (int j = 1; j <= nxColCount; j++)
			{
				CString str = x.GetValueRowCol(i, j);
				value[j - 1] = x.GetValueRowCol(i, j);
			}
			// String Array에 대해 Validation 검사. (필드, 레코드 전체)
			if (!MakeKeyAndDataArray(value, aCols, aKey, aData))
			{
				DeleteKeyAndDataArray(aKey, aData);
				x.DestroyWindow();
				if (m_sWarningText.IsEmpty())
					SetWarningText(_T("Invalid data. Operation has been canceled."));
				DisplayWarningText();
				return FALSE;
			}
		}
	}

	// 모드에 따라 Append 또는 Modify를 한다.
	if (nPasteMode == TB_PASTE_MODE_APPEND) // DB에 추가된다.
	{
		AddToDB(aKey, aData);
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY)  // DB 수정 없다.
	{
		// 직접 그리드를 수정한다.
		CArray<int, int> aColID;
		aColID.SetSize(nPastedCols);
		for (int j = 0; j < nPastedCols; j++)
			VERIFY(GetColumnIDFor(range.left + j, aColID[j]));
		qsort((void*)aColID.GetData(), aColID.GetSize(), sizeof(UINT), CCompFunc::intAsc);
		ROWCOL nColNo;
		for (int j = 0; j < nPastedCols; j++)
		{
			VERIFY(GetColumnNoFor(aColID[j], nColNo));
			SetExpressionRowCol(range.top, nColNo, value[nColNo - 1]);
			OnModifyCell(range.top, nColNo);
		}
	}
	else if (nPasteMode == TB_PASTE_MODE_MODIFY) // DB가 수정된다.
	{
		// 기존 key를 가져온다.
		CStringArray aOldKey;
		aOldKey.SetSize(x.GetRowCount());
		int nxRowCount = x.GetRowCount();
		for (int i = 0; i < nxRowCount; i++)
		{
			aOldKey[i] = (TCHAR)_ttol(GetValueRowCol(range.top + i, aCols[0]));
		}
		ModifyToDB(aOldKey, aKey, aData);
	}
	DeleteKeyAndDataArray(aKey, aData);

	x.DestroyWindow();

	return FALSE;   // 실제 Paste는 하지 않는다.
}

BOOL CCMSectRivetAngleGridNew::PasteFromClipboard(const CGXRange& range, CString* aDefVal, int nKeyCol, CGXGridWnd& x, int& nPasteMode, int& nPastedCols)
{
	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	// Paste된 범위를 구한다.
	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();

	// 규칙
	// 범위가 다르면 경고 메세지를 보내고 리턴
	// Paste된 내용이 레코드의 모든 필드를 만족하지 못하면 Default 값으로 채운다.
	// column 구분이 없기 때문에 칼럼 by 칼럼으로 Paste한다.
	// 즉 column의 위치가 변경되었으면 변경된 대로 Paste 한다.
	// 고의적으로 Hide된 칼럼은 Default 값으로 채운다.

	// 용어
	// 원본 범위 : Copy해서 Clipboard로 들어간 row, col 갯수
	// 대상 범위 : User가 마우스로 선택해 놓은 범위

	// 범위에 고의로 숨겨진 Column(HideByIntend)이 포함되어 있으면 이 칼럼을
	// 삽입하고 Default Value로 채운다.
	CArray<int, int> aHideCol;
	int nColCount = GetColCount();
	int nPasteColCount = r.GetWidth();
	for (int k = 1; k < nPasteColCount; k++)
	{
		if (range.left + k < nColCount && IsHiddenByIntend(range.left + k))
		{
			x.InsertCols(k + 1, 1);
			aHideCol.Add(range.left + k);
			r.right++;
			nPasteColCount++;
		}
	}

	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	int nLeftInsertWidth, nRightInsertWidth;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Append Row를 넘어서면 수정과 추가가 중복되므로 에러
		if (nRow != GetRowCount() && (nRow + r.GetHeight()) > GetRowCount())
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}

		// 편집 모드이고 원본 범위가 2줄 이상이면 에러
		if (IsEdit() && r.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}

		// 현재 셀이 Append Row에 있으면 모두 추가(DB에 추가된다.)
		if (nRow == GetRowCount())
			nPasteMode = TB_PASTE_MODE_APPEND;
		// 레코드 수정상태를 유지(DB를 수정하지 않는다.)
		else if (IsEdit()) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 모두 수정(DB에서 수정이 된다.)
		else nPasteMode = TB_PASTE_MODE_MODIFY;
	}
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// 편집 모드에서 편집 레코드가 대상 범위에 포함되어 있고 2줄 이상이면 에러 표시
		if (IsEdit() && nRow >= range.top && nRow <= range.bottom && range.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}

		// Append Row가 범위에 포함된 경우 다른 Row도 범위에 포함되어 있으면 에러
		if (range.bottom == GetRowCount() && range.GetHeight() > 1)
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_TB_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

		// Append 단독 Row면 레코드 추가
		if (range.bottom == GetRowCount()) nPasteMode = TB_PASTE_MODE_APPEND;
		// 레코드 수정 상태를 유지하며 DB에서 수정은 일어나지 않는다.
		else if (IsEdit() && range.top == nRow) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 아니면 레코드 수정이 된다.
		else nPasteMode = TB_PASTE_MODE_MODIFY;

		// Fill Mode인지 검사한다.
		if (bFillMode)
		{
			// 대상 범위의 줄수 만큼 Row를 만들어 채운다.
			x.SetRowCount(range.GetHeight());
			CString value;
			for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
			{
				value = x.GetValueRowCol(1, i + 1);
				for (int j = 1; j < range.GetHeight(); j++)
					x.SetExpressionRowCol(j + 1, i + 1, value);
			}
			r.bottom = x.GetRowCount();
		}
	}

	// 레코드 또는 레코드 셋을 만든다.
	// 왼쪽과 오른쪽에 모자라는 칼럼수를 삽입한다.
	nPastedCols = r.GetWidth();
	nLeftInsertWidth = range.left - 1;
	if (nLeftInsertWidth > 0) x.InsertCols(1, nLeftInsertWidth);
	nRightInsertWidth = GetColCount() - x.GetColCount();
	if (nRightInsertWidth > 0) x.InsertCols(x.GetColCount() + 1, nRightInsertWidth);
	// 모자라는 값을 카피한다. (Append는 Default값, Modify는 기존 값)
	if (nPasteMode == TB_PASTE_MODE_APPEND)
	{
		int nID;
		CString value;
		// 속도 향상을 위해 column별로 복사
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			nID = j - 1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID - 1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 Default값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			nID = aHideCol[j] - 1;
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, aHideCol[j], aDefVal[nID - 1]);
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth + nPastedCols + 1; j <= GetColCount(); j++)
		{
			nID = j - 1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID - 1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY || nPasteMode == TB_PASTE_MODE_MODIFY)
	{
		CString value;
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top - 1 + i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 기존의 값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top - 1 + i, aHideCol[j]);
				x.SetExpressionRowCol(i, aHideCol[j], value);
			}
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth + nPastedCols + 1; j <= GetColCount(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top - 1 + i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
	}

END_OF_PASTE:
	if (!sErrMsg.IsEmpty())
	{
		SetWarningText(sErrMsg);
		return FALSE;
	}
	return TRUE;
}
