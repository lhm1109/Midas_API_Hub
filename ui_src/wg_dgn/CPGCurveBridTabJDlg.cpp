// CPGCurveBridTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGCurveBridTabJDlg.h"
#include "CPGCurveBridDlg.h"
#include "CPGCurvGuideImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGCurveBridTabJDlg dialog


CCPGCurveBridTabJDlg::CCPGCurveBridTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGCurveBridTabJDlg::IDD, pParent)
{
	m_nCurveType = 0;
	
	//{{AFX_DATA_INIT(CCPGCurveBridTabJDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGCurveBridDlg*)pParent;

	m_Data.Initialize();
}

void CCPGCurveBridTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGCurveBridTabJDlg)
	DDX_Control(pDX,  IDC_DGN_CPG_CPCB_RAD_EDIT,    m_edtRadius);
	DDX_Control(pDX,  IDC_DGN_CPG_CPCB_RAD_UNT,     m_untRadius);
	DDX_Radio  (pDX,  IDC_DGN_CPG_CPCB_CURVE_RDO1,  m_nCurveType);

	DDX_Control(pDX,  IDC_CPCB_FOLDE_GROUP_BOX,     m_chkGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGCurveBridTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGCurveBridTabJDlg)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DGN_CPG_CPCB_CURVE_GUIDE_BTN, &CCPGCurveBridTabJDlg::OnBnClickedDgnCpgCpcbCurveGuideBtn)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGCurveBridTabJDlg message handlers

BOOL CCPGCurveBridTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitUnit();
	
	Data2Dlg();
	ControlsEnableDisable();

	m_chkGroup.InitControl(this, CCPGCurveBridTabIDlg::IDD, IDC_CPCB_FOLDE_GROUP_BOX,TRUE,TRUE);
	ResizeWindow();

	m_chkGroup.ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGCurveBridTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	m_chkGroup.SetFoldState(FALSE);

	return CChildDialog::DestroyWindow();
}

void CCPGCurveBridTabJDlg::InitUnit()
{
	m_edtRadius.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untRadius.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCPGCurveBridTabJDlg::Data2Dlg()
{
	m_edtRadius.SetEditUnit(m_Data.dRadius);
	m_nCurveType = m_Data.nCurveType;
	
	UpdateData(FALSE);
}

BOOL CCPGCurveBridTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dRadius  = m_edtRadius.GetEditValue();
	m_Data.nCurveType = m_nCurveType;

	return TRUE;
}

void CCPGCurveBridTabJDlg::GetCpcbData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGCurveBridTabJDlg::SetCpcbData()
{
	Data2Dlg();
}

void CCPGCurveBridTabJDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);
	
	BOOL bEnable = TRUE;
	BOOL bAllowCode = FALSE;
	
	if(m_pParent->m_nOption==0 && m_pParent->m_bBothIJ) bEnable = TRUE;
	else                                                bEnable = FALSE; 
    
    GetDlgItem(IDC_DGN_CPG_CPCB_CURVE_GUIDE_BTN)->ShowWindow(FALSE);

    switch ( m_pParent->m_nDgnCode )
    {
    case AASHTO_LRFD07_CSG:
    case AASHTO_LRFD12_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD20_CSG:
    case STL_AASHTO_LRFD12:
    case STL_AASHTO_LRFD19:
    case KDS_24_14_31_2018_CSG:
	case KSSC_2014_CSG:
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
        bAllowCode = TRUE;
        break;
    default:
        bAllowCode = FALSE;
        break;
    }

	GetDlgItem(IDC_DGN_CPG_CPCB_RAD_STC)->EnableWindow(bEnable);	
	GetDlgItem(IDC_DGN_CPG_CPCB_RAD_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPCB_RAD_UNT)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CPG_CPCB_CURVE_GRP)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_CPCB_CURVE_RDO1)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_CPCB_CURVE_RDO2)->EnableWindow(bAllowCode && bEnable);
    
    this->RedrawWindow();
}

CRect CCPGCurveBridTabJDlg::GetResizeHeight()
{	
	CRect r;
	if(m_hWnd != NULL) GetWindowRect(r);
	return r;
}

void CCPGCurveBridTabJDlg::ResizeWindow()
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
	case KDS_24_14_31_2018_CSG:
    case KSSC_2014_CSG:
    case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
        m_chkGroup.SetFoldState(FALSE);
        break;
    default:
        m_chkGroup.SetFoldState(TRUE);
        break;
    }

	m_pParent->ResizeWindow();
}

LRESULT CCPGCurveBridTabJDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->ResizeWindow();
	return 0L;
}

void CCPGCurveBridTabJDlg::OnBnClickedDgnCpgCpcbCurveGuideBtn()
{
	CCPGCurvGuideImageDlg dlg;
	dlg.DoModal();
}
