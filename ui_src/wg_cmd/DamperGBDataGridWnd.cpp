// ACS_ResultGridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBDataGridWnd.h"
#include "DamperGBDataTableDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDamperGBDataGridWnd

CDamperGBDataGridWnd::CDamperGBDataGridWnd()
{
	m_aPrimaryKey.Add(0);
}

CDamperGBDataGridWnd::~CDamperGBDataGridWnd()
{
}


BEGIN_MESSAGE_MAP(CDamperGBDataGridWnd, CGXGridWnd)
	//{{AFX_MSG_MAP(CDamperGBDataGridWnd)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDamperGBDataGridWnd message handlers
void CDamperGBDataGridWnd::Initialize(int nCode, int nPropType, int nCategory, CString strInitName, int nRow)
{
	m_pDoc = CDBDoc::GetDocPoint();
	CTBBrowserWndEx::Initialize();

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);
	// 컬럼 갯수 설정	

	m_nPropType = nPropType;
	m_nCategory = nCategory;
	m_strClassify = strInitName;
	m_nCode = nCode;
	m_Code.Initialize();
	m_bCode = nCode < 0 ? FALSE : TRUE;

	if (m_bCode)
	{
		IDamperGBDBMgr::Inst()->GetCodeDB(m_Code);
		m_nRow = nRow;
		if (nPropType == 0 || nPropType == 1)
		{
			m_nCol = 8;
		}
		else if (nPropType == 2)
		{
			m_nCol = 4;
		}
		else
		{
			m_nCol = 5;
		}
		SetColCount(m_nCol);
		SetRowCount(m_nRow);
		SetTitle_Code(nPropType);
		SetHeaderTitle_Code(nPropType);
	}
	else
	{
		m_nRow = 1;
		if (nPropType == 0)
		{
			m_nCol = 6;
		}
		else if (nPropType == 1)
		{
			m_nCol = 5;
		}
		else if (nPropType == 2)
		{
			m_nCol = 4;
		}
		else if (nPropType == 3 || nPropType == 5)
		{
			m_nCol = 6;
		}
		else if (nPropType == 4)
		{
			m_nCol = 7;
		}
		RemoveRows(0, 0);
		SetColCount(m_nCol);
		SetRowCount(m_nRow);
		SetTitle_User(nPropType);
		SetHeaderTitle_User(nPropType);
	}
	

	if (m_bCode)
		Data2Grid();
	Redraw();

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
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_STRING)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0));

	SetStyleRange(CGXRange().SetCols(3, m_nCol), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	SetRowHeight(0, 0, 36);
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetEnabled(FALSE));

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetScrollBarMode(SB_HORZ, gxnEnabled);
	SetScrollBarMode(SB_VERT, gxnEnabled);

	GetParam()->EnableMoveRows(TRUE);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	if (m_bCode)
	{
		SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
			.SetReadOnly(TRUE));
	}
	else
	{
		SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
			.SetReadOnly(FALSE));
	}

	LockUpdate(FALSE);
	Redraw();
}

void CDamperGBDataGridWnd::SetHeaderTitle_Code(int nPropType)
{
	int nColWidth[8] = { 30, 120, 120, 120, 120, 120, 120, 120 };
	CString csTitle;
	int nColCount = 0;
	switch (nPropType)
	{
	case 0: nColCount = 8; break;
	case 1: nColCount = 8; break;
	case 2: nColCount = 5; break;
	case 4: nColCount = 6; break;
	case 5: nColCount = 6; break;
	default:
		break;
	}

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));

	for (int i = 0; i < nColCount; i++)
	{
		csTitle = m_aTitle[i];
		double dWidth = nColWidth[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		SetColWidth(i + 1, i + 1, dWidth);
	}
	SetColWidth(0, 0, 0);

}

void CDamperGBDataGridWnd::SetHeaderTitle_User(int nPropType)
{
	int nColWidth[7] = { 30, 120, 120, 120, 120, 120, 120};
	CString csTitle;
	int nColCount = 0;
	switch (nPropType)
	{
	case 0: nColCount = 6; break;
	case 1: nColCount = 5; break;
	case 2: nColCount = 4; break;
	case 3: nColCount = 6; break;
	case 4: nColCount = 7; break;
	case 5: nColCount = 6; break;
	default:
		break;
	}

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));

	for (int i = 0; i < nColCount; i++)
	{
		csTitle = m_aTitle[i];
		double dWidth = nColWidth[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		SetColWidth(i + 1, i + 1, dWidth);
	}
	SetColWidth(0, 0, 0);

}

void CDamperGBDataGridWnd::GetTitleUnitName(CString &strUnit_L, CString &strUnit_F)
{
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int iLenID = CurIndex.nBase_Length;
	int iForID = CurIndex.nBase_Force;

	switch (iLenID)
	{
	case (D_UNITSYS_LENGTH_INDEX_MM): strUnit_L = D_UNITSYS_LENGTH_NAME_MM; break;
	case (D_UNITSYS_LENGTH_INDEX_CM): strUnit_L = D_UNITSYS_LENGTH_NAME_CM; break;
	case (D_UNITSYS_LENGTH_INDEX_M):  strUnit_L = D_UNITSYS_LENGTH_NAME_M;  break;
	case (D_UNITSYS_LENGTH_INDEX_IN): strUnit_L = D_UNITSYS_LENGTH_NAME_IN; break;
	case (D_UNITSYS_LENGTH_INDEX_FT): strUnit_L = D_UNITSYS_LENGTH_NAME_FT; break;
	default:
		break;
	}

	switch (iForID)
	{
	case (D_UNITSYS_FORCE_INDEX_KG): strUnit_F = D_UNITSYS_FORCE_NAME_KG; break;
	case (D_UNITSYS_FORCE_INDEX_TON):strUnit_F = D_UNITSYS_FORCE_NAME_TON; break;
	case (D_UNITSYS_FORCE_INDEX_N):  strUnit_F = D_UNITSYS_FORCE_NAME_N;  break;
	case (D_UNITSYS_FORCE_INDEX_KN): strUnit_F = D_UNITSYS_FORCE_NAME_KN; break;
	case (D_UNITSYS_FORCE_INDEX_LBF): strUnit_F = D_UNITSYS_FORCE_NAME_LBF; break;
	case (D_UNITSYS_FORCE_INDEX_KIP): strUnit_F = D_UNITSYS_FORCE_NAME_KIP; break;
	default:
		break;
	}
}

void CDamperGBDataGridWnd::SetTitle_Code(int nPropType)
{
	m_aTitle.RemoveAll();
	CString strUnit_L, strUnit_F;
	GetTitleUnitName(strUnit_L, strUnit_F);

	CString strYF, strL, strInitStiff, strYieldDisp, strPreYieldStiff, strStartDisp, strStartForce, strYF100, strEquiStiff, strDesignF,strStorStiff;
	strYF.Format(_LS(IDS_CMD_DMGB_YIELD_FORCE), strUnit_F);
	strL.Format(_LS(IDS_CMD_DMGB_LENGTH), strUnit_L);
	strInitStiff.Format(_LS(IDS_CMD_DMGB_INIT_STIFF), strUnit_F, strUnit_L);
	strYieldDisp.Format(_LS(IDS_CMD_DMGB_YIELD_DISP), strUnit_L);
	strPreYieldStiff.Format(_LS(IDS_CMD_DMGB_INIT_YIELD), strUnit_F, strUnit_L);
	strStartDisp.Format(_LS(IDS_CMD_DMGB_START_DISP), strUnit_L);
	strStartForce.Format(_LS(IDS_CMD_DMGB_START_FORCE), strUnit_F);
	strYF100.Format(_LS(IDS_CMD_DMGB_YIELD_FORCE_100), strUnit_F);
	strEquiStiff.Format(_LS(IDS_CMD_DMGB_DAMP_STIFF), strUnit_F, strUnit_L);
	strDesignF.Format(_LS(IDS_CMD_DMGB_DEFINE_DAMP), strUnit_F);
	strStorStiff.Format(_LS(IDS_CMD_DMGB_STIFF), strUnit_F, strUnit_L);

	CString aTitle1[8] = { _LS(IDS_WG_CMD__ADDD__No),  _LS(IDS_CMD_DMGB_SPEC), strYF, strL , strInitStiff, strYieldDisp,
		_LS(IDS_CMD_DMGB_YIELD_RTO_Q235B),  _LS(IDS_CMD_DMGB_YIELD_RTO_LOW) };

	CString aTitle2[8] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC),strYF, strPreYieldStiff, strYieldDisp ,
		_LS(IDS_CMD_DMGB_YIELD_RTO_LY225),	_LS(IDS_CMD_DMGB_YIELD_RTO_LY160), _LS(IDS_CMD_DMGB_YIELD_RTO_LY100) };

	CString aTitle3[5] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC), strStartDisp, strStartForce, strInitStiff };

	CString aTitle5[6] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC), strYF100, _LS(IDS_CMD_DMGB_YIELD_RTO), _LS(IDS_CMD_DMGB_DAMP_RTO) ,strEquiStiff };

	CString aTitle6[6] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC),  strDesignF, _LS(IDS_CMD_DMGB_DAMP_COEF), _LS(IDS_CMD_DMGB_ALPHA), strStorStiff };

	// Set Title
	int i = 0;
	if (nPropType == 0)
	{
		for (i = 0; i < 8; i++)
		{
			m_aTitle.Add(aTitle1[i]);
		}
	}
	else if (nPropType == 1)
	{
		for (i = 0; i < 8; i++)
		{
			m_aTitle.Add(aTitle2[i]);
		}
	}
	else if (nPropType == 2)
	{
		for (i = 0; i < 5; i++)
		{
			m_aTitle.Add(aTitle3[i]);
		}
	}
	else if (nPropType == 4)
	{
		for (i = 0; i < 6; i++)
		{
			m_aTitle.Add(aTitle5[i]);
		}
	}
	else if (nPropType == 5)
	{

		for (i = 0; i < 6; i++)
		{
			m_aTitle.Add(aTitle6[i]);
		}
	}
}

void CDamperGBDataGridWnd::SetTitle_User(int nPropType)
{
	m_aTitle.RemoveAll();
	CString strUnit_L, strUnit_F;
	GetTitleUnitName(strUnit_L, strUnit_F);

	CString strYF, strL, strInitStiff, strYieldDisp, strPreYieldStiff, strStartDisp, strStartForce, strYF100, strEquiStiff, strDesignF, strStorStiff;
	CString strMaxForce, strDampIndex, strDgnDisp;
	strYF.Format(_LS(IDS_CMD_DMGB_YIELD_FORCE), strUnit_F);
	strL.Format(_LS(IDS_CMD_DMGB_LENGTH), strUnit_L);
	strInitStiff.Format(_LS(IDS_CMD_DMGB_INIT_STIFF), strUnit_F, strUnit_L);
	strYieldDisp.Format(_LS(IDS_CMD_DMGB_YIELD_DISP), strUnit_L);
	strPreYieldStiff.Format(_LS(IDS_CMD_DMGB_INIT_YIELD), strUnit_F, strUnit_L);
	strStartDisp.Format(_LS(IDS_CMD_DMGB_START_DISP), strUnit_L);
	strStartForce.Format(_LS(IDS_CMD_DMGB_START_FORCE), strUnit_F);
	strYF100.Format(_LS(IDS_CMD_DMGB_YIELD_FORCE_100), strUnit_F);
	strEquiStiff.Format(_LS(IDS_CMD_DMGB_DAMP_EQUISTIFF), strUnit_F, strUnit_L);
	strDesignF.Format(_LS(IDS_CMD_DMGB_DEFINE_DAMP), strUnit_F);
	strStorStiff.Format(_LS(IDS_CMD_DMGB_STIFF), strUnit_F, strUnit_L);
	strDampIndex.Format(_LS(IDS_CMD_DMGB_DAMP_COEF), strUnit_F);
	strDgnDisp.Format(_LS(IDS_CMD_DMGB_DESIGN_DISP), strUnit_L);
	strMaxForce.Format(_LS(IDS_CMD_DMGB_MAX_FORCE), strUnit_F);

	CString aTitle1[6] = { _LS(IDS_WG_CMD__ADDD__No),  _LS(IDS_CMD_DMGB_SPEC), strYF, strL , strInitStiff, _LS(IDS_CMD_DMGB_YIELD_RTO)};

	CString aTitle2[5] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC),strYF, strPreYieldStiff, _LS(IDS_CMD_DMGB_YIELD_RTO) };

	CString aTitle3[4] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC), strStartForce, strInitStiff };

	CString aTitle4[6] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC), strDampIndex, _LS(IDS_CMD_DMGB_ALPHA), strDesignF, strMaxForce };

	CString aTitle5[7] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC), strYF100, _LS(IDS_CMD_DMGB_INIT_YIELD), _LS(IDS_CMD_DMGB_YIELD_RTO), _LS(IDS_CMD_DMGB_DAMP_RTO) ,strEquiStiff};

	CString aTitle6[6] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_DMGB_SPEC), strDesignF, strDampIndex, _LS(IDS_CMD_DMGB_ALPHA), strStorStiff };
	// Set Title
	int i = 0;
	if (nPropType == 0)
	{
		for (i = 0; i < 6; i++)
		{
			m_aTitle.Add(aTitle1[i]);
		}
	}
	else if (nPropType == 1)
	{
		for (i = 0; i < 5; i++)
		{
			m_aTitle.Add(aTitle2[i]);
		}
	}
	else if (nPropType == 2)
	{
		for (i = 0; i < 4; i++)
		{
			m_aTitle.Add(aTitle3[i]);
		}
	}
	else if (nPropType == 3)
	{
		for (i = 0; i < 6; i++)
		{
			m_aTitle.Add(aTitle4[i]);
		}
	}
	else if (nPropType == 4)
	{
		for (i = 0; i < 7; i++)
		{
			m_aTitle.Add(aTitle5[i]);
		}
	}
	else if (nPropType == 5)
	{

		for (i = 0; i < 6; i++)
		{
			m_aTitle.Add(aTitle6[i]);
		}
	}
}


void CDamperGBDataGridWnd::Data2Grid()
{
	CString strType = _T("");
	T_DMGB_D data;
	GetItemNameKey();
	double dDampCoef[4] = { 50,100,150,200 };
	for (int k = 0; k < m_nRow; k++)
	{
		strType.Format(_T("%d"), k + 1);
		SetValueRange(CGXRange(k + 1, 1), strType);
		data.Initialize();
		if (m_nPropType == 0)
		{
			data.BRB.Initialize();
			m_Code.m_mapCodeBRB.Lookup(m_aNameKey[k], data.BRB);
			data.nPropType = 0;
			m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(data);
			strType = data.BRB.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.BRB.dFy);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.BRB.dLength);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.BRB.dInitStiff);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.4f"), data.BRB.dDisp);
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.3f"), 0.035);
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.3f"), 0.025);
			SetValueRange(CGXRange(k + 1, 8), strType);
		}
		else if (m_nPropType == 1)
		{
			data.MYD.Initialize();
			m_Code.m_mapCodeMYD.Lookup(m_aNameKey[k], data.MYD);
			data.nPropType = 1;
			m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(data);
			strType = data.MYD.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.MYD.dFy);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.MYD.dInitStiff);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.4f"), data.MYD.dDisp);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.3f"), 0.025);
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.3f"), 0.035);
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.3f"), 0.05);
			SetValueRange(CGXRange(k + 1, 8), strType);
		}
		else if (m_nPropType == 2)
		{
			data.FD.Initialize();
			m_Code.m_mapCodeFD.Lookup(m_aNameKey[k], data.FD);
			data.nPropType = 2;
			m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(data);
			strType = data.FD.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.4f"), data.FD.dDisp);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.FD.dForce);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.FD.dInitStiff);
			SetValueRange(CGXRange(k + 1, 5), strType);
		}
		else if (m_nPropType == 4)
		{
			data.HDRD.Initialize();
			m_Code.m_mapCodeHDRD.Lookup(m_aNameKey[k], data.HDRD);
			data.nPropType = 4;
			m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(data);
			strType = data.HDRD.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.HDRD.dFy);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.HDRD.dYieldStiffRot);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.HDRD.dEquiDampRto);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.HDRD.dEquiStiff);
			SetValueRange(CGXRange(k + 1, 6), strType);
		}
		else if (m_nPropType == 5)
		{
			data.VED.Initialize();
			m_Code.m_mapCodeVED.Lookup(m_aNameKey[k], data.VED);
			data.nPropType = 5;
			m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(data);
			strType = data.VED.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.VED.dDesignF);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.0f"), dDampCoef[k]);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.VED.dIndex);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.VED.dEffectStiff);
			SetValueRange(CGXRange(k + 1, 6), strType);
		}
		
	}
}

void CDamperGBDataGridWnd::GetItemNameKey()
{
	int j;
	CString strKey;
	m_aNameKey.RemoveAll();

	CStringArray astrInputType;
	astrInputType.RemoveAll();
	IDamperGBDBMgr::Inst()->GetDBInputTypeList(m_nPropType, m_nCategory, astrInputType);
	if (astrInputType.GetCount() < 1)
		return;

	if (m_nPropType == 0)
	{
		for (j = 0; j < 6; j++)
		{
			strKey = m_strClassify + astrInputType[j];
			if (m_Code.m_mapCodeBRB.Lookup(strKey, DmgbD.BRB))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 1)
	{
		for (j = 0; j < 2; j++)
		{
			strKey = m_strClassify + astrInputType[j];
			if (m_Code.m_mapCodeMYD.Lookup(strKey, DmgbD.MYD))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 2)
	{
		for (j = 0; j < 6; j++)
		{
			strKey = astrInputType[j];
			if (m_Code.m_mapCodeFD.Lookup(strKey, DmgbD.FD))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 4)
	{
		for (j = 0; j < 5; j++)
		{
			strKey = astrInputType[j];
			if (m_Code.m_mapCodeHDRD.Lookup(strKey, DmgbD.HDRD))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 5)
	{
		for (j = 0; j < 4; j++)
		{
			strKey =astrInputType[j];
			if (m_Code.m_mapCodeVED.Lookup(strKey, DmgbD.VED))
				m_aNameKey.Add(strKey);
		}
	}
}

void CDamperGBDataGridWnd::Grid2Data(T_DMGB_D &data)
{
	ROWCOL nRow, nCol;
	if (!GetCurrentCell(nRow, nCol))
		return;

	DmgbD.Initialize();
	CString strKey = m_aNameKey[nRow - 1];
	int nIndex = nRow - 1;

	if (m_bCode)
	{
		if (m_nPropType == 0)
		{
			m_Code.m_mapCodeBRB.Lookup(strKey, data.BRB);
			data.BRB.nCode = m_nCode;
		}
		else if (m_nPropType == 1)
		{
			m_Code.m_mapCodeMYD.Lookup(strKey, data.MYD);
			data.MYD.nCode = m_nCode;
		}
		else if (m_nPropType == 2)
		{
			m_Code.m_mapCodeFD.Lookup(strKey, data.FD);
			data.FD.nCode = m_nCode;
		}
		else if (m_nPropType == 3)
		{
			m_Code.m_mapCodeVFD.Lookup(strKey, data.VFD);
			data.VFD.nCode = m_nCode;
		}
		else if (m_nPropType == 4)
		{
			m_Code.m_mapCodeHDRD.Lookup(strKey, data.HDRD);
			data.HDRD.nCode = m_nCode;
		}
		else if (m_nPropType == 5)
		{
			m_Code.m_mapCodeVED.Lookup(strKey, data.VED);
			data.VED.nCode = m_nCode;
		}
	}
	
	data.nPropType = m_nPropType;
	m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(data);
}

BOOL CDamperGBDataGridWnd::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bReturn;
	bReturn = CGXGridWnd::OnLButtonDblClkRowCol(nRow, nCol, nFlags, pt);

	if (nRow < 1)
		return bReturn;

	CString strKey = m_aNameKey[nRow - 1];
	int nIndex = nRow - 1;

	T_DMGB_D DmgbD; DmgbD.Initialize();
	Grid2Data(DmgbD);
	((CDamperGBDataTableDlg*)(GetParent()))->OnLButtonDblClkOnGrid(DmgbD);
	return bReturn;
}

BOOL CDamperGBDataGridWnd::SaveUserDefineData2Ini(CString strClassify)
{
	if (!SaveData(strClassify))
		return FALSE;

	CString csFolder = IDamperGBDBMgr::Inst()->GetDamperFileFolder(m_nPropType);
	CString csFileName = csFolder + _T("\\") + strClassify + _T(".ini");
	LPCTSTR szFileName;
	if (csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;

	if (strClassify.Compare(_T("")) == 0)
	{
		AfxMessageBox(_LS(IDS_CMD_LOAD_ISOLATION_CLASSIFY_ERROR), MB_OK);
		return FALSE;
	}
	if (CFileCtrl::FileExists(szFileName))
	{
		AfxMessageBox(_LS(IDS_CMD_LOAD_ISOLATION_EXIST_FILE), MB_OK);
		return FALSE;
	}

	if (IDamperGBDBMgr::Inst()->WriteDamperInfo(m_User, m_nPropType, csFileName, strClassify))
	{
		CDBLib::RunTextEditor(csFileName, _T(""));
		return TRUE;
	}

	return FALSE;
}


void CDamperGBDataGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetStyleRange(CGXRange(nRow, nCol), rstyle);
}

void CDamperGBDataGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_Cell(nRow, nCol, rstyle, strTemp);
}

void CDamperGBDataGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetCoveredCellsRowCol(nRow1, nCol1, nRow2, nCol2);
	SetStyleRange(CGXRange(nRow1, nCol1, nRow2, nCol2), rstyle);
}

void CDamperGBDataGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_CellMerge(nRow1, nCol1, nRow2, nCol2, rstyle, strTemp);
}

BOOL CDamperGBDataGridWnd::OnDeleteRecords(CRowColArray& awRows)
{
	if (!DeleteRecord(awRows))
	{
		SetWarningText(_T("Error : Can't delete data."));
		return FALSE;
	}
	CString strType;
	m_nRow = GetRowCount();
	for (int k = 0; k < m_nRow - 1; k++)
	{
		strType.Format(_T("%d"), k + 1);
		SetValueRange(CGXRange(k + 1, 1), strType);
	}
	return TRUE;
}

void CDamperGBDataGridWnd::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow, TRUE);
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SetNewLineNo(nRow, nCol);
}

void CDamperGBDataGridWnd::SetNewLineNo(ROWCOL nRow, ROWCOL nCol)
{
	CString strLineNo;
	strLineNo.Format(_T("%d"), nRow);
	SetValueRange(CGXRange(nRow, 1), strLineNo);
}


BOOL CDamperGBDataGridWnd::OnPasteFromClipboard(const CGXRange& range)
{
	if (CGXGridWnd::OnPasteFromClipboard(range))
	{
		int nRow = GetRowCount();
		for (int i = 1; i < nRow + 1; i++)
		{
			SetNewLineNo(i, 1);
		}
		return TRUE;
	}
	return FALSE;
}


BOOL CDamperGBDataGridWnd::SaveData(CString strClassify)
{
	int nModeNum = GetRowCount();
	m_User.Initialize();
	T_DMGB_D dmgbD;
	CString strKey;
	for (int i = 0; i < nModeNum; i++)
	{
		if (m_nPropType == 0)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				dmgbD.BRB.Initialize();
				dmgbD.BRB.strInputType = GetValueRowCol(i + 1, 2);
				if (dmgbD.BRB.strInputType.GetLength() < 1)
					continue;
				dmgbD.BRB.dFy = _ttof(GetValueRowCol(i + 1, 3));
				dmgbD.BRB.dLength = _ttof(GetValueRowCol(i + 1, 4));
				dmgbD.BRB.dInitStiff = _ttof(GetValueRowCol(i + 1, 5));
				dmgbD.BRB.dStiffRot = _ttof(GetValueRowCol(i + 1, 6));
				dmgbD.BRB.strClassify = strClassify;
				dmgbD.nPropType = m_nPropType;
				strKey = strClassify + dmgbD.BRB.strInputType;
				m_User.m_mapCodeBRB.SetAt(strKey, dmgbD.BRB);
			}
		}
		else if (m_nPropType == 1)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				dmgbD.MYD.Initialize();
				dmgbD.MYD.strInputType = GetValueRowCol(i + 1, 2);
				if (dmgbD.MYD.strInputType.GetLength() < 1)
					continue;
				dmgbD.MYD.dFy = _ttof(GetValueRowCol(i + 1, 3));
				dmgbD.MYD.dInitStiff = _ttof(GetValueRowCol(i + 1, 4));
				dmgbD.MYD.dStiffRot = _ttof(GetValueRowCol(i + 1, 5));
				dmgbD.MYD.strClassify = strClassify;
				dmgbD.nPropType = m_nPropType;
				strKey = strClassify + dmgbD.MYD.strInputType;
				m_User.m_mapCodeMYD.SetAt(strKey, dmgbD.MYD);
			}
		}
		else if (m_nPropType == 2)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				dmgbD.FD.Initialize();
				dmgbD.FD.strInputType = GetValueRowCol(i + 1, 2);
				if (dmgbD.FD.strInputType.GetLength() < 1)
					continue;
				dmgbD.FD.dForce = _ttof(GetValueRowCol(i + 1, 3));
				dmgbD.FD.dInitStiff = _ttof(GetValueRowCol(i + 1, 4));
				
				dmgbD.FD.strClassify = strClassify;
				dmgbD.nPropType = m_nPropType;
				strKey = strClassify + dmgbD.FD.strInputType;
				m_User.m_mapCodeFD.SetAt(strKey, dmgbD.FD);
			}
		}
		else if (m_nPropType == 3)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				dmgbD.VFD.Initialize();
				dmgbD.VFD.strInputType = GetValueRowCol(i + 1, 2);
				if (dmgbD.VFD.strInputType.GetLength() < 1)
					continue;
				dmgbD.VFD.dDamperF = _ttof(GetValueRowCol(i + 1, 3));
				dmgbD.VFD.dIndex = _ttof(GetValueRowCol(i + 1, 4));
				dmgbD.VFD.dDesignF = _ttof(GetValueRowCol(i + 1, 5));
				dmgbD.VFD.dDisp = _ttof(GetValueRowCol(i + 1, 6));
				dmgbD.VFD.strClassify = strClassify;
				dmgbD.nPropType = m_nPropType;
				strKey = strClassify + dmgbD.VFD.strInputType;
				m_User.m_mapCodeVFD.SetAt(strKey, dmgbD.VFD);
			}
		}
		else if (m_nPropType == 4)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				dmgbD.HDRD.Initialize();
				dmgbD.HDRD.strInputType = GetValueRowCol(i + 1, 2);
				if (dmgbD.HDRD.strInputType.GetLength() < 1)
					continue;
				dmgbD.HDRD.dFy = _ttof(GetValueRowCol(i + 1, 3));
				dmgbD.HDRD.dInitStiff = _ttof(GetValueRowCol(i + 1, 4));
				dmgbD.HDRD.dYieldStiffRot = _ttof(GetValueRowCol(i + 1, 5));
				dmgbD.HDRD.dEquiStiff = _ttof(GetValueRowCol(i + 1, 6));
				dmgbD.HDRD.dEquiDampRto = _ttof(GetValueRowCol(i + 1, 7));
				dmgbD.HDRD.strClassify = strClassify;
				dmgbD.nPropType = m_nPropType;
				strKey = strClassify + dmgbD.HDRD.strInputType;
				m_User.m_mapCodeHDRD.SetAt(strKey, dmgbD.HDRD);
			}
		}
		else if (m_nPropType == 5)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				dmgbD.VED.Initialize();
				dmgbD.VED.strInputType = GetValueRowCol(i + 1, 2);
				if (dmgbD.VED.strInputType.GetLength() < 1)
					continue;
				dmgbD.VED.dDesignF = _ttof(GetValueRowCol(i + 1, 3));
				dmgbD.VED.dDamperF = _ttof(GetValueRowCol(i + 1, 4));
				dmgbD.VED.dIndex = _ttof(GetValueRowCol(i + 1, 5));
				dmgbD.VED.dEffectStiff = _ttof(GetValueRowCol(i + 1, 6));
				dmgbD.VED.strClassify = strClassify;
				dmgbD.nPropType = m_nPropType;
				strKey = strClassify + dmgbD.VED.strInputType;
				m_User.m_mapCodeVED.SetAt(strKey, dmgbD.VED);
			}
		}
	}

	return TRUE;
}

CString CDamperGBDataGridWnd::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum;
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			const CGXStyle style = LookupStyleRowCol(nRow, nCol);
			if (pControl)
			{
				if (pControl->IsActive())
					bGet = pControl->GetValue(csNum);
				else if (style.GetIncludeControl() && style.GetControl() == GX_IDS_CTRL_CHECKBOX3D)
				{
					bGet = pControl->GetValue(csNum);
					if (csNum == _T("0")) csNum = _T("1");
					else csNum = _T("0");
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}