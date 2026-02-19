//////////
// DgnSeisSetParamDlg.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisSetParamDlg.h"

#include "..\wg_base\PL_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#define LIST_COLUMN_NUM_SEIS_PARAM_H14_H24	6
#define LIST_COLUMN_NUM_SEIS_PARAM_H29	7

#define COMBO_EARTHQUAKE_TYPE_AXIS_DIR	_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_EARTHQUAKE_TYPE_AXIS_DIR)
#define COMBO_EARTHQUAKE_TYPE_PERP_DIR	_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_EARTHQUAKE_TYPE_PERP_DIR)
#define COMBO_SEISMIC_FORCE_TYPE1		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_SEISMIC_FORCE_TYPE1)
#define COMBO_SEISMIC_FORCE_TYPE2		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_SEISMIC_FORCE_TYPE2)
#define COMBO_FLUCTUATING_NO_10			_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_FLUCTUATING_NO_10)
#define COMBO_ACCIDENTAL_NO_11			_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_ACCIDENTAL_NO_11)
#define COMBO_LIMIT_STATE1				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_LIMIT_STATE1)
#define COMBO_LIMIT_STATE2				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_LIMIT_STATE2)
#define COMBO_LIMIT_STATE3				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_LIMIT_STATE3)
#define LIST_SEIS_PARAM_CASENAME		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_CASENAME)
#define LIST_SEIS_PARAM_EXAM_TARGET		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_STATIC_SESMIC_EXAM_TARGET)
#define LIST_SEIS_PARAM_SEIS_PERFORM	_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_SEIS_PERFORM)
#define LIST_SEIS_PARAM_EARTHQUAKE_TYPE _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_STATIC_EARTHQUAKE_TYPE)
#define LIST_SEIS_PARAM_LIMIT_TYPE		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_STATIC_LIMIT_TYPE)
#define LIST_SEIS_PARAM_DIR				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_DIR)
#define LIST_SEIS_PARAM_TYPE			_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_TYPE)
#define LIST_SEIS_PARAM_LOADCASE		_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_LIST_SEIS_PARAM_LOADCASE)
#define RADIO_EXAM_TARGET1				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_EXISTING_SECT)
#define RADIO_EXAM_TARGET2				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_REINFORCE_SECT)
#define RADIO_SEIS_PERFORM1				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_PERFORM1)
#define RADIO_SEIS_PERFORM2				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_PERFORM2)
#define RADIO_SEIS_PERFORM3				_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_PERFORM3)

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetParamDlg
CDgnSeisSetParamDlg::CDgnSeisSetParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisSetParamDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_examTarget = 0;
	m_seisPerform2 = 0;

	m_aSesmicPerformanceCtrl.RemoveAll();
	m_aSesmicPerformanceCtrl.Add(IDC_STATIC_SESMIC_PERFORMANCE);
	m_aSesmicPerformanceCtrl.Add(IDC_RADIO_SEIS_PERFORM2);
	m_aSesmicPerformanceCtrl.Add(IDC_RADIO_SEIS_PERFORM3);

	m_aLimitStateOfBridgeCtrl.RemoveAll();
	m_aLimitStateOfBridgeCtrl.Add(IDC_STATIC_LIMIT_STATE_BRIDGE);
	m_aLimitStateOfBridgeCtrl.Add(IDC_STATIC_EARTHQUAKE_TYPE);
	m_aLimitStateOfBridgeCtrl.Add(IDC_COMBO_EARTHQUAKE_TYPE);
	m_aLimitStateOfBridgeCtrl.Add(IDC_STATIC_LIMIT_TYPE);
	m_aLimitStateOfBridgeCtrl.Add(IDC_COMBO_LIMIT_TYPE);

	m_aEarthquakeLoadCtrl.RemoveAll();
	m_aEarthquakeLoadCtrl.Add(IDC_STATIC_EARTHQUAKE_LOAD);
	m_aEarthquakeLoadCtrl.Add(IDC_STATIC_SEIS_FORCE_DIR);
	m_aEarthquakeLoadCtrl.Add(IDC_COMBO_SEIS_FORCE_DIR);
	m_aEarthquakeLoadCtrl.Add(IDC_STATIC_SEIS_FROCE_TYPE);
	m_aEarthquakeLoadCtrl.Add(IDC_COMBO_SEIS_FROCE_TYPE);
	m_aEarthquakeLoadCtrl.Add(IDC_STATIC_LOADCASE);
	m_aEarthquakeLoadCtrl.Add(IDC_LIST_LOADCASE);
	m_aEarthquakeLoadCtrl.Add(IDC_BUTTON_ADD);
	m_aEarthquakeLoadCtrl.Add(IDC_BUTTON_MODIFY);
	m_aEarthquakeLoadCtrl.Add(IDC_BUTTON_DELETE);
	m_aEarthquakeLoadCtrl.Add(IDC_LIST_SEIS_PARAM);

	m_aSystemButtonCtrl.RemoveAll();
	m_aSystemButtonCtrl.Add(IDOK);
	m_aSystemButtonCtrl.Add(IDCANCEL);

	m_deletedKeys.RemoveAll();
}

CDgnSeisSetParamDlg::~CDgnSeisSetParamDlg()
{
	m_pDoc = NULL;
}

void CDgnSeisSetParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlCodeDlg)
	DDX_Control(pDX, IDC_COMBO_DESIGN_CODE,		m_DesignCode);
	DDX_Control(pDX, IDC_EDIT_CASE_NAME,		m_caseName);
	DDX_Radio  (pDX, IDC_RADIO_SEIS_EXISTING_SECT, m_examTarget);
	DDX_Radio  (pDX, IDC_RADIO_SEIS_PERFORM2,	m_seisPerform2);
	DDX_Control(pDX, IDC_COMBO_EARTHQUAKE_TYPE, m_EarthquakeType);
	DDX_Control(pDX, IDC_COMBO_LIMIT_TYPE,		m_LimitType);
	DDX_Control(pDX, IDC_COMBO_SEIS_FORCE_DIR,	m_seisForceDir);
	DDX_Control(pDX, IDC_COMBO_SEIS_FROCE_TYPE,	m_seisFroceType);
	DDX_Control(pDX, IDC_LIST_LOADCASE,			m_loadCaseList);
	DDX_Control(pDX, IDC_LIST_SEIS_PARAM,		m_seisParamList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnSeisSetParamDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisBrgInfoDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DESIGN_CODE, OnSelectedChangedDesigncodeCmb)
	ON_CBN_SELCHANGE(IDC_COMBO_EARTHQUAKE_TYPE, OnSelectedChangedEarthquakeTypeCmb)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SEIS_PARAM, OnNMClickSeisParamList)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnAddBtn)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnModBtn)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnDelBtn)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnSeisSetParamDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	
	MoveSesmicPerformGroupCtrl();
	InitControl();
	InitDesignCodeComboBox();
	InitLoadCaseListCtrl();
	InitSeiaParamListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

#pragma region // Initailize Control
void CDgnSeisSetParamDlg::MoveSesmicPerformGroupCtrl()
{
	CRect rTarget, rSource;
	GetDlgItem(IDC_STATIC_LIMIT_STATE_BRIDGE)->GetWindowRect(rSource);
	GetDlgItem(IDC_STATIC_SESMIC_PERFORMANCE)->GetWindowRect(rTarget);

	int nDistX = rSource.left - rTarget.left/* + globalUtils.ScaleByDPI(8)*/;
	CDlgUtil::CtrlMoveDistX(this, m_aSesmicPerformanceCtrl, nDistX);
}

void CDgnSeisSetParamDlg::InitControl()
{
	CButton* pBtnExamTarget = (CButton*)GetDlgItem(IDC_RADIO_SEIS_EXISTING_SECT);
	pBtnExamTarget->SetCheck(1);
	CButton* pBtnSeisPerform = (CButton*)GetDlgItem(IDC_RADIO_SEIS_PERFORM2);
	pBtnSeisPerform->SetCheck(1);
	
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);

	m_seisForceDir.ResetContent();
	m_seisForceDir.AddString(COMBO_EARTHQUAKE_TYPE_AXIS_DIR);
	m_seisForceDir.AddString(COMBO_EARTHQUAKE_TYPE_PERP_DIR);
	m_seisForceDir.SetCurSel(0);

	m_seisFroceType.ResetContent();
	m_seisFroceType.AddString(COMBO_SEISMIC_FORCE_TYPE1);
	m_seisFroceType.AddString(COMBO_SEISMIC_FORCE_TYPE2);
	m_seisFroceType.SetCurSel(0);

	m_EarthquakeType.ResetContent();
	m_EarthquakeType.AddString(COMBO_FLUCTUATING_NO_10);
	m_EarthquakeType.AddString(COMBO_ACCIDENTAL_NO_11);
	m_EarthquakeType.SetCurSel(0);

	UpdateComboLimitType();
}

void CDgnSeisSetParamDlg::UpdateComboLimitType()
{
	int Index = m_EarthquakeType.GetCurSel();

	m_LimitType.ResetContent();
	switch (Index)
	{
	case 0: //작용의 종류가 "변동작용-No.10"인 경우, "한계상태1"
		m_LimitType.AddString(COMBO_LIMIT_STATE1);
		break;
	case 1:	//작용의 종류가 "우발작용-No.11"인 경우, "한계상태2", "한계상태3"
		m_LimitType.AddString(COMBO_LIMIT_STATE2);
		m_LimitType.AddString(COMBO_LIMIT_STATE3);
		break;
	default:
		ASSERT(FALSE);
		return;
	}

	m_LimitType.SetCurSel(0);
}

void CDgnSeisSetParamDlg::InitDesignCodeComboBox()
{
	m_DesignCode.ResetContent();
	m_DesignCode.EnableWindow(FALSE);

	CStringArray StlCodeName;
	CDBLib::GetBrdgSeisEvalCodeNameList_JP(StlCodeName);

	CString strStlCodeName = _T("");
	for (int i = 0; i < StlCodeName.GetSize(); i++)
	{
		strStlCodeName = CDBLib::ViewConvertCodeName(StlCodeName[i]);
#if defined(_RUS)
		CInitCtrl::ConvertEngRusDesignCode(strStlCodeName);
#endif
		m_DesignCode.AddString(strStlCodeName);
	}

	T_SIDC_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(rData);

	SetDesignCodeComboBox(rData.nDesignCode);
}

void CDgnSeisSetParamDlg::SetDesignCodeComboBox(const int& nDesignCode)
{
	CString strCode = CDBLib::GetBrdgSeisEvalCodeName_JP(nDesignCode);

#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(strCode);
#endif

	int Index = 0;
	if (strCode != _T(""))
	{
		Index = m_DesignCode.FindStringExact(-1, strCode);
	}
	else
	{
		Index = 0;
	}
	if (Index != -1)
	{
		m_DesignCode.SetCurSel(Index);
		UpdateControlByDesigncode();
	}
}

void CDgnSeisSetParamDlg::UpdateControlByDesigncode()
{
	CString strDgnCode = _T("");
	m_DesignCode.GetWindowText(strDgnCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strDgnCode);
#endif

	int nDesignCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(strDgnCode);
	switch (nDesignCode)
	{
	case JROAD5_H14:
	case JROAD5_H24:
		UpdateControlByH14_24();
		break;
	case JROAD5_H29:
		UpdateControlByH29();
		break;
	default:
		ASSERT(FALSE);
		return;
	}

	if (nDesignCode == JROAD5_H24 || nDesignCode == JROAD5_H29) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEIS_EXISTING_SECT);
		pButton->SetCheck(TRUE);
	}
	
	GetDlgItem(IDC_RADIO_SEIS_EXISTING_SECT)->EnableWindow(nDesignCode == JROAD5_H14);
	GetDlgItem(IDC_RADIO_SEIS_REINFORCE_SECT)->EnableWindow(nDesignCode == JROAD5_H14);
}

void CDgnSeisSetParamDlg::UpdateControlByH14_24()
{
	CDlgUtil::CtrlShowHide(this, m_aLimitStateOfBridgeCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aSesmicPerformanceCtrl, TRUE);

	CRect rTarget, rSource;
	GetDlgItem(IDC_STATIC_SESMIC_PERFORMANCE)->GetWindowRect(rSource);
	GetDlgItem(IDC_STATIC_EARTHQUAKE_LOAD)->GetWindowRect(rTarget);

	int nDistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(4);
	MoveBelowControl(nDistY);
}

void CDgnSeisSetParamDlg::UpdateControlByH29()
{
	CDlgUtil::CtrlShowHide(this, m_aSesmicPerformanceCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aLimitStateOfBridgeCtrl, TRUE);

	CRect rTarget, rSource;
	GetDlgItem(IDC_STATIC_LIMIT_STATE_BRIDGE)->GetWindowRect(rSource);
	GetDlgItem(IDC_STATIC_EARTHQUAKE_LOAD)->GetWindowRect(rTarget);

	int nDistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(4);
	MoveBelowControl(nDistY);
}

void CDgnSeisSetParamDlg::MoveBelowControl(const int& nDistY)
{
	CDlgUtil::CtrlMoveDistY(this, m_aEarthquakeLoadCtrl, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aSystemButtonCtrl, nDistY);

	//대화창 크기조정
	CRect rectCtrl;
	this->GetWindowRect(rectCtrl);
	this->SetWindowPos(NULL, 0, 0, rectCtrl.Width(), (rectCtrl.Height() + nDistY), SWP_NOMOVE | SWP_NOZORDER);
}

void CDgnSeisSetParamDlg::InitLoadCaseListCtrl()
{
	m_loadCaseList.ResetContent();

	CArray<T_THIS_K, T_THIS_K> keys;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_THIS_K& key = keys[i];
		T_THIS_D data;
		m_pDoc->m_pAttrCtrl->GetThis(key, data);
		if (data.nAnalMethod == D_THIS_ANALMETHOD::D_THIS_ANALMETHOD_STATIC) //정적 해석 케이스는 제외
			continue;

		int nIndex = m_loadCaseList.AddString(data.LoadCaseName);
		m_loadCaseList.SetItemData(nIndex, (DWORD_PTR)key);
		m_loadCaseList.SetCheck(nIndex, 0);
	}
}

void CDgnSeisSetParamDlg::InitSeiaParamListCtrl()
{
	CString strCode;
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	int nCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(strCode);

	SetHeaderTitleListCtrl(nCode);
	m_seisParamList.ModifyStyle(0, LVS_SHOWSELALWAYS);

	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->GetCount() < 1)
		return;

	m_seisParamList.DeleteAllItems();

	CArray<T_SIPA_K, T_SIPA_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIPA_K& key = keys[i];
		T_SIPA_D data;

		m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(key, data);

		if (nCode == data.nDesignCode)
			InsertItemListCtrl(key, data, nCode);
	}
}

void CDgnSeisSetParamDlg::SetHeaderTitleListCtrl(const int& nDesignCode)
{
	CArray<int, int> aColWidth;
	CArray<CString, CString> aTitle;
	int nColumnSize = 0;

	if (nDesignCode == JROAD5_H14 || nDesignCode == JROAD5_H24) {
		nColumnSize = LIST_COLUMN_NUM_SEIS_PARAM_H14_H24;
		aColWidth.Add(100);
		aColWidth.Add(90);
		aColWidth.Add(90);
		aColWidth.Add(70);
		aColWidth.Add(110);
		aColWidth.Add(70);
		aTitle.Add(LIST_SEIS_PARAM_CASENAME);
		aTitle.Add(LIST_SEIS_PARAM_SEIS_PERFORM);
		aTitle.Add(LIST_SEIS_PARAM_DIR);
		aTitle.Add(LIST_SEIS_PARAM_TYPE);
		aTitle.Add(LIST_SEIS_PARAM_LOADCASE);
		aTitle.Add(LIST_SEIS_PARAM_EXAM_TARGET);
	}
	else if (nDesignCode == JROAD5_H29) {
		nColumnSize = LIST_COLUMN_NUM_SEIS_PARAM_H29;
		aColWidth.Add(100);
		aColWidth.Add(70);
		aColWidth.Add(70);
		aColWidth.Add(90);
		aColWidth.Add(70);
		aColWidth.Add(110);
		aColWidth.Add(70);
		aTitle.Add(LIST_SEIS_PARAM_CASENAME);
		aTitle.Add(LIST_SEIS_PARAM_EARTHQUAKE_TYPE);
		aTitle.Add(LIST_SEIS_PARAM_LIMIT_TYPE);
		aTitle.Add(LIST_SEIS_PARAM_DIR);
		aTitle.Add(LIST_SEIS_PARAM_TYPE);
		aTitle.Add(LIST_SEIS_PARAM_LOADCASE);
		aTitle.Add(LIST_SEIS_PARAM_EXAM_TARGET);
	}

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_seisParamList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_seisParamList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < nColumnSize; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_seisParamList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

BOOL CDgnSeisSetParamDlg::InsertItemListCtrl(T_SIPA_K Key, T_SIPA_D& Data, const int& nDesignCode)
{
	int nColumnSize = 0;
	if (nDesignCode == JROAD5_H14 || nDesignCode == JROAD5_H24)
		nColumnSize = LIST_COLUMN_NUM_SEIS_PARAM_H14_H24;
	else if(nDesignCode == JROAD5_H29)
		nColumnSize = LIST_COLUMN_NUM_SEIS_PARAM_H29;

	int nItem = m_seisParamList.GetItemCount();
	for (int i = 0; i < nColumnSize; i++)
	{
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem = i;
		CString str;
		if(nDesignCode == JROAD5_H14 || nDesignCode == JROAD5_H24)
			str = DataToStr_H14H24(i, Data);
		else if(nDesignCode == JROAD5_H29)
			str = DataToStr_H29(i, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_seisParamList.InsertItem(&lvitem);
		}
		else m_seisParamList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

CString CDgnSeisSetParamDlg::DataToStr_H14H24(int i, T_SIPA_D& data)
{
	CString str = _T("");

	switch (i)
	{
	case 0://조사 케이스
		str = data.strCaseName;
		break;
	case 1://내진성능
		if (data.nSesmicPerformanceType == T_SIPA_D::kLevel1)
			str = RADIO_SEIS_PERFORM1;
		else if (data.nSesmicPerformanceType == T_SIPA_D::kLevel2)
			str = RADIO_SEIS_PERFORM2;
		else if (data.nSesmicPerformanceType == T_SIPA_D::kLevel3)
			str = RADIO_SEIS_PERFORM3;
		else { ASSERT(0); }
		break;
	case 2://방향
		if(data.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			str = COMBO_EARTHQUAKE_TYPE_AXIS_DIR;
		else if (data.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
			str = COMBO_EARTHQUAKE_TYPE_PERP_DIR;
		else { ASSERT(0); }
		break;
	case 3://타입
		if(data.nSeismicForceType == T_SIPA_D::kType1)
			str = COMBO_SEISMIC_FORCE_TYPE1;
		else if (data.nSeismicForceType == T_SIPA_D::kType2)
			str = COMBO_SEISMIC_FORCE_TYPE2;
		else { ASSERT(0); }
		break;
	case 4://하중 케이스
	{
		for (int idx = 0; idx < data.aSeisThisK.GetSize(); idx++)
		{
			T_THIS_D thisD;
			if(m_pDoc->m_pAttrCtrl->GetThis(data.aSeisThisK[idx], thisD) == FALSE)
				continue;

			if (idx != 0)
				str = str + _T(",");

			str = str + thisD.LoadCaseName;
		}
	}
		break;
	case 5://검토대상
		if (data.nExamTarget == T_SIPA_D::kExistingSection)
			str = RADIO_EXAM_TARGET1;
		else if (data.nExamTarget == T_SIPA_D::kReinforceSection)
			str = RADIO_EXAM_TARGET2;
		else { ASSERT(0); }
		break;
	default: ASSERT(0); break;
	}

	return str;
}

CString CDgnSeisSetParamDlg::DataToStr_H29(int i, T_SIPA_D& data)
{
	CString str = _T("");

	switch (i)
	{
	case 0://조사 케이스
		str = data.strCaseName;
		break;
	case 1://작용의 종류
		if (data.nEarthquakeType == T_SIPA_D::kFluctuating_No10)
			str = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_FLUCTUATING_NO_10);
		else if (data.nEarthquakeType == T_SIPA_D::eEarthquakeType::kAccidental_No11)
			str = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_ACCIDENTAL_NO_11);
		else { ASSERT(0); }
		break;
	case 2://한계상태
		if (data.nLimitType == T_SIPA_D::eLimitStateType::kLimitState1)
			str = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_LIMIT_STATE1);
		else if (data.nLimitType == T_SIPA_D::eLimitStateType::kLimitState2)
			str = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_LIMIT_STATE2);
		else if (data.nLimitType == T_SIPA_D::eLimitStateType::kLimitState3)
			str = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_COMBO_LIMIT_STATE3);
		else { ASSERT(0); }
		break;
	case 3://방향
		if (data.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			str = COMBO_EARTHQUAKE_TYPE_AXIS_DIR;
		else if (data.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
			str = COMBO_EARTHQUAKE_TYPE_PERP_DIR;
		else { ASSERT(0); }
		break;
	case 4://타입
		if (data.nSeismicForceType == T_SIPA_D::kType1)
			str = COMBO_SEISMIC_FORCE_TYPE1;
		else if (data.nSeismicForceType == T_SIPA_D::kType2)
			str = COMBO_SEISMIC_FORCE_TYPE2;
		else { ASSERT(0); }
		break;
	case 5://하중 케이스
	{
		for (int idx = 0; idx < data.aSeisThisK.GetSize(); idx++)
		{
			T_THIS_D thisD;
			if (m_pDoc->m_pAttrCtrl->GetThis(data.aSeisThisK[idx], thisD) == FALSE)
				continue;

			if (idx != 0)
				str = str + _T(",");

			str = str + thisD.LoadCaseName;
		}
	}
	break;
	case 6://검토대상
		if (data.nExamTarget == T_SIPA_D::kExistingSection)
			str = RADIO_EXAM_TARGET1;
		else if (data.nExamTarget == T_SIPA_D::kReinforceSection)
			str = RADIO_EXAM_TARGET2;
		else { ASSERT(0); }
		break;
	default: ASSERT(0); break;
	}

	return str;
}
#pragma endregion

void CDgnSeisSetParamDlg::DlgToData(T_SIPA_D& rData)
{
	CString str;
	rData.Initialize();
	
	//설계 코드
	m_DesignCode.GetWindowText(str);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(str);
#endif
	rData.nDesignCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(str);
	//조사 케이스명
	m_caseName.GetWindowText(rData.strCaseName);
	//교량의 내진성능
	rData.nSesmicPerformanceType = T_SIPA_D::kLevel0;
	if (rData.nDesignCode == JROAD5_H14 || rData.nDesignCode == JROAD5_H24) {
		if(m_seisPerform2 == 0)
			rData.nSesmicPerformanceType = T_SIPA_D::kLevel2;
		else
			rData.nSesmicPerformanceType = T_SIPA_D::kLevel3;
	}
	//교량의 한계상태 - 작용의 종류
	m_EarthquakeType.GetWindowText(str);
	rData.nEarthquakeType = T_SIPA_D::kUnKnown;
	if (rData.nDesignCode == JROAD5_H29) {
		if (str == COMBO_FLUCTUATING_NO_10)
			rData.nEarthquakeType = T_SIPA_D::kFluctuating_No10;
		else if (str == COMBO_ACCIDENTAL_NO_11)
			rData.nEarthquakeType = T_SIPA_D::kAccidental_No11;
	}
	//교량의 한계상태 - 한계 상태
	m_LimitType.GetWindowText(str);
	rData.nLimitType = T_SIPA_D::kLimitState0;
	if (rData.nDesignCode == JROAD5_H29) {
		if (str == COMBO_LIMIT_STATE1)
			rData.nLimitType = T_SIPA_D::kLimitState1;
		else if (str == COMBO_LIMIT_STATE2)
			rData.nLimitType = T_SIPA_D::kLimitState2;
		else if (str == COMBO_LIMIT_STATE3)
			rData.nLimitType = T_SIPA_D::kLimitState3;
	}
	//지진 하중 - 지진력 방향
	m_seisForceDir.GetWindowText(str);
	rData.nSeismicForceDirection = T_SIPA_D::kUnKnown_Dir;
	if (str == COMBO_EARTHQUAKE_TYPE_AXIS_DIR)
		rData.nSeismicForceDirection = T_SIPA_D::kAxis_Dir;
	else if (str == COMBO_EARTHQUAKE_TYPE_PERP_DIR)
		rData.nSeismicForceDirection = T_SIPA_D::kPerp_Dir;
	//지진 하중 - 하중 타입
	m_seisFroceType.GetWindowText(str);
	rData.nSeismicForceType = T_SIPA_D::kType0;
	if (str == COMBO_SEISMIC_FORCE_TYPE1)
		rData.nSeismicForceType = T_SIPA_D::kType1;
	else if (str == COMBO_SEISMIC_FORCE_TYPE2)
		rData.nSeismicForceType = T_SIPA_D::kType2;
	//검토대상
	rData.nExamTarget = T_SIPA_D::kExistingSection;
	if (rData.nDesignCode == JROAD5_H14) {
		if (m_examTarget == 0)
			rData.nExamTarget = T_SIPA_D::kExistingSection;
		else
			rData.nExamTarget = T_SIPA_D::kReinforceSection;
	}
	//동적 하중 케이스
	GetThisKeys(rData.aSeisThisK);
}

void CDgnSeisSetParamDlg::GetThisKeys(CArray<T_THIS_K, T_THIS_K>& rKeys)
{
	int nCount = m_loadCaseList.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if(m_loadCaseList.GetCheck(i) != 1)
			continue;

		T_THIS_K key = m_loadCaseList.GetItemData(i);
		rKeys.Add(key);
	}
}

BOOL CDgnSeisSetParamDlg::CheckAddedData(IN T_SIPA_D& data)
{
	if (m_seisParamList.GetItemCount() > 3)//4개만 등록가능
	{
		CString msg;
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_UNABLE_EXCEED_LIMIT_SEISPARM);
		AfxMessageBox(msg);
		return FALSE;
	}

	CString msg;
	//로드 케이스 선택 확인
	if (data.aSeisThisK.GetSize() < 1) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_EMPTY_LOADCASE);
		AfxMessageBox(msg);
		return FALSE;
	}

	if (data.aSeisThisK.GetCount() > 3) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_UNABLE_EXCEED_LIMIT_LOADCASE);
		AfxMessageBox(msg);
		return FALSE;
	}

	//조사 케이스명 공백체크
	if (data.strCaseName.IsEmpty() == true) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_EMPTY_CASENAME);
		AfxMessageBox(msg);
		return FALSE;
	}

	//기존 등록된 내진성능과 같은 타입인지 확인
	if (IsSameSeismicPerformanceType(data) == FALSE) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_DIFFERENT_SEIS_PERFORM);
		AfxMessageBox(msg);
		return FALSE;
	}

	//지진력방향 + 지진파 타입의 조합이 중복되는지 확인
	if (IsDuplicatedSeismicForceType(data) == FALSE) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_DUPLI_SEIS_FORCE_TYPE);
		AfxMessageBox(msg);
		return FALSE;
	}

	//조사 케이스명 중복체크
	if (IsDuplicatedCaseName(data.strCaseName) == TRUE) {
		msg.Format(_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_DUPLI_CASENAME), data.strCaseName);
		AfxMessageBox(msg);
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnSeisSetParamDlg::CheckModifiedData(const int& nSelItem, IN T_SIPA_D& modifyData)
{
	CString str;
	m_DesignCode.GetWindowText(str);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(str);
#endif
	int nCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(str);

	T_SIPA_D oldData; oldData.Initialize();
	oldData.nDesignCode = nCode;

	if (nCode == JROAD5_H14 || nCode == JROAD5_H24)
		ListToData_H14H24(nSelItem, oldData);
	else if (nCode == JROAD5_H29)
		ListToData_H29(nSelItem, oldData);

	CString msg;
	//로드 케이스 선택 확인
	if (modifyData.aSeisThisK.GetSize() < 1) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_EMPTY_LOADCASE);
		AfxMessageBox(msg);
		return FALSE;
	}

	if (modifyData.aSeisThisK.GetCount() > 3) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_UNABLE_EXCEED_LIMIT_LOADCASE);
		AfxMessageBox(msg);
		return FALSE;
	}

	//조사 케이스명 공백체크
	if (modifyData.strCaseName.IsEmpty() == true) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_EMPTY_CASENAME);
		AfxMessageBox(msg);
		return FALSE;
	}

	BOOL bChangedCaseName = oldData.strCaseName != modifyData.strCaseName;
	
	if (bChangedCaseName == TRUE) {
		//조사 케이스명 중복체크
		if (IsDuplicatedCaseName(modifyData.strCaseName) == TRUE) {
			msg.Format(_LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_DUPLI_CASENAME), modifyData.strCaseName);
			AfxMessageBox(msg);
			return FALSE;
		}
	}

	//지진력방향 + 지진파 타입의 조합이 중복되는지 확인
	if (IsDuplicatedSeismicForceType(modifyData, nSelItem) == FALSE) {
		msg = _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_MSG_DUPLI_SEIS_FORCE_TYPE);
		AfxMessageBox(msg);
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnSeisSetParamDlg::IsDuplicatedCaseName(IN CString& strCaseName)
{
	CString msg;
	//조사 케이스명 중복체크
	int nItem;
	for (nItem = 0; nItem < m_seisParamList.GetItemCount(); nItem++)
	{
		CString str = m_seisParamList.GetItemText(nItem, 0);//조사 케이스명
		if (str == strCaseName)
			return TRUE;
	}

	return FALSE;
}

BOOL CDgnSeisSetParamDlg::IsSameSeismicPerformanceType(const T_SIPA_D& data)
{
	if (data.nDesignCode != BrdgSE_CodeJP::JROAD5_H14 && data.nDesignCode != BrdgSE_CodeJP::JROAD5_H24) return TRUE;

	CString strSeisPerform = _T("");
	if (data.nSesmicPerformanceType == T_SIPA_D::kLevel1)
		strSeisPerform = RADIO_SEIS_PERFORM1;
	else if (data.nSesmicPerformanceType == T_SIPA_D::kLevel2)
		strSeisPerform = RADIO_SEIS_PERFORM2;
	else if (data.nSesmicPerformanceType == T_SIPA_D::kLevel3)
		strSeisPerform = RADIO_SEIS_PERFORM3;

	int nItem;
	for (nItem = 0; nItem < m_seisParamList.GetItemCount(); nItem++)
	{
		CString str = m_seisParamList.GetItemText(nItem, 1);//내진성능
		if(strSeisPerform != str)
			return FALSE;
	}

	return TRUE;
}

BOOL CDgnSeisSetParamDlg::IsDuplicatedSeismicForceType(const T_SIPA_D& data, const int& nSelItem)
{
	CString str;

	if (data.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
		str = COMBO_EARTHQUAKE_TYPE_AXIS_DIR;
	else if (data.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
		str = COMBO_EARTHQUAKE_TYPE_PERP_DIR;

	if (data.nSeismicForceType == T_SIPA_D::kType1)
		str = str + COMBO_SEISMIC_FORCE_TYPE1;
	else if (data.nSeismicForceType == T_SIPA_D::kType2)
		str = str + COMBO_SEISMIC_FORCE_TYPE2;


	int nItem;
	for (nItem = 0; nItem < m_seisParamList.GetItemCount(); nItem++)
	{
		if(nSelItem == nItem)
			continue;

		CString strDir = m_seisParamList.GetItemText(nItem, 2);//방향
		CString strType = m_seisParamList.GetItemText(nItem, 3);//타입
		
		CString strCompare = strDir + strType;

		if (strCompare == str)
			return FALSE;
	}

	return TRUE;
}

BOOL CDgnSeisSetParamDlg::InsertItem(int nIndex, T_SIPA_D& data)
{
	CString str;
	m_DesignCode.GetWindowText(str);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(str);
#endif
	int nCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(str);

	int nSize = 0;
	if (nCode == JROAD5_H14 || nCode == JROAD5_H24)
		nSize = LIST_COLUMN_NUM_SEIS_PARAM_H14_H24;
	else if (nCode == JROAD5_H29)
		nSize = LIST_COLUMN_NUM_SEIS_PARAM_H29;

	LVITEM lvitem;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < nSize; i++)
	{
		lvitem.iSubItem = i;
		if (nCode == JROAD5_H14 || nCode == JROAD5_H24)
			str = DataToStr_H14H24(i, data);
		else if(nCode == JROAD5_H29)
			str = DataToStr_H29(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
			nItem = m_seisParamList.InsertItem(&lvitem);
		else
			m_seisParamList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CDgnSeisSetParamDlg::ModifyItem(int nIndex, T_SIPA_D& data)
{
	CString str;
	m_DesignCode.GetWindowText(str);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(str);
#endif
	int nCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(str);

	int nSize = 0;
	if (nCode == JROAD5_H14 || nCode == JROAD5_H24)
		nSize = LIST_COLUMN_NUM_SEIS_PARAM_H14_H24;
	else if (nCode == JROAD5_H29)
		nSize = LIST_COLUMN_NUM_SEIS_PARAM_H29;

	int nItem = nIndex;
	if (nItem != -1)
	{
		for (int i = 0; i < nSize; i++)
		{
			if (nCode == JROAD5_H14 || nCode == JROAD5_H24)
				str = DataToStr_H14H24(i, data);
			else if (nCode == JROAD5_H29)
				str = DataToStr_H29(i, data);
			m_seisParamList.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

void CDgnSeisSetParamDlg::DeleteItem(int nIndex)
{
	m_seisParamList.DeleteItem(nIndex);
}

void CDgnSeisSetParamDlg::UpdataDialog(const int& nItem)
{
	CString str;
	m_DesignCode.GetWindowText(str);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(str);
#endif
	int nCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(str);

	if (nCode == JROAD5_H14 || nCode == JROAD5_H24) {
		UpdataDialog_H14H24(nItem);
	}
	else if (nCode == JROAD5_H29) {
		UpdataDialog_H29(nItem);
	}
}

void CDgnSeisSetParamDlg::UpdataDialog_H14H24(const int& nItem)
{
	CString str;
	//조사 케이스명
	str = m_seisParamList.GetItemText(nItem, 0);
	m_caseName.SetWindowText(str);

	//교량의 내진성능
	str = m_seisParamList.GetItemText(nItem, 1);

	if (str == RADIO_SEIS_PERFORM2) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEIS_PERFORM2);
		pButton->SetCheck(TRUE);
	}
	else if (str == RADIO_SEIS_PERFORM3) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEIS_PERFORM3);
		pButton->SetCheck(TRUE);
	}
	else { ASSERT(FALSE); }

	//지진하중 - 지진력 방향
	str = m_seisParamList.GetItemText(nItem, 2);
	if (str.IsEmpty() == false) {
		int iIndex = m_seisForceDir.FindStringExact(-1, str);
		if (iIndex >= 0) m_seisForceDir.SetCurSel(iIndex);
		else			 m_seisForceDir.SetCurSel(0);
	}

	//지진 하중 - 하중 타입
	str = m_seisParamList.GetItemText(nItem, 3);
	if (str.IsEmpty() == false) {
		int iIndex = m_seisFroceType.FindStringExact(-1, str);
		if (iIndex >= 0) m_seisFroceType.SetCurSel(iIndex);
		else			 m_seisFroceType.SetCurSel(0);
	}

	//동적 하중 케이스
	str = m_seisParamList.GetItemText(nItem, 4);

	T_THIS_K_LIST aSeisThisK;
	GetLoadCaseKeys(str, aSeisThisK);

	for (int i = 0; i < m_loadCaseList.GetCount(); i++)
	{
		T_THIS_K key = m_loadCaseList.GetItemData(i);
		INT_PTR nIndex = aSeisThisK.FindIndex(key);
		m_loadCaseList.SetCheck(i, (nIndex >= 0));
		m_loadCaseList.SetSel(i, (nIndex >= 0));
	}

	//검토대상
	str = m_seisParamList.GetItemText(nItem, 5);
	if (str == RADIO_EXAM_TARGET1) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEIS_EXISTING_SECT);
		pButton->SetCheck(TRUE);
	}
	else if (str == RADIO_EXAM_TARGET2) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEIS_REINFORCE_SECT);
		pButton->SetCheck(TRUE);
	}
	else { ASSERT(FALSE); }
}

void CDgnSeisSetParamDlg::UpdataDialog_H29(const int& nItem)
{
	CString str;
	//조사 케이스명
	str = m_seisParamList.GetItemText(nItem, 0);
	m_caseName.SetWindowText(str);

	//교량의 한계상태 - 작용의 종류
	str = m_seisParamList.GetItemText(nItem, 1);
	if (str.IsEmpty() == false) {
		int iIndex = m_EarthquakeType.FindStringExact(-1, str);
		if (iIndex >= 0) m_EarthquakeType.SetCurSel(iIndex);
		else			 m_EarthquakeType.SetCurSel(0);
	}

	//교량의 한계상태 - 한계 상태
	UpdateComboLimitType();
	str = m_seisParamList.GetItemText(nItem, 2);
	if (str.IsEmpty() == false) {
		int iIndex = m_LimitType.FindStringExact(-1, str);
		if (iIndex >= 0) m_LimitType.SetCurSel(iIndex);
		else			 m_LimitType.SetCurSel(0);
	}

	//지진하중 - 지진력 방향
	str = m_seisParamList.GetItemText(nItem, 3);
	if (str.IsEmpty() == false) {
		int iIndex = m_seisForceDir.FindStringExact(-1, str);
		if (iIndex >= 0) m_seisForceDir.SetCurSel(iIndex);
		else			 m_seisForceDir.SetCurSel(0);
	}

	//지진 하중 - 하중 타입
	str = m_seisParamList.GetItemText(nItem, 4);
	if (str.IsEmpty() == false) {
		int iIndex = m_seisFroceType.FindStringExact(-1, str);
		if (iIndex >= 0) m_seisFroceType.SetCurSel(iIndex);
		else			 m_seisFroceType.SetCurSel(0);
	}

	//동적 하중 케이스
	str = m_seisParamList.GetItemText(nItem, 5);

	T_THIS_K_LIST aSeisThisK;
	GetLoadCaseKeys(str, aSeisThisK);

	for (int i = 0; i < m_loadCaseList.GetCount(); i++)
	{
		T_THIS_K key = m_loadCaseList.GetItemData(i);
		INT_PTR nIndex = aSeisThisK.FindIndex(key);
		m_loadCaseList.SetCheck(i, (nIndex >= 0));
		m_loadCaseList.SetSel(i, (nIndex >= 0));
	}


	//검토대상
	str = m_seisParamList.GetItemText(nItem, 6);
	if (str == RADIO_EXAM_TARGET1) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEIS_EXISTING_SECT);
		pButton->SetCheck(TRUE);
	}
	else if (str == RADIO_EXAM_TARGET2) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEIS_REINFORCE_SECT);
		pButton->SetCheck(TRUE);
	}
	else { ASSERT(FALSE); }

}

#pragma region // Event Function
void CDgnSeisSetParamDlg::OnSelectedChangedDesigncodeCmb()
{
	UpdateControlByDesigncode();

	m_caseName.SetWindowText(_T(""));

	CString strDgnCode = _T("");
	m_DesignCode.GetWindowText(strDgnCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strDgnCode);
#endif
	int nDesignCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(strDgnCode);

	InitSeiaParamListCtrl();

	for (int i = 0; i < m_loadCaseList.GetCount(); i++)
	{
		m_loadCaseList.SetCheck(i, FALSE);
		m_loadCaseList.SetSel(i, FALSE);
	}
}

void CDgnSeisSetParamDlg::OnAddBtn()
{
	UpdateData(TRUE);

	T_SIPA_D data;
	DlgToData(data);

	if (CheckAddedData(data) == FALSE)
		return;

	int nCount = m_seisParamList.GetItemCount();
	InsertItem(nCount, data);
}

void CDgnSeisSetParamDlg::OnModBtn()
{
	int nItem = m_seisParamList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	UpdateData(TRUE);

	T_SIPA_D data;
	DlgToData(data);

	if (CheckModifiedData(nItem, data) == FALSE)
		return;

	ModifyItem(nItem, data);
}

void CDgnSeisSetParamDlg::OnDelBtn()
{
	int nItem = m_seisParamList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	T_SIPA_K key = (T_SIPA_K)m_seisParamList.GetItemData(nItem);
	//기존에 등록된 데이터만 삭제 key list 에 넣는다.
	BOOL bExist = m_pDoc->m_pAttrCtrl2->GetQSipa()->Exist(key);
	if(bExist == TRUE)
		m_deletedKeys.Add(key);

	UpdateData(TRUE);
	DeleteItem(nItem);
}

/**
* 도시-H29 일때 교량의 한계상태 - 작용의 종류 콤보박스 값 변경시
*/
void CDgnSeisSetParamDlg::OnSelectedChangedEarthquakeTypeCmb()
{
	UpdateComboLimitType();//교량의 한계상태 - 한계상태 콤보박스 값 변경
}

void CDgnSeisSetParamDlg::OnNMClickSeisParamList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1)
	{
		*pResult = 0;
		//m_caseName.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		return;
	}

	int nItem = pNMListView->iItem;
	if (nItem == -1) {
		*pResult = 0;
		return;
	}
	
	UpdataDialog(nItem);

	//m_caseName.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
}

void CDgnSeisSetParamDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CDgnSeisSetParamDlg::OnOk()
{
	CString str;
	m_DesignCode.GetWindowText(str);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(str);
#endif
	int nCode = CDBLib::GetBrdgSeisEvalCodeNo_JP(str);

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Modify_SIPA)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return;

	BOOL bResult = FALSE;
	//삭제된 데이터 처리
	for (int i = 0; i < m_deletedKeys.GetCount(); i++)
	{
		if (m_pDoc->m_pEditData->DelSipa(m_deletedKeys[i]) == FALSE) {
			ASSERT(FALSE);
			m_pDoc->m_pUndoCtrl->CancelEditDB();
			return;
		}
	}
	//추가 및 변경
	int nSize = m_seisParamList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		T_SIPA_K key = (T_SIPA_K)m_seisParamList.GetItemData(nItem);
		T_SIPA_D data;
		data.Initialize();
		data.nDesignCode = nCode;

		if (nCode == JROAD5_H14 || nCode == JROAD5_H24)
			ListToData_H14H24(nItem, data);
		else if (nCode == JROAD5_H29)
			ListToData_H29(nItem, data);

		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Exist(key) == TRUE) {
			if (m_pDoc->m_pEditData->ModifySipa(key, data) == FALSE) {
				ASSERT(FALSE);
				m_pDoc->m_pUndoCtrl->CancelEditDB();
				return;
			}
		}
		else {
			key = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetStartNum();
			if(m_pDoc->m_pEditData->AddSipa(key, data) == FALSE) {
				ASSERT(FALSE);
				m_pDoc->m_pUndoCtrl->CancelEditDB();
				return;
			}
		}
	}

	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIPA_CMD);
	CDialogMove::OnOK();
}

void CDgnSeisSetParamDlg::ListToData_H14H24(const int& nItem, T_SIPA_D& rData)
{
	CString str;
	//조사 케이스명
	rData.strCaseName = m_seisParamList.GetItemText(nItem, 0);
	//교량의 내진성능
	str = m_seisParamList.GetItemText(nItem, 1);
	if (str == RADIO_SEIS_PERFORM1)
		rData.nSesmicPerformanceType = T_SIPA_D::kLevel1;
	else if (str == RADIO_SEIS_PERFORM2)
		rData.nSesmicPerformanceType = T_SIPA_D::kLevel2;
	else if (str == RADIO_SEIS_PERFORM3)
		rData.nSesmicPerformanceType = T_SIPA_D::kLevel3;
	//지진하중 - 지진력 방향
	str = m_seisParamList.GetItemText(nItem, 2);
	if (str == COMBO_EARTHQUAKE_TYPE_AXIS_DIR)
		rData.nSeismicForceDirection = T_SIPA_D::kAxis_Dir;
	else if (str == COMBO_EARTHQUAKE_TYPE_PERP_DIR)
		rData.nSeismicForceDirection = T_SIPA_D::kPerp_Dir;
	//지진 하중 - 하중 타입
	str = m_seisParamList.GetItemText(nItem, 3);
	if (str == COMBO_SEISMIC_FORCE_TYPE1)
		rData.nSeismicForceType = T_SIPA_D::kType1;
	else if (str == COMBO_SEISMIC_FORCE_TYPE2)
		rData.nSeismicForceType = T_SIPA_D::kType2;
	//동적 하중 케이스
	str = m_seisParamList.GetItemText(nItem, 4);
	GetLoadCaseKeys(str, rData.aSeisThisK);
	//검토대상
	str = m_seisParamList.GetItemText(nItem, 5);
	if (str == RADIO_EXAM_TARGET1)
		rData.nExamTarget = T_SIPA_D::kExistingSection;
	else if (str == RADIO_EXAM_TARGET2)
		rData.nExamTarget = T_SIPA_D::kReinforceSection;
}

void CDgnSeisSetParamDlg::ListToData_H29(const int& nItem, T_SIPA_D& rData)
{
	CString str;
	//조사 케이스명
	rData.strCaseName = m_seisParamList.GetItemText(nItem, 0);
	//교량의 한계상태 - 작용의 종류
	str = m_seisParamList.GetItemText(nItem, 1);
	if (str == COMBO_FLUCTUATING_NO_10)
		rData.nEarthquakeType = T_SIPA_D::kFluctuating_No10;
	else if (str == COMBO_ACCIDENTAL_NO_11)
		rData.nEarthquakeType = T_SIPA_D::kAccidental_No11;
	//교량의 한계상태 - 한계 상태
	str = m_seisParamList.GetItemText(nItem, 2);
	if (str == COMBO_LIMIT_STATE1)
		rData.nLimitType = T_SIPA_D::kLimitState1;
	else if (str == COMBO_LIMIT_STATE2)
		rData.nLimitType = T_SIPA_D::kLimitState2;
	else if (str == COMBO_LIMIT_STATE3)
		rData.nLimitType = T_SIPA_D::kLimitState3;
	//지진하중 - 지진력 방향
	str = m_seisParamList.GetItemText(nItem, 3);
	if (str == COMBO_EARTHQUAKE_TYPE_AXIS_DIR)
		rData.nSeismicForceDirection = T_SIPA_D::kAxis_Dir;
	else if (str == COMBO_EARTHQUAKE_TYPE_PERP_DIR)
		rData.nSeismicForceDirection = T_SIPA_D::kPerp_Dir;
	//지진 하중 - 하중 타입
	str = m_seisParamList.GetItemText(nItem, 4);
	if (str == COMBO_SEISMIC_FORCE_TYPE1)
		rData.nSeismicForceType = T_SIPA_D::kType1;
	else if (str == COMBO_SEISMIC_FORCE_TYPE2)
		rData.nSeismicForceType = T_SIPA_D::kType2;
	//동적 하중 케이스
	str = m_seisParamList.GetItemText(nItem, 5);
	GetLoadCaseKeys(str, rData.aSeisThisK);
	//검토대상
	str = m_seisParamList.GetItemText(nItem, 6);
	if (str == RADIO_EXAM_TARGET1)
		rData.nExamTarget = T_SIPA_D::kExistingSection;
	else if (str == RADIO_EXAM_TARGET2)
		rData.nExamTarget = T_SIPA_D::kReinforceSection;
}

void CDgnSeisSetParamDlg::GetLoadCaseKeys(const CString& str, T_KEY_LIST& rThisKeys)
{
	CStringArray results;
	GetLoadCaseNames(str, results);

	for (int i = 0; i < results.GetCount(); i++)
	{
		CString strLoadCase = results[i];
		T_THIS_K key = m_pDoc->m_pAttrCtrl->GetThisKey(strLoadCase);
		rThisKeys.Add(key);
	}
}

void CDgnSeisSetParamDlg::GetLoadCaseNames(const CString& str, CStringArray& rNames)
{
	if (str.Find(_T(","), 0) < 0) {
		rNames.Add(str);
	}
	else {
		CString delimiter = _T(",");
		CStrParser::SplitString(str, delimiter, rNames);
	}
}
#pragma endregion