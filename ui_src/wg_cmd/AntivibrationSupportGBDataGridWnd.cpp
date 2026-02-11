// ACS_ResultGridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBDataGridWnd.h"
#include "AntivibrationSupportGBDataTableDlg.h"
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
// CAntivibrationSupportGBDataGridWnd

CAntivibrationSupportGBDataGridWnd::CAntivibrationSupportGBDataGridWnd()
{
	m_aPrimaryKey.Add(0);
}

CAntivibrationSupportGBDataGridWnd::~CAntivibrationSupportGBDataGridWnd()
{
}


BEGIN_MESSAGE_MAP(CAntivibrationSupportGBDataGridWnd, CGXGridWnd)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBDataGridWnd)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBDataGridWnd message handlers
void CAntivibrationSupportGBDataGridWnd::Initialize(int nPropType, int nCode, CString strInitName, int nRow)
{
	m_pDoc = CDBDoc::GetDocPoint();
	CTBBrowserWndEx::Initialize();

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // ÇöÀç ¼¿ÀÇ border ¼³Á¤ º¯°æ
		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);
	// ÄÃ·³ °¹¼ö ¼³Á¤	

	m_nPropType = nPropType;
	m_strClassify = strInitName;
	m_nCode = nCode;
	m_Code.Initialize();
	m_bCode = nCode < 0 ? FALSE : TRUE;
	if (m_nPropType < 2)
	{
		if (m_nCode == 0)
			IAntivibrationSupportGBDBMgr::Inst()->GetCodeDB(m_Code);
		else if (m_nCode == 1)
			IAntivibrationSupportGBDBMgr::Inst()->GetCodeDB_JGT(m_Code);
		else
			m_bCode = FALSE;
	}
	else
	{
		if (m_nCode == 0)
			IAntivibrationSupportGBDBMgr::Inst()->GetCodeDB(m_Code);
		else
			m_bCode = FALSE;
	}
		

	if (m_bCode)
	{
		m_nRow = nRow;
		if (nPropType == 0)
		{
			m_nCol = 5;
		}
		else if (nPropType == 1)
		{
			m_nCol = 9;
		}
		else if (nPropType == 2)
		{
			m_nCol = 9;
		}
		else if (nPropType == 3)
		{
			m_nCol = 6;
		}
		else if (nPropType == 4)
		{
			m_nCol = 7;
		}
	}
	else
	{
		m_nRow = 1;

		if (nPropType == 0)
		{
			m_nCol = 5;
		}
		else if (nPropType == 1)
		{
			m_nCol = 9;
		}
		else if (nPropType == 2)
		{
			m_nCol = 9;
		}
		else if (nPropType == 3)
		{
			m_nCol = 6;
		}
		else if (nPropType == 4)
		{
			m_nCol = 7;
		}
		RemoveRows(0, 0);
	}
	SetColCount(m_nCol + 1);
	SetRowCount(m_nRow);
	SetTitle(nPropType);
	SetHeaderTitle(nPropType);

	if (m_bCode)
		Code2Grid();
	Redraw();

	// Ç¥ÁØ ½ºÅ¸ÀÏ º¯°æ
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

	// ÄÃ·³ ½ºÅ¸ÀÏ º¯°æ
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_STRING)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0));

	SetStyleRange(CGXRange().SetCols(3, m_nCol+1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	SetRowHeightByDPI(0, 0, 36);
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetEnabled(FALSE));

	// ÇöÀç ¼¿ ¼³Á¤
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetScrollBarMode(SB_HORZ, gxnEnabled);
	SetScrollBarMode(SB_VERT, gxnEnabled);

	GetParam()->EnableMoveRows(TRUE);

	// Sorting Á¤º¸ ÃÊ±âÈ­
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

void CAntivibrationSupportGBDataGridWnd::SetHeaderTitle(int nPropType)
{
	int nColWidth[10] = { 30, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
	//int nColWidth4[8] = { 30, 150, 100, 100, 100, 100, 200,100 };
	CString csTitle;
	int nColCount = 0;
	switch (nPropType)
	{
	case 0: nColCount = 6; break;
	case 1: nColCount = 10; break;
	case 2: nColCount = 10; break;
	case 3: nColCount = 7; break;
	case 4: nColCount = 8; break;
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
		double dWidth = (!m_bCode && (nPropType == 1 || nPropType == 2)) ? 80 : nColWidth[i];
		if (i == 0)
			dWidth = nColWidth[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		SetColWidthByDPI(i + 1, i + 1, dWidth);
	}
	SetColWidthByDPI(0, 0, 0);

}

void CAntivibrationSupportGBDataGridWnd::GetTitleUnitName(CString &strUnit_L, CString &strUnit_F)
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

void CAntivibrationSupportGBDataGridWnd::SetTitle(int nPropType)
{
	m_aTitle.RemoveAll();
	CString strUnit_L, strUnit_F;
	GetTitleUnitName(strUnit_L, strUnit_F);

	CString strD, strKv, strKh, strTr, strK1, strKd, strQd, strInK1, strVB, strR, strHorD;
	strD.Format(_LS(IDS_CMD_ASGB_DATA_EFFECT_D), strUnit_L);
	strKv.Format(_LS(IDS_CMD_ASGB_DATA_Kv), strUnit_F, strUnit_L);
	strKh.Format(_LS(IDS_CMD_ASGB_DATA_Kh), strUnit_F, strUnit_L);
	strTr.Format(_LS(IDS_CMD_ASGB_DATA_TR), strUnit_L);
	strK1.Format(_LS(IDS_CMD_ASGB_DATA_K1), strUnit_F, strUnit_L);
	strKd.Format(_LS(IDS_CMD_ASGB_DATA_KD), strUnit_F, strUnit_L);
	strQd.Format(_LS(IDS_CMD_ASGB_DATA_QD), strUnit_F);
	strInK1.Format(_LS(IDS_CMD_ASGB_DATA_INIT_K1), strUnit_F, strUnit_L);
	strVB.Format(_LS(IDS_CMD_ASGB_DATA_VER_BEAT), strUnit_F);
	strR.Format(_LS(IDS_CMD_ASGB_DATA_R), strUnit_L);
	strHorD.Format(_LS(IDS_CMD_ASGB_DATA_HOR_DIST), strUnit_L);

	CString aTitle1[6] = { _LS(IDS_WG_CMD__ADDD__No),  _LS(IDS_CMD_ASGB_DATA_TYPE), strD, strKv , strKh, strTr };

	CString aTitle2[10] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_ASGB_DATA_TYPE),strD, strKv, strKh ,
		_LS(IDS_CMD_ASGB_DATA_DAMP_RATIO),	strTr, strK1 , strKd, strQd };

	CString aTitle3[10] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_ASGB_DATA_TYPE), strD, strKv, strKh ,
		_LS(IDS_CMD_ASGB_DATA_DAMP_RATIO),	strTr, strK1 , strKd, strQd };

	CString aTitle4[7] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_ASGB_DATA_TYPE), strD, strKv ,strInK1,
		_LS(IDS_CMD_ASGB_DATA_U), strTr };

	CString aTitle5[8] = { _LS(IDS_WG_CMD__ADDD__No),  _LS(IDS_CMD_ASGB_DATA_TYPE),	strVB, strKv,
		_LS(IDS_CMD_ASGB_DATA_US), _LS(IDS_CMD_ASGB_DATA_UF) , strR, strHorD };

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
		for (i = 0; i < 10; i++)
		{
			m_aTitle.Add(aTitle2[i]);
		}
	}
	else if (nPropType == 2)
	{
		for (i = 0; i < 10; i++)
		{
			m_aTitle.Add(aTitle3[i]);
		}
	}
	else if (nPropType == 3)
	{
		for (i = 0; i < 7; i++)
		{
			m_aTitle.Add(aTitle4[i]);
		}
	}
	else if (nPropType == 4)
	{

		for (i = 0; i < 8; i++)
		{
			m_aTitle.Add(aTitle5[i]);
		}
	}
}

void CAntivibrationSupportGBDataGridWnd::Code2Grid()
{
	CString strType = _T("");
	T_ASGB_D data;
	GetItemNameKey();
	for (int k = 0; k < m_nRow; k++)
	{
		strType.Format(_T("%d"), k + 1);
		SetValueRange(CGXRange(k + 1, 1), strType);
		data.Initialize();
		if (m_nPropType == 0)
		{
			data.LNR.Initialize();
			m_Code.m_mapCodeLNR.Lookup(m_aNameKey[k], data.LNR);
			data.nPropType = 0;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			//strType = data.LNR.strClassify;
			//SetValueRange(CGXRange(k + 1, 2), strType);
			strType = data.LNR.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.LNR.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.LNR.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.LNR.dKh);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.LNR.dTr);
			SetValueRange(CGXRange(k + 1, 6), strType);

		}
		else if (m_nPropType == 1)
		{
			data.LRB.Initialize();
			m_Code.m_mapCodeLRB.Lookup(m_aNameKey[k], data.LRB);
			data.nPropType = 1;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			//strType = data.LRB.strClassify;
			//SetValueRange(CGXRange(k + 1, 2), strType);
			strType = data.LRB.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.LRB.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.LRB.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.LRB.dHorEquiStiff);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.LRB.dEquiRot);
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.2f"), data.LRB.dTr);
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.2f"), data.LRB.dK1);
			SetValueRange(CGXRange(k + 1, 8), strType);
			strType.Format(_T("%.2f"), data.LRB.dKd);
			SetValueRange(CGXRange(k + 1, 9), strType);
			strType.Format(_T("%.2f"), data.LRB.dOd);
			SetValueRange(CGXRange(k + 1, 10), strType);
		}
		else if (m_nPropType == 2)
		{
			data.HDR.Initialize();
			m_Code.m_mapCodeHDR.Lookup(m_aNameKey[k], data.HDR);
			data.nPropType = 2;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.HDR.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.HDR.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.HDR.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.HDR.dHorEquiStiff);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.HDR.dEquiRot);
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.2f"), data.HDR.dTr);
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.2f"), data.HDR.dK1);
			SetValueRange(CGXRange(k + 1, 8), strType);
			strType.Format(_T("%.2f"), data.HDR.dKd);
			SetValueRange(CGXRange(k + 1, 9), strType);
			strType.Format(_T("%.2f"), data.HDR.dOd);
			SetValueRange(CGXRange(k + 1, 10), strType);
		}
		else if (m_nPropType == 3)
		{
			data.ESB.Initialize();
			m_Code.m_mapCodeESB.Lookup(m_aNameKey[k], data.ESB);
			data.nPropType = 3;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.ESB.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.ESB.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.ESB.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.ESB.dK1);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType = data.ESB.strU;
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.2f"), data.ESB.dTr);
			SetValueRange(CGXRange(k + 1, 7), strType);
		}
		else if (m_nPropType == 4)
		{
			data.FPS.Initialize();
			m_Code.m_mapCodeFPS.Lookup(m_aNameKey[k], data.FPS);
			data.nPropType = 4;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.FPS.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.FPS.dVBearing);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType = data.FPS.strKv;
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType = data.FPS.strUs;
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType = data.FPS.strUf;
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType = data.FPS.strR;
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.2f"), data.FPS.dHorDis);
			SetValueRange(CGXRange(k + 1, 8), strType);
		}
	}
}

/*void CAntivibrationSupportGBDataGridWnd::Data2Grid()
{
	CString strType = _T("");
	T_ASGB_D data;
	for (int k = 0; k < m_nRow; k++)
	{
		strType.Format(_T("%d"), k + 1);
		SetValueRange(CGXRange(k + 1, 1), strType);
		data.Initialize();
		if (m_nPropType == 0)
		{
			data.LNR.Initialize();
			m_User.m_mapCodeLNR.Lookup(k, data.LNR);
			data.nPropType = 0;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.LNR.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.LNR.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.LNR.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.LNR.dKh);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.LNR.dTr);
			SetValueRange(CGXRange(k + 1, 6), strType);

		}
		else if (m_nPropType == 1)
		{
			data.LRB.Initialize();
			m_User.m_mapCodeLRB.Lookup(k, data.LRB);
			data.nPropType = 1;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.LRB.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.LRB.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.LRB.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.LRB.dHorEquiStiff);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.LRB.dEquiRot);
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.2f"), data.LRB.dTr);
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.2f"), data.LRB.dK1);
			SetValueRange(CGXRange(k + 1, 8), strType);
			strType.Format(_T("%.2f"), data.LRB.dKd);
			SetValueRange(CGXRange(k + 1, 9), strType);
			strType.Format(_T("%.2f"), data.LRB.dOd);
			SetValueRange(CGXRange(k + 1, 10), strType);
		}
		else if (m_nPropType == 2)
		{
			data.HDR.Initialize();
			m_User.m_mapCodeHDR.Lookup(k, data.HDR);
			data.nPropType = 2;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.HDR.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.HDR.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.HDR.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.HDR.dHorEquiStiff);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.HDR.dEquiRot);
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.2f"), data.HDR.dTr);
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.2f"), data.HDR.dK1);
			SetValueRange(CGXRange(k + 1, 8), strType);
			strType.Format(_T("%.2f"), data.HDR.dKd);
			SetValueRange(CGXRange(k + 1, 9), strType);
			strType.Format(_T("%.2f"), data.HDR.dOd);
			SetValueRange(CGXRange(k + 1, 10), strType);
		}
		else if (m_nPropType == 3)
		{
			data.ESB.Initialize();
			m_User.m_mapCodeESB.Lookup(k, data.ESB);
			data.nPropType = 3;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.ESB.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.ESB.dD);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.ESB.dKv);
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.ESB.dK1);
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.ESB.dU);;
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.2f"), data.ESB.dTr);
			SetValueRange(CGXRange(k + 1, 7), strType);
		}
		else if (m_nPropType == 4)
		{
			data.FPS.Initialize();
			m_User.m_mapCodeFPS.Lookup(k, data.FPS);
			data.nPropType = 4;
			m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
			strType = data.FPS.strInputType;
			SetValueRange(CGXRange(k + 1, 2), strType);
			strType.Format(_T("%.2f"), data.FPS.dVBearing);
			SetValueRange(CGXRange(k + 1, 3), strType);
			strType.Format(_T("%.2f"), data.FPS.dKv);;
			SetValueRange(CGXRange(k + 1, 4), strType);
			strType.Format(_T("%.2f"), data.FPS.dUs);;
			SetValueRange(CGXRange(k + 1, 5), strType);
			strType.Format(_T("%.2f"), data.FPS.dUf);;
			SetValueRange(CGXRange(k + 1, 6), strType);
			strType.Format(_T("%.2f"), data.FPS.nR);;
			SetValueRange(CGXRange(k + 1, 7), strType);
			strType.Format(_T("%.2f"), data.FPS.dHorDis);
			SetValueRange(CGXRange(k + 1, 8), strType);
		}
	}
}*/

void CAntivibrationSupportGBDataGridWnd::GetItemNameKey()
{
	int j;
	CString strKey;
	m_aNameKey.RemoveAll();

	CStringArray astrInputType;
	astrInputType.RemoveAll();
	int nClassify = 0;
	if (m_strClassify == _T("FPS-I"))
		nClassify = 0;
	else
		nClassify = 1;
	IAntivibrationSupportGBDBMgr::Inst()->GetDBInputTypeList(m_nPropType, nClassify, astrInputType);
	if (astrInputType.GetCount() < 1)
		return;

	if (m_nPropType == 0)
	{
		for (j = 0; j < astrInputType.GetCount(); j++)
		{
			strKey = m_strClassify + astrInputType[j];
			if (m_Code.m_mapCodeLNR.Lookup(strKey, AsgbD.LNR))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 1)
	{
		for (j = 0; j < astrInputType.GetCount(); j++)
		{
			strKey = m_strClassify + astrInputType[j];
			if (m_Code.m_mapCodeLRB.Lookup(strKey, AsgbD.LRB))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 2)
	{
		for (j = 0; j < astrInputType.GetCount(); j++)
		{
			strKey = astrInputType[j];
			if (m_Code.m_mapCodeHDR.Lookup(strKey, AsgbD.HDR))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 3)
	{
		for (j = 0; j < astrInputType.GetCount(); j++)
		{
			strKey = m_strClassify + astrInputType[j];
			if (m_Code.m_mapCodeESB.Lookup(strKey, AsgbD.ESB))
				m_aNameKey.Add(strKey);
		}
	}
	else if (m_nPropType == 4)
	{
		if (nClassify == 0)
		{
			for (j = 0; j < astrInputType.GetCount(); j++)
			{
				strKey = m_strClassify + astrInputType[j];
				if (m_Code.m_mapCodeFPS.Lookup(strKey, AsgbD.FPS))
					m_aNameKey.Add(strKey);
			}
		}
		else
		{
			for (j = 0; j < astrInputType.GetCount(); j++)
			{
				strKey = m_strClassify + astrInputType[j];
				if (m_Code.m_mapCodeFPS.Lookup(strKey, AsgbD.FPS))
					m_aNameKey.Add(strKey);
			}
		}
	}
}

void CAntivibrationSupportGBDataGridWnd::Grid2Data(T_ASGB_D &data)
{
	ROWCOL nRow, nCol;
	if (!GetCurrentCell(nRow, nCol))
		return;

	AsgbD.Initialize();
	CString strKey = m_aNameKey[nRow - 1];
	int nIndex = nRow - 1;

	if (m_bCode == TRUE)
	{
		if (m_nPropType == 0)
		{
			m_Code.m_mapCodeLNR.Lookup(strKey, data.LNR);
			data.LNR.nCode = m_nCode;
		}
		else if (m_nPropType == 1)
		{
			m_Code.m_mapCodeLRB.Lookup(strKey, data.LRB);
			data.LRB.nCode = m_nCode;
		}
		else if (m_nPropType == 2)
		{
			m_Code.m_mapCodeHDR.Lookup(strKey, data.HDR);
			data.HDR.nCode = m_nCode;
		}
		else if (m_nPropType == 3)
		{
			m_Code.m_mapCodeESB.Lookup(strKey, data.ESB);
			data.ESB.nCode = m_nCode;
		}
		else if (m_nPropType == 4)
		{
			m_Code.m_mapCodeFPS.Lookup(strKey, data.FPS);
			data.FPS.nCode = m_nCode;
		}
	}
	data.nPropType = m_nPropType;
	m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(data);
}

CString CAntivibrationSupportGBDataGridWnd::KvUnitChange(CString str)
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int iLenID = CurIndex.nBase_Length;
	int iForID = CurIndex.nBase_Force;
	double dRtoL = 1;
	double dRotF = 1;
	switch (iLenID)
	{
	case (D_UNITSYS_LENGTH_INDEX_MM): dRtoL = 0.001; break;
	case (D_UNITSYS_LENGTH_INDEX_CM): dRtoL = 0.01; break;
	case (D_UNITSYS_LENGTH_INDEX_M):  dRtoL = 1.; break;
	case (D_UNITSYS_LENGTH_INDEX_IN): dRtoL = 0.0254; break;
	case (D_UNITSYS_LENGTH_INDEX_FT): dRtoL = 0.3048; break;
	default:
		break;
	}
	switch (iForID)
	{
	case (D_UNITSYS_FORCE_INDEX_KG):	dRotF /= 9.80665; break;
	case (D_UNITSYS_FORCE_INDEX_TON):	dRotF /= 9806.65; break;
	case (D_UNITSYS_FORCE_INDEX_N):		dRotF /= 1.; break;
	case (D_UNITSYS_FORCE_INDEX_KN):	dRotF /= 1000.; break;
	case (D_UNITSYS_FORCE_INDEX_LBF):	dRotF /= 4.448223; break;
	case (D_UNITSYS_FORCE_INDEX_KIP):	dRotF /= 4448.223; break;
	default:
		break;
	}

	int length = str.Find(_T("~"));
	CString lengthLeft = str.Left(length);
	double dKvc1 = _tstof(lengthLeft);

	str = str.Right(str.GetLength() - length - 1);
	double dKvc2 = _tstof(str);

	CString strNew;
	strNew.Format(_T("%.0f~%.0f"), dKvc1*dRtoL*dRotF, dKvc2*dRtoL*dRotF);
	return strNew;
}

CString CAntivibrationSupportGBDataGridWnd::RUnitChange(CString str)
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int iLenID = CurIndex.nBase_Length;
	double dRtoL = 1;

	switch (iLenID)
	{
	case (D_UNITSYS_LENGTH_INDEX_MM): dRtoL = 0.001; break;
	case (D_UNITSYS_LENGTH_INDEX_CM): dRtoL = 0.01; break;
	case (D_UNITSYS_LENGTH_INDEX_M):  dRtoL = 1.; break;
	case (D_UNITSYS_LENGTH_INDEX_IN): dRtoL = 0.0254; break;
	case (D_UNITSYS_LENGTH_INDEX_FT): dRtoL = 0.3048; break;
	default:
		break;
	}

	//»ñÈ¡µÚÒ»¸ö×Ö·û£¨Êý¾Ý¸öÊý£©
	int length = str.Find(_T("/"));
	
	CString lengthPointStr = str.Left(length);
	int tempInt = _ttoi(lengthPointStr.GetBuffer(lengthPointStr.GetLength()));

	//½«¸÷¸öÓÃ/¸ô¿ªµÄÊý¾Ý·ÅÖÃÔÚstrPoint[]ÄÚ
	CArray<double, double> arR;
	str = str.Right(str.GetLength() - length - 1);
	for (int i = 0; i < tempInt; i++)
	{
		length = str.Find(_T("/"));
		double dTemp = _tstof(str.Left(length));
		arR.Add(dTemp);
		str = str.Right(str.GetLength() - length - 1);
	}

	CString temp;
	CString strNew;
	for (int i = 0; i < arR.GetCount(); i++)
	{
		if (i == 0)
			temp.Format(_T("%.2f"), arR[i] / dRtoL);
		else
			temp.Format(_T("/%.2f"), arR[i] / dRtoL);
		strNew += temp;
	}
	return strNew;
}

//BOOL CAntivibrationSupportGBDataGridWnd::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
//{
//	BOOL bReturn;
//	bReturn = CGXGridWnd::OnLButtonDblClkRowCol(nRow, nCol, nFlags, pt);
//
//	if (nRow < 1)
//		return bReturn;
//
//	CString strKey = m_aNameKey[nRow-1];
//	int nIndex = nRow-1;
//
//	T_ASGB_D AsgbD; AsgbD.Initialize();
//	Grid2Data(strKey, nIndex, AsgbD);
//	((CAntivibrationSupportGBDataTableDlg*)(GetParent()))->OnLButtonDblClkOnGrid(AsgbD);
//	return bReturn;
//}

BOOL CAntivibrationSupportGBDataGridWnd::SaveUserDefineData2Ini(CString strClassify)
{
	if (!SaveData(strClassify))
		return FALSE;

	CString csFolder = IAntivibrationSupportGBDBMgr::Inst()->GetIsolatorFileFolder(m_nPropType);
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

	if (IAntivibrationSupportGBDBMgr::Inst()->WriteIsolatorInfo(m_User, m_nPropType, csFileName, strClassify))
	{
		CDBLib::RunTextEditor(csFileName, _T(""));
		return TRUE;
	}

	return FALSE;
}


void CAntivibrationSupportGBDataGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetStyleRange(CGXRange(nRow, nCol), rstyle);
}

void CAntivibrationSupportGBDataGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_Cell(nRow, nCol, rstyle, strTemp);
}

void CAntivibrationSupportGBDataGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetCoveredCellsRowCol(nRow1, nCol1, nRow2, nCol2);
	SetStyleRange(CGXRange(nRow1, nCol1, nRow2, nCol2), rstyle);
}

void CAntivibrationSupportGBDataGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_CellMerge(nRow1, nCol1, nRow2, nCol2, rstyle, strTemp);
}

BOOL CAntivibrationSupportGBDataGridWnd::OnDeleteRecords(CRowColArray &awRows)
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

void CAntivibrationSupportGBDataGridWnd::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow, TRUE);
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SetNewLineNo(nRow, nCol);
}

void CAntivibrationSupportGBDataGridWnd::SetNewLineNo(ROWCOL nRow, ROWCOL nCol)
{
	CString strLineNo;
	strLineNo.Format(_T("%d"), nRow);
	SetValueRange(CGXRange(nRow, 1), strLineNo);
}


BOOL CAntivibrationSupportGBDataGridWnd::OnPasteFromClipboard(const CGXRange &range)
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


BOOL CAntivibrationSupportGBDataGridWnd::SaveData(CString strClassify)
{
	int nModeNum = GetRowCount();
	m_User.Initialize();
	T_ASGB_D asgbD;
	CString strKey;
	for (int i = 0; i < nModeNum; i++)
	{
		if (m_nPropType == 0)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				asgbD.LNR.Initialize();
				asgbD.LNR.strInputType = GetValueRowCol(i + 1, 2);
				if (asgbD.LNR.strInputType.GetLength() < 1)
					continue;
				asgbD.LNR.dD = _ttof(GetValueRowCol(i + 1, 3));
				asgbD.LNR.dKv = _ttof(GetValueRowCol(i + 1, 4));
				asgbD.LNR.dKvt = asgbD.LNR.dKv;
				asgbD.LNR.dKh = _ttof(GetValueRowCol(i + 1, 5));
				asgbD.LNR.dTr = _ttof(GetValueRowCol(i + 1, 6));
				asgbD.LNR.strClassify = strClassify;
				asgbD.nPropType = m_nPropType;
				strKey = strClassify + asgbD.LNR.strInputType;
				m_User.m_mapCodeLNR.SetAt(strKey, asgbD.LNR);
			}
		}
		else if (m_nPropType == 1)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				asgbD.LRB.Initialize();
				asgbD.LRB.strInputType = GetValueRowCol(i + 1, 2);
				if (asgbD.LRB.strInputType.GetLength() < 1)
					continue;
				asgbD.LRB.dD = _ttof(GetValueRowCol(i + 1, 3));
				asgbD.LRB.dKv = _ttof(GetValueRowCol(i + 1, 4));
				asgbD.LRB.dKvt = asgbD.LRB.dKv;
				asgbD.LRB.dHorEquiStiff = _ttof(GetValueRowCol(i + 1, 5));
				asgbD.LRB.dEquiRot = _ttof(GetValueRowCol(i + 1, 6));
				asgbD.LRB.dTr = _ttof(GetValueRowCol(i + 1, 7));
				asgbD.LRB.dK1 = _ttof(GetValueRowCol(i + 1, 8));
				asgbD.LRB.dKd = _ttof(GetValueRowCol(i + 1, 9));
				asgbD.LRB.dOd = _ttof(GetValueRowCol(i + 1, 10));			
				asgbD.LRB.strClassify = strClassify;
				asgbD.nPropType = m_nPropType;
				strKey = strClassify + asgbD.LRB.strInputType;
				m_User.m_mapCodeLRB.SetAt(strKey, asgbD.LRB);
			}
		}
		else if (m_nPropType == 2)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				asgbD.HDR.Initialize();
				asgbD.HDR.strInputType = GetValueRowCol(i + 1, 2);
				if (asgbD.HDR.strInputType.GetLength() < 1)
					continue;
				asgbD.HDR.dD = _ttof(GetValueRowCol(i + 1, 3));
				asgbD.HDR.dKv = _ttof(GetValueRowCol(i + 1, 4));
				asgbD.HDR.dKvt = asgbD.HDR.dKv;
				asgbD.HDR.dHorEquiStiff = _ttof(GetValueRowCol(i + 1, 5));
				asgbD.HDR.dEquiRot = _ttof(GetValueRowCol(i + 1, 6));
				asgbD.HDR.dTr = _ttof(GetValueRowCol(i + 1, 7));
				asgbD.HDR.dK1 = _ttof(GetValueRowCol(i + 1, 8));
				asgbD.HDR.dKd = _ttof(GetValueRowCol(i + 1, 9));
				asgbD.HDR.dOd = _ttof(GetValueRowCol(i + 1, 10));
				
				asgbD.HDR.strClassify = strClassify;
				asgbD.nPropType = m_nPropType;
				strKey = strClassify + asgbD.HDR.strInputType;
				m_User.m_mapCodeHDR.SetAt(strKey, asgbD.HDR);
			}
		}
		else if (m_nPropType == 3)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				asgbD.ESB.Initialize();
				asgbD.ESB.strInputType = GetValueRowCol(i + 1, 2);
				if (asgbD.ESB.strInputType.GetLength() < 1)
					continue;
				asgbD.ESB.dD = _ttof(GetValueRowCol(i + 1, 3));
				asgbD.ESB.dKv = _ttof(GetValueRowCol(i + 1, 4));
				asgbD.ESB.dKvt = asgbD.ESB.dKv;
				asgbD.ESB.dK1 = _ttof(GetValueRowCol(i + 1, 5));
				asgbD.ESB.dU = _ttof(GetValueRowCol(i + 1, 6));
				asgbD.ESB.dTr = _ttof(GetValueRowCol(i + 1, 7));
				asgbD.ESB.strClassify = strClassify;
				asgbD.nPropType = m_nPropType;
				strKey = strClassify + asgbD.ESB.strInputType;
				m_User.m_mapCodeESB.SetAt(strKey, asgbD.ESB);
			}
		}
		else if (m_nPropType == 4)
		{
			int nIdx = GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(GetValueRowCol(i + 1, 1));
			if (nIdx > 0)
			{
				asgbD.FPS.Initialize();
				asgbD.FPS.strInputType = GetValueRowCol(i + 1, 2);
				if (asgbD.FPS.strInputType.GetLength() < 1)
					continue;
				asgbD.FPS.dVBearing = _ttof(GetValueRowCol(i + 1, 3));
				asgbD.FPS.dKv = _ttof(GetValueRowCol(i + 1, 4));
				asgbD.FPS.dKvt = asgbD.FPS.dKv;
				asgbD.FPS.dK1 = asgbD.FPS.dKv;
				asgbD.FPS.dUs = _ttof(GetValueRowCol(i + 1, 5));
				asgbD.FPS.dUf = _ttof(GetValueRowCol(i + 1, 6));
				asgbD.FPS.nR = _ttof(GetValueRowCol(i + 1, 7));
				asgbD.FPS.dHorDis = _ttof(GetValueRowCol(i + 1, 8));
				asgbD.FPS.strClassify = strClassify;
				asgbD.nPropType = m_nPropType;
				strKey = strClassify + asgbD.FPS.strInputType;
				m_User.m_mapCodeFPS.SetAt(strKey, asgbD.FPS);
			}
		}
	}

	return TRUE;
}

CString CAntivibrationSupportGBDataGridWnd::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
