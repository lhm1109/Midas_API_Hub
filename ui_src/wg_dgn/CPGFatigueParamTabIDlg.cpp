// CPGFatigueParamTabIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGFatigueParamTabIDlg.h"
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


CCPGFatigueParamTabIDlg::CCPGFatigueParamTabIDlg(CWnd* pParent /*=NULL*/, int* pElemType/*=NULL*/)
	: CChildDialog(CCPGFatigueParamTabIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGFatigueParamTabIDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pElemType = pElemType;
	m_nWarpType = 0;

	ASSERT(pParent);
	m_pParent = (CCPGFatigueParamDlg*)pParent;

	m_Data.Initialize();
	m_pDoc = CDBDoc::GetDocPoint();

	arWarpingCtrlID.RemoveAll();
	arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO);
	arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO);
	arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_TOP_EDT);
	arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_TOP_UNT);
	arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_BOT_EDT);
	arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_BOT_UNT);
}

void CCPGFatigueParamTabIDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGFatigueParamTabIDlg)
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_CATEGORY_CMB,         m_cmbCategory);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_ADTT_EDIT,            m_dADTT);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_NUM_EDIT,             m_dNum);
	DDX_Radio  (pDX, IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO,     m_nWarpType);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_TOP_EDT,              m_edtTopFlange);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_TOP_UNT,              m_untTopFlange);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_BOT_EDT,              m_edtBotFlange);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_BOT_UNT,              m_untBotFlange);

	DDX_Control(pDX,  IDC_CPFP_FOLDE_GROUP_BOX,                m_chkGroup);
	//}}AFX_DATA_MAP 
}


BEGIN_MESSAGE_MAP(CCPGFatigueParamTabIDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGFatigueParamTabIDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO, OnWarpingStressTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO, OnWarpingStressTypeRdo)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabIDlg message handlers

BOOL CCPGFatigueParamTabIDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	InitCombo();
	InitUnit();

	Data2Dlg();

	ControlsEnableDisable();

	m_chkGroup.InitControl(this, CCPGFatigueParamTabIDlg::IDD, IDC_CPFP_FOLDE_GROUP_BOX,TRUE,TRUE);
	ResizeWindow();
	
	m_chkGroup.ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGFatigueParamTabIDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	m_chkGroup.SetFoldState(FALSE);

	return CChildDialog::DestroyWindow();
}

void CCPGFatigueParamTabIDlg::InitCombo()
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

void CCPGFatigueParamTabIDlg::InitUnit()
{
	m_edtTopFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untTopFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtBotFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untBotFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dADTT.SetUnitType(D_UNITSYS_NONE);
	m_dNum.SetUnitType(D_UNITSYS_NONE);
}

void CCPGFatigueParamTabIDlg::OnWarpingStressTypeRdo() 
{
	ControlsEnableDisable();
}

void CCPGFatigueParamTabIDlg::Data2Dlg()
{
	m_cmbCategory.SetCurSel(m_Data.nDetailCategory);
	m_dADTT.SetEditUnit(m_Data.dADTT);
	m_dNum.SetEditUnit(m_Data.dNum);
	m_nWarpType = m_Data.nWarpType;
	m_edtTopFlange.SetEditUnit(m_Data.dTopFlange);
	m_edtBotFlange.SetEditUnit(m_Data.dBotFlange);	
	
	UpdateData(FALSE);
}

BOOL CCPGFatigueParamTabIDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dADTT = m_dADTT.GetEditValue();
	m_Data.dNum  = m_dNum.GetEditValue();	
	m_Data.nDetailCategory = m_cmbCategory.GetItemData(m_cmbCategory.GetCurSel());
	m_Data.nWarpType = m_nWarpType;
	m_Data.dTopFlange = m_edtTopFlange.GetEditValue();
	m_Data.dBotFlange = m_edtBotFlange.GetEditValue();

	return TRUE;
}

void CCPGFatigueParamTabIDlg::GetCpfpData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCPGFatigueParamTabIDlg::SetCpfpData()
{
	Data2Dlg();
}

void CCPGFatigueParamTabIDlg::ControlsEnableDisable()
{
	if (m_pElemType)
	{
		m_nWarpType = *m_pElemType;
		UpdateData(FALSE);
	}

	UpdateData(TRUE);

	BOOL bEnable = FALSE;
	BOOL bAllowCode = FALSE;

	if(m_pParent->m_nOption==0) {bEnable = TRUE; }
	else                        {bEnable = FALSE;} 
	
	CRect rStcNum, rEdtNum, rStcADDT, rEdtADDT;
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_EDIT)->GetWindowRect(rEdtNum);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_STC)->GetWindowRect(rStcNum);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_EDIT)->GetWindowRect(rEdtADDT);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_STC)->GetWindowRect(rStcADDT);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_STC)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_EDIT)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_STC)->SetWindowText(_T("n(cycles)")); 
	
	CDlgUtil::CtrlEnableDisable(this, arWarpingCtrlID, TRUE);

 int nDelY = rEdtADDT.top - rEdtNum.top;

	 if(nDelY == 0 &&m_pParent->m_nDgnCode != KSSC_2014_CSG)
	 {
		 CArray<UINT, UINT> aCtrlID;
		 aCtrlID.SetSize(11);  
		 aCtrlID.SetAt(0,IDC_DGN_CPG_FATIGUE_NUM_STC);
		 aCtrlID.SetAt(1,IDC_DGN_CPG_FATIGUE_NUM_EDIT);
		 aCtrlID.SetAt(2,IDC_DGN_CPG_FATIGUE_WARPING_GRP);     
		 aCtrlID.SetAt(3,IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO);
		 aCtrlID.SetAt(4,IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO);
		 aCtrlID.SetAt(5, IDC_DGN_CPG_FATIGUE_TOP_STC);     
		 aCtrlID.SetAt(6, IDC_DGN_CPG_FATIGUE_TOP_EDT);
		 aCtrlID.SetAt(7, IDC_DGN_CPG_FATIGUE_TOP_UNT);
		 aCtrlID.SetAt(8, IDC_DGN_CPG_FATIGUE_BOT_STC);     
		 aCtrlID.SetAt(9, IDC_DGN_CPG_FATIGUE_BOT_EDT);
		 aCtrlID.SetAt(10, IDC_DGN_CPG_FATIGUE_BOT_UNT);
		 CDlgUtil::CtrlMoveDistY(this, aCtrlID,rEdtADDT.Height());
	 }

	if (m_pParent->m_nDgnCode == AASHTO_LRFD07_CSG || m_pParent->m_nDgnCode == AASHTO_LRFD12_CSG ||
		m_pParent->m_nDgnCode == AASHTO_LRFD16_CSG || m_pParent->m_nDgnCode == AASHTO_LRFD17_CSG || m_pParent->m_nDgnCode == AASHTO_LRFD20_CSG ||
        m_pParent->m_nDgnCode == STL_AASHTO_LRFD12 || m_pParent->m_nDgnCode == STL_AASHTO_LRFD19 )
	{
		bAllowCode = TRUE; 
	}
	else if(m_pParent->m_nDgnCode == KSSC_2014_CSG)
	{ 
		bAllowCode = TRUE;    
		GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_STC)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_EDIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_STC)->SetWindowText(_T("N"));

		if(nDelY != 0 ) // Move up controls in case of KSSC2014_CSG
		{
			CArray<UINT, UINT> aCtrlID;
			aCtrlID.SetSize(11);  
			aCtrlID.SetAt(0,IDC_DGN_CPG_FATIGUE_NUM_STC);
			aCtrlID.SetAt(1,IDC_DGN_CPG_FATIGUE_NUM_EDIT);
			aCtrlID.SetAt(2,IDC_DGN_CPG_FATIGUE_WARPING_GRP);     
			aCtrlID.SetAt(3,IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO);
			aCtrlID.SetAt(4,IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO);
			aCtrlID.SetAt(5, IDC_DGN_CPG_FATIGUE_TOP_STC);     
			aCtrlID.SetAt(6, IDC_DGN_CPG_FATIGUE_TOP_EDT);
			aCtrlID.SetAt(7, IDC_DGN_CPG_FATIGUE_TOP_UNT);
			aCtrlID.SetAt(8, IDC_DGN_CPG_FATIGUE_BOT_STC);     
			aCtrlID.SetAt(9, IDC_DGN_CPG_FATIGUE_BOT_EDT);
			aCtrlID.SetAt(10, IDC_DGN_CPG_FATIGUE_BOT_UNT);
			CDlgUtil::CtrlMoveDistY(this, aCtrlID,nDelY);
		}

	}
	else                                                {bAllowCode = FALSE; }

	GetDlgItem(IDC_DGN_CPG_FATIGUE_CATEGORY_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_CATEGORY_CMB)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_EDIT)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CPG_FATIGUE_WARPING_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO)->EnableWindow(bEnable);

	if (m_pElemType)
	{
		GetDlgItem(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO)->EnableWindow(m_nWarpType!=1);
	}

	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_STC)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_EDT)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_UNT)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_STC)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_EDT)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_UNT)->EnableWindow(bEnable && m_nWarpType==1);

	if(m_pParent->m_nDgnCode == KSSC_2014_CSG)
	{
		T_CPGD_D CpgdD; CpgdD.Initialize();
		m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);
		if(!CpgdD.bDistortion)    CDlgUtil::CtrlEnableDisable(this, arWarpingCtrlID, CpgdD.bDistortion);
	}

	this->RedrawWindow();
}

CRect CCPGFatigueParamTabIDlg::GetResizeHeight()
{	
	CRect r;
	if(m_hWnd != NULL) GetWindowRect(r);
	return r;
}

void CCPGFatigueParamTabIDlg::ResizeWindow()
{
    switch ( m_pParent->m_nDgnCode )
    {
    case AASHTO_LRFD07_CSG:
    case AASHTO_LRFD12_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD20_CSG:
    case STL_AASHTO_LRFD12:
    case STL_AASHTO_LRFD19:
    case KSSC_2014_CSG:
        m_chkGroup.SetFoldState(FALSE);
        break;
    default:
        m_chkGroup.SetFoldState(TRUE);
        break;
    }
	
	m_pParent->ResizeWindow();
}

LRESULT CCPGFatigueParamTabIDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->ResizeWindow();
	return 0L;
}