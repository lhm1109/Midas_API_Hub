// DgnCRCDgnParameter_TMH7.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCRCDgnParameter_TMH7.h"

#include "DgnCRCAnnexDgnParameter_TMH7_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_TMH7 dialog
CDgnCRCDgnParameter_TMH7::CDgnCRCDgnParameter_TMH7(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CDgnCRCDgnParameter_TMH7::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCRCDgnParameter_TMH7)
	//}}AFX_DATA_INIT
	m_pParamData = pParamData;
	
	m_bUser = FALSE;
	m_TMH7LS1 = FALSE;
	m_TMH7LS2 = FALSE;
	m_TMH7LS3 = FALSE;
	m_TMH7LS4 = FALSE;
	m_TMH7LS5 = FALSE;
	m_bULS1 = FALSE;
	m_bULS2 = FALSE;
	m_bULS3 = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnCRCDgnParameter_TMH7::~CDgnCRCDgnParameter_TMH7()
{
}

void CDgnCRCDgnParameter_TMH7::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_TMH7)	
	DDX_Check  (pDX, IDC_BS_DESIGN_PARAMETER1,  m_bUser);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER6,  m_edtComp);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER7,  m_untComp);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER9,  m_edtTens);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER10, m_untTens);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER13, m_edtCSComp);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER14, m_untCSComp);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER16, m_edtCSTens);
	DDX_Control(pDX, IDC_BS_DESIGN_PARAMETER17, m_untCSTens);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER2,   m_TMH7LS1);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER3,   m_TMH7LS2);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER4,   m_TMH7LS3);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER5,   m_TMH7LS4);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER6,   m_TMH7LS5);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER8,   m_bULS1);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER9,   m_bULS2);
	DDX_Check  (pDX, IDC_BS_PRITN_PARAMETER10,  m_bULS3);
	//}}AFX_DATA_MAP
}

void CDgnCRCDgnParameter_TMH7::InitUnit()
{
	m_edtComp.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_Comp);
	m_untComp.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_Comp);
	m_edtTens.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_Tens);
	m_untTens.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_Tens);
	m_edtCSComp.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_CSComp);
	m_untCSComp.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_CSComp);
	m_edtCSTens.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_CSTens);
	m_untCSTens.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dPS_CSTens);
}

void CDgnCRCDgnParameter_TMH7::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_BS_DESIGN_PARAMETER2)->EnableWindow(m_bUser);
}

void CDgnCRCDgnParameter_TMH7::Data2Dlg()
{
	m_bUser = m_pParamData->bUserInputData;
	
	m_edtComp.SetEditUnit(m_pParamData->dPS_Comp);
	m_edtTens.SetEditUnit(m_pParamData->dPS_Tens);
	m_edtCSComp.SetEditUnit(m_pParamData->dPS_CSComp);
	m_edtCSTens.SetEditUnit(m_pParamData->dPS_CSTens);

	m_TMH7LS1 = m_pParamData->bServiceabilityEuro[0];
	m_TMH7LS2 = m_pParamData->bServiceabilityEuro[1];
	m_TMH7LS3 = m_pParamData->bServiceabilityEuro[2];
	m_TMH7LS4 = m_pParamData->bServiceabilityEuro[3];
	m_TMH7LS5 = m_pParamData->bServiceabilityEuro[4];

	m_bULS1 = m_pParamData->bUltimateEuro[0];
	m_bULS2 = m_pParamData->bUltimateEuro[1];
	m_bULS3 = m_pParamData->bUltimateEuro[2];

	UpdateData(FALSE);
}

BOOL CDgnCRCDgnParameter_TMH7::Dlg2Data()
{
	UpdateData(TRUE);

	m_pParamData->bUserInputData = m_bUser;

	m_pParamData->dPS_Comp = m_edtComp.GetEditValue();
	m_pParamData->dPS_Tens = m_edtTens.GetEditValue();
	m_pParamData->dPS_CSComp = m_edtCSComp.GetEditValue();
	m_pParamData->dPS_CSTens = m_edtCSTens.GetEditValue();

	m_pParamData->bServiceabilityEuro[0] = m_TMH7LS1;
	m_pParamData->bServiceabilityEuro[1] = m_TMH7LS2;
	m_pParamData->bServiceabilityEuro[2] = m_TMH7LS3;
	m_pParamData->bServiceabilityEuro[3] = m_TMH7LS4;
	m_pParamData->bServiceabilityEuro[4] = m_TMH7LS5;

	m_pParamData->bUltimateEuro[0] = m_bULS1;
	m_pParamData->bUltimateEuro[1] = m_bULS2;
	m_pParamData->bUltimateEuro[2] = m_bULS3;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_TMH7, CMyChildDialog)
	//{{AFX_MSG_MAP(CDgnCRCDgnParameter_TMH7)
	ON_BN_CLICKED(IDC_BS_DESIGN_PARAMETER1, OnCRCDgnParamBSUserChk)
	ON_BN_CLICKED(IDC_BS_DESIGN_PARAMETER2, OnCRCDgnParamBSUserBtn)
	ON_BN_CLICKED(IDC_BS_PRITN_PARAMETER11, OnCRCDgnParamBSSelAllBtn)
	ON_BN_CLICKED(IDC_BS_PRITN_PARAMETER12, OnCRCDgnParamBSUnSelAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//------------------------------------------------------------------
// OnInitDialog
//------------------------------------------------------------------
BOOL CDgnCRCDgnParameter_TMH7::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->ExistPscd())
	{
		m_pParamData->dTsPreTens = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);
		m_pParamData->dTsPostTens = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);
	}

	InitUnit();

	Data2Dlg();

	EnableDisableControls();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCRCDgnParameter_TMH7::OnCRCDgnParamBSUserChk()
{
	EnableDisableControls();
}

void CDgnCRCDgnParameter_TMH7::OnCRCDgnParamBSUserBtn()
{
	CDgnCRCAnnexDgnParameter_TMH7_Dlg dlg;
	dlg.m_pParamData = m_pParamData;
	
	if (dlg.DoModal() == IDOK)
	{
		m_pParamData = dlg.m_pParamData;
	}
}

void CDgnCRCDgnParameter_TMH7::OnCRCDgnParamBSSelAllBtn()
{
	m_TMH7LS1 = TRUE;
	m_TMH7LS2 = TRUE;
	m_TMH7LS3 = TRUE;
	m_TMH7LS4 = TRUE;
	m_TMH7LS5 = TRUE;
	m_bULS1 = TRUE;
	m_bULS2 = TRUE;
	m_bULS3 = TRUE;

	UpdateData(FALSE);
}

void CDgnCRCDgnParameter_TMH7::OnCRCDgnParamBSUnSelAllBtn()
{
	m_TMH7LS1 = FALSE;
	m_TMH7LS2 = FALSE;
	m_TMH7LS3 = FALSE;
	m_TMH7LS4 = FALSE;
	m_TMH7LS5 = FALSE;
	m_bULS1 = FALSE;
	m_bULS2 = FALSE;
	m_bULS3 = FALSE;
	
	UpdateData(FALSE);
}