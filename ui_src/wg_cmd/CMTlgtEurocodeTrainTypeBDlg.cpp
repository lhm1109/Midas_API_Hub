#include "stdafx.h"

#include "../wg_base/wg_base_strParser.h"
#include "../wg_db/UnitCtrl.h"
#include "../wg_db/DBDoc.h"

#include "wg_cmd.h"

#include "CMTlgtEurocodeTrainTypeBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMTlgtEurocodeTrainTypeBDlg::CCMTlgtEurocodeTrainTypeBDlg(CDBDoc* pDoc,
                                                           CWnd* pParent) :
    CDialogMove(CCMTlgtEurocodeTrainTypeBDlg::IDD, pParent),
    m_SuperControl(nullptr)
{
}

CCMTlgtEurocodeTrainTypeBDlg::~CCMTlgtEurocodeTrainTypeBDlg()
{
}

bool CCMTlgtEurocodeTrainTypeBDlg::SetSuperControl(CWnd* pControl)
{
    if ( !pControl )
    {
        ASSERT(0); return false;
    }

    m_SuperControl = pControl;

    return true;
}

bool CCMTlgtEurocodeTrainTypeBDlg::GetOptionValueNU(UINT& runForceNumber,
                                                    double& rdSpace)
{
    auto* pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;

    runForceNumber = static_cast<UINT>(m_EdtForceNumb.GetEditValue());
    rdSpace        = pUnitCtrl->ConvertUnitLengthIn(m_EdtSpace.GetEditValue());

    return true;
}

void CCMTlgtEurocodeTrainTypeBDlg::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_ETC_TLGT_FORCE_NUMB_EDT, m_EdtForceNumb);
    DDX_Control(pDX, IDC_ETC_TLGT_SPACING_EDT, m_EdtSpace);
    DDX_Control(pDX, IDC_ETC_TLGT_SPACING_UNIT_STC, m_StcSpaceUnit);

    return;
}

BOOL CCMTlgtEurocodeTrainTypeBDlg::OnInitDialog()
{
    __super::OnInitDialog();

    SetInitPosbySuperControl();
    m_EdtForceNumb.SetUnitType(D_UNITSYS_NONE);
    m_EdtSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_StcSpaceUnit.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);

    return TRUE;
}

BEGIN_MESSAGE_MAP(CCMTlgtEurocodeTrainTypeBDlg, CDialogMove)
    ON_BN_CLICKED(IDOK, OnOK)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

void CCMTlgtEurocodeTrainTypeBDlg::OnOK()
{
    CString str;
    if ( !IsValid(str) )
    {
        AfxMessageBox(str);
        return;
    }

    return __super::OnOK();
}

void CCMTlgtEurocodeTrainTypeBDlg::OnCancel()
{
    return __super::OnCancel();
}

bool CCMTlgtEurocodeTrainTypeBDlg::IsValid(CString& str)
{
    const auto unForceNumb = static_cast<UINT>(m_EdtForceNumb.GetEditValue());
    if ( unForceNumb == 0U )
    {
        str.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_FORCE_NUMBER));
        SetForceAndClear(m_EdtForceNumb);
        return false;
    }

    CString strSpaceText;
    m_EdtSpace.GetWindowText(strSpaceText);

    auto dTemp = 0.0;
    if ( strSpaceText == _T("0") ||
        !CStrParser::GetFloatNumber(strSpaceText, dTemp) )
    {
        str.Format(_LS(IDS_CMD_ERR_INVALID_VALUE), _LS(IDS_CMD_TLGT_SPACING));
        SetForceAndClear(m_EdtSpace);
        return false;
    }

    const auto& dSpace = m_EdtSpace.GetEditValue();
    if ( dSpace <= 0.0 ) //이건 입력변수라 0.0 잡으려면 tolerance를 줄수가 없..
    {
        str.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_SPACING));
        SetForceAndClear(m_EdtSpace);
        return false;
    }

    return true;
}

void CCMTlgtEurocodeTrainTypeBDlg::SetForceAndClear(CEditUnit& rEditUnit)
{
    rEditUnit.SetFocus();
    rEditUnit.SetSel(0, 1, TRUE);

    return;
}

void CCMTlgtEurocodeTrainTypeBDlg::SetInitPosbySuperControl()
{
    //상위 함수인 SetInitPos로 부모 Control기준으로 초기 대화상자 위치를 잡으려고 했는디,
    //상위 Control이 아닌 상위 Dialog를 잡아와서 강제로 잡게 하기 위해 별도 함수를 뚫음.

    CRect rtCurDlg;
    __super::GetWindowRect(rtCurDlg);

    CRect rtControl;
    m_SuperControl->GetWindowRect(rtControl);

    const auto WidthCurDlg = rtCurDlg.Width();
    const auto HeightCurDlg = rtCurDlg.Height();

    rtCurDlg.left = rtControl.right;
    rtCurDlg.top = rtControl.top;
    rtCurDlg.right = rtCurDlg.left + WidthCurDlg;
    rtCurDlg.bottom = rtCurDlg.top + HeightCurDlg;

    __super::MoveWindow(rtCurDlg);
    __super::MoveDlgWithinScreen();

    return;
}