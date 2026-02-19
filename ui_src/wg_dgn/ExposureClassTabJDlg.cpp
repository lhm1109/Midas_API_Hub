// ExposureClassTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ExposureClassTabJDlg.h"
#include "ExposureClassDlg.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExposureClassTabJDlg dialog
CExposureClassTabJDlg::CExposureClassTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CExposureClassTabJDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExposureClassTabJDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CExposureClassDlg*)pParent;

	m_Data.Initialize();

	m_bBothIJ = FALSE;
	m_nOption = 0;
	m_bUserInput = FALSE;
}

void CExposureClassTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExposureClassTabJDlg)
	DDX_Check  (pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK,     m_bUserInput);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB,      m_cmbTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB,   m_cmbBottom);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT,      m_edtTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT,      m_untTop);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT,   m_edtBottom);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT,   m_untBottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExposureClassTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CExposureClassTabJDlg)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK, OnUserInputChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExposureClassTabJDlg::SetData(BOOL bBothIJ, int nOption)
{
	m_bBothIJ = bBothIJ;
	m_nOption = nOption;
}

void CExposureClassTabJDlg::GetData(T_PESC_D &PescD)
{
	PescD.bUserInputJ = m_Data.bUserInputJ;
	PescD.iExposureClassJ[0] = m_Data.iExposureClassJ[0];
	PescD.iExposureClassJ[1] = m_Data.iExposureClassJ[1];
	PescD.dCrackWidthJ[0] = m_Data.dCrackWidthJ[0];
	PescD.dCrackWidthJ[1] = m_Data.dCrackWidthJ[1];
}

void CExposureClassTabJDlg::InitUnit()
{
	m_edtTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_edtBottom.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBottom.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CExposureClassTabJDlg::InitComboBox()
{
	CString strCombo[NUM_PSC_EC2] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_X0),  _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XC1), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XC2),
									 _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XC3), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XC4), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XD1),
									 _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XD2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XD3), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XS1),
									 _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XS2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XS3), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XF1),
									 _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XF2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XF3), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XF4),
									 _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XA1), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XA2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_XA3) };

	for (int i = 0; i < NUM_PSC_EC2; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::InitComboBox_LSD12()
{
	CString strCombo[NUM_PSC_LSD12] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_EC1), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_EC2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_EC3),
									   _LS(IDS_DGN_PSC_EXPOSURE_CLASS_EC4), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_ED1), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_ED2),
									   _LS(IDS_DGN_PSC_EXPOSURE_CLASS_ED3), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_ES1), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_ES2),
									   _LS(IDS_DGN_PSC_EXPOSURE_CLASS_ES3) };

	for (int i = 0; i < NUM_PSC_LSD12; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::InitComboBox_RUS()
{
	CString strCombo[NUM_PSC_RUS] = { _LS(IDS_DGN_PSC_CRACK_CATEGORY_2A), _LS(IDS_DGN_PSC_CRACK_CATEGORY_2B), _LS(IDS_DGN_PSC_CRACK_CATEGORY_3A),
									 _LS(IDS_DGN_PSC_CRACK_CATEGORY_3B), _LS(IDS_DGN_PSC_CRACK_CATEGORY_3C) };

	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for (int i = 0; i < NUM_PSC_RUS; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::InitComboBox_AS5100()
{
	CString strCombo[NUM_PSC_AS5100] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_A),_LS(IDS_DGN_PSC_EXPOSURE_CLASS_B1),
										_LS(IDS_DGN_PSC_EXPOSURE_CLASS_B2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_C1),
										_LS(IDS_DGN_PSC_EXPOSURE_CLASS_C2), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_U) };

	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for (int i = 0; i < NUM_PSC_AS5100; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove, rToMoveSTC;
	int nDistX = 0;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;
	
	if(m_pParent->m_iDgnCode==SNiP_20503_84_PSC || m_pParent->m_iDgnCode==SP_35_13330_11_PSC || m_pParent->m_iDgnCode==SNiP_20503_84_PSC_MKS || m_pParent->m_iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT);
		
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK)->GetWindowRect(rToMove);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT)->GetWindowRect(rToMoveSTC);
		
		nDistX = rRef.left - rToMove.left + globalUtils.ScaleByDPI(10);
		nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(20);
		
		CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
		
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_STC);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_STC);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB);
		
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_STC)->GetWindowRect(rToMove);
		
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		
		CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

		aControls.RemoveAll();
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT);
		aControls.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT);
		nDistX = rToMoveSTC.Width();
		CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, 0);
	}
	else
	{
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
}

void CExposureClassTabJDlg::ShowHideControls()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_CMB)->ShowWindow(!m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_CMB)->ShowWindow(!m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_EDT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_TOP_UNT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_EDT)->ShowWindow(m_bUserInput);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_BOTTOM_UNT)->ShowWindow(m_bUserInput);
}

void CExposureClassTabJDlg::ControlsEnableDisable()
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

	if(m_pParent->m_iDgnCode!=SNiP_20503_84_PSC && m_pParent->m_iDgnCode!=SP_35_13330_11_PSC && m_pParent->m_iDgnCode!=SNiP_20503_84_PSC_MKS && m_pParent->m_iDgnCode!=SP_35_13330_11_PSC_MKS )
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK)->EnableWindow(FALSE);
	}
}

void CExposureClassTabJDlg::Data2Dlg()
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
		int nMaxNum =0;
        if ( CDBLib::IsPscCodeLSD_KR(m_pParent->m_iDgnCode) )  nMaxNum = NUM_PSC_LSD12-1;
		else if(m_pParent->m_iDgnCode==SNiP_20503_84_PSC)      nMaxNum = NUM_PSC_RUS-1;
		else if(m_pParent->m_iDgnCode==SP_35_13330_11_PSC)     nMaxNum = NUM_PSC_RUS-1;
		else if(m_pParent->m_iDgnCode==SNiP_20503_84_PSC_MKS)  nMaxNum = NUM_PSC_RUS-1;
		else if(m_pParent->m_iDgnCode==SP_35_13330_11_PSC_MKS) nMaxNum = NUM_PSC_RUS-1;
		else if(m_pParent->m_iDgnCode==AS_5100_5_17_PSC)       nMaxNum = NUM_PSC_AS5100-1;
		else if(m_pParent->m_iDgnCode==CS_454_20_PSC_RATING )  nMaxNum = NUM_R_PSC_CS454 - 1;
		else                                                   nMaxNum = NUM_PSC_EC2-1;
		
		int nEx_TopNum = (m_Data.iExposureClassJ[0] > nMaxNum)? 0 : m_Data.iExposureClassJ[0];
		int nEx_BotNum = (m_Data.iExposureClassJ[1] > nMaxNum)? 0 : m_Data.iExposureClassJ[1];
		
		m_cmbTop.SetCurSel(nEx_TopNum);
		m_cmbBottom.SetCurSel(nEx_BotNum);

		if(m_pParent->m_iDgnCode==SNiP_20503_84_PSC || m_pParent->m_iDgnCode==SP_35_13330_11_PSC || m_pParent->m_iDgnCode==SNiP_20503_84_PSC_MKS || m_pParent->m_iDgnCode==SP_35_13330_11_PSC_MKS)
		{
			m_bUserInput = m_Data.bUserInputJ;
		}
		else 
		{
			m_bUserInput = FALSE;
		}
		
		m_edtTop.SetEditUnit(m_Data.dCrackWidthJ[0]);
		m_edtBottom.SetEditUnit(m_Data.dCrackWidthJ[1]);
	}
}

void CExposureClassTabJDlg::Dlg2Data()
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
// CExposureClassTabJDlg message handlers
BOOL CExposureClassTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_Data = m_pParent->m_Data;
	m_bBothIJ = m_pParent->m_bBothIJ;
	m_nOption = m_pParent->m_nOption;

	InitUnit();
		
    if ( CDBLib::IsPscCodeLSD_KR(m_pParent->m_iDgnCode) )
	{
		InitComboBox_LSD12();
	}
	else if (m_pParent->m_iDgnCode == SNiP_20503_84_PSC || m_pParent->m_iDgnCode == SP_35_13330_11_PSC || m_pParent->m_iDgnCode == SNiP_20503_84_PSC_MKS || m_pParent->m_iDgnCode == SP_35_13330_11_PSC_MKS)
	{
		InitComboBox_RUS();
	}
	else if (m_pParent->m_iDgnCode == IRC_112_2011_PSC || m_pParent->m_iDgnCode == IRC_112_2020_PSC)
	{
		InitComboBox_IRC112();
	}
	else if (m_pParent->m_iDgnCode == AS_5100_5_17_PSC)
	{
		InitComboBox_AS5100();
	}
	else if (m_pParent->m_iDgnCode == IRS_PSC)
	{
		InitComboBox_IRS();
	}
	else if (m_pParent->m_iDgnCode == BS5400_90_PSC || m_pParent->m_iDgnCode == TMH07_3_1989 || m_pParent->m_iDgnCode == BD_44_15_PSC)
	{
		InitComboBox_BS();
	}
	else if (m_pParent->m_iDgnCode == CS_454_20_PSC_RATING )
	{
		InitComboBox_CS454();
	}
	else if (m_pParent->m_iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		InitComboBox_ABNT();
	}
	else
	{
		InitComboBox();
	}

	AlignControls();
	ControlsEnableDisable();

	Data2Dlg();

	UpdateData(FALSE);

	OnUserInputChk();

	if(m_pParent->m_iDgnCode==SNiP_20503_84_PSC || m_pParent->m_iDgnCode==SP_35_13330_11_PSC || m_pParent->m_iDgnCode==SNiP_20503_84_PSC_MKS || m_pParent->m_iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP)->SetWindowText(_LS(IDS_DGN_PSC_CRACK_WIDTH_TITLE_NAME));
	}
	else if (m_pParent->m_iDgnCode == BS5400_90_PSC || m_pParent->m_iDgnCode == TMH07_3_1989 || m_pParent->m_iDgnCode == CS_454_20_PSC_RATING )
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP)->SetWindowText(_LS(IDS_DGN_ENVIRONMENT_TYPE));
	}
	else if (m_pParent->m_iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP)->SetWindowText(_LS(IDS_DGN_ENVIRONMENTAL_AGGRESSIVENESS_CLASS));
	}
	else
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_GRP)->SetWindowText(_LS(IDS_DGN_EXPOSURE_CLASS));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CExposureClassTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_Data.iExposureClassJ[0] = m_Data.iExposureClassJ[0];
	m_pParent->m_Data.iExposureClassJ[1] = m_Data.iExposureClassJ[1];

	m_pParent->m_Data.bUserInputJ = m_Data.bUserInputJ;
	m_pParent->m_Data.dCrackWidthJ[0] = m_Data.dCrackWidthJ[0];
	m_pParent->m_Data.dCrackWidthJ[1] = m_Data.dCrackWidthJ[1];

	return CChildDialog::DestroyWindow();
}

void CExposureClassTabJDlg::OnUserInputChk()
{
	UpdateData(TRUE);
	
	ShowHideControls();
}

void CExposureClassTabJDlg::InitComboBox_IRC112()
{
	CString strCombo[NUM_PSC_IRC112] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Moderate), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Severe), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Very_Severe),
		_LS(IDS_DGN_PSC_EXPOSURE_CLASS_Extreme) };

	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for (int i = 0; i < NUM_PSC_IRC112; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::InitComboBox_IRS()
{
	CString strCombo[NUM_PSC_IRS] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Moderate), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Severe),
		_LS(IDS_DGN_PSC_EXPOSURE_CLASS_Extreme) };

	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for (int i = 0; i < NUM_PSC_IRS; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::InitComboBox_BS()
{
	CString strCombo[NUM_PSC_BS] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Moderate),
								   _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Severe),
								   _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Very_Severe),
								   _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Extreme) };

	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for (int i = 0; i < NUM_PSC_BS; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::InitComboBox_CS454()
{
	CString strCombo[NUM_R_PSC_CS454] = { _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Extreme),
										  _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Very_Severe),
										  _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Severe),
										  _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Moderate) };

	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for (int i = 0; i < NUM_R_PSC_CS454; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}

void CExposureClassTabJDlg::InitComboBox_ABNT()
{
	CString strCombo[NUM_R_PSC_ABNT] = { _LS(IDS_DGN_PSC_ABNT_EXPOSURE_I),
											_LS(IDS_DGN_PSC_ABNT_EXPOSURE_II),
											_LS(IDS_DGN_PSC_ABNT_EXPOSURE_III),
												_LS(IDS_DGN_PSC_ABNT_EXPOSURE_IV) };

	m_cmbTop.ResetContent();
	m_cmbBottom.ResetContent();

	for (int i = 0; i < NUM_R_PSC_ABNT; i++)
	{
		m_cmbTop.AddString(strCombo[i]);
		m_cmbBottom.AddString(strCombo[i]);
	}
}