// CMSdviGrid.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdviGrid.h"
#include "CMSdviDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KEY_FMT         _T("%ld")
#define DATA_FMT        _T("%g")

const int COL_DOF=1;
const int COL_Ce =2;
const int COL_P1 =3;
const int COL_C1 =4;
const int COL_A1 =5;
const int COL_K0 =6;

const int ROW_DATA=2;

/////////////////////////////////////////////////////////////////////////////
// CCMSdviGrid
CString CCMSdviGrid::m_aCurDefVal[] = { _T("0"), _T("0."), _T("0."), _T("0."), _T("0."), _T("0.") };

CCMSdviGrid::CCMSdviGrid()
{
	m_nColCount=6;
	m_nHeaderRow=2;
	m_bOnInit=TRUE;
	
	AddColInfo(_T("DOF") , t_string, 0);
	AddColInfo(_T("Ce"), t_real, 0);  
	AddColInfo(_T("P1"), t_real, 0);  
	AddColInfo(_T("C1"), t_real, 0);  
	AddColInfo(_T("A1"), t_real, 0);  
	AddColInfo(_T("K0"), t_real, 0);  
	m_aPrimaryKey.Add(0);
}

CCMSdviGrid::~CCMSdviGrid()
{
}

BEGIN_MESSAGE_MAP(CCMSdviGrid, CTBBrowserWndEx)
	//{{AFX_MSG_MAP(CCMSdviGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSdviGrid message handlers

void CCMSdviGrid::Initialize(CCMSdviDlg* pParent)
{
	ASSERT(pParent);
	m_pParent = pParent;

	CTBBrowserWndEx::Initialize();

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	int m_nColCount = GetInitialColCount();
	SetColCount(m_nColCount);
	SetHeaderTitle(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(FALSE);

	// 컬럼 스타일 변경
	// DOF
	CString strCompnt = _T("Dx\nDy\nDz\nRx\nRy\nRz");
	SetStyleRange(CGXRange().SetCols(COL_DOF, COL_DOF), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetVerticalAlignment(DT_VCENTER)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strCompnt));

	SetStyleRange(CGXRange().SetCols(COL_Ce, m_nColCount), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC));

	if (m_nHeaderRow-1 > 0)  SetFrozenRows(m_nHeaderRow-1, m_nHeaderRow-1);    // m_nHeaderRow번째 줄까지 Header로 표현 (Frozen Row 지정)

	SetRowHeightByDPI(0, 0, 26);
	SetRowHeightByDPI(1, 1, 40);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	LockUpdate(FALSE);
}

BOOL CCMSdviGrid::SetRowHeight(ROWCOL nFromRow, ROWCOL nToRow, int nHeight, int* anHeightArray/* = NULL*/, UINT flags/* = GX_UPDATENOW*/, GXCmdType ctCmd/* = gxDo*/)
{
	return CTBBrowserWndEx::SetRowHeight(nFromRow,nToRow,nHeight,anHeightArray,flags,ctCmd);
}

void CCMSdviGrid::MakeItem()
{
	CString strTitleProp[5];
	GetPropHeaderTitle(m_data.GetActiveDOF(), strTitleProp);
	SetStyleRange(CGXRange(1, COL_Ce), CGXStyle().SetValue(strTitleProp[0]));
	SetStyleRange(CGXRange(1, COL_P1), CGXStyle().SetValue(strTitleProp[1]));
	SetStyleRange(CGXRange(1, COL_C1), CGXStyle().SetValue(strTitleProp[2]));
	SetStyleRange(CGXRange(1, COL_A1), CGXStyle().SetValue(strTitleProp[3]));
	SetStyleRange(CGXRange(1, COL_K0), CGXStyle().SetValue(strTitleProp[4]));

	ROWCOL nRow = ROW_DATA;
	InsertEmptyRecord(nRow);

	SetValue(nRow, m_data.CompProp[m_data.GetActiveDOF()]);
	m_nEditMode = noMode;

	SetCurrentCell(ROW_DATA,COL_Ce);
	SetRowHeightByDPI(ROW_DATA, ROW_DATA, 21);
}

void CCMSdviGrid::GetData(T_SDVI_D& data)
{
	// Component 값만 곱게 채워서 보낸다
	int nIdx = GetActiveDOFInt(GetValueRowCol(ROW_DATA,COL_DOF));
	GetValue(ROW_DATA, data.CompProp[nIdx]);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSdviGrid Implementation
BOOL CCMSdviGrid::GetValue(ROWCOL nRow, T_SDVI_COMP &data)
{
	for (int i=COL_DOF; i<=m_nColCount; i++)
	{
		CString str = GetValueRowCol(nRow, i);
		ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CCMSdviGrid::SetValue(ROWCOL nRow, const T_SDVI_COMP &data)
{
	CString strDof[] = { _T("Dx"),_T("Dy"),_T("Dz"),_T("Rx"),_T("Ry"),_T("Rz") };
	SetStyleRange(CGXRange(nRow, COL_DOF), CGXStyle().SetValue(strDof[m_data.GetActiveDOF()]));
	
	for(int i=COL_Ce; i<=m_nColCount; i++)
	{
		CString str;
		ConvDataToStr(i, data,str);
		SetStyleRange(CGXRange(nRow, i), CGXStyle().SetValue(str));
	}

	return TRUE;
}

void CCMSdviGrid::ConvStrToData(int i, CString& value, T_SDVI_COMP& data)
{
			 if(i==COL_DOF) data.bDof  = TRUE;
	else if(i==COL_Ce ) data.Ce     = _tstof(value);
	else if(i==COL_P1 ) data.P1     = _tstof(value);
	else if(i==COL_C1 ) data.C1     = _tstof(value);
	else if(i==COL_A1 ) data.Alpha1 = _tstof(value);
	else if(i==COL_K0 ) data.K0     = _tstof(value);
	else ASSERT(0);
}

void CCMSdviGrid::ConvDataToStr(int i, const T_SDVI_COMP& data, CString& value)
{
			if(i==COL_Ce) value.Format(DATA_FMT, data.Ce    );
	else if(i==COL_P1) value.Format(DATA_FMT, data.P1    );
	else if(i==COL_C1) value.Format(DATA_FMT, data.C1    );
	else if(i==COL_A1) value.Format(DATA_FMT, data.Alpha1);
	else if(i==COL_K0) value.Format(DATA_FMT, data.K0    );
	else ASSERT(0);
}

void CCMSdviGrid::SetHeaderTitle(BOOL bInit)
{  
	// Title text
	SetStyleRange(CGXRange(0, COL_DOF), CGXStyle().SetValue(_T("DOF")));
	SetStyleRange(CGXRange(0, COL_Ce), CGXStyle().SetValue(_LS(IDS_CMD_SEIS_PROP_SDVI_DASHPOT_PROP)));
	SetStyleRange(CGXRange(0, COL_K0), CGXStyle().SetValue(_LS(IDS_CMD_SEIS_PROP_SDVI_SPRING_PROP )));

	// Width
	int nColSize[] = { 50, 123, 103, 123, 103, 123 };

	for (int i=0; i<GetColCount(); i++)
	{
		SetStyleRange(CGXRange(1, i+1), CGXStyle()
			.SetValue(m_aTitle[i])
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));

		if (bInit) SetColWidthByDPI(i+1, i+1, nColSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 0);

	// Merge cell
	SetCoveredCellsRowCol(0, COL_DOF, 1, COL_DOF);
	SetCoveredCellsRowCol(0, COL_Ce, 0, COL_A1);
}

void CCMSdviGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	int nIdx = GetActiveDOFInt(GetValueRowCol(ROW_DATA,COL_DOF));
	CString strTitleProp[5];
	GetPropHeaderTitle(nIdx, strTitleProp);
	SetStyleRange(CGXRange(1, COL_Ce), CGXStyle().SetValue(strTitleProp[0]));
	SetStyleRange(CGXRange(1, COL_P1), CGXStyle().SetValue(strTitleProp[1]));
	SetStyleRange(CGXRange(1, COL_C1), CGXStyle().SetValue(strTitleProp[2]));
	SetStyleRange(CGXRange(1, COL_A1), CGXStyle().SetValue(strTitleProp[3]));
	SetStyleRange(CGXRange(1, COL_K0), CGXStyle().SetValue(strTitleProp[4]));
}

// Paste를 지원하기 위해서는 이 함수를 각자 override해야 한다.
BOOL CCMSdviGrid::OnPasteFromClipboard(const CGXRange &range)
{
	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CString csErrMsg;
	CRowColArray awRows, awCols;

	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	CGXGridWnd x;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), this, 0);
	x.Initialize();
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();
	ROWCOL nPastedRows = r.bottom - r.top + 1;   
	ROWCOL nPastedCols = r.right - r.left + 1;  

	ROWCOL nTotalRow = GetRowCount();
	ROWCOL nTotalCol = GetColCount();

	GetSelectedRows(awRows);
	GetSelectedCols(awCols);

	CString csValue;
	if (awRows.GetSize() > 1) // Select된 것이 2줄 이상
	{
		if (nPastedRows != 1)
		{
			if (awRows.GetSize() != nPastedRows)
			{
				csErrMsg = _LS(IDS_TB_GRID_ERR_MSG2);
				goto PASTE_ERROR;
			}
		}
	}
	else 
	{
		ROWCOL nTgtRow;
		if (awRows.GetSize() == 0)  
		{
			ROWCOL ncRow, ncCol;
			if (!GetCurrentCell(ncRow, ncCol))  
			{
				csErrMsg = _LS(IDS_TB_GRID_ERR_MSG4);
				goto PASTE_ERROR;
			}
			nTgtRow = ncRow;
		}
		else nTgtRow = awRows[0];

		if (nTgtRow + nPastedRows - 1 > nTotalRow)
		{
			csErrMsg = _LS(IDS_TB_GRID_ERR_MSG5);
			goto PASTE_ERROR;
		}

		if(awCols[0] + nPastedCols - 1 > nTotalCol)
		{
			csErrMsg = _LS(IDS_TB_GRID_ERR_MSG5);
			goto PASTE_ERROR;
		}
	}

	// 일단 ReadOnly 모두 풀어준다. : ReadOnly Cell 에 대해서 붙여넣기 불가.
	{
		BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
		GetParam()->SetLockReadOnly(FALSE);
		CGXGridWnd::OnPasteFromClipboard(range);
		GetParam()->SetLockReadOnly(bOldReadOnlyLock);
	}

	// CCMSdviGrid : 빈칸은 비우고 자동계산 값 채워줌
	EnableDisableCells();
	SetAutoCalcValue();  

	x.DestroyWindow();
	return TRUE;   

PASTE_ERROR:
	SetWarningText(csErrMsg);
	DisplayWarningText();
	x.DestroyWindow();
	return FALSE;
}

BOOL CCMSdviGrid::ValidateField(CString& value, int nColID)
{
	if (nColID > GetColCount())
		return FALSE;

	int nErrCode = 0;

	if (nColID == 0) return TRUE;
	else
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCMSdviGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// Set color of header rows.
	if (nRow < m_nHeaderRow && nRow > 0)  // m_nHeaderRow번째 줄까지 Header로 표현 (Style)
	{
		style.SetInterior(m_Color.RowColHeaderBg)
			.SetTextColor(m_Color.NormalModeFg);
		return TRUE;
	}

	return CTBBrowserWndEx::GetStyleRowCol(nRow, nCol, style, mt, nType);
}

void CCMSdviGrid::UpdateType(int nDamperType, int nDashpotType, int nInputType) 
{  
	m_nDamperType=nDamperType; 
	m_nDashpotType=nDashpotType; 
	m_nInputType=nInputType; 

	EnableDisableCells();
	SetAutoCalcValue();
	m_bOnInit=FALSE;
}

void CCMSdviGrid::EnableDisableCells()
{
	BOOL bEnable[][5] = 
	{
		// [Damper Type][Dashpot Type][Input Type]
		{  TRUE, FALSE, FALSE, FALSE, FALSE }, // [Single][Linear]
		{  TRUE,  TRUE, FALSE,  TRUE, FALSE }, // [Single][Elastic][A1]
		{  TRUE,  TRUE,  TRUE, FALSE, FALSE }, // [Single][Elastic][C1]
		{  TRUE, FALSE, FALSE, FALSE,  TRUE }, // [Kelvin][Linear]
		{  TRUE,  TRUE, FALSE,  TRUE,  TRUE }, // [Kelvin][Elastic][A1]
		{  TRUE,  TRUE,  TRUE, FALSE,  TRUE }, // [Kelvin][Elastic][C1]
		{  TRUE, FALSE, FALSE, FALSE,  TRUE }, // [Maxwel][Linear]
		{  TRUE,  TRUE, FALSE,  TRUE,  TRUE }, // [Maxwel][Elastic][A1]
		{  TRUE,  TRUE,  TRUE, FALSE,  TRUE }, // [Maxwel][Elastic][C1]
		//{ FALSE, FALSE, FALSE, FALSE, FALSE }, // DOF Disabled
	};

	int nIndex=0;
			 if(m_nDamperType==0 && m_nDashpotType==0) nIndex=0;
	else if(m_nDamperType==0 && m_nDashpotType==1 && m_nInputType==0) nIndex=1;
	else if(m_nDamperType==0 && m_nDashpotType==1 && m_nInputType==1) nIndex=2;
	else if(m_nDamperType==1 && m_nDashpotType==0                   ) nIndex=3;
	else if(m_nDamperType==1 && m_nDashpotType==1 && m_nInputType==0) nIndex=4;
	else if(m_nDamperType==1 && m_nDashpotType==1 && m_nInputType==1) nIndex=5;
	else if(m_nDamperType==2 && m_nDashpotType==0                   ) nIndex=6;
	else if(m_nDamperType==2 && m_nDashpotType==1 && m_nInputType==0) nIndex=7;
	else if(m_nDamperType==2 && m_nDashpotType==1 && m_nInputType==1) nIndex=8;
	//const int IDX_DISABLE=9;


//     ROWCOL nRow=nDof+2;
//     BOOL bDOF = _ttoi(GetValueRowCol(nRow, COL_DOF_CHK));
	
		for(int i=0; i<5; i++)
		{
			ROWCOL nCol = COL_Ce+i;

			BOOL bColEnable = bEnable[nIndex][i];
			SetStyleRange(CGXRange(ROW_DATA, nCol), CGXStyle().SetEnabled(bColEnable));

			// 활성화됐는데 빈칸이면 0, 비활성화됐으면 빈칸으로 만들어주자
			if(bColEnable && GetValueRowCol(ROW_DATA,nCol).IsEmpty()) SetStyleRange(CGXRange(ROW_DATA, nCol),CGXStyle().SetValue(_T("0")));
			if(!bColEnable) SetStyleRange(CGXRange(ROW_DATA, nCol),CGXStyle().SetValue(_T("")));
		}    

}

// // Field의 값이 변경되면 Table의 스타일과 값에 변경이 생긴다.
// BOOL CCMSdviGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
// {
//   BOOL bResult = CGXGridWnd::OnStartEditing(nRow, nCol);
// 
//   // DOF를 체크하다니!
//   if(nCol==COL_DOF_CHK) 
//   {
//     BOOL bEnable[][6] = 
//     {
//       // [Damper Type][Dashpot Type][Input Type]
//       { TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE }, // [Single][Linear]
//       { TRUE,  TRUE,  TRUE, FALSE,  TRUE, FALSE }, // [Single][Elastic][A1]
//       { TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE }, // [Single][Elastic][C1]
//       { TRUE,  TRUE, FALSE, FALSE, FALSE,  TRUE }, // [Kelvin][Linear]
//       { TRUE,  TRUE,  TRUE, FALSE,  TRUE,  TRUE }, // [Kelvin][Elastic][A1]
//       { TRUE,  TRUE,  TRUE,  TRUE, FALSE,  TRUE }, // [Kelvin][Elastic][C1]
//       { TRUE,  TRUE, FALSE, FALSE, FALSE,  TRUE }, // [Maxwel][Linear]
//       { TRUE,  TRUE,  TRUE, FALSE,  TRUE,  TRUE }, // [Maxwel][Elastic][A1]
//       { TRUE,  TRUE,  TRUE,  TRUE, FALSE,  TRUE }, // [Maxwel][Elastic][C1]
//       { TRUE, FALSE, FALSE, FALSE, FALSE, FALSE }, // DOF Disabled
//     };
// 
//     int nIndex=0;
//     if(m_nDamperType==0 && m_nDashpotType==0) nIndex=0;
//     else if(m_nDamperType==0 && m_nDashpotType==1 && m_nInputType==0) nIndex=1;
//     else if(m_nDamperType==0 && m_nDashpotType==1 && m_nInputType==1) nIndex=2;
//     else if(m_nDamperType==1 && m_nDashpotType==0                   ) nIndex=3;
//     else if(m_nDamperType==1 && m_nDashpotType==1 && m_nInputType==0) nIndex=4;
//     else if(m_nDamperType==1 && m_nDashpotType==1 && m_nInputType==1) nIndex=5;
//     else if(m_nDamperType==2 && m_nDashpotType==0                   ) nIndex=6;
//     else if(m_nDamperType==2 && m_nDashpotType==2 && m_nInputType==0) nIndex=7;
//     else if(m_nDamperType==2 && m_nDashpotType==2 && m_nInputType==1) nIndex=8;
//     const int IDX_DISABLE=9;
// 
//     // 아직 바뀌기 전이므로 반대값으로 처리
//     BOOL bDOF = _ttoi(GetValueRowCol(nRow, COL_DOF_CHK))==0 ? TRUE:FALSE;
//     for(int i=0; i<6; i++)
//     {
//       ROWCOL nCol_prop=i+2;
// 
//       BOOL bColEnable = bDOF==TRUE ? bEnable[nIndex][i] : bEnable[IDX_DISABLE][i];
//       SetStyleRange(CGXRange(nRow, nCol_prop), CGXStyle().SetEnabled(bColEnable));
// 
//       // 활성화됐는데 빈칸이면 0, 비활성화됐으면 빈칸으로 만들어주자
//       if(bColEnable && GetValueRowCol(nRow,nCol_prop).IsEmpty()) SetStyleRange(CGXRange(nRow, nCol_prop),CGXStyle().SetValue(_T("0")));
//       if(!bColEnable) SetStyleRange(CGXRange(nRow, nCol_prop),CGXStyle().SetValue(_T("")));
//     }  
//   }
// 
//   return bResult;
// }

BOOL CCMSdviGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bResult = CGXGridWnd::OnEndEditing(nRow, nCol);
	if(!m_bOnInit) SetAutoCalcValue();
	return bResult;
}

void CCMSdviGrid::SetAutoCalcValue()
{
	UpdateData(TRUE);
	
	if(m_nDashpotType==0) return;

	if(m_nInputType==0) 
	{
		double A1 = _tstof(GetValueRowCol(ROW_DATA, COL_A1));
		double Ce = _tstof(GetValueRowCol(ROW_DATA, COL_Ce));
		double dValue =  A1*Ce;
		SetStyleRange(CGXRange(ROW_DATA, COL_C1),CGXStyle().SetValue(dValue));
	}
	else if(m_nInputType==1)
	{
		double C1 = _tstof(GetValueRowCol(ROW_DATA, COL_C1));
		double Ce = _tstof(GetValueRowCol(ROW_DATA, COL_Ce));
		double dValue =  Ce!=0. ? C1/Ce : 0.;
		SetStyleRange(CGXRange(ROW_DATA, COL_A1),CGXStyle().SetValue(dValue));
	}
}

BOOL CCMSdviGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
			case VK_DELETE: return TRUE;
		}
	}

	return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

int CCMSdviGrid::GetActiveDOFInt(CString value)
{
			 if (value == _T("Dx")) return 0;
	else if (value == _T("Dy")) return 1;
	else if (value == _T("Dz")) return 2;
	else if (value == _T("Rx")) return 3;
	else if (value == _T("Ry")) return 4;
	else if (value == _T("Rz")) return 5;
	return -1;
}

CString CCMSdviGrid::GetActiveDOFStr(int idx)
{
			 if (idx == 0) return _T("Dx");
	else if (idx == 1) return _T("Dy");
	else if (idx == 2) return _T("Dz");
	else if (idx == 3) return _T("Rx");
	else if (idx == 4) return _T("Ry");
	else if (idx == 5) return _T("Rz");
	return _T("");
}

void CCMSdviGrid::GetPropHeaderTitle(int nComp, CString aTitle[5])
{
	T_SDVI_UNIT* pUnit = &(CUnitCtrl::m_SDVI_UNIT);
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CString strTitle,strUnit[5];

	if(nComp<3) // Dx,Dy,Dz
	{
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Ce_Dxyz, strUnit[0]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->P1_Dxyz, strUnit[1]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->C1_Dxyz, strUnit[2]); 
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->K0_Dxyz, strUnit[4]); 
	}
	else if(nComp>=3) // Rx,Ry,Rz
	{
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Ce_Rxyz, strUnit[0]);
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->P1_Rxyz, strUnit[1]);
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->C1_Rxyz, strUnit[2]);
		pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->K0_Rxyz, strUnit[4]);
	}
	
	aTitle[0].Format(_T("Ce\n(%s)"), strUnit[0]);
	aTitle[1].Format(_T("P1\n(%s)"), strUnit[1]);
	aTitle[2].Format(_T("C1\n(%s)"), strUnit[2]);
	aTitle[3].Format(_T("Alpha1"));
	aTitle[4].Format(_T("K0\n(%s)"), strUnit[4]);
}