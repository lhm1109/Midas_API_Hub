// CMThnlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThnlDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThnlDlg
//#define CDialog CCMDlgBarBase

#define CDialog CCMChildBarBase

//IMPLEMENT_DYNCREATE(CCMThnlDlg, CDialog)

CCMThnlDlg::CCMThnlDlg()
	: CDialog(CCMThnlDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThnlDlg)
	//}}AFX_DATA_INIT
	m_aCtrlDelete.Add(IDC_CMD_FUNC_NAME);
	m_aCtrlDelete.Add(IDC_CMD_FUNC_BTN);
	m_aCtrlDelete.Add(IDC_CMD_LOADTYPE);
	m_aCtrlDelete.Add(IDC_CMD_DIR_X);
	m_aCtrlDelete.Add(IDC_CMD_DIR_Y);
	m_aCtrlDelete.Add(IDC_CMD_DIR_Z);
	m_aCtrlDelete.Add(IDC_CMD_ARRIVAL_TIME_EDIT);
	m_aCtrlDelete.Add(IDC_CMD_ARRIVAL_TIME_UNIT);
	m_aCtrlDelete.Add(IDC_CMD_SCALE_FACTOR);

	m_aCtrlOptionRadio.Add(IDC_CMD_OPTION_ADD);
	m_aCtrlOptionRadio.Add(IDC_CMD_OPTION_REPLACE);
	m_aCtrlOptionRadio.Add(IDC_CMD_OPTION_DELETE);

	m_aCtrlDirRadio.Add(IDC_CMD_DIR_X);
	m_aCtrlDirRadio.Add(IDC_CMD_DIR_Y);
	m_aCtrlDirRadio.Add(IDC_CMD_DIR_Z);
}

CCMThnlDlg::~CCMThnlDlg()
{
}

BOOL CCMThnlDlg::Dlg2Data(T_THNL_D &data)
{
	data.Initialize();

	UINT nLoadCaseType;
	if (!m_wndLoadCase.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
		return FALSE;
	}

	BOOL bGet = m_wndFuncName.GetSelectedThfc(data.nThisFuncKey);
	if (!bGet || data.nThisFuncKey == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Time_history_function_is_));
		return FALSE;
	}
	T_THFC_D thfc;
	m_pDoc->m_pAttrCtrl->GetThfc(data.nThisFuncKey, thfc);
	data.nLoadType = thfc.nThisDataType-2;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDirRadio, data.nDirection);
	data.nDirection++;

	data.dArrivalTime = m_wndArTime.GetEditValue();

	if (!m_wndScaleFactor.GetEditValue(data.dFactor))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMThnlDlg::Data2Dlg(T_THNL_D &data)
{
	m_wndLoadCase.ChangeSelect(D_LOADCASE_HISTORY, data.LoadCaseKey);

	m_wndFuncName.ChangeSelect(data.nThisFuncKey);

	if (data.nLoadType+2 == 3) m_wndLoadType.SetWindowText(_LS(IDS_WG_CMD__ADDD__Force));
	else m_wndLoadType.SetWindowText(_LS(IDS_WG_CMD__ADDD__Moment));

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDirRadio, data.nDirection-1);

	m_wndArTime.SetEditUnit(data.dArrivalTime);

	CString str;
	str.Format(_T("%g"), data.dFactor);
	m_wndScaleFactor.SetWindowText(str);

	UpdateData(FALSE);
	return TRUE;
}

void CCMThnlDlg::UpdateLoadType()
{
	T_THFC_K thfcKey = 0;
	T_THFC_D thfcData;
	BOOL bGet = m_wndFuncName.GetSelectedThfc(thfcKey);
	if (!bGet || thfcKey == 0)
		m_wndLoadType.SetWindowText(_T(""));
	else
	{
		T_THFC_D thfc;
		if(m_pDoc->m_pAttrCtrl->GetThfc(thfcKey, thfcData))
		{
			if (thfcData.nThisDataType == 3)
				m_wndLoadType.SetWindowText(_LS(IDS_WG_CMD__ADDD__Force));
			else 
				m_wndLoadType.SetWindowText(_LS(IDS_WG_CMD__ADDD__Moment));
		}
		else
		{
			m_wndLoadType.SetWindowText(_T(""));
		}
	}
}

void CCMThnlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThnlDlg)
	DDX_Control(pDX, IDC_CMD_LOADTYPE, m_wndLoadType);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_UNIT, m_wndArTimeUnit);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_EDIT, m_wndArTime);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME, m_wndFuncName);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThnlDlg, CDialog)
	//{{AFX_MSG_MAP(CCMThnlDlg)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_OPTION_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_CMD_FUNC_BTN, OnCmdFuncBtn)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME, OnSelchangeCmdFuncName)
	ON_BN_CLICKED(IDC_CMD_OPTION_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_CMD_OPTION_REPLACE, OnChangeOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThnlDlg diagnostics

#ifdef _DEBUG
void CCMThnlDlg::AssertValid() const
{
	CDialog::AssertValid();
}

void CCMThnlDlg::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMThnlDlg message handlers

/*
void CCMThnlDlg::OnInitialUpdate() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_wndLoadCase.SetLoadType(D_SELECTLOAD_HISTORY, TRUE);
	m_wndArTime.SetUnitType(CUnitCtrl::m_THNL_UNIT.dArrivalTime);
	m_wndArTimeUnit.SetUnitType(CUnitCtrl::m_THNL_UNIT.dArrivalTime);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOptionRadio, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDirRadio, 0);
	m_wndArTime.SetWindowText(_T("0"));
	m_wndScaleFactor.SetWindowText(_T("1"));

	m_wndFuncName.ChangeType(FALSE, FALSE, TRUE, TRUE);

	UpdateLoadType();
}
*/

BOOL CCMThnlDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitDialog();
	
	m_wndLoadCase.SetIgnoreInitialLoad(TRUE);
	m_wndLoadCase.SetLoadType(D_SELECTLOAD_HISTORY, TRUE);
	m_wndArTime.SetUnitType(CUnitCtrl::m_THNL_UNIT.dArrivalTime);
	m_wndArTimeUnit.SetUnitType(CUnitCtrl::m_THNL_UNIT.dArrivalTime);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOptionRadio, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDirRadio, 0);
	m_wndArTime.SetWindowText(_T("0"));
	m_wndScaleFactor.SetWindowText(_T("1"));

	m_wndFuncName.ChangeType(FALSE, FALSE, TRUE, TRUE, FALSE,FALSE);

	UpdateLoadType();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMThnlDlg::OnCmdLoadcaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HISTORY_LOADCASE, 0));	
}

void CCMThnlDlg::OnChangeOption() 
{
	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOptionRadio, nOption);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDelete, nOption != 2);
}

void CCMThnlDlg::OnCmdFuncBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_TIME_TFUNCTION, 0));	
}

void CCMThnlDlg::OnCmdClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMThnlDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	T_THNL_D data;

	CArray<T_NODE_K, T_NODE_K> aKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aKey);
	if (aKey.GetSize() <= 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
		return;
	}

	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOptionRadio, nOption);

	if (nOption == 2) // delete
	{
		UINT nLoadCaseType;
		if (!m_wndLoadCase.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
			return;
		}
	}
	else // add/replace
	{
		if (!Dlg2Data(data)) return;
	}

	BOOL bSuccess;
	if (nOption == 0 || nOption == 1)
		bSuccess = m_pDoc->m_pDataCtrl->AddThnl(aKey, data, nOption == 1);
	else bSuccess = m_pDoc->m_pDataCtrl->DelThnl(aKey, data.LoadCaseKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMThnlDlg::OnSelchangeCmdFuncName() 
{
	// TODO: Add your control notification handler code here
	UpdateLoadType();
}

BOOL CCMThnlDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_THNL_K thnlkey;
	T_THNL_D data;

	thnlkey.keymap = key;
	m_pDoc->m_pAttrCtrl->GetThnl(thnlkey, data);
	Data2Dlg(data);

	return TRUE;
}

void CCMThnlDlg::Execute()
{
	OnCmdApply();
}

void CCMThnlDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateLoadType();
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
		break;
	}
}

void CCMThnlDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	//T_THSL_K Key;
	//T_THSL_D Data;

	BOOL bThslCmd = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THFC_DEL):
		case(UR_THFC_ADD):
		case(UR_THFC_MFD):
		case(UR_THFC_MFS):
				bThslCmd = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	if(bThslCmd)
		UpdateLoadType();
}