// CMMvldSubItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldSubItemCHDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMMvhcListDlg.h"
#include "CMMvldItemCHDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key  : 31;
	} typekey;
};

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemCHDlg dialog


CCMMvldSubItemCHDlg::CCMMvldSubItemCHDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldSubItemCHDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_nPos = 0;
	ASSERT(pParent);
	m_pMvldItemDlg = (CCMMvldItemCHDlg*)pParent;
	//{{AFX_DATA_INIT(CCMMvldSubItemCHDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvldSubItemCHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldSubItemCHDlg)
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST, m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_wndSelList);
	DDX_Control(pDX, IDC_CMD_MAX_NUM, m_wndMaxNum);
	DDX_Control(pDX, IDC_CMD_MIN_NUM, m_wndMinNum);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_VCLASS, m_wndVClass);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldSubItemCHDlg::Data2Dlg()
{
	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if (m_Data.nVehicleType == 1)
	{
		m_pDoc->m_pAttrCtrl->GetMvhc(m_Data.VehicleKey, mvhc);
		m_wndVClass.SelectString(-1, _T("VC:")+mvhc.VehicleClassName);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey, mvhl);
		m_wndVClass.SelectString(-1, _T("VL:")+mvhl.VehicleLoadName);
	}

	CString csVal;
	csVal.Format(_T("%g"), m_Data.dVehicleScaleFactor);
	m_wndScaleFactor.SetWindowText(csVal);

	csVal.Format(_T("%d"), m_Data.nMinLoadedLanes);
	m_wndMinNum.SetWindowText(csVal);

	csVal.Format(_T("%d"), m_Data.nMaxLoadedLanes);
	m_wndMaxNum.SetWindowText(csVal);

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
		for (j = 0; j < nSelSize; j++)
			if (m_aLaneList[i] == m_Data.aSelectedLanes[j]) break;
		if (j == nSelSize) m_aSelFlag[i] = FALSE;
		else m_aSelFlag[i] = TRUE;
	}

	int nIndex;

	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();

	// Add String
	CString LaneName;
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
}

BOOL CCMMvldSubItemCHDlg::Dlg2Data()
{
	m_Data.Initialize();
	int nCurSel = m_wndVClass.GetCurSel();
	if (nCurSel < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_Class_is_not_sele));
		return FALSE;
	}
	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_wndVClass.GetItemData(nCurSel);
	m_Data.nVehicleType = TypeKey.typekey.type + 1;
	m_Data.VehicleKey = TypeKey.typekey.key;
	CFormulaEdit::GetEditValue(&m_wndScaleFactor, m_Data.dVehicleScaleFactor);
	CFormulaEdit::GetEditValue(&m_wndMinNum, m_Data.nMinLoadedLanes);
	CFormulaEdit::GetEditValue(&m_wndMaxNum, m_Data.nMaxLoadedLanes);
	int nSize = m_aLaneList.GetSize();
	int nCount = 0;
	// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
	int nMaxLane = 20;
	for (int i=0; i<nSize; i++)
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
	return TRUE;
}

BOOL CCMMvldSubItemCHDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	return m_pMvldItemDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
}

void CCMMvldSubItemCHDlg::FillVClassList()
{
	int nIndex;
	T_MVLD_TYPE_KEY TypeKey;
	T_MVHC_D mvhc;
	CArray<T_MVHC_K, T_MVHC_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetMvhcKeyList(aKeyList);
	int nSize = aKeyList.GetSize();
	m_wndVClass.ResetContent();
	for (int i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhc(aKeyList[i], mvhc);
		nIndex = m_wndVClass.AddString(_T("VC:")+mvhc.VehicleClassName);
		TypeKey.typekey.type = 0;
		TypeKey.typekey.key = aKeyList[i];
		m_wndVClass.SetItemData(nIndex, TypeKey.value);
	}

	T_MVHL_D mvhl;
	CArray<T_MVHL_K, T_MVHL_K> aKeyListMvhl;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aKeyListMvhl);
	nSize = aKeyListMvhl.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(aKeyListMvhl[i], mvhl);
		nIndex = m_wndVClass.AddString(_T("VL:")+mvhl.VehicleLoadName);
		TypeKey.typekey.type = 1;
		TypeKey.typekey.key = aKeyListMvhl[i];
		m_wndVClass.SetItemData(nIndex, TypeKey.value);
	}

	m_wndVClass.SetCurSel(0);
}

CString CCMMvldSubItemCHDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLANch_D slan;
		m_pDoc->m_pAttrCtrl->GetSlanch(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLANch_D llan;
		m_pDoc->m_pAttrCtrl->GetLlanch(nLaneKey, llan);
		return llan.LineLaneName;
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}



BEGIN_MESSAGE_MAP(CCMMvldSubItemCHDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldSubItemCHDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST, OnDblclkCmdSelList)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST, OnDblclkCmdUnselList)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS, OnCmdBtnVclass)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemCHDlg message handlers

BOOL CCMMvldSubItemCHDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	if (m_pDoc->m_pAttrCtrl->GetCountSlanch() > 0) m_bSurface = TRUE;
	else m_bSurface = FALSE;

	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dVehicleScaleFactor = 1.0;
	}

	if (m_bSurface) m_pDoc->m_pAttrCtrl->GetSlanchKeyList(m_aLaneList);
	else m_pDoc->m_pAttrCtrl->GetLlanchKeyList(m_aLaneList);

	FillVClassList();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldSubItemCHDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldSubItemCHDlg::OnCmdBtnAdd() 
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

void CCMMvldSubItemCHDlg::OnCmdBtnDelete() 
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

void CCMMvldSubItemCHDlg::OnDblclkCmdSelList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDelete();
}

void CCMMvldSubItemCHDlg::OnDblclkCmdUnselList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd();
}

void CCMMvldSubItemCHDlg::OnCmdBtnVclass() 
{
	// TODO: Add your control notification handler code here
	CCMMvhcListDlg dlg;
	dlg.SetModeAddOnly();
	dlg.DoModal();
	T_MVLD_TYPE_KEY TypeKey;
	int nCurSel = m_wndVClass.GetCurSel();
	if (nCurSel >= 0) 
	TypeKey.value = m_wndVClass.GetItemData(nCurSel);
	FillVClassList();
	CString csName;
	if (nCurSel >= 0) // 예전 선택 복구
	{
		if (TypeKey.typekey.type == 0)  // mvhc
		{
			T_MVHC_D mvhc;
			if (m_pDoc->m_pAttrCtrl->GetMvhc(TypeKey.typekey.key, mvhc))
				csName = _T("VC:")+mvhc.VehicleClassName;
			else csName = _T("");
		}
		else
		{
			T_MVHL_D mvhl;
			if (m_pDoc->m_pAttrCtrl->GetMvhl(TypeKey.typekey.key, mvhl))
				csName = _T("VL:")+mvhl.VehicleLoadName;
			else csName = _T("");
		}
	}
	else csName = _T("");
	if (csName == _T("")) m_wndVClass.SetCurSel(0);
	else m_wndVClass.SelectString(-1, csName);
}

void CCMMvldSubItemCHDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}
