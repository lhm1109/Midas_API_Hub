#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionTendonElemRenumberDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "PostTensionDrawTendonLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPostTensionTendonElemRenumberDlg dialog

CPostTensionTendonElemRenumberDlg::CPostTensionTendonElemRenumberDlg(CWnd* pParent /*=NULL*/)
    : CMenuBarChildDlg(CPostTensionTendonElemRenumberDlg::IDD, pParent)
{
    m_SelType = 1;
    m_tmStartNode = _T("1");
    m_tmStartElement = _T("1");
    m_nRotAxis = 0;
    m_nPolAxis = 0;
    m_aCtrlCartesian.Add(IDC_TM_TENDON_RENUM_CARTESIAN_SORT_PREF_GRP);
    m_aCtrlCartesian.Add(IDC_TM_TENDON_RENUM_CARTESIAN_SORT_ORDER_TXT);
    m_aCtrlCartesian.Add(IDC_TM_TENDON_RENUM_CARTESIAN_GLOBAL_AXIS_TXT);
    m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_1ST_TXT);
    m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_2ND_TXT);
    m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_3RD_TXT);
    m_aCtrlCartesian.Add(IDC_TM_TENDON_RENUM_CARTESIAN_1ST_CMB);
    m_aCtrlCartesian.Add(IDC_TM_TENDON_RENUM_CARTESIAN_2ND_CMB);
    m_aCtrlCartesian.Add(IDC_TM_TENDON_RENUM_CARTESIAN_3RD_CMB);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_SORT_PREF_GRP);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_SORT_ORDER_TXT);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_ITEM_TXT);
    m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_TXT);
    m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_TXT);
    m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_3RD_TXT);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_CMB);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_CMB);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_3RD_CMB);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_SORT_REF_AXIS_GRP);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_GLOBAL_AXIS_RDO);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_TWO_POINTS_RDO);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_REF_AXIS_CMB);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_TXT);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_TXT);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_CMB);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_CMB);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_UNIT);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_UNIT);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_SORT_REF_AXIS_GRP2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_GLOBAL_AXIS_RDO2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_TWO_POINTS_RDO2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_REF_AXIS_CMB2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_TXT2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_TXT2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_CMB2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_CMB2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_UNIT2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_UNIT2);
    m_aCtrlCylinder.Add(IDC_TM_TENDON_PIC);
    m_aCtrlRefAxisGlobal.Add(IDC_TM_TENDON_RENUM_CYLINDER_REF_AXIS_CMB);
    m_aCtrlRefAxisTwoPnt.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_CMB);
    m_aCtrlRefAxisTwoPnt.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_CMB);
    m_aCtrlPolAxisGlobal.Add(IDC_TM_TENDON_RENUM_CYLINDER_REF_AXIS_CMB2);
    m_aCtrlPolAxisTwoPnt.Add(IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_CMB2);
    m_aCtrlPolAxisTwoPnt.Add(IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_CMB2);
    //m_aCtrlButton.Add(IDC_TM_TENDON_EXECUTE);
    //m_aCtrlButton.Add(IDC_TM_TENDON_CLOSE);
    m_szCmb[0] = _LSX((+)X);
    m_szCmb[1] = _LSX((-)X);
    m_szCmb[2] = _LSX((+)Y);
    m_szCmb[3] = _LSX((-)Y);
    m_szCmb[4] = _LSX((+)Z);
    m_szCmb[5] = _LSX((-)Z);
}

void CPostTensionTendonElemRenumberDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_UNIT, m_wnd1stPointUnit);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_UNIT, m_wnd2ndPointUnit);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_UNIT2, m_wnd1stPointUnit2);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_UNIT2, m_wnd2ndPointUnit2);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_SORT_METHOD_CMB, m_SortMethodCmb);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_REF_AXIS_CMB, m_RefAxisCmb);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_REF_AXIS_CMB2, m_PolAxisCmb);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_1ST_CMB, m_CynOrderCmb1);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_2ND_CMB, m_CynOrderCmb2);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_3RD_CMB, m_CynOrderCmb3);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CARTESIAN_3RD_CMB, m_CarOrderCmb3);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CARTESIAN_2ND_CMB, m_CarOrderCmb2);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CARTESIAN_1ST_CMB, m_CarOrderCmb1);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_CMB, m_wndRot1stPoint);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_CMB, m_wndRot2ndPoint);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_1ST_POINT_CMB2, m_wndPol1stPoint);
    DDX_Control(pDX, IDC_TM_TENDON_RENUM_CYLINDER_2ND_POINT_CMB2, m_wndPol2ndPoint);
    DDX_Radio(pDX, IDC_TM_TENDON_SELECT_ALL_RDO, m_SelType);
    DDX_Text(pDX, IDC_TM_TENDON_DEL_START_NODE_EDT, m_tmStartNode);
    DDX_Text(pDX, IDC_TM_TENDON_DEL_START_ELEM_EDT, m_tmStartElement);
    DDX_Radio(pDX, IDC_TM_TENDON_RENUM_CYLINDER_GLOBAL_AXIS_RDO, m_nRotAxis);
    DDX_Radio(pDX, IDC_TM_TENDON_RENUM_CYLINDER_GLOBAL_AXIS_RDO2, m_nPolAxis);
}

BEGIN_MESSAGE_MAP(CPostTensionTendonElemRenumberDlg, CMenuBarChildDlg)
    ON_CBN_SELCHANGE(IDC_TM_TENDON_RENUM_CYLINDER_REF_AXIS_CMB, OnSelchangeCynAxisOrder)
    ON_CBN_SELCHANGE(IDC_TM_TENDON_RENUM_CYLINDER_1ST_CMB, OnSelchangeCynOrder1st)
    ON_CBN_SELCHANGE(IDC_TM_TENDON_RENUM_CYLINDER_2ND_CMB, OnSelchangeCynOrder2nd)
    ON_CBN_SELCHANGE(IDC_TM_TENDON_RENUM_CARTESIAN_1ST_CMB, OnSelchangeCarOrder1st)
    ON_CBN_SELCHANGE(IDC_TM_TENDON_RENUM_CARTESIAN_2ND_CMB, OnSelchangeCarOrder2nd)
    ON_CBN_SELCHANGE(IDC_TM_TENDON_RENUM_SORT_METHOD_CMB, OnSelchangeSortMethodCmb)
    ON_BN_CLICKED(IDC_TM_TENDON_RENUM_CYLINDER_GLOBAL_AXIS_RDO, OnTmCylinderAxisRdo)
    ON_BN_CLICKED(IDC_TM_TENDON_RENUM_CYLINDER_GLOBAL_AXIS_RDO2, OnTmCylinderAxisRdo)
    ON_BN_CLICKED(IDC_TM_TENDON_RENUM_CYLINDER_TWO_POINTS_RDO, OnTmCylinderAxisRdo)
    ON_BN_CLICKED(IDC_TM_TENDON_RENUM_CYLINDER_TWO_POINTS_RDO2, OnTmCylinderAxisRdo)
    //ON_BN_CLICKED(IDC_TM_TENDON_EXECUTE, OnTmExecute)
    //ON_BN_CLICKED(IDC_TM_TENDON_CLOSE, OnTmClose)
END_MESSAGE_MAP()

BOOL CPostTensionTendonElemRenumberDlg::OnInitDialog() 
{
    CMenuBarChildDlg::OnInitDialog();
    m_pDoc = CDBDoc::GetDocPoint();
    m_SelType = 1;
    m_tmStartNode = _T("1");
    m_tmStartElement = _T("1");
    m_nRotAxis = 0;
    m_nPolAxis = 0;
    m_wndRot1stPoint.SetAttUcsPos();
    m_wndRot1stPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
    m_wndRot2ndPoint.SetAttUcsPos();
    m_wndRot2ndPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
    m_wndRot1stPoint.SetNextLink(&m_wndRot2ndPoint);
    m_wndRot2ndPoint.SetNextLink(NULL);
    m_wndRot1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
    m_wndRot2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
    m_wndPol1stPoint.SetAttUcsPos();
    m_wndPol1stPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
    m_wndPol2ndPoint.SetAttUcsPos();
    m_wndPol2ndPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
    m_wndPol1stPoint.SetNextLink(&m_wndPol2ndPoint);
    m_wndPol2ndPoint.SetNextLink(NULL);
    m_wndPol1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
    m_wndPol2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
    m_wnd1stPointUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_wnd2ndPointUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_wnd1stPointUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_wnd2ndPointUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
    //AlignControl();
    SetInitCombString();
    SetEditControl();
    UpdateData(FALSE);
    return TRUE;
}

void CPostTensionTendonElemRenumberDlg::AlignControl()
{
    //CRect rRef;
    //CRect rToMove;
    //int nDistY;
    //GetDlgItem(IDC_TM_TENDON_RENUM_CYLINDER_SORT_REF_AXIS_GRP)->GetWindowRect(rToMove);
    //GetDlgItem(IDC_TM_TENDON_RENUM_CARTESIAN_SORT_PREF_GRP)->GetWindowRect(rRef);
    //nDistY = rRef.top - rToMove.top;
    //CDlgUtil::CtrlMoveDistY(this, m_aCtrlCylinder, nDistY);
    //CWnd* pWndLast = GetDlgItem(IDC_TM_TENDON_PIC);
    //CRect rectLast, rectExecute;
    //pWndLast->GetWindowRect(rectLast);
    //CRect r;
    //GetWindowRect(r);
    //GetDlgItem(IDC_TM_TENDON_EXECUTE)->GetWindowRect(rectExecute);
    //r.bottom = rectLast.bottom + rectExecute.Height();
    //MoveWindow(r);
}

void CPostTensionTendonElemRenumberDlg::SetInitCombString()
{
    m_CarOrderCmb1.AddString(m_szCmb[0]);
    m_CarOrderCmb1.AddString(m_szCmb[1]);
    m_CarOrderCmb1.AddString(m_szCmb[2]);
    m_CarOrderCmb1.AddString(m_szCmb[3]);
    m_CarOrderCmb1.AddString(m_szCmb[4]);
    m_CarOrderCmb1.AddString(m_szCmb[5]);
    m_CarOrderCmb1.SetCurSel(4);
    m_CarOrderCmb2.AddString(m_szCmb[0]);
    m_CarOrderCmb2.AddString(m_szCmb[1]);
    m_CarOrderCmb2.AddString(m_szCmb[2]);
    m_CarOrderCmb2.AddString(m_szCmb[3]);
    m_CarOrderCmb2.SetCurSel(2);
    m_CarOrderCmb3.AddString(m_szCmb[0]);
    m_CarOrderCmb3.AddString(m_szCmb[1]);
    m_CarOrderCmb3.SetCurSel(0);
    m_SortMethodCmb.AddString(_LS(IDS_TM_RENUMBER_CARTESIAN_COORDINATES));
    m_SortMethodCmb.AddString(_LS(IDS_TM_RENUMBER_CYLINDRICAL_COORDINATES));
    m_SortMethodCmb.SetCurSel(0);
    m_RefAxisCmb.AddString(m_szCmb[0]);
    m_RefAxisCmb.AddString(m_szCmb[1]);
    m_RefAxisCmb.AddString(m_szCmb[2]);
    m_RefAxisCmb.AddString(m_szCmb[3]);
    m_RefAxisCmb.AddString(m_szCmb[4]);
    m_RefAxisCmb.AddString(m_szCmb[5]);
    m_RefAxisCmb.SetCurSel(4);
    m_PolAxisCmb.AddString(m_szCmb[0]);
    m_PolAxisCmb.AddString(m_szCmb[1]);
    m_PolAxisCmb.AddString(m_szCmb[2]);
    m_PolAxisCmb.AddString(m_szCmb[3]);
    m_PolAxisCmb.SetCurSel(0);
    m_CynOrderCmb1.AddString(_LS(IDS_TM_RENUMBER_RADIUS));
    m_CynOrderCmb1.AddString(_LS(IDS_TM_RENUMBER_ROTATING_ANGLE));
    m_CynOrderCmb1.AddString(_LS(IDS_TM_RENUMBER_HEIGHT));
    m_CynOrderCmb1.SetCurSel(2);
    m_CynOrderCmb2.AddString(_LS(IDS_TM_RENUMBER_RADIUS));
    m_CynOrderCmb2.AddString(_LS(IDS_TM_RENUMBER_ROTATING_ANGLE));
    m_CynOrderCmb2.SetCurSel(1);
    m_CynOrderCmb3.AddString(_LS(IDS_TM_RENUMBER_RADIUS));
    m_CynOrderCmb3.SetCurSel(0);
}

void CPostTensionTendonElemRenumberDlg::OnSelchangeSortMethodCmb() 
{  
    //SetFrameSize();
    SetEditControl();
}

void CPostTensionTendonElemRenumberDlg::SetFrameSize()
{
    //int nSortMethod = m_SortMethodCmb.GetCurSel();
    //CRect rectRef, rectToMove, rectParent, rectButton;
    //CWnd* pWnd = GetDlgItem(IDC_TM_TENDON_RENUM_CYLINDER_OPTION_GRP);
    //pWnd->GetWindowRect(rectRef);
    //if (nSortMethod == 0)
    //    GetDlgItem(IDC_TM_TENDON_RENUM_CARTESIAN_SORT_PREF_GRP)->GetWindowRect(rectToMove);
    //else
    //    GetDlgItem(IDC_TM_TENDON_PIC)->GetWindowRect(rectToMove);
    //int nHeight = rectToMove.bottom - rectRef.top + 5;
    //rectRef.bottom = rectRef.top + nHeight;
    //pWnd->GetParent()->ScreenToClient(rectRef);
    //pWnd->MoveWindow(rectRef);
    //GetDlgItem(IDC_TM_TENDON_EXECUTE)->GetWindowRect(rectButton);
    //GetDlgItem(IDC_TM_TENDON_RENUM_CYLINDER_OPTION_GRP)->GetWindowRect(rectToMove);
    //int nY = rectToMove.bottom + 5  - rectButton.top;
    //CDlgUtil::CtrlMoveDistY(this, m_aCtrlButton, nY);
    //GetWindowRect(rectParent);
    //int nDlgHeight = rectRef.bottom + 5 + rectButton.Height();
    //MoveWindow(0, 0, rectParent.Width(), nDlgHeight);
    //((CPostTensionDrawTendonLineDlg*)(GetParent()->GetParent()))->SetHolderSize(nDlgHeight);
}

void CPostTensionTendonElemRenumberDlg::OnTmCylinderAxisRdo() 
{
    UpdateData(TRUE);
    SetEditControl();
}

void CPostTensionTendonElemRenumberDlg::SetEditControl()
{
    CEdit* pEdit;
    int nSortMethod = m_SortMethodCmb.GetCurSel();
    CDlgUtil::CtrlShowHide(this, m_aCtrlCartesian, nSortMethod == 0);
    CDlgUtil::CtrlShowHide(this, m_aCtrlCylinder, nSortMethod == 1);
    CDlgUtil::CtrlEnableDisable(this, m_aCtrlRefAxisGlobal, m_nRotAxis == 0);
    CDlgUtil::CtrlEnableDisable(this, m_aCtrlRefAxisTwoPnt, m_nRotAxis == 1);
    CDlgUtil::CtrlEnableDisable(this, m_aCtrlPolAxisGlobal, m_nPolAxis == 0);
    CDlgUtil::CtrlEnableDisable(this, m_aCtrlPolAxisTwoPnt, m_nPolAxis == 1);
}

void CPostTensionTendonElemRenumberDlg::OnSelchangeCarOrder1st() 
{
    m_CarOrderCmb2.ResetContent();
    int nCurSel = m_CarOrderCmb1.GetCurSel();
    switch(nCurSel)
    {
    case 0:
    case 1:
        m_CarOrderCmb2.AddString(m_szCmb[2]);
        m_CarOrderCmb2.AddString(m_szCmb[3]);
        m_CarOrderCmb2.AddString(m_szCmb[4]);
        m_CarOrderCmb2.AddString(m_szCmb[5]);
        break;
    case 2:
    case 3:
        m_CarOrderCmb2.AddString(m_szCmb[0]);
        m_CarOrderCmb2.AddString(m_szCmb[1]);
        m_CarOrderCmb2.AddString(m_szCmb[4]);
        m_CarOrderCmb2.AddString(m_szCmb[5]);
        break;
    case 4:
    case 5:
        m_CarOrderCmb2.AddString(m_szCmb[0]);
        m_CarOrderCmb2.AddString(m_szCmb[1]);
        m_CarOrderCmb2.AddString(m_szCmb[2]);
        m_CarOrderCmb2.AddString(m_szCmb[3]);
        break;
    default:  ASSERT(0); return;
    }
    m_CarOrderCmb2.SetCurSel(0);
    OnSelchangeCarOrder2nd();
}

void CPostTensionTendonElemRenumberDlg::OnSelchangeCarOrder2nd() 
{
    CString strTemp;
    m_CarOrderCmb3.ResetContent();
    switch(m_CarOrderCmb2.GetCurSel())
    {
    case 0:
    case 1:
        m_CarOrderCmb2.GetLBText(2, strTemp);
        m_CarOrderCmb3.AddString(strTemp);
        m_CarOrderCmb2.GetLBText(3, strTemp);
        m_CarOrderCmb3.AddString(strTemp);
        break;
    case 2:
    case 3:
        m_CarOrderCmb2.GetLBText(0, strTemp);
        m_CarOrderCmb3.AddString(strTemp);
        m_CarOrderCmb2.GetLBText(1, strTemp);
        m_CarOrderCmb3.AddString(strTemp);
        break;
    default : ASSERT(0); return;
    }
    m_CarOrderCmb3.SetCurSel(0);
}

void CPostTensionTendonElemRenumberDlg::OnSelchangeCynOrder1st()
{
    CString sOrder[3]={_LS(IDS_TM_RENUMBER_RADIUS), _LS(IDS_TM_RENUMBER_ROTATING_ANGLE), _LS(IDS_TM_RENUMBER_HEIGHT)};
    m_CynOrderCmb2.ResetContent();
    for(int i = 0; i < 3 ; i++)
    {
        if(m_CynOrderCmb1.GetCurSel() != i) m_CynOrderCmb2.AddString(sOrder[i]);
    }
    m_CynOrderCmb2.SetCurSel(0);
    OnSelchangeCynOrder2nd();
}

void CPostTensionTendonElemRenumberDlg::OnSelchangeCynOrder2nd()
{
    CString strTemp;
    m_CynOrderCmb3.ResetContent();
    switch(m_CynOrderCmb2.GetCurSel())
    {
    case 0:
        m_CynOrderCmb2.GetLBText(1, strTemp);
        m_CynOrderCmb3.AddString(strTemp);
        break;
    case 1:
        m_CynOrderCmb2.GetLBText(0, strTemp);
        m_CynOrderCmb3.AddString(strTemp);
        break;
    }
    m_CynOrderCmb3.SetCurSel(0);
}

void CPostTensionTendonElemRenumberDlg::OnSelchangeCynAxisOrder()
{
    m_PolAxisCmb.ResetContent();
    int nCurSel = m_RefAxisCmb.GetCurSel();
    switch(nCurSel)
    {
    case 0:
    case 1:
        m_PolAxisCmb.AddString(m_szCmb[2]);
        m_PolAxisCmb.AddString(m_szCmb[3]);
        m_PolAxisCmb.AddString(m_szCmb[4]);
        m_PolAxisCmb.AddString(m_szCmb[5]);
        break;
    case 2:
    case 3:
        m_PolAxisCmb.AddString(m_szCmb[0]);
        m_PolAxisCmb.AddString(m_szCmb[1]);
        m_PolAxisCmb.AddString(m_szCmb[4]);
        m_PolAxisCmb.AddString(m_szCmb[5]);
        break;
    case 4:
    case 5:
        m_PolAxisCmb.AddString(m_szCmb[0]);
        m_PolAxisCmb.AddString(m_szCmb[1]);
        m_PolAxisCmb.AddString(m_szCmb[2]);
        m_PolAxisCmb.AddString(m_szCmb[3]);
        break;
    default:  ASSERT(0); return;
    }
    m_PolAxisCmb.SetCurSel(0);
}

BOOL CPostTensionTendonElemRenumberDlg::GetRot1stPoint(double &Px, double &Py, double &Pz)
{
    if(m_wndRot1stPoint.IsKeyPressed())
    {
        CString strPos;
        m_wndRot1stPoint.GetWindowText(strPos);
        return GetPosition(strPos,Px,Py,Pz);
    }
    else
    {
        double wx, wy, wz;
        m_wndRot1stPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
    }
    return TRUE;
}

BOOL CPostTensionTendonElemRenumberDlg::GetRot2ndPoint(double &Px, double &Py, double &Pz)
{
    if(m_wndRot2ndPoint.IsKeyPressed())
    {
        CString strPos;
        m_wndRot2ndPoint.GetWindowText(strPos);
        return GetPosition(strPos,Px,Py,Pz);
    }
    else
    {
        double wx, wy, wz;
        m_wndRot2ndPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
    }
    return TRUE;
}

BOOL CPostTensionTendonElemRenumberDlg::GetPol1stPoint(double &Px, double &Py, double &Pz)
{
    if(m_wndPol1stPoint.IsKeyPressed())
    {
        CString strPos;
        m_wndPol1stPoint.GetWindowText(strPos);
        return GetPosition(strPos,Px,Py,Pz);
    }
    else
    {
        double wx, wy, wz;
        m_wndPol1stPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
    }
    return TRUE;
}

BOOL CPostTensionTendonElemRenumberDlg::GetPol2ndPoint(double &Px, double &Py, double &Pz)
{
    if(m_wndPol2ndPoint.IsKeyPressed())
    {
        CString strPos;
        m_wndPol2ndPoint.GetWindowText(strPos);
        return GetPosition(strPos,Px,Py,Pz);
    }
    else
    {
        double wx, wy, wz;
        m_wndPol2ndPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
    }
    return TRUE;
}

BOOL CPostTensionTendonElemRenumberDlg::CheckOrthogonal()
{
    double rot1[3], rot2[3], pol1[3], pol2[3], VecRot[3], nVecRot[3], VecPol[3], nVecPol[3];
    CString strTemp;
    if(m_nRotAxis == 0)
    {
        rot1[0] = 0, rot1[1] = 0, rot1[2] = 0;
        m_RefAxisCmb.GetLBText(m_RefAxisCmb.GetCurSel(), strTemp);
        if      (strTemp == m_szCmb[0]) { rot2[0] = 1.;  rot2[1] = 0.;  rot2[2] = 0.; }
        else if (strTemp == m_szCmb[1]) { rot2[0] = -1.; rot2[1] = 0.;  rot2[2] = 0.; }
        else if (strTemp == m_szCmb[2]) { rot2[0] = 0.;  rot2[1] = 1.;  rot2[2] = 0.; }
        else if (strTemp == m_szCmb[3]) { rot2[0] = 0.;  rot2[1] = -1.; rot2[2] = 0.; }
        else if (strTemp == m_szCmb[4]) { rot2[0] = 0.;  rot2[1] = 0.;  rot2[2] = 1.; }
        else if (strTemp == m_szCmb[5]) { rot2[0] = 0.;  rot2[1] = 0.;  rot2[2] = -1.;}
        else                            { ASSERT(0); rot2[0] = 0.; rot2[1] = 0.; rot2[2] = 0.;};
    }
    else
    {
        GetRot1stPoint(rot1[0], rot1[1], rot1[2]);
        GetRot2ndPoint(rot2[0], rot2[1], rot2[2]);
    }
    VecRot[0] = rot2[0] - rot1[0];
    VecRot[1] = rot2[1] - rot1[1];
    VecRot[2] = rot2[2] - rot1[2];
    if(!CMathFunc::mathNormalize(VecRot, nVecRot))
    {
        GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_ROT_TWO_POINTS_CLOSE));
        return FALSE;
    }
    if(m_nPolAxis == 0)
    {
        pol1[0] = 0, pol1[1] = 0, pol1[2] = 0;
        m_PolAxisCmb.GetLBText(m_PolAxisCmb.GetCurSel(), strTemp);
        if      (strTemp == m_szCmb[0]) { pol2[0] = 1.;  pol2[1] = 0.;  pol2[2] = 0.; }
        else if (strTemp == m_szCmb[1]) { pol2[0] = -1.; pol2[1] = 0.;  pol2[2] = 0.; }
        else if (strTemp == m_szCmb[2]) { pol2[0] = 0.;  pol2[1] = 1.;  pol2[2] = 0.; }
        else if (strTemp == m_szCmb[3]) { pol2[0] = 0.;  pol2[1] = -1.; pol2[2] = 0.; }
        else if (strTemp == m_szCmb[4]) { pol2[0] = 0.;  pol2[1] = 0.;  pol2[2] = 1.; }
        else if (strTemp == m_szCmb[5]) { pol2[0] = 0.;  pol2[1] = 0.;  pol2[2] = -1.;}
        else                            { ASSERT(0); pol2[0] = 0.; pol2[1] = 0.; pol2[2] = 0.;};
    }
    else
    {
        GetPol1stPoint(pol1[0], pol1[1], pol1[2]);
        GetPol2ndPoint(pol2[0], pol2[1], pol2[2]);
    }
    VecPol[0] = pol2[0] - pol1[0];
    VecPol[1] = pol2[1] - pol1[1];
    VecPol[2] = pol2[2] - pol1[2];
    if(!CMathFunc::mathNormalize(VecPol, nVecPol))
    {
        GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_POL_TWO_POINTS_CLOSE));
        return FALSE;
    }
    double dTolerance= m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
    double dDistance;
    if(!CMathFunc::mathIntersectLine(rot1, rot2, pol1, pol2, dTolerance, dDistance, m_org))
    {
        GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_POL_ROL_NOT_INTERSECT));
        return FALSE;
    }
    double dAngle = CMathFunc::mathCrossAngle(nVecRot, nVecPol);
    double dTol = 0.001;
    if(fabs(dAngle - 90) > dTol)
    {
        GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_POL_ROL_NOT_ORTHOGONAL));
        return FALSE;
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            m_rot[i] = m_org[i] + nVecRot[i];
            m_pol[i] = m_org[i] + nVecPol[i];
        }
        return TRUE;
    }
}

void CPostTensionTendonElemRenumberDlg::Execute()
{
    int nSortMethod = m_SortMethodCmb.GetCurSel();
    switch(nSortMethod)
    {
        case 0: ExecuteCartesian();   break;
        case 1: ExecuteCylindrical(); break;
    }
}

void CPostTensionTendonElemRenumberDlg::ExecuteCartesian()
{
    UpdateData(TRUE);
    CArray<T_NODE_K, T_NODE_K> aNodeKey;
    CArray<T_ELEM_K, T_ELEM_K> aElemKey;
    BOOL SelectCheckNode=FALSE;
    BOOL SelectCheckElem=FALSE;
    int OrderTemp = GetOrderOpt();
    StartNode = (T_NODE_K)_ttoi(m_tmStartNode);
    StartElem = (T_ELEM_K)_ttoi(m_tmStartElement);
    I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
    /*switch(m_NumTarget)
    {
      case 0:
          if(m_SelType == 0)
          {
              m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
              m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp);
          }
          else
          {
              if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE))
              {
                  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_)); return;
              }
              if(m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp))
                  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
          }
          break;
      case 1:
          if(m_SelType == 0)
          {
              m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey);
              m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp);
          }
          else
          {
              if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE))
              {
                  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_)); return;
              }
              if(m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp))
                  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
          }
          break;
      case 2:
          if(m_SelType == 0)
          {
              m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
              m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey,FALSE);
              m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp);
          }
          else
          {
              if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE)) SelectCheckNode = TRUE;
              if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE)) SelectCheckElem = TRUE;
              if(SelectCheckNode && SelectCheckElem)
              {
                  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_and_elemen)); return;
              }
              if(m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp))
                  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
          }
        break;
    }*/
}

void CPostTensionTendonElemRenumberDlg::ExecuteCylindrical()
{
    UpdateData(TRUE);
    CArray<T_NODE_K, T_NODE_K> aNodeKey;
    CArray<T_ELEM_K, T_ELEM_K> aElemKey;
    BOOL SelectCheckNode=FALSE;
    BOOL SelectCheckElem=FALSE;
    if(!CheckOrthogonal()) return;
    CString strTemp1,strTemp2;
    int OrderTemp;
    m_CynOrderCmb1.GetLBText(m_CynOrderCmb1.GetCurSel(), strTemp1);
    m_CynOrderCmb2.GetLBText(m_CynOrderCmb2.GetCurSel(), strTemp2);
    if((strTemp1 == _LS(IDS_TM_RENUMBER_HEIGHT)) && (strTemp2 == _LS(IDS_TM_RENUMBER_ROTATING_ANGLE)))      OrderTemp = 6;
    else if((strTemp1 == _LS(IDS_TM_RENUMBER_HEIGHT)) && (strTemp2 == _LS(IDS_TM_RENUMBER_RADIUS)))         OrderTemp = 5;
    else if((strTemp1 == _LS(IDS_TM_RENUMBER_ROTATING_ANGLE)) && (strTemp2 == _LS(IDS_TM_RENUMBER_HEIGHT))) OrderTemp = 4;
    else if((strTemp1 == _LS(IDS_TM_RENUMBER_ROTATING_ANGLE)) && (strTemp2 == _LS(IDS_TM_RENUMBER_RADIUS))) OrderTemp = 3;
    else if((strTemp1 == _LS(IDS_TM_RENUMBER_RADIUS)) && (strTemp2 == _LS(IDS_TM_RENUMBER_HEIGHT)))         OrderTemp = 2;
    else OrderTemp = 1;
    StartNode = (T_NODE_K)_ttoi(m_tmStartNode);
    StartElem = (T_ELEM_K)_ttoi(m_tmStartElement);
    I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
    /*switch(m_NumTarget)
    {
      case 0:
          if(m_SelType == 0)
          {
              m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
              m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol);
          }
          else
          {
              if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE))
              {
                  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_)); return;
              }
              if(m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol))
                  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
          }
          break;
      case 1:
          if(m_SelType == 0)
          {
              m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey);
              m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol);
          }
          else
          {
              if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE))
              {
                  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_)); return;
              }
              if(m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol))
                  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
          }
          break;
      case 2:
          if(m_SelType == 0)
          {
              m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
              m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey);
              m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol);
          }
          else
          {
              if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE)) SelectCheckNode = TRUE;
              if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE)) SelectCheckElem = TRUE;
              if(SelectCheckNode && SelectCheckElem)
              {
                  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_and_elemen)); return;
              }
              if(m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol))
                  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
          }
        break;
    }*/
}

//void CPostTensionTendonElemRenumberDlg::OnTmExecute()
//{
//    ((CPostTensionDrawTendonLineDlg*)(GetParent()->GetParent()))->OnTmExecAndClose(TRUE);
//}
//
//void CPostTensionTendonElemRenumberDlg::OnTmClose()
//{
//    ((CPostTensionDrawTendonLineDlg*)(GetParent()->GetParent()))->OnTmExecAndClose(FALSE);
//}

int CPostTensionTendonElemRenumberDlg::GetOrderOpt()
{
    int nRet = 0;
    int nCursel1 = m_CarOrderCmb1.GetCurSel();
    int nCursel2 = m_CarOrderCmb2.GetCurSel();
    int nCursel3 = m_CarOrderCmb3.GetCurSel();
    nRet = nCursel1*8 + nCursel2*2 + nCursel3;
    return nRet;
}

