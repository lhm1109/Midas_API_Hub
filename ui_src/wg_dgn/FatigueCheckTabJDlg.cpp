// ExposureClassTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "FatigueCheckDlg.h"
#include "FatigueCheckTabJDlg.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFatigueCheckTabJDlg dialog
CFatigueCheckTabJDlg::CFatigueCheckTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CFatigueCheckTabJDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFatigueCheckTabJDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CFatigueCheckDlg*)pParent;

	m_Data.Initialize();
}

void CFatigueCheckTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFatigueCheckTabJDlg)
	DDX_Control(pDX, IDC_DGN_DESIGN_FATI_CHK_T0_EDT,   m_edtt0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFatigueCheckTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CFatigueCheckTabJDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CFatigueCheckTabJDlg::GetData(T_FACK_D &FackD)
{
	FackD.dt0[1] = m_Data.dt0[1];
}

void CFatigueCheckTabJDlg::InitUnit()
{
	m_edtt0.SetUnitType(D_UNITSYS_NONE);
}

void CFatigueCheckTabJDlg::ControlsEnableDisable()
{
	BOOL bEnable = FALSE;
	BOOL bAllowCode = FALSE;
	
	if(m_pParent->m_nOption==0 && !m_pParent->m_bBothIJ) bEnable = TRUE;
	else                                                 bEnable = FALSE;
	
	if(m_pParent->m_nDgnCode==EUROCODE2_2_05_PSC) bAllowCode = TRUE;
	else                                          bAllowCode = FALSE;
	
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_AGE_STC)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_T0_STC )->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_T0_EDT )->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_T0_UNT )->EnableWindow(bAllowCode && bEnable);
}

void CFatigueCheckTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);
	if(m_pParent->m_bBothIJ)
	{
		m_Data.dt0[1] = m_Data.dt0[0];
	}
	else
	{
		m_Data.dt0[1] = m_edtt0.GetEditValue();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFatigueCheckTabJDlg message handlers
BOOL CFatigueCheckTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_Data = m_pParent->m_Data;
			
	InitUnit();
	UpdateData(FALSE);
	
	ControlsEnableDisable();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFatigueCheckTabJDlg::DestroyWindow() 
{
	Dlg2Data();

	return CChildDialog::DestroyWindow();
}

