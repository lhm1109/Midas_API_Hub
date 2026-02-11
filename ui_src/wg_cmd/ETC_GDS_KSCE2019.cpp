// ETC_GDS_KSCE2019.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_KSCE2019.h"

#include "SpfcKSCE2019Util.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KSCE2019 dialog


CETC_GDS_KSCE2019::CETC_GDS_KSCE2019(T_SPFC_D* pParamData, BOOL bUseTs, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_KSCE2019::IDD, pParent, std::make_shared<CSpfcKSCE2019Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_KSCE2019)
	//}}AFX_DATA_INIT
	m_pParamData = pParamData;
	m_bUseTs = bUseTs;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcKSCE2019Util>(m_pSpfcBaseUtil);

	if (!m_pParamData->SpecFuncName.IsEmpty())
	{
		m_pSpfcUtil->SetZone(m_pParamData->CodeParam.KSCE2019.nSeisZone);
		m_pSpfcUtil->SetSite(m_pParamData->CodeParam.KSCE2019.nSiteClass);
		m_pSpfcUtil->SetI(m_pParamData->CodeParam.KSCE2019.dIe);
		m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.KSCE2019.dMaxPeriod);
		m_pSpfcUtil->SetFa(m_pParamData->CodeParam.KSCE2019.dFa);
		m_pSpfcUtil->SetFv(m_pParamData->CodeParam.KSCE2019.dFv);
	}
	else
	{
		m_pSpfcUtil->SetZone(0);
		m_pSpfcUtil->SetSite(0);
		m_pSpfcUtil->SetI(1.0);
		m_pSpfcUtil->SetMaxPeriod(6.0);
		m_pSpfcUtil->SetFa(0.0);
		m_pSpfcUtil->SetFv(0.0);
	}

	SetInitValueforPOSpectrum();
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CETC_GDS_KSCE2019::SetInitValueforPOSpectrum()
{
// 확인 필요
//   m_dFa  = 1.46;
//   m_dFv  = 1.58;
//   m_dTs = 1.0; //m_dSd1 / m_dSds;
//   m_dI = 1.0;
//   m_dMaxPeriod = PRD_ED;
}

void CETC_GDS_KSCE2019::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KSCE2019)
	DDX_Control(pDX, IDC_CMD_ZONE_CMB,         m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB,   m_cmbImportance);	
	DDX_Control(pDX, IDC_CMD_SITE_CMB,         m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_FA_EDIT,          m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT,          m_editFv);
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT,      m_editMaxPeriod);	
	DDX_Control(pDX, IDC_CMD_TS_EDT,           m_edtTs);
	DDX_Control(pDX, IDC_CMD_TS_UNT,           m_untTs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_KSCE2019, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_KSCE2019)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB,       OnSelchangeCmdZoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_IMPORTANCE_CMB, OnSelchangeCmImpFactorCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_IMPORTANCE_CMB,OnEditchangeCmImpFactorCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB,       OnSelchangeCmdSiteCmb)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KSCE2019 message handlers

BOOL CETC_GDS_KSCE2019::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	InitUnit();
	InitZoneCombo();
	InitIFactorCombo();  
	InitSiteCombo();
	m_editMaxPeriod.SetEditUnit(PRD_ED);

	GetDlgItem(IDC_CMD_FA_STC )->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_FV_STC )->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_FA_EDIT)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_FV_EDIT)->ShowWindow(FALSE);

	AlignControls();
	ShowHideControls();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_KSCE2019::SetFaFv()
{
	int nZone = m_cmbSeisZone.GetCurSel();
	CString strI;
	m_cmbImportance.GetLBText(m_cmbImportance.GetCurSel(), strI);    
	double dI = _tstof(strI);      

	double dZoneFactor = m_pSpfcUtil->CalZonFactor(nZone, dI);
	int    nSiteClass   = m_cmbSiteClass.GetCurSel();

	double dFa, dFv;  

	if(nSiteClass==0)
	{
		dFa = dFv = 0.0;
	}
	else if(nSiteClass==5) // S6
	{
		// 사용자 입력, 저장된 값 사용
		dFa = m_editFa.GetEditValue();
		dFv = m_editFv.GetEditValue();
	}
	else
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		if(!pDoc->m_pAttrCtrl->CalcKSCE2019Fa(dZoneFactor, nSiteClass, dFa)) return;
		if(!pDoc->m_pAttrCtrl->CalcKSCE2019Fv(dZoneFactor, nSiteClass, dFv)) return;
	}
	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);

	if(m_bUseTs)
	{
		GetFinalValue();
		const double ts = m_pSpfcUtil->GetTs();
		m_edtTs.SetEditUnit(ts,5);
	}
}

void CETC_GDS_KSCE2019::OnSelchangeCmdZoneCmb() 
{ 
	SetFaFv();
}

void CETC_GDS_KSCE2019::OnSelchangeCmImpFactorCmb() 
{
	SetFaFv();
}
void CETC_GDS_KSCE2019::OnEditchangeCmImpFactorCmb() 
{
	SetFaFv();
}

void CETC_GDS_KSCE2019::OnSelchangeCmdSiteCmb() 
{
	SetFaFv();

	int nSiteClass = m_cmbSiteClass.GetCurSel();

	if(nSiteClass==0) // S1
	{
		GetDlgItem(IDC_CMD_FA_STC )->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_FV_STC )->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_FA_EDIT)->ShowWindow(FALSE); 
		GetDlgItem(IDC_CMD_FV_EDIT)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_FA_STC )->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_FV_STC )->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_FA_EDIT)->ShowWindow(TRUE); 
		GetDlgItem(IDC_CMD_FV_EDIT)->ShowWindow(TRUE);

		if(nSiteClass==5) //S6
		{
			m_editFa.SetReadOnly(FALSE);
			m_editFv.SetReadOnly(FALSE);
		}
		else if(nSiteClass!=0)
		{
			m_editFa.SetReadOnly(TRUE);
			m_editFv.SetReadOnly(TRUE);
		}
	}
}

void CETC_GDS_KSCE2019::InitUnit()
{
	m_edtTs.SetUnitType(D_UNITSYS_BASE_TIME);
	m_untTs.SetUnitType(D_UNITSYS_BASE_TIME);
}

void CETC_GDS_KSCE2019::InitZoneCombo()
{
	CString aItem[2] = {_T("Zone1(0.11)"), _T("Zone2(0.07)")};

	m_cmbSeisZone.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZone.AddString(aItem[i]);
	m_cmbSeisZone.SetCurSel(0);
}

void CETC_GDS_KSCE2019::InitIFactorCombo()
{
	CString aItem[7] = {_T("0.4"), _T("0.57"), _T("0.73"), _T("1.0"), _T("1.4"), _T("2.0"), _T("2.6")};

	m_cmbImportance.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(0);
}

void CETC_GDS_KSCE2019::InitSiteCombo()
{
	CString aItem[6] = {_T("S1"), _T("S2"), _T("S3"), _T("S4"), _T("S5"), _T("S6")};

	m_cmbSiteClass.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(0);
}

void CETC_GDS_KSCE2019::AlignControls()
{
	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	CArray<UINT, UINT> aPeriod; aPeriod.RemoveAll();
	CArray<UINT, UINT> aTs; aTs.RemoveAll();
	
	aPeriod.Add(IDC_CMD_PERIOD_TEXT);
	aPeriod.Add(IDC_CMD_PERIOD_EDIT);
	aPeriod.Add(IDC_CMD_PERIOD_UNIT);

	aTs.Add(IDC_CMD_TS_STC);
	aTs.Add(IDC_CMD_TS_EDT);
	aTs.Add(IDC_CMD_TS_UNT);

	GetDlgItem(IDC_CMD_PERIOD_EDIT)->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_TS_EDT     )->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aTs, nDistX, nDistY);
}

void CETC_GDS_KSCE2019::ShowHideControls()
{
	CArray<UINT, UINT> aPeriod; aPeriod.RemoveAll();
	CArray<UINT, UINT> aTs; aTs.RemoveAll();

	aPeriod.Add(IDC_CMD_PERIOD_TEXT);
	aPeriod.Add(IDC_CMD_PERIOD_EDIT);
	aPeriod.Add(IDC_CMD_PERIOD_UNIT);

	aTs.Add(IDC_CMD_TS_STC);
	aTs.Add(IDC_CMD_TS_EDT);
	aTs.Add(IDC_CMD_TS_UNT);

	CDlgUtil::CtrlShowHide(this, aPeriod, !m_bUseTs);
	CDlgUtil::CtrlShowHide(this, aTs, m_bUseTs);
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_KSCE2019::ISVALID()
{ 	
	GetFinalValue();
	return m_pSpfcUtil->CheckValid();
}

void CETC_GDS_KSCE2019::End()
{
	CMyChildDialog::End();
}

void CETC_GDS_KSCE2019::GetFinalValue()
{
	CString str;

	m_pSpfcUtil->SetZone(m_cmbSeisZone.GetCurSel());

	CString strI;
	m_cmbImportance.GetWindowText(strI);
	m_pSpfcUtil->SetI(_tstof(strI));

	m_pSpfcUtil->SetZoneFactor(m_pSpfcUtil->CalZonFactor(m_pSpfcUtil->GetZone(), m_pSpfcUtil->GetI()));

	m_pSpfcUtil->SetFa(m_editFa.GetEditValue());
	m_pSpfcUtil->SetFv(m_editFv.GetEditValue());
	m_pSpfcUtil->SetSite(m_cmbSiteClass.GetCurSel());
	if (m_pSpfcUtil->GetSite() == 0) // S1
	{
		m_pSpfcUtil->SetT0(0.06);
		m_pSpfcUtil->SetTs(0.3);
		m_pSpfcUtil->SetTl(3.0);
	}
	else
	{
		double dTs = (m_pSpfcUtil->GetFa() == 0.0) ? 0.0 : m_pSpfcUtil->GetFv() / (2.5 * m_pSpfcUtil->GetFa());
		m_pSpfcUtil->SetTs(dTs);
		m_pSpfcUtil->SetT0(0.2 * dTs);
		m_pSpfcUtil->SetTl(3.0);
	}

	m_pSpfcUtil->SetMaxPeriod(m_editMaxPeriod.GetEditValue());
}

BOOL CETC_GDS_KSCE2019::Dlg2Data()
{
	m_pParamData->CodeParam.KSCE2019.nSeisZone = m_pSpfcUtil->GetZone();
	m_pParamData->CodeParam.KSCE2019.nSiteClass = m_pSpfcUtil->GetSite();
	m_pParamData->CodeParam.KSCE2019.dIe = m_pSpfcUtil->GetI();
	m_pParamData->CodeParam.KSCE2019.dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();
	m_pParamData->CodeParam.KSCE2019.dFa = m_pSpfcUtil->GetFa();
	m_pParamData->CodeParam.KSCE2019.dFv = m_pSpfcUtil->GetFv();

	return TRUE;
}

void CETC_GDS_KSCE2019::Data2Dlg()
{
	SetPushOverData(TRUE);
}

void CETC_GDS_KSCE2019::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData->nSpecCodeType != D_SPFC_CODE_KSCE2019) return;

	if(!m_pParamData->SpecFuncName.IsEmpty())
	{
		m_pSpfcUtil->SetZone(m_pParamData->CodeParam.KSCE2019.nSeisZone);
		m_pSpfcUtil->SetSite(m_pParamData->CodeParam.KSCE2019.nSiteClass);
		m_pSpfcUtil->SetI(m_pParamData->CodeParam.KSCE2019.dIe);
		m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.KSCE2019.dMaxPeriod);
	}
	else
	{
		m_pSpfcUtil->SetZone(0);
		m_pSpfcUtil->SetSite(0);
		m_pSpfcUtil->SetI(1.0);
		m_pSpfcUtil->SetMaxPeriod(6.0);
	}

	const double zoneFactor = m_pSpfcUtil->CalZonFactor(m_pSpfcUtil->GetZone(), m_pSpfcUtil->GetI());
	m_pSpfcUtil->SetZoneFactor(zoneFactor);

	const int site = m_pSpfcUtil->GetSite();
	if (site == 0)
	{
		m_pSpfcUtil->SetFa(0.0);
		m_pSpfcUtil->SetFv(0.0);
	}
	else if (site == 5) // S6
	{
		// 사용자 입력, 저장된 값 사용
		m_pSpfcUtil->SetFa(m_pParamData->CodeParam.KSCE2019.dFa);
		m_pSpfcUtil->SetFv(m_pParamData->CodeParam.KSCE2019.dFv);
	}
	else
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		double dFa, dFv;
		if (!pDoc->m_pAttrCtrl->CalcKSCE2019Fa(zoneFactor, m_pSpfcUtil->GetSite(), dFa)) return;
		if (!pDoc->m_pAttrCtrl->CalcKSCE2019Fv(zoneFactor, m_pSpfcUtil->GetSite(), dFv)) return;
		m_pSpfcUtil->SetFa(dFa);
		m_pSpfcUtil->SetFv(dFv);
	}

	if (site == 0) // S1
	{
		m_pSpfcUtil->SetT0(0.06);
		m_pSpfcUtil->SetTs(0.3);
		m_pSpfcUtil->SetTl(3.0);
	}
	else
	{
		const double dTs = m_pSpfcUtil->GetFv() / (2.5 * m_pSpfcUtil->GetFa());
		m_pSpfcUtil->SetTs(dTs);
		m_pSpfcUtil->SetT0(0.2 * dTs);
		m_pSpfcUtil->SetTl(3.0);
	}

	if(bInit)
	{
		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetI());
		if(CB_ERR == m_cmbImportance.FindStringExact(-1, strI))
		{
			m_cmbImportance.SetWindowText(strI);
		}
		else
		{
			m_cmbImportance.SelectString(-1, strI);
		}
		//m_cmbImportance.SetWindowText(strI);    

		// 3. 데이터를 넣는다.
		m_cmbSeisZone.SetCurSel(m_pSpfcUtil->GetZone());
		m_cmbSiteClass.SetCurSel(m_pSpfcUtil->GetSite());

		m_editFa.SetEditUnit(m_pSpfcUtil->GetFa(), 5);
		m_editFv.SetEditUnit(m_pSpfcUtil->GetFv(), 5);
	
		//////////////////////////////////////////////////////////////////////////
		if (site == 0) // S1
		{
			GetDlgItem(IDC_CMD_FA_STC )->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_FV_STC )->ShowWindow(FALSE);
			GetDlgItem(IDC_CMD_FA_EDIT)->ShowWindow(FALSE); 
			GetDlgItem(IDC_CMD_FV_EDIT)->ShowWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_CMD_FA_STC )->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_FV_STC )->ShowWindow(TRUE);
			GetDlgItem(IDC_CMD_FA_EDIT)->ShowWindow(TRUE); 
			GetDlgItem(IDC_CMD_FV_EDIT)->ShowWindow(TRUE);

			if (site == 5) //S6
			{
				m_editFa.SetReadOnly(FALSE);
				m_editFv.SetReadOnly(FALSE);
			}
			else
			{
				m_editFa.SetReadOnly(TRUE);
				m_editFv.SetReadOnly(TRUE);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		m_editMaxPeriod.SetEditUnit(m_pSpfcUtil->GetMaxPeriod());

		if(m_bUseTs)
		{
			GetFinalValue();
			const double ts = m_pSpfcUtil->GetTs();
			m_edtTs.SetEditUnit(ts,5);
		}
	}

}

double CETC_GDS_KSCE2019::GetTs() const
{
	return m_pSpfcUtil->GetTs();
}
