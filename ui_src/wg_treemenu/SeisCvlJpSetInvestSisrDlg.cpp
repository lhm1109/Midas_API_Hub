// CSeisCvlJpSetInvestSisrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestSisrDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LIST_COLUMN_NUM_USER 6

#define COMBO_DESIGN_CRITERIA_ROAD3	_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_AUTO_DESI_CRIT_CMB_ROAD3)
#define COMBO_DESIGN_CRITERIA_ROAD4	_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_AUTO_DESI_CRIT_CMB_ROAD4)
#define COMBO_DESIGN_CRITERIA_ROAD5	_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_AUTO_DESI_CRIT_CMB_ROAD5)

#define COMBO_SHEAR_SPAN_TYPE_COLUMN_LENGTH		_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_CMB_COLUMN_LENGTH)
#define COMBO_SHEAR_SPAN_TYPE_INERTIAL_FORCE	_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_CMB_INERTIAL_FORCE)
#define COMBO_SHEAR_SPAN_TYPE_USER_INPUT		_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_CMB_USER_INPUT)

#define LIST_HEAD_POS	_LS(IDS_IDD_TM_SISR_IDC_LIST_HEAD_POS)
#define LIST_HEAD_ELEM	_LS(IDS_IDD_TM_SISR_IDC_LIST_HEAD_ELEM)
#define LIST_HEAD_PLUS_SUS	_LS(IDS_IDD_TM_SISR_IDC_LIST_HEAD_PLUS_SUS)
#define LIST_HEAD_PLUS_SUC	_LS(IDS_IDD_TM_SISR_IDC_LIST_HEAD_PLUS_SUC)
#define LIST_HEAD_MINUS_SUS	_LS(IDS_IDD_TM_SISR_IDC_LIST_HEAD_MINUS_SUS)
#define LIST_HEAD_MINUS_SUC	_LS(IDS_IDD_TM_SISR_IDC_LIST_HEAD_MINUS_SUC)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSisrDlg dialog


CSeisCvlJpSetInvestSisrDlg::CSeisCvlJpSetInvestSisrDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetInvestSisrDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_calMethod = 0;
	m_targetElements = 0;

	m_aAutoCalcCtrl.RemoveAll();
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_DESI_CRIT_STC);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_DESI_CRIT_CMB);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_SIGMA_CE_CHK);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_GRP);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_STC);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_CMB);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_SHEAR_SPAN_LENGTH_STC);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_SHEAR_SPAN_LENGTH_EDT);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_SHEAR_SPAN_UNIT_STC);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_APPLY_CC_CHK);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_APPLY_CC_GRP);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_APPLY_CC1_RDO);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_APPLY_CC2_RDO);
	m_aAutoCalcCtrl.Add(IDC_TM_SISR_AUTO_APPLY_CC2_EDT);

	m_aUserInputCtrl.RemoveAll();
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_TART_ELEM_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_TART_ELEM_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_TART_POSI_CHK);
	m_aUserInputCtrl.Add(IDC_I_GRP);
	m_aUserInputCtrl.Add(IDC_STATIC_I_PLUS);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_PLUS_SUS_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_PLUS_SUS_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_PLUS_SUS_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_PLUS_SUC_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_PLUS_SUC_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_PLUS_SUC_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_STATIC_I_MINUS);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_MINUS_SUS_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_MINUS_SUS_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_MINUS_SUS_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_MINUS_SUC_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_MINUS_SUC_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_I_MINUS_SUC_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_J_GRP);
	m_aUserInputCtrl.Add(IDC_STATIC_J_PLUS);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUS_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUS_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUS_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUC_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUC_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUC_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_STATIC_J_MINUS);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUS_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUS_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUS_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUC_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUC_EDT);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUC_UNIT_STC);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_ADD_BTN);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_MOD_BTN);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_DEL_BTN);
	m_aUserInputCtrl.Add(IDC_TM_SISR_USER_LST);

	m_aUserJCtrl.RemoveAll();
	m_aUserJCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUS_EDT);
	m_aUserJCtrl.Add(IDC_TM_SISR_USER_J_PLUS_SUC_EDT);
	m_aUserJCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUS_EDT);
	m_aUserJCtrl.Add(IDC_TM_SISR_USER_J_MINUS_SUC_EDT);
}

CSeisCvlJpSetInvestSisrDlg::~CSeisCvlJpSetInvestSisrDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetInvestSisrDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_UNIT:
		UpdateUnitCtrl();
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateUserListCtrl();
		break;
	default:
		break;
	}
}

void CSeisCvlJpSetInvestSisrDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SISR_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SISR_SIPA_CMB, m_caseName);
	DDX_Control(pDX, IDC_TM_SISR_SIGR_CMB, m_groupName);
	DDX_Radio(pDX, IDC_TM_SISR_CALC_METHOD1_RDO, m_calMethod);
	DDX_Control(pDX, IDC_TM_SISR_AUTO_DESI_CRIT_CMB, m_DesignCriteria);
	DDX_Check(pDX, IDC_TM_SISR_AUTO_SIGMA_CE_CHK, m_sigma_ce);
	DDX_Check(pDX, IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK, m_extraByShearSpan);
	DDX_Control(pDX, IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_CMB, m_shearSpanType);
	DDX_Control(pDX, IDC_TM_SISR_AUTO_SHEAR_SPAN_LENGTH_EDT, m_userLength);
	DDX_Check(pDX, IDC_TM_SISR_AUTO_APPLY_CC_CHK, m_ApplyCc);
	DDX_Radio(pDX, IDC_TM_SISR_AUTO_APPLY_CC1_RDO, m_targetElements);
	DDX_Control(pDX, IDC_TM_SISR_AUTO_APPLY_CC2_EDT, m_elemAuto);
	DDX_Control(pDX, IDC_TM_SISR_USER_TART_ELEM_EDT, m_elemInput);
	DDX_Check(pDX, IDC_TM_SISR_USER_TART_POSI_CHK, m_bCommon);
	DDX_Control(pDX, IDC_TM_SISR_USER_I_PLUS_SUS_EDT, m_IpulsSus);
	DDX_Control(pDX, IDC_TM_SISR_USER_I_PLUS_SUC_EDT, m_IpulsSuc);
	DDX_Control(pDX, IDC_TM_SISR_USER_I_MINUS_SUS_EDT, m_IminusSus);
	DDX_Control(pDX, IDC_TM_SISR_USER_I_MINUS_SUC_EDT, m_IminusSuc);
	DDX_Control(pDX, IDC_TM_SISR_USER_J_PLUS_SUS_EDT, m_JpulsSus);
	DDX_Control(pDX, IDC_TM_SISR_USER_J_PLUS_SUC_EDT, m_JpulsSuc);
	DDX_Control(pDX, IDC_TM_SISR_USER_J_MINUS_SUS_EDT, m_JminusSus);
	DDX_Control(pDX, IDC_TM_SISR_USER_J_MINUS_SUC_EDT, m_JminusSuc);
	DDX_Control(pDX, IDC_TM_SISR_USER_LST, m_userInputList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestSisrDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestSisrDlg)
	ON_CBN_SELCHANGE(IDC_TM_SISR_SIPA_CMB, OnChangedCaseCmb)
	ON_CBN_SELCHANGE(IDC_TM_SISR_SIGR_CMB, OnChangedGroupCmb)
	ON_BN_CLICKED(IDC_TM_SISR_CALC_METHOD1_RDO, OnChangedCalMethodRdo)
	ON_BN_CLICKED(IDC_TM_SISR_CALC_METHOD2_RDO, OnChangedCalMethodRdo)
	ON_CBN_SELCHANGE(IDC_TM_SISR_AUTO_DESI_CRIT_CMB, OnChangedDesignCriteria)
	ON_BN_CLICKED(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK, OnChangedExtraByShearSpanChk)
	ON_CBN_SELCHANGE(IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_CMB, OnChangedShearSpanTypeCmb)
	ON_BN_CLICKED(IDC_TM_SISR_AUTO_APPLY_CC_CHK, OnChangedApplyCcChk)
	ON_BN_CLICKED(IDC_TM_SISR_AUTO_APPLY_CC1_RDO, OnChangedTargetElementsRdo)
	ON_BN_CLICKED(IDC_TM_SISR_AUTO_APPLY_CC2_RDO, OnChangedTargetElementsRdo)
	ON_EN_SETFOCUS(IDC_TM_SISR_AUTO_APPLY_CC2_EDT, OnSetFocusElemAutoEdt)
	ON_EN_SETFOCUS(IDC_TM_SISR_USER_TART_ELEM_EDT, OnSetFocusElemInputEdt)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SISR_USER_LST, OnNMClickUserInputList)
	ON_BN_CLICKED(IDC_TM_SISR_USER_ADD_BTN, OnAddBtn)
	ON_BN_CLICKED(IDC_TM_SISR_USER_MOD_BTN, OnModBtn)
	ON_BN_CLICKED(IDC_TM_SISR_USER_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(IDC_TM_SISR_USER_TART_POSI_CHK, OnClickCommonPosiChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetInvestSisrDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	InitMoveCtrl();
	InitControl();
	InitCaseNameCtrl();
	InitGroupNameCtrl();
	OnChangedCalMethodRdo();
	UpdateDesignCriteriaCtrl();
	UpdateApplyCcChkCtrl();
	OnChangedExtraByShearSpanChk();
	InitShearSpanTypeCtrl();
	UpdateUnitCtrl();
	OnChangedApplyCcChk();
	InitPositionTypeCtrl();
	InitUserListCtrl();
	UpdateUserListCtrl();
	UpdateUserShearForceCtrl();

	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedCaseCmb()
{
	UpdateUserListCtrl();
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedGroupCmb()
{
	//전단 자동입력 관련 컨트롤
	UpdateDesignCriteriaCtrl();
	UpdateApplyCcChkCtrl();
	//전단 직접입력 관련 컨트롤
	UpdateUserShearForceCtrl();
	UpdateUserListCtrl();
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedCalMethodRdo()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_CALC_METHOD1_RDO);

	CDlgUtil::CtrlShowHide(this, m_aAutoCalcCtrl, pBtn->GetCheck());
	CDlgUtil::CtrlShowHide(this, m_aUserInputCtrl, !pBtn->GetCheck());
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedDesignCriteria()
{
	CString str;
	m_DesignCriteria.GetWindowText(str);

	GetDlgItem(IDC_TM_SISR_AUTO_SIGMA_CE_CHK)->EnableWindow(str == COMBO_DESIGN_CRITERIA_ROAD3);
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedExtraByShearSpanChk()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK);

	GetDlgItem(IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_STC)->EnableWindow(pBtn->GetCheck());
	GetDlgItem(IDC_TM_SISR_AUTO_SHEAR_SPAN_TYPE_CMB)->EnableWindow(pBtn->GetCheck());
	GetDlgItem(IDC_TM_SISR_AUTO_SHEAR_SPAN_LENGTH_STC)->EnableWindow(pBtn->GetCheck());
	GetDlgItem(IDC_TM_SISR_AUTO_SHEAR_SPAN_LENGTH_EDT)->EnableWindow(pBtn->GetCheck());
	GetDlgItem(IDC_TM_SISR_AUTO_SHEAR_SPAN_UNIT_STC)->EnableWindow(pBtn->GetCheck());

	OnChangedShearSpanTypeCmb();
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedApplyCcChk()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC_CHK);

	GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC1_RDO)->EnableWindow(pBtn->GetCheck());
	GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC2_RDO)->EnableWindow(pBtn->GetCheck());
	GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC2_EDT)->EnableWindow(pBtn->GetCheck());

	OnChangedTargetElementsRdo();
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedShearSpanTypeCmb()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK);
	if (pBtn->GetCheck() != 1)
		return;

	CString str;
	m_shearSpanType.GetWindowText(str);

	GetDlgItem(IDC_TM_SISR_AUTO_SHEAR_SPAN_LENGTH_EDT)->EnableWindow(str == COMBO_SHEAR_SPAN_TYPE_USER_INPUT);
}

void CSeisCvlJpSetInvestSisrDlg::OnChangedTargetElementsRdo()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC2_RDO);
	GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC2_EDT)->EnableWindow(pBtn->GetCheck());
}

void CSeisCvlJpSetInvestSisrDlg::OnSetFocusElemAutoEdt()
{
	m_elemInput.Disconnect();
	m_elemInput.Invalidate(FALSE);

	CString strPreviousText = _T("");
	m_elemAuto.GetWindowText(strPreviousText);

	m_elemAuto.Connect(SC_ID_ELEM, &m_elemAuto);

	m_elemAuto.SetWindowText(strPreviousText);
	m_elemAuto.SelectByStr(strPreviousText);

	ResetViewControls();

	CArray<T_ELEM_K, T_ELEM_K> aElements;
	GetNodeList(strPreviousText, aElements);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElements, FALSE);
}

void CSeisCvlJpSetInvestSisrDlg::OnSetFocusElemInputEdt()
{
	m_elemAuto.Disconnect();
	m_elemAuto.Invalidate(FALSE);

	CString strPreviousText = _T("");
	m_elemInput.GetWindowText(strPreviousText);

	m_elemInput.Connect(SC_ID_ELEM, &m_elemInput);

	m_elemInput.SetWindowText(strPreviousText);
	m_elemInput.SelectByStr(strPreviousText);

	ResetViewControls();

	CArray<T_ELEM_K, T_ELEM_K> aElements;
	GetNodeList(strPreviousText, aElements);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElements, FALSE);
}

void CSeisCvlJpSetInvestSisrDlg::OnNMClickUserInputList(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1) {
		*pResult = 0;
		return;
	}

	int nItem = m_userInputList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;

	ResetViewControls();
	UpdataDialog_User(nItem);
	ClickCommonPosiChk();
	UpdateData(FALSE);
}

void CSeisCvlJpSetInvestSisrDlg::OnAddBtn()
{
	UpdateData(TRUE);

	if (CheckData_User() == FALSE)
		return;

	CArray<T_SISR_USER_D, T_SISR_USER_D> aData;
	DlgToData_User(aData);

	for (int i = 0; i < aData.GetCount(); i++)
	{
		int nCount = m_userInputList.GetItemCount();
		InsertItem_User(nCount, aData[i]);
		// 추가된 데이터를 m_UserDataList에 반영
		m_UserDataList.push_back(aData[i]);
	}

	return;
}

void CSeisCvlJpSetInvestSisrDlg::OnModBtn()
{
	UpdateData(TRUE);

	int nItem = m_userInputList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	if (CheckData_User(TRUE) == FALSE)
		return;

	CArray<T_SISR_USER_D, T_SISR_USER_D> aData;
	DlgToData_User(aData);
	
	if (aData.GetCount() < 1)
		return;

	T_SISR_USER_D data = aData[0];

	for (int j = 0; j < LIST_COLUMN_NUM_USER; j++)
	{
		CString str = DataToStr_User(j, data);
		m_userInputList.SetItemText(nItem, j, str);
	}

	// 수정된 데이터를 m_UserDataList에 반영
	T_ELEM_K preElemK = _ttoi(m_userInputList.GetItemText(nItem, 0));
	auto iter = std::find_if(m_UserDataList.begin(), m_UserDataList.end(), [preElemK](const T_SISR_USER_D& object) {
		return object.kElem == preElemK;
		});
	if (iter != m_UserDataList.end())
	{
		*iter = data;
	}
}

void CSeisCvlJpSetInvestSisrDlg::OnDelBtn()
{
	UpdateData(TRUE);

	int nItem = m_userInputList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	// 수정된 데이터를 m_UserDataList에 반영
	T_ELEM_K preElemK = _ttoi(m_userInputList.GetItemText(nItem, 0));
	auto iter = std::find_if(m_UserDataList.begin(), m_UserDataList.end(), [preElemK](const T_SISR_USER_D& object) {
		return object.kElem == preElemK;
		});
	if (iter != m_UserDataList.end())
	{
		m_UserDataList.erase(iter);
	}

	m_userInputList.DeleteItem(nItem);
}

void CSeisCvlJpSetInvestSisrDlg::OnClickCommonPosiChk()
{
	UpdateData(TRUE);

	ClickCommonPosiChk();
}

void CSeisCvlJpSetInvestSisrDlg::ClickCommonPosiChk()
{
	for (int i = 0; i < m_aUserJCtrl.GetSize(); i++)
	{
		CWnd* pWnd = GetDlgItem(m_aUserJCtrl[i]);
		if (pWnd)
			pWnd->EnableWindow(!m_bCommon);
	}
	UpdateUserShearForceCtrl();
}

void CSeisCvlJpSetInvestSisrDlg::Execute()
{
	UpdateData(TRUE);

	if (m_nOption == 0)
		AddOrModData();
	else
		DelData();
}

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSisrDlg diagnostics
// 
void CSeisCvlJpSetInvestSisrDlg::InitMoveCtrl()
{
	CRect rTarget, rSource;
	GetDlgItem(IDC_TM_SISR_AUTO_DESI_CRIT_STC)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SISR_USER_TART_ELEM_STC)->GetWindowRect(rTarget);

	int nDistX = rTarget.left - rSource.left;
	CDlgUtil::CtrlMoveDistX(this, m_aAutoCalcCtrl, nDistX);
}

void CSeisCvlJpSetInvestSisrDlg::InitControl()
{
	CButton* pBtn = NULL;
	//옵션 추가/변경 삭제 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;
	//계산방법 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_CALC_METHOD1_RDO);
	pBtn->SetCheck(1);
	m_calMethod = 0;
	//σce (프로스트레스에 의한 응력)를 별도로 고려
	pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_AUTO_SIGMA_CE_CHK);
	pBtn->SetCheck(0);
	m_sigma_ce = FALSE;
	//전단 스팬비에 의한 할증 체크박스
	pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK);
	pBtn->SetCheck(0);
	m_extraByShearSpan = FALSE;
	//Cc=1.0 적용 체크박스
	pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC_CHK);
	pBtn->SetCheck(0);
	m_ApplyCc = FALSE;
	//개별요소 Select Edit
	m_elemAuto.Connect(SC_ID_ELEM, &m_elemAuto);
}

void CSeisCvlJpSetInvestSisrDlg::InitCaseNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->GetCount() < 1)
		return;

	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	m_caseName.ResetContent();

	CArray<T_SIPA_K, T_SIPA_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIPA_K& key = keys[i];
		T_SIPA_D data;
		m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(key, data);
		if (sidcD.nDesignCode != data.nDesignCode)
			continue;

		m_caseName.AddString(data.strCaseName);
	}
	m_caseName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSisrDlg::InitGroupNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->GetCount() < 1)
		return;

	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	m_groupName.ResetContent();

	CArray<T_SIGR_K, T_SIGR_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_K& key = keys[i];
		T_SIGR_D data;
		m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(key, data);
		
		if (data.nSubType == T_SIGR_D::kLower) {
			//※강부재에서는 전단내력을 계산할 수 없습니다.
			T_SIGR_LOWER_D dLower;
			if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, dLower) == FALSE)
				continue;
			if (dLower.nMaterialType == T_SIGR_LOWER_D::kSteel)
				continue;
		}

		m_groupName.AddString(data.strName);
	}
	m_groupName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSisrDlg::UpdateDesignCriteriaCtrl()
{
	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_D data;
	T_SIGR_K key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
	m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(key, data);

	m_DesignCriteria.ResetContent();

	if (data.nSubType == T_SIGR_D::eSubType::kUpper) {
		m_DesignCriteria.AddString(COMBO_DESIGN_CRITERIA_ROAD3);
		m_DesignCriteria.SetCurSel(0);
	}
	else if (data.nSubType == T_SIGR_D::eSubType::kLower) {
		m_DesignCriteria.AddString(COMBO_DESIGN_CRITERIA_ROAD4);
		m_DesignCriteria.AddString(COMBO_DESIGN_CRITERIA_ROAD5);
		m_DesignCriteria.SetCurSel(1);
	}
	else {
		m_DesignCriteria.AddString(COMBO_DESIGN_CRITERIA_ROAD3);
		m_DesignCriteria.AddString(COMBO_DESIGN_CRITERIA_ROAD4);
		m_DesignCriteria.AddString(COMBO_DESIGN_CRITERIA_ROAD5);
		m_DesignCriteria.SetCurSel(2);
	}

	CString str;
	m_DesignCriteria.GetWindowText(str);

	GetDlgItem(IDC_TM_SISR_AUTO_SIGMA_CE_CHK)->EnableWindow(str == COMBO_DESIGN_CRITERIA_ROAD3);
}

void CSeisCvlJpSetInvestSisrDlg::UpdateApplyCcChkCtrl()
{
	CString str;
	m_groupName.GetWindowText(str);

	T_SIGR_D data;
	T_SIGR_K key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(key, data);

	if (data.nSubType == T_SIGR_D::eSubType::kUpper) {
		((CButton*)GetDlgItem(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC_CHK))->SetCheck(FALSE);
	}
	
	BOOL bCheckCc = FALSE;
	if (data.nSubType != T_SIGR_D::eSubType::kUpper) {
		bCheckCc = ((CButton*)GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC_CHK))->GetCheck();
	}

	GetDlgItem(IDC_TM_SISR_AUTO_EXTRA_BY_SHEAR_SPAN_CHK)->EnableWindow(data.nSubType != T_SIGR_D::eSubType::kUpper);
	GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC_CHK)->EnableWindow(data.nSubType != T_SIGR_D::eSubType::kUpper);
	GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC1_RDO)->EnableWindow(bCheckCc);
	GetDlgItem(IDC_TM_SISR_AUTO_APPLY_CC2_RDO)->EnableWindow(bCheckCc);
}

void CSeisCvlJpSetInvestSisrDlg::InitShearSpanTypeCtrl()
{
	m_shearSpanType.ResetContent();
	m_shearSpanType.AddString(COMBO_SHEAR_SPAN_TYPE_COLUMN_LENGTH);
	m_shearSpanType.AddString(COMBO_SHEAR_SPAN_TYPE_INERTIAL_FORCE);
	m_shearSpanType.AddString(COMBO_SHEAR_SPAN_TYPE_USER_INPUT);
	m_shearSpanType.SetCurSel(0);
}

void CSeisCvlJpSetInvestSisrDlg::UpdateUnitCtrl()
{
	CString strUnit;
	T_SISR_UNIT* pUnit = &(CUnitCtrl::m_SISR_UNIT);
	T_SISR_USER_UNIT* pUnitUser = &(CUnitCtrl::m_SISR_USER_UNIT);

	//전단스팬a [단위]
	CUnitCtrl::GetUnitSystem(pUnit->Length, strUnit);
	GetDlgItem(IDC_TM_SISR_AUTO_SHEAR_SPAN_UNIT_STC)->SetWindowText(strUnit);
	//전단내력
	CUnitCtrl::GetUnitSystem(pUnitUser->Force, strUnit);
	GetDlgItem(IDC_TM_SISR_USER_MIN_SHEA_FORC_UNIT_STC)->SetWindowText(strUnit);
	GetDlgItem(IDC_TM_SISR_USER_MAX_SHEA_FORC_UNIT_STC)->SetWindowText(strUnit);
}

void CSeisCvlJpSetInvestSisrDlg::InitPositionTypeCtrl()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SISR_USER_TART_POSI_CHK);
	if (pBtn == NULL)
		return;
	pBtn->SetCheck(0); // 초기화
}

void CSeisCvlJpSetInvestSisrDlg::UpdateUserShearForceCtrl()
{
	CString str;
	m_groupName.GetWindowText(str);

	T_SIGR_D data;
	T_SIGR_K key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(key, data);

	GetDlgItem(IDC_TM_SISR_USER_I_PLUS_SUC_EDT)->EnableWindow(data.nSubType != T_SIGR_D::kLower);
	GetDlgItem(IDC_TM_SISR_USER_I_MINUS_SUC_EDT)->EnableWindow(data.nSubType != T_SIGR_D::kLower);
	GetDlgItem(IDC_TM_SISR_USER_J_PLUS_SUC_EDT)->EnableWindow(data.nSubType != T_SIGR_D::kLower);
	GetDlgItem(IDC_TM_SISR_USER_J_MINUS_SUC_EDT)->EnableWindow(data.nSubType != T_SIGR_D::kLower);
	if (data.nSubType != T_SIGR_D::kLower) {
		GetDlgItem(IDC_TM_SISR_USER_I_PLUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_PLUS_SUS_STC));
		GetDlgItem(IDC_TM_SISR_USER_I_MINUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_MINUS_SUS_STC));
		GetDlgItem(IDC_TM_SISR_USER_J_PLUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_PLUS_SUS_STC));
		GetDlgItem(IDC_TM_SISR_USER_J_MINUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_MINUS_SUS_STC));
	}
	else {
		GetDlgItem(IDC_TM_SISR_USER_I_PLUS_SUC_EDT)->SetWindowText(_T(""));
		GetDlgItem(IDC_TM_SISR_USER_I_MINUS_SUC_EDT)->SetWindowText(_T(""));
		GetDlgItem(IDC_TM_SISR_USER_J_PLUS_SUC_EDT)->SetWindowText(_T(""));
		GetDlgItem(IDC_TM_SISR_USER_J_MINUS_SUC_EDT)->SetWindowText(_T(""));
		GetDlgItem(IDC_TM_SISR_USER_I_PLUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_PLUS_PS_STC));
		GetDlgItem(IDC_TM_SISR_USER_I_MINUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_MINUS_PS_STC));
		GetDlgItem(IDC_TM_SISR_USER_J_PLUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_PLUS_PS_STC));
		GetDlgItem(IDC_TM_SISR_USER_J_MINUS_SUS_STC)->SetWindowText(_LS(IDS_IDD_TM_SISR_IDC_TM_SISR_USER_MINUS_PS_STC));
	}
}

void CSeisCvlJpSetInvestSisrDlg::InitUserListCtrl()
{
	CString aTitle[] = { LIST_HEAD_ELEM, LIST_HEAD_POS, LIST_HEAD_PLUS_SUS, LIST_HEAD_PLUS_SUC, LIST_HEAD_MINUS_SUS, LIST_HEAD_MINUS_SUC };
	int nColWidth[] = { 40, 36, 56, 56, 56, 56 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_userInputList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_userInputList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < LIST_COLUMN_NUM_USER; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_userInputList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlJpSetInvestSisrDlg::UpdateUserListCtrl()
{
	m_userInputList.DeleteAllItems();

	CString strCase; m_caseName.GetWindowText(strCase);
	CString strGroup; m_groupName.GetWindowText(strGroup);

	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);

	CArray<T_SISR_K, T_SISR_K> kSisrKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(kSisrKeys);
	for (int i = 0; i < kSisrKeys.GetCount(); i++)
	{
		T_SISR_K& kSisr = kSisrKeys[i];
		T_SISR_D dSisr;

		m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr);

		if (dSisr.kSipa == kSipa && dSisr.kSigr == kSigr) {
			UpdateUserListBySisrKey(kSisr);
			return;
		}
	}
}

void CSeisCvlJpSetInvestSisrDlg::UpdateUserListBySisrKey(const T_SISR_K& kSisr)
{
	CArray<T_SISR_USER_K, T_SISR_USER_K> keyList;
	m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetKeysEqualSisrKey(kSisr, keyList);

	m_UserDataList.clear();
	for (int i = 0; i < keyList.GetCount(); i++)
	{
		T_SISR_USER_K key = keyList[i];
		
		T_SISR_USER_D data;
		if(m_pDoc->m_pAttrCtrl2->GetQSisrUser()->Get(key, data) == FALSE)
			continue;

		InsertItemUserListCtrl(key, data);
		m_UserDataList.push_back(data);
	}
}

void CSeisCvlJpSetInvestSisrDlg::DlgToData_User(OUT CArray<T_SISR_USER_D, T_SISR_USER_D>& aData)
{
	CString str;
	//대상 요소
	m_elemInput.GetWindowText(str);

	T_KEY_LIST keys;
	GetNodeList(str, keys);

	//전단내력 공통
	BOOL bCommon = m_bCommon;

	// I방향 전단내력
	m_IpulsSus.GetWindowText(str);
	double dPulsSus = _wtof(str);
	m_IpulsSuc.GetWindowText(str);
	double dPulsSuc = _wtof(str);

	m_IminusSus.GetWindowText(str);
	double dMinusSus = _wtof(str);
	m_IminusSuc.GetWindowText(str);
	double dMinusSuc = _wtof(str);

	//J방향 전단내력
	m_JpulsSus.GetWindowText(str);
	double dJPulsSus = _wtof(str);
	m_JpulsSuc.GetWindowText(str);
	double dJPulsSuc = _wtof(str);

	m_JminusSus.GetWindowText(str);
	double dJMinusSus = _wtof(str);
	m_JminusSuc.GetWindowText(str);
	double dJMinusSuc = _wtof(str);
	
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SISR_USER_D data;
		data.kElem = keys[i];
		// I 단
		data.I_PLUS_D.dSus = dPulsSus;
		data.I_PLUS_D.dSuc = dPulsSuc;
		data.I_MINUS_D.dSus = dMinusSus;
		data.I_MINUS_D.dSuc = dMinusSuc;

		// J 단
		data.J_PLUS_D.dSus = dJPulsSus;
		data.J_PLUS_D.dSuc = dJPulsSuc;
		data.J_MINUS_D.dSus = dJMinusSus;
		data.J_MINUS_D.dSuc = dJMinusSuc;
		if (bCommon)
		{
			data.J_PLUS_D = data.I_PLUS_D;
			data.J_MINUS_D = data.I_MINUS_D;
		}
		aData.Add(data);
	}
}

void CSeisCvlJpSetInvestSisrDlg::InsertItem_User(const int& nIndex, IN T_SISR_USER_D& data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < LIST_COLUMN_NUM_USER; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_User(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
			nItem = m_userInputList.InsertItem(&lvitem);
		else
			m_userInputList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetInvestSisrDlg::InsertItemUserListCtrl(T_SISR_USER_K Key, T_SISR_USER_D& Data)
{
	int nItem = m_userInputList.GetItemCount();
	for (int i = 0; i < LIST_COLUMN_NUM_USER; i++)
	{
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem = i;
		CString str = DataToStr_User(i, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_userInputList.InsertItem(&lvitem);
		}
		else m_userInputList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

CString CSeisCvlJpSetInvestSisrDlg::DataToStr_User(const int& index, const T_SISR_USER_D& data)
{
	CString str = _T("");
	BOOL bExist_I = CheckExistData_IJ(data, T_SISR_USER_D::kI);
	BOOL bExist_J = CheckExistData_IJ(data, T_SISR_USER_D::kJ);

	switch (index)
	{
	case 0://대상요소
		str.Format(_T("%d"), data.kElem);
		break;
	case 1://위치
		if(data.IsSame()) str = _T("I&J");
		else
		{
			if (bExist_I)
				str = _T("I");
			else
				str = _T("J");
		}
		break;
		case 2://(+)방향 Sus
			if(bExist_I) str.Format(_T("%g"), data.I_PLUS_D.dSus);
			else str.Format(_T("%g"), data.J_PLUS_D.dSus);
			break;
		case 3://(+)방향 Suc
			if (bExist_I) str.Format(_T("%g"), data.I_PLUS_D.dSuc);
			else str.Format(_T("%g"), data.J_PLUS_D.dSuc);
			break;
		case 4://(-)방향 Sus
			if (bExist_I) str.Format(_T("%g"), data.I_MINUS_D.dSus);
			else str.Format(_T("%g"), data.J_MINUS_D.dSus);
			break;
		case 5://(-)방향 Suc
			if (bExist_I) str.Format(_T("%g"), data.I_MINUS_D.dSuc);
			else str.Format(_T("%g"), data.J_MINUS_D.dSuc);
			break;
	default: ASSERT(0); break;
	}

	return str;
}

void CSeisCvlJpSetInvestSisrDlg::UpdataDialog_User(const int& nItem)
{
	//대상요소
	CString str = m_userInputList.GetItemText(nItem, 0);
	m_elemInput.SetWindowText(str);
	
	if (nItem > m_UserDataList.size()) return;

	// 데이터
	T_ELEM_K ElemK = _ttoi(str);
	m_JpulsSus.SetWindowText(_T(""));
	m_JpulsSuc.SetWindowText(_T(""));
	m_JminusSus.SetWindowText(_T(""));
	m_JminusSuc.SetWindowText(_T(""));
	m_IpulsSus.SetWindowText(_T(""));
	m_IpulsSuc.SetWindowText(_T(""));
	m_IminusSus.SetWindowText(_T(""));
	m_IminusSuc.SetWindowText(_T(""));
	CString strPlusSus = _T("");
	CString strPlusSuc = _T("");
	CString strMinusSus = _T("");
	CString strMinusSuc = _T("");

	T_SISR_USER_D UserD = m_UserDataList[nItem];
	//위치
	m_bCommon = UserD.IsSame();

	// I단
	strPlusSus.Format(_T("%g"), UserD.I_PLUS_D.dSus);
	strPlusSuc.Format(_T("%g"), UserD.I_PLUS_D.dSuc);
	strMinusSus.Format(_T("%g"), UserD.I_MINUS_D.dSus);
	strMinusSuc.Format(_T("%g"), UserD.I_MINUS_D.dSuc);
	//전단내력 (+)방향 Sus
	m_IpulsSus.SetWindowText(strPlusSus);
	//전단내력 (+)방향 Suc
	m_IpulsSuc.SetWindowText(strPlusSuc);
	//전단내력 (+)방향 Sus
	m_IminusSus.SetWindowText(strMinusSus);
	//전단내력 (+)방향 Suc
	m_IminusSuc.SetWindowText(strMinusSuc);

	// J단
	strPlusSus.Format(_T("%g"), UserD.J_PLUS_D.dSus);
	strPlusSuc.Format(_T("%g"), UserD.J_PLUS_D.dSuc);
	strMinusSus.Format(_T("%g"), UserD.J_MINUS_D.dSus);
	strMinusSuc.Format(_T("%g"), UserD.J_MINUS_D.dSuc);

	//전단내력 (+)방향 Sus
	m_JpulsSus.SetWindowText(strPlusSus);
	//전단내력 (+)방향 Suc
	m_JpulsSuc.SetWindowText(strPlusSuc);
	//전단내력 (+)방향 Sus
	m_JminusSus.SetWindowText(strMinusSus);
	//전단내력 (+)방향 Suc
	m_JminusSuc.SetWindowText(strMinusSuc);
}

BOOL CSeisCvlJpSetInvestSisrDlg::CheckData_User(BOOL bModify/*=FALSE*/)
{
	CString str;
	//대상 요소
	m_elemInput.GetWindowText(str);

	T_KEY_LIST keys;
	GetNodeList(str, keys);

	if (keys.GetCount() < 1) {
		str = _LS(IDS_IDD_TM_SISR_MSG_INVALID_ELEMENT_USER);
		AfxMessageBox(str);
		return FALSE;
	}

	m_groupName.GetWindowText(str);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);

	T_ELEM_K_LIST groupElemKeys;
	GetGroupElements(kSigr, groupElemKeys);

	for (int i = 0; i < keys.GetCount(); i++)
	{
		//기존에 등록된 요소가 있는지 체크
		if (IsExistElemUser(keys[i], bModify) == TRUE) {
			str.Format(_LS(IDS_IDD_TM_SISR_MSG_EXIST_ELEMENT_USER), keys[i]);
			AfxMessageBox(str);
			return FALSE;
		}
		//현재 그룹에 속한 요소를 선택 했는지 체크
		if (groupElemKeys.GetCount() > 0) {
			const auto itr = std::find(&groupElemKeys[0], &groupElemKeys[0] + groupElemKeys.GetCount(), keys[i]);
			if (itr == &groupElemKeys[0] + groupElemKeys.GetCount()) {
				str.Format(_LS(IDS_IDD_TM_SISR_MSG_NOT_INCLUDE_ELEM_IN_GROUP), keys[i]);
				AfxMessageBox(str);
				return FALSE;
			}
		}
	}

	//전단내력
	m_IpulsSus.GetWindowText(str);
	if (str == _T("")) {
		str = _LS(IDS_IDD_TM_SISR_MSG_EMPTY_SHEAR_FORCE);
		AfxMessageBox(str);
		return FALSE;
	}
	m_IpulsSuc.GetWindowText(str);
	if (m_IpulsSuc.IsWindowEnabled() && str == _T("")) {
		str = _LS(IDS_IDD_TM_SISR_MSG_EMPTY_SHEAR_FORCE);
		AfxMessageBox(str);
		return FALSE;
	}
	m_IminusSus.GetWindowText(str);
	if (str == _T("")) {
		str = _LS(IDS_IDD_TM_SISR_MSG_EMPTY_SHEAR_FORCE);
		AfxMessageBox(str);
		return FALSE;
	}
	m_IminusSuc.GetWindowText(str);
	if (m_IminusSuc.IsWindowEnabled() && str == _T("")) {
		str = _LS(IDS_IDD_TM_SISR_MSG_EMPTY_SHEAR_FORCE);
		AfxMessageBox(str);
		return FALSE;
	}

	return TRUE;
}

BOOL CSeisCvlJpSetInvestSisrDlg::IsExistElemUser(const T_ELEM_K& kElem, BOOL bModify/*=FALSE*/)
{
	T_KEY_LIST allElemKeyList;

	int nSize = m_userInputList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		CString str;
		T_KEY_LIST elemKeys;

		str = m_userInputList.GetItemText(nItem, 0);
		GetNodeList(str, elemKeys);

		for (int i = 0; i < elemKeys.GetCount(); i++)
		{
			if (bModify == TRUE) {
				if(elemKeys[i] == kElem)
					continue;
			}
			allElemKeyList.Add(elemKeys[i]);
		}
	}

	if (allElemKeyList.GetCount() < 1)
		return FALSE;

	const auto itr = std::find(&allElemKeyList[0], &allElemKeyList[0] + allElemKeyList.GetCount(), kElem);
	if (itr != &allElemKeyList[0] + allElemKeyList.GetCount())
		return TRUE;

	return FALSE;
}

void CSeisCvlJpSetInvestSisrDlg::ResetViewControls()
{
	m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

BOOL CSeisCvlJpSetInvestSisrDlg::AddOrModData()
{
	T_SISR_D data;
	CArray<T_SISR_USER_D, T_SISR_USER_D> userList;
	DlgToData(data, userList);

	if (CheckData(data) == FALSE)
		return FALSE;
	if (m_calMethod == 1 && userList.GetCount() < 1)
		return FALSE;
	if (CheckData_UserList(userList) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_SISR)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	BOOL bResult = FALSE;
	T_SISR_K key = GetExistingDataKey(data);
	if (key == 0) { // Add
		bResult = AddData(data, userList);
	}
	else { //Mod
		bResult = ModData(key, data, userList);
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SISR_CMD);
	ASSERT(bResult);
	return bResult;
}

BOOL CSeisCvlJpSetInvestSisrDlg::AddData(T_SISR_D& dSisr, CArray<T_SISR_USER_D, T_SISR_USER_D>& userList)
{
	T_SISR_K kSisr = m_pDoc->m_pAttrCtrl2->GetQSisr()->GetStartNum();
	BOOL bResult = m_pDoc->m_pEditData->AddSisr(kSisr, dSisr);
	if (bResult == FALSE) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_calMethod != 1)//kUserInput 이 아니라면 여기까지만 DB 추가
		return TRUE;

	bResult = AddData_SisrUser(kSisr, userList);
	ASSERT(bResult);

	return bResult;
}

BOOL CSeisCvlJpSetInvestSisrDlg::ModData(const T_SISR_K& kSisr, T_SISR_D& dSisr, CArray<T_SISR_USER_D, T_SISR_USER_D>& userList)
{
	//기본적으로 자동계산을 하고 직접입력된 값이 있는 경우 그 값을 자동계산된 값을 대신하여 보고서에 출력 되도록 하기로 하여
	//자동계산 텝과 직접입력 텝 둘다에 조사 케이스 + 조사 대상 조합으로 등록이 가능하도록 함.
	if (m_pDoc->m_pEditData->ModifySisr(kSisr, dSisr) == FALSE) return FALSE;

	T_ELEM_K_LIST elemKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetElemKeysEqualSisrKey(kSisr, elemKeys);

	if (elemKeys.GetCount() < 1) {
		return AddData_SisrUser(kSisr, userList);
	}
	else {
		//삭제된 것, 추가된 것, 변경된 것을 찾아 T_SISR_USER_D 업데이트
		return ModData_SisrUser(kSisr, userList);
	}

	return FALSE;
}

BOOL CSeisCvlJpSetInvestSisrDlg::DelData()
{
	T_SISR_D delData;
	CArray<T_SISR_USER_D, T_SISR_USER_D> userList;
	DlgToData(delData, userList);

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Delete_SISR)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	BOOL bResult = FALSE;
	T_SISR_K key = 0;
	T_SISR_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSisr()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSisr()->GetNext(pos, key, data);
		if (data.kSipa == delData.kSipa && data.kSigr == delData.kSigr) {

			if (DelData_SisrUser(key) == TRUE)
				bResult = m_pDoc->m_pEditData->DelSisr(key);
			ASSERT(bResult);
		}
	}
	if(bResult == FALSE)
		AfxMessageBox(_LS(IDS_IDD_TM_SISR_MSG_NOT_EXIST_EQUAL_SIPA_SIGR));

	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SISR_CMD);
	return bResult;
}

BOOL CSeisCvlJpSetInvestSisrDlg::AddData_SisrUser(const T_SISR_K& kSisr, IN CArray<T_SISR_USER_D, T_SISR_USER_D>& userList)
{
	if (userList.GetCount() < 1)
		return TRUE;

	BOOL bResult = FALSE;
	for (int i = 0; i < userList.GetCount(); i++)
	{
		T_SISR_USER_D& dUser = userList[i];
		dUser.kSisr = kSisr;

		T_SISR_USER_K key = m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetStartNum();
		bResult = m_pDoc->m_pEditData->AddSisrUser(key, dUser);
		ASSERT(bResult);
	}

	return bResult;
}

BOOL CSeisCvlJpSetInvestSisrDlg::DelData_SisrUser(const T_SISR_K& kSisr)
{
	CArray<T_SISR_USER_K, T_SISR_USER_K> aSisrKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetKeysEqualSisrKey(kSisr, aSisrKeys);
	if (aSisrKeys.GetCount() < 1) { return TRUE; }  /// User 가 없는 경우, 지울 필요 없으니 return TRUE;

	//T_SISR_USER_D 삭제
	BOOL bResult = FALSE;
	for (int i = 0; i < aSisrKeys.GetCount(); i++)
	{
		bResult = m_pDoc->m_pEditData->DelSisrUser(aSisrKeys[i]);
	}
	return bResult;
}

BOOL CSeisCvlJpSetInvestSisrDlg::ModData_SisrUser(const T_SISR_K& kSisr, IN CArray<T_SISR_USER_D, T_SISR_USER_D>& userList)
{
	T_KEY_LIST aElemKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetElemKeysEqualSisrKey(kSisr, aElemKeys);

	std::list<T_ELEM_K> deletedElemKeys;
	for (int i = 0; i < aElemKeys.GetCount(); i++) deletedElemKeys.push_back(aElemKeys[i]);

	BOOL bResult = FALSE;
	for (int i = 0; i < userList.GetCount(); i++)
	{
		T_SISR_USER_D& dUser = userList[i];
		dUser.kSisr = kSisr;

		if (aElemKeys.GetCount() < 1) {//추가
			T_SISR_USER_K key = m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetStartNum();
			bResult = m_pDoc->m_pEditData->AddSisrUser(key, dUser);
			ASSERT(bResult);
		}
		else
		{
			const auto itr = std::find(&aElemKeys[0], &aElemKeys[0] + aElemKeys.GetCount(), dUser.kElem);
			if (itr == &aElemKeys[0] + aElemKeys.GetCount()) { //추가
				T_SISR_USER_K key = m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetStartNum();
				bResult = m_pDoc->m_pEditData->AddSisrUser(key, dUser);
				ASSERT(bResult);
			}
			else {
				T_SISR_USER_K key = m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetKeyEqualElemKey(dUser.kElem);
				bResult = m_pDoc->m_pEditData->ModifySisrUser(key, dUser);
				ASSERT(bResult);
				//삭제를 위해 aElemKeys 에서 aElemKeys 에서 업데이트된 Elem key 삭제
				deletedElemKeys.remove(dUser.kElem);
			}
		}
	}

	//삭제
	std::list<T_ELEM_K>::iterator itr;
	for (itr = deletedElemKeys.begin(); itr != deletedElemKeys.end(); ++itr) {
		T_ELEM_K kElem = *itr;
		T_SISR_USER_K key = m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetKeyEqualElemKey(kElem);
		bResult = m_pDoc->m_pEditData->DelSisrUser(key);
		ASSERT(bResult);
	}

	return bResult;
}

void CSeisCvlJpSetInvestSisrDlg::DlgToData(OUT T_SISR_D& rData, OUT CArray<T_SISR_USER_D, T_SISR_USER_D>& userList)
{
	CString str;
	//조사 케이스
	m_caseName.GetWindowText(str);
	rData.kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(str);
	//조사 대상
	m_groupName.GetWindowText(str);
	T_KEY key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Exist(key) == FALSE) {
		ASSERT(FALSE);
		return;
	}
	rData.kSigr = key;
	//계산방법
	DlgToData_Auto(rData.AUTO_D);
	//DlgToData_UserLisr(userList);
	for (int i = 0; i < m_UserDataList.size(); ++i)
	{
		userList.Add(m_UserDataList[i]);
	}
}

void CSeisCvlJpSetInvestSisrDlg::DlgToData_Auto(T_SISR_AUTO_D& rData)
{
	CString str;
	//적용기준
	m_DesignCriteria.GetWindowText(str);
	if (str == COMBO_DESIGN_CRITERIA_ROAD3)
		rData.nDesignCriteria = T_SISR_AUTO_D::kRoad3;
	else if (str == COMBO_DESIGN_CRITERIA_ROAD4)
		rData.nDesignCriteria = T_SISR_AUTO_D::kRoad4;
	if (str == COMBO_DESIGN_CRITERIA_ROAD5)
		rData.nDesignCriteria = T_SISR_AUTO_D::kRoad5;
	//σce (프로스트레스에 의한 응력)를 별도로 고려
	rData.bSigmaCe = m_sigma_ce;
	//전단 스팬비에 의한 할증 적용여부
	rData.bExtraByShearSpan = m_extraByShearSpan;
	//전단 스팬 산정방식
	m_shearSpanType.GetWindowText(str);
	if (str == COMBO_SHEAR_SPAN_TYPE_COLUMN_LENGTH)
		rData.nShearSpanType = T_SISR_AUTO_D::kColumnLength;
	else if (str == COMBO_SHEAR_SPAN_TYPE_INERTIAL_FORCE)
		rData.nShearSpanType = T_SISR_AUTO_D::kLocInertialForce;
	else if (str == COMBO_SHEAR_SPAN_TYPE_USER_INPUT)
		rData.nShearSpanType = T_SISR_AUTO_D::kUserInput;
	//전단스팬a
	m_userLength.GetWindowText(str);
	rData.dUserLength = _wtof(str);
	//Cc=1.0 적용여부
	rData.bApplyCc = m_ApplyCc;
	//Cc=1.0 적용요소 방식
	if (m_targetElements == 0)
		rData.nTargetElements = T_SISR_AUTO_D::kWholeGroup;
	else if (m_targetElements == 1)
		rData.nTargetElements = T_SISR_AUTO_D::kEachElement;
	//Cc=1.0 적용 대상요소
	m_elemAuto.GetWindowText(str);
	GetNodeList(str, rData.aCcElements);
}

void CSeisCvlJpSetInvestSisrDlg::DlgToData_UserLisr(OUT CArray<T_SISR_USER_D, T_SISR_USER_D>& rList)
{
	for (int nItem = 0; nItem < m_userInputList.GetItemCount(); nItem++)
	{
		int Elemk = _ttoi(m_userInputList.GetItemText(nItem, 0)) ;//대상요소
		 auto iter = std::find_if(m_UserDataList.begin(), m_UserDataList.end(), [Elemk](const T_SISR_USER_D object) {
			 return object.kElem == Elemk;
			});
		 if (iter == m_UserDataList.end()) continue;

		CString strPos = m_userInputList.GetItemText(nItem, 1);//위치
		CString strPulsSus = m_userInputList.GetItemText(nItem, 2);//(+)방향 Sus
		CString strPulsSuc = m_userInputList.GetItemText(nItem, 3);//(+)방향 Suc
		CString strMinusSus = m_userInputList.GetItemText(nItem, 4);//(-)방향 Sus
		CString strMinusSuc = m_userInputList.GetItemText(nItem, 5);//(-)방향 Suc

		T_SISR_USER_D data;
		//data.kElem = _ttoi(strElem);

		//if(strPos == _T("I"))
		//	data.nPosType = T_SISR_USER_D::kI;
		//else if (strPos == _T("J"))
		//	data.nPosType = T_SISR_USER_D::kJ;
		//else if (strPos == _T("I&J"))
		//	data.nPosType = T_SISR_USER_D::kI_J;

		rList.Add(data);
	}
}

BOOL CSeisCvlJpSetInvestSisrDlg::CheckData(OUT T_SISR_D& rData)
{
	CString str;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Exist(rData.kSipa) == FALSE) {
		str = _LS(IDS_IDD_TM_SISR_MSG_NOT_EXIST_SIPA_VALUE);
		AfxMessageBox(str);
		return FALSE;
	}

	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Exist(rData.kSigr) == FALSE) {
		str = _LS(IDS_IDD_TM_SISR_MSG_NOT_EXIST_SIGR_VALUE);
		AfxMessageBox(str);
		return FALSE;
	}

	if (m_calMethod != 0)//kAutomatic 이 아니면 여기까지 체크
		return TRUE;
	//자동계산 방식일 때 체크해야 할 것들
	if (rData.AUTO_D.nShearSpanType == T_SISR_AUTO_D::kUserInput) {
		if (rData.AUTO_D.dUserLength <= 0) {
			str = _LS(IDS_IDD_TM_SISR_MSG_INVALID_VALUE_SPAN_LENGTH);
			AfxMessageBox(str);
			return FALSE;
		}
	}

	if (rData.AUTO_D.bApplyCc == TRUE && rData.AUTO_D.nTargetElements == T_SISR_AUTO_D::kEachElement) {
		T_ELEM_K_LIST groupElemKeys;
		GetGroupElements(rData.kSigr, groupElemKeys);

		for (int i = 0; i < rData.AUTO_D.aCcElements.GetCount(); i++)
		{
			T_ELEM_K kElem = rData.AUTO_D.aCcElements[i];

			const auto itr = std::find(&groupElemKeys[0], &groupElemKeys[0] + groupElemKeys.GetCount(), kElem);
			if (itr == &groupElemKeys[0] + groupElemKeys.GetCount()) {
				str.Format(_LS(IDS_IDD_TM_SISR_MSG_NOT_INCLUDE_ELEM_IN_GROUP), kElem);
				AfxMessageBox(str);
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CSeisCvlJpSetInvestSisrDlg::GetGroupElements(const T_SIGR_K& kSigr, OUT T_ELEM_K_LIST& groupElemKeys)
{
	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE)
		return;

	if (dSigr.nSubType == T_SIGR_D::kLower) {
		T_SIGR_LOWER_D dLower;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, dLower) == TRUE) {
			for (int i = 0; i < dLower.COLM_D.GetCount(); i++) groupElemKeys.Append(dLower.COLM_D[i].aInvestigatedElem);
			for (int i = 0; i < dLower.BEAM_D.GetCount(); i++) groupElemKeys.Append(dLower.BEAM_D[i].aInvestigatedElem);
			for (int i = 0; i < dLower.FOOT_D.GetCount(); i++) groupElemKeys.Append(dLower.FOOT_D[i].aInvestigatedElem);
		}
	}
	else if (dSigr.nSubType == T_SIGR_D::kUpper) {
		T_SIGR_UPPER_D dUpper;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(kSigr, dUpper) == TRUE)
			groupElemKeys.Append(dUpper.aElements);
	}
	else if (dSigr.nSubType == T_SIGR_D::kUser) {
		T_SIGR_USER_D dUser;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(kSigr, dUser) == TRUE)
			groupElemKeys.Append(dUser.aInvestigatedElem);
	}
}

BOOL CSeisCvlJpSetInvestSisrDlg::CheckData_UserList(const CArray<T_SISR_USER_D, T_SISR_USER_D>& userList)
{
	CString str;
	for (int i = 0; i < userList.GetCount(); i++)
	{
		if (m_pDoc->m_pAttrCtrl->ExistElem(userList[i].kElem) == FALSE) {
			str.Format(_LS(IDS_IDD_TM_SISR_MSG_INVALID_USER_INPUT_ELEMEMT), userList[i].kElem);
			AfxMessageBox(str);
			return FALSE;
		}

		if (userList[i].I_PLUS_D.dSus < 0 || userList[i].J_PLUS_D.dSus < 0) {
			str = _LS(IDS_IDD_TM_SISR_MSG_INVALID_USER_INPUT_SHEAR_FORCE);
			AfxMessageBox(str);
			return FALSE;
		}

		if (userList[i].I_PLUS_D.dSuc < 0 || userList[i].J_PLUS_D.dSuc < 0) {
			str = _LS(IDS_IDD_TM_SISR_MSG_INVALID_USER_INPUT_SHEAR_FORCE);
			AfxMessageBox(str);
			return FALSE;
		}

		if (userList[i].I_MINUS_D.dSus < 0 || userList[i].J_MINUS_D.dSus < 0) {
			str = _LS(IDS_IDD_TM_SISR_MSG_INVALID_USER_INPUT_SHEAR_FORCE);
			AfxMessageBox(str);
			return FALSE;
		}

		if (userList[i].I_MINUS_D.dSuc < 0 || userList[i].J_MINUS_D.dSuc < 0) {
			str = _LS(IDS_IDD_TM_SISR_MSG_INVALID_USER_INPUT_SHEAR_FORCE);
			AfxMessageBox(str);
			return FALSE;
		}
	}

	return TRUE;
}

T_SISR_K CSeisCvlJpSetInvestSisrDlg::GetExistingDataKey(const T_SISR_D& newData)
{
	CString str;
	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	T_SISR_K key;
	T_SISR_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSisr()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSisr()->GetNext(pos, key, data);
		if (data.kSipa == newData.kSipa && data.kSigr == newData.kSigr) {
			return key;
		}
	}
	return 0;
}

BOOL CSeisCvlJpSetInvestSisrDlg::CheckExistData_IJ(const T_SISR_USER_D& data, const int& nPos)
{
	if (nPos == T_SISR_USER_D::kI) {
		if (data.I_PLUS_D.dSus > 0 || data.I_PLUS_D.dSuc > 0 ||
			data.I_MINUS_D.dSus > 0 || data.I_MINUS_D.dSuc > 0)
			return TRUE;
	}
	else if (nPos == T_SISR_USER_D::kJ) {
		if (data.J_PLUS_D.dSus > 0 || data.J_PLUS_D.dSuc > 0 ||
			data.J_MINUS_D.dSus > 0 || data.J_MINUS_D.dSuc > 0)
			return TRUE;
	}
	return FALSE;
}