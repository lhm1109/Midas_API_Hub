// CMMvldSubItemDlg.cpp : implementation file
//
// 2003. 09. 15   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldSubItemBSDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\VehlDef.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"

#include "CMMvhlListDlg.h"
#include "CMMvldItemBSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemBSDlg dialog


CCMMvldSubItemBSDlg::CCMMvldSubItemBSDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldSubItemBSDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_nPos = 0;
	m_aSelLlan2.RemoveAll();

	ASSERT(pParent);
	m_pMvldItemDlg = (CCMMvldItemBSDlg*)pParent;

	aCtrlRailHide.RemoveAll();
	aCtrlRailHide.Add(IDC_WG_CMD_STATIC7);
	aCtrlRailHide.Add(IDC_CMD_SEL_LIST2);
	aCtrlRailHide.Add(IDC_CMD_BTN_ADD2);
	aCtrlRailHide.Add(IDC_CMD_BTN_DELETE2);

	m_aOKCancelApply.RemoveAll();
	m_aOKCancelApply.Add(IDOK);
	m_aOKCancelApply.Add(IDCANCEL);
	m_aOKCancelApply.Add(IDC_CMD_APPLY);

	m_aUnSelLane.RemoveAll();
	m_aUnSelLane.Add(IDC_WG_CMD_STATIC4);
	m_aUnSelLane.Add(IDC_CMD_UNSEL_LIST);

	m_aSelLane.RemoveAll();
	m_aSelLane.Add(IDC_WG_CMD_STATIC5);
	m_aSelLane.Add(IDC_CMD_SEL_LIST);

	m_aSelLaneAddDel.RemoveAll();
	m_aSelLaneAddDel.Add(IDC_CMD_BTN_ADD);
	m_aSelLaneAddDel.Add(IDC_CMD_BTN_DELETE);

	//{{AFX_DATA_INIT(CCMMvldSubItemBSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvldSubItemBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldSubItemBSDlg)
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST, m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_wndSelList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST2, m_wndSelList2);
	DDX_Control(pDX, IDC_CMD_LANE_NUM, m_wndLaneNum);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_VCLASS, m_wndVClass);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldSubItemBSDlg::Data2Dlg()
{
	CString csVal;
	csVal.Format(_T("%g"), m_Data.dScaleFactor);
	m_wndScaleFactor.SetWindowText(csVal);
	//int nLaneNum = m_pMvldItemDlg->GetLaneNum();
	int nLaneNum = m_Data.nNumLoadedLanes;
	m_wndLaneNum.SetValue(nLaneNum);
	//m_wndLaneNum.SetReadOnly(TRUE);    // Read only

	// Lane 갯수에 따라 다르다.
	T_MVHL_D mvhl;
	m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey, mvhl);
	m_wndVClass.SelectString(-1, mvhl.VehicleLoadName);

	// Select 된 것, 아닌 것 구분
	int nAllSize = m_aLaneList.GetSize();
	int nSelSize;
	int nMaxCount = m_Data.aSelectedLanes.GetSize();
	for (nSelSize = 0; nSelSize < nMaxCount; nSelSize++)
		if (m_Data.aSelectedLanes[nSelSize] == 0) break;

	m_aSelFlag.SetSize(nAllSize);
	for (int i = 0; i < nAllSize; i++)
	{
		int j = 0;
		for ( j = 0; j < nSelSize; j++)
			if (m_aLaneList[i] == m_Data.aSelectedLanes[j]) break;
		if (j == nSelSize) m_aSelFlag[i] = FALSE;
		else m_aSelFlag[i] = TRUE;
	}

	int nIndex;
	CString LaneName;
	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}

	T_LLAN_BS_KEY key;
	for (int i=0; i<m_Data.aTwoLanes.GetSize(); i++)
	{
		if (m_Data.aTwoLanes[i].Lane1 == 0 || m_Data.aTwoLanes[i].Lane2 == 0) break;

		key.key.key1 = m_Data.aTwoLanes[i].Lane1;
		key.key.key2 = m_Data.aTwoLanes[i].Lane2;

		LaneName = GetLaneName(key.key.key1);
		LaneName += _T(" : ");
		LaneName += GetLaneName(key.key.key2);
		nIndex = m_wndSelList2.AddString(LaneName);
		m_wndSelList2.SetItemData(nIndex, key.keymap);

		m_aSelLlan2.Add(key);
	}

	OnSelChangeVClass();
}

BOOL CCMMvldSubItemBSDlg::Dlg2Data()
{
	m_Data.Initialize();

	int nLaneNum;
	CFormulaEdit::GetEditValue(&m_wndLaneNum, m_Data.nNumLoadedLanes);
	CFormulaEdit::GetEditValue(&m_wndScaleFactor, m_Data.dScaleFactor);
	nLaneNum = m_Data.nNumLoadedLanes;

	int nCurSel = m_wndVClass.GetCurSel();
	if (nCurSel < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERR_Vehicle_is_not_selected));    // _T("Error : Vehicle is not selected!")
		return FALSE;
	}
	m_Data.VehicleKey= m_wndVClass.GetItemData(nCurSel);

	int nCount = 0;
	// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
	int nMaxLane = 20;
	int nSize = m_aLaneList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			if (nCount >= nMaxLane)
			{
				CString csMsg;
				csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
				AfxMessageBox(csMsg);
				return FALSE;
			}
			m_Data.aSelectedLanes.Add(m_aLaneList[i]);
			nCount++;
		}
	}
	if (nLaneNum != nCount)
	{
		CString strMsg;
		strMsg.Format(_LS(IDS_WG_CMD_ERR_Selected_Lane_Number), nLaneNum);
		AfxMessageBox(strMsg);
		return FALSE;
	}

	T_LLAN_BS_KEY key;
	m_Data.aTwoLanes.SetSize(m_aSelLlan2.GetSize());
	for (int i=0; i<m_aSelLlan2.GetSize(); i++)
	{
		key = m_aSelLlan2[i];
		m_Data.aTwoLanes[i].Lane1 = key.key.key1;
		m_Data.aTwoLanes[i].Lane2 = key.key.key2;
	}

	// Permit 이 선택되었을때 HB Straddling Two Lanes 을 선택할 수 없음
	T_MVHL_D MvhlD; MvhlD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey, MvhlD)) return FALSE;
	if(!MvhlD.bStandard && MvhlD.nSubType == 6) // User Defined Vehicle Permit
	{
		if(m_aSelLlan2.GetSize() > 0)
		{
			AfxMessageBox(_LS(IDS_CMD_MVHL_BS_PERMIT_SEL_LANE_ERROR));
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CCMMvldSubItemBSDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	return m_pMvldItemDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
}

void CCMMvldSubItemBSDlg::AlignControl()
{
	BOOL bNetworkRail = m_pMvldItemDlg->IsNetworkRail();
	if (!bNetworkRail) return;

	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	GetDlgItem(IDC_CMD_BTN_VCLASS)->GetWindowRect(rTo);
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	pWndLast->GetWindowRect(rMove);
	nDistX = rTo.right - rMove.right + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistX(this, m_aOKCancelApply, nDistX);

	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(rMove);
	rMove.right = rTo.right + globalUtils.ScaleByDPI(16);
	ScreenToClient(rMove);
	GetDlgItem(IDC_WG_CMD_STATIC0)->MoveWindow(rMove);

	GetDlgItem(IDC_WG_CMD_STATIC3)->GetWindowRect(rMove);
	rMove.right = rTo.right + globalUtils.ScaleByDPI(16);
	ScreenToClient(rMove);
	GetDlgItem(IDC_WG_CMD_STATIC3)->MoveWindow(rMove);


	GetDlgItem(IDC_CMD_BTN_ADD)->GetWindowRect(rMove);
	GetDlgItem(IDC_WG_CMD_STATIC3)->GetWindowRect(rTo);

	int nCentTo = rTo.left + (rTo.right - rTo.left) * 0.5;
	int nCentMove = rMove.left + (rMove.right - rMove.left) * 0.5;

	nDistX = nCentTo - nCentMove - globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistX(this, m_aUnSelLane, nDistX);

	nDistX = nCentTo - nCentMove;
	CDlgUtil::CtrlMoveDistX(this, m_aSelLaneAddDel, nDistX);

	nDistX = nCentTo - nCentMove + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistX(this, m_aSelLane, nDistX);

	// resize self size
	GetDlgItem(IDC_WG_CMD_STATIC3)->GetWindowRect(rTo);
	CRect r;
	GetWindowRect(r);
	r.right = rTo.right + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvldSubItemBSDlg::SetCtrlShowHide()
{
	BOOL bNetworkRail = m_pMvldItemDlg->IsNetworkRail();
	if (bNetworkRail)
	{
		CDlgUtil::CtrlShowHide(this, aCtrlRailHide, !bNetworkRail);
		m_wndSelList2.ResetContent();
		m_aSelLlan2.RemoveAll();
	}
}

void CCMMvldSubItemBSDlg::FillVClassList()
{
	BOOL bAutoLiveLoadComb = m_pMvldItemDlg->IsAutoLiveLoadComb();
	BOOL bNetworkRail = m_pMvldItemDlg->IsNetworkRail();
	m_wndVClass.ResetContent();

	int nIndex;
	T_MVHL_D mvhl;
	CArray<T_MVHL_K, T_MVHL_K> aKeyListMvhl;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aKeyListMvhl);
	int nSize = aKeyListMvhl.GetSize();

	for (int i = 0; i < nSize; i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(aKeyListMvhl[i], mvhl)) continue;

		if (bNetworkRail)
		{
			if (!mvhl.bStandard) continue;
			if (mvhl.nStandardCode != D_MVHL_BS_NETWORK_RAIL_STANDARDS) continue;
		}
		else
		{
			if (mvhl.bStandard && mvhl.nStandardCode == D_MVHL_BS_NETWORK_RAIL_STANDARDS) continue;
		}

		T_MVLDbs_D prev;
		if (mvhl.bStandard)
		{
			CString csTypeName = mvhl.VehicleTypeName;
			csTypeName.MakeUpper();
			if (bAutoLiveLoadComb)  // HA && HB(Auto) or Pedestrian
			{
				if (csTypeName != _T("HA & HB (BS5400)") && csTypeName != _T("HA & HB(AUTO)") && csTypeName != _T("PEDESTRIAN")) continue;
			}
			else
			{
				if (csTypeName == _T("HA & HB (BS5400)") || csTypeName == _T("HA & HB(AUTO)") || csTypeName == _T("ALL MODEL 1") || csTypeName == _T("ALL MODEL 2(UDL+KEL)")) continue;
			}

			if(csTypeName == _T("SV 80") || csTypeName == _T("SV 100") || csTypeName == _T("SV 150") || csTypeName == _T("SV 196") || csTypeName == _T("SV-Train") || csTypeName == _T("SV TT") ||
				 csTypeName == _T("SOV 250") || csTypeName == _T("SOV 350") || csTypeName == _T("SOV 450") || csTypeName == _T("SOV 600")) continue;
		}
		else  // User
		{
			if (bAutoLiveLoadComb)  // HA && HB(Auto) or Pedestrian
			{
				if (mvhl.nSubType != 0 && mvhl.nSubType != 4 && mvhl.nSubType != 5 && mvhl.nSubType != 6) continue;
				//if (mvhl.nSubType != 0 && mvhl.nSubType != 1 && mvhl.nSubType != 2) continue;
			}
			else
			{
				if (mvhl.nSubType == 0 || mvhl.nSubType == 4) continue;
				//if (mvhl.nSubType == 0 || mvhl.nSubType == 1) continue;
			}
		}
		nIndex = m_wndVClass.AddString(mvhl.VehicleLoadName);
		m_wndVClass.SetItemData(nIndex, aKeyListMvhl[i]);
	}
	m_wndVClass.SetCurSel(0);
}

CString CCMMvldSubItemBSDlg::GetLaneName(UINT nLaneKey)
{
	CString csLaneName(_T(""));

	if (m_bSurface)
	{
		T_SLAN_D data;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, data);
		csLaneName = data.LaneName;
	}
	else 
	{
		T_LLAN_D data;
		m_pDoc->m_pAttrCtrl->GetLlan(nLaneKey, data);
		csLaneName = data.LineLaneName;
	}

	return csLaneName;
}


BEGIN_MESSAGE_MAP(CCMMvldSubItemBSDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldSubItemBSDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST, OnDblclkCmdSelList)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST, OnDblclkCmdUnselList)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS, OnCmdBtnVclass)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD2, OnCmdBtnAdd2)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE2, OnCmdBtnDelete2)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS2, OnCmdBtnVclass)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST2, OnDblclkCmdSelList2)
	ON_CBN_SELCHANGE(IDC_CMD_VCLASS, OnSelChangeVClass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemBSDlg message handlers

BOOL CCMMvldSubItemBSDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dScaleFactor = 1.0;
	}

	BOOL bNetworkRail = m_pMvldItemDlg->IsNetworkRail();

	CArray<UINT, UINT> aLaneList;
	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0 || m_pDoc->m_pAttrCtrl->GetCountSlanop() > 0)
	{
		m_pDoc->m_pAttrCtrl->GetSlanKeyList(aLaneList);
		m_bSurface = TRUE;
		T_SLAN_K Key;
		T_SLAN_D Data;
		for (int i = 0; i < aLaneList.GetSize(); ++i)
		{
			Key = aLaneList[i];
			m_pDoc->m_pAttrCtrl->GetSlan(Key, Data);

			if (bNetworkRail && Data.nLaneType != 1) continue;
			if (!bNetworkRail && Data.nLaneType == 1) continue;

			m_aLaneList.Add(Key);
		}
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetLlanKeyList(aLaneList);
		m_bSurface = FALSE;
		T_LLAN_K Key;
		T_LLAN_D Data;
		for (int i = 0; i < aLaneList.GetSize(); ++i)
		{
			Key = aLaneList[i];
			m_pDoc->m_pAttrCtrl->GetLlan(Key, Data);

			if (bNetworkRail && Data.nLaneType != 1) continue;
			if (!bNetworkRail && Data.nLaneType == 1) continue;

			m_aLaneList.Add(Key);
		}
	}

	// JBSEON-20060523 신미영 대리 요청 - Default 초기갯수와 맞추기
	if (!m_bModify)
		m_Data.nNumLoadedLanes = m_aLaneList.GetSize();
		
	FillVClassList();
	AlignControl();
	SetCtrlShowHide();

	m_wndLaneNum.SetRange(1, 10);
	m_wndLaneNum.SetInteger(TRUE);

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldSubItemBSDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldSubItemBSDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndUnselList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_wndUnselList.DeleteString(aIndex[i]);
	}
	
	CString csLaneName;
	m_wndSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldSubItemBSDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
		DeleteItemFromSelList2(m_aLaneList[nIndex]);
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldSubItemBSDlg::OnDblclkCmdSelList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDelete();
}

void CCMMvldSubItemBSDlg::OnDblclkCmdUnselList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd();
}

void CCMMvldSubItemBSDlg::OnDblclkCmdSelList2() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDelete2();
}

void CCMMvldSubItemBSDlg::OnCmdBtnVclass() 
{
	// TODO: Add your control notification handler code here
	CCMMvhlListDlg dlg;
	dlg.DoModal();

	T_MVHL_K MvhlK;
	int nCurSel1 = m_wndVClass.GetCurSel();
	if (nCurSel1 >= 0) MvhlK = m_wndVClass.GetItemData(nCurSel1);

	FillVClassList();
	
	CString csName;
	if (nCurSel1 >= 0) // 예전 선택 복구
	{
		T_MVHL_D mvhl;
		if (m_pDoc->m_pAttrCtrl->GetMvhl(MvhlK, mvhl))
			csName = mvhl.VehicleLoadName;
		else csName = _T("");
	}
	else csName = _T("");
	if (csName == _T("")) m_wndVClass.SetCurSel(0);
	else m_wndVClass.SelectString(-1, csName);
	OnSelChangeVClass();
}

void CCMMvldSubItemBSDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}

void CCMMvldSubItemBSDlg::OnCmdBtnAdd2() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount != 2)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERR_Selected_Lane_Number2));
		return;
	}

	T_LLAN_BS_KEY key;
	nIndex = m_wndSelList.GetItemData(aIndex[0]);
	key.key.key1 = m_aLaneList[nIndex];
	nIndex = m_wndSelList.GetItemData(aIndex[1]);
	key.key.key2 = m_aLaneList[nIndex];

	if (IsDuplicatedItem(key)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERR_EXIST_SAME_ITEM));
		return;
	}
	if (!m_bSurface)
	{
		T_LLAN_D LlanD1, LlanD2;
		m_pDoc->m_pAttrCtrl->GetLlan(key.key.key1, LlanD1);
		m_pDoc->m_pAttrCtrl->GetLlan(key.key.key2, LlanD2);
		if (LlanD1.nLoadDistribution != 2 ||
				LlanD2.nLoadDistribution != 2 ) // Cross Beam 아닌 경우
		{
			AfxMessageBox(_LS(IDS_WG_CMD_MVLD_BS_LANE_TYPE_ERROR));
			return;
		}
	}

	CString csLaneName;
	csLaneName = GetLaneName(key.key.key1);
	csLaneName += _T(" : ");
	csLaneName += GetLaneName(key.key.key2);
	nIndex = m_wndSelList2.AddString(csLaneName);
	m_wndSelList2.SetItemData(nIndex, key.keymap);
	m_aSelLlan2.Add(key);

}

void CCMMvldSubItemBSDlg::OnCmdBtnDelete2() 
{
	// TODO: Add your control notification handler code here
	unsigned int idata;
	int nMax = m_aSelLlan2.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList2.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		idata = m_wndSelList2.GetItemData(aIndex[i]);
		if (m_wndSelList2.DeleteString(aIndex[i]) == LB_ERR) continue;
		int k = 0;
		for (k=0; k<nMax; k++)
			if (idata == m_aSelLlan2[k].keymap) break;
		if (k == nMax) { ASSERT(0); continue; }
		m_aSelLlan2.RemoveAt(k);
	}
}

BOOL CCMMvldSubItemBSDlg::IsDuplicatedItem(T_LLAN_BS_KEY& key)
{
	int nNum = m_aSelLlan2.GetSize();
	for (int i=0; i<nNum; i++)
	{
		if ((m_aSelLlan2[i].key.key1 == key.key.key1 && m_aSelLlan2[i].key.key2 == key.key.key2) ||
				(m_aSelLlan2[i].key.key1 == key.key.key2 && m_aSelLlan2[i].key.key2 == key.key.key1)) 
				return TRUE;
	}
	return FALSE;
}

void CCMMvldSubItemBSDlg::DeleteItemFromSelList2(UINT LlanK)
{
	T_LLAN_BS_KEY key; 
	int nNum = m_wndSelList2.GetCount();
	for (int i=nNum-1; i>=0; i--)
	{
		key.keymap = m_wndSelList2.GetItemData(i);
		if (key.key.key1 != LlanK && key.key.key2 != LlanK) continue;
		if (m_wndSelList2.DeleteString(i) == LB_ERR) continue;
		int k = 0;
		for (k=0; k<m_aSelLlan2.GetSize(); k++)
			if (key.keymap == m_aSelLlan2[k].keymap) break;
		if (k == m_aSelLlan2.GetSize()) { ASSERT(0); continue; }
		m_aSelLlan2.RemoveAt(k);
	}
}

void CCMMvldSubItemBSDlg::OnSelChangeVClass()
{
	BOOL bNetworkRail = m_pMvldItemDlg->IsNetworkRail();
	if (bNetworkRail)
	{
		SetCtrlShowHide();
		return;
	}

	BOOL bEnable = FALSE;
	DWORD MvhlK;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_wndVClass, MvhlK))
	{
		T_MVHL_D MvhlD;
		if (!m_pDoc->m_pAttrCtrl->GetMvhl(MvhlK, MvhlD)) { ASSERT(0); MvhlD.Initialize(); }
		if (MvhlD.bStandard)
		{
			MvhlD.VehicleTypeName.MakeUpper();
			if (MvhlD.VehicleTypeName == _T("HA & HB(AUTO)")) bEnable = TRUE;
			if (MvhlD.VehicleTypeName == _T("HA & HB (BS5400)")) bEnable = TRUE;
		}
		else
		{
			//if (MvhlD.nSubType == 0 || MvhlD.nSubType == 1) // BS5400 or HA & HB(Auto)
			if (MvhlD.nSubType == 0 || MvhlD.nSubType == 4) // BS5400 or HA & HB(Auto)
				bEnable = TRUE;
		}
	}
	
	GetDlgItem(IDC_CMD_BTN_ADD2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_BTN_DELETE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_SEL_LIST2)->EnableWindow(bEnable);
	if (!bEnable) 
	{
		m_wndSelList2.ResetContent();
		m_aSelLlan2.RemoveAll();
	}
}
