// ETC_GDS_JP_BRG2016.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_JP_BRG2012.h"

#include "SpfcJPBRG2012Util.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_JP_BRG2012 dialog


CETC_GDS_JP_BRG2012::CETC_GDS_JP_BRG2012(T_SPFC_D* pParamData, int nCode, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_JP_BRG2012::IDD, pParent, std::make_shared<CSpfcJPBRG2012Util>(nCode))
{
	//{{AFX_DATA_INIT(CETC_GDS_JP_BRG2012)
	m_pParamData = pParamData;
	m_nCode   = nCode; //0:2012, 1:2017 
	m_nMethod = -1;
	m_nSoil = -1;
	m_nZone = -1;	
	m_dMaxPeriod = PRD_ED;

	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcJPBRG2012Util>(m_pSpfcBaseUtil);

	//}}AFX_DATA_INIT
}


void CETC_GDS_JP_BRG2012::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_JP_BRG2012)
	DDX_Control(pDX, IDC_ETC_GDS_CZ_EDIT, m_wndCz);
	DDX_Control(pDX, IDC_ETC_GDS_DAMPING, m_wndDamping);
	DDX_Radio(pDX, IDC_ETC_GDS_METHOD_RDO, m_nMethod);
	DDX_Radio(pDX, IDC_ETC_GDS_SOIL_RDO, m_nSoil);
	DDX_Radio(pDX, IDC_ETC_GDS_ZONE_RDO, m_nZone);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_JP_BRG2012, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_JP_BRG2012)
	ON_BN_CLICKED(IDC_ETC_GDS_ZONE_RDO,  OnEtcGdsZoneRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_ZONE_RDO1, OnEtcGdsZoneRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_ZONE_RDO2, OnEtcGdsZoneRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_ZONE_RDO3, OnEtcGdsZoneRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_ZONE_RDO4, OnEtcGdsZoneRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_METHOD_RDO , OnEtcGdsZoneRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_METHOD_RDO2, OnEtcGdsZoneRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_METHOD_RDO3, OnEtcGdsZoneRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_JP_BRG2012 message handlers

BOOL CETC_GDS_JP_BRG2012::OnInitDialog() 
{  
	CMyChildDialog::OnInitDialog();
	
	m_wndCz.SetReadOnly(TRUE);

	m_wndDamping.SetReadOnly(TRUE);
	//SetDefaultValue();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_JP_BRG2012::SetDefaultValue()
{
	m_nMethod  = 0;
	m_nZone    = 0;
	m_nSoil    = 0;
	m_pSpfcUtil->SetCd(0.);
	m_pSpfcUtil->SetCz(1.0);
	m_pSpfcUtil->SetDamping(0.05);
	m_wndCz.SetEditUnit(1.0);
	m_wndDamping.SetEditUnit(0.05);
}

void CETC_GDS_JP_BRG2012::OnEtcGdsZoneRdo() 
{
	UpdateData(TRUE);
	// level, zone
	double dCz[3][5] = {{1.00, 1.00, 0.85, 0.85, 0.70},
						{1.20, 1.00, 1.20, 1.00, 0.80},
						{1.00, 1.00, 0.85, 0.85, 0.70}};
	m_wndCz.SetEditUnit(dCz[m_nMethod][m_nZone]);	
}

void CETC_GDS_JP_BRG2012::End()
{
	UpdateData(TRUE);
		
	m_pSpfcUtil->SetMethod(m_nMethod);
	m_pSpfcUtil->SetZone(m_nZone);
	m_pSpfcUtil->SetSoil(m_nSoil);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	m_pSpfcUtil->SetCz(m_wndCz.GetEditValue());
	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());
	m_pSpfcUtil->SetCd(1.5 / (40.0 * m_pSpfcUtil->GetDamping() + 1.0) + 0.5);

	CMyChildDialog::End();
}

void CETC_GDS_JP_BRG2012::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), 0.05);
	pDampingWnd->SetWindowText(csDamping);
}

BOOL CETC_GDS_JP_BRG2012::ISVALID()
{
	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());

	return m_pSpfcUtil->CheckValid();
}

BOOL CETC_GDS_JP_BRG2012::Dlg2Data()
{
	if(m_nCode==0) //2012
	{
		m_pParamData->CodeParam.JPBRG2012.nMethod    = m_nMethod;
		m_pParamData->CodeParam.JPBRG2012.nSeisZone  = m_nZone;
		m_pParamData->CodeParam.JPBRG2012.nSoilClass = m_nSoil;
		m_pParamData->CodeParam.JPBRG2012.dCz        = m_pSpfcUtil->GetCz();
		m_pParamData->CodeParam.JPBRG2012.dDamping   = m_pSpfcUtil->GetDamping();
		m_pParamData->CodeParam.JPBRG2012.dMaxPeriod = m_dMaxPeriod;
	}
	else if(m_nCode==1) //2017
	{
		m_pParamData->CodeParam.JPBRG2017.nMethod    = m_nMethod;
		m_pParamData->CodeParam.JPBRG2017.nSeisZone  = m_nZone;
		m_pParamData->CodeParam.JPBRG2017.nSoilClass = m_nSoil;
		m_pParamData->CodeParam.JPBRG2017.dCz        = m_pSpfcUtil->GetCz();
		m_pParamData->CodeParam.JPBRG2017.dDamping   = m_pSpfcUtil->GetDamping();
		m_pParamData->CodeParam.JPBRG2017.dMaxPeriod = m_dMaxPeriod;
	}
	else
		ASSERT(0);

	return TRUE;
}

void CETC_GDS_JP_BRG2012::Data2Dlg()
{
	SetPushOverData(TRUE);
}

void CETC_GDS_JP_BRG2012::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(!m_pParamData->SpecFuncName.IsEmpty())
	{
		if(m_nCode==0) //2012
		{
			m_nMethod     = m_pParamData->CodeParam.JPBRG2012.nMethod   ;
			m_nZone       = m_pParamData->CodeParam.JPBRG2012.nSeisZone ;
			m_nSoil       = m_pParamData->CodeParam.JPBRG2012.nSoilClass;
			m_pSpfcUtil->SetCz(m_pParamData->CodeParam.JPBRG2002.dCz);
			m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.JPBRG2002.dDamping);
			m_dMaxPeriod  = m_pParamData->CodeParam.JPBRG2012.dMaxPeriod ;

			m_pSpfcUtil->SetMethod(m_nMethod);
			m_pSpfcUtil->SetZone(m_nZone);
			m_pSpfcUtil->SetSoil(m_nSoil);
			m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

		}
		else if(m_nCode==1) //2017
		{
			m_nMethod     = m_pParamData->CodeParam.JPBRG2017.nMethod   ;
			m_nZone       = m_pParamData->CodeParam.JPBRG2017.nSeisZone ;
			m_nSoil       = m_pParamData->CodeParam.JPBRG2017.nSoilClass;
			m_pSpfcUtil->SetCz(m_pParamData->CodeParam.JPBRG2002.dCz);
			m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.JPBRG2002.dDamping);
			m_dMaxPeriod  = m_pParamData->CodeParam.JPBRG2017.dMaxPeriod ;

			m_pSpfcUtil->SetMethod(m_nMethod);
			m_pSpfcUtil->SetZone(m_nZone);
			m_pSpfcUtil->SetSoil(m_nSoil);
			m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
		}
		else
			ASSERT(0);
	}
	else
	{
		SetDefaultValue();
	}

	m_pSpfcUtil->SetCd(1.5 / (40.0 * m_pSpfcUtil->GetDamping() + 1.0) + 0.5);

	if(bInit)
	{
		UpdateData(FALSE);
		m_wndCz.SetEditUnit(m_pSpfcUtil->GetCz());
		m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());

		GetDlgItem(IDC_ETC_GDS_DAMPING_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_ETC_GDS_DAMPING)->ShowWindow(FALSE);
	}

}