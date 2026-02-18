// CDgnSeisAnchorPryout.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorPryout.h"

#include "DgnSeisAnchorPryoutSub.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_base\UtilFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorPryout dialog

CDgnSeisAnchorPryout::CDgnSeisAnchorPryout(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorPryout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorPryout)	
	//}}AFX_DATA_INIT

	m_bUser = FALSE;

	m_Data.Fail.PryoutL.Initialize();
	m_Data.Fail.PryoutT.Initialize();

	m_pPryoutSubL = NULL;
	m_pPryoutSubT = NULL;
	m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
}

void CDgnSeisAnchorPryout::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorPryout)
	DDX_Check  (pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_USER_CHK,    m_bUser);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB,         m_tabPryout);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorPryout::PostNcDestroy() 
{
	CChildDialog::PostNcDestroy();
}

void CDgnSeisAnchorPryout::SetData(T_ANEV_D& Data)
{
	m_Data = Data;
	m_bUser = m_Data.Fail.bUser[0];
	if(!m_bUser)
	{
		m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
	}
}

void CDgnSeisAnchorPryout::GetData(T_ANEV_D& Data)
{
	Data = m_Data;
}

void CDgnSeisAnchorPryout::Data2Dlg() 
{ 
	m_bUser = m_Data.Fail.bUser[1];

	if(!m_bUser) SetRstData();

	int nCurTab = m_tabPryout.GetCurSel();
	if(nCurTab == 0) 
	{
		m_pPryoutSubL->SetSubData(0, m_Data);
		m_pPryoutSubL->Data2Dlg();		
	}
	else
	{
		m_pPryoutSubT->SetSubData(1, m_Data);		
		m_pPryoutSubT->Data2Dlg();
	}

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorPryout::Dlg2Data() 
{
	UpdateData(TRUE);

	int nCurTab = m_tabPryout.GetCurSel();
	if(nCurTab == 0) 
	{
		if(!m_pPryoutSubL->Dlg2Data()) return FALSE;
		m_pPryoutSubL->GetSubData(m_Data);
	}
	else
	{
		if(!m_pPryoutSubT->Dlg2Data()) return FALSE;
		m_pPryoutSubT->GetSubData(m_Data);
	}

	m_Data.Fail.bUser[1] = m_bUser;
		
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorPryout, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_PRYOUT_USER_CHK,         OnDgnSeisAnchorPryoutUesrChk)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB, OnDgnSeisAnchorPryoutChangingTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB, OnDgnSeisAnchorPryoutChangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorPryout message handlers

BOOL CDgnSeisAnchorPryout::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SEIS_ANCHOR_PRYOUT_PLACEHOLDER);

	m_tabPryout.SetPlaceHolder(pPlaceHolder);
	m_tabPryout.DeleteAllItems();

	// 교축방향
	m_pPryoutSubL = new CDgnSeisAnchorPryoutSub(this);
	m_pPryoutSubL->SetSubData(0, m_Data);
	m_tabPryout.AddTab(m_pPryoutSubL, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_PRYOUT_L), CDgnSeisAnchorPryoutSub::IDD, TRUE);  

	// 교축직각방향
	m_pPryoutSubT = new CDgnSeisAnchorPryoutSub(this);
	m_pPryoutSubT->SetSubData(1, m_Data);
	m_tabPryout.AddTab(m_pPryoutSubT, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_PRYOUT_T), CDgnSeisAnchorPryoutSub::IDD, TRUE);  

	m_tabPryout.ShowTab(0);

	Data2Dlg();
	OnDgnSeisAnchorPryoutUesrChk();	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorPryout::SetRstData()
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
	//ShapeMgr.UpdateCa1Auto();

	double dArea, dCa1, dCa1P, dCa2, dCa2P, dHef;
	int nBc, nResistAnchorNum;	

	ShapeMgr.UpdatehefAuto();

	std::vector<std::pair<int, int>> aRepBearingIdx;
	
	BOOL bSingleResist;
	dArea = dCa1 = dCa1P = dCa2 = dCa2P = dHef = 0.0;
	nBc = 0;
	std::vector<std::pair<int, int>> aRepBearingIdxTemp;
	if(ShapeMgr.GetPryoutRepresentVal(0, aRepBearingIdxTemp, dArea, nBc, dCa1, dCa1P, dCa2, dCa2P, bSingleResist, nResistAnchorNum))
	{
		ShapeMgr.GetPryoutRepresentGrpBearing(0, aRepBearingIdx);
		dHef = 0.0;
		for (int i = 0; i < aRepBearingIdx.size(); i++)
		{
			int nBearingI, nBearingJ;
			nBearingI = aRepBearingIdx[i].first;
			nBearingJ = aRepBearingIdx[i].second;

			dHef += ShapeMgr.Gethef(0, nBearingI, nBearingJ);
		}
		dHef /= aRepBearingIdx.size();

		m_Data.Fail.PryoutL.nNum = nBc;
		m_Data.Fail.PryoutL.dANc = dArea;
		m_Data.Fail.PryoutL.dANco = 9 * dHef * dHef;
		m_Data.Fail.PryoutL.dHef = dHef;
		m_Data.Fail.PryoutL.dCa1 = dCa1;
		m_Data.Fail.PryoutL.dCa1_q = dCa1P;
		m_Data.Fail.PryoutL.dCa2 = dCa2;
		m_Data.Fail.PryoutL.dCa2_q = dCa2P;
	}
	
	dArea = dCa1 = dCa1P = dCa2 = dCa2P = dHef = 0.0;
	nBc = 0;
	if(ShapeMgr.GetPryoutRepresentVal(1, aRepBearingIdxTemp, dArea, nBc, dCa1, dCa1P, dCa2, dCa2P, bSingleResist, nResistAnchorNum))
	{
		ShapeMgr.GetPryoutRepresentGrpBearing(1, aRepBearingIdx);
		dHef = 0.0;
		for (int i = 0; i < aRepBearingIdx.size(); i++)
		{
			int nBearingI, nBearingJ;
			nBearingI = aRepBearingIdx[i].first;
			nBearingJ = aRepBearingIdx[i].second;

			dHef += ShapeMgr.Gethef(1, nBearingI, nBearingJ);
		}
		dHef /= aRepBearingIdx.size();

		m_Data.Fail.PryoutT.nNum = nBc;
		m_Data.Fail.PryoutT.dANc = dArea;
		m_Data.Fail.PryoutT.dANco = 9 * dHef * dHef;
		m_Data.Fail.PryoutT.dHef = dHef;
		m_Data.Fail.PryoutT.dCa1 = dCa1;
		m_Data.Fail.PryoutT.dCa1_q = dCa1P;
		m_Data.Fail.PryoutT.dCa2 = dCa2;
		m_Data.Fail.PryoutT.dCa2_q = dCa2P;
	}
}

void CDgnSeisAnchorPryout::MakeEmfFile(CString strPathForEmf, int nDir)
{
	m_tabPryout.ShowTab(nDir);

	NMHDR nmhdr;
	nmhdr.code=TCN_SELCHANGE;
	nmhdr.idFrom=IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB;
	nmhdr.hwndFrom=m_tabPryout.m_hWnd;
	SendMessage(WM_NOTIFY,IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB,(LPARAM)&nmhdr);

	::PumpMessagesAll();

	if(nDir == 0)
	{
		m_pPryoutSubL->MakeEmfFile(strPathForEmf);
	}
	else if(nDir == 1)
	{
		m_pPryoutSubT->MakeEmfFile(strPathForEmf);
	}	
	else
	{
		ASSERT(0);
	}
}

void CDgnSeisAnchorPryout::OnDgnSeisAnchorPryoutUesrChk() 
{
	UpdateData(TRUE);

	m_Data.Fail.bUser[1] = m_bUser;

	if(!m_bUser)
	{
		Data2Dlg();
		
		m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
	}
	else
	{
		if(m_pPryoutSubL->m_hWnd != NULL)
		{		
			m_pPryoutSubL->SetSubData(0, m_Data);			
		}
		if(m_pPryoutSubT->m_hWnd != NULL) 
		{
			m_pPryoutSubT->SetSubData(1, m_Data);					
		}
	}

	int nCurTab = m_tabPryout.GetCurSel();
	if(nCurTab == 0) 
	{
		m_pPryoutSubL->EnableDisableControls();
		m_pPryoutSubL->SetViewData();
		m_bNeedReCalc[0] = FALSE;
	}
	else
	{
		m_pPryoutSubT->EnableDisableControls();
		m_pPryoutSubT->SetViewData();
		m_bNeedReCalc[1] = FALSE;
	}	
	
}

void CDgnSeisAnchorPryout::OnDgnSeisAnchorPryoutChangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Dlg2Data()) return;

	//Data2Dlg();	
	*pResult = 0;
}

void CDgnSeisAnchorPryout::OnDgnSeisAnchorPryoutChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//if(!Dlg2Data()) return;

	Data2Dlg();

	int nCurTab = m_tabPryout.GetCurSel();
	if(m_bNeedReCalc[nCurTab])
	{
		OnDgnSeisAnchorPryoutUesrChk();
	}
	*pResult = 0;
}

