
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombParaPage.h"

#include "CmdSetOrthoLCDlg.h"
#include "CMLoadCombDefPartialFactorDlg.h"
#include "CMLoadCombDefCombFactorDlg.h"
#include "CMLoadCombDefGCoefDlg.h"
#include "LoadCombDefineData.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\DlgTabCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombParaPage dialog


CCMLoadCombParaPage::CCMLoadCombParaPage(CLoadCombDefineData *pData, CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMLoadCombParaPage::IDD, pParent)
{
	m_pData = pData;
	m_pParent = (CCMLoadCombDefDlg*) pParent;

	m_bSeismicEvaluation = FALSE;
}

void CCMLoadCombParaPage::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCombParaPage)
	DDX_Check(pDX, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO, m_nConstLoad);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_PARA_ORTHRLT_CHECK, m_chkOrthRlt);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_PARA_WINDCASE_COMBO, m_cmbWCase);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_PARA_ECASE_COMBO, m_cmbECase);
	DDX_Text(pDX, IDC_CMD_LOADCOMB_DEF_PARA_CASE_L_EDIT, m_dPhiL);
	DDX_Text(pDX, IDC_CMD_LOADCOMB_DEF_PARA_CASE_W_EDIT, m_dPhiW);
	DDX_Text(pDX, IDC_CMD_LOADCOMB_DEF_PARA_CASE_T_EDIT, m_dPhiT);
	DDX_Text(pDX, IDC_CMD_LOADCOMB_DEF_PARA_CASE_G_EDIT, m_dGravL);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_PARA_CASE_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLoadCombParaPage, CChildDialog)
	//{{AFX_MSG_MAP(CCMLoadCombParaPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LOADCOMB_PARA_CASE_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_PARA_ORTHRLT_CHECK,		OnChkSetOtholc)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_PARA_SETSEICASE_BUTTON,	OnCmdSetOtholcBtn)	
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_PARA_CASE_ADD_BUTTON,		OnWindAndEarthCaseCombAddBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_PARA_CASE_EDIT_BUTTON,		OnWindAndEarthCaseCombEditBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_PARA_CASE_DEL_BUTTON,		OnWindAndEarthCaseCombDelBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_PARA_SPECCASE_BUTTON,	OnConSpecCasePartCoefBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_PARA_CASE_DETIAL_SET_BUTTON, OnCombCoefDetailSetBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_PARA_CASE_G_SET_BUTTON,	OnGravCoefDetailSetBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDlgTabCtrl::WM_CDLGTABCTRL_ACTIVE_TAB, OpenNewTab)
	ON_REGISTERED_MESSAGE(CDlgTabCtrl::WM_CDLGTABCTRL_INACTIVE_TAB, CloseCurTab)
END_MESSAGE_MAP()

BOOL CCMLoadCombParaPage::OnInitDialog()
{
	CChildDialog::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	m_aCtrlConstLoad.RemoveAll();
	m_aCtrlConstLoad.Add(IDC_CMD_LOADCOMB_DEF_PARA_CASE_STATIC);
	m_aCtrlConstLoad.Add(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO);
	m_aCtrlConstLoad.Add(IDC_CMD_LOADCOMB_DEF_PARA_CASE_CS_RADIO);
	m_aCtrlConstLoad.Add(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO);

	CArray<T_SGLD_K,T_SGLD_K> arSgldKey; 
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(arSgldKey);
	BOOL bStageMode = m_pDoc->m_pAttrCtrl->GetCountStag() > 0;
	BOOL bFinalStage = m_pDoc->m_pStagCtrl->IsFinalStag();
	BOOL bPostCS = bFinalStage && bStageMode;
	m_bExistCS = (arSgldKey.GetSize() > 0 && bPostCS) ? TRUE : FALSE;
	if(m_bExistCS) 
	{
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_CS_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_CS_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO)->EnableWindow(FALSE);
	}
	m_nConstLoad = m_pData->m_nStage;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(m_arKeyList);
	//ILoadCombDefineMgr::Instance()->GetUsedKeyList(m_bSeismicEvaluation, m_arKeyList);
	Initial_WCaseComboBox();
	Initial_ECaseComboBox();
	SetListCtrlHeader();
	
	Data2Dlg();
	
	return TRUE;
}

BOOL CCMLoadCombParaPage::Data2Dlg()
{
	m_nConstLoad = m_pData->m_nStage;
	if (m_nConstLoad == 0)
		CheckRadioButton(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO);
	else if (m_nConstLoad == 1)
		CheckRadioButton(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_CS_RADIO);
	else if (m_nConstLoad == 2)
		CheckRadioButton(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO);
	
	m_chkOrthRlt.SetCheck(m_pData->m_bBiEarthquake);
	OnChkSetOtholc();
	int nIdxE = m_cmbECase.GetCurSel();
	MakeItemEx();
	m_cmbECase.SetCurSel(nIdxE);

	m_dPhiL = m_pData->m_dCombFactor[0];
	m_dPhiW = m_pData->m_dCombFactor[1];
	m_dPhiT = m_pData->m_dCombFactor[2];
	m_dGravL = m_pData->m_dLLGravityFactor;

	CString strTemp;
	strTemp.Format(_T("%.2f"), m_dPhiL);
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_L_EDIT)->SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), m_dPhiW);
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_W_EDIT)->SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), m_dPhiT);
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_T_EDIT)->SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), m_dGravL);
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_G_EDIT)->SetWindowText(strTemp);

	return TRUE;
}

BOOL CCMLoadCombParaPage::Dlg2Data()
{
	//UpdateData(TRUE);
	if (GetCheckedRadioButton(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO) == IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO)
		m_nConstLoad = 0;
	else if (GetCheckedRadioButton(IDC_CMD_LOADCOMB_DEF_PARA_CASE_ST_RADIO, IDC_CMD_LOADCOMB_DEF_PARA_CASE_ALL_RADIO) == IDC_CMD_LOADCOMB_DEF_PARA_CASE_CS_RADIO)
		m_nConstLoad = 1;
	else
		m_nConstLoad = 2;
	m_pData->m_nStage = m_nConstLoad;

	m_pData->m_bBiEarthquake = IsDlgButtonChecked(IDC_CMD_LOADCOMB_DEF_PARA_ORTHRLT_CHECK);

	CString strTemp;
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_L_EDIT)->GetWindowText(strTemp);
	m_dPhiL = _tstof(strTemp);
	m_pData->m_dCombFactor[0] = m_dPhiL;

	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_W_EDIT)->GetWindowText(strTemp);
	m_dPhiW = _tstof(strTemp);
	m_pData->m_dCombFactor[1] = m_dPhiW;

	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_T_EDIT)->GetWindowText(strTemp);
	m_dPhiT = _tstof(strTemp);
	m_pData->m_dCombFactor[2] = m_dPhiT;

	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_CASE_G_EDIT)->GetWindowText(strTemp);
	m_dGravL = _tstof(strTemp);
	m_pData->m_dLLGravityFactor = m_dGravL;
	//UpdateData(FALSE);
	ILoadCombDefineMgr::Instance()->SetLCaseCoef(m_dPhiW, m_dPhiL, m_dGravL);
	return TRUE;
}

void CCMLoadCombParaPage::OnChkSetOtholc()
{
	GetDlgItem(IDC_CMD_LOADCOMB_DEF_PARA_SETSEICASE_BUTTON)->EnableWindow(m_chkOrthRlt.GetCheck()?TRUE:FALSE);
}

void CCMLoadCombParaPage::OnCmdSetOtholcBtn()
{
	CCmdSetOrthoLCDlg dlg;
	dlg.SetUserDefDataList(m_pData->m_mapBiEarthquake, TRUE);
	dlg.DoModal();
	dlg.GetUserDefDataList(m_pData->m_mapBiEarthquake);
	if (m_pData->m_mapBiEarthquake.size() > 0)
		m_pData->m_bBiEarthquake = TRUE;
}

void CCMLoadCombParaPage::ChangeDataStore(LOAD_CASE_K keyW, LOAD_CASE_K keyE, T_COMB_WE &data)
{
	data.keyE = keyE.Key;
	data.keyW = keyW.Key;
	data.nLoadCaseType = keyE.nLoadCaseType;
}

void CCMLoadCombParaPage::OnWindAndEarthCaseCombAddBtn()
{	
	LOAD_CASE_K keyW,keyE;
	if (!Dlg2Item(keyW,keyE)) return;
	int nIdxE = m_cmbECase.GetCurSel();
	T_COMB_WE data;
	ChangeDataStore(keyW,keyE,data);
	//if (!ValidItem(data)) return;

	for (auto itr = m_pData->m_mapBC2C.begin(); itr != m_pData->m_mapBC2C.end(); itr++)
	{
		if(itr->first == keyW || itr->second == keyE)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_case_is_already_assigned_));
			return;
		}
	}

	m_pData->m_mapBC2C.insert(std::make_pair(keyW, keyE));
	INT_PTR nNumLC = m_pData->m_mapBC2C.size();
	m_ListCtrl.InsertItem(nNumLC-1, _T(""));
	SetItem(nNumLC-1, data);
	m_ListCtrl.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
	m_cmbECase.SetCurSel(nIdxE);
}

void CCMLoadCombParaPage::OnWindAndEarthCaseCombEditBtn()
{
	int nIdxE = m_cmbECase.GetCurSel();
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	LOAD_CASE_K keyW,keyE;
	if (!Dlg2Item(keyW,keyE)) return;

	T_COMB_WE data;
	ChangeDataStore(keyW,keyE,data);
	//if (!ValidItem(data)) return;

	LOAD_CASE_K keyOld = m_pData->m_mapBC2C[keyW];
	if (keyOld == keyE)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__An_identical_load_group_already_exists_));
		return;
	}

	m_pData->m_mapBC2C[keyW] = keyE; // .insert(std::make_pair(keyW, keyE));
	SetItem(nIndex, data);
	m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	m_cmbECase.SetCurSel(nIdxE);
}

void CCMLoadCombParaPage::OnWindAndEarthCaseCombDelBtn()
{
	int nIdxE = m_cmbECase.GetCurSel();
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	T_STLD_K StldK = m_ListCtrl.GetItemData(nItem);
	LOAD_CASE_K keyW, keyE;
	keyW.Key = StldK;
	keyW.nLoadCaseType = D_LCOM_STATIC;
	m_pData->m_mapBC2C.erase(keyW);
	m_ListCtrl.DeleteItem(nItem);

	int i=0;
	for(auto itr=m_pData->m_mapBC2C.begin(); itr != m_pData->m_mapBC2C.end(); itr++)
	{
		keyW = itr->first;
		keyE = itr->second;
		T_COMB_WE data;
		ChangeDataStore(keyW, keyE, data);

		SetItem(i, data);
		m_ListCtrl.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		i++;
	}
	// Set Focus.
	if(m_pData->m_mapBC2C.size() > nItem)
		m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	m_cmbECase.SetCurSel(nIdxE);
}

void CCMLoadCombParaPage::OnConSpecCasePartCoefBtn()
{
	CCMLoadCombDefPartialFactorDlg dlg;
	dlg.SetDataList(m_pData->m_mapSpecialFactor);
	dlg.DoModal();	
	dlg.GetDataList(m_pData->m_mapSpecialFactor);
}

void CCMLoadCombParaPage::OnCombCoefDetailSetBtn()
{
	CCMLoadCombDefCombFactorDlg dlg;
	dlg.SetDataList(m_pData->m_mapCombFactor, m_pData->m_dLLAdjustCombFactor);
	dlg.DoModal();
	dlg.GetDataList(m_pData->m_mapCombFactor, m_pData->m_dLLAdjustCombFactor);
}

void CCMLoadCombParaPage::OnGravCoefDetailSetBtn()
{
	CCMLoadCombDefGCoefDlg dlg;
	dlg.SetDataList(m_pData->m_mapGravityFactor);
	dlg.DoModal();
	dlg.GetDataList(m_pData->m_mapGravityFactor);
}

void CCMLoadCombParaPage::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex;
	T_COMB_WE data;
	if (!GetSelectedItem(nIndex))
		data.Initialize();
	else
	{
		T_STLD_K StldK = m_ListCtrl.GetItemData(nIndex);
		LOAD_CASE_K keyW, keyE;
		keyW.Key = StldK;
		keyW.nLoadCaseType = D_LCOM_STATIC;
		keyE = m_pData->m_mapBC2C[keyW];
		ChangeDataStore(keyW, keyE, data);
	}

	Item2Dlg(nIndex, data);

	*pResult = 0;
}

void CCMLoadCombParaPage::Initial_WCaseComboBox()
{
	m_cmbWCase.ResetContent();
	CArray<T_STLD_K,T_STLD_K> arStldKey; 
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);

	T_STLD_D StldD;
	BOOL bExist = FALSE;
	for (int i=0; i<arStldKey.GetCount(); i++)
	{
		StldD.Initialize();
		for (int j = 0; j < m_arKeyList.GetSize(); j++)
		{
			if (arStldKey[i] == m_arKeyList.GetAt(j))   bExist = TRUE;
		}
		if (!m_pDoc->m_pAttrCtrl->GetStld(arStldKey[i], StldD) || !bExist)
			continue;
		if (StldD.LoadCaseType == _T("W"))
			m_cmbWCase.AddString(StldD.LoadCaseName);
		else
			continue;
	}
	m_cmbWCase.SetCurSel(0);
}

void CCMLoadCombParaPage::Initial_ECaseComboBox()
{
	m_cmbECase.ResetContent();
	m_arLCtype.RemoveAll();

	CArray<T_STLD_K,T_STLD_K> arStldKey; 
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKey);
	CArray<T_SPLC_K, T_SPLC_K> arSplcKey;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcKey);

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
		if (StldD.LoadCaseType == _T("E") || StldD.LoadCaseType == _T("EVT"))
		{
			m_cmbECase.AddString(StldD.LoadCaseName);
			T_COMB_CASE LCType;
			LCType.nLoadCaseType = 0;
			LCType.strName = StldD.LoadCaseName;
			m_arLCtype.Add(LCType);
		}
		else
			continue;
	}
	T_SPLC_D SplcD;
	for (int j=0; j<arSplcKey.GetCount(); j++)
	{
		SplcD.Initialize();
		BOOL bExist = FALSE;
		for (int k = 0; k < m_arKeyList.GetSize(); k++)
		{
			if (arSplcKey[j] == m_arKeyList.GetAt(k))   bExist = TRUE;
		}
		if (!m_pDoc->m_pAttrCtrl->GetSplc(arSplcKey[j], SplcD) || !bExist)
			continue;
		m_cmbECase.AddString(SplcD.LoadCaseName);
		T_COMB_CASE LCType;
		LCType.nLoadCaseType = 1;
		LCType.strName = SplcD.LoadCaseName;
		m_arLCtype.Add(LCType);
	}
	m_cmbECase.SetCurSel(0);
}

void CCMLoadCombParaPage::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_CMD_CSCS__Combined), _LS(IDS_WG_CMD__ADDD__Wind), _LS(IDS_WG_CMD__ADDD__Seismic)};
	int nColWidth[] = {60, 80, 90};
	int nColNum = 3;
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

BOOL CCMLoadCombParaPage::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMLoadCombParaPage::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	T_COMB_WE data;
	LOAD_CASE_K keyW, keyE;
	int k=0;
	for (auto itr = m_pData->m_mapBC2C.begin(); itr != m_pData->m_mapBC2C.end(); itr++)
	{
		m_ListCtrl.InsertItem(k, _T(""));
		keyW = itr->first;
		keyE = itr->second;
		data.Initialize();
		ChangeDataStore(keyW, keyE, data);
		SetItem(k, data);
		k++;
	}
	
	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCMLoadCombParaPage::DataToStr(int i, T_COMB_WE &data)
{
	CString value = _T("");
	T_STLD_K keyW;
	keyW = data.keyW;

	if (i == 1)
	{
		T_STLD_D rDataW;
		m_pDoc->m_pAttrCtrl->GetStld(keyW, rDataW);
		value = rDataW.LoadCaseName;
	}
	else
	{
		if(data.nLoadCaseType==D_LCOM_STATIC)	// 0=Static
		{
			T_STLD_K keyE = data.keyE;
			T_STLD_D rDataE;
			m_pDoc->m_pAttrCtrl->GetStld(keyE, rDataE);
			value = rDataE.LoadCaseName;
		}
		else if(data.nLoadCaseType==D_LCOM_SPECTRUM) // 1=Response Spectrum
		{
			T_SPLC_K key = data.keyE;
			T_SPLC_D rData;
			m_pDoc->m_pAttrCtrl->GetSplc(key, rData);
			value = rData.LoadCaseName;
		}
	}
	
	return value;
}

void CCMLoadCombParaPage::SetItem(int nIndex, T_COMB_WE &data)
{
	LVITEM lvitem;
	CString str;
	CString strIndex;
	for(int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		if(i==0) str.Format(_T("%d"), nIndex+1);
		else	 str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ListCtrl.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	m_ListCtrl.SetItemData(nIndex, data.keyW);
}

BOOL CCMLoadCombParaPage::ValidItem(T_COMB_WE& data)
{
	if (data.keyW <= 0 || data.keyE <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	BOOL bStldKW = FALSE;
	BOOL bStldKE = FALSE;
	for(int i=0; i<m_arKeyList.GetSize(); i++)
	{
		if(data.keyW == m_arKeyList.GetAt(i))   bStldKW = TRUE;
		if(data.keyE == m_arKeyList.GetAt(i))   bStldKE = TRUE;
	}

	if(!bStldKW && !bStldKE)
	{
		CString strErrorMsg=_T("");
		CString strErrorCase=_T("");
		strErrorCase.Format(_T("%s and %s"),m_strCaseW,m_strCaseE);
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),strErrorCase);
		AfxMessageBox(strErrorMsg, MB_OK);
		return FALSE;
	}
	else if(!bStldKW && bStldKE)
	{
		CString strErrorMsg=_T("");
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),m_strCaseW);
		AfxMessageBox(strErrorMsg, MB_OK);
		return FALSE;
	}
	else if(bStldKW && !bStldKE)
	{
		CString strErrorMsg=_T("");
		strErrorMsg.Format(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_EQ),m_strCaseE);
		AfxMessageBox(strErrorMsg, MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CCMLoadCombParaPage::Dlg2Item(LOAD_CASE_K &keyW, LOAD_CASE_K &keyE)
{
	int IndexW = m_cmbWCase.GetCurSel();
	int IndexE = m_cmbECase.GetCurSel();

	if(IndexW != -1 || IndexE != -1 )
	{
		CString strName=_T("");

		m_cmbWCase.GetLBText(IndexW,strName);
		CString strLCNameW = strName;
		m_strCaseW  = strLCNameW; 
		keyW.Key = m_pDoc->m_pAttrCtrl->GetStldKey(m_strCaseW);
		keyW.nLoadCaseType = D_LCOM_STATIC;

		m_cmbECase.GetLBText(IndexE,strName);
		CString strLCNameE = strName;
		m_strCaseE  = strLCNameE;

		int iLoadCaseType = GetLoadCaseType(m_strCaseE);

		int nKey;
		if(iLoadCaseType==0)
		{
			nKey = m_pDoc->m_pAttrCtrl->GetStldKey(strLCNameE);

			keyE.Key = nKey;
			keyE.nLoadCaseType = D_LCOM_STATIC;
		}
		else if(iLoadCaseType==1)
		{
			nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCNameE);

			keyE.Key = nKey;
			keyE.nLoadCaseType = D_LCOM_SPECTRUM;
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}

	return TRUE;
}

int CCMLoadCombParaPage::GetLoadCaseType(CString strName)
{
	int iType;
	int iArrSize = m_arLCtype.GetSize();
	for(int i=0; i<iArrSize; i++)
	{
		T_COMB_CASE LCtype = m_arLCtype.GetAt(i);
		if(strName.Compare(LCtype.strName) == 0)
			iType = LCtype.nLoadCaseType;	
	}
	return iType;
}

BOOL CCMLoadCombParaPage::Item2Dlg(int nIndex, T_COMB_WE &data)
{
	if(m_pData->m_mapBC2C.size()!=0)
	{
		T_STLD_K KeyW = data.keyW;
		T_STLD_D rDataW;
		rDataW.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyW, rDataW);
		//ASSERT(bCheck);            
		CString strLoadCaseNameW = rDataW.LoadCaseName;
		int iCbIndexW = m_cmbWCase.SelectString(-1,strLoadCaseNameW);
		m_cmbWCase.SetCurSel(iCbIndexW);

		if(data.nLoadCaseType == 0)  // 0=static
		{
			T_STLD_K KeyE = data.keyE;
			T_STLD_D rDataE;
			rDataE.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(KeyE, rDataE);
			//ASSERT(bCheck);            
			CString strLoadCaseNameE = rDataE.LoadCaseName;
			int iCbIndexE = m_cmbECase.SelectString(-1,strLoadCaseNameE);
			m_cmbECase.SetCurSel(iCbIndexE);
		}
		else if(data.nLoadCaseType == 1)  // 1=Response Spectrum
		{
			T_SPLC_K KeyE = data.keyE;
			T_SPLC_D rDataE;
			rDataE.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(KeyE, rDataE);
			//ASSERT(bCheck);            
			CString strLoadCaseNameE = rDataE.LoadCaseName;
			int iCbIndexE = m_cmbECase.SelectString(-1,strLoadCaseNameE);
			m_cmbECase.SetCurSel(iCbIndexE);
		}
	}

	return TRUE;
}

BOOL CCMLoadCombParaPage::CheckLoadCaseName(CString str, int& Index)
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

LRESULT CCMLoadCombParaPage::OpenNewTab(WPARAM wParam, LPARAM lParam)
{
	if (!Data2Dlg())
		return 0L;
	return 0L; 
}

LRESULT CCMLoadCombParaPage::CloseCurTab(WPARAM wParam, LPARAM lParam)
{
	if (!Dlg2Data())
		return 0L;

	return 0L;  
}
