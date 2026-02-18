// CDgnSeisAnchor2Pryout.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchor2Pryout.h"

#include "DgnSeisAnchor2PryoutSub.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_base\UtilFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2Pryout dialog

CDgnSeisAnchor2Pryout::CDgnSeisAnchor2Pryout(CWnd* pParent /*=NULL*/, BOOL bPrint/* = FALSE*/)
	: CChildDialog(CDgnSeisAnchor2Pryout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchor2Pryout)	
	//}}AFX_DATA_INIT

	m_bUser = FALSE;
	m_bPrintCF = bPrint;

	m_Data.Fail.PryoutL.Initialize();
	m_Data.Fail.PryoutT.Initialize();

	m_pPryoutSubL = NULL;
	m_pPryoutSubT = NULL;
	m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
	
	for(int i=0; i<4; i++)
	{
		m_nRatTypeFailPry[i] = 0;
	}
}

void CDgnSeisAnchor2Pryout::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchor2Pryout)
	DDX_Check  (pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_USER_CHK,    m_bUser);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB,         m_tabPryout);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchor2Pryout::PostNcDestroy() 
{
	CChildDialog::PostNcDestroy();
}

void CDgnSeisAnchor2Pryout::SetData(T_ANEV_D& Data, int nRatType[4])
{
	m_Data = Data;
	m_bUser = m_Data.Fail.bUser[1];
	if(!m_bUser)
	{
		m_bNeedReCalc[0] = m_bNeedReCalc[1] = TRUE;
	}

	for (int i = 0; i < 4; i++)
	{
		m_nRatTypeFailPry[i] = nRatType[i];
	}
}

void CDgnSeisAnchor2Pryout::GetData(T_ANEV_D& Data)
{
	Data = m_Data;
}

void CDgnSeisAnchor2Pryout::Data2Dlg() 
{ 
	m_bUser = m_Data.Fail.bUser[1];

	if(!m_bUser) SetRstData();

	int nCurTab = m_tabPryout.GetCurSel();
	if(nCurTab == 0) 
	{
		m_pPryoutSubL->SetSubData(0, m_Data, m_nRatTypeFailPry);
		m_pPryoutSubL->Data2Dlg();		
	}
	else
	{
		m_pPryoutSubT->SetSubData(1, m_Data, m_nRatTypeFailPry);
		m_pPryoutSubT->Data2Dlg();
	}

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchor2Pryout::Dlg2Data(BOOL bOKBtn/*=FALSE*/) 
{
	UpdateData(TRUE);

	int nCurTab = m_tabPryout.GetCurSel();
	if(nCurTab == 0) 
	{
		if(!m_pPryoutSubL->Dlg2Data(bOKBtn)) return FALSE;
		m_pPryoutSubL->GetSubData(m_Data);
	}
	else
	{
		if(!m_pPryoutSubT->Dlg2Data(bOKBtn)) return FALSE;
		m_pPryoutSubT->GetSubData(m_Data);
	}

	m_Data.Fail.bUser[1] = m_bUser;
		
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchor2Pryout, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_PRYOUT_USER_CHK,         OnDgnSeisAnchor2PryoutUesrChk)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB, OnDgnSeisAnchor2PryoutChangingTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_SEIS_ANCHOR_PRYOUT_TAB, OnDgnSeisAnchor2PryoutChangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2Pryout message handlers

BOOL CDgnSeisAnchor2Pryout::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SEIS_ANCHOR_PRYOUT_PLACEHOLDER);

	m_tabPryout.SetPlaceHolder(pPlaceHolder);
	m_tabPryout.DeleteAllItems();

	// 교축방향
	m_pPryoutSubL = new CDgnSeisAnchor2PryoutSub(this, m_bPrintCF);
	m_pPryoutSubL->SetSubData(0, m_Data, m_nRatTypeFailPry);
	m_tabPryout.AddTab(m_pPryoutSubL, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_PRYOUT_L), CDgnSeisAnchor2PryoutSub::IDD, TRUE);  

	// 교축직각방향
	m_pPryoutSubT = new CDgnSeisAnchor2PryoutSub(this, m_bPrintCF);
	m_pPryoutSubT->SetSubData(1, m_Data, m_nRatTypeFailPry);
	m_tabPryout.AddTab(m_pPryoutSubT, _LS(IDS_DGN_SEIS_ANCHOR_FAIL_PRYOUT_T), CDgnSeisAnchor2PryoutSub::IDD, TRUE);  

	m_tabPryout.ShowTab(0);

	Data2Dlg();
	OnDgnSeisAnchor2PryoutUesrChk();	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchor2Pryout::SetRstData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_BRPR_D BrprD;
	std::vector<std::vector<T_BRPR_D>> aArBrprD;
	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(m_Data.BearingProp.aBrprKs[i].aBrprK[j]>0)
			{
				if (!pDoc->m_pAttrCtrl2->GetBrpr(m_Data.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}
		}
	}
	CDgnSeisAnchorShapeMgr ShapeMgr;
	ShapeMgr.SetData(aArBrprD, m_Data);
	ShapeMgr.SetDULR(m_nRatTypeFailPry[0], m_nRatTypeFailPry[1]);
	ShapeMgr.SetPryoutDULR(m_nRatTypeFailPry[2], m_nRatTypeFailPry[3]);

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

void CDgnSeisAnchor2Pryout::MakeEmfFile(CString strPathForEmf, int nDir)
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

void CDgnSeisAnchor2Pryout::SetViewData()
{
	int nCurTab = m_tabPryout.GetCurSel();
	if (nCurTab == 0)
	{
		m_pPryoutSubL->EnableDisableControls();
		m_pPryoutSubL->SetSubData(0, m_Data, m_nRatTypeFailPry);
		m_pPryoutSubL->SetViewData();
		m_bNeedReCalc[0] = FALSE;
	}
	else
	{
		m_pPryoutSubT->EnableDisableControls();
		m_pPryoutSubT->SetSubData(1, m_Data, m_nRatTypeFailPry);
		m_pPryoutSubT->SetViewData();
		m_bNeedReCalc[1] = FALSE;
	}
}

void CDgnSeisAnchor2Pryout::OnDgnSeisAnchor2PryoutUesrChk() 
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
			m_pPryoutSubL->SetSubData(0, m_Data, m_nRatTypeFailPry);
		}
		if(m_pPryoutSubT->m_hWnd != NULL) 
		{
			m_pPryoutSubT->SetSubData(1, m_Data, m_nRatTypeFailPry);
		}
	}

	SetViewData();
}

void CDgnSeisAnchor2Pryout::OnDgnSeisAnchor2PryoutChangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Dlg2Data()) return;

	//Data2Dlg();	
	*pResult = 0;
}

void CDgnSeisAnchor2Pryout::OnDgnSeisAnchor2PryoutChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//if(!Dlg2Data()) return;

	Data2Dlg();

	int nCurTab = m_tabPryout.GetCurSel();
	if(m_bNeedReCalc[nCurTab])
	{
		OnDgnSeisAnchor2PryoutUesrChk();
	}
	*pResult = 0;
}

