// DgnMPhiCurveConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnMPhiCurveConditionDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnMPhiCurveConditionDlg dialog


CDgnMPhiCurveConditionDlg::CDgnMPhiCurveConditionDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnMPhiCurveConditionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnMPhiCurveConditionDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

CDgnMPhiCurveConditionDlg::~CDgnMPhiCurveConditionDlg()
{
}

void CDgnMPhiCurveConditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnMPhiCurveConditionDlg)
	DDX_Check  (pDX, IDC_DGN_MPHI_CURVE_CONDITION_STRAIN_ECU_CHK,           m_becu);
	DDX_Radio  (pDX, IDC_DGN_MPHI_CURVE_CONDITION_VALUE_INELASTIC_RDO,      m_necuType);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CONDITION_ECU_EDT,                  m_edtecu_User);
	DDX_Check  (pDX, IDC_DGN_MPHI_CURVE_CONDITION_PEAK_85_CHK,              m_bfck85);
	DDX_Check  (pDX, IDC_DGN_MPHI_CURVE_CONDITION_STRAIN_01_CHK,            m_beru_tens);
	DDX_Check  (pDX, IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_CHK,      m_beru_comp);
	DDX_Radio  (pDX, IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_RDO1,     m_neruType);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_EDT,      m_edteru_comp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnMPhiCurveConditionDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnMPhiCurveConditionDlg)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CONDITION_STRAIN_ECU_CHK,         OnDgnMPhiCurveConditionecuChk)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CONDITION_VALUE_INELASTIC_RDO,    OnDgnMPhiCurveConditionecutypeRdo)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CONDITION_USER_DEFINE_RDO,        OnDgnMPhiCurveConditionecutypeRdo)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_CHK,    OnDgnMPhiCurveConditioneru_compChk)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_RDO1,   OnDgnMPhiCurveConditionerutypeRdo)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_RDO2,   OnDgnMPhiCurveConditionerutypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDgnMPhiCurveConditionDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);

	if(m_becu)
	{
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_VALUE_INELASTIC_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_USER_DEFINE_RDO    )->EnableWindow(TRUE);

		if(m_necuType == 0)
		{
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_ECU_STC)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_ECU_EDT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_ECU_STC)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_ECU_EDT)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_VALUE_INELASTIC_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_USER_DEFINE_RDO    )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_ECU_STC            )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_ECU_EDT            )->EnableWindow(FALSE);
	}

	if(m_beru_comp)
	{
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_RDO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_RDO2)->EnableWindow(TRUE);

		if(m_neruType == 0)
		{
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_STC)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_EDT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_STC)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_EDT)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_RDO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_RDO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_STC )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_CONDITION_BUCKLING_STRAIN_EDT )->EnableWindow(FALSE);
	}
}

void CDgnMPhiCurveConditionDlg::Data2Dlg()
{
	m_becu      = m_Data.becu;
	m_necuType  = m_Data.necuType;
	m_edtecu_User.SetEditUnit(m_Data.decu_User);
	m_bfck85    = m_Data.bfck85;
	m_beru_tens = m_Data.beru_tens;
	m_beru_comp = m_Data.beru_comp;
	m_neruType  = m_Data.neruType;
	m_edteru_comp.SetEditUnit(m_Data.deru_comp);

	UpdateData(FALSE);
}

BOOL CDgnMPhiCurveConditionDlg::Dlg2Data()
{ 
	UpdateData(TRUE);

	m_Data.becu      = m_becu;
	m_Data.necuType  = m_necuType;
	m_Data.decu_User = m_edtecu_User.GetEditValue();
	m_Data.bfck85    = m_bfck85;
	m_Data.beru_tens = m_beru_tens;
	m_Data.beru_comp = m_beru_comp;
	m_Data.neruType  = m_neruType;
	m_Data.deru_comp = m_edteru_comp.GetEditValue();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnMPhiCurveConditionDlg message handlers

BOOL CDgnMPhiCurveConditionDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!m_pDoc->m_pAttrCtrl2->GetEcuc(m_Data)) 
	{
		m_Data.Initialize();

#if defined(_CIVIL_ORG)
		m_Data.necuType = 0;
		m_Data.decu_User = 0.004;
#endif
	}

	Data2Dlg();

	UpdateData(FALSE);

	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnMPhiCurveConditionDlg::OnOK() 
{
	Dlg2Data();

	if(!m_pDoc->m_pDataCtrl->AddEcuc(m_Data)) return;

	CDialogMove::OnOK();
}

void CDgnMPhiCurveConditionDlg::OnDgnMPhiCurveConditionecuChk()
{
	ControlsEnableDisable();
}

void CDgnMPhiCurveConditionDlg::OnDgnMPhiCurveConditionecutypeRdo()
{
	ControlsEnableDisable();
}

void CDgnMPhiCurveConditionDlg::OnDgnMPhiCurveConditioneru_compChk()
{
	ControlsEnableDisable();
}

void CDgnMPhiCurveConditionDlg::OnDgnMPhiCurveConditionerutypeRdo()
{
	ControlsEnableDisable();
}

