#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemDPTPeriodDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum EN_DPT_PERIOD_METHOD
{
    EN_DPT_PERIOD_METHOD_RCS = 0,
    EN_DPT_PERIOD_METHOD_STL,
    EN_DPT_PERIOD_METHOD_USR,
};

CSeisItemDPTPeriodDlg::CSeisItemDPTPeriodDlg(CWnd* pParent)
    : CDialogMove(CSeisItemDPTPeriodDlg::IDD, pParent)
{
    m_dHeight = 0.0;
    m_dDefaultN = 0.025;
    m_dMajorT = 0.0;
    m_dOrthoT = 0.0;
}

void CSeisItemDPTPeriodDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMD_EDT_MAJR_HGHT, m_edtMajorH);
    DDX_Control(pDX, IDC_CMD_EDT_MAJR_NUMB, m_edtMajorN);
    DDX_Control(pDX, IDC_CMD_EDT_MAJR_RSLT, m_edtMajorT);

    DDX_Control(pDX, IDC_CMD_EDT_ORTH_HGHT, m_edtOrthoH);
    DDX_Control(pDX, IDC_CMD_EDT_ORTH_NUMB, m_edtOrthoN);
    DDX_Control(pDX, IDC_CMD_EDT_ORTH_RSLT, m_edtOrthoT);
}

BEGIN_MESSAGE_MAP(CSeisItemDPTPeriodDlg, CDialogMove)
    ON_BN_CLICKED(IDC_CMD_RDO_MAJR_EQN1, OnChangeMajorEQ)
    ON_BN_CLICKED(IDC_CMD_RDO_MAJR_EQN2, OnChangeMajorEQ)
    ON_BN_CLICKED(IDC_CMD_RDO_MAJR_EQN3, OnChangeMajorEQ)
    ON_BN_CLICKED(IDC_CMD_BTN_MAJR_CALC, OnBtnCalcMajor)

    ON_BN_CLICKED(IDC_CMD_RDO_ORTH_EQN1, OnChangeOrthoEQ)
    ON_BN_CLICKED(IDC_CMD_RDO_ORTH_EQN2, OnChangeOrthoEQ)
    ON_BN_CLICKED(IDC_CMD_RDO_ORTH_EQN3, OnChangeOrthoEQ)
    ON_BN_CLICKED(IDC_CMD_BTN_ORTH_CALC, OnBtnCalcOrtho)

    ON_BN_CLICKED(IDC_CMD_BTN_OK, OnOK)
    ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCancel)
END_MESSAGE_MAP()

void CSeisItemDPTPeriodDlg::SetData(const double& dH)
{
    m_dHeight = dH;
}

BOOL CSeisItemDPTPeriodDlg::OnInitDialog()
{
    CDialogMove::OnInitDialog();
    InitUnit();
    SetAlign();
    Data2Dlg();
    return TRUE;
}

void CSeisItemDPTPeriodDlg::InitUnit()
{
    m_edtMajorH.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_edtOrthoH.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CSeisItemDPTPeriodDlg::SetAlign()
{
    m_edtMajorH.SetAlign(SS_RIGHT);
    m_edtMajorN.SetAlign(SS_RIGHT);
    m_edtMajorT.SetAlign(SS_RIGHT);

    m_edtOrthoH.SetAlign(SS_RIGHT);
    m_edtOrthoN.SetAlign(SS_RIGHT);
    m_edtOrthoT.SetAlign(SS_RIGHT);
}

void CSeisItemDPTPeriodDlg::Data2Dlg()
{
    UINT nMethodMajor = EN_DPT_PERIOD_METHOD_RCS;
    SetMethod(true, nMethodMajor);
    m_edtMajorH.SetEditUnitAuto(m_dHeight);
    m_edtMajorN.SetEditUnitAuto(m_dDefaultN);
    SetCtrlByMethod(true, nMethodMajor);

    UINT nMethodOrtho = EN_DPT_PERIOD_METHOD_RCS;
    SetMethod(false, nMethodOrtho);
    m_edtOrthoH.SetEditUnitAuto(m_dHeight);
    m_edtOrthoN.SetEditUnitAuto(m_dDefaultN);
    SetCtrlByMethod(false, nMethodOrtho);
}

void CSeisItemDPTPeriodDlg::SetMethod(bool bMajor, UINT nMethod)
{
    CArray<UINT, UINT> aCtrlID;
    if ( bMajor )
    {
        aCtrlID.Add(IDC_CMD_RDO_MAJR_EQN1);
        aCtrlID.Add(IDC_CMD_RDO_MAJR_EQN2);
        aCtrlID.Add(IDC_CMD_RDO_MAJR_EQN3);
    }
    else
    {
        aCtrlID.Add(IDC_CMD_RDO_ORTH_EQN1);
        aCtrlID.Add(IDC_CMD_RDO_ORTH_EQN2);
        aCtrlID.Add(IDC_CMD_RDO_ORTH_EQN3);
    }
    CDlgUtil::CtrlRadioSetCheck(this, aCtrlID, nMethod);
}

UINT CSeisItemDPTPeriodDlg::GetMethod(bool bMajor)
{
    CArray<UINT, UINT> aCtrlID;
    if ( bMajor )
    {
        aCtrlID.Add(IDC_CMD_RDO_MAJR_EQN1);
        aCtrlID.Add(IDC_CMD_RDO_MAJR_EQN2);
        aCtrlID.Add(IDC_CMD_RDO_MAJR_EQN3);
    }
    else
    {
        aCtrlID.Add(IDC_CMD_RDO_ORTH_EQN1);
        aCtrlID.Add(IDC_CMD_RDO_ORTH_EQN2);
        aCtrlID.Add(IDC_CMD_RDO_ORTH_EQN3);
    }

    int nMethod = 0;
    CDlgUtil::CtrlRadioGetCheck(this, aCtrlID, nMethod);

    switch ( nMethod )
    {
    case 0: return EN_DPT_PERIOD_METHOD_RCS;
    case 1: return EN_DPT_PERIOD_METHOD_STL;
    case 2: return EN_DPT_PERIOD_METHOD_USR;
    }
    ASSERT(0); return EN_DPT_PERIOD_METHOD_RCS;
}

void CSeisItemDPTPeriodDlg::OnChangeMajorEQ()
{
    UINT nMethod = GetMethod(true);
    SetCtrlByMethod(true, nMethod);
}

void CSeisItemDPTPeriodDlg::OnChangeOrthoEQ()
{
    UINT nMethod = GetMethod(false);
    SetCtrlByMethod(false, nMethod);
}

void CSeisItemDPTPeriodDlg::SetCtrlByMethod(bool bMajor, UINT nMethod)
{
    CArray<UINT, UINT> aCtrlH, aCtrlN;
    if ( bMajor )
    {
        aCtrlH.Add(IDC_CMD_LBL_MAJR_HGHT);
        aCtrlH.Add(IDC_CMD_EDT_MAJR_HGHT);
        aCtrlH.Add(IDC_CMD_UNT_MAJR_HGHT);

        aCtrlN.Add(IDC_CMD_LBL_MAJR_NUMB);
        aCtrlN.Add(IDC_CMD_EDT_MAJR_NUMB);
    }
    else
    {
        aCtrlH.Add(IDC_CMD_LBL_ORTH_HGHT);
        aCtrlH.Add(IDC_CMD_EDT_ORTH_HGHT);
        aCtrlH.Add(IDC_CMD_UNT_ORTH_HGHT);

        aCtrlN.Add(IDC_CMD_LBL_ORTH_NUMB);
        aCtrlN.Add(IDC_CMD_EDT_ORTH_NUMB);
    }

    BOOL bEnableH = TRUE;
    BOOL bEnableN = TRUE;
    switch ( nMethod )
    {
    case EN_DPT_PERIOD_METHOD_RCS:
    case EN_DPT_PERIOD_METHOD_STL:
        {
            bEnableH = TRUE;
            bEnableN = FALSE;
        }
        break;
    case EN_DPT_PERIOD_METHOD_USR:
        {
            bEnableH = TRUE;
            bEnableN = TRUE;
        }
        break;
    default: ASSERT(0); break;
    }
    CDlgUtil::CtrlEnableDisable(this, aCtrlH, bEnableH);
    CDlgUtil::CtrlEnableDisable(this, aCtrlN, bEnableN);
}

void CSeisItemDPTPeriodDlg::OnBtnCalcMajor()
{
    double dT = CalcPeriod(true);
    m_edtMajorT.SetEditUnitAuto(dT);
}

void CSeisItemDPTPeriodDlg::OnBtnCalcOrtho()
{
    double dT = CalcPeriod(false);
    m_edtOrthoT.SetEditUnitAuto(dT);
}

double CSeisItemDPTPeriodDlg::CalcPeriod(bool bMajor)
{
    if ( bMajor )
    {
        UINT nMethod = GetMethod(true);
        double dH = m_edtMajorH.GetEditValue();
        double dN = m_edtMajorN.GetEditValue();
        return CalcPeriod(nMethod, dH, dN);
    }
    else
    {
        UINT nMethod = GetMethod(false);
        double dH = m_edtOrthoH.GetEditValue();
        double dN = m_edtOrthoN.GetEditValue();
        return CalcPeriod(nMethod, dH, dN);
    }
}

double CSeisItemDPTPeriodDlg::CalcPeriod(UINT nMethod, const double& dH, const double& dN)
{
    auto L_GetFactor = [ ] (UINT nMethod, const double& dN) -> double
    {
        switch ( nMethod )
        {
        case EN_DPT_PERIOD_METHOD_RCS: return 0.02;
        case EN_DPT_PERIOD_METHOD_STL: return 0.03;
        case EN_DPT_PERIOD_METHOD_USR: return dN;
        }
        ASSERT(0); return 0.02;
    };

    double dFact = L_GetFactor(nMethod, dN);
    return dH * dFact;
}

bool CSeisItemDPTPeriodDlg::GetPeriod(double& rdMajorT, double& rdOrthoT)
{
    rdMajorT = m_dMajorT;
    rdOrthoT = m_dOrthoT;
    return true;
}

void CSeisItemDPTPeriodDlg::OnOK()
{
    UpdateData(TRUE);
    m_dMajorT = CalcPeriod(true);
    m_dOrthoT = CalcPeriod(false);
    CDialogMove::OnOK();
}

void CSeisItemDPTPeriodDlg::OnCancel()
{
    CDialogMove::OnCancel();
}