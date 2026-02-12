// ETC_GDS_KS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_EP_KDS2018.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS dialog


CETC_EP_KDS2018::CETC_EP_KDS2018(T_POSL_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_EP_KDS2018::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_GDS_KS)
	m_pData = pParamData;
}

CETC_EP_KDS2018::~CETC_EP_KDS2018()
{
}

void CETC_EP_KDS2018::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KS)
	DDX_Control(pDX, IDC_CMD_ZONE_CMB, m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_EP_EDIT, m_editDampingRatio);
	DDX_Control(pDX, IDC_CMD_EP_FA_EDIT, m_editFa);
	DDX_Control(pDX, IDC_CMD_EP_FV_EDIT, m_editFv);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_EP_KDS2018, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_KS)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB, OnSelchangeCmdZoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_IMPORTANCE_CMB, OnSelchangeCmdImpCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_IMPORTANCE_CMB, OnEditchangeCmdImpCmb)
	ON_EN_CHANGE(IDC_CMD_EP_EDIT, OnSelchangeCmdDampingEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS message handlers


BOOL CETC_EP_KDS2018::OnInitDialog()
{
	CMyChildDialog::OnInitDialog();


	InitZoneCombo();
	InitImportanceCombo();
	InitSiteCombo();

	GetDlgItem(IDC_CMD_EP_FA_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_EP_FV_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_EP_FA_EDIT)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_EP_FV_EDIT)->ShowWindow(FALSE);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




void CETC_EP_KDS2018::OnSelchangeCmdZoneCmb()
{
	SetFaFv();
}

void CETC_EP_KDS2018::OnSelchangeCmdImpCmb()
{
	SetFaFv();
}

void CETC_EP_KDS2018::OnEditchangeCmdImpCmb()
{
	SetFaFv();
}

void CETC_EP_KDS2018::OnSelchangeCmdSiteCmb()
{
	SetFaFv();

	int nSiteClass = m_cmbSiteClass.GetCurSel();

	if (nSiteClass == 0) // S1
	{
		GetDlgItem(IDC_CMD_EP_FA_STC)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_EP_FV_STC)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_EP_FA_EDIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_EP_FV_EDIT)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_EP_FA_STC)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_EP_FV_STC)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_EP_FA_EDIT)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_EP_FV_EDIT)->ShowWindow(TRUE);

		if (nSiteClass == 5) //S6
		{
			m_editFa.SetReadOnly(FALSE);
			m_editFv.SetReadOnly(FALSE);
		}
		else if (nSiteClass != 0)
		{
			m_editFa.SetReadOnly(TRUE);
			m_editFv.SetReadOnly(TRUE);
		}
	}
}


void CETC_EP_KDS2018::OnSelchangeCmdDampingEdit()
{	
	m_dDampingRatio = m_editDampingRatio.GetEditValue();
	Dlg2Data();
}


void CETC_EP_KDS2018::Data2Dlg()
{
	SetPushOverData(TRUE);
}

BOOL CETC_EP_KDS2018::Dlg2Data()
{
	m_pData->KDS2018.nSeismicZone = m_nZone;
	m_pData->KDS2018.nSiteClass = m_nSite;
	m_pData->KDS2018.dSeismicRiskFactor = m_dSeismicRiskFactor;
	m_pData->KDS2018.dFa = m_dFa;
	m_pData->KDS2018.dFv = m_dFv;
	m_pData->KDS2018.dDampingRatio = m_dDampingRatio;

	return TRUE;
}


void CETC_EP_KDS2018::InitZoneCombo()
{
	CString aItem[2] = { _T("Zone1(0.11)"), _T("Zone2(0.07)") };

	m_cmbSeisZone.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZone.AddString(aItem[i]);
	m_cmbSeisZone.SetCurSel(0);
}

void CETC_EP_KDS2018::InitImportanceCombo()
{
	CString aItem[7] = { _T("0.4"), _T("0.57"), _T("0.73"), _T("1.0"), _T("1.4"), _T("2.0"), _T("2.6") };

	m_cmbImportance.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(0);
}

void CETC_EP_KDS2018::InitSiteCombo()
{
	CString aItem[6] = { _T("S1"), _T("S2"), _T("S3"), _T("S4"), _T("S5"), _T("S6") };

	m_cmbSiteClass.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(0);
}


void CETC_EP_KDS2018::SetFaFv()
{
	int nZone = m_cmbSeisZone.GetCurSel();
	CString strI;
	m_cmbImportance.GetLBText(m_cmbImportance.GetCurSel(), strI);
	double dI = _tstof(strI);

	double dZoneFactor = CalZonFactor(nZone, dI);
	int    nSiteClass = m_cmbSiteClass.GetCurSel();

	double dFa, dFv;

	if (nSiteClass == 0)
	{
		dFa = dFv = 0.0;
	}
	else if (nSiteClass == 5) // S6
	{
		dFa = m_editFa.GetEditValue();
		dFv = m_editFv.GetEditValue();
	}
	else
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		if (!pDoc->m_pAttrCtrl->CalcKSCE2019Fa(dZoneFactor, nSiteClass, dFa)) return;
		if (!pDoc->m_pAttrCtrl->CalcKSCE2019Fv(dZoneFactor, nSiteClass, dFv)) return;
	}
	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);
	m_nZone = nZone;
	m_dSeismicRiskFactor = dI;
	m_nSite = nSiteClass;
	m_dFa = dFa;
	m_dFv = dFv;
	Dlg2Data();
}


double CETC_EP_KDS2018::CalZonFactor(int nZone, double dI)
{
	double dZone = (nZone == 0) ? 0.11 : 0.07;

	double dZoneFactor = dZone * dI;

	return dZoneFactor;
}


void CETC_EP_KDS2018::SetPushOverData(BOOL bInit/*=TRUE*/)
{

	m_nZone = m_pData->KDS2018.nSeismicZone;
	m_nSite = m_pData->KDS2018.nSiteClass;
	m_dSeismicRiskFactor = m_pData->KDS2018.dSeismicRiskFactor;
	m_dDampingRatio = m_pData->KDS2018.dDampingRatio;

	m_dZoneFactor = CalZonFactor(m_nZone, m_dSeismicRiskFactor);

	if (m_nSite == 0)
	{
		m_dFa = m_dFv = 0.0;
	}
	else if (m_nSite == 5) // S6
	{
		// 사용자 입력, 저장된 값 사용
		m_dFa = m_pData->KDS2018.dFa;
		m_dFv = m_pData->KDS2018.dFv;
	}
	else
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		if (!pDoc->m_pAttrCtrl->CalcKSCE2019Fa(m_dZoneFactor, m_nSite, m_dFa)) return;
		if (!pDoc->m_pAttrCtrl->CalcKSCE2019Fv(m_dZoneFactor, m_nSite, m_dFv)) return;
	}


	if (bInit)
	{
		CString strI, strR;
		strI.Format(_T("%g"), m_dSeismicRiskFactor);
		if (CB_ERR == m_cmbImportance.FindStringExact(-1, strI))
		{
			m_cmbImportance.SetWindowText(strI);
		}
		else
		{
			m_cmbImportance.SelectString(-1, strI);
		}
		//m_cmbImportance.SetWindowText(strI);    

		// 3. 데이터를 넣는다.
		m_cmbSeisZone.SetCurSel(m_nZone);
		m_cmbSiteClass.SetCurSel(m_nSite);

		m_editFa.SetEditUnit(m_dFa, 5);
		m_editFv.SetEditUnit(m_dFv, 5);

		//////////////////////////////////////////////////////////////////////////
		if (m_nSite == 0) // S1
		{
			GetDlgItem(IDC_CMD_EP_FA_STC)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_EP_FV_STC)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_EP_FA_EDIT)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_EP_FV_EDIT)->ShowWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_CMD_EP_FA_STC)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_EP_FV_STC)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_EP_FA_EDIT)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_EP_FV_EDIT)->ShowWindow(TRUE);

			if (m_nSite == 5) //S6
			{
				m_editFa.SetReadOnly(FALSE);
				m_editFv.SetReadOnly(FALSE);
			}
			else if (m_nSite != 0)
			{
				m_editFa.SetReadOnly(TRUE);
				m_editFv.SetReadOnly(TRUE);
			}
		}
	}
	m_editDampingRatio.SetEditUnit(m_dDampingRatio);

}