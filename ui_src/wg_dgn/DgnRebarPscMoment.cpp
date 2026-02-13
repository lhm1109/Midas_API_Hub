// DgnRebarPscMoment.cpp : implementation file
//
// Add by sshan. MNET:2607.('20061211)
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarPscMoment.h"

#include "DgnRebarPscMoment_I.h"
#include "DgnRebarPscMoment_J.h"
#include "..\wg_cmd\CMSecViewWnd.h"
#include "DgnRebarPscDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarPscMoment dialog


CDgnRebarPscMoment::CDgnRebarPscMoment(CCMSecViewWnd* pSectView, int nPart, CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRebarPscMoment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarPscMoment)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pSectView);
	m_pSectView = pSectView;
	ASSERT(pParent);
	m_pParent = (CDgnRebarPscDlg*)pParent;

	m_nPart = nPart;

	m_pMomentI = NULL;
	m_pMomentJ = NULL;

	m_Data.Initialize();
}


void CDgnRebarPscMoment::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarPscMoment)
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_SAME_CHECK, m_chkSameIJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRebarPscMoment, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRebarPscMoment)
	ON_BN_CLICKED(IDC_DGN_SAME_CHECK, OnDgnSameCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_TAB, OnSelchangeDgnTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface

void CDgnRebarPscMoment::SetCurData(T_RPSC_D* pData)
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

BOOL CDgnRebarPscMoment::GetCurData(T_RPSC_D* pData)
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
// CDgnRebarPscMoment message handlers

BOOL CDgnRebarPscMoment::OnInitDialog() 
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


BOOL CDgnRebarPscMoment::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Dlg2Data(&m_Data);
	return CChildDialog::DestroyWindow();
}

void CDgnRebarPscMoment::OnDgnSameCheck() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data(&m_Data);
	ASSERT(m_pMomentJ);
	m_pMomentJ->SetCurData(&m_Data.Mbar[1], !m_Data.bMbarJ);
	m_pSectView->Invalidate();
}

void CDgnRebarPscMoment::OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CDgnRebarPscMoment::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	// TabWinddow Create & Setting 
	m_pMomentI = new CDgnRebarPscMoment_I(this);
	m_ctrlTab.AddTab(m_pMomentI, _LS(IDS_DGN_RPSC_I), CDgnRebarPscMoment_I::IDD, TRUE);

	m_pMomentJ = new CDgnRebarPscMoment_J(this);
	m_ctrlTab.AddTab(m_pMomentJ, _LS(IDS_DGN_RPSC_J), CDgnRebarPscMoment_J::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CDgnRebarPscMoment::Data2Dlg()
{
	int nCheck = m_Data.bMbarJ ? 0 : 1;
	m_chkSameIJ.SetCheck(nCheck);
	m_pMomentI->SetCurData(&m_Data.Mbar[0]);
	m_pMomentJ->SetCurData(&m_Data.Mbar[1], !m_Data.bMbarJ);
}

BOOL CDgnRebarPscMoment::Dlg2Data(T_RPSC_D* pData, BOOL bSaveAlways/*=TRUE*/)
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

void CDgnRebarPscMoment::RedrawView()
{
	ASSERT(m_pSectView);
	Dlg2Data(&m_Data, FALSE);

	if(m_nPart == 2)
	{
		// Modify by sshan. Composite_I Section만 들어오므로 nPart는 2.('20070209)
		// CPG일경우는 nPart를 2로 설정
		// 재오씨와 협의 하여 결정
		for(int c=0 ; c<m_Data.Mbar[0].GetSize() ; c++)
		{
			m_Data.Mbar[0][c].nPart = 2;
		}
		for(int c=0 ; c<m_Data.Mbar[1].GetSize() ; c++)
		{
			m_Data.Mbar[1][c].nPart = 2;
		}
	}

	m_pSectView->Invalidate();
}