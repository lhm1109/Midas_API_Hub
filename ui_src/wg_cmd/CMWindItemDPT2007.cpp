#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindItemDPT2007.h"
#include "CMWindItemDPTinfoDlg.h"
#include "CMWindItemDPTLcomDlg.h"
#include "..\wg_db\LateralWindDPT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMWindItemDPT2007::CCMWindItemDPT2007(CWnd* pParent)
    : CChildDialog(CCMWindItemDPT2007::IDD, pParent)
{
    m_pParent = static_cast < CCMWindItemDlg* >( pParent );
    m_bModify = false;
    m_KeyWAorWT = NULL;

    m_bInit = false;
    m_Data.Init();
    m_WnatD.Initialize();

    m_aCtrlExRadio.Add(IDC_CMD_RDO_XDIR_POS);
    m_aCtrlExRadio.Add(IDC_CMD_RDO_XDIR_NEG);
    m_aCtrlExRadio.Add(IDC_CMD_RDO_XDIR_NONE);

    m_aCtrlEyRadio.Add(IDC_CMD_RDO_YDIR_POS);
    m_aCtrlEyRadio.Add(IDC_CMD_RDO_YDIR_NEG);
    m_aCtrlEyRadio.Add(IDC_CMD_RDO_YDIR_NONE);
}

void CCMWindItemDPT2007::SetData2Dlg(T_WIND_DPT2007& data, BOOL bModify, T_KEY KeyWAorWT)
{
    m_Data      = data;
    m_bModify   = bModify;
    m_KeyWAorWT = KeyWAorWT;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( !pDoc->m_pAttrCtrl2->GetWnat(m_Data.keyWnat, m_WnatD) )
    {
        m_WnatD.Initialize();
    }
}

BOOL CCMWindItemDPT2007::SetDlg2Data(T_WIND_DPT2007& data)
{
    BOOL bOK = Dlg2Data();
    data = m_Data;
    return bOK;
}

void CCMWindItemDPT2007::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMD_FRM_WIND_PARA, m_frmWindPara);

    DDX_Control(pDX, IDC_CMD_CMB_WIND_ZONE, m_cmbWindZone);
    DDX_Control(pDX, IDC_CMD_EDT_WIND_SPED, m_edtWindVelo);
    DDX_Control(pDX, IDC_CMD_CMB_TRRN_CATE, m_cmbTrrnCate);
    DDX_Control(pDX, IDC_CMD_CMB_IMPO_FACT, m_cmbImpoFact);

    DDX_Control(pDX, IDC_CMD_CHK_TOPO_EFFT, m_chkTopoEfft);
    DDX_Control(pDX, IDC_CMD_CMB_HILL_SHAP, m_cmbHillShap);
    DDX_Control(pDX, IDC_CMD_CMB_BLDG_LOCA, m_cmbBldgLoca);
    DDX_Control(pDX, IDC_CMD_EDT_HILL_HGHT, m_edtHillHght);
    DDX_Control(pDX, IDC_CMD_EDT_HILL_LENG, m_edtHillLeng);
    DDX_Control(pDX, IDC_CMD_EDT_BLDG_DIST, m_edtBldgDist);
    DDX_Control(pDX, IDC_CMD_UNT_HILL_HGHT, m_untHillHght);
    DDX_Control(pDX, IDC_CMD_UNT_HILL_LENG, m_untHillLeng);
    DDX_Control(pDX, IDC_CMD_UNT_BLDG_DIST, m_untBldgDist);

    DDX_Control(pDX, IDC_CMD_EDT_GUST_MAJR, m_edtGustMajr);
    DDX_Control(pDX, IDC_CMD_EDT_GUST_ORTH, m_edtGustOrth);

    DDX_Control(pDX, IDC_CMD_CHK_WIND_ACROS, m_chkWindCros);
    DDX_Control(pDX, IDC_CMD_CHK_WIND_TORS, m_chkWindTors);
    DDX_Control(pDX, IDC_CMD_CHK_WIND_RESP, m_chkWindResp);
}

BEGIN_MESSAGE_MAP(CCMWindItemDPT2007, CChildDialog)
    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
    ON_BN_CLICKED(IDC_CMD_RDO_MTHD_SIMP, OnChangeMethod)
    ON_BN_CLICKED(IDC_CMD_RDO_MTHD_GNRL, OnChangeMethod)
    ON_BN_CLICKED(IDC_CMD_CHK_TOPO_EFFT, OnChangeTopographicEffect)
    ON_BN_CLICKED(IDC_CMD_BTN_GUST_FACT, OnButtonGustFactor)
    ON_BN_CLICKED(IDC_CMD_BTN_WIND_VIBR, OnButtonWindVibration)
END_MESSAGE_MAP()

BOOL CCMWindItemDPT2007::DestroyWindow()
{
    Dlg2Data();

	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
            m_frmWindPara.SetFoldState(FALSE, FALSE);
		}
	}

    return CChildDialog::DestroyWindow();
}

BOOL CCMWindItemDPT2007::OnInitDialog()
{
    CChildDialog::OnInitDialog();
    if ( !m_bInit )
    {
        InitDefaultData();
        m_bInit = true;
    }

    AlignCtrl();
    InitFold();
    InitUnit();
    SetAlign();
    InitCombo(m_Data.unMethod);

    Data2Dlg();
    CDlgUtil::CtrlEnableDisable(this, IDC_CMD_CHK_WIND_RESP, FALSE);

	if (m_KeyWAorWT != NULL)
	{
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RDO_MTHD_SIMP, FALSE);
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RDO_MTHD_GNRL, FALSE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_COMM_PARA, FALSE, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_TOPO_EFFT, FALSE, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_GUST_FACT, FALSE, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_ADDI_PARA, FALSE, TRUE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRM_WECC, FALSE, TRUE);
	}
    return TRUE;
}

void CCMWindItemDPT2007::InitFold()
{
    m_frmWindPara.InitControl(this, CCMWindItemDPT2007::IDD, IDC_CMD_FRM_WIND_PARA, TRUE, TRUE);
    m_frmWindPara.SetFoldState(FALSE);
}

void CCMWindItemDPT2007::InitUnit()
{
    m_edtHillHght.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dHillHeight);
    m_edtHillLeng.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dHillLength);
    m_edtBldgDist.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dHillDistance);
    m_untHillHght.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dHillHeight);
    m_untHillLeng.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dHillLength);
    m_untBldgDist.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dHillDistance);
}

void CCMWindItemDPT2007::SetAlign()
{
    m_edtWindVelo.SetAlign(SS_RIGHT);

    m_edtHillHght.SetAlign(SS_RIGHT);
    m_edtHillLeng.SetAlign(SS_RIGHT);
    m_edtBldgDist.SetAlign(SS_RIGHT);

    m_edtGustMajr.SetAlign(SS_RIGHT);
    m_edtGustOrth.SetAlign(SS_RIGHT);
}

void CCMWindItemDPT2007::InitCombo(UINT nMethod)
{
    std::vector<std::pair<CString, DWORD>> vWindZone;
    vWindZone.push_back(std::make_pair(_T("Zone 1"), EN_DPT_WIND_ZONE1));
    vWindZone.push_back(std::make_pair(_T("Zone 2"), EN_DPT_WIND_ZONE2));
    vWindZone.push_back(std::make_pair(_T("Zone 3"), EN_DPT_WIND_ZONE3));
    vWindZone.push_back(std::make_pair(_T("Zone 4A"), EN_DPT_WIND_ZONE4A));
    vWindZone.push_back(std::make_pair(_T("Zone 4B"), EN_DPT_WIND_ZONE4B));
    CDlgUtil::InitComboBox(m_cmbWindZone, vWindZone);

    InitComboTerrain(nMethod);

    UINT nImpoFact = 0;
    std::vector<std::pair<CString, DWORD>> vImpoFact;
    vImpoFact.push_back(std::make_pair(_T("0.80"), nImpoFact++));
    vImpoFact.push_back(std::make_pair(_T("1.00"), nImpoFact++));
    vImpoFact.push_back(std::make_pair(_T("1.15"), nImpoFact++));
    CDlgUtil::InitComboBox(m_cmbImpoFact, vImpoFact);

    std::vector<std::pair<CString, DWORD>> vHillShape;
    vHillShape.push_back(std::make_pair(_T("2D Ridge or Valley"), EN_DPT_WIND_HLSP_2D_RIDGEVALLY));
    vHillShape.push_back(std::make_pair(_T("2D Escarpment"), EN_DPT_WIND_HLSP_2D_ESCARPMENT));
    vHillShape.push_back(std::make_pair(_T("3D Axisym. Hill"), EN_DPT_WIND_HLSP_3D_AXISYMHILL));
    CDlgUtil::InitComboBox(m_cmbHillShap, vHillShape);

    std::vector<std::pair<CString, DWORD>> vBuilding;
    vBuilding.push_back(std::make_pair(_T("Upwind"), EN_DPT_WIND_BDLC_UPWIND));
    vBuilding.push_back(std::make_pair(_T("Downwind"), EN_DPT_WIND_BDLC_DNWIND));
    CDlgUtil::InitComboBox(m_cmbBldgLoca, vBuilding);
}

void CCMWindItemDPT2007::InitComboTerrain(UINT nMethod)
{
    std::vector<std::pair<CString, DWORD>> vTerrain;
    vTerrain.push_back(std::make_pair(_T("A"), EN_DPT_WIND_EXPO_A));
    vTerrain.push_back(std::make_pair(_T("B"), EN_DPT_WIND_EXPO_B));
    if ( nMethod == EN_DPT_WIND_METHOD_DETAIL )
    {
        vTerrain.push_back(std::make_pair(_T("C"), EN_DPT_WIND_EXPO_C));
    }
    CDlgUtil::InitComboBox(m_cmbTrrnCate, vTerrain);
}

LRESULT CCMWindItemDPT2007::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    BOOL bFolded = m_frmWindPara.GetFoldState();
    if ( !bFolded )
    {
        OnChangeMethod();
    }

    CRect rcChild;
    GetWindowRect(rcChild);
    m_pParent->AlignCtrl2Param(rcChild.bottom);
    return 0l;
}

void CCMWindItemDPT2007::OnChangeMethod()
{
    UINT nMethod = GetCalcMethod();

    BOOL bFolded = m_frmWindPara.GetFoldState();
    if ( !bFolded )
    {
        SetCtrlByCalcMethod(nMethod);
    }

    UpdateGridData(nMethod);
}

void CCMWindItemDPT2007::OnChangeTopographicEffect()
{
    m_Data.bTopographicEffect = m_chkTopoEfft.GetCheck();
    SetEnableTopographic(m_Data.bTopographicEffect);
}

void CCMWindItemDPT2007::OnButtonGustFactor()
{
    Dlg2Data();

    T_WIND_DPT2007 CopyD;
    m_Data.Copy(CopyD);

    CCMWindItemDPTinfoDlg Dialog;
    Dialog.SetData(&CopyD);
    if ( Dialog.DoModal() == IDOK )
    {
        CLateralLoad LateralLoad;
        double dZ = LateralLoad.GetBldgHeightAboveGroundLevel();

        CLateralWindDPT windDPT(CDBDoc::GetDocPoint());
        double dGx = windDPT.CalcGustFactor(CopyD, dZ, true);
        double dGy = windDPT.CalcGustFactor(CopyD, dZ, false);
        m_edtGustMajr.SetEditUnitAuto(dGx);
        m_edtGustOrth.SetEditUnitAuto(dGy);

        m_Data.unOpenStructStiffType = CopyD.unOpenStructStiffType;
        m_Data.dBreadth[0] = CopyD.dBreadth[0];
        m_Data.dBreadth[1] = CopyD.dBreadth[1];

        m_Data.dFrequency[0] = CopyD.dFrequency[0];
        m_Data.dFrequency[1] = CopyD.dFrequency[1];
        m_Data.dDamping[0]   = CopyD.dDamping[0];
        m_Data.dDamping[1]   = CopyD.dDamping[1];
    }
}

void CCMWindItemDPT2007::OnButtonWindVibration()
{
    T_WIND_DPT2007_VIBR CopyD;
    m_Data.Vibr.Copy(CopyD);
    CopyD.bAcross = m_chkWindCros.GetCheck();
    CopyD.bTorsion = m_chkWindTors.GetCheck();
    CopyD.bResponse = m_chkWindResp.GetCheck();

    CCMWindItemDPTLcomDlg Dialog;
    Dialog.SetData(m_Data.unMethod, &CopyD);
    if ( Dialog.DoModal() == IDOK )
    {
        CopyD.Copy(m_Data.Vibr);
    }
}

void CCMWindItemDPT2007::UpdateGridData(UINT nMethod)
{
    if ( nMethod == EN_DPT_WIND_METHOD_SIMPLE ) return;

    bool bAlong   = true;
    bool bAcross = m_chkWindCros.GetCheck() ? true : false;
    bool bTorsion = m_chkWindTors.GetCheck() ? true : false;
    m_pParent->UpdateGrid(bAlong, bAcross, bTorsion);
}

void CCMWindItemDPT2007::InitDefaultData()
{
    T_CMD_WIND_STORY_D StoryD;
    GetStoryData(StoryD);

    double dMassX  = 0.0;
    double dMassY  = 0.0;
    double dMassXY = 0.0;
    double dTotal  = 0.0;
    GetStoryMass(dMassX, dMassY, dMassXY, dTotal);

    // m_Data.Init();
    m_Data.dBreadth[0] = StoryD.dLx;
    m_Data.dBreadth[1] = StoryD.dLy;

    T_WIND_DPT2007_VIBR& rVibr = m_Data.Vibr;
    rVibr.dParam[EN_DPT_WIND_VIBR_MASS_X] = dMassX;
    rVibr.dParam[EN_DPT_WIND_VIBR_MASS_Y] = dMassY;
    rVibr.dParam[EN_DPT_WIND_VIBR_MASS_T] = dMassXY;
}

void CCMWindItemDPT2007::GetStoryData(T_CMD_WIND_STORY_D& rData)
{
    rData.Init();
    if ( m_pParent )
    {
        m_pParent->GetStoryData(rData);
    }
}

void CCMWindItemDPT2007::GetStoryMass(double& rdMassX, double& rdMassY, double& rdMassXY, double& rdTotal)
{
    rdMassX  = 0.0;
    rdMassY  = 0.0;
    rdMassXY = 0.0;
    rdTotal  = 0.0;
    if ( m_pParent )
    {
        m_pParent->GetStoryMass(rdMassX, rdMassY, rdMassXY, rdTotal);
    }
}

void CCMWindItemDPT2007::AlignCtrl()
{
    CWnd* pWndLast = GetDlgItem(IDC_CMD_FRM_WECC);
    if ( pWndLast == nullptr ) { ASSERT(0); return; }

    CRect rcLast, rcDlg;
    GetWindowRect(rcDlg);
    pWndLast->GetWindowRect(rcLast);
    rcDlg.bottom = rcLast.bottom + 4;
    MoveWindow(rcDlg);
}

void CCMWindItemDPT2007::Data2Dlg()
{
    SetCalcMode(m_Data.unMethod);
    SetCtrlByCalcMethod(m_Data.unMethod);

    CDlgUtil::CobxSetCurSelItemData(m_cmbWindZone, m_Data.unWindZone);
    m_edtWindVelo.SetEditUnitAuto(m_Data.dBasicWindSpeed);
    CDlgUtil::CobxSetCurSelItemData(m_cmbTrrnCate, m_Data.unExpoCategory);
    m_cmbImpoFact.SelectString(-1, nsGetStr(_T("%g"), m_Data.dImportanceFactor));

    m_chkTopoEfft.SetCheck(m_Data.bTopographicEffect);
    CDlgUtil::CobxSetCurSelItemData(m_cmbHillShap, m_Data.unHillShape);
    CDlgUtil::CobxSetCurSelItemData(m_cmbBldgLoca, m_Data.unBldgLocation);
    m_edtHillHght.SetEditUnitAuto(m_Data.dHillHeight);
    m_edtHillLeng.SetEditUnitAuto(m_Data.dHillLength);
    m_edtBldgDist.SetEditUnitAuto(m_Data.dHillDistance);
    SetEnableTopographic(m_Data.bTopographicEffect);

    m_edtGustMajr.SetEditUnitAuto(m_Data.dGf[0]);
    m_edtGustOrth.SetEditUnitAuto(m_Data.dGf[1]);

    const T_WIND_DPT2007_VIBR& Vibr = m_Data.Vibr;
	m_chkWindCros.SetCheck(Vibr.bAcross);
    m_chkWindTors.SetCheck(Vibr.bTorsion);
    m_chkWindResp.SetCheck(BST_UNCHECKED/*Vibr.bResponse*/);

    CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
    CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);

}

BOOL CCMWindItemDPT2007::Dlg2Data()
{
    auto L_GetComboData = [ this ] (const CComboBox& cCombo)-> double
    {
        CString csValue;
        cCombo.GetWindowText(csValue);
        return _tstof(csValue);
    };

    m_Data.unMethod = GetCalcMethod();

    m_Data.unWindZone        = static_cast< EN_DPT_WIND_ZONE >( m_cmbWindZone.GetCurSel() );
    m_Data.dBasicWindSpeed   = m_edtWindVelo.GetEditValue();
    m_Data.unExpoCategory    = static_cast< EN_DPT_WIND_EXPO >( m_cmbTrrnCate.GetCurSel() );
    m_Data.dImportanceFactor = L_GetComboData(m_cmbImpoFact);

    m_Data.bTopographicEffect = m_chkTopoEfft.GetCheck();
    m_Data.unHillShape        = static_cast< EN_DPT_WIND_HLSP >( m_cmbHillShap.GetCurSel() );
    m_Data.unBldgLocation     = static_cast< EN_DPT_WIND_BDLC >( m_cmbBldgLoca.GetCurSel() );
    m_Data.dHillHeight        = m_edtHillHght.GetEditValue();
    m_Data.dHillLength        = m_edtHillLeng.GetEditValue();
    m_Data.dHillDistance      = m_edtBldgDist.GetEditValue();

    m_Data.dGf[0] = m_edtGustMajr.GetEditValue();
    m_Data.dGf[1] = m_edtGustOrth.GetEditValue();

    T_WIND_DPT2007_VIBR& rVibr = m_Data.Vibr;
    rVibr.bAcross   = m_chkWindCros.GetCheck();
    rVibr.bTorsion  = m_chkWindTors.GetCheck();
    rVibr.bResponse = m_chkWindResp.GetCheck();

    CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
    CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);
    return TRUE;
}

EN_DPT_WIND_METHOD CCMWindItemDPT2007::GetCalcMethod()
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_MTHD_SIMP);
    aCtrlID.Add(IDC_CMD_RDO_MTHD_GNRL);

    int nMethod = 0;
    CDlgUtil::CtrlRadioGetCheck(this, aCtrlID, nMethod);

    switch ( nMethod )
    {
    case 0: return EN_DPT_WIND_METHOD_SIMPLE;
    case 1: return EN_DPT_WIND_METHOD_DETAIL;
    }
    ASSERT(0); return EN_DPT_WIND_METHOD_DETAIL;
}

void CCMWindItemDPT2007::SetCalcMode(UINT nMethod)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_RDO_MTHD_SIMP);
    aCtrlID.Add(IDC_CMD_RDO_MTHD_GNRL);
    CDlgUtil::CtrlRadioSetCheck(this, aCtrlID, nMethod);
}

void CCMWindItemDPT2007::SetCtrlByCalcMethod(UINT nMethod)
{
    const int nSelTerr = m_cmbTrrnCate.GetCurSel();

    InitComboTerrain(nMethod);
    SetEnableGustFactor(nMethod);
    SetEnableAdditional_Ecc(nMethod);

    const int nModTerr = m_cmbTrrnCate.GetCount() > nSelTerr ? nSelTerr : 0;
    m_cmbTrrnCate.SetCurSel(nModTerr);
}

void CCMWindItemDPT2007::SetEnableTopographic(BOOL bTopographic)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_LBL_HILL_SHAP);
    aCtrlID.Add(IDC_CMD_LBL_BLDG_LOCA);
    aCtrlID.Add(IDC_CMD_LBL_HILL_HGHT);
    aCtrlID.Add(IDC_CMD_LBL_HILL_LENG);
    aCtrlID.Add(IDC_CMD_LBL_BLDG_DIST);
    aCtrlID.Add(IDC_CMD_CMB_HILL_SHAP);
    aCtrlID.Add(IDC_CMD_CMB_BLDG_LOCA);
    aCtrlID.Add(IDC_CMD_EDT_HILL_HGHT);
    aCtrlID.Add(IDC_CMD_EDT_HILL_LENG);
    aCtrlID.Add(IDC_CMD_EDT_BLDG_DIST);
    aCtrlID.Add(IDC_CMD_UNT_HILL_HGHT);
    aCtrlID.Add(IDC_CMD_UNT_HILL_LENG);
    aCtrlID.Add(IDC_CMD_UNT_BLDG_DIST);
    CDlgUtil::CtrlEnableDisable(this, aCtrlID, bTopographic);
}

void CCMWindItemDPT2007::SetEnableAdditional_Ecc(UINT nMethod)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_CHK_WIND_ACROS);
    aCtrlID.Add(IDC_CMD_CHK_WIND_TORS);
    aCtrlID.Add(IDC_CMD_CHK_WIND_RESP);
    aCtrlID.Add(IDC_CMD_BTN_WIND_VIBR);

    BOOL bEnableAdd = TRUE;
    BOOL bEnableEcc = FALSE;
    if (nMethod == EN_DPT_WIND_METHOD_SIMPLE)
    {
        bEnableAdd = FALSE;
        bEnableEcc = TRUE;
    }
    CDlgUtil::CtrlEnableDisable(this, aCtrlID, bEnableAdd);

    CDlgUtil::CtrlEnableDisable(this, m_aCtrlExRadio, bEnableEcc);
    CDlgUtil::CtrlEnableDisable(this, m_aCtrlEyRadio, bEnableEcc);
}

void CCMWindItemDPT2007::SetEnableGustFactor(UINT nMethod)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_BTN_GUST_FACT);

    BOOL bEnable = TRUE;
    if ( nMethod == EN_DPT_WIND_METHOD_SIMPLE )
    {
        bEnable = FALSE;
    }
    CDlgUtil::CtrlEnableDisable(this, aCtrlID, bEnable);
}