// SeisInfillStrutAssignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisInfillStrutAssignDlg.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
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
// SeisInfillStrutAssignDlg dialog

CSeisInfillStrutAssignDlg::CSeisInfillStrutAssignDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisInfillStrutAssignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SeisInfillStrutAssignDlg)
	m_nNodeNumIndex = 0;
// 	m_bIntersectNode = TRUE;
// 	m_bIntersectElem = TRUE;
	m_nCheckOrtho = FALSE;
 	m_nAssignMethod = EN_METHOD_BY_NODE;
	m_nOption = 0;
	m_nBracintOption = EN_DIAGONAL;
	//}}AFX_DATA_INIT
	m_bPropInit = FALSE;
	m_pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	// Material은 User Type만 되도록 한다
	CArray<CString, CString> MatlType;
	MatlType.Add(_T("U"));
	m_ctrlCobxMatl.SetMatlType(MatlType);
}

CSeisInfillStrutAssignDlg::~CSeisInfillStrutAssignDlg()
{
}

void CSeisInfillStrutAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SeisInfillStrutAssignDlg)
	DDX_Control(pDX, IDC_TM_ORTHO_CHECK, m_chkOrtho);
	DDX_Control(pDX, IDC_TM_INFILL_STRUT_CMB, m_cobxStrut);
	DDX_Control(pDX, IDC_TM_NODES_STATIC, m_wndNodesTitle);
	DDX_Control(pDX, IDC_TM_MAT_STATIC1, m_wndMatTitle);
	DDX_Control(pDX, IDC_TM_NODES_EDIT, m_wndNodeListEdit);
	DDX_Control(pDX, IDC_TM_MAT_NAME_COMBO  , m_ctrlCobxMatl);
	DDX_Control(pDX, IDC_TM_MAT_NO_EDIT     , m_ctrlEditMatl);
	DDX_Check(pDX, IDC_TM_ORTHO_CHECK, m_nCheckOrtho);
	DDX_Radio(pDX, IDC_TM_ASSIGN_BY_NODE_BTN, m_nAssignMethod);
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nOption);
	DDX_Radio(pDX, IDC_TM_BRTYPE_DIA_RDO, m_nBracintOption);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSeisInfillStrutAssignDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(SeisInfillStrutAssignDlg)
	ON_BN_CLICKED(IDC_TM_MAT_BUTTON, OnTmMatButton)
	ON_BN_CLICKED(IDC_TM_INFILL_STRUT_BTN, OnTmStrutButton)
	ON_EN_SETFOCUS(IDC_TM_NODES_EDIT, OnSetfocusTmNodesEdit)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	ON_BN_CLICKED(IDC_TM_ORTHO_CHECK, OnTmOrthoCheck)
	ON_BN_CLICKED(IDC_TM_ASSIGN_BY_NODE_BTN, OnTmAssignMethodBtn)
	ON_BN_CLICKED(IDC_TM_ASSIGN_BY_ELEM_BTN, OnTmAssignMethodBtn)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SeisInfillStrutAssignDlg message handlers

void CSeisInfillStrutAssignDlg::Execute()
{
	_CreateElement();
}

BOOL CSeisInfillStrutAssignDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitCtrls();
	UpdateData(FALSE);

	OnTmAssignMethodBtn();

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

void CSeisInfillStrutAssignDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	if(::IsWindow(m_hWnd))
	{
		GotoDlgCtrl(&m_wndNodeListEdit);
	}
}

void CSeisInfillStrutAssignDlg::InitPropertyCtrls()
{
	m_ctrlCobxMatl.SetEditBoxPoint(&m_ctrlEditMatl);
	m_ctrlEditMatl.SetComboBoxPoint(&m_ctrlCobxMatl);

	if (!m_bPropInit)
	{
		m_ctrlEditMatl.SetEditBoxText(0);
		m_bPropInit = TRUE;
	}
}

void CSeisInfillStrutAssignDlg::OnTmMatButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));
}

void CSeisInfillStrutAssignDlg::OnTmStrutButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_MAIN_RIBBON_SEISEVAL_MASONRY_INFILL_STRUT,0));
}

/*********************************************************************
	 *
	 *
 *****   ELEMENT CREATION PARAMETER 설정 OPERATION
	***
	 */

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CSeisInfillStrutAssignDlg::GetMatlIDS(int nElemType,int& nMatID)
{
	nMatID = 0;
	if (!m_pDoc->m_pAttrCtrl->IsNeedMatl(nElemType)) return FALSE;
	
	T_MATL_K MatlK = 0;
	m_ctrlCobxMatl.GetSelectedMatl(MatlK);
	if(MatlK == 0) return FALSE;
	
	CString strMatl = _T("");
	m_ctrlEditMatl.GetWindowText(strMatl);
	nMatID = _ttoi(strMatl);
	if(nMatID == 0) return FALSE;

	return TRUE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CSeisInfillStrutAssignDlg::GetKNodeDatas(int nElemType, CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
																	 double &BAngle, double &RPWx, double &RPWy, double &RPWz)
{
	nBAngleOrRefP = 0;
	BAngle = 0.;
	RPWx = RPWy = RPWz = 0.0;  

	return TRUE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CSeisInfillStrutAssignDlg::GetNodeListAndNodeNum(int nElemType,
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

BOOL CSeisInfillStrutAssignDlg::GetParameter(_ElemCreate & EC)
{
	UpdateData(TRUE);
	
	// Element Type이 Compression only, Truss인 요소로 생성
	EC.m_nElemType = TRUSS_EL;
	EC.m_nSubType = 0;

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

	T_MSIS_K MsisK = m_cobxStrut.GetItemData(m_cobxStrut.GetCurSel());
	if(MsisK<1) RetVal = FALSE;

	EC.m_nSecID = 0; // 유효보는 DataCtrl::AddElem() 에서 단면 키를 할당함
	EC.m_bIntersectElem = FALSE;
	EC.m_bIntersectNode = FALSE;
		
	return RetVal;
}

BOOL CSeisInfillStrutAssignDlg::GetDistance(CMouseEdit* pDistEdit, double &dblDist)
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
void CSeisInfillStrutAssignDlg::OnSetfocusTmNodesEdit() 
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

void CSeisInfillStrutAssignDlg::SetTempElemDataByCurSetting()
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

void CSeisInfillStrutAssignDlg::_CreateElement()
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
		EData.nSubType = EC.m_nSubType; // Effective beam
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

		int nCmbIdx = m_cobxStrut.GetCurSel();
		if(nCmbIdx<0) 
		{
			GSaveHistoryNF(_LS(IDS_TM_WARNING_MSIS_UNDEFINED));
			return;
		}
		T_MSIS_K MsisK = m_cobxStrut.GetItemData(nCmbIdx);
		if(!m_pDoc->m_pDataCtrl->AddElem(0,EData,EC.m_bIntersectNode,EC.m_bIntersectElem,EC.m_nBAngleOrRefPoint,EC.m_RPWx,EC.m_RPWy,EC.m_RPWz, EC.m_bCreateInnerPoint, 0, MsisK, m_nBracintOption))
		{
			I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
			pIGM->SetTempElemErrorFlag(TRUE);
			pIGM->CursorClearHide();
			pIGM->CancelAllTempElem();
		}    
		IUsageCounter::Use(_T("SPE_MSIS"));
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Element_Creation_Arguement_));
	}
	//IUsageCounter::Use("EBAEMASS", FALSE);
}

LRESULT CSeisInfillStrutAssignDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		_CreateElement(); 
		m_wndNodeListEdit.SetWindowText(_T(""));
	}
	return 0L;
}

LRESULT CSeisInfillStrutAssignDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
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
LRESULT CSeisInfillStrutAssignDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
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

void CSeisInfillStrutAssignDlg::OnTmIntstTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}

void CSeisInfillStrutAssignDlg::OnTmOrthoCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_nCheckOrtho)
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_XYZOrthoMode();
	else
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();
}

void CSeisInfillStrutAssignDlg::OnTmAssignMethodBtn()
{
	UpdateData(TRUE);
	BOOL bByNode = m_nAssignMethod==EN_METHOD_BY_NODE;
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_STATIC10, bByNode, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_TM_OPT_GRP, !bByNode, TRUE);
}

void CSeisInfillStrutAssignDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_nAssignMethod==EN_METHOD_BY_NODE)
	{
		_CreateElement();
	}
	else
	{
		AssignProperty();
	}
}

void CSeisInfillStrutAssignDlg::AssignProperty()
{
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	T_ELEM_K ElemK=NULL;
	CArray<T_ELEM_K, T_ELEM_K> aFiltedSelKey;

	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		ElemK = aSelKey.GetAt(i);
		if(!CheckInfillStrut(ElemK)) continue;   

		aFiltedSelKey.Add(ElemK);
	}

	if (aFiltedSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_No_Selected_Member));
		return;
	}

	int nCmbIdx = m_cobxStrut.GetCurSel();
	if(nCmbIdx<0) 
	{
		GSaveHistoryNF(_LS(IDS_TM_WARNING_MSIS_UNDEFINED));
		return;
	}

	T_MSIS_K MsisK = m_cobxStrut.GetItemData(nCmbIdx);

	T_MISA_D data;
	data.Initialize();
	data.MsisK = MsisK;
	data.nBracingType = m_nBracintOption;
	BOOL bSuccess=FALSE;
	if (m_nOption == 0) // Add/Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddMisa(aFiltedSelKey, data);	
	else if (m_nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelMisa(aFiltedSelKey);

	if (m_nOption == 0)
	{
		IUsageCounter::Use(_T("SPE_MSIS"));
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
void CSeisInfillStrutAssignDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

BOOL CSeisInfillStrutAssignDlg::CheckInfillStrut(T_ELEM_K ElemK)
{
	T_ELEM_D ElemD;

	// Element Type이 "Compression only, Truss"이고 요소 양단 절점이 기둥에 연결되어 있는 경우만 적용 가능
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
	if(ElemD.eltyp != TRUSS_EL)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG__ERROR_MISA_ASSIGN_d), ElemK);
		return FALSE;
	}  

	BOOL bIsColm[2] = {FALSE};
	T_NODE_K NodeK[2] = { static_cast<T_NODE_K>(ElemD.elnod[0]), static_cast<T_NODE_K>(ElemD.elnod[1]) };
	T_ELEM_K ColmK[2]={0};
	if(!m_pDoc->m_pAttrCtrl->GetColmorWallKeyConnectToInfillStrut(NodeK, ColmK, bIsColm))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG__ERROR_MISA_ASSIGN_CONNECT_COLM_d), ElemK);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CSeisInfillStrutAssignDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CSeisInfillStrutAssignDlg::UpdateBuffer()
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
		case(UR_MSIS_ADD): 
		case(UR_MSIS_DEL): 
			UpdateMsisCmb();
			break;
		default:
			break;
		}
	} // end of while
}

BOOL CSeisInfillStrutAssignDlg::GetSectIDS(T_NODE_K NodeK, int& nSecID)
{
	return TRUE;
}

void CSeisInfillStrutAssignDlg::PostNcDestroy()
{
	// 수직부재절점만 선택모드 끄기
	I_GENModelBase::GetCurMySelfST()->SetSelectNodeOnVerElemOnly(FALSE);

	CMenuBarChildDlg::PostNcDestroy();
}

void CSeisInfillStrutAssignDlg::InitCtrls()
{
	InitPropertyCtrls();
	InitMsisCmb();
	m_cobxStrut.SetCurSel(0);
	InitNodeCtrl();
}

void CSeisInfillStrutAssignDlg::InitMsisCmb()
{
	CArray<T_MSIS_K,T_MSIS_K> aMsisK;
	m_pDoc->m_pAttrCtrl2->GetMsisKeyList(aMsisK);
	T_MSIS_D MsisD;

	m_cobxStrut.ResetContent();
	for(int i=0; i<aMsisK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetMsis(aMsisK[i], MsisD);
		m_cobxStrut.AddString(MsisD.strName);
		m_cobxStrut.SetItemData(i,aMsisK[i]);
	}
}

void CSeisInfillStrutAssignDlg::UpdateMsisCmb()
{
	int nCurIdx = m_cobxStrut.GetCurSel();
	if(nCurIdx<0) nCurIdx = 0;
	InitMsisCmb();
	m_cobxStrut.SetCurSel(nCurIdx);
}

void CSeisInfillStrutAssignDlg::InitNodeCtrl()
{
	m_wndNodeListEdit.SetAttNothing();
	m_wndNodeListEdit.SetModeToUse(MOUSEEDIT_USE_NODE_LIST_INPUT);
	m_wndNodeListEdit.SetEscNotifyWindow(this);
	m_wndNodeListEdit.SetEnterNotifyWindow(this);
	m_wndNodeListEdit.SetLButtonDownNotifyWindow(this);
//   m_wndNodeUserEdit.SetElemCreate();

	//////////////////////////////////////////////////////////////////
	// PreParsing Mode 설정...
//   m_wndNodeUserEdit.SetPreParsingMode(m_nPreParseType);

	GotoDlgCtrl(&m_wndNodeListEdit);

	// user edit ///////////////
//   m_cobxNodeEdit.SetCurSel(0);
	////////////////////////////

}

