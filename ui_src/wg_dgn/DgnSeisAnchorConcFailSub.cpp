// CDgnSeisAnchorConcFailSub.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorConcFailSub.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorConcFailSub dialog

CDgnSeisAnchorConcFailSub::CDgnSeisAnchorConcFailSub(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorConcFailSub::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorConcFailSub)	
	//}}AFX_DATA_INIT

	m_nDir = 0;
	m_Data.Fail.ConcFailL.Initialize();
}

void CDgnSeisAnchorConcFailSub::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorConcFailSub)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_NUM_EDT,   m_edtNum);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_AVC_EDT,   m_edtAVc);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_AVC_UNT,   m_untAVc);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_AVCO_EDT,  m_edtAVco);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_AVCO_UNT,  m_untAVco);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_HA_EDT,    m_edtHa);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_HA_UNT,    m_untHa);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA1_EDT,   m_edtCa1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA1_UNT,   m_untCa1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA1_Q_EDT, m_edtCa1_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA1_Q_UNT, m_untCa1_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA2_EDT,   m_edtCa2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA2_UNT,   m_untCa2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA2_Q_EDT, m_edtCa2_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_CA2_Q_UNT, m_untCa2_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_VIEW, m_View);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorConcFailSub::PostNcDestroy() 
{
	CChildDialog::PostNcDestroy();
}

void CDgnSeisAnchorConcFailSub::SetSubData(int nDir, T_ANEV_D& Data)
{
	m_nDir = nDir;
	m_Data = Data;
}

void CDgnSeisAnchorConcFailSub::GetSubData(T_ANEV_D& Data)
{
	Data = m_Data;
}

void CDgnSeisAnchorConcFailSub::EnableDisableControls()
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_PARAM_GRP, m_Data.Fail.bUser[0], TRUE);	
}

T_ANEV_CONC_FAIL_D* CDgnSeisAnchorConcFailSub::GetMyConcFail()
{
	if(m_nDir == 0)
	{
		return &m_Data.Fail.ConcFailL;
	}
	else if(m_nDir == 1)
	{
		return &m_Data.Fail.ConcFailT;
	}
	else
	{
		ASSERT(0);
		return &m_Data.Fail.ConcFailL;
	}	
}

void CDgnSeisAnchorConcFailSub::Data2Dlg() 
{ 
	T_ANEV_CONC_FAIL_D* pConcFail = GetMyConcFail();	

	m_edtNum.SetEditUnit(pConcFail->nNum);
	m_edtAVc.SetEditUnit(pConcFail->dAVc);
	m_edtAVco.SetEditUnit(pConcFail->dAVco);
	m_edtHa.SetEditUnit(pConcFail->dHa);
	m_edtCa1.SetEditUnit(pConcFail->dCa1);
	m_edtCa1_q.SetEditUnit(pConcFail->dCa1_q);
	m_edtCa2.SetEditUnit(pConcFail->dCa2);
	m_edtCa2_q.SetEditUnit(pConcFail->dCa2_q);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorConcFailSub::Dlg2Data() 
{
	UpdateData(TRUE);
	T_ANEV_CONC_FAIL_D* pConcFail = GetMyConcFail();

	if(m_Data.Fail.bUser[0])
	{
		pConcFail->nNum = m_edtNum.GetEditValue();
		pConcFail->dAVc = m_edtAVc.GetEditValue();
		pConcFail->dAVco = m_edtAVco.GetEditValue();
		pConcFail->dHa = m_edtHa.GetEditValue();
		pConcFail->dCa1 = m_edtCa1.GetEditValue();
		pConcFail->dCa1_q = m_edtCa1_q.GetEditValue();
		pConcFail->dCa2 = m_edtCa2.GetEditValue();
		pConcFail->dCa2_q = m_edtCa2_q.GetEditValue();

		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		CString strText;
		if (pConcFail->nNum <= 0)     { strText.Format(_T("[주의] %s의 저항받침개수가 없습니다.   "), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
		if (pConcFail->dAVc <= 0.0)   { strText.Format(_T("[주의] %s의 AVc  값이 0보다 커야합니다."), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
		if (pConcFail->dAVco <= 0.0)  { strText.Format(_T("[주의] %s의 AVco 값이 0보다 커야합니다."), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
		if (pConcFail->dHa <= 0.0)    { strText.Format(_T("[주의] %s의 Ha   값이 0보다 커야합니다."), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
		if (pConcFail->dCa1 <= 0.0)   { strText.Format(_T("[주의] %s의 Ca1  값이 0보다 커야합니다."), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
		if (pConcFail->dCa1_q <= 0.0) { strText.Format(_T("[주의] %s의 Ca'1 값이 0보다 커야합니다."), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
		if (pConcFail->dCa2 <= 0.0)   { strText.Format(_T("[주의] %s의 Ca2  값이 0보다 커야합니다."), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
		if (pConcFail->dCa2_q <= 0.0) { strText.Format(_T("[주의] %s의 Ca'2 값이 0보다 커야합니다."), m_Data.strName); pDoc->DisplayHistoryMessage(strText); return FALSE; }
	}	

	return TRUE;
}

void CDgnSeisAnchorConcFailSub::MakeEmfFile(CString strPathForEmf)
{
	CRect Rect;
	m_View.GetWindowRect(Rect);
	m_View.MakeEmfFile(strPathForEmf, Rect.Width() * 100, Rect.Height() * 100);
}

void CDgnSeisAnchorConcFailSub::InitUnit() 
{
	m_edtNum.SetUnitType(D_UNITSYS_NONE);
	m_edtAVc.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dAVc);
	m_untAVc.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dAVc);
	m_edtAVco.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dAVco);
	m_untAVco.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dAVco);
	m_edtHa.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHa);
	m_untHa.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHa);
	m_edtCa1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1);
	m_untCa1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1);
	m_edtCa1_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1_q);
	m_untCa1_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1_q);
	m_edtCa2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2);
	m_untCa2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2);
	m_edtCa2_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2_q);
	m_untCa2_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2_q);
}

void CDgnSeisAnchorConcFailSub::InitView() 
{
	m_View.SetParent(this);
	m_View.Init();
	m_View.SetMode(EN_DRAW_WINMODE_SELECT);
	int nRatType[4] = { 0 };
	m_View.MakeDrawUnit(nRatType);

	SetViewData();
}

void CDgnSeisAnchorConcFailSub::SetViewData() 
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

	m_View.SetDrawType(1, m_nDir, FALSE);
	m_View.SetData(aArBrprD, m_Data, m_Data.Fail.bUser[0]);
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorConcFailSub, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorConcFailSub message handlers

BOOL CDgnSeisAnchorConcFailSub::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitUnit();
	InitView();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}