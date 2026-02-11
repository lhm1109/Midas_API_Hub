// BndrCglpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrCglpDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_main\wg_mainres2.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrCglpDlg dialog


CBndrCglpDlg::CBndrCglpDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrCglpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrCglpDlg)
	m_nOption = 0;
	m_nSelectOpt = 0;
	//}}AFX_DATA_INIT

	m_aCtrlGenLkProp.Add(IDC_TM_NL_LINK_PROP_COMBO);
	m_aCtrlGenLkProp.Add(IDC_TM_NL_LINK_PROP_BUTTON);
}


void CBndrCglpDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrCglpDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO,				m_wndGroupCombo);
	DDX_Radio(pDX, IDC_TM_OPT_ADD,							m_nOption);
	DDX_Radio(pDX, IDC_TWO_NODE_RDO,						m_nSelectOpt);	
	DDX_Control(pDX, IDC_TM_2NODES_EDIT,				m_wnd2Node);
	DDX_Control(pDX, IDC_TM_GLINK_NUM_EDT,			m_wndGlinkEdt);	
	DDX_Control(pDX, IDC_TM_NL_LINK_PROP_COMBO, m_wndNllpCobx);
	DDX_Control(pDX, IDC_TM_NL_LINK_PROP_EDIT,	m_wndNllp);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBndrCglpDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrCglpDlg)		
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON,		OnDefineGroupButton)
	ON_CBN_SELCHANGE(IDC_TM_NL_LINK_PROP_COMBO, OnSelchangeNlLinkPropCombo)
	ON_BN_CLICKED(IDC_TM_NL_LINK_PROP_BUTTON,		OnNlLinkPropButton) 	
	ON_BN_CLICKED(IDC_TM_OPT_ADD,								OnOption)	
	ON_BN_CLICKED(IDC_TWO_NODE_RDO,							OnSelOption)	
	ON_BN_CLICKED(IDC_TM_CLOSE,									OnClose)	
	ON_BN_CLICKED(IDC_TM_OPT_DELETE,						OnOption)	
	ON_BN_CLICKED(IDC_GLINK_NUM_RDO,						OnSelOption)	
	ON_BN_CLICKED(IDC_TM_EXECUTE,								OnApply)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrCglpDlg message handlers

BOOL CBndrCglpDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	m_wnd2Node.SetAttNodeList();
	m_wnd2Node.SetLButtonDownNotifyWindow(this);
	m_wnd2Node.SetEnterNotifyWindow(this);
	m_wnd2Node.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wnd2Node.SetMaxNodeKeyNum(2);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlGenLkProp, m_nOption==0);
	m_wnd2Node.EnableWindow(!m_nSelectOpt);
	m_wndGlinkEdt.EnableWindow(m_nSelectOpt);	
	DisplayCurNllp();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrCglpDlg::OnDefineGroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD);
}

void CBndrCglpDlg::OnSelchangeNlLinkPropCombo() 
{
	// TODO: Add your control notification handler code here
	DisplayCurNllp();
}

void CBndrCglpDlg::DisplayCurNllp()
{
	T_NLLP_K NllpK;
	T_NLLP_D NllpD;
	m_wndNllpCobx.GetSelectedNllp(NllpK);

	CString StrTemp;
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNllp(NllpK,NllpD))
	{
		if (NllpD.nApplicationType == 0)
		{
			switch(NllpD.nElemType)
			{
			case 0:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop1);
				break;
			case 1:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop2);
				break;
			case 2:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop2);
				break;
			case 3:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop2);
				break;
			case 4:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop2);
				break;
			}
		}
		else if(NllpD.nApplicationType == 1)
		{
			switch(NllpD.nType)
			{
			case 0:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop1);
				break;
			case 1:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop2);
				break;
			case 2:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop3);
				break;
			case 3:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop4);
				break;
			case 4:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop5);
				break;
			case 5:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop6);
				break;
			case 6:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop7);
				break;
			}
		}
		else if (NllpD.nApplicationType == 2)
		{
			switch(NllpD.nSeisType)
			{
			case 0:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDVI);
				break;
			case 1:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDVE);
				break;
			case 2:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDST);
				break;
			case 3:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDHY);
				break;
			case 4:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDIS);
				break;
			case 5:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_ASGB);
				break;
			case 6:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_DMGB);
				break;
			}
		}
		else ASSERT(0);
	}
	m_wndNllp.SetWindowText(StrTemp);
}

void CBndrCglpDlg::OnNlLinkPropButton() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_BNDR_NLLINK_PROPERTY	,0));
}

void CBndrCglpDlg::OnOption() 
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlGenLkProp, m_nOption==0);
}

void CBndrCglpDlg::OnSelOption() 
{
	UpdateData(TRUE);
	m_wnd2Node.EnableWindow(!m_nSelectOpt);
	m_wndGlinkEdt.EnableWindow(m_nSelectOpt);	
}


LRESULT CBndrCglpDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_wnd2Node.GetSafeHwnd() == (HWND)lParam)
	{
		CArray<int, int> aKeyNode;
		CString csNodeList;
		m_wnd2Node.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
			return 0L;
		OnApply();
		m_wnd2Node.ClearContents();
	}
	return 0L;  
}

LRESULT CBndrCglpDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wnd2Node.GetSafeHwnd() == (HWND)lParam)
	{
		OnApply();
	}
	return 0L;
}


BOOL CBndrCglpDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_CGLP_D data;
	T_NLNK_D DataNlnk;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc) ;
	pDoc->m_pAttrCtrl->GetCglp(key, data);
	pDoc->m_pAttrCtrl->GetNlnk(data.GLinkK, DataNlnk);

	// Boundary Group
	m_wndGroupCombo.ChangeSelect(data.GroupKey);

	// General Link Property
	m_wndNllpCobx.ChangeSelect(data.GLinkPropK);
	DisplayCurNllp();

	CString strNode;
	strNode.Format(_T("%ld, %ld"), DataNlnk.Node1, DataNlnk.Node2);
	m_wnd2Node.SetWindowText(strNode);

	UpdateData(FALSE);
	return TRUE;
}

void CBndrCglpDlg::OnClose() 
{
	CloseDlg();		
}

void CBndrCglpDlg::OnApply() 
{
	UpdateData(TRUE);  	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_CGLP_D rData;
	rData.Initialize();

	T_BNGR_K BngrK;
	T_NLLP_K NllpK;

	m_wndGroupCombo.GetSelectedBngr(BngrK);	
	m_wndNllpCobx.GetSelectedNllp(NllpK);

	rData.GroupKey = BngrK;
	rData.GLinkPropK = NllpK;

	int nType = -1;
	T_NODE_K NodeK1, NodeK2;	
	T_NLNK_K GLinkK;
	NodeK1 = NodeK2 = GLinkK = 0;
	
	if(m_nSelectOpt==0)		// Select from two nodes....
	{
		nType = 1;
		CArray<int, int> aNodes;
		CString csNodeList;
		m_wnd2Node.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aNodes) || aNodes.GetSize() != 2)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Select_2_nodes_));
			return;
		}
		NodeK1 = aNodes[0]; 
		NodeK2 = aNodes[1];
	}
	else		// get general link number from direct input..
	{		
		nType = 2;
		//int nKey;
		//CFormulaEdit::GetEditValue(&m_wndGlinkEdt, nKey);

		CString tempS;
		int nErrCode;
		m_wndGlinkEdt.GetWindowText(tempS);
		if(!IsValidLongNumber(tempS, nErrCode))
		{
			InvalidLongNumber(nErrCode);
			return;
		}
		GLinkK = (T_NLNK_K)_ttoi(tempS);
	}

	if(m_nOption==0)
		pDoc->m_pDataCtrl->AddCglp(rData, nType, NodeK1, NodeK2, GLinkK);
	else
		pDoc->m_pDataCtrl->DelCglp(BngrK, nType, NodeK1, NodeK2, GLinkK);
}


// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과), 4(정수아님)
BOOL CBndrCglpDlg::IsValidLongNumber(CString value, int &nErrCode)
{
	value.TrimLeft();
	value.TrimRight();
	if (value == _T("")) return TRUE; // 공백은 0으로 생각하여 TRUE 리턴
	// 수식이면 변환, 수식이 아니면 TRUE 반환(수식은 첫자가 '=' 임)
	//if (!ConvertFormulaToValue(value)) { nErrCode = 1; return FALSE; }

	int ecode;
	// 숫자인지 문자열인지 검사
	if (!IsNumeric(value, ecode)) 
	{
		if (ecode == 1) nErrCode = 3;
		else nErrCode = 2;
		return FALSE;
	}

	TCHAR *num = new TCHAR[value.GetLength()+1];
	TCHAR *endp;
	long n;

	_stprintf(num, value);
	n = _tcstol(num, &endp, 10);
	BOOL bAllScaned = endp == &num[value.GetLength()];
	delete[] num;

	// 정수인지 실수인지 조사
	if (!bAllScaned) { nErrCode = 4; return FALSE; }
	// 정수 범위 검사
	if (n == LONG_MAX || n == LONG_MIN) { nErrCode = 3; return FALSE; }

	return TRUE;
}

BOOL CBndrCglpDlg::InvalidLongNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD_ERROR_FORMULA_INVALID));
	case 2: 
	case 4: return ValidateError(_LS(IDS_CMD_ERROR_REQUIRE_INTEGER));
	case 3: return ValidateError(_LS(IDS_CMD_ERROR_OUT_OF_RANGE));
	}
	return FALSE;
}

BOOL CBndrCglpDlg::IsNumeric(CString str, int& ecode)
{
	if (str.GetLength() < 1) return FALSE;

	TCHAR *num = new TCHAR[str.GetLength()+1];
	TCHAR *endp;
	double n;

	_stprintf(num, str);
	n = _tcstod(num, &endp);
	BOOL bAllScaned = endp == &num[str.GetLength()];
	delete[] num;

	if (n == HUGE_VAL || n == -HUGE_VAL) 
		ecode = 1;
	else if (!bAllScaned) 
		ecode = 2;
	else ecode = 0;

	if (ecode) return FALSE;
	else return TRUE;
}

BOOL CBndrCglpDlg::ValidateError(LPCTSTR lpszError)
{
	AfxMessageBox(lpszError);
	return FALSE;
}

