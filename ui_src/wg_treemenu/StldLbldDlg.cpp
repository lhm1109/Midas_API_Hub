// StldLbldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldLbldDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "StructStldDlg.h"

#define LBLD_ON_THE_LOADINGLINE 0
#define LBLD_SELECTED_ELEMENT   1 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
	enum LOAD_TYPE
	{
		CONCENTRATED_FORCES = 0,
		CONCENTRATED_MOMENTS_TORSIONS,
		UNIFORM_LOADS,
		UNIFORM_MOMENTS_TORSIONS,
		TRAPEZOIDAL_LOADS,
		TRAPEZOIDAL_MOMENTS_TORSIONS,
		CURVED_LOADS,
		UNIFORM_PRESSURE,
		TRAPEZOIDAL_PRESSURE,
	};
}

/////////////////////////////////////////////////////////////////////////////
// CStldLbldDlg dialog


CStldLbldDlg::CStldLbldDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldLbldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldLbldDlg)
	m_nOption = 0;
	m_nDirection = 0;
	m_nProj = 1;
	m_nRelative = -1;
	m_bCopyLoad = FALSE;
	m_nAxis = -1;
	m_csDistance = _T("0");
	m_nCurveMethod = 0;
	//}}AFX_DATA_INIT

	m_nSelType = -1;
	m_pBitmap = 0;
	m_ValueD[0] = m_ValueD[1] = m_ValueD[2] = m_ValueD[3] = 0.0;

	m_bInitialized      = FALSE;
	m_bFirstInit        = TRUE;
	m_bAdditionalDistJ  = FALSE;

	m_aCopyLoadCtrl.Add(IDC_TM_AXIS_TITLE);
	m_aCopyLoadCtrl.Add(IDC_TM_AXIS_X);
	m_aCopyLoadCtrl.Add(IDC_TM_AXIS_Y);
	m_aCopyLoadCtrl.Add(IDC_TM_AXIS_Z);
	m_aCopyLoadCtrl.Add(IDC_TM_DIST_TITLE);
	m_aCopyLoadCtrl.Add(IDC_TM_DIST_EDIT);
	m_aCopyLoadCtrl.Add(IDC_TM_DIST_UNIT);
	m_aCopyLoadCtrl.Add(IDC_TM_DIST_EXAM);

	m_aLengOptCtrl.Add(IDC_TM_RELATIVE);
	m_aLengOptCtrl.Add(IDC_TM_ABSOLUTE);

	m_aNodeListCtrl.Add(IDC_TM_NODE_LIST);

	m_pDoc = CDBDoc::GetDocPoint();
}

CStldLbldDlg::~CStldLbldDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CStldLbldDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldLbldDlg)

	DDX_Control(pDX, IDC_TM_CBO_LC,           m_LoadCase);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC,      m_wndLdgrCobx);
	DDX_Radio  (pDX, IDC_TM_OPT_ADD,          m_nOption);
	DDX_Control(pDX, IDC_TM_LOAD_TYPE,        m_cboLoadType);
	DDX_Control(pDX, IDC_TM_SEL_COMBO,        m_CobxSelType);
	DDX_Control(pDX, IDC_TM_ECCEN_CHK,        m_chkEccen);
	DDX_Radio  (pDX, IDC_TM_ECCEN_CENTROID_RDO, m_nEccenType);
	DDX_Control(pDX, IDC_TM_ECCEN_DIR_CMB,    m_cmbEccenDir);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST_TXT,   m_txtEccenDist);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST1_EDT,  m_edtEccenDistI);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST2_EDT,  m_edtEccenDistJ);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST2_CHK,  m_chkEccenDistJ);
	DDX_Control(pDX, IDC_TM_STLD_LBLD_ADDITIONAL_H_CHK,               m_chkAdditionalH);
	DDX_Control(pDX, IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_STC,        m_stcAdditionalDist);
	DDX_Check  (pDX, IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_CHK,  m_bAdditionalDistJ);
	DDX_Control(pDX, IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_I_END_EDT,  m_edtAdditionalDistI);
	DDX_Control(pDX, IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_EDT,  m_edtAdditionalDistJ);
	DDX_CBIndex(pDX, IDC_TM_DIR_COMBO, m_nDirection);
	DDX_Control(pDX, IDC_TM_DIR_COMBO, m_wndDirection);
	DDX_Radio  (pDX, IDC_TM_PROJ_YES,  m_nProj);
	DDX_Radio  (pDX, IDC_TM_RELATIVE,  m_nRelative);
	DDX_Control(pDX, IDC_TM_D1,        m_wndD1);
	DDX_Control(pDX, IDC_TM_D2,        m_wndD2);
	DDX_Control(pDX, IDC_TM_D3,        m_wndD3);
	DDX_Control(pDX, IDC_TM_D4,        m_wndD4);
	DDX_Control(pDX, IDC_TM_NODE_LIST, m_wndNodeList); 
	DDX_Check  (pDX, IDC_TM_COPY_LOAD, m_bCopyLoad);
	DDX_Radio  (pDX, IDC_TM_AXIS_X,    m_nAxis);
	DDX_Control(pDX, IDC_TM_DIST_UNIT, m_wndDistUnit);
	DDX_Radio  (pDX, IDC_TM_CURVE_MTD_RDO, m_nCurveMethod);
	DDX_Control(pDX, IDC_TM_A_EDIT,    m_wndA);
	DDX_Control(pDX, IDC_TM_B_EDIT,    m_wndB);
	DDX_Control(pDX, IDC_TM_C_EDIT,    m_wndC);
	DDX_Text   (pDX, IDC_TM_DIST_EDIT, m_csDistance);
	//}}AFX_DATA_MAP
	UINT nID[][2] = {
		{IDC_TM_D1, IDC_TM_P1},
		{IDC_TM_D2, IDC_TM_P2},
		{IDC_TM_D3, IDC_TM_P3},
		{IDC_TM_D4, IDC_TM_P4},
	};
	DDX_Control(pDX, IDC_TM_UP1, m_Unit);
	DDX_Control(pDX, IDC_TM_UP2, m_Unit2);

	for (int i = 0; i < 4; i++)
		DDX_Control(pDX, nID[i][1], m_ValueP[i]);

	CString csValue;
	if (pDX->m_bSaveAndValidate)
	{
		if (!m_wndD1.GetEditValue(m_ValueD[0])) pDX->Fail();
		if (!m_wndD2.GetEditValue(m_ValueD[1])) pDX->Fail();
		if (!m_wndD3.GetEditValue(m_ValueD[2])) pDX->Fail();
		if (!m_wndD4.GetEditValue(m_ValueD[3])) pDX->Fail();
	}
	else
	{
		CString csVal;
		csVal.Format(_LS(IDS_WG_TREEMENU__g), m_ValueD[0]);
		m_wndD1.SetWindowText(csVal);
		csVal.Format(_LS(IDS_WG_TREEMENU__g), m_ValueD[1]);
		m_wndD2.SetWindowText(csVal);
		csVal.Format(_LS(IDS_WG_TREEMENU__g), m_ValueD[2]);
		m_wndD3.SetWindowText(csVal);
		csVal.Format(_LS(IDS_WG_TREEMENU__g), m_ValueD[3]);
		m_wndD4.SetWindowText(csVal);
	}

	DDX_Control(pDX, IDC_TM_PICTURE, m_wndLbldTypePicture);
}

void CStldLbldDlg::InitLoadTypeCombo()
{
	m_cboLoadType.ResetContent();

	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Concentrated_Forces));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Concentrated_Moments_Torsions));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Uniform_Loads));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Uniform_Moments_Torsions));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Trapezoidal_Loads));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Trapezoidal_Moments_Torsions));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Curved_Loads));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Uniform_Pressure));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Trapezoidal_Pressure));

	m_cboLoadType.SetCurSel(2); // Default : Uniform Load
}

void CStldLbldDlg::InitEccenDirectionCombo()
{
	// Eccentricity 안의 Direction Combo Box 
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Local_y), 2);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Local_z), 3);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Global_X), 4);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Global_Y), 5);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Global_Z), 6);
	CDlgUtil::CobxSetCurSelItemData(m_cmbEccenDir, 2);  // LY
}

void CStldLbldDlg::DirectionCtrlMan()
{
	const int nSel = m_CobxSelType.GetCurSel();
	const int nLoadType = m_cboLoadType.GetCurSel();
	m_nSelType = static_cast<int>(m_CobxSelType.GetItemData(nSel));
	m_wndDirection.ResetContent();

	if (nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE) // MQC:3822 by KJH 입력제한 함
	{
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_y));   m_wndDirection.SetItemData(0, 2);

		const bool bNoAdditionalH = (m_chkAdditionalH.GetCheck() == 0) ? true : false;
		if(bNoAdditionalH == true)
		{
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_z ));m_wndDirection.SetItemData(1, 3);
		}

		m_wndDirection.SetCurSel(0); // Local y
	}
	else
	{
		if (m_nSelType == LBLD_ON_THE_LOADINGLINE)
		{
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_x)); m_wndDirection.SetItemData(0, 1);
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_y)); m_wndDirection.SetItemData(1, 2);
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_z)); m_wndDirection.SetItemData(2, 3);
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X)); m_wndDirection.SetItemData(3, 4);
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y)); m_wndDirection.SetItemData(4, 5);
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z)); m_wndDirection.SetItemData(5, 6);

			m_wndDirection.SetCurSel(5);// Default Global Z 
		}
		else if (m_nSelType == LBLD_SELECTED_ELEMENT)
		{
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X)); m_wndDirection.SetItemData(0, 4);
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y)); m_wndDirection.SetItemData(1, 5);
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z)); m_wndDirection.SetItemData(2, 6);

			m_wndDirection.SetCurSel(2);// Default Global Z 
		}
		else
		{
			ASSERT(false);
		}
	}

	if (m_nSelType == LBLD_SELECTED_ELEMENT)
	{
		CButton * pBtn;

		m_nProj = 0;
		pBtn = (CButton*)GetDlgItem(IDC_TM_PROJ_NO);
		if(pBtn)
		{
			pBtn->SetCheck(BST_UNCHECKED);
			pBtn->EnableWindow(FALSE);
		}
	
		pBtn = (CButton*)GetDlgItem(IDC_TM_PROJ_YES);
		if(pBtn)
		{
			pBtn->SetCheck(BST_CHECKED);
			pBtn->EnableWindow(TRUE);
		}

		CDlgUtil::CtrlEnableDisable(this, IDC_TM_COPY_LOAD, FALSE);
		CtrlEnableDisable(m_aCopyLoadCtrl, FALSE);
	}
	else
	{
		CWnd* pWndNO = GetDlgItem(IDC_TM_PROJ_NO);
		CWnd* pWndYES = GetDlgItem(IDC_TM_PROJ_YES);
		switch (nLoadType)
		{
		case CONCENTRATED_FORCES:
		case CONCENTRATED_MOMENTS_TORSIONS:
		{
			pWndNO->EnableWindow(FALSE);
			pWndYES->EnableWindow(FALSE);
			break;
		}
		case UNIFORM_LOADS:
		case UNIFORM_MOMENTS_TORSIONS:
		case TRAPEZOIDAL_LOADS:
		case TRAPEZOIDAL_MOMENTS_TORSIONS:
		case CURVED_LOADS:
			pWndNO->EnableWindow(TRUE);
			pWndYES->EnableWindow(TRUE);
			break;
		case UNIFORM_PRESSURE:
		case TRAPEZOIDAL_PRESSURE:
			pWndNO->EnableWindow(FALSE);
			pWndYES->EnableWindow(FALSE);
			break;
		default:
			ASSERT(false);
		}

		CDlgUtil::CtrlEnableDisable(this,IDC_TM_COPY_LOAD,TRUE);
		CtrlEnableDisable(m_aCopyLoadCtrl, m_bCopyLoad);		
	}
}

void CStldLbldDlg::AlignControl()
{
	UpdateData(TRUE);

	int nLoadType = m_cboLoadType.GetCurSel();

	CArray<UINT, UINT> aEccen;
	CArray<UINT, UINT> aEccen2;
	CArray<UINT, UINT> aAdditionalH;
	CArray<UINT, UINT> aValue;
	CArray<UINT, UINT> aValueCurved;
	CArray<UINT, UINT> aEtc;
	CArray<UINT, UINT> aApplyCancel;

	aEccen.Add(IDC_TM_ECCEN_FRM);
	aEccen.Add(IDC_TM_ECCEN_CHK);
	aEccen.Add(IDC_TM_ECCEN_OFFSET_RDO);
	aEccen.Add(IDC_TM_ECCEN_CENTROID_RDO);
	aEccen.Add(IDC_TM_ECCEN_DIR_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIR_CMB);
	aEccen.Add(IDC_TM_ECCEN_DIST_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST1_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST1_EDT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_EDT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_CHK);

	aEccen2.Add(IDC_TM_ECCEN_OFFSET_RDO);
	aEccen2.Add(IDC_TM_ECCEN_CENTROID_RDO);
	aEccen2.Add(IDC_TM_ECCEN_DIR_TXT);
	aEccen2.Add(IDC_TM_ECCEN_DIR_CMB);
	aEccen2.Add(IDC_TM_ECCEN_DIST_TXT);
	aEccen2.Add(IDC_TM_ECCEN_DIST1_TXT);
	aEccen2.Add(IDC_TM_ECCEN_DIST1_EDT);
	aEccen2.Add(IDC_TM_ECCEN_DIST2_TXT);
	aEccen2.Add(IDC_TM_ECCEN_DIST2_EDT);
	aEccen2.Add(IDC_TM_ECCEN_DIST2_CHK);

	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_H_FRM);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_H_CHK);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_STC);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_I_END_STC);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_I_END_EDT);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_STC);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_EDT);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_CHK);

	aValue.Add(IDC_TM_LBLD_LINE_FRM);
	aValue.Add(IDC_TM_LINE_UNIT_STATIC);
	aValue.Add(IDC_TM_UP1);
	aValue.Add(IDC_TM_RELATIVE);      aValue.Add(IDC_TM_ABSOLUTE);
	aValue.Add(IDC_STATIC_X1);        aValue.Add(IDC_STATIC_X2);
	aValue.Add(IDC_STATIC_X3);        aValue.Add(IDC_STATIC_X4);
	aValue.Add(IDC_TM_D1);            aValue.Add(IDC_TM_D2);
	aValue.Add(IDC_TM_D3);            aValue.Add(IDC_TM_D4);
	aValue.Add(IDC_TM_FM_TITLE1);     aValue.Add(IDC_TM_FM_TITLE2);
	aValue.Add(IDC_TM_FM_TITLE3);     aValue.Add(IDC_TM_FM_TITLE4);
	aValue.Add(IDC_TM_P1);            aValue.Add(IDC_TM_P2);
	aValue.Add(IDC_TM_P3);            aValue.Add(IDC_TM_P4);

	aValueCurved.Add(IDC_TM_LBLD_CURVE_FRM);
	aValueCurved.Add(IDC_TM_CURVE_MTD_RDO);     aValueCurved.Add(IDC_TM_CURVE_MTD_RDO2);
	aValueCurved.Add(IDC_TM_A_TEXT);            aValueCurved.Add(IDC_TM_B_TEXT);
	aValueCurved.Add(IDC_TM_C_TEXT);            aValueCurved.Add(IDC_TM_A_EDIT);
	aValueCurved.Add(IDC_TM_B_EDIT);            aValueCurved.Add(IDC_TM_C_EDIT);
	aValueCurved.Add(IDC_TM_CURVE_UNIT_STATIC); aValueCurved.Add(IDC_TM_UP2);

	aEtc.Add(IDC_TM_DIR_STC);           aEtc.Add(IDC_TM_DIR_COMBO);
	aEtc.Add(IDC_TM_LBLD_PROJ_STC);     aEtc.Add(IDC_TM_PROJ_YES);
	aEtc.Add(IDC_TM_PROJ_NO);           aEtc.Add(IDC_TM_LBLD_LINE_FRM);
	aEtc.Add(IDC_TM_LINE_UNIT_STATIC);  aEtc.Add(IDC_TM_UP1);
	aEtc.Add(IDC_TM_RELATIVE);          aEtc.Add(IDC_TM_ABSOLUTE);
	aEtc.Add(IDC_STATIC_X1);            aEtc.Add(IDC_STATIC_X2);
	aEtc.Add(IDC_STATIC_X3);            aEtc.Add(IDC_STATIC_X4);
	aEtc.Add(IDC_TM_D1);                aEtc.Add(IDC_TM_D2);
	aEtc.Add(IDC_TM_D3);                aEtc.Add(IDC_TM_D4);
	aEtc.Add(IDC_TM_FM_TITLE1);         aEtc.Add(IDC_TM_FM_TITLE2);
	aEtc.Add(IDC_TM_FM_TITLE3);         aEtc.Add(IDC_TM_FM_TITLE4);
	aEtc.Add(IDC_TM_P1);                aEtc.Add(IDC_TM_P2);
	aEtc.Add(IDC_TM_P3);                aEtc.Add(IDC_TM_P4);
	aEtc.Add(IDC_TM_NODE_LIST_STC);     aEtc.Add(IDC_TM_NODE_LIST);
	aEtc.Add(IDC_TM_COPY_LOAD);         aEtc.Add(IDC_TM_COPY_LOAD_FRM);
	aEtc.Add(IDC_TM_AXIS_TITLE);        aEtc.Add(IDC_TM_AXIS_X);
	aEtc.Add(IDC_TM_AXIS_Y);            aEtc.Add(IDC_TM_AXIS_Z);
	aEtc.Add(IDC_TM_DIST_TITLE);        aEtc.Add(IDC_TM_DIST_EDIT);
	aEtc.Add(IDC_TM_DIST_UNIT);         aEtc.Add(IDC_TM_DIST_EXAM);

	aApplyCancel.Add(IDC_TM_EXECUTE);
	aApplyCancel.Add(IDC_TM_CLOSE);

	// Additional H 를 Eccen. form Centroid 위치로 옮긴다.
	CRect rRef;
	CRect rMove;
	int nDistY;
	
	GetDlgItem(IDC_TM_ECCEN_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_STLD_LBLD_ADDITIONAL_H_FRM)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, aAdditionalH, nDistY);

	// Value
	GetDlgItem(IDC_TM_LBLD_LINE_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_LBLD_CURVE_FRM)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, aValueCurved, nDistY);

	BOOL bPressure = FALSE;
	BOOL bCurved = FALSE;

	if(nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE) bPressure = TRUE;
	else if(nLoadType == CURVED_LOADS) bCurved = TRUE;

	if(bPressure)
	{
		CDlgUtil::CtrlShowHide(this, aEccen,         FALSE);
		CDlgUtil::CtrlShowHide(this, aValue,         TRUE);
		CDlgUtil::CtrlShowHide(this, aValueCurved,   FALSE);
		CDlgUtil::CtrlShowHide(this, aAdditionalH,   TRUE);

		GetDlgItem(IDC_TM_STLD_LBLD_ADDITIONAL_H_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_DIR_STC)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(15);
		CDlgUtil::CtrlMoveDistY(this, aEtc, nDistY);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, aEccen, TRUE);
		//CDlgUtil::CtrlShowHide(this, aEccen2, FALSE);
		CDlgUtil::CtrlShowHide(this, aAdditionalH, FALSE);
		CDlgUtil::CtrlShowHide(this, aValueCurved, FALSE);

		GetDlgItem(IDC_TM_ECCEN_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_DIR_STC)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(15);
		CDlgUtil::CtrlMoveDistY(this, aEtc, nDistY);

		if(bCurved)
		{
			// Value
			GetDlgItem(IDC_TM_LBLD_LINE_FRM)->GetWindowRect(rRef);
			GetDlgItem(IDC_TM_LBLD_CURVE_FRM)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top;
			CDlgUtil::CtrlMoveDistY(this, aValueCurved, nDistY);

			CDlgUtil::CtrlShowHide(this, aValue,       FALSE);
			CDlgUtil::CtrlShowHide(this, aValueCurved, TRUE);
		}
		else
		{
			// Value
			GetDlgItem(IDC_TM_LBLD_PROJ_STC)->GetWindowRect(rRef);
			GetDlgItem(IDC_TM_LBLD_LINE_FRM)->GetWindowRect(rMove);
			nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, aValue, nDistY);

			CDlgUtil::CtrlShowHide(this, aValue,       TRUE);
			CDlgUtil::CtrlShowHide(this, aValueCurved, FALSE);
		}
	}

	GetDlgItem(IDC_TM_COPY_LOAD_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EXECUTE)->GetWindowRect(rMove);
	nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aApplyCancel, nDistY);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_EXECUTE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+1;

	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);

	RedrawWindow();
}

void CStldLbldDlg::SetUnitType(int nLoadType)
{
	// Uniform이 콤보로 삽입되었기 때문에 nLoadType변경

	// Uniform은 Distributed만 있다.
	if(nLoadType == TRAPEZOIDAL_LOADS || nLoadType == TRAPEZOIDAL_MOMENTS_TORSIONS) nLoadType -= 2;
	if(nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE) nLoadType = 4;

	if(nLoadType != CURVED_LOADS)
	{
		const int nUnitType = CUnitCtrl::m_BMLD_UNIT.Force_Type[nLoadType];
		m_Unit.SetUnitType(nUnitType);  
		for (int i = 0; i < 4; i++)
		{
			m_ValueP[i].SetUnitType(nUnitType);
		}
	}
	else
	{    
		const int nUnitType2 = CUnitCtrl::m_BMLD_UNIT.Force_Type[2];
		m_Unit2.SetUnitType(nUnitType2);
	}
}

void CStldLbldDlg::ChangeBitmap(int nBitmap)
{
#if 0
	UINT aBitmapID[] = { 
		IDB_TM_BMLD_1, IDB_TM_BMLD_2, IDB_TM_BMLD_3, 
		IDB_TM_BMLD_4, IDB_TM_BMLD_5, IDB_TM_BMLD_6,
		IDB_TM_BMLD_7, IDB_TM_BMLD_8, IDB_TM_BMLD_9,
	};

	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_PICTURE);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nBitmap-1]);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
#else
	CString aBmldTypePicture[] = {
		_T("SVG\\illustration\\Dialog\\tm_bmld1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld3.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld4.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld5.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld6.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld7.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld8.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld9.svg")
	};
	m_wndLbldTypePicture.SetImage(aBmldTypePicture[nBitmap]);
#endif
}

void CStldLbldDlg::ChangeValueTitle(int nLoadType)
{
	if(nLoadType == CURVED_LOADS) return;  // Curved load인 경우는 return
	UINT aTitleID[] = { 
		IDC_TM_FM_TITLE1, IDC_TM_FM_TITLE2, IDC_TM_FM_TITLE3, IDC_TM_FM_TITLE4 
	};
	CString aName[][4] = {
		{_LS(IDS_WG_TREEMENU_P1), _LS(IDS_WG_TREEMENU_P2), _LS(IDS_WG_TREEMENU_P3), _LS(IDS_WG_TREEMENU_P4)},
		{_LS(IDS_WG_TREEMENU_M1), _LS(IDS_WG_TREEMENU_M2), _LS(IDS_WG_TREEMENU_M3), _LS(IDS_WG_TREEMENU_M4)},
		{_T("w"), _T(""), _T(""), _T("")},
		{_T("M"), _T(""), _T(""), _T("")},
		{_LS(IDS_WG_TREEMENU_W1), _LS(IDS_WG_TREEMENU_W2), _LS(IDS_WG_TREEMENU_W3), _LS(IDS_WG_TREEMENU_W4)},
		{_LS(IDS_WG_TREEMENU_M1), _LS(IDS_WG_TREEMENU_M2), _LS(IDS_WG_TREEMENU_M3), _LS(IDS_WG_TREEMENU_M4)},
		{_T(""), _T(""), _T(""), _T("")},
		{_T("p"), _T(""), _T(""), _T("")},
		{_LS(IDS_WG_TREEMENU_P1), _LS(IDS_WG_TREEMENU_P2), _LS(IDS_WG_TREEMENU_P3), _LS(IDS_WG_TREEMENU_P4)},
	};
	CBCGPStatic* pTitleWnd;
	for (int i = 0; i < sizeof(aTitleID)/sizeof(UINT); i++)
	{
		pTitleWnd = (CBCGPStatic*)GetDlgItem(aTitleID[i]);
		pTitleWnd->SetWindowText(aName[nLoadType][i]);
	}
}

void CStldLbldDlg::CurveValueCtrlMan()
{
	UpdateData();
	m_wndC.EnableWindow(m_nCurveMethod == 1);
	UpdateData(FALSE);	
}

void CStldLbldDlg::EnableElementSelection(BOOL bEnable)
{
	GetDlgItem(IDC_TM_SEL_COMBO)->EnableWindow(bEnable);
}

void CStldLbldDlg::EnableEccentricity(BOOL bEnable)
{
	if (bEnable)
	{
		int nSel = m_cboLoadType.GetCurSel();
		// Concentrated Force, Uniform Load, Trapezoidal Load, Curved Load
		if (nSel == 0 || nSel == 2 || nSel == 4 || nSel == 6)  
			bEnable = TRUE;
		else bEnable = FALSE;
	}

// 	CArray<UINT, UINT> aCtrlEccen;
// 	//aCtrlEccen.Add(IDC_TM_ECCEN_FRM);
// 	aCtrlEccen.Add(IDC_TM_ECCEN_DIR_CMB);
// 	aCtrlEccen.Add(IDC_TM_ECCEN_CENTROID_RDO);
// 	aCtrlEccen.Add(IDC_TM_ECCEN_OFFSET_RDO);
// 	aCtrlEccen.Add(IDC_TM_ECCEN_DIST1_EDT);
// 	aCtrlEccen.Add(IDC_TM_ECCEN_DIST2_EDT);
// 	aCtrlEccen.Add(IDC_TM_ECCEN_DIST2_CHK);
// 	CDlgUtil::CtrlEnableDisable(this, aCtrlEccen, bEnable);
	m_chkEccen.EnableWindow(bEnable); // Enable Window가 virtual이 아니므로 별도 처리
	if (bEnable) OnTmEccenDist2Chk();
}

void CStldLbldDlg::EnableAdditionalH(BOOL bEnable)
{
	UpdateData(TRUE);
	
// 	CArray<UINT, UINT> aCtrlAdditional;
// 	//aCtrlAdditional.Add(IDC_TM_STLD_LBLD_ADDITIONAL_H_FRM);
// 	aCtrlAdditional.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_I_END_EDT);
// 	aCtrlAdditional.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_EDT);
// 	aCtrlAdditional.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_CHK);
// 	CDlgUtil::CtrlEnableDisable(this, aCtrlAdditional, bEnable);
	m_chkAdditionalH.EnableWindow(bEnable); // Enable Window가 virtual이 아니므로 별도 처리
	if (bEnable) OnTmAdditionalDistJChk();

	DirectionCtrlMan();
}

void CStldLbldDlg::EnableCurvedType(BOOL bEnable)
{
	CArray<UINT, UINT> aCtrlCurvedType;
	aCtrlCurvedType.Add(IDC_TM_CURVE_MTD_RDO);
	aCtrlCurvedType.Add(IDC_TM_CURVE_MTD_RDO2);
	aCtrlCurvedType.Add(IDC_TM_A_EDIT);
	aCtrlCurvedType.Add(IDC_TM_B_EDIT);
	aCtrlCurvedType.Add(IDC_TM_C_EDIT);
	CDlgUtil::CtrlEnableDisable(this, aCtrlCurvedType, bEnable);
}

void CStldLbldDlg::EnableValueByUniform(int nUniform)
{
	if(nUniform == CURVED_LOADS) return;  // Curved Load인 경우는 return;

	// Uniform이 콤보로 삽입되었기 때문에 nUniform값 변경

	// Uniform은 Distributed만 있다.
	if(nUniform == UNIFORM_LOADS || nUniform == UNIFORM_MOMENTS_TORSIONS) nUniform = 0;
	else if(nUniform == UNIFORM_PRESSURE) nUniform = 0;
	else nUniform = 1;

	UINT nID1[] = {
		IDC_TM_D1, IDC_TM_D2, IDC_TM_D3, IDC_TM_D4,
		IDC_TM_P1, IDC_TM_P2, IDC_TM_P3, IDC_TM_P4,
	};
	UINT nID2[] = {
		IDC_TM_P2, IDC_TM_P3, IDC_TM_P4,
	};

	if (m_nOption == 2) return; // delete이면 그냥 리턴한다.
	int nLoadType = m_cboLoadType.GetCurSel();  

	// Uniform이 콤보로 삽입되었기 때문에 nLoadType값 변경 
	if(nLoadType == TRAPEZOIDAL_LOADS || nLoadType == TRAPEZOIDAL_MOMENTS_TORSIONS) nLoadType -= 2;
	if(nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE) nLoadType = 4;

	int i = 0;
	if (nUniform == 1)  // Non-Uniform
	{
		// D1, D2, D3, D4, P1, P2, P3, P4 다 푼다.
		for(int i=0; i < 8; i++)
			GetDlgItem(nID1[i])->EnableWindow(TRUE);
		for(int i=0; i < 4; i++)
			m_ValueD[i] = 0;
	}
	else  // Uniform
	{
		for(int i=0; i < 3; i++)   // P2, P3, P4는 Disable시킨다.
			GetDlgItem(nID2[i])->EnableWindow(FALSE);

		switch(nLoadType)
		{
		case 0: case 1: // concentrated force or moment
			// D1, D2, D3, D4를 모두 Enable시킨다.
			for(int i=0; i < 4; i++)
			{
				GetDlgItem(nID1[i])->EnableWindow(TRUE);
				m_ValueD[i] = 0; 
			}
			break;
		case 2: case 3: case 4: // distributed force or moment + pressure
			// D1, D2는 Enable 시킨다.
			for(int i=0; i < 2; i++)
				GetDlgItem(nID1[i])->EnableWindow(TRUE);
			// D3, D4는 Disable시킨다.
			for (i = 2; i < 4; i++)
				GetDlgItem(nID1[i])->EnableWindow(FALSE);
			// D2의 초기값을 1로 하고 나머지는 모두 0으로 한다.
			for (int i = 0; i < 4; i++) 
				m_ValueD[i] = 0;
			m_ValueD[1] = 1;
			
			break;
		}
	}
	for(int i=0; i < 4; i++) m_ValueP[i].SetWindowText(_T("0"));
	UpdateData(FALSE);
}

void CStldLbldDlg::EnableProjection(BOOL bEnable)
{
	UINT nID[] = {
		IDC_TM_PROJ_YES, IDC_TM_PROJ_NO,
	};
	CWnd* pWnd;

	// Uniform이 콤보로 삽입되었기 때문에 변경

	int nLoadType = m_cboLoadType.GetCurSel();
	if(nLoadType == TRAPEZOIDAL_LOADS || nLoadType == TRAPEZOIDAL_MOMENTS_TORSIONS) nLoadType -= 2;
	if(nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE) nLoadType = 4;
	if (bEnable)
	{
		if (nLoadType < 2) bEnable = FALSE;
		if (m_nDirection <= 3) bEnable = FALSE;
		if (nLoadType == 4) bEnable = FALSE;
	}

	int nSel  = m_CobxSelType.GetCurSel();
	int nType = (int)m_CobxSelType.GetItemData(nSel);
	
	if(nType == LBLD_ON_THE_LOADINGLINE)
	{
		for (int i = 0; i < sizeof(nID)/sizeof(UINT); i++)
		{
			pWnd = GetDlgItem(nID[i]);
			ASSERT(pWnd);
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CStldLbldDlg::EnableValueEdit(BOOL bEnable)
{
	UINT nID[] = {
		IDC_TM_LOAD_TYPE, 
		IDC_TM_DIR_COMBO,
		IDC_TM_D1, IDC_TM_D2, IDC_TM_D3, IDC_TM_D4,
		IDC_TM_P1, IDC_TM_P2, IDC_TM_P3, IDC_TM_P4,
	};
	CWnd* pWnd;

	for (int i = 0; i < sizeof(nID)/sizeof(UINT); i++)
	{
		pWnd = GetDlgItem(nID[i]);
		ASSERT(pWnd);
		pWnd->EnableWindow(bEnable);
	}
	EnableProjection(bEnable);
}

void CStldLbldDlg::PostNcDestroy() 
{
	m_bInitialized = FALSE;
	CMenuBarChildDlg::PostNcDestroy();
}

BEGIN_MESSAGE_MAP(CStldLbldDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldLbldDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_COPY_LOAD, OnTmCopyLoad)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_CBN_SELCHANGE(IDC_TM_LOAD_TYPE, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_TM_DIR_COMBO, OnChangeDirection)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_CBN_SELCHANGE(IDC_TM_SEL_COMBO, OnSelchangeTmSelCombo)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_CURVE_MTD_RDO, OnTmCurveMtdRdo)
	ON_BN_CLICKED(IDC_TM_CURVE_MTD_RDO2, OnTmCurveMtdRdo)
	ON_BN_CLICKED(IDC_TM_ECCEN_DIST2_CHK, OnTmEccenDist2Chk)
	ON_BN_CLICKED(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_CHK, OnTmAdditionalDistJChk)
	ON_BN_CLICKED(IDC_TM_ECCEN_CHK, OnClickEccenBtn)
	ON_BN_CLICKED(IDC_TM_STLD_LBLD_ADDITIONAL_H_CHK, OnClickAdditionalDIstBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	//ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldLbldDlg message handlers

BOOL CStldLbldDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);
	CMenuBarChildDlg::OnInitDialog();
	
	m_wndNodeList.SetAttNodeList();
	m_wndNodeList.SetLButtonDownNotifyWindow(this);
	m_wndNodeList.SetEnterNotifyWindow(this);
	m_wndNodeList.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndNodeList.SetMaxNodeKeyNum(2);

	InitLoadTypeCombo();

	int nLoadType = m_cboLoadType.GetCurSel();

	SetUnitType(nLoadType);
	ChangeBitmap(nLoadType);
	ChangeValueTitle(nLoadType);


	m_nOption = 0;          // Default : Add
	m_nEccenType = 0;
	m_nDirection = 5;       // Default : Global Z
	m_nProj = 1;            // Default : No projection
	m_bCopyLoad = FALSE;    // Default : Do not copy
#if defined(_CIVIL)
	m_nAxis = 0;            // Default : x
#else
	m_nAxis = 2;            // Default : z
#endif
	m_nRelative = 0;        // Default : Relative
 
	EnableProjection(TRUE); // TRUE를 넘겨주더라도 조건에 따라서 FALSE가 된다.
	EnableValueByUniform(nLoadType);
	CtrlEnableDisable(m_aCopyLoadCtrl, m_bCopyLoad);

	m_wndDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_CobxSelType.AddString(_LS(IDS_TM_LBLD_On_the_loading_line));
	m_CobxSelType.SetItemData(0,LBLD_ON_THE_LOADINGLINE);
	m_CobxSelType.AddString(_LS(IDS_TM_LBLD_Selected_element));
	m_CobxSelType.SetItemData(1,LBLD_SELECTED_ELEMENT);
	m_CobxSelType.SetCurSel(0);

	InitEccenDirectionCombo();

	// Eccentricity 안의 Distance 설정..
//	if (!m_bFirstInit) m_chkEccen.ReInit();
//	m_chkEccen.InitControl(this, CStldLbldDlg::IDD, IDC_TM_ECCEN_FRM,TRUE,TRUE);
//	m_chkEccen.SetDefault(TRUE);
	m_chkEccen.SetCheck(FALSE);
	OnClickEccenBtn();

	m_txtEccenDist.SetTextFmt(_LS(IDS_TM_DISTANCE_S));
	m_txtEccenDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtEccenDistI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtEccenDistJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtEccenDistI.SetEditUnit(0.0);
	m_edtEccenDistJ.SetEditUnit(0.0);
	m_chkEccenDistJ.SetCheck(FALSE);
	OnTmEccenDist2Chk();

	// Addtional 안의 Distance 설정...
//	if (!m_bFirstInit) m_chkAdditionalH.ReInit();
//	m_chkAdditionalH.InitControl(this, CStldLbldDlg::IDD, IDC_TM_STLD_LBLD_ADDITIONAL_H_FRM,TRUE,TRUE);
//	m_chkAdditionalH.SetDefault(FALSE);
	m_chkAdditionalH.SetCheck(FALSE);
	OnClickAdditionalDIstBtn();

	m_stcAdditionalDist.SetTextFmt(_LS(IDS_TM_DISTANCE_S));
	m_stcAdditionalDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAdditionalDistI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAdditionalDistJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bAdditionalDistJ = FALSE;

	UpdateData(FALSE);

	OnTmAdditionalDistJChk();

	AlignControl();

	m_nSelType = -1;
	DirectionCtrlMan();

	m_bInitialized = TRUE;
	m_bFirstInit = FALSE;

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldLbldDlg::OnTmExecute() 
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	
	CArray<int, int> aKeyNode;
	CString csNodeList;
	int i;
	double wx=0, wy=0, wz=0;
	CArray<double, double> aDistList;

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_BMLD_D data;
	data.Initialize();
	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	int nLoadType = m_cboLoadType.GetCurSel();

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	if (m_nOption == 0 || m_nOption == 1) // add or replace
	{
		//if(m_nSelType == 0) // On the Loading Line 
		{
			m_wndNodeList.GetWindowText(csNodeList);
			if(!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
			{
				AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Select_two_nodes_));
				return;
			}
			if(aKeyNode.GetSize() == 2 && aKeyNode[0] == aKeyNode[1])
			{       
				AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Select_two_nodes_));
				return;      
			}
		}
		
		if(m_nSelType == 1)  // Selected Element 
		{
			 pIGM->GetSelectedElemKeyList(aSelKey);  
		}
		
		data.LoadCaseKey = nLoadCaseKey;

		// Uniform이 콤보로 삽입되었기 때문에 LoadType변경
		switch (nLoadType)
		{
		case CONCENTRATED_FORCES:
		case CONCENTRATED_MOMENTS_TORSIONS:
		case UNIFORM_LOADS:
		case UNIFORM_MOMENTS_TORSIONS:
			data.LoadType = nLoadType + 1;
			break;
		case TRAPEZOIDAL_LOADS:
			data.LoadType = 3;
			break;
		case TRAPEZOIDAL_MOMENTS_TORSIONS:
			data.LoadType = 4;
			break;
		case CURVED_LOADS:
			data.LoadType = 3;  // Curved Load는 Trapezoidal Load처럼 취급
			break;
		case UNIFORM_PRESSURE:
			data.LoadType = 5;
			break;
		case TRAPEZOIDAL_PRESSURE:
			data.LoadType = 5;
			break;
		default:
			ASSERT(false);
			break;
		}

		data.LoadDirection = (int)m_wndDirection.GetItemData(m_wndDirection.GetCurSel());
		data.Projection = (m_nProj==0)?1:0;
		data.LoadCommandType = 1; // BML, LBL, FBL, TBL;
		double *p;

		data.GroupKey = LdgrK;

		p = data.Force_Type[data.LoadType-1];
		// Uniform이 콤보로 삽입되었기 때문에 변경 
		BOOL bUniform = (nLoadType == UNIFORM_LOADS || nLoadType == UNIFORM_MOMENTS_TORSIONS) ? TRUE : FALSE;
		if(nLoadType == UNIFORM_PRESSURE) bUniform = TRUE;
		if(nLoadType == TRAPEZOIDAL_PRESSURE) bUniform = FALSE;

		if (bUniform)  // Uniform
		{
			for(int i=0; i < 4; i++)
				data.DistanceRatio[i] = m_ValueD[i];
			p[0] = m_ValueP[0].GetEditValue();
			for (i = 1; i < 4; i++)
			{
				if (data.DistanceRatio[i] != 0) p[i] = p[0];
				else p[i] = 0;
			}
		}
		else // Non-Uniform
		{
			if(nLoadType != CURVED_LOADS)
			{
				for(int i=0; i < 4; i++)
				{
					data.DistanceRatio[i] = m_ValueD[i];
					p[i] = m_ValueP[i].GetEditValue();
				}
			}
			else  // Curved Load인 경우
			{
				for(i = 0; i < 4; i++)
				{
					data.DistanceRatio[i] = i / 3.;          
					p[i] = 1.0;
				}        
			}
		}

		double ux = 0.0, uy = 0.0, uz = 0.0;
		if (m_bCopyLoad)
		{
			switch(m_nAxis)
			{
			case 0: ux = 1.0; break;
			case 1: uy = 1.0; break;
			case 2: uz = 1.0; break;
			}
			pIGM->GetWcsVector(ux, uy, uz, wx, wy, wz);

			if(!GetUEDistance(m_csDistance, aDistList) || aDistList.GetSize() <= 0)
			{
				AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_value_));
				return;
			}
		}

		if(data.LoadType == 5)
		{
			data.bAdditionalH = (m_chkAdditionalH.GetCheck() != 0) ? TRUE : FALSE;
			if(data.bAdditionalH)
			{
				data.dAdditionalDistI = m_edtAdditionalDistI.GetEditValue();
				data.bAdditionalDistJ = m_bAdditionalDistJ;
				if(data.bAdditionalDistJ) data.dAdditionalDistJ = m_edtAdditionalDistJ.GetEditValue();
			}

			data.bEccentricity = FALSE;
			data.nEccenType    = 0;
			data.nEccenDir     = 2;
			data.dDistI        = 0.0;
			data.dDistJ        = 0.0;
			data.bDistJ        = FALSE;
		}
		else
		{
			data.bEccentricity = (m_chkEccen.GetCheck() != 0) ? TRUE : FALSE;
			if(data.bEccentricity)
			{
				data.nEccenType = m_nEccenType;
				CDlgUtil::CobxGetItemDataByCurSel(m_cmbEccenDir, (DWORD&)data.nEccenDir);
				data.dDistI = m_edtEccenDistI.GetEditValue();
				data.bDistJ = (m_chkEccenDistJ.GetCheck() != 0) ? TRUE : FALSE;
				if (data.bDistJ) data.dDistJ = m_edtEccenDistJ.GetEditValue();
			}

			data.bAdditionalH     = FALSE;
			data.dAdditionalDistI = 0.0;
			data.dAdditionalDistJ = 0.0;
			data.dAdditionalDistJ = FALSE;
		}
	}
	else  // delete
	{
		pIGM->GetSelectedElemKeyList(aSelKey);
		T_ELEM_D ElemData;
	
		// Remove element which is not beam type
		for (int i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
		}
		if (aSelKey.GetSize() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
			return;
		}
	}

	BOOL bSuccess=FALSE;
	UINT NodeI, NodeJ;
	NodeI = NodeJ = 0;

	if(aKeyNode.GetSize() == 2)
	{
		NodeI = aKeyNode[0];
		NodeJ = aKeyNode[1];
	}

	BOOL bAbsolute = FALSE, bReplace = FALSE;
	if(m_nRelative == 1) bAbsolute = TRUE;
	if(m_nOption == 1) bReplace = TRUE;
	double da = m_wndA.GetEditValue();
	double db = m_wndB.GetEditValue();
	double dc = m_wndC.GetEditValue();        
	BOOL bCurvedLoad = FALSE;
	if(nLoadType == CURVED_LOADS) bCurvedLoad = TRUE;

	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddLbld(NodeI, NodeJ, data, bReplace, m_nSelType, aSelKey,
																						m_bCopyLoad, wx, wy, wz, aDistList, bAbsolute,
																						bCurvedLoad, m_nCurveMethod, da, db, dc);
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelLbld(aSelKey, nLoadCaseKey,LdgrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
}

void CStldLbldDlg::OnTmClose() 
{
	CloseDlg();
}

void CStldLbldDlg::OnClickLoadCaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CStldLbldDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

void CStldLbldDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD: m_nOption = 0; break;
	case IDC_TM_OPT_REPLACE: m_nOption = 1; break;
	case IDC_TM_OPT_DELETE: m_nOption = 2; break;
	}
	
	int nSel = m_CobxSelType.GetCurSel();
	int nType = (int)m_CobxSelType.GetItemData(nSel);

	if (m_nOption == 2) // delete 
	{
		EnableValueEdit(FALSE);
		GetDlgItem(IDC_TM_COPY_LOAD)->EnableWindow(FALSE);
		CtrlEnableDisable(m_aCopyLoadCtrl, FALSE);
		CtrlEnableDisable(m_aLengOptCtrl, FALSE);
		CtrlEnableDisable(m_aNodeListCtrl, FALSE);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
		m_CobxSelType.EnableWindow(FALSE);
		EnableEccentricity(FALSE);
		EnableAdditionalH(FALSE);
		EnableCurvedType(FALSE);
	}
	else 
	{
		EnableValueEdit(TRUE);		
		EnableValueByUniform(m_cboLoadType.GetCurSel());

		if(nType == LBLD_SELECTED_ELEMENT) { GetDlgItem(IDC_TM_COPY_LOAD)->EnableWindow(FALSE); }
		else                               { GetDlgItem(IDC_TM_COPY_LOAD)->EnableWindow(TRUE);  }
		
		CtrlEnableDisable(m_aCopyLoadCtrl, m_bCopyLoad);
		CtrlEnableDisable(m_aLengOptCtrl, TRUE);
		CtrlEnableDisable(m_aNodeListCtrl, TRUE);
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
		m_CobxSelType.EnableWindow(TRUE);
		EnableEccentricity(TRUE);
		EnableAdditionalH(TRUE);
		EnableCurvedType(TRUE);
	}
}

void CStldLbldDlg::OnChangeLoadType() 
{
	// TODO: Add your control notification handler code here
	int nLoadType = m_cboLoadType.GetCurSel();
	SetUnitType(nLoadType);
	EnableValueByUniform(nLoadType);	
	ChangeBitmap(nLoadType);
	ChangeValueTitle(nLoadType);
	DirectionCtrlMan();
	CurveValueCtrlMan();  
	CDlgUtil::CtrlShowHide(this, m_aCtrlLine, nLoadType != CURVED_LOADS);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCurve, nLoadType == CURVED_LOADS);
	EnableProjection(TRUE);
	
	// Concentrated Force, Uniform Load, Trapezoidal Load, Curved Load
	if (nLoadType == CONCENTRATED_FORCES || nLoadType == UNIFORM_LOADS || nLoadType == TRAPEZOIDAL_LOADS || nLoadType == CURVED_LOADS)
	{
		m_chkEccen.SetCheck(FALSE);
		OnClickEccenBtn();
		m_chkAdditionalH.SetCheck(FALSE);
		OnClickAdditionalDIstBtn();
		EnableEccentricity(TRUE);
		EnableAdditionalH(FALSE);
	}
	else if (nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE)
	{
		m_chkEccen.SetCheck(FALSE);
		OnClickEccenBtn();
		m_chkAdditionalH.SetCheck(TRUE);
		OnClickAdditionalDIstBtn();
		EnableEccentricity(FALSE);
		EnableAdditionalH(TRUE);
	}
	else
	{
		m_chkEccen.SetCheck(FALSE);
		OnClickEccenBtn();
		m_chkAdditionalH.SetCheck(FALSE);
		OnClickAdditionalDIstBtn();
		EnableEccentricity(FALSE);
		EnableAdditionalH(FALSE);
	}

	AlignControl();
}

void CStldLbldDlg::OnSelchangeTmSelCombo() 
{
	DirectionCtrlMan();	
}

void CStldLbldDlg::OnTmEccenDist2Chk() 
{
	BOOL bEnable = m_chkEccenDistJ.GetCheck() != 0 ? TRUE : FALSE;
	m_edtEccenDistJ.EnableWindow(bEnable);
}

void CStldLbldDlg::OnTmAdditionalDistJChk() 
{
	UpdateData(TRUE);

	m_edtAdditionalDistJ.EnableWindow(m_bAdditionalDistJ);
}

void CStldLbldDlg::OnChangeDirection() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_wndDirection.GetCurSel();
	m_nDirection = (int)m_wndDirection.GetItemData(nSel);
	EnableProjection(TRUE);	
}

void CStldLbldDlg::OnTmCopyLoad() 
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_TM_COPY_LOAD);
	m_bCopyLoad = pButton->GetCheck();
	CtrlEnableDisable(m_aCopyLoadCtrl, m_bCopyLoad);		
}

void CStldLbldDlg::OnTmCurveMtdRdo() 
{
	CurveValueCtrlMan();
}

void CStldLbldDlg::OnClickEccenBtn()
{
	CArray<UINT, UINT> aEccen;
	aEccen.Add(IDC_TM_ECCEN_FRM);
	aEccen.Add(IDC_TM_ECCEN_OFFSET_RDO);
	aEccen.Add(IDC_TM_ECCEN_CENTROID_RDO);
	aEccen.Add(IDC_TM_ECCEN_DIR_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIR_CMB);
	aEccen.Add(IDC_TM_ECCEN_DIST_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST1_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST1_EDT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_EDT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_CHK);

	CDlgUtil::CtrlEnableDisable(this, aEccen, m_chkEccen.GetCheck());

	if(m_chkEccen.GetCheck())
		OnTmEccenDist2Chk();
}

void CStldLbldDlg::OnClickAdditionalDIstBtn()
{
	CArray<UINT, UINT> aAdditionalH;
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_H_FRM);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_STC);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_I_END_STC);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_I_END_EDT);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_STC);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_EDT);
	aAdditionalH.Add(IDC_TM_STLD_LBLD_ADDITIONAL_DISTANCE_J_END_CHK);

	CDlgUtil::CtrlEnableDisable(this, aAdditionalH, m_chkAdditionalH.GetCheck());


	if (m_chkAdditionalH.GetCheck())
	{
		OnTmAdditionalDistJChk();
	}

	// TODO - Load Type -> enum
	const int nLoadType = m_cboLoadType.GetCurSel();
	if (nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE)
	{
		DirectionCtrlMan();
	}
}

LRESULT CStldLbldDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wndNodeList.GetSafeHwnd() == (HWND)lParam)
	{
		CArray<int, int> aKeyNode;
		CString csNodeList;
		m_wndNodeList.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
			return 0L;
		OnTmExecute();
	}
	return 0L;
}

LRESULT CStldLbldDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_wndNodeList.GetSafeHwnd() == (HWND)lParam)
	{
		CArray<int, int> aKeyNode;
		CString csNodeList;
		m_wndNodeList.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
			return 0L;
		OnTmExecute();
	}
	return 0L;
}

LRESULT CStldLbldDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CArray<UINT, UINT> aEccen;
	aEccen.Add(IDC_TM_ECCEN_FRM);
	aEccen.Add(IDC_TM_ECCEN_OFFSET_RDO);
	aEccen.Add(IDC_TM_ECCEN_CENTROID_RDO);
	aEccen.Add(IDC_TM_ECCEN_DIR_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIR_CMB);
	aEccen.Add(IDC_TM_ECCEN_DIST_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST1_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST1_EDT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_TXT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_EDT);
	aEccen.Add(IDC_TM_ECCEN_DIST2_CHK);

	if (!m_bInitialized) return 0L;
	CWnd* pParent = GetParent();
	pParent = pParent->GetParent();
	((CStructStldDlg*)pParent)->LayoutChildDialog();

	int nLoadType = m_cboLoadType.GetCurSel();
	if(nLoadType == UNIFORM_PRESSURE || nLoadType == TRAPEZOIDAL_PRESSURE) // MQC:3822 by KJH 입력제한 함
	{
		CDlgUtil::CtrlShowHide(this, aEccen, FALSE);
		DirectionCtrlMan();
	}

	return 0L;
}