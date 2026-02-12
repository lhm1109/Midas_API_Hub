// ETC_DESIGN_SPECT_TS.cpp : implementation file
//
///////
#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_DESIGN_SPECT_TS.h"

#include "ETC_GDS_KS_BRG.h"
#include "ETC_GDS_KSCE2019.h"
#include "ETC_GDS_KDS2019.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CETC_DESIGN_SPECT_TS dialog

CETC_DESIGN_SPECT_TS::CETC_DESIGN_SPECT_TS(CWnd* pParent /*=NULL*/ )
	: CDialogMove(CETC_DESIGN_SPECT_TS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_DESIGN_SPECT_TS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bFirstActive = TRUE;
	m_bInitActive = TRUE;
}

void CETC_DESIGN_SPECT_TS::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_DESIGN_SPECT_TS)
	DDX_Control(pDX, IDC_ETC_DESIGN_SPECTRUM_TS_CODE_CMB, m_cmbCode);
	//}}AFX_DATA_MAP
}

void CETC_DESIGN_SPECT_TS::InitCombo()
{
	m_cmbCode.ResetContent();
	m_cmbCode.SetItemData(m_cmbCode.AddString(_T("KDS(41-17-00:2019)")), D_SPFC_CODE_KDS2019);
	m_cmbCode.SetItemData(m_cmbCode.AddString(_T("KDS(17-10-00:2018)")), D_SPFC_CODE_KSCE2019);
	m_cmbCode.SetItemData(m_cmbCode.AddString(_T("Korea(Bridge)")),      D_SPFC_CODE_KS_BRG);
	m_cmbCode.SetCurSel(1);
}

void CETC_DESIGN_SPECT_TS::CreatePage()
{
	T_SPFC_D SpfcD_KS_BRG; SpfcD_KS_BRG.Initialize(); 
	SpfcD_KS_BRG.nSpecCodeType = D_SPFC_CODE_KS_BRG;
	m_pKSBRG	= new	CETC_GDS_KS_BRG(&SpfcD_KS_BRG, TRUE);
	ASSERT(m_pKSBRG);	
	if(!m_pKSBRG) return;

	T_SPFC_D SpfcD_KSCE2019; SpfcD_KSCE2019.Initialize(); 
	SpfcD_KSCE2019.nSpecCodeType = D_SPFC_CODE_KSCE2019;
	m_pKSCE2019	= new	CETC_GDS_KSCE2019(&SpfcD_KSCE2019, TRUE);
	ASSERT(m_pKSCE2019);	
	if(!m_pKSCE2019) return;

	T_SPFC_D SpfcD_KDS2019; SpfcD_KDS2019.Initialize();  
	SpfcD_KDS2019.nSpecCodeType = D_SPFC_CODE_KDS2019;
	m_pKDS2019	= new	CETC_GDS_KDS2019(&SpfcD_KDS2019, TRUE);
	ASSERT(m_pKDS2019);	
	if(!m_pKDS2019) return;
	
	// holder
	CRect rc;
	CWnd *cwnd = (CWnd *)GetDlgItem(IDC_ETC_DESIGN_SPECTRUM_TS_PLACEHOLDER);	
	if(!cwnd)	return;
	cwnd->GetWindowRect(&rc);		// holder 위치.. 
	this->ScreenToClient(&rc);	// 현재 대화상자 기준좌표. 

	// create dlg
	// Sunken 옵션이 보여지기 위해서 한칸씩 더해준다...왜그래야 할까...
	CRect rDlg;
	if(m_pKSBRG->GetSafeHwnd()==0)
	{
		m_pKSBRG->Create(IDD_ETC_GDS_KS_BRIDGE,this);
		m_pKSBRG->GetWindowRect(rDlg);
		m_pKSBRG->SetWindowPos(NULL,rc.left+1,rc.top+1,rc.Width()-2,rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pKSCE2019->GetSafeHwnd()==0)
	{
		m_pKSCE2019->Create(IDD_ETC_GDS_KSCE2019,this);
		m_pKSCE2019->GetWindowRect(rDlg);
		m_pKSCE2019->SetWindowPos(NULL,rc.left+1,rc.top+1,rc.Width()-2,rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pKDS2019->GetSafeHwnd()==0)
	{
		m_pKDS2019->Create(IDD_ETC_GDS_KDS2019,this);
		m_pKDS2019->GetWindowRect(rDlg);
		m_pKDS2019->SetWindowPos(NULL,rc.left+1,rc.top+1,rc.Width()-2,rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

void CETC_DESIGN_SPECT_TS::ShowPage()
{
	UpdateData(TRUE);

	int nCode = m_cmbCode.GetItemData(m_cmbCode.GetCurSel());
	if(m_pKSBRG) m_pKSBRG->ShowWindow(nCode == D_SPFC_CODE_KS_BRG);
	if(m_pKSCE2019) m_pKSCE2019->ShowWindow(nCode == D_SPFC_CODE_KSCE2019);
	if(m_pKDS2019) m_pKDS2019->ShowWindow(nCode == D_SPFC_CODE_KDS2019);
}


BEGIN_MESSAGE_MAP(CETC_DESIGN_SPECT_TS, CDialogMove)
	//{{AFX_MSG_MAP(CETC_DESIGN_SPECT_TS)
	ON_CBN_SELCHANGE(IDC_ETC_DESIGN_SPECTRUM_TS_CODE_CMB, OnDgnSpectTsCodeCmb)
	ON_BN_CLICKED(IDC_ETC_DESIGN_SPECTRUM_TS_OK_BTN,      OnDgnSpectTsOKBtn)
	ON_BN_CLICKED(IDC_ETC_DESIGN_SPECTRUM_TS_CANCEL_BTN,  OnDgnSpectTsCancelBtn)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_DESIGN_SPECT_TS message handlers

BOOL CETC_DESIGN_SPECT_TS::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCombo();
	CreatePage();
	ShowPage();
	
	m_bInitActive = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_DESIGN_SPECT_TS::OnDgnSpectTsCodeCmb()
{
	ShowPage();
}

void CETC_DESIGN_SPECT_TS::OnDgnSpectTsOKBtn()
{
	int nCode = m_cmbCode.GetItemData(m_cmbCode.GetCurSel());

	if(nCode == D_SPFC_CODE_KS_BRG)
	{
		if(m_pKSBRG) 
		{
			m_pKSBRG->End();
			m_dTs = m_pKSBRG->GetTs();
		}			
	}	
	else if(nCode == D_SPFC_CODE_KSCE2019)
	{
		if(m_pKSCE2019) m_dTs = m_pKSCE2019->GetTs();
	}	
	else if(nCode == D_SPFC_CODE_KDS2019)
	{
		if(m_pKDS2019) m_dTs = m_pKDS2019->GetTs();
	}	

	CDialogMove::OnOK();
}

void CETC_DESIGN_SPECT_TS::OnDgnSpectTsCancelBtn()
{
	CDialogMove::OnCancel();
}

void CETC_DESIGN_SPECT_TS::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogMove::OnActivate(nState, pWndOther, bMinimized);
	if(m_bInitActive) return;

	if (nState == WA_ACTIVE)
	{
		if (m_bFirstActive)
		{
			int nCode = m_cmbCode.GetItemData(m_cmbCode.GetCurSel());
			switch (nCode)
			{
			case D_SPFC_CODE_KS_BRG:
				if (m_pKSBRG)
				{
					m_pKSBRG->Invalidate();
					m_pKSBRG->UpdateWindow();
				}
				break;
			case D_SPFC_CODE_KSCE2019:
				if (m_pKSCE2019)
				{
					m_pKSCE2019->Invalidate();
					m_pKSCE2019->UpdateWindow();
				}
				break;
			case D_SPFC_CODE_KDS2019:
				if (m_pKDS2019)
				{
					m_pKDS2019->Invalidate();
					m_pKDS2019->UpdateWindow();
				}
				break;
			}

			m_bFirstActive = FALSE;
		}
	}
	else if (nState == WA_INACTIVE)
	{
		if(!m_bFirstActive)	m_bFirstActive = TRUE;
	}
}
