// CMTdmtAASHTO.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtAUSTRALIA.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAUSTRALIA dialog


CCMTdmtAUSTRALIA::CCMTdmtAUSTRALIA(CWnd* pParent /*=NULL*/)
: CDlgChild(CCMTdmtAUSTRALIA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtAUSTRALIA)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMTdmtAUSTRALIA::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtAUSTRALIA)

	DDX_Control(pDX, IDC_CMD_HCRP_AS_COMP,      m_edtAScomp);
	DDX_Control(pDX, IDC_CMD_HCRP_AS_COMP_UNIT, m_AScompunit);
	DDX_Radio(pDX,   IDC_CMD_HCRP_AS_ENV_RADIO1, m_nExposureEnv);
	DDX_Control(pDX, IDC_CMD_HCRP_AS_THICK, m_edtASthick);
	DDX_Control(pDX, IDC_CMD_HCRP_AS_THICK_UNIT, m_ASthickunit);
	DDX_Radio(pDX,   IDC_CMD_HCRP_AS_SHNK_RADIO1, m_nEpsDry);
	DDX_Control(pDX, IDC_CMD_HCRP_AS_SHNK_USER_EDT, m_edtEpsDry);
	//DDX_Control(pDX, IDC_CMD_HCRP_AS_STRESSFACTOR_EDT, m_edtStressFactor);

	DDX_Control(pDX, IDC_CMD_HCRP_AS_AGE_UNIT, m_ASageunit);
	DDX_Control(pDX, IDC_CMD_HCRP_AS_AGE, m_ASage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtAUSTRALIA, CDlgChild)
//{{AFX_MSG_MAP(CCMTdmtAUSTRALIA)
	ON_BN_CLICKED(IDC_CMD_HCRP_AS_SHNK_RADIO1, OnDryRdo)
	ON_BN_CLICKED(IDC_CMD_HCRP_AS_SHNK_RADIO2, OnDryRdo)
	ON_BN_CLICKED(IDC_CMD_HCRP_AS_SHNK_RADIO3, OnDryRdo)
	ON_BN_CLICKED(IDC_CMD_HCRP_AS_SHNK_RADIO4, OnDryRdo)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAUSTRALIA message handlers

BOOL CCMTdmtAUSTRALIA::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// TODO: Add extra initialization here
	ASSERT(m_pData != 0); // Please Set m_pData;
	
	SetUnitType();

	m_ASage.SetRange(0, 10000);
	m_ASage.SetValue(3);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmtAUSTRALIA::OnDryRdo()
{
	SetEnable();
}

//--------------------------------------------------------------------
//
void CCMTdmtAUSTRALIA::SetUnitType()
{
	//----------------------------------------------------------------
	// AS Standard
	//----------------------------------------------------------------
	m_edtAScomp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_AScompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_edtASthick.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dThickness);
	m_ASthickunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dThickness);
	m_ASageunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_edtEpsDry.SetUnitType(D_UNITSYS_NONE);
	//m_edtStressFactor.SetUnitType(D_UNITSYS_NONE);
}

void CCMTdmtAUSTRALIA::SetData2DlgByCodeChange(int nCode)
{
	if (nCode == TDMT_CODE_AUSTRALIA_2009 || nCode == TDMT_CODE_AUSTRALIA_2011 || nCode == TDMT_CODE_AUSTRALIA_2016 || nCode == TDMT_CODE_AUSTRALIA_2017)
	{
		GetDlgItem(IDC_CMD_HCRP_AS_SHNK_RADIO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_HCRP_AS_SHNK_RADIO3)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_CMD_HCRP_AS_SHNK_RADIO1)->SetWindowText(_LS(IDS_ETC_HCRP_TDMT_AUSTRALIA_IDC_CMD_HCRP_AS_SHNK_RADIO1));
	}
	else if (nCode == TDMT_CODE_AUSTRALIA_2018_AMD_2021 || nCode == TDMT_CODE_AUSTRALIA_2017_AMD_2024)
	{
		GetDlgItem(IDC_CMD_HCRP_AS_SHNK_RADIO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_HCRP_AS_SHNK_RADIO3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CMD_HCRP_AS_SHNK_RADIO1)->SetWindowText(_LS(IDS_ETC_HCRP_TDMT_AUSTRALIA_IDC_CMD_HCRP_AS_SHNK_RADIO1_NEW));

		if (m_nEpsDry != 0 && m_nEpsDry != 3)
		{
			m_nEpsDry = 0;
		}
	}
	else
		ASSERT(0);

	UpdateData(FALSE);
}

void CCMTdmtAUSTRALIA::SetData2Dlg()
{ 
	m_edtAScomp.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_ASage.SetValue(m_pData->COMMON.dConcreteAge);

	m_edtASthick.SetEditUnit(m_pData->AUSTRALIA.dThickness);
	m_nExposureEnv = m_pData->AUSTRALIA.nExposureEnv;
	m_nEpsDry     = m_pData->AUSTRALIA.nEps_dry;
	m_edtEpsDry.SetEditUnit(m_pData->AUSTRALIA.dEps_dry);
	//m_edtStressFactor.SetEditUnit(m_pData->AUSTRALIA.dStressfactor);
	UpdateData(FALSE);

	SetEnable();	
}

void CCMTdmtAUSTRALIA::SetDlg2Data()
{
	UpdateData(TRUE);
	
	m_pData->COMMON.dCompStrength     = m_edtAScomp.GetEditValue();
	m_pData->COMMON.dConcreteAge      = m_ASage.GetEditValue();
	m_pData->AUSTRALIA.nExposureEnv   = m_nExposureEnv; 
	m_pData->AUSTRALIA.dThickness     = m_edtASthick.GetEditValue();
	m_pData->AUSTRALIA.nEps_dry       = m_nEpsDry;
	m_pData->AUSTRALIA.dEps_dry       = m_edtEpsDry.GetEditValue();		
	//m_pData->AUSTRALIA.dStressfactor  = m_edtStressFactor.GetEditValue();
}

void CCMTdmtAUSTRALIA::SetEnable()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_CMD_HCRP_AS_SHNK_USER_EDT)->EnableWindow(m_nEpsDry == 3);
}

void CCMTdmtAUSTRALIA::SetFrameName(const CString& strFrameName)
{
	GetDlgItem(IDC_CMD_AS_FRAME)->SetWindowText(strFrameName);
}