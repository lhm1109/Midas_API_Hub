// CMTdmtAASHTO.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtNEWZEALAND.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtNEWZEALAND dialog


CCMTdmtNEWZEALAND::CCMTdmtNEWZEALAND(CWnd* pParent /*=NULL*/)
: CDlgChild(CCMTdmtNEWZEALAND::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtNEWZEALAND)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMTdmtNEWZEALAND::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtNEWZEALAND)

	DDX_Control(pDX, IDC_CMD_HCRP_NZ_COMP,      m_edtAScomp);
	DDX_Control(pDX, IDC_CMD_HCRP_NZ_COMP_UNIT, m_AScompunit);
	DDX_Radio(pDX,   IDC_CMD_HCRP_NZ_ENV_RADIO1, m_nExposureEnv);
	DDX_Control(pDX, IDC_CMD_HCRP_NZ_RFACTOR_SHRINKAGE_EDT, m_edtRFactorShirink);
	DDX_Control(pDX, IDC_CMD_HCRP_STRESSFACTOR_EDT        , m_edtAggFactor);
	DDX_Control(pDX, IDC_CMD_HCRP_NZ_THICK, m_edtASthick);
	DDX_Control(pDX, IDC_CMD_HCRP_NZ_THICK_UNIT, m_ASthickunit);  

	DDX_Control(pDX, IDC_CMD_HCRP_NZ_AGE_UNIT, m_ASageunit);
	DDX_Control(pDX, IDC_CMD_HCRP_NZ_DRYING_CBX, m_cbxDrying);  
	DDX_Control(pDX, IDC_CMD_HCRP_NZ_DRYING_EDT, m_edtDrying);  

	DDX_Control(pDX, IDC_CMD_HCRP_NZ_AGE, m_ASage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtNEWZEALAND, CDlgChild)
//{{AFX_MSG_MAP(CCMTdmtNEWZEALAND)
ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_NZ_AGE_SPIN, OnDeltaposCmdHcrpAsAgeSpin)
ON_CBN_SELCHANGE(IDC_CMD_HCRP_NZ_DRYING_CBX, OnSelchangeDryingCbx)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMTdmtNEWZEALAND message handlers

BOOL CCMTdmtNEWZEALAND::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	m_nCode = TDMT_CODE_NEWZEALAND;
	// TODO: Add extra initialization here
	ASSERT(m_pData != 0); // Please Set m_pData;
	
	SetCtrl(m_nCode);
	SetUnitType();
	SetEnable();

	// MQC 16803
	GetDlgItem(IDC_CMD_HCRP_NZ_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_HCRP_NZ_ENV_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_HCRP_NZ_ENV_RADIO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_HCRP_NZ_ENV_RADIO3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_HCRP_NZ_ENV_RADIO4)->ShowWindow(SW_HIDE);
	m_ASage.SetRange(0, 10000);
	m_ASage.SetValue(3);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmtNEWZEALAND::SetCtrl(int nCode)
{
	m_cbxDrying.ResetContent();
	if (nCode == TDMT_CODE_NEWZEALAND)
	{
		m_cbxDrying.AddString(_T("1500(Hastings,Palmerston North,Masterton,Wellington,Blenheim,Kaikoura)"));
		m_cbxDrying.AddString(_T("1460(Nelson) "));
		m_cbxDrying.AddString(_T("1315(Kaitaia,Tauranga)"));
		m_cbxDrying.AddString(_T("1080(New Plymouth,Taranaki)"));
		m_cbxDrying.AddString(_T("1000(Whangarei,Auckland Hunua,Hamilton)"));
		m_cbxDrying.AddString(_T("990(Auckland)"));
		m_cbxDrying.AddString(_T("950(Christchurch,Timaru,Oamaru Southern greywacke)"));
		m_cbxDrying.AddString(_T("775(Westport,Queenstown,Wanaka,Invercargill)"));
		m_cbxDrying.AddString(_T("735(Dunedin)"));
		m_cbxDrying.AddString(_T("570(Waiau)"));
		m_cbxDrying.AddString(_LS(IDS_WG_CMD_USER_DEFINED)); // 10번
	}
	else if (nCode == TDMT_CODE_NEWZEALAND_2022)
	{
		m_cbxDrying.AddString(_T("1100(Hastings,Palmerston North,Masterton,Wellington,Blenheim,Kaikoura)"));
		m_cbxDrying.AddString(_T("1100(Nelson) "));
		m_cbxDrying.AddString(_T("960(Kaitaia,Tauranga)"));
		m_cbxDrying.AddString(_T("730(New Plymouth,Taranaki)"));
		m_cbxDrying.AddString(_T("720(Whangarei,Auckland Hunua,Hamilton)"));
		m_cbxDrying.AddString(_T("660(Auckland)"));
		m_cbxDrying.AddString(_T("700(Christchurch,Timaru,Oamaru Southern greywacke)"));
		m_cbxDrying.AddString(_T("590(Westport,Queenstown,Wanaka,Invercargill)"));
		m_cbxDrying.AddString(_T("520(Dunedin)"));
		m_cbxDrying.AddString(_T("390(Waiau)"));
		m_cbxDrying.AddString(_LS(IDS_WG_CMD_USER_DEFINED)); // 10번
	}
	else
		ASSERT(0);

	SetComboWidth(m_cbxDrying);
}

void CCMTdmtNEWZEALAND::OnDeltaposCmdHcrpAsAgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_NZ_AGE,0,10000,pNMHDR,pResult);
	
	*pResult = 0;
}

//--------------------------------------------------------------------
//
void CCMTdmtNEWZEALAND::SetUnitType()
{
	//----------------------------------------------------------------
	// AS Standard
	//----------------------------------------------------------------
	m_edtAScomp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_AScompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_edtASthick.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dThickness);  
	m_ASthickunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dThickness);
	m_ASageunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_ASage.SetValue(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_edtRFactorShirink.SetUnitType(D_UNITSYS_NONE);
	m_edtAggFactor.SetUnitType(D_UNITSYS_NONE);
}

void CCMTdmtNEWZEALAND::SetData2DlgByCodeChange(int nCode)
{
	m_nCode = nCode;
	SetCtrl(m_nCode);
	m_cbxDrying.SetCurSel(m_pData->NEWZEALAND.nEps_dry);
	OnSelchangeDryingCbx(); //m_edtDrying.SetEditUnit(m_pData->NEWZEALAND.dEps_dry);

	if (m_nModify == 1) // Modify
	{
		//m_edtRFactorShirink.SetEditUnit(m_pData->NEWZEALAND.dRFactorShirink);
	}
	else
	{
		if (m_nCode == TDMT_CODE_NEWZEALAND_2022) m_edtRFactorShirink.SetEditUnit(0.74);
		else                                      m_edtRFactorShirink.SetEditUnit(0.72);
	}

	if (m_nCode == TDMT_CODE_NEWZEALAND)
	{
		GetDlgItem(IDC_CMD_HCRP_NZ_RFACTOR_SHRINKAGE_ST)->SetWindowText(_LS(IDS_ETC_HCRP_TDMT_NEWZEALAND_IDC_CMD_HCRP_NZ_RFACTOR_SHRINKAGE_ST));
		GetDlgItem(IDC_CMD_HCRP_NZ_STATIC4             )->SetWindowText(_LS(IDS_ETC_HCRP_TDMT_NEWZEALAND_IDC_CMD_HCRP_NZ_STATIC4));

		GetDlgItem(IDC_CMD_HCRP_STRESSFACTOR_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_HCRP_STRESSFACTOR_EDT)->ShowWindow(SW_HIDE);
	}
	else if (m_nCode == TDMT_CODE_NEWZEALAND_2022)
	{
		GetDlgItem(IDC_CMD_HCRP_NZ_RFACTOR_SHRINKAGE_ST)->SetWindowText(_LS(IDS_ETC_HCRP_TDMT_NEWZEALAND_2022_IDC_CMD_HCRP_NZ_RFACTOR_SHRINKAGE_ST));
		GetDlgItem(IDC_CMD_HCRP_NZ_STATIC4             )->SetWindowText(_LS(IDS_ETC_HCRP_TDMT_NEWZEALAND_2022_IDC_CMD_HCRP_NZ_STATIC4));

		GetDlgItem(IDC_CMD_HCRP_STRESSFACTOR_STC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_HCRP_STRESSFACTOR_EDT)->ShowWindow(SW_SHOW);
	}
	else
		ASSERT(0);

	UpdateData(FALSE);
}

void CCMTdmtNEWZEALAND::SetData2Dlg()
{ 
	m_edtAScomp.SetEditUnit(m_pData->COMMON.dCompStrength);
	m_ASage.SetValue(m_pData->COMMON.dConcreteAge);

	m_edtASthick.SetEditUnit(m_pData->NEWZEALAND.dThickness);
	if (m_nModify == 1) // Modify
	{
		m_edtRFactorShirink.SetEditUnit(m_pData->NEWZEALAND.dRFactorShirink);
	}
	else
	{
		if (m_nCode == TDMT_CODE_NEWZEALAND_2022) m_edtRFactorShirink.SetEditUnit(0.74);
		else                                      m_edtRFactorShirink.SetEditUnit(0.72);
	}
	m_edtAggFactor.SetEditUnit(m_pData->NEWZEALAND.dAggFactor);
	m_nExposureEnv = m_pData->NEWZEALAND.nExposureEnv;
	m_cbxDrying.SetCurSel(m_pData->NEWZEALAND.nEps_dry);
	m_edtDrying.SetEditUnit(m_pData->NEWZEALAND.dEps_dry);  
 
	SetEnable();

	UpdateData(FALSE);
}

void CCMTdmtNEWZEALAND::SetDlg2Data()
{
	UpdateData(TRUE);
	
	m_pData->COMMON.dCompStrength       = m_edtAScomp.GetEditValue();
	m_pData->COMMON.dConcreteAge        = m_ASage.GetEditValue();
	m_pData->NEWZEALAND.nExposureEnv    = m_nExposureEnv; 
	m_pData->NEWZEALAND.dThickness      = m_edtASthick.GetEditValue();
	m_pData->NEWZEALAND.dRFactorShirink = m_edtRFactorShirink.GetEditValue();
	m_pData->NEWZEALAND.dAggFactor      = m_edtAggFactor.GetEditValue();
	m_pData->NEWZEALAND.nEps_dry        = m_cbxDrying.GetCurSel();
	m_pData->NEWZEALAND.dEps_dry        = m_edtDrying.GetEditValue();
}

void CCMTdmtNEWZEALAND::OnSelchangeDryingCbx()
{
	double dDryingVal = 0.0;;
	int nDrying = m_cbxDrying.GetCurSel();
	if(nDrying != 10) // User Defined가 아닌경우
	{
		switch (nDrying)
		{
		case 0:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 1500 : 1100;
				break;
		case 1:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 1460 : 1100;
			break;
		case 2:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 1315 : 960;
			break;
		case 3:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 1080 : 730;
			break;
		case 4:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 1000 : 720;
			break;
		case 5:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 990 : 660;
			break;
		case 6:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 950 : 700;
			break;
		case 7:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 775 : 590;
			break;
		case 8:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 735 : 520;
			break;
		case 9:
			dDryingVal = (m_nCode == TDMT_CODE_NEWZEALAND) ? 570 : 390;
			break;
		default: 
			ASSERT(0);
			dDryingVal = 0.0;
			break;
		}
		CString strTemp;
		strTemp.Format(_T("%g"), dDryingVal);
		m_edtDrying.SetWindowText(strTemp);
	}   
	
	SetEnable();
}

void CCMTdmtNEWZEALAND::SetEnable()
{
	int nDrying = m_cbxDrying.GetCurSel();

	GetDlgItem(IDC_CMD_HCRP_NZ_DRYING_EDT)->EnableWindow(nDrying == 10);
}

// 콤보박스 안의 글자 보이게 함.
void CCMTdmtNEWZEALAND::SetComboWidth(CComboBox& ComboBox)
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC* pDC = ComboBox.GetDC();
	for (int i=0;i < ComboBox.GetCount();i++)
	{
		ComboBox.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}  
	ComboBox.SetDroppedWidth(dx);
	ComboBox.ReleaseDC(pDC);
}

