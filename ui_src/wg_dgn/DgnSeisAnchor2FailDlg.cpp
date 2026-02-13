// CDgnSeisAnchor2FailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchor2FailDlg.h"
#include "DgnSeisAnchorShapeMgr.h"
#include "BrdgSeisEval.h"
#include "BrdgSeisEvalKEC2012.h"
#include "BrdgSeisEvalKISTEC2015.h"
#include "BrdgSeisEvalKISTEC2019.h"
#include "BrdgSeisEvalKALIS2023.h"

#include "DgnSeisAnchor2ConcFail.h"
#include "DgnSeisAnchor2Pryout.h"
#include "..\wg_base\UtilFunc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2FailDlg dialog

CDgnSeisAnchor2FailDlg::CDgnSeisAnchor2FailDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchor2FailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchor2FailDlg)	
	//}}AFX_DATA_INIT

	m_pConcFail = NULL;
 	m_pPryOut = NULL;

	m_bSetEmf = FALSE;;
	m_nTypeForEmf = 0;
	m_nDirForEmf = 0;

	m_bIsReport = FALSE;
	for (int i = 0; i < 4; i++)
	{
		m_nRatTypeFail[i] = 0;
	}
}

void CDgnSeisAnchor2FailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchor2FailDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_FAIL_TAB,               m_tabFail);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchor2FailDlg::MakeEmfAndClose(CString strPathForEmf, int nType, int nDir)
{
	m_bSetEmf = TRUE;
	m_strPathForEmf = strPathForEmf;
	m_nTypeForEmf = nType;
	m_nDirForEmf = nDir;

	// 1.CDgnSeisAnchorShapeMgr에서 사용되는 static 변수 초기화.
	CDgnSeisAnchorShapeMgr::InitMapData4CalcShape();
}

void CDgnSeisAnchor2FailDlg::MakeEmfFile() 
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

void CDgnSeisAnchor2FailDlg::Data2Dlg() 
{ 
	if(m_pConcFail->m_hWnd != NULL) 
	{	
		m_pConcFail->SetData(m_Data, m_nRatTypeFail);
		m_pConcFail->Data2Dlg();		
	}

	if(m_pPryOut->m_hWnd != NULL) 
	{
		m_pPryOut->SetData(m_Data, m_nRatTypeFail);
		m_pPryOut->Data2Dlg();
	}

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchor2FailDlg::Dlg2Data(BOOL bOKBtn/*=FALSE*/, BOOL bRedraw/* = FALSE*/)
{
	UpdateData(TRUE);

	if(m_pConcFail->m_hWnd != NULL) 
	{
		if(!m_pConcFail->Dlg2Data(bOKBtn)) return FALSE;
		m_pConcFail->GetData(m_Data);
		if(bRedraw && !m_Data.Fail.bUser[0])
		{
			m_pConcFail->SetViewData();
		}
	}

	if(m_pPryOut->m_hWnd != NULL) 
	{
		if(!m_pPryOut->Dlg2Data(bOKBtn)) return FALSE;
		m_pPryOut->GetData(m_Data);
		if (bRedraw && !m_Data.Fail.bUser[1])
		{
			m_pPryOut->SetViewData();
		}

	}

	return TRUE;
}

void CDgnSeisAnchor2FailDlg::Calc_BearingAnchorForDlg(int nFailType)
{
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
	if(!pSeisEval->Calc_BearingAnchorForDlg(nFailType, m_Data, rResD))
	{
		ASSERT(0);
	}

	if(pSeisEval != nullptr)
	{
		delete pSeisEval;
		pSeisEval = nullptr;
	}

	// 검토 방향
	m_nRatTypeFail[0] = rResD.AnchorD[0].FACD.nRatType;
	m_nRatTypeFail[1] = rResD.AnchorD[1].FACD.nRatType;
	m_nRatTypeFail[2] = rResD.AnchorD[0].FAPD.nRatType;
	m_nRatTypeFail[3] = rResD.AnchorD[1].FAPD.nRatType;

}

BEGIN_MESSAGE_MAP(CDgnSeisAnchor2FailDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_SEIS_ANCHOR_FAIL_TAB, OnDgnSeisAnchorFailChangingTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_SEIS_ANCHOR_FAIL_TAB, OnDgnSeisAnchorFailChangeTab)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_FAIL_REDRAW_BTN, OnDgnSeisAnchorRedrawFailBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_FAIL_OK_BTN,     OnDgnSeisAnchorFailOKBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_FAIL_CANCEL_BTN, OnDgnSeisAnchorFailCancelBtn)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2FailDlg message handlers

BOOL CDgnSeisAnchor2FailDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SEIS_ANCHOR_FAIL_PLACEHOLDER);
	
	m_tabFail.SetPlaceHolder(pPlaceHolder);
	m_tabFail.DeleteAllItems();

	//////////////////////////////////////////////////////////////////////////
	if(m_bIsReport)
	{
		// CBrdgSeisEvalKISTEC2015_Report 에서 세팅됨.
	}
	else
	{
		Calc_BearingAnchorForDlg(2);
// 		T_BRDGEVAL_BEARING rResD;
// 		CBrdgSeisEvalKISTEC2015 Dgn;
// 		if (!Dgn.Calc_BearingAnchorForDlg(m_Data, rResD))
// 		{
// 			ASSERT(0);
// 		}
// 
// 		// 검토 방향
// 		m_nRatTypeFail[0] = rResD.AnchorD[0].FACD.nRatType;
// 		m_nRatTypeFail[1] = rResD.AnchorD[1].FACD.nRatType;
// 		m_nRatTypeFail[2] = rResD.AnchorD[0].FAPD.nRatType;
// 		m_nRatTypeFail[3] = rResD.AnchorD[1].FAPD.nRatType;

	}
	//////////////////////////////////////////////////////////////////////////

	// 콘크리트 파괴
	m_pConcFail = new CDgnSeisAnchor2ConcFail(this, m_bSetEmf);
	m_pConcFail->SetData(m_Data, m_nRatTypeFail);
	m_tabFail.AddTab(m_pConcFail, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_CONC_FAIL), CDgnSeisAnchor2ConcFail::IDD, TRUE);  

	// 프라이아웃 파괴
	m_pPryOut = new CDgnSeisAnchor2Pryout(this, m_bSetEmf);
	m_pPryOut->SetData(m_Data, m_nRatTypeFail);
	m_tabFail.AddTab(m_pPryOut, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_PRYOUT), CDgnSeisAnchor2Pryout::IDD, TRUE);  
	
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

void CDgnSeisAnchor2FailDlg::OnTimer( UINT_PTR nIDEvent )   
{   
	KillTimer(nIDEvent);   
	 
	MakeEmfFile();
	PostMessage(WM_CLOSE, 0, 0);
}

void CDgnSeisAnchor2FailDlg::OnDgnSeisAnchorFailChangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Dlg2Data()) return;

	Data2Dlg();
	*pResult = 0;
}

void CDgnSeisAnchor2FailDlg::OnDgnSeisAnchorFailChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Dlg2Data()) return;

	Data2Dlg();
	*pResult = 0;
}

void CDgnSeisAnchor2FailDlg::OnDgnSeisAnchorRedrawFailBtn()
{
	// 1.CDgnSeisAnchorShapeMgr에서 사용되는 static 변수 초기화.
	CDgnSeisAnchorShapeMgr::InitMapData4CalcShape();

	//if (!Dlg2Data(TRUE)) return;

	UpdateData(TRUE);

	int nFailType = 0;
	// Set data
	if (m_pConcFail->m_hWnd != NULL)
	{
		nFailType = 0;
		if (!m_pConcFail->Dlg2Data()) { ASSERT(0); return; }
		m_pConcFail->GetData(m_Data);
	}

	if (m_pPryOut->m_hWnd != NULL)
	{
		nFailType = 1;
		if (!m_pPryOut->Dlg2Data()) { ASSERT(0); return; }
		m_pPryOut->GetData(m_Data);
	}

	// Calc
	Calc_BearingAnchorForDlg(nFailType);

	// Draw
	if (m_pConcFail->m_hWnd != NULL)
	{
		if (!m_Data.Fail.bUser[0])
		{
			m_pConcFail->SetData(m_Data, m_nRatTypeFail);
			m_pConcFail->Data2Dlg();
			m_pConcFail->SetViewData();
		}
	}

	if (m_pPryOut->m_hWnd != NULL)
	{
		if (!m_Data.Fail.bUser[1])
		{
			m_pPryOut->SetData(m_Data, m_nRatTypeFail);
			m_pPryOut->Data2Dlg();
			m_pPryOut->SetViewData();
		}
	}
}

void CDgnSeisAnchor2FailDlg::OnDgnSeisAnchorFailOKBtn()
{
	if(!Dlg2Data(TRUE)) return;

	CDialogMove::OnOK();
}

void CDgnSeisAnchor2FailDlg::OnDgnSeisAnchorFailCancelBtn()
{
	CDialogMove::OnCancel();
}