// StldPresDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPresDlg2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
//#include "..\wg_cmd\CMPsltDlg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
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
// CStldPresDlg2 dialog
#define CDialog CMenuBarChildDlg
static int SaveWhoAmI  = -100;
static int SaveWhoAmI2 = -100;

CStldPresDlg2::CStldPresDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CStldPresDlg2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldPresDlg2)
	m_nOption = 0;
	m_nProjection = 1;
	m_nSelection = 1;
	m_nLoadType = 0;
	m_nInputType = 0;
	//}}AFX_DATA_INIT
	m_pBitmap = NULL;
}

CStldPresDlg2::~CStldPresDlg2()
{
	if(m_pBitmap) delete m_pBitmap;
}

void CStldPresDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPresDlg2)
	DDX_Control(pDX, IDC_TM_ELEM_TYPE, m_CobxEType);
	DDX_Control(pDX, IDC_TM_VALUE1, m_wndPres1);
	DDX_Control(pDX, IDC_TM_VALUE2, m_wndPres2);
	DDX_Control(pDX, IDC_TM_VALUE3, m_wndPres3);
	DDX_Control(pDX, IDC_TM_VALUE4, m_wndPres4);
	DDX_Control(pDX, IDC_TM_VALUE4_UNIT, m_wndPresUnit4);
	DDX_Control(pDX, IDC_TM_VALUE3_UNIT, m_wndPresUnit3);
	DDX_Control(pDX, IDC_TM_VALUE2_UNIT, m_wndPresUnit2);
	DDX_Control(pDX, IDC_TM_VALUE1_UNIT, m_wndPresUnit1);
	DDX_Control(pDX, IDC_TM_VECTOR_EDIT, m_EditVector);
	DDX_Control(pDX, IDC_TM_PREEDGE_COMBO, m_CobxFaceEdge);
	DDX_Control(pDX, IDC_TM_DIR_COMBO, m_CobxDir);
	DDX_Control(pDX, IDC_TM_LDGR_CBO, m_CobxLdgr);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_CobxLC);
	DDX_Control(pDX, IDC_TM_CBO_LTYPE, m_CobxPslt);  
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_TM_PROJ_YES, m_nProjection);
	DDX_Radio(pDX, IDC_TM_SELECT_BY_NODE, m_nSelection);
	DDX_Radio(pDX, IDC_TM_UNIFORM, m_nLoadType);
	DDX_Radio(pDX, IDC_TM_PRES2_LCASE_RDO, m_nInputType);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndElementTypePicture);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldPresDlg2, CDialog)
	//{{AFX_MSG_MAP(CStldPresDlg2)
	ON_BN_CLICKED(IDC_TM_UNIFORM, OnTmSubLoadType)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnTmOption)
	ON_CBN_SELCHANGE(IDC_TM_DIR_COMBO, OnSelchangeTmDirCombo)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE, OnSelchangeTmElemType)
	ON_CBN_SELCHANGE(IDC_TM_PREEDGE_COMBO, OnSelchangeTmPreedgeCombo)
	ON_CBN_SELCHANGE(IDC_TM_CBO_LTYPE    , OnSelchangeTmLType)
	ON_BN_CLICKED(IDC_TM_SELECT_BY_NODE, OnTmSelectType)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnTmBtnLc)
	ON_BN_CLICKED(IDC_TM_BTN_LTYPE, OnTmBtnLType)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_LINEAR, OnTmSubLoadType)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnTmOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnTmOption)
	ON_BN_CLICKED(IDC_TM_SELECT_BY_ELEM, OnTmSelectType)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_PRES2_LCASE_RDO, OnInputTypeRdo)
	ON_BN_CLICKED(IDC_TM_PRES2_LTYPE_RDO, OnInputTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// 변경된 ElemType과 변경 되지 않은 ElemType을 모두 참조 해서.. Control 관리 할것..
/*
int CStldPresDlg2::GetPlateSolidOptionMan()
{
	if(NULL ==m_wndElemType.GetSafeHwnd()) return 0;
	int nElemType = m_wndElemType.GetCurSel();
	
	if(nElemType >0) nElemType++;
	

	//Plate Face Select : 0  Edge Select : 7
	//Solid All Select  : 8

	if(nElemType == 0 )
	{
		CtrlShowHide(this->m_arCtrlPlateOpt, TRUE);
		CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
		//////////////////////////////////////////
		//Solid Option을 비활성화 ,  Plate Option을 활성화 시킨다.
		//Plate Option을 참조 nElemType을 설정..
		if(m_nPlateOption == 1)
			nElemType = 7;
	}
	else if( nElemType == 4) //Solid
	{
		CWnd * pSrc, * pDest;
		pDest = GetDlgItem(IDC_TM_FACE_RADIO);
		pSrc  = GetDlgItem(IDC_TM_SOLID_ALL_RADIO);

		ASSERT(pDest); ASSERT(pSrc);

		CRect DestRect, SrcRect;
		pDest->GetWindowRect(&DestRect);
		pSrc ->GetWindowRect(&SrcRect);

		int YDelta = DestRect.top - SrcRect.top;

		CtrlShowHide(this->m_arCtrlPlateOpt, FALSE);
		CtrlShowHide(this->m_arCtrlSolidOpt, TRUE);
		this->CtrlMoveDistY(this->m_arCtrlSolidOpt, YDelta);
		

		//////////////////////////////////////////
		//Plate Option을 비활성화 , Solid Option을 활성화 시킨다. 
		//Solid Option을 참조 nElemType설정..
		switch(m_nSolidOption)
		{
		case 0: nElemType = 8; break;
		case 1: nElemType = 4; break;
		case 2: nElemType = 5; break;
		case 3: nElemType = 6; break;
		default: ASSERT( FALSE); break;
		}
	}
	else if( nElemType == 5) // Wall
	{
		nElemType = 9;
		CtrlShowHide(this->m_arCtrlPlateOpt, FALSE);
		CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
	}
	else
	{
		CtrlShowHide(this->m_arCtrlPlateOpt, FALSE);
		CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
	}

	
	CArray<UINT,UINT> SelMCtrls;
	//SelMCtrls.Add(IDC_TM_STATIC2);
	SelMCtrls.Add(IDC_TM_SELECT_BY_NODE);
	//SelMCtrls.Add(IDC_TM_SELECT_BY_ELEM);

	CArray<UINT,UINT> TSelMCtrls;
	TSelMCtrls.Add(IDC_TM_SELECT_BY_NODE);
	TSelMCtrls.Add(IDC_TM_SELECT_BY_ELEM);
	//CDlgUtil::CtrlRadioGetCheck(this,TSelMCtrls,m_nSelectionMethod);
	if(nElemType == 0)
	{
		
		CDlgUtil::CtrlRadioSetCheck(this,TSelMCtrls, 1);
		CtrlEnableDisable(SelMCtrls,FALSE);
		m_nSelectionMethod = 1;
	}
	else
	{
		CtrlEnableDisable(SelMCtrls,TRUE);
	}


	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_TM_DIR_TITLE);
	Ctrls.Add(IDC_TM_DIR_COMBO);
	
	if((nElemType == 8          && m_nSelectionMethod != 1) || 
		 (m_nSelectionMethod != 1 && nElemType <= 6 && nElemType >= 0) ||
		 (nElemType == 7 && m_nSelectionMethod != 1)||
		 (nElemType == 9 && m_nSelectionMethod != 1))
	{
		this->CtrlEnableDisable(Ctrls,FALSE);  
	}
	else 
	{
		this->CtrlEnableDisable(Ctrls,TRUE);   
	}

	return nElemType;
}
*/
/////////////////////////////////////////////////////////////////////////////
// CStldPresDlg2 message handlers

/*
int CStldPresDlg2::GetElemTypeExternal()
{
	UpdateData(TRUE);
	
	// Edge Select By Node ( 9 )  Face Select By Node ( 10);
		// 0 Plate Face       
		// 1 Planestress 
		// 2 Planestrain
		// 3 Axisymmetric
		// 4 Solid 8 Face
		// 5 Solid 6 Face
		// 6 Solid 4 Face
		// 7 Plate Edge
		// 8 Solid Face 
		// 9 Plate Edge By Node 
		//10 Solid Face By Node
		//11 stress Edge By Node
		//12 strain Edge By Node
		//13 axisymmetric Edge By Node
		//14 8 Node Solid Select By Node
		//15 6 Node Solid Select By Node
		//16 4 Node Solid Select By Node
		//17 Wall Edge By Node
		//18 Wall Edge 

	int nElemType = GetPlateSolidOptionMan();

	if(m_nSelectionMethod == 0 && nElemType == 9) nElemType = 17;  // Wall Edge By Node
	if(m_nSelectionMethod == 1 && nElemType == 9) nElemType = 18;  // Wall Edge 

	if(m_nSelectionMethod == 0 && nElemType == 7) nElemType = 9;
	if(m_nSelectionMethod == 0 && nElemType == 8) nElemType = 10;
	
	if(m_nSelectionMethod == 0 && nElemType == 1) nElemType = 11;  // stress Edge By Node
	if(m_nSelectionMethod == 0 && nElemType == 2) nElemType = 12;  // strain Edge By Node
	if(m_nSelectionMethod == 0 && nElemType == 3) nElemType = 13;  // axisymmetric Edge By Node
	
	if(m_nSelectionMethod == 0 && nElemType == 4) nElemType = 14;// 8 Node Solid Select By Node
	if(m_nSelectionMethod == 0 && nElemType == 5) nElemType = 15;// 6 Node Solid Select By Node
	if(m_nSelectionMethod == 0 && nElemType == 6) nElemType = 16;// 4 Node Solid Select By Node


	return nElemType;

	return 0;
}
*/

class _EType
{ 
public:
	int nFaceOrEdge;
	int nLoadType;
	int nWhoAmI;
};

_EType ETypes[6];
#define _FACE 0
#define _EDGE 1

#define PLATESTRESS_FACE 0
#define PLATESTRESS_EDGE 1
#define SOLID_FACE 2
#define PLAINSTRAIN_EDGE 3
#define AXISY_EDGE 4
#define WALL_EDGE  5
//int LoadDirectionNew;    // 0=Normal, 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=Vector 
#define DIR_NORMAL 0
#define DIR_LX     1
#define DIR_LY     2
#define DIR_LZ     3
#define DIR_GX     4
#define DIR_GY     5
#define DIR_GZ     6
#define DIR_VECTOR 7

#define FACE_1   1
#define FACE_2   2
#define FACE_3   3
#define FACE_4   4
#define FACE_5   5
#define FACE_6   6

#define EDGE_1   1
#define EDGE_2   2
#define EDGE_3   3
#define EDGE_4   4


int CStldPresDlg2::GetElemTypeExternal()
{
	UpdateData(TRUE);
	
	// Edge Select By Node ( 9 )  Face Select By Node ( 10);
	// 0 Plate Face       
	// 1 Planestress 
	// 2 Planestrain
	// 3 Axisymmetric
	// 4 Solid 8 Face
	// 5 Solid 6 Face
	// 6 Solid 4 Face
	// 7 Plate Edge
	// 8 Solid Face 
	// 9 Plate Edge By Node 
	//10 Solid Face By Node
	//11 stress Edge By Node
	//12 strain Edge By Node
	//13 axisymmetric Edge By Node
	//14 8 Node Solid Select By Node
	//15 6 Node Solid Select By Node
	//16 4 Node Solid Select By Node
	//17 Wall Edge By Node
	//18 Wall Edge 
	int nRetVal;
	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);

	if(this->m_nSelection == 1) // Select By Element
	{
		switch(((_EType*)m_CobxEType.GetItemData(nSel))->nWhoAmI)
		{
		case PLATESTRESS_FACE: nRetVal = 0; break;
		case PLATESTRESS_EDGE: nRetVal = 7; break;
		case WALL_EDGE :       nRetVal = 18; break;
		case SOLID_FACE:       nRetVal = 8; break;
		case PLAINSTRAIN_EDGE: nRetVal = 2; break;
		case AXISY_EDGE:       nRetVal = 3; break;
		}
	}
	else                        // Select By Node
	{
		switch(((_EType*)m_CobxEType.GetItemData(nSel))->nWhoAmI)
		{
		case PLATESTRESS_FACE: nRetVal = 0; break;
		case PLATESTRESS_EDGE: nRetVal = 9; break;
		case WALL_EDGE :       nRetVal = 17; break;
		case SOLID_FACE:       nRetVal = 10; break;
		case PLAINSTRAIN_EDGE: nRetVal = 12; break;
		case AXISY_EDGE:       nRetVal = 13; break;
		}
	}
	return nRetVal;
}

void CStldPresDlg2::CheckMode(BOOL bCheck)
{
	if(CDBDoc::GetDocPoint())
	{
		if(bCheck)
		{
			int nElemType = GetElemTypeExternal();
			CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(TRUE);
			CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(nElemType,m_CobxFaceEdge.GetCurSel());	
		}
		else
		{
			CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);  
			CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);	
		}
	}
}

/*
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure
	int LoadType;            // 1=Plate/PlaneStress/Wall Load 2=Axisymmetric/PlaneStrain 3=Solid
	int nSubLoadType;        // 0=Pressure(Plate/PlaneStress,Axisymmetric/PlaneStrain,Solid), 1=Concentrated Load(x), 2=단위길이당 Edge Load(Plate/PlaneStress/Wall)
	int nEdgeFace;           // PlateFace는 사용안함, Plate(PlaneStress,Wall)Edge/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	int LoadDirectionNew;    // 0=Normal, 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=Vector
													 // Plate(Face) : Local(1~3), Global(4~6), Vector(7)
													 // Plate(Edge) : Normal(0), Local(1~3), Global(4~6), Vector(7)
													 // Solid(Face) : Normal(0), Local(1~3), Global(4~6), Vector(7)
													 // Plane Strain(Edge) : Normal(0), LX(1), LY(2), Vector(7)
													 // Axisymmetric(Edge) : Normal(0), LX(1), LY(2), Vector(7)
													 // Wall(Edge)  : Normal(0), Local(1~3), Global(4~6), Vector(7)
	int Projection;          // LoadDirection == Global(4, 5, 6)에만 0,1  1=Projection 
	double Force[5];         // PU, P1, P2, P3, P4          (nSubLoadType=0 인 경우 사용)
	double EdgeLoad[3];      // PU, P1, P2 단위길이당 하중  (nSubLoadType=2 인 경우 사용)
	double Vector[3];        // LoadDirection == Vector인 경우
	T_LDGR_K GroupKey;
*/

BOOL CStldPresDlg2::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitCtrls();
	SetPresUnit();
	SetDirCobx();
	SetEdgeFaceCobx();
	SelectionCtrlMan();
	ProjectionCtrlMan();
	LoadCtrlMan();
	
	CheckMode(TRUE);

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CStldPresDlg2::InitCtrls()
{
	ETypes[0].nFaceOrEdge = _FACE; ETypes[0].nLoadType = 1; ETypes[0].nWhoAmI = PLATESTRESS_FACE;
	ETypes[1].nFaceOrEdge = _EDGE; ETypes[1].nLoadType = 1; ETypes[1].nWhoAmI = PLATESTRESS_EDGE;
	ETypes[2].nFaceOrEdge = _FACE; ETypes[2].nLoadType = 3; ETypes[2].nWhoAmI = SOLID_FACE;
	ETypes[3].nFaceOrEdge = _EDGE; ETypes[3].nLoadType = 2; ETypes[3].nWhoAmI = PLAINSTRAIN_EDGE;
	ETypes[4].nFaceOrEdge = _EDGE; ETypes[4].nLoadType = 2; ETypes[4].nWhoAmI = AXISY_EDGE;
	ETypes[5].nFaceOrEdge = _EDGE; ETypes[5].nLoadType = 1; ETypes[5].nWhoAmI = WALL_EDGE ;

	m_CobxEType.AddString(_LS(IDS_TM_0524Plate_Plane_Stress_Face_)
); m_CobxEType.SetItemData(0,(DWORD_PTR)&(ETypes[0]  ));
	m_CobxEType.AddString(_LS(IDS_TM_0524Plate_Plane_Stress_Edge_)
); m_CobxEType.SetItemData(1,(DWORD_PTR)&(ETypes[1]));
	m_CobxEType.AddString(_LS(IDS_TM_0524Solid_Face_)
);              m_CobxEType.SetItemData(2,(DWORD_PTR)&(ETypes[2]));
	m_CobxEType.AddString(_LS(IDS_TM_0524Plane_Strain_Edge_)
);       m_CobxEType.SetItemData(3,(DWORD_PTR)&(ETypes[3]));
	m_CobxEType.AddString(_LS(IDS_TM_0524Axisymmetric_Edge_)
);       m_CobxEType.SetItemData(4,(DWORD_PTR)&(ETypes[4]));
#ifndef _CIVIL 
	m_CobxEType.AddString(_LS(IDS_TM_0524Wall_Edge_)
);               m_CobxEType.SetItemData(5,(DWORD_PTR)&(ETypes[5]));
#endif

	m_CobxEType.SetCurSel(0);

	m_EditVector.SetAttUcsDistance();
	m_EditVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_EditVector.SetWindowText(_T("0, 0, 0"));

	m_aCtrlLCaseInputEnable.RemoveAll();
	m_aCtrlLCaseInputEnable.Add(IDC_TM_CBO_LC);
	m_aCtrlLCaseInputEnable.Add(IDC_TM_BTN_LC);
	m_aCtrlLCaseInputEnable.Add(IDC_TM_ELEM_TYPE);

	m_aCtrlLTypeInputEnable.RemoveAll();
	m_aCtrlLTypeInputEnable.Add(IDC_TM_CBO_LTYPE);
	m_aCtrlLTypeInputEnable.Add(IDC_TM_BTN_LTYPE);

	OnInputTypeRdo();

	ResetPsltCmb();
}

void CStldPresDlg2::SetPresUnit()
{
	int nUnitType;
	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);

	DWORD_PTR pData = m_CobxEType.GetItemData(nSel);
	
	if( pData && ((_EType*)pData)->nFaceOrEdge == _FACE)
		nUnitType = CUnitCtrl::m_PRES_UNIT.Force;
	else
		nUnitType = CUnitCtrl::m_PRES_UNIT.EdgeLoad;

	m_wndPres1.SetUnitType(nUnitType);
	m_wndPres2.SetUnitType(nUnitType);
	m_wndPres3.SetUnitType(nUnitType);
	m_wndPres4.SetUnitType(nUnitType);
	m_wndPresUnit1.SetUnitType(nUnitType);
	m_wndPresUnit2.SetUnitType(nUnitType);
	m_wndPresUnit3.SetUnitType(nUnitType);
	m_wndPresUnit4.SetUnitType(nUnitType);
}

void CStldPresDlg2::SetEdgeFaceCobx()
{
	this->m_CobxFaceEdge.ResetContent();
	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);
	CString StrTitle;
	CString csItem;

	DWORD_PTR pData = m_CobxEType.GetItemData(nSel);

	if(pData && ((_EType*)pData)->nFaceOrEdge == _FACE)
	{
		StrTitle = _LS(IDS_WG_TREEMENU_Pressure_Face__);
		
		if(((_EType*)m_CobxEType.GetItemData(nSel))->nWhoAmI == PLATESTRESS_FACE)
		{
			m_CobxFaceEdge.AddString(_LS(IDS_TM_0524Face__1));
			m_CobxFaceEdge.SetItemData(0,FACE_1);
		}
		else
		{
			for(int i = 0; i < 6 ; i++)
			{
				csItem.Format(_LS(IDS_WG_TREEMENU_Face___d), i+1);
				m_CobxFaceEdge.AddString(csItem);
			}

			m_CobxFaceEdge.SetItemData(0,FACE_1);
			m_CobxFaceEdge.SetItemData(1,FACE_2);
			m_CobxFaceEdge.SetItemData(2,FACE_3);
			m_CobxFaceEdge.SetItemData(3,FACE_4);
			m_CobxFaceEdge.SetItemData(4,FACE_5);
			m_CobxFaceEdge.SetItemData(5,FACE_6);
		}
	}
	else if(pData && ((_EType*)pData)->nFaceOrEdge == _EDGE)
	{
		
		StrTitle = _LS(IDS_WG_TREEMENU_Pressure_Edge__);
		for (int i = 0; i < 4; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Edge___d), i+1);
			m_CobxFaceEdge.AddString(csItem);
		}
		m_CobxFaceEdge.SetItemData(0,EDGE_1);
		m_CobxFaceEdge.SetItemData(1,EDGE_2);
		m_CobxFaceEdge.SetItemData(2,EDGE_3);
		m_CobxFaceEdge.SetItemData(3,EDGE_4);
	}
	else
	{
		ASSERT(FALSE);
	}
	m_CobxFaceEdge.SetCurSel(0);
	CWnd * pTWnd = GetDlgItem(IDC_TM_PRSEDGE_TITLE);
	pTWnd->SetWindowText(StrTitle);
}

void CStldPresDlg2::ChangeBitmap()
{
#if 0
	UINT aBitmapID[] = { 
		IDB_TM_PRES1, IDB_TM_PRES2, IDB_TM_PRES3, IDB_TM_PRES4, IDB_TM_PRES5, 
		IDB_TM_PRES6,
	};

	UINT nBitmapID;
	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);
	
	DWORD_PTR pData = m_CobxEType.GetItemData(nSel);
	ASSERT(pData);

	switch(((_EType*)pData)->nWhoAmI)
	{
	case PLATESTRESS_FACE:
		nBitmapID = IDB_TM_PRES1;
		break;
	case PLATESTRESS_EDGE:
	case WALL_EDGE :
		nBitmapID = IDB_TM_PRES2;
		break;
	case SOLID_FACE:
		nBitmapID = IDB_TM_PRES3;
		break;
	case PLAINSTRAIN_EDGE:
	case AXISY_EDGE:
		nBitmapID = IDB_TM_PRES6;
		break;
	}

	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_PICTURE);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(nBitmapID);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
#else
	CString aElemTypePicture[] = {
		_T("SVG\\illustration\\Dialog\\tm_pres1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_pres2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_pres3.svg"),
		_T("SVG\\illustration\\Dialog\\tm_pres4.svg"),
		_T("SVG\\illustration\\Dialog\\tm_pres5.svg"),
		_T("SVG\\illustration\\Dialog\\tm_pres6.svg")
	};

	UINT nBitmapIndex;
	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);

	DWORD_PTR pData = m_CobxEType.GetItemData(nSel);
	ASSERT(pData);

	switch (((_EType*)pData)->nWhoAmI)
	{
	case PLATESTRESS_FACE:
		nBitmapIndex = 0;
		break;
	case PLATESTRESS_EDGE:
	case WALL_EDGE:
		nBitmapIndex = 1;
		break;
	case SOLID_FACE:
		nBitmapIndex = 2;
		break;
	case PLAINSTRAIN_EDGE:
	case AXISY_EDGE:
		nBitmapIndex = 5;
		break;
	}
	
	m_wndElementTypePicture.SetImage(aElemTypePicture[nBitmapIndex]);
#endif
}

void CStldPresDlg2::SetDirCobx()
{
	/*
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Normal));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
	

m_CobxDir.AddString(_LS(IDS_TM_0524Normal)
)  
m_CobxDir.AddString(_LS(IDS_TM_0524Local_x)
) 
m_CobxDir.AddString(_LS(IDS_TM_0524Local_y)
) 
m_CobxDir.AddString(_LS(IDS_TM_0524Local_z)
) 
m_CobxDir.AddString(_LS(IDS_TM_0524Global_X)
)
m_CobxDir.AddString(_LS(IDS_TM_0524Global_Y)
)
m_CobxDir.AddString(_LS(IDS_TM_0524Global_Z)
)
m_CobxDir.AddString(_LS(IDS_TM_0524Vector)
)  

	*/
	

	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);

	DWORD_PTR pData = m_CobxEType.GetItemData(nSel);
	ASSERT(pData);

	int CurWhoAmI = ((_EType*)pData)->nWhoAmI;

	if(SaveWhoAmI != CurWhoAmI)
	{
		m_CobxDir.ResetContent();
		switch(CurWhoAmI)
		{
		case PLATESTRESS_FACE:
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_x)
) ;m_CobxDir.SetItemData(0,DIR_LX);
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_y)
) ;m_CobxDir.SetItemData(1,DIR_LY);
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_z)
) ;m_CobxDir.SetItemData(2,DIR_LZ);
			m_CobxDir.AddString(_LS(IDS_TM_0524Global_X)
);m_CobxDir.SetItemData(3,DIR_GX);
			m_CobxDir.AddString(_LS(IDS_TM_0524Global_Y)
);m_CobxDir.SetItemData(4,DIR_GY);
			m_CobxDir.AddString(_LS(IDS_TM_0524Global_Z)
);m_CobxDir.SetItemData(5,DIR_GZ);
			m_CobxDir.AddString(_LS(IDS_TM_0524Vector)
)  ;m_CobxDir.SetItemData(6,DIR_VECTOR);
			m_CobxDir.SetCurSel(2);
			break;
		case WALL_EDGE :
		case PLATESTRESS_EDGE:
		case SOLID_FACE:
			m_CobxDir.AddString(_LS(IDS_TM_0524Normal)
)  ;m_CobxDir.SetItemData(0,DIR_NORMAL);
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_x)
) ;m_CobxDir.SetItemData(1,DIR_LX); 
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_y)
) ;m_CobxDir.SetItemData(2,DIR_LY);
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_z)
) ;m_CobxDir.SetItemData(3,DIR_LZ);
			m_CobxDir.AddString(_LS(IDS_TM_0524Global_X)
);m_CobxDir.SetItemData(4,DIR_GX);
			m_CobxDir.AddString(_LS(IDS_TM_0524Global_Y)
);m_CobxDir.SetItemData(5,DIR_GY);
			m_CobxDir.AddString(_LS(IDS_TM_0524Global_Z)
);m_CobxDir.SetItemData(6,DIR_GZ);
			m_CobxDir.AddString(_LS(IDS_TM_0524Vector)
)  ;m_CobxDir.SetItemData(7,DIR_VECTOR);
			m_CobxDir.SetCurSel(0);
			break;
		case PLAINSTRAIN_EDGE:
		case AXISY_EDGE:
			m_CobxDir.AddString(_LS(IDS_TM_0524Normal)
)  ;m_CobxDir.SetItemData(0,DIR_NORMAL);
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_x)
) ;m_CobxDir.SetItemData(1,DIR_LX); 
			m_CobxDir.AddString(_LS(IDS_TM_0524Local_y)
) ;m_CobxDir.SetItemData(2,DIR_LY);
			m_CobxDir.AddString(_LS(IDS_TM_0524Vector)
)  ;m_CobxDir.SetItemData(3,DIR_VECTOR);
			m_CobxDir.SetCurSel(0);
			break;
		}
		SaveWhoAmI = CurWhoAmI;
		ChangeBitmap();
	}

	if(this->m_nSelection == 0) // Select By node
		m_CobxFaceEdge.EnableWindow(FALSE);
	else 
		m_CobxFaceEdge.EnableWindow(TRUE);
}

void CStldPresDlg2::SelectionCtrlMan()
{
	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);
	CButton* pNode, *pElem;
	pNode = (CButton*)GetDlgItem(IDC_TM_SELECT_BY_NODE);
	pElem = (CButton*)GetDlgItem(IDC_TM_SELECT_BY_ELEM);

	ASSERT(pNode && pElem);
	if(!pNode || !pElem) return;
	
	DWORD_PTR pData = m_CobxEType.GetItemData(nSel);
	ASSERT(pData);

	int CurWhoAmI = ((_EType*)pData)->nWhoAmI;

	if(CurWhoAmI != SaveWhoAmI2)
	{
		if(CurWhoAmI == PLATESTRESS_FACE)
		{
			m_nSelection = 1;
			pNode->SetCheck(BST_UNCHECKED);
			pElem->SetCheck(BST_CHECKED );
			pNode->EnableWindow(FALSE);
		}
		else
		{
			m_nSelection = 0;
			pNode->SetCheck(BST_CHECKED);
			pElem->SetCheck(BST_UNCHECKED );
			pNode->EnableWindow(TRUE);
		}
		SaveWhoAmI2 = CurWhoAmI;
	}

	m_CobxFaceEdge.EnableWindow(m_nSelection == 1);
}

void CStldPresDlg2::ProjectionCtrlMan()
{
	int nSel = this->m_CobxDir.GetCurSel();
	ASSERT(nSel>= 0);
	int nDir = (int)m_CobxDir.GetItemData(nSel);
	
	CArray<UINT, UINT> Ctrls;
	Ctrls.Add(IDC_TM_PROJ_YES);
	Ctrls.Add(IDC_TM_PROJ_NO );

	if(nDir == DIR_GX || nDir == DIR_GY || nDir == DIR_GZ)
		CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);
	else
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
	
	if(nDir == DIR_VECTOR)
		m_EditVector.EnableWindow(TRUE);
	else
		m_EditVector.EnableWindow(FALSE);
}

void CStldPresDlg2::LoadCtrlMan()
{
	BOOL bPresEnable[4];
	memset(bPresEnable,0,sizeof(BOOL)*4);
	
	if(m_nLoadType == 0)
	{
		bPresEnable[0] = TRUE;
		bPresEnable[1] = FALSE; m_wndPres2.SetEditUnit(0);
		bPresEnable[2] = FALSE; m_wndPres3.SetEditUnit(0);
		bPresEnable[3] = FALSE; m_wndPres4.SetEditUnit(0);
	}
	else
	{
		bPresEnable[0] = TRUE;
		int nESel = m_CobxEType.GetCurSel();
		ASSERT(nESel >= 0);
		
		DWORD_PTR pData = m_CobxEType.GetItemData(nESel);
		ASSERT(pData);

		if(((_EType*)pData)->nFaceOrEdge == _FACE)
		{
			bPresEnable[1] = TRUE;
			bPresEnable[2] = TRUE;
			bPresEnable[3] = TRUE;
		}
		else
		{
			bPresEnable[1] = TRUE;
			bPresEnable[2] = FALSE; m_wndPres3.SetEditUnit(0);
			bPresEnable[3] = FALSE; m_wndPres4.SetEditUnit(0);
		}
	}

	BOOL bInputByLC = m_nInputType==0;
	m_wndPres1.EnableWindow(bPresEnable[0] && bInputByLC);
	m_wndPres2.EnableWindow(bPresEnable[1] && bInputByLC);
	m_wndPres3.EnableWindow(bPresEnable[2] && bInputByLC);
	m_wndPres4.EnableWindow(bPresEnable[3] && bInputByLC);

	GetDlgItem(IDC_TM_UNIFORM)->EnableWindow(bInputByLC);
	GetDlgItem(IDC_TM_LINEAR )->EnableWindow(bInputByLC);
}

void CStldPresDlg2::OPCtrlMan()
{
	if(m_nOption == 0 || m_nOption == 1) //Add
	{
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_FRAME,TRUE,FALSE);
		if(m_nInputType==0)
		{
				CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_STATIC14,TRUE,FALSE);
		}
		SelectionCtrlMan();
		ProjectionCtrlMan();
		LoadCtrlMan();
		CheckMode(TRUE); 
	}
	else  // Delete
	{
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_FRAME,FALSE,FALSE);
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_STATIC14,FALSE,FALSE);
		CheckMode(FALSE); 
	}
}


void CStldPresDlg2::OnTmSubLoadType() 
{
	UpdateData(TRUE);
	LoadCtrlMan();
}

void CStldPresDlg2::OnTmOption() 
{
	UpdateData(TRUE);
	OPCtrlMan();
}

void CStldPresDlg2::OnSelchangeTmDirCombo() 
{
	UpdateData(TRUE);
	ProjectionCtrlMan();
}

void CStldPresDlg2::OnSelchangeTmElemType() 
{
	UpdateData(TRUE);
	SetPresUnit();
	SetDirCobx();
	SetEdgeFaceCobx();
	SelectionCtrlMan();
	ProjectionCtrlMan();
	LoadCtrlMan();
	CheckMode(TRUE); 
}

void CStldPresDlg2::OnSelchangeTmPreedgeCombo() 
{
	UpdateData(TRUE);
	SetPresUnit();
	CheckMode(TRUE); 
}

void CStldPresDlg2::OnTmSelectType() 
{
	UpdateData(TRUE);
	SetDirCobx();
	m_CobxFaceEdge.EnableWindow(m_nSelection == 1);
	CheckMode(TRUE); 
	// TODO: Add your control notification handler code here
}

void CStldPresDlg2::PostNcDestroy() 
{
	CheckMode(FALSE);	
	CDialog::PostNcDestroy();
	SaveWhoAmI = -100;
	SaveWhoAmI2= -100;
}

void CStldPresDlg2::OnTmBtnLc() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldPresDlg2::OnTmBtnLType()
{
	T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
	CMD_DoModal(D_CMD_PSLT_ITEM_DLG, &PsltK);
	ResetPsltCmb();

	for(int i=0; i<m_CobxPslt.GetCount(); i++)
	{
		T_PSLT_K PsltK_cur = m_CobxPslt.GetItemData(i);
		if(PsltK==PsltK_cur) m_CobxPslt.SetCurSel(i);
	}
}

void CStldPresDlg2::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);		
}


/*
m_wndDirVector.GetWindowText(strDirVector);
				if(TRUE == m_wndDirVector.IsKeyPressed())
				{
					if(GetDirectionVector(strDirVector,ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ))
					{
						 pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
																ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
					}
					else return FALSE;
				}
				else
				{
					//double dwx,dwy,dwz;
					m_wndDirVector.GetCurMousePosDist(ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ,
																						ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ);
				}
				*/
void CStldPresDlg2::GetWCSVectorVal(double& WX, double& WY, double& WZ)
{
	double UX,UY,UZ;
	if(TRUE == m_EditVector.IsKeyPressed())
	{
		CString StrText;
		m_EditVector.GetWindowText(StrText);    
		CStrParser::GetDirectionVector(StrText,UX,UY,UZ);
		I_GENModelBase::GetCurMySelfST()->GetWcsVector(UX,UY,UZ,WX,WY,WZ);
	}
	else
	{
		m_EditVector.GetCurMousePosDist(WX,WY,WZ,UX,UY,UZ);
	}
}


void CStldPresDlg2::OnTmClose() 
{
	if(CDBDoc::GetDocPoint())
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
	CloseDlg();
}


/*
struct T_PRES_D
{
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure
	int LoadType;            // 1=Plate/PlaneStress/Wall Load 2=Axisymmetric/PlaneStrain 3=Solid
	int nSubLoadType;        // 0=Pressure(Plate/PlaneStress,Axisymmetric/PlaneStrain,Solid), 1=Concentrated Load(x), 2=단위길이당 Edge Load(Plate/PlaneStress/Wall)
	int nEdgeFace;           // PlateFace는 사용안함, Plate(PlaneStress,Wall)Edge/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	int LoadDirectionNew;    // 0=Normal, 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=Vector
													 // Plate(Face) : Local(1~3), Global(4~6), Vector(7)
													 // Plate(Edge) : Normal(0), Local(1~3), Global(4~6), Vector(7)
													 // Solid(Face) : Normal(0), Local(1~3), Global(4~6), Vector(7)
													 // Plane Strain(Edge) : Normal(0), LX(1), LY(2), Vector(7)
													 // Axisymmetric(Edge) : Normal(0), LX(1), LY(2), Vector(7)
													 // Wall(Edge)  : Normal(0), Local(1~3), Global(4~6), Vector(7)
	int Projection;          // LoadDirection == Global(4, 5, 6)에만 0,1  1=Projection 
	double Force[5];         // PU, P1, P2, P3, P4          (nSubLoadType=0 인 경우 사용)
	double EdgeLoad[3];      // PU, P1, P2 단위길이당 하중  (nSubLoadType=2 인 경우 사용)
	double Vector[3];        // LoadDirection == Vector인 경우
	T_LDGR_K GroupKey;
*/


void CStldPresDlg2::OnTmExecute() 
{
	UpdateData(TRUE);

	if(m_nOption == 0 || m_nOption == 1) // Add or Replace
	{
		// TODO: Add your control notification handler code here
		T_PRES_D Data;
		Data.Initialize();
		
		Data.LoadCommandTypeNew = 1; // Command Type 설정 1=Pressure  2=Hydrostatic Pressure

		// LoadType 설정  1=Plate/PlaneStress/Wall Load 2=Axisymmetric/PlaneStrain 3=Solid
		int nESel = m_CobxEType.GetCurSel();
		ASSERT(nESel >= 0);

		DWORD_PTR pData = m_CobxEType.GetItemData(nESel);
		ASSERT(pData);

		Data.LoadType = ((_EType*)pData)->nLoadType;
		// SubLoadType 설정 0=Pressure(Plate/PlaneStress,Axisymmetric/PlaneStrain,Solid), 
		//1=Concentrated Load(x), 2=단위길이당 Edge Load(Plate/PlaneStress/Wall)
		if(((_EType*)pData)->nFaceOrEdge == _FACE)
			Data.nSubLoadType = 0;
		else
		{
			if(((_EType*)pData)->nWhoAmI == PLAINSTRAIN_EDGE ||
				((_EType*)pData)->nWhoAmI == AXISY_EDGE         )
				Data.nSubLoadType = 2;
			else
				Data.nSubLoadType = 2;
		}    

		//EdgeFace Index 설정  Select By Node 이면 하중 재하시 Element 별로 적용할것.
		Data.nEdgeFace = this->m_CobxFaceEdge.GetItemData(m_CobxFaceEdge.GetCurSel());
		//Load Direction 설정 
		Data.LoadDirectionNew = this->m_CobxDir.GetItemData(m_CobxDir.GetCurSel());
		//Project Option 설정
		if(Data.LoadDirectionNew == 4 || Data.LoadDirectionNew == 5 || Data.LoadDirectionNew == 6)
		{
			if(m_nProjection ==  0)
				Data.Projection = 1;
			else
				Data.Projection = 0;
		}
		else
		{
			Data.Projection = 0;
		}
		this->m_CobxLdgr.GetSelectedLdgr(Data.GroupKey);
		
		GetWCSVectorVal(Data.Vector[0],Data.Vector[1],Data.Vector[2]);

		CArray<UINT, UINT> aSelKey;
		CArray<int , int > arFaceOrEdge;
		GetElemByType(aSelKey,arFaceOrEdge);
		BOOL bSuccess=FALSE;

		// Input Type : Load Case
		if(m_nInputType==0)
		{
			if(Data.nSubLoadType == 2)       // Edge Load 
			{
				if(m_nLoadType == 0) // Uniform 
				{
					Data.EdgeLoad[0] = m_wndPres1.GetEditValue();
				}
				else  //Linear
				{
					Data.EdgeLoad[1] = m_wndPres1.GetEditValue();
					Data.EdgeLoad[2] = m_wndPres2.GetEditValue();
				}
			}
			else if(Data.nSubLoadType == 0)  // Face Load 
			{
				if(m_nLoadType == 0) // Uniform 
				{
					Data.Force[0] = m_wndPres1.GetEditValue();
				}
				else  //Linear
				{
					Data.Force[1] = m_wndPres1.GetEditValue();
					Data.Force[2] = m_wndPres2.GetEditValue();
					Data.Force[3] = m_wndPres3.GetEditValue();
					Data.Force[4] = m_wndPres4.GetEditValue();
				}
			}

			unsigned int nLType;
			m_CobxLC.GetSelectedLoad(nLType,Data.LoadCaseKey); // Load Case설정 

			if(m_nSelection == 0) // By Node 
			{
				BOOL bTemp;
				if (m_nOption == 0 || m_nOption == 1) // Add or Replace
				{
					int nElem  = aSelKey.GetSize();
					int nFE    = arFaceOrEdge.GetSize();
					ASSERT( nElem == nFE );

					for( int i = 0 ; i < nElem ; i++)
					{
						Data.nEdgeFace = arFaceOrEdge[i];
						bTemp = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey[i],Data,(m_nOption==0)?FALSE:TRUE);
						bSuccess &= bTemp;
					}
				}
			}
			else   // By Element 
			{
				bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey,Data,(m_nOption ==0)?FALSE:TRUE);
			}
			if(bSuccess) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
		}
		// Input Type : Load Type
		else
		{
			T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
			T_PSLT_D PsltD;
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) return;
#if defined (_MGEN_CH)
			Data.PSltKey = PsltK;
#endif
			if(!CDBDoc::GetDocPoint()->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Pressure_Load)), CMDTYPE_REMOVE_ANALYSIS)) { ASSERT(0); return; }
			for(int k=0; k<PSLT_NUM; k++)
			{
				if(PsltD.dLoadP1[k]==0. && PsltD.dLoadP2[k]==0. && PsltD.dLoadP3[k]==0. && PsltD.dLoadP4[k]==0.) continue;

				memset(Data.EdgeLoad,0,sizeof(Data.EdgeLoad));
				memset(Data.Force,0,sizeof(Data.Force));

				if(Data.nSubLoadType == 2)       // Edge Load 
				{
					if(PsltD.nLoadType[k] == 0) // Uniform 
					{
						Data.EdgeLoad[0] = PsltD.dLoadP1[k];
					}
					else  //Linear
					{
						Data.EdgeLoad[1] = PsltD.dLoadP1[k];
						Data.EdgeLoad[2] = PsltD.dLoadP2[k];
					}
				}
				else if(Data.nSubLoadType == 0)  // Face Load 
				{
					if(PsltD.nLoadType[k] == 0) // Uniform 
					{
						Data.Force[0] = PsltD.dLoadP1[k];
					}
					else  //Linear
					{
						Data.Force[1] = PsltD.dLoadP1[k];
						Data.Force[2] = PsltD.dLoadP2[k];
						Data.Force[3] = PsltD.dLoadP3[k];
						Data.Force[4] = PsltD.dLoadP4[k];
					}
				}

				Data.LoadCaseKey = PsltD.LoadCaseKey[k];  // Load Case설정 

				BOOL bTransaction=FALSE;
				bSuccess = TRUE;
				if(m_nSelection == 0) // By Node 
				{
					BOOL bTemp;
					if (m_nOption == 0 || m_nOption == 1) // Add or Replace
					{
						int nElem  = aSelKey.GetSize();
						int nFE    = arFaceOrEdge.GetSize();
						ASSERT( nElem == nFE );

						for( int i = 0 ; i < nElem ; i++)
						{
							Data.nEdgeFace = arFaceOrEdge[i];
							bTemp = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey[i],Data,(m_nOption==0)?FALSE:TRUE,bTransaction);
							bSuccess &= bTemp;
						}
					}
				}
				else   // By Element 
				{
					bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey,Data,(m_nOption==0)?FALSE:TRUE,bTransaction);
				}
			}
			
			if(bSuccess && CDBDoc::GetDocPoint()->m_pDataCtrl->CheckModifiedModel()) 
			{
				CDBDoc::GetDocPoint()->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, LT_PRES_CMD);
				CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
			}
			else
			{
				CDBDoc::GetDocPoint()->m_pUndoCtrl->CancelEditDB();
			}
		}
	}
	else // Delete 
	{
		CArray<UINT, UINT> aSelKey;
		CArray<int , int > arFaceOrEdge;
		GetElemByType(aSelKey,arFaceOrEdge);
		unsigned int LCType, LCKey;
		T_LDGR_K LdgrK;
		this->m_CobxLC.GetSelectedLoad(LCType,LCKey);
		this->m_CobxLdgr.GetSelectedLdgr(LdgrK);
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->DelPres(aSelKey, LCKey,LdgrK))
			 CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CStldPresDlg2::GetElemByType(CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	
	if(m_nOption == 1 || m_nOption == 0) // Add , Replace
	{
		if(m_nSelection == 1) // By Select
		{
			pIGM->GetSelectedElemKeyList(aSelKey);
		}
		else
		{
			int nESel = m_CobxEType.GetCurSel();
			ASSERT(nESel >= 0);
		
		
			CArray<T_ELEM_K,T_ELEM_K> aElemList; // Select된 element list
			CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
			
			pIGM->GetSelectedNodeKeyList(aSelNodeKey);
			pIGM->GetActiveElemKeys(aElemList);

	  DWORD_PTR pData = m_CobxEType.GetItemData(nESel);
	  ASSERT(pData);
			
			if(((_EType*)pData)->nFaceOrEdge == _FACE)
			{
				 CDBDoc::GetDocPoint()->SelectFaceByNode(aElemList, 
																								 aSelNodeKey, 
																								 aSelKey,
																								 arFaceOrEdge);
			}else if(((_EType*)pData)->nFaceOrEdge == _EDGE)
			{
				CDBDoc::GetDocPoint()->SelectEdgeByNode(aElemList   , 
																								aSelNodeKey , 
																								aSelKey     ,
																								arFaceOrEdge);
			}
		}
	}
	else  // Delete
	{
		pIGM->GetSelectedElemKeyList(aSelKey);
	}
	
	int i;
	T_ELEM_D ElemData;
	
	if (m_nOption == 2) // delete
	{
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (ElemData.eltyp == PLATE_EL || 
					ElemData.eltyp == PLSTRS_EL ||
					ElemData.eltyp == PLSTRN_EL ||
					ElemData.eltyp == AXISYM_EL ||
					ElemData.eltyp == SOLID_EL  ||
					ElemData.eltyp == WALL_EL   ) continue;
			aSelKey.RemoveAt(i);
		}
		return;
	}

	int nSel = m_CobxEType.GetCurSel();
	ASSERT(nSel >= 0);

	DWORD_PTR pData = m_CobxEType.GetItemData(nSel);
	ASSERT(pData);
	
	switch(((_EType*)pData)->nWhoAmI)
	{
	case PLATESTRESS_EDGE:
	case PLATESTRESS_FACE:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsPlate(ElemData.eltyp) && 
					!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsPlstrs(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(m_nSelection == 0) // Select By Node
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case WALL_EDGE :
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsWall(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(m_nSelection  == 0)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case SOLID_FACE:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsSolid(ElemData.eltyp))
			{ 
				aSelKey.RemoveAt(i);
				if(m_nSelection == 0) // Select By Node
					arFaceOrEdge.RemoveAt(i);
			}
		}  
		break;
	case PLAINSTRAIN_EDGE:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsPlstrn(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(m_nSelection == 0) // Select By Node
					arFaceOrEdge.RemoveAt(i);
			}
		}
	
		break;

	case AXISY_EDGE:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsAxisym(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(m_nSelection == 0) // Select By Node
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	}
}

void CStldPresDlg2::OnInputTypeRdo()
{
	UpdateData(TRUE);
	LoadCtrlMan();

	// Load Type방식일 때 Delete 기능은 사용 안 함
	if(m_nInputType==1 && m_nOption==2) 
	{
		m_nOption=0;
		UpdateData(FALSE);
		OPCtrlMan();
	}
	GetDlgItem(IDC_TM_OPT_DELETE)->EnableWindow(m_nInputType==0);

	if(m_nInputType==0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLCaseInputEnable, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLTypeInputEnable, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLCaseInputEnable, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLTypeInputEnable, TRUE);
	}
}

void CStldPresDlg2::OnSelchangeTmLType()
{
	T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
	if(PsltK == -1)
		return;
	T_PSLT_D PsltD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) { ASSERT(0); return; }
	m_CobxEType.SetCurSel(PsltD.nElemType);
	OnSelchangeTmElemType();
}

void CStldPresDlg2::ResetPsltCmb()
{
	m_CobxPslt.ResetContent();
	CArray<T_PSLT_K,T_PSLT_K> aPsltK;
	T_PSLT_D PsltD;
	CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPsltKeyList(aPsltK);
	for(int i=0; i<aPsltK.GetSize(); i++)
	{
		T_PSLT_K PsltK = aPsltK[i];
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) continue;
		m_CobxPslt.AddString(PsltD.strName);
		m_CobxPslt.SetItemData(i,PsltK);
	}
	m_CobxPslt.SetCurSel(0);
	OnSelchangeTmLType();
}