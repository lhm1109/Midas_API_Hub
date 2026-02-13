// CPGShearConnTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGShearConnTabJDlg.h"
#include "CPGShearConnDlg.h"
#include "ShearConnectorFyDlg.h"
#include "CPGShearConnTabDiaDlg.h"
#include "../wg_db/DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnTabJDlg dialog
CCPGShearConnTabJDlg::CCPGShearConnTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGShearConnTabJDlg::IDD, pParent)
{
	m_nSFCalc = 0;

	//{{AFX_DATA_INIT(CCPGShearConnTabJDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGShearConnDlg*)pParent;

	m_Data.Initialize();
}

void CCPGShearConnTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGShearConnTabJDlg)
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_CATEGORY_CMB,  m_cmbCategory);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_PTCH_EDIT,     m_edtPitch);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_PITCH_UNIT,    m_untPitch);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_HEIG_EDIT,     m_edtHeight);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_HEIGHT_UNIT,   m_untHeight);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_DIA_EDIT,      m_edtDia);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_DIA_UNIT,      m_untDia);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_FU_EDIT,       m_edtFu);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_FU_UNIT,       m_untFu);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_SPACE_EDIT,    m_edtSpace);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_SPACE_UNIT,    m_untSpace);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_NUM_EDIT,      m_edtNum);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_LENGTH_EDT,    m_edtLength);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_LENGTH_UNT,    m_untLength);
	DDX_Radio  (pDX, IDC_DGN_CPG_CPSC_SFCALC1_RDO,   m_nSFCalc);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_FCR_EDT,    m_edtFrcForce);
	DDX_Control(pDX, IDC_DGN_CPG_CPSC_F_RC_UNIT,  m_untFrcForce);
	DDX_Control(pDX,  IDC_CPSC_FOLDE_GROUP_BOX,     m_chkGroup);
	//DDX_Control(pDX,  IDC_CPSC_FRC_GROUP_BOX,     m_chkFrc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGShearConnTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGShearConnTabJDlg)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	ON_BN_CLICKED   (IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_BTN,    OnShearConnectDiaBtn)
	ON_BN_CLICKED   (IDC_DGN_CPG_SHEAR_CONN_TAB_FU_BTN,     OnShearConnectFuBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnTabJDlg message handlers

BOOL CCPGShearConnTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitCombo();
	InitUnit();

	Data2Dlg();

	m_chkGroup.InitControl(this, CCPGShearConnTabJDlg::IDD, IDC_CPSC_FOLDE_GROUP_BOX,TRUE,TRUE);
	//m_chkFrc.InitControl(this, CCPGShearConnTabJDlg::IDD, IDC_CPSC_FOLDE_GROUP_BOX,TRUE,TRUE);
	ResizeWindow();
	m_chkGroup.ShowWindow(SW_HIDE);
	//m_chkFrc.ShowWindow(SW_HIDE);

	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGShearConnTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	//m_chkFrc.SetFoldState(FALSE);
	m_chkGroup.SetFoldState(FALSE);

	return CChildDialog::DestroyWindow();
}

void CCPGShearConnTabJDlg::InitCombo()
{
	switch (m_pParent->m_nDgnCode)
	{
	case CSA_S6_14_CSG:
		{
			m_cmbCategory.ResetContent();
			m_cmbCategory.SetItemData(0, m_cmbCategory.AddString(_T("A")));
			m_cmbCategory.SetItemData(1, m_cmbCategory.AddString(_T("B")));
			m_cmbCategory.SetItemData(2, m_cmbCategory.AddString(_T("B1")));
			m_cmbCategory.SetItemData(3, m_cmbCategory.AddString(_T("C")));
			m_cmbCategory.SetItemData(4, m_cmbCategory.AddString(_T("C1")));
			m_cmbCategory.SetItemData(5, m_cmbCategory.AddString(_T("D")));
			m_cmbCategory.SetItemData(6, m_cmbCategory.AddString(_T("E")));
			m_cmbCategory.SetItemData(7, m_cmbCategory.AddString(_T("E1")));
			m_cmbCategory.SetItemData(8, m_cmbCategory.AddString(_T("M164")));
			m_cmbCategory.SetItemData(9, m_cmbCategory.AddString(_T("M253")));
			m_cmbCategory.SetCurSel(5);
			m_Data.nCategory = 5;
		}
		break;
	case CSA_S6_19_CSG:
		{
			m_cmbCategory.ResetContent();
			m_cmbCategory.SetItemData(0, m_cmbCategory.AddString(_T("A")));
			m_cmbCategory.SetItemData(1, m_cmbCategory.AddString(_T("B")));
			m_cmbCategory.SetItemData(2, m_cmbCategory.AddString(_T("B1")));
			m_cmbCategory.SetItemData(3, m_cmbCategory.AddString(_T("C")));
			m_cmbCategory.SetItemData(4, m_cmbCategory.AddString(_T("C1")));
			m_cmbCategory.SetItemData(5, m_cmbCategory.AddString(_T("D")));
			m_cmbCategory.SetItemData(6, m_cmbCategory.AddString(_T("E")));
			m_cmbCategory.SetItemData(7, m_cmbCategory.AddString(_T("E1")));
			m_cmbCategory.SetItemData(8, m_cmbCategory.AddString(_T("M164")));
			m_cmbCategory.SetItemData(9, m_cmbCategory.AddString(_T("M253")));
			m_cmbCategory.SetItemData(10, m_cmbCategory.AddString(_T("S")));
			m_cmbCategory.SetCurSel(5);
			m_Data.nCategory = 5;
		}
		break;
	default:
		{
			m_cmbCategory.ResetContent();
			m_cmbCategory.SetItemData(0, m_cmbCategory.AddString(_T("A")));
			m_cmbCategory.SetItemData(1, m_cmbCategory.AddString(_T("B")));
			m_cmbCategory.SetItemData(2, m_cmbCategory.AddString(_T("B'")));
			m_cmbCategory.SetItemData(3, m_cmbCategory.AddString(_T("C")));
			m_cmbCategory.SetItemData(4, m_cmbCategory.AddString(_T("C'")));
			m_cmbCategory.SetItemData(5, m_cmbCategory.AddString(_T("D")));
			m_cmbCategory.SetItemData(6, m_cmbCategory.AddString(_T("E")));
			m_cmbCategory.SetItemData(7, m_cmbCategory.AddString(_T("E'")));
			m_cmbCategory.SetCurSel(0);
		}
		break;
	}
}

void CCPGShearConnTabJDlg::InitUnit()
{
	m_edtPitch.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPitch.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDia.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDia.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtFu.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untFu.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtNum.SetUnitType(D_UNITSYS_NONE);
	m_edtLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untFrcForce.SetUnitType(D_UNITSYS_BASE_FORCE);
}

void CCPGShearConnTabJDlg::Data2Dlg()
{
	m_cmbCategory.SetCurSel(m_Data.nCategory);
	m_edtPitch.SetEditUnit(m_Data.dPitch);
	m_edtHeight.SetEditUnit(m_Data.dHeight);
	m_edtDia.SetEditUnit(m_Data.dDia);
	m_edtFu.SetEditUnit(m_Data.dFu);
	m_edtSpace.SetEditUnit(m_Data.dSpace);
	m_edtNum.SetEditUnit(m_Data.nNum);
	m_edtLength.SetEditUnit(m_Data.dLength);
	m_nSFCalc = m_Data.nSFCalc;
	m_edtFrcForce.SetEditUnit(m_Data.dFrc);
	
	UpdateData(FALSE);
}

BOOL CCPGShearConnTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	switch (m_pParent->m_nDgnCode)
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			m_Data.nCategory = 5; ///TODO : 로그를 보면 MQC A/B에서 요청된 사항인 듯 한데, 왜 제한했는지 모르겠네...
		}
		break;
	default:
		{
			m_Data.nCategory = m_cmbCategory.GetItemData(m_cmbCategory.GetCurSel());
		}
		break;
	}

	m_Data.dPitch = m_edtPitch.GetEditValue();
	m_Data.dHeight = m_edtHeight.GetEditValue();
	m_Data.dDia = m_edtDia.GetEditValue();	
	m_Data.dFu = m_edtFu.GetEditValue();	
	m_Data.dSpace = m_edtSpace.GetEditValue();	
	m_Data.nNum = m_edtNum.GetEditValue();	
	m_Data.dLength = m_edtLength.GetEditValue();	
	m_Data.nSFCalc = m_nSFCalc;
	m_Data.dFrc = m_edtFrcForce.GetEditValue();	

	return TRUE;
}

void CCPGShearConnTabJDlg::GetCpscData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGShearConnTabJDlg::SetCpscData()
{
	Data2Dlg();
}

void CCPGShearConnTabJDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);
	
	BOOL bEnable = TRUE;	
	if(m_pParent->m_nOption==0 && m_pParent->m_bBothIJ) bEnable = TRUE;
	else                                                bEnable = FALSE; 
		
	BOOL bAllowCode = [](int nCode)
	{
		switch (nCode)
		{
		case AASHTO_LRFD07_CSG:
		case AASHTO_LRFD12_CSG:
		case AASHTO_LRFD16_CSG:
		case AASHTO_LRFD17_CSG:
		case AASHTO_LRFD20_CSG:
		case KSSC_2014_CSG:
		case KDS_24_14_31_2018_CSG:
		case CSA_S6_14_CSG:
		case CSA_S6_19_CSG:
		case AS5100_6_17_CSG:
			return TRUE;
		default:
			return FALSE;
		}
	}(m_pParent->m_nDgnCode);
	
	GetDlgItem(IDC_DGN_CPG_CPSC_CATEGORY_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_CATEGORY_CMB)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_PTCH_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_PTCH_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_PITCH_UNIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_HEIG_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_HEIG_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_HEIGHT_UNIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_DIA_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_DIA_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_DIA_UNIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_FU_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_FU_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_FU_UNIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_SPACE_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_SPACE_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_SPACE_UNIT)->EnableWindow(bEnable);		
	GetDlgItem(IDC_DGN_CPG_CPSC_NUM_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_NUM_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_NUM_UNT)->EnableWindow(bEnable);	
	GetDlgItem(IDC_DGN_CPG_CPSC_LENGTH_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_LENGTH_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_LENGTH_UNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_SFCALC_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_SFCALC1_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_SFCALC2_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_CPSC_FRC_GROUP_BOX)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_FCR_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_FCR_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPSC_F_RC_UNIT)->EnableWindow(bEnable);

	BOOL bLenghth = FALSE;
	BOOL bScale = FALSE;
	BOOL bFcr = FALSE;

	const int nCode = m_pParent->m_nDgnCode;
	if (nCode == KDS_24_14_31_2018_CSG || nCode == KSSC_2014_CSG)
	{
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_BTN)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_FU_BTN)->EnableWindow(bEnable);

		bLenghth = TRUE;  bScale = TRUE;  bFcr = FALSE;
	}
	else if (nCode == KSCE_LSD15_CSG)
	{
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_BTN)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_FU_BTN)->EnableWindow(bEnable);

		bLenghth = FALSE;  bScale = FALSE;  bFcr = FALSE;
	}
	else if (nCode == CSA_S6_14_CSG || nCode == CSA_S6_19_CSG)
	{
		GetDlgItem(IDC_DGN_CPG_CPSC_CATEGORY_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CPG_CPSC_CATEGORY_CMB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_BTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_FU_BTN)->ShowWindow(SW_HIDE);

		bLenghth = TRUE;  bScale = FALSE;  bFcr = FALSE;
	}
	else if (nCode == AS5100_6_17_CSG)
	{
		// Category
		GetDlgItem(IDC_DGN_CPG_CPSC_CATEGORY_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CPG_CPSC_CATEGORY_CMB)->ShowWindow(SW_HIDE);

		bLenghth = FALSE;  bScale = FALSE;  bFcr = FALSE;
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_DIA_BTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_TAB_FU_BTN)->ShowWindow(SW_HIDE);

		bLenghth = TRUE;  bScale = TRUE;  bFcr = TRUE;
	}

	// Length between Max. Moment and Zero Moment
	GetDlgItem(IDC_DGN_CPG_CPSC_LENGTH_STC)->ShowWindow(bLenghth);
	GetDlgItem(IDC_DGN_CPG_CPSC_LENGTH_EDT)->ShowWindow(bLenghth);
	GetDlgItem(IDC_DGN_CPG_CPSC_LENGTH_UNT)->ShowWindow(bLenghth);

	// Norminal Shear Force Calculation
	GetDlgItem(IDC_DGN_CPG_CPSC_SFCALC_STC)->ShowWindow(bScale);
	GetDlgItem(IDC_DGN_CPG_CPSC_SFCALC1_RDO)->ShowWindow(bScale);
	GetDlgItem(IDC_DGN_CPG_CPSC_SFCALC2_RDO)->ShowWindow(bScale);

	// Net range of cross-frame force
	GetDlgItem(IDC_CPSC_FRC_GROUP_BOX)->ShowWindow(bFcr);
	GetDlgItem(IDC_DGN_CPG_CPSC_FCR_STC)->ShowWindow(bFcr);
	GetDlgItem(IDC_DGN_CPG_CPSC_FCR_EDT)->ShowWindow(bFcr);
	GetDlgItem(IDC_DGN_CPG_CPSC_F_RC_UNIT)->ShowWindow(bFcr);

	this->RedrawWindow();
}

CRect CCPGShearConnTabJDlg::GetResizeHeight()
{	
	CRect r;
	if(m_hWnd != NULL) GetWindowRect(r);
	return r;
}

void CCPGShearConnTabJDlg::ResizeDialog()
{
	UINT nLastCtrlID = IDC_DGN_CPG_CPSC_LENGTH_EDT;

	int nCode = m_pParent->m_nDgnCode;
	if (nCode == KDS_24_14_31_2018_CSG || nCode == KSSC_2014_CSG)
	{
		nLastCtrlID = IDC_DGN_CPG_CPSC_SFCALC2_RDO;
		//bLenghth = TRUE;  bScale = TRUE;  bFcr = FALSE;
	}
	else if (nCode == KSCE_LSD15_CSG)
	{
		nLastCtrlID = IDC_DGN_CPG_CPSC_NUM_EDIT;
		//bLenghth = FALSE;  bScale = FALSE;  bFcr = FALSE;
	}
	else if (nCode == CSA_S6_14_CSG || nCode == CSA_S6_19_CSG)
	{
		nLastCtrlID = IDC_DGN_CPG_CPSC_LENGTH_EDT;
		//bLenghth = TRUE;  bScale = FALSE;  bFcr = FALSE;
	}
	else if (nCode == AS5100_6_17_CSG)
	{
		nLastCtrlID = IDC_DGN_CPG_CPSC_NUM_EDIT;
		//bLenghth = FALSE;  bScale = FALSE;  bFcr = FALSE;
	}
	else
	{
		nLastCtrlID = IDC_DGN_CPG_CPSC_FCR_EDT;
	}

	CRect rectLast;
	GetDlgItem(nLastCtrlID)->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + 20;

	MoveWindow(r);

	m_pParent->ResizeWindow();
}

void CCPGShearConnTabJDlg::ResizeWindow()
{

}

LRESULT CCPGShearConnTabJDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->ResizeWindow();
	return 0L;
}


void CCPGShearConnTabJDlg::OnShearConnectDiaBtn()
{
	double dDia = 0.;

	// 대화창 호출  CPGShearConnTabDiaDlg
	CCPGShearConnTabDiaDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dDia);
		m_edtDia.SetEditUnit(dDia);
	}
}

void CCPGShearConnTabJDlg::OnShearConnectFuBtn()
{
	double dFy = 0.;

	// 대화창 호출  ShearConnectFyDlg //여기는 철근의 fy값. 이 값을 참고로 하기로 함.
	CShearConnectorFyDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dFy);
		m_edtFu.SetEditUnit(dFy);
	}
}
