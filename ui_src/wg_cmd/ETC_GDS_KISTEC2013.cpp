// ETC_GDS_KISTEC2013.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_KISTEC2013.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KISTEC2013 dialog


CETC_GDS_KISTEC2013::CETC_GDS_KISTEC2013(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_KISTEC2013::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_GDS_KISTEC2013)
	//}}AFX_DATA_INIT
	m_pParamData = pParamData;
	SetInitValueforPOSpectrum();
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CETC_GDS_KISTEC2013::SetInitValueforPOSpectrum()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CArray<T_STOR_K, T_STOR_K> aKeyList;
	pDoc->m_pAttrCtrl->GetStorKeyList(aKeyList);

	// Additional Seismic Load Item 을 만든다.

	m_dFa  = 1.36;
	m_dFv  = 1.96;
	m_dSds = 0.52668;
	m_dSd1 = 0.33649;
	m_dTs = m_dSd1 / m_dSds;
	m_dR = 1.0;
	m_dI = 1.0;
	if(aKeyList.IsEmpty())
	{
		m_dC = 1.0;
	}
	else
	{
		T_SEME_D data;
		pDoc->m_pAttrCtrl2->GetSeme(data);
		int nStructureType = data.data.eval.nResistSystem;
		double dTable4_2_1[3][4] = {{1.3, 1.1, 1.0, 1.0},
																{1.4, 1.2, 1.1, 1.0},
																{1.0, 1.0, 1.0, 1.0}};
		int nCount = aKeyList.GetSize();
		 
		ASSERT(nCount >= 0);

		if(nCount>=4) m_dC = dTable4_2_1[nStructureType][3];
		else m_dC = dTable4_2_1[nStructureType][nCount-1];
	}
	m_bRelieve = FALSE;
	m_dMaxPeriod = PRD_ED;
}

void CETC_GDS_KISTEC2013::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KISTEC2013)
	DDX_Control(pDX, IDC_CMD_ZONE_CMB, m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_ZONE_FACTOR__CMB, m_cmbSeisZoneFactor);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_FA_EDIT, m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT, m_editFv);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
	DDX_Control(pDX, IDC_CMD_C_EDT, m_editC);
	DDX_Control(pDX, IDC_CMD_R_CMB, m_cmbR);	
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);	
 	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_KISTEC2013, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_KISTEC2013)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB, OnSelchangeCmdZoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_FACTOR__CMB, OnSelchangeCmdZoneFactorCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)

	ON_CBN_EDITCHANGE(IDC_CMD_ZONE_FACTOR__CMB, OnChangeCmdZoneFactor)

	ON_EN_CHANGE(IDC_CMD_FA_EDIT,      OnChangeCmdFaEdit)
	ON_EN_CHANGE(IDC_CMD_FV_EDIT,      OnChangeCmdFvEdit)
	ON_EN_CHANGE(IDC_CMD_SDS_EDIT,     OnChangeCmdSdsEdit)
	ON_EN_CHANGE(IDC_CMD_SD1_EDIT,     OnChangeCmdSd1Edit)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KISTEC2013 message handlers

BOOL CETC_GDS_KISTEC2013::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	InitZoneCombo();
	InitZoneFactorCombo();
	InitSiteCombo();
	InitIFactorCombo();  
	InitRCombo();

	SetFaFvSdsSd1();
	m_editC.SetEditUnit(m_dC);

	Data2Dlg();
	
	m_editMaxPeriod.SetEditUnit(PRD_ED);

	m_cmbSeisZone .EnableWindow(FALSE);
	m_cmbSiteClass.EnableWindow(FALSE);

	//CtrlManager();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_KISTEC2013::SetFaFvSdsSd1()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();

	CString str; 
	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	int    nSiteClass   = m_cmbSiteClass.GetCurSel();

	double dFa, dFv, dSds, dSd1;  
	// dCu 는 여기서 사용 안함!!!!

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	// PMS:4428-HSSHIM-20120104. ZoneFactor에 의한 보간방법을 사용하도록 변경
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Fa(dZoneFactor, nSeismicZone, nSiteClass, dFa)) return;
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Fv(dZoneFactor, nSeismicZone, nSiteClass, dFv)) return;
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;

	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);
	m_editSds.SetEditUnit(dSds, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
}

void CETC_GDS_KISTEC2013::OnSelchangeCmdZoneCmb() 
{ 
	SetComboValue();
	SetFaFvSdsSd1();
}

void CETC_GDS_KISTEC2013::OnSelchangeCmdZoneFactorCmb() 
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	CString csValue;

	int nTmp = m_cmbSeisZoneFactor.GetCurSel();
	if (nTmp == 0)
	{
		csValue.Format(_T("%g"), 0.14);
		m_cmbSeisZoneFactor.SetWindowText(csValue);
	}
	else
	{
		csValue.Format(_T("%g"), 0.22);
		m_cmbSeisZoneFactor.SetWindowText(csValue);
	}

	SetFaFvSdsSd1();
}

void CETC_GDS_KISTEC2013::OnSelchangeCmdSiteCmb() 
{
	SetFaFvSdsSd1();
}

void CETC_GDS_KISTEC2013::OnChangeCmdZoneFactor() 
{
	SetFaFvSdsSd1();
}

void CETC_GDS_KISTEC2013::OnChangeCmdFaEdit() 
{
	CString str;

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	double dFa = m_editFa.GetEditValue();
	double dSds;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;

	m_editSds.SetEditUnit(dSds, 5);
}

void CETC_GDS_KISTEC2013::OnChangeCmdFvEdit() 
{
	CString str; 

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	double dFv = m_editFv.GetEditValue();
	double dSd1;  
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;

	m_editSd1.SetEditUnit(dSd1, 5);
}

void CETC_GDS_KISTEC2013::OnChangeCmdSdsEdit() 
{
}

void CETC_GDS_KISTEC2013::OnChangeCmdSd1Edit() 
{
}

void CETC_GDS_KISTEC2013::InitZoneCombo()
{
	CString aItem[2] = {_T("1"), _T("2")};

	m_cmbSeisZone.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		CDlgUtil::CobxAddItem(m_cmbSeisZone,aItem[i],i);
	m_cmbSeisZone.SetCurSel(0);
}

void CETC_GDS_KISTEC2013::InitZoneFactorCombo()
{
	CString aItem[2] = {_T("0.14"), _T("0.22")};

	m_cmbSeisZoneFactor.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZoneFactor.AddString(aItem[i]);
	m_cmbSeisZoneFactor.SetCurSel(1);
}

void CETC_GDS_KISTEC2013::InitSiteCombo()
{
	CString aItem[5] = {_T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se")};

	m_cmbSiteClass.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		CDlgUtil::CobxAddItem(m_cmbSiteClass,aItem[i],i);
	m_cmbSiteClass.SetCurSel(3);
}

void CETC_GDS_KISTEC2013::InitIFactorCombo()
{
	CString aItem[3] = {_T("1.0"), _T("1.2"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(0);
}

void CETC_GDS_KISTEC2013::InitRCombo()
{
	CString aItem[15] = {_T("1.25"), _T("1.5"), _T("2.5"), _T("3"), _T("3.25"), _T("3.5"), _T("4"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("7.5"), _T("8")};

	m_cmbR.ResetContent();	

	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbR.AddString(aItem[i]);		  
	m_cmbR.SetWindowText(_T("1.0"));
}

void CETC_GDS_KISTEC2013::SetComboValue()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();

	if(nSeismicZone == 0)
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.22"));
	}
	else 
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.14"));
	}
}

void CETC_GDS_KISTEC2013::MakeSpectrumData(BOOL bOnlyCalc/*=FALSE*/)
{
	double st	= PRD_ST;
	double ed = m_dMaxPeriod;
	int Step	= PRD_NUM;
	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if(bOnlyCalc)
	{
		m_dT0 = 0.2 * m_dTs;
		ed*=m_nPeriodCoeff;
		Step*=m_nStepCoeff;    
	}

	double Tn, dt;
	double dSa;
		
	dt = (ed-st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance
	  
	BOOL bRelieve = FALSE;
	if(m_bRelieve && m_dT0 > 0.06 && m_dT0 < 0.3)
	{
		m_dT0 = 0.3;
		bRelieve = TRUE;
	}

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if(Tn >= 0 && Tn <= m_dT0)				 
		{
			if(bRelieve) dSa = (2 * m_dSds * Tn + 0.4 * m_dSds) * m_dI / m_dR * m_dC;
			else         dSa = (0.6 * (m_dSds / m_dT0) * Tn + 0.4 * m_dSds) * m_dI / m_dR * m_dC;
		}
		else if(Tn > m_dT0 && Tn <= m_dTs) dSa = m_dSds * m_dI / m_dR * m_dC; 
		else if(Tn > m_dTs)				 			   dSa = min((m_dSd1 / Tn) * m_dI / m_dR * m_dC, m_dSds * m_dI / m_dR * m_dC);
				
		if(Tn > m_dT0  + dTol && Tn < m_dT0 + dt - dTol) // transition point		
		{
			m_parPeriod->Add(m_dT0);
		  m_parAccel->Add(m_dSds * m_dI / m_dR * m_dC);
		}		
		if(Tn > m_dTs + dTol && Tn < m_dTs + dt - dTol) // transition point		
		{
			m_parPeriod->Add(m_dTs);
			m_parAccel->Add(m_dSds * m_dI / m_dR * m_dC);
		}	
		
	  m_parPeriod->Add(Tn);
		m_parAccel->Add(dSa);
	}
	
	if(bOnlyCalc) return;

	CString strSite[]={_T("Sa"),_T("Sb"),_T("Sc"),_T("Sd"),_T("Se")};
	
	// Zone 은 0 = 1, 1 = 2
	// 수정할려면 CString strSite[]={_T("1"),_T("2")}; 이거 추가 한 후에 Msg 수정
	m_sDescript.Format(_LS(IDS_WG_CMD__ADD_KISTEC2013_PROFILE), m_nZone+1, m_dZoneFactor, strSite[m_nSite], m_dFa, m_dFv, m_dSds, m_dSd1, m_dI, m_dR, m_dC);

	m_strFuncName=_T("KISTEC2013");  
}

void CETC_GDS_KISTEC2013::MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel)// for Pushover Curve
{  
	aPeriod.RemoveAll();
	aAccel.RemoveAll();
 
	double st= PRD_ST;
	double ed= m_dMaxPeriod*2;
	int Step =  PRD_NUM*4;
	double Tn, dt;
 	double dSa;
	double dTs, dT0;
	
	if(dDamping >= 0)
	{
		SRa = (3.21-0.68*log(dDamping))/2.12;  
		SRv = (2.31-0.41*log(dDamping))/1.65;  
	}  

	dt = (ed-st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	dTs = m_dTs * SRv / SRa;
	dT0 = 0.2 * dTs;

	BOOL bRelieve = FALSE;
	if(m_bRelieve && dT0 > 0.06 && dT0 < 0.3)
	{
		dT0 = 0.3;
		bRelieve = TRUE;
	}
	
	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
	
		if(Tn >= 0 && Tn <= dT0)			 
		{
			if(bRelieve) dSa = (0.4 + (SRa - 0.4) * Tn / 0.3) * m_dSds * m_dI / m_dR * m_dC;
			else         dSa = (0.4 + (SRa - 0.4) * Tn / dT0) * m_dSds * m_dI / m_dR * m_dC; 
		}
		else if(Tn > dT0 && Tn <= dTs) dSa = (m_dSds * m_dI / m_dR) * SRa * m_dC; 
		else if(Tn > dTs)					 	   dSa = min((m_dSd1 / Tn) * (m_dI / m_dR) * SRv * m_dC, (m_dSds * m_dI / m_dR) * SRa * m_dC);
				
		if(Tn > dT0 + dTol && Tn < dT0 + dt - dTol) // transition point		
		{
			aPeriod.Add(dT0);			
		  aAccel.Add((m_dSds * m_dI / m_dR) * SRa * m_dC);
		}		
		if(Tn > dTs + dTol && Tn < dTs + dt - dTol) // transition point		
		{
			aPeriod.Add(dTs);			
			aAccel.Add((m_dSds * m_dI / m_dR) * SRa * m_dC);
		}	
		
	  aPeriod.Add(Tn);
		aAccel.Add(dSa);
	}  
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_KISTEC2013::ISVALID()
{ 	
	GetFinalValue();

	CString strMsg;  
	if(m_dFa <= 0.)
	{    
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_dFv <= 0.)
	{    
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_dSds <= 0.)
	{    
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_dSd1 <= 0.)
	{    
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sd1"));    
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_dI <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ie"));    
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_dR <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));    
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if(m_dC <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("C"));    
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_KISTEC2013::End()
{
	CMyChildDialog::End();
}

void CETC_GDS_KISTEC2013::GetFinalValue()
{
	CString str; 

	m_nZone = m_cmbSeisZone.GetCurSel();
	
	m_cmbSeisZoneFactor.GetWindowText(str);
	m_dZoneFactor = _tstof(str);
	
	m_nSite = m_cmbSiteClass.GetCurSel();    
	m_dFa  = m_editFa.GetEditValue();
	m_dFv  = m_editFv.GetEditValue();
 	m_dSds  = m_editSds.GetEditValue();
	m_dSd1  = m_editSd1.GetEditValue();  
	m_dC    = m_editC  .GetEditValue();  
	m_dTs = m_dSd1 / m_dSds;
	m_dT0 = 0.2 * m_dTs;

	CString strI, strR;
	m_cmbImportance.GetWindowText(strI);    
	m_cmbR.GetWindowText(strR);
	m_dI = _tstof(strI);      
	m_dR = _tstof(strR);
	
	m_dMaxPeriod = m_editMaxPeriod.GetEditValue();
}

void CETC_GDS_KISTEC2013::UpdateDlgData()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	T_SEME_D SemeD;

	pDoc->m_pAttrCtrl2->GetSeme(SemeD);

	CDlgUtil::CobxSetCurSelItemData(m_cmbSeisZone, SemeD.data.eval.nSeismicZone);
	CDlgUtil::CobxSetCurSelItemData(m_cmbSiteClass, SemeD.data.eval.nSiteClass);

	SetComboValue();
	SetFaFvSdsSd1();
}

BOOL CETC_GDS_KISTEC2013::Dlg2Data()
{
	m_pParamData->dResModCoef =  m_dR;
	return TRUE;
}
void CETC_GDS_KISTEC2013::Data2Dlg()
{
	SetPushOverData(TRUE);
}