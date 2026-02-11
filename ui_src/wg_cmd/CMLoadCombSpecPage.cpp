
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombSpecPage.h"
#include "LoadCombDefineData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\DlgTabCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombSpcePage dialog


CCMLoadCombSpecPage::CCMLoadCombSpecPage(CLoadCombDefineData *pData, CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMLoadCombSpecPage::IDD, pParent)
{
	m_pData = pData;
	m_pParent = (CCMLoadCombDefDlg*) pParent;
	m_arKeyList.RemoveAll();
	m_bSeismicEvaluation = FALSE;
}

void CCMLoadCombSpecPage::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCombSpcePage)
	DDX_Check(pDX, IDC_CMD_LOADCOMB_DEF_SPEC_L_UN_CHECK, m_bAdverse);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_PARA_CASE_LIST, m_ListLCase);
	DDX_Check(pDX, IDC_CMD_LOADCOMB_DEF_SPEC_MUTEX_CASE_CHECK, m_bMutex);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_SPEC_CASE_COMBO, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_PARA_CASE_LIST2, m_ListMutexCase);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_SPEC_MUTEX_CASE_COMBO, m_cmbMutexCase);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_SPEC_LCASE_COMBO, m_cmbLCase);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLoadCombSpecPage, CChildDialog)
	//{{AFX_MSG_MAP(CCMLoadCombSpcePage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LOADCOMB_PARA_CASE_LIST, OnChangedCurrentItem1)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_SPEC_L_UN_CHECK,			OnChkAdverseCase)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_SPCE_LCASE_ADD_BUTTON,		OnAdverseAddBtn)	
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_SPEC_LCASE_DEL_BUTTON,		OnAdverseDelBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LOADCOMB_PARA_CASE_LIST2,OnChangedCurrentItem2)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_SPEC_MUTEX_CASE_CHECK,	OnChkMutexCase)
	ON_CBN_SELCHANGE(IDC_CMD_LOADCOMB_DEF_SPEC_CASE_COMBO,		OnChangeSelMutexCase)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_SPCE_MUTEXCASE_ADD_BUTTON,	OnMutexAddBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_SPEC_MUTEXCASE_EDIT_BUTTON,	OnMutexEditBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_SPEC_MUTEXCASE_DEL_BUTTON,	OnMutexDelBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDlgTabCtrl::WM_CDLGTABCTRL_ACTIVE_TAB, OpenNewTab)
	ON_REGISTERED_MESSAGE(CDlgTabCtrl::WM_CDLGTABCTRL_INACTIVE_TAB, CloseCurTab)
END_MESSAGE_MAP()

BOOL CCMLoadCombSpecPage::OnInitDialog()
{
	CChildDialog::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	m_aCtrlAdverse.RemoveAll();
	m_aCtrlAdverse.Add(IDC_CMD_LOADCOMB_SPEC_LCASE_COMBO);
	m_aCtrlAdverse.Add(IDC_CMD_LOADCOMB_PARA_CASE_LIST);
	m_aCtrlAdverse.Add(IDC_CMD_LOADCOMB_SPCE_LCASE_ADD_BUTTON);
	m_aCtrlAdverse.Add(IDC_CMD_LOADCOMB_SPEC_LCASE_DEL_BUTTON);
	m_aCtrlMutex.RemoveAll();
	m_aCtrlMutex.Add(IDC_CMD_LOADCOMB_DEF_SPEC_CASE_COMBO);
	m_aCtrlMutex.Add(IDC_CMD_LOADCOMB_DEF_SPEC_MUTEX_CASE_COMBO);
	m_aCtrlMutex.Add(IDC_CMD_LOADCOMB_PARA_CASE_LIST2);
	m_aCtrlMutex.Add(IDC_CMD_LOADCOMB_SPCE_MUTEXCASE_ADD_BUTTON);
	m_aCtrlMutex.Add(IDC_CMD_LOADCOMB_SPEC_MUTEXCASE_EDIT_BUTTON);
	m_aCtrlMutex.Add(IDC_CMD_LOADCOMB_SPEC_MUTEXCASE_DEL_BUTTON);

	SetListCtrlHeader();
	m_pDoc->m_pAttrCtrl->GetStldKeyList(m_arKeyList);
	//ILoadCombDefineMgr::Instance()->GetUsedKeyList(m_bSeismicEvaluation, m_arKeyList);
	
	Data2Dlg();
	
	return TRUE;
}

BOOL CCMLoadCombSpecPage::Data2Dlg()
{
	Initial_LCaseComboBox();
	Initial_CaseComboBox();
	Initial_MutexCaseComboBox();
	if (m_pData->m_nSpecitlArrange == 0)
	{
		m_bAdverse = FALSE;
		m_bMutex = FALSE;
	}
	else if (m_pData->m_nSpecitlArrange == 1)
	{
		m_bAdverse = TRUE;
		m_bMutex = FALSE;
	}
	else if (m_pData->m_nSpecitlArrange == 2)
	{
		m_bAdverse = FALSE;
		m_bMutex = TRUE;
	}
	CheckDlgButton(IDC_CMD_LOADCOMB_DEF_SPEC_L_UN_CHECK,m_bAdverse);
	CheckDlgButton(IDC_CMD_LOADCOMB_DEF_SPEC_MUTEX_CASE_CHECK, m_bMutex);
	OnChkAdverseCase();
	OnChkMutexCase();

	MakeItemEx();
	return TRUE;
}

BOOL CCMLoadCombSpecPage::Dlg2Data()
{
	if (!m_bAdverse && !m_bMutex)
		m_nSpecitlArrange = 0;
	else if (m_bAdverse && !m_bMutex)
		m_nSpecitlArrange = 1;
	else if (!m_bAdverse && m_bMutex)
		m_nSpecitlArrange = 2;
	m_pData->m_nSpecitlArrange = m_nSpecitlArrange;

	return TRUE;
}

void CCMLoadCombSpecPage::Initial_LCaseComboBox()
{
	m_cmbLCase.ResetContent();
	
	CArray<T_STLD_K,T_STLD_K> arStldKey; 
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);
	CStringArray LcomType;
	T_STLD_D StldD;
	for (int i=0; i<arStldKey.GetCount(); i++)
	{
		StldD.Initialize();
		BOOL bExist = FALSE;
		for (int j = 0; j < m_arKeyList.GetSize(); j++)
		{
			if (arStldKey[i] == m_arKeyList.GetAt(j))   bExist = TRUE;
		}
		if (!m_pDoc->m_pAttrCtrl->GetStld(arStldKey[i], StldD) || !bExist)
			continue;
		if (StldD.LoadCaseType == _T("L") || StldD.LoadCaseType == _T("LR") || StldD.LoadCaseType == _T("W")
			|| StldD.LoadCaseType == _T("S") || StldD.LoadCaseType == _T("T") || StldD.LoadCaseType == _T("WP")
			|| StldD.LoadCaseType == _T("FP") || StldD.LoadCaseType == _T("SF") || StldD.LoadCaseType == _T("B"))
			LcomType.Add(StldD.LoadCaseName);
		else
			continue;
	}

	for (int i = 0; i < LcomType.GetCount(); i++)
	{
		int iIndex = m_cmbLCase.AddString(LcomType[i]);
		m_cmbLCase.SetItemData(iIndex, i);
	}

	if (m_cmbLCase.GetCount() < 1)
		return;
	m_cmbLCase.SetCheck(0);
}

void CCMLoadCombSpecPage::Initial_CaseComboBox()
{
	m_cmbLoadCase.ResetContent();
	CArray<T_STLD_K,T_STLD_K> arStldKey; 
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	T_STLD_D StldD;
	
	for (int i=0; i<arStldKey.GetCount(); i++)
	{
		StldD.Initialize();
		BOOL bExist = FALSE;
		for (int j = 0; j < m_arKeyList.GetSize(); j++)
		{
			if (arStldKey[i] == m_arKeyList.GetAt(j))   bExist = TRUE;
		}
		if (!m_pDoc->m_pAttrCtrl->GetStld(arStldKey[i], StldD) || !bExist)
			continue;
		if (StldD.LoadCaseType == _T("L") || StldD.LoadCaseType == _T("LR") || StldD.LoadCaseType == _T("W")
			|| StldD.LoadCaseType == _T("S") ||  StldD.LoadCaseType == _T("T") || StldD.LoadCaseType == _T("WP")
			|| StldD.LoadCaseType == _T("FP") ||  StldD.LoadCaseType == _T("SF") || StldD.LoadCaseType == _T("B"))
			m_cmbLoadCase.AddString(StldD.LoadCaseName);
		else
			continue;
	}
	m_cmbLoadCase.SetCurSel(0);
}

void CCMLoadCombSpecPage::Initial_MutexCaseComboBox()
{
	m_cmbMutexCase.ResetContent();
	CArray<T_STLD_K,T_STLD_K> arStldKey; 
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);
	CStringArray LcomType;
	CString strCaseName = _T("");
	m_cmbLoadCase.GetWindowText(strCaseName);
	T_STLD_D StldD;
	
	for (int i=0; i<arStldKey.GetCount(); i++)
	{
		StldD.Initialize();
		BOOL bExist = FALSE;
		for (int j = 0; j < m_arKeyList.GetSize(); j++)
		{
			if (arStldKey[i] == m_arKeyList.GetAt(j))   bExist = TRUE;
		}
		if (!m_pDoc->m_pAttrCtrl->GetStld(arStldKey[i], StldD) || !bExist)
			continue;
		if (StldD.LoadCaseType == _T("L") || StldD.LoadCaseType == _T("LR") || StldD.LoadCaseType == _T("W")
			|| StldD.LoadCaseType == _T("S") ||  StldD.LoadCaseType == _T("T") || StldD.LoadCaseType == _T("WP")
			|| StldD.LoadCaseType == _T("FP") ||  StldD.LoadCaseType == _T("SF") || StldD.LoadCaseType == _T("B"))
		{
			if (strCaseName == StldD.LoadCaseName)
				continue;
			LcomType.Add(StldD.LoadCaseName);
		}
		else
			continue;
	}

	for (int i = 0; i < LcomType.GetCount(); i++)
	{
		int iIndex = m_cmbMutexCase.AddString(LcomType[i]);
		m_cmbMutexCase.SetItemData(iIndex, i);
	}

	if (m_cmbMutexCase.GetCount() < 1)
		return;
	m_cmbMutexCase.SetCheck(0);
}

void CCMLoadCombSpecPage::SetListCtrlHeader()
{
	CString aTitle1[] = {_LS(IDS_WG_CMD__ADDD__Key), _LS(IDS_CMD_UN_LIVE_LOAD_CASE)};
	CString aTitle2[] = {_LS(IDS_CMD_LOAD_CASE), _LS(IDS_CMD_MUTEX_LOAD_CASE)};
	int nColWidth[] = {60, 100};
	int nColNum = 2;
	CString title1, title2;
	int i;

	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListLCase.GetSafeHwnd());
	dwStyle = ListView_GetExtendedListViewStyle(m_ListMutexCase.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListLCase.GetSafeHwnd(), dwStyle);
	ListView_SetExtendedListViewStyle(m_ListMutexCase.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title1 = aTitle1[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title1.GetBuffer(0);
		m_ListLCase.InsertColumn(i,&lvcolumn);
		m_ListLCase.SetColumn(i, &lvcolumn);
		title1.ReleaseBuffer();
	}

	for(i = 0; i < nColNum; i++)
	{
		title2 = aTitle2[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title2.GetBuffer(0);
		m_ListMutexCase.InsertColumn(i,&lvcolumn);
		m_ListMutexCase.SetColumn(i, &lvcolumn);
		title2.ReleaseBuffer();
	}
}

void CCMLoadCombSpecPage::MakeItemEx()
{
	//
	m_ListLCase.DeleteAllItems();
	m_ListMutexCase.DeleteAllItems();
	INT_PTR nItemCount1 = m_pData->m_aLLUnfavorable.GetCount();
	INT_PTR nItemCount2 = m_pData->m_mapExclusion.size();
	if(nItemCount1 == 0 && nItemCount2 == 0) return;

	for (int nCount = 0; nCount < nItemCount1; nCount++)
	{
		m_ListLCase.InsertItem(nCount, _T(""));

		T_ADVE_CASE data;
		LOAD_CASE_K_ARR arLiveCase = m_pData->m_aLLUnfavorable[nCount];
		data.nIndex = nCount;
		CString strBlank = _T(",");
		for (int i=0; i<arLiveCase.GetCount(); i++)
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(arLiveCase[i].Key, StldD))
				return;
			data.strLiveLoad += StldD.LoadCaseName + strBlank;
			data.vAdverKey.push_back(arLiveCase[i].Key);
		}

		SetItem(nCount, data);
	}
	m_ListLCase.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  

	//
	int nIdx = 0;
	for (auto itr = m_pData->m_mapExclusion.begin(); itr != m_pData->m_mapExclusion.end(); itr++)
	{
		m_ListMutexCase.InsertItem(nIdx, _T(""));

		T_MUTX_CASE data;
		T_STLD_K StldK = itr->first.Key;
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD))
			return;
		data.nIndex = nIdx;
		data.strLiveCase = StldD.LoadCaseName;

		LOAD_CASE_K liveK;
		liveK.Key = StldK;
		liveK.nLoadCaseType = D_LCOM_STATIC;
		LOAD_CASE_K_ARR arKey = m_pData->m_mapExclusion[liveK];
		CString strBlank = _T(",");
		for (int i=0; i<arKey.GetCount(); i++)
		{
			StldD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(arKey[i].Key, StldD))
				return;
			data.strMutexLoad += StldD.LoadCaseName+strBlank;
			data.aStldKey.Add(arKey[i].Key);
		}

		SetItem(nIdx, data);
		nIdx++;
	}
	m_ListMutexCase.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CCMLoadCombSpecPage::SetItem(int nIndex, T_ADVE_CASE &data)
{
	LVITEM lvitem;
	CString str;
	CString strIndex;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		if(i==0) str.Format(_T("%d"), nIndex+1);
		else	 str = data.strLiveLoad;
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ListLCase.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CCMLoadCombSpecPage::Dlg2Item(CArrayEx<T_ADVE_CASE, T_ADVE_CASE&> &arAdveCase)
{
	int nLCount = m_cmbLCase.GetCount();
	T_KEY_LIST lstKey;
	CString strTemp = _T("");
	T_STLD_K StldK;
	CString strBlank = _T(",");
	T_ADVE_CASE data;

	for (int i = 0; i < nLCount; i++)
	{
		if (m_cmbLCase.GetCheck(i))
		{
			m_cmbLCase.GetLBText(i, strTemp);
			StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strTemp);
			lstKey.Add(StldK);
		}
		else
			continue;
	}

	CArrayEx<T_ADVE_COMB, T_ADVE_COMB&> arCaseList;
	int nCount = combination(lstKey, arCaseList);

	for (int i=0; i< arCaseList.GetCount(); i++)
	{
		T_ADVE_COMB combD;
		combD = arCaseList[i];
		data.Initialize();
		CString strCaseList = _T("");
		for (int k=0; k< combD.aStldComb.GetCount(); k++)
		{
			T_STLD_D stldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(combD.aStldComb[k], stldD))
				ASSERT(FALSE);
			strTemp = stldD.LoadCaseName;
			data.vAdverKey.push_back(combD.aStldComb[k]);
			strCaseList += strTemp+strBlank;
		}
		data.strLiveLoad = strCaseList;
		arAdveCase.Add(data);
	
	}
	
	return TRUE;
}

BOOL CCMLoadCombSpecPage::IsCombinationEqual(const LOAD_CASE_K_ARR &arKey1, const LOAD_CASE_K_ARR &arKey2)
{
	int nCount = arKey1.GetCount();
	if (arKey2.GetCount() != nCount)
		return FALSE;

	for (int i=0; i<nCount; i++)
	{
		if (arKey1[i].Key == arKey2[i].Key && arKey1[i].nLoadCaseType == arKey2[i].nLoadCaseType)
			continue;
		else
			return FALSE;
	}
	return TRUE;
}

int CCMLoadCombSpecPage::combination(const T_KEY_LIST &lstKey, CArrayEx<T_ADVE_COMB, T_ADVE_COMB&> &arLstComb)
{
	if (lstKey.GetCount() < 1)
		return 0;

	int nItemCount = lstKey.GetCount();
	int* KeyBuf = new int[nItemCount];
	qsort(KeyBuf, nItemCount, sizeof(T_KEY), CCompFunc::UINTAsc);

	T_ADVE_COMB tempCombD;
	for (int i=0; i<lstKey.GetCount(); i++)
	{
		tempCombD.Initialize();
		tempCombD.aStldComb.Add(lstKey[i]);
		arLstComb.Add(tempCombD);
		combination_op(lstKey, tempCombD, arLstComb);
	}

	return arLstComb.GetCount();
}

void CCMLoadCombSpecPage::combination_op(const T_KEY_LIST &lstKey, const T_ADVE_COMB &combOut, CArrayEx<T_ADVE_COMB, T_ADVE_COMB&> &arLstComb)
{
	if (combOut.aStldComb.GetCount() == lstKey.GetCount())
		return;

	for (int i = 0; i < lstKey.GetCount(); i++)
	{	
		if (combOut.aStldComb[combOut.aStldComb.GetCount()-1] >= lstKey[i])
			continue;

		T_ADVE_COMB tempCombD;
		tempCombD = combOut;
		tempCombD.aStldComb.Add(lstKey[i]);
		arLstComb.Add(tempCombD);
		combination_op(lstKey, tempCombD, arLstComb);
	}
	
}

BOOL CCMLoadCombSpecPage::Item2Dlg(int nIndex, T_ADVE_CASE &data)
{
	return TRUE;
}

void CCMLoadCombSpecPage::SetItem(int nIndex, T_MUTX_CASE &data)
{
	LVITEM lvitem;
	CString str;
	CString strIndex;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		if(i==0) str = data.strLiveCase;
		else	 str = data.strMutexLoad;
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ListMutexCase.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(data.strLiveCase);
	m_ListMutexCase.SetItemData(nIndex, StldK);
}

BOOL CCMLoadCombSpecPage::Dlg2Item(T_MUTX_CASE &data)
{
	int nLCount = m_cmbMutexCase.GetCount();
	CString strCaseList = _T("");
	CString strBland = _T(",");
	T_STLD_K StldK;
	for (int i=0; i<nLCount; i++)
	{
		if (m_cmbMutexCase.GetCheck(i))
		{
			CString strTemp = _T("");
			m_cmbMutexCase.GetLBText(i, strTemp);
			strCaseList += strTemp+strBland;
			StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strTemp);
			data.aStldKey.Add(StldK);			
		}
		else
			continue;
	}
	int nIndex = m_cmbLoadCase.GetCurSel();
	m_cmbLoadCase.GetLBText(nIndex, data.strLiveCase);
	data.strMutexLoad = strCaseList;

	return TRUE;
}
BOOL CCMLoadCombSpecPage::Item2Dlg(int nIndex, T_MUTX_CASE &data)
{
	if (m_pData->m_mapExclusion.size() != 0)
	{
		CString strCaseName = _T("");
		T_STLD_D StldD;
		CString strMutexName = _T("");
		for (int i=0; i<data.aStldKey.GetCount(); i++)
		{
			StldD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(data.aStldKey[i],StldD))
				ASSERT(FALSE);
			strCaseName = StldD.LoadCaseName;
			for (int k=0; k<m_cmbMutexCase.GetCount(); k++)
			{
				m_cmbMutexCase.GetLBText(k, strMutexName);
				/*if (strCaseName == strMutexName)
				m_cmbMutexCase.SetCheck(k,TRUE);*/
			}			
		}		
	}
	return TRUE;
}

void CCMLoadCombSpecPage::OnChkAdverseCase()
{
	UpdateData(TRUE);
	m_bAdverse = IsDlgButtonChecked(IDC_CMD_LOADCOMB_DEF_SPEC_L_UN_CHECK);
	if (m_bAdverse)
	{
		m_bMutex = FALSE;
		for(int i=0; i<m_aCtrlMutex.GetSize(); i++)
			GetDlgItem(m_aCtrlMutex[i])->EnableWindow(FALSE);
		for(int i=0; i<m_aCtrlAdverse.GetSize(); i++)
			GetDlgItem(m_aCtrlAdverse[i])->EnableWindow(TRUE);
	}
	else
	{
		for(int i=0; i<m_aCtrlAdverse.GetSize(); i++)
			GetDlgItem(m_aCtrlAdverse[i])->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CCMLoadCombSpecPage::OnAdverseAddBtn()
{
	CArrayEx<T_ADVE_CASE, T_ADVE_CASE&>  arAdvCase;
	if (!Dlg2Item(arAdvCase)) return;

	T_ADVE_CASE data;
	
	for (int k=0; k< arAdvCase.GetCount(); k++)
	{
		data.Initialize();
		data = arAdvCase[k];
		BOOL bRepeat = FALSE;
		LOAD_CASE_K_ARR arKeyNew, tempArKey;
		int iIdx = 0;
		for (auto itr = data.vAdverKey.begin(); itr != data.vAdverKey.end(); itr++)
		{
			LOAD_CASE_K key;
			key.Key = data.vAdverKey[iIdx];
			key.nLoadCaseType = D_LCOM_STATIC;
			arKeyNew.Add(key);
			iIdx++;
		}
		
		for (int i = 0; i < m_pData->m_aLLUnfavorable.GetSize(); i++)
		{
			tempArKey.RemoveAll();
			tempArKey = m_pData->m_aLLUnfavorable[i];
			if (IsCombinationEqual(tempArKey, arKeyNew))
				bRepeat = TRUE;
		}

		if (!bRepeat)
		{
			m_pData->m_aLLUnfavorable.Add(arKeyNew);
			int nNumLC = m_pData->m_aLLUnfavorable.GetSize();
			m_ListLCase.InsertItem(nNumLC - 1, _T(""));
			SetItem(nNumLC - 1, data);
			m_ListLCase.SetItemState(nNumLC - 1, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	
}

void CCMLoadCombSpecPage::OnAdverseDelBtn()
{
	int nItem = m_ListLCase.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	m_ListLCase.DeleteItem(nItem);
	m_pData->m_aLLUnfavorable.RemoveAt(nItem);
	CString strBlank = _T(",");
	for(int i=0; i<m_pData->m_aLLUnfavorable.GetSize(); i++)
	{
		LOAD_CASE_K_ARR arCase;
		arCase = m_pData->m_aLLUnfavorable.GetAt(i);

		T_ADVE_CASE data;
		data.nIndex = i;
		for (int k=0; k<arCase.GetCount(); k++)
		{
			T_STLD_K StldK = arCase[k].Key;
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD))
				return;
			data.strLiveLoad += StldD.LoadCaseName + strBlank;
			data.vAdverKey.push_back(StldK);
		}
		
		SetItem(i, data);
		m_ListLCase.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
	// Set Focus.
	if(m_pData->m_aLLUnfavorable.GetSize() > nItem)
		m_ListLCase.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLoadCombSpecPage::OnMutexAddBtn()
{
	T_MUTX_CASE data;
	if (!Dlg2Item(data)) return;
	LOAD_CASE_K keyNew;
	T_STLD_D StldDNew;
	keyNew.Key = m_pDoc->m_pAttrCtrl->GetStldKey(data.strLiveCase);
	keyNew.nLoadCaseType = D_LCOM_STATIC;

	LOAD_CASE_K_ARR arNew;
	for (int j=0; j<data.aStldKey.GetCount(); j++)
	{
		LOAD_CASE_K keytemp;
		keytemp.Key = data.aStldKey[j];
		keytemp.nLoadCaseType = D_LCOM_STATIC;
		arNew.Add(keytemp);
	}

	CString strBlank = _T(",");
	int i=0;
	BOOL bRepeat = FALSE;
	LOAD_CASE_K keyRepeat;
	LOAD_CASE_K_ARR arCaseRepeat;
	for (auto itr=m_pData->m_mapExclusion.begin(); itr != m_pData->m_mapExclusion.end(); itr++)
	{
		LOAD_CASE_K key = itr->first;
		LOAD_CASE_K_ARR arCase = itr->second;

		T_STLD_D StldD;
		CString strLstCase = _T("");
		for (int k=0; k<arCase.GetCount(); k++)
		{
			StldD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(arCase[k].Key, StldD))
				return;
			strLstCase += StldD.LoadCaseName + strBlank;
		}
		
		if (keyNew.Key==key.Key)
		{
			if (strLstCase == data.strMutexLoad)
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
				return;
			}
			else
			{
				for (int k = 0; k < arCase.GetCount(); k++)
				{
					for (int nk = 0; nk < arNew.GetCount(); nk++)
					{
						if (arNew[nk] == arCase[k])
						{
							arNew.RemoveAt(nk);
						}
					}
				}
				for (int nk = 0; nk < arNew.GetCount(); nk++)
				{
					arCase.Add(arNew[nk]);
				}
				keyRepeat = key;
				arCaseRepeat.RemoveAll();
				arCaseRepeat.Copy(arCase);
				bRepeat = TRUE;
			}
		}
	}
	if (bRepeat)
	{
		int nItem = -1;
		int nCount = m_ListMutexCase.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			T_STLD_K key = m_ListMutexCase.GetItemData(i);
			if (key == keyRepeat.Key)
			{
				nItem = i; break;
			}
		}
		m_ListMutexCase.DeleteItem(nItem);
		m_pData->m_mapExclusion.erase(keyRepeat);
		m_pData->m_mapExclusion[keyRepeat] = arCaseRepeat;
		T_STLD_D StldD;
		CString strLstCase = _T("");
		for (int k = 0; k < arCaseRepeat.GetCount(); k++)
		{
			StldD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(arCaseRepeat[k].Key, StldD))
				return;
			strLstCase += StldD.LoadCaseName + strBlank;
		}
		data.strMutexLoad = strLstCase;
	}		
	else
		m_pData->m_mapExclusion[keyNew] = arNew;
	INT_PTR nNumLC = m_pData->m_mapExclusion.size();
	m_ListMutexCase.InsertItem(nNumLC-1, _T(""));
	
	SetItem(nNumLC-1, data);
	m_ListMutexCase.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMLoadCombSpecPage::OnMutexEditBtn()
{
	int nIndex;
	if (!GetSelectedItem2(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_MUTX_CASE data;
	if (!Dlg2Item(data)) return;

	LOAD_CASE_K keyNew;
	T_STLD_D StldDNew;
	keyNew.Key = m_pDoc->m_pAttrCtrl->GetStldKey(data.strLiveCase);
	keyNew.nLoadCaseType = D_LCOM_STATIC;

	LOAD_CASE_K_ARR arNew;
	for (int j=0; j<data.aStldKey.GetCount(); j++)
	{
		LOAD_CASE_K keytemp;
		keytemp.Key = data.aStldKey[j];
		keytemp.nLoadCaseType = D_LCOM_STATIC;
		arNew.Add(keytemp);
	}

	CString strBlank = _T(",");
	for (auto itr=m_pData->m_mapExclusion.begin(); itr!=m_pData->m_mapExclusion.end(); itr++)
	{
		if (keyNew == itr->first)
		{
			LOAD_CASE_K_ARR arCase = itr->second;
			T_STLD_D StldD;
			CString strLstCase = _T("");
			for (int k=0; k<arCase.GetCount(); k++)
			{
				StldD.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetStld(arCase[k].Key, StldD))
					return;
				strLstCase += StldD.LoadCaseName + strBlank;
			}
			if (strLstCase == data.strMutexLoad)
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
				return;
			}
		}
		else
			continue;
	}
	
	m_pData->m_mapExclusion[keyNew] = arNew;
	SetItem(nIndex, data);
	m_ListMutexCase.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CCMLoadCombSpecPage::GetSelectedItem1(int &nIndex)
{
	int iItem = m_ListLCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

BOOL CCMLoadCombSpecPage::GetSelectedItem2(int &nIndex)
{
	int iItem = m_ListMutexCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMLoadCombSpecPage::OnMutexDelBtn()
{
	int nItem = m_ListMutexCase.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}

	T_STLD_K key = m_ListMutexCase.GetItemData(nItem);

	LOAD_CASE_K keyNew;
	keyNew.Key = key;
	keyNew.nLoadCaseType = D_LCOM_STATIC;
	m_pData->m_mapExclusion.erase(keyNew);
	m_ListMutexCase.DeleteItem(nItem);

	int i=0;
	CString strBlank = _T(",");
	for (auto itr=m_pData->m_mapExclusion.begin(); itr!=m_pData->m_mapExclusion.end(); itr++)
	{
		T_MUTX_CASE data;
		data.nIndex = i;
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(itr->first.Key, StldD))
			return;
		data.strLiveCase = StldD.LoadCaseName;

		LOAD_CASE_K_ARR arCase = itr->second;
		for (int k=0; k<arCase.GetCount(); k++)
		{
			StldD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(arCase[k].Key, StldD))
				return;
			data.strMutexLoad += StldD.LoadCaseName + strBlank;
			data.aStldKey.Add(arCase[k].Key);
		}

		SetItem(i, data);
		m_ListMutexCase.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		i++;
	}

	// Set Focus.
	if(m_pData->m_mapExclusion.size() > nItem)
		m_ListMutexCase.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLoadCombSpecPage::OnChkMutexCase()
{
	UpdateData(TRUE);
	m_bMutex = IsDlgButtonChecked(IDC_CMD_LOADCOMB_DEF_SPEC_MUTEX_CASE_CHECK);
	if (m_bMutex)
	{
		m_bAdverse = FALSE;
		for(int i=0; i<m_aCtrlAdverse.GetSize(); i++)
			GetDlgItem(m_aCtrlAdverse[i])->EnableWindow(FALSE);
		for(int i=0; i<m_aCtrlMutex.GetSize(); i++)
			GetDlgItem(m_aCtrlMutex[i])->EnableWindow(TRUE);
	}
	else
	{
		for(int i=0; i<m_aCtrlMutex.GetSize(); i++)
			GetDlgItem(m_aCtrlMutex[i])->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CCMLoadCombSpecPage::OnChangeSelMutexCase()
{
	UpdateData(TRUE);
	Initial_MutexCaseComboBox();
	UpdateData(FALSE);
}

void CCMLoadCombSpecPage::OnChangedCurrentItem1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex;
	T_ADVE_CASE data;
	if (!GetSelectedItem1(nIndex))
		data.Initialize();
	else
	{
		LOAD_CASE_K_ARR arLiveCase = m_pData->m_aLLUnfavorable[nIndex];
		data.nIndex = nIndex;
		CString strBlank = _T(",");
		for (int i=0; i<arLiveCase.GetCount(); i++)
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(arLiveCase[i].Key, StldD))
				return;
			data.strLiveLoad += StldD.LoadCaseName + strBlank;
			data.vAdverKey.push_back(arLiveCase[i].Key);
		}
	}

	Item2Dlg(nIndex, data);

	*pResult = 0;
}
void CCMLoadCombSpecPage::OnChangedCurrentItem2(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex;
	T_MUTX_CASE data;
	if (!GetSelectedItem2(nIndex))
		data.Initialize();
	else
	{
		T_STLD_K StldK = m_ListMutexCase.GetItemData(nIndex);
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD))
			return;
		data.nIndex = nIndex;
		data.strLiveCase = StldD.LoadCaseName;

		LOAD_CASE_K liveK;
		liveK.Key = StldK;
		liveK.nLoadCaseType = D_LCOM_STATIC;
		LOAD_CASE_K_ARR arKey = m_pData->m_mapExclusion[liveK];
		CString strBlank = _T(",");
		for (int i=0; i<arKey.GetCount(); i++)
		{
			StldD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(arKey[i].Key, StldD))
				return;
			data.strMutexLoad += StldD.LoadCaseName+strBlank;
			data.aStldKey.Add(arKey[i].Key);
		}	
	}

	Item2Dlg(nIndex, data);

	*pResult = 0;
}

LRESULT CCMLoadCombSpecPage::OpenNewTab(WPARAM wParam, LPARAM lParam)
{
	//OnActive_Tab();  
	if (!Data2Dlg())
		return 0L;
	return 0L; 
}

LRESULT CCMLoadCombSpecPage::CloseCurTab(WPARAM wParam, LPARAM lParam)
{
	if (!Dlg2Data())
		return 0L;

	return 0L;  
}


