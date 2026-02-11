// CMThmsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThmsDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThmsDlg dialog
#define CDialog CCMChildBarBase

CCMThmsDlg::CCMThmsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMThmsDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThmsDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_aCtrlDir[0].Add(IDC_CMD_SCALE_FACTOR);
	m_aCtrlDir[0].Add(IDC_CMD_ARRIVAL_TIME);
	m_aCtrlDir[1].Add(IDC_CMD_SCALE_FACTOR2);
	m_aCtrlDir[1].Add(IDC_CMD_ARRIVAL_TIME2);
	m_aCtrlDir[2].Add(IDC_CMD_SCALE_FACTOR3);
	m_aCtrlDir[2].Add(IDC_CMD_ARRIVAL_TIME3);
}

CCMThmsDlg::~CCMThmsDlg()
{
}

void CCMThmsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThmsDlg)
	DDX_Control(pDX, IDC_CMD_ANGLE_OF_HGA_EDIT, m_wndHGA);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	DDX_Radio(pDX, IDC_CMD_OPTION_ADD, m_nOption);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR3, m_wndScaleFactor[2]);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR2, m_wndScaleFactor[1]);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor[0]);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME3, m_wndFuncName[2]);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME2, m_wndFuncName[1]);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME, m_wndFuncName[0]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_UNIT3, m_wndArTimeUnit[2]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_UNIT2, m_wndArTimeUnit[1]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_UNIT, m_wndArTimeUnit[0]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME3, m_wndArTime[2]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME2, m_wndArTime[1]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME, m_wndArTime[0]);
}


BEGIN_MESSAGE_MAP(CCMThmsDlg, CDialog)
	//{{AFX_MSG_MAP(CCMThmsDlg)
// 	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
// 	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_FUNC_BTN, OnCmdFuncBtn)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME, OnSelchangeCmdFuncName)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_ANGLE_OF_HGA_SPIN, OnDeltaposCmdAngleOfHgaSpin)
	ON_BN_CLICKED(IDC_CMD_FUNC_BTN2, OnCmdFuncBtn)
	ON_BN_CLICKED(IDC_CMD_FUNC_BTN3, OnCmdFuncBtn)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME2, OnSelchangeCmdFuncName)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME3, OnSelchangeCmdFuncName)
	ON_BN_CLICKED(IDC_CMD_OPTION_ADD, OnCmdOptionAdd)
	ON_BN_CLICKED(IDC_CMD_OPTION_DELETE, OnCmdOptionDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _DEBUG
void CCMThmsDlg::AssertValid() const
{
	CDialog::AssertValid();
}

void CCMThmsDlg::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CCMThmsDlg message handlers

BOOL CCMThmsDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitDialog();
	 
	m_wndLoadCase.SetIgnoreInitialLoad(TRUE);
	m_wndLoadCase.SetLoadType(D_SELECTLOAD_HISTORY, TRUE);
	T_THFC_K nKey;
	for (int i = 0; i < 3; i++)
	{
		m_wndFuncName[i].ChangeType(TRUE, TRUE, FALSE, FALSE, FALSE); // normalacc, accel¸¸ Çã¿ë
		m_wndArTime[i].SetUnitType(CUnitCtrl::m_THMS_UNIT.dArrivalTime);
		m_wndArTimeUnit[i].SetUnitType(CUnitCtrl::m_THMS_UNIT.dArrivalTime);
		m_wndFuncName[i].GetSelectedThfc(nKey);
		m_wndScaleFactor[i].SetWindowText(_T("1"));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDir[i], nKey != 0);
	}

	m_wndHGA.SetRange(-360, 360);
	m_wndHGA.SetValue(0);

	m_aCtrl.Add(IDC_CMD_FUNC_NAME);
	m_aCtrl.Add(IDC_CMD_FUNC_BTN);
	m_aCtrl.Add(IDC_CMD_SCALE_FACTOR);
	m_aCtrl.Add(IDC_CMD_ARRIVAL_TIME);
	m_aCtrl.Add(IDC_CMD_FUNC_NAME2);
	m_aCtrl.Add(IDC_CMD_FUNC_BTN2);
	m_aCtrl.Add(IDC_CMD_SCALE_FACTOR2);
	m_aCtrl.Add(IDC_CMD_ARRIVAL_TIME2);
	m_aCtrl.Add(IDC_CMD_FUNC_NAME3);
	m_aCtrl.Add(IDC_CMD_FUNC_BTN3);
	m_aCtrl.Add(IDC_CMD_SCALE_FACTOR3);
	m_aCtrl.Add(IDC_CMD_ARRIVAL_TIME3);
	m_aCtrl.Add(IDC_CMD_ANGLE_OF_HGA_EDIT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThmsDlg::Execute()
{
	CArray<T_NODE_K, T_NODE_K> aNodeK;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aNodeK);
	if(aNodeK.GetSize()<1)
	{
		GSaveHistoryNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
		return;
	}

	UpdateData();

	T_THMS_D data;
	data.Initialize();

	BOOL bSuc=FALSE;
	if(m_nOption==0) // Add or replace
	{
		if(!Dlg2Data(data)) return;
		bSuc = m_pDoc->m_pDataCtrl->AddThms(aNodeK, data);
	}
	else if(m_nOption==1) // Delete
	{
		GetLoadCaseKey(data.ThisKey);
		bSuc = m_pDoc->m_pDataCtrl->DelThms(aNodeK, data.ThisKey);
	}
	if(bSuc) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

// void CCMThmsDlg::OnCmdClose() 
// {
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
// }

void CCMThmsDlg::OnCmdLoadcaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HISTORY_LOADCASE, 0));
}

void CCMThmsDlg::OnCmdFuncBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_TIME_TFUNCTION, 0));
}

void CCMThmsDlg::OnSelchangeCmdFuncName() 
{
	const MSG* pMsg = GetCurrentMessage();
	int nDir=0;
	UINT nID = LOWORD(pMsg->wParam);
	if (nID == IDC_CMD_FUNC_NAME) nDir = 0;
	else if (nID == IDC_CMD_FUNC_NAME2) nDir = 1;
	else if (nID == IDC_CMD_FUNC_NAME3) nDir = 2;
	
	T_THFC_K nKey;
	m_wndFuncName[nDir].GetSelectedThfc(nKey);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDir[nDir], nKey != 0);
}

BOOL CCMThmsDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_THMS_K Key;
	T_THMS_D data;
	Key.keymap = key;
	data.Initialize();

	m_pDoc->m_pAttrCtrl->GetThms(Key, data);
	Data2Dlg(data);

	return TRUE;
}

void CCMThmsDlg::OnDeltaposCmdAngleOfHgaSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_ANGLE_OF_HGA_EDIT,_T("-360"),_T("360"),1,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMThmsDlg::OnCmdOptionAdd()
{
	FuncDirCtrl(TRUE);
}

void CCMThmsDlg::OnCmdOptionDelete()
{
	FuncDirCtrl(FALSE);
}

void CCMThmsDlg::FuncDirCtrl(BOOL bAdd)
{
	if (bAdd)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrl, TRUE);
		int nIndex = ((CComboBox*)GetDlgItem(IDC_CMD_FUNC_NAME))->GetCurSel();
		if (nIndex == 0)
		{
			CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SCALE_FACTOR, FALSE);
			CDlgUtil::CtrlEnableDisable(this, IDC_CMD_ARRIVAL_TIME, FALSE);
		}
		nIndex = ((CComboBox*)GetDlgItem(IDC_CMD_FUNC_NAME2))->GetCurSel();
		if (nIndex == 0)
		{
			CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SCALE_FACTOR2, FALSE);
			CDlgUtil::CtrlEnableDisable(this, IDC_CMD_ARRIVAL_TIME2, FALSE);
		}
		nIndex = ((CComboBox*)GetDlgItem(IDC_CMD_FUNC_NAME3))->GetCurSel();
		if (nIndex == 0)
		{
			CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SCALE_FACTOR3, FALSE);
			CDlgUtil::CtrlEnableDisable(this, IDC_CMD_ARRIVAL_TIME3, FALSE);
		}
	}
	else
		CDlgUtil::CtrlEnableDisable(this, m_aCtrl, FALSE);
}

BOOL CCMThmsDlg::Dlg2Data(T_THMS_D &data)
{
	data.Initialize();

	if(!GetLoadCaseKey(data.ThisKey)) return FALSE;

	CString csDir[] = {_T("X"), _T("Y"), _T("Z")};
	for (int i = 0; i < 3; i++)
	{
		m_wndFuncName[i].GetSelectedThfc(data.nThisFuncKey[i]);
		if (data.nThisFuncKey[i] == 0) continue;
		
		if (!m_wndScaleFactor[i].GetEditValue(data.dFactor[i]))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_for_), csDir[i]);
			return FALSE;
		}

		data.dArrivalTime[i] = m_wndArTime[i].GetEditValue();
	}

	data.dAngleHGA = m_wndHGA.GetEditValue();

	return TRUE;
}

BOOL CCMThmsDlg::GetLoadCaseKey(T_THIS_K& key)
{
	UINT nLoadCaseType;
	if(!m_wndLoadCase.GetSelectedLoad(nLoadCaseType, key))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
		return FALSE;
	}
	return TRUE;
}
void CCMThmsDlg::Data2Dlg(T_THMS_D &data)
{
	m_wndLoadCase.ChangeSelect(D_LOADCASE_HISTORY, data.ThisKey);

	CString csVal;
	for (int i = 0; i < 3; i++)
	{
		m_wndFuncName[i].ChangeSelect(data.nThisFuncKey[i]);
		csVal.Format(_T("%g"), data.dFactor[i]);
		m_wndScaleFactor[i].SetWindowText(csVal);
		csVal.Format(_T("%g"), data.dArrivalTime[i]);
		m_wndArTime[i].SetWindowText(csVal);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDir[i], data.nThisFuncKey[i] != 0);
	}

	m_wndHGA.SetValue(data.dAngleHGA);
}
/*
void CCMThmsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
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

void CCMThmsDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_THMS_K Key;
	T_THMS_D Data;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THMS_ADD):
			{
				pViewBuff->GetThms(nKey, Key, Data);
			}
			break;
		case(UR_THMS_DEL):
			{
				pViewBuff->GetThms(nKey, Key, Data);
			}
			break;
		default:
			break;
		}
	} // end of while
}
*/


