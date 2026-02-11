// ETC_GDS_JPN2000.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_JPN2000.h"
#include "SpfcJPN2000Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_JPN2000 dialog


CETC_GDS_JPN2000::CETC_GDS_JPN2000(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_JPN2000::IDD, pParent, std::make_shared<CSpfcJPN2000Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_JPN2000)
	m_pParamData = pParamData;

	m_wndZ = 0;
	m_wndTc = 0;
	m_dC0 = 0.2;
	m_dMaxPeriod = PRD_ED;

	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcJPN2000Util>(m_pSpfcBaseUtil);
	//}}AFX_DATA_INIT
	SetValue();
}


void CETC_GDS_JPN2000::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_JPN2000)
	DDX_Radio(pDX, IDC_ETC_GDS_JPN2000_Z_RDO1, m_wndZ);
	DDX_Radio(pDX, IDC_ETC_GDS_JPN2000_TC_RDO1, m_wndTc);
	DDX_Text(pDX, IDC_ETC_GDS_JPN2000_C0_EDIT, m_dC0);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_JPN2000, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_JPN2000)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_JPN2000 message handlers

BOOL CETC_GDS_JPN2000::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
void CETC_GDS_JPN2000::End()
{
	UpdateData(TRUE);

	Dlg2UtilData();
	SetValue();
	CMyChildDialog::End();
}

//-----------------------------------------------------------------------------------------------------
// INIT : 2003.09.20. by KYE-HONG
// FUNC : 계산알고리즘에 사용되는 Default값을 Setting함. 
//-----------------------------------------------------------------------------------------------------
void CETC_GDS_JPN2000::SetValue()
{
	m_pSpfcUtil->SetParamFromCalc();
}

BOOL CETC_GDS_JPN2000::ISVALID()
{  
	return m_pSpfcUtil->CheckValid();
}

BOOL CETC_GDS_JPN2000::Dlg2Data()
{
	m_pParamData->CodeParam.JP2000.nZ           = m_wndZ;
	m_pParamData->CodeParam.JP2000.nSoilClass   = m_wndTc;
	m_pParamData->CodeParam.JP2000.dCo          = m_dC0;
	m_pParamData->CodeParam.JP2000.dMaxPeriod   = m_dMaxPeriod;

	return TRUE;
}
void CETC_GDS_JPN2000::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_wndZ        = m_pParamData->CodeParam.JP2000.nZ          ;
	m_wndTc       = m_pParamData->CodeParam.JP2000.nSoilClass  ;
	m_dC0         = m_pParamData->CodeParam.JP2000.dCo         ;
	m_dMaxPeriod  = m_pParamData->CodeParam.JP2000.dMaxPeriod  ;

	Dlg2UtilData();
	SetValue();
	if(bInit) UpdateData(FALSE);
}

void CETC_GDS_JPN2000::Dlg2UtilData()
{
	m_pSpfcUtil->SetWndZ(m_wndZ);
	m_pSpfcUtil->SetWndTc(m_wndTc);
	m_pSpfcUtil->SetC0(m_dC0);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}
