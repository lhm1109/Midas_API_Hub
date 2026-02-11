#include "stdafx.h"
#include <math.h>
#include "wg_cmd.h"
#include "CMWindItemDPTinfoDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMWindItemDPTinfoDlg::CCMWindItemDPTinfoDlg(CWnd* pParent)
    : CDialogMove(CCMWindItemDPTinfoDlg::IDD, pParent)
{
    m_pData = nullptr;
}

void CCMWindItemDPTinfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMD_CMB_STRT_TYPE, m_cmbStrtType);
    DDX_Control(pDX, IDC_CMD_EDT_STRT_MAJR, m_edtSizeMajr);
    DDX_Control(pDX, IDC_CMD_EDT_STRT_ORTH, m_edtSizeOrth);
    DDX_Control(pDX, IDC_CMD_UNT_STRT_MAJR, m_untSizeMajr);
    DDX_Control(pDX, IDC_CMD_UNT_STRT_ORTH, m_untSizeOrth);

    DDX_Control(pDX, IDC_CMD_EDT_FREQ_MAJR, m_edtFreqMajr);
    DDX_Control(pDX, IDC_CMD_EDT_FREQ_ORTH, m_edtFreqOrth);
    DDX_Control(pDX, IDC_CMD_EDT_DAMP_MAJR, m_edtDampMajr);
    DDX_Control(pDX, IDC_CMD_EDT_DAMP_ORTH, m_edtDampOrth);
}

BEGIN_MESSAGE_MAP(CCMWindItemDPTinfoDlg, CDialogMove)
    ON_BN_CLICKED(IDC_CMD_BTN_OK, OnButtonOK)
    ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnButtonCancel)
END_MESSAGE_MAP()

BOOL CCMWindItemDPTinfoDlg::OnInitDialog()
{
    CDialogMove::OnInitDialog();
    InitUnit();
    SetAlign();
    InitCombo();
    Data2Dlg();
    SetEnableStructType(false);
    return TRUE;
}

void CCMWindItemDPTinfoDlg::InitUnit()
{
    m_edtSizeMajr.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dBreadth);
    m_edtSizeOrth.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dBreadth);
    m_untSizeMajr.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dBreadth);
    m_untSizeOrth.SetUnitType(CUnitCtrl::m_WIND_UNIT.DPT2007_dBreadth);
}

void CCMWindItemDPTinfoDlg::SetAlign()
{
    m_edtSizeMajr.SetAlign(SS_RIGHT);
    m_edtSizeOrth.SetAlign(SS_RIGHT);

    m_edtFreqMajr.SetAlign(SS_RIGHT);
    m_edtFreqOrth.SetAlign(SS_RIGHT);
    m_edtDampMajr.SetAlign(SS_RIGHT);
    m_edtDampOrth.SetAlign(SS_RIGHT);
}

void CCMWindItemDPTinfoDlg::InitCombo()
{
    std::vector<std::pair<CString, DWORD>> vStrtType;
    vStrtType.push_back(std::make_pair(_T("( Partial ) Enclosed"), EN_DPT_WIND_OPEN_STRT_ENCL));
    vStrtType.push_back(std::make_pair(_T("Opened or Others"), EN_DPT_WIND_OPEN_STRT_OPEN));
    CDlgUtil::InitComboBox(m_cmbStrtType, vStrtType);
}

void CCMWindItemDPTinfoDlg::SetData(const T_WIND_DPT2007* pData)
{
    m_pData = const_cast< T_WIND_DPT2007* >( pData );
}

bool CCMWindItemDPTinfoDlg::Data2Dlg()
{
    if ( m_pData )
    {
        m_cmbStrtType.SetCurSel(m_pData->unOpenStructType);
        m_edtSizeMajr.SetEditUnitAuto(m_pData->dBreadth[0]);
        m_edtSizeOrth.SetEditUnitAuto(m_pData->dBreadth[1]);

        m_edtFreqMajr.SetEditUnitAuto(m_pData->dFrequency[0]);
        m_edtFreqOrth.SetEditUnitAuto(m_pData->dFrequency[1]);
        m_edtDampMajr.SetEditUnitAuto(m_pData->dDamping[0]);
        m_edtDampOrth.SetEditUnitAuto(m_pData->dDamping[1]);
    }
    return true;
}

bool CCMWindItemDPTinfoDlg::Dlg2Data()
{
    if ( m_pData == nullptr ) { ASSERT(0); return false; }
    m_pData->unOpenStructType = static_cast< EN_DPT_WIND_OPEN_STRT >( m_cmbStrtType.GetCurSel() );
    m_pData->dBreadth[0]      = m_edtSizeMajr.GetEditValue();
    m_pData->dBreadth[1]      = m_edtSizeOrth.GetEditValue();

    m_pData->dFrequency[0] = m_edtFreqMajr.GetEditValue();
    m_pData->dFrequency[1] = m_edtFreqOrth.GetEditValue();
    m_pData->dDamping[0]   = m_edtDampMajr.GetEditValue();
    m_pData->dDamping[1]   = m_edtDampOrth.GetEditValue();
    return true;
}

void CCMWindItemDPTinfoDlg::OnButtonOK()
{
    if ( !Dlg2Data() ) { ASSERT(0); return; }
    CDialogMove::OnOK();
}

void CCMWindItemDPTinfoDlg::OnButtonCancel()
{
    CDialogMove::OnCancel();
}

void CCMWindItemDPTinfoDlg::SetEnableStructType(bool bEnable)
{
    CArray<UINT, UINT> aCtrlID;
    aCtrlID.Add(IDC_CMD_LBL_STRT_TYPE);
    aCtrlID.Add(IDC_CMD_CMB_STRT_TYPE);
    CDlgUtil::CtrlEnableDisable(this, aCtrlID, bEnable);
}