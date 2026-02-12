// NLLinkPrtItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBItemDlg.h"
#include "AntivibrationSupportGBItem_LNR.h"
#include "AntivibrationSupportGBItem_LRB.h"
#include "AntivibrationSupportGBItem_HDR.h"
#include "AntivibrationSupportGBItem_ESB.h"
#include "AntivibrationSupportGBItem_FPS.h"
#include <math.h>
#include "AntivibrationSupportGBListDlg.h"
#include "AntivibrationSupportGBDataTableDlg.h"

#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_dbLock\LockOption.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItemDlg dialog


CAntivibrationSupportGBItemDlg::CAntivibrationSupportGBItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAntivibrationSupportGBItemDlg::IDD, pParent)
{
	m_bModify = FALSE;

	//{{AFX_DATA_INIT(CAntivibrationSupportGBItemDlg)
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
	m_nPropType = 0;
	m_nCode = 0;
	m_nCategory = 0;
	m_nInputType = 0;
	m_nInputMethod = 1;
	m_bOnInit = TRUE;
	m_bChangeData = FALSE;
}

CAntivibrationSupportGBItemDlg::~CAntivibrationSupportGBItemDlg()
{
	delete m_pLNR;
	delete m_pLRB;
	delete m_pHDR;
	delete m_pESB;
	delete m_pFPS;
}

void CAntivibrationSupportGBItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntivibrationSupportGBItemDlg)
	DDX_Control(pDX, IDC_CMD_ANTISUPPORT_GB_NAME_EDIT, m_wndName);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION_EDIT, m_wndDescription);
	DDX_Radio(pDX, IDC_CMD_TYPE_LNR_RDO, m_nPropType);

	DDX_Radio(pDX, IDC_CMD_INPUT_TYPE_USER_RDO, m_nInputMethod);
	DDX_Control(pDX, IDC_CMD_INPUT_CODE_COMBO, m_wndCode);
	DDX_Control(pDX, IDC_CMD_INPUT_CLASSIFY_COMBO, m_wndCategory);
	DDX_Control(pDX, IDC_CMD_INPUT_TYPE_COMBO, m_wndInputType);
	DDX_Control(pDX, IDD_ETC_ANTISUPPORT_GB_PLACEHOLDER, m_wndPlaceHolder);

	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CAntivibrationSupportGBItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBItemDlg)
	ON_BN_CLICKED(IDC_CMD_INPUT_DATA_BUTTON, OnCmdRdferDatabaseBtn)
	ON_BN_CLICKED(IDC_CMD_TYPE_LNR_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_LRB_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_HDR_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_ESB_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_FPS_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_INPUT_TYPE_USER_RDO, OnCmdInputMethodRdo)
	ON_BN_CLICKED(IDC_CMD_USER_TYPE_DATA_RDO, OnCmdInputMethodRdo)
	ON_CBN_SELCHANGE(IDC_CMD_INPUT_CODE_COMBO, OnSelChangeInputCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_INPUT_CLASSIFY_COMBO, OnSelChangeInputCategoryCmb)
	ON_CBN_SELCHANGE(IDC_CMD_INPUT_TYPE_COMBO, OnSelChangeInputTypeCmb)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItemDlg message handlers

BOOL CAntivibrationSupportGBItemDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	m_pLNR = new CAntivibrationSupportGBItem_LNR();
	m_pLRB = new CAntivibrationSupportGBItem_LRB();
	m_pHDR = new CAntivibrationSupportGBItem_HDR();
	m_pESB = new CAntivibrationSupportGBItem_ESB();
	m_pFPS = new CAntivibrationSupportGBItem_FPS();
	m_pLNR->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pLRB->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pHDR->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pESB->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pFPS->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_bAddUser = FALSE;
	if (!m_bModify)
	{
		m_Data.Initialize();
	}

	if (m_Data.nPropType == 0)
	{
		m_nCode = m_Data.LNR.nCode;
		m_nCategory = m_Data.LNR.nClassify;
		m_nInputType = m_Data.LNR.nInputType;
	}
	else if (m_Data.nPropType == 1)
	{
		m_nCode = m_Data.LRB.nCode;
		m_nCategory = m_Data.LRB.nClassify;
		m_nInputType = m_Data.LRB.nInputType;
	}
	else if (m_Data.nPropType == 2)
	{
		m_nCode = m_Data.HDR.nCode;
		m_nCategory = m_Data.HDR.nClassify;
		m_nInputType = m_Data.HDR.nInputType;
	}
	else if (m_Data.nPropType == 3)
	{
		m_nCode = m_Data.ESB.nCode;
		m_nCategory = m_Data.ESB.nClassify;
		m_nInputType = m_Data.ESB.nInputType;
	}
	else if (m_Data.nPropType == 4)
	{
		m_nCode = m_Data.FPS.nCode;
		m_nCategory = m_Data.FPS.nClassify;
		m_nInputType = m_Data.FPS.nInputType;
	}

	Data2Dlg();
	EnableDisableItemCtrls();

	UpdateData(FALSE);
	m_bOnInit = FALSE;

	return TRUE;
}

void CAntivibrationSupportGBItemDlg::Data2Dlg()
{
	UpdateData(TRUE);
	if (m_Data.nPropType < 0 || m_Data.nPropType > 4) return;

	m_wndName.SetWindowText(m_Data.PropName);
	m_wndDescription.SetWindowText(m_Data.Description);

	if (m_Data.nPropType == 0)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LNR_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_ESB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FPS_RDO))->SetCheck(FALSE);
		if (m_Data.LNR.nInputMethod == 0)
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(FALSE);

			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(FALSE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(TRUE);
			if (m_Data.LNR.nCode < 2)
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			else
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}

		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}
		m_pLNR->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.LNR.nCode);
		m_wndCategory.SetCurSel(m_Data.LNR.nClassify);
		m_wndInputType.SetCurSel(m_Data.LNR.nInputType);
	}
	else if (m_Data.nPropType == 1)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LNR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LRB_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_ESB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FPS_RDO))->SetCheck(FALSE);
		if (m_Data.LRB.nInputMethod == 0)
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(FALSE);

			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(FALSE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(TRUE);
			if (m_Data.LRB.nCode < 2)
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			else
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}

		m_pLRB->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.LRB.nCode);
		m_wndCategory.SetCurSel(m_Data.LRB.nClassify);
		m_wndInputType.SetCurSel(m_Data.LRB.nInputType);
	}
	else if (m_Data.nPropType == 2)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LNR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDR_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_ESB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FPS_RDO))->SetCheck(FALSE);
		if (m_Data.HDR.nInputMethod == 0)
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(FALSE);
			
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(FALSE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(TRUE);
			if (m_Data.HDR.nCode == 0)
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			else
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}
		if (m_wndCode.GetCurSel() == 0)
		{
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			m_wndCategory.ResetContent();
		}
		else
		{
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			InitInputCategoryCmb();
		}
		m_pHDR->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.HDR.nCode);
		m_wndCategory.SetCurSel(m_Data.HDR.nClassify);
		m_wndInputType.SetCurSel(m_Data.HDR.nInputType);
	}
	else if (m_Data.nPropType == 3)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LNR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_ESB_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FPS_RDO))->SetCheck(FALSE);
		if (m_Data.ESB.nInputMethod == 0)
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(FALSE);

			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(FALSE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(TRUE);
			if (m_Data.ESB.nCode == 0)
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			else
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}

		m_pESB->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.ESB.nCode);
		m_wndCategory.SetCurSel(m_Data.ESB.nClassify);
		m_wndInputType.SetCurSel(m_Data.ESB.nInputType);
	}
	else if (m_Data.nPropType == 4)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LNR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_LRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDR_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_ESB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FPS_RDO))->SetCheck(TRUE);
		if (m_Data.FPS.nInputMethod == 0)
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(TRUE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(FALSE);

			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(FALSE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(FALSE);
			((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(TRUE);
			if (m_Data.FPS.nCode == 0)
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			else
				GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}

		m_pFPS->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.FPS.nCode);
		m_wndCategory.SetCurSel(m_Data.FPS.nClassify);
		m_wndInputType.SetCurSel(m_Data.FPS.nInputType);
	}
	m_bAddUser = FALSE;
	UpdateData(FALSE);
}

BOOL CAntivibrationSupportGBItemDlg::Dlg2Data()
{
	UpdateData();

	if (m_Data.nPropType == 0)
	{
		m_pLNR->SetDlg2Data(m_Data);
		m_Data.LNR.nInputMethod = m_nInputMethod;
		m_Data.LNR.nCode = m_wndCode.GetCurSel();
		m_Data.LNR.nClassify = m_wndCategory.GetCurSel();
		m_Data.LNR.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.LNR.strClassify);
		m_wndInputType.GetWindowText(m_Data.LNR.strInputType);
		m_Data.nPropType = 0;
	}
	else if (m_Data.nPropType == 1)
	{
		m_pLRB->SetDlg2Data(m_Data);
		m_Data.LRB.nInputMethod = m_nInputMethod;
		m_Data.LRB.nCode = m_wndCode.GetCurSel();
		m_Data.LRB.nClassify = m_wndCategory.GetCurSel();
		m_Data.LRB.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.LRB.strClassify);
		m_wndInputType.GetWindowText(m_Data.LRB.strInputType);
		m_Data.nPropType = 1;
	}
	else if (m_Data.nPropType == 2)
	{
		m_pHDR->SetDlg2Data(m_Data);
		m_Data.HDR.nInputMethod = m_nInputMethod;
		m_Data.HDR.nCode = m_wndCode.GetCurSel();
		m_Data.HDR.nClassify = 0;
		m_Data.HDR.nInputType = m_wndInputType.GetCurSel();
		m_wndInputType.GetWindowText(m_Data.HDR.strInputType);
		m_Data.nPropType = 2;
	}
	else if (m_Data.nPropType == 3)
	{
		m_pESB->SetDlg2Data(m_Data);
		m_Data.ESB.nInputMethod = m_nInputMethod;
		m_Data.ESB.nCode = m_wndCode.GetCurSel();
		m_Data.ESB.nClassify = m_wndCategory.GetCurSel();
		m_Data.ESB.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.ESB.strClassify);
		m_wndInputType.GetWindowText(m_Data.ESB.strInputType);
		m_Data.nPropType = 3;
	}
	else if (m_Data.nPropType == 4)
	{
		m_pFPS->SetDlg2Data(m_Data);
		m_Data.FPS.nInputMethod = m_nInputMethod;
		m_Data.FPS.nCode = m_wndCode.GetCurSel();
		m_Data.FPS.nClassify = m_wndCategory.GetCurSel();
		m_Data.FPS.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.FPS.strClassify);
		m_wndInputType.GetWindowText(m_Data.FPS.strInputType);
		m_Data.nPropType = 4;
	}

	m_wndName.GetWindowText(m_Data.PropName);
	if (m_Data.PropName.Compare(_T("")) == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_BlankPropertyName));
		return FALSE;
	}
	m_wndDescription.GetWindowText(m_Data.Description);

	//	m_Data.nPropType = m_Data.nPropType;

	return TRUE;
}


void CAntivibrationSupportGBItemDlg::OnCmdRdferDatabaseBtn()
{
	UpdateData();
	int nCode = m_wndCode.GetCurSel();
	CString strClassify = _T("");
	m_wndCategory.GetWindowText(strClassify);
	int nRow = m_nInputCount;

	CAntivibrationSupportGBDataTableDlg dlg;
	dlg.SetParamData(m_Data.nPropType, nCode, nRow, strClassify);
	dlg.DoModal();
	BOOL bCancel = FALSE;
	if (dlg.GetParamData(m_Data, nCode, m_bAddUser, bCancel))
	{
		m_bChangeData = TRUE;
	}
	else
		return;
	
	switch (m_Data.nPropType)
	{
	case 0: m_Data.PropName = m_Data.LNR.strInputType;	break;
	case 1: m_Data.PropName = m_Data.LRB.strInputType;	break;
	case 2: m_Data.PropName = m_Data.HDR.strInputType;	break;
	case 3: m_Data.PropName = m_Data.ESB.strInputType;	break;
	case 4: m_Data.PropName = m_Data.FPS.strInputType;	break;
	default:
		break;
	}

	if (!bCancel && m_bChangeData)
	{
		if (m_bAddUser)
			InitInputCodeCmb();
		else
			Data2Dlg();
	}
}

void CAntivibrationSupportGBItemDlg::OnCmdPropTypeRdo()
{
	UpdateData(TRUE);
	SetRedraw(FALSE);
	m_bChangeData = FALSE;
	int nCurType = GetCheckedRadioButton(IDC_CMD_TYPE_LNR_RDO, IDC_CMD_TYPE_FPS_RDO);
	if (nCurType == IDC_CMD_TYPE_LNR_RDO)
	{
		m_Data.nPropType = 0;
	}
	else if (nCurType == IDC_CMD_TYPE_LRB_RDO)
	{
		m_Data.nPropType = 1;
	}
	else if (nCurType == IDC_CMD_TYPE_HDR_RDO)
	{
		m_Data.nPropType = 2;
	}
	else if (nCurType == IDC_CMD_TYPE_ESB_RDO)
	{
		m_Data.nPropType = 3;
	}
	else if (nCurType == IDC_CMD_TYPE_FPS_RDO)
	{
		m_Data.nPropType = 4;
	}
	m_nInputMethod = 1;
	((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(TRUE);
	m_wndCode.SetCurSel(0);
	OnCmdInputMethodRdo();
	InitInputCategoryCmb();
	OnSelChangeInputTypeCmb();
	UpdateData(FALSE);
	SetRedraw(TRUE);
	RedrawWindow();
	
}

void CAntivibrationSupportGBItemDlg::OnCmdInputMethodRdo()
{
	m_wndName.Clear();
	UpdateData(TRUE);
	int nCurType = GetCheckedRadioButton(IDC_CMD_INPUT_TYPE_USER_RDO, IDC_CMD_USER_TYPE_DATA_RDO);
	if (nCurType == IDC_CMD_INPUT_TYPE_USER_RDO)
	{
		GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(FALSE);
		m_nInputMethod = 0;
	}
	else
	{
		
		GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		InitInputCodeCmb();
		if (m_wndCode.GetCurSel() < 2)
		{
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
		}
		if (m_wndCode.GetCurSel() == 0 && m_Data.nPropType == 2)
		{
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
			m_wndCategory.ResetContent();
		}
		else
		{
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			InitInputCategoryCmb();
		}
		m_nInputMethod = 1;
		if (m_Data.nPropType == 0)
		{
			m_Data.LNR.Initialize();
			m_pLNR->SetData2Dlg(m_Data);
		}
		else if (m_Data.nPropType == 1)
		{
			m_Data.LRB.Initialize();
			m_pLRB->SetData2Dlg(m_Data);
		}
		else if (m_Data.nPropType == 2)
		{
			m_Data.HDR.Initialize();
			m_pHDR->SetData2Dlg(m_Data);
		}
		else if (m_Data.nPropType == 3)
		{
			m_Data.ESB.Initialize();
			m_pESB->SetData2Dlg(m_Data);
		}
		else if (m_Data.nPropType == 4)
		{
			m_Data.FPS.Initialize();
			m_pFPS->SetData2Dlg(m_Data);
		}
		CString strName;
		m_wndInputType.GetWindowText(strName);
		m_wndName.SetWindowText(strName);
	}
	EnableDisableItemCtrls();
	if (m_Data.nPropType == 0)
	{
		m_pLNR->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 1)
	{
		m_pLRB->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 2)
	{
		m_pHDR->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 3)
	{
		m_pESB->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 4)
	{
		m_pFPS->EnableDisableItemCtrls(m_nInputMethod);
	}

	UpdateData(FALSE);
	RedrawWindow();
}

void CAntivibrationSupportGBItemDlg::OnSelChangeInputCodeCmb()
{
	m_nCode = m_wndCode.GetCurSel();
	CString strCode = _T("");
	m_wndCode.GetWindowText(strCode);
	InitInputCategoryCmb();
	if (strCode == _LS(IDS_CMD_ISOTATION_GB_CODE))
	{
		GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
		if (m_nPropType == 2)
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
	}
	else if (strCode == _LS(IDS_CMD_ISOTATION_GB_CODE_RUBBER))
	{
		GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
		if (m_nPropType == 2)
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
	}
		

}

void CAntivibrationSupportGBItemDlg::InitInputCodeCmb()
{
	m_wndCode.ResetContent();
	CString strCode = _LS(IDS_CMD_ISOTATION_GB_CODE);
	m_wndCode.AddString(strCode);
	if (m_Data.nPropType < 2)
	{
		CString strRubber = _LS(IDS_CMD_ISOTATION_GB_CODE_RUBBER);
		m_wndCode.AddString(strRubber);
		m_wndCode.SetCurSel(1);
	}
	IAntivibrationSupportGBDBMgr::Inst()->InitUserDefineFolder(m_Data.nPropType, m_UserD);
	if (m_Data.nPropType == 0)
	{
		if (m_UserD.m_mapCodeLNR.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.LNR.nCode);
	}
	else if (m_Data.nPropType == 1)
	{
		if (m_UserD.m_mapCodeLRB.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.LRB.nCode);
	}
	else if (m_Data.nPropType == 2)
	{
		if (m_UserD.m_mapCodeHDR.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.HDR.nCode);
	}
	else if (m_Data.nPropType == 3)
	{
		if (m_UserD.m_mapCodeESB.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.ESB.nCode);
	}
	else if (m_Data.nPropType == 4)
	{
		if (m_UserD.m_mapCodeFPS.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.FPS.nCode);
	}

	
	m_nCode = m_wndCode.GetCurSel();
}

void CAntivibrationSupportGBItemDlg::OnSelChangeInputCategoryCmb()
{
	UpdateData();
	m_nCategory = m_wndCategory.GetCurSel();
	CString strCode = _T("");
	m_wndCode.GetWindowText(strCode);
	//InitInputCategoryCmb();
	if (strCode == _T("User"))
		InitUserInputTypeCmb();
	else
		InitInputTypeCmb();

	OnSelChangeInputTypeCmb();
}

void CAntivibrationSupportGBItemDlg::InitInputCategoryCmb()
{
	UpdateData();
	m_wndCategory.ResetContent();

	BOOL bCode = TRUE;
	CStringArray astrClassify;
	if (m_Data.nPropType == 0 || m_Data.nPropType == 1)
	{
		if (m_wndCode.GetCurSel() == 0)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetDBClassifyList(m_nPropType, astrClassify);
		}
		else if (m_wndCode.GetCurSel() == 1)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetDBClassifyList_JGT(m_nPropType, astrClassify);
		}
		else
			bCode = FALSE;
	}
	else
	{
		if (m_wndCode.GetCurSel() == 0)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetDBClassifyList(m_nPropType, astrClassify);
		}
		else
			bCode = FALSE;
	}

	if (!bCode)
	{
		IAntivibrationSupportGBDBMgr::Inst()->InitUserDefineFolder(m_Data.nPropType, m_UserD);
	}
	int i = 0;

	if (!bCode)
	{
		astrClassify.RemoveAll();
		IAntivibrationSupportGBDBMgr::Inst()->GetClassifyList(m_nPropType, 1, astrClassify);
		for (i = 0; i < astrClassify.GetCount(); i++)
		{
			m_wndCategory.AddString(astrClassify[i]);
		}
		m_wndCategory.SetCurSel(0);
		InitInputTypeCmb();
	}
	else
	{
		for (i = 0; i < astrClassify.GetCount(); i++)
		{
			m_wndCategory.AddString(astrClassify[i]);
		}

		m_nCategory = m_wndCategory.SetCurSel(0);
		InitInputTypeCmb();
	}

	if (!m_bModify)
		OnSelChangeInputTypeCmb();

}

void CAntivibrationSupportGBItemDlg::OnSelChangeInputTypeCmb()
{
	UpdateData(TRUE);
	int nClassify = m_wndCategory.GetCurSel();
	int nInputType = m_wndInputType.GetCurSel();
	m_wndCategory.SetCurSel(nClassify);
	m_wndInputType.SetCurSel(nInputType);

	CString strClassify, strInputType;
	m_wndCategory.GetWindowText(strClassify);
	m_wndInputType.GetWindowText(strInputType);

	CString strTemp = _T("");
	strTemp = strClassify + strInputType;
	m_Code.Initialize();
	m_UserD.Initialize();
	BOOL bCode = TRUE;
	if (m_Data.nPropType == 0 || m_Data.nPropType == 1)
	{
		if (m_wndCode.GetCurSel() == 0)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetCodeDB(m_Code);
		}
		else if (m_wndCode.GetCurSel() == 1)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetCodeDB_JGT(m_Code);
		}
		else
			bCode = FALSE;
	}
	else
	{
		if (m_wndCode.GetCurSel() == 0)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetCodeDB(m_Code);
		}
		else
			bCode = FALSE;
	}

	if (!bCode)
	{
		IAntivibrationSupportGBDBMgr::Inst()->InitUserDefineFolder(m_Data.nPropType, m_UserD);
	}
	
	if (m_Data.nPropType == 0)
	{
		if (bCode)
			m_Code.m_mapCodeLNR.Lookup(strTemp, m_Data.LNR);	
		else
			m_UserD.m_mapCodeLNR.Lookup(strTemp, m_Data.LNR);
		m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(m_Data);
		m_Data.LNR.nInputMethod = m_nInputMethod;
		m_Data.LNR.nCode = m_wndCode.GetCurSel();
		m_Data.LNR.nClassify = m_wndCategory.GetCurSel();
		m_Data.LNR.nInputType = m_wndInputType.GetCurSel();

		m_pLNR->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 1)
	{
		if (bCode)
			m_Code.m_mapCodeLRB.Lookup(strTemp, m_Data.LRB);	
		else
			m_UserD.m_mapCodeLRB.Lookup(strTemp, m_Data.LRB);
		m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(m_Data);
		m_Data.LRB.nInputMethod = m_nInputMethod;
		m_Data.LRB.nCode = m_wndCode.GetCurSel();
		m_Data.LRB.nClassify = m_wndCategory.GetCurSel();
		m_Data.LRB.nInputType = m_wndInputType.GetCurSel();
		m_pLRB->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 2)
	{
		if (bCode)
			m_Code.m_mapCodeHDR.Lookup(strInputType, m_Data.HDR);	
		else
			m_UserD.m_mapCodeHDR.Lookup(strTemp, m_Data.HDR);
		m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(m_Data);
		m_Data.HDR.nInputMethod = m_nInputMethod;
		m_Data.HDR.nCode = m_wndCode.GetCurSel();
		m_Data.HDR.nInputType = m_wndInputType.GetCurSel();
		m_pHDR->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 3)
	{
		if (bCode)
			m_Code.m_mapCodeESB.Lookup(strTemp, m_Data.ESB);		
		else
			m_UserD.m_mapCodeESB.Lookup(strTemp, m_Data.ESB);
		m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(m_Data);
		m_Data.ESB.nInputMethod = m_nInputMethod;
		m_Data.ESB.nCode = m_wndCode.GetCurSel();
		m_Data.ESB.nClassify = m_wndCategory.GetCurSel();
		m_Data.ESB.nInputType = m_wndInputType.GetCurSel();
		m_pESB->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 4)
	{
		if (bCode)
			m_Code.m_mapCodeFPS.Lookup(strTemp, m_Data.FPS);	
		else
			m_UserD.m_mapCodeFPS.Lookup(strTemp, m_Data.FPS);
		m_pDoc->m_pUnitCtrl->ConvertUnitAsgbOut(m_Data);
		m_Data.FPS.nInputMethod = m_nInputMethod;
		m_Data.FPS.nCode = m_wndCode.GetCurSel();
		m_Data.FPS.nClassify = m_wndCategory.GetCurSel();
		m_Data.FPS.nInputType = m_wndInputType.GetCurSel();
		m_pFPS->SetData2Dlg(m_Data);
	}

	EnableDisableItemCtrls();
	CString strName;
	m_wndInputType.GetWindowText(strName);
	m_wndName.SetWindowText(strName);
	UpdateData(FALSE);
}

void CAntivibrationSupportGBItemDlg::InitUserInputTypeCmb()
{
	m_wndInputType.ResetContent();
	CString strClassify;
	m_wndCategory.GetWindowText(strClassify);
	CStringArray astrInputType;
	IAntivibrationSupportGBDBMgr::Inst()->GetInputTypeList(strClassify, m_nPropType, astrInputType);
	switch (m_nPropType)
	{
	case 0:
	{
		for (int i = 0; i < astrInputType.GetCount(); i++)
		{
			m_wndInputType.AddString(astrInputType[i]);
		}
	}
	break;
	case 1:
	{
		for (int i = 0; i < astrInputType.GetCount(); i++)
		{
			m_wndInputType.AddString(astrInputType[i]);
		}
	}
	break;
	case 2:
	{
		for (int i = 0; i < astrInputType.GetCount(); i++)
		{
			m_wndInputType.AddString(astrInputType[i]);
		}
	}
	break;
	case 3:
	{
		for (int i = 0; i < astrInputType.GetCount(); i++)
		{
			m_wndInputType.AddString(astrInputType[i]);
		}
	}
	break;
	case 4:
	{
		for (int i = 0; i < astrInputType.GetCount(); i++)
		{
			m_wndInputType.AddString(astrInputType[i]);
		}
	}
	break;
	default:
		break;
	}
	m_wndInputType.SetCurSel(0);
}

void CAntivibrationSupportGBItemDlg::InitInputTypeCmb()
{
	CStringArray astrInputType;
	CString _tcscategory;
	m_wndCategory.GetWindowText(_tcscategory);
	m_nCategory = m_wndCategory.GetCurSel();
	BOOL bCode = TRUE;
	if (m_Data.nPropType == 0 || m_Data.nPropType == 1)
	{
		if (m_wndCode.GetCurSel() == 0)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetDBInputTypeList(m_nPropType, m_nCategory, astrInputType);
		}
		else if (m_wndCode.GetCurSel() == 1)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetDBInputTypeList_JGT(m_nPropType, m_nCategory, astrInputType);
		}
		else
			bCode = FALSE;
	}
	else
	{
		if (m_wndCode.GetCurSel() == 0)
		{
			IAntivibrationSupportGBDBMgr::Inst()->GetDBInputTypeList(m_nPropType, m_nCategory, astrInputType);
		}
		else
			bCode = FALSE;
	}

	int i = 0;
	m_wndInputType.ResetContent();
	int nIndex = 0;
	if (!bCode)
	{
		astrInputType.RemoveAll();
		IAntivibrationSupportGBDBMgr::Inst()->GetInputTypeList(_tcscategory, m_nPropType, astrInputType);
		
	}
	for (i = 0; i < astrInputType.GetCount(); i++)
	{
		m_wndInputType.AddString(astrInputType[i]);
		nIndex++;
	}

	m_nInputCount = astrInputType.GetCount();
	m_wndInputType.SetCurSel(0);
}

void CAntivibrationSupportGBItemDlg::OnCmdOK()
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CAntivibrationSupportGBItemDlg::OnCmdCancel()
{
	CDialogMove::OnCancel();
}

BOOL CAntivibrationSupportGBItemDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	T_ASGB_D Data;
	m_pDoc->m_pAttrCtrl->GetAsgb(m_OldKey, Data);   // T_NLLP_D의 key를 주고 T_NLLP_D를 받는다.
	if (m_bModify)
	{
		m_Data.nSeq = Data.nSeq;   // old data의 seq를 현재 data의 seq로 넣어준다.
		bSuccess = m_pDoc->m_pDataCtrl->ModifyAsgb(m_OldKey, m_OldKey, m_Data);
	}
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddAsgb(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

void CAntivibrationSupportGBItemDlg::OnCmdApply()
{
	if (!ApplyOrOK()) return;
	m_OldKey = m_Key;
}

void CAntivibrationSupportGBItemDlg::EnableDisableItemCtrls()
{
	if (m_Data.nPropType == 0)
	{
		m_pLNR->UpdateData(FALSE);
		m_pLNR->ShowWindow(SW_SHOW);
		m_pLRB->ShowWindow(SW_HIDE);
		m_pHDR->ShowWindow(SW_HIDE);
		m_pESB->ShowWindow(SW_HIDE);
		m_pFPS->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 1)
	{
		m_pLRB->UpdateData(FALSE);
		m_pLNR->ShowWindow(SW_HIDE);
		m_pLRB->ShowWindow(SW_SHOW);
		m_pHDR->ShowWindow(SW_HIDE);
		m_pESB->ShowWindow(SW_HIDE);
		m_pFPS->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 2)
	{
		m_pHDR->UpdateData(FALSE);
		m_pLNR->ShowWindow(SW_HIDE);
		m_pLRB->ShowWindow(SW_HIDE);
		m_pHDR->ShowWindow(SW_SHOW);
		m_pESB->ShowWindow(SW_HIDE);
		m_pFPS->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 3)
	{
		m_pESB->UpdateData(FALSE);
		m_pLNR->ShowWindow(SW_HIDE);
		m_pLRB->ShowWindow(SW_HIDE);
		m_pHDR->ShowWindow(SW_HIDE);
		m_pESB->ShowWindow(SW_SHOW);
		m_pFPS->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 4)
	{
		m_pFPS->UpdateData(FALSE);
		m_pLNR->ShowWindow(SW_HIDE);
		m_pLRB->ShowWindow(SW_HIDE);
		m_pHDR->ShowWindow(SW_HIDE);
		m_pESB->ShowWindow(SW_HIDE);
		m_pFPS->ShowWindow(SW_SHOW);
	}
	RedrawWindow();
}


void CAntivibrationSupportGBItemDlg::LayoutChildDlg(CChildDialog* pChildDlg)
{
	CRect	DlgRect, ParentRect, PHRect;
	GetClientRect(&ParentRect);
	pChildDlg->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);

	int		Sx, Sy, Wx, Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();

	m_wndPlaceHolder.SetWindowPos(NULL, Sx, Sy, Wx, Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	//
	CRect ExRect, ApplyRect;
	GetDlgItem(IDOK)->GetWindowRect(&ExRect);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(&ApplyRect);

	Wy = ExRect.Height();
	ExRect.top = PHRect.bottom + 5;
	ExRect.bottom = ExRect.top + Wy;
	ApplyRect.top = ExRect.top;
	ApplyRect.bottom = ExRect.bottom;
	ScreenToClient(&ExRect); ScreenToClient(&ApplyRect);
	int nOff = ParentRect.bottom - ExRect.bottom;
	GetDlgItem(IDOK)->MoveWindow(ExRect);
	GetDlgItem(IDC_CMD_APPLY)->MoveWindow(ApplyRect);
	GetWindowRect(&ParentRect);
	SetWindowPos(NULL, ParentRect.left, ParentRect.top, ParentRect.Width(), ParentRect.Height() - nOff + 10, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CAntivibrationSupportGBItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
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
	default:
		break;
	}
}

void CAntivibrationSupportGBItemDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	//T_KEY prevKey = CDlgUtil::CobxGetCurSelItemData(m_wndSeisDvProp, m_wndSeisDvProp.GetCurSel());

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_ASGB_ADD):
		case(UR_ASGB_MFD):
			//OnSelChangeSeisTypeCmb();
			//CDlgUtil::CobxSetCurSelItemData(m_wndSeisDvProp, prevKey);
			break;
		case(UR_ASGB_DEL):
		{
			//OnSelChangeSeisTypeCmb();
			//if(prevKey!=nKey) CDlgUtil::CobxSetCurSelItemData(m_wndSeisDvProp, prevKey);
			//else m_wndSeisDvProp.SetCurSel(0);
		}
		break;
		default:
			break;
		}
	} // end of while

	//if(m_wndSeisDvProp.GetCurSel()<0) m_wndSeisDvProp.SetCurSel(0);
}

