// CSeisCvlForceDirDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlForceDirDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\InitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\QSort.h"
#include "..\wg_dgn\BrdgSeisEvalKISTEC2015.h"
#include "..\wg_dgn\BrdgSeisEvalKISTEC2019.h"
#include "..\wg_dgn\BrdgSeisEvalKALIS2023.h"
#include "..\wg_dgn\BrdgSeisEvalKEC2012.h"

#include "..\wg_main\wg_mainRes2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSeisCvlForceDirDlg dialog
CSeisCvlForceDirDlg::CSeisCvlForceDirDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisCvlForceDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCvlForceDirDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nCalcMethod = 0;

	m_EvgpK = 0;
	m_nEvgpColm = 0;
	m_nDir = 0;
	m_nBehav = 0;
	m_dHe1 = 0.0;
	m_dHe2 = 0.0;
	
	m_pDoc = CDBDoc::GetDocPoint();

	m_pGrid = NULL;

	m_TopElemK = 0;
	m_BotElemK = 0;
	m_dTopMy = 0.0;
	m_dBotMy = 0.0;
	m_dHc = 0.0;
	m_dHp = 0.0;
}

CSeisCvlForceDirDlg::~CSeisCvlForceDirDlg()
{
	if (m_pGrid) { delete m_pGrid; m_pGrid = NULL; }
}


void CSeisCvlForceDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataExchange)
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_LCOM_CMB,     m_cmbLcom);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_BEHAV_EDT,    m_edtBehav);
	DDX_Radio  (pDX, IDC_TM_SEIS_CVL_MISC_CALC_RDO1,    m_nCalcMethod);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_MPHI_TOP_CMB, m_cmbMPhiTop);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_MPHI_BOT_CMB, m_cmbMPhiBot);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_BEARING_CMB,  m_cmbBearing);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_SUM_EDT,      m_edtSum);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_SUM_UNT,      m_untSum);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_HE1_EDT,      m_edtHe1);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_HE1_UNT,      m_untHe1);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_HE2_EDT,      m_edtHe2);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_HE2_UNT,      m_untHe2);

	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_VIEW, m_ContourView);

	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

void CSeisCvlForceDirDlg::InitUnit()
{
	m_edtSum.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSum.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtHe1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHe1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtHe2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHe2.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CSeisCvlForceDirDlg::InitCombo()
{
	// Load Case/Combination
	// Response Spectrum Load Case 나오도록 변경 
//   m_cmbLcom.ResetContent();
//   m_cmbLcom.SetLoadType(D_SELECTLOAD_SPECTRUM);
//   m_cmbLcom.SetCurSel(0);

	m_cmbLcom.ResetContent();

	int nBridgeDir = 0;
	T_SPLC_D SplcD; SplcD.Initialize();
	CArray<T_SPLC_K, T_SPLC_K> aSplcK;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	for(int i=0; i<aSplcK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD)) continue;
		if(!m_pDoc->m_pAttrCtrl->GetBridgeDir(SplcD, nBridgeDir)) continue;

		if(m_nDir == 0)
		{
			if(nBridgeDir == 0)
			{
				m_cmbLcom.SetItemData(m_cmbLcom.AddString(SplcD.LoadCaseName), aSplcK[i]);
			}
		}
		else
		{
			if(nBridgeDir == 1)
			{
				m_cmbLcom.SetItemData(m_cmbLcom.AddString(SplcD.LoadCaseName), aSplcK[i]);
			}
		}
	}

	if(m_cmbLcom.GetCount() > 0) m_cmbLcom.SetCurSel(0);
	  
	// M-Phi Curve
	m_cmbMPhiTop.ResetContent();
	m_cmbMPhiBot.ResetContent();

	CArray<T_MPCC_K,T_MPCC_K> aMpccK;
	m_pDoc->m_pAttrCtrl2->GetMpccKeyList(aMpccK);

	T_MPCC_D MpccD; MpccD.Initialize();

	for(int i=0; i<aMpccK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(aMpccK[i], MpccD)) continue;
		if(!m_pDoc->m_pAttrCtrl2->ExistEvgp(MpccD.EvgpK)) continue;
		if(MpccD.EvgpK != m_EvgpK || MpccD.nEvgpColm != m_nEvgpColm) continue;

		if(MpccD.nEvgpPos == 0) m_cmbMPhiTop.SetItemData(m_cmbMPhiTop.AddString(MpccD.strName), aMpccK[i]);
		else                    m_cmbMPhiBot.SetItemData(m_cmbMPhiBot.AddString(MpccD.strName), aMpccK[i]);		
	}

	SetMPhiCbxInitSel(m_cmbMPhiTop, 0);
	SetMPhiCbxInitSel(m_cmbMPhiBot, 1);

	// Total Number of Bearing Rows
	m_cmbBearing.ResetContent();
	m_cmbBearing.AddString(_LS(IDS_WG_TREEMENU_FORCE_DIR_BEARING_ROW1));
	m_cmbBearing.AddString(_LS(IDS_WG_TREEMENU_FORCE_DIR_BEARING_ROW2));
	m_cmbBearing.SetCurSel(0);
}

void CSeisCvlForceDirDlg::SetMPhiCbxInitSel(MComboBox& Cobx, int nPos)
{
	//nPos : 0=Top, 1=Bottom
	int nCnt = Cobx.GetCount();
	T_MPCC_K MpccK;
	T_MPCC_D MpccD;
	for (int i = 0; i < nCnt; i++)
	{
		MpccK = CDlgUtil::CobxGetCurSelItemData(Cobx, i);
				
		MpccD.Initialize();
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) ASSERT(0);

		if(!MpccD.bEvgp) continue;

		if(MpccD.nEvgpPos == nPos && MpccD.EvgpK == m_EvgpK)
		{
			Cobx.SetCurSel(i);
			return;
		}
	}
	Cobx.SetCurSel(-1);
}

void CSeisCvlForceDirDlg::InitView()
{
	m_ContourView.SetParent(this);
	m_ContourView.Init();
	//m_ContourView.SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, 40, 15);
	m_ContourView.SetMode(EN_DRAW_WINMODE_SELECT);
	//m_ContourView.SetContext(TRUE); //이거 주석 풀면 컨텍스트 메뉴 열림
	//m_ContourView.SetDrawType(DRAW_EDIT_BTN_TYPE_XY);
	m_ContourView.MakeDrawUnit();
}

void CSeisCvlForceDirDlg::InitGrid()
{
	m_pGrid->Initialize(this);
}

void CSeisCvlForceDirDlg::InitDialogTitle()
{
	if(m_nDir == 0) SetWindowText(_LS(IDS_WG_TREEMENU_FORCE_DIR_LONGI));
	else            SetWindowText(_LS(IDS_WG_TREEMENU_FORCE_DIR_TRANS));
}

void CSeisCvlForceDirDlg::AlignControl()
{
	CArray<UINT,UINT> aCanti;  aCanti.RemoveAll();

	aCanti.Add(IDC_TM_SEIS_CVL_MISC_BEARING_STC);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_BEARING_CMB);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_SUM_STC);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_SUM_EDT);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_SUM_UNT);

	CRect rRef, rMove;  
	int nDistX, nDistY;

	GetDlgItem(IDC_TM_SEIS_CVL_MISC_CALC_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_BEARING_STC)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aCanti, nDistX, nDistY);

	CWnd* pWndLast = GetDlgItem(IDC_TM_SEIS_CVL_MISC_OK_BTN);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CSeisCvlForceDirDlg::ControlsShowHide()
{
	CArray<UINT,UINT> aRahmen; aRahmen.RemoveAll();
	CArray<UINT,UINT> aCanti;  aCanti.RemoveAll();

	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_CALC_STC);
	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_CALC_RDO1);
	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_CALC_RDO2);
	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_MPHI_GRP);
	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_MPHI_TOP_STC);
	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_MPHI_TOP_CMB);
	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_MPHI_BOT_STC);
	aRahmen.Add(IDC_TM_SEIS_CVL_MISC_MPHI_BOT_CMB);

	aCanti.Add(IDC_TM_SEIS_CVL_MISC_BEARING_STC);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_BEARING_CMB);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_SUM_STC);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_SUM_EDT);
	aCanti.Add(IDC_TM_SEIS_CVL_MISC_SUM_UNT);

	CDlgUtil::CtrlShowHide(this, aRahmen, m_nBehav==0);
	CDlgUtil::CtrlShowHide(this, aCanti,  m_nBehav==1);
}

void CSeisCvlForceDirDlg::EnableDisableControls()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_MPHI_GRP)->EnableWindow(m_nCalcMethod==1);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_MPHI_TOP_STC)->EnableWindow(m_nCalcMethod==1);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_MPHI_TOP_CMB)->EnableWindow(m_nCalcMethod==1);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_MPHI_BOT_STC)->EnableWindow(m_nCalcMethod==1);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_MPHI_BOT_CMB)->EnableWindow(m_nCalcMethod==1);

	int nBearing = m_cmbBearing.GetCurSel();
	
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_SUM_STC)->EnableWindow(nBearing==1);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_SUM_EDT)->EnableWindow(nBearing==1);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_SUM_UNT)->EnableWindow(nBearing==1);

	GetDlgItem(IDC_TM_SEIS_CVL_MISC_HE2_STC)->EnableWindow(m_nBehav==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_HE2_EDT)->EnableWindow(m_nBehav==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_HE2_UNT)->EnableWindow(m_nBehav==0);
}

void CSeisCvlForceDirDlg::GetEffLengthParam()
{
	UpdateData(TRUE);
	
	if(m_EvgpK == 0) { ASSERT(0); return; }
	if(!m_pDoc->IsPostMode()) return;

	///////////////////////////////////////////////////////////////////////////////
	T_EVGP_D EvgpD; EvgpD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(m_EvgpK, EvgpD)) { ASSERT(0); return; }

	UINT nLoadType;
	UINT nLoadKey;
	UINT nMnMxType;
	//if(!m_cmbLcom.GetSelectedLoad(nLoadType, nLoadKey, &nMnMxType)) { ASSERT(0); return; }

	nLoadType = D_LOADCASE_SPECTRUM;
	nLoadKey = m_cmbLcom.GetItemData(m_cmbLcom.GetCurSel());
	nMnMxType = 0;

	T_LCOM_D LcomD; LcomD.Initialize();
	if(!m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadType, nLoadKey, LcomD)) { ASSERT(0); return; }
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	T_ELEM_K ElemK;
	T_ELEM_D ElemD; ElemD.Initialize();

	T_NODE_D NodeD1; NodeD1.Initialize();
	T_NODE_D NodeD2; NodeD2.Initialize();

	T_STRB_D StrbD; StrbD.Initialize();
	T_STRB_D StrbDMax; StrbDMax.Initialize();
	T_STRB_D StrbDMin; StrbDMin.Initialize();

	// Top
	m_TopElemK = m_pDoc->m_pAttrCtrl2->GetTopElemK(EvgpD, m_nEvgpColm);
	if(!m_pDoc->m_pAttrCtrl->GetElem(m_TopElemK, ElemD)) { ASSERT(0); return; }
	
	if(!m_pDoc->m_pPostCtrl->GetStrbNew(m_TopElemK, &StrbDMax, &StrbDMin, &StrbD, TRUE)) { ASSERT(0); return; }

	if     (nMnMxType == 1) StrbD = StrbDMax;
	else if(nMnMxType == 2) StrbD = StrbDMin;

	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }

	if(m_nDir == 0)
	{
		if(NodeD1.z > NodeD2.z) m_dTopMy = StrbD.dblForce[0][4];
		else                    m_dTopMy = StrbD.dblForce[3][10];
	}
	else
	{
		if(NodeD1.z > NodeD2.z) m_dTopMy = StrbD.dblForce[0][5];
		else                    m_dTopMy = StrbD.dblForce[3][11];
	}	

	// Bot
	m_BotElemK = m_pDoc->m_pAttrCtrl2->GetBotElemK(EvgpD, m_nEvgpColm);
	if(!m_pDoc->m_pAttrCtrl->GetElem(m_BotElemK, ElemD)) { ASSERT(0); return; }

	if(!m_pDoc->m_pPostCtrl->GetStrbNew(m_BotElemK, &StrbDMax, &StrbDMin, &StrbD, TRUE)) { ASSERT(0); return; }

	if     (nMnMxType == 1) StrbD = StrbDMax;
	else if(nMnMxType == 2) StrbD = StrbDMin;
		
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }

	if(m_nDir == 0)
	{
		if(NodeD1.z > NodeD2.z) m_dBotMy = StrbD.dblForce[3][10];
		else                    m_dBotMy = StrbD.dblForce[0][4];
	}
	else
	{
		if(NodeD1.z > NodeD2.z) m_dBotMy = StrbD.dblForce[3][11];
		else                    m_dBotMy = StrbD.dblForce[0][5];
	}	

	// Mid
	CArray<T_ELEM_K, T_ELEM_K> aColumn;
	aColumn.Copy(EvgpD.aEvgpColm[m_nEvgpColm].aColumn);

	double dMidMy = 0.0;

	int nColumn = aColumn.GetSize();
	if(nColumn/2 >= 1) 
	{
		T_ELEM_D ElemD; ElemD.Initialize();
		T_NODE_D NodeD; NodeD.Initialize();
		CArray<T_ELEM_K, T_ELEM_K> aElemK; aElemK.RemoveAll();
		CArray<double, double> aNodePos; aNodePos.RemoveAll();

		for(int i=0; i<aColumn.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetElem(aColumn[i], ElemD)) continue;
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD)) continue;

			aElemK.Add(aColumn[i]);
			aNodePos.Add(NodeD.z);
		}
		
		CQSort::QSortDblWithIndex(aElemK.GetData(), aNodePos.GetData(), aElemK.GetSize());
		
		ElemK = aElemK[nColumn/2-1];
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return; }

		if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbDMax, &StrbDMin, &StrbD, TRUE)) { ASSERT(0); return; }

		if     (nMnMxType == 1) StrbD = StrbDMax;
		else if(nMnMxType == 2) StrbD = StrbDMin;

		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }

		if(m_nDir == 0)
		{
			if(NodeD1.z > NodeD2.z) dMidMy = StrbD.dblForce[0][4];
			else                    dMidMy = StrbD.dblForce[3][10];
		}
		else
		{
			if(NodeD1.z > NodeD2.z) dMidMy = StrbD.dblForce[0][5];
			else                    dMidMy = StrbD.dblForce[3][11];
		}		
	}
	
	// 거동특성
	if(fabs(m_dTopMy) > fabs(dMidMy)) 
	{
		if(fabs(m_dBotMy) * 0.5 >= fabs(m_dTopMy))
		{
			m_nBehav = 1;
		}
		else
		{
			m_nBehav = 0;
		}
	}
	else
	{
		m_nBehav = 1;
	}

	// 기둥 높이
	m_dHc = 0.0;
	for(int i=0; i<aColumn.GetSize(); i++)
	{
		m_dHc += m_pDoc->calcLAVElem(aColumn[i]);
	}

	// Pier Cap 높이
	m_dHp = 0.0;

	double hi, bi, hj, bj;
	
	if(EvgpD.bPierCap)
	{
		for(int i=0; i<EvgpD.aPierCap.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetElem(EvgpD.aPierCap[i], ElemD)) continue;
			if(!m_pDoc->m_pAttrCtrl->GetSectDimen(ElemD.elpro, hi, bi, hj, bj)) continue;

			m_dHp = __max(m_dHp, hi);
			m_dHp = __max(m_dHp, hj);			
		}
	}
}


BEGIN_MESSAGE_MAP(CSeisCvlForceDirDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisCvlForceDirDlg)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_LCOM_CMB,      OnCvlForceDirLcomCmb)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_LCOM_BTN,         OnCvlForceDirLcomBtn)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_BEARING_CMB,   OnCvlForceDirBearingCmb)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_CALC_RDO1,        OnCvlForceDirCalcMethodRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_CALC_RDO2,        OnCvlForceDirCalcMethodRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_CALC_BTN,         OnCvlForceDirCalcBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_OK_BTN,           OnCvlForceDirOKBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_CANCEL_BTN,       OnCvlForceDirCancelBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlForceDirDlg message handlers

BOOL CSeisCvlForceDirDlg::OnInitDialog() 
{
	if (m_pGrid == nullptr)
		m_pGrid = new CSeisCvlForceDirGrid();

	CDialogMove::OnInitDialog();
	
	InitUnit();
	InitCombo();
	InitGrid();
	InitView();
	InitDialogTitle();

	AlignControl();
		
	OnCvlForceDirLcomCmb();

	GetDlgItem(IDC_TM_SEIS_CVL_MISC_LCOM_BTN)->ShowWindow(SW_HIDE); // RS Case 만 보여주는 것으로 변경되면서 숨김
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlForceDirDlg::OnCvlForceDirLcomCmb()
{
	GetEffLengthParam();
	ControlsShowHide();
	EnableDisableControls();

	CString strBehav;
	if(m_nBehav == 0) strBehav = _LS(IDS_WG_TREEMENU_FORCE_DIR_BEHAV_RAHMEN);
	else              strBehav = _LS(IDS_WG_TREEMENU_FORCE_DIR_BEHAV_CANTI);
	
	m_edtBehav.SetWindowText(strBehav);

	UpdateGraphData();
	
	m_pGrid->SetData2Grid(m_TopElemK, m_BotElemK, m_dTopMy, m_dBotMy);
}

void CSeisCvlForceDirDlg::OnCvlForceDirLcomBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEISEVAL_LOAD_COMB,0)); 
}

void CSeisCvlForceDirDlg::OnCvlForceDirBearingCmb()
{
	EnableDisableControls();
}

void CSeisCvlForceDirDlg::OnCvlForceDirCalcMethodRdo()
{
	EnableDisableControls();
}

void CSeisCvlForceDirDlg::OnCvlForceDirCalcBtn()
{
	UpdateData(TRUE);

	m_dHe1 = m_dHe2 = 0.0;

	if(m_nBehav == 0) // 라멘거동
	{
		if(m_nCalcMethod == 1)
		{
			if(m_cmbMPhiTop.GetCurSel() == -1)
			{
				AfxMessageBox(_LS(IDS_WG_TREEMENU_MPHI_TOP_INVALID));
				return ;
			}
			else if(m_cmbMPhiBot.GetCurSel() == -1)
			{
				AfxMessageBox(_LS(IDS_WG_TREEMENU_MPHI_BOT_INVALID));
				return ;
			}
		}

		T_MPCC_K TopMpccK;
		T_MPCC_K BotMpccK;

		TopMpccK = m_cmbMPhiTop.GetItemData(m_cmbMPhiTop.GetCurSel());
		BotMpccK = m_cmbMPhiBot.GetItemData(m_cmbMPhiBot.GetCurSel());

		EN_LOCAL_DIR enDir;
		if(m_nDir == 0) enDir = LONGI;
		else            enDir = TRANS; 

		EN_BEHAVIOR_TYPE enBehavior;
		if(m_nBehav == 0) enBehavior = RAHMEN;
		else              enBehavior = CANTILEVER;

		double dHe[2];
		dHe[0] = dHe[1] = 0.0;
		
		auto* pSeisEval = [&]() -> CBrdgSeisEval*
		{
			T_BSED_D BsedD; BsedD.Initialize();
			m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD);
			switch(BsedD.nDesignCode)
			{
				case KEC_2012:	    return new CBrdgSeisEvalKEC2012();
				case KISTEC_2015:	return new CBrdgSeisEvalKISTEC2015();
				case KISTEC_2019:	return new CBrdgSeisEvalKISTEC2019();
				case KALIS_2023:	return new CBrdgSeisEvalKALIS2023();
				default:			ASSERT(0);  return new CBrdgSeisEvalKISTEC2015();
			}
		}();

		if(pSeisEval != nullptr)
		{
			pSeisEval->CalcHe(TopMpccK, BotMpccK, enDir, enBehavior, m_nCalcMethod, m_dHc, dHe);
			delete pSeisEval;
			pSeisEval = nullptr;
		}

		m_dHe1 = dHe[0];
		m_dHe2 = dHe[1];
	}
	else // 캔틸레버거동
	{
		int nBearing = m_cmbBearing.GetCurSel();
		double dSum = m_edtSum.GetEditValue();

		m_dHe1 = m_dHc + m_dHp;

		if(nBearing == 1) // 2열
		{
			m_dHe1 += dSum;
		}
	}
	
	m_edtHe1.SetEditUnit(m_dHe1);
	m_edtHe2.SetEditUnit(m_dHe2);
}

void CSeisCvlForceDirDlg::UpdateGraphData()
{
	double dZero = m_pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	T_EVGP_D EvgpD; EvgpD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(m_EvgpK, EvgpD)) { ASSERT(0); return; }

	UINT nLoadType;
	UINT nLoadKey;
	UINT nMnMxType;
	//if(!m_cmbLcom.GetSelectedLoad(nLoadType, nLoadKey, &nMnMxType)) { ASSERT(0); return; }

	nLoadType = D_LOADCASE_SPECTRUM;
	nLoadKey = m_cmbLcom.GetItemData(m_cmbLcom.GetCurSel());
	nMnMxType = 0;

	T_LCOM_D LcomD; LcomD.Initialize();
	if(!m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadType, nLoadKey, LcomD)) { ASSERT(0); return; }
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	T_ELEM_D ElemD; ElemD.Initialize();

	T_NODE_D NodeD1; NodeD1.Initialize();
	T_NODE_D NodeD2; NodeD2.Initialize();

	T_STRB_D StrbD; StrbD.Initialize();
	T_STRB_D StrbDMax; StrbDMax.Initialize();
	T_STRB_D StrbDMin; StrbDMin.Initialize();

	CMap<T_NODE_K, T_NODE_K, int, int> NodeMap;
	int nTemp = 0;

	std::vector<std::pair<double,double>> aPairXY;

	double dX = 0.0;
	double dY = 0.0;

	CArray<T_ELEM_K, T_ELEM_K> aColumn;
	aColumn.Copy(EvgpD.aEvgpColm[m_nEvgpColm].aColumn);

	for(int i=0; i<aColumn.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetElem(aColumn[i], ElemD)) continue;

		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }

		if(!m_pDoc->m_pPostCtrl->GetStrbNew(aColumn[i], &StrbDMax, &StrbDMin, &StrbD, TRUE)) continue;

		if     (nMnMxType == 1) StrbD = StrbDMax;
		else if(nMnMxType == 2) StrbD = StrbDMin;

		dX = NodeD1.z;
		if(m_nDir == 0) dY = StrbD.dblForce[0][4];
		else          	dY = StrbD.dblForce[0][5];

		if(NodeMap.Lookup(ElemD.elnod[0], nTemp))
		{
			aPairXY.push_back(make_pair(dX+dZero, dY));
		}
		else
		{
			NodeMap.SetAt(ElemD.elnod[0], nTemp);

			aPairXY.push_back(make_pair(dX, dY));
		}

		dX = NodeD2.z;
		if(m_nDir == 0) dY = StrbD.dblForce[3][10];
		else          	dY = StrbD.dblForce[3][11];		

		if(NodeMap.Lookup(ElemD.elnod[1], nTemp))
		{
			aPairXY.push_back(make_pair(dX+dZero, dY));
		}
		else
		{
			NodeMap.SetAt(ElemD.elnod[1], nTemp);

			aPairXY.push_back(make_pair(dX, dY));
		}
	}

	std::sort(aPairXY.begin(), aPairXY.end());

	std::vector<double> aX, aY;
	for(auto EachPair : aPairXY)
	{
		aX.push_back(EachPair.first);
		aY.push_back(EachPair.second);
	}

	m_ContourView.SetData(aX, aY);
}

void CSeisCvlForceDirDlg::OnCvlForceDirOKBtn()
{
	CDialogMove::OnOK();
}

void CSeisCvlForceDirDlg::OnCvlForceDirCancelBtn()
{
	CDialogMove::OnCancel();
}

