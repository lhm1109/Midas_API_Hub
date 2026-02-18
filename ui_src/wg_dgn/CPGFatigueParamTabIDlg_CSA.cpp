// CPGFatigueParamTabIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGFatigueParamTabIDlg_CSA.h"
#include "CPGFatigueParamDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabJDlg dialog


CCPGFatigueParamTabIDlg_CSA::CCPGFatigueParamTabIDlg_CSA(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGFatigueParamTabIDlg_CSA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGFatigueParamTabIDlg_CSA)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGFatigueParamDlg*)pParent;

	m_Data.Initialize();
	m_pDoc = CDBDoc::GetDocPoint();

}

void CCPGFatigueParamTabIDlg_CSA::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGFatigueParamTabIDlg_CSA)
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_CMB, m_cmbGirderType);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_DAILY_CMB,       m_cmbDailyTruck);

	DDX_Check  (pDX, IDC_DGN_FATIGUE_AUTO_CHK,            m_bAutoChk);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_W_CSA_EDT,       m_edtWeightTruck);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_W_UNT,           m_untWeightTruck);
	DDX_Text(pDX, IDC_DGN_CPG_FATIGUE_Y_EDT,              m_dDesignLife);
	DDX_Text(pDX, IDC_DGN_FATIGUE_ND_EDT,                 m_dNdValue);
	DDX_Text(pDX, IDC_DGN_FATIGUE_P_EDT,                  m_dPValue);

	//   DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_GRP,          m_chkGirderGroup);
	//   DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_DAILY_GRP,                m_chkDailyGroup);
	//}}AFX_DATA_MAP 
}


BEGIN_MESSAGE_MAP(CCPGFatigueParamTabIDlg_CSA, CChildDialog)
	//{{AFX_MSG_MAP(CCPGFatigueParamTabIDlg_CSA)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO, OnWarpingStressTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO, OnWarpingStressTypeRdo)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DGN_FATIGUE_AUTO_CHK, &CCPGFatigueParamTabIDlg_CSA::OnBnClickedDgnFatigueAutoChk)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabIDlg_CSA message handlers

BOOL CCPGFatigueParamTabIDlg_CSA::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitCombo();
	InitUnit();
	InitChk();
	Data2Dlg();

	ControlsEnableDisable();


	ResizeWindow();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGFatigueParamTabIDlg_CSA::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	//   m_chkGirderGroup.SetFoldState(FALSE);
	//   m_chkDailyGroup.SetFoldState(FALSE);

	return CChildDialog::DestroyWindow();
}

void CCPGFatigueParamTabIDlg_CSA::InitCombo()
{

	m_cmbGirderType.ResetContent();
	m_cmbGirderType.SetItemData(0, m_cmbGirderType.AddString(_T("Simple-span")));
	m_cmbGirderType.SetItemData(1, m_cmbGirderType.AddString(_T("Continuous(Near support)")));
	m_cmbGirderType.SetItemData(2, m_cmbGirderType.AddString(_T("Continuous(Other locations)")));
	m_cmbGirderType.SetItemData(3, m_cmbGirderType.AddString(_T("Cantilever")));
	m_cmbGirderType.SetCurSel(0);

	m_cmbDailyTruck.ResetContent();
	m_cmbDailyTruck.SetItemData(0, m_cmbDailyTruck.AddString(_T("A - ADTT(4000)")));
	m_cmbDailyTruck.SetItemData(1, m_cmbDailyTruck.AddString(_T("B - ADTT(1000)")));
	m_cmbDailyTruck.SetItemData(2, m_cmbDailyTruck.AddString(_T("C - ADTT(250)")));
	m_cmbDailyTruck.SetItemData(3, m_cmbDailyTruck.AddString(_T("D - ADTT(50)")));
	m_cmbDailyTruck.SetCurSel(0);

}
void CCPGFatigueParamTabIDlg_CSA::InitUnit()
{
	m_edtWeightTruck.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untWeightTruck.SetUnitType(D_UNITSYS_BASE_FORCE);
}

void CCPGFatigueParamTabIDlg_CSA::InitChk()
{
	m_bAutoChk = TRUE;
	GetDlgItem(IDC_DGN_FATIGUE_ND_STC)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_FATIGUE_ND_EDT)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CCPGFatigueParamTabIDlg_CSA::OnWarpingStressTypeRdo() 
{
	ControlsEnableDisable();
}

void CCPGFatigueParamTabIDlg_CSA::Data2Dlg()
{
	m_cmbGirderType.SetCurSel(m_Data.nGirderType);  
	m_cmbDailyTruck.SetCurSel(m_Data.nADTT);        
	m_edtWeightTruck.SetEditUnit(m_Data.dWeight);
	m_bAutoChk = m_Data.bAutoChk;
	m_dDesignLife = m_Data.dDesignLife;
	m_dPValue     = m_Data.dp;
	m_dNdValue    = m_Data.dNd;
	UpdateData(FALSE);
}

BOOL CCPGFatigueParamTabIDlg_CSA::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.bAutoChk        = m_bAutoChk;
	m_Data.dWeight         = m_edtWeightTruck.GetEditValue();
	m_Data.nGirderType     = m_cmbGirderType.GetItemData(m_cmbGirderType.GetCurSel());   
	m_Data.nADTT           = m_cmbDailyTruck.GetItemData(m_cmbDailyTruck.GetCurSel());   
	m_Data.dDesignLife     = m_dDesignLife;
	m_Data.dp              = m_dPValue;
	m_Data.dNd             = m_dNdValue;
	return TRUE;
}

void CCPGFatigueParamTabIDlg_CSA::GetCpfpData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCPGFatigueParamTabIDlg_CSA::SetCpfpData()
{
	Data2Dlg();
}

void CCPGFatigueParamTabIDlg_CSA::ControlsEnableDisable()
{
	UpdateData(TRUE);

	BOOL bEnable = FALSE;
	BOOL bAllowCode = FALSE;

	if(m_pParent->m_nOption==0) {bEnable = TRUE; }
	else                        {bEnable = FALSE;} 

	GetDlgItem(IDC_DGN_CPG_FATIGUE_W_CSA_EDT)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_W_STC)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_Y_EDT)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_Y_STC)->ShowWindow(TRUE);


	bAllowCode = TRUE; 

	GetDlgItem(IDC_DGN_CPG_FATIGUE_CATEGORY_CSA_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_CATEGORY_CSA_CMB)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_W_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_W_CSA_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_Y_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_Y_EDT)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_DAILY_GRP)->EnableWindow(bEnable);
	
	GetDlgItem(IDC_DGN_CPG_FATIGUE_DAILY_CMB)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_W_UNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_Y_STC2)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_FATIGUE_P_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_FATIGUE_P_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_FATIGUE_AUTO_CHK)->EnableWindow(bEnable);

	if(m_bAutoChk == TRUE && m_pParent->m_nOption==0)
	{
		GetDlgItem(IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_EDT)->EnableWindow(FALSE);
	}
	else if(m_bAutoChk == FALSE && m_pParent->m_nOption==0)
	{
		GetDlgItem(IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_EDT)->EnableWindow(TRUE);
	}
	else if(m_pParent->m_nOption!=0)
	{
		GetDlgItem(IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_EDT)->EnableWindow(FALSE);
	}
	else
		ASSERT(0);

	this->RedrawWindow();
}

CRect CCPGFatigueParamTabIDlg_CSA::GetResizeHeight()
{	
	CRect r;
	if(m_hWnd != NULL) GetWindowRect(r);
	return r;
}

void CCPGFatigueParamTabIDlg_CSA::ResizeWindow()
{
	//   m_chkGirderGroup.SetFoldState(FALSE);
	//   m_chkDailyGroup.SetFoldState(FALSE);


	m_pParent->ResizeWindow();
}

LRESULT CCPGFatigueParamTabIDlg_CSA::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->ResizeWindow();
	return 0L;
}

void CCPGFatigueParamTabIDlg_CSA::OnBnClickedDgnFatigueAutoChk()
{
	UpdateData(TRUE);
	if(m_bAutoChk == FALSE)
	{
		GetDlgItem(IDC_DGN_FATIGUE_ND_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_CMB)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_FATIGUE_ND_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FATIGUE_ND_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_FATIGUE_GIRDER_TYPE_CMB)->EnableWindow(TRUE);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
