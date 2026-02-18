// ExposureClassTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ExposureClassTabJDlg_RC.h"
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
// CExposureClassTabJDlg_RC dialog
CExposureClassTabJDlg_RC::CExposureClassTabJDlg_RC(CWnd* pParent /*=NULL*/)
	: CChildDialog(CExposureClassTabJDlg_RC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExposureClassTabJDlg_RC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CExposureClassDlg_RC*)pParent;

	m_Data.Initialize();

	m_bBothIJ = FALSE;
	m_nOption = 0;
	m_bUserInput = FALSE;
}

void CExposureClassTabJDlg_RC::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExposureClassTabJDlg_RC)
	DDX_Check  (pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK,     m_bUserInput);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB,      m_cmbTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB,   m_cmbBottom);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT,      m_edtTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT,      m_untTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT,   m_edtBottom);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT,   m_untBottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExposureClassTabJDlg_RC, CChildDialog)
	//{{AFX_MSG_MAP(CExposureClassTabJDlg_RC)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK, OnUserInputChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExposureClassTabJDlg_RC::SetData(BOOL bBothIJ, int nOption)
{
	m_bBothIJ = bBothIJ;
	m_nOption = nOption;
}

void CExposureClassTabJDlg_RC::GetData(T_RESC_D &RescD)
{
	RescD.bUserInputJ = m_Data.bUserInputJ;
	RescD.iExposureClassJ[0] = m_Data.iExposureClassJ[0];
	RescD.iExposureClassJ[1] = m_Data.iExposureClassJ[1];
	RescD.dCrackWidthJ[0] = m_Data.dCrackWidthJ[0];
	RescD.dCrackWidthJ[1] = m_Data.dCrackWidthJ[1];
}

void CExposureClassTabJDlg_RC::InitUnit()
{
	m_edtTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_edtBottom.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBottom.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CExposureClassTabJDlg_RC::InitComboBox()
{
	CString strCombo[NUM_AS5100] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_A),_LS(IDS_DGN_PSC_EXPOSURE_CLASS_B1),
									_LS(IDS_DGN_PSC_EXPOSURE_CLASS_B2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_C1),
									_LS(IDS_DGN_PSC_EXPOSURE_CLASS_C2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_U) };

	for (int i = 0; i < NUM_AS5100; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}
void CExposureClassTabJDlg_RC::AlignControls()
{
	CRect rRef;
	CRect rToMove, rToMoveSTC;
	int nDistX = 0;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;
		
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

void CExposureClassTabJDlg_RC::ShowHideControls()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB)->ShowWindow(!m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB)->ShowWindow(!m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT)->ShowWindow(m_bUserInput);
}

void CExposureClassTabJDlg_RC::ControlsEnableDisable()
{
	if(!m_bBothIJ && m_nOption==0)
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
}

void CExposureClassTabJDlg_RC::Data2Dlg()
{ 
	if(m_bBothIJ)
	{
		m_cmbTop.SetCurSel(m_Data.iExposureClassI[0]);
		m_cmbBottom.SetCurSel(m_Data.iExposureClassI[1]);  

		m_bUserInput = m_Data.bUserInputI;

		m_edtTop.SetEditUnit(m_Data.dCrackWidthI[0]);
		m_edtBottom.SetEditUnit(m_Data.dCrackWidthI[1]);
	}
	else
	{
		int nMaxNum = NUM_AS5100;
		int nEx_TopNum = (m_Data.iExposureClassJ[0] > nMaxNum)? 0 : m_Data.iExposureClassJ[0];
		int nEx_BotNum = (m_Data.iExposureClassJ[1] > nMaxNum)? 0 : m_Data.iExposureClassJ[1];
		
		m_cmbTop.SetCurSel(nEx_TopNum);
		m_cmbBottom.SetCurSel(nEx_BotNum);
		m_bUserInput = FALSE;		
		
		m_edtTop.SetEditUnit(m_Data.dCrackWidthJ[0]);
		m_edtBottom.SetEditUnit(m_Data.dCrackWidthJ[1]);
	}
}

void CExposureClassTabJDlg_RC::Dlg2Data()
{
	UpdateData(TRUE);

	if(m_bBothIJ)
	{
		m_Data.iExposureClassJ[0] = m_Data.iExposureClassI[0];
		m_Data.iExposureClassJ[1] = m_Data.iExposureClassI[1];

		m_Data.bUserInputJ = m_Data.bUserInputI;
		m_Data.dCrackWidthJ[0] = m_Data.dCrackWidthI[0];
		m_Data.dCrackWidthJ[1] = m_Data.dCrackWidthI[1];
	}
	else
	{
		m_Data.iExposureClassJ[0] = m_cmbTop.GetCurSel();
		m_Data.iExposureClassJ[1] = m_cmbBottom.GetCurSel();

		m_Data.bUserInputJ = m_bUserInput;
		m_Data.dCrackWidthJ[0] = m_edtTop.GetEditValue();
		m_Data.dCrackWidthJ[1] = m_edtBottom.GetEditValue();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExposureClassTabJDlg_RC message handlers
BOOL CExposureClassTabJDlg_RC::OnInitDialog() 
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

BOOL CExposureClassTabJDlg_RC::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_Data.iExposureClassJ[0] = m_Data.iExposureClassJ[0];
	m_pParent->m_Data.iExposureClassJ[1] = m_Data.iExposureClassJ[1];

	m_pParent->m_Data.bUserInputJ = m_Data.bUserInputJ;
	m_pParent->m_Data.dCrackWidthJ[0] = m_Data.dCrackWidthJ[0];
	m_pParent->m_Data.dCrackWidthJ[1] = m_Data.dCrackWidthJ[1];

	return CChildDialog::DestroyWindow();
}

void CExposureClassTabJDlg_RC::OnUserInputChk()
{
	UpdateData(TRUE);
	
	ShowHideControls();
}