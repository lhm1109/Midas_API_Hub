// IehpItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
// * 혹시 BiLinear 가 사용되면 아래부분 풀어서 사용
// 
// #include "IehpProp_PMmcDlg.h"
// //#include "IehpProp_MLinDlg.h"
// 
// #include "..\wg_base\wg_base_CompFunc.h"
// #include "..\wg_db\wg_db_UnitCtrl.h"
// 
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
// 
// #define EPSILON 1.e-12
// #define DATA_FMT _T("%.15g")
// 
// /////////////////////////////////////////////////////////////////////////////
// // CIehpProp_PMMLinGrid
// CString CIehpProp_PMMLinGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0")};
// CIehpProp_PMMLinGrid::CIehpProp_PMMLinGrid()
// {
// 	m_pData = 0;
// 	m_pDataAxial = 0;
// 
// 	m_nHeadRow   = 0;
// 	m_nReturnDir = 0; // 아래
// }
// 
// CIehpProp_PMMLinGrid::~CIehpProp_PMMLinGrid()
// {
// }
// 
// 
// BEGIN_MESSAGE_MAP(CIehpProp_PMMLinGrid, CTBBrowserWnd/*CTBCommon*/)
// 	//{{AFX_MSG_MAP(CIehpProp_PMMLinGrid)
// 		// NOTE - the ClassWizard will add and remove mapping macros here.
// 	//}}AFX_MSG_MAP
// END_MESSAGE_MAP()
// 
// 
// /////////////////////////////////////////////////////////////////////////////
// // CIehpProp_PMMLinGrid message handlers
// 
// void CIehpProp_PMMLinGrid::Initialize(T_IEHP_PMMULT* pData, T_UNIT_SYSTEM &UnitSystem, int nSymmetry, int nDof, int nAxalCmbIndex)
// {
// 	ASSERT(pData);
// 	m_pData = pData;
// 	m_pDataAxial = &m_pData->aAxial[nAxalCmbIndex];
// 	m_nSymmetry  = nSymmetry;
// 	m_nDof = nDof;
// 
// 	CDBDoc *pDoc = CDBDoc::GetDocPoint();
// 	//CTBCommon::Initialize(pDoc, UnitSystem);
// 	CTBGrid::Initialize();
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	LockUpdate(TRUE);
// 
// 	GetParam()->EnableUndo(FALSE);
// 	GetParam()->EnableTrackRowHeight(FALSE);  
// 	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged
// 
// 	CGXProperties* pProp = GetParam()->GetProperties();
// 	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
// 	pProp->SetMarkRowHeader(FALSE);
// 	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
// 												(CGXStyle) pProp->sInvertNoBorder);
// 
// 	// 컬럼 갯수 설정
// 	int m_nColCount = 3;//GetInitialColCount();
// 	SetColCount(3);         
// 	SetRowCount(5);  
// 
// 	SetHeaderTitle(UnitSystem, TRUE);
// 
// 	// 표준 스타일 변경
// 	ChangeStandardStyle(CGXStyle()
// 			.SetFont(CGXFont_GC().SetSize(9))
// 			.SetVerticalAlignment(DT_BOTTOM)
// 			.SetAllowEnter(FALSE));
// 
// 	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
// 	ChangeRowHeaderStyle(CGXStyle()
// 					.SetFont(CGXFont_GC().SetBold(FALSE))
// 					.SetHorizontalAlignment(DT_RIGHT)
// 					.SetControl(GX_IDS_CTRL_HEADER));
// 	GetParam()->SetNumberedRowHeaders(FALSE);
// 
// 	// 컬럼 스타일 변경
// //   SetStyleRange(CGXRange().SetCols(1), CGXStyle()
// //     .SetFont(CGXFont_GC().SetBold(FALSE))
// //     .SetHorizontalAlignment(DT_RIGHT)
// //     .SetControl(GX_IDS_CTRL_HEADER));
// 	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
// 		.SetReadOnly(TRUE)
// 		.SetControl(GX_IDS_CTRL_STATIC)
// 		.SetValueType(GX_VT_STRING) // Items!!
// 		.SetHorizontalAlignment(DT_CENTER)
// 		);
// 
// 	SetStyleRange(CGXRange().SetCols(2, m_nColCount), CGXStyle()
// 			.SetHorizontalAlignment(DT_RIGHT)
// 			.SetValueType(GX_VT_NUMERIC)
// 			.SetFormat(GX_FMT_FLOAT)
// 			.SetPlaces(4));
// 	SetRowHeight(0, 0, 36);
// 	SetRowHeight(1, 5, 20);
// 
// 	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
// 			.SetPlaces(4));
// 
// 	// 추가 변경
// 	if(m_nSymmetry==0)
// 	{
// 		SetStyleRange(CGXRange(1, 2, 3, 3), CGXStyle().SetReadOnly(FALSE).SetEnabled(TRUE));
// 	}
// 	else
// 	{
// 		SetStyleRange(CGXRange(2, 2, 3, 2), CGXStyle().SetReadOnly(FALSE).SetEnabled(TRUE));
// 		SetStyleRange(CGXRange(3, 3, 3, 3), CGXStyle().SetReadOnly(FALSE).SetEnabled(TRUE));
// 	}
// 
// 	// 현재 셀 설정
// 	SetRightValidCell(1, 0);
// 	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
// 
// 	// 숨기기
// 	HideCols(0, 0, TRUE);
// 
// 	LockUpdate(FALSE);
// 
// //   Data2Grid();
// }
// void CIehpProp_PMMLinGrid::Setdata(T_IEHP_PMMULT* pData, int nSymmetry, int nAxalCmbIndex)
// {
// 	ASSERT(pData);
// 	m_pData = pData;
// 	m_pDataAxial = &m_pData->aAxial[nAxalCmbIndex];
// 	m_nSymmetry  = nSymmetry;
// 	Data2Grid();
// }
// 
// 
// void CIehpProp_PMMLinGrid::Data2Grid()
// {
// 	UpdateData(TRUE);
// 	LockUpdate(TRUE);
// 
// 	if(m_nSymmetry==0) // Sym
// 	{
// 		GetParam()->SetLockReadOnly(FALSE);
// 		SetStyleRange(CGXRange(1, 2, 5, 3), CGXStyle().SetReadOnly(FALSE).SetEnabled(TRUE));
// 
// 		SetValueRange(CGXRange(1, 2), -m_pDataAxial->dDispNotUse[4]);  // m_pDataAxial->dDisp[0] 
// 		SetValueRange(CGXRange(2, 2), -m_pDataAxial->dDispNotUse[3]);  // m_pDataAxial->dDisp[1]
// 		SetValueRange(CGXRange(3, 2),  m_pDataAxial->dDispNotUse[2]);
// 		SetValueRange(CGXRange(4, 2),  m_pDataAxial->dDispNotUse[3]);
// 		SetValueRange(CGXRange(5, 2),  m_pDataAxial->dDispNotUse[4]);
// 
// 		SetValueRange(CGXRange(1, 3), -m_pDataAxial->dForcNotUse[4]);   // m_pDataAxial->dForc[0] 
// 		SetValueRange(CGXRange(2, 3), -m_pDataAxial->dForcNotUse[3]);   // m_pDataAxial->dForc[1]
// 		SetValueRange(CGXRange(3, 3),  m_pDataAxial->dForcNotUse[2]);
// 		SetValueRange(CGXRange(4, 3),  m_pDataAxial->dForcNotUse[3]);
// 		SetValueRange(CGXRange(5, 3),  m_pDataAxial->dForcNotUse[4]);
// 
// 		SetStyleRange(CGXRange(1, 2, 3, 3), CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));
// 		GetParam()->SetLockReadOnly(TRUE);
// 	}
// 	else
// 	{
// 		GetParam()->SetLockReadOnly(FALSE);
// 		SetStyleRange(CGXRange(1, 2, 5, 3), CGXStyle().SetReadOnly(FALSE).SetEnabled(TRUE));
// 
// 		double dForc = m_pDataAxial->dForcNotUse[3];
// 		double dDisp = m_pDataAxial->dDispNotUse[3];
// 		double dStiff = (dDisp==0.0 )? 0.0 : dForc / dDisp;
// 		double dDisp2 = (dStiff==0.0)? 0.0 : m_pDataAxial->dForcNotUse[1] / dStiff;
// 
// 		SetValueRange(CGXRange(1, 2), m_pDataAxial->dDispNotUse[0]); 
// 		SetValueRange(CGXRange(2, 2), dDisp2                );  // m_pDataAxial->dDisp[1]
// 		SetValueRange(CGXRange(3, 2), m_pDataAxial->dDispNotUse[2]);
// 		SetValueRange(CGXRange(4, 2), m_pDataAxial->dDispNotUse[3]);
// 		SetValueRange(CGXRange(5, 2), m_pDataAxial->dDispNotUse[4]);
// 
// 		SetValueRange(CGXRange(1, 3), m_pDataAxial->dForcNotUse[0]); 
// 		SetValueRange(CGXRange(2, 3), m_pDataAxial->dForcNotUse[1]);
// 		SetValueRange(CGXRange(3, 3), m_pDataAxial->dForcNotUse[2]);
// 		SetValueRange(CGXRange(4, 3), m_pDataAxial->dForcNotUse[3]);
// 		SetValueRange(CGXRange(5, 3), m_pDataAxial->dForcNotUse[4]);
// 
// 		SetStyleRange(CGXRange(2, 2, 3, 2), CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));
// 		SetStyleRange(CGXRange(3, 3, 3, 3), CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));
// 		GetParam()->SetLockReadOnly(TRUE);
// 
// 	}
// 
// 	LockUpdate(FALSE);
// 	Redraw();
// 
// 	DBChanged();
// }
// 
// void CIehpProp_PMMLinGrid::Grid2Data()
// {
// 	m_pDataAxial->dDispNotUse[0] = _tstof(GetValueRowCol(1, 2)); // double 
// 	m_pDataAxial->dDispNotUse[1] = _tstof(GetValueRowCol(2, 2)); // double
// 	m_pDataAxial->dDispNotUse[2] = _tstof(GetValueRowCol(3, 2)); // double
// 	m_pDataAxial->dDispNotUse[3] = _tstof(GetValueRowCol(4, 2)); // double
// 	m_pDataAxial->dDispNotUse[4] = _tstof(GetValueRowCol(5, 2)); // double
// 
// 	m_pDataAxial->dForcNotUse[0] = _tstof(GetValueRowCol(1, 3)); // double 
// 	m_pDataAxial->dForcNotUse[1] = _tstof(GetValueRowCol(2, 3)); // double
// 	m_pDataAxial->dForcNotUse[2] = _tstof(GetValueRowCol(3, 3)); // double
// 	m_pDataAxial->dForcNotUse[3] = _tstof(GetValueRowCol(4, 3)); // double
// 	m_pDataAxial->dForcNotUse[4] = _tstof(GetValueRowCol(5, 3)); // double
//
// }
// 
// 
// void CIehpProp_PMMLinGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
// {
// 	// Set Title
// 
// 	CString strNull[] = { _T("P2(-)"),  _T("P1(-)"),   _T("0"),   _T("P1(+)"), _T("P2(+)")};
// 	for (int i = 0 ; i < 5 ; i++)
// 	{
// 		SetStyleRange(CGXRange(i+1, 1), CGXStyle().SetValue(strNull[i]));
// 	}
// 
// 	CStringArray aTitle;
// 	CArray<int, int> aUnit;
// 
// 	aTitle.Add(_T(""));
// 	aTitle.Add(_LS(IDS_CMD_IEHP_MULTI_LINEAR_DISPALCEMENT));
// 	aTitle.Add(_LS(IDS_CMD_IEHP_MULTI_LINEAR_FORCE));
// 	aUnit.Add(0);
// 	aUnit.Add(CUnitCtrl::m_IEHP_UNIT.PROPMulti.dDisp);
// 	aUnit.Add(CUnitCtrl::m_IEHP_UNIT.PROPMulti.dForc);
// 
// 	CString csTitle, csUnit;
// 	int aSize[] = {43, 90, 90};
// 	int nColCount = GetColCount();
// 
// 	for (int i = 0; i < nColCount; i++)
// 	{
// 		CUnitCtrl::GetUnitSystem(aUnit[i], csUnit);
// 
// 		if (csUnit.IsEmpty()) csTitle = aTitle[i];
// 		else csTitle = aTitle[i] + _T("\n(")+csUnit+_T(")");
// 		SetStyleRange(CGXRange(0, i+1), CGXStyle()
// 			.SetValue(csTitle)
// 			.SetUserAttribute(1/*ID_USER_ATTR_COL_ID*/, aTitle[i]));
// 		if (bInit) SetColWidth(i+1, i+1, aSize[i]);
// 	}
// 
// 	if (bInit) SetColWidth(0, 0, 20);
// 
// }
// 
// void CIehpProp_PMMLinGrid::DBChanged()
// {
// 	CIehpProp_PMmcDlg* pDlg = (CIehpProp_PMmcDlg*)GetParent();
// 	if (!pDlg) return;
// 	if (pDlg->GetSafeHwnd() == 0) return;
// 	if (!IsWindow(pDlg->GetSafeHwnd())) return;
// 
// 	Grid2Data();
// 	pDlg->OnDataChanged(); 
// }
// 
// 
// void CIehpProp_PMMLinGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
// {
// 	CGXGridWnd::OnModifyCell(nRow, nCol);
// }
// 
// 
// BOOL CIehpProp_PMMLinGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
// {
// 	CString strValue ;
// 	double dData  = 0.0;
// 	double dData2 = 0.0;
// 	double dData3 = 0.0;
// 
// 	BOOL bChange=FALSE;
// 	if(m_nSymmetry==0) // Sym
// 	{
// 		if(nRow == 4 && nCol == 2) bChange=TRUE;
// 		if(nRow == 4 && nCol == 3) bChange=TRUE;
// 		if(nRow == 5 && nCol == 2) bChange=TRUE;
// 		if(nRow == 5 && nCol == 3) bChange=TRUE;
// 
// 		if(bChange)
// 		{
// 			GetParam()->SetLockReadOnly(FALSE);
// 			SetStyleRange(CGXRange(1, 2, 5, 3), CGXStyle().SetReadOnly(FALSE).SetEnabled(TRUE));
// 
// 			strValue = GetValueRowCol(nRow, nCol);
// 			ConvertFormulaToValue(strValue);
// 			dData = _tstof(strValue); //CMathFunc::mathAbs(_tstof(strValue));
// 
// 			if(nRow == 4 && nCol == 2) SetValueRange(CGXRange(2, 2), -dData);
// 			if(nRow == 4 && nCol == 3) SetValueRange(CGXRange(2, 3), -dData);
// 			if(nRow == 5 && nCol == 2) SetValueRange(CGXRange(1, 2), -dData);
// 			if(nRow == 5 && nCol == 3) SetValueRange(CGXRange(1, 3), -dData);
// 
// 			SetStyleRange(CGXRange(1, 2, 3, 3), CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));
// 			GetParam()->SetLockReadOnly(TRUE);
// 		}
// 	}
// 	else
// 	{
// 		if(nRow == 4 && nCol == 2) bChange=TRUE;
// 		if(nRow == 4 && nCol == 3) bChange=TRUE;
// 		if(nRow == 2 && nCol == 3) bChange=TRUE;
// 
// 		if(bChange)
// 		{
// 			GetParam()->SetLockReadOnly(FALSE);
// 			SetStyleRange(CGXRange(1, 2, 5, 3), CGXStyle().SetReadOnly(FALSE).SetEnabled(TRUE));
// 
// 			strValue = GetValueRowCol(4, 3); ConvertFormulaToValue(strValue);
// 			dData    = _tstof(strValue); 
// 			strValue = GetValueRowCol(4, 2); ConvertFormulaToValue(strValue);
// 			dData2   = _tstof(strValue); 
// 			strValue = GetValueRowCol(2, 3); ConvertFormulaToValue(strValue);
// 			dData3   = _tstof(strValue); 
// 
// 			double dForc = dData;
// 			double dDisp = dData2;
// 			double dStiff = (dDisp==0.0 )? 0.0 : dForc / dDisp;
// 			double dDisp2 = (dStiff==0.0)? 0.0 : dData3 / dStiff;
// 			SetValueRange(CGXRange(2, 2), dDisp2);
// 
// 			SetStyleRange(CGXRange(2, 2, 3, 2), CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));
// 			SetStyleRange(CGXRange(3, 3, 3, 3), CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));
// 			GetParam()->SetLockReadOnly(TRUE);
// 		}
// 
// 	}
// 
// 	CGXGridWnd::OnEndEditing(nRow, nCol);
// 	DBChanged();
// 	return TRUE;
// }
// 
// BOOL CIehpProp_PMMLinGrid::OnPasteFromClipboard(const CGXRange &range)
// {
// 	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
// 	CString csErrMsg;
// 	CRowColArray awRows, awCols;
// 	int nTotalRow, nTotalCol;
// 
// 	CGXRange r;
// 	r.top = r.bottom = 1;
// 	r.left = r.right = 1;
// 	CGXGridWnd x;
// 	x.Create(WS_CHILD, CRect(0, 0, 1, 1), this, 0);
// 	x.Initialize();
// 	x.SetRowCount(0); x.SetColCount(1);
// 	x.OnPasteFromClipboard(r);
// 
// 	r.top = 1; r.left = 1;
// 	r.bottom = x.GetRowCount(); r.right = x.GetColCount();
// 	int nPastedRows = r.bottom - r.top + 1;   
// 	int nPastedCols = r.right - r.left + 1;  
// 
// 	if (nPastedCols > 3) // Total Col_No without Header  
// 	{
// 		csErrMsg = _LS(IDS_DB_STDG_GRID_ERR_MSG5);
// 		goto PASTE_ERROR;
// 	}
// 
// 	nTotalRow = GetRowCount();
// 	nTotalCol = GetColCount();
// 
// 	GetSelectedRows(awRows);
// 	GetSelectedCols(awCols);
// 
// 	if (awRows.GetSize() > 1) // Select된 것이 2줄 이상
// 	{
// 		if (nPastedRows != 1)
// 		{
// 			if (awRows.GetSize() != nPastedRows)
// 			{
// 				csErrMsg = _LS(IDS_DB_STDG_GRID_ERR_MSG2);
// 				goto PASTE_ERROR;
// 			}
// 		}
// 	}
// 	else 
// 	{
// 		int nTgtRow;
// 		if (awRows.GetSize() == 0)  
// 		{
// 			ROWCOL ncRow, ncCol;
// 			if (!GetCurrentCell(ncRow, ncCol))  
// 			{
// 				csErrMsg = _LS(IDS_DB_STDG_GRID_ERR_MSG4);
// 				goto PASTE_ERROR;
// 			}
// 			nTgtRow = ncRow;
// 		}
// 		else nTgtRow = awRows[0];
// 
// 		if (nTgtRow + nPastedRows - 1 > nTotalRow)
// 		{
// 			csErrMsg = _LS(IDS_DB_STDG_GRID_ERR_MSG5);
// 			goto PASTE_ERROR;
// 		}
// 
// 		if(awCols[0] + nPastedCols - 1 > nTotalCol)
// 		{
// 			csErrMsg = _LS(IDS_DB_STDG_GRID_ERR_MSG5);
// 			goto PASTE_ERROR;
// 		}
// 	}
// 
// 	x.DestroyWindow();
// 
// 	CGXGridWnd::OnPasteFromClipboard(range);
// 	//(2008.07.17) Add by Unsang!! 출력포멧갱신을 위해 해당 동작을 수행함!!//
// 	ROWCOL nRow, nCol;
// 	GetCurrentCell(nRow, nCol);
// 	Grid2Data();
// 	Data2Grid(); 
// 	SetCurrentCell(nRow, nCol);
// 	/////////////////////////////////////////////////////////////////////////  
// 	return TRUE;   
// 
// PASTE_ERROR:
// 	SetWarningText(csErrMsg);
// 	DisplayWarningText();
// 	x.DestroyWindow();
// 	return FALSE;
// }
// 
// // Delete 를 막아줌 
// BOOL CIehpProp_PMMLinGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
// {
// 	CRowColArray awRows;
// 	ROWCOL nRow=1, nCol=1;
// 
// 	//(2008.01.02) Add by Unsang :: Delete를 막아줌!!
// 	if((nMessage == WM_KEYDOWN) && (nChar == VK_DELETE)) return TRUE;
// 
// 	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
// 		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
// 
// 	int nErrCode = 0;   
// 	CString strCurrentValue = GetValueRowCol(nRow, nCol);
// 	BOOL bIsValid = TRUE;
// 	if(nCol == 2 || nCol == 3)  bIsValid = IsValidDoubleNumber(strCurrentValue, nErrCode); // nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
// 
// 	if(nMessage == WM_KEYDOWN)
// 	{
// 		switch(nChar)
// 		{
// 		case VK_TAB: 
// 			{
// 				if(!bIsValid)     { /*ErrorMessageOut(nErrCode);*/ return TRUE; }
// 				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
// 				if( bShift && SetLeftValidCell(nRow,  nCol)) goto GT_PROC_RETURN;
// 				if(!bShift && SetRightValidCell(nRow, nCol)) goto GT_PROC_RETURN;
// 
// 				CWnd* pWnd = GetParent();
// 				if(pWnd && pWnd->GetSafeHwnd())
// 				{
// 					CWnd* pNextWnd = pWnd->GetNextDlgTabItem(this, bShift);
// 					if(pNextWnd && pNextWnd->GetSafeHwnd()) pNextWnd->SetFocus();
// 				}
// 			}
// 			break;
// 		case VK_RETURN:
// 			if(!bIsValid)     { /*ErrorMessageOut(nErrCode);*/ return TRUE; }
// 			if(SetReturnValidCell(nRow, nCol)) goto GT_PROC_RETURN;
// 			break;
// 		case VK_RIGHT:
// 			if(!bIsValid)     { /*ErrorMessageOut(nErrCode);*/ return TRUE; }
// 			if(SetRightValidCell(nRow, nCol)) goto GT_PROC_RETURN;
// 			break;
// 		case VK_LEFT:
// 			if(!bIsValid)     { /*ErrorMessageOut(nErrCode);*/ return TRUE; }
// 			if(SetLeftValidCell(nRow, nCol)) goto GT_PROC_RETURN;
// 			break;
// 		default: break;
// 		}
// 	}
// 
// 	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
// 
// GT_PROC_RETURN:
// 	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
// 	{
// 		Redraw();
// 	}
// 
// 	return TRUE;
// 
// }
// 
// BOOL CIehpProp_PMMLinGrid::SetReturnValidCell(ROWCOL nRow, ROWCOL nCol)
// {
// 	CGXStyle style;
// 	WORD   nCtrlID;
// 	ROWCOL ncRow, ncCol;
// 	if(!GetCurrentCell(ncRow, ncCol)) goto GT_RCEL;
// 
// 	style = LookupStyleRowCol(nRow, nCol);
// 	//if(!GetStyleRowCol(nRow, nCol, style)) goto GT_RCEL;
// 
// 	nCtrlID = style.GetControl();
// 	if(nCtrlID==GX_IDS_CTRL_PUSHBTN)
// 	{
// 		OnClickedButtonRowCol(nRow, nCol);
// 		return TRUE;
// 	}
// 	else goto GT_RCEL;
// 
// 	//nCtrlID==GX_IDS_CTRL_CBS_DROPDOWNLIST//GX_IDS_CTRL_TABBED_COMBOBOX
// 
// 	return FALSE;
// 
// GT_RCEL:
// 
// 	return SetDownValidCell(nRow, nCol);
// //   return (m_nReturnDir==0) ? SetRightValidCell(nRow, nCol) : SetDownValidCell(nRow, nCol);
// //   return (m_nReturnDir==0) ? SetDownValidCell(nRow, nCol) : SetRightValidCell(nRow, nCol);
// }
// 
// BOOL CIehpProp_PMMLinGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
// {
// 	long i=nRow+1, j=nCol; // 다음 행, 현재 열
// 	if(i>GetRowCount()) { i=m_nHeadRow+1; j=nCol+1; }
// 
// 	BOOL bCovered = FALSE;
// 	while(j<=GetColCount())
// 	{
// 		while(i<=GetRowCount())
// 		{
// 			// 열이 선택될 수 있는지 조사
// 			CGXStyle style; style.Free();
// 			ComposeStyleRowCol(i, j, &style);
// 
// 			// Is it a covered cell?
// 	    CGXRange cvr;
// 	    if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
// 	    {
// 		    if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
// 		    {
// 			    i = cvr.bottom+1;
// 			    bCovered = TRUE;
// 					continue;
// 		    }
// 	    }
// 			if(IsRowHidden(i)) { ++i; continue; }
// 			if(IsColHidden(j)) break;
// 
// 			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
// 			BOOL bReadOnly = style.GetReadOnly();
// 			if(bEnable && !bReadOnly)
// 			{
// 				if(SetCurrentCell(i, j)) return TRUE;
// 			}
// 			bCovered = TRUE;
// 			++i;  // 다음 행을 조사
// 		}
// 		++j;    // 다음 열로 이동
// 		i = 1;
// 	}
// 
// 	return FALSE;
// }
// 
// BOOL CIehpProp_PMMLinGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
// {
// 	long i=nRow, j=nCol+1; // 현재 행 다음 열
// 	if(j>GetColCount()) { ++i; j=1; }
// 
// 	BOOL bCovered = FALSE;
// 	while(i<=GetRowCount())
// 	{
// 		while(j<=GetColCount())
// 		{
// 			// 열이 선택될 수 있는지 조사
// 			CGXStyle style; style.Free();
// 			ComposeStyleRowCol(i, j, &style);
// 
// 			// Is it a covered cell?
// 	    CGXRange cvr;
// 	    if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
// 	    {
// 		    if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
// 		    {
// 			    i = cvr.bottom;
// 			    j = cvr.right+1;
// 					bCovered = TRUE;
// 					continue;
// 		    }
// 	    }
// 			if(IsRowHidden(i)) break;
// 			if(IsColHidden(j)) { ++j; continue; }
// 
// 			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
// 			BOOL bReadOnly = style.GetReadOnly();
// 			if(bEnable && !bReadOnly)
// 			{
// 				if(SetCurrentCell(i, j)) return TRUE;
// 			}
// 			bCovered = TRUE;
// 			++j;  // 다음 열을 조사
// 		}
// 		++i;    // 다음 줄로 이동
// 		j = 1;
// 	}
// 
// 	return FALSE;
// }
// 
// BOOL CIehpProp_PMMLinGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
// {
// 	long i=nRow, j=nCol-1; // 현재 행 이전 열
// 	if(nCol==0 || j<1) { --i; j=GetColCount(); }  
// 
// 	BOOL bCovered = FALSE;
// 	while(i>=1)
// 	{
// 		while(j>=1)
// 		{
// 			// 열이 선택될 수 있는지 조사
// 			CGXStyle style; style.Free();
// 			ComposeStyleRowCol(i, j, &style);
// 
// 			// Is it a covered cell?
// 	    CGXRange cvr;
// 	    if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
// 	    {
// 		    if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
// 		    {
// 					--i;
// 					j = GetColCount();
// 					//if(cvr.top==cvr.bottom || cvr.left==cvr.right) --i;
// 					bCovered = TRUE;
// 					continue;
// 		    }
// 	    }
// 			if(IsRowHidden(i)) break;
// 			if(IsColHidden(j)) { --j; continue; }
// 
// 			BOOL bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
// 			BOOL bReadOnly = style.GetReadOnly();
// 			if(bEnable && !bReadOnly)
// 			{
// 				if(GetCoveredCellsRowCol(i, j, cvr))
// 	      {
// 		      if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
// 		      {
// 			      j = cvr.left;
// 		      }
// 	      }
// 				if(SetCurrentCell(i, j)) return TRUE;
// 			}
// 			bCovered = TRUE;
// 			--j;  // 이전 열을 조사
// 		}
// 		--i;    // 이전 줄로 이동
// 		j = GetColCount();
// 	}
// 
// 	return FALSE;
// }
// 
// BOOL CIehpProp_PMMLinGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
// {
// 	BOOL bReturn;
// 	bReturn = CTBBrowserWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);
// 
// 	//COLORREF EnabledHeadColor = GDlgAdmin_GetColor(3);
// 	
// 	if(nRow == 0)
// 		style.SetHorizontalAlignment(DT_CENTER);
// 
// 	if(nCol == 1)
// 		//style.SetInterior(EnabledHeadColor)
// 		style.SetHorizontalAlignment(DT_CENTER)
// 				 .SetTextColor(m_Color.NormalModeFg);
// 	else
// 		return TRUE;
// 
// 	return bReturn;
// }
// 
// BOOL CIehpProp_PMMLinGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
// {
// 	CString value;
// 	value = GetValueRowCol(nRow, nCol);
// 
// 	if (!ValidateField(value, nCol)) return SetFieldValidation(FALSE);
// 	return SetFieldValidation(TRUE);
// }
// 
// BOOL CIehpProp_PMMLinGrid::ValidateField(CString& value, int nColID)
// {
// 	int nErrCode=0;
// 	if (nColID == 2 || nColID == 3) 
// 		IsValidDoubleNumber(value, nErrCode);
// 	else
// 		return TRUE;
// 
// 	switch(nErrCode)
// 	{
// 		case 1: SetWarningText(_LS(IDS_DGN_INVALID_FORMULA)); break;
// 		case 2: SetWarningText(_LS(IDS_DGN_CHARACTER_EXIST)); break;
// 		case 3: SetWarningText(_LS(IDS_DGN_BIG_VALUE)); break;
// 		case 4: SetWarningText(_LS(IDS_DGN__ADDD__Integer_Is_Required)); break;
// 	}
// 	if (nErrCode != 0)
// 		return FALSE;
// 
// 	return TRUE;
// }
// 
// const CString& CIehpProp_PMMLinGrid::GetValueRowCol(ROWCOL nRow, ROWCOL nCol)
// {
// 	static CString value;
// 	value = _T(""); // 초기화 한다.
// 
// 	// 요청된 셀이 현재 셀이 아닐 때
// 	ROWCOL ncRow, ncCol;
// 	GetCurrentCell(ncRow, ncCol);
// 	if (ncRow != nRow || ncCol != nCol)
// 		return CGXGridWnd::GetValueRowCol(nRow, nCol);
// 
// 	// 셀이 Active 상태가 아니거나 수정되지 않으면 원래 함수 결과 리턴
// 	CGXControl *pControl = GetControl(nRow, nCol);
// 	if (!pControl || !pControl->IsActive() || !pControl->GetModify())
// 		return CGXGridWnd::GetValueRowCol(nRow, nCol);
// 
// 	// 현재 셀이 Active 상태이고 수정이 되었으면 셀을 저장하고
// 	// 값을 가져온 후 셀을 이전 상태로 다시 복구한다.
// 	// 왜 이렇게 하냐 하면 Formula가 들어가면 셀에 저장되어야 값이
// 	// Evaluate되기 때문이다. 만약 Formula를 직접 Evaluate해 주면
// 	// 그냥 Control의 값을 리턴하면 된다. (땜빵 코드)
// 	// 그런데... One(Zero) based ComboBox의 경우는 저장되는 것이 숫자인 경우가
// 	// 있다. 이런 경우에는 저장/복구 과정을 통해야 한다.
// 	BOOL bCombo = pControl->IsKindOf(CONTROL_CLASS(CGXComboBox));
// 	if (GetSheetContext() == NULL && !bCombo) 
// 	{
// 		pControl->GetCurrentText(value);
// 		return value;
// 	}
// 
// 	CString cur, prev;
// 	int nStart, nEnd;
// 	BOOL bEdit;
// 
// 	bEdit = pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl));
// 	if (bEdit) ((CGXEditControl*)pControl)->GetSel(nStart, nEnd);
// 
// 	// 현재 텍스트를 cur 변수에 저장한다.
// 	pControl->GetCurrentText(cur);
// 	// 이전 값을 prev 변수에 저장한다.
// 	prev = GetExpressionRowCol(nRow, nCol);
// 	// 현재 값을 셀에 저장. 저장이 안 되면 _T("")  리턴 ???
// 	if (!pControl->Store()) 
// 	{
// 		pControl->SetCurrentText(cur);
// 		pControl->SetActive(TRUE);
// 		return value;
// 	}
// 	// 저장된 값을 가져온다.
// 	value = CGXGridWnd::GetValueRowCol(nRow, nCol);
// 	// 이전 값을 셀에 저장하기 위해 Control에 값 재 설정 : version1
// 	// 에러 요소 : Side Effect로 OnModifyCell이 호출되어 결국은 
// 	// CTBBrowserWnd::Edit까지 호출
// 	/*
// 	pControl->SetCurrentText(prev);
// 	// 셀에 값 설정
// 	pControl->Store();
// 	*/
// 	// 새로운 방법: SetExpressionRowCol을 그냥 호출하면 이전 값을 그대로
// 	// 복구할 수 있다. 셀에서 ESC를 쳐서 편집 취소하면 이전값으로 복구된다.
// 	CGXGridWnd::SetExpressionRowCol(nRow, nCol, prev); // do nothing...
// 
// 	// 편집중인 값을 컨트롤에 설정
// 	// pControl->SetCurrentText는 OnModifyCell을 호출하게 되므로 여기서
// 	// 다른 방법을 강구해야 한다.
// 	//pControl->SetCurrentText(cur);
// 	pControl->SetControlText(nRow, nCol, cur);
// 	pControl->SetActive(TRUE);
// 	if (bEdit) ((CGXEditControl*)pControl)->SetSel(nStart, nEnd);
// 
// 	return value;
// }

