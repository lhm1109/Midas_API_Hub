// ExposureClassTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ExposureClassTabDlg_RC.h"
#include "ExposureClassDlg_RC.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExposureClassTabDlg_RC dialog
CExposureClassTabDlg_RC::CExposureClassTabDlg_RC(CWnd* pParent /*=NULL*/)
	: CChildDialog(CExposureClassTabDlg_RC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExposureClassTabDlg_RC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CExposureClassDlg_RC*)pParent;

	m_Data.Initialize();

	m_bBothIJ = FALSE;
	m_nOption = 0;
	m_bUserInput = FALSE;
}

void CExposureClassTabDlg_RC::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExposureClassTabDlg_RC)
	DDX_Check  (pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK,     m_bUserInput);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB,      m_cmbTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB,   m_cmbBottom);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT,      m_edtTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT,      m_untTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT,   m_edtBottom);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT,   m_untBottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExposureClassTabDlg_RC, CChildDialog)
	//{{AFX_MSG_MAP(CExposureClassTabDlg_RC)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK, OnUserInputChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExposureClassTabDlg_RC::SetData(BOOL bBothIJ, int nOption)
{
	m_bBothIJ = bBothIJ;
	m_nOption = nOption;
}

void CExposureClassTabDlg_RC::GetData(T_RESC_D &RescD)
{
	RescD.bUserInputI = m_Data.bUserInputI;
	RescD.iExposureClassI[0] = m_Data.iExposureClassI[0];
	RescD.iExposureClassI[1] = m_Data.iExposureClassI[1];
	RescD.dCrackWidthI[0] = m_Data.dCrackWidthI[0];
	RescD.dCrackWidthI[1] = m_Data.dCrackWidthI[1];
}

void CExposureClassTabDlg_RC::InitUnit()
{
	m_edtTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTop.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtBottom.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBottom.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CExposureClassTabDlg_RC::InitComboBox()
{
	CString strCombo[NUM_AS5100] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_A),_LS(IDS_DGN_PSC_EXPOSURE_CLASS_B1),
									_LS(IDS_DGN_PSC_EXPOSURE_CLASS_B2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_C1),
									_LS(IDS_DGN_PSC_EXPOSURE_CLASS_C2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_U) };
	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for(int i=0; i< NUM_AS5100; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabDlg_RC::AlignControls()
{
	CRect rRef;
	CRect rToMove, rToMoveSTC;
	int nDistX = 0;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;

	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK)->ShowWindow(SW_HIDE);
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_STC);
	aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB);
	aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_STC);
	aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB);

	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_STC)->GetWindowRect(rToMove);

	nDistX = rRef.left - rToMove.left + globalUtils.ScaleByDPI(10);
	nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(30);

	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
}

void CExposureClassTabDlg_RC::ShowHideControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB)->ShowWindow(!m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB)->ShowWindow(!m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT)->ShowWindow(m_bUserInput);
}

void CExposureClassTabDlg_RC::ControlsEnableDisable()
{
	if(m_nOption==0)
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP       )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK  )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_STC   )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB   )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT   )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT   )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP       )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK  )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_STC   )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB   )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT   )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT   )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK)->EnableWindow(FALSE);	
}

void CExposureClassTabDlg_RC::Data2Dlg()
{
	int nMaxNum = NUM_AS5100-1;
	int nEx_TopNum = (m_Data.iExposureClassI[0] > nMaxNum)? 0 : m_Data.iExposureClassI[0];
	int nEx_BotNum = (m_Data.iExposureClassI[1] > nMaxNum)? 0 : m_Data.iExposureClassI[1];

	m_cmbTop.SetCurSel(nEx_TopNum);
	m_cmbBottom.SetCurSel(nEx_BotNum);
	m_bUserInput = FALSE;

	m_edtTop.SetEditUnit(m_Data.dCrackWidthI[0]);
	m_edtBottom.SetEditUnit(m_Data.dCrackWidthI[1]);
}

void CExposureClassTabDlg_RC::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.iExposureClassI[0] = m_cmbTop.GetCurSel();
	m_Data.iExposureClassI[1] = m_cmbBottom.GetCurSel();

	m_Data.bUserInputI = m_bUserInput;
	m_Data.dCrackWidthI[0] = m_edtTop.GetEditValue();
	m_Data.dCrackWidthI[1] = m_edtBottom.GetEditValue();
}

/////////////////////////////////////////////////////////////////////////////
// CExposureClassTabDlg_RC message handlers
BOOL CExposureClassTabDlg_RC::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_Data = m_pParent->m_Data;
	m_bBothIJ = m_pParent->m_bBothIJ;
	m_nOption = m_pParent->m_nOption;
		
	InitUnit();
	InitComboBox();
	
	AlignControls();
	ControlsEnableDisable();

	Data2Dlg();

	UpdateData(FALSE);

	OnUserInputChk();

	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP)->SetWindowText(_LS(IDS_DGN_EXPOSURE_CLASS));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CExposureClassTabDlg_RC::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_Data.iExposureClassI[0] = m_Data.iExposureClassI[0];
	m_pParent->m_Data.iExposureClassI[1] = m_Data.iExposureClassI[1];

	m_pParent->m_Data.bUserInputI = m_Data.bUserInputI;
	m_pParent->m_Data.dCrackWidthI[0] = m_Data.dCrackWidthI[0];
	m_pParent->m_Data.dCrackWidthI[1] = m_Data.dCrackWidthI[1];

	return CChildDialog::DestroyWindow();
}

void CExposureClassTabDlg_RC::OnUserInputChk()
{
	UpdateData(TRUE);

	ShowHideControls();
}