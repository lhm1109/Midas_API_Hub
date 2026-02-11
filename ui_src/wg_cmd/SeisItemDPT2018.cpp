#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemDPT2018.h"
#include "SeisItemDptPeriodDlg.h"
#include "..\wg_db\LateralSeisDPT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSeisItemDPT2018::CSeisItemDPT2018(CWnd* pParent)
    : CChildDialog(CSeisItemDPT2018::IDD, pParent)
{
    m_pParent = static_cast < CSeisItemDlgNew* >( pParent );
    m_bInit = FALSE;
    m_Data.Init();
}

void CSeisItemDPT2018::SetData2Dlg(T_SEIS_DPT2018& data, BOOL bModify)
{
    m_Data = data;
    m_bModify = bModify;
}

BOOL CSeisItemDPT2018::SetDlg2Data(T_SEIS_DPT2018& data)
{
    BOOL bOK = Dlg2Data();
    data = m_Data;
    return bOK;
}

void CSeisItemDPT2018::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMD_FRM_SEIS_PARA, m_frmSeisPara);

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
    DDX_Control(pDX, IDC_CMD_EDT_SPECT_CUU, m_edtSpectCuu);

    DDX_Control(pDX, IDC_CMD_CMB_RISK_CATE, m_cmbRiskCate);
    DDX_Control(pDX, IDC_CMD_CMB_IMPO_FACT, m_cmbImpoFact);

    DDX_Control(pDX, IDC_CMD_EDT_DCATE_SDS, m_edtSDCatSds);
    DDX_Control(pDX, IDC_CMD_EDT_DCATE_SD1, m_edtSDCatSd1);
    DDX_Control(pDX, IDC_CMD_EDT_DCATE_FIN, m_edtSDCatFin);

    DDX_Control(pDX, IDC_CMD_RDO_PERI_ANAL, m_rdoPeriAnal);
    DDX_Control(pDX, IDC_CMD_RDO_PERI_APPR, m_rdoPeriAppr);
    DDX_Control(pDX, IDC_CMD_EDT_ANAL_MAJR, m_edtAnalMajr);
    DDX_Control(pDX, IDC_CMD_EDT_ANAL_ORTH, m_edtAnalOrth);
    DDX_Control(pDX, IDC_CMD_EDT_APPR_MAJR, m_edtApprMajr);
    DDX_Control(pDX, IDC_CMD_EDT_APPR_ORTH, m_edtApprOrth);
    DDX_Control(pDX, IDC_CMD_EDT_PERI_MAJR, m_edtPeriMajr);
    DDX_Control(pDX, IDC_CMD_EDT_PERI_ORTH, m_edtPeriOrth);
    DDX_Control(pDX, IDC_CMD_CMB_FACT_MAJR, m_cmbFactMajr);
    DDX_Control(pDX, IDC_CMD_CMB_FACT_ORTH, m_cmbFactOrth);
    DDX_Control(pDX, IDC_CMD_CMB_DAMP_RATO, m_cmbDampRato);
}

BEGIN_MESSAGE_MAP(CSeisItemDPT2018, CChildDialog)
    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
    ON_BN_CLICKED(IDC_CMD_RDO_REGION_BK, OnChangeRegion)
    ON_BN_CLICKED(IDC_CMD_RDO_REGION_EX, OnChangeRegion)
    ON_BN_CLICKED(IDC_CMD_RDO_CALC_GRAP, OnChangeCalcMethod)
    ON_BN_CLICKED(IDC_CMD_RDO_CALC_TABL, OnChangeCalcMethod)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_SEIS_ZONE, OnSelchangeCmdZoneCmb)
    ON_BN_CLICKED(IDC_CMD_CHK_CODE_VALU, OnChangeCodeCalc)
    ON_BN_CLICKED(IDC_CMD_RDO_PERI_ANAL, OnChangePeriodCalcMethod)
    ON_BN_CLICKED(IDC_CMD_RDO_PERI_APPR, OnChangePeriodCalcMethod)
    ON_BN_CLICKED(IDC_CMD_BTN_PERI_APPR, OnBtnApproximatePeriod)

    ON_CBN_SELCHANGE(IDC_CMD_CMB_SITE_CLAS, OnChangeSeisData)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_SPECT_SSS, OnSelChangeCmdSsCmb)
	ON_CBN_SELCHANGE(IDC_CMD_CMB_SPECT_S1S, OnSelChangeCmdS1Cmb)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_IMPO_FACT, OnSelChangeCmdIFCmb)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_FACT_MAJR, OnSelChangeCmdMFMajorCmb)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_FACT_ORTH, OnSelChangeCmdMFOrthoCmb)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_DAMP_RATO, OnSelChangeCmdDampingCmb)
    ON_CBN_EDITCHANGE(IDC_CMD_CMB_SITE_CLAS, OnChangeSeisData)
    ON_CBN_EDITCHANGE(IDC_CMD_CMB_SPECT_SSS, OnChangeSeisData)
    ON_CBN_EDITCHANGE(IDC_CMD_CMB_SPECT_S1S, OnChangeSeisData)
    ON_EN_CHANGE(IDC_CMD_EDT_SPECT_FAA, OnChangeSeisData)
    ON_EN_CHANGE(IDC_CMD_EDT_SPECT_FVV, OnChangeSeisData)
    ON_CBN_SELCHANGE(IDC_CMD_CMB_RISK_CATE, OnChangeRiskCategory)

    ON_EN_CHANGE(IDC_CMD_EDT_ANAL_MAJR, OnChangePeriod)
    ON_EN_CHANGE(IDC_CMD_EDT_ANAL_ORTH, OnChangePeriod)
    ON_EN_CHANGE(IDC_CMD_EDT_APPR_MAJR, OnChangePeriod)
    ON_EN_CHANGE(IDC_CMD_EDT_APPR_ORTH, OnChangePeriod)
END_MESSAGE_MAP()

BOOL CSeisItemDPT2018::DestroyWindow()
{
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
            m_frmSeisPara.SetFoldState(FALSE, FALSE);
		}
	}

    return CChildDialog::DestroyWindow();
}

BOOL CSeisItemDPT2018::OnInitDialog()
{
    CChildDialog::OnInitDialog();
    if ( !m_bInit )
    {
        InitData();
        m_bInit = TRUE;
    }
    if ( !m_bModify )
    {
        m_Data.Init();
    }

    InitFold();
    InitUnit();
    SetAlign();
    InitCombo();
    Data2Dlg();

    SetCalcMethod(EN_DPT_SEIS_METHOD_TABLE);
    SetCtrlByCalcMethod(false);

    SetCtrlPeriod(false);
    return TRUE;
}

void CSeisItemDPT2018::InitData()
{
}

void CSeisItemDPT2018::InitFold()
{
    m_frmSeisPara.InitControl(this, CSeisItemDPT2018::IDD, IDC_CMD_FRM_SEIS_PARA, TRUE, TRUE);
    m_frmSeisPara.SetFoldState(FALSE);
}

void CSeisItemDPT2018::InitUnit()
{
}

void CSeisItemDPT2018::SetAlign()
{
    m_edtSpectFaa.SetAlign(SS_RIGHT);
    m_edtSpectSds.SetAlign(SS_RIGHT);
    m_edtSpectFvv.SetAlign(SS_RIGHT);
    m_edtSpectSd1.SetAlign(SS_RIGHT);
    m_edtSpectCuu.SetAlign(SS_RIGHT);

    m_edtAnalMajr.SetAlign(SS_RIGHT);
    m_edtAnalOrth.SetAlign(SS_RIGHT);
    m_edtApprMajr.SetAlign(SS_RIGHT);
    m_edtApprOrth.SetAlign(SS_RIGHT);
    m_edtPeriMajr.SetAlign(SS_RIGHT);
    m_edtPeriOrth.SetAlign(SS_RIGHT);
}

void CSeisItemDPT2018::InitCombo()
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
    CDlgUtil::InitComboBox(m_cmbFactMajr, vModiFact);
    CDlgUtil::InitComboBox(m_cmbFactOrth, vModiFact);

    std::vector<std::pair<CString, DWORD>> vDamping;
    seisDPT.GetListDamping(vDamping);
    CDlgUtil::InitComboBox(m_cmbDampRato, vDamping);
}

LRESULT CSeisItemDPT2018::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    BOOL bFolded = m_frmSeisPara.GetFoldState();
    if ( !bFolded )
    {
        OnChangeRegion();
        OnChangeCalcMethod();
    }

    CRect rcChild;
    GetWindowRect(rcChild);
    m_pParent->AlignCtrl2Param(rcChild.bottom);
    return 0l;
}

void CSeisItemDPT2018::SetComboData(MComboBox& rCombo, const double& dValue, int nDecimal)
{
    CString csFormat = nsGetStr(_T("%%.%df"), nDecimal);
    CString csValue  = nsGetStr(csFormat, dValue);
    rCombo.SetWindowText(csValue);
}

double CSeisItemDPT2018::GetComboData(const MComboBox& cCombo)
{
    CString csValue;
    cCombo.GetWindowText(csValue);
    return _tstof(csValue);
}

void CSeisItemDPT2018::Data2Dlg()
{
    SetRegion(m_Data.unRegion);
    SetCalcMethod(m_Data.unMethod);

    CDlgUtil::CobxSetCurSelItemData(m_cmbSeisZone, m_Data.nSeisZone);

    CDlgUtil::CobxSetCurSelItemData(m_cmbSiteClas, m_Data.unSiteClass);
    m_chkCodeCalc.SetCheck(m_Data.bByCode);
    SetComboData(m_cmbSpectSss, m_Data.dSs, 3);
    m_edtSpectFaa.SetEditUnitAuto(m_Data.dFa);
    m_edtSpectSds.SetEditUnitAuto(m_Data.dSds);
    SetComboData(m_cmbSpectS1s, m_Data.dS1, 3);
    m_edtSpectFvv.SetEditUnitAuto(m_Data.dFv);
    m_edtSpectSd1.SetEditUnitAuto(m_Data.dSd1);
    double dCu = CalcCu(m_Data.dSd1);
    m_edtSpectCuu.SetEditUnitAuto(dCu);

    CDlgUtil::CobxSetCurSelItemData(m_cmbRiskCate, m_Data.unOccupancyCategory);
    SetComboData(m_cmbImpoFact, m_Data.dImportanceFactor);

    SetSeismicDesignCategory(m_Data);
    SetCtrlByRegion(m_Data.unRegion, m_Data.bByCode);
    SetCtrlByCalcMethod(false);

    const T_SEIS_DPT2018_PERIOD& Major = m_Data.Period[0];
    const T_SEIS_DPT2018_PERIOD& Ortho = m_Data.Period[1];
    SetPeriodCalcMethod(m_Data.unPeriodMethod);
    m_edtAnalMajr.SetEditUnitAuto(Major.dT_anal);
    m_edtApprMajr.SetEditUnitAuto(Major.dT_appr);
    m_edtPeriMajr.SetEditUnitAuto(Major.dT_final);
    m_edtAnalOrth.SetEditUnitAuto(Ortho.dT_anal);
    m_edtApprOrth.SetEditUnitAuto(Ortho.dT_appr);
    m_edtPeriOrth.SetEditUnitAuto(Ortho.dT_final);
    SetCtrlByPeriodCalcMethod(m_Data.unPeriodMethod);

    SetComboData(m_cmbFactMajr, m_Data.dR[0]);
    SetComboData(m_cmbFactOrth, m_Data.dR[1]);
    SetComboData(m_cmbDampRato, m_Data.dDamping, 3);
}

BOOL CSeisItemDPT2018::Dlg2Data()
{
    m_Data.unRegion = GetRegion();
    m_Data.unMethod = GetCalcMethod();

    m_Data.nSeisZone = static_cast< int > ( m_cmbSeisZone.GetCurSel() );

    m_Data.unSiteClass = static_cast< EN_DPT_SEIS_CLASS >( m_cmbSiteClas.GetCurSel() );
    m_Data.bByCode     = m_chkCodeCalc.GetCheck();
    m_Data.dSs  = GetComboData(m_cmbSpectSss);
    m_Data.dFa  = m_edtSpectFaa.GetEditValue();
    m_Data.dSds = m_edtSpectSds.GetEditValue();
    m_Data.dS1  = GetComboData(m_cmbSpectS1s);
    m_Data.dFv  = m_edtSpectFvv.GetEditValue();
    m_Data.dSd1 = m_edtSpectSd1.GetEditValue();

    m_Data.unOccupancyCategory = static_cast< EN_DPT_SEIS_RISK >( m_cmbRiskCate.GetCurSel() );
    m_Data.dImportanceFactor = GetComboData(m_cmbImpoFact);

    T_SEIS_DPT2018_PERIOD& rMajor = m_Data.Period[0];
    T_SEIS_DPT2018_PERIOD& rOrtho = m_Data.Period[1];
    m_Data.unPeriodMethod = GetPeriodCalcMethod();
    rMajor.dT_anal  = m_edtAnalMajr.GetEditValue();
    rMajor.dT_appr  = m_edtApprMajr.GetEditValue();
    rMajor.dT_final = m_edtPeriMajr.GetEditValue();
    rOrtho.dT_anal  = m_edtAnalOrth.GetEditValue();
    rOrtho.dT_appr  = m_edtApprOrth.GetEditValue();
    rOrtho.dT_final = m_edtPeriOrth.GetEditValue();

    m_Data.dR[0]    = GetComboData(m_cmbFactMajr);
    m_Data.dR[1]    = GetComboData(m_cmbFactOrth);
    m_Data.dDamping = GetComboData(m_cmbDampRato);
    return TRUE;
}

void CSeisItemDPT2018::SetRegion(UINT nRegion)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_REGION_BK);
    aCtrlID.Add(IDC_CMD_RDO_REGION_EX);
    CDlgUtil::CtrlRadioSetCheck(this, aCtrlID, nRegion);
}

void CSeisItemDPT2018::SetCalcMethod(UINT nMethod)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_CALC_GRAP);
    aCtrlID.Add(IDC_CMD_RDO_CALC_TABL);
    CDlgUtil::CtrlRadioSetCheck(this, aCtrlID, nMethod);
}

void CSeisItemDPT2018::SetPeriodCalcMethod(UINT nCalcMethod)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_PERI_ANAL);
    aCtrlID.Add(IDC_CMD_RDO_PERI_APPR);
    CDlgUtil::CtrlRadioSetCheck(this, aCtrlID, nCalcMethod);
}

EN_DPT_SEIS_REGION CSeisItemDPT2018::GetRegion()
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

EN_DPT_SEIS_METHOD CSeisItemDPT2018::GetCalcMethod()
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

EN_DPT_SEIS_PERIOD CSeisItemDPT2018::GetPeriodCalcMethod()
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_PERI_ANAL);
    aCtrlID.Add(IDC_CMD_RDO_PERI_APPR);

    int nMethod = 0;
    CDlgUtil::CtrlRadioGetCheck(this, aCtrlID, nMethod);

    switch ( nMethod )
    {
    case 0: return EN_DPT_SEIS_PERIOD_ANAL;
    case 1: return EN_DPT_SEIS_PERIOD_APPR;
    }
    ASSERT(0); return EN_DPT_SEIS_PERIOD_ANAL;
}

void CSeisItemDPT2018::OnChangeRegion()
{
    BOOL bFolded = m_frmSeisPara.GetFoldState();
    if ( !bFolded )
    {
        m_Data.unRegion = GetRegion();
        m_Data.bByCode = m_chkCodeCalc.GetCheck() ? true : false;
        SetCtrlByRegion(m_Data.unRegion, m_Data.bByCode);
        
		m_Data.unMethod = GetCalcMethod();
		SetCtrlByCalcMethod(m_Data.unMethod);
    }

    SetSeismicDesignCategory(m_Data);
}

void CSeisItemDPT2018::OnChangeCalcMethod()
{
    BOOL bFolded = m_frmSeisPara.GetFoldState();
    if ( !bFolded )
    {
        m_Data.unMethod = GetCalcMethod();
        SetCtrlByCalcMethod(m_Data.unMethod);
    }
}

void CSeisItemDPT2018::OnChangeCodeCalc()
{
    BOOL bFolded = m_frmSeisPara.GetFoldState();
    if ( !bFolded )
    {
        m_Data.unRegion = GetRegion();
        m_Data.bByCode = m_chkCodeCalc.GetCheck() ? true : false;
        SetCtrlByRegion(m_Data.unRegion, m_Data.bByCode);
    }
}

void CSeisItemDPT2018::OnChangeSeisData()
{
    if ( !Dlg2Data() ) { ASSERT(0); return; }

    T_SEIS_DPT2018& cData = m_Data;
    if ( cData.bByCode )
    {
        CLateralSeisDPT seisDPT;
        cData.dFa  = seisDPT.CalcFa(cData.unSiteClass, cData.dSs);
        cData.dFv  = seisDPT.CalcFv(cData.unSiteClass, cData.dS1);
        cData.dSds = seisDPT.CalcSds(cData.dFa, cData.dSs, cData.dDamping);
        cData.dSd1 = seisDPT.CalcSd1(cData.dFv, cData.dS1, cData.dDamping);
    }

    UpdateSeisData(cData);
    SetSeismicDesignCategory(cData);
}

void CSeisItemDPT2018::OnChangeRiskCategory()
{
    m_Data.unOccupancyCategory = static_cast< EN_DPT_SEIS_RISK >( m_cmbRiskCate.GetCurSel() );

    CLateralSeisDPT seisDPT;
    m_Data.dImportanceFactor = seisDPT.GetImportanceFactor(m_Data.unOccupancyCategory);
    SetComboData(m_cmbImpoFact, m_Data.dImportanceFactor);
    SetSeismicDesignCategory(m_Data);
}

void CSeisItemDPT2018::UpdateSeisData(const T_SEIS_DPT2018& cData)
{
    if ( cData.bByCode )
    {
        m_edtSpectFaa.SetEditUnitAuto(cData.dFa);
        m_edtSpectFvv.SetEditUnitAuto(cData.dFv);
        m_edtSpectSds.SetEditUnitAuto(cData.dSds);
        m_edtSpectSd1.SetEditUnitAuto(cData.dSd1);
    }
    double dCu = CalcCu(cData.dSd1);
    m_edtSpectCuu.SetEditUnitAuto(dCu);
}

void CSeisItemDPT2018::OnChangePeriodCalcMethod()
{
    m_Data.unPeriodMethod = GetPeriodCalcMethod();
    SetCtrlByPeriodCalcMethod(m_Data.unPeriodMethod);
    UpdateFundamentalPeriod();
    SetSeismicDesignCategory(m_Data);
}

void CSeisItemDPT2018::OnSelchangeCmdZoneCmb()
{
    m_Data.nSeisZone = static_cast< int > ( m_cmbSeisZone.GetCurSel() );
    SetSeismicDesignCategory(m_Data);
}

CString CSeisItemDPT2018::GetSelItem(const std::vector<std::pair<CString, DWORD>>& vList, int nSel)
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

void CSeisItemDPT2018::OnSelChangeCmdSsCmb()
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

void CSeisItemDPT2018::OnSelChangeCmdS1Cmb()
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

void CSeisItemDPT2018::OnSelChangeCmdIFCmb()
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

void CSeisItemDPT2018::OnSelChangeCmdMFMajorCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...    	
	const int nSel = m_cmbFactMajr.GetCurSel();    

	CLateralSeisDPT seisDPT;
	std::vector<std::pair<CString, DWORD>> vListMF;
	seisDPT.GetListModiFact(vListMF);

	const CString& strCurMF = GetSelItem(vListMF, nSel);
    m_cmbFactMajr.SetWindowText(strCurMF);

	OnChangeSeisData();
}

void CSeisItemDPT2018::OnSelChangeCmdMFOrthoCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...	
	const int nSel = m_cmbFactOrth.GetCurSel();

	CLateralSeisDPT seisDPT;
	std::vector<std::pair<CString, DWORD>> vListMF;
	seisDPT.GetListModiFact(vListMF);

	const CString& strCurMF = GetSelItem(vListMF, nSel);
    m_cmbFactOrth.SetWindowText(strCurMF);

	OnChangeSeisData();
}

void CSeisItemDPT2018::OnSelChangeCmdDampingCmb()
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

void CSeisItemDPT2018::OnBtnApproximatePeriod()
{
    double dH = GetBuildingHeight();

    CSeisItemDPTPeriodDlg Dialog;
    Dialog.SetData(dH);
    if ( Dialog.DoModal() == IDOK )
    {
        double dMajorT = 0.0, dOrthoT = 0.0;
        if ( Dialog.GetPeriod(dMajorT, dOrthoT) )
        {
            m_edtApprMajr.SetEditUnitAuto(dMajorT);
            m_edtApprOrth.SetEditUnitAuto(dOrthoT);
            UpdateFundamentalPeriod();
            SetSeismicDesignCategory(m_Data);
        }
    }
}

void CSeisItemDPT2018::OnChangePeriod()
{
    UpdateFundamentalPeriod();
    SetSeismicDesignCategory(m_Data);
}

void CSeisItemDPT2018::UpdateFundamentalPeriod()
{
    double dApprMajorT = m_edtApprMajr.GetEditValue();
    double dApprOrthoT = m_edtApprOrth.GetEditValue();

    double dMajorT = 0.0;
    double dOrthoT = 0.0;
    switch ( GetPeriodCalcMethod() )
    {
    case EN_DPT_SEIS_PERIOD_ANAL:
        {
            double dCu = CalcCu(m_Data.dSd1);
            double dAnalMajorT = m_edtAnalMajr.GetEditValue();
            double dAnalOrthoT = m_edtAnalOrth.GetEditValue();
            dMajorT = min(dAnalMajorT, dCu * dApprMajorT);
            dOrthoT = min(dAnalOrthoT, dCu * dApprOrthoT);
        }
        break;
    case EN_DPT_SEIS_PERIOD_APPR:
        {
            dMajorT = dApprMajorT;
            dOrthoT = dApprOrthoT;
        }
        break;
    default: ASSERT(0); break;
    }

    m_edtPeriMajr.SetEditUnitAuto(dMajorT);
    m_edtPeriOrth.SetEditUnitAuto(dOrthoT);

    T_SEIS_DPT2018_PERIOD& rMajorT = m_Data.Period[0];
    rMajorT.dT_appr  = m_edtApprMajr.GetEditValue();
    rMajorT.dT_anal  = m_edtAnalMajr.GetEditValue();
    rMajorT.dT_final = m_edtPeriMajr.GetEditValue();

    T_SEIS_DPT2018_PERIOD& rOrthoT = m_Data.Period[1];
    rOrthoT.dT_appr  = m_edtApprOrth.GetEditValue();
    rOrthoT.dT_anal  = m_edtAnalOrth.GetEditValue();
    rOrthoT.dT_final = m_edtPeriOrth.GetEditValue();
}

void CSeisItemDPT2018::SetCtrlByRegion(UINT unRegion, bool bByCode)
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

void CSeisItemDPT2018::SetCtrlByCalcMethod(bool bEnable)
{
//     CArray<UINT, UINT> aCtrlID;
//     aCtrlID.Add(IDC_CMD_FRM_METHOD);
//     aCtrlID.Add(IDC_CMD_RDO_CALC_GRAP);
//     aCtrlID.Add(IDC_CMD_RDO_CALC_TABL);
//     CDlgUtil::CtrlEnableDisable(this, aCtrlID, bEnable);

    GetDlgItem(IDC_CMD_RDO_CALC_GRAP)->EnableWindow(FALSE);
    GetDlgItem(IDC_CMD_RDO_CALC_TABL)->EnableWindow(m_Data.unRegion == EN_DPT_SEIS_REGION_BANGKOK);
}

void CSeisItemDPT2018::SetCtrlPeriod(bool bEnable)
{
    m_edtPeriMajr.EnableWindow(bEnable);
    m_edtPeriOrth.EnableWindow(bEnable);
}

void CSeisItemDPT2018::SetCtrlByPeriodCalcMethod(UINT nMethod)
{
    CArray<UINT, UINT> aAnalCtrl;
    aAnalCtrl.Add(IDC_CMD_EDT_ANAL_MAJR);
    aAnalCtrl.Add(IDC_CMD_EDT_ANAL_ORTH);

    CArray<UINT, UINT> aApprCtrl;
    aApprCtrl.Add(IDC_CMD_EDT_APPR_MAJR);
    aApprCtrl.Add(IDC_CMD_EDT_APPR_ORTH);
    aApprCtrl.Add(IDC_CMD_BTN_PERI_APPR);

    switch ( nMethod )
    {
    case EN_DPT_SEIS_PERIOD_ANAL:
        {
            CDlgUtil::CtrlEnableDisable(this, aAnalCtrl, TRUE);
            CDlgUtil::CtrlEnableDisable(this, aApprCtrl, TRUE);
        }
        break;
    case EN_DPT_SEIS_PERIOD_APPR:
        {
            CDlgUtil::CtrlEnableDisable(this, aAnalCtrl, FALSE);
            CDlgUtil::CtrlEnableDisable(this, aApprCtrl, TRUE);
        }
        break;
    default: ASSERT(0); break;
    }
}

double CSeisItemDPT2018::GetBuildingHeight()
{
    CDBDoc* pDoc = ( CDBDoc* ) CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return 0.0; }

    CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl; if ( pAttrCtrl == nullptr ) { ASSERT(0); return 0.0; }
    CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl; if ( pUnitCtrl == nullptr ) { ASSERT(0); return 0.0; }

    // 주의 !
    // Seismic Load 에서는 m 단위로 변환하여 받아온다.
    T_UNIT_INDEX CurIndex;
    pUnitCtrl->GetUnitIndexCurrent(CurIndex);

    T_UNIT_INDEX CodeIndex = CurIndex;
    CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
    pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

    CArray<T_STOR_K, T_STOR_K> aStorK;
    pAttrCtrl->GetStorKeyList(aStorK);
    int nStorNum = aStorK.GetSize();

    T_STOR_D StorD;
    StorD.Initialize();
    if ( nStorNum > 0 )
    {
        pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
    }

    double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
    double dHn = StorD.dStoryLevel - dBaseLevel;
    if ( !pAttrCtrl->GetCountStor() )
    {
        dHn = 0;
    }

    // <Remember> Data 받고나서는 User Unit 기준
    pUnitCtrl->SetUnitIndexCurrent(CurIndex);
    return dHn;
}

double CSeisItemDPT2018::CalcCu(const double& dSd1)
{
    CLateralSeisDPT seisDPT;
    return seisDPT.CalcCu(dSd1);
}

void CSeisItemDPT2018::SetSeismicDesignCategory(const T_SEIS_DPT2018& cData)
{
    CString csSds = _T("A");
    CString csSd1 = _T("A");
    CString csFin = _T("A");

    CLateralSeisDPT seisDPT;
    bool bGetSDC = seisDPT.GetSeisDesignCategory(cData, csSds, csSd1, csFin, true);
    ASSERT(bGetSDC);

    auto L_SetEditText = [ ] (CEdit& rEdit, const CString& csText)
    {
        rEdit.SetWindowText(csText);
    };
    L_SetEditText(m_edtSDCatSds, csSds);
    L_SetEditText(m_edtSDCatSd1, csSd1);
    L_SetEditText(m_edtSDCatFin, csFin);
}