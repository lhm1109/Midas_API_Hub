// ETC_GDS_KDS2019.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_KDS2019.h"
#include "SpfcKDS2019Util.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KDS2019 dialog


CETC_GDS_KDS2019::CETC_GDS_KDS2019(T_SPFC_D* pParamData, BOOL bUseTs, CWnd* pParent /*=NULL*/)
    : CMyChildDialog(CETC_GDS_KDS2019::IDD, pParent, std::make_shared<CSpfcKDS2019Util>())
{
    //{{AFX_DATA_INIT(CETC_GDS_KDS2019)
    //}}AFX_DATA_INIT
    m_strFuncName=_T("KDS(41-17-00:2019)");
    m_pParamData = pParamData;
    m_bUseTs = bUseTs;
    m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcKDS2019Util>(m_pSpfcBaseUtil);
    SetInitValueforPOSpectrum();
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CETC_GDS_KDS2019::SetInitValueforPOSpectrum()
{
    //m_dFa  = 1.38;
    //m_dFv  = 1.38;
    //m_dSds = 0.506;
    //m_dSd1 = 0.2024;
    //m_dTs = m_dSd1 / m_dSds;
    //m_dR = 4.0;
    //m_dI = 1.2;
    //m_dMaxPeriod = PRD_ED;
}

void CETC_GDS_KDS2019::DoDataExchange(CDataExchange* pDX)
{
    CMyChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CETC_GDS_KDS2019)
    DDX_Control(pDX, IDC_CMD_ZONE_CMB, m_cmbSeisZone);
    DDX_Control(pDX, IDC_CMD_ZONE_FACTOR__CMB, m_cmbSeisZoneFactor);
    DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
    DDX_Control(pDX, IDC_CMD_FA_EDIT, m_editFa);
    DDX_Control(pDX, IDC_CMD_FV_EDIT, m_editFv);
    DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
    DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
    DDX_Control(pDX, IDC_CMD_R_CMB, m_cmbR);	
    DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);	
    DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editMaxPeriod);	
    DDX_Control(pDX, IDC_CMD_TS_EDT,           m_edtTs);
    DDX_Control(pDX, IDC_CMD_TS_UNT,           m_untTs);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_KDS2019, CMyChildDialog)
    //{{AFX_MSG_MAP(CETC_GDS_KDS2019)
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
// CETC_GDS_KDS2019 message handlers

BOOL CETC_GDS_KDS2019::OnInitDialog() 
{
    CMyChildDialog::OnInitDialog();
	GetDlgItem(IDC_WG_CMD_STATIC3)->SetWindowText(_LS(IDS_CMD_EPA));
    GetDlgItem(IDC_WG_CMD_STATIC9)->SetWindowText(_LS(IDS_CMD_EPA));
    
	InitUnit();
    InitZoneCombo();
    InitZoneFactorCombo();
    InitSiteCombo();
    InitIFactorCombo();  
    InitRCombo();

    AlignControls();
    ShowHideControls();

    SetFaFvSdsSd1();

    m_editMaxPeriod.SetEditUnit(PRD_ED);
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_KDS2019::SetFaFvSdsSd1()
{
    CString str; 
    m_cmbSeisZoneFactor.GetWindowText(str);
    double dZoneFactor  = _tstof(str);
    int    nSiteClass   = m_cmbSiteClass.GetCurSel();

    double dFa=0., dFv=0., dSds=0., dSd1=0.;
    if(nSiteClass >= 0 && nSiteClass <= 4)
    {
        CDBDoc* pDoc = CDBDoc::GetDocPoint();
        if(!pDoc->m_pAttrCtrl->CalcKDS2019Fa(dZoneFactor, nSiteClass, dFa)) return;
        if(!pDoc->m_pAttrCtrl->CalcKDS2019Fv(dZoneFactor, nSiteClass, dFv)) return;
        if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;
        if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;
    }

    m_editFa.SetEditUnit(dFa, 5);
    m_editFv.SetEditUnit(dFv, 5);
    m_editSds.SetEditUnit(dSds, 5);
    m_editSd1.SetEditUnit(dSd1, 5);

    if(m_bUseTs)
    {
        GetFinalValue();
        const double ts = m_pSpfcUtil->GetTs();
        m_edtTs.SetEditUnit(ts,5);
    }
}

void CETC_GDS_KDS2019::InitUnit()
{
    m_edtTs.SetUnitType(D_UNITSYS_BASE_TIME);
    m_untTs.SetUnitType(D_UNITSYS_BASE_TIME);
}

void CETC_GDS_KDS2019::OnSelchangeCmdZoneCmb() 
{
    SetComboValue();

    int nSiteClass = m_cmbSiteClass.GetCurSel();
    if(nSiteClass==5) return;
    
    SetFaFvSdsSd1();
}

void CETC_GDS_KDS2019::OnSelchangeCmdZoneFactorCmb() 
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

    int nSiteClass = m_cmbSiteClass.GetCurSel();
    if(nSiteClass==5) return;

    SetFaFvSdsSd1();
}

void CETC_GDS_KDS2019::OnSelchangeCmdSiteCmb() 
{
    SetFaFvSdsSd1();
}

void CETC_GDS_KDS2019::OnChangeCmdZoneFactor() 
{
    int nSiteClass = m_cmbSiteClass.GetCurSel();
    if(nSiteClass==5) return;

    SetFaFvSdsSd1();
}

void CETC_GDS_KDS2019::OnChangeCmdFaEdit() 
{
    int nSiteClass = m_cmbSiteClass.GetCurSel();
    if(nSiteClass==5) return;

    CString str;

    m_cmbSeisZoneFactor.GetWindowText(str);
    double dZoneFactor  = _tstof(str);
    double dFa = m_editFa.GetEditValue();
    double dSds;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;

    m_editSds.SetEditUnit(dSds, 5);
}

void CETC_GDS_KDS2019::OnChangeCmdFvEdit() 
{
    int nSiteClass = m_cmbSiteClass.GetCurSel();
    if(nSiteClass==5) return;

    CString str; 

    m_cmbSeisZoneFactor.GetWindowText(str);
    double dZoneFactor  = _tstof(str);
    double dFv = m_editFv.GetEditValue();
    double dSd1;  
    
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;

    m_editSd1.SetEditUnit(dSd1, 5);
}

void CETC_GDS_KDS2019::OnChangeCmdSdsEdit() 
{
}

void CETC_GDS_KDS2019::OnChangeCmdSd1Edit() 
{
}

void CETC_GDS_KDS2019::InitZoneCombo()
{
    CString aItem[2] = {_T("1"), _T("2")};

    m_cmbSeisZone.ResetContent();
    for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
        m_cmbSeisZone.AddString(aItem[i]);
    m_cmbSeisZone.SetCurSel(0);
}

void CETC_GDS_KDS2019::InitZoneFactorCombo()
{
    CString aItem[2] = {_T("0.14"), _T("0.22")};

    m_cmbSeisZoneFactor.ResetContent();
    for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
        m_cmbSeisZoneFactor.AddString(aItem[i]);
    m_cmbSeisZoneFactor.SetCurSel(1);
}

void CETC_GDS_KDS2019::InitSiteCombo()
{
    CString aItem[6] = {_T("S1"), _T("S2"), _T("S3"), _T("S4"), _T("S5"), _T("S6")};

    m_cmbSiteClass.ResetContent();
    for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
        m_cmbSiteClass.AddString(aItem[i]);
    m_cmbSiteClass.SetCurSel(1);
}

void CETC_GDS_KDS2019::InitIFactorCombo()
{
    CString aItem[3] = {_T("1.0"), _T("1.2"), _T("1.5")};

    m_cmbImportance.ResetContent();
    for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
        m_cmbImportance.AddString(aItem[i]);
    m_cmbImportance.SetCurSel(1);
}

void CETC_GDS_KDS2019::InitRCombo()
{
    CString aItem[15] = {_T("1.25"), _T("1.5"), _T("2.5"), _T("3"), _T("3.25"), _T("3.5"), _T("4"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("7.5"), _T("8")};

    m_cmbR.ResetContent();	

    for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
        m_cmbR.AddString(aItem[i]);		  
    m_cmbR.SetCurSel(6);	
}

void CETC_GDS_KDS2019::AlignControls()
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

void CETC_GDS_KDS2019::ShowHideControls()
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

void CETC_GDS_KDS2019::SetComboValue()
{
    int nSeismicZone = m_cmbSeisZone.GetCurSel();
    int nSiteClass   = m_cmbSiteClass.GetCurSel();

    if(nSeismicZone == 0)
    {
        m_cmbSeisZoneFactor.SetWindowText(_T("0.22"));
    }
    else 
    {
        m_cmbSeisZoneFactor.SetWindowText(_T("0.14"));
    }
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_KDS2019::ISVALID()
{ 	
    GetFinalValue();
    return m_pSpfcUtil->CheckValid();
}

void CETC_GDS_KDS2019::End()
{
    CMyChildDialog::End();
}

void CETC_GDS_KDS2019::GetFinalValue()
{
    m_pSpfcUtil->SetZone(m_cmbSeisZone.GetCurSel());

    CString strZoneFactor;
    m_cmbSeisZoneFactor.GetWindowText(strZoneFactor);
    m_pSpfcUtil->SetZoneFactor(_tstof(strZoneFactor));

    m_pSpfcUtil->SetSite(m_cmbSiteClass.GetCurSel());

    m_pSpfcUtil->SetFa(m_editFa.GetEditValue());
    m_pSpfcUtil->SetFv(m_editFv.GetEditValue());
    m_pSpfcUtil->SetSds(m_editSds.GetEditValue());
    m_pSpfcUtil->SetSd1(m_editSd1.GetEditValue());

    double dTs = m_editSd1.GetEditValue() / m_editSds.GetEditValue();
    m_pSpfcUtil->SetTs(dTs);

    double dT0 = 0.2 * dTs;
    m_pSpfcUtil->SetT0(dT0);

    CString strI, strR;
    m_cmbImportance.GetWindowText(strI);
    m_cmbR.GetWindowText(strR);
    m_pSpfcUtil->SetI(_tstof(strI));
    m_pSpfcUtil->SetR(_tstof(strR));

    m_pSpfcUtil->SetMaxPeriod(m_editMaxPeriod.GetEditValue());
}

BOOL CETC_GDS_KDS2019::Dlg2Data()
{
    m_pParamData->CodeParam.KDS2019.nSeisZone = m_pSpfcUtil->GetZone();
    m_pParamData->CodeParam.KDS2019.nSiteClass = m_pSpfcUtil->GetSite();
    m_pParamData->CodeParam.KDS2019.dZoneFactor = m_pSpfcUtil->GetZoneFactor();
    m_pParamData->CodeParam.KDS2019.dFa = m_pSpfcUtil->GetFa();
    m_pParamData->CodeParam.KDS2019.dFv = m_pSpfcUtil->GetFv();
    m_pParamData->CodeParam.KDS2019.dSds = m_pSpfcUtil->GetSds();
    m_pParamData->CodeParam.KDS2019.dSd1 = m_pSpfcUtil->GetSd1();
    m_pParamData->CodeParam.KDS2019.dIe = m_pSpfcUtil->GetI();
    m_pParamData->CodeParam.KDS2019.dCoef = m_pSpfcUtil->GetR();
    m_pParamData->CodeParam.KDS2019.dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();

    m_pParamData->dResModCoef = m_pSpfcUtil->GetR();

    CString strSite[] = { _T("S1"),_T("S2"),_T("S3"),_T("S4"),_T("S5"),_T("S6") };
    m_pParamData->Description.Format(_LS(IDS_WG_CMD__ADD_KDS2019_PROFILE), m_pSpfcUtil->GetZone() + 1, m_pSpfcUtil->GetZoneFactor(), strSite[m_pSpfcUtil->GetSite()], m_pSpfcUtil->GetFa(), m_pSpfcUtil->GetFv(), m_pSpfcUtil->GetSds(), m_pSpfcUtil->GetSd1(), m_pSpfcUtil->GetI(), m_pSpfcUtil->GetR());

    return TRUE;
}

void CETC_GDS_KDS2019::Data2Dlg()
{
    SetPushOverData(TRUE);
}

void CETC_GDS_KDS2019::SetPushOverData(BOOL bInit/*=TRUE*/)
{
    if(m_pParamData->nSpecCodeType != D_SPFC_CODE_KDS2019) return;

    m_pSpfcUtil->SetZone(m_pParamData->CodeParam.KDS2019.nSeisZone);
    m_pSpfcUtil->SetSite(m_pParamData->CodeParam.KDS2019.nSiteClass);
    m_pSpfcUtil->SetZoneFactor(m_pParamData->CodeParam.KDS2019.dZoneFactor);
    m_pSpfcUtil->SetFa(m_pParamData->CodeParam.KDS2019.dFa);
    m_pSpfcUtil->SetFv(m_pParamData->CodeParam.KDS2019.dFv);
    m_pSpfcUtil->SetSds(m_pParamData->CodeParam.KDS2019.dSds);
    m_pSpfcUtil->SetSd1(m_pParamData->CodeParam.KDS2019.dSd1);
    m_pSpfcUtil->SetI(m_pParamData->CodeParam.KDS2019.dIe);
    m_pSpfcUtil->SetR(m_pParamData->CodeParam.KDS2019.dCoef);
    m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.KDS2019.dMaxPeriod);
    m_pSpfcUtil->SetTs(m_pSpfcUtil->GetSd1() / m_pSpfcUtil->GetSds());
    m_pSpfcUtil->SetT0(0.2 * m_pSpfcUtil->GetTs());

    if (bInit)
    {
        CString strI, strR;
        strI.Format(_T("%g"), m_pSpfcUtil->GetI());
        strR.Format(_T("%g"), m_pSpfcUtil->GetR());
        m_cmbImportance.SetWindowText(strI);
        m_cmbR.SetWindowText(strR);

        // 데이터를 UI 컨트롤에 설정
        m_cmbSeisZone.SetCurSel(m_pSpfcUtil->GetZone());
        CString strTemp;
        strTemp.Format(_T("%g"), m_pSpfcUtil->GetZoneFactor());
        m_cmbSeisZoneFactor.SetWindowText(strTemp);
        m_cmbSiteClass.SetCurSel(m_pSpfcUtil->GetSite());
        m_editFa.SetEditUnit(m_pSpfcUtil->GetFa(), 5);
        m_editFv.SetEditUnit(m_pSpfcUtil->GetFv(), 5);
        m_editSds.SetEditUnit(m_pSpfcUtil->GetSds(), 5);
        m_editSd1.SetEditUnit(m_pSpfcUtil->GetSd1(), 5);
    }

    if (m_bUseTs)
    {
        GetFinalValue();
        m_edtTs.SetEditUnit(m_pSpfcUtil->GetTs(), 5);
    }
}

double CETC_GDS_KDS2019::GetTs() const
{
	return m_pSpfcUtil->GetTs();
}