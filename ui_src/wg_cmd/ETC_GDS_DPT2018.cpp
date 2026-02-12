#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_DPT2018.h"
#include "..\wg_db\LateralSeisDPT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CETC_GDS_DPT2018::CETC_GDS_DPT2018(T_SPFC_D* pParamData, CWnd* pParent)
    : CMyChildDialog(CETC_GDS_DPT2018::IDD, pParent)
{
    CLateralSeisDPT seisDPT;
    m_strFuncName = seisDPT.GetName();
    m_pParamData  = pParamData;
    m_Data.Init();
    InitDataForPushover();
}

void CETC_GDS_DPT2018::InitDataForPushover()
{
    m_Data.dFa   = 1.38;
    m_Data.dFv   = 1.38;
    m_Data.dSds  = 0.506;
    m_Data.dSd1  = 0.2024;
    m_Data.dResModFactor = 4.0;
    m_Data.dImportFactor = 1.2;
}

void CETC_GDS_DPT2018::DoDataExchange(CDataExchange* pDX)
{
    CMyChildDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMD_RDO_REGION_BK, m_rdoRegionBK);
    DDX_Control(pDX, IDC_CMD_RDO_REGION_EX, m_rdoRegionEx);
    DDX_Control(pDX, IDC_CMD_RDO_CALC_GRAP, m_rdoCalcGrap);
    DDX_Control(pDX, IDC_CMD_RDO_CALC_TABL, m_rdoCalcTabl);

    DDX_Control(pDX, IDC_CMD_CMB_SEIS_ZONE, m_cmbSeisZone);

    DDX_Control(pDX, IDC_CMD_CMB_SITE_CLAS, m_cmbSiteClas);
    DDX_Control(pDX, IDC_CMD_CHK_CODE_VALU, m_chkCodeCalc);
    DDX_Control(pDX, IDC_CMD_CMB_SPECT_SSS, m_cmbSpectSss);
    DDX_Control(pDX, IDC_CMD_EDT_SPECT_FAA, m_edtSpectFaa);
    DDX_Control(pDX, IDC_CMD_EDT_SPECT_SDS, m_edtSpectSds);
    DDX_Control(pDX, IDC_CMD_CMB_SPECT_S1S, m_cmbSpectS1s);
    DDX_Control(pDX, IDC_CMD_EDT_SPECT_FVV, m_edtSpectFvv);
    DDX_Control(pDX, IDC_CMD_EDT_SPECT_SD1, m_edtSpectSd1);

    DDX_Control(pDX, IDC_CMD_CMB_RISK_CATE, m_cmbRiskCate);
    DDX_Control(pDX, IDC_CMD_CMB_IMPO_FACT, m_cmbImpoFact);
    DDX_Control(pDX, IDC_CMD_CMB_FACT_MAJR, m_cmbModiFact);
    DDX_Control(pDX, IDC_CMD_CMB_DAMP_RATO, m_cmbDampRato);
}

BEGIN_MESSAGE_MAP(CETC_GDS_DPT2018, CMyChildDialog)
    ON_BN_CLICKED(IDC_CMD_RDO_REGION_BK, OnChangeRegion)
    ON_BN_CLICKED(IDC_CMD_RDO_REGION_EX, OnChangeRegion)
    ON_BN_CLICKED(IDC_CMD_RDO_CALC_GRAP, OnChangeCalcMethod)
    ON_BN_CLICKED(IDC_CMD_RDO_CALC_TABL, OnChangeCalcMethod)
    ON_BN_CLICKED(IDC_CMD_CHK_CODE_VALU, OnChangeCodeCalc)

    ON_CBN_SELCHANGE(IDC_CMD_CMB_SITE_CLAS, OnChangeSeisData)
	ON_CBN_SELCHANGE(IDC_CMD_CMB_SPECT_SSS, OnSelChangeCmdSsCmb)
	ON_CBN_SELCHANGE(IDC_CMD_CMB_SPECT_S1S, OnSelChangeCmdS1Cmb)
	ON_CBN_SELCHANGE(IDC_CMD_CMB_IMPO_FACT, OnSelChangeCmdIFCmb)
	ON_CBN_SELCHANGE(IDC_CMD_CMB_FACT_MAJR, OnSelChangeCmdMFCmb)
	ON_CBN_SELCHANGE(IDC_CMD_CMB_DAMP_RATO, OnSelChangeCmdDampingCmb)
    ON_CBN_EDITCHANGE(IDC_CMD_CMB_SITE_CLAS, OnChangeSeisData)
    ON_CBN_EDITCHANGE(IDC_CMD_CMB_SPECT_SSS, OnChangeSeisData)
    ON_CBN_EDITCHANGE(IDC_CMD_CMB_SPECT_S1S, OnChangeSeisData)
    ON_EN_CHANGE(IDC_CMD_EDT_SPECT_FAA, OnChangeSeisData)
    ON_EN_CHANGE(IDC_CMD_EDT_SPECT_FVV, OnChangeSeisData)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_RISK_CATE, OnChangeRiskCategory)
END_MESSAGE_MAP()

BOOL CETC_GDS_DPT2018::OnInitDialog()
{
    CMyChildDialog::OnInitDialog();
    SetAlign();
    InitCombo();

    InitData();
    Data2Dlg();
    SetCalcMethod(EN_DPT_SEIS_METHOD_TABLE);
    SetCtrlByCalcMethod(false);
    return TRUE;
}

void CETC_GDS_DPT2018::SetAlign()
{
    m_edtSpectFaa.SetAlign(SS_RIGHT);
    m_edtSpectSds.SetAlign(SS_RIGHT);
    m_edtSpectFvv.SetAlign(SS_RIGHT);
    m_edtSpectSd1.SetAlign(SS_RIGHT);
}

void CETC_GDS_DPT2018::InitCombo()
{
    std::vector<std::pair<CString, DWORD>> vSeisZone;
    for ( int i = 0; i < 10; ++i )
    {
        CString csZone = nsGetStr(_T("%d"), i + 1);
        vSeisZone.push_back(std::make_pair(csZone, i));
    }
    CDlgUtil::InitComboBox(m_cmbSeisZone, vSeisZone);

    CLateralSeisDPT seisDPT;

    std::vector<std::pair<CString, DWORD>> vSiteClass;
    seisDPT.GetListSiteClass(vSiteClass);    
    CDlgUtil::InitComboBox(m_cmbSiteClas, vSiteClass);

    std::vector<std::pair<CString, DWORD>> vSpectrumSs;
    seisDPT.GetListSpectrumSs(vSpectrumSs);
    CDlgUtil::InitComboBox(m_cmbSpectSss, vSpectrumSs);

    std::vector<std::pair<CString, DWORD>> vSpectrumS1;
    seisDPT.GetListSpectrumS1(vSpectrumS1);    
    CDlgUtil::InitComboBox(m_cmbSpectS1s, vSpectrumS1);

    std::vector<std::pair<CString, DWORD>> vRiskCategory;
    seisDPT.GetListRiskCategory(vRiskCategory);    
    CDlgUtil::InitComboBox(m_cmbRiskCate, vRiskCategory);

    std::vector<std::pair<CString, DWORD>> vImpoFact;
    seisDPT.GetListImpoFact(vImpoFact);
    CDlgUtil::InitComboBox(m_cmbImpoFact, vImpoFact);

    std::vector<std::pair<CString, DWORD>> vModiFact;
    seisDPT.GetListModiFact(vModiFact);    
    CDlgUtil::InitComboBox(m_cmbModiFact, vModiFact);

    std::vector<std::pair<CString, DWORD>> vDamping;
    seisDPT.GetListDamping(vDamping);    
    CDlgUtil::InitComboBox(m_cmbDampRato, vDamping);
}

void CETC_GDS_DPT2018::SetComboData(MComboBox& rCombo, const double& dValue, int nDecimal)
{
    CString csFormat = nsGetStr(_T("%%.%df"), nDecimal);
    CString csValue  = nsGetStr(csFormat, dValue);
    rCombo.SetWindowText(csValue);
}

double CETC_GDS_DPT2018::GetComboData(const MComboBox& cCombo)
{
    CString csValue;
    cCombo.GetWindowText(csValue);
    return _tstof(csValue);
}

BOOL CETC_GDS_DPT2018::ISVALID()
{
    if ( !GetDlgData() ) { ASSERT(0); return FALSE; }
    return CheckData(m_Data);
}

void CETC_GDS_DPT2018::InitData()
{
    m_Data.Init();
    if ( m_pParamData == nullptr ) { ASSERT(0); return; }

    m_Data = m_pParamData->CodeParam.DPT2018;
    if ( m_Data.dMaxPeriod < 1.0e-7 )
    {
        m_Data.dMaxPeriod = PRD_ED;
    }
}

void CETC_GDS_DPT2018::Data2Dlg()
{
    if ( m_pParamData == nullptr ) { ASSERT(0); return; }

    SetRegion(m_Data.unRegion);
    SetCalcMethod(m_Data.unMethod);

    CDlgUtil::CobxSetCurSelItemData(m_cmbSeisZone, max(0, m_Data.nSeisZone - 1));

    CDlgUtil::CobxSetCurSelItemData(m_cmbSiteClas, m_Data.unSiteClass);
    m_chkCodeCalc.SetCheck(m_Data.bByCode);
    SetComboData(m_cmbSpectSss, m_Data.dSs, 3);
    m_edtSpectFaa.SetEditUnitAuto(m_Data.dFa);
    m_edtSpectSds.SetEditUnitAuto(m_Data.dSds);
    SetComboData(m_cmbSpectS1s, m_Data.dS1, 3);
    m_edtSpectFvv.SetEditUnitAuto(m_Data.dFv);
    m_edtSpectSd1.SetEditUnitAuto(m_Data.dSd1);

    CDlgUtil::CobxSetCurSelItemData(m_cmbRiskCate, m_Data.unOccupancyCategory);
    SetComboData(m_cmbImpoFact, m_Data.dImportFactor);

    SetComboData(m_cmbModiFact, m_Data.dResModFactor);
    SetComboData(m_cmbDampRato, m_Data.dDampingRatio, 3);

    SetCtrlByRegion(m_Data.unRegion, m_Data.bByCode);
    SetCtrlByCalcMethod(false);
}

BOOL CETC_GDS_DPT2018::Dlg2Data()
{
    // if ( !GetDlgData() ) { ASSERT(0); return FALSE; }
    // ISVALID()에서 데이터 가져옴.
    m_pParamData->CodeParam.DPT2018 = m_Data;
    m_pParamData->dResModCoef = m_Data.dResModFactor;
    m_pParamData->Description = GetDescription(m_Data);
    return TRUE;
}

bool CETC_GDS_DPT2018::GetDlgData()
{
    m_Data.unRegion = GetRegion();
    m_Data.unMethod = GetCalcMethod();

    m_Data.nSeisZone = static_cast< int > ( m_cmbSeisZone.GetCurSel() ) + 1;

    m_Data.unSiteClass = static_cast< EN_DPT_SEIS_CLASS >( m_cmbSiteClas.GetCurSel() );
    m_Data.bByCode     = m_chkCodeCalc.GetCheck();
    m_Data.dSs  = GetComboData(m_cmbSpectSss);
    m_Data.dFa  = m_edtSpectFaa.GetEditValue();
    m_Data.dSds = m_edtSpectSds.GetEditValue();
    m_Data.dS1  = GetComboData(m_cmbSpectS1s);
    m_Data.dFv  = m_edtSpectFvv.GetEditValue();
    m_Data.dSd1 = m_edtSpectSd1.GetEditValue();

    m_Data.unOccupancyCategory = static_cast< EN_DPT_SEIS_RISK >( m_cmbRiskCate.GetCurSel() );
    m_Data.dImportFactor = GetComboData(m_cmbImpoFact);

    m_Data.dResModFactor = GetComboData(m_cmbModiFact);
    m_Data.dDampingRatio = GetComboData(m_cmbDampRato);
    return true;
}

void CETC_GDS_DPT2018::SetRegion(UINT nRegion)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_REGION_BK);
    aCtrlID.Add(IDC_CMD_RDO_REGION_EX);
    CDlgUtil::CtrlRadioSetCheck(this, aCtrlID, nRegion);
}

void CETC_GDS_DPT2018::SetCalcMethod(UINT nMethod)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_CALC_GRAP);
    aCtrlID.Add(IDC_CMD_RDO_CALC_TABL);
    CDlgUtil::CtrlRadioSetCheck(this, aCtrlID, nMethod);
}

EN_DPT_SEIS_REGION CETC_GDS_DPT2018::GetRegion()
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_REGION_BK);
    aCtrlID.Add(IDC_CMD_RDO_REGION_EX);

    int nMethod = 0;
    CDlgUtil::CtrlRadioGetCheck(this, aCtrlID, nMethod);

    switch ( nMethod )
    {
    case 0: return EN_DPT_SEIS_REGION_BANGKOK;
    case 1: return EN_DPT_SEIS_REGION_NOT_BANGKOK;
    }
    ASSERT(0); return EN_DPT_SEIS_REGION_BANGKOK;
}

EN_DPT_SEIS_METHOD CETC_GDS_DPT2018::GetCalcMethod()
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_CALC_GRAP);
    aCtrlID.Add(IDC_CMD_RDO_CALC_TABL);

    int nMethod = 0;
    CDlgUtil::CtrlRadioGetCheck(this, aCtrlID, nMethod);

    switch ( nMethod )
    {
    case 0: return EN_DPT_SEIS_METHOD_GRAPH;
    case 1: return EN_DPT_SEIS_METHOD_TABLE;
    }
    ASSERT(0); return EN_DPT_SEIS_METHOD_TABLE;
}

void CETC_GDS_DPT2018::OnChangeRegion()
{
    UINT nRegion = GetRegion();
    bool bByCode = m_chkCodeCalc.GetCheck() ? true : false;
    SetCtrlByRegion(nRegion, bByCode);
}

void CETC_GDS_DPT2018::OnChangeCalcMethod()
{
    UINT nMethod = GetCalcMethod();
    SetCtrlByCalcMethod(nMethod);
}

void CETC_GDS_DPT2018::OnChangeCodeCalc()
{
    UINT nRegion = GetRegion();
    bool bByCode = m_chkCodeCalc.GetCheck() ? true : false;
    SetCtrlByRegion(nRegion, bByCode);
}

void CETC_GDS_DPT2018::OnChangeRiskCategory()
{
    m_Data.unOccupancyCategory = static_cast< EN_DPT_SEIS_RISK >( m_cmbRiskCate.GetCurSel() );

    CLateralSeisDPT seisDPT;
    m_Data.dImportFactor = seisDPT.GetImportanceFactor(m_Data.unOccupancyCategory);
    SetComboData(m_cmbImpoFact, m_Data.dImportFactor);
}

CString GetSelItem(const std::vector<std::pair<CString, DWORD>>& vList, int nSel)
{
	for (auto Cur : vList)
	{
		if (nSel == Cur.second)
		{
			return Cur.first;
		}
	}
	ASSERT(0);
	return _T("");
}

void CETC_GDS_DPT2018::OnSelChangeCmdSsCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...    	
	const int nSel = m_cmbSpectSss.GetCurSel();

	CLateralSeisDPT seisDPT;
	std::vector<std::pair<CString, DWORD>> vListSs;
	seisDPT.GetListSpectrumSs(vListSs);

	const CString& strCurSs = GetSelItem(vListSs, nSel);
	m_cmbSpectSss.SetWindowText(strCurSs);

	OnChangeSeisData();
}

void CETC_GDS_DPT2018::OnSelChangeCmdS1Cmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...    	
	const int nSel = m_cmbSpectS1s.GetCurSel();

	CLateralSeisDPT seisDPT;
	std::vector<std::pair<CString, DWORD>> vListS1;
	seisDPT.GetListSpectrumS1(vListS1);

	const CString& strCurS1 = GetSelItem(vListS1, nSel);
	m_cmbSpectS1s.SetWindowText(strCurS1);

	OnChangeSeisData();
}

void CETC_GDS_DPT2018::OnSelChangeCmdIFCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...    	
	const int nSel = m_cmbImpoFact.GetCurSel();

	CLateralSeisDPT seisDPT;
	std::vector<std::pair<CString, DWORD>> vListIF;
	seisDPT.GetListImpoFact(vListIF);

	const CString& strCurImpo = GetSelItem(vListIF, nSel);
	m_cmbImpoFact.SetWindowText(strCurImpo);

	OnChangeSeisData();
}

void CETC_GDS_DPT2018::OnSelChangeCmdMFCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...    	
	const int nSel = m_cmbModiFact.GetCurSel();

	CLateralSeisDPT seisDPT;
	std::vector<std::pair<CString, DWORD>> vListMF;
	seisDPT.GetListModiFact(vListMF);

	const CString& strCurMF = GetSelItem(vListMF, nSel);
	m_cmbModiFact.SetWindowText(strCurMF);

	OnChangeSeisData();
}

void CETC_GDS_DPT2018::OnSelChangeCmdDampingCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...    	
	const int nSel = m_cmbDampRato.GetCurSel();

	CLateralSeisDPT seisDPT;
	std::vector<std::pair<CString, DWORD>> vListDamp;
	seisDPT.GetListDamping(vListDamp);

	const CString& strCurDamp = GetSelItem(vListDamp, nSel);
	m_cmbDampRato.SetWindowText(strCurDamp);

	OnChangeSeisData();
}

void CETC_GDS_DPT2018::OnChangeSeisData()
{
    T_SPFC_DPT2018 cData;
    cData.unSiteClass = static_cast< EN_DPT_SEIS_CLASS >( m_cmbSiteClas.GetCurSel() );
    cData.bByCode     = m_chkCodeCalc.GetCheck();
    cData.dSs  = GetComboData(m_cmbSpectSss);
    cData.dS1  = GetComboData(m_cmbSpectS1s);
    cData.dDampingRatio = GetComboData(m_cmbDampRato);
    if ( cData.bByCode )
    {
        CLateralSeisDPT seisDPT;
        cData.dFa  = seisDPT.CalcFa(cData.unSiteClass, cData.dSs);
        cData.dFv  = seisDPT.CalcFv(cData.unSiteClass, cData.dS1);
		cData.dSds = seisDPT.CalcSds(cData.dFa, cData.dSs, cData.dDampingRatio);
		cData.dSd1 = seisDPT.CalcSd1(cData.dFv, cData.dS1, cData.dDampingRatio);
    }
    else
    {
        cData.dFa  = m_edtSpectFaa.GetEditValue();
        cData.dFv  = m_edtSpectFvv.GetEditValue();
        cData.dSds = m_edtSpectSds.GetEditValue();
        cData.dSd1 = m_edtSpectSd1.GetEditValue();
    }

    UpdateSeisData(cData);
}

void CETC_GDS_DPT2018::UpdateSeisData(const T_SPFC_DPT2018& cData)
{
    if ( cData.bByCode )
    {
        m_edtSpectFaa.SetEditUnitAuto(cData.dFa);
        m_edtSpectFvv.SetEditUnitAuto(cData.dFv);
        m_edtSpectSds.SetEditUnitAuto(cData.dSds);
        m_edtSpectSd1.SetEditUnitAuto(cData.dSd1);
    }
}

void CETC_GDS_DPT2018::SetCtrlByRegion(UINT unRegion, bool bByCode)
{
    switch ( unRegion )
    {
    case EN_DPT_SEIS_REGION_BANGKOK:
        {
            CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_SEIS_ZONE, TRUE, TRUE);
            CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_SPEC_ACCE, FALSE, TRUE);
        }
        break;
    case EN_DPT_SEIS_REGION_NOT_BANGKOK:
        {
            CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_SEIS_ZONE, FALSE, TRUE);
            CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_SPEC_ACCE, TRUE, TRUE);

            CArray<UINT, UINT> aByCodeCtrl;
            aByCodeCtrl.Add(IDC_CMD_LBL_SPECT_FAA);
            aByCodeCtrl.Add(IDC_CMD_LBL_SPECT_FVV);
            aByCodeCtrl.Add(IDC_CMD_LBL_SPECT_SDS);
            aByCodeCtrl.Add(IDC_CMD_LBL_SPECT_SD1);
            aByCodeCtrl.Add(IDC_CMD_LBL_SPECT_GSS);
            aByCodeCtrl.Add(IDC_CMD_LBL_SPECT_GS1);
            aByCodeCtrl.Add(IDC_CMD_EDT_SPECT_FAA);
            aByCodeCtrl.Add(IDC_CMD_EDT_SPECT_FVV);
            aByCodeCtrl.Add(IDC_CMD_EDT_SPECT_SDS);
            aByCodeCtrl.Add(IDC_CMD_EDT_SPECT_SD1);
            CDlgUtil::CtrlEnableDisable(this, aByCodeCtrl, !bByCode);

            CArray<UINT, UINT> aDisableCtrl;
            aDisableCtrl.Add(IDC_CMD_LBL_SPECT_CUU);
            aDisableCtrl.Add(IDC_CMD_EDT_SPECT_CUU);
            CDlgUtil::CtrlEnableDisable(this, aDisableCtrl, FALSE);
        }
        break;
    default: ASSERT(0); break;
    }
}

void CETC_GDS_DPT2018::SetCtrlByCalcMethod(bool bEnable)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_FRM_METHOD);
    aCtrlID.Add(IDC_CMD_RDO_CALC_GRAP);
    aCtrlID.Add(IDC_CMD_RDO_CALC_TABL);
    CDlgUtil::CtrlEnableDisable(this, aCtrlID, bEnable);
}

bool CETC_GDS_DPT2018::CheckData(const T_SPFC_DPT2018& cData)
{
    const double dZero = 1.0e-7;
    if ( cData.unRegion == EN_DPT_SEIS_REGION_NOT_BANGKOK )
    {
        if ( cData.dFa < dZero )
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa")));
            return FALSE;
        }

        if ( cData.dFv < dZero )
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv")));
            return FALSE;
        }

        if ( cData.dSds < dZero )
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds")));
            return FALSE;
        }

        if ( cData.dSd1 < dZero )
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sd1")));
            return FALSE;
        }
    }

    if ( cData.dImportFactor < dZero )
    {
        AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I")));
        return FALSE;
    }

    if ( cData.dResModFactor < dZero )
    {
        AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R")));
        return FALSE;
    }

    if ( cData.dMaxPeriod < dZero )
    {
        AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));
        return FALSE;
    }

    return TRUE;
}

void CETC_GDS_DPT2018::MakeSpectrumData(BOOL bOnlyCalc)
{
    CLateralSeisDPT seisDPT;
    MArray<FVal> aSpecRS;
    if ( !seisDPT.MakeRSFunction(m_Data, aSpecRS) ) { ASSERT(0); return; }

    int nSpecRS = aSpecRS.GetSize();
    if ( nSpecRS < 1 ) { ASSERT(0); return; }

    m_parPeriod->SetSize(nSpecRS);
    m_parAccel->SetSize(nSpecRS);
    for ( int i = 0; i < nSpecRS; ++i )
    {
        const FVal& SpecRS = aSpecRS[i];
        m_parPeriod->SetAt(i, SpecRS.dVar);
        m_parAccel->SetAt(i, SpecRS.dValue);
    }

    m_sDescript = GetDescription(m_Data);
}

void CETC_GDS_DPT2018::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& radPeriod, CArray <double, double>& radAccel)
{
    radPeriod.RemoveAll();
    radAccel.RemoveAll();
    ASSERT(0);
}

CString CETC_GDS_DPT2018::GetDescription(const T_SPFC_DPT2018& cData)
{
    CLateralSeisDPT seisDPT;
    return seisDPT.GetDescription(cData);
}