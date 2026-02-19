#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnExcelPrintDlg.h"

// #include "..\wg_base\PL_StrParser.h"
// #include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
// #include "..\wg_db\wg_db_DBLib.h"
// #include "..\wg_db\wg_db_DataCtrl.h"
// #include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
// #include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#define COMBO_EARTHQUAKE_TYPE_AXIS_DIR	_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_EARTHQUAKE_TYPE_AXIS_DIR)
#define COMBO_EARTHQUAKE_TYPE_PERP_DIR	_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_EARTHQUAKE_TYPE_PERP_DIR)
#define COMBO_SEISMIC_FORCE_TYPE1		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_SEISMIC_FORCE_TYPE1)
#define COMBO_SEISMIC_FORCE_TYPE2		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_SEISMIC_FORCE_TYPE2)
#define LIST_SEIS_PARAM_CASENAME		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_CASENAME)
#define LIST_SEIS_PARAM_SEIS_PERFORM	_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_SEIS_PERFORM)
#define LIST_SEIS_PARAM_EARTHQUAKE_TYPE _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_STATIC_EARTHQUAKE_TYPE)
#define LIST_SEIS_PARAM_LIMIT_TYPE		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_STATIC_LIMIT_TYPE)
#define LIST_SEIS_PARAM_DIR				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_DIR)
#define LIST_SEIS_PARAM_TYPE			_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_TYPE)
#define LIST_SEIS_PARAM_LOADCASE		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_LOADCASE)

CDgnExcelPrintDlg::CDgnExcelPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnExcelPrintDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_summarySize = 0;
}

CDgnExcelPrintDlg::~CDgnExcelPrintDlg()
{}

void CDgnExcelPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SEIS_LIST, m_loadPrintList);
	DDX_Radio(pDX, IDC_SEIS_SUMMARY_A3_RDO, m_summarySize);
}

BEGIN_MESSAGE_MAP(CDgnExcelPrintDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisBrgInfoDlg)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_NOTIFY(LVN_ITEMCHANGED,IDC_SEIS_LIST, CheckBoxCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnExcelPrintDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitLoadPrintListCtrl();
	InitButtonCtrl();
	return TRUE;
}

int CDgnExcelPrintDlg::GetCurrentDgnCode()
{
	if (m_pDoc == NULL) return 0;

	T_SIDC_D dSidc;
	dSidc.Initialize();
	if (m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc) == FALSE) return 0;

	return dSidc.nDesignCode;
}

void CDgnExcelPrintDlg::InitLoadPrintListCtrl()
{
	SetHeaderTitleListCtrl();
	m_loadPrintList.ModifyStyle(0, LVS_SHOWSELALWAYS);

	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->GetCount() < 1)
		return;

	m_loadPrintList.DeleteAllItems();
	m_loadPrintList.SetExtendedStyle(LVS_EX_CHECKBOXES);
	CArray<T_SIPA_K, T_SIPA_K> keys;
	m_pDoc->m_pAttrCtrl2-> GetQSipa()->GetKeyList(keys);
	// sort
	SortList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIPA_K& key = keys[i];
		T_SIPA_D data;
		m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(key, data);
		if (data.nDesignCode != GetCurrentDgnCode()) continue;

		InsertItemListCtrl(key, data);
	}
	// Init All Check
	for (int i = 0; i < m_loadPrintList.GetItemCount(); i++)
		m_loadPrintList.SetCheck(i, TRUE);
}

void CDgnExcelPrintDlg::SetHeaderTitleListCtrl()
{
	CArray<int, int> aColWidth;
	CArray<CString, CString> aTitle;
	int nColumnSize = 4;

	aColWidth.Add(100);
	aColWidth.Add(85);
	aColWidth.Add(55);
	aColWidth.Add(110);
	aTitle.Add(LIST_SEIS_PARAM_CASENAME);
	aTitle.Add(LIST_SEIS_PARAM_DIR);
	aTitle.Add(LIST_SEIS_PARAM_TYPE);
	aTitle.Add(LIST_SEIS_PARAM_LOADCASE);

	CString title;
	LV_COLUMN lvcolumn;
	DWORD dwStyle;

	dwStyle = ListView_GetExtendedListViewStyle(m_loadPrintList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_loadPrintList.GetSafeHwnd(), dwStyle);
 
	for (int i = 0; i < nColumnSize; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);

		m_loadPrintList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CDgnExcelPrintDlg::InsertItemListCtrl(T_SIPA_K Key, T_SIPA_D& Data)
{
	CArray<CString, CString> dataArray;
	int nColumnSize = 4;
	int nItem = m_loadPrintList.GetItemCount();
	for (int i = 0; i < nColumnSize; i++)
	{
		LVITEM lvItem;
		lvItem.iItem = nItem;
		lvItem.iSubItem = i;
		CString str;
		str = DataToStr(i, Data);
		lvItem.pszText = str.GetBuffer(0);
		lvItem.mask = LVIF_TEXT;

		if (i == 0)
		{
			lvItem.mask |= LVIF_PARAM;
			lvItem.lParam = (LPARAM)Key;
			nItem = m_loadPrintList.InsertItem(&lvItem);
		}
		else m_loadPrintList.SetItem(&lvItem);

		str.ReleaseBuffer();
	}

}

CString CDgnExcelPrintDlg::DataToStr(int i, T_SIPA_D& Data)
{
	CString str;
	switch (i)
	{
	case 0: //조사 케이스
		str = Data.strCaseName;
		break;
	case 1: //방향
		if (Data.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			str = COMBO_EARTHQUAKE_TYPE_AXIS_DIR;
		else if (Data.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
			str = COMBO_EARTHQUAKE_TYPE_PERP_DIR;
		else { ASSERT(0); }
		break;
	case 2://타입
		if (Data.nSeismicForceType == T_SIPA_D::kType1)
			str = COMBO_SEISMIC_FORCE_TYPE1;
		else if (Data.nSeismicForceType == T_SIPA_D::kType2)
			str = COMBO_SEISMIC_FORCE_TYPE2;
		else { ASSERT(0); }
		break;
	case 3://하중케이스
	{
		for (int i = 0; i < Data.aSeisThisK.GetSize(); i++)
		{
			T_THIS_D thisD;
			if (m_pDoc->m_pAttrCtrl->GetThis(Data.aSeisThisK[i], thisD) == FALSE)
				continue;
			if (i != 0)
				str = str + _T(",");

			str = str + thisD.LoadCaseName;
		}
		break;
	}
	default: ASSERT(0); break;
	}
		return str;
}

void CDgnExcelPrintDlg::SortList(CArray<T_SIPA_K, T_SIPA_K> &rkeys)
{
	CArray<T_SIPA_K, T_SIPA_K> typeOne;
	CArray<T_SIPA_K, T_SIPA_K> typeTwo;

	for (int i = 0; i < rkeys.GetCount(); i++)
	{
		T_SIPA_K key = rkeys[i];
		T_SIPA_D data;

		m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(key, data);
		if (data.nSeismicForceType == T_SIPA_D::kType1)
		{
			if (data.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) typeOne.InsertAt(0,key);
			else typeOne.Add(key);
		}
		else
		{
			if (data.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) typeTwo.InsertAt(0, key);
			else typeTwo.Add(key);
		}
	}

	for (int i = 0; i < typeTwo.GetCount(); i++)
	{
		typeOne.Add(typeTwo[i]);
	}

	rkeys.Copy(typeOne);
}

void CDgnExcelPrintDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CDgnExcelPrintDlg::OnOk()
{
	SetPrintSipaKeyList();
	CDialogMove::OnOK();
}

void CDgnExcelPrintDlg::InitButtonCtrl()
{
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DW)->EnableWindow(FALSE);

	CButton* pBtnSummarySize = (CButton*)GetDlgItem(IDC_SEIS_SUMMARY_A3_RDO);
	pBtnSummarySize->SetCheck(1);
}

void CDgnExcelPrintDlg::CheckBoxCheck(NMHDR* pNMHDR, LRESULT* pResult)
{
	int chkCount = 0; // 체크된 갯수
	for (int i = 0; i < m_loadPrintList.GetItemCount(); i++)
	{
		if (m_loadPrintList.GetCheck(i))
			chkCount++;
	}
	if (chkCount == 1)
	{
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DW)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DW)->EnableWindow(FALSE);
	}
}

void CDgnExcelPrintDlg::SetPrintSipaKeyList()
{
	m_aPrintSipaK.RemoveAll();
	for(int i = 0; i < m_loadPrintList.GetItemCount(); i++)
	{
		if(m_loadPrintList.GetCheck(i))
		{
			T_SIPA_K SipaK = (T_SIPA_K)m_loadPrintList.GetItemData(i);
			m_aPrintSipaK.Add(SipaK);
		}
	}
}

void CDgnExcelPrintDlg::GetPrintOptions(OUT SDgnExcelPrintOpt& rPrintOpt)
{
	rPrintOpt.aPrintSipaK.Copy(m_aPrintSipaK);
	rPrintOpt.summarySize = (SDgnExcelPrintOpt::eSummarySize)m_summarySize;
}