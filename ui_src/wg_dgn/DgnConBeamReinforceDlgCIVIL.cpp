// CDgnConBeamReinforceDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamReinforceDlgCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndCIVIL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceDlgCIVIL dialog


CDgnConBeamReinforceDlgCIVIL::CDgnConBeamReinforceDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDgnConBeamReinforceCommonDlgCIVIL(CDgnConBeamReinforceDlgCIVIL::IDD, pParent)
{
	m_TopGrid = new CDgnBeamGridWndCIVIL(m_pBeamData);
	m_BotGrid = new CDgnBeamGridWndCIVIL(m_pBeamData);

	m_aCrackCtrl.RemoveAll();
	m_aCrackCtrl.Add(IDC_DGN_CONC_CrackFrm);
	m_aCrackCtrl.Add(IDC_DGN_CON_BEAM_CRACK1);
	m_aCrackCtrl.Add(IDC_DGN_CON_BEAM_CRACK2);
	m_aCrackCtrl.Add(IDC_DGN_CON_BEAM_CRACK3);
	m_aCrackCtrl.Add(IDC_DGN_CON_BEAM_CRACK4);
}


void CDgnConBeamReinforceDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDgnConBeamReinforceCommonDlgCIVIL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamReinforceDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_RC_TOP_LAYER_GRID, *m_TopGrid);
	DDX_Control(pDX, IDC_DGN_RC_BOT_LAYER_GRID, *m_BotGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBeamReinforceDlgCIVIL, CDgnConBeamReinforceCommonDlgCIVIL)
	//{{AFX_MSG_MAP(CDgnConBeamReinforceDlgCIVIL)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceDlgCIVIL message handlers


BOOL CDgnConBeamReinforceDlgCIVIL::OnInitDialog() 
{
	CDgnConBeamReinforceCommonDlgCIVIL::OnInitDialog();
	
	ShowHideCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBeamReinforceDlgCIVIL::PostNcDestroy() 
{
	if(m_TopGrid != NULL){ delete m_TopGrid; m_TopGrid=NULL; }
	if(m_BotGrid != NULL){ delete m_BotGrid; m_BotGrid=NULL; }
	
	CDgnConBeamReinforceCommonDlgCIVIL::PostNcDestroy();
}

void CDgnConBeamReinforceDlgCIVIL::SetLayerNumToGrid( int nTopBot, int nLayerNum )
{
	if(nTopBot == 0)
	{
		m_TopGrid->SetLayerNum(nLayerNum);  
	}
	else if(nTopBot == 1)
	{
		m_BotGrid->SetLayerNum(nLayerNum); 
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CDgnConBeamReinforceDlgCIVIL::CheckReDraw_RebarGrid( _DGNC_RBAR& RebarData, int& iChkResult )
{
	CString str;
	// Top
	for(int i=0; i<m_nTopLayer; i++)
	{
		str = m_TopGrid->GetValueRowCol(i+1, 1);
		RebarData.iBarNum[0][i] = _ttoi(str);
		if(RebarData.iBarNum[0][i] <= 0)
			iChkResult |= 2;
		
		str = m_TopGrid->GetValueRowCol(i+1, 4);
		RebarData.dDt[i] = _tstof(str);
		double CL = 0;
		if(i == 0) CL = 0;
		else       CL = RebarData.dDt[i-1];
		if(RebarData.dDt[i] <= CL)
			iChkResult |= 4;
		str = m_TopGrid->GetValueRowCol(i+1, 2);
		RebarData.strBarNa1[0][i] = str;
		str = m_TopGrid->GetValueRowCol(i+1, 3);
		RebarData.strBarNa2[0][i] = str;
	}
	
	// Bottom
	for(int i=0; i<m_nBotLayer ; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+1, 1);
		RebarData.iBarNum[1][i] = _ttoi(str);
		if(RebarData.iBarNum[1][i] <= 0)
			iChkResult |= 8;
		str = m_BotGrid->GetValueRowCol(i+1, 4);
		RebarData.dDb[i] = _tstof(str);
		double CL = 0;
		if(i == 0) CL = 0;
		else       CL = RebarData.dDb[i-1];
		if(RebarData.dDb[i] <= CL)
			iChkResult |= 16;
		str = m_BotGrid->GetValueRowCol(i+1, 2);
		RebarData.strBarNa1[1][i] = str;
		str = m_BotGrid->GetValueRowCol(i+1, 3);
		RebarData.strBarNa2[1][i] = str;
	}
	
	return TRUE;
}

void CDgnConBeamReinforceDlgCIVIL::SetDataToGrid()
{
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		// top Rebar
		m_TopGrid->SetValueRange(CGXRange(i+1,1), double(m_pBeamData->iTopBarNum[m_nIMJ][i]));
		m_TopGrid->SetValueRange(CGXRange(i+1,2), m_pBeamData->strTopBarNa1[m_nIMJ][i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,3), m_pBeamData->strTopBarNa2[m_nIMJ][i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,4), m_pBeamData->dDt[m_nIMJ][i]);
		
		// Bot Rebar
		m_BotGrid->SetValueRange(CGXRange(i+1,1), double(m_pBeamData->iBotBarNum[m_nIMJ][i]));
		m_BotGrid->SetValueRange(CGXRange(i+1,2), m_pBeamData->strBotBarNa1[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,3), m_pBeamData->strBotBarNa2[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,4), m_pBeamData->dDb[m_nIMJ][i]);
	}
}

void CDgnConBeamReinforceDlgCIVIL::InitGridData()
{
	 m_nIMJ = 0;
	 m_RchkData.BEAM.Initialize();
	 m_pBeamData = &(m_RchkData.BEAM);
	 for(int i=0; i<3; i++)
	 {
		 m_nTPosLayer[i] = 1;
		 m_nBPosLayer[i] = 1;
	 }
	 
	 m_TopGrid->SetTopBottom(0);
	 m_TopGrid->Initialize();
	 m_TopGrid->SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced );
	 m_TopGrid->SetScrollBarMode(SB_HORZ, gxnDisabled);
	 
	 m_BotGrid->SetTopBottom(1);
	 m_BotGrid->Initialize();
	 m_BotGrid->SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced );
	 m_BotGrid->SetScrollBarMode(SB_HORZ, gxnDisabled);
	 
	 UINT uInitLayer = 1;
	 m_TopGrid->SetLayerNum(uInitLayer);
	m_BotGrid->SetLayerNum(uInitLayer);
}

void CDgnConBeamReinforceDlgCIVIL::SetDataToGrid_TabCtrl()
{
 for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
 {
	 // top Rebar
	 m_TopGrid->SetValueRange(CGXRange(i+1,1), double(m_TabBeamData.iTopBarNum[m_nIMJ][i]));
	 m_TopGrid->SetValueRange(CGXRange(i+1,2), m_TabBeamData.strTopBarNa1[m_nIMJ][i]);
	 m_TopGrid->SetValueRange(CGXRange(i+1,3), m_TabBeamData.strTopBarNa2[m_nIMJ][i]);
	 m_TopGrid->SetValueRange(CGXRange(i+1,4), m_TabBeamData.dDt[m_nIMJ][i]);
	 
	 // Bot Rebar
	 m_BotGrid->SetValueRange(CGXRange(i+1,1), double(m_TabBeamData.iBotBarNum[m_nIMJ][i]));
	 m_BotGrid->SetValueRange(CGXRange(i+1,2), m_TabBeamData.strBotBarNa1[m_nIMJ][i]);
	 m_BotGrid->SetValueRange(CGXRange(i+1,3), m_TabBeamData.strBotBarNa2[m_nIMJ][i]);
	 m_BotGrid->SetValueRange(CGXRange(i+1,4), m_TabBeamData.dDb[m_nIMJ][i]);
	}  
}

void CDgnConBeamReinforceDlgCIVIL::GetDataFromGrid_TabCtrl( int iIMJ )
{
	CString str = _T("");
	CString strSel = _T("");
	int nIndex, nLayer;
	
	// Top Rebar
	nIndex = m_cTopLayerCB.GetCurSel();
	m_cTopLayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);
	m_nTPosLayer[iIMJ] = nLayer; 
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		str = m_TopGrid->GetValueRowCol(i+1, 1);
		m_TabBeamData.iTopBarNum[iIMJ][i] = _ttoi(str);
		
		str = m_TopGrid->GetValueRowCol(i+1, 2);
		m_TabBeamData.strTopBarNa1[iIMJ][i] = str;
		
		str = m_TopGrid->GetValueRowCol(i+1, 3);
		m_TabBeamData.strTopBarNa2[iIMJ][i] = str;
		
		str = m_TopGrid->GetValueRowCol(i+1, 4);
		m_TabBeamData.dDt[iIMJ][i] = _tstof(str);
	}
	
	// Bot Rebar
	nIndex = m_cBotLayerCB.GetCurSel();
	m_cBotLayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);
	m_nBPosLayer[iIMJ] = nLayer; 
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+1, 1);
		m_TabBeamData.iBotBarNum[iIMJ][i] = _ttoi(str);
		
		str = m_BotGrid->GetValueRowCol(i+1, 2);
		m_TabBeamData.strBotBarNa1[iIMJ][i] = str;
		
		str = m_BotGrid->GetValueRowCol(i+1, 3);
		m_TabBeamData.strBotBarNa2[iIMJ][i] = str;
		
		str = m_BotGrid->GetValueRowCol(i+1, 4);
		m_TabBeamData.dDb[iIMJ][i] = _tstof(str);
	}  
}

void CDgnConBeamReinforceDlgCIVIL::ShowHideCtrl()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CDgnDataCtrl DataCtrl;
	T_DCON_D DConD;
	if ( !DataCtrl.Get_DgnConDcon(DConD) )
	{
		DConD.Initialize();
	}

	bool bCrackShow = true;
	if (DConD.DesignCode == CONCODE_EC2_2_05 || DConD.DesignCode == CONCODE_BS5400_90 || 
		DConD.DesignCode == CONCODE_AREMA_2023 || DConD.DesignCode == CONCODE_TMH07_89 ||
		DConD.DesignCode == CONCODE_AS5100_5_17)
		bCrackShow = false;

	CDlgUtil::CtrlShowHide(this, m_aCrackCtrl, bCrackShow);

	if (m_pDoc->m_pAttrCtrl->IsDcon_CrackForAASHTO()) // AASHTO07 기준 이상
	{
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK4)->ShowWindow(SW_HIDE);
	}

}
