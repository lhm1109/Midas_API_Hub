// CMLoadCombDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombDefCombFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr
#include "..\wg_dbLock\WebLockUtil.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_dbLock\LockUtil.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "..\wg_base\ServiceProvider.h"
#include "..\wg_cmd2\CMD2_ServiceDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombDefCombFactorDlg dialog


CCMLoadCombDefCombFactorDlg::CCMLoadCombDefCombFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadCombDefCombFactorDlg::IDD, pParent)
{
	m_bSeismicEvaluation = FALSE;
}

void CCMLoadCombDefCombFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCombDefCombFactorDlg)
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_CASE_COMBO, m_LoadCase);
	DDX_Text(pDX, IDC_CMD_LOADCOMB_DEF_FACTOR_EDIT, m_dCoef);
	DDX_Text(pDX, IDC_CMD_LOADCOMB_DEF_GAMMAL_EDIT, m_dGammaL);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_VARIA_CASE_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CCMLoadCombDefCombFactorDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMLoadCombDefCombFactorDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LOADCOMB_VARIA_CASE_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_VARIA_ADD_BUTTON, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_VARIA_EDIT_BUTTON, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_VARIA_DEL_BUTTON, OnCmdBtnDel)
	ON_BN_CLICKED(IDOK, OnDlgOK)
	ON_BN_CLICKED(IDCANCEL, OnDlgCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMLoadCombDefCombFactorDlg::SetDataList(const std::map<LOAD_CASE_K, double> &mapCombFactor, double dGammaL)
{
	m_mapCombFactor.clear();
	m_mapCombFactor = mapCombFactor;
	m_dGammaL = dGammaL;
}

void CCMLoadCombDefCombFactorDlg::GetDataList(std::map<LOAD_CASE_K, double> &mapCombFactor, double &dGammaL)
{
	mapCombFactor = m_mapCombFactor;
	dGammaL = m_dGammaL;
}

BOOL CCMLoadCombDefCombFactorDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	// LoadCase ComboBox.
	m_pDoc->m_pAttrCtrl->GetStldKeyList(m_arKeyList);
	//ILoadCombDefineMgr::Instance()->GetUsedKeyList(m_bSeismicEvaluation, m_arKeyList);
	Initial_LCaseComboBox();
	SetListCtrlHeader();
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_FACTOR_EDIT)->SetWindowText(_T("1.0"));
	CString str;
	str.Format(_T("%.2f"), m_dGammaL);
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_GAMMAL_EDIT)->SetWindowText(str);
	MakeItemEx();
	return TRUE;
}

void CCMLoadCombDefCombFactorDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex;
	T_COMB_COEF data;
	if (!GetSelectedItem(nIndex))
		data.Initialize();
	else
	{
		T_STLD_K StldK = m_ListCtrl.GetItemData(nIndex);
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD))
			return;
		LOAD_CASE_K key;
		key.Key = StldK;
		key.nLoadCaseType = D_LCOM_STATIC;

		data.index = nIndex;
		data.strLoadCase = StldD.LoadCaseName;
		data.dCoef = m_mapCombFactor[key];
	}

	Item2Dlg(nIndex, data);

	*pResult = 0;
}

void CCMLoadCombDefCombFactorDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLoadCombDefCombFactorDlg::UpdateBuffer() 
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	auto itr = m_mapCombFactor.begin();
	T_COMB_COEF data;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	INT_PTR i;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_STLD_ADD):
			// do nothing
			break;
		case(UR_STLD_DEL):
			//
			pViewBuff->GetStld(nKey, Key, Data);
			i = m_mapCombFactor.size()-1;
			for (itr = m_mapCombFactor.end(); itr != m_mapCombFactor.begin(); itr--)
			{
				if (itr->first.Key == Key)
				{
					m_ListCtrl.DeleteItem(i);
					m_mapCombFactor.erase(itr->first);
				}
				i--;
			}
			break;
		case(UR_STLD_MFD):
			// 
			pViewBuff->GetStld(nKey, Key, Data);
			KeyBak = Key;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_STLD_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetStld(nKey, Key, Data);
			i=0;
			for (itr = m_mapCombFactor.begin(); itr != m_mapCombFactor.end(); itr++)
			{
				if (itr->first.Key == Key)
				{
					data.index = i;
					data.strLoadCase = Data.LoadCaseName;
					data.dCoef = itr->second;
					SetItem(i, data);
				}
				i++;
			}
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

void CCMLoadCombDefCombFactorDlg::Initial_LCaseComboBox()
{
	m_LoadCase.ResetContent();
	m_arLCtype.RemoveAll();

	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

	int nIdx = 0;
	if(iCount>0)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bExist = FALSE;
			for (int j = 0; j < m_arKeyList.GetCount(); j++)
			{
				if (iKey == m_arKeyList.GetAt(j))   bExist = TRUE;
			}
			if (!m_pDoc->m_pAttrCtrl->GetStld(iKey, rData) ||!bExist)
				continue;            

			CString strLoadType = rData.LoadCaseType;
			CString strLoadCase = rData.LoadCaseName;
			if (strLoadType==_T("L") || strLoadType==_T("LR") || strLoadType==_T("W")
				|| strLoadType==_T("S") || strLoadType==_T("T") || strLoadType==_T("WP")
				|| strLoadType==_T("FP") || strLoadType==_T("SF") || strLoadType==_T("B"))
			{
				m_LoadCase.InsertString(nIdx, strLoadCase);
				T_COMB_COEF LCType;
				LCType.index		= m_LoadCase.GetCount()-1;
				LCType.strLoadCase	= strLoadCase;
				m_arLCtype.Add(LCType);
				nIdx++;
			}
			else
				continue;
		}    
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCase);  
}

void CCMLoadCombDefCombFactorDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Variable_Load_Case), _LS(IDS_WG_CMD__ADDD__Factor)};
	int nColWidth[] = {90, 60};
	int nColNum = 2;
	CString title;
	int i;

	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
		m_ListCtrl.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMLoadCombDefCombFactorDlg::OnCmdBtnAdd()
{
	T_COMB_COEF data;

	if (!Dlg2Item(data)) return;

	T_STLD_K stldK = m_pDoc->m_pAttrCtrl->GetStldKey(data.strLoadCase);
	LOAD_CASE_K key;
	key.Key = stldK;
	key.nLoadCaseType = D_LCOM_STATIC;

	for (auto itr = m_mapCombFactor.begin(); itr != m_mapCombFactor.end(); itr++)
	{
		LOAD_CASE_K key = itr->first;

		if(key.Key == stldK)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
	}

	m_mapCombFactor[key] = data.dCoef;
	INT_PTR nNumLC = m_mapCombFactor.size();
	m_ListCtrl.InsertItem(nNumLC-1, _T(""));
	SetItem(nNumLC-1, data);
	m_ListCtrl.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMLoadCombDefCombFactorDlg::OnCmdBtnModify()
{
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_COMB_COEF data;
	if (!Dlg2Item(data)) return;

	T_STLD_K stldK = m_ListCtrl.GetItemData(nIndex);
	LOAD_CASE_K key;
	key.Key = stldK;
	key.nLoadCaseType = D_LCOM_STATIC;
	double dValOld = m_mapCombFactor[key];

	if (data.dCoef == dValOld)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
		return;
	}

	m_mapCombFactor.insert(std::make_pair(key, data.dCoef));
	SetItem(nIndex, data);
	m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLoadCombDefCombFactorDlg::OnCmdBtnDel()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	
	T_STLD_K stldK = m_ListCtrl.GetItemData(nItem);
	LOAD_CASE_K key;
	key.Key = stldK;
	key.nLoadCaseType = D_LCOM_STATIC;
	m_mapCombFactor.erase(key);
	m_ListCtrl.DeleteItem(nItem);

	int i=0;
	T_COMB_COEF data;
	for (auto itr=m_mapCombFactor.begin(); itr != m_mapCombFactor.end(); itr++)
	{
		key = itr->first;
		data.index = i;
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(key.Key, StldD))
			ASSERT(0);
		data.strLoadCase = StldD.LoadCaseName;
		data.dCoef = itr->second;

		SetItem(i, data);
		m_ListCtrl.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		i++;
	}

	// Set Focus.
	if(m_mapCombFactor.size() > nItem)
		m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLoadCombDefCombFactorDlg::OnDlgOK()
{
	CDialogMove::OnOK();
}

void CCMLoadCombDefCombFactorDlg::OnDlgCancel()
{
	CDialogMove::OnCancel();
}

BOOL CCMLoadCombDefCombFactorDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMLoadCombDefCombFactorDlg::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	INT_PTR nItemCount = m_mapCombFactor.size();
	if(nItemCount == 0) return;

	T_COMB_COEF data;
	LOAD_CASE_K key;

	int i=0;
	for (auto itr = m_mapCombFactor.begin(); itr != m_mapCombFactor.end(); itr++)
	{
		m_ListCtrl.InsertItem(i, _T(""));
		key = itr->first;
		data.index = i;		
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(key.Key, StldD))
			ASSERT(0);
		data.strLoadCase = StldD.LoadCaseName;
		data.dCoef = itr->second;

		SetItem(i, data);
		i++;
	}

	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CCMLoadCombDefCombFactorDlg::SetItem(int nIndex, T_COMB_COEF &data)
{
	LVITEM lvitem;
	CString str;
	CString strCase;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		if(i==0) str = data.strLoadCase;
		else	 str.Format(_T("%.2f"),data.dCoef);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ListCtrl.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	strCase = data.strLoadCase;
	T_STLD_K key = m_pDoc->m_pAttrCtrl->GetStldKey(strCase);
	m_ListCtrl.SetItemData(nIndex, key);
}

BOOL CCMLoadCombDefCombFactorDlg::Dlg2Item(T_COMB_COEF &data)
{
	UpdateData(TRUE);
	int Index = m_LoadCase.GetCurSel();
	CString strVal;
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_FACTOR_EDIT)->GetWindowText(strVal);
	m_dCoef = _tstof(strVal);
	
	if(Index != -1)
	{
		CString strName=_T("");
		m_LoadCase.GetLBText(Index,strName);
		m_strLCName  = strName;

		//int nKey = m_pDoc->m_pAttrCtrl->GetStldKey(strName);

		data.strLoadCase = strName;
		data.dCoef = m_dCoef;
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}
	UpdateData(FALSE);
	return TRUE;
}

BOOL CCMLoadCombDefCombFactorDlg::Item2Dlg(int nIndex, T_COMB_COEF &data)
{
	SetLoadCaseCombo(data);
	return TRUE;
}

BOOL CCMLoadCombDefCombFactorDlg::CheckLoadCaseName(CString str, int& Index)
{
	BOOL bCheck=TRUE;
	int Count = m_ListCtrl.GetItemCount();
	for(int i=0; i<Count; i++)
	{
		CString strLcName = m_ListCtrl.GetItemText(i,0);
		if(str == strLcName)  
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

void CCMLoadCombDefCombFactorDlg::SetLoadCaseCombo(T_COMB_COEF &data)
{
	if(m_mapCombFactor.size()!=0)
	{           
		CString strLoadCaseName = data.strLoadCase;
		int iCbIndex = m_LoadCase.SelectString(-1,strLoadCaseName);
		m_LoadCase.SetCurSel(iCbIndex);
	}
}
