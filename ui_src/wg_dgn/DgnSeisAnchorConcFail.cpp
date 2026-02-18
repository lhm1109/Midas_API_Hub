// CDgnSeisAnchorConcFail.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorConcFail.h"

#include "DgnSeisAnchorConcFailSub.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_base\UtilFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorConcFail dialog

CDgnSeisAnchorConcFail::CDgnSeisAnchorConcFail(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorConcFail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorConcFail)	
	//}}AFX_DATA_INIT

	m_bUser = FALSE;

	m_Data.Fail.ConcFailL.Initialize();
	m_Data.Fail.ConcFailT.Initialize();

 	m_pConcFailSubL = NULL;
 	m_pConcFailSubT = NULL;
	m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
}

void CDgnSeisAnchorConcFail::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorConcFail)
	DDX_Check  (pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_USER_CHK,    m_bUser);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_TAB,         m_tabConcFail);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorConcFail::PostNcDestroy() 
{
	CChildDialog::PostNcDestroy();
}

void CDgnSeisAnchorConcFail::SetData(T_ANEV_D& Data)
{
	m_Data = Data;
	m_bUser = m_Data.Fail.bUser[0];
	if(!m_bUser)
	{
		m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
	}
}

void CDgnSeisAnchorConcFail::GetData(T_ANEV_D& Data)
{
	Data = m_Data;
}

void CDgnSeisAnchorConcFail::Data2Dlg() 
{ 
	m_bUser = m_Data.Fail.bUser[0];

	if(!m_bUser) SetRstData();

	int nCurTab = m_tabConcFail.GetCurSel();
	if(nCurTab == 0) 
	{
		m_pConcFailSubL->SetSubData(0, m_Data);
		m_pConcFailSubL->Data2Dlg();		
	}
	else
	{
		m_pConcFailSubT->SetSubData(1, m_Data);		
		m_pConcFailSubT->Data2Dlg();
	}

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorConcFail::Dlg2Data() 
{
	UpdateData(TRUE);

	int nCurTab = m_tabConcFail.GetCurSel();
	if(nCurTab == 0) 
	{
		if(!m_pConcFailSubL->Dlg2Data()) return FALSE;
		m_pConcFailSubL->GetSubData(m_Data);
	}
	else
	{
		if(!m_pConcFailSubT->Dlg2Data()) return FALSE;
		m_pConcFailSubT->GetSubData(m_Data);
	}

	m_Data.Fail.bUser[0] = m_bUser;
		
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDgnSeisAnchorConcFail, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_CONC_FAIL_USER_CHK,         OnDgnSeisAnchorConcFailUesrChk)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_TAB, OnDgnSeisAnchorConcFailChangingTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_SEIS_ANCHOR_CONC_FAIL_TAB, OnDgnSeisAnchorConcFailChangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorConcFail message handlers

BOOL CDgnSeisAnchorConcFail::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SEIS_ANCHOR_CONC_FAIL_PLACEHOLDER);
	
	m_tabConcFail.SetPlaceHolder(pPlaceHolder);
	m_tabConcFail.DeleteAllItems();

	// 교축방향
	m_pConcFailSubL = new CDgnSeisAnchorConcFailSub(this);
	m_pConcFailSubL->SetSubData(0, m_Data);
	m_tabConcFail.AddTab(m_pConcFailSubL, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_CONC_FAIL_L), CDgnSeisAnchorConcFailSub::IDD, TRUE);  

	// 교축직각방향
	m_pConcFailSubT = new CDgnSeisAnchorConcFailSub(this);
	m_pConcFailSubT->SetSubData(1, m_Data);
	m_tabConcFail.AddTab(m_pConcFailSubT, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_CONC_FAIL_T), CDgnSeisAnchorConcFailSub::IDD, TRUE);  

	m_tabConcFail.ShowTab(0);
	
	Data2Dlg();
	OnDgnSeisAnchorConcFailUesrChk();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorConcFail::SetRstData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_BRPR_D BrprD;
	std::vector<std::vector<T_BRPR_D>> aArBrprD;
	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(!pDoc->m_pAttrCtrl2->GetBrpr(m_Data.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
			aArBrprD[i].push_back(BrprD);
		}
	}
	CDgnSeisAnchorShapeMgr ShapeMgr;
	ShapeMgr.SetData(aArBrprD, m_Data);
	ShapeMgr.UpdateCa1Auto();
	//ShapeMgr.UpdatehefAuto();

	std::vector<std::pair<int, int>> aRepBearingIdx;
	double dLp, dCa1, dCa1P, dCa2, dCa2P, dHa;
	double dEachCa1, dEachCa1P, dEachCa2, dEachCa2P;
	double dCa1Temp, dCa1Recal;
	int nBc;
	double dAVcoSum = 0.0;
	double dAVco = 0.0;
	dLp = dCa1 = dCa1P = dCa2 = dCa2P = dHa = 0.0;
	nBc = 0;
	if(ShapeMgr.GetConcFailRepresentVal(0, dLp, nBc, dCa1, dCa1P, dCa2, dCa2P))
	{				
		ShapeMgr.GetConcfailRepresentGrpBearing(0, aRepBearingIdx);
		dHa = dCa1Recal = 0.0;
		for (int i = 0; i < aRepBearingIdx.size(); i++)
		{
			if (!ShapeMgr.GetReCalcCa1(0, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dCa1Temp))
			{
				ShapeMgr.GetConcFailBearingCa(0, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dEachCa1, dEachCa1P, dEachCa2, dEachCa2P);
				dCa1Temp = dEachCa1;
			}

			dAVcoSum  += 4.5 * dCa1Temp * dCa1Temp;
			dCa1Recal += dCa1Temp;
			dHa += min(m_Data.dHcop, 1.5 * dCa1Temp);
		}
		if(aRepBearingIdx.size()>0)
		{
			dAVco = dAVcoSum / aRepBearingIdx.size();
			dCa1Recal /= aRepBearingIdx.size();
			dHa /= aRepBearingIdx.size();
		}
		
		// 이거 바꾸면 CDgnSeisAnchorEvalEditBtn::Setha 여기도 처리해야 함
		m_Data.Fail.ConcFailL.nNum = nBc;
		m_Data.Fail.ConcFailL.dAVc = dLp * dHa;
		m_Data.Fail.ConcFailL.dAVco = dAVco;
		m_Data.Fail.ConcFailL.dHa = dHa;
		m_Data.Fail.ConcFailL.dCa1 = dCa1Recal;
		m_Data.Fail.ConcFailL.dCa1_q = dCa1P;
		m_Data.Fail.ConcFailL.dCa2 = dCa2;
		m_Data.Fail.ConcFailL.dCa2_q = dCa2P;
	}
	
	dAVcoSum = 0.0;
	dAVco = 0.0;
	dLp = dCa1 = dCa1P = dCa2 = dCa2P = dHa = 0.0;
	nBc = 0;
	if(ShapeMgr.GetConcFailRepresentVal(1, dLp, nBc, dCa1, dCa1P, dCa2, dCa2P))
	{
		ShapeMgr.GetConcfailRepresentGrpBearing(1, aRepBearingIdx);
		dHa = dCa1Recal = 0.0;
		for (int i = 0; i < aRepBearingIdx.size(); i++)
		{
			if (!ShapeMgr.GetReCalcCa1(1, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dCa1Temp))
			{
				ShapeMgr.GetConcFailBearingCa(1, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dEachCa1, dEachCa1P, dEachCa2, dEachCa2P);
				dCa1Temp = dEachCa1;
			}
			
			dAVcoSum += 4.5 * dCa1Temp * dCa1Temp;
			dCa1Recal += dCa1Temp;
			dHa += min(m_Data.dHcop, 1.5 * dCa1Temp);
		}

		if (aRepBearingIdx.size() > 0)
		{
			dAVco = dAVcoSum / aRepBearingIdx.size();
			dCa1Recal /= aRepBearingIdx.size();
			dHa /= aRepBearingIdx.size();
		}

		// 이거 바꾸면 CDgnSeisAnchorEvalEditBtn::Setha 여기도 처리해야 함
		m_Data.Fail.ConcFailT.nNum = nBc;
		m_Data.Fail.ConcFailT.dAVc = dLp * dHa;
		m_Data.Fail.ConcFailT.dAVco = dAVco;
		m_Data.Fail.ConcFailT.dHa = dHa;
		m_Data.Fail.ConcFailT.dCa1 = dCa1Recal;
		m_Data.Fail.ConcFailT.dCa1_q = dCa1P;
		m_Data.Fail.ConcFailT.dCa2 = dCa2;
		m_Data.Fail.ConcFailT.dCa2_q = dCa2P;
	}
}

void CDgnSeisAnchorConcFail::MakeEmfFile(CString strPathForEmf, int nDir)
{
	m_tabConcFail.ShowTab(nDir);

	NMHDR nmhdr;
	nmhdr.code=TCN_SELCHANGE;
	nmhdr.idFrom=IDC_DGN_SEIS_ANCHOR_CONC_FAIL_TAB;
	nmhdr.hwndFrom=m_tabConcFail.m_hWnd;
	SendMessage(WM_NOTIFY,IDC_DGN_SEIS_ANCHOR_CONC_FAIL_TAB,(LPARAM)&nmhdr);

	::PumpMessagesAll();

	if(nDir == 0)
	{
		m_pConcFailSubL->MakeEmfFile(strPathForEmf);
	}
	else if(nDir == 1)
	{
		m_pConcFailSubT->MakeEmfFile(strPathForEmf);
	}	
	else
	{
		ASSERT(0);
	}
}

void CDgnSeisAnchorConcFail::OnDgnSeisAnchorConcFailUesrChk() 
{
	UpdateData(TRUE);
	
	m_Data.Fail.bUser[0] = m_bUser;

	if(!m_bUser)
	{
		Data2Dlg();

		m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
	}
	else
	{
		if(m_pConcFailSubL->m_hWnd != NULL) 
		{
			m_pConcFailSubL->SetSubData(0, m_Data);		
		}
		if(m_pConcFailSubT->m_hWnd != NULL) 
		{
			m_pConcFailSubT->SetSubData(1, m_Data);				
		}
	}
	
	int nCurTab = m_tabConcFail.GetCurSel();
	if(nCurTab == 0) 
	{
		m_pConcFailSubL->EnableDisableControls();		
		m_pConcFailSubL->SetViewData();
		m_bNeedReCalc[0] = FALSE;
	}
	else
	{
		m_pConcFailSubT->EnableDisableControls();
		m_pConcFailSubT->SetViewData();
		m_bNeedReCalc[1] = FALSE;
	}		
}

void CDgnSeisAnchorConcFail::OnDgnSeisAnchorConcFailChangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Dlg2Data()) return;

	//Data2Dlg();
	
	*pResult = 0;
}

void CDgnSeisAnchorConcFail::OnDgnSeisAnchorConcFailChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//if(!Dlg2Data()) return;

	Data2Dlg();
	int nCurTab = m_tabConcFail.GetCurSel();
	if(m_bNeedReCalc[nCurTab])
	{
		OnDgnSeisAnchorConcFailUesrChk();
	}
	*pResult = 0;
}