// RatingParamKR2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingParamKR2Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingParamKR2Dlg dialog
CRatingParamKR2Dlg::CRatingParamKR2Dlg(CWnd* pParent )
	: CDialogMove(CRatingParamKR2Dlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
#ifdef _DEBUG
	T_RACD_D RacdD; RacdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);
	if(RacdD.nRatingCode != KSCE_USD10_RATING && RacdD.nRatingCode != KSCE_RAIL_USD11_RATING)
	{
		ASSERT(0);
	}
#endif

	// KSCE_USD10_RATING와 KSCE_RAIL_USD11_RATING일 경우만 Init을 다르게 하는 처리..
	//ASSERT(m_Data.strDgnLiveLoad == _T("DB"));
	//m_Data.dDgnLiveLoad = 24.0;
	//m_Data.bRefRebar = TRUE;	

	//{{AFX_DATA_INIT(CRatingParamKR2Dlg)	
	//}}AFX_DATA_INIT
}


CRatingParamKR2Dlg::~CRatingParamKR2Dlg()
{
	
}


void CRatingParamKR2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingParamKR2Dlg)
	DDX_Control(pDX, IDC_DGN_PS_TYPE_RADIO1, m_ratioPSType1);
	DDX_Control(pDX, IDC_DGN_PS_TYPE_RADIO2, m_ratioPSType2);
	DDX_Control(pDX, IDC_DGN_PS_TYPE_RADIO3, m_ratioPSType3);	
	DDX_Control(pDX, IDC_DGN_FLEXURE_TYPE_RADIO1, m_ratioFlexureType1);
	DDX_Control(pDX, IDC_DGN_FLEXURE_TYPE_RADIO2, m_ratioFlexureType2);
	DDX_Control(pDX, IDC_RDO_DEFLECTION, m_ratioDeflection);
	DDX_Control(pDX, IDC_RDO_STRAIN, m_ratioStrain);
	DDX_Control(pDX, IDC_DGN_TEN_BAR_CHECK, m_chkTensionBar);
	DDX_Control(pDX, IDC_DGN_LIVE_LOAD_EDIT, m_editLiveLoad);
	DDX_Control(pDX, IDC_DGN_LIVE_LOAD_LVAL_EDIT, m_editLiveLoadLVal);
	DDX_Control(pDX, IDC_DGN_METHOD_CHECK1, m_chkDgnOutput1);
	DDX_Control(pDX, IDC_DGN_METHOD_CHECK2, m_chkDgnOutput2);
	DDX_Control(pDX, IDC_DGN_METHOD_CHECK3, m_chkDgnOutput3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRatingParamKR2Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CRatingParamKR2Dlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_SEL_ALL_BTN, OnDgnSelectAll)
	ON_BN_CLICKED(IDC_UNSEL_ALL_BTN, OnDgnUnSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CRatingParamKR2Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CCheckDialogOpen::SetSoftwareRendering();
	
	InitUnit();
	
	if(m_pDoc->m_pAttrCtrl2->ExistRakr())
	{
		m_pDoc->m_pAttrCtrl2->GetRakr(m_Data);
	}

	T_RACD_D DgnCodeD; DgnCodeD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(DgnCodeD);
	if(DgnCodeD.nRatingCode == KSCE_USD10_RATING)           {m_strLiveLoadType = _T("DB"); m_Data.dDgnLiveLoad = 24.0;} // ???
	else if(DgnCodeD.nRatingCode == KSCE_RAIL_USD11_RATING) {m_strLiveLoadType = _T("LS"); m_Data.dDgnLiveLoad = 22.0;} // ???
	else{ASSERT(0);}
	
	GetLiveLoadTypeName();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRatingParamKR2Dlg::InitUnit()
{
	m_editLiveLoad.SetUnitType(D_UNITSYS_NONE);	
	m_editLiveLoadLVal.SetUnitType(D_UNITSYS_NONE);
}

void CRatingParamKR2Dlg::Data2Dlg()
{
	if(m_Data.nSurveyMeth == 0)//처짐
	{
		m_ratioDeflection.SetCheck(TRUE);
		m_ratioStrain.SetCheck(FALSE);
	}
	else if(m_Data.nSurveyMeth == 1)//변형률
	{
		m_ratioDeflection.SetCheck(FALSE);
		m_ratioStrain.SetCheck(TRUE);
	}
	else
		ASSERT(0);

	if(m_Data.iTndnKind == 0)//저릴랙세이션(0.28)
	{
		m_ratioPSType1.SetCheck(TRUE);
		m_ratioPSType2.SetCheck(FALSE);
		m_ratioPSType3.SetCheck(FALSE);
	}
	else if(m_Data.iTndnKind == 1)//응력제거강재(0.4)
	{
		m_ratioPSType1.SetCheck(FALSE);
		m_ratioPSType2.SetCheck(FALSE);
		m_ratioPSType3.SetCheck(TRUE);
	}
	else if(m_Data.iTndnKind == 2)//강봉(0.55)
	{
		m_ratioPSType1.SetCheck(FALSE);
		m_ratioPSType2.SetCheck(TRUE);
		m_ratioPSType3.SetCheck(FALSE);
	}
	else
		ASSERT(0);

	if(m_Data.ifpsMethod == 0)//설계기준
	{
		m_ratioFlexureType1.SetCheck(TRUE);
		m_ratioFlexureType2.SetCheck(FALSE);
	}
	else if(m_Data.ifpsMethod == 1)//상세해석
	{
		m_ratioFlexureType1.SetCheck(FALSE);
		m_ratioFlexureType2.SetCheck(TRUE);
	}
	else
		ASSERT(0);

	if(m_Data.bRefRebar)//인장철근고려
		m_chkTensionBar.SetCheck(TRUE);
	else
		m_chkTensionBar.SetCheck(FALSE);

	//설계활하중
	m_editLiveLoad.SetEditUnit(m_Data.dDgnLiveLoad);
	m_editLiveLoadLVal.SetWindowText(m_strLiveLoadType);

	if(m_Data.bPrintParm[0])//단면력 집계
		m_chkDgnOutput1.SetCheck(TRUE);
	else
		m_chkDgnOutput1.SetCheck(FALSE);

	if(m_Data.bPrintParm[1])//휨강도 검토
		m_chkDgnOutput2.SetCheck(TRUE);
	else
		m_chkDgnOutput2.SetCheck(FALSE);

	if(m_Data.bPrintParm[2])//전단강도 검토
		m_chkDgnOutput3.SetCheck(TRUE);
	else
		m_chkDgnOutput3.SetCheck(FALSE);
}

void CRatingParamKR2Dlg::Dlg2Data()
{
	if(m_ratioDeflection.GetCheck())//처짐
		m_Data.nSurveyMeth = 0;
	else if(m_ratioStrain.GetCheck())//변형률
		m_Data.nSurveyMeth = 1;
	else
		ASSERT(0);

	if(m_ratioPSType1.GetCheck())//저릴랙세이션(0.28)
		m_Data.iTndnKind = 0;
	else if(m_ratioPSType2.GetCheck())//강봉(0.55)
		m_Data.iTndnKind = 2;
	else if(m_ratioPSType3.GetCheck())//응력제거강재(0.4)
		m_Data.iTndnKind = 1;
	else
		ASSERT(0);

	if(m_ratioFlexureType1.GetCheck())//설계기준
		m_Data.ifpsMethod = 0;
	else if(m_ratioFlexureType2.GetCheck())//상세해석
		m_Data.ifpsMethod = 1;
	else
		ASSERT(0);

	//인장철근고려
	m_Data.bRefRebar = m_chkTensionBar.GetCheck();

	//설계활하중
	m_Data.dDgnLiveLoad = m_editLiveLoad.GetEditValue();

	m_editLiveLoadLVal.GetWindowText(m_Data.strDgnLiveLoad);

	m_Data.bPrintParm[0] = m_chkDgnOutput1.GetCheck();//단면력집계
	m_Data.bPrintParm[1] = m_chkDgnOutput2.GetCheck();//휨강도검토
	m_Data.bPrintParm[2] = m_chkDgnOutput3.GetCheck();//휨강도검토
}


BOOL CRatingParamKR2Dlg::GetLiveLoadTypeName()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RACD_D DgnCodeD; DgnCodeD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(DgnCodeD);
	if     (DgnCodeD.nRatingCode == KSCE_USD10_RATING)     {m_strLiveLoadType = _T("DB");}  // ???
	else if(DgnCodeD.nRatingCode == KSCE_RAIL_USD11_RATING){m_strLiveLoadType = _T("LS");}  // ???
	else{ASSERT(0);}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRatingParamKR2Dlg message handlers
void CRatingParamKR2Dlg::OnOk() 
{
	Dlg2Data();
	
	if(!m_pDoc->m_pDataCtrl->AddRakr(m_Data))
		return;

	CDialogMove::OnOK();		
}

void CRatingParamKR2Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CRatingParamKR2Dlg::OnDgnSelectAll() 
{
	m_chkDgnOutput1.SetCheck(TRUE);
	m_chkDgnOutput2.SetCheck(TRUE);
	m_chkDgnOutput3.SetCheck(TRUE);
}

void CRatingParamKR2Dlg::OnDgnUnSelectAll() 
{
	m_chkDgnOutput1.SetCheck(FALSE);
	m_chkDgnOutput2.SetCheck(FALSE);
	m_chkDgnOutput3.SetCheck(FALSE);
}
