// NLLinkPrtItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBItemDlg.h"
#include "DamperGBItem1_BRB.h"
#include "DamperGBItem2_MYD.h"
#include "DamperGBItem3_FD.h"
#include "DamperGBItem4_VFD.h"
#include "DamperGBItem5_HDRD.h"
#include "DamperGBItem6_VED.h"
#include <math.h>
#include "DamperGBListDlg.h"
#include "DamperGBDataTableDlg.h"

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
// CDamperGBItemDlg dialog


CDamperGBItemDlg::CDamperGBItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDamperGBItemDlg::IDD, pParent)
{
	m_bModify = FALSE;

	//{{AFX_DATA_INIT(CDamperGBItemDlg)
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
	m_nPropType = 0;
	m_nCode = 0;
	m_nCategory = 0;
	m_nInputType = 0;
	m_nInputMethod = 1;
	m_bOnInit = TRUE;
	m_bInit_VFD = TRUE;
	m_bChangeData = FALSE;
}

CDamperGBItemDlg::~CDamperGBItemDlg()
{
	delete m_pBRB;
	delete m_pMYD;
	delete m_pFD;
	delete m_pVFD;
	delete m_pHDRD;
	delete m_pVED;
}

void CDamperGBItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBItemDlg)
	DDX_Control(pDX, IDC_CMD_DAMPER_GB_NAME_EDIT, m_wndName);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION_EDIT, m_wndDescription);
	DDX_Radio(pDX, IDC_CMD_TYPE_BRB_RDO, m_nPropType);

	DDX_Radio(pDX, IDC_CMD_INPUT_TYPE_USER_RDO, m_nInputMethod);
	DDX_Control(pDX, IDC_CMD_INPUT_CODE_COMBO, m_wndCode);
	DDX_Control(pDX, IDC_CMD_INPUT_CLASSIFY_COMBO, m_wndCategory);
	DDX_Control(pDX, IDC_CMD_INPUT_TYPE_COMBO, m_wndInputType);
	DDX_Control(pDX, IDD_ETC_DAMPER_GB_PLACEHOLDER, m_wndPlaceHolder);

	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CDamperGBItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDamperGBItemDlg)
	ON_BN_CLICKED(IDC_CMD_INPUT_DATA_BUTTON, OnCmdRdferDatabaseBtn)
	ON_BN_CLICKED(IDC_CMD_TYPE_BRB_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_MYD_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_FD_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_VD_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_HDRD_RDO, OnCmdPropTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TYPE_VED_RDO, OnCmdPropTypeRdo)
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
// CDamperGBItemDlg message handlers

BOOL CDamperGBItemDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	if (!m_bModify)
	{
		m_Data.Initialize();
	}

	if (m_Data.nPropType == 0)
	{
		m_nCode = m_Data.BRB.nCode;
		m_nCategory = m_Data.BRB.nClassify;
		m_nInputType = m_Data.BRB.nInputType;
	}
	else if (m_Data.nPropType == 1)
	{
		m_nCode = m_Data.MYD.nCode;
		m_nCategory = m_Data.MYD.nClassify;
		m_nInputType = m_Data.MYD.nInputType;
	}
	else if (m_Data.nPropType == 2)
	{
		m_nCode = m_Data.FD.nCode;
		m_nCategory = m_Data.FD.nClassify;
		m_nInputType = m_Data.FD.nInputType;
	}
	else if (m_Data.nPropType == 3)
	{
		m_nCode = m_Data.VFD.nCode;
		m_nCategory = m_Data.VFD.nClassify;
		m_nInputType = m_Data.VFD.nInputType;
	}
	else if (m_Data.nPropType == 4)
	{
		m_nCode = m_Data.HDRD.nCode;
		m_nCategory = m_Data.HDRD.nClassify;
		m_nInputType = m_Data.HDRD.nInputType;
	}
	else if (m_Data.nPropType == 5)
	{
		m_nCode = m_Data.VED.nCode;
		m_nCategory = m_Data.VED.nClassify;
		m_nInputType = m_Data.VED.nInputType;
	}

	m_pBRB = new CDamperGBItem_BRB();
	m_pMYD = new CDamperGBItem_MYD();
	m_pFD = new CDamperGBItem_FD();
	m_pVFD = new CDamperGBItem_VFD();
	m_pHDRD = new CDamperGBItem_HDRD();
	m_pVED = new CDamperGBItem_VED();
	m_pBRB->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pMYD->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pFD->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pVFD->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pHDRD->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_pVED->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_bAddUser = FALSE;

	Data2Dlg();
	EnableDisableItemCtrls();

	UpdateData(FALSE);
	m_bOnInit = FALSE;

	return TRUE;
}

void CDamperGBItemDlg::Data2Dlg()
{
	UpdateData(TRUE);
	if (m_Data.nPropType < 0 || m_Data.nPropType > 5) return;

	m_nPropType = m_Data.nPropType;
	m_wndName.SetWindowText(m_Data.PropName);
	m_wndDescription.SetWindowText(m_Data.Description);

	if (m_Data.nPropType == 0)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_BRB_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_MYD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDRD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VED_RDO))->SetCheck(FALSE);
		if (m_Data.BRB.nInputMethod == 0)
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
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);		
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}

		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}
		m_pBRB->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.BRB.nCode);
		m_wndCategory.SetCurSel(m_Data.BRB.nClassify);
		m_wndInputType.SetCurSel(m_Data.BRB.nInputType);
		m_nInputMethod = m_Data.BRB.nInputMethod;
	}
	else if (m_Data.nPropType == 1)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_BRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_MYD_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDRD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VED_RDO))->SetCheck(FALSE);
		if (m_Data.MYD.nInputMethod == 0)
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
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}

		m_pMYD->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.MYD.nCode);
		m_wndCategory.SetCurSel(m_Data.MYD.nClassify);
		m_wndInputType.SetCurSel(m_Data.MYD.nInputType);
		m_nInputMethod = m_Data.MYD.nInputMethod;
	}
	else if (m_Data.nPropType == 2)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_BRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_MYD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FD_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDRD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VED_RDO))->SetCheck(FALSE);
		if (m_Data.FD.nInputMethod == 0)
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
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
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
		m_pFD->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.FD.nCode);
		m_wndCategory.SetCurSel(m_Data.FD.nClassify);
		m_wndInputType.SetCurSel(m_Data.FD.nInputType);
		m_nInputMethod = m_Data.FD.nInputMethod;
	}
	else if (m_Data.nPropType == 3)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_BRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_MYD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VD_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDRD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VED_RDO))->SetCheck(FALSE);
		if (m_Data.VFD.nInputMethod == 0)
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
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		
		m_pVFD->SetData2Dlg(m_Data);
		m_nInputMethod = m_Data.VFD.nInputMethod;
		m_bInit_VFD = FALSE;
		if (m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
			m_wndCode.SetCurSel(m_Data.VFD.nCode);
			m_wndCategory.SetCurSel(m_Data.VFD.nClassify);
			m_wndInputType.SetCurSel(m_Data.VFD.nInputType);
		}
	}
	else if (m_Data.nPropType == 4)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_BRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_MYD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDRD_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VED_RDO))->SetCheck(FALSE);
		if (m_Data.HDRD.nInputMethod == 0)
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
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}

		m_pHDRD->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.HDRD.nCode);
		m_wndCategory.SetCurSel(m_Data.HDRD.nClassify);
		m_wndInputType.SetCurSel(m_Data.HDRD.nInputType);
		m_nInputMethod = m_Data.HDRD.nInputMethod;
	}
	else if (m_Data.nPropType == 5)
	{
		((CButton *)GetDlgItem(IDC_CMD_TYPE_BRB_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_MYD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_FD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_HDRD_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_TYPE_VED_RDO))->SetCheck(TRUE);
		if (m_Data.VED.nInputMethod == 0)
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
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		}
		if (!m_bChangeData || m_bAddUser)
		{
			InitInputCodeCmb();
			InitInputCategoryCmb();
		}

		m_pVED->SetData2Dlg(m_Data);
		m_wndCode.SetCurSel(m_Data.VED.nCode);
		m_wndCategory.SetCurSel(m_Data.VED.nClassify);
		m_wndInputType.SetCurSel(m_Data.VED.nInputType);
		m_nInputMethod = m_Data.VED.nInputMethod;
	}
	UpdateData(FALSE);
}

BOOL CDamperGBItemDlg::Dlg2Data()
{
	UpdateData();

	if (m_Data.nPropType == 0)
	{
		if (!m_pBRB->SetDlg2Data(m_Data))
			return FALSE;
		m_Data.BRB.nInputMethod = m_nInputMethod;
		m_Data.BRB.nCode = m_wndCode.GetCurSel();
		m_Data.BRB.nClassify = m_wndCategory.GetCurSel();
		m_Data.BRB.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.BRB.strClassify);
		m_wndInputType.GetWindowText(m_Data.BRB.strInputType);
	}
	else if (m_Data.nPropType == 1)
	{
		if (!m_pMYD->SetDlg2Data(m_Data))
			return FALSE;
		m_Data.MYD.nInputMethod = m_nInputMethod;
		m_Data.MYD.nCode = m_wndCode.GetCurSel();
		m_Data.MYD.nClassify = m_wndCategory.GetCurSel();
		m_Data.MYD.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.MYD.strClassify);
		m_wndInputType.GetWindowText(m_Data.MYD.strInputType);
	}
	else if (m_Data.nPropType == 2)
	{
		if (!m_pFD->SetDlg2Data(m_Data))
			return FALSE;
		m_Data.FD.nInputMethod = m_nInputMethod;
		m_Data.FD.nCode = m_wndCode.GetCurSel();
		m_Data.FD.nClassify = 0;
		m_Data.FD.nInputType = m_wndInputType.GetCurSel();
		m_wndInputType.GetWindowText(m_Data.FD.strInputType);
	}
	else if (m_Data.nPropType == 3)
	{
		if (!m_pVFD->SetDlg2Data(m_Data))
			return FALSE;
		m_Data.VFD.nInputMethod = m_nInputMethod;
		m_Data.VFD.nCode = m_wndCode.GetCurSel();
		m_Data.VFD.nClassify = m_wndCategory.GetCurSel();
		m_Data.VFD.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.VFD.strClassify);
		m_wndInputType.GetWindowText(m_Data.VFD.strInputType);
	}
	else if (m_Data.nPropType == 4)
	{
		if (!m_pHDRD->SetDlg2Data(m_Data))
			return FALSE;
		m_Data.HDRD.nInputMethod = m_nInputMethod;
		m_Data.HDRD.nCode = m_wndCode.GetCurSel();
		m_Data.HDRD.nClassify = m_wndCategory.GetCurSel();
		m_Data.HDRD.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.HDRD.strClassify);
		m_wndInputType.GetWindowText(m_Data.HDRD.strInputType);
	}
	else if (m_Data.nPropType == 5)
	{
		if (!m_pVED->SetDlg2Data(m_Data))
			return FALSE;
		m_Data.VED.nInputMethod = m_nInputMethod;
		m_Data.VED.nCode = m_wndCode.GetCurSel();
		m_Data.VED.nClassify = m_wndCategory.GetCurSel();
		m_Data.VED.nInputType = m_wndInputType.GetCurSel();
		m_wndCategory.GetWindowText(m_Data.VED.strClassify);
		m_wndInputType.GetWindowText(m_Data.VED.strInputType);
	}
	m_wndName.GetWindowText(m_Data.PropName);
	if (m_Data.PropName.Compare(_T("")) == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_BlankPropertyName));
		return FALSE;
	}
	m_wndDescription.GetWindowText(m_Data.Description);

	return TRUE;
}

void CDamperGBItemDlg::OnCmdRdferDatabaseBtn()
{
	UpdateData();
	int nCode = m_wndCode.GetCurSel();
	CString strClassify = _T("");
	m_wndCategory.GetWindowText(strClassify);
	m_nCategory = m_wndCategory.GetCurSel();
	int nRow = m_nInputCount;

	CDamperGBDataTableDlg dlg;
	dlg.SetParamData(m_Data.nPropType, nCode, m_nCategory, nRow, strClassify);
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
	case 0: m_Data.PropName = m_Data.BRB.strInputType;	break;
	case 1: m_Data.PropName = m_Data.MYD.strInputType;	break;
	case 2: m_Data.PropName = m_Data.FD.strInputType;	break;
	case 3: m_Data.PropName = m_Data.VFD.strInputType;	break;
	case 4: m_Data.PropName = m_Data.HDRD.strInputType;	break;
	case 5: m_Data.PropName = m_Data.VED.strInputType;	break;
	default:
		break;
	}

	if (bCancel && m_bChangeData)
	{
		if (m_bAddUser)
			InitInputCodeCmb();
		else
			Data2Dlg();
	}

}

void CDamperGBItemDlg::OnCmdPropTypeRdo()
{
	UpdateData(TRUE);
	SetRedraw(FALSE);
	m_bChangeData = FALSE;
	m_nInputMethod = 1;
	int nCurType = GetCheckedRadioButton(IDC_CMD_TYPE_BRB_RDO, IDC_CMD_TYPE_VED_RDO);
	if (nCurType == IDC_CMD_TYPE_BRB_RDO)
	{
		m_Data.nPropType = 0;
	}
	else if (nCurType == IDC_CMD_TYPE_MYD_RDO)
	{
		m_Data.nPropType = 1;
	}
	else if (nCurType == IDC_CMD_TYPE_FD_RDO)
	{
		m_Data.nPropType = 2;
	}
	else if (nCurType == IDC_CMD_TYPE_VD_RDO)
	{
		m_Data.nPropType = 3;
		m_nInputMethod = 0;
	}
	else if (nCurType == IDC_CMD_TYPE_HDRD_RDO)
	{
		m_Data.nPropType = 4;
	}
	else if (nCurType == IDC_CMD_TYPE_VED_RDO)
	{
		m_Data.nPropType = 5;
	}
	if (nCurType == IDC_CMD_TYPE_VD_RDO)
	{
		((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CMD_INPUT_TYPE_USER_RDO))->SetCheck(FALSE);
		((CButton *)GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO))->SetCheck(TRUE);
	}
	

	m_wndCode.SetCurSel(0);
	OnCmdInputMethodRdo();
	InitInputCategoryCmb();
	OnSelChangeInputTypeCmb();
	UpdateData(FALSE);
	SetRedraw(TRUE);
	RedrawWindow();
}

void CDamperGBItemDlg::OnCmdInputMethodRdo()
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
	}
	else
	{
		GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		InitInputCodeCmb();
		if (m_wndCode.GetCurSel() == 0)
		{
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
		}
		if (m_wndCode.GetCurSel() == 0 && m_Data.nPropType > 1)
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
	}
	EnableDisableItemCtrls();
	if (m_Data.nPropType == 0)
	{
		m_pBRB->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 1)
	{
		m_pMYD->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 2)
	{
		m_pFD->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 3)
	{
		m_wndCode.ResetContent();
		m_wndCategory.ResetContent();
		m_wndInputType.ResetContent();
		m_pVFD->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 4)
	{
		m_pHDRD->EnableDisableItemCtrls(m_nInputMethod);
	}
	else if (m_Data.nPropType == 5)
	{
		m_pVED->EnableDisableItemCtrls(m_nInputMethod);
	}
	UpdateData(FALSE);
	RedrawWindow();
}

void CDamperGBItemDlg::OnSelChangeInputCodeCmb()
{
	m_nCode = m_wndCode.GetCurSel();
	InitInputCategoryCmb();
	if (m_nCode == 0)
	{
		GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
		if (m_nPropType > 1)
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(FALSE);
	}
	/*else
	{
		if (m_nPropType > 1)
			GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
	}*/
		
	CString strCode = _T("");
	m_wndCode.GetWindowText(strCode);
	if (strCode == _T("User"))
	{
		GetDlgItem(IDC_CMD_INPUT_CLASSIFY_COMBO)->EnableWindow(TRUE);
	}
}

void CDamperGBItemDlg::InitInputCodeCmb()
{
	m_wndCode.ResetContent();

	CString strCode = _T("");
	if (m_Data.nPropType == 0)
		strCode = _T("DB13(J)T 8422-2021");
	else if (m_Data.nPropType != 3)
		strCode = _T("DB11-2075-2022");
	m_wndCode.AddString(strCode);
	IDamperGBDBMgr::Inst()->InitUserDefineFolder(m_Data.nPropType, m_UserD);
	if (m_Data.nPropType == 0)
	{
		if (m_UserD.m_mapCodeBRB.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.BRB.nCode);
	}
	else if (m_Data.nPropType == 1)
	{
		if (m_UserD.m_mapCodeMYD.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.MYD.nCode);
	}
	else if (m_Data.nPropType == 2)
	{
		if (m_UserD.m_mapCodeFD.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.FD.nCode);
	}
	else if (m_Data.nPropType == 3)
	{
		if (m_UserD.m_mapCodeVFD.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(0);
	}
	else if (m_Data.nPropType == 4)
	{
		if (m_UserD.m_mapCodeHDRD.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.HDRD.nCode);
	}
	else if (m_Data.nPropType == 5)
	{
		if (m_UserD.m_mapCodeVED.GetCount() > 0)
		{
			CString strUser = _T("User");
			m_wndCode.AddString(strUser);
		}
		m_wndCode.SetCurSel(m_Data.VED.nCode);
	}
	
	m_nCode = m_wndCode.GetCurSel();
}

void CDamperGBItemDlg::OnSelChangeInputCategoryCmb()
{
	UpdateData();
	m_nCategory = m_wndCategory.GetCurSel();
	CString strCode = _T("");
	m_wndCode.GetWindowText(strCode);
	if (strCode == _T("User"))
		InitUserInputTypeCmb();
	else
		InitInputTypeCmb();

	OnSelChangeInputTypeCmb();
}

void CDamperGBItemDlg::InitInputCategoryCmb()
{
	UpdateData();
	m_wndCategory.ResetContent();

	if (m_nPropType == 3)
		return;
	CStringArray astrClassify;
	IDamperGBDBMgr::Inst()->GetDBClassifyList(m_Data.nPropType, astrClassify);
	
	for (int i = 0; i < astrClassify.GetCount(); i++)
	{
		m_wndCategory.AddString(astrClassify[i]);
	}
	m_nCategory = m_wndCategory.SetCurSel(0);
	InitInputTypeCmb();

	if (!m_bModify)
		OnSelChangeInputTypeCmb();

}

void CDamperGBItemDlg::OnSelChangeInputTypeCmb()
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
	if (m_wndCode.GetCurSel() == 0)
	{
		IDamperGBDBMgr::Inst()->GetCodeDB(m_Code);
		bCode = TRUE;
	}
	else 
	{
		if (m_nPropType != 3)
			bCode = FALSE;
	}
		
	if (!bCode)
	{
		IDamperGBDBMgr::Inst()->InitUserDefineFolder(m_Data.nPropType, m_UserD);
	}

	if (m_Data.nPropType == 0)
	{
		if (bCode)
			m_Code.m_mapCodeBRB.Lookup(strTemp, m_Data.BRB);	
		else
			m_UserD.m_mapCodeBRB.Lookup(strTemp, m_Data.BRB);	
		m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(m_Data);
		m_Data.BRB.nInputMethod = m_nInputMethod;
		m_Data.BRB.nCode = m_wndCode.GetCurSel();
		m_Data.BRB.nClassify = m_wndCategory.GetCurSel();
		m_Data.BRB.nInputType = m_wndInputType.GetCurSel();

		m_pBRB->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 1)
	{
		if (bCode)
			m_Code.m_mapCodeMYD.Lookup(strTemp, m_Data.MYD);
		else
			m_UserD.m_mapCodeMYD.Lookup(strTemp, m_Data.MYD);
		m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(m_Data);
		m_Data.MYD.nInputMethod = m_nInputMethod;
		m_Data.MYD.nCode = m_wndCode.GetCurSel();
		m_Data.MYD.nClassify = m_wndCategory.GetCurSel();
		m_Data.MYD.nInputType = m_wndInputType.GetCurSel();
		m_pMYD->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 2)
	{
		if (bCode)
			m_Code.m_mapCodeFD.Lookup(strInputType, m_Data.FD);
		else
			m_UserD.m_mapCodeFD.Lookup(strInputType, m_Data.FD);
		m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(m_Data);
		m_Data.FD.nInputMethod = m_nInputMethod;
		m_Data.FD.nCode = m_wndCode.GetCurSel();
		m_Data.FD.nInputType = m_wndInputType.GetCurSel();
		m_pFD->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 3)
	{
		if (!bCode)
		{
			m_UserD.m_mapCodeVFD.Lookup(strInputType, m_Data.VFD);
			m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(m_Data);
			m_Data.VFD.nCode = m_wndCode.GetCurSel();
			m_Data.VFD.nInputType = m_wndInputType.GetCurSel();
		}
		else
		{
			if (m_bInit_VFD)
			{
				m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(m_Data);
				m_bInit_VFD = FALSE;
			}
		}
				
		m_Data.VFD.nInputMethod = m_nInputMethod;
		m_pVFD->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 4)
	{
		if (bCode)
			m_Code.m_mapCodeHDRD.Lookup(strInputType, m_Data.HDRD);
		else
			m_UserD.m_mapCodeHDRD.Lookup(strInputType, m_Data.HDRD);
		m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(m_Data);
		m_Data.HDRD.nInputMethod = m_nInputMethod;
		m_Data.HDRD.nCode = m_wndCode.GetCurSel();
		m_Data.HDRD.nInputType = m_wndInputType.GetCurSel();
		m_pHDRD->SetData2Dlg(m_Data);
	}
	else if (m_Data.nPropType == 5)
	{
		if (bCode)
			m_Code.m_mapCodeVED.Lookup(strInputType, m_Data.VED);
		else
			m_UserD.m_mapCodeVED.Lookup(strInputType, m_Data.VED);
		m_pDoc->m_pUnitCtrl->ConvertUnitDmgbOut(m_Data);
		m_Data.VED.nInputMethod = m_nInputMethod;
		m_Data.VED.nCode = m_wndCode.GetCurSel();
		m_Data.VED.nInputType = m_wndInputType.GetCurSel();
		m_pVED->SetData2Dlg(m_Data);
	}
	EnableDisableItemCtrls();
	CString strName;
	m_wndInputType.GetWindowText(strName);
	m_wndName.SetWindowText(strName);
	UpdateData(FALSE);
}

void CDamperGBItemDlg::InitUserInputTypeCmb()
{
	m_wndInputType.ResetContent();
	CString strClassify;
	m_wndCategory.GetWindowText(strClassify);
	CStringArray astrInputType;
	IDamperGBDBMgr::Inst()->GetInputTypeList(strClassify, m_nPropType, astrInputType);
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
	case 5:
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

void CDamperGBItemDlg::InitInputTypeCmb()
{
	m_wndInputType.ResetContent();

	if (m_nPropType == 3)
		return;

	CStringArray astrInputType;
	CString _tcscategory;
	int nCategory = m_wndCategory.GetCurSel();
	IDamperGBDBMgr::Inst()->GetDBInputTypeList(m_nPropType, nCategory, astrInputType);
	m_nInputCount = astrInputType.GetCount();

	for (int i=0; i< m_nInputCount; i++)
	{
		m_wndInputType.AddString(astrInputType[i]);
	}
	
	m_wndInputType.SetCurSel(0);
}

void CDamperGBItemDlg::OnCmdOK()
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CDamperGBItemDlg::OnCmdCancel()
{
	CDialogMove::OnCancel();
}

BOOL CDamperGBItemDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	T_DMGB_D Data;
	m_pDoc->m_pAttrCtrl->GetDmgb(m_OldKey, Data);   // T_NLLP_D의 key를 주고 T_NLLP_D를 받는다.
	if (m_bModify)
	{
		m_Data.nSeq = Data.nSeq;   // old data의 seq를 현재 data의 seq로 넣어준다.
		bSuccess = m_pDoc->m_pDataCtrl->ModifyDmgb(m_OldKey, m_OldKey, m_Data);
	}
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddDmgb(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

void CDamperGBItemDlg::OnCmdApply()
{
	if (!ApplyOrOK()) return;
	m_OldKey = m_Key;
}

void CDamperGBItemDlg::EnableDisableItemCtrls()
{
	if (m_nInputMethod ==0)
	{
		GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_CODE_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_CLASSIFY_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_TYPE_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_INPUT_CODE_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INPUT_TYPE_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_CODE_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_CLASSIFY_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_TYPE_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_INPUT_DATA_BUTTON)->EnableWindow(TRUE);
	}

	/*if (m_nPropType == 3)
	{
		GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_USER_TYPE_DATA_RDO)->EnableWindow(TRUE);
	}*/

	if (m_Data.nPropType == 0)
	{
		m_pBRB->UpdateData(FALSE);
		m_pBRB->ShowWindow(SW_SHOW);
		m_pMYD->ShowWindow(SW_HIDE);
		m_pFD->ShowWindow(SW_HIDE);
		m_pVFD->ShowWindow(SW_HIDE);
		m_pHDRD->ShowWindow(SW_HIDE);
		m_pVED->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 1)
	{
		m_pMYD->UpdateData(FALSE);
		m_pBRB->ShowWindow(SW_HIDE);
		m_pMYD->ShowWindow(SW_SHOW);
		m_pFD->ShowWindow(SW_HIDE);
		m_pVFD->ShowWindow(SW_HIDE);
		m_pHDRD->ShowWindow(SW_HIDE);
		m_pVED->ShowWindow(SW_HIDE);
		m_pVED->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 2)
	{
		m_pFD->UpdateData(FALSE);
		m_pBRB->ShowWindow(SW_HIDE);
		m_pMYD->ShowWindow(SW_HIDE);
		m_pFD->ShowWindow(SW_SHOW);
		m_pVFD->ShowWindow(SW_HIDE);
		m_pHDRD->ShowWindow(SW_HIDE);
		m_pVED->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 3)
	{
		m_pVFD->UpdateData(FALSE);
		m_pBRB->ShowWindow(SW_HIDE);
		m_pMYD->ShowWindow(SW_HIDE);
		m_pFD->ShowWindow(SW_HIDE);
		m_pVFD->ShowWindow(SW_SHOW);
		m_pHDRD->ShowWindow(SW_HIDE);
		m_pVED->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 4)
	{
		m_pHDRD->UpdateData(FALSE);
		m_pBRB->ShowWindow(SW_HIDE);
		m_pMYD->ShowWindow(SW_HIDE);
		m_pFD->ShowWindow(SW_HIDE);
		m_pVFD->ShowWindow(SW_HIDE);
		m_pHDRD->ShowWindow(SW_SHOW);
		m_pVED->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nPropType == 5)
	{
		m_pHDRD->UpdateData(FALSE);
		m_pBRB->ShowWindow(SW_HIDE);
		m_pMYD->ShowWindow(SW_HIDE);
		m_pFD->ShowWindow(SW_HIDE);
		m_pVFD->ShowWindow(SW_HIDE);
		m_pHDRD->ShowWindow(SW_HIDE);
		m_pVED->ShowWindow(SW_SHOW);
	}
	RedrawWindow();
}


void CDamperGBItemDlg::LayoutChildDlg(CChildDialog* pChildDlg)
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

void CDamperGBItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDamperGBItemDlg::UpdateBuffer()
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
		case(UR_DMGB_ADD):
		case(UR_DMGB_MFD):
			//OnSelChangeSeisTypeCmb();
			//CDlgUtil::CobxSetCurSelItemData(m_wndSeisDvProp, prevKey);
			break;
		case(UR_DMGB_DEL):
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

