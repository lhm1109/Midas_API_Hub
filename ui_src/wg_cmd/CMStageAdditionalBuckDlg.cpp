// CMBuckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageAdditionalBuckDlg.h"
#include "CMStageItemBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_main\wg_mainRes2.h"


using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageAdditionalBuckDlg

//IMPLEMENT_DYNCREATE(CCMStageAdditionalBuckDlg, CDialogMove)

CCMStageAdditionalBuckDlg::CCMStageAdditionalBuckDlg(CWnd* pParent)
	: CDialogMove(CCMStageAdditionalBuckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageAdditionalBuckDlg)
	m_nLoadCategory = 0;
	m_nLoadType = 0;
	//}}AFX_DATA_INIT
	m_aLoadFactor.SetSize(2);
	m_aLoadFactor[0] = IDC_CMD_POSITIVE_RDO;
	m_aLoadFactor[1] = IDC_CMD_SEARCH_RDO;

	m_aBuckComb.SetSize(2);
	m_aBuckComb[0] = IDC_CMD_BUCK_LOADTYPE_RDO;
	m_aBuckComb[1] = IDC_CMD_BUCK_LOADTYPE_RDO2;

	m_aCategoryComb.SetSize(2);
	m_aCategoryComb[0] = IDC_CMD_BUCK_LOADTYPE_RDO3;
	m_aCategoryComb[1] = IDC_CMD_BUCK_LOADTYPE_RDO4;
}

CCMStageAdditionalBuckDlg::~CCMStageAdditionalBuckDlg()
{
}

void CCMStageAdditionalBuckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageAdditionalBuckDlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_BUCK_NUM_EDIT, m_wndBuckNumEdit);
	DDX_Control(pDX, IDC_CMD_SEARCH_FROM_EDT, m_wndSearchFrom);
	DDX_Control(pDX, IDC_CMD_SEARCH_TO_EDT, m_wndSearchTo);
	//DDX_Radio(pDX, IDC_CMD_BUCK_LOADTYPE_RDO3, m_nLoadCategory);
	DDX_Radio(pDX, IDC_CMD_BUCK_LOADTYPE_RDO, m_nLoadType);
	DDX_Radio(pDX, IDC_CMD_POSITIVE_RDO, m_bPositive);
	DDX_Control(pDX, IDC_CMD_STURM_CHK, m_bSturm);
	DDX_Control(pDX, IDC_CMD_BUCK_STIFFOPT_CHK, m_bAxialForce);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMStageAdditionalBuckDlg Implementation

void CCMStageAdditionalBuckDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_CMD_BUCK_loadcase), _LS(IDS_CMD_BUCK_scale), _LS(IDS_CMD_BUCK_LoadType) };
	CRect rtList;
	m_List.GetWindowRect(rtList);
	double dWidth = (rtList.Width() * 1.0) - globalUtils.GetSystemMetricsForDPI(&m_List, SM_CXVSCROLL) - 1.0;
	int nColWidth[] = { dWidth * 0.36, dWidth * 0.28, dWidth * 0.36 };
	int nColNum = 3;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMStageAdditionalBuckDlg::GetSelectedItem(int& nIndex)
{
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMStageAdditionalBuckDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nCSItem = m_Data.CSBucKCase.GetSize();
	int nSTItem = m_Data.nNumSTBuckCase;
	int nItemCount = nCSItem + nSTItem;
	if (nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		if (nCount < nCSItem)
		{
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, m_Data.CSBucKCase[nCount]);
		}
		else //	if (nCount < nSTItem)
		{
			int i = nCount - nCSItem;
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, m_Data.STBuckCase[i]);
		}
	}

	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

CString CCMStageAdditionalBuckDlg::DataToStr(int i, T_CSBK_BASE& data)
{
	CString aLoadType[] = { _LS(IDS_CMD_BUCK_Variable), _LS(IDS_CMD_BUCK_Constant) };
	CString strValue = _T("");

	T_STLD_K key = data.LoadCaseKey;
	T_STLD_D DataStld; DataStld.Initialize();

	if (i == 0)
	{
		CString PreNm = _T("");
		if (data.nCSType == 0)	// static
		{
			PreNm = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STATIC, LOAD_NORMAL);
			if (m_pDoc->IsPostMode())
			{
				m_pDoc->m_pPostCtrl->GetStld(key, DataStld);
			}
			else
			{
				m_pDoc->m_pAttrCtrl->GetStld(key, DataStld);
			}
		}
		else  // CS
		{
			PreNm = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STAGE, LOAD_NORMAL);

			if (m_pDoc->IsPostMode())
			{
				m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(key, DataStld);
			}
			else
			{
				m_pDoc->m_pAttrCtrl->GetSgld(key, DataStld);
			}
		}
		strValue.Format(_T("%s%s"), PreNm, DataStld.LoadCaseName);
	}
	else if (i == 1) strValue.Format(_T("%g"), data.dblFactor);
	else if (i == 2) strValue = aLoadType[data.nLoadType];
	return strValue;
}

void CCMStageAdditionalBuckDlg::SetItem(int nIndex, T_CSBK_BASE& data)
{
	LVITEM lvitem;
	CString str;
	for (int i = 0; i < 3; i++)
	{
		lvitem.iItem = nIndex;
		lvitem.iSubItem = i;
		str = DataToStr(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CCMStageAdditionalBuckDlg::ValidItem(T_CSBK_BASE& data)
{
	if (data.LoadCaseKey <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	if (data.dblFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_scale_factor_));
		return FALSE;
	}

	return TRUE;
}

BOOL CCMStageAdditionalBuckDlg::Dlg2Item(T_CSBK_BASE& data)
{
	if (!GetSelectedLoadCaseKey(data.LoadCaseKey,data.nCSType))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}

	CString csScaleFactor;
	m_wndScaleFactor.GetWindowText(csScaleFactor);
	if (!CStrParser::GetFloatNumber(csScaleFactor, data.dblFactor)) return FALSE;

	CDlgUtil::CtrlRadioGetCheck(this, m_aBuckComb, m_nLoadType);
	data.nLoadType = m_nLoadType;

	return TRUE;
}

BOOL CCMStageAdditionalBuckDlg::Item2Dlg(T_CSBK_BASE& data)
{
	SelectLoadCaseByKey(data.LoadCaseKey,data.nCSType);

	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dblFactor);
	m_wndScaleFactor.SetWindowText(csScaleFactor);

	CDlgUtil::CtrlRadioSetCheck(this, m_aBuckComb, m_nLoadType);
	data.nLoadType = m_nLoadType;

	int nLoadFactor;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadFactor, nLoadFactor);
	if (nLoadFactor == 0) m_bPositive = TRUE;
	else                  m_bPositive = FALSE;

	m_Data.bPositive = m_bPositive;

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMStageAdditionalBuckDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageAdditionalBuckDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	//ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_REMOVE_DATA, OnCmdRemoveData)
	ON_BN_CLICKED(IDC_CMD_POSITIVE_RDO, OnCmdLoadFactor)
	ON_BN_CLICKED(IDC_CMD_SEARCH_RDO, OnCmdLoadFactor)

	//ON_BN_CLICKED(IDC_CMD_BUCK_LOADTYPE_RDO3, OnCmdCategoryChanged)
	//ON_BN_CLICKED(IDC_CMD_BUCK_LOADTYPE_RDO4, OnCmdCategoryChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageAdditionalBuckDlg message handlers

BOOL CCMStageAdditionalBuckDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	GetDlgItem(IDC_CMD_BUCK_LOADTYPE_RDO3)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_BUCK_LOADTYPE_RDO4)->EnableWindow(FALSE);

	if (m_bInitAdd)
	{
		GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
		m_Data.Initialize();
	}

	InitializeLoadCaseCombo();

	Data2Dlg();

	EnableDisableCtrl();

	SetListCtrlHeader();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// RemoveAllSglsExceptSummationAndCSDL
void CCMStageAdditionalBuckDlg::GetStageTypeKey(CArray<T_STLD_K, T_STLD_K>& rKeyList)
{
	T_STCT_D DataStct;
	if (!m_pDoc->m_pAttrCtrl->GetStct(DataStct)) DataStct.Initialize();
	const auto nErection = DataStct.aErection.GetSize();

	CArray<T_STLD_K, T_STLD_K> KeyList;
	KeyList.Copy(rKeyList);

	CString strSUMKey = _LS(IDS_DB_SGLD_SUM);
	T_STLD_K nSUMKey = m_pDoc->m_pAttrCtrl->GetSgldKey(strSUMKey);
	CString strDLKey = _LS(IDS_DB_SGLD_DL);
	T_STLD_K nDLKey = m_pDoc->m_pAttrCtrl->GetSgldKey(strDLKey);

	if (DataStct.nIncludeNonLinearAnalysis != 0)
		nDLKey = NULL;

	int iSumPos, iDLPos;
	BOOL bExistSUM = FALSE;
	BOOL bExistDL = FALSE;
	for (int i = KeyList.GetSize() - 1; i >= 0; i--)
	{
		if (KeyList[i] == nSUMKey)
		{
			bExistSUM = TRUE;
			iSumPos = i;
		}
		if (KeyList[i] == nDLKey)
		{
			bExistDL = TRUE;
			iDLPos = i;
		}
	}
	if (!bExistSUM)
	{
		ASSERT(0);
	}

	T_SGLD_D DataSgld;
	rKeyList.RemoveAll();
	// summation 부터
	if (bExistSUM)
	{
		ASSERT(nSUMKey);
		rKeyList.Add(nSUMKey);
	}
	if (bExistDL)
	{
		ASSERT(nDLKey);
		if (nDLKey != NULL) rKeyList.Add(nDLKey);
	}

	// erection stage 만
	if (nErection > 0)
	{
		for (int i = 0; i < KeyList.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetSgld(KeyList[i], DataSgld);
			for (int j = 0; j < nErection; j++)
			{
				const T_STCT_EREC& load = DataStct.aErection[j];
				if (load.strErection == DataSgld.LoadCaseName)
				{
					rKeyList.Add(KeyList[i]);
					break;
				}
			}
		}
	}
}

void CCMStageAdditionalBuckDlg::GetStageTypeKeySumDL(CArray<T_STLD_K, T_STLD_K>& rKeyList)
{
	T_STCT_D DataStct;
	if (!m_pDoc->m_pAttrCtrl->GetStct(DataStct)) DataStct.Initialize();

	CArray<T_STLD_K, T_STLD_K> KeyList;
	KeyList.Copy(rKeyList);

	CString strSUMKey = _LS(IDS_DB_SGLD_SUM);
	T_STLD_K nSUMKey = m_pDoc->m_pAttrCtrl->GetSgldKey(strSUMKey);
	CString strDLKey = _LS(IDS_DB_SGLD_DL);
	T_STLD_K nDLKey = m_pDoc->m_pAttrCtrl->GetSgldKey(strDLKey);

	if (DataStct.nIncludeNonLinearAnalysis != 0)
		nDLKey = NULL;

	int iSumPos, iDLPos;
	BOOL bExistSUM = FALSE;
	BOOL bExistDL = FALSE;
	for (int i = KeyList.GetSize() - 1; i >= 0; i--)
	{
		if (KeyList[i] == nSUMKey)
		{
			bExistSUM = TRUE;
			iSumPos = i;
		}
		if (KeyList[i] == nDLKey)
		{
			bExistDL = TRUE;
			iDLPos = i;
		}
	}
	if (!bExistSUM)
	{
		ASSERT(0);
	}

	T_STLD_D DataStld;
	rKeyList.RemoveAll();
	// summation 부터
	if (bExistSUM)
	{
		ASSERT(nSUMKey);
		rKeyList.Add(nSUMKey);
	}
	if (bExistDL)
	{
		ASSERT(nDLKey);
		if (nDLKey != NULL) rKeyList.Add(nDLKey);
	}
}

void CCMStageAdditionalBuckDlg::GetStageTypeKeyErection(CArray<T_STLD_K, T_STLD_K>& rKeyList)
{
	T_STCT_D DataStct;
	if (!m_pDoc->m_pAttrCtrl->GetStct(DataStct)) DataStct.Initialize();
	const auto nErection = DataStct.aErection.GetSize();

	CArray<T_STLD_K, T_STLD_K> KeyList;
	KeyList.Copy(rKeyList);

	if (DataStct.nIncludeNonLinearAnalysis != 0)
	{
		rKeyList.RemoveAll();
		return;
	}

	// erection stage 만
	T_STLD_D DataStld;
	rKeyList.RemoveAll();
	if (nErection > 0)
	{
		for (int i = 0; i < KeyList.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetSgld(KeyList[i], DataStld);
			for (int j = 0; j < nErection; j++)
			{
				const T_STCT_EREC& load = DataStct.aErection[j];
				if (load.strErection == DataStld.LoadCaseName)
				{
					rKeyList.Add(KeyList[i]);
					break;
				}
			}
		}
	}
}

void CCMStageAdditionalBuckDlg::InitializeLoadCaseCombo()
{
	m_cmbLoadCase.ResetContent();
	m_aLoadType.RemoveAll();
	m_aLoadCaseKeys.RemoveAll();

	T_STLD_D DataStld;
	CArray<T_STLD_K, T_STLD_K> KeyListCS;
	CArray<T_STLD_K, T_STLD_K> KeyListCSSumDL;
	CArray<T_STLD_K, T_STLD_K> KeyListCSErect;
	CArray<T_STLD_K, T_STLD_K> KeyListST;
	if (m_pDoc->IsPostMode())
	{
		m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(KeyListCS);
		//GetStageTypeKey(KeyListCS);

		KeyListCSSumDL.Copy(KeyListCS);
		GetStageTypeKeySumDL(KeyListCSSumDL);

		KeyListCSErect.Copy(KeyListCS);
		GetStageTypeKeyErection(KeyListCSErect);

		m_pDoc->m_pPostCtrl->GetValidStldKeyList(KeyListST);
		for (int i = KeyListST.GetSize() - 1; i >= 0; i--)
		{
			m_pDoc->m_pPostCtrl->GetStld(KeyListST[i], DataStld);
			if (DataStld.LoadCaseType != _T("TCS"))
			{
				KeyListST.RemoveAt(i);
			}
		}
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetSgldKeyList(KeyListCS);
		//GetStageTypeKey(KeyListCS);

		KeyListCSSumDL.Copy(KeyListCS);
		GetStageTypeKeySumDL(KeyListCSSumDL);

		KeyListCSErect.Copy(KeyListCS);
		GetStageTypeKeyErection(KeyListCSErect);

		m_pDoc->m_pAttrCtrl->GetStldKeyList(KeyListST);
		for (int i = KeyListST.GetSize() - 1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetStld(KeyListST[i], DataStld);
			if (DataStld.LoadCaseType != _T("TCS"))
			{
				KeyListST.RemoveAt(i);
			}
		}
	}

	CString strName = _T("");
	CString PreNmCS = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STAGE, LOAD_NORMAL);
	for (int i = 0; i < KeyListCSSumDL.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetSgld(KeyListCSSumDL[i], DataStld);
		CString LCName = DataStld.LoadCaseName;

		strName.Format(_T("%s%s"), PreNmCS, LCName);
		int nIndex = m_cmbLoadCase.AddString(strName);
		m_cmbLoadCase.SetItemData(nIndex, m_aLoadCaseKeys.GetSize());
		m_aLoadCaseKeys.Add(KeyListCSSumDL[i]);
		m_aLoadType.Add(1);  // CS
	}

	for (int i = 0; i < KeyListCSErect.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetSgld(KeyListCSErect[i], DataStld);
		CString LCName = DataStld.LoadCaseName;

		strName.Format(_T("%s%s"), PreNmCS, LCName);
		int nIndex = m_cmbLoadCase.AddString(strName);
		m_cmbLoadCase.SetItemData(nIndex, m_aLoadCaseKeys.GetSize());
		m_aLoadCaseKeys.Add(KeyListCSErect[i]);
		m_aLoadType.Add(2);  // CS
	}

	CString PreNmST = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STATIC, LOAD_NORMAL);
	for (int i = 0; i < KeyListST.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(KeyListST[i], DataStld);
		CString LCName = DataStld.LoadCaseName;

		strName.Format(_T("%s%s"), PreNmST, LCName);
		int nIndex = m_cmbLoadCase.AddString(strName);
		m_cmbLoadCase.SetItemData(nIndex, m_aLoadCaseKeys.GetSize());
		m_aLoadCaseKeys.Add(KeyListST[i]);
		m_aLoadType.Add(0);  // ST
	}
	
	// 첫 번째 항목 선택
	if (m_cmbLoadCase.GetCount() > 0)
		m_cmbLoadCase.SetCurSel(0);
}

// 선택된 LoadCase Key 가져오기
BOOL CCMStageAdditionalBuckDlg::GetSelectedLoadCaseKey(T_STLD_K& outKey, int& nCSType)
{
	int nSel = m_cmbLoadCase.GetCurSel();
	if (nSel == CB_ERR)
		return FALSE;
	
	DWORD_PTR nIndex = m_cmbLoadCase.GetItemData(nSel);
	if (nIndex < 0 || nIndex >= (DWORD_PTR)m_aLoadCaseKeys.GetSize())
		return FALSE;
	
	outKey = m_aLoadCaseKeys[nIndex];
	nCSType = m_aLoadType[nIndex];
	return TRUE;
}

// Key로 LoadCase 선택
BOOL CCMStageAdditionalBuckDlg::SelectLoadCaseByKey(T_STLD_K key, int nCSType)
{
	for (int i = 0; i < m_aLoadCaseKeys.GetSize(); i++)
	{
		if (m_aLoadCaseKeys[i] == key && m_aLoadType[i] == nCSType)
		{
			for (int j = 0; j < m_cmbLoadCase.GetCount(); j++)
			{
				if (m_cmbLoadCase.GetItemData(j) == (DWORD_PTR)i)
				{
					m_cmbLoadCase.SetCurSel(j);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CCMStageAdditionalBuckDlg::OnCmdApply()
{
	Dlg2Data();

	m_bBucklingChk = TRUE;
	CDialogMove::OnOK();
}

void CCMStageAdditionalBuckDlg::OnCmdClose()
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

//void CCMStageAdditionalBuckDlg::OnClickLoadCaseBtn()
//{
//	// TODO: Add your control notification handler code here
//	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
//}

void CCMStageAdditionalBuckDlg::OnCmdBtnAdd()
{
	if (m_Data.nNumSTBuckCase >= D_CSBK_NUMBUCK)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
		return;
	}

	T_CSBK_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;

	int nCase = m_Data.CSBucKCase.GetSize() + m_Data.nNumSTBuckCase;
	if (data.nCSType == 1 || data.nCSType == 2)	// CS
	{
		for (int i = 0; i < m_Data.CSBucKCase.GetSize(); i++)
		{
			if (m_Data.CSBucKCase[i].LoadCaseKey == data.LoadCaseKey)
			{
				CString csLoadCase;
				m_cmbLoadCase.GetWindowText(csLoadCase);
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
				AfxMessageBox(msg);
				return;
			}
		}
		m_Data.CSBucKCase.Add(data);
	}
	else
	{
		for (int i = 0; i < m_Data.nNumSTBuckCase; i++)
		{
			if (m_Data.STBuckCase[i].LoadCaseKey == data.LoadCaseKey)
			{
				CString csLoadCase;
				m_cmbLoadCase.GetWindowText(csLoadCase);
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
				AfxMessageBox(msg);
				return;
			}
		}

		m_Data.STBuckCase[m_Data.nNumSTBuckCase] = data;
		m_Data.nNumSTBuckCase++;
	}

	nCase = m_Data.CSBucKCase.GetSize() + m_Data.nNumSTBuckCase;

	MakeItemEx();
	m_List.SetItemState(nCase++, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMStageAdditionalBuckDlg::OnCmdBtnModify()
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_CSBK_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;

	int nCase = m_Data.CSBucKCase.GetSize() + m_Data.nNumSTBuckCase;

	for (int i = 0; i < nCase; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외

		if (data.nCSType == 1 || data.nCSType == 2)
		{
			for (int j = 0; j < m_Data.CSBucKCase.GetSize(); j++)
			{
				if (m_Data.CSBucKCase[j].LoadCaseKey == data.LoadCaseKey)
				{
					CString csLoadCase;
					m_cmbLoadCase.GetWindowText(csLoadCase);
					CString msg;
					msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
					AfxMessageBox(msg);
					return;
				}
			}
		}
		else 
		{
			if (m_Data.STBuckCase[i].LoadCaseKey == data.LoadCaseKey)
			{
				CString csLoadCase;
				m_cmbLoadCase.GetWindowText(csLoadCase);
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
				AfxMessageBox(msg);
				return;
			}
		}
	}

	if (data.nCSType == 1 || data.nCSType == 2)	// CS type
	{
		m_Data.CSBucKCase[nIndex] = data;
	}
	else // if (data.nCSType == 0)	// ST type
	{
		int nSTIndex = nIndex - m_Data.CSBucKCase.GetSize();
		if (nSTIndex >= D_CSBK_NUMBUCK)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
			return;
		}
	
		if (nSTIndex >= 0)
		{
			m_Data.STBuckCase[nSTIndex] = data;
		}
	}

	SetItem(nIndex, data);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMStageAdditionalBuckDlg::OnCmdBtnDelete()
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_List.DeleteItem(nIndex);

	int nCase = m_Data.CSBucKCase.GetSize() + m_Data.nNumSTBuckCase;
	if (nIndex < m_Data.CSBucKCase.GetSize())
	{
		if (nIndex >= 0 && nIndex < m_Data.CSBucKCase.GetSize())
		{
			m_Data.CSBucKCase.RemoveAt(nIndex);
		}
	}
	else
	{
		for (int i = nIndex; i < m_Data.nNumSTBuckCase - 1; i++)
		{
			m_Data.STBuckCase[i] = m_Data.STBuckCase[i + 1];
		}
		m_Data.nNumSTBuckCase--;
	}

	// number after delete
	nCase = m_Data.CSBucKCase.GetSize() + m_Data.nNumSTBuckCase;
	if (nCase > 0)
	{
		if (nIndex >= nCase) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCMStageAdditionalBuckDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nIndex;
	T_CSBK_BASE data;
	if (!GetSelectedItem(nIndex))
	{
		data.Initialize();
		data.dblFactor = 1.0;
		data.nLoadType = 0;
	}
	else
	{
		if (nIndex < m_Data.CSBucKCase.GetSize())
		{
			data = m_Data.CSBucKCase[nIndex];
		}
		else
		{
			int nSTIndex = nIndex - m_Data.CSBucKCase.GetSize();
			if (nSTIndex >= 0 && nSTIndex < m_Data.nNumSTBuckCase)
			{
				data = m_Data.STBuckCase[nSTIndex];
			}
			else
			{
				data.Initialize();
				data.dblFactor = 1.0;
				data.nLoadType = 0;
			}
		}
	}

	// 값을 설정한다.
	Item2Dlg(data);

	*pResult = 0;
}

void CCMStageAdditionalBuckDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
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

void CCMStageAdditionalBuckDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	int i;

	BOOL bMFD = FALSE;
	BOOL bRefresh = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_STLD_ADD):
			// do nothing
			bRefresh = TRUE;
			break;
		case(UR_STLD_DEL):
			// 현재 loadcase list에서 지워진 것 삭제한다.
			pViewBuff->GetStld(nKey, Key, Data);
			for (i = m_Data.nNumSTBuckCase - 1; i >= 0; i--)
			{
				if (m_Data.STBuckCase[i].LoadCaseKey == Key)
				{
					m_List.DeleteItem(i);
					for (int j = i; j < m_Data.nNumSTBuckCase - 1; j++)
						m_Data.STBuckCase[j] = m_Data.STBuckCase[j + 1];
					m_Data.nNumSTBuckCase--;
				}
			}
			bRefresh = TRUE;
			break;
		case(UR_STLD_MFD):
			// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
			pViewBuff->GetStld(nKey, Key, Data);
			KeyBak = Key;
			DataBak = Data;
			bMFD = TRUE;
			bRefresh = TRUE;
			break;
		case(UR_STLD_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetStld(nKey, Key, Data);
			for (i = 0; i < m_Data.nNumSTBuckCase; i++)
			{
				if (m_Data.STBuckCase[i].LoadCaseKey == Key)
				{
					m_Data.STBuckCase[i].LoadCaseKey = KeyBak;
					SetItem(i, m_Data.STBuckCase[i]);
				}
			}
			// 플래그 수정
			bMFD = FALSE;
			bRefresh = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if (bRefresh)
	{
		CString csNumMode;
		csNumMode.Format(_T("%d"), m_Data.nNumMode);
		m_wndBuckNumEdit.SetWindowText(csNumMode);
		MakeItemEx();

		//T_STLD_K currentKey;
		//int nCSType;
		//BOOL bHadSelection = GetSelectedLoadCaseKey(currentKey, nCSType);

		InitializeLoadCaseCombo();

		//if (bHadSelection)
		//	SelectLoadCaseByKey(currentKey, nCSType);
	}
}

void CCMStageAdditionalBuckDlg::OnCmdRemoveData()
{
	m_bBucklingChk = FALSE;
	CDialogMove::OnOK();
}

void CCMStageAdditionalBuckDlg::OnCmdLoadFactor()
{
	EnableDisableCtrl();
}

void CCMStageAdditionalBuckDlg::EnableDisableCtrl()
{
	if (!CCMStageItemBase::IsCurStageBase())
	{
		CArray<UINT, UINT> Ctrls;
		Ctrls.Add(IDC_CMD_APPLY);
		CDlgUtil::CtrlShowHide(this,Ctrls,FALSE);

		Ctrls.RemoveAll();
		Ctrls.Add(IDC_CMD_REMOVE_DATA);
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
	}

	int nLoadFactor;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadFactor, nLoadFactor);

	if (nLoadFactor == 0)
	{
		GetDlgItem(IDC_CMD_SEARCH_FROM_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SEARCH_TO_EDT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SEARCH_FROM_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SEARCH_TO_EDT)->EnableWindow(TRUE);
	}
}

void CCMStageAdditionalBuckDlg::Data2Dlg()
{
	CString csNumMode,/* csIter, csConvTol,*/ csSearchFrom, csSearchTo;

	csNumMode.Format(_T("%d"), m_Data.nNumMode);
	m_wndBuckNumEdit.SetWindowText(csNumMode);
	m_wndBuckNumEdit.SetValue(m_Data.nNumMode);
	m_wndBuckNumEdit.SetRange(0, 1000);
	m_wndBuckNumEdit.SetInteger(TRUE);
	//csIter.Format(_T("%d"), m_Data.nNumItr);
	//csConvTol.Format(_T("%g"), m_Data.ConvTol);
	//m_wndIterEdit.SetWindowText(csIter);
	//m_wndIterEdit.SetRange(1, 1000);
	//m_wndConvTol.SetWindowText(csConvTol);

	csSearchFrom.Format(_T("%g"), m_Data.dLoadFactorFrom);
	m_wndSearchFrom.SetWindowText(csSearchFrom);

	csSearchTo.Format(_T("%g"), m_Data.dLoadFactorTo);
	m_wndSearchTo.SetWindowText(csSearchTo);

	m_bPositive = m_Data.bPositive;
	if (m_bPositive) CDlgUtil::CtrlRadioSetCheck(this, m_aLoadFactor, 0);
	else            CDlgUtil::CtrlRadioSetCheck(this, m_aLoadFactor, 1);

	m_bAxialForce.SetCheck(m_Data.bConsiderAxialOnly);

	m_bSturm.SetCheck(m_Data.bSturmSeq);

	m_wndScaleFactor.SetWindowText(_T("1"));

	m_nLoadCategory = 0;
	//m_nLoadType = 0;
}

void CCMStageAdditionalBuckDlg::Dlg2Data()
{
	CString csNumMode,/* csIter, csConvTol,*/ csSearchFrom, csSearchTo;
	m_wndBuckNumEdit.GetWindowText(csNumMode);
	m_wndSearchFrom.GetWindowText(csSearchFrom);
	m_wndSearchTo.GetWindowText(csSearchTo);
	//m_wndIterEdit.GetWindowText(csIter);
	//m_wndConvTol.GetWindowText(csConvTol);

	if (!CStrParser::GetINumber(csNumMode, m_Data.nNumMode)) return;
	if (!CStrParser::GetFloatNumber(csSearchFrom, m_Data.dLoadFactorFrom)) return;
	if (!CStrParser::GetFloatNumber(csSearchTo, m_Data.dLoadFactorTo)) return;
	// 	if(!CStrParser::GetINumber(csIter, m_Data.nNumItr)) return;
	// 	if(!CStrParser::GetFloatNumber(csConvTol, m_Data.ConvTol)) return;

	int nLoadFactor;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadFactor, nLoadFactor);
	if (nLoadFactor == 0) m_bPositive = TRUE;
	else                  m_bPositive = FALSE;

	m_Data.bPositive = m_bPositive;
	m_Data.bSturmSeq = m_bSturm.GetCheck();
	m_Data.bConsiderAxialOnly = m_bAxialForce.GetCheck();

	// 입력되지 않는 Case는 초기화한다.
	for (int i = m_Data.nNumSTBuckCase; i < D_CSBK_NUMBUCK; i++)
		m_Data.STBuckCase[i].Initialize();
}

