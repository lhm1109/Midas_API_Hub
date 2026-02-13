// CDgnSeisAnchorFailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorFailDlg.h"
#include "DgnSeisAnchorShapeMgr.h"
#include "BrdgSeisEval.h"
#include "BrdgSeisEvalKEC2012.h"
#include "BrdgSeisEvalKISTEC2015.h"
#include "BrdgSeisEvalKISTEC2019.h"
#include "BrdgSeisEvalKALIS2023.h"

#include "DgnSeisAnchorConcFail.h"
#include "DgnSeisAnchorPryout.h"
#include "..\wg_base\UtilFunc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorFailDlg dialog

CDgnSeisAnchorFailDlg::CDgnSeisAnchorFailDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchorFailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorFailDlg)	
	//}}AFX_DATA_INIT

	m_pConcFail = NULL;
 	m_pPryOut = NULL;

	m_bSetEmf = FALSE;;
	m_nTypeForEmf = 0;
	m_nDirForEmf = 0;
}

void CDgnSeisAnchorFailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorFailDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_FAIL_TAB,               m_tabFail);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorFailDlg::MakeEmfAndClose(CString strPathForEmf, int nType, int nDir)
{
	m_bSetEmf = TRUE;
	m_strPathForEmf = strPathForEmf;
	m_nTypeForEmf = nType;
	m_nDirForEmf = nDir;
}

void CDgnSeisAnchorFailDlg::MakeEmfFile() 
{
	::PumpMessagesAll();

	m_tabFail.ShowTab(m_nTypeForEmf);

	NMHDR nmhdr;
	nmhdr.code=TCN_SELCHANGE;
	nmhdr.idFrom=IDC_DGN_SEIS_ANCHOR_FAIL_TAB;
	nmhdr.hwndFrom=m_tabFail.m_hWnd;
	SendMessage(WM_NOTIFY,IDC_DGN_SEIS_ANCHOR_FAIL_TAB,(LPARAM)&nmhdr);

	::PumpMessagesAll();

	if(m_nTypeForEmf == 0)
	{
		m_pConcFail->MakeEmfFile(m_strPathForEmf, m_nDirForEmf);
	}
	else if(m_nTypeForEmf == 1)
	{
		m_pPryOut->MakeEmfFile(m_strPathForEmf, m_nDirForEmf);
	}	
	else
	{
		ASSERT(0);
	}
}

void CDgnSeisAnchorFailDlg::Data2Dlg() 
{ 
	if(m_pConcFail->m_hWnd != NULL) 
	{	
		m_pConcFail->SetData(m_Data);
		m_pConcFail->Data2Dlg();		
	}

	if(m_pPryOut->m_hWnd != NULL) 
	{
		m_pPryOut->SetData(m_Data);
		m_pPryOut->Data2Dlg();
	}

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorFailDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	if(m_pConcFail->m_hWnd != NULL) 
	{
		if(!m_pConcFail->Dlg2Data()) return FALSE;
		m_pConcFail->GetData(m_Data);
	}

	if(m_pPryOut->m_hWnd != NULL) 
	{
		if(!m_pPryOut->Dlg2Data()) return FALSE;
		m_pPryOut->GetData(m_Data);
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorFailDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_SEIS_ANCHOR_FAIL_TAB, OnDgnSeisAnchorFailChangingTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_SEIS_ANCHOR_FAIL_TAB, OnDgnSeisAnchorFailChangeTab)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_FAIL_OK_BTN,     OnDgnSeisAnchorFailOKBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_FAIL_CANCEL_BTN, OnDgnSeisAnchorFailCancelBtn)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorFailDlg message handlers

BOOL CDgnSeisAnchorFailDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SEIS_ANCHOR_FAIL_PLACEHOLDER);
	
	m_tabFail.SetPlaceHolder(pPlaceHolder);
	m_tabFail.DeleteAllItems();

	// 콘크리트 파괴
	m_pConcFail = new CDgnSeisAnchorConcFail(this);
	m_pConcFail->SetData(m_Data);
	m_tabFail.AddTab(m_pConcFail, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_CONC_FAIL), CDgnSeisAnchorConcFail::IDD, TRUE);  

	// 프라이아웃 파괴
	m_pPryOut = new CDgnSeisAnchorPryout(this);
	m_pPryOut->SetData(m_Data);
	m_tabFail.AddTab(m_pPryOut, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_PRYOUT), CDgnSeisAnchorPryout::IDD, TRUE);  

	//////////////////////////////////////////////////////////////////////////
	// 1.CDgnSeisAnchorShapeMgr에서 사용되는 static 변수 초기화.
	CDgnSeisAnchorShapeMgr::InitMapData4CalcShape();

	auto* pSeisEval = []() -> CBrdgSeisEval*
	{
		T_BSED_D BsedD; BsedD.Initialize();
		CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetDgnBsed(BsedD);
		switch(BsedD.nDesignCode)
		{
			case KEC_2012:	    return new CBrdgSeisEvalKEC2012();
			case KISTEC_2015:	return new CBrdgSeisEvalKISTEC2015();
			case KISTEC_2019:	return new CBrdgSeisEvalKISTEC2019();
			case KALIS_2023:	return new CBrdgSeisEvalKALIS2023();
			default:			ASSERT(0);  return new CBrdgSeisEvalKISTEC2015();
		}
	}();

	T_BRDGEVAL_BEARING rResD;
	if(!pSeisEval->Calc_BearingAnchorForDlg(0, m_Data, rResD))
	{
		ASSERT(0);
	}
	
	if(pSeisEval != nullptr)
	{
		delete pSeisEval;
		pSeisEval = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////

	m_tabFail.ShowTab(0);

	Data2Dlg();
	
	if(m_bSetEmf)
	{
        ShowWindow(SW_SHOWMINIMIZED);
        PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);

		SetTimer(524, 524, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorFailDlg::OnTimer( UINT_PTR nIDEvent )   
{   
	KillTimer(nIDEvent);   
	 
	MakeEmfFile();
	PostMessage(WM_CLOSE, 0, 0);
}

void CDgnSeisAnchorFailDlg::OnDgnSeisAnchorFailChangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Dlg2Data()) return;

	Data2Dlg();
	*pResult = 0;
}

void CDgnSeisAnchorFailDlg::OnDgnSeisAnchorFailChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Dlg2Data()) return;

	Data2Dlg();
	*pResult = 0;
}

void CDgnSeisAnchorFailDlg::OnDgnSeisAnchorFailOKBtn()
{
	if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CDgnSeisAnchorFailDlg::OnDgnSeisAnchorFailCancelBtn()
{
	CDialogMove::OnCancel();
}