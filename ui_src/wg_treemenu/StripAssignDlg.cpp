// StripAssignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StripAssignDlg.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_dbRes2.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_base\I_GENStateDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStripAssignDlg dialog

CStripAssignDlg::CStripAssignDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStripAssignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStripAssignDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_nByNodeElem = 1;
	m_nAddDel = 0;

	m_bIntersectNode = TRUE;
	m_bIntersectElem = TRUE;
	m_nCheckOrtho = FALSE;

	m_aCtrlByNode.RemoveAll();
	m_aCtrlByNode.Add(IDC_TM_NODES_STATIC);
	m_aCtrlByNode.Add(IDC_TM_NODES_EDIT);
	m_aCtrlByNode.Add(IDC_TM_INTSECT_NODE_CHECK);
	m_aCtrlByNode.Add(IDC_TM_INTSECT_ELEM_CHECK);
	m_aCtrlByNode.Add(IDC_TM_ORTHO_CHECK);
	m_aCtrlByNode.Add(IDC_WG_TREEMENU_STATIC10);
	m_aCtrlByNode.Add(IDC_TM_INTST_TOL);

	m_aCtrlByElem.RemoveAll();
	m_aCtrlByElem.Add(IDC_WG_TREEMENU_STATIC0);
	m_aCtrlByElem.Add(IDC_TM_STRIP_ASSIGN_ADDREP_RDO);
	m_aCtrlByElem.Add(IDC_TM_STRIP_ASSIGN_DELETE_RDO);
}

CStripAssignDlg::~CStripAssignDlg()
{

}

void CStripAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStripAssignDlg)
	DDX_Radio(pDX, IDC_TM_STRIP_ASSIGN_BYNODE_RDO, m_nByNodeElem);
	DDX_Radio(pDX, IDC_TM_STRIP_ASSIGN_ADDREP_RDO, m_nAddDel);
	DDX_Control(pDX, IDC_TM_STRIP_ASSIGN_NAME_CMB, m_cmbPrcs);
	DDX_Control(pDX, IDC_TM_NODES_EDIT  , m_wndNodeListEdit);
	DDX_Check(pDX, IDC_TM_INTSECT_NODE_CHECK, m_bIntersectNode);
	DDX_Check(pDX, IDC_TM_INTSECT_ELEM_CHECK, m_bIntersectElem);
	DDX_Check(pDX, IDC_TM_ORTHO_CHECK, m_nCheckOrtho);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CStripAssignDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CStripAssignDlg)
	ON_BN_CLICKED(IDC_TM_STRIP_ASSIGN_NAME_BTN, OnTmPrcsButton)  
	ON_BN_CLICKED(IDC_TM_STRIP_ASSIGN_BYNODE_RDO, OnClickByNodeRdo)  
	ON_BN_CLICKED(IDC_TM_STRIP_ASSIGN_BYELEM_RDO, OnClickByElemRdo)  
	ON_EN_SETFOCUS(IDC_TM_NODES_EDIT, OnSetfocusTmNodesEdit)
	ON_BN_CLICKED(IDC_TM_ORTHO_CHECK, OnTmOrthoCheck)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStripAssignDlg message handlers

// 유효보로 이루어진 Tapered group을 걸러내서 Assign
void CStripAssignDlg::Execute()
{
	UpdateData(TRUE);

	if(m_nByNodeElem==0)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_STRIP_ASSIGN_NO_SELECTED_NODES));
		return;
	}

	CArray<T_ELEM_K,T_ELEM_K> aElemK_All, aElemK_EBeam;
	CArray<T_MEMB_K,T_MEMB_K> aMembK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK_All);
	m_pDoc->m_pAttrCtrl2->FilterEBeamKListFromElem(aElemK_All,aElemK_EBeam);
	m_pDoc->m_pAttrCtrl->GetMembKListFromElem(aElemK_EBeam,aMembK);

	CArray<T_MSTR_K,T_MSTR_K> aMstrK;  aMstrK.RemoveAll();
	CArray<T_MSTR_D,T_MSTR_D&> aMstrD; aMstrD.RemoveAll();

	T_MSTR_D MstrD;
	MstrD.Initialize();
	MstrD.PrcsK = m_cmbPrcs.GetItemData(m_cmbPrcs.GetCurSel());
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	if(aMembK.IsEmpty())
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_STRIP_ASSIGN_NO_SELECTED_EBEAM));
		return;
	}

	if(m_nAddDel == 0) // Add/replace
	{
		for(int i=0; i<aMembK.GetSize(); i++)
		{
			MstrD.MembK = aMembK[i];
			aMstrD.Add(MstrD);
		}
		m_pDoc->m_pDataCtrl->AddMstr(aMstrD);
	}
	else // Delete
	{
		for(int i=0; i<aMembK.GetSize(); i++)
		{
			T_MSTR_K MstrK = m_pDoc->m_pAttrCtrl2->GetMstrKByMemb(aMembK[i]);
			if(MstrK>0)
			{
				aMstrK.Add(MstrK);
			}
		}
		m_pDoc->m_pDataCtrl->DelMstr(aMstrK);
	}

	IUsageCounter::Use(_T("STRIPASS"), FALSE);
}

BOOL CStripAssignDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitPrcsCmb();
	m_cmbPrcs.SetCurSel(0);
	InitNodeCtrl();
	OnClickByElemRdo();

	// Orthogonal Mode Setting
	if(m_nCheckOrtho)
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_XYZOrthoMode();
	else
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CStripAssignDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CStripAssignDlg::UpdateBuffer()
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
		case(UR_PRCS_ADD): 
		case(UR_PRCS_DEL): 
			UpdatePrcsCmb();
			break;
		default:
			break;
		}
	} // end of while
}

void CStripAssignDlg::PostNcDestroy()
{
	// 수직부재절점만 선택모드 끄기
	I_GENModelBase::GetCurMySelfST()->SetSelectNodeOnVerElemOnly(FALSE);

	CMenuBarChildDlg::PostNcDestroy();
}

void CStripAssignDlg::InitPrcsCmb()
{
	CArray<T_PRCS_K,T_PRCS_K> aPrcsK;
	m_pDoc->m_pAttrCtrl2->GetPrcsKeyList(aPrcsK);
	T_PRCS_D PrcsD;

	m_cmbPrcs.ResetContent();
	for(int i=0; i<aPrcsK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetPrcs(aPrcsK[i], PrcsD);
		m_cmbPrcs.AddString(PrcsD.strName);
		m_cmbPrcs.SetItemData(i,aPrcsK[i]);
	}
}

void CStripAssignDlg::UpdatePrcsCmb()
{
	int nCurIdx = m_cmbPrcs.GetCurSel();
	if(nCurIdx<0) nCurIdx = 0;
	InitPrcsCmb();
	m_cmbPrcs.SetCurSel(nCurIdx);
}

void CStripAssignDlg::OnTmPrcsButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_MAIN_DEFINE_STRIP,0));
}

void CStripAssignDlg::OnSetfocusTmNodesEdit() 
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

void CStripAssignDlg::SetTempElemDataByCurSetting()
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

void CStripAssignDlg::_CreateElemStrip()
{
	T_PRCS_K PrcsK = m_cmbPrcs.GetItemData(m_cmbPrcs.GetCurSel());
	T_MSTR_D MstrD;
	MstrD.PrcsK = PrcsK;

	// 두 점을 얻는다.
	CString strNodes;
	m_wndNodeListEdit.GetWindowText(strNodes);
	CArray<T_NODE_K,T_NODE_K> aNodeK;
	GetNodeList(strNodes,aNodeK);
	ASSERT(aNodeK.GetSize() == 2);

	// 이 절점 사이에 유효보가 있는가?
	CArray<T_MEMB_K,T_MEMB_K> aMembK;
	m_pDoc->m_pAttrCtrl2->GetMembKofEBeamByNode(aNodeK[0],aNodeK[1],aMembK);

	if(!aMembK.IsEmpty())
	{
		for(int i=0; i<aMembK.GetSize(); i++)
		{
			MstrD.MembK = aMembK[i];    
			T_MSTR_K MstrK_Exist = m_pDoc->m_pAttrCtrl2->GetMstrKByMemb(aMembK[i]); // 여기에 스트립이 존재하는가?
			if(MstrK_Exist)
			{
				if(!m_pDoc->m_pDataCtrl->ModifyMstr(MstrK_Exist, MstrK_Exist, MstrD)) { ASSERT(0); return; }
			}
			else
			{
				if(!m_pDoc->m_pDataCtrl->AddMstr(MstrD)) { ASSERT(0); return; }
			}
		}
	}
	else
	{
		// 유효보가 없으면 만들자
		CArray<T_ELEM_K,T_ELEM_K> aElemKNew;

		// Element Dynamic Label을 그리지 않기 위해 따로 Transaction 처리
		MstrD.MembK = m_pDoc->m_pDataCtrl->MakeDummyEffectiveBeam(aNodeK[0], aNodeK[1], m_bIntersectNode, m_bIntersectElem, aElemKNew);

		if(!m_pDoc->m_pUndoCtrl->StartEditDB(_LS(IDS_TM_Add_MSTR), CMDTYPE_REMOVE_ANALYSIS, FALSE)) return;
		for(int i=0; i<aElemKNew.GetSize(); i++)
		{
			T_ELEM_K ElemK = aElemKNew[i];
			if(ElemK==m_pDoc->m_pAttrCtrl->GetFirstElemKInMemb(ElemK))
			{
				m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK,MstrD.MembK);
				m_pDoc->m_pEditData->AddMstr(m_pDoc->m_pAttrCtrl2->GetStartNumMstr(), MstrD);
			}
		}
		m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, LT_MSTR_CMD);
	}
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CStripAssignDlg::GetKNodeDatas(int nElemType, CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
																				double &BAngle, double &RPWx, double &RPWy, double &RPWz)
{
	nBAngleOrRefP = 0;
	BAngle = 0.;
	RPWx = RPWy = RPWz = 0.0;  

	return TRUE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CStripAssignDlg::GetNodeListAndNodeNum(int nElemType,
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

BOOL CStripAssignDlg::GetParameter(_ElemCreate & EC)
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
//   if(!GetMatlIDS(EC.m_nElemType,EC.m_nMatID)) 
//     RetVal = FALSE;

//   T_PRCS_K PrcsK = m_cmbPrcs.GetItemData(m_cmbPrcs.GetCurSel());
//   if(PrcsK<1) RetVal = FALSE;

	EC.m_nSecID = 0; // 유효보는 DataCtrl::AddElem() 에서 단면 키를 할당함
	EC.m_bIntersectElem = m_bIntersectElem;
	EC.m_bIntersectNode = m_bIntersectNode;

	return RetVal;
}

/*********************************************************************
	 *
	 *
 *****  Model View에서의 Mouse Event Notify를 받는다. 
	***
	 */
LRESULT CStripAssignDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
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
			 _CreateElemStrip();
			 
			 pIGM->ResetTempElem();
			 pIGM->SetModelingModeST(IG_STATE_INITALL);
			 pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
			 m_wndNodeListEdit.ImAssocWindowST();
		 }
		}
	}
	return 0L;
}

LRESULT CStripAssignDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		_CreateElemStrip(); 
		m_wndNodeListEdit.SetWindowText(_T(""));
	}
	return 0L;
}

LRESULT CStripAssignDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
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

void CStripAssignDlg::EnableDisableCtrls()
{  
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlByNode, m_nByNodeElem==0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlByElem, m_nByNodeElem==1);
}

void CStripAssignDlg::OnClickByNodeRdo()
{
	UpdateData(TRUE);
	EnableDisableCtrls();

	m_wndNodeListEdit.SetFocus();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->SetModelingModeST(IG_STATE_INITALL);
	pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
}

void CStripAssignDlg::OnClickByElemRdo()
{
	UpdateData(TRUE);
	EnableDisableCtrls();

	m_wndNodeListEdit.SetWindowText(_T(""));
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->SetModelingModeST(IG_STATE_INITALL);
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_SELECT_SINGLE,0));
}

void CStripAssignDlg::InitNodeCtrl()
{
	I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();

	m_wndNodeListEdit.SetAttNothing();
	m_wndNodeListEdit.SetModeToUse(MOUSEEDIT_USE_NODE_LIST_INPUT);
	m_wndNodeListEdit.SetEscNotifyWindow(this);
	m_wndNodeListEdit.SetEnterNotifyWindow(this);
	m_wndNodeListEdit.SetLButtonDownNotifyWindow(this);
}

void CStripAssignDlg::OnTmOrthoCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_nCheckOrtho)
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_XYZOrthoMode();
	else
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();
}

void CStripAssignDlg::OnTmIntstTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}