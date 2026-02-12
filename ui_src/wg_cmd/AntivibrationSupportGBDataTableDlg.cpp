// NLLinkPrtListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBDataTableDlg.h"
#include "AntivibrationSupportGBItemDlg.h"
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
// CAntivibrationSupportGBDataTableDlg dialog


CAntivibrationSupportGBDataTableDlg::CAntivibrationSupportGBDataTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAntivibrationSupportGBDataTableDlg::IDD, pParent),m_wndGrid()
{
	//{{AFX_DATA_INIT(CAntivibrationSupportGBDataTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bInitSelect = FALSE;
	m_bOK = FALSE;
	m_bSave = FALSE;
	m_bCancel = FALSE;
}

CAntivibrationSupportGBDataTableDlg:: ~CAntivibrationSupportGBDataTableDlg()
{

}

void CAntivibrationSupportGBDataTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntivibrationSupportGBDataTableDlg)
	DDX_Radio(pDX, IDC_ETC_ANTISUPPORT_GB_DB_RADIO, m_nType);
	DDX_Control(pDX, IDC_ETC_ANTISUPPORT_GB_TYPE_EDIT, m_Classify);
	DDX_Control(pDX, IDC_ETC_ANTISUPPORT_GB_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAntivibrationSupportGBDataTableDlg, CDialogMove)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBDataTableDlg)
	ON_BN_CLICKED(IDC_ETC_ANTISUPPORT_GB_DB_RADIO, OnChkDBorUserRatio)
	ON_BN_CLICKED(IDC_ETC_ANTISUPPORT_GB_USER_RADIO, OnChkDBorUserRatio)
	ON_BN_CLICKED(IDC_CMD_BTN_SAVE, OnCmdBtnSave)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOK)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CAntivibrationSupportGBDataTableDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	if (!CreateFolder())
		return FALSE;

	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CheckRadioButton(IDC_ETC_ANTISUPPORT_GB_DB_RADIO, IDC_ETC_ANTISUPPORT_GB_USER_RADIO, IDC_ETC_ANTISUPPORT_GB_DB_RADIO);
	m_Classify.SetWindowText(strInitName);
	GetDlgItem(IDC_ETC_ANTISUPPORT_GB_TYPE_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_BTN_OK)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMD_BTN_SAVE)->ShowWindow(FALSE);

	m_wndGrid.Initialize(m_nPropType, m_nCode, strInitName, m_nRow);
	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_wndGrid.MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAntivibrationSupportGBDataTableDlg::OnChkDBorUserRatio()
{
	if (GetCheckedRadioButton(IDC_ETC_ANTISUPPORT_GB_DB_RADIO, IDC_ETC_ANTISUPPORT_GB_USER_RADIO) == IDC_ETC_ANTISUPPORT_GB_DB_RADIO)
	{
		m_nType = 0;
		GetDlgItem(IDC_CMD_BTN_OK)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_BTN_SAVE)->ShowWindow(FALSE);
		GetDlgItem(IDC_ETC_ANTISUPPORT_GB_TYPE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_ANTISUPPORT_GB_TYPE_EDIT)->SetWindowText(strInitName);
		m_wndGrid.Initialize(m_nPropType, m_nCode, strInitName, m_nRow);
	}
	else
	{
		m_nType = 1;
		GetDlgItem(IDC_CMD_BTN_OK)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_BTN_SAVE)->ShowWindow(TRUE);
		GetDlgItem(IDC_ETC_ANTISUPPORT_GB_TYPE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_ANTISUPPORT_GB_TYPE_EDIT)->SetWindowText(strInitName);
		m_wndGrid.Initialize(m_nPropType, -1, strInitName, m_nRow);
	}
}

//void CAntivibrationSupportGBDataTableDlg::OnLButtonDblClkOnGrid(T_ASGB_D &data)
//{
//	m_Data = data;
//	m_bOK = TRUE;
//	CDialogMove::OnOK();
//}

void CAntivibrationSupportGBDataTableDlg::OnCmdBtnSave()
{
	UpdateData(TRUE);
	CString strClassify;
	m_Classify.GetWindowText(strClassify);
	if (!m_wndGrid.SaveUserDefineData2Ini(strClassify))
		return;
	m_bSave = TRUE;
}

void CAntivibrationSupportGBDataTableDlg::OnCmdBtnOK() 
{
	T_ASGB_D data;	data.Initialize();
	m_wndGrid.Grid2Data(data);
	m_Data = data;
	CString strClassify = _T("");
	m_Classify.GetWindowText(strClassify);
	switch (m_nPropType)
	{
	case 0: m_Data.LNR.strClassify = strClassify; break;
	case 1: m_Data.LRB.strClassify = strClassify; break;
	case 2: m_Data.HDR.strClassify = strClassify; break;
	case 3: m_Data.ESB.strClassify = strClassify; break;
	case 4: m_Data.FPS.strClassify = strClassify; break;
	default:
		break;
	}
	CDialogMove::OnOK();
}

void CAntivibrationSupportGBDataTableDlg::OnCmdBtnCancel() 
{
	m_bCancel = TRUE;
	CDialogMove::OnCancel();
}

void CAntivibrationSupportGBDataTableDlg::SetParamData(int nPropType, int nCode, int nRow, CString strClassify)
{
	m_nPropType = nPropType;
	m_nCode = nCode;
	m_nRow = nRow;
	strInitName = strClassify;
}

BOOL CAntivibrationSupportGBDataTableDlg::GetParamData(T_ASGB_D &ParamData, int &nCode, BOOL &bAdd, BOOL &bCancel)
{
	ParamData = m_Data;
	ParamData.nPropType = m_nPropType;
	nCode = m_nCode;
	bAdd = m_bSave;
	bCancel = m_bCancel;
	return TRUE;
}

BOOL CAntivibrationSupportGBDataTableDlg::CreateFolder()
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
	CString strWorkingFolder1 = strWorkingFolder + _T("\\IsolatorGB");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder1))
		return FALSE;

	CString strWorkingFolder2 = strWorkingFolder1 + _T("\\LNR");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	CString strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\LRB");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\HDR");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\ESB");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	strWorkingFolder2 = strWorkingFolder1 + _T("\\FPS");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder2))
		return FALSE;
	strWorkingFolder3 = strWorkingFolder2 + _T("\\User");
	if (!MFileCtrl::MakeDirectory(strWorkingFolder3))
		return FALSE;

	return TRUE;
}
