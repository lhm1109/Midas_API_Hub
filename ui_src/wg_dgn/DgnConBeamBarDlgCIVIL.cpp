// DgnConBeamBarDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamBarDlgCIVIL.h"
#include "DgnBeamGridWndCIVIL.h"
#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DgnConBeamBarDlgCIVIL dialog

DgnConBeamBarDlgCIVIL::DgnConBeamBarDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDialogMove(DgnConBeamBarDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(DgnConBeamBarDlgCIVIL)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nTopLayer = 0;
	m_nBotLayer = 0;

	m_TopGrid = NULL;
	m_BotGrid = NULL;
}

void DgnConBeamBarDlgCIVIL::SetRchkData(int nIMJ, T_RCHK_K RchkKey, CMap<int,int,T_RCHK_K,T_RCHK_K>& arRchkKey, T_RCHK_BEAM* pBeamData)
{
	m_nIMJ = nIMJ;
	m_RchkKey = RchkKey;
	m_pBeamData = NULL;
	m_pBeamData = pBeamData;


	int Index=0;
	T_RCHK_K RchkKey1;
	m_arRchkKey.RemoveAll();
	POSITION Pos = arRchkKey.GetStartPosition();
	while(Pos)
	{
		RchkKey1 = 0;
		arRchkKey.GetNextAssoc(Pos, Index, RchkKey1);
		m_arRchkKey.SetAt(Index,RchkKey1);
	}
}

void DgnConBeamBarDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DgnConBeamBarDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_RC_As_Top_UNIT, m_AsTopUT);
	DDX_Control(pDX, IDC_DGN_RC_As_Top, m_AsTop);
	DDX_Control(pDX, IDC_DGN_RC_As_Bot_Unit, m_AsBotUT);
	DDX_Control(pDX, IDC_DGN_RC_As_Bot, m_AsBot);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_SPACE_UNIT, m_cStirrupSpaceUT);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_SPACE, m_cStirrupSpace);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_SIZE, m_cStirrupSize);
	DDX_Control(pDX, IDC_DGN_RC_TOP_LAYER_CB, m_cTopLayerCB);
	DDX_Control(pDX, IDC_DGN_RC_BOT_LAYER_CB, m_cBotLayerCB);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_NUMBER, m_cStirrupNum);

	DDX_Control(pDX, IDC_DGN_RC_TOP_LAYER_GRID, *m_TopGrid);
	DDX_Control(pDX, IDC_DGN_RC_BOT_LAYER_GRID, *m_BotGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DgnConBeamBarDlgCIVIL, CDialogMove)
	//{{AFX_MSG_MAP(DgnConBeamBarDlgCIVIL)
	ON_CBN_SELCHANGE(IDC_DGN_RC_BOT_LAYER_CB, OnSelchangeDgnRcBotLayerCb)
	ON_CBN_SELCHANGE(IDC_DGN_RC_TOP_LAYER_CB, OnSelchangeDgnRcTopLayerCb)
	ON_BN_CLICKED(IDC_REDRAW_BTN, OnRedrawBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Initialize 관련 
//

//----------------------------------------------------------------------------
// Initialize Layer CB
//----------------------------------------------------------------------------
void DgnConBeamBarDlgCIVIL::InitLayerCB()
{
	CString str;
	m_cTopLayerCB.ResetContent();
	m_cBotLayerCB.ResetContent();

	for(int i=0; i<5; i++)
	{
		str.Format(_T("%d"), i+1);
		m_cTopLayerCB.AddString(str);
		m_cBotLayerCB.AddString(str);
	}
}

//----------------------------------------------------------------------------
// Initialize Stirrup CB
//----------------------------------------------------------------------------
void DgnConBeamBarDlgCIVIL::InitStirrupCB()
{
	if(m_cStirrupSize.GetCount()>0) m_cStirrupSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cStirrupSize);
/*
	if(m_cStirrupNum.GetCount()>0) m_cStirrupNum.ResetContent();
	CStringArray aRebarNum;
	if(CDBLib::GetSubRebarNumList_Civil(aRebarNum))
	{
		for(int i=0; i<aRebarNum.GetSize(); i++) m_cStirrupNum.AddString(aRebarNum[i]);
	}
*/
}

void DgnConBeamBarDlgCIVIL::SetData2Dlg()
{
	m_cStirrupSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_cStirrupSpaceUT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_AsTop.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsTopUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsBot.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsBotUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_cStirrupNum.SetUnitType(D_UNITSYS_NONE);

	// Stirrup Bar
//	m_cStirrupNum.SelectString(-1, m_pBeamData->dSubBarNum[m_nIMJ]);
	m_cStirrupNum.SetEditUnit(m_pBeamData->dSubBarNum[m_nIMJ]);
	m_cStirrupSpace.SetEditUnit(m_pBeamData->dSubBarDist[m_nIMJ]);
	int nIndex = m_cStirrupSize.SelectString(-1, m_pBeamData->strSubBarNa[m_nIMJ]);

	// Rebar Layer
	CString str;
	m_nTopLayer = 0;
	m_nBotLayer = 0;
	for(int i=0; i<5; i++){
		if(m_pBeamData->iTopBarNum[m_nIMJ][i]>0) m_nTopLayer++;
		if(m_pBeamData->iBotBarNum[m_nIMJ][i]>0) m_nBotLayer++;
	}

	if(m_nTopLayer == 0) m_nTopLayer = 3;        
	str.Format(_T("%d"), m_nTopLayer);
	nIndex = m_cTopLayerCB.SelectString(-1, str);    
	ASSERT(nIndex != LB_ERR);

	if(m_nBotLayer == 0) m_nBotLayer = 3;
	str.Format(_T("%d"), m_nBotLayer);
	nIndex = m_cBotLayerCB.SelectString(-1, str);    
	ASSERT(nIndex != LB_ERR);

	// Initialize는 그리드 내부에서..
	SetDataToGrid();
	m_TopGrid->SetLayerNum(m_nTopLayer);  
	m_BotGrid->SetLayerNum(m_nBotLayer); 
	
	T_UNIT_INDEX CurIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);
	double dAsTop=0.0, dAsBot=0.0;
	for(int i=0; i<5; i++)
	{
		// Top.
		if(m_pBeamData->iTopBarNum[m_nIMJ][i] > 0)
		{
			double dAs1=0.0, dAs2=0.0;
			if(m_pBeamData->strTopBarNa1[m_nIMJ][i]!=_T(""))  dAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pBeamData->strTopBarNa1[m_nIMJ][i]);
			if(m_pBeamData->strTopBarNa2[m_nIMJ][i]!=_T(""))  dAs2 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pBeamData->strTopBarNa2[m_nIMJ][i]);
			if(dAs2==0.0) dAs2 = dAs1;
			// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
			double dBarNum1 = m_pBeamData->iTopBarNum[m_nIMJ][i]/2.;
			double dBarNum2 = dBarNum1;
			dAsTop += dAs1*dBarNum1 + dAs2*dBarNum2;
		}
		// Bottom.
		if(m_pBeamData->iBotBarNum[m_nIMJ][i] > 0)
		{
			double dAs1=0.0, dAs2=0.0;
			if(m_pBeamData->strBotBarNa1[m_nIMJ][i]!=_T(""))  dAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pBeamData->strBotBarNa1[m_nIMJ][i]);
			if(m_pBeamData->strBotBarNa2[m_nIMJ][i]!=_T(""))  dAs2 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pBeamData->strBotBarNa2[m_nIMJ][i]);
			if(dAs2==0.0) dAs2 = dAs1;
			// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
			double dBarNum1 = m_pBeamData->iBotBarNum[m_nIMJ][i]/2.;
			double dBarNum2 = dBarNum1;
			dAsBot += dAs1*dBarNum1 + dAs2*dBarNum2;
		}
	}
	UpdateAsInfo(dAsTop, dAsBot);
	UpdateData(FALSE);
}

//----------------------------------------------------------------------------
// OnInitDialog    
//----------------------------------------------------------------------------
BOOL DgnConBeamBarDlgCIVIL::OnInitDialog() 
{
	if (m_TopGrid == nullptr)
		m_TopGrid = new CDgnBeamGridWndCIVIL(m_pBeamData, this);
	if (m_BotGrid == nullptr)
		m_BotGrid = new CDgnBeamGridWndCIVIL(m_pBeamData, this);

	CDialogMove::OnInitDialog();

	// Init LayerCB
	InitLayerCB();

	// Init StirrupCB
	InitStirrupCB();

	// Init Grid
	m_TopGrid->Initialize();

	m_BotGrid->Initialize();

	// Init Data
	SetData2Dlg();
	
	//  Init View Window
	CWnd* PlaceHolder = GetDlgItem(IDC_STATIC_VIEW); ASSERT(PlaceHolder);
	m_SectWnd.Initial(PlaceHolder);
	UpdateViewWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//

BOOL DgnConBeamBarDlgCIVIL::CheckInputData()
{
	int iChkResult = 0;
	_DGNC_RBAR RebarData;
	
	int nIndex = m_cStirrupSize.GetCurSel();
	int nError;
	CString str, errMsg;
	CDgnDataCtrl DataCtrl;

	int Index=0;
	T_RCHK_K RchkKey; 
	POSITION Pos = m_arRchkKey.GetStartPosition();
	while(Pos)
	{
		RebarData.Initialize();
		RchkKey=0; 
		str=_T(""); errMsg=_T("");
		m_arRchkKey.GetNextAssoc(Pos, Index, RchkKey);
		
		if(nIndex >= 0) 
			m_cStirrupSize.GetLBText(nIndex, RebarData.strSubBarNa);

		RebarData.iSectNo  = RchkKey;
		RebarData.iBarLayt = m_nTopLayer;
		RebarData.iBarLayb = m_nBotLayer;

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

		// Check Rebar Data
		nError = DataCtrl.Check_RbarCivil(1, RebarData);

		if(nError == 0)  
		{
			// Top
			for(int i=0; i<m_nTopLayer; i++)
			{
				if(RebarData.bCheckRebar[0][i] == FALSE)
					iChkResult |= 32;

				if(RebarData.bCheckSpace[0][i] == FALSE)
					iChkResult |= 64;

				if(RebarData.bCheckCovt[i] == FALSE)
					iChkResult |= 128;
			}

			// Bottom
			for(int i=0; i<m_nBotLayer; i++)
			{
				if(RebarData.bCheckRebar[1][i] == FALSE)
					iChkResult |= 256;

				if(RebarData.bCheckSpace[1][i] == FALSE)
					iChkResult |= 512;

				if(RebarData.bCheckCovb[i] == FALSE)
					iChkResult |= 1024;
			}
		}
		else
		{
			if(nError == 1)       iChkResult |= 2048;
			else if(nError == 2)  iChkResult |= 4096;
			else if(nError == 3)  iChkResult |= 8192;
		}

		// Check Stirrup Bar
		if(m_cStirrupNum.GetEditValue() <= 0.)    iChkResult |= 16384;

		if(m_cStirrupSpace.GetEditValue() <= 0.)  iChkResult |= 32768;

		CString strSubBarNa;
		nIndex = m_cStirrupSize.GetCurSel();
		if(nIndex < 0)  iChkResult |= 65536;
	}
 
	if(iChkResult & 2)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Num_Err));
//      errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 철근 개수가 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;

	}
	else if(iChkResult & 4)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Dc_Err));
//      errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 8)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Num_Err));
//      errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 철근 개수가 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 16)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Dc_Err));
//      errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 32)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Info_Err));
		//errMsg.Format(_T("[오류] Top Rebar 정보 중 Layer의 Rebar 정보가 올바르지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;        
	}
	else if(iChkResult & 256)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Info_Warning));
//        errMsg.Format(_T("[오류] Bottom Rebar 정보 중 Layer의 Rebar 정보가 올바르지 않습니다."));
		AfxMessageBox(errMsg);
		return FALSE;
	}
	else if(iChkResult & 2048)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Code));  /*AfxMessageBox(_T("No good Code"));*/       
		return FALSE;  
	}
	else if(iChkResult & 4096)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Not_exist_Section)); /*AfxMessageBox(_T("Not exist Section."));*/ 
		return FALSE;  
	}
	else if(iChkResult & 8192)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Section));   /*AfxMessageBox(_T("No good Section."));*/   
		return FALSE; 
	}
	else if(iChkResult & 16384)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Num_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 32768)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Spacing_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 65536)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Size_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Size) 입력이 적당하지 않습니다."));
		return FALSE;
	}
	else if(iChkResult & 64)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Space_Warning));
//        errMsg.Format(_T("[경고] Top Rebar 정보 중 Layer의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 128)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Dc_Warning));
//        errMsg.Format(_T("[경고] Top Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 512)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Space_Warning));
//        errMsg.Format(_T("[경고] Bottom Rebar 정보 중 Layer의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 1024)
	{
		errMsg.Format(_LS(IDS_DGN_CON_BEAM_Bot_Rebar_Dc_Warning));
//        errMsg.Format(_T("[경고] Bottom Rebar 정보 중 Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"));
		if(AfxMessageBox(errMsg, MB_YESNO) == IDNO) return FALSE;
	}

	UpdateAsInfo(RebarData.dAst, RebarData.dAsb);

	return TRUE;
}

void DgnConBeamBarDlgCIVIL::SetDlg2Data()
{
	// Stirrup Bar
	m_pBeamData->dSubBarDist[m_nIMJ] = m_cStirrupSpace.GetEditValue();
//	int nIndex = m_cStirrupNum.GetCurSel();
//  if(nIndex>=0) m_cStirrupNum.GetLBText(nIndex, m_pBeamData->strSubBarNum[m_nIMJ]);
	m_pBeamData->dSubBarNum[m_nIMJ] = m_cStirrupNum.GetEditValue();

	int nIndex = m_cStirrupSize.GetCurSel();
	if(nIndex >= 0) m_cStirrupSize.GetLBText(nIndex, m_pBeamData->strSubBarNa[m_nIMJ]);

	// Rebar
	GetDataFromGrid();
}

//----------------------------------------------------------------------------
// OnCancel
//----------------------------------------------------------------------------
void DgnConBeamBarDlgCIVIL::OnCancel() 
{
	CDialogMove::OnCancel();
}

//----------------------------------------------------------------------------
// OnOK
//----------------------------------------------------------------------------
void DgnConBeamBarDlgCIVIL::OnOK() 
{
	// 저장하기 전 Data의 유효성 여부 Check - Data 저장
	if(CheckInputData() == FALSE) return;
		
	SetDlg2Data();	
	CDialogMove::OnOK();

}

void DgnConBeamBarDlgCIVIL::PostNcDestroy() 
{
	if(m_TopGrid != NULL){ delete m_TopGrid; m_TopGrid=NULL; }
	if(m_BotGrid != NULL){ delete m_BotGrid; m_BotGrid=NULL; }
	
	CDialogMove::PostNcDestroy();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Change Layer CB 관련 
// 

void DgnConBeamBarDlgCIVIL::OnSelchangeDgnRcBotLayerCb() 
{
	CString strSel = _T("");
	int nIndex, nLayer;

	nIndex = m_cBotLayerCB.GetCurSel();
	if(nIndex>=0) m_cBotLayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);

	if(nLayer<1 || nLayer>5) return;

	m_nBotLayer = nLayer;
	m_BotGrid->SetLayerNum(m_nBotLayer);  
}

void DgnConBeamBarDlgCIVIL::OnSelchangeDgnRcTopLayerCb() 
{
	CString strSel = _T("");
	int nIndex, nLayer;

	nIndex = m_cTopLayerCB.GetCurSel();
	m_cTopLayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);

	if(nLayer<1 || nLayer>5) return;

	m_nTopLayer = nLayer;
	m_TopGrid->SetLayerNum(m_nTopLayer);  
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Grid 관련
// 

void DgnConBeamBarDlgCIVIL::SetDataToGrid()
{
	for(int i=0; i<5; i++)
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

void DgnConBeamBarDlgCIVIL::GetDataFromGrid()
{
	CString str = _T("");

	// Top Rebar
	for(int i=0; i<5; i++)
	{
		str = m_TopGrid->GetValueRowCol(i+1, 1);
		m_pBeamData->iTopBarNum[m_nIMJ][i] = _ttoi(str);

		str = m_TopGrid->GetValueRowCol(i+1, 2);
		m_pBeamData->strTopBarNa1[m_nIMJ][i] = str;

		str = m_TopGrid->GetValueRowCol(i+1, 3);
		m_pBeamData->strTopBarNa2[m_nIMJ][i] = str;

		str = m_TopGrid->GetValueRowCol(i+1, 4);
		m_pBeamData->dDt[m_nIMJ][i] = _tstof(str);
	}

	// Bot Rebar
	for(int i=0; i<5; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+1, 1);
		m_pBeamData->iBotBarNum[m_nIMJ][i] = _ttoi(str);

		str = m_BotGrid->GetValueRowCol(i+1, 2);
		m_pBeamData->strBotBarNa1[m_nIMJ][i] = str;

		str = m_BotGrid->GetValueRowCol(i+1, 3);
		m_pBeamData->strBotBarNa2[m_nIMJ][i] = str;

		str = m_BotGrid->GetValueRowCol(i+1, 4);
		m_pBeamData->dDb[m_nIMJ][i] = _tstof(str);

	}  
}


void DgnConBeamBarDlgCIVIL::UpdateViewWnd()
{
	T_RCHK_D RchkData; RchkData.Initialize();
	RchkData.nType = 0;

	CString str = _T("");

	// Top Rebar
	for(int i=0; i<m_nTopLayer; i++)
	{
		str = m_TopGrid->GetValueRowCol(i+1, 1);
		RchkData.BEAM.iTopBarNum[m_nIMJ][i] = _ttoi(str);

		str = m_TopGrid->GetValueRowCol(i+1, 4);
		RchkData.BEAM.dDt[m_nIMJ][i] = _tstof(str);
	}

	// Bot Rebar
	for(int i=0; i<m_nBotLayer; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+1, 1);
		RchkData.BEAM.iBotBarNum[m_nIMJ][i] = _ttoi(str);

		str = m_BotGrid->GetValueRowCol(i+1, 4);
		RchkData.BEAM.dDb[m_nIMJ][i] = _tstof(str);
	}  

	RchkData.BEAM.dSubBarNum[m_nIMJ] = m_cStirrupNum.GetEditValue();
	//int nIndex = m_cStirrupNum.GetCurSel();
	//if(nIndex>=0) m_cStirrupNum.GetLBText(nIndex, RchkData.BEAM.strSubBarNum[m_nIMJ]);

	m_SectWnd.Draw_Shape(0, m_RchkKey, FALSE, TRUE);
	m_SectWnd.Draw_ChkRebar(m_RchkKey, m_nIMJ, RchkData, FALSE, TRUE);
	m_SectWnd.Invalidate();
}


void DgnConBeamBarDlgCIVIL::OnRedrawBtn() 
{
	if(CheckInputData()==FALSE) return;
	
	UpdateViewWnd();
}

void DgnConBeamBarDlgCIVIL::UpdateAsInfo(double dAst, double Asb)
{
	m_AsTop.SetEditUnit(dAst);
	m_AsBot.SetEditUnit(Asb);
}