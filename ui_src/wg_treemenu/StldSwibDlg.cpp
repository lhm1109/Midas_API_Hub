// StldSwibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldSwibDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldSwibDlg dialog
#define CDialog CMenuBarChildDlg

CStldSwibDlg::CStldSwibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldSwibDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldSwibDlg)
	m_nOption = 0;
	m_b2ndNode = FALSE;
	m_strXFactor = _T("");
	m_strYFactor = _T("");
	m_strZFactor = _T("");
	//}}AFX_DATA_INIT
}

CStldSwibDlg::~CStldSwibDlg()
{
	;
}


void CStldSwibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldSwibDlg)
	DDX_Control(pDX, IDC_TM_2ND_NODE_EDIT, m_wnd2ndNode);
	DDX_Control(pDX, IDC_TM_1ST_NODE_EDIT, m_wnd1stNode);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Radio(pDX, IDC_TM_ADDREP_RADIO, m_nOption);
	DDX_Check(pDX, IDC_TM_2ND_NODE_CHECK, m_b2ndNode);
	DDX_Text(pDX, IDC_TM_X_EDIT, m_strXFactor);
	DDX_Text(pDX, IDC_TM_Y_EDIT, m_strYFactor);
	DDX_Text(pDX, IDC_TM_Z_EDIT, m_strZFactor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStldSwibDlg, CDialog)
	//{{AFX_MSG_MAP(CStldSwibDlg)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnTmBtnLc)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_ADDREP_RADIO, OnTmOptionRadio)
	ON_BN_CLICKED(IDC_TM_2ND_NODE_CHECK, OnTm2ndNodeCheck)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_DEL_RADIO, OnTmOptionRadio)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldSwibDlg message handlers
BOOL CStldSwibDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wnd1stNode.SetAttNodeList();
	m_wnd1stNode.SetLButtonDownNotifyWindow(this);
	m_wnd1stNode.SetEnterNotifyWindow(this);
	m_wnd1stNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wnd1stNode.SetMaxNodeKeyNum(1);

	m_wnd2ndNode.SetAttNodeList();
	m_wnd2ndNode.SetLButtonDownNotifyWindow(this);
	m_wnd2ndNode.SetEnterNotifyWindow(this);
	m_wnd2ndNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wnd2ndNode.SetMaxNodeKeyNum(1);

	CtrlMan();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldSwibDlg::OnTmBtnLc() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldSwibDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);		
}

void CStldSwibDlg::OnTmOptionRadio() 
{
	UpdateData(TRUE);
	CtrlMan();
}

void CStldSwibDlg::OnTm2ndNodeCheck() 
{
	UpdateData(TRUE);
	CtrlMan(); 
}


void CStldSwibDlg::CtrlMan()
{
	CArray<UINT, UINT> Ctrls;
	CArray<UINT, UINT> Ctrls2nd;
	
	Ctrls.Add(IDC_TM_STATIC3       );
	Ctrls.Add(IDC_TM_1ST_NODE_EDIT );
	Ctrls.Add(IDC_TM_2ND_NODE_CHECK);
	Ctrls.Add(IDC_TM_2ND_NODE_EDIT ); 
	Ctrls.Add(IDC_TM_STATIC11      );
	Ctrls.Add(IDC_TM_STATIC13      );
	Ctrls.Add(IDC_TM_STATIC16      );
	Ctrls.Add(IDC_TM_X_EDIT        );
	Ctrls.Add(IDC_TM_Y_EDIT        );
	Ctrls.Add(IDC_TM_Z_EDIT        );

	Ctrls2nd.Add(IDC_TM_2ND_NODE_EDIT ); 

	if(m_nOption == 0 ) // Add/Replace
	{
		CtrlEnableDisable(Ctrls,TRUE);
		CtrlEnableDisable(Ctrls2nd,m_b2ndNode);
	}
	else                // Delete
	{
		CtrlEnableDisable(Ctrls,FALSE);
	}

}


void CStldSwibDlg::GetCurGroupKey(void * pLGKey)
{
	CString StrGrupName;
	int nCurSel = this->m_wndLdgrCobx.GetCurSel();
	m_wndLdgrCobx.GetLBText(nCurSel,StrGrupName);

	T_LDGR_K LdgrK;
	LdgrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKey(StrGrupName);

	*((T_LDGR_K*)pLGKey) = LdgrK;
}

LRESULT CStldSwibDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	
	if((HWND)lParam == this->m_wnd1stNode.GetSafeHwnd())
	{
		UpdateData();
		if(m_nOption == 0)
		{
			if(m_b2ndNode)
				m_wnd2ndNode.SetFocus();
		}
	}
	
	return 0L;
}

LRESULT CStldSwibDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	
	if((HWND)lParam == this->m_wnd1stNode.GetSafeHwnd())
	{
		UpdateData();
		if(m_nOption == 0)
		{
			if(m_b2ndNode)
				m_wnd2ndNode.SetFocus();
		}
	}
	
	return 0L;
}

void CStldSwibDlg::OnTmClose() 
{
	CloseDlg();
}

void CStldSwibDlg::OnTmExecute() 
{
	UpdateData();

	/*
	T_SWIB_D SwibD;
	CArray<UINT,UINT> SelElemKeys;
	CDBDoc::GetDocPoint()->m_pViewCtrl->GetAllSelectedElem(SelElemKeys);
	*/

	


	//I_GENModelBase::GetCurMySelfST()->GetSelectedElemKeyList(SelElemKeys);
	//
	//CDBDoc::GetDocPoint()->m_pAttrCtrl->Getelem
	if(m_nOption == 0) // Add/Replace
	{
		this->m_nOption;
		this->m_b2ndNode;
		this->m_wnd1stNode;
		this->m_wnd2ndNode;
		this->m_strXFactor;
		this->m_strYFactor;
		this->m_strZFactor;
	}
	else               // Delete...
	{
		
	}
}
