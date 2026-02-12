// CMRebarPscShear.cpp : implementation file
//
// 2004. 03. 08   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarPscShear.h"

#include "CMRebarPscDlg.h"
#include "CMRebarPscShear_I.h"
#include "CMRebarPscShear_J.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscShear dialog


CCMRebarPscShear::CCMRebarPscShear(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarPscShear::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarPscShear)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pParent);
	m_pParent = (CCMRebarPscDlg*)pParent;

	m_pRebarI = NULL;
	m_pRebarJ = NULL;

	m_Data.Initialize();
}


void CCMRebarPscShear::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarPscShear)
	DDX_Control(pDX, IDC_CMD_TAB, m_wndTabCtrl);
	DDX_Control(pDX, IDC_CMD_SAME_CHECK, m_chkSameIJ);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarPscShear, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarPscShear)
	ON_BN_CLICKED(IDC_CMD_SAME_CHECK, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interface

void CCMRebarPscShear::SetCurData(T_RPSC_D* pData)
{
	ASSERT(pData);
	m_Data = *pData;
	if (m_Data.bSbarJ == 0)
	{
		m_Data.Sbar[1] = m_Data.Sbar[0];
	}
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
		Data2Dlg();
}

BOOL CCMRebarPscShear::GetCurData(T_RPSC_D* pData)
{
	ASSERT(pData);
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd)) Dlg2Data(pData);
	else
	{
		pData->bSbarJ = m_Data.bSbarJ;
		pData->Sbar[0] = m_Data.Sbar[0]; 
		pData->Sbar[1] = m_Data.Sbar[1]; 
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscShear message handlers

BOOL CCMRebarPscShear::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->LayoutChildDlg();
	
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMRebarPscShear::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Dlg2Data(&m_Data);	
	return CChildDialog::DestroyWindow();
}

void CCMRebarPscShear::OnCmdSameCheck() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data(&m_Data);
	ASSERT(m_pRebarJ);
	m_pRebarJ->SetCurData(&m_Data.Sbar[1], !m_Data.bSbarJ);	
}

void CCMRebarPscShear::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	// save data
	Dlg2Data(&m_Data);
	ASSERT(m_pRebarJ);
	m_pRebarJ->SetCurData(&m_Data.Sbar[1], !m_Data.bSbarJ);
 
	// view update
	int nSel = m_wndTabCtrl.GetCurSel();
	m_pParent->UpdateView(nSel == 0);

	*pResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMRebarPscShear::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_wndTabCtrl.SetPlaceHolder(pPlaceHolder);

	m_pRebarI = new CCMRebarPscShear_I();
	m_wndTabCtrl.AddTab(m_pRebarI, _LS(IDS_CMD_RPSC_I), CCMRebarPscShear_I::IDD, TRUE);

	m_pRebarJ = new CCMRebarPscShear_J();
	m_wndTabCtrl.AddTab(m_pRebarJ, _LS(IDS_CMD_RPSC_J), CCMRebarPscShear_J::IDD, TRUE);

	m_wndTabCtrl.ShowTab(0);
}

void CCMRebarPscShear::Data2Dlg()
{
	int nCheck = m_Data.bSbarJ ? 0 : 1;
	m_chkSameIJ.SetCheck(nCheck);
	m_pRebarI->SetCurData(&m_Data.Sbar[0]);
	m_pRebarJ->SetCurData(&m_Data.Sbar[1], !m_Data.bSbarJ); 
}

BOOL CCMRebarPscShear::Dlg2Data(T_RPSC_D* pData)
{
	pData->bSbarJ = (m_chkSameIJ.GetCheck() == 0) ? TRUE : FALSE;
	m_pRebarI->GetCurData(&pData->Sbar[0]);
	m_pRebarJ->GetCurData(&pData->Sbar[1]);
	if (!pData->bSbarJ)
	{
		pData->Sbar[1] = pData->Sbar[0];
	}
	return  TRUE;
}
