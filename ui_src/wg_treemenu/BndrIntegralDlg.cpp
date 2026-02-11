// BndrSospDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrIntegralDlg.h"
#include "..\wg_base\wg_base_StrParser.h" // 파싱 - mylee
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h" // CreateOrAcivateDlg - mylee
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralDlg dialog


CBndrIntegralDlg::CBndrIntegralDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrIntegralDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrIntegralDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_nBottom[0]=m_nBottom[1]=0;

	m_arType.Add(IDC_TM_ABUT_RDO);
	m_arType.Add(IDC_TM_PILE_RDO);

	//m_pBitmap = 0;

	m_bFirstInit = TRUE;
}

CBndrIntegralDlg::~CBndrIntegralDlg()
{
}

void CBndrIntegralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrIntegralDlg)
	DDX_Control(pDX, IDC_TREEMENU_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO,       m_wndGroupCombo);
	//DDX_Control(pDX, IDC_TM_REF_FRM,           m_frmRefFigure);
	//DDX_Control(pDX, IDC_TM_FIGURE_BMP,        m_wndPicture);
	DDX_Control(pDX, IDC_TM_PICTURE,           m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBndrIntegralDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrIntegralDlg)
	ON_BN_CLICKED(IDC_TM_PILE_RDO, OnTmBndrSospType)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
// 	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
// 	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TM_ABUT_RDO, OnTmBndrSospType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBndrIntegralDlg::AlignControl()
{
	CRect rectRef;
	CRect rectTarget;
	CWnd* wndSub;

	int nSel = 0;
	//CDlgUtil::CtrlRadioGetCheck(this , m_arType, nSel);
	GetTypeSel(nSel);
	if(nSel == 0)
		wndSub = m_pAbutDlg;
	else
		wndSub = m_pPileDlg;  

	
// 	CArray<UINT,UINT> arOkClose;
// 	arOkClose.Add(IDC_TM_EXECUTE);
// 	arOkClose.Add(IDC_TM_CLOSE);
// 	
// 	((CBCGPStatic*)GetDlgItem(IDC_TM_EXECUTE))->GetWindowRect(&rectRef);	
// 
// 	int nDist = (rectRef.bottom-rectRef.top)*0.5;
// 	CDlgUtil::CtrlMoveDistY(this, arOkClose, rectTarget.bottom - rectRef.top + nDist);
}

void CBndrIntegralDlg::GetTypeSel(int& nSel)
{
	CDlgUtil::CtrlRadioGetCheck(this , m_arType, nSel);
#if defined(_MGEN)
	nSel = 1;
#endif
}

void CBndrIntegralDlg::InitSubCtrl()
{
	m_pAbutDlg->InitSubCtrl();
	m_pPileDlg->InitSubCtrl();
}

void CBndrIntegralDlg::ChangeBitmap(int nType)
{
/*
	UINT aBitmapID[] = { IDB_TM_BNDR_INTG_ABUT, IDB_TM_BNDR_INTG_PILE };
#if defined(_MGEN) // 20081008 mylee
	aBitmapID[1] = IDB_TM_BNDR_INTG_PILE_GEN;
#endif

	//if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_FIGURE_BMP);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nType]);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
*/

	CString aBitmapPath[] = {
	_T("SVG\\illustration\\Dialog\\tm_bndr_abut.svg"), // IDB_TM_BNDR_INTG_ABUT
	_T("SVG\\illustration\\Dialog\\tm_bndr_pile.svg")  // IDB_TM_BNDR_INTG_PILE
	}; 

	m_wndPicture.SetImage(aBitmapPath[nType]);
}
/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralDlg message handlers

BOOL CBndrIntegralDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

#if defined(_MGEN) // 20081008 mylee - GEN 에서는 Rdo 숨김
	CustomizeControls_GEN();
#endif
	CreateChildDlgs();			// load type dlg를 생성 

	//if (!m_bFirstInit) m_frmRefFigure.ReInit();
	//m_frmRefFigure.InitControl(this, CBndrIntegralDlg::IDD, IDC_TM_REF_FRM,TRUE,FALSE,3,4);
	
	InitSubCtrl();
	int nSel = 0;
	CDlgUtil::CtrlRadioSetCheck(this , m_arType, nSel);

	GetTypeSel(nSel);
	ShowChildDlgs(nSel);
	ChangeBitmap(nSel);

	m_bFirstInit = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrIntegralDlg::CustomizeControls_GEN()
{
	CRect rectTo, rectFrom;
	
	CArray<UINT,UINT> arCtrl;
	arCtrl.Add(IDC_TM_TYPE_FRM);
	arCtrl.Add(IDC_TM_ABUT_RDO);
	arCtrl.Add(IDC_TM_PILE_RDO);
	CDlgUtil::CtrlShowHide(this, arCtrl, FALSE);
	CDlgUtil::CtrlMoveDistX(this, arCtrl, globalUtils.ScaleByDPI(500));

	arCtrl.RemoveAll();
	arCtrl.Add(IDC_TM_REF_FRM);
	//arCtrl.Add(IDC_TM_FIGURE_BMP);
	arCtrl.Add(IDC_TM_PICTURE);
	arCtrl.Add(IDC_TREEMENU_PLACEHOLDER);
	//arCtrl.Add(IDC_TM_EXECUTE);
	//arCtrl.Add(IDC_TM_CLOSE);
	GetDlgItem(IDC_TM_REF_FRM)->GetWindowRect(&rectFrom);
	GetDlgItem(IDC_TM_TYPE_FRM)->GetWindowRect(&rectTo);

	int nDist = (rectTo.top-rectFrom.top);
	CDlgUtil::CtrlMoveDistY(this, arCtrl, nDist);
}

void CBndrIntegralDlg::OnTmBndrSospType() 
{
	int nSel = 0;
	//CDlgUtil::CtrlRadioGetCheck(this , m_arType, nSel);
	GetTypeSel(nSel);

	ShowChildDlgs(nSel);

	ChangeBitmap(nSel);
}

void CBndrIntegralDlg::OnTmClose() 
{
	CloseDlg();
}

void CBndrIntegralDlg::PostNcDestroy() 
{
	//CheckMode(FALSE);
	m_wndPicture.CloseDialog();
	CMenuBarChildDlg::PostNcDestroy();
}

void CBndrIntegralDlg::OnDestroy() 
{
/*
	if(m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
*/
	DestroyChildDlgs(); // added mylee 061121
	CMenuBarChildDlg::OnDestroy();
}

void CBndrIntegralDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);
}

void CBndrIntegralDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		m_pAbutDlg->UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		m_pAbutDlg->UpdateBuffer();
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

void CBndrIntegralDlg::OnTmExecute()
{
	Execute();
}

void CBndrIntegralDlg::Execute()
{
	int nSel = 0;
	//CDlgUtil::CtrlRadioGetCheck(this , m_arType, nSel);
	GetTypeSel(nSel);

	if(nSel == 0)
		ExecuteAbutment();
	else if(nSel ==1)
		ExecutePile();
	else ASSERT(0);
}

void CBndrIntegralDlg::ExecuteAbutment() 
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CArray<int,int> rFace;
	T_ELEM_D ElemData;
	if(!GetParameter())
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Soil_Spring_Data)); // need change by mylee
		return;
	}

	int nElType = AbutmentData.nElType;
	if(nElType == 3) AbutmentData.nElType = 2;

	if(nElType != 3)
	{
		CStrParser::GetNodeList(AbutmentData.strEList, arElemKeyList);    
	}
	else
	{
		CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
		CArray<T_ELEM_K, T_ELEM_K> aElemList;
		//pIGM->GetSelectedNodeKeyList(aSelNodeKey);
		CStrParser::GetNodeList(AbutmentData.strEList, aSelNodeKey);    
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

		/*
		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_frame_element_));
			return;
		}*/
	}
	else if(nElType == 1)  // Planar 일때.
	{
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Plane타입만 선택한다.
		{
			m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
			if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp))
				arElemKeyList.RemoveAt(i-1);
		}

		/*
		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
			return;
		}*/
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
				rFace.Add(AbutmentData.nFace);
			}
		}

		/*
		if(!arElemKeyList.GetSize())
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
			return;
		}*/
	}
	
	// Node for Footing
	CArray<T_NODE_K, T_NODE_K> arNodeK;
	CStrParser::GetNodeList(AbutmentData.strNList, arNodeK); 

	if(arNodeK.GetSize()==0 && arElemKeyList.GetSize()==0) // Elem and Node check
	{
		if      (nElType == 0) GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_frame_element_));
		else if (nElType == 1) GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
		else if (nElType == 2) GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
		else ASSERT(0);
		// GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		// GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_and_elemen));
		return;
	}

	// execute Abutment
	T_BNGR_K KeyBngr;
	m_wndGroupCombo.GetSelectedBngr(KeyBngr);
	AbutmentData.dGround = 0; // not used 070206 mylee
	AbutmentData.dEarth = 0; // "
	BOOL bResult = m_pDoc->m_pDataCtrl->MakeAbutmentSpring(KeyBngr, AbutmentData.nElType, &arElemKeyList,
													&rFace, AbutmentData.aVector, AbutmentData.nDirection,
													AbutmentData.dGround/*not used*/, AbutmentData.dVoid, AbutmentData.dGS, AbutmentData.dCycle,
													AbutmentData.dEarth/*not used*/, AbutmentData.dHeight, AbutmentData.dWidth, AbutmentData.dDeck,
													AbutmentData.dTemp, AbutmentData.MatlK, AbutmentData.dThermal, 
													&arNodeK, AbutmentData.dFWidth, AbutmentData.dFPress, AbutmentData.nRotation);
	if (bResult)
		 m_pDoc->m_pViewCtrl->UnselectAll(NULL);

}

void CBndrIntegralDlg::ExecutePile()
{  
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	T_ELEM_D ElemData;

	if(!GetParameter())
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Soil_Spring_Data)); // need change
		return;
	}
	
	// Select Frame Element only
	pIGM->GetSelectedElemKeyList(arElemKeyList); 
	for(int i=arElemKeyList.GetSize(); i > 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
		if(!m_pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp))
			arElemKeyList.RemoveAt(i-1);
	}
	if(!arElemKeyList.GetSize())
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_frame_element_)); 
		return;
	}

	// execute Pile
	T_BNGR_K KeyBngr;
	m_wndGroupCombo.GetSelectedBngr(KeyBngr);
	BOOL bResult = m_pDoc->m_pDataCtrl->MakePileSpring(KeyBngr, &arElemKeyList, PileData.nSType,
													PileData.dGroundLevel, PileData.dUnitSoilWeight, PileData.dK0, PileData.dCU,
													PileData.dInternalAngle, PileData.dPileDiameter, PileData.nKType, PileData.dK1,
													PileData.dKH, PileData.dTriAxial);
		
	if (bResult)
		 m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CBndrIntegralDlg::GetParameter()
{
	int nSel = 0;
	//CDlgUtil::CtrlRadioGetCheck(this , m_arType, nSel);
	GetTypeSel(nSel);
	if(!m_pAbutDlg->GetParameter()) return FALSE; // Abutment GetParameter()
	if(!m_pPileDlg->GetParameter()) return FALSE; // Pile GetParameter()
	
	return TRUE;
}

BOOL CBndrIntegralDlg::CreateChildDlgs(void)
{
	CRect rc,SubRect;
	m_wndPlaceHolder.GetWindowRect(&rc);
	this->ScreenToClient(&rc);

	m_pAbutDlg= new CBndrIntegralAbut(&AbutmentData);	
	ASSERT(m_pAbutDlg);	
	m_pAbutDlg->Create(IDD_TM_BNDR_INTEBR_ABUT,this);				
	m_pAbutDlg->GetWindowRect(&SubRect);
	this->ScreenToClient(&SubRect);
	m_nBottom[1] = SubRect.Height() + rc.top;
	m_pAbutDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),SubRect.Height(),SWP_NOZORDER|SWP_NOACTIVATE);			
	
	m_pPileDlg= new CBndrIntegralPileDlg(&PileData);	
	ASSERT(m_pPileDlg);	
	m_pPileDlg->Create(IDD_TM_BNDR_INTEBR_PILE,this);				
	m_pPileDlg->GetWindowRect(&SubRect);
	this->ScreenToClient(&SubRect);
	m_nBottom[0] = SubRect.Height() + rc.top;
	m_pPileDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),SubRect.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	
	return TRUE;
}

BOOL	CBndrIntegralDlg::DestroyChildDlgs(void)
{
	if(m_pAbutDlg)
	{
		m_pAbutDlg->DestroyWindow();
		delete m_pAbutDlg;
		m_pAbutDlg=NULL;
	}

	if(m_pPileDlg)
	{
		m_pPileDlg->DestroyWindow();
		delete m_pPileDlg;
		m_pPileDlg=NULL;
	}
	
	return TRUE;
}

BOOL	CBndrIntegralDlg::ShowChildDlgs(int which)
{
	// 모두 hide
	m_pAbutDlg->ShowWindow(SW_HIDE);	
	m_pPileDlg->ShowWindow(SW_HIDE);	
	
	// 현재 선택된것만 show
	switch(which)
	{
		case 0:	m_pAbutDlg->ShowWindow(SW_SHOW);	break;
		case 1:	m_pPileDlg->ShowWindow(SW_SHOW);	break;
	}
	AlignControl();
	return TRUE;
}