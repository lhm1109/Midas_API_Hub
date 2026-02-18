// CRCDgnLcomDlg.cpp: implementation of the CCRCDgnLcomDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CRCDgnLcomDlg.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_cmd\wg_cmd_LoadCombCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCRCDgnLcomDlg::CCRCDgnLcomDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCRCDgnLcomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCRCDgnLcomDlg)
	m_bDead   = 0;
	m_bDesign = 0;
	m_bTemper = 0;
	m_bEarth  = 0;
	m_bFinalA = 0; 
	m_bFinalB = 0;
	m_bFinalC = 0;

	//}}AFX_DATA_INIT
}


void CCRCDgnLcomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenKFacDlg)
	DDX_Check(pDX, IDC_DGN_CRC_DESIGN_LCOM1, m_bDead);
	DDX_Check(pDX, IDC_DGN_CRC_DESIGN_LCOM2, m_bDesign);
	DDX_Check(pDX, IDC_DGN_CRC_DESIGN_LCOM3, m_bTemper);
	DDX_Check(pDX, IDC_DGN_CRC_DESIGN_LCOM4, m_bEarth);
	DDX_Check(pDX, IDC_DGN_CRC_DESIGN_LCOM5, m_bFinalA);
	DDX_Check(pDX, IDC_DGN_CRC_DESIGN_LCOM6, m_bFinalB);
	DDX_Check(pDX, IDC_DGN_CRC_DESIGN_LCOM7, m_bFinalC);
	//}}AFX_DATA_MAP
}


#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CCRCDgnLcomDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CCRCDgnLcomDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCRCDgnLcomDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();


	Init_Data();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCRCDgnLcomDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCRCDgnLcomDlg::OnOK() 
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CLoadCombCtrl LoadCombCtrl;
	LoadCombCtrl.m_nDesignType   = 1; // Concrete
	LoadCombCtrl.Set_LcomType(D_LCOMTYPE_CONCRETE);

	T_POSTCTRL_DATACOUNT* pDataCount = pDoc->m_pPostCtrl->GetPointDataCount();
	BOOL bExistStage = (pDataCount->m_nNumStage > 0 ? TRUE : FALSE);

	if(bExistStage)   LoadCombCtrl.m_bCSAnalysis = TRUE;
	else              LoadCombCtrl.m_bCSAnalysis = FALSE;

	LoadCombCtrl.m_bLossFactor = FALSE;

	LoadCombCtrl.Set_AddEnvelope(FALSE);

	int nConstLoad = 0;
	BOOL bCS = pDoc->m_pAttrCtrl->ExistConstStag();
	if(bCS) nConstLoad = 2;
	else    nConstLoad = 0;

	LoadCombCtrl.Set_ConstLoad(nConstLoad);

	LoadCombCtrl.m_bJSCE02[0]   = m_bDead;
	LoadCombCtrl.m_bJSCE02[1]   = m_bDesign;
	LoadCombCtrl.m_bJSCE02[2]   = m_bTemper;
	LoadCombCtrl.m_bJSCE02[3]   = m_bEarth;
	LoadCombCtrl.m_bJSCE02[4]   = m_bFinalA;
	LoadCombCtrl.m_bJSCE02[5]   = m_bFinalB;
	LoadCombCtrl.m_bJSCE02[6]   = m_bFinalC;

	BOOL bOK = LoadCombCtrl.Set_DefaultLoadComb(1, _T("JARA02"));//Replace

	if(bOK) CDialogMove::OnOK();
	else	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Generation_code_unavailable_),MB_OK);
}

void CCRCDgnLcomDlg::Init_Data()
{
	m_bDead   = TRUE;
	m_bDesign = TRUE;
	m_bTemper = TRUE;
	m_bEarth  = FALSE;
	m_bFinalA = TRUE; 
	m_bFinalB = TRUE;
	m_bFinalC = TRUE;

}