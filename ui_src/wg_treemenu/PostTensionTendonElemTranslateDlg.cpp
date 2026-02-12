#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionTendonElemTranslateDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MOUSEEDIT_USE_WHAT_SET_DIST MOUSEEDIT_USE_NODE_SET_DIST

/////////////////////////////////////////////////////////////////////////////
// CPostTensionTendonElemTranslateDlg dialog

CPostTensionTendonElemTranslateDlg::CPostTensionTendonElemTranslateDlg(CWnd* pParent /*=NULL*/)
    : CMenuBarChildDlg(CPostTensionTendonElemTranslateDlg::IDD, pParent)
{
    m_nCopyOrMove = 0;
    m_nOption = 1;
    m_nChooseAxis = 0;
    m_aIterNum.Add(IDC_TM_TENDON_NTIMES_STC);
    m_aIterNum.Add(IDC_TM_TENDON_ITER_NUM_EDT);
    m_aIterNum.Add(IDC_TM_TENDON_ITER_NUM_SPIN);
    m_aIterNum2.Add(IDC_TM_TENDON_NTIMES_STC2);
    m_aIterNum2.Add(IDC_TM_TENDON_ITER_NUM_EDT2);
    m_aIterNum2.Add(IDC_TM_TENDON_ITER_NUM_SPIN2);
    m_NodeInc.Add(IDC_TM_TENDON_NODE_INC_EDT);
    m_NodeInc.Add(IDC_TM_TENDON_NODE_INC_SPIN);
    m_EqualDistance.Add(IDC_TM_TENDON_EQU_XYZ_STC);
    m_EqualDistance.Add(IDC_TM_TENDON_XYZ_EQUDIST_EDT);
    m_EqualDistance.Add(IDC_TM_TENDON_DIST_UNIT1);
    m_UEqualDistance.Add(IDC_TM_TENDON_STC4);
    m_UEqualDistance.Add(IDC_TM_TENDON_XAXIS_RDO);
    m_UEqualDistance.Add(IDC_TM_TENDON_YAXIS_RDO);
    m_UEqualDistance.Add(IDC_TM_TENDON_ZAXIS_RDO);
    m_UEqualDistance.Add(IDC_TM_TENDON_ARBITRARY_RDO);
    m_UEqualDistance.Add(IDC_TM_TENDON_STC5);
    m_UEqualDistance.Add(IDC_TM_TENDON_UNEQ_DIST_EDT);
    m_UEqualDistance.Add(IDC_TM_TENDON_DIST_UNIT2);
    m_UEqualDistance.Add(IDC_TM_TENDON_STC3);
    m_UEqualDistance.Add(IDC_TM_TENDON_DIR_STC);
    m_UEqualDistance.Add(IDC_TM_TENDON_DIR_VECTOR_EDT);
    m_UEqualDistance.Add(IDC_TM_TENDON_DIST_UNIT3);
}

void CPostTensionTendonElemTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TM_TENDON_ITER_NUM_EDT2, m_wndIterNum2);
    DDX_Control(pDX, IDC_TM_TENDON_DIR_VECTOR_EDT, m_wndDirVector);
    DDX_Control(pDX, IDC_TM_TENDON_XYZ_EQUDIST_EDT, m_wndEQDist);
    DDX_Control(pDX, IDC_TM_TENDON_UNEQ_DIST_EDT, m_wndUEQDist);
    DDX_Control(pDX, IDC_TM_TENDON_ITER_NUM_EDT, m_wndIterNum);
    DDX_Control(pDX, IDC_TM_TENDON_NODE_INC_EDT, m_wndNodeInc);
    DDX_Control(pDX, IDC_TM_TENDON_DIST_UNIT3, m_DistUnit3);
    DDX_Control(pDX, IDC_TM_TENDON_DIST_UNIT2, m_DistUnit2);
    DDX_Control(pDX, IDC_TM_TENDON_DIST_UNIT1, m_DistUnit1);
    DDX_Radio(pDX, IDC_TM_TENDON_COPY_RDO, m_nCopyOrMove);
    DDX_Radio(pDX, IDC_TM_TENDON_NODE_INC_RDO, m_nOption);
    DDX_Radio(pDX, IDC_TM_TENDON_XAXIS_RDO, m_nChooseAxis);
}

//#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CPostTensionTendonElemTranslateDlg, CMenuBarChildDlg)
//#undef CDialog
    ON_BN_CLICKED(IDC_TM_TENDON_COPY_RDO, OnTmCopyRadio)
    ON_BN_CLICKED(IDC_TM_TENDON_MOVE_RDO, OnTmMoveRadio)
    ON_BN_CLICKED(IDC_TM_TENDON_ARBITRARY_RDO, OnTmAxisRadios)
    //ON_NOTIFY(UDN_DELTAPOS, IDC_TM_TENDON_ITER_NUM_SPIN, OnDeltaposTmIterNumSpin)
    ON_BN_CLICKED(IDC_TM_TENDON_NODE_INC_RDO, OnTmNodeIncRadio)
    ON_BN_CLICKED(IDC_TM_TENDON_EQUAL_DIST_RDO, OnTmEqualDistRadio)
    ON_BN_CLICKED(IDC_TM_TENDON_UNEQUAL_DIST_RDO, OnTmUnequalDistRadio)
    //ON_NOTIFY(UDN_DELTAPOS, IDC_TM_TENDON_NODE_INC_SPIN, OnDeltaposTmIncSpin)
    //ON_NOTIFY(UDN_DELTAPOS, IDC_TM_TENDON_ITER_NUM_SPIN2, OnDeltaposTmIterNumSpin)
    ON_BN_CLICKED(IDC_TM_TENDON_XAXIS_RDO, OnTmAxisRadios)
    ON_BN_CLICKED(IDC_TM_TENDON_YAXIS_RDO, OnTmAxisRadios)
    ON_BN_CLICKED(IDC_TM_TENDON_ZAXIS_RDO, OnTmAxisRadios)
END_MESSAGE_MAP()

BOOL CPostTensionTendonElemTranslateDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();
    m_wndEQDist.SetAttUcsDistance();
    m_wndEQDist.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
    m_wndDirVector.SetAttUcsDistance();
    m_wndDirVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
    m_DistUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_DistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_DistUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);
    m_wndIterNum.SetRange(SHRT_MIN, SHRT_MAX);
    m_wndIterNum.SetWindowText(_T("1"));
    m_wndIterNum2.SetRange(SHRT_MIN, SHRT_MAX);
    m_wndIterNum2.SetWindowText(_T("1"));
    m_wndDirVector.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
    m_wndEQDist.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
    m_wndNodeInc.SetRange(SHRT_MIN, SHRT_MAX);
    m_wndNodeInc.SetWindowText(_T("0"));
    if(m_nCopyOrMove == 0)
    {
        CtrlEnableDisable(m_EnableOnMove,FALSE);
    }
    else
    {
        CtrlEnableDisable(m_EnableOnMove,TRUE);
    }
    BOOL bEnable;
    if (m_nOption == 2 && m_nChooseAxis == 3)
        bEnable = TRUE;
    else bEnable = FALSE;
    CWnd* pWnd = GetDlgItem(IDC_TM_TENDON_DIR_VECTOR_EDT);
    if(pWnd) pWnd->EnableWindow(bEnable);
    pWnd = GetDlgItem(IDC_TM_TENDON_DIR_STC);
    if(pWnd) pWnd->EnableWindow(bEnable);
    if( m_nOption == 0)
    {
        CtrlEnableDisable(m_EqualDistance,FALSE);
        CtrlEnableDisable(m_UEqualDistance,FALSE);
        if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum, TRUE);
        else CtrlEnableDisable(m_aIterNum, FALSE);
        CtrlEnableDisable(m_aIterNum2, FALSE);
    }
    else if (m_nOption == 1)
    {
        CtrlEnableDisable(m_NodeInc, FALSE);
        CtrlEnableDisable(m_UEqualDistance,FALSE);
        if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum2, TRUE);
        else CtrlEnableDisable(m_aIterNum2, FALSE);
        CtrlEnableDisable(m_aIterNum, FALSE);
    }
    else
    {
        CtrlEnableDisable(m_NodeInc, FALSE);
        CtrlEnableDisable(m_EqualDistance,FALSE);
        CtrlEnableDisable(m_aIterNum, FALSE);
        CtrlEnableDisable(m_aIterNum2, FALSE);
    }
#ifdef _MVIL
    CtrlEnableDisable(m_aThik, FALSE);
#endif
    GotoDlgCtrl(&m_wndNodeInc);
    return FALSE;
}

void CPostTensionTendonElemTranslateDlg::OnTmCopyRadio() 
{
    if(1 == m_nCopyOrMove)
    {
        m_nCopyOrMove = 0;
        CtrlEnableDisable(m_EnableOnMove,FALSE);
        BOOL bEnable, bEnable2;
        if (m_nOption == 0) { bEnable = TRUE; bEnable2 = FALSE; }
        else if (m_nOption == 1) { bEnable = FALSE; bEnable2 = TRUE; }
        else { bEnable = FALSE; bEnable2 = FALSE; }
        CtrlEnableDisable(m_aIterNum, bEnable);
        CtrlEnableDisable(m_aIterNum2, bEnable2);
#ifdef _MVIL
        CtrlEnableDisable(m_aThik, FALSE);
#endif
    }
}

void CPostTensionTendonElemTranslateDlg::OnTmMoveRadio() 
{
    if(0 == m_nCopyOrMove)
    {
        m_nCopyOrMove = 1;
        CtrlEnableDisable(m_EnableOnMove,TRUE);
        CtrlEnableDisable(m_aIterNum, FALSE);
        CtrlEnableDisable(m_aIterNum2, FALSE);
    }
}

void CPostTensionTendonElemTranslateDlg::OnTmAxisRadios() 
{
    UpdateData(TRUE);
    if(m_nChooseAxis == 3)
    {
        CWnd* pWnd = GetDlgItem(IDC_TM_TENDON_DIR_VECTOR_EDT);
        if(pWnd) pWnd->EnableWindow(TRUE);
        pWnd = GetDlgItem(IDC_TM_TENDON_DIR_STC);
        if(pWnd) pWnd->EnableWindow(TRUE);
    }
    else
    {
        CWnd* pWnd = GetDlgItem(IDC_TM_TENDON_DIR_VECTOR_EDT);
        if(pWnd) pWnd->EnableWindow(FALSE);
        pWnd = GetDlgItem(IDC_TM_TENDON_DIR_STC);
        if(pWnd) pWnd->EnableWindow(FALSE);
    }
    GotoDlgCtrl(&m_wndUEQDist);
}

void CPostTensionTendonElemTranslateDlg::OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
    const MSG* pMsg = GetCurrentMessage();
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int NIter;
    CString strIter;
    CFormulaEditSpin* pIterNum;
    UINT nID = LOWORD(pMsg->wParam);
    switch(nID)
    {
    case IDC_TM_TENDON_ITER_NUM_SPIN: pIterNum = &m_wndIterNum; break;
    case IDC_TM_TENDON_ITER_NUM_SPIN2: pIterNum = &m_wndIterNum2; break;
    default: ASSERT(0);
    }
    pIterNum->GetEditValue(NIter);
    NIter -= pNMUpDown->iDelta;
    if(NIter < 1) NIter = 1;
    strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
    pIterNum->SetWindowText(strIter);
    *pResult = 0;
}

void CPostTensionTendonElemTranslateDlg::OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
    const MSG* pMsg = GetCurrentMessage();
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int NIter;
    CString strIter;
    CFormulaEditSpin* pIterNum;
    UINT nID = LOWORD(pMsg->wParam);
    switch(nID)
    {
    case IDC_TM_TENDON_NODE_INC_SPIN: pIterNum = &m_wndNodeInc; break;
    default: ASSERT(0);
    }
    pIterNum->GetEditValue(NIter);
    NIter -= pNMUpDown->iDelta;
    strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
    pIterNum->SetWindowText(strIter);
    *pResult = 0;
}

void CPostTensionTendonElemTranslateDlg::OnTmNodeIncRadio()
{
    if(m_nOption != 0)
    {
        m_nOption = 0;
        CtrlEnableDisable(m_NodeInc,TRUE);
        CtrlEnableDisable(m_EqualDistance,FALSE);
        CtrlEnableDisable(m_UEqualDistance,FALSE);
        if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum, TRUE);
        else CtrlEnableDisable(m_aIterNum, FALSE);
        CtrlEnableDisable(m_aIterNum2, FALSE);
    }
    GotoDlgCtrl(&m_wndNodeInc);
}

void CPostTensionTendonElemTranslateDlg::OnTmEqualDistRadio() 
{
    if(m_nOption != 1)
    {
        m_nOption = 1;
        CtrlEnableDisable(m_EqualDistance,TRUE);
        CtrlEnableDisable(m_NodeInc,FALSE);
        CtrlEnableDisable(m_UEqualDistance,FALSE);
        CtrlEnableDisable(m_aIterNum, FALSE);
        if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum2, TRUE);
        else CtrlEnableDisable(m_aIterNum2, FALSE);
    }
    GotoDlgCtrl(&m_wndEQDist);
}

void CPostTensionTendonElemTranslateDlg::OnTmUnequalDistRadio() 
{
    if(m_nOption != 2)
    {
        m_nOption = 2;
        CtrlEnableDisable(m_UEqualDistance,TRUE);
        CtrlEnableDisable(m_NodeInc,FALSE);
        CtrlEnableDisable(m_EqualDistance,FALSE);
        OnTmAxisRadios();
        CtrlEnableDisable(m_aIterNum, FALSE);
        CtrlEnableDisable(m_aIterNum2, FALSE);
    }
}

BOOL CPostTensionTendonElemTranslateDlg::GetParameter(_TendonElemTranslate& ET)
{
    UpdateData(TRUE);
    CString strDirVector;
    CString strEQDist;
    CString strUEQDist;
    CString strIterNum;
    I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
    ET.m_bIsCopy = (m_nCopyOrMove == 0);
    ET.m_nOption = m_nOption;
    if (m_nOption == 0)
    {
        if (!m_wndNodeInc.GetEditValue(ET.m_nNodeInc))
        {
            GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_increment_valu));
            return FALSE;
        }
    }
    else if (m_nOption == 1)
    {
        m_wndEQDist.GetWindowText(strEQDist);
        if(TRUE == m_wndEQDist.IsKeyPressed())
        {
            if(GetEQDistance(strEQDist,ET.m_EQ_UX, ET.m_EQ_UY, ET.m_EQ_UZ)) 
            {
                pIGM->GetWcsVector(ET.m_EQ_UX, ET.m_EQ_UY, ET.m_EQ_UZ,ET.m_EQ_WX, ET.m_EQ_WY, ET.m_EQ_WZ);
            }
            else return FALSE;
        }
        else
        {
            m_wndEQDist.GetCurMousePosDist(ET.m_EQ_WX, ET.m_EQ_WY, ET.m_EQ_WZ,
                                           ET.m_EQ_UX, ET.m_EQ_UY, ET.m_EQ_UZ);
        }
    }
    else
    {
        ET.m_nUEQ_Axis = m_nChooseAxis;
        m_wndUEQDist.GetWindowText(strUEQDist);
        if(GetUEDistance(strUEQDist,ET.m_UEQ_DistList)) ;
        else return FALSE;
        if(!ET.m_UEQ_DistList.GetSize()) return FALSE;
        ET.m_UEQ_UX  = 0.;
        ET.m_UEQ_UY  = 0.;
        ET.m_UEQ_UZ  = 0.;
        switch(m_nChooseAxis)
        {
        case 0:
            ET.m_UEQ_UX = 1.0;
            pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
                               ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
            break;
        case 1:
            ET.m_UEQ_UY = 1.0;
            pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
                               ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
            break;
        case 2:
            ET.m_UEQ_UZ = 1.0;
            pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
                               ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
            break;
        case 3:
            {
                m_wndDirVector.GetWindowText(strDirVector);
                if (TRUE == m_wndDirVector.IsKeyPressed())
                {
                    if (GetDirectionVector(strDirVector, ET.m_UEQ_UX, ET.m_UEQ_UY, ET.m_UEQ_UZ))
                    {
                        pIGM->GetWcsVector(ET.m_UEQ_UX, ET.m_UEQ_UY, ET.m_UEQ_UZ,
                            ET.m_UEQ_WX, ET.m_UEQ_WY, ET.m_UEQ_WZ);
                    }
                    else return FALSE;
                }
                else
                {
                    m_wndDirVector.GetCurMousePosDist(ET.m_UEQ_WX, ET.m_UEQ_WY, ET.m_UEQ_WZ,
                        ET.m_UEQ_UX, ET.m_UEQ_UY, ET.m_UEQ_UZ);
                }
                double VL = ET.m_UEQ_WX * ET.m_UEQ_WX + ET.m_UEQ_WY * ET.m_UEQ_WY +
                    ET.m_UEQ_WZ * ET.m_UEQ_WZ;
                if (pIGM->IsZero(VL))
                {
                    return FALSE;
                }
                VL = sqrt(VL);
                ET.m_UEQ_WX = ET.m_UEQ_WX / VL;
                ET.m_UEQ_WY = ET.m_UEQ_WY / VL;
                ET.m_UEQ_WZ = ET.m_UEQ_WZ / VL;
            }
            break;
        default:
            return FALSE;
        }
    }
    if (m_nOption == 0 || m_nOption == 1)
    {
        if(m_nCopyOrMove == 0)
        {  
            if (m_nOption == 0) m_wndIterNum.GetWindowText(strIterNum);
            else m_wndIterNum2.GetWindowText(strIterNum);
            if(GetNumberOfTimes(strIterNum,ET.m_NTimes));
            else return FALSE;
        }
    }
    else
    {
        if (m_nCopyOrMove == 0)
            ET.m_NTimes = ET.m_UEQ_DistList.GetSize();
    }

    return TRUE;
}

void CPostTensionTendonElemTranslateDlg::MoveElem(_TendonElemTranslate& ET)
{
    CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
    I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
    CArray<unsigned int, unsigned int> NKeyArray;
    if(pIGM->GetSelectedElemKeyList(NKeyArray))
    {
        if (ET.m_nOption == 0)
        {
            if (pDoc->m_pDataCtrl->TranslateElem_Move_Node(NKeyArray, ET.m_nNodeInc, ET.m_bDelFreeNode))
            {
                pDoc->m_pViewCtrl->UnselectAll(NULL);
            }
        }
        else if (ET.m_nOption == 1)
        {
            if((pDoc->m_pDataCtrl)->TranslateElem_Move_Equal(NKeyArray, ET.m_EQ_WX, 
                                                             ET.m_EQ_WY, ET.m_EQ_WZ, ET.m_bDelFreeNode))
            {
                pDoc->m_pViewCtrl->UnselectAll(NULL);
            }
        }
        else
        {
            double Dist = ET.m_UEQ_DistList[0];
            if((pDoc->m_pDataCtrl)->TranslateElem_Move_Equal(NKeyArray, ET.m_UEQ_WX*Dist,
                                                             ET.m_UEQ_WY*Dist, ET.m_UEQ_WZ*Dist, ET.m_bDelFreeNode))
            {
                pDoc->m_pViewCtrl->UnselectAll(NULL);
            }
        }
    }
    else
    {
        GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
    }
}

void CPostTensionTendonElemTranslateDlg::CopyElem(_TendonElemTranslate& ET)
{
    CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
    I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
    CArray<unsigned int, unsigned int> NKeyArray;
    if(pIGM->GetSelectedElemKeyList(NKeyArray))
    {
        if (ET.m_nOption == 0)
        {
            if (pDoc->m_pDataCtrl->TranslateElem_Copy_Node(NKeyArray, 
                                ET.m_NTimes, ET.m_nNodeInc, ET.m_bAttrNodeCopy, ET.m_bAttrElemCopy,
                                ET.m_nMatlInc, ET.m_bMatlIncRep,
                                ET.m_nSectInc, ET.m_bSectIncRep,
                                ET.m_nThikInc, ET.m_bThikIncRep,
                                ET.m_bIntstNode, ET.m_bIntstElem))
            {
                pDoc->m_pViewCtrl->UnselectAll(NULL);
            }
        }
        else if (ET.m_nOption == 1)
        {
            if((pDoc->m_pDataCtrl)->TranslateElem_Copy_Equal(NKeyArray,ET.m_NTimes,ET.m_EQ_WX, 
                                                            ET.m_EQ_WY, ET.m_EQ_WZ, ET.m_bAttrNodeCopy, ET.m_bAttrElemCopy,
                                                            ET.m_nMatlInc, ET.m_bMatlIncRep, 
                                                            ET.m_nSectInc, ET.m_bSectIncRep, 
                                                            ET.m_nThikInc, ET.m_bThikIncRep,
                                                            ET.m_bIntstNode, ET.m_bIntstElem))
            {
                pDoc->m_pViewCtrl->UnselectAll(NULL);
            }
        }
        else
        {
            if((pDoc->m_pDataCtrl)->TranslateElem_Copy_Unequal(NKeyArray,ET.m_NTimes,ET.m_UEQ_WX,
                                                            ET.m_UEQ_WY, ET.m_UEQ_WZ,ET.m_UEQ_DistList,
                                                            ET.m_bAttrNodeCopy, ET.m_bAttrElemCopy,
                                                            ET.m_nMatlInc, ET.m_bMatlIncRep, 
                                                            ET.m_nSectInc, ET.m_bSectIncRep, 
                                                            ET.m_nThikInc, ET.m_bThikIncRep,
                                                            ET.m_bIntstNode, ET.m_bIntstElem))
            {
                pDoc->m_pViewCtrl->UnselectAll(NULL);
            }
        }
    }
    else
    {
        GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
    }
}

void CPostTensionTendonElemTranslateDlg::Execute()
{
    _TendonElemTranslate ET;
    if(GetParameter(ET))
    {
        if(ET.m_bIsCopy == FALSE)
        {
            MoveElem(ET);
        }
        else
        {
            CopyElem(ET); 
        }
    }
    else
    {
        GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
    }
}

