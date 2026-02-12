// CMRebarCompositeMoment.cpp : implementation file
//
// 2004. 03. 09   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarCompositeMoment.h"

#include "CMRebarCompositeMoment_I.h"
#include "CMRebarCompositeMoment_J.h"
#include "CMSecViewWnd.h"
#include "CMRebarCompositeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeMoment dialog


CCMRebarCompositeMoment::CCMRebarCompositeMoment(CCMSecViewWnd* pSectView, CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarCompositeMoment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarCompositeMoment)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pSectView);
	m_pSectView = pSectView;
	ASSERT(pParent);
	m_pParent = (CCMRebarCompositeDlg*)pParent;

	m_pMomentI = NULL;
	m_pMomentJ = NULL;

	m_bPart1Enabled = FALSE;
	m_Data.Initialize();
}


void CCMRebarCompositeMoment::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarCompositeMoment)
	DDX_Control(pDX, IDC_CMD_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_CMD_SAME_CHECK, m_chkSameIJ);
	DDX_Control(pDX, IDC_CMD_CRACK_CHK, m_chkCrack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarCompositeMoment, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarCompositeMoment)
	ON_BN_CLICKED(IDC_CMD_SAME_CHECK, OnCmdSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface

void CCMRebarCompositeMoment::SetCurData(T_RPSC_D* pData)
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

BOOL CCMRebarCompositeMoment::GetCurData(T_RPSC_D* pData)
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
// CCMRebarCompositeMoment message handlers

BOOL CCMRebarCompositeMoment::OnInitDialog() 
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


BOOL CCMRebarCompositeMoment::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Dlg2Data(&m_Data);
	return CChildDialog::DestroyWindow();
}

void CCMRebarCompositeMoment::OnCmdSameCheck() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data(&m_Data);
	ASSERT(m_pMomentJ);
	m_pMomentJ->SetCurData(&m_Data.Mbar[1], !m_Data.bMbarJ);
	m_pSectView->Invalidate();
}

void CCMRebarCompositeMoment::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCMRebarCompositeMoment::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	// TabWinddow Create & Setting 
	m_pMomentI = new CCMRebarCompositeMoment_I(this);
	m_ctrlTab.AddTab(m_pMomentI, _LS(IDS_CMD_RPSC_I), CCMRebarCompositeMoment_I::IDD, TRUE);

	m_pMomentJ = new CCMRebarCompositeMoment_J(this);
	m_ctrlTab.AddTab(m_pMomentJ, _LS(IDS_CMD_RPSC_J), CCMRebarCompositeMoment_J::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CCMRebarCompositeMoment::Data2Dlg()
{
	int nCheck = m_Data.bMbarJ ? 0 : 1;
	m_chkSameIJ.SetCheck(nCheck);
	
	// Crack Check
	BOOL bHasPart2 = FALSE;
	for (int i=0 ; i<m_Data.Mbar[0].GetSize() ; ++i)
		if(m_Data.Mbar[0][i].nPart == 2) { bHasPart2 = TRUE; break; }
	for (int i=0 ; i<m_Data.Mbar[1].GetSize() ; ++i)
		if(m_Data.Mbar[1][i].nPart == 2) { bHasPart2 = TRUE; break; }
	m_chkCrack.EnableWindow(bHasPart2);
	m_chkCrack.SetCheck(m_Data.bCracked);

	m_pMomentI->SetCurData(&m_Data.Mbar[0]);
	m_pMomentJ->SetCurData(&m_Data.Mbar[1], !m_Data.bMbarJ);
}

BOOL CCMRebarCompositeMoment::Dlg2Data(T_RPSC_D* pData, BOOL bSaveAlways/*=TRUE*/)
{
	pData->bMbarJ = (m_chkSameIJ.GetCheck() == 0) ? TRUE : FALSE;
	pData->bCracked = (m_chkCrack.IsWindowEnabled() && m_chkCrack.GetCheck() == 1) ? TRUE : FALSE;
	m_pMomentI->GetCurData(&pData->Mbar[0], bSaveAlways);
	m_pMomentJ->GetCurData(&pData->Mbar[1], bSaveAlways);
	if (!pData->bMbarJ)
	{
		pData->Mbar[1].Copy(pData->Mbar[0]);
	}

	return TRUE;
}

void CCMRebarCompositeMoment::RedrawView()
{
	ASSERT(m_pSectView);
	Dlg2Data(&m_Data, FALSE);
	m_pSectView->Invalidate();
}

void CCMRebarCompositeMoment::CrackButtonCheck()
{
	Dlg2Data(&m_Data, FALSE);

	BOOL bHasPart2 = FALSE;
	for (int i=0 ; i<m_Data.Mbar[0].GetSize() ; ++i)
		if(m_Data.Mbar[0][i].nPart == 2) { bHasPart2 = TRUE; break; }
	for (int i=0 ; i<m_Data.Mbar[1].GetSize() ; ++i)
		if(m_Data.Mbar[1][i].nPart == 2) { bHasPart2 = TRUE; break; }
	
	m_chkCrack.EnableWindow(bHasPart2);
	if (!bHasPart2) m_chkCrack.SetCheck(FALSE);
}

void CCMRebarCompositeMoment::SetPart1Enabled(BOOL bEnable)
{
	m_bPart1Enabled = bEnable;
}

BOOL CCMRebarCompositeMoment::IsPart1Enabled()
{
	return m_bPart1Enabled;
}