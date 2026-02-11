// NLLinkPrtListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBDataTableDlg.h"
#include "DamperGBItemDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\MIT_lib\MFileCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDamperGBDataTableDlg dialog


CDamperGBDataTableDlg::CDamperGBDataTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDamperGBDataTableDlg::IDD, pParent),m_wndGrid()
{
	//{{AFX_DATA_INIT(CDamperGBDataTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bInitSelect = FALSE;
	m_bOK = FALSE;
	m_bSave = FALSE;
	m_bCancel = TRUE;
}

CDamperGBDataTableDlg:: ~CDamperGBDataTableDlg()
{

}

void CDamperGBDataTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBDataTableDlg)
	DDX_Radio(pDX, IDC_ETC_DAMPER_GB_DB_RADIO, m_nType);
	DDX_Control(pDX, IDC_ETC_DAMPER_GB_TYPE_EDIT, m_Classify);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDamperGBDataTableDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDamperGBDataTableDlg)
	ON_BN_CLICKED(IDC_ETC_DAMPER_GB_DB_RADIO, OnChkDBorUserRatio)
	ON_BN_CLICKED(IDC_ETC_DAMPER_GB_USER_RADIO, OnChkDBorUserRatio)
	ON_BN_CLICKED(IDC_CMD_BTN_SAVE, OnCmdBtnSave)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOK)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CDamperGBDataTableDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	if (!CreateFolder())
		return FALSE;

	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CheckRadioButton(IDC_ETC_DAMPER_GB_DB_RADIO, IDC_ETC_DAMPER_GB_USER_RADIO, IDC_ETC_DAMPER_GB_DB_RADIO);
	m_Classify.SetWindowText(strInitName);
	GetDlgItem(IDC_ETC_DAMPER_GB_TYPE_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_BTN_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_BTN_OK)->ShowWindow(TRUE);

	m_wndGrid.SubclassDlgItem(IDC_ETC_DAMPER_GB_GRID, this);
	if (m_nPropType == 3)
	{
		m_wndGrid.Initialize(-1, m_nPropType, m_nCategory, strInitName, m_nRow);
		CheckRadioButton(IDC_ETC_DAMPER_GB_DB_RADIO, IDC_ETC_DAMPER_GB_USER_RADIO, IDC_ETC_DAMPER_GB_USER_RADIO);
		GetDlgItem(IDC_ETC_DAMPER_GB_DB_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_DAMPER_GB_TYPE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_BTN_SAVE)->EnableWindow(TRUE);
	}		
	else
		m_wndGrid.Initialize(m_nCode, m_nPropType, m_nCategory, strInitName, m_nRow);
	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_wndGrid.MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBDataTableDlg::OnChkDBorUserRatio()
{
	if (GetCheckedRadioButton(IDC_ETC_DAMPER_GB_DB_RADIO, IDC_ETC_DAMPER_GB_USER_RADIO) == IDC_ETC_DAMPER_GB_DB_RADIO)
	{
		m_nType = 0;
		GetDlgItem(IDC_CMD_BTN_OK)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_BTN_SAVE)->ShowWindow(FALSE);
		GetDlgItem(IDC_ETC_DAMPER_GB_TYPE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_DAMPER_GB_TYPE_EDIT)->SetWindowText(strInitName);
		m_wndGrid.Initialize(m_nCode, m_nPropType, m_nCategory, strInitName, m_nRow);
	}
	else
	{
		m_nType = 1;
		GetDlgItem(IDC_CMD_BTN_OK)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_BTN_SAVE)->ShowWindow(TRUE);
		GetDlgItem(IDC_ETC_DAMPER_GB_TYPE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_DAMPER_GB_TYPE_EDIT)->SetWindowText(strInitName);
		m_wndGrid.Initialize(-1, m_nPropType, m_nCategory, strInitName, m_nRow);
	}
}

void CDamperGBDataTableDlg::OnLButtonDblClkOnGrid(T_DMGB_D &data)
{
	if (GetCheckedRadioButton(IDC_ETC_DAMPER_GB_DB_RADIO, IDC_ETC_DAMPER_GB_USER_RADIO) == IDC_ETC_DAMPER_GB_USER_RADIO)
		return;
	m_Data = data;
	m_bOK = TRUE;
	CDialogMove::OnOK();
}

void CDamperGBDataTableDlg::OnCmdBtnSave()
{
	UpdateData(TRUE);
	CString strClassify;
	m_Classify.GetWindowText(strClassify);
	if (!m_wndGrid.SaveUserDefineData2Ini(strClassify))
		return;
	m_bSave = TRUE;
}

void CDamperGBDataTableDlg::OnCmdBtnOK() 
{
	m_bCancel = FALSE;
	T_DMGB_D data;	data.Initialize();
	m_wndGrid.Grid2Data(data);
	m_Data = data;
	CString strClassify = _T("");
	m_Classify.GetWindowText(strClassify);
	switch (m_nPropType)
	{
	case 0: m_Data.BRB.strClassify = strClassify; break;
	case 1: m_Data.MYD.strClassify = strClassify; break;
	case 2: m_Data.FD.strClassify = strClassify; break;
	case 3: m_Data.VFD.strClassify = strClassify; break;
	case 4: m_Data.HDRD.strClassify = strClassify; break;
	case 5: m_Data.VED.strClassify = strClassify; break;
	default:
		break;
	}
	CDialogMove::OnOK();
}

void CDamperGBDataTableDlg::OnCmdBtnCancel() 
{
	m_bCancel = TRUE;
	CDialogMove::OnCancel();
}

void CDamperGBDataTableDlg::SetParamData(int nPropType, int nCode, int nCategory,int nRow, CString strClassify)
{
	m_nPropType = nPropType;
	m_nCode = nCode;
	m_nCategory = nCategory;
	m_nRow = nRow;
	strInitName = strClassify;
}

BOOL CDamperGBDataTableDlg::GetParamData(T_DMGB_D &ParamData, int& nCode, BOOL& bAdd, BOOL &bCancel)
{
	ParamData = m_Data;
	ParamData.nPropType = m_nPropType;
	nCode = m_nCode;
	bAdd = m_bSave;
	bCancel = m_bCancel;
	return TRUE;
}

BOOL CDamperGBDataTableDlg::CreateFolder()
{
	CString strExeFolder;
	::GetModuleFileName(NULL, strExeFolder.GetBuffer(_MAX_PATH), _MAX_PATH);
	strExeFolder.ReleaseBuffer();
	int nFind = strExeFolder.ReverseFind('\\');
	ASSERT(nFind != -1);
	if (nFind != -1)
		strExeFolder = strExeFolder.Left(nFind + 1);

	CString strWorkingFolder = strExeFolder + _T("SDCDBase");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder))
		return FALSE;
	CString strWorkingFolder1 = strWorkingFolder + _T("\\DamperGB");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder1))
		return FALSE;

	CString strWorkingFolder2 = strWorkingFolder1 + _T("\\BRB");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	CString strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\MYD");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\FD");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\VFD");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\HDRD");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\VED");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	return TRUE;
}