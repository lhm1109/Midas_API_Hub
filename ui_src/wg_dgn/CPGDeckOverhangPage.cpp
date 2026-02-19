// CPGDeckOverhangPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGDeckOverhangPage.h"
#include "CPGDeckOverhangDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGDeckOverhangPage dialog
CCPGDeckOverhangPage::CCPGDeckOverhangPage(CWnd* pParent, int nTab)
	: CChildDialog(CCPGDeckOverhangPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGDeckOverhangPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGDeckOverhangDlg*)pParent;

	m_nTab = nTab;

	m_Data.Initialize();
}

void CCPGDeckOverhangPage::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGDeckOverhangPage)
	DDX_Control(pDX,  IDC_DGN_CPG_DOHL_DIST_EDT,    m_edtDist);
	DDX_Control(pDX,  IDC_DGN_CPG_DOHL_DIST_UNT,    m_untDist);
	DDX_Control(pDX,  IDC_DGN_CPG_DOHL_CONC_EDT,    m_edtConc);
	DDX_Control(pDX,  IDC_DGN_CPG_DOHL_CONC_UNT,    m_untConc);
	DDX_Control(pDX,  IDC_DGN_CPG_DOHL_ECCEN_EDT,   m_edtEccen);
	DDX_Control(pDX,  IDC_DGN_CPG_DOHL_ECCEN_UNT,   m_untEccen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGDeckOverhangPage, CChildDialog)
	//{{AFX_MSG_MAP(CCPGDeckOverhangPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGDeckOverhangPage message handlers
BOOL CCPGDeckOverhangPage::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

 	InitUnit();
 	ControlsEnableDisable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGDeckOverhangPage::DestroyWindow() 
{
 	Dlg2Data();
	
	if(m_nTab==0) m_pParent->m_Data_I = m_Data;
	else          m_pParent->m_Data_J = m_Data;

	return CChildDialog::DestroyWindow();
}

void CCPGDeckOverhangPage::InitUnit()
{
	m_edtDist.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untDist.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtConc.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untConc.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtEccen.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untEccen.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCPGDeckOverhangPage::Data2Dlg()
{
	m_Data.dDistForce = m_edtDist.GetEditValue();
	m_Data.dConcForce = m_edtConc.GetEditValue();
	m_Data.dEccenLoad = m_edtEccen.GetEditValue();
}

BOOL CCPGDeckOverhangPage::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dDistForce = m_edtDist.GetEditValue();
	m_Data.dConcForce = m_edtConc.GetEditValue();
	m_Data.dEccenLoad = m_edtEccen.GetEditValue();

	return TRUE;
}

void CCPGDeckOverhangPage::GetDohlData()
{
 	Dlg2Data();

	if(m_nTab==0) m_pParent->m_Data_I = m_Data;
	else          m_pParent->m_Data_J = m_Data;
}

void CCPGDeckOverhangPage::ControlsEnableDisable()
{
	UpdateData(TRUE);
	
	BOOL bEnable = FALSE;
	BOOL bAllowCode = FALSE;

	if(m_nTab==0)
	{
		if(m_pParent->m_nOption==0) bEnable = TRUE;
		else                        bEnable = FALSE; 
	}
	else
	{
		if(m_pParent->m_nOption==0 && !m_pParent->m_bBoth) bEnable = TRUE;
		else                                               bEnable = FALSE; 
	}
		
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
        bAllowCode = TRUE;
        break;
    default:
        bAllowCode = FALSE;
        break;
    }

	GetDlgItem(IDC_DGN_CPG_DOHL_DIST_STC)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_DOHL_DIST_EDT)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_DOHL_DIST_UNT)->EnableWindow(bAllowCode && bEnable);

	GetDlgItem(IDC_DGN_CPG_DOHL_CONC_STC)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_DOHL_CONC_EDT)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_DOHL_CONC_UNT)->EnableWindow(bAllowCode && bEnable);

	GetDlgItem(IDC_DGN_CPG_DOHL_ECCEN_STC)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_DOHL_ECCEN_EDT)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_CPG_DOHL_ECCEN_UNT)->EnableWindow(bAllowCode && bEnable);
}