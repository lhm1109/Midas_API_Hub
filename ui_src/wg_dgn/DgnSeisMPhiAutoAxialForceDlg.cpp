// DgnSeisMPhiAutoAxialForceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisMPhiAutoAxialForceDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiAutoAxialForceDlg dialog

CDgnSeisMPhiAutoAxialForceDlg::CDgnSeisMPhiAutoAxialForceDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisMPhiAutoAxialForceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisMPhiAutoAxialForceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDoc = CDBDoc::GetDocPoint();

	m_EvgpK = 0;
	m_nEvgpColm = 0;
	m_nEvgpPos = 0;
	m_dAxialForce = 0.0;
}

CDgnSeisMPhiAutoAxialForceDlg::~CDgnSeisMPhiAutoAxialForceDlg()
{
}

void CDgnSeisMPhiAutoAxialForceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisMPhiAutoAxialForceDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_LCOM_CMB,          m_cmbLcom);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_AXIAL_FORCE_EDT,   m_edtAxialForce);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_AXIAL_FORCE_UNT,   m_untAxialForce);
	//}}AFX_DATA_MAP
}

void CDgnSeisMPhiAutoAxialForceDlg::InitUnit()
{
	m_edtAxialForce.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untAxialForce.SetUnitType(D_UNITSYS_BASE_FORCE);
}

void CDgnSeisMPhiAutoAxialForceDlg::InitCombo()
{
	m_cmbLcom.SetLoadType(D_SELECTLOAD_ALLWITHENVELOPE);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbLcom);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiAutoAxialForceDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisMPhiAutoAxialForceDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisMPhiAutoAxialForceDlg)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_LCOM_CMB,      OnDgnSeisMPhiLcomCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_LCOM_BTN,         OnDgnSeisMPhiLcomBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_OK_BTN,           OnDgnSeisMPhiOKBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CANCEL_BTN,       OnDgnSeisMPhiCanCelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiAutoAxialForceDlg message handlers

BOOL CDgnSeisMPhiAutoAxialForceDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitUnit();
	InitCombo();

	OnDgnSeisMPhiLcomCmb();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisMPhiAutoAxialForceDlg::OnDgnSeisMPhiLcomCmb()
{
	UpdateData(TRUE);

	if(m_EvgpK == 0) { ASSERT(0); return; }

	///////////////////////////////////////////////////////////////////////////////
	T_EVGP_D EvgpD; EvgpD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(m_EvgpK, EvgpD)) { ASSERT(0); return; }
	if(EvgpD.nType == 1) { ASSERT(0); return; }
	
	T_ELEM_K ElemK;
	T_ELEM_D ElemD; ElemD.Initialize();
	
	if(m_nEvgpPos == 0) ElemK = m_pDoc->m_pAttrCtrl2->GetTopElemK(EvgpD, m_nEvgpColm);
	else                ElemK = m_pDoc->m_pAttrCtrl2->GetBotElemK(EvgpD, m_nEvgpColm);
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return; }

	UINT nLoadType;
	UINT nLoadKey;
	UINT nMnMxType;
	if(!m_cmbLcom.GetSelectedLoad(nLoadType, nLoadKey, &nMnMxType)) { ASSERT(0); return; }

	T_LCOM_D LcomD; LcomD.Initialize();
	if(!m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadType, nLoadKey, LcomD)) { ASSERT(0); return; }
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	T_STRB_D StrbD; StrbD.Initialize();
	T_STRB_D StrbDMax; StrbDMax.Initialize();
	T_STRB_D StrbDMin; StrbDMin.Initialize();

	if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbDMax, &StrbDMin, &StrbD, TRUE)) { ASSERT(0); return; }

	if     (nMnMxType == 1) StrbD = StrbDMax;
	else if(nMnMxType == 2) StrbD = StrbDMin;
		
	T_NODE_D NodeD1; NodeD1.Initialize();
	T_NODE_D NodeD2; NodeD2.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }

	if(m_nEvgpPos == 0)
	{
		if(NodeD1.z > NodeD2.z) m_dAxialForce = -StrbD.dblForce[0][0];
		else                    m_dAxialForce = -StrbD.dblForce[3][6];
	}
	else
	{
		if(NodeD1.z > NodeD2.z) m_dAxialForce = -StrbD.dblForce[3][6];
		else                    m_dAxialForce = -StrbD.dblForce[0][0];
	}	

	m_edtAxialForce.SetEditUnit(m_dAxialForce);

	UpdateData(FALSE);
}

void CDgnSeisMPhiAutoAxialForceDlg::OnDgnSeisMPhiLcomBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_COMBINATION,0)); 
}

void CDgnSeisMPhiAutoAxialForceDlg::OnDgnSeisMPhiOKBtn()
{
	CDialogMove::OnOK();
}

void CDgnSeisMPhiAutoAxialForceDlg::OnDgnSeisMPhiCanCelBtn()
{
	CDialogMove::OnCancel();
}