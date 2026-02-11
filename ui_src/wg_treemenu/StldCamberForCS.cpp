// StldCamberForCS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldCamberForCS.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\AttrCtrl2.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\wg_base\ServiceProvider.h"

#include "..\wg_tb\Tb_ServiceDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldCamberForCS dialog

CStldCamberForCS::CStldCamberForCS(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldCamberForCS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldCamberForCS)
	m_nOption = 0;
	m_nCamberType = 0;
	m_nCamberResult = 0;
	//}}AFX_DATA_INIT
}

void CStldCamberForCS::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldCamberForCS)
	DDX_Radio  (pDX, IDC_TM_OPT_ADD_RDO,               m_nOption);
	DDX_Radio  (pDX, IDC_TM_DEF_CAMBER_RDO,            m_nCamberType);
	DDX_Radio  (pDX, IDC_TM_DEF_CAMBER_FCM_RDO,        m_nCamberResult);
	DDX_Control(pDX, IDC_TM_USER_CAMBER_NODE_EDT,      m_edtSelNode);
	DDX_Control(pDX, IDC_TM_USER_CAMBER_CAMB_EDT,      m_edtCamber);
	DDX_Control(pDX, IDC_TM_USER_CAMBER_CAMB_UNT,      m_untCamber);
	DDX_Control(pDX, IDC_TM_KEY_NODE_CAMBER_START_EDT, m_edtStart);
	DDX_Control(pDX, IDC_TM_KEY_NODE_CAMBER_END_EDT,   m_edtEnd);
	DDX_Control(pDX, IDC_TM_KEY_NODE_CAMBER_MAX_EDT,   m_edtMaxCamber);
	DDX_Control(pDX, IDC_TM_KEY_NODE_CAMBER_MAX_UNT,   m_untMaxCamber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldCamberForCS, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldCamberForCS)
	ON_BN_CLICKED(IDC_TM_OPT_ADD_RDO,         OnTmCamberOptRdo)
	ON_BN_CLICKED(IDC_TM_OPT_DEL_RDO,         OnTmCamberOptRdo)
	ON_BN_CLICKED(IDC_TM_DEF_CAMBER_RDO,      OnTmCamberTypeRdo)
	ON_BN_CLICKED(IDC_TM_KEY_NODE_CAMBER_RDO, OnTmCamberTypeRdo)
	ON_BN_CLICKED(IDC_TM_USER_CAMBER_RDO,     OnTmCamberTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CStldCamberForCS::InitUnit()
{
	m_edtCamber.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untCamber.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtMaxCamber.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMaxCamber.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CStldCamberForCS::InitControls()
{
	m_edtStart.SetWindowText(_T("0, 0, 0"));
	m_edtEnd.SetWindowText(_T("0, 0, 0"));

	m_edtStart.SetAttUcsPos();
	m_edtStart.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_edtEnd.SetAttUcsPos();
	m_edtEnd.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_edtEnd.SetLButtonDownNotifyWindow(this);
	m_edtEnd.SetEnterNotifyWindow(this);
	m_edtStart.SetNextLink(&m_edtEnd);
	m_edtEnd.SetNextLink(NULL);
}

void CStldCamberForCS::InitMemberData()
{
	m_aDefCamber.RemoveAll();
	m_aDefCamber.Add(IDC_TM_DEF_CAMBER_STC);
	m_aDefCamber.Add(IDC_TM_DEF_CAMBER_FCM_RDO);
	m_aDefCamber.Add(IDC_TM_DEF_CAMBER_GEN_RDO);

	m_aKeyNodeCamber.RemoveAll();
	m_aKeyNodeCamber.Add(IDC_TM_KEY_NODE_CAMBER_START_STC);
	m_aKeyNodeCamber.Add(IDC_TM_KEY_NODE_CAMBER_START_EDT);
	m_aKeyNodeCamber.Add(IDC_TM_KEY_NODE_CAMBER_END_STC);
	m_aKeyNodeCamber.Add(IDC_TM_KEY_NODE_CAMBER_END_EDT);
	m_aKeyNodeCamber.Add(IDC_TM_KEY_NODE_CAMBER_MAX_STC);
	m_aKeyNodeCamber.Add(IDC_TM_KEY_NODE_CAMBER_MAX_EDT);
	m_aKeyNodeCamber.Add(IDC_TM_KEY_NODE_CAMBER_MAX_UNT);

	m_aUserCamber.RemoveAll();
	m_aUserCamber.Add(IDC_TM_USER_CAMBER_STC);
	m_aUserCamber.Add(IDC_TM_USER_CAMBER_NODE_STC);
	m_aUserCamber.Add(IDC_TM_USER_CAMBER_NODE_EDT);
	m_aUserCamber.Add(IDC_TM_USER_CAMBER_CAMB_STC);
	m_aUserCamber.Add(IDC_TM_USER_CAMBER_CAMB_EDT);
	m_aUserCamber.Add(IDC_TM_USER_CAMBER_CAMB_UNT);
}

void CStldCamberForCS::AlignControls()
{
	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	GetDlgItem(IDC_TM_DEF_CAMBER_STC)->GetWindowRect(rTo);
	GetDlgItem(IDC_TM_KEY_NODE_CAMBER_START_STC)->GetWindowRect(rMove);
	nDistX = rTo.left - rMove.left;
	nDistY = rTo.top - rMove.top - globalUtils.ScaleByDPI(1);
	CDlgUtil::CtrlMoveDistXY(this, m_aKeyNodeCamber, nDistX, nDistY);

	GetDlgItem(IDC_TM_DEF_CAMBER_STC)->GetWindowRect(rTo);
	GetDlgItem(IDC_TM_USER_CAMBER_STC)->GetWindowRect(rMove);
	nDistX = rTo.left - rMove.left;
	nDistY = rTo.top - rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aUserCamber, nDistX, nDistY);
}

BOOL CStldCamberForCS::IsCamberRst()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if(pDoc->m_pPostCtrl->IsFromD2EResult() || pDoc->IsExcuted() || !pDoc->IsPostMode() || !pDoc->m_pAttrCtrl->ExistConstStag())
	{		
		return FALSE;
	}

	// KYE-HONG-20050311
	// 시공단계 Nonlinear에서는 사용할수 없음 : From 정진상 
	T_STCT_D StctData;
	if(!pDoc->m_pAttrCtrl->GetStct(StctData)) StctData.Initialize();
	if(StctData.nIncludeNonLinearAnalysis == 1)
	{
		return FALSE;
	}

	return TRUE;
}

void CStldCamberForCS::EnableDisableControls()
{
	UpdateData(TRUE);

	BOOL bIsCamberRst = IsCamberRst();
	if(bIsCamberRst)
	{	
		GetDlgItem(IDC_TM_DEF_CAMBER_RDO)->EnableWindow(m_nOption != 1);
	}
	else
	{
		GetDlgItem(IDC_TM_DEF_CAMBER_RDO)->EnableWindow(FALSE);
		if(m_nCamberType == 0)
		{
			m_nCamberType = 1;
			UpdateData(FALSE);

			ControlsShowHide();
			m_edtSelNode.Disconnect();
		}
	}	

	GetDlgItem(IDC_TM_KEY_NODE_CAMBER_RDO)->EnableWindow(m_nOption!=1);
	GetDlgItem(IDC_TM_USER_CAMBER_RDO)->EnableWindow(m_nOption!=1);
	CDlgUtil::CtrlEnableDisable(this, m_aDefCamber,     m_nOption!=1 && bIsCamberRst);
	CDlgUtil::CtrlEnableDisable(this, m_aKeyNodeCamber, m_nOption!=1);
	CDlgUtil::CtrlEnableDisable(this, m_aUserCamber,    m_nOption!=1);
}

void CStldCamberForCS::ControlsShowHide()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aDefCamber,     m_nCamberType==0);
	CDlgUtil::CtrlShowHide(this, m_aKeyNodeCamber, m_nCamberType==1);
	CDlgUtil::CtrlShowHide(this, m_aUserCamber,    m_nCamberType==2);
}

/////////////////////////////////////////////////////////////////////////////
// CStldCamberForCS message handlers
BOOL CStldCamberForCS::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitUnit();
	InitControls();
	InitMemberData();

	AlignControls();

	UpdateData(FALSE);

	EnableDisableControls();
	ControlsShowHide();
	// TODO: Add extra initialization here

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStldCamberForCS::OnTmCamberOptRdo()
{
	UpdateData(TRUE);

	EnableDisableControls();
	ControlsShowHide();
}

void CStldCamberForCS::OnTmCamberTypeRdo()
{
	UpdateData(TRUE);

	EnableDisableControls();
	ControlsShowHide();

	if(m_nCamberType==2)
		m_edtSelNode.Connect(SC_ID_NODE, &m_edtSelNode);	
	else
		m_edtSelNode.Disconnect();
}

void CStldCamberForCS::GetNodeCamberSummationData(int nRstType, CMap<T_NODE_K,T_NODE_K, double, double>& mapNodeCamberSummationData)
{		
	mapNodeCamberSummationData.RemoveAll();

	switch(nRstType)
	{
	case 0: // FCM Camber
		{
			CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
			ASSERT(pServiceHeadOffice);	
			int nReq = pServiceHeadOffice->ReqService(_ULS(tb), EN_TBSA_FCM_CAMBER_NODE_DATA, (void*)&mapNodeCamberSummationData);
		}
		break;
	case 1: // General Camber
		{
			CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
			ASSERT(pServiceHeadOffice);	
			int nReq = pServiceHeadOffice->ReqService(_ULS(tb), EN_TBSA_GEN_CAMBER_NODE_DATA, (void*)&mapNodeCamberSummationData);
		}
		break;
	default:
		ASSERT(0);
	}
}

BOOL CStldCamberForCS::CalcCamberValByLength(CArray<T_NODE_K, T_NODE_K>& aNodeKey,	double WX1, double WY1, double WZ1, double WX2, double WY2, double WZ2, CArray<double, double>& aCamberVal)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	aCamberVal.RemoveAll();

	// ax2 + bx + c = 0
	// a = 4*H / l2
	// b = -4*H / l
	double dMaxCamber = m_edtMaxCamber.GetEditValue();
	double dLength = CMathFunc::mathLength(WX1, WY1, WZ1, WX2, WY2, WZ2);

	T_NODE_D NodeD;
	double dLength_T = 0.0;
	double dValue = 0.0;

	for(int i = 0; i < aNodeKey.GetSize(); i++)
	{
		NodeD.Initialize();

		// 전 후 처리 분류 해야 하나???
		if(!pDoc->m_pAttrCtrl->GetNode(aNodeKey[i], NodeD))
		{
			ASSERT(0);
			return FALSE; 
		}
		dLength_T = CMathFunc::mathLength(WX1, WY1, WZ1, NodeD.x, NodeD.y, NodeD.z);				
		dValue = ( -((4.0*dMaxCamber)/(dLength*dLength)) * (dLength_T*dLength_T) ) +( ((4*dMaxCamber)/dLength) * dLength_T );		

		aCamberVal.Add(dValue);
	}

	return TRUE;
}

void CStldCamberForCS::Execute() 
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_CMCS_D CmcsD; CmcsD.Initialize();

	CArray<T_CMCS_K, T_CMCS_K> aCmcsK;
	CArray<T_CMCS_D, T_CMCS_D&> aCmcsD;	

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	int nSize;

	if(m_nOption == 0) 		// Add/Mod
	{
		if(m_nCamberType == 0)// Deformation Camber
		{
			CArray<T_NODE_K, T_NODE_K> aSelKey;

			pIGM->GetSelectedNodeKeyList(aSelKey);			
			nSize = aSelKey.GetSize();
			if(nSize < 1)
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
				return;
			}

			BOOL bRstExistInNode = FALSE;
			CMap<T_NODE_K,T_NODE_K, double, double> mapNodeCamberSummationData;
			GetNodeCamberSummationData(m_nCamberResult, mapNodeCamberSummationData);
			for(int i = 0; i < nSize; i++)
			{			
				double dDeform;
				if(!mapNodeCamberSummationData.Lookup(aSelKey[i], dDeform))
				{
					continue;
				}

				bRstExistInNode = TRUE;

				if(!pDoc->m_pAttrCtrl2->GetCmcs(aSelKey[i], CmcsD))
				{
					CmcsD.Initialize();
				}
				CmcsD.dDeform = dDeform;

				aCmcsK.Add(aSelKey[i]);
				aCmcsD.Add(CmcsD);
			}

			if(bRstExistInNode == FALSE)
			{				
				pDoc->DisplayHistoryMessage(_LS(IDS_TM_NO_RST_ON_SEL_NODE));
				return;
			}
		}
		else if(m_nCamberType == 1)// Using Interpolation by Key Nodes Camber
		{
			CArray<T_NODE_K, T_NODE_K> aNodeKey;
			double ux, uy, uz;
			double WX1, WY1, WZ1;
			double WX2, WY2, WZ2;

			if (!CFormulaEdit::GetPosition(&m_edtStart, ux, uy, uz))
			{
				AfxMessageBox(_STR(_T("Error : 1st position is invalid!")));
				return;
			}
			m_edtStart.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);
			if (!CFormulaEdit::GetPosition(&m_edtEnd, ux, uy, uz))
			{
				AfxMessageBox(_STR(_T("Error : 2nd position is invalid!")));
				return;
			}
			m_edtEnd.GetWcsPos(ux, uy, uz, WX2, WY2, WZ2);

			// Get Node Key List
			double dbTolerance = pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
			pDoc->m_pDataCtrl->GetNodeBy2Point(WX1, WY1, WZ1, WX2, WY2, WZ2, dbTolerance, aNodeKey, TRUE);

			if (aNodeKey.GetSize() == 0)
			{
				AfxMessageBox(_STR(_T("Error : No Node on line!")));
				return;
			}

			CArray<double, double> aCamberVal;
			if(!CalcCamberValByLength(aNodeKey,	WX1, WY1, WZ1, WX2, WY2, WZ2, aCamberVal))
			{
				return;
			}			

			ASSERT(aNodeKey.GetSize() == aCamberVal.GetSize());

			nSize = aNodeKey.GetSize();
			for(int i = 0; i < nSize; i++)
			{
				if(!pDoc->m_pAttrCtrl2->GetCmcs(aNodeKey[i], CmcsD))
				{
					CmcsD.Initialize();
				}
				CmcsD.dUser = aCamberVal[i];				
				aCmcsK.Add(aNodeKey[i]);
				aCmcsD.Add(CmcsD);
			}
		}
		else if(m_nCamberType == 2)// User defined Camber
		{
			CArray<T_NODE_K, T_NODE_K> aNodeKey;
			pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeKey);
			int nSize = aNodeKey.GetSize();
			if(nSize < 1)
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
				return;
			}

			double dMaxCamber = m_edtCamber.GetEditValue();

			for(int i = 0; i < nSize; i++)
			{
				if(!pDoc->m_pAttrCtrl2->GetCmcs(aNodeKey[i], CmcsD))
				{
					CmcsD.Initialize();
				}
				CmcsD.dUser = dMaxCamber;	
				aCmcsK.Add(aNodeKey[i]);
				aCmcsD.Add(CmcsD);
			}
		}
		else
		{
			ASSERT(0);
			return;
		}

		if(!pDoc->m_pDataCtrl->AddCmcs(aCmcsK, aCmcsD))
		{
			pDoc->DisplayHistoryMessage(_LS(IDS_TM_COMMONWND_Invalid_data__Operation_has_been_c));
			return;
		}
	}
	else if(m_nOption == 1) // Delete
	{
		CArray<T_NODE_K, T_NODE_K> aSelKey;

		pIGM->GetSelectedNodeKeyList(aSelKey);

		if(!pDoc->m_pDataCtrl->DelCmcs(aSelKey))
		{
			ASSERT(0);
			return;
		}	
	}
	else
	{
		ASSERT(0);
	}

	pIGM->UnselectAllElem(FALSE);
	pIGM->UnselectAllNode();
}

void CStldCamberForCS::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DEFAULT:
		// do something...
		break;
	case D_UPDATE_BUFFER_BEFORE:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
		{
			//DeleteChildDlgs();
			//CreateChildDlgs();
			// Add by ZINU.('01.1.2).
			// do something...
			break;
		}
	case D_UPDATE_UNIT:
		// do something...
		break;
	case D_UPDATE_SEL_PRE:
	case D_UPDATE_SEL_POST:
		EnableDisableControls();
		break;
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
	}
}


