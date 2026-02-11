// CMLoadCombDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombDefPartialFactorDlg.h"

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
// CCMLoadCombDefPartialFactorDlg dialog


CCMLoadCombDefPartialFactorDlg::CCMLoadCombDefPartialFactorDlg( CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadCombDefPartialFactorDlg::IDD, pParent)
{
	m_bSeismicEvaluation = FALSE;
}

void CCMLoadCombDefPartialFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCombDefPartialFactorDlg)
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_CASE_COMBO, m_LoadCase);
	DDX_Text(pDX, IDC_CMD_LOADCOMB_DEF_PARTFACTOR_EDIT, m_dCoef);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_PARA_CASE_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CCMLoadCombDefPartialFactorDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMLoadCombDefPartialFactorDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LOADCOMB_PARA_CASE_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_PART_ADD_BUTTON, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_PART_EDIT_BUTTON, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_PART_DEL_BUTTON, OnCmdBtnDel)
	ON_BN_CLICKED(IDOK, OnDlgOK)
	ON_BN_CLICKED(IDCANCEL, OnDlgCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMLoadCombDefPartialFactorDlg::SetDataList(const std::map<LOAD_CASE_K, double> &mapParCoef)
{
	m_mapSpecialFactor.clear();
	m_mapSpecialFactor = mapParCoef;
}

void CCMLoadCombDefPartialFactorDlg::GetDataList(std::map<LOAD_CASE_K, double> &mapParCoef)
{
	mapParCoef = m_mapSpecialFactor;
}

BOOL CCMLoadCombDefPartialFactorDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	// LoadCase ComboBox.
	m_pDoc->m_pAttrCtrl->GetStldKeyList(m_arKeyList);
	//ILoadCombDefineMgr::Instance()->GetUsedKeyList(m_bSeismicEvaluation, m_arKeyList);
	Initial_LCaseComboBox();
	
	SetListCtrlHeader();
	MakeItemEx();
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARTFACTOR_EDIT)->SetWindowText(_T("1.0"));	
	return TRUE;
}

void CCMLoadCombDefPartialFactorDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex;
	T_SPEC_COEF data;
	if (!GetSelectedItem(nIndex))
		data.Initialize();
	else
	{
		UINT key = m_ListCtrl.GetItemData(nIndex);
		data.index = nIndex;
		if (!IsLoadCaseSplc(nIndex))
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(key, StldD))
				return;
			data.strLoadCase = StldD.LoadCaseName;
			data.iLoadCaseType = D_LCOM_STATIC;
		}
		else
		{
			T_SPLC_D SplcD;
			if (!m_pDoc->m_pAttrCtrl->GetSplc(key, SplcD))
				return;
			data.strLoadCase = SplcD.LoadCaseName;
			data.iLoadCaseType = D_LCOM_SPECTRUM;
		}
		LOAD_CASE_K caseKey;
		caseKey.Key = key;
		caseKey.nLoadCaseType = data.iLoadCaseType;
		data.dCoef = m_mapSpecialFactor[caseKey];
	}

	Item2Dlg(nIndex, data);

	*pResult = 0;
}

void CCMLoadCombDefPartialFactorDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CCMLoadCombDefPartialFactorDlg::UpdateBuffer() 
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	auto itr = m_mapSpecialFactor.begin();
	T_SPEC_COEF data;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	INT_PTR i = 0;

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
				i = m_mapSpecialFactor.size()-1;
				for (itr = m_mapSpecialFactor.end(); itr != m_mapSpecialFactor.begin(); itr--)
				{
					if (itr->first.Key == Key && itr->first.nLoadCaseType == D_LCOM_STATIC)
					{
						m_ListCtrl.DeleteItem(i);
						m_mapSpecialFactor.erase(itr->first);
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
				for (itr = m_mapSpecialFactor.begin(); itr != m_mapSpecialFactor.end(); itr++)
				{
					if (itr->first.Key == Key && itr->first.nLoadCaseType == D_LCOM_STATIC)
					{
						data.index = i;
						data.iLoadCaseType = D_LCOM_SPECTRUM;
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

	// 
	T_SPLC_K spKey, spKeyBak;
	T_SPLC_D spData, spDataBak;
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SPLC_ADD):
				// do nothing
				break;
		case(UR_SPLC_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetSplc(nKey, spKey, spData);
				i = m_mapSpecialFactor.size()-1;
				for (itr = m_mapSpecialFactor.end(); itr != m_mapSpecialFactor.begin(); itr--)
				{
					if (itr->first.Key == spKey && itr->first.nLoadCaseType == D_LCOM_SPECTRUM)
					{
						m_ListCtrl.DeleteItem(i);
						m_mapSpecialFactor.erase(itr->first);
					}
					i--;
				}
				break;
		case(UR_SPLC_MFD):
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetSplc(nKey, spKey, spData);
				spKeyBak = spKey;
				spDataBak = spData;
				bMFD = TRUE;
				break;
		case(UR_SPLC_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetSplc(nKey, spKey, spData);
				i=0;
				for (itr = m_mapSpecialFactor.begin(); itr != m_mapSpecialFactor.end(); itr++)
				{
					if (itr->first.Key == spKey && itr->first.nLoadCaseType == D_LCOM_SPECTRUM)
					{
						data.index = i;
						data.iLoadCaseType = D_LCOM_SPECTRUM;
						data.strLoadCase = spData.LoadCaseName;
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

void CCMLoadCombDefPartialFactorDlg::Initial_LCaseComboBox()
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
			BOOL bExist = FALSE;
			for (int j=0; j<m_arKeyList.GetCount(); j++)
			{
				if (iKey == m_arKeyList.GetAt(j))  
				{
					bExist = TRUE;
					break;
				}
					
			}
			T_STLD_D rData;
			rData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(iKey, rData) || !bExist)
				continue;
			         
			CString strLoadType = rData.LoadCaseType;
			CString strLoadCase = rData.LoadCaseName;
			m_LoadCase.InsertString(nIdx, strLoadCase);
			T_SPEC_COEF LCType;
			LCType.index			= nIdx;
			LCType.iLoadCaseType	= D_LCOM_STATIC; 
			LCType.strLoadCase		= strLoadCase;
			m_arLCtype.Add(LCType);		
			nIdx++;
		}    
	}

	// Spectrum.
	CArray<T_SPLC_K, T_SPLC_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(KeyList);
	iCount = KeyList.GetSize();  
	if(iCount > 0)
	{
		for(int i=0; i<iCount; i++)
		{
			T_SPLC_D rData;
			rData.Initialize();
			int key = KeyList.GetAt(i);
			BOOL bExist = FALSE;
			for (int j = 0; j < m_arKeyList.GetCount(); j++)
			{
				if (key == m_arKeyList.GetAt(j))
				{
					bExist = TRUE;
					break;
				}
			}
			if (!m_pDoc->m_pAttrCtrl->GetSplc(key, rData) || !bExist)
				continue;

			CString str = rData.LoadCaseName;
			m_LoadCase.InsertString(nIdx, str);

			T_SPEC_COEF LCType;
			LCType.index	= nIdx;
			LCType.iLoadCaseType = D_LCOM_SPECTRUM; 
			LCType.strLoadCase = str;
			m_arLCtype.Add(LCType); 
			nIdx++;
		}
	}
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCase);  
}

void CCMLoadCombDefPartialFactorDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__Partial_Factor)};
	int nColWidth[] = {70, 70};
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

void CCMLoadCombDefPartialFactorDlg::OnCmdBtnAdd()
{
	T_SPEC_COEF data;
	if (!Dlg2Item(data)) return;
	LOAD_CASE_K caseKey;
	if (data.iLoadCaseType == D_LCOM_STATIC)
		caseKey.Key = m_pDoc->m_pAttrCtrl->GetStldKey(data.strLoadCase);
	else
		caseKey.Key = m_pDoc->m_pAttrCtrl->GetSplcKey(data.strLoadCase);
	caseKey.nLoadCaseType =  data.iLoadCaseType;

	CString strCase = _T("");
	for (auto itr = m_mapSpecialFactor.begin(); itr != m_mapSpecialFactor.end(); itr++)
	{
		LOAD_CASE_K key = itr->first;
		if (key.nLoadCaseType == D_LCOM_STATIC)
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(key.Key, StldD))
				return;
			strCase = StldD.LoadCaseName;		
		}
		else
		{
			T_SPLC_D SplcD;
			if (!m_pDoc->m_pAttrCtrl->GetSplc(key.Key, SplcD))
				return;
			strCase = SplcD.LoadCaseName;
		}
		if (strCase == data.strLoadCase)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
	}

	m_mapSpecialFactor[caseKey] = data.dCoef;
	INT_PTR nNumLC = m_mapSpecialFactor.size();
	m_ListCtrl.InsertItem(nNumLC-1, _T(""));
	SetItem(nNumLC-1, data);
	m_ListCtrl.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMLoadCombDefPartialFactorDlg::OnCmdBtnModify()
{
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_SPEC_COEF data;
	if (!Dlg2Item(data)) return;
	
	UINT Nkey = m_ListCtrl.GetItemData(nIndex);
	LOAD_CASE_K key;
	key.Key = Nkey;
	key.nLoadCaseType = IsLoadCaseSplc(nIndex) ? D_LCOM_SPECTRUM : D_LCOM_STATIC;
	double dValOld = m_mapSpecialFactor[key];

	if (data.dCoef == dValOld)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
		return;
	}
	
	m_mapSpecialFactor.insert(std::make_pair(key, data.dCoef));
	SetItem(nIndex, data);
	m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLoadCombDefPartialFactorDlg::OnCmdBtnDel()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	
	UINT nkey = m_ListCtrl.GetItemData(nItem);
	LOAD_CASE_K key;
	key.Key = nkey;
	if (!IsLoadCaseSplc(nItem))
		key.nLoadCaseType = D_LCOM_STATIC;
	else
		key.nLoadCaseType = D_LCOM_SPECTRUM;		
	m_mapSpecialFactor.erase(key);
	m_ListCtrl.DeleteItem(nItem);

	int i=0;
	T_SPEC_COEF data;
	for (auto itr=m_mapSpecialFactor.begin(); itr != m_mapSpecialFactor.end(); itr++)
	{
		key = itr->first;
		data.index = i;
		if (key.nLoadCaseType == D_LCOM_STATIC)
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(key.Key, StldD))
				ASSERT(0);
			data.strLoadCase = StldD.LoadCaseName;
			data.iLoadCaseType = D_LCOM_STATIC;
		}
		else
		{
			T_SPLC_D SplcD;
			if (!m_pDoc->m_pAttrCtrl->GetSplc(key.Key, SplcD))
				ASSERT(0);
			data.strLoadCase = SplcD.LoadCaseName;
			data.iLoadCaseType = D_LCOM_SPECTRUM;
		}
		data.dCoef = itr->second;

		SetItem(i, data);
		m_ListCtrl.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		i++;
	}
	// Set Focus.
	if(m_mapSpecialFactor.size() > nItem)
		m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLoadCombDefPartialFactorDlg::OnDlgOK()
{
	CDialogMove::OnOK();
}

void CCMLoadCombDefPartialFactorDlg::OnDlgCancel()
{
	CDialogMove::OnCancel();
}

BOOL CCMLoadCombDefPartialFactorDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMLoadCombDefPartialFactorDlg::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	INT_PTR nItemCount = m_mapSpecialFactor.size();
	if(nItemCount == 0) return;
	T_SPEC_COEF data;
	LOAD_CASE_K key;

	int i=0;
	for (auto itr = m_mapSpecialFactor.begin(); itr != m_mapSpecialFactor.end(); itr++)
	{
		m_ListCtrl.InsertItem(i, _T(""));
		key = itr->first;
		data.index = i;		
		if (key.nLoadCaseType == D_LCOM_STATIC)
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(key.Key, StldD))
				ASSERT(0);
			data.strLoadCase = StldD.LoadCaseName;
			data.iLoadCaseType = D_LCOM_STATIC;
		}
		else
		{
			T_SPLC_D SplcD;
			if (!m_pDoc->m_pAttrCtrl->GetSplc(key.Key, SplcD))
				ASSERT(0);
			data.strLoadCase = SplcD.LoadCaseName;
			data.iLoadCaseType = D_LCOM_SPECTRUM;
		}
		data.dCoef = itr->second;

		SetItem(i, data);
		i++;
	}
	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CCMLoadCombDefPartialFactorDlg::SetItem(int nIndex, T_SPEC_COEF &data)
{
	LVITEM lvitem;
	CString str;
	CString strCase;
	BOOL bSplc = IsLoadCaseSplc(nIndex);
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
	if (!bSplc)
	{
		T_STLD_K key = m_pDoc->m_pAttrCtrl->GetStldKey(strCase);
		m_ListCtrl.SetItemData(nIndex, key);
	}
	else
	{
		T_SPLC_K key = m_pDoc->m_pAttrCtrl->GetSplcKey(strCase);
		m_ListCtrl.SetItemData(nIndex, key);
	}
}

BOOL CCMLoadCombDefPartialFactorDlg::Dlg2Item(T_SPEC_COEF &data)
{
	UpdateData(TRUE);
	CString strCoef;
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARTFACTOR_EDIT)->GetWindowText(strCoef);
	m_dCoef = _tstof(strCoef);
	int Index = m_LoadCase.GetCurSel();
	if(Index != -1)
	{
		CString strName=_T("");
		m_LoadCase.GetLBText(Index,strName);
		m_strLCName  = strName;
		BOOL bSplc = IsLoadCaseSplc(Index);

		int nKey = -1;
		if(!bSplc )
		{
			data.index = Index;
			data.strLoadCase = strName;
			data.iLoadCaseType = D_LCOM_STATIC;	
			data.dCoef = m_dCoef;
		}
		else
		{
			data.index = Index;
			data.strLoadCase = strName;
			data.iLoadCaseType = D_LCOM_SPECTRUM;	
			data.dCoef = m_dCoef;
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}

	return TRUE;
}

BOOL CCMLoadCombDefPartialFactorDlg::Item2Dlg(int nIndex, T_SPEC_COEF &data)
{
	SetLoadCaseCombo(data);
	return TRUE;
}

BOOL CCMLoadCombDefPartialFactorDlg::CheckLoadCaseName(CString str, int& Index)
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

BOOL CCMLoadCombDefPartialFactorDlg::IsLoadCaseSplc(int iIndex)
{
	BOOL bSplc = FALSE;
	int iArrSize = m_arLCtype.GetSize();
	for(int i=0; i<iArrSize; i++)
	{
		T_SPEC_COEF LCtype = m_arLCtype.GetAt(i);
		if(iIndex==LCtype.index)
			bSplc = LCtype.iLoadCaseType==D_LCOM_SPECTRUM ? TRUE : FALSE;		
	}
	return bSplc;
}

void CCMLoadCombDefPartialFactorDlg::SetLoadCaseCombo(T_SPEC_COEF &data)
{
	if(m_mapSpecialFactor.size()!=0)
	{
		int iCbIndex = m_LoadCase.SelectString(-1,data.strLoadCase);
		m_LoadCase.SetCurSel(iCbIndex);
	}
}

