// CDgnSeisAnchorPryoutSub.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorPryoutSub.h"

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
// CDgnSeisAnchorPryoutSub dialog

CDgnSeisAnchorPryoutSub::CDgnSeisAnchorPryoutSub(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorPryoutSub::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorPryoutSub)	
	//}}AFX_DATA_INIT

	m_nDir = 0;
	m_Data.Fail.ConcFailL.Initialize();
}

void CDgnSeisAnchorPryoutSub::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorPryoutSub)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_NUM_EDT,   m_edtNum);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_ANC_EDT,   m_edtANc);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_ANC_UNT,   m_untANc);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_ANCO_EDT,  m_edtANco);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_ANCO_UNT,  m_untANco);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_HEF_EDT,   m_edtHef);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_HEF_UNT,   m_untHef);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA1_EDT,   m_edtCa1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA1_UNT,   m_untCa1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA1_Q_EDT, m_edtCa1_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA1_Q_UNT, m_untCa1_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA2_EDT,   m_edtCa2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA2_UNT,   m_untCa2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA2_Q_EDT, m_edtCa2_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_CA2_Q_UNT, m_untCa2_q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PRYOUT_VIEW, m_View);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorPryoutSub::PostNcDestroy() 
{
	CChildDialog::PostNcDestroy();
}

void CDgnSeisAnchorPryoutSub::SetSubData(int nDir, T_ANEV_D& Data)
{
	m_nDir = nDir;
	m_Data = Data;
}

void CDgnSeisAnchorPryoutSub::GetSubData(T_ANEV_D& Data)
{
	Data = m_Data;
}

void CDgnSeisAnchorPryoutSub::EnableDisableControls()
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SEIS_ANCHOR_PRYOUT_PARAM_GRP, m_Data.Fail.bUser[1], TRUE);
}

T_ANEV_PRYOUT_D* CDgnSeisAnchorPryoutSub::GetMyPryout()
{
	if(m_nDir == 0)
	{
		return &m_Data.Fail.PryoutL;
	}
	else if(m_nDir == 1)
	{
		return &m_Data.Fail.PryoutT;
	}
	else
	{
		ASSERT(0);
		return &m_Data.Fail.PryoutL;
	}	
}

void CDgnSeisAnchorPryoutSub::Data2Dlg() 
{ 
	T_ANEV_PRYOUT_D* pPryout = GetMyPryout();

	m_edtNum.SetEditUnit(pPryout->nNum);
	m_edtANc.SetEditUnit(pPryout->dANc);
	m_edtANco.SetEditUnit(pPryout->dANco);
	m_edtHef.SetEditUnit(pPryout->dHef);
	m_edtCa1.SetEditUnit(pPryout->dCa1);
	m_edtCa1_q.SetEditUnit(pPryout->dCa1_q);
	m_edtCa2.SetEditUnit(pPryout->dCa2);
	m_edtCa2_q.SetEditUnit(pPryout->dCa2_q);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorPryoutSub::Dlg2Data() 
{
	UpdateData(TRUE);

	if(m_Data.Fail.bUser[1])
	{
		T_ANEV_PRYOUT_D* pPryout = GetMyPryout();
		pPryout->nNum = m_edtNum.GetEditValue();
		pPryout->dANc = m_edtANc.GetEditValue();
		pPryout->dANco = m_edtANco.GetEditValue();
		pPryout->dHef = m_edtHef.GetEditValue();
		pPryout->dCa1 = m_edtCa1.GetEditValue();
		pPryout->dCa1_q = m_edtCa1_q.GetEditValue();
		pPryout->dCa2 = m_edtCa2.GetEditValue();
		pPryout->dCa2_q = m_edtCa2_q.GetEditValue();

		if (pPryout->nNum   <= 0)   { AfxMessageBox(_T("[Error] 저항앵커개수 > 0")); return FALSE; }
		if (pPryout->dANc   <= 0.0) { AfxMessageBox(_T("[Error] ANc > 0")); return FALSE; }
		if (pPryout->dANco  <= 0.0) { AfxMessageBox(_T("[Error] ANco > 0")); return FALSE; }
		if (pPryout->dHef   <= 0.0) { AfxMessageBox(_T("[Error] Hef > 0")); return FALSE; }
		if (pPryout->dCa1   <= 0.0) { AfxMessageBox(_T("[Error] Ca1 > 0")); return FALSE; }
		if (pPryout->dCa1_q <= 0.0) { AfxMessageBox(_T("[Error] Ca'1 > 0")); return FALSE; }
		if (pPryout->dCa2   <= 0.0) { AfxMessageBox(_T("[Error] Ca2 > 0")); return FALSE; }
		if (pPryout->dCa2_q <= 0.0) { AfxMessageBox(_T("[Error] Ca'2 > 0")); return FALSE; }

	}

	return TRUE;
}

void CDgnSeisAnchorPryoutSub::InitView() 
{
	m_View.SetParent(this);
	m_View.Init();
	m_View.SetMode(EN_DRAW_WINMODE_SELECT);
	int nRatType[4] = { 0 };
	m_View.MakeDrawUnit(nRatType);

	SetViewData();
}

void CDgnSeisAnchorPryoutSub::SetViewData() 
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

	m_View.SetDrawType(2, m_nDir, FALSE);
	m_View.SetData(aArBrprD, m_Data, m_Data.Fail.bUser[1]);
}

void CDgnSeisAnchorPryoutSub::MakeEmfFile(CString strPathForEmf)
{
	CRect Rect;
	m_View.GetWindowRect(Rect);
	m_View.MakeEmfFile(strPathForEmf, Rect.Width() * 100, Rect.Height() * 100);
}

void CDgnSeisAnchorPryoutSub::InitUnit() 
{
	m_edtNum.SetUnitType(D_UNITSYS_NONE);
	m_edtANc.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dANc);
	m_untANc.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dANc);
	m_edtANco.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dANco);
	m_untANco.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dANco);
	m_edtHef.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHef);
	m_untHef.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHef);
	m_edtCa1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1);
	m_untCa1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1);
	m_edtCa1_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1_q);
	m_untCa1_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa1_q);
	m_edtCa2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2);
	m_untCa2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2);
	m_edtCa2_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2_q);
	m_untCa2_q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCa2_q);
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorPryoutSub, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorPryoutSub message handlers

BOOL CDgnSeisAnchorPryoutSub::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitUnit();
	InitView();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}