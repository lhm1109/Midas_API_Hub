// CMRebarPscMoment.cpp : implementation file
//
// 2004. 03. 09   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarPscMoment.h"

#include "CMRebarPscMoment_I.h"
#include "CMRebarPscMoment_J.h"
#include "CMSecViewWnd.h"
#include "CMRebarPscDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscMoment dialog


CCMRebarPscMoment::CCMRebarPscMoment(CCMSecViewWnd* pSectView, CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarPscMoment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarPscMoment)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pSectView);
	m_pSectView = pSectView;
	ASSERT(pParent);
	m_pParent = (CCMRebarPscDlg*)pParent;

	m_pMomentI = NULL;
	m_pMomentJ = NULL;

	m_Data.Initialize();
}


void CCMRebarPscMoment::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarPscMoment)
	DDX_Control(pDX, IDC_CMD_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_CMD_SAME_CHECK, m_chkSameIJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarPscMoment, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarPscMoment)
	ON_BN_CLICKED(IDC_CMD_SAME_CHECK, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface

void CCMRebarPscMoment::SetCurData(T_RPSC_D* pData)
{
	ASSERT(pData);
	m_Data = *pData;
	if (m_Data.bMbarJ == 0)
	{
		m_Data.Mbar[1].Copy(m_Data.Mbar[0]);
	}
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
		Data2Dlg();
}

BOOL CCMRebarPscMoment::GetCurData(T_RPSC_D* pData)
{
	ASSERT(pData);
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd)) Dlg2Data(pData);
	else
	{
		pData->bMbarJ = m_Data.bMbarJ;
		pData->Mbar[0].Copy(m_Data.Mbar[0]);
		pData->Mbar[1].Copy(m_Data.Mbar[1]);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscMoment message handlers

BOOL CCMRebarPscMoment::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->LayoutChildDlg();

	Data2Dlg();
	
	int nSel = m_ctrlTab.GetCurSel();
	m_pSectView->SetDataSourceRebar(&m_Data, nSel==1);
	m_pSectView->Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMRebarPscMoment::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Dlg2Data(&m_Data);
	return CChildDialog::DestroyWindow();
}

void CCMRebarPscMoment::OnCmdSameCheck() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data(&m_Data);
	ASSERT(m_pMomentJ);
	m_pMomentJ->SetCurData(&m_Data.Mbar[1], !m_Data.bMbarJ);
	m_pSectView->Invalidate();
}

void CCMRebarPscMoment::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	// save data
	Dlg2Data(&m_Data);
	ASSERT(m_pMomentJ);
	m_pMomentJ->SetCurData(&m_Data.Mbar[1], !m_Data.bMbarJ);
 
	// view update
	int nSel = m_ctrlTab.GetCurSel();
	m_pSectView->SetDataSourceRebar(&m_Data, nSel==1);
	m_pParent->UpdateView(nSel == 0);
	m_pSectView->Invalidate();

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMRebarPscMoment::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	// TabWinddow Create & Setting 
	m_pMomentI = new CCMRebarPscMoment_I(this);
	m_ctrlTab.AddTab(m_pMomentI, _LS(IDS_CMD_RPSC_I), CCMRebarPscMoment_I::IDD, TRUE);

	m_pMomentJ = new CCMRebarPscMoment_J(this);
	m_ctrlTab.AddTab(m_pMomentJ, _LS(IDS_CMD_RPSC_J), CCMRebarPscMoment_J::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CCMRebarPscMoment::Data2Dlg()
{
	int nCheck = m_Data.bMbarJ ? 0 : 1;
	m_chkSameIJ.SetCheck(nCheck);
	m_pMomentI->SetCurData(&m_Data.Mbar[0]);
	m_pMomentJ->SetCurData(&m_Data.Mbar[1], !m_Data.bMbarJ);
}

BOOL CCMRebarPscMoment::Dlg2Data(T_RPSC_D* pData, BOOL bSaveAlways/*=TRUE*/)
{
	pData->bMbarJ = (m_chkSameIJ.GetCheck() == 0) ? TRUE : FALSE;
	m_pMomentI->GetCurData(&pData->Mbar[0], bSaveAlways);
	m_pMomentJ->GetCurData(&pData->Mbar[1], bSaveAlways);
	if (!pData->bMbarJ)
	{
		pData->Mbar[1].Copy(pData->Mbar[0]);
	}

	return TRUE;
}

void CCMRebarPscMoment::RedrawView()
{
	ASSERT(m_pSectView);
	Dlg2Data(&m_Data, FALSE);
	m_pSectView->Invalidate();
}