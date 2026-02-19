// DgnRCPlateBeamRebarMBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCPlateBeamRebarMBarDlg.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define T_RIPB_LAYER 3

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateBeamRebarMBarDlg dialog


CDgnRCPlateBeamRebarMBarDlg::CDgnRCPlateBeamRebarMBarDlg(T_RPBM_D* pData, CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRCPlateBeamRebarMBarDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_pData = pData;

	m_iThik = 0;

	m_TopGrid = NULL;
	m_BotGrid = NULL;
}

CDgnRCPlateBeamRebarMBarDlg::~CDgnRCPlateBeamRebarMBarDlg()
{
	
}

void CDgnRCPlateBeamRebarMBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCPlateBeamRebarMBarDlg)  
	DDX_Control(pDX, IDC_DGN_Plate_TOP_LAYER_CMB,        m_cmbTopLayer);
	DDX_Control(pDX, IDC_DGN_Plate_TOP_As_EDT,           m_edtAsTop);	
	DDX_Control(pDX, IDC_DGN_Plate_BOT_LAYER_CMB,        m_cmbBotLayer);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_As_EDT,           m_edtAsBot);	    
	DDX_Control(pDX, IDC_DGN_Plate_TOP_LAYER_GRID, *m_TopGrid);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_LAYER_GRID, *m_BotGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnRCPlateBeamRebarMBarDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRCPlateBeamRebarMBarDlg)  
	ON_CBN_SELCHANGE(IDC_DGN_Plate_TOP_LAYER_CMB,  OnSelchangeDgnRcTopLayerCmb)
	ON_CBN_SELCHANGE(IDC_DGN_Plate_BOT_LAYER_CMB,  OnSelchangeDgnRcBotLayerCmb)  
	ON_BN_CLICKED(IDC_DGN_Plate_NUM,               OnClickedNumCTC)
	ON_BN_CLICKED(IDC_DGN_Plate_CTC,               OnClickedNumCTC)
	ON_MESSAGE(WM_USER_BEFORE_END_EDITING,         OnBeforeEndEditing)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateBeamRebarMBarDlg message handlers

BOOL CDgnRCPlateBeamRebarMBarDlg::OnInitDialog() 
{
	T_RCHK_BEAM  BeamData;
	BeamData.Initialize();

	if (m_TopGrid == nullptr)
		m_TopGrid = new CDgnBeamGridWndCIVIL(&BeamData);
	if (m_BotGrid == nullptr)
		m_BotGrid = new CDgnBeamGridWndCIVIL(&BeamData);

	CChildDialog::OnInitDialog();

	InitCtrl();
		
	InitUnit();

	Data2Dlg();

	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDgnRCPlateBeamRebarMBarDlg::OnBeforeEndEditing(WPARAM wParam, LPARAM lParam)
{
	Dlg2Data();
	ReCalcAs();
	return 1;
}

void CDgnRCPlateBeamRebarMBarDlg::OnClickedNumCTC()
{
	SetNumCTC(TRUE);
	ReCalcAs();
}

void CDgnRCPlateBeamRebarMBarDlg::SetNumCTC(BOOL bClick/*=TRUE*/)
{
	if(bClick) CDlgUtil::CtrlRadioSetCheck(this, m_aRdoNumCTC, 1);// NUM은 사용안하므로..

	int nNumCTC;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoNumCTC, nNumCTC);
	m_TopGrid->SetNumCTC(nNumCTC);
	m_BotGrid->SetNumCTC(nNumCTC);
}

void CDgnRCPlateBeamRebarMBarDlg::InitCtrl()
{
	m_aRdoNumCTC.RemoveAll();
	m_aRdoNumCTC.Add(IDC_DGN_Plate_NUM);
	m_aRdoNumCTC.Add(IDC_DGN_Plate_CTC);  
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoNumCTC, 0);

	GetDlgItem(IDC_DGN_Plate_NUM)->EnableWindow(FALSE);
 
	InitCombo();
	InitGrid();  
}

void CDgnRCPlateBeamRebarMBarDlg::InitCombo()
{
	CString str;

	m_cmbTopLayer.ResetContent();
	m_cmbBotLayer.ResetContent();

	for(int i=0; i<T_RIPB_LAYER+1; i++)  // Layer 3 (0포함)
	{
		str.Format(_T("%d"), i);
		m_cmbTopLayer.AddString(str);
		m_cmbBotLayer.AddString(str);
	}

	m_cmbTopLayer.SetCurSel(1);
	m_cmbBotLayer.SetCurSel(1);
}

void CDgnRCPlateBeamRebarMBarDlg::InitGrid()
{
	m_TopGrid->SetTopBottom(0);
	m_TopGrid->Initialize();
	m_TopGrid->SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced );
	m_TopGrid->SetScrollBarMode(SB_HORZ, gxnDisabled);

	m_BotGrid->SetTopBottom(1);
	m_BotGrid->Initialize();
	m_BotGrid->SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced );
	m_BotGrid->SetScrollBarMode(SB_HORZ, gxnDisabled);

	m_TopGrid->SetLayerNum(1); // Default 1
	m_BotGrid->SetLayerNum(1); // Default 1
}

void CDgnRCPlateBeamRebarMBarDlg::InitUnit()
{
	m_edtAsTop.SetUnitType(D_UNITSYS_BASE_AREA);	
	m_edtAsBot.SetUnitType(D_UNITSYS_BASE_AREA);

	CString strCurArea;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_AREA, strCurArea);
	

	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CString strUnitLen;
	if(CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		strUnitLen = _T("/ft");
	}
	else
	{
		strUnitLen = _T("/m");
	}  

	GetDlgItem(IDC_DGN_Plate_TOP_As_UNIT)->SetWindowText(strCurArea + strUnitLen);
	GetDlgItem(IDC_DGN_Plate_BOT_As_UNIT)->SetWindowText(strCurArea + strUnitLen);
}

void CDgnRCPlateBeamRebarMBarDlg::ReCalcAs()
{
	if (!GetSafeHwnd() || !IsWindow(GetSafeHwnd())) return;
	
	Dlg2Data();

	double dAsTop = 0.0; // 반영하기
	double dAsBot = 0.0; // 반영하기
	CalcAs(*m_pData, dAsTop, dAsBot);

	m_edtAsTop.SetEditUnit(dAsTop);
	m_edtAsBot.SetEditUnit(dAsBot);
}

void CDgnRCPlateBeamRebarMBarDlg::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoNumCTC, m_pData->bNum ? 0 : 1);

	m_cmbTopLayer.SetCurSel(m_pData->nTopLayer);
	m_cmbBotLayer.SetCurSel(m_pData->nBotLayer);

	SetDataToGrid();  
	ReCalcAs();

	UpdateData(FALSE);	
}

BOOL CDgnRCPlateBeamRebarMBarDlg::Dlg2Data()
{ 
	UpdateData(TRUE);

	int nNumCTC;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoNumCTC, nNumCTC);
	m_pData->bNum = nNumCTC == 0;
	m_pData->nTopLayer = m_cmbTopLayer.GetCurSel();
	m_pData->nBotLayer = m_cmbBotLayer.GetCurSel();

	GetDataFromGrid();

	// 설계 요청 : Data를 세팅할 때는 Num, CTC 중 아무거나 로 계산해도 사용자가 입력한 정보로 계산되도록 해달라고 함..
	double dOneMiter = M_InitValueCurUnit(1, N, M, D_UNITSYS_BASE_LENGTH);
	if(m_pData->bNum)
	{ 
		for (int i = 0; i < T_RIPB_LAYER; i++)
		{
			int nNum = m_pData->nTopBarNum[i];
			if(nNum <= 0)
			{
				m_pData->dTopBarCTC[i] = 0;
			}
			else
			{
				m_pData->dTopBarCTC[i] = dOneMiter / (double)nNum;
			}
		}
		for (int i = 0; i < T_RIPB_LAYER; i++)
		{
			int nNum = m_pData->nBotBarNum[i];
			if(nNum <= 0)
			{
				m_pData->dBotBarCTC[i] = 0;
			}
			else
			{
				m_pData->dBotBarCTC[i] = dOneMiter / (double)nNum;
			}
		}
	}
	else
	{
		for (int i = 0; i < T_RIPB_LAYER; i++)
		{
			double dCTCMiter = m_pData->dTopBarCTC[i] * m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH); // Convert Unit In
			if(dCTCMiter <= 0)
			{
				m_pData->nTopBarNum[i] = 0;
			}
			else
			{
				m_pData->nTopBarNum[i] = (int)(dOneMiter / dCTCMiter);
			}
		}
		for (int i = 0; i < T_RIPB_LAYER; i++)
		{
			double dCTCMiter = m_pData->dBotBarCTC[i] * m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH); // Convert Unit In
			if(dCTCMiter <= 0)
			{
				m_pData->nBotBarNum[i] = 0;
			}
			else
			{
				m_pData->nBotBarNum[i] = (int)(dOneMiter / dCTCMiter);
			}
		}
	}
	
	//ASSERT(0); // 아래 함수 CheckData쪽으로 옮기자..
//	if(!CheckRebarData(m_Data)) return FALSE;

	return TRUE;
}

void CDgnRCPlateBeamRebarMBarDlg::SetDataToGrid()
{
	int nNumCTC;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoNumCTC, nNumCTC);
	SetNumCTC();
	for(int i=0; i<T_RIPB_LAYER; i++)
	{
		m_TopGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_NUM_CTC), nNumCTC == 0 ? m_pData->nTopBarNum[i] : m_pData->dTopBarCTC[i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_SIZE1), m_pData->strTopBarNa1[i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_SIZE2), m_pData->strTopBarNa2[i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_DT_DB), m_pData->dDt[i]);

		m_BotGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_NUM_CTC), nNumCTC == 0 ? m_pData->nBotBarNum[i] : m_pData->dBotBarCTC[i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_SIZE1), m_pData->strBotBarNa1[i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_SIZE2), m_pData->strBotBarNa2[i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,D_REBAR_GRID_COL_DT_DB), m_pData->dDb[i]);
	}
	
	m_TopGrid->SetLayerNum(m_pData->nTopLayer);
	m_BotGrid->SetLayerNum(m_pData->nBotLayer);  
}

void CDgnRCPlateBeamRebarMBarDlg::GetDataFromGrid()
{
	CString str = _T("");
	int nNumCTC;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoNumCTC, nNumCTC);
	for(int i=0; i<T_RIPB_LAYER; i++)
	{
		str = m_TopGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_NUM_CTC);
		if(nNumCTC == 0)
		{
			m_pData->nTopBarNum[i] =  _ttoi(str);
		}
		else
		{
			m_pData->dTopBarCTC[i] =  _tstof(str);      
		}		

		str = m_TopGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_SIZE1);
		m_pData->strTopBarNa1[i] = str;

		str = m_TopGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_SIZE2);
		m_pData->strTopBarNa2[i] = str;

		str = m_TopGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_DT_DB);
		m_pData->dDt[i] = _tstof(str);
	}

	for(int i=0; i<T_RIPB_LAYER; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_NUM_CTC);
		if(nNumCTC == 0)
		{
			m_pData->nBotBarNum[i] =  _ttoi(str);
		}
		else
		{
			m_pData->dBotBarCTC[i] =  _tstof(str);      
		}		

		str = m_BotGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_SIZE1);
		m_pData->strBotBarNa1[i] = str;

		str = m_BotGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_SIZE2);
		m_pData->strBotBarNa2[i] = str;

		str = m_BotGrid->GetValueRowCol(i+1, D_REBAR_GRID_COL_DT_DB);
		m_pData->dDb[i] = _tstof(str);
	}  
}

void CDgnRCPlateBeamRebarMBarDlg::ControlsEnableDisable()
{ 
	
	
}
/*
BOOL CDgnRCPlateBeamRebarMBarDlg::CheckRebarData(T_RRPL_SUB_D &rData)
{
	// 이쪽은 안건드리는 걸로....
	if(!CheckRebarData_MainRebar01(rData)) return FALSE;
	if(!CheckRebarData_MainRebar02(rData)) return FALSE;
	if(!CheckRebarData_Stirrup(rData)) return FALSE;	

	return TRUE;
}

BOOL CDgnRCPlateBeamRebarMBarDlg::CheckRebarData_MainRebar01(T_RRPL_SUB_D &rData)
{
	// Rebar Check
	int  nInput = 0;
	BOOL bInput[3];
	for(int i=0; i<3; i++) bInput[i] = TRUE;
	if(rData.nTopBarNum[0][0] == 0 && rData.nBotBarNum[0][0] == 0) bInput[0] = FALSE; // I_Node
	if(rData.nTopBarNum[1][0] == 0 && rData.nBotBarNum[1][0] == 0) bInput[1] = FALSE; // Middle
	if(rData.nTopBarNum[2][0] == 0 && rData.nBotBarNum[2][0] == 0) bInput[2] = FALSE; // J_Node

	for(i=0; i<3; i++) if(bInput[i] == TRUE) nInput = nInput + 1;

	if(nInput == 0)          // 입력이 하나도 안됨
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Rebar_Info_Err));
		//    AfxMessageBox(_T("[오류] Rebar 정보가 입력되지 않았습니다."));
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnRCPlateBeamRebarMBarDlg::CheckRebarData_MainRebar02(T_RRPL_SUB_D &rData)
{
	_DGNC_RBAR RebarData; RebarData.Initialize();

	int nError=0;
	CString str, errMsg;
	CDgnDataCtrl DataCtrl;

	int Index=0;
	T_RCHK_K RchkKey; 
	for(int iPosi=0; iPosi<1; iPosi++)
	{
		if(!CheckPosiInputData_New(rData)) 
		{
			//       POSITION Pos = m_arRchkKey.GetStartPosition();
			//       while(Pos)
			{
				RebarData.Initialize();
				RchkKey=0; 
				str=_T(""); errMsg=_T("");

				//RebarData.iSectNo  = RchkKey;
				RebarData.iBarLayt = rData.nTopLayer;
				RebarData.iBarLayb = rData.nBotLayer;

				// Top
				for(int i=0; i<rData.nTopLayer; i++)
				{     
					RebarData.iBarNum[0][i] = (int)rData.nTopBarNum[iPosi][i];
					if(RebarData.iBarNum[0][i] < 0)
					{
						AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Top_Rebar_Num_Err));
						return FALSE;
					}

					RebarData.dDt[i] = rData.dDt[iPosi][i];
					double CL = 0;
					if(i == 0) CL = 0;
					else       CL = RebarData.dDt[i-1];

					if(i == 0)
					{
						if(RebarData.dDt[i] < CL)
						{
							iChkResult |= 4;
						}
					}
					else
					{
						if(RebarData.dDt[i] <= CL)
							iChkResult |= 4;
					}
					RebarData.strBarNa1[0][i] = rData.strTopBarNa1[iPosi][i];
					RebarData.strBarNa2[0][i] = rData.strTopBarNa2[iPosi][i];
				}

				// Bottom
				for(i=0; i<rData.nBotLayer ; i++)
				{      
					RebarData.iBarNum[1][i] = (int)rData.nBotBarNum[iPosi][i];
					if(RebarData.iBarNum[1][i] < 0)
						iChkResult |= 8;      
					RebarData.dDb[i] = rData.dDb[iPosi][i];
					double CL = 0;
					if(i == 0) CL = 0;
					else       CL = RebarData.dDb[i-1];

					if(i == 0) 
					{
						if(RebarData.dDb[i] < CL)
							iChkResult |= 16;
					}
					else
					{
						if(RebarData.dDb[i] <= CL)
							iChkResult |= 16;
					}

					RebarData.strBarNa1[1][i] = rData.strBotBarNa1[iPosi][i];
					RebarData.strBarNa2[1][i] = rData.strBotBarNa2[iPosi][i];
				}

				// Check Rebar Data
				nError = DataCtrl.Check_RbarCivil4PlateRating(1, m_iThik, RebarData);

				if(nError == 0)  
				{
					// Top
					for(int i=0; i<rData.nTopLayer; i++)
					{
						if(RebarData.bCheckRebar[0][i] == FALSE)
						{ iChkResult |= 32; }

						if(RebarData.bCheckSpace[0][i] == FALSE)
						{ iChkResult |= 64; }

						if(RebarData.bCheckCovt[i] == FALSE)
						{ iChkResult |= 128; }
					}

					// Bottom
					for(i=0; i<rData.nBotLayer; i++)
					{
						if(RebarData.bCheckRebar[1][i] == FALSE)
						{ iChkResult |= 256; }

						if(RebarData.bCheckSpace[1][i] == FALSE)
						{ iChkResult |= 512; }

						if(RebarData.bCheckCovb[i] == FALSE)
						{ iChkResult |= 1024; }
					}
				}
				else
				{
					if(nError == 1)       iChkResult |= 2048;
					else if(nError == 2)  iChkResult |= 4096;
					else if(nError == 3)  iChkResult |= 8192;
				}
			}
		} //if(CheckPosiInputData(iPosi))
		//else { bExistData[iPosi]=FALSE; } // FALSE==No Input Data.
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
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Code));  
		//AfxMessageBox(_T("No good Code"));
		return FALSE;  
	}
	else if(iChkResult & 4096)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Not_exist_Section)); 
		//AfxMessageBox(_T("Not exist Section."));
		return FALSE;  
	}
	else if(iChkResult & 8192)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Section));   
		//AfxMessageBox(_T("No good Section."));
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

	return TRUE;
}

BOOL CDgnRCPlateBeamRebarMBarDlg::CheckPosiInputData_New(T_RRPL_SUB_D &rData)
{
	if(rData.bChkSubBar[0])
	{
		double dZero = 1.0E-7;

		if(rData.strSubBarNa[0] != _T("")) return FALSE;
		if(fabs(rData.dSubBarNum[0]-2.)>dZero) return FALSE;
		if(rData.dSubBarDist[0]!=0.0) return FALSE;
		if(rData.dSubBarAngle[0]!=0.0) return FALSE;
	}

	for(int j=0; j<rData.nTopLayer; j++)
	{
		if(rData.dDt[0][j]!=0.0)        return FALSE; 
		if(rData.nTopBarNum[0][j]!=0.0) return FALSE; 
		if(rData.strTopBarNa1[0][j]!=_T("")) return FALSE; 
		if(rData.strTopBarNa2[0][j]!=_T("")) return FALSE; 
	}
	
	for(j=0; j<rData.nBotLayer; j++)
	{
		if(rData.dDb[0][j]!=0.0) return FALSE; 
		if(rData.nBotBarNum[0][j]!=0.0) return FALSE; 
		if(rData.strBotBarNa1[0][j]!=_T("")) return FALSE; 
		if(rData.strBotBarNa2[0][j]!=_T("")) return FALSE; 
	}

	return TRUE;
}

BOOL CDgnRCPlateBeamRebarMBarDlg::CheckRebarData_Stirrup(T_RRPL_SUB_D &rData)
{
	double dZero = 1.0E-7;

	if(rData.bChkSubBar[0])
	{
		if(rData.strSubBarNa[0]==_T("")) return FALSE;
		if(fabs(rData.dSubBarNum[0])<dZero) return FALSE;//if(fabs(rData.dSubBarNum[0]-2.)>dZero) return FALSE;
		if(rData.dSubBarDist[0]<dZero) return FALSE;
	}

	return TRUE;
}
*/

void CDgnRCPlateBeamRebarMBarDlg::CalcAs(T_RPBM_D& RpbmD, double& dAsTop, double& dAsBot)
{
	dAsTop = 0.0;
	dAsBot = 0.0;

	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	m_pDoc->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);

	double dUnitLen;
	if(CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		dUnitLen = M_InitValueCurUnit(1, N, FT, D_UNITSYS_BASE_LENGTH);
	}
	else
	{
		dUnitLen = M_InitValueCurUnit(1, N, M, D_UNITSYS_BASE_LENGTH);
	}  

    const double dZero = 1.0E-7;
	for(int i=0; i<RpbmD.nTopLayer; i++)
	{
		// Top.
		double dAs1=0.0, dAs2=0.0;
		if(RpbmD.strTopBarNa1[i]!=_T(""))
			dAs1 = m_pDoc->m_pMatlDB->Get_RebarArea(RpbmD.strTopBarNa1[i]);    
		if(RpbmD.strTopBarNa2[i]!=_T(""))
		    dAs2 = m_pDoc->m_pMatlDB->Get_RebarArea(RpbmD.strTopBarNa2[i]);
		if(dAs2<dZero) dAs2 = dAs1;

		double dBarNum1;
		double dBarNum2;

		double dCTC;
		if(RpbmD.bNum)
		{
			dCTC = dUnitLen / RpbmD.nTopBarNum[i];
		}
		else
		{        
			dCTC = RpbmD.dTopBarCTC[i];      
		}

		if(dCTC == 0)
		{
			dBarNum1 = 0;
		}
		else
		{
			dBarNum1 = dUnitLen / dCTC;
		}
		dBarNum1 = dBarNum1 / 2;
		dBarNum2 = dBarNum1;

		dAsTop += (dAs1*dBarNum1 + dAs2*dBarNum2);
	}
	for (int i = 0; i < RpbmD.nBotLayer; i++)
	{
		// Bot.
		double dAs1=0.0, dAs2=0.0;
		if(RpbmD.strBotBarNa1[i]!=_T(""))
			dAs1 = m_pDoc->m_pMatlDB->Get_RebarArea(RpbmD.strBotBarNa1[i]);    
		if(RpbmD.strBotBarNa2[i]!=_T(""))
		    dAs2 = m_pDoc->m_pMatlDB->Get_RebarArea(RpbmD.strBotBarNa2[i]);
		if(dAs2<dZero) dAs2 = dAs1;

		double dBarNum1;
		double dBarNum2;

		double dCTC;
		if(RpbmD.bNum)
		{
			dCTC = dUnitLen / RpbmD.nBotBarNum[i];
		}
		else
		{        
			dCTC = RpbmD.dBotBarCTC[i];      
		}

		if(dCTC == 0)
		{
			dBarNum1 = 0;
		}
		else
		{
			dBarNum1 = dUnitLen / dCTC;
		}
		dBarNum1 = dBarNum1 / 2;
		dBarNum2 = dBarNum1;

		dAsBot += (dAs1*dBarNum1 + dAs2*dBarNum2);
	}
}

void CDgnRCPlateBeamRebarMBarDlg::PostNcDestroy() 
{	
	if(m_TopGrid != NULL){ delete m_TopGrid; m_TopGrid=NULL; }
	if(m_BotGrid != NULL){ delete m_BotGrid; m_BotGrid=NULL; }

	CChildDialog::PostNcDestroy();
}

void CDgnRCPlateBeamRebarMBarDlg::OnOK()
{
	if(!Dlg2Data()) return;

	CChildDialog::OnOK();
}

void CDgnRCPlateBeamRebarMBarDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CChildDialog::OnCancel();
}

void CDgnRCPlateBeamRebarMBarDlg::OnSelchangeDgnRcTopLayerCmb() 
{
	int nLayer = m_cmbTopLayer.GetCurSel();
	m_TopGrid->SetLayerNum(nLayer); 
	ReCalcAs();
}

void CDgnRCPlateBeamRebarMBarDlg::OnSelchangeDgnRcBotLayerCmb() 
{
	int nLayer = m_cmbBotLayer.GetCurSel();
	m_BotGrid->SetLayerNum(nLayer); 
	ReCalcAs();
}

void CDgnRCPlateBeamRebarMBarDlg::OnDestroy() 
{
	Dlg2Data();
	CChildDialog::OnDestroy();
}
