// DgnRebarCpgStiff.cpp : implementation file
//
// Add by sshan. ('20061227)
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarCpgStiff.h"

#include "DgnRebarCpgStiff_I.h"
#include "DgnRebarCpgStiff_J.h"
#include "..\wg_cmd\CMSecViewWnd.h"
#include "DgnRebarCpgStiffMain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiff dialog


CDgnRebarCpgStiff::CDgnRebarCpgStiff(CCMSecViewWnd* pSectView, CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRebarCpgStiff::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarCpgStiff)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pSectView);
	m_pSectView = pSectView;
	ASSERT(pParent);
	m_pParent = (CDgnRebarCpgStiffMain*)pParent;

	m_pStiffI = NULL;
	m_pStiffJ = NULL;

	m_Data.Initialize();
}


void CDgnRebarCpgStiff::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarCpgStiff)
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_SAME_CHECK, m_chkSameIJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRebarCpgStiff, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRebarCpgStiff)
	ON_BN_CLICKED(IDC_DGN_SAME_CHECK, OnDgnSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_TAB, OnSelchangeDgnTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface

void CDgnRebarCpgStiff::SetCurData(T_CLRS_D* pData)
{
	ASSERT(pData);
	m_Data = *pData;
	if (m_Data.bLStifJ == 0)
	{
		m_Data.LStif[1].Copy(m_Data.LStif[0]);
	}
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
		Data2Dlg();
}

BOOL CDgnRebarCpgStiff::GetCurData(T_CLRS_D* pData)
{
	ASSERT(pData);
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd)) Dlg2Data(pData);
	else
	{
		pData->bLStifJ = m_Data.bLStifJ;
		pData->LStif[0].Copy(m_Data.LStif[0]);
		pData->LStif[1].Copy(m_Data.LStif[1]);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiff message handlers

BOOL CDgnRebarCpgStiff::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->LayoutChildDlg();

	Data2Dlg();
	
	int nSel = m_ctrlTab.GetCurSel();
	//m_pSectView->SetDataSourceRebar(&m_Data, nSel==1);
	//m_pSectView->Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDgnRebarCpgStiff::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Dlg2Data(&m_Data);
	return CChildDialog::DestroyWindow();
}

void CDgnRebarCpgStiff::OnDgnSameCheck() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data(&m_Data);
	ASSERT(m_pStiffJ);
	m_pStiffJ->SetCurData(&m_Data.LStif[1], !m_Data.bLStifJ);
	m_pSectView->Invalidate();
}

void CDgnRebarCpgStiff::OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	// save data
	Dlg2Data(&m_Data);
	ASSERT(m_pStiffJ);
	m_pStiffJ->SetCurData(&m_Data.LStif[1], !m_Data.bLStifJ);
 
	// view update
	int nSel = m_ctrlTab.GetCurSel();
	//m_pSectView->SetDataSourceRebar(&m_Data, nSel==1);
	//m_pParent->UpdateView(nSel == 0);
	//m_pSectView->Invalidate();

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CDgnRebarCpgStiff::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	// TabWinddow Create & Setting 
	m_pStiffI = new CDgnRebarCpgStiff_I(this);
	m_ctrlTab.AddTab(m_pStiffI, _LS(IDS_DGN_RPSC_I), CDgnRebarCpgStiff_I::IDD, TRUE);

	m_pStiffJ = new CDgnRebarCpgStiff_J(this);
	m_ctrlTab.AddTab(m_pStiffJ, _LS(IDS_DGN_RPSC_J), CDgnRebarCpgStiff_J::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CDgnRebarCpgStiff::Data2Dlg()
{
	int nCheck = m_Data.bLStifJ ? 0 : 1;
	m_chkSameIJ.SetCheck(nCheck);
	m_pStiffI->SetCurData(&m_Data.LStif[0]);
	m_pStiffJ->SetCurData(&m_Data.LStif[1], !m_Data.bLStifJ);
}

BOOL CDgnRebarCpgStiff::Dlg2Data(T_CLRS_D* pData, BOOL bSaveAlways/*=TRUE*/)
{
	pData->bLStifJ = (m_chkSameIJ.GetCheck() == 0) ? TRUE : FALSE;
	m_pStiffI->GetCurData(&pData->LStif[0], bSaveAlways);
	m_pStiffJ->GetCurData(&pData->LStif[1], bSaveAlways);
	if (!pData->bLStifJ)
	{
		pData->LStif[1].Copy(pData->LStif[0]);
	}

	return TRUE;
}

void CDgnRebarCpgStiff::RedrawView()
{
	ASSERT(m_pSectView);
	Dlg2Data(&m_Data, FALSE);
	m_pSectView->Invalidate();
}