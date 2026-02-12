#include "stdafx.h"
#include <math.h>
#include "wg_cmd.h"
#include "CMWindItemDPTLcomDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMWindItemDPTLcomDlg::CCMWindItemDPTLcomDlg(CWnd* pParent)
    : CDialogMove(CCMWindItemDPTLcomDlg::IDD, pParent)
{
    m_pData = nullptr;
    m_enCalcMethod = EN_DPT_WIND_METHOD_SIMPLE;
}

void CCMWindItemDPTLcomDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMD_EDT_FREQ_ALNG, m_edtFreqAlng);
    DDX_Control(pDX, IDC_CMD_EDT_FREQ_CROS, m_edtFreqCros);
    DDX_Control(pDX, IDC_CMD_EDT_FREQ_TORS, m_edtFreqTors);

    DDX_Control(pDX, IDC_CMD_EDT_MASS_ALNG, m_edtMassAlng);
    DDX_Control(pDX, IDC_CMD_EDT_MASS_CROS, m_edtMassCros);
    DDX_Control(pDX, IDC_CMD_EDT_MASS_TORS, m_edtMassTors);
    DDX_Control(pDX, IDC_CMD_UNT_MASS_ALNG, m_untMassAlng);
    DDX_Control(pDX, IDC_CMD_UNT_MASS_CROS, m_untMassCros);
    DDX_Control(pDX, IDC_CMD_UNT_MASS_TORS, m_untMassTors);

    DDX_Control(pDX, IDC_CMD_EDT_DAMP_ALNG, m_edtDampAlng);
    DDX_Control(pDX, IDC_CMD_EDT_DAMP_CROS, m_edtDampCros);
    DDX_Control(pDX, IDC_CMD_EDT_DAMP_TORS, m_edtDampTors);
}

BEGIN_MESSAGE_MAP(CCMWindItemDPTLcomDlg, CDialogMove)
    ON_BN_CLICKED(IDC_CMD_BTN_OK, OnButtonOK)
    ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnButtonCancel)
END_MESSAGE_MAP()

BOOL CCMWindItemDPTLcomDlg::OnInitDialog()
{
    CDialogMove::OnInitDialog();
    InitUnit();
    SetAlign();
    Data2Dlg();
    return TRUE;
}

void CCMWindItemDPTLcomDlg::InitUnit()
{
    m_edtMassAlng.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_X]);
    m_edtMassCros.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_Y]);
    m_edtMassTors.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_T]);
    m_untMassAlng.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_X]);
    m_untMassCros.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_Y]);
    m_untMassTors.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_T]);
}

void CCMWindItemDPTLcomDlg::SetAlign()
{
    m_edtFreqAlng.SetAlign(SS_RIGHT);
    m_edtFreqCros.SetAlign(SS_RIGHT);
    m_edtFreqTors.SetAlign(SS_RIGHT);

    m_edtMassAlng.SetAlign(SS_RIGHT);
    m_edtMassCros.SetAlign(SS_RIGHT);
    m_edtMassTors.SetAlign(SS_RIGHT);

    m_edtDampAlng.SetAlign(SS_RIGHT);
    m_edtDampCros.SetAlign(SS_RIGHT);
    m_edtDampTors.SetAlign(SS_RIGHT);
}

void CCMWindItemDPTLcomDlg::SetData(EN_DPT_WIND_METHOD enMethod, const T_WIND_DPT2007_VIBR* pData)
{
    m_enCalcMethod = enMethod;
    m_pData = const_cast< T_WIND_DPT2007_VIBR* >( pData );
}

bool CCMWindItemDPTLcomDlg::Data2Dlg()
{
    if ( m_pData )
    {
        m_edtFreqAlng.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_FREQ_X]);
        m_edtFreqCros.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_FREQ_Y]);
        m_edtFreqTors.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_FREQ_T]);

        m_edtMassAlng.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_MASS_X]);
        m_edtMassCros.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_MASS_Y]);
        m_edtMassTors.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_MASS_T]);

        m_edtDampAlng.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_DAMP_X]);
        m_edtDampCros.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_DAMP_Y]);
        m_edtDampTors.SetEditUnitAuto(m_pData->dParam[EN_DPT_WIND_VIBR_DAMP_T]);

        SetEnableCtrl();
    }
    return true;
}

bool CCMWindItemDPTLcomDlg::Dlg2Data()
{
    if ( m_pData == nullptr ) { ASSERT(0); return false; }

    m_pData->dParam[EN_DPT_WIND_VIBR_FREQ_X] = m_edtFreqAlng.GetEditValue();
    m_pData->dParam[EN_DPT_WIND_VIBR_FREQ_Y] = m_edtFreqCros.GetEditValue();
    m_pData->dParam[EN_DPT_WIND_VIBR_FREQ_T] = m_edtFreqTors.GetEditValue();

    m_pData->dParam[EN_DPT_WIND_VIBR_MASS_X] = m_edtMassAlng.GetEditValue();
    m_pData->dParam[EN_DPT_WIND_VIBR_MASS_Y] = m_edtMassCros.GetEditValue();
    m_pData->dParam[EN_DPT_WIND_VIBR_MASS_T] = m_edtMassTors.GetEditValue();

    m_pData->dParam[EN_DPT_WIND_VIBR_DAMP_X] = m_edtDampAlng.GetEditValue();
    m_pData->dParam[EN_DPT_WIND_VIBR_DAMP_Y] = m_edtDampCros.GetEditValue();
    m_pData->dParam[EN_DPT_WIND_VIBR_DAMP_T] = m_edtDampTors.GetEditValue();
    return true;
}

void CCMWindItemDPTLcomDlg::OnButtonOK()
{
    if ( !Dlg2Data() ) { ASSERT(0); return; }
    CDialogMove::OnOK();
}

void CCMWindItemDPTLcomDlg::OnButtonCancel()
{
    CDialogMove::OnCancel();
}

void CCMWindItemDPTLcomDlg::SetEnableCtrl()
{
    auto L_GetCtrlID = [ ] (int nVibrType, CArray<UINT, UINT>& raCtrlID)
    {
        raCtrlID.RemoveAll();
        switch ( nVibrType )
        {
        case EN_DPT_WIND_VIBR_FREQ_X:
            {
                raCtrlID.Add(IDC_CMD_LBL_FREQ_ALNG);
                raCtrlID.Add(IDC_CMD_EDT_FREQ_ALNG);
                raCtrlID.Add(IDC_CMD_UNT_FREQ_ALNG);
            }
            break;
        case EN_DPT_WIND_VIBR_FREQ_Y:
            {
                raCtrlID.Add(IDC_CMD_LBL_FREQ_CROS);
                raCtrlID.Add(IDC_CMD_EDT_FREQ_CROS);
                raCtrlID.Add(IDC_CMD_UNT_FREQ_CROS);
            }
            break;
        case EN_DPT_WIND_VIBR_FREQ_T:
            {
                raCtrlID.Add(IDC_CMD_LBL_FREQ_TORS);
                raCtrlID.Add(IDC_CMD_EDT_FREQ_TORS);
                raCtrlID.Add(IDC_CMD_UNT_FREQ_TORS);
            }
            break;
        case EN_DPT_WIND_VIBR_MASS_X:
            {
                raCtrlID.Add(IDC_CMD_LBL_MASS_ALNG);
                raCtrlID.Add(IDC_CMD_EDT_MASS_ALNG);
                raCtrlID.Add(IDC_CMD_UNT_MASS_ALNG);
            }
            break;
        case EN_DPT_WIND_VIBR_MASS_Y:
            {
                raCtrlID.Add(IDC_CMD_LBL_MASS_CROS);
                raCtrlID.Add(IDC_CMD_EDT_MASS_CROS);
                raCtrlID.Add(IDC_CMD_UNT_MASS_CROS);
            }
            break;
        case EN_DPT_WIND_VIBR_MASS_T:
            {
                raCtrlID.Add(IDC_CMD_LBL_MASS_TORS);
                raCtrlID.Add(IDC_CMD_EDT_MASS_TORS);
                raCtrlID.Add(IDC_CMD_UNT_MASS_TORS);
            }
            break;
        case EN_DPT_WIND_VIBR_DAMP_X:
            {
                raCtrlID.Add(IDC_CMD_LBL_DAMP_ALNG);
                raCtrlID.Add(IDC_CMD_EDT_DAMP_ALNG);
            }
            break;
        case EN_DPT_WIND_VIBR_DAMP_Y:
            {
                raCtrlID.Add(IDC_CMD_LBL_DAMP_CROS);
                raCtrlID.Add(IDC_CMD_EDT_DAMP_CROS);
            }
            break;
        case EN_DPT_WIND_VIBR_DAMP_T:
            {
                raCtrlID.Add(IDC_CMD_LBL_DAMP_TORS);
                raCtrlID.Add(IDC_CMD_EDT_DAMP_TORS);
            }
            break;
        default: ASSERT(0); break;
        }
    };

    bool bEnable[EN_DPT_WIND_VIBR_NUM] = { false, };
    GetVibrFlag(m_enCalcMethod, *m_pData, bEnable);

    for ( int i = EN_DPT_WIND_VIBR_FREQ_X; i <EN_DPT_WIND_VIBR_NUM; ++i )
    {
        CArray<UINT, UINT> aCtrlID;
        L_GetCtrlID(i, aCtrlID);
        CDlgUtil::CtrlEnableDisable(this, aCtrlID, bEnable[i]);
    }
}

void CCMWindItemDPTLcomDlg::GetVibrFlag(UINT nMethod, const T_WIND_DPT2007_VIBR& cVibr, bool bUse[EN_DPT_WIND_VIBR_NUM])
{
    bool bCross = false;
    bool bTorsn = false;
    bool bRespo = false;
    if ( nMethod == EN_DPT_WIND_METHOD_DETAIL )
    {
        bCross = cVibr.bAcross ? true : false;
        bTorsn = cVibr.bTorsion ? true : false;
        bRespo = cVibr.bResponse ? true : false;
    }

    for ( int i = EN_DPT_WIND_VIBR_FREQ_X; i <EN_DPT_WIND_VIBR_NUM; ++i )
    {
        bUse[i] = false;
    }

    const bool bCrossRespo = bCross || bRespo;
    const bool bTorsnRepso = bTorsn || bRespo;

    bUse[EN_DPT_WIND_VIBR_FREQ_X] = bCrossRespo;
    bUse[EN_DPT_WIND_VIBR_FREQ_Y] = bCrossRespo;
    bUse[EN_DPT_WIND_VIBR_FREQ_T] = bTorsnRepso;
    bUse[EN_DPT_WIND_VIBR_MASS_X] = bRespo;
    bUse[EN_DPT_WIND_VIBR_MASS_Y] = bRespo;
    bUse[EN_DPT_WIND_VIBR_MASS_T] = bRespo;
    bUse[EN_DPT_WIND_VIBR_DAMP_X] = bCrossRespo;
    bUse[EN_DPT_WIND_VIBR_DAMP_Y] = bCrossRespo;
    bUse[EN_DPT_WIND_VIBR_DAMP_T] = bTorsnRepso;
}