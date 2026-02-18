// DgnSeisFailureModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisFailureModeDlg.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_base\DlgUtil.h"

#include "BrdgSeisEvalKISTEC2015.h"
#include "BrdgSeisEvalKISTEC2019.h"
#include "BrdgSeisEvalKALIS2023.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisFailureModeDlg dialog


CDgnSeisFailureModeDlg::CDgnSeisFailureModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisFailureModeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisFailureModeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nDir = 0;
	m_nPos = 1;
	m_bBending = TRUE;
	m_bShear = TRUE;

	m_pDoc = CDBDoc::GetDocPoint();

	m_bSetEmf = FALSE;
	m_strPath = _T("");

	m_EvgpKForEmf = NULL;
	m_EvgpColmForEmf = 0;

	T_BSED_D BsedD; BsedD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD);
	if      (BsedD.nDesignCode == KISTEC_2015) m_pSeisEval = new CBrdgSeisEvalKISTEC2015;
	else if (BsedD.nDesignCode == KISTEC_2019) m_pSeisEval = new CBrdgSeisEvalKISTEC2019;
	else if (BsedD.nDesignCode == KALIS_2023)  m_pSeisEval = new CBrdgSeisEvalKALIS2023;
	else                                       m_pSeisEval = new CBrdgSeisEvalKISTEC2015;
}

CDgnSeisFailureModeDlg::~CDgnSeisFailureModeDlg()
{
	if(m_pSeisEval)
	{
		delete m_pSeisEval;
		m_pSeisEval = nullptr;
	}
}

void CDgnSeisFailureModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisFailureModeDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_FAILURE_MODE_EVAL_GRUP_CMB,     m_cmbEvalGrup);
	DDX_Control(pDX, IDC_DGN_SEIS_FAILURE_MODE_EVGP_COLM_CMB,     m_cmbEvgpColm);
	DDX_Radio  (pDX, IDC_DGN_SEIS_FAILURE_MODE_LONGI_RDO,         m_nDir);
	DDX_Control(pDX, IDC_DGN_SEIS_FAILURE_MODE_BEHAV_EDT,         m_edtBehav);
	DDX_Radio  (pDX, IDC_DGN_SEIS_FAILURE_MODE_POS_TOP_RDO,       m_nPos);
	DDX_Control(pDX, IDC_DGN_SEIS_FAILURE_MODE_RESULT_EDT,        m_edtFailMode);
	DDX_Check  (pDX, IDC_DGN_SEIS_FAILURE_MODE_BENDING_CURVE_CHK, m_bBending);
	DDX_Check  (pDX, IDC_DGN_SEIS_FAILURE_MODE_SHEAR_CURVE_CHK,   m_bShear);
	DDX_Control(pDX, IDC_DGN_SEIS_FAILURE_MODE_VIEW, m_View);
	//}}AFX_DATA_MAP
}

void CDgnSeisFailureModeDlg::InitCombo()
{
	// Evaluation Group
	m_cmbEvalGrup.ResetContent();

	T_EVGP_D EvgpD; EvgpD.Initialize();
	CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);
	
	for(int i=0; i<aEvgpK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[i], EvgpD)) continue;
		if(EvgpD.nType == 1) continue;
		m_cmbEvalGrup.SetItemData(m_cmbEvalGrup.AddString(EvgpD.strName), aEvgpK[i]);
	}

	m_cmbEvalGrup.SetCurSel(0);

	// Column
	m_cmbEvgpColm.ResetContent();

	if(aEvgpK.GetSize() > 0)
	{
		T_EVGP_K EvgpK;
		EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return;

		for(int i=0;i<EvgpD.aEvgpColm.GetSize(); i++)
		{
			m_cmbEvgpColm.SetItemData(m_cmbEvgpColm.AddString(EvgpD.aEvgpColm[i].strName), i);
		}

		m_cmbEvgpColm.SetCurSel(0);
	}
}

void CDgnSeisFailureModeDlg::InitEvgpColmCombo()
{
	// Column
	m_cmbEvgpColm.ResetContent();

	T_EVGP_D EvgpD; EvgpD.Initialize();
	CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);

	if(aEvgpK.GetSize() > 0)
	{
		T_EVGP_K EvgpK;
		EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return;

		for(int i=0;i<EvgpD.aEvgpColm.GetSize(); i++)
		{
			m_cmbEvgpColm.SetItemData(m_cmbEvgpColm.AddString(EvgpD.aEvgpColm[i].strName), i);
		}

		m_cmbEvgpColm.SetCurSel(0);
	}
}

void CDgnSeisFailureModeDlg::InitView()
{   
	m_View.SetParent(this);
	m_View.Init();
	//m_View.SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, 40, 15);
	m_View.SetMode(EN_DRAW_WINMODE_SELECT);
	//m_View.SetContext(TRUE, TRUE);// 이거 주석 풀면 컨텍스트 메뉴 열림
	//m_View.SetDrawType(DRAW_EDIT_BTN_TYPE_XY);
	m_View.MakeDrawUnit();
}

void CDgnSeisFailureModeDlg::MakeEmfAndClose(T_EVGP_K EvgpK, int EvgpColm, int nDir, int nPos, BOOL bBending, BOOL bShear, CString strPath)
{
	m_EvgpKForEmf = EvgpK;
	m_EvgpColmForEmf = EvgpColm;
	m_nDir = nDir;
	m_nPos = nPos;
	m_bBending = bBending;
	m_bShear = bShear;
	
	m_bSetEmf = TRUE;
	m_strPath = strPath;
}

void CDgnSeisFailureModeDlg::MakeEmfAndClose(T_EVGP_K EvgpK, int EvgpColm, CString strPath)
{
	m_EvgpKForEmf = EvgpK;
	m_EvgpColmForEmf = EvgpColm;
	
	m_bSetEmf = TRUE;
	m_strPath = strPath;
}

void CDgnSeisFailureModeDlg::UpdateGraph()
{
	UpdateData(TRUE);

	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

	int nEvgpColm = m_cmbEvgpColm.GetItemData(m_cmbEvgpColm.GetCurSel());

	int nBehav = GetEvgpBehav();
	if(nBehav == 0) m_edtBehav.SetWindowText(_LS(IDS_DGN_SEIS_FAILURE_MODE_BEHAV_RAHMEN));
	else            m_edtBehav.SetWindowText(_LS(IDS_DGN_SEIS_FAILURE_MODE_BEHAV_CANTI));

	EN_LOCAL_DIR enDir;
	if(m_nDir == 0) enDir = LONGI;
	else            enDir = TRANS; 

	EN_POSITION enPos;
	if(m_nPos == 0) enPos = TOP;
	else            enPos = BOTTOM; 

	if(m_bSetEmf)
	{
		ShowWindow(FALSE);
	}

	T_BRDGEVAL_FAILURE_MODE FModeD; FModeD.initialize();
	T_BRDGEVAL_CAPACITY     CapaD;  CapaD.initialize();
	if(!m_pSeisEval->GetFailureData(EvgpK, nEvgpColm, enDir, enPos, FModeD, CapaD)) ASSERT(0);
	
	if(m_bSetEmf)
	{
		ShowWindow(FALSE);
	}

	CString strFailMode;
	if     (FModeD.enFalureMode == TYPE1_BEND)     strFailMode = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND);
	else if(FModeD.enFalureMode == TYPE2_BEND_SHR) strFailMode = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR);
	else if(FModeD.enFalureMode == TYPE3_SHR)      strFailMode = _LS(IDS_DGN_SEIS_FAILURE_MODE_SHR);
	else                                           strFailMode = _T("-");

	m_edtFailMode.SetWindowText(strFailMode);

	//////////////////////
	// Test	
	//if(FModeD.enFalureMode == TYPE0_NONE)
	//{
	//FModeD.enFalureMode = TYPE1_BEND;
	//SetGraphTempData(FModeD.enFalureMode);
	//}
	//else
	//{
	SetGraphData(FModeD, CapaD);
	//}
	//////////////////////
}

void CDgnSeisFailureModeDlg::EnableDisableControls()
{
	int nBehav = GetEvgpBehav();

	if(nBehav == 0)
	{
		GetDlgItem(IDC_DGN_SEIS_FAILURE_MODE_POS_TOP_RDO)->EnableWindow(TRUE);
	}
	else 
	{
		m_nPos = 1;
		GetDlgItem(IDC_DGN_SEIS_FAILURE_MODE_POS_TOP_RDO)->EnableWindow(FALSE);

		UpdateData(FALSE);
	}
}

int CDgnSeisFailureModeDlg::GetEvgpBehav()
{
	UpdateData(TRUE);

	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

	int nEvgpColm = m_cmbEvgpColm.GetItemData(m_cmbEvgpColm.GetCurSel());

	T_EVPM_D EvpmD; EvpmD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetEvpm(EvgpK, nEvgpColm, EvpmD)) return 0;
	
	if(m_nDir == 0) return EvpmD.nBehavL;
	else            return EvpmD.nBehavT;
	
	return 0;
}

void CDgnSeisFailureModeDlg::SetGraphTempData(int nFailMode)
{
	FailureModeDrawD Data;
	FailureModeGraphD EachData;

	nFailMode = 1; 
	if(m_bBending)
	{
		if(nFailMode == TYPE1_BEND)
		{
			EachData.Init();
						
			EachData.aX.push_back(0.0);
			EachData.aY.push_back(0.0);
			
			EachData.aX.push_back(20.0);
			EachData.aY.push_back(25.0);
			
			EachData.aX.push_back(50.0);
			EachData.aY.push_back(35.0);
			
			Data.aData.push_back(EachData);
		}
		else if(nFailMode == TYPE2_BEND_SHR)
		{
			EachData.Init();
			EachData.aX.push_back(0.0);
			EachData.aY.push_back(0.0);
			
			EachData.aX.push_back(13.0);
			EachData.aY.push_back(55.0);
			
			EachData.aX.push_back(80.0);
			EachData.aY.push_back(85.0);
			
			Data.aData.push_back(EachData);
		}
		else if(nFailMode == TYPE3_SHR)
		{
			EachData.Init();
			EachData.aX.push_back(0.0);
			EachData.aY.push_back(0.0);
			
			EachData.aX.push_back(50.0);
			EachData.aY.push_back(80.0);
			
			EachData.aX.push_back(95.0);
			EachData.aY.push_back(100.0);
			
			Data.aData.push_back(EachData);
		}
	}

	if(m_bShear)
	{
		Data.ShearData.Init();
		Data.ShearData.aX.push_back(0.0);
		Data.ShearData.aY.push_back(80.0);

		Data.ShearData.aX.push_back(40.0);
		Data.ShearData.aY.push_back(77.0);

		Data.ShearData.aX.push_back(65.0);
		Data.ShearData.aY.push_back(30.0);

		Data.ShearData.aX.push_back(70);
		Data.ShearData.aY.push_back(30.0);		
	}	

	Data.bLimit = TRUE;
	Data.dLimit = 60.0;

	double dTestVal = 1.0;
	if(m_bBending)
	{
		for (int i = 0; i < Data.aData[0].aX.size(); i++)
		{
			Data.aData[0].aX[i] *= dTestVal;
		}
		for (int i = 0; i < Data.aData[0].aY.size(); i++)
		{
			Data.aData[0].aY[i] *= dTestVal;
		}
	}
	if(m_bShear)
	{
		for (int i = 0; i < Data.ShearData.aX.size(); i++)
		{
			Data.ShearData.aX[i] *= dTestVal;
		}
		for (int i = 0; i < Data.ShearData.aY.size(); i++)
		{
			Data.ShearData.aY[i] *= dTestVal;
		}
	}
	
	Data.dLimit *= dTestVal;

	m_View.SetData(Data);
}

void CDgnSeisFailureModeDlg::SetGraphData(T_BRDGEVAL_FAILURE_MODE& FModeD, T_BRDGEVAL_CAPACITY& CapaD)
{
	int nFailMode = FModeD.enFalureMode;
	FailureModeDrawD Data;
	FailureModeGraphD EachData;

	COLORREF GuideColor = RGB(200,200,200);  

	if(m_bBending)
	{
		if(nFailMode == TYPE1_BEND)
		{
			EachData.Init();

			EachData.aX.push_back(0.0);
			EachData.aY.push_back(0.0);

			EachData.aX.push_back(CapaD.BendCapaD.dDeltay);
			EachData.aY.push_back(CapaD.BendCapaD.dFy);

			EachData.aX.push_back(CapaD.BendCapaD.dDeltau);
			EachData.aY.push_back(CapaD.BendCapaD.dFu);

			Data.aData.push_back(EachData);
		}
		else if(nFailMode == TYPE2_BEND_SHR)
		{
			EachData.Init();
			EachData.aX.push_back(0.0);
			EachData.aY.push_back(0.0);
			
			EachData.aX.push_back(CapaD.BendCapaD.dDeltay);
			EachData.aY.push_back(CapaD.BendCapaD.dFy);      
			
			EachData.aX.push_back(CapaD.BendCapaD.dDeltau);
			EachData.aY.push_back(CapaD.BendCapaD.dFu);      
			
			Data.aData.push_back(EachData);
		}
		else if(nFailMode == TYPE3_SHR)
		{
			EachData.Init();
			EachData.aX.push_back(0.0);
			EachData.aY.push_back(0.0);
			
			EachData.aX.push_back(CapaD.BendCapaD.dDeltay);
			EachData.aY.push_back(CapaD.BendCapaD.dFy);
			
			EachData.aX.push_back(CapaD.BendCapaD.dDeltau);
			EachData.aY.push_back(CapaD.BendCapaD.dFu);
			
			Data.aData.push_back(EachData);
		}
	}

	if(m_bShear)
	{
		double dVn2 = 0.0, dVn5 = 0.0;
		if (CapaD.ShearCapaD.dVn1 > 0.0 || CapaD.ShearCapaD.dVn2 > 0.0)
		{
			double dVn = m_pSeisEval->Get_WallVn(CapaD.ShearCapaD);
			//dVn2 = dVn5 = min(CapaD.ShearCapaD.dVn1, CapaD.ShearCapaD.dVn2);
			dVn2 = dVn5 = dVn;
		}
		else
		{
			dVn2 = CapaD.ShearCapaD.dVc2 + CapaD.ShearCapaD.dVs + CapaD.ShearCapaD.dVp;
			dVn5 = CapaD.ShearCapaD.dVc5 + CapaD.ShearCapaD.dVs + CapaD.ShearCapaD.dVp;
		}

		Data.ShearData.Init();
		Data.ShearData.aX.push_back(0.0);
		Data.ShearData.aY.push_back(dVn2);

		Data.ShearData.aX.push_back(2.0*CapaD.BendCapaD.dDeltay);
		Data.ShearData.aY.push_back(dVn2);

		Data.ShearData.aX.push_back(5.0*CapaD.BendCapaD.dDeltay);
		Data.ShearData.aY.push_back(dVn5);

		Data.ShearData.aX.push_back(8.0*CapaD.BendCapaD.dDeltay);
		Data.ShearData.aY.push_back(dVn5);
	}	

	if(CapaD.BendCapaD.dDeltac <= 0.0)
	{
		Data.bLimit = FALSE;
		Data.dLimit = 0.0;
	}
	else
	{
		Data.bLimit = TRUE;
		Data.dLimit = CapaD.BendCapaD.dDeltac;
	}

	m_View.SetData(Data);
}
/*
void CDgnSeisFailureModeDlg::SetGraphData(T_BRDGEVAL_FAILURE_MODE& FModeD, T_BRDGEVAL_CAPACITY& CapaD)
{
	int nFailMode = FModeD.enFalureMode;
	FailureModeDrawD Data;
	FailureModeGraphD EachData;

	if(m_bBending)
	{
		EachData.Init();
		EachData.aX.push_back(0.0);
		EachData.aY.push_back(0.0);

		EachData.aX.push_back(CapaD.BendCapaD.dFy);
		EachData.aY.push_back(CapaD.BendCapaD.dDeltay);

		if(FModeD.dFn > CapaD.BendCapaD.dFu)
		{
			EachData.aX.push_back(CapaD.BendCapaD.dFu);
			EachData.aY.push_back(CapaD.BendCapaD.dDeltau);

			EachData.aX.push_back(FModeD.dFn);
			EachData.aY.push_back(FModeD.dDeltac);
		}
		else
		{
			EachData.aX.push_back(FModeD.dFn);
			EachData.aY.push_back(FModeD.dDeltac);

			EachData.aX.push_back(CapaD.BendCapaD.dFu);
			EachData.aY.push_back(CapaD.BendCapaD.dDeltau);
		}

		if(nFailMode == TYPE1_BEND)
		{
			EachData.Color = RGB(49,49,254);  
			EachData.GuideColor = RGB(190,190,255);  
			//EachData.strX = _T("c1");
			//EachData.strY = _T("n1");
		}
		else if(nFailMode == TYPE2_BEND_SHR)
		{
			EachData.Color = RGB(246,5,5);
			EachData.GuideColor = RGB(255,190,190);  
			//EachData.strX = _T("c2");
			//EachData.strY = _T("n2");
		}
		else if(nFailMode == TYPE3_SHR)
		{
			EachData.Color = RGB(92,92,92);
			EachData.GuideColor = RGB(190,190,190);  
			//EachData.strX = _T("c3");
			//EachData.strY = _T("n3");
		}

		Data.aData.push_back(EachData);
	}

	if(m_bShear)
	{
		Data.ShearData.aX.push_back(0.0);
		Data.ShearData.aY.push_back(CapaD.ShearCapaD.dVc2 + CapaD.ShearCapaD.dVs + CapaD.ShearCapaD.dVp);

		Data.ShearData.aX.push_back(CapaD.BendCapaD.dDeltay * 2);
		Data.ShearData.aY.push_back(CapaD.ShearCapaD.dVc2 + CapaD.ShearCapaD.dVs + CapaD.ShearCapaD.dVp);

		Data.ShearData.aX.push_back(CapaD.BendCapaD.dDeltay * 5);
		Data.ShearData.aY.push_back(CapaD.ShearCapaD.dVc5 + CapaD.ShearCapaD.dVs + CapaD.ShearCapaD.dVp); // dVc5 = dVc8

		Data.ShearData.aX.push_back(CapaD.BendCapaD.dDeltay * 8);
		Data.ShearData.aY.push_back(CapaD.ShearCapaD.dVc5 + CapaD.ShearCapaD.dVs + CapaD.ShearCapaD.dVp); // dVc5 = dVc8

		Data.ShearData.Color = RGB(172,184,217);
	}	

	m_View.SetData(Data);
}
*/

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisFailureModeDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisFailureModeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisFailureModeDlg)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_FAILURE_MODE_EVAL_GRUP_CMB,     OnDgnSeisFailureModeEvalGrupCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_FAILURE_MODE_EVGP_COLM_CMB,     OnDgnSeisFailureModeEvgpColmCmb)	
	ON_BN_CLICKED(IDC_DGN_SEIS_FAILURE_MODE_LONGI_RDO,            OnDgnSeisFailureModeDirRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_FAILURE_MODE_TRANS_RDO,            OnDgnSeisFailureModeDirRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_FAILURE_MODE_POS_TOP_RDO,          OnDgnSeisFailureModePosRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_FAILURE_MODE_POS_BOT_RDO,          OnDgnSeisFailureModePosRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_FAILURE_MODE_BENDING_CURVE_CHK,    OnDgnSeisFailureModeBendingChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_FAILURE_MODE_SHEAR_CURVE_CHK,      OnDgnSeisFailureModeShearChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_FAILURE_MODE_CLOSE_BTN,            OnDgnSeisFailureModeCloaseBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisFailureModeDlg message handlers

BOOL CDgnSeisFailureModeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCombo();
	InitEvgpColmCombo();
	InitView();

	Data2Dlg();

	UpdateGraph();
	
	if(m_bSetEmf)
	{
		CRect Rect;
		m_View.GetWindowRect(Rect);
		m_View.MakeEmfFile(m_strPath, Rect.Width() * 100, Rect.Height() * 100);
		PostMessage(WM_CLOSE, 0, 0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisFailureModeDlg::Data2Dlg()
{
	UpdateData(FALSE);

	if(m_EvgpKForEmf != NULL)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cmbEvalGrup, m_EvgpKForEmf);
		InitEvgpColmCombo();
		CDlgUtil::CobxSetCurSelItemData(m_cmbEvgpColm, m_EvgpColmForEmf);
	}  
}

void CDgnSeisFailureModeDlg::OnDgnSeisFailureModeEvalGrupCmb()
{
	InitEvgpColmCombo();
	EnableDisableControls();
	UpdateGraph();
}

void CDgnSeisFailureModeDlg::OnDgnSeisFailureModeEvgpColmCmb()
{
	EnableDisableControls();
	UpdateGraph();
}

void CDgnSeisFailureModeDlg::OnDgnSeisFailureModeDirRdo()
{
	EnableDisableControls();
	UpdateGraph();
}

void CDgnSeisFailureModeDlg::OnDgnSeisFailureModePosRdo()
{
	UpdateGraph();
}

void CDgnSeisFailureModeDlg::OnDgnSeisFailureModeBendingChk()
{
	UpdateGraph();
}

void CDgnSeisFailureModeDlg::OnDgnSeisFailureModeShearChk()
{
	UpdateGraph();
}

void CDgnSeisFailureModeDlg::OnDgnSeisFailureModeCloaseBtn()
{
	CDialogMove::OnCancel();
}