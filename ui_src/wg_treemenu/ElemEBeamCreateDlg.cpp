// ElemEBeamCreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemEBeamCreateDlg.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_dbRes2.h"
#include "..\wg_db\IUsageCounter.h"

#include "..\wg_base\I_GENStateDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemEBeamCreateDlg dialog

CElemEBeamCreateDlg::CElemEBeamCreateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemEBeamCreateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemEBeamCreateDlg)
	m_nNodeNumIndex = 0;
	m_bIntersectNode = TRUE;
	m_bIntersectElem = TRUE;
	m_nCheckOrtho = FALSE;
	m_nPreParseType = 0;
	//}}AFX_DATA_INIT
	m_bPropInit = FALSE;
	m_pBitmap = 0;
	m_pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}

CElemEBeamCreateDlg::~CElemEBeamCreateDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CElemEBeamCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemEBeamCreateDlg)
	DDX_Control(pDX, IDC_TM_ORTHO_CHECK, m_chkOrtho);
	DDX_Control(pDX, IDC_TM_NODES_USEREDIT, m_wndNodeUserEdit);
	DDX_Control(pDX, IDC_TM_NODES_COBX, m_cobxNodeEdit);
	DDX_Control(pDX, IDC_TM_EBEAM_PREB_CMB, m_cobxPreb);
	DDX_Control(pDX, IDC_TM_NODES_STATIC, m_wndNodesTitle);
	DDX_Control(pDX, IDC_TM_MAT_STATIC1, m_wndMatTitle);
	DDX_Control(pDX, IDC_TM_NODES_EDIT, m_wndNodeListEdit);
	DDX_Control(pDX, IDC_TM_MAT_NAME_COMBO  , m_ctrlCobxMatl);
	DDX_Control(pDX, IDC_TM_MAT_NO_EDIT     , m_ctrlEditMatl);
	DDX_Check(pDX, IDC_TM_INTSECT_NODE_CHECK, m_bIntersectNode);
	DDX_Check(pDX, IDC_TM_INTSECT_ELEM_CHECK, m_bIntersectElem);
	DDX_Check(pDX, IDC_TM_ORTHO_CHECK, m_nCheckOrtho);
	DDX_CBIndex(pDX, IDC_TM_NODES_COBX, m_nPreParseType);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemEBeamCreateDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemEBeamCreateDlg)
	ON_BN_CLICKED(IDC_TM_MAT_BUTTON, OnTmMatButton)
	ON_BN_CLICKED(IDC_TM_EBEAM_PREB_BUTTON, OnTmPrebButton)
	ON_EN_SETFOCUS(IDC_TM_NODES_EDIT, OnSetfocusTmNodesEdit)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	ON_BN_CLICKED(IDC_TM_ORTHO_CHECK, OnTmOrthoCheck)
	ON_CBN_SELCHANGE(IDC_TM_NODES_COBX, OnSelchangeTmNodesCobx)
	ON_BN_CLICKED(IDC_TM_NODES_ENTER_BTN, OnTmNodesEnterBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemEBeamCreateDlg message handlers

void CElemEBeamCreateDlg::Execute()
{
	_CreateElement();
}

BOOL CElemEBeamCreateDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitCtrls();

	// Orthogonal Mode Setting
	if(m_nCheckOrtho)
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_XYZOrthoMode();
	else
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();
	
	// 수직부재절점만 선택모드 켜기
	I_GENModelBase::GetCurMySelfST()->SetSelectNodeOnVerElemOnly(TRUE);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemEBeamCreateDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	if(::IsWindow(m_hWnd))
	{
		GotoDlgCtrl(&m_wndNodeListEdit);
	}
}

void CElemEBeamCreateDlg::InitPropertyCtrls()
{
	m_ctrlCobxMatl.SetEditBoxPoint(&m_ctrlEditMatl);
	m_ctrlEditMatl.SetComboBoxPoint(&m_ctrlCobxMatl);

	if (!m_bPropInit)
	{
		m_ctrlEditMatl.SetEditBoxText(1);
		m_bPropInit = TRUE;
	}
}

void CElemEBeamCreateDlg::OnTmMatButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));
}

void CElemEBeamCreateDlg::OnTmPrebButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_MAIN_DEFINE_EBEAM,0));
}

/*********************************************************************
	 *
	 *
 *****   ELEMENT CREATION PARAMETER 설정 OPERATION
	***
	 */

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemEBeamCreateDlg::GetMatlIDS(int nElemType,int& nMatID)
{
	CString strM;
	m_ctrlEditMatl.GetWindowText(strM);
	
	if (m_pDoc->m_pAttrCtrl->IsNeedMatl(nElemType))
		GetINumber(strM,nMatID);
	else nMatID = 0;

	return TRUE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemEBeamCreateDlg::GetKNodeDatas(int nElemType, CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
																	 double &BAngle, double &RPWx, double &RPWy, double &RPWz)
{
	nBAngleOrRefP = 0;
	BAngle = 0.;
	RPWx = RPWy = RPWz = 0.0;  

	return TRUE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemEBeamCreateDlg::GetNodeListAndNodeNum(int nElemType,
																					 CArray<unsigned int,unsigned int>& _NodeList,
																					 int& nMaxNodeNum)
{
	CString strNodes;
	m_wndNodeListEdit.GetWindowText(strNodes);
	GetNodeList(strNodes,_NodeList);

	//Frame Type : 2개로 고정
	nMaxNodeNum = 2;
		
	return TRUE;
}

BOOL CElemEBeamCreateDlg::GetParameter(_ElemCreate & EC)
{
	UpdateData(TRUE);
		
	EC.m_nElemType = BEAM_EL;

	BOOL RetVal = TRUE;

	///////////////////////////////////////////////////////////////////////////
	// DB에서 Element Type과 부합되는 Data를 요구하므로 
	// Element Type과 연관된 Data 이외의 Data는 무효한 Data를 설정해야 한다. 
	// 현재 해당 질의 함수에서 이러한 처리를 수행하고 있다. 
	if(!GetNodeListAndNodeNum(EC.m_nElemType,EC.m_NodeKeyList,EC.m_nMaxNodeNum))
			 RetVal = FALSE;
	
	if(!GetKNodeDatas(EC.m_nElemType,EC.m_NodeKeyList,EC.m_nBAngleOrRefPoint,
								EC.m_fBAngle,EC.m_RPWx,EC.m_RPWy,EC.m_RPWz))
			 RetVal = FALSE;
	if(!GetMatlIDS(EC.m_nElemType,EC.m_nMatID)) 
			 RetVal = FALSE;

	T_PREB_K PrebK = m_cobxPreb.GetItemData(m_cobxPreb.GetCurSel());
	if(PrebK<1) RetVal = FALSE;

	EC.m_nSecID = 0; // 유효보는 DataCtrl::AddElem() 에서 단면 키를 할당함
	EC.m_bIntersectElem = m_bIntersectElem;
	EC.m_bIntersectNode = m_bIntersectNode;
		
	return RetVal;
}

BOOL CElemEBeamCreateDlg::GetDistance(CMouseEdit* pDistEdit, double &dblDist)
{
	CString strDist;
	pDistEdit->GetWindowText(strDist);
	return GetFloatNumber(strDist, dblDist);
}

	/*
	***
 *****
	 *
	 *
**********************************************************************/  
void CElemEBeamCreateDlg::OnSetfocusTmNodesEdit() 
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(pIGM)
	{
		SetTempElemDataByCurSetting();
		
		pIGM->SetTempElemFirstDrawFlag();
		
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
		if(NList.GetSize() != 0)
		{
			m_wndNodeListEdit.SetWindowText(_T(""));
			pIGM->ResetTempElem();
			I_GENModelBase::SetModelingModeST(IG_STATE_ELEM_ADD);
		}
	}
}

void CElemEBeamCreateDlg::SetTempElemDataByCurSetting()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(!pIGM) return;
	_ElemCreate EC;

	if(GetParameter(EC))
	{
		pIGM->SetTempElemData(EC.m_nElemType,EC.m_nMaxNodeNum);
		pIGM->SetTempElemNodeList(EC.m_NodeKeyList,EC.m_nElemType,EC.m_nMaxNodeNum,pIGM);
	}
}

void CElemEBeamCreateDlg::_CreateElement()
{
	_ElemCreate EC;
	if(GetParameter(EC)) 
	{
		if(EC.m_NodeKeyList.GetSize()<1)
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Create_EBeam_Empty_Node));
			return;
		}

		T_ELEM_D EData;  
		EData.Initialize();
		
		EData.eltyp = EC.m_nElemType;
		EData.angle = EC.m_fBAngle;
		EData.elmat = EC.m_nMatID;
		EData.elpro = EC.m_nSecID; // m_nSecID==0

		EData.dNonlLength = EC.m_dNonlLength;
		EData.dTensionForce = EC.m_dTensionForce;
		EData.dLimit = EC.m_dLimit; // MNET:2774 20070507 mylee
		EData.bUseLimit = EC.m_bUseLimit;
		EData.dCableLengthRatio = EC.m_dLnLRatio;
		EData.nCableOpt = EC.m_nTensCableKind;
		EData.nSubType = 1; // Effective beam
		EData.nWallId = EC.m_nWallID;
		EData.nNodeConnectivity = EC.m_nNodeConnectivity;
		EData.nWallType = EC.m_nWallType;

		if(EC.m_nMaxNodeNum == EC.m_NodeKeyList.GetSize())
		{
			for(int i = 0 ; i < EC.m_nMaxNodeNum ; i++)
					EData.elnod[i] = EC.m_NodeKeyList[i];
		}
		else
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Node_Number____));
		}

		int nCmbIdx = m_cobxPreb.GetCurSel();
		if(nCmbIdx<0) 
		{
			GSaveHistoryNF(_LS(IDS_TM_WARNING_PREB_UNDEFINED));
			return;
		}
		T_PREB_K PrebK = m_cobxPreb.GetItemData(nCmbIdx);
		if(!m_pDoc->m_pDataCtrl->AddElem(0,EData,EC.m_bIntersectNode,EC.m_bIntersectElem,EC.m_nBAngleOrRefPoint,EC.m_RPWx,EC.m_RPWy,EC.m_RPWz, EC.m_bCreateInnerPoint, PrebK))
		{
			I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
			pIGM->SetTempElemErrorFlag(TRUE);
			pIGM->CursorClearHide();
			pIGM->CancelAllTempElem();
		}    
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Element_Creation_Arguement_));
	}
	IUsageCounter::Use(_T("EBAEMASS"), FALSE);
}

LRESULT CElemEBeamCreateDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		_CreateElement(); 
		m_wndNodeListEdit.SetWindowText(_T(""));
	}
	return 0L;
}

LRESULT CElemEBeamCreateDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
{
	if(m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(!pIGM) return 0L;
		
		m_wndNodeListEdit.SetWindowText(_T(""));
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
	}
	return 0L;
}

/*********************************************************************
	 *
	 *
 *****  Model View에서의 Mouse Event Notify를 받는다. 
	***
	 */
LRESULT CElemEBeamCreateDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam && 
		 pIGM->GetModelingModeST() == IG_STATE_ELEM_ADD)
	{
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);

		CString str = _T("");
		CString Tstr;

		int nCount = NList.GetSize();
		if(nCount == 1)
			m_wndNodeListEdit.SetWindowText(_T(""));
		
		if(nCount)
		{
			if(nCount == 1) 
			{
				str.Format(_LS(IDS_WG_TREEMENU__d),NList[0]);
			}
			else
		 {
			 str.Format(_LS(IDS_WG_TREEMENU__d),NList[0]);
			 for(int i = 1 ; i < nCount ; i++)
			 {
				 Tstr.Format(_LS(IDS_WG_TREEMENU____d),NList[i]);
				 str = str + Tstr;    
			 }
		 }
		 m_wndNodeListEdit.SetWindowText(str);
		 
		 if(pIGM->IsTempNodeComplete())
		 {
			 _CreateElement();
			 
			 pIGM->ResetTempElem();
			 pIGM->SetModelingModeST(IG_STATE_INITALL);
			 pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
			 m_wndNodeListEdit.ImAssocWindowST();
		 }
		}
	}
	return 0L;
}

void CElemEBeamCreateDlg::OnTmIntstTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}

void CElemEBeamCreateDlg::OnTmOrthoCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_nCheckOrtho)
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_XYZOrthoMode();
	else
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();
}

void CElemEBeamCreateDlg::OnSelchangeTmNodesCobx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);                  // 0 ABS 1 REL 2 DIST_ANG
	m_wndNodeUserEdit.SetPreParsingMode(m_nPreParseType);
}

void CElemEBeamCreateDlg::OnTmNodesEnterBtn() 
{
	// TODO: Add your control notification handler code here
	m_wndNodeUserEdit.EnterPressOP();
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CElemEBeamCreateDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
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
		//ASSERT(FALSE);
		break;
	}
}

void CElemEBeamCreateDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_PREB_ADD): 
		case(UR_PREB_DEL): 
			UpdatePrebCmb();
			break;
		default:
			break;
		}
	} // end of while
}

BOOL CElemEBeamCreateDlg::GetSectIDS(T_NODE_K NodeK, int& nSecID)
{
	return TRUE;
}

void CElemEBeamCreateDlg::PostNcDestroy()
{
	// 수직부재절점만 선택모드 끄기
	I_GENModelBase::GetCurMySelfST()->SetSelectNodeOnVerElemOnly(FALSE);

	CMenuBarChildDlg::PostNcDestroy();
}

void CElemEBeamCreateDlg::InitCtrls()
{
	InitPropertyCtrls();
	InitPrebCmb();
	m_cobxPreb.SetCurSel(0);
	InitNodeCtrl();
}

void CElemEBeamCreateDlg::InitPrebCmb()
{
	CArray<T_PREB_K,T_PREB_K> aPrebK;
	m_pDoc->m_pAttrCtrl2->GetPrebKeyList(aPrebK);
	T_PREB_D PrebD;

	m_cobxPreb.ResetContent();
	for(int i=0; i<aPrebK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetPreb(aPrebK[i], PrebD);
		m_cobxPreb.AddString(PrebD.strName);
		m_cobxPreb.SetItemData(i,aPrebK[i]);
	}
}

void CElemEBeamCreateDlg::UpdatePrebCmb()
{
	int nCurIdx = m_cobxPreb.GetCurSel();
	if(nCurIdx<0) nCurIdx = 0;
	InitPrebCmb();
	m_cobxPreb.SetCurSel(nCurIdx);
}

void CElemEBeamCreateDlg::InitNodeCtrl()
{
	m_wndNodeListEdit.SetAttNothing();
	m_wndNodeListEdit.SetModeToUse(MOUSEEDIT_USE_NODE_LIST_INPUT);
	m_wndNodeListEdit.SetEscNotifyWindow(this);
	m_wndNodeListEdit.SetEnterNotifyWindow(this);
	m_wndNodeListEdit.SetLButtonDownNotifyWindow(this);
	m_wndNodeUserEdit.SetElemCreate();

	//////////////////////////////////////////////////////////////////
	// PreParsing Mode 설정...
	m_wndNodeUserEdit.SetPreParsingMode(m_nPreParseType);

	GotoDlgCtrl(&m_wndNodeListEdit);

	// user edit ///////////////
	m_cobxNodeEdit.SetCurSel(0);
	////////////////////////////

}

