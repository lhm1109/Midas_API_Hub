// StldPrstDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPrstDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldPrstDlg dialog


CStldPrstDlg::CStldPrstDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldPrstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldPrstDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	//m_aDeleteCtrl.Add(IDC_TM_LT_PRST);
	//m_aDeleteCtrl.Add(IDC_TM_LT_POST);
	m_aDeleteCtrl.Add(IDC_TM_TENSION);
	m_aDeleteCtrl.Add(IDC_TM_DI);
	m_aDeleteCtrl.Add(IDC_TM_DM);
	m_aDeleteCtrl.Add(IDC_TM_DJ);
	m_aDeleteCtrl.Add(IDC_TM_ST_SEL);
	m_aDeleteCtrl.Add(IDC_TM_ST_LINE);
	m_aDeleteCtrl.Add(IDC_TM_ST_LINE_NODE);

	//m_aLoadTypeCtrl.Add(IDC_TM_LT_PRST);
	//m_aLoadTypeCtrl.Add(IDC_TM_LT_POST);

	m_aSelTypeCtrl.Add(IDC_TM_ST_SEL);
	m_aSelTypeCtrl.Add(IDC_TM_ST_LINE);
}


void CStldPrstDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPrstDlg)
	DDX_Control(pDX, IDC_TM_ST_LINE_NODE, m_wndLineEdit);
	DDX_Control(pDX, IDC_TM_DM_UNIT, m_wndDMUnit);
	DDX_Control(pDX, IDC_TM_DJ_UNIT, m_wndDJUnit);
	DDX_Control(pDX, IDC_TM_DI_UNIT, m_wndDIUnit);
	DDX_Control(pDX, IDC_TM_TENSION_UNIT, m_wndTensionUnit);
	DDX_Control(pDX, IDC_TM_DM, m_wndDM);
	DDX_Control(pDX, IDC_TM_DJ, m_wndDJ);
	DDX_Control(pDX, IDC_TM_DI, m_wndDI);
	DDX_Control(pDX, IDC_TM_TENSION, m_wndTension);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_TM_PRST_DIRECT_CMB, m_cmbDirect);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC3, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldPrstDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldPrstDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_ST_SEL, OnChangeSelType)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_ST_LINE, OnChangeSelType)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldPrstDlg message handlers

BOOL CStldPrstDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Set Default Values
	int nSelType = 0;
	//CDlgUtil::CtrlRadioSetCheck(this, m_aLoadTypeCtrl, 0); // Default : Prestress
	CDlgUtil::CtrlRadioSetCheck(this, m_aSelTypeCtrl, nSelType); // Default : selection
	m_wndLineEdit.EnableWindow(nSelType == 1);

	// Enable/Disable/Show/Hide/Align Controls
	m_wndTension.SetUnitType(CUnitCtrl::m_PRST_UNIT.dTension);
	m_wndTensionUnit.SetUnitType(CUnitCtrl::m_PRST_UNIT.dTension);
	m_wndDI.SetUnitType(CUnitCtrl::m_PRST_UNIT.dDistanceI);
	m_wndDIUnit.SetUnitType(CUnitCtrl::m_PRST_UNIT.dDistanceI);
	m_wndDJ.SetUnitType(CUnitCtrl::m_PRST_UNIT.dDistanceJ);
	m_wndDJUnit.SetUnitType(CUnitCtrl::m_PRST_UNIT.dDistanceJ);
	m_wndDM.SetUnitType(CUnitCtrl::m_PRST_UNIT.dDistanceM);
	m_wndDMUnit.SetUnitType(CUnitCtrl::m_PRST_UNIT.dDistanceM);

	m_wndLineEdit.SetAttNodeList();
	m_wndLineEdit.SetLButtonDownNotifyWindow(this);
	m_wndLineEdit.SetEnterNotifyWindow(this);
	m_wndLineEdit.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndLineEdit.SetMaxNodeKeyNum(2);

	m_nOption = 0;  // Default : Add

	m_cmbDirect.ResetContent();
	m_cmbDirect.AddString(_LS(IDS_WG_TREEMENU_Local_y));
	m_cmbDirect.AddString(_LS(IDS_WG_TREEMENU_Local_z));
	m_cmbDirect.SetCurSel(1); // Default Local Z

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);


	UpdateData(FALSE);

	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_prst 1.svg"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldPrstDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	T_NODE_K KeyNodeI, KeyNodeJ;
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_ELEM_D ElemData;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);

	int nSelType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aSelTypeCtrl, nSelType);
	if (nSelType == 0 || m_nOption == 2)  // by selection or delete
	{
		for (int i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
		}

		if (aSelKey.GetSize() == 0) 
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected__s_element_), _LS(IDS_WG_TREEMENU_Beam));
			return;
		}
	}
	else  // by line
	{
		CArray<int,int> NodeKeyList;
		CString csNodeList;
		m_wndLineEdit.GetWindowText(csNodeList);
		BOOL bSuccess = GetNodeList(csNodeList,NodeKeyList);
		if (!bSuccess) 
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
			return;
		}
		if (NodeKeyList.GetSize() != 2) 
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
			return;
		}
		KeyNodeI = NodeKeyList[0];
		KeyNodeJ = NodeKeyList[1];
		if (KeyNodeI == KeyNodeJ) 
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
			return;
		}
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_PRST_D data;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;

	//int nCheck;
	//CDlgUtil::CtrlRadioGetCheck(this, m_aLoadTypeCtrl, nCheck);
	//data.LoadType = nCheck+1;
	data.nDirection = m_cmbDirect.GetCurSel();
	data.dTension   = m_wndTension.GetEditValue();
	data.dTensionJ  = data.dTension;
	data.dDistanceI = m_wndDI.GetEditValue();
	data.dDistanceJ = m_wndDJ.GetEditValue();
	data.dDistanceM = m_wndDM.GetEditValue();

	BOOL bSuccess=FALSE;
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	
	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
	{
		
		data.GroupKey = LdgrK;

		if (nSelType == 0)  // by selection    
			bSuccess = m_pDoc->m_pDataCtrl->AddPrst(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
		else 
			bSuccess = m_pDoc->m_pDataCtrl->AddPrst(KeyNodeI, KeyNodeJ, data, (m_nOption==0)?FALSE:TRUE);
	}
	else if (m_nOption == 2)  // delete
	{
		// always selection
		bSuccess = m_pDoc->m_pDataCtrl->DelPrst(aSelKey, nLoadCaseKey,LdgrK);	
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

LRESULT CStldPrstDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_wndLineEdit.IsKeyPressed()) return 0L;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(m_wndLineEdit.GetSafeHwnd() == (HWND)lParam)
	{
		if(pIGM->GetModelingModeST() == IG_STATE_GET_NODE_LIST)
		{
			CArray<int,int> NodeKeyList;
			CString csNodeList;
			m_wndLineEdit.GetWindowText(csNodeList);
			BOOL bSuccess = GetNodeList(csNodeList,NodeKeyList);
			if (!bSuccess) return 0L;
			if (NodeKeyList.GetSize() != 2) return 0L;
			OnTmExecute();
		}
	}
	return 0L;
}

void CStldPrstDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldPrstDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldPrstDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD: m_nOption = 0; break;
	case IDC_TM_OPT_REPLACE: m_nOption = 1; break;
	case IDC_TM_OPT_DELETE: m_nOption = 2; break;
	}

	CtrlEnableDisable(m_aDeleteCtrl, m_nOption != 2);
	if (m_nOption != 2) //Delete
	{
		int nSelType;
		CDlgUtil::CtrlRadioGetCheck(this, m_aSelTypeCtrl, nSelType);
		m_wndLineEdit.EnableWindow(nSelType == 1);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else
	{
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}
}

void CStldPrstDlg::OnChangeSelType() 
{
	// TODO: Add your control notification handler code here
	int nSelType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aSelTypeCtrl, nSelType);
	m_wndLineEdit.EnableWindow(nSelType == 1);
}

void CStldPrstDlg::OnTmDefineLdgrBtn() 
{
	// TODO: Add your control notification handler code here
		CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

