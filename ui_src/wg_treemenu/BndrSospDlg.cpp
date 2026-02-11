// BndrSospDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrSospDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
//#include "..\wg_cmd\wg_cmdAll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrSospDlg dialog


CBndrSospDlg::CBndrSospDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrSospDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrSospDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_nType = 0;
	m_nBottom[0]=m_nBottom[1]=0;

	
}

CBndrSospDlg::~CBndrSospDlg()
{
}

void CBndrSospDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrSospDlg)
	
	DDX_Control(pDX, IDC_TREEMENU_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrSospDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrSospDlg)
	
	ON_BN_CLICKED(IDC_TM_BNDR_SOSP_TYPE_USER, OnTmBndrSospType)
	ON_BN_CLICKED(IDC_TM_BNDR_SOSP_TYPE_INTEGRAL, OnTmBndrSospType)
	
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBndrSospDlg::AlignControl()
{
	CRect rectRef;
	CRect rectTarget;
	CWnd* wndSub;
	if(m_nType == 0)
		wndSub = m_pIntegralDlg;
	else
		wndSub = m_pUserDlg;  
	
	CArray<UINT,UINT> arOkClose;
	arOkClose.Add(IDC_TM_EXECUTE);
	arOkClose.Add(IDC_TM_CLOSE);
	
	wndSub->GetWindowRect(&rectTarget);
	((CBCGPStatic*)GetDlgItem(IDC_TM_EXECUTE))->GetWindowRect(&rectRef);	


	int nDist = (rectRef.bottom-rectRef.top)*0.5;
	CDlgUtil::CtrlMoveDistY(this, arOkClose, rectTarget.bottom - rectRef.top + nDist);

}

void CBndrSospDlg::InitSubCtrl()
{
	m_pIntegralDlg->InitSubCtrl();
	m_pUserDlg->InitSubCtrl();
}


/////////////////////////////////////////////////////////////////////////////
// CBndrSospDlg message handlers

BOOL CBndrSospDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	CreateChildDlgs();			// load type dlg를 생성 
	m_nType = 0;
	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_INTEGRAL);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_USER);
	InitSubCtrl();
	CDlgUtil::CtrlRadioSetCheck(this , arType, m_nType);
	ShowChildDlgs(m_nType);	// 현재 mode에 맞게 보여줌 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrSospDlg::OnTmBndrSospType() 
{
	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_INTEGRAL);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_USER);
	CDlgUtil::CtrlRadioGetCheck(this , arType, m_nType);
	ShowChildDlgs(m_nType);

}

void CBndrSospDlg::OnTmClose() 
{
	CloseDlg();
}

BOOL CBndrSospDlg::GetParameter()
{
	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_INTEGRAL);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_USER);
	
	CDlgUtil::CtrlRadioGetCheck(this , arType, m_nType);
	if(!m_pIntegralDlg->GetParameter()) return FALSE;
	if(!m_pUserDlg->GetParameter()) return FALSE;
	
	return TRUE;

}
void CBndrSospDlg::ExecuteUser() 
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CArray<int,int> rFace;
	T_ELEM_D ElemData;
	if(!GetParameter())
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Soil_Spring_Data));
		return;
	}

	int nElType = UserData.ElType;
	if(nElType == 3) UserData.ElType = 2;
	

	if(nElType != 3)
	{
		pIGM->GetSelectedElemKeyList(arElemKeyList);    
	}
	else
	{
		CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
		CArray<T_ELEM_K, T_ELEM_K> aElemList;
		pIGM->GetSelectedNodeKeyList(aSelNodeKey);
		pIGM->GetActiveElemKeys(aElemList);
		CDBDoc::GetDocPoint()->SelectFaceByNode(aElemList, 
																						aSelNodeKey, 
																						arElemKeyList,
																						rFace);
	}

	if(nElType == 0) //Frame
	{
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Frame타입만 선택한다.
		{
			m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
			if(!m_pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp))
				arElemKeyList.RemoveAt(i-1);
		}

		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
			return;
		}
	}
	else if(nElType == 1)  // Planar 일때.
	{
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Plane타입만 선택한다.
		{
			m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
			if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp))
				arElemKeyList.RemoveAt(i-1);
		}

		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
			return;
		}
	}
	else if(nElType == 2) // Solid (Face) 일때.
	{
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Solid타입만 선택한다.
		{
			m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
			if(!m_pDoc->m_pAttrCtrl->IsSolidType(ElemData.eltyp))
			{
				arElemKeyList.RemoveAt(i-1);
			}
			else
			{
				//if(m_CobxElemType.GetCurSel() == 2) // Solid Face
				//{
					rFace.Add(UserData.nFace);
				//}
			}
		}

		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
			return;
		}
	}
	T_BNGR_K KeyBngr;
	m_wndGroupCombo.GetSelectedBngr(KeyBngr);

	if(m_pDoc->m_pDataCtrl->MakeSoilSpringUser(KeyBngr, arElemKeyList, UserData.nNonType, UserData.ElType, rFace, UserData.dWidth,
		UserData.dStiff, UserData.nSpringDirect, UserData.dVx, UserData.dVy, UserData.dVz,
		UserData.bDist, UserData.nGradientDirect, UserData.dReference, UserData.dGradientStiff))
	{
		 m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}

}
void CBndrSospDlg::ExecuteIntegral()
{  
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CArray<int,int> rFace;
	T_ELEM_D ElemData;
	if(!GetParameter())
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Soil_Spring_Data));
		return;
	}

	int nElType = IntegralData.nElType;
	if(nElType == 3) IntegralData.nElType = 2;
	

	if(nElType != 3)
	{
		pIGM->GetSelectedElemKeyList(arElemKeyList);    
	}
	else
	{
		CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
		CArray<T_ELEM_K, T_ELEM_K> aElemList;
		pIGM->GetSelectedNodeKeyList(aSelNodeKey);
		pIGM->GetActiveElemKeys(aElemList);
		CDBDoc::GetDocPoint()->SelectFaceByNode(aElemList, 
																						aSelNodeKey, 
																						arElemKeyList,
																						rFace);
	}

	if(nElType == 0) //Frame
	{
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Frame타입만 선택한다.
		{
			m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
			if(!m_pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp))
				arElemKeyList.RemoveAt(i-1);
		}

		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
			return;
		}
	}
	else if(nElType == 1)  // Planar 일때.
	{
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Plane타입만 선택한다.
		{
			m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
			if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp))
				arElemKeyList.RemoveAt(i-1);
		}

		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
			return;
		}
	}
	else if(nElType == 2) // Solid (Face) 일때.
	{
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Solid타입만 선택한다.
		{
			m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
			if(!m_pDoc->m_pAttrCtrl->IsSolidType(ElemData.eltyp))
			{
				arElemKeyList.RemoveAt(i-1);
			}
			else
			{
				//if(m_CobxElemType.GetCurSel() == 2) // Solid Face
				//{
					rFace.Add(IntegralData.nFace);
				//}
			}
		}

		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
			return;
		}
	}


	T_BNGR_K KeyBngr;
	m_wndGroupCombo.GetSelectedBngr(KeyBngr);

	if(m_pDoc->m_pDataCtrl->MakeSoilSpringIntegral(KeyBngr, arElemKeyList,IntegralData.nNonType, IntegralData.nElType, rFace,
 	            IntegralData.dWidth, IntegralData.dVoid, IntegralData.dWeight, IntegralData.dAbut, IntegralData.dGround, IntegralData.dDeck, 
	            IntegralData.nDirection, IntegralData.dIncre, IntegralData.dThermal))
	{
		 m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}

}

void CBndrSospDlg::OnTmExecute() 
{
	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_INTEGRAL);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_USER);
	CDlgUtil::CtrlRadioGetCheck(this , arType, m_nType);
	if(m_nType == 0)
		ExecuteIntegral();
	else if(m_nType ==1)
		ExecuteUser();
	else ASSERT(0);
}

void CBndrSospDlg::PostNcDestroy() 
{
	//CheckMode(FALSE);
	CMenuBarChildDlg::PostNcDestroy();
}

void CBndrSospDlg::OnDestroy() 
{
	DestroyChildDlgs();
	CMenuBarChildDlg::OnDestroy();
}

BOOL	CBndrSospDlg::CreateChildDlgs(void)
{

	CRect rc,SubRect;
	m_wndPlaceHolder.GetWindowRect(&rc);
	this->ScreenToClient(&rc);

	m_pIntegralDlg= new CBndrSospIntegral(&IntegralData);	
	ASSERT(m_pIntegralDlg);	
	m_pIntegralDlg->Create(IDD_TM_BNDR_SOSP_INTEGRAL,this);				
	m_pIntegralDlg->GetWindowRect(&SubRect);
	this->ScreenToClient(&SubRect);
	m_nBottom[1] = SubRect.Height() + rc.top;
	m_pIntegralDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),SubRect.Height(),SWP_NOZORDER|SWP_NOACTIVATE);			

	
	m_pUserDlg= new CBndrSospUserDlg(&UserData);	
	ASSERT(m_pUserDlg);	
	m_pUserDlg->Create(IDD_TM_BNDR_SOSP_USER,this);				
	m_pUserDlg->GetWindowRect(&SubRect);
	this->ScreenToClient(&SubRect);
	m_nBottom[0] = SubRect.Height() + rc.top;
	m_pUserDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),SubRect.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	
	return TRUE;
}
BOOL	CBndrSospDlg::DestroyChildDlgs(void)
{
	if(m_pIntegralDlg)
	{
		m_pIntegralDlg->ShowWindow(SW_HIDE);
		m_pIntegralDlg->DestroyWindow();
		delete	m_pIntegralDlg;	m_pIntegralDlg=NULL;
	}

	if(m_pUserDlg)
	{
		m_pUserDlg->ShowWindow(SW_HIDE);
		m_pUserDlg->DestroyWindow();
		delete	m_pUserDlg;	m_pUserDlg=NULL;
	}

	
	return TRUE;
}
BOOL	CBndrSospDlg::ShowChildDlgs(int which)
{
	// 모두 hide
	m_pIntegralDlg->ShowWindow(SW_HIDE);	
	m_pUserDlg->ShowWindow(SW_HIDE);	
	

	// 현재 선택된것만 show
	switch(which)
	{
		
		case 0:	m_pIntegralDlg->ShowWindow(SW_SHOW);	break;
		case 1:	m_pUserDlg->ShowWindow(SW_SHOW);	break;
	}
	AlignControl();
	return TRUE;
}

void CBndrSospDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		m_pIntegralDlg->UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		m_pIntegralDlg->UpdateBuffer();
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