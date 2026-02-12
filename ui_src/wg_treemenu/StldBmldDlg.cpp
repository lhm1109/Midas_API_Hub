// StldBmldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldBmldDlg.h"

#include "..\wg_base\DlgUtil.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "StructStldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldBmldDlg dialog

CStldBmldDlg::CStldBmldDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldBmldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldBmldDlg)
	m_nOption    = -1;
	m_nProj      = -1;
	m_nRelative  = 0;
	m_nDirection = 0;
	//}}AFX_DATA_INIT
	m_pBitmap = 0;
	for(int i=0; i<4; i++) m_ValueD[i] = 0;

	m_bInitialized      = FALSE;
	m_bFirstInit        = TRUE;
	m_bAdditionalDistJ  = FALSE;

	m_aLengOptCtrl.Add(IDC_TM_RELATIVE);
	m_aLengOptCtrl.Add(IDC_TM_ABSOLUTE);

	m_pDoc = CDBDoc::GetDocPoint();

	m_aEccen.Add(IDC_TM_ECCEN_FRM);
	m_aEccen.Add(IDC_TM_ECCEN_OFFSET_RDO);
	m_aEccen.Add(IDC_TM_ECCEN_CENTROID_RDO);
	m_aEccen.Add(IDC_TM_ECCEN_DIR_TXT);
	m_aEccen.Add(IDC_TM_ECCEN_DIR_CMB);
	m_aEccen.Add(IDC_TM_ECCEN_DIST_TXT);
	m_aEccen.Add(IDC_TM_ECCEN_DIST1_TXT);
	m_aEccen.Add(IDC_TM_ECCEN_DIST1_EDT);
	m_aEccen.Add(IDC_TM_ECCEN_DIST2_TXT);
	m_aEccen.Add(IDC_TM_ECCEN_DIST2_EDT);
	m_aEccen.Add(IDC_TM_ECCEN_DIST2_CHK);

	m_aEtc.Add(IDC_TM_DIR_STC);       m_aEtc.Add(IDC_TM_DIR_COMBO);
	m_aEtc.Add(IDC_TM_BML_PROJ_STC);  m_aEtc.Add(IDC_TM_BML_PROJ0);
	m_aEtc.Add(IDC_TM_BML_PROJ1);     m_aEtc.Add(IDC_TM_BML_VALUE_GRP);
	m_aEtc.Add(IDC_TM_RELATIVE);      m_aEtc.Add(IDC_TM_ABSOLUTE);
	m_aEtc.Add(IDC_TM_BML_D1_STC);    m_aEtc.Add(IDC_TM_BML_D1);
	m_aEtc.Add(IDC_TM_FM_TITLE1);     m_aEtc.Add(IDC_TM_BML_P1);
	m_aEtc.Add(IDC_TM_BML_D2_STC);    m_aEtc.Add(IDC_TM_BML_D2);
	m_aEtc.Add(IDC_TM_FM_TITLE2);     m_aEtc.Add(IDC_TM_BML_P2);
	m_aEtc.Add(IDC_TM_BML_D3_STC);    m_aEtc.Add(IDC_TM_BML_D3);
	m_aEtc.Add(IDC_TM_FM_TITLE3);     m_aEtc.Add(IDC_TM_BML_P3);
	m_aEtc.Add(IDC_TM_BML_D4_STC);    m_aEtc.Add(IDC_TM_BML_D4);
	m_aEtc.Add(IDC_TM_FM_TITLE4);     m_aEtc.Add(IDC_TM_BML_P4);
	m_aEtc.Add(IDC_TM_BML_UP_STC);    m_aEtc.Add(IDC_TM_BML_UP1);

	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_H_FRM);
	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_H_CHK);
	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_STC);
	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_I_END_STC);
	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_I_END_EDT);
	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_STC);
	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_EDT);
	m_aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_CHK);
}

CStldBmldDlg::~CStldBmldDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CStldBmldDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldBmldDlg)
	DDX_Control(pDX, IDC_TM_BML_CBO_LC,       m_LoadCase);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC,      m_wndLdgrCobx);
	DDX_Radio  (pDX, IDC_TM_BML_OPT_ADD,      m_nOption);
	DDX_Control(pDX, IDC_TM_BML_LOAD_TYPE,    m_cboLoadType);
	DDX_Control(pDX, IDC_TM_ECCEN_CHK,        m_chkEccen);
	DDX_Radio  (pDX, IDC_TM_ECCEN_CENTROID_RDO, m_nEccenType);
	DDX_Control(pDX, IDC_TM_ECCEN_DIR_CMB,    m_cmbEccenDir);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST_TXT,   m_txtEccenDist);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST1_EDT,  m_edtEccenDistI);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST2_EDT,  m_edtEccenDistJ);
	DDX_Control(pDX, IDC_TM_ECCEN_DIST2_CHK,  m_chkEccenDistJ);
	DDX_Control(pDX, IDC_TM_STLD_BMLD_ADDITIONAL_H_CHK,               m_chkAdditionalH);
	DDX_Control(pDX, IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_STC,        m_stcAdditionalDist);
	DDX_Check  (pDX, IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_CHK,  m_bAdditionalDistJ);
	DDX_Control(pDX, IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_I_END_EDT,  m_edtAdditionalDistI);
	DDX_Control(pDX, IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_EDT,  m_edtAdditionalDistJ);
	DDX_CBIndex(pDX, IDC_TM_DIR_COMBO,  m_nDirection);
	DDX_Control(pDX, IDC_TM_DIR_COMBO,  m_wndDirection);
	DDX_Radio  (pDX, IDC_TM_BML_PROJ0,  m_nProj);
	DDX_Radio  (pDX, IDC_TM_RELATIVE,   m_nRelative);
	DDX_Control(pDX, IDC_TM_BML_D1,     m_wndD1);
	DDX_Control(pDX, IDC_TM_BML_D2,     m_wndD2);
	DDX_Control(pDX, IDC_TM_BML_D3,     m_wndD3);
	DDX_Control(pDX, IDC_TM_BML_D4,     m_wndD4);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndBmldTypePicture);
	//}}AFX_DATA_MAP
	UINT nID[][2] = {
		{IDC_TM_BML_D1, IDC_TM_BML_P1},
		{IDC_TM_BML_D2, IDC_TM_BML_P2},
		{IDC_TM_BML_D3, IDC_TM_BML_P3},
		{IDC_TM_BML_D4, IDC_TM_BML_P4},
	};
	DDX_Control(pDX, IDC_TM_BML_UP1, m_Unit);
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
}

void CStldBmldDlg::InitLoadTypeCombo()
{
	m_cboLoadType.ResetContent();

	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Concentrated_Forces));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Concentrated_Moments_Torsions));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Uniform_Loads));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Uniform_Moments_Torsions));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Trapezoidal_Loads));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Trapezoidal_Moments_Torsions));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Uniform_Pressure));
	m_cboLoadType.AddString(_LS(IDS_WG_TREEMENU_Trapezoidal_Pressure));

	m_cboLoadType.SetCurSel(2); // Uniform Load
}

void CStldBmldDlg::InitEccenDirectionCombo()
{
	// Eccen. from Centroid 안의 Direction Combo Box 
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Local_y), 2);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Local_z), 3);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Global_X), 4);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Global_Y), 5);
	CDlgUtil::CobxAddItem(m_cmbEccenDir, _LS(IDS_WG_TREEMENU_Global_Z), 6);
	CDlgUtil::CobxSetCurSelItemData(m_cmbEccenDir, 2);  // LY
}

void CStldBmldDlg::InitDirectionCombo()
{
	UpdateData(TRUE);

	int nLoadType = m_cboLoadType.GetCurSel()+1;

	m_wndDirection.ResetContent();

	if(nLoadType == 1 || nLoadType == 2 || nLoadType == 3 ||
		 nLoadType == 4 || nLoadType == 5 || nLoadType == 6)
	{
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_x));   m_wndDirection.SetItemData(0, 1);
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_y));   m_wndDirection.SetItemData(1, 2);
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_z));   m_wndDirection.SetItemData(2, 3);
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X));  m_wndDirection.SetItemData(3, 4);
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y));  m_wndDirection.SetItemData(4, 5);
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z));  m_wndDirection.SetItemData(5, 6);

		m_wndDirection.SetCurSel(5); // Global Z
	}
	else if(nLoadType == 7 || nLoadType == 8) // MQC:3822 by KJH 입력제한 함
	{
		BOOL bNoAdditionalH = (m_chkAdditionalH.GetCheck() == 0) ? TRUE : FALSE;
		m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_y));   m_wndDirection.SetItemData(0, 2);
		if(bNoAdditionalH)
			m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_z ));m_wndDirection.SetItemData(1, 3);

		m_wndDirection.SetCurSel(0); // Local y
	}
}

void CStldBmldDlg::AlignControl()
{
	UpdateData(TRUE);

	int nLoadType = m_cboLoadType.GetCurSel()+1;

	CArray<UINT, UINT> aApplyCancel;

	aApplyCancel.Add(IDC_TM_EXECUTE);
	aApplyCancel.Add(IDC_TM_CLOSE);

	// Additional H 를 Eccen. form Centroid 위치로 옮긴다.
	CRect rRef;
	CRect rMove;
	int nDistY;
	
	GetDlgItem(IDC_TM_ECCEN_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_STLD_BMLD_ADDITIONAL_H_FRM)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aAdditionalH, nDistY);

	BOOL bPressure = FALSE;
	if(nLoadType == 7 || nLoadType == 8) bPressure = TRUE;
	
	if(bPressure)
	{
		CDlgUtil::CtrlShowHide(this, m_aEccen, FALSE);
		//CDlgUtil::CtrlShowHide(this, aEccen2, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aAdditionalH, TRUE);

		GetDlgItem(IDC_TM_STLD_BMLD_ADDITIONAL_H_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_DIR_STC)->GetWindowRect(rMove);

		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(15);
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

		m_chkEccen.SetCheck(FALSE);
		OnClickEccenBtn();
		m_chkAdditionalH.SetCheck(TRUE);
		OnClickAdditionalDIstBtn();
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aEccen, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aAdditionalH, FALSE);
		GetDlgItem(IDC_TM_ECCEN_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_DIR_STC)->GetWindowRect(rMove);
			
		nDistY = rRef.top - rMove.top + globalUtils.ScaleByDPI(15);
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);
		CDlgUtil::CtrlShowHide(this, m_aEtc, TRUE);	
	}
	
	GetDlgItem(IDC_TM_BML_VALUE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EXECUTE)->GetWindowRect(rMove);
	nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aApplyCancel, nDistY);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_BML_VALUE_GRP);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(180);

	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);

	RedrawWindow();
}

void CStldBmldDlg::SetUnitType()
{
	UpdateData(TRUE);

	int nLoadType = m_cboLoadType.GetCurSel()+1;

	if(nLoadType == 5 || nLoadType == 6) nLoadType -= 2; // 분포하중 
	if(nLoadType == 7 || nLoadType == 8) nLoadType = 5;  // 압력하중

	int nUnitType;
	nUnitType = CUnitCtrl::m_BMLD_UNIT.Force_Type[nLoadType-1];

	m_Unit.SetUnitType(nUnitType);
	for (int i = 0; i < 4; i++)
	{
		m_ValueP[i].SetUnitType(nUnitType);
	}
}

void CStldBmldDlg::ChangeBitmap(int nBitmap)
{
#if 0
	UINT aBitmapID[] = { 
		IDB_TM_BMLD_1, IDB_TM_BMLD_2, IDB_TM_BMLD_3, 
		IDB_TM_BMLD_4, IDB_TM_BMLD_5, IDB_TM_BMLD_6,
		IDB_TM_BMLD_8, IDB_TM_BMLD_9,
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
		_T("SVG\\illustration\\Dialog\\tm_bmld8.svg"),
		_T("SVG\\illustration\\Dialog\\tm_bmld9.svg")
	};
	m_wndBmldTypePicture.SetImage(aBmldTypePicture[nBitmap - 1]);
#endif
}

void CStldBmldDlg::ChangeValueTitle(int nLoadType)
{
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
		{_T("p"), _T(""), _T(""), _T("")},
		{_LS(IDS_WG_TREEMENU_P1), _LS(IDS_WG_TREEMENU_P2), _LS(IDS_WG_TREEMENU_P3), _LS(IDS_WG_TREEMENU_P4)},
	};
	CBCGPStatic* pTitleWnd;
	for (int i = 0; i < sizeof(aTitleID)/sizeof(UINT); i++)
	{
		pTitleWnd = (CBCGPStatic*)GetDlgItem(aTitleID[i]);
		pTitleWnd->SetWindowText(aName[nLoadType-1][i]);
	}
}

void CStldBmldDlg::EnableEccentricity(BOOL bEnable)
{
	if (bEnable)
	{
		int nSel = m_cboLoadType.GetCurSel();
		if (nSel == 0 || nSel == 2 || nSel == 4)  
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

void CStldBmldDlg::EnableAdditionalH(BOOL bEnable)
{
	UpdateData(TRUE);
	
// 	CArray<UINT, UINT> aCtrlAdditional;
// 	//aCtrlAdditional.Add(IDC_TM_STLD_BMLD_ADDITIONAL_H_FRM);
// 	aCtrlAdditional.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_I_END_EDT);
// 	aCtrlAdditional.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_EDT);
// 	aCtrlAdditional.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_CHK);
// 	CDlgUtil::CtrlEnableDisable(this, aCtrlAdditional, bEnable);
	m_chkAdditionalH.EnableWindow(bEnable); // Enable Window가 virtual이 아니므로 별도 처리
	if (bEnable) OnTmAdditionalDistJChk();

	InitDirectionCombo();
}

// UpdataData(TRUE)가 호출된 후 이 함수가 호출되어야 정상 동작한다.
void CStldBmldDlg::EnableProjection(BOOL bEnable)
{
	UINT nID[] = {
		IDC_TM_BML_PROJ0, IDC_TM_BML_PROJ1,
	};
	CWnd* pWnd;

	// Uniform이 콤보로 삽입되었기 때문에 변경
	int nLoadType = m_cboLoadType.GetCurSel()+1;
	if (nLoadType == 5 || nLoadType == 6) nLoadType -= 2;
	if (nLoadType == 7 || nLoadType == 8) nLoadType = 5;
	if (bEnable)
	{
		if (nLoadType < 3) bEnable = FALSE;
		if (m_nDirection <= 3) bEnable = FALSE;
		if (nLoadType == 5) bEnable = FALSE;
	}
	for (int i = 0; i < sizeof(nID)/sizeof(UINT); i++)
	{
		pWnd = GetDlgItem(nID[i]);
		ASSERT(pWnd);
		pWnd->EnableWindow(bEnable);
	}
}

// UpdataData(TRUE)가 호출된 후 이 함수가 호출되어야 정상 동작한다.
void CStldBmldDlg::EnableValueByUniform(int nUniform)
{
	// Uniform이 콤보로 삽입되었기 때문에 nUniform값 변경
	// Uniform은 Distributed만 있다.
	if(nUniform == 3 || nUniform == 4) nUniform = 0;
	else if (nUniform == 7) nUniform = 0;
	else nUniform = 1;

	UINT nID1[] = {
		IDC_TM_BML_D1, IDC_TM_BML_D2, IDC_TM_BML_D3, IDC_TM_BML_D4,
		IDC_TM_BML_P1, IDC_TM_BML_P2, IDC_TM_BML_P3, IDC_TM_BML_P4,
	};
	UINT nID2[] = {
		IDC_TM_BML_P2, IDC_TM_BML_P3, IDC_TM_BML_P4,
	};

	if (m_nOption == 2) return; // delete이면 그냥 리턴한다.
	int nLoadType = m_cboLoadType.GetCurSel()+1;
	// Uniform이 콤보로 삽입되었기 때문에 nLoadType값 변경 
	if(nLoadType == 5 || nLoadType == 6) nLoadType -= 2;
	if(nLoadType == 7 || nLoadType == 8) nLoadType = 5; 

	int i;
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
		case 1: case 2: // concentrated force or moment
			// D1, D2, D3, D4를 모두 Enable시킨다.
			for(int i=0; i < 4; i++)
			{
				GetDlgItem(nID1[i])->EnableWindow(TRUE);
				m_ValueD[i] = 0; 
			}
			break;
		case 3: case 4: // distributed force or moment
			// D1, D2는 Enable 시킨다.
			for(int i=0; i < 2; i++)
				GetDlgItem(nID1[i])->EnableWindow(TRUE);
			// D3, D4는 Disable시킨다.
			for (i = 2; i < 4; i++)
				GetDlgItem(nID1[i])->EnableWindow(FALSE);
			// D2의 초기값을 1로 하고 나머지는 모두 0으로 한다.
			for(int i=0; i < 4; i++) 
				m_ValueD[i] = 0;
			m_ValueD[1] = 1;
			
			break;
		case 5: // Pressure
			// D1, D2는 Enable 시킨다.
			for(int i=0; i < 2; i++)
				GetDlgItem(nID1[i])->EnableWindow(TRUE);
			// D3, D4는 Disable시킨다.
			for (i = 2; i < 4; i++)
				GetDlgItem(nID1[i])->EnableWindow(FALSE);
			// D2의 초기값을 1로 하고 나머지는 모두 0으로 한다.
			for(int i=0; i < 4; i++) 
				m_ValueD[i] = 0;
			m_ValueD[1] = 1;
			
			break;
		}
	}
	for(int i=0; i < 4; i++) m_ValueP[i].SetWindowText(_T("0"));
	UpdateData(FALSE);
}

// UpdataData(TRUE)가 호출된 후 이 함수가 호출되어야 정상 동작한다.
void CStldBmldDlg::EnableValueEdit(BOOL bEnable)
{
	UINT nID[] = {
		IDC_TM_BML_LOAD_TYPE, 
		IDC_TM_DIR_COMBO,
		IDC_TM_BML_D1, IDC_TM_BML_D2, IDC_TM_BML_D3, IDC_TM_BML_D4,
		IDC_TM_BML_P1, IDC_TM_BML_P2, IDC_TM_BML_P3, IDC_TM_BML_P4,
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

void CStldBmldDlg::PostNcDestroy() 
{
	m_bInitialized = FALSE;
	CMenuBarChildDlg::PostNcDestroy();
}


BEGIN_MESSAGE_MAP(CStldBmldDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldBmldDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BML_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_BML_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_BML_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_BML_OPT_DELETE, OnChangeOption)
	ON_CBN_SELCHANGE(IDC_TM_BML_LOAD_TYPE, OnChangeLoadType)
	ON_BN_CLICKED(IDC_TM_ECCEN_DIST2_CHK, OnTmEccenDist2Chk)
	ON_BN_CLICKED(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_CHK, OnTmAdditionalDistJChk)
	ON_CBN_SELCHANGE(IDC_TM_DIR_COMBO, OnChangeDirection)
	ON_BN_CLICKED(IDC_TM_ECCEN_CHK, OnClickEccenBtn)
	ON_BN_CLICKED(IDC_TM_STLD_BMLD_ADDITIONAL_H_CHK, OnClickAdditionalDIstBtn)
	//}}AFX_MSG_MAP
	//ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldBmldDlg message handlers

BOOL CStldBmldDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);
	CMenuBarChildDlg::OnInitDialog();
	
	InitLoadTypeCombo();

	int nLoadType = m_cboLoadType.GetCurSel()+1;

	InitDirectionCombo();
	SetUnitType();
	ChangeBitmap(nLoadType);
	ChangeValueTitle(nLoadType);

	m_nOption    = 0;    
	m_nEccenType = 0;
	m_nDirection = 5; // default : Global Z
	m_nProj      = 1;
	
	EnableProjection(TRUE); // TRUE를 넘겨주더라도 조건에 따라서 FALSE가 된다.
	EnableValueByUniform(nLoadType);
	m_nRelative = 0;  // default : relative

	InitEccenDirectionCombo();
	
	// Eccentricity 안의 Distance 설정..
//	if (!m_bFirstInit) m_chkEccen.ReInit();
//	m_chkEccen.InitControl(this, CStldBmldDlg::IDD, IDC_TM_ECCEN_FRM,TRUE,TRUE);
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
//	m_chkAdditionalH.InitControl(this, CStldBmldDlg::IDD, IDC_TM_STLD_BMLD_ADDITIONAL_H_FRM,TRUE,TRUE);
//	m_chkAdditionalH.SetDefault(FALSE);
	m_chkAdditionalH.SetCheck(FALSE);
	OnClickAdditionalDIstBtn();

	m_stcAdditionalDist.SetTextFmt(_LS(IDS_TM_DISTANCE_S));
	m_stcAdditionalDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAdditionalDistI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAdditionalDistJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bAdditionalDistJ = FALSE;

	UpdateData(FALSE);	

	AlignControl();

	OnTmAdditionalDistJChk();

	m_bInitialized = TRUE;
	m_bFirstInit = FALSE;

	if(GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	if (m_chkEccen.GetCheck())
	{
		CtrlShowHide(m_aEccen, TRUE);
	}
	else
	{
		CtrlShowHide(m_aEccen, FALSE);
	}

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldBmldDlg::OnTmExecute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_ELEM_D ElemData;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
	}
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_BMLD_D data;
	data.Initialize();
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	data.GroupKey = LdgrK;

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	if (m_nOption == 0 || m_nOption == 1)
	{
		int nLoadType = m_cboLoadType.GetCurSel()+1;
		data.LoadCaseKey = nLoadCaseKey;
		data.LoadType = nLoadType;

		// Uniform이 콤보로 삽입되었기 때문에 LoadType변경
		if(data.LoadType == 5 || data.LoadType == 6) data.LoadType -= 2;
		if(data.LoadType == 7 || data.LoadType == 8) data.LoadType = 5;

		//data.LoadDirection = m_nDirection+1;
		data.LoadDirection = (int)m_wndDirection.GetItemData(m_wndDirection.GetCurSel());
		data.Projection = (m_nProj==0)?1:0;
		data.LoadCommandType = 1; // BML, LBL, FBL, TBL;
		double *p;
		p = data.Force_Type[data.LoadType-1];
		// Uniform이 콤보로 삽입되었기 때문에 변경 
		BOOL bUniform = (nLoadType == 3 || nLoadType == 4) ? TRUE : FALSE;
		if(nLoadType == 7) bUniform = TRUE;
		if(nLoadType == 8) bUniform = FALSE;

		if (bUniform) // Uniform
		{
			for(int i=0; i < 4; i++)
				data.DistanceRatio[i] = m_ValueD[i];
			p[0] = m_ValueP[0].GetEditValue();
			for (int i = 1; i < 4; i++)
			{
				if (data.DistanceRatio[i] != 0) p[i] = p[0];
				else p[i] = 0;
			}
		}
		else // Non-Uniform
		{
			for(int i=0; i < 4; i++)
			{
				data.DistanceRatio[i] = m_ValueD[i];
				p[i] = m_ValueP[i].GetEditValue();
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

	BOOL bSuccess=FALSE;
	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddBmld(aSelKey, data, 
																						(m_nOption==0)?FALSE:TRUE,
																						(m_nRelative==0)?FALSE:TRUE);	
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelBmld(aSelKey, nLoadCaseKey, LdgrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldBmldDlg::OnTmClose() 
{
	CloseDlg();
}

void CStldBmldDlg::OnClickLoadCaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldBmldDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);
}

void CStldBmldDlg::OnChangeOption() 
{
	UpdateData(TRUE);
	if (m_nOption == 2) // delete 
	{
		EnableValueEdit(FALSE);
		CtrlEnableDisable(m_aLengOptCtrl, FALSE);
		m_wndLdgrCobx.ShowHideAll(TRUE);
		EnableEccentricity(FALSE);
		EnableAdditionalH(FALSE);
	}
	else 
	{
		EnableValueEdit(TRUE);		
		EnableValueByUniform(m_cboLoadType.GetCurSel()+1);
		CtrlEnableDisable(m_aLengOptCtrl, TRUE);
		m_wndLdgrCobx.ShowHideAll(FALSE);
		EnableEccentricity(TRUE);
		EnableAdditionalH(TRUE);
	}
}

void CStldBmldDlg::OnChangeLoadType()
{
	UpdateData(TRUE);

	int nSel = m_cboLoadType.GetCurSel()+1;
	SetUnitType();
	EnableValueByUniform(nSel);
	ChangeBitmap(nSel);
	ChangeValueTitle(nSel);
	InitDirectionCombo();

	EnableProjection(TRUE);

	// Concentrated Force, Uniform Load, Trapezoidal Load
	if (nSel == 1 || nSel == 3 || nSel == 5)  
	{
		m_chkEccen.SetCheck(FALSE);
		OnClickEccenBtn();
		m_chkAdditionalH.SetCheck(FALSE);
		OnClickAdditionalDIstBtn();
		EnableEccentricity(TRUE);
		EnableAdditionalH(FALSE);
	}
	else
	{
		if(nSel == 7 || nSel == 8)
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
	}

	AlignControl();
	// value를 초기화 해야 한다면 여기에 넣어야 한다.
}

void CStldBmldDlg::OnTmEccenDist2Chk() 
{
	BOOL bEnable = m_chkEccenDistJ.GetCheck() != 0 ? TRUE : FALSE;
	m_edtEccenDistJ.EnableWindow(bEnable);
}

void CStldBmldDlg::OnTmAdditionalDistJChk() 
{
	UpdateData(TRUE);

	m_edtAdditionalDistJ.EnableWindow(m_bAdditionalDistJ);
}

void CStldBmldDlg::OnChangeDirection() 
{
	UpdateData(TRUE);

	int nSel = m_wndDirection.GetCurSel();
	m_nDirection = (int)m_wndDirection.GetItemData(nSel);

	EnableProjection(TRUE);
}

void CStldBmldDlg::OnClickEccenBtn()
{
	CRect rRef;
	CRect rMove;
	int nDistY;
	int nLoadType = m_cboLoadType.GetCurSel() + 1;
	if (m_chkEccen.GetCheck())
	{
		CDlgUtil::CtrlEnableDisable(this, m_aEccen, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aEccen, TRUE);
		
		OnTmEccenDist2Chk();
		CDlgUtil::CtrlShowHide(this, m_aAdditionalH, FALSE);
		GetDlgItem(IDC_TM_ECCEN_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_DIR_STC)->GetWindowRect(rMove);
		
		CtrlShowHide(m_aEtc, TRUE);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(16);
		CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);
	}
	else if (!m_chkEccen.GetCheck())
	{
		CDlgUtil::CtrlEnableDisable(this, m_aEccen, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aEccen, FALSE);
		if (nLoadType != 7 && nLoadType != 8)
			CDlgUtil::CtrlShowHide(this, m_aAdditionalH, FALSE);
		
		GetDlgItem(IDC_TM_ECCEN_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_DIR_STC)->GetWindowRect(rMove);
		
		CtrlShowHide(m_aEtc, TRUE);
		nDistY = rRef.top - rMove.top + globalUtils.ScaleByDPI(16);
		if (nLoadType != 7 && nLoadType != 8)
			CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);
	}
}

void CStldBmldDlg::OnClickAdditionalDIstBtn()
{
	CArray<UINT, UINT> aAdditionalH;
	aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_H_FRM);
	aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_STC);
	aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_I_END_STC);
	aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_I_END_EDT);
	aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_STC);
	aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_EDT);
	aAdditionalH.Add(IDC_TM_STLD_BMLD_ADDITIONAL_DISTANCE_J_END_CHK);

	CDlgUtil::CtrlEnableDisable(this, aAdditionalH, m_chkAdditionalH.GetCheck());

	if (m_chkAdditionalH.GetCheck())
		OnTmAdditionalDistJChk();

	InitDirectionCombo();
}

LRESULT CStldBmldDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CArray<UINT, UINT> aEccen;
	//aEccen.Add(IDC_TM_ECCEN_FRM);
	//aEccen.Add(IDC_TM_ECCEN_CHK);
	//aEccen.Add(IDC_TM_ECCEN_OFFSET_RDO);
	//aEccen.Add(IDC_TM_ECCEN_CENTROID_RDO);
	//aEccen.Add(IDC_TM_ECCEN_DIR_TXT);
	//aEccen.Add(IDC_TM_ECCEN_DIR_CMB);
	//aEccen.Add(IDC_TM_ECCEN_DIST_TXT);
	//aEccen.Add(IDC_TM_ECCEN_DIST1_TXT);
	//aEccen.Add(IDC_TM_ECCEN_DIST1_EDT);
	//aEccen.Add(IDC_TM_ECCEN_DIST2_TXT);
	//aEccen.Add(IDC_TM_ECCEN_DIST2_EDT);
	//aEccen.Add(IDC_TM_ECCEN_DIST2_CHK);

	if (!m_bInitialized) return 0L;
	CWnd* pParent = GetParent();
	pParent = pParent->GetParent();
	((CStructStldDlg*)pParent)->LayoutChildDialog();

	int nLoadType = m_cboLoadType.GetCurSel()+1;
	if(nLoadType == 7 || nLoadType == 8) // MQC:3822 by KJH 입력제한 함
	{
		CDlgUtil::CtrlShowHide(this, aEccen, FALSE);
		InitDirectionCombo();
	}

	return 0L;
}

BOOL CStldBmldDlg::ExternalInit_by_SelectLabel(UINT key)
{
	return FALSE;
}