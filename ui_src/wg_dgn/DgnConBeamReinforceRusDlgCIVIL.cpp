// CDgnConBeamReinforceRusDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamReinforceRusDlgCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"

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

#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndRusCIVIL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceRusDlgCIVIL dialog


CDgnConBeamReinforceRusDlgCIVIL::CDgnConBeamReinforceRusDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDgnConBeamReinforceCommonDlgCIVIL(CDgnConBeamReinforceRusDlgCIVIL::IDD, pParent)
{
	m_TopGrid = NULL;
	m_BotGrid = NULL;
}


void CDgnConBeamReinforceRusDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDgnConBeamReinforceCommonDlgCIVIL::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamReinforceRusDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_RC_TOP_LAYER_GRID, *m_TopGrid);
	DDX_Control(pDX, IDC_DGN_RC_BOT_LAYER_GRID, *m_BotGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBeamReinforceRusDlgCIVIL, CDgnConBeamReinforceCommonDlgCIVIL)
	//{{AFX_MSG_MAP(CDgnConBeamReinforceRusDlgCIVIL)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceRusDlgCIVIL message handlers


BOOL CDgnConBeamReinforceRusDlgCIVIL::OnInitDialog() 
{
	if (m_TopGrid == nullptr)
		m_TopGrid = new CDgnBeamGridWndRusCIVIL(m_pBeamData);
	if (m_BotGrid == nullptr)
		m_BotGrid = new CDgnBeamGridWndRusCIVIL(m_pBeamData);

	CDgnConBeamReinforceCommonDlgCIVIL::OnInitDialog();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBeamReinforceRusDlgCIVIL::PostNcDestroy() 
{
	if(m_TopGrid != NULL){ delete m_TopGrid; m_TopGrid=NULL; }
	if(m_BotGrid != NULL){ delete m_BotGrid; m_BotGrid=NULL; }
	
	CDgnConBeamReinforceCommonDlgCIVIL::PostNcDestroy();
}

void CDgnConBeamReinforceRusDlgCIVIL::SetLayerNumToGrid( int nTopBot, int nLayerNum )
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

BOOL CDgnConBeamReinforceRusDlgCIVIL::CheckReDraw_RebarGrid( _DGNC_RBAR& RebarData, int& iChkResult )
{
	CString str;
	// Top
	for(int i=0; i<m_nTopLayer; i++)
	{
		str = m_TopGrid->GetValueRowCol(i+2, 1);
		RebarData.iBarNum[0][i] = _ttoi(str);
		if(RebarData.iBarNum[0][i] <= 0)
			iChkResult |= 2;
		
		str = m_TopGrid->GetValueRowCol(i+2, 4);
		RebarData.dDt[i] = _tstof(str);
		double CL = 0;
		if(i == 0) CL = 0;
		else       CL = RebarData.dDt[i-1];
		if(RebarData.dDt[i] <= CL)
			iChkResult |= 4;
		str = m_TopGrid->GetValueRowCol(i+2, 2);
		RebarData.strBarNa1[0][i] = str;
		str = m_TopGrid->GetValueRowCol(i+2, 3);
		RebarData.strBarNa2[0][i] = str;

		// Group에 대한 Error Check (iChkResult를 이용한 Error Message전달은 가독성이 안좋아서 여기서 바로 Error뱉어 버리자..)
		str = m_TopGrid->GetValueRowCol(i+2, 5); // Group App.
		if(str != _T("0") && str != _T("1"))
		{
			CString szTemp;
			szTemp.Format(_LS(IDS_DGN_STR_IS_INVALID),_LS(IDS_DGN_APP_DOT));
			AfxMessageBox(szTemp);
			return FALSE;
		}
		if(str == _T("1"))
		{
			str = m_TopGrid->GetValueRowCol(i+2, 6);  // Group Num.
			if(_ttoi(str) <= 1)
			{
				str.Format(_LS(IDS_DGN_STR_IS_INVALID),_LS(IDS_DGN_NUM_DOT));
				AfxMessageBox(str);
				return FALSE;
			}
			str = m_TopGrid->GetValueRowCol(i+2, 7);  // Group Gap
			if(_tstof(str) < 0.0)
			{
				str.Format(_LS(IDS_DGN_STR_IS_INVALID),_LS(IDS_DGN_GAP));
				AfxMessageBox(str);
				return FALSE;
			}
		}
	}
	
	// Bottom
	for(int i=0; i<m_nBotLayer ; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+2, 1);
		RebarData.iBarNum[1][i] = _ttoi(str);
		if(RebarData.iBarNum[1][i] <= 0)
			iChkResult |= 8;
		str = m_BotGrid->GetValueRowCol(i+2, 4);
		RebarData.dDb[i] = _tstof(str);
		double CL = 0;
		if(i == 0) CL = 0;
		else       CL = RebarData.dDb[i-1];
		if(RebarData.dDb[i] <= CL)
			iChkResult |= 16;
		str = m_BotGrid->GetValueRowCol(i+2, 2);
		RebarData.strBarNa1[1][i] = str;
		str = m_BotGrid->GetValueRowCol(i+2, 3);
		RebarData.strBarNa2[1][i] = str;

		// Group에 대한 Error Check (iChkResult를 이용한 Error Message전달은 가독성이 안좋아서 여기서 바로 Error뱉어 버리자..)
		str = m_BotGrid->GetValueRowCol(i+2, 5); // Group App.
		if(str != _T("0") && str != _T("1"))
		{
			CString szTemp;
			szTemp.Format(_LS(IDS_DGN_STR_IS_INVALID),_LS(IDS_DGN_APP_DOT));
			AfxMessageBox(szTemp);
			return FALSE;
		}
		if(str == _T("1"))
		{
			str = m_BotGrid->GetValueRowCol(i+2, 6);  // Group Num.
			if(_ttoi(str) <= 1)
			{
				str.Format(_LS(IDS_DGN_STR_IS_INVALID),_LS(IDS_DGN_NUM_DOT));
				AfxMessageBox(str);
				return FALSE;
			}
			str = m_BotGrid->GetValueRowCol(i+2, 7);  // Group Gap
			if(_tstof(str) < 0.0)
			{
				str.Format(_LS(IDS_DGN_STR_IS_INVALID),_LS(IDS_DGN_GAP));
				AfxMessageBox(str);
				return FALSE;
			}
		}
	 }

	return TRUE;
}

void CDgnConBeamReinforceRusDlgCIVIL::SetDataToGrid()
{
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		// top Rebar
		m_TopGrid->SetValueRange(CGXRange(i+2,1), double(m_pBeamData->iTopBarNum[m_nIMJ][i]));
		m_TopGrid->SetValueRange(CGXRange(i+2,2), m_pBeamData->strTopBarNa1[m_nIMJ][i]);
		m_TopGrid->SetValueRange(CGXRange(i+2,3), m_pBeamData->strTopBarNa2[m_nIMJ][i]);
		m_TopGrid->SetValueRange(CGXRange(i+2,4), m_pBeamData->dDt[m_nIMJ][i]);		
		m_TopGrid->SetValueRange(CGXRange(i+2,5), m_pBeamData->bTopGroup[m_nIMJ][i] ? _T("1") : _T("0"));
		m_TopGrid->SetValueRange(CGXRange(i+2,6), double(m_pBeamData->nTopGroupNum[m_nIMJ][i]));
		m_TopGrid->SetValueRange(CGXRange(i+2,7), m_pBeamData->dTopGroupGap[m_nIMJ][i]);

		// Bot Rebar
		m_BotGrid->SetValueRange(CGXRange(i+2,1), double(m_pBeamData->iBotBarNum[m_nIMJ][i]));
		m_BotGrid->SetValueRange(CGXRange(i+2,2), m_pBeamData->strBotBarNa1[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+2,3), m_pBeamData->strBotBarNa2[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+2,4), m_pBeamData->dDb[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+2,5), m_pBeamData->bBotGroup[m_nIMJ][i] ? _T("1") : _T("0"));
		m_BotGrid->SetValueRange(CGXRange(i+2,6), double(m_pBeamData->nBotGroupNum[m_nIMJ][i]));
		m_BotGrid->SetValueRange(CGXRange(i+2,7), m_pBeamData->dBotGroupGap[m_nIMJ][i]);
	}
}

void CDgnConBeamReinforceRusDlgCIVIL::InitGridData()
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

void CDgnConBeamReinforceRusDlgCIVIL::SetDataToGrid_TabCtrl()
{
 for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
 {
		// top Rebar
		m_TopGrid->SetValueRange(CGXRange(i+2,1), double(m_TabBeamData.iTopBarNum[m_nIMJ][i]));
		m_TopGrid->SetValueRange(CGXRange(i+2,2), m_TabBeamData.strTopBarNa1[m_nIMJ][i]);
		m_TopGrid->SetValueRange(CGXRange(i+2,3), m_TabBeamData.strTopBarNa2[m_nIMJ][i]);
		m_TopGrid->SetValueRange(CGXRange(i+2,4), m_TabBeamData.dDt[m_nIMJ][i]);
		m_TopGrid->SetValueRange(CGXRange(i+2,5), m_TabBeamData.bTopGroup[m_nIMJ][i] ? _T("1") : _T("0"));
		m_TopGrid->SetValueRange(CGXRange(i+2,6), double(m_TabBeamData.nTopGroupNum[m_nIMJ][i]));
		m_TopGrid->SetValueRange(CGXRange(i+2,7), m_TabBeamData.dTopGroupGap[m_nIMJ][i]);	 

		// Bot Rebar
		m_BotGrid->SetValueRange(CGXRange(i+2,1), double(m_TabBeamData.iBotBarNum[m_nIMJ][i]));
		m_BotGrid->SetValueRange(CGXRange(i+2,2), m_TabBeamData.strBotBarNa1[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+2,3), m_TabBeamData.strBotBarNa2[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+2,4), m_TabBeamData.dDb[m_nIMJ][i]);
		m_BotGrid->SetValueRange(CGXRange(i+2,5), m_TabBeamData.bBotGroup[m_nIMJ][i] ? _T("1") : _T("0"));
		m_BotGrid->SetValueRange(CGXRange(i+2,6), double(m_TabBeamData.nBotGroupNum[m_nIMJ][i]));
		m_BotGrid->SetValueRange(CGXRange(i+2,7), m_TabBeamData.dBotGroupGap[m_nIMJ][i]);
	}  
}

void CDgnConBeamReinforceRusDlgCIVIL::GetDataFromGrid_TabCtrl( int iIMJ )
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
		str = m_TopGrid->GetValueRowCol(i+2, 1);
		m_TabBeamData.iTopBarNum[iIMJ][i] = _ttoi(str);
		
		str = m_TopGrid->GetValueRowCol(i+2, 2);
		m_TabBeamData.strTopBarNa1[iIMJ][i] = str;
		
		str = m_TopGrid->GetValueRowCol(i+2, 3);
		m_TabBeamData.strTopBarNa2[iIMJ][i] = str;
		
		str = m_TopGrid->GetValueRowCol(i+2, 4);
		m_TabBeamData.dDt[iIMJ][i] = _tstof(str);
	
		str = m_TopGrid->GetValueRowCol(i+2, 5);
		m_TabBeamData.bTopGroup[iIMJ][i] = str == _T("1") ? TRUE : FALSE;
		
		str = m_TopGrid->GetValueRowCol(i+2, 6);
		m_TabBeamData.nTopGroupNum[iIMJ][i] = _ttoi(str);
			
		str = m_TopGrid->GetValueRowCol(i+2, 7);
		m_TabBeamData.dTopGroupGap[iIMJ][i] = _tstof(str);
	}
	
	// Bot Rebar
	nIndex = m_cBotLayerCB.GetCurSel();
	m_cBotLayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);
	m_nBPosLayer[iIMJ] = nLayer; 
	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+2, 1);
		m_TabBeamData.iBotBarNum[iIMJ][i] = _ttoi(str);
		
		str = m_BotGrid->GetValueRowCol(i+2, 2);
		m_TabBeamData.strBotBarNa1[iIMJ][i] = str;
		
		str = m_BotGrid->GetValueRowCol(i+2, 3);
		m_TabBeamData.strBotBarNa2[iIMJ][i] = str;
		
		str = m_BotGrid->GetValueRowCol(i+2, 4);
		m_TabBeamData.dDb[iIMJ][i] = _tstof(str);

		str = m_BotGrid->GetValueRowCol(i+2, 5);
		m_TabBeamData.bBotGroup[iIMJ][i] = str == _T("1") ? TRUE : FALSE;
		
		str = m_BotGrid->GetValueRowCol(i+2, 6);
		m_TabBeamData.nBotGroupNum[iIMJ][i] = _ttoi(str);
			
		str = m_BotGrid->GetValueRowCol(i+2, 7);
		m_TabBeamData.dBotGroupGap[iIMJ][i] = _tstof(str);		
	}  
}
