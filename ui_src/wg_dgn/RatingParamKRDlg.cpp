// RatingParamKRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingParamKRDlg.h"

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
// CRatingParamKRDlg dialog


CRatingParamKRDlg::CRatingParamKRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRatingParamKRDlg::IDD, pParent)
{
	m_Data.Initialize();
	m_pDoc = CDBDoc::GetDocPoint();

	//{{AFX_DATA_INIT(CRatingParamKRDlg)	
	//}}AFX_DATA_INIT
}


CRatingParamKRDlg::~CRatingParamKRDlg()
{
	
}


void CRatingParamKRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingParamKRDlg)
	DDX_Control(pDX, IDC_DGN_PS_TYPE_RADIO1, m_ratioPSType1);
	DDX_Control(pDX, IDC_DGN_PS_TYPE_RADIO2, m_ratioPSType2);
	DDX_Control(pDX, IDC_DGN_PS_TYPE_RADIO3, m_ratioPSType3);
	DDX_Control(pDX, IDC_DGN_FLEXURE_TYPE_RADIO1, m_ratioFlexureType1);
	DDX_Control(pDX, IDC_DGN_FLEXURE_TYPE_RADIO2, m_ratioFlexureType2);
	DDX_Control(pDX, IDC_DGN_TEN_BAR_CHECK, m_chkTensionBar);
	DDX_Control(pDX, IDC_DGN_LIVE_LOAD_EDIT, m_editLiveLoad);
	DDX_Control(pDX, IDC_DGN_METHOD_CHECK1, m_chkDgnOutput1);
	DDX_Control(pDX, IDC_DGN_METHOD_CHECK2, m_chkDgnOutput2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRatingParamKRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CRatingParamKRDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_SEL_ALL_BTN, OnDgnSelectAll)
	ON_BN_CLICKED(IDC_UNSEL_ALL_BTN, OnDgnUnSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CRatingParamKRDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	InitUnit();

	if(m_pDoc->m_pAttrCtrl2->ExistRakr())
	{
		m_pDoc->m_pAttrCtrl2->GetRakr(m_Data);
	}
	
	Data2Dlg();
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRatingParamKRDlg::InitUnit()
{
	m_editLiveLoad.SetUnitType(D_UNITSYS_NONE);	
}

void CRatingParamKRDlg::Data2Dlg()
{
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

	if(m_Data.bPrintParm[0])//강도설계법
		m_chkDgnOutput1.SetCheck(TRUE);
	else
		m_chkDgnOutput1.SetCheck(FALSE);

	if(m_Data.bPrintParm[1])//허용응력설계법
		m_chkDgnOutput2.SetCheck(TRUE);
	else
		m_chkDgnOutput2.SetCheck(FALSE);
}

void CRatingParamKRDlg::Dlg2Data()
{
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

	m_Data.bPrintParm[0] = m_chkDgnOutput1.GetCheck();//강도설계법
	m_Data.bPrintParm[1] = m_chkDgnOutput2.GetCheck();//허용응력설계법	
}


/////////////////////////////////////////////////////////////////////////////
// CRatingParamKRDlg message handlers

void CRatingParamKRDlg::OnOk() 
{
	Dlg2Data();
	
	if(!m_pDoc->m_pDataCtrl->AddRakr(m_Data))
		return;

	CDialogMove::OnOK();		
}

void CRatingParamKRDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CRatingParamKRDlg::OnDgnSelectAll() 
{
	m_chkDgnOutput1.SetCheck(TRUE);
	m_chkDgnOutput2.SetCheck(TRUE);
}

void CRatingParamKRDlg::OnDgnUnSelectAll() 
{
	m_chkDgnOutput1.SetCheck(FALSE);
	m_chkDgnOutput2.SetCheck(FALSE);
}
