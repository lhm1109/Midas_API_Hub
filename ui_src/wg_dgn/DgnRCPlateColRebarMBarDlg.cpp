// DgnRCPlateColRebarMBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCPlateColRebarMBarDlg.h"

#include "DgnDataCtrl.h"
#include "DgnBeamGridWndCIVIL.h"
#include "DgnRebarTool.h"

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

#define T_RIPC_LAYER 3

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateColRebarMBarDlg dialog


CDgnRCPlateColRebarMBarDlg::CDgnRCPlateColRebarMBarDlg(T_RPCM_D* pData, CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRCPlateColRebarMBarDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_pData = pData;

	m_iThik = 0;

	T_RCHK_BEAM  Data;
	Data.Initialize();

	m_TopGrid = NULL;
	m_BotGrid = NULL;
}

CDgnRCPlateColRebarMBarDlg::~CDgnRCPlateColRebarMBarDlg()
{
	
}

void CDgnRCPlateColRebarMBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCPlateColRebarMBarDlg)  
	DDX_Control(pDX, IDC_DGN_Plate_TOP_LAYER_CMB,        m_cmbTopLayer);
	DDX_Control(pDX, IDC_DGN_Plate_TOP_As_EDT,           m_edtAsTop);	
	DDX_Control(pDX, IDC_DGN_Plate_BOT_LAYER_CMB,        m_cmbBotLayer);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_As_EDT,           m_edtAsBot);

	DDX_Control(pDX, IDC_DGN_Plate_TOP_LAYER_GRID, *m_TopGrid);
	DDX_Control(pDX, IDC_DGN_Plate_BOT_LAYER_GRID, *m_BotGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnRCPlateColRebarMBarDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRCPlateColRebarMBarDlg)  
	ON_CBN_SELCHANGE(IDC_DGN_Plate_TOP_LAYER_CMB,  OnSelchangeDgnRcTopLayerCmb)
	ON_CBN_SELCHANGE(IDC_DGN_Plate_BOT_LAYER_CMB,  OnSelchangeDgnRcBotLayerCmb)  
	ON_BN_CLICKED(IDC_DGN_Plate_NUM,               OnClickedNumCTC)
	ON_BN_CLICKED(IDC_DGN_Plate_CTC,               OnClickedNumCTC)
	ON_MESSAGE(WM_USER_BEFORE_END_EDITING,         OnBeforeEndEditing)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateColRebarMBarDlg message handlers

BOOL CDgnRCPlateColRebarMBarDlg::OnInitDialog() 
{
	T_RCHK_BEAM  Data;
	Data.Initialize();

	if (m_TopGrid == nullptr)
		m_TopGrid = new CDgnBeamGridWndCIVIL(&Data);
	if (m_BotGrid == nullptr)
		m_BotGrid = new CDgnBeamGridWndCIVIL(&Data);

	CChildDialog::OnInitDialog();

	InitCtrl();
		
	InitUnit();

	Data2Dlg();

	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDgnRCPlateColRebarMBarDlg::OnBeforeEndEditing(WPARAM wParam, LPARAM lParam)
{
	Dlg2Data();
	ReCalcAs();
	return 1;
}

void CDgnRCPlateColRebarMBarDlg::OnClickedNumCTC()
{
	SetNumCTC(TRUE);
	ReCalcAs();
}

void CDgnRCPlateColRebarMBarDlg::SetNumCTC(BOOL bClick/*=FALSE*/)
{
	if(bClick) CDlgUtil::CtrlRadioSetCheck(this, m_aRdoNumCTC, 1);// NUM은 사용안하므로..

	int nNumCTC;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoNumCTC, nNumCTC);
	m_TopGrid->SetNumCTC(nNumCTC);
	m_BotGrid->SetNumCTC(nNumCTC);
}

void CDgnRCPlateColRebarMBarDlg::InitCtrl()
{
	m_aRdoNumCTC.RemoveAll();
	m_aRdoNumCTC.Add(IDC_DGN_Plate_NUM);
	m_aRdoNumCTC.Add(IDC_DGN_Plate_CTC);  
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoNumCTC, 0);

	GetDlgItem(IDC_DGN_Plate_NUM)->EnableWindow(FALSE);

	InitCombo();
	InitGrid();  
}

void CDgnRCPlateColRebarMBarDlg::InitCombo()
{
	CString str;

	m_cmbTopLayer.ResetContent();
	m_cmbBotLayer.ResetContent();

	for(int i=0; i<T_RIPC_LAYER+1; i++)  // Layer 3 (0포함)
	{
		str.Format(_T("%d"), i);
		m_cmbTopLayer.AddString(str);
		m_cmbBotLayer.AddString(str);
	}

	m_cmbTopLayer.SetCurSel(1);
	m_cmbBotLayer.SetCurSel(1);
}

void CDgnRCPlateColRebarMBarDlg::InitGrid()
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

void CDgnRCPlateColRebarMBarDlg::InitUnit()
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

void CDgnRCPlateColRebarMBarDlg::ReCalcAs()
{
	if (!GetSafeHwnd() || !IsWindow(GetSafeHwnd())) return;
	
	Dlg2Data();

	double dAsTop = 0.0; // 반영하기
	double dAsBot = 0.0; // 반영하기
    CDgnRebarTool RebarTool;
    RebarTool.CalcPlateAs(*m_pData, dAsTop, dAsBot);

	m_edtAsTop.SetEditUnit(dAsTop);
	m_edtAsBot.SetEditUnit(dAsBot);
}

void CDgnRCPlateColRebarMBarDlg::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoNumCTC, m_pData->bNum ? 0 : 1);

	m_cmbTopLayer.SetCurSel(m_pData->nTopLayer);
	m_cmbBotLayer.SetCurSel(m_pData->nBotLayer);

	SetDataToGrid();  
	ReCalcAs();

	UpdateData(FALSE);	
}

BOOL CDgnRCPlateColRebarMBarDlg::Dlg2Data()
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
		for (int i = 0; i < T_RIPC_LAYER; i++)
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
		for (int i = 0; i < m_pData->nBotLayer; i++)
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
		for (int i = 0; i < T_RIPC_LAYER; i++)
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
		for (int i = 0; i < m_pData->nBotLayer; i++)
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

void CDgnRCPlateColRebarMBarDlg::SetDataToGrid()
{
	int nNumCTC;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoNumCTC, nNumCTC);
	SetNumCTC();
	for(int i=0; i<T_RIPC_LAYER; i++)
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

void CDgnRCPlateColRebarMBarDlg::GetDataFromGrid()
{
	CString str = _T("");
	int nNumCTC;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoNumCTC, nNumCTC);
	for(int i=0; i<T_RIPC_LAYER; i++)
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

	for(int i=0; i<T_RIPC_LAYER; i++)
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

void CDgnRCPlateColRebarMBarDlg::ControlsEnableDisable()
{ 
	

}

void CDgnRCPlateColRebarMBarDlg::PostNcDestroy() 
{	
	if(m_TopGrid != NULL){ delete m_TopGrid; m_TopGrid=NULL; }
	if(m_BotGrid != NULL){ delete m_BotGrid; m_BotGrid=NULL; }

	CChildDialog::PostNcDestroy();
}

void CDgnRCPlateColRebarMBarDlg::OnOK()
{
	if(!Dlg2Data()) return;

	CChildDialog::OnOK();
}

void CDgnRCPlateColRebarMBarDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CChildDialog::OnCancel();
}

void CDgnRCPlateColRebarMBarDlg::OnSelchangeDgnRcTopLayerCmb() 
{
	int nLayer = m_cmbTopLayer.GetCurSel();
	m_TopGrid->SetLayerNum(nLayer); 
	ReCalcAs();
}

void CDgnRCPlateColRebarMBarDlg::OnSelchangeDgnRcBotLayerCmb() 
{
	int nLayer = m_cmbBotLayer.GetCurSel();
	m_BotGrid->SetLayerNum(nLayer); 
	ReCalcAs();
}

void CDgnRCPlateColRebarMBarDlg::OnDestroy() 
{
	Dlg2Data();
	CChildDialog::OnDestroy();
}
