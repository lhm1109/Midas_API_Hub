// CDgnSeisAnchor2PryoutSub.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchor2PryoutSub.h"

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
// CDgnSeisAnchor2PryoutSub dialog

CDgnSeisAnchor2PryoutSub::CDgnSeisAnchor2PryoutSub(CWnd* pParent /*=NULL*/, BOOL bPrint/*=FALSE*/)
	: CChildDialog(CDgnSeisAnchor2PryoutSub::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchor2PryoutSub)	
	//}}AFX_DATA_INIT

	m_nDir = 0;
	m_Data.Fail.ConcFailL.Initialize();
	m_pGrid = new CDgnSeisAnchor2PryoutSubGrid(this);
	m_bPrintCFsub = bPrint;

	for (int i = 0; i < 4; i++)
	{
		m_nRatTypeFailPrySub[i] = 0;
	}
}

void CDgnSeisAnchor2PryoutSub::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchor2PryoutSub)
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
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_CONC_FAIL_VIEW2, m_View2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR2_PRYOUT_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchor2PryoutSub::PostNcDestroy() 
{
	CChildDialog::PostNcDestroy();
}

void CDgnSeisAnchor2PryoutSub::SetSubData(int nDir, T_ANEV_D& Data, int nRatType[4])
{
	m_nDir = nDir;
	m_Data = Data;

	for (int i = 0; i < 4; i++)
	{
		m_nRatTypeFailPrySub[i] = nRatType[i];
	}
}

void CDgnSeisAnchor2PryoutSub::GetSubData(T_ANEV_D& Data)
{
	Data = m_Data;
}

void CDgnSeisAnchor2PryoutSub::EnableDisableControls()
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SEIS_ANCHOR_PRYOUT_PARAM_GRP, m_Data.Fail.bUser[1], TRUE);
	m_pGrid->EnableDisableGrid(!m_Data.Fail.bUser[1]);

	GetDlgItem(IDC_DGN_SEIS_ANCHOR_PRYOUT_NUM_EDT)->EnableWindow(FALSE); // 무조건
}

T_ANEV_PRYOUT_D* CDgnSeisAnchor2PryoutSub::GetMyPryout()
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

void CDgnSeisAnchor2PryoutSub::Data2Dlg() 
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

	m_pGrid->SetData2Grid(pPryout, m_Data);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchor2PryoutSub::Dlg2Data(BOOL bOKBtn/*=FALSE*/) 
{
	UpdateData(TRUE);
	T_ANEV_PRYOUT_D* pPryout = GetMyPryout();

	if(m_Data.Fail.bUser[1])
	{
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

	if (m_Data.Fail.bUser[1])
		return TRUE;

	CArray<T_ANEV_FAIL_SEL_LAYER_D, T_ANEV_FAIL_SEL_LAYER_D> aFailSelLayerCopy;
	aFailSelLayerCopy.Copy(pPryout->aFailSelLayer);

	pPryout->aFailSelLayer.RemoveAll();
	m_pGrid->GetGrid2Data(pPryout);

	BOOL bChk = FALSE;
	for (int i = 0; i < pPryout->aFailSelLayer.GetSize(); i++)
	{
		for (int j = 0; j < pPryout->aFailSelLayer[i].aFailSel.GetSize(); j++)
		{
			if (pPryout->aFailSelLayer[i].aFailSel[j] == TRUE)
			{
				bChk = TRUE;
			}
		}

	}
	if (!bChk)
	{
		pPryout->aFailSelLayer.Copy(aFailSelLayerCopy);
		if (!m_bPrintCFsub)
		{
			if (bOKBtn)
			{
				AfxMessageBox(_LS(IDS_DB_DT_ANEV_ERROR_FAIL_SELECT_CHECK));
				return TRUE;
			}
		}
		return FALSE;
	}

	return TRUE;
}

void CDgnSeisAnchor2PryoutSub::InitView() 
{
	if (m_bPrintCFsub)
	{
		// 계산서 출력시 그림 방향 변환 위함.
		//m_View2.SubclassDlgItem(IDC_DGN_SEIS_ANCHOR_CONC_FAIL_VIEW2, this);
		m_View2.SetParent(this);
		m_View2.Init();
		m_View2.SetMode(EN_DRAW_WINMODE_SELECT);
	}
	else
	{
		//m_View.SubclassDlgItem(IDC_DGN_SEIS_ANCHOR_PRYOUT_VIEW, this);
		m_View.SetParent(this);
		m_View.Init();
		m_View.SetMode(EN_DRAW_WINMODE_SELECT);
	}

	SetViewData();
}

void CDgnSeisAnchor2PryoutSub::SetViewData() 
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

	if (m_bPrintCFsub)
	{
		m_View2.MakeDrawUnit(m_nRatTypeFailPrySub);
		m_View2.SetDrawType(2, m_nDir, !m_bPrintCFsub);
		m_View2.SetData(aArBrprD, m_Data, m_Data.Fail.bUser[1]);
	}
	else
	{
		m_View.MakeDrawUnit(m_nRatTypeFailPrySub);
		m_View.SetDrawType(2, m_nDir, !m_bPrintCFsub);
		m_View.SetData(aArBrprD, m_Data, m_Data.Fail.bUser[1]);
	}
}

void CDgnSeisAnchor2PryoutSub::MakeEmfFile(CString strPathForEmf)
{
	CRect Rect;
	if (m_nRatTypeFailPrySub)
	{
		m_View2.GetWindowRect(Rect);
		m_View2.MakeEmfFile(strPathForEmf, Rect.Width() * 100, Rect.Height() * 100);
	}
	else
	{
		m_View.GetWindowRect(Rect);
		m_View.MakeEmfFile(strPathForEmf, Rect.Width() * 100, Rect.Height() * 100);
	}
}

void CDgnSeisAnchor2PryoutSub::InitUnit() 
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

BEGIN_MESSAGE_MAP(CDgnSeisAnchor2PryoutSub, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2PryoutSub message handlers

BOOL CDgnSeisAnchor2PryoutSub::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_pGrid->Initialize();

	InitUnit();
	InitView();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}