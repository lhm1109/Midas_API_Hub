// DgnRCRatingPlateRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatingPlateRebarDlg.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndCIVIL.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOTALLAYER 3

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateRebarDlg dialog


CDgnRCRatingPlateRebarDlg::CDgnRCRatingPlateRebarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRCRatingPlateRebarDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();

	m_iThik = 0;

	m_TopGrid = NULL;
	m_BotGrid = NULL;
}

CDgnRCRatingPlateRebarDlg::~CDgnRCRatingPlateRebarDlg()
{
}

void CDgnRCRatingPlateRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCRatingPlateRebarDlg)
	DDX_Control(pDX, IDC_DGN_Plate_TOP_LAYER_CMB,        m_cmbTopLayer);
	DDX_Control(pDX, IDC_DGN_Plate_TOP_As_EDT,           m_edtAsTop);
	DDX_Control(pDX, IDC_DGN_Plate_TOP_As_UNIT,          m_untAsTop);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_LAYER_CMB,        m_cmbBotLayer);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_As_EDT,           m_edtAsBot);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_As_UNIT,          m_untAsBot);
	DDX_Check  (pDX, IDC_DGN_Plate_STIRRUP_CHK,          m_bSRebar);
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_SIZE_CMB,     m_cmbSSize);
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_SPACE_EDT,    m_edtSSpace);
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_SPACE_UNIT,   m_untSSpace); 
	DDX_Control(pDX, IDC_DGN_Plate_STIRRUP_NUMBER_EDT,   m_edtSNum);

	DDX_Control(pDX, IDC_DGN_Plate_TOP_LAYER_GRID, *m_TopGrid);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_LAYER_GRID, *m_BotGrid);
	//}}AFX_DATA_MAP
}


void CDgnRCRatingPlateRebarDlg::SetData(const int iThik, T_RRPL_SUB_D& rData)
{
	m_Data = rData;
	m_iThik = iThik;
}

void CDgnRCRatingPlateRebarDlg::GetData(T_RRPL_SUB_D& rData)
{
	rData  = m_Data;
}

void CDgnRCRatingPlateRebarDlg::InitCombo()
{
	CString str;

	m_cmbTopLayer.ResetContent();
	m_cmbBotLayer.ResetContent();

	for(int i=0; i<TOTALLAYER+1; i++)  // Layer 3 (0포함)
	{
		str.Format(_T("%d"), i);
		m_cmbTopLayer.AddString(str);
		m_cmbBotLayer.AddString(str);
	}

	m_cmbTopLayer.SetCurSel(1);
	m_cmbBotLayer.SetCurSel(1);

	m_cmbSSize.ResetContent();
	if(m_cmbSSize.GetCount()>0) m_cmbSSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_cmbSSize);
	m_cmbSSize.SetCurSel(0);
}

void CDgnRCRatingPlateRebarDlg::InitGrid()
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

void CDgnRCRatingPlateRebarDlg::InitUnit()
{
	m_edtAsTop.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untAsTop.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtAsBot.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untAsBot.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtSSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSNum.SetEditUnit(D_UNITSYS_NONE); 
}

void CDgnRCRatingPlateRebarDlg::Data2Dlg()
{
	double dAsTop = 0.0; // 반영하기
	double dAsBot = 0.0; // 반영하기
	CalcAs(m_Data, dAsTop, dAsBot);

	m_edtAsTop.SetEditUnit(dAsTop);
	m_edtAsBot.SetEditUnit(dAsBot);
	m_cmbTopLayer.SetCurSel(m_Data.nTopLayer);
	m_cmbBotLayer.SetCurSel(m_Data.nBotLayer);

	SetDataToGrid();

	m_bSRebar = m_Data.bChkSubBar[0];
	m_cmbSSize.SelectString(-1, m_Data.strSubBarNa[0]);
	m_edtSSpace.SetEditUnit(m_Data.dSubBarDist[0]);
	m_edtSNum.SetEditUnit(m_Data.dSubBarNum[0]);

	UpdateData(FALSE);	
}

BOOL CDgnRCRatingPlateRebarDlg::Dlg2Data()
{ 
	UpdateData(TRUE);

	m_Data.nTopLayer = m_cmbTopLayer.GetCurSel();
	m_Data.nBotLayer = m_cmbBotLayer.GetCurSel();

	GetDataFromGrid();

	m_Data.bChkSubBar[0] = m_bSRebar;
	m_cmbSSize.GetLBText(m_cmbSSize.GetCurSel(), m_Data.strSubBarNa[0]); 
	m_Data.dSubBarDist[0] = m_edtSSpace.GetEditValue();
	m_Data.dSubBarNum[0]  = m_edtSNum.GetEditValue();  
	
	if(!CheckRebarData(m_Data)) return FALSE;

	return TRUE;
}

void CDgnRCRatingPlateRebarDlg::SetDataToGrid()
{
	for(int i=0; i<TOTALLAYER; i++)
	{
		m_TopGrid->SetValueRange(CGXRange(i+1,1), m_Data.dTopBarNum[0][i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,2), m_Data.strTopBarNa1[0][i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,3), m_Data.strTopBarNa2[0][i]);
		m_TopGrid->SetValueRange(CGXRange(i+1,4), m_Data.dDt[0][i]);

		m_BotGrid->SetValueRange(CGXRange(i+1,1), m_Data.dBotBarNum[0][i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,2), m_Data.strBotBarNa1[0][i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,3), m_Data.strBotBarNa2[0][i]);
		m_BotGrid->SetValueRange(CGXRange(i+1,4), m_Data.dDb[0][i]);
	}
	
	m_TopGrid->SetLayerNum(m_Data.nTopLayer);
	m_BotGrid->SetLayerNum(m_Data.nBotLayer);
}

void CDgnRCRatingPlateRebarDlg::GetDataFromGrid()
{
	CString str = _T("");

	for(int i=0; i<TOTALLAYER; i++)
	{
		str = m_TopGrid->GetValueRowCol(i+1, 1);
		m_Data.dTopBarNum[0][i] = _ttoi(str);

		str = m_TopGrid->GetValueRowCol(i+1, 2);
		m_Data.strTopBarNa1[0][i] = str;

		str = m_TopGrid->GetValueRowCol(i+1, 3);
		m_Data.strTopBarNa2[0][i] = str;

		str = m_TopGrid->GetValueRowCol(i+1, 4);
		m_Data.dDt[0][i] = _tstof(str);
	}

	for(int i=0; i<TOTALLAYER; i++)
	{
		str = m_BotGrid->GetValueRowCol(i+1, 1);
		m_Data.dBotBarNum[0][i] = _ttoi(str);

		str = m_BotGrid->GetValueRowCol(i+1, 2);
		m_Data.strBotBarNa1[0][i] = str;

		str = m_BotGrid->GetValueRowCol(i+1, 3);
		m_Data.strBotBarNa2[0][i] = str;

		str = m_BotGrid->GetValueRowCol(i+1, 4);
		m_Data.dDb[0][i] = _tstof(str);
	}  
}

void CDgnRCRatingPlateRebarDlg::ControlsEnableDisable()
{ 
	CArray<UINT, UINT> aControls;
	CDlgUtil::GetCtrlIDByIncRect(this, aControls, IDC_DGN_Plate_STIRRUP_GRP, FALSE);  
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bSRebar);
	GetDlgItem(IDC_DGN_Plate_STIRRUP_CHK)->EnableWindow(TRUE);
}

BOOL CDgnRCRatingPlateRebarDlg::CheckRebarData(T_RRPL_SUB_D &rData)
{
	// 이쪽은 안건드리는 걸로....
	if(!CheckRebarData_MainRebar01(rData)) return FALSE;
	if(!CheckRebarData_MainRebar02(rData)) return FALSE;
	if(!CheckRebarData_Stirrup(rData)) return FALSE;	

	return TRUE;
}

BOOL CDgnRCRatingPlateRebarDlg::CheckRebarData_MainRebar01(T_RRPL_SUB_D &rData)
{
	// Rebar Check
	int  nInput = 0;
	BOOL bInput[3];
	for(int i=0; i<3; i++) bInput[i] = TRUE;
	if(rData.dTopBarNum[0][0] == 0 && rData.dBotBarNum[0][0] == 0) bInput[0] = FALSE; // I_Node
	if(rData.dTopBarNum[1][0] == 0 && rData.dBotBarNum[1][0] == 0) bInput[1] = FALSE; // Middle
	if(rData.dTopBarNum[2][0] == 0 && rData.dBotBarNum[2][0] == 0) bInput[2] = FALSE; // J_Node

	for(int i=0; i<3; i++) if(bInput[i] == TRUE) nInput = nInput + 1;

	if(nInput == 0)          // 입력이 하나도 안됨
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Rebar_Info_Err));
		//    AfxMessageBox(_T("[오류] Rebar 정보가 입력되지 않았습니다."));
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnRCRatingPlateRebarDlg::CheckRebarData_MainRebar02(T_RRPL_SUB_D &rData)
{
	int iChkResult = 0;
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
					RebarData.iBarNum[0][i] = (int)rData.dTopBarNum[iPosi][i];
					if(RebarData.iBarNum[0][i] < 0)
						iChkResult |= 2;

					RebarData.dDt[i] = rData.dDt[iPosi][i];
					double CL = 0;
					if(i == 0) CL = 0;
					else       CL = RebarData.dDt[i-1];

					if(i == 0)
					{
						if(RebarData.dDt[i] < CL)
							iChkResult |= 4;
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
				for(int i=0; i<rData.nBotLayer ; i++)
				{      
					RebarData.iBarNum[1][i] = (int)rData.dBotBarNum[iPosi][i];
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
					for(int i=0; i<rData.nBotLayer; i++)
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

				// Check Stirrup Bar
				if(rData.bChkSubBar[iPosi])
				{
					m_cmbSSize.GetLBText(m_cmbSSize.GetCurSel(), RebarData.strSubBarNa);

					if(rData.dSubBarNum[iPosi]<= 0.)    iChkResult |= 16384; // m_cStirrupNum.GetEditValue()

					if(rData.dSubBarDist[iPosi] <= 0.)  iChkResult |= 32768; // m_cStirrupSpace.GetEditValue()

					//         CString strSubBarNa;
					//         nIndex = m_nStiCBox[iPosi]; // m_cmbSSize.GetCurSel();
					//         if(nIndex < 0)  iChkResult |= 65536;

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

	return TRUE;
}

BOOL CDgnRCRatingPlateRebarDlg::CheckPosiInputData_New(T_RRPL_SUB_D &rData)
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
		if(rData.dTopBarNum[0][j]!=0.0) return FALSE; 
		if(rData.strTopBarNa1[0][j]!=_T("")) return FALSE; 
		if(rData.strTopBarNa2[0][j]!=_T("")) return FALSE; 
	}
	
	for(int j=0; j<rData.nBotLayer; j++)
	{
		if(rData.dDb[0][j]!=0.0) return FALSE; 
		if(rData.dBotBarNum[0][j]!=0.0) return FALSE; 
		if(rData.strBotBarNa1[0][j]!=_T("")) return FALSE; 
		if(rData.strBotBarNa2[0][j]!=_T("")) return FALSE; 
	}

	return TRUE;
}

BOOL CDgnRCRatingPlateRebarDlg::CheckRebarData_Stirrup(T_RRPL_SUB_D &rData)
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

void CDgnRCRatingPlateRebarDlg::CalcAs(T_RRPL_SUB_D& RchkBeamD, double& dAsTop, double& dAsBot)
{
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	m_pDoc->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);

	for(int i=0; i<T_RCHK_BEAM_LAYER; i++)
	{
		// Top.
		int nTopBarNum=  (int)RchkBeamD.dTopBarNum[0][i];
		if(nTopBarNum > 0)
		{
			double dAs1=0.0, dAs2=0.0;
			if(RchkBeamD.strTopBarNa1[0][i]!=_T(""))  dAs1 = m_pDoc->m_pMatlDB->Get_RebarArea(RchkBeamD.strTopBarNa1[0][i]);
			if(RchkBeamD.strTopBarNa2[0][i]!=_T(""))  dAs2 = m_pDoc->m_pMatlDB->Get_RebarArea(RchkBeamD.strTopBarNa2[0][i]);
			if(dAs2==0.0) dAs2 = dAs1;
			// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
			double dBarNum1 = RchkBeamD.dTopBarNum[0][i]/2.;
			double dBarNum2 = dBarNum1;

			dAsTop += (dAs1*dBarNum1 + dAs2*dBarNum2);
		}
		// Bottom.
		int nBotBarNum=  (int)RchkBeamD.dBotBarNum[0][i];
		if(nBotBarNum > 0)
		{
			double dAs1=0.0, dAs2=0.0;
			if(RchkBeamD.strBotBarNa1[0][i]!=_T(""))  dAs1 = m_pDoc->m_pMatlDB->Get_RebarArea(RchkBeamD.strBotBarNa1[0][i]);
			if(RchkBeamD.strBotBarNa2[0][i]!=_T(""))  dAs2 = m_pDoc->m_pMatlDB->Get_RebarArea(RchkBeamD.strBotBarNa2[0][i]);
			if(dAs2==0.0) dAs2 = dAs1;
			// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
			double dBarNum1 = RchkBeamD.dBotBarNum[0][i]/2.;
			double dBarNum2 = dBarNum1;

			dAsBot += (dAs1*dBarNum1 + dAs2*dBarNum2);
		}
	}
}

void CDgnRCRatingPlateRebarDlg::PostNcDestroy() 
{	
	if(m_TopGrid != NULL){ delete m_TopGrid; m_TopGrid=NULL; }
	if(m_BotGrid != NULL){ delete m_BotGrid; m_BotGrid=NULL; }

	CDialogMove::PostNcDestroy();
}


BEGIN_MESSAGE_MAP(CDgnRCRatingPlateRebarDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnRCRatingPlateRebarDlg)
	ON_CBN_SELCHANGE(IDC_DGN_Plate_TOP_LAYER_CMB,  OnSelchangeDgnRcTopLayerCmb)
	ON_CBN_SELCHANGE(IDC_DGN_Plate_BOT_LAYER_CMB,  OnSelchangeDgnRcBotLayerCmb)
	ON_BN_CLICKED(IDC_DGN_Plate_STIRRUP_CHK,       OnClickedShearChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateRebarDlg message handlers

BOOL CDgnRCRatingPlateRebarDlg::OnInitDialog() 
{
	T_RCHK_BEAM  BeamData;
	BeamData.Initialize();

	if (m_TopGrid == nullptr)
		m_TopGrid = new CDgnBeamGridWndCIVIL(&BeamData);
	if (m_BotGrid == nullptr)
		m_BotGrid = new CDgnBeamGridWndCIVIL(&BeamData);

	CDialogMove::OnInitDialog();

	InitCombo();
	InitGrid();
	InitUnit();
 
	Data2Dlg();
 
	ControlsEnableDisable();

 return TRUE;  // return TRUE unless you set the focus to a control
								// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCRatingPlateRebarDlg::OnOK() // Apply
{
	if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CDgnRCRatingPlateRebarDlg::OnCancel()  //Close
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnRCRatingPlateRebarDlg::OnSelchangeDgnRcTopLayerCmb() 
{
	int nLayer = m_cmbTopLayer.GetCurSel();
	m_TopGrid->SetLayerNum(nLayer); 
}

void CDgnRCRatingPlateRebarDlg::OnSelchangeDgnRcBotLayerCmb() 
{
	int nLayer = m_cmbBotLayer.GetCurSel();
	m_BotGrid->SetLayerNum(nLayer); 
}

void CDgnRCRatingPlateRebarDlg::OnClickedShearChk()
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}