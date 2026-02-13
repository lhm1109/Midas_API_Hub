// DgnStlBoltDeductionPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlBoltDeductionPage.h" 
#include "DgnStlBoltDeductionDlg.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlBoltDeductionPage dialog
CDgnStlBoltDeductionPage::CDgnStlBoltDeductionPage(CWnd* pParent, int nTab)
	: CChildDialog(CDgnStlBoltDeductionPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlBoltDeductionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CDgnStlBoltDeductionDlg*)pParent;

	m_nTab = nTab;

	m_bTotal = FALSE;

	m_Data.Initialize();
}

void CDgnStlBoltDeductionPage::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlBoltDeductionPage)
	DDX_Check  (pDX, IDC_DGN_STL_STBD_TOTAL_CHK,      m_bTotal);
	DDX_Control(pDX, IDC_DGN_STL_STBD_TOTAL_EDT     , m_edtTotal);
	DDX_Control(pDX, IDC_DGN_STL_STBD_TOP_FLANGE_EDT, m_edt_TF  );
	DDX_Control(pDX, IDC_DGN_STL_STBD_BOT_FLANGE_EDT, m_edt_BF  );
	DDX_Control(pDX, IDC_DGN_STL_STBD_LEFT_WEB_EDT  , m_edt_LW  );
	DDX_Control(pDX, IDC_DGN_STL_STBD_RIGHT_WEB_EDT , m_edt_RW  );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlBoltDeductionPage, CChildDialog)
	//{{AFX_MSG_MAP(CDgnStlBoltDeductionPage)
	ON_BN_CLICKED(IDC_DGN_STL_STBD_TOTAL_CHK, OnTotalChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnStlBoltDeductionPage message handlers
BOOL CDgnStlBoltDeductionPage::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

 	InitUnit();
 	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnStlBoltDeductionPage::OnTotalChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_STL_STBD_TOTAL_EDT     )->EnableWindow(m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_TOP_FLANGE_EDT)->EnableWindow(!m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_BOT_FLANGE_EDT)->EnableWindow(!m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_LEFT_WEB_EDT  )->EnableWindow(!m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_RIGHT_WEB_EDT )->EnableWindow(!m_bTotal);
}

BOOL CDgnStlBoltDeductionPage::DestroyWindow() 
{
 	Dlg2Data();
	
	if(m_nTab==0) m_pParent->m_Data_I = m_Data;
	else          m_pParent->m_Data_J = m_Data;

	return CChildDialog::DestroyWindow();
}

void CDgnStlBoltDeductionPage::InitUnit()
{
	m_edtTotal.SetUnitType(D_UNITSYS_NONE);
	m_edt_TF.SetUnitType(D_UNITSYS_NONE);
	m_edt_BF.SetUnitType(D_UNITSYS_NONE);
	m_edt_LW.SetUnitType(D_UNITSYS_NONE);
	m_edt_RW.SetUnitType(D_UNITSYS_NONE);
}

void CDgnStlBoltDeductionPage::Data2Dlg()
{
	m_bTotal = m_Data.bTotal;

	m_edtTotal.SetEditUnit(m_Data.dTotal    );
	m_edt_TF.SetEditUnit(m_Data.dTopFlange);
	m_edt_BF.SetEditUnit(m_Data.dBotFlange);
	m_edt_LW.SetEditUnit(m_Data.dLeftWeb  );
	m_edt_RW.SetEditUnit(m_Data.dRightWeb );
}

BOOL CDgnStlBoltDeductionPage::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bTotal = m_bTotal;

	m_Data.dTotal     = m_edtTotal.GetEditValue();
	m_Data.dTopFlange = m_edt_TF.GetEditValue();
	m_Data.dBotFlange = m_edt_BF.GetEditValue();
	m_Data.dLeftWeb   = m_edt_LW.GetEditValue();
	m_Data.dRightWeb  = m_edt_RW.GetEditValue();

	return TRUE;
}

void CDgnStlBoltDeductionPage::GetDohlData()
{
 	Dlg2Data();

	if(m_nTab==0) m_pParent->m_Data_I = m_Data;
	else          m_pParent->m_Data_J = m_Data;
}

void CDgnStlBoltDeductionPage::ControlsEnableDisable()
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
		
	if (m_pParent->m_strDgnCode == STLCODE_JROAD_H24 || m_pParent->m_strDgnCode == STLCODE_JROAD_H14 ||
		m_pParent->m_strDgnCode == STLCODE_JROAD_H29 || m_pParent->m_strDgnCode == STLCODE_JROAD_R07)
	{
		bAllowCode = TRUE;
	}

	bAllowCode = TRUE;

	GetDlgItem(IDC_DGN_STL_STBD_TOTAL_CHK     )->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_STL_STBD_TOTAL_EDT     )->EnableWindow(bAllowCode && bEnable && m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_TOP_FLANGE_STC)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_STL_STBD_TOP_FLANGE_EDT)->EnableWindow(bAllowCode && bEnable && !m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_BOT_FLANGE_STC)->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_STL_STBD_BOT_FLANGE_EDT)->EnableWindow(bAllowCode && bEnable && !m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_LEFT_WEB_STC  )->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_STL_STBD_LEFT_WEB_EDT  )->EnableWindow(bAllowCode && bEnable && !m_bTotal);
	GetDlgItem(IDC_DGN_STL_STBD_RIGHT_WEB_STC )->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_STL_STBD_RIGHT_WEB_EDT )->EnableWindow(bAllowCode && bEnable && !m_bTotal);
		
}