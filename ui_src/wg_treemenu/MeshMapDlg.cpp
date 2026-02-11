// MeshMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MeshMapDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_cmd\wg_cmdAll.h"
//#include "..\wg_cmd2\wg_cmd2All.h"
#include "..\wg_cmd2\wg_cmd2_DomainDlgs.h"
//#include "..\wg_cmd2\wg_cmd2Res2.h"
#include "..\wg_cmd2\ExportFuncCMD2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\AutoIDMaker.h"
#include "..\wg_db\ViewBuff.h"
#include "..\wg_db\IUsageCounter.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_main\wg_mainres2.h"

#include "MeshElemSubTypePlateDlg.h"
#include "MeshElemSubTypePlstDlg.h"
#include "MeshElemSubTypeWallDlg.h"


#include "MapMesh_4Point.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshMapDlg dialog
int CMeshMapDlg::m_nCurETypeIndex = 0;

CMeshMapDlg::CMeshMapDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CMeshMapDlg::IDD, pParent)
{
	m_bPropInit = FALSE;

	m_NodeD1.Initialize();
	m_NodeD2.Initialize();
	m_NodeD3.Initialize();
	m_NodeD4.Initialize();
	m_MashNodeD1.Initialize();
	m_MashNodeD2.Initialize();
	m_MashNodeD3.Initialize();
	m_MashNodeD4.Initialize();

	m_nThickThin = 0;
	m_bDOF = TRUE;
	m_nMembPlate = 0;
	m_nWallIDType = 1;
	m_nWallID = 1;
	//{{AFX_DATA_INIT(CMeshMapDlg)
	//}}AFX_DATA_INIT
}

CMeshMapDlg::~CMeshMapDlg()
{
}

void CMeshMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshMapDlg)
	DDX_Control(pDX, IDC_TM_MESH_NODES_EDIT, m_EditNodeList);
	DDX_Control(pDX, IDC_TM_MATL_EDIT   , m_EditMatl);
	DDX_Control(pDX, IDC_TM_THIC_EDIT   , m_EditThik);
	DDX_Control(pDX, IDC_TM_MATL_COMBO  , m_CobxMatl);
	DDX_Control(pDX, IDC_TM_THIC_COMBO  , m_CobxThik);
	DDX_Control(pDX, IDC_TM_ELEM_COMBO  , m_ComboElemType);
	DDX_Control(pDX, IDC_TM_1_2_DIVISION_EDIT  , m_Edit12DirDivision);
	DDX_Control(pDX, IDC_TM_1_4_DIVISION_EDIT  , m_Edit14DirDivision);
	DDX_Control(pDX, IDC_TM_1_2_BIASRATIO_EDIT  , m_Edit12DirBiasRatio);
	DDX_Control(pDX, IDC_TM_1_4_BIASRATIO_EDIT  , m_Edit14DirBiasRatio);
	DDX_Control(pDX, IDC_TM_TRI_RADIO  , m_RadioTri);
	DDX_Control(pDX, IDC_TM_QUAD_RADIO  , m_RadioQuad);
	DDX_Control(pDX, IDC_TM_THIC_EDIT  , m_EditThik);
	DDX_Control(pDX, IDC_TM_MATL_EDIT  , m_EditMatl);
	DDX_Control(pDX, IDC_TM_DOMAIN_NAME_EDIT  , m_EditDomainName);
	DDX_Control(pDX, IDC_TM_DEL_LINE_ELEM_CHECK  , m_ChkDelLineElem);
	DDX_Control(pDX, IDC_TM_DIV_LINE_ELEM_CHECK  , m_ChkSubDivLineElem);
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CMeshMapDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CMeshMapDlg)
	ON_EN_SETFOCUS(IDC_TM_MESH_NODES_EDIT, OnSetfocusTmNodesEdit)
	ON_BN_CLICKED(IDC_TM_MATL_BUTTON, OnTmMatButton)
	ON_BN_CLICKED(IDC_TM_ELEM_BUTTON, OnElemTypeButton)
	ON_BN_CLICKED(IDC_TM_THIC_BUTTON, OnTmThickButton)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_COMBO, OnSelchangeTmElemTypeCombo)
	ON_BN_CLICKED(IDC_TM_DEL_LINE_ELEM_CHECK, OnDelectLineElem)
	ON_BN_CLICKED(IDC_TM_DOMAIN_BUTTON, OnTmDomainButton)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshMapDlg message handlers

BOOL CMeshMapDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);  

	m_Edit12DirDivision.SetUnitType(D_UNITSYS_NONE);
	m_Edit14DirDivision.SetUnitType(D_UNITSYS_NONE);
	m_Edit12DirBiasRatio.SetUnitType(D_UNITSYS_NONE);
	m_Edit14DirBiasRatio.SetUnitType(D_UNITSYS_NONE);

	InitPropertyCtrls();
	InitElemTypeCombo();
		
	I_GENModelBase::SetModelingModeST(IG_STATE_RESET);

	m_EditNodeList.SetAttNothing();
	m_EditNodeList.SetModeToUse(MOUSEEDIT_USE_NODE_LIST_INPUT);
	m_EditNodeList.SetEscNotifyWindow(this);
	m_EditNodeList.SetEnterNotifyWindow(this);
	m_EditNodeList.SetLButtonDownNotifyWindow(this);
	SetTempElemDataByCurSetting();

	I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();

	InitData();
	OnSetfocusTmNodesEdit();
	OnDelectLineElem();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);
	UpdateData(FALSE);

	SetDomainName();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeshMapDlg::SetDomainName()
{
	CArray<CString,CString&> aStrMadoList;
	m_pDoc->m_pAttrCtrl->GetMadoList(aStrMadoList);
	int nSize = aStrMadoList.GetSize();
	CString strName;
	if(nSize > 0)
	{    
		CAutoIDMaker maker;    
		while (TRUE)
		{
			nSize--;
			if(nSize < 0)
			{
				strName = _T("");
				break;
			}
			maker.SetStartID(aStrMadoList[nSize]);
			strName = maker.GetNextID();
			if(!m_pDoc->m_pAttrCtrl->ExistMado(strName)) break;      
		}
	}
	else
		strName = _T("1");
	
	SetDlgItemText(IDC_TM_DOMAIN_NAME_EDIT, strName);  
}

void CMeshMapDlg::Execute()
{
	if(!(CheckArg()))	return;

	MESH4N_ARGUMENT Arg;
	Arg.d_u_division = m_Edit12DirDivision.GetEditValue();
	Arg.d_v_division = m_Edit14DirDivision.GetEditValue();
	Arg.d_u_bias     = m_Edit12DirBiasRatio.GetEditValue();
	Arg.d_v_bias     = m_Edit14DirBiasRatio.GetEditValue();
	Arg.Pnt[0].xyz[0] = m_MashNodeD1.x; Arg.Pnt[0].xyz[1] = m_MashNodeD1.y; Arg.Pnt[0].xyz[2] = m_MashNodeD1.z; 
	Arg.Pnt[1].xyz[0] = m_MashNodeD2.x; Arg.Pnt[1].xyz[1] = m_MashNodeD2.y; Arg.Pnt[1].xyz[2] = m_MashNodeD2.z; 
	Arg.Pnt[2].xyz[0] = m_MashNodeD3.x; Arg.Pnt[2].xyz[1] = m_MashNodeD3.y; Arg.Pnt[2].xyz[2] = m_MashNodeD3.z; 
	Arg.Pnt[3].xyz[0] = m_MashNodeD4.x; Arg.Pnt[3].xyz[1] = m_MashNodeD4.y; Arg.Pnt[3].xyz[2] = m_MashNodeD4.z; 
	if(m_RadioTri.GetCheck())
	  Arg.n_MeshType = 0;
	else
		Arg.n_MeshType = 1;
	Arg.bHighOrder = FALSE;//m_AdvOption.bHighOrder;
	Arg.nPropK     = 0;//m_nPropertyID;

	CMapMesh_4Point Mesher(Arg);

	if (!Mesher.Perform()) return;

	T_MESH_NODE_D_LIST arMeshNode;
	arMeshNode.RemoveAll();
	T_MESH_ELEM_D_LIST arMeshElem;
	arMeshElem.RemoveAll();
		
	int nNodeSize = Mesher.GetNodeData(arMeshNode);
	int nElemSize = Mesher.GetElemData(arMeshElem);

	m_arNodeK.RemoveAll();
	m_arNodeK.SetSize(nNodeSize);
	m_arNodeD.RemoveAll();
	m_arNodeD.SetSize(nNodeSize);

	//int nDBNodeSize = nNodeSize-m_NodeList.GetSize();

	CArray<T_NODE_K, T_NODE_K> arNodeK; //DB에 입력할 노드
	arNodeK.RemoveAll();
	//arNodeK.SetSize(nDBNodeSize);
	CArray<T_NODE_D, T_NODE_D&> arNodeD; //DB에 입력할 노드
	arNodeD.RemoveAll();
	//arNodeD.SetSize(nDBNodeSize);

	//int nDBNodeCount = 0;

	T_NODE_K NewNodeK = m_pDoc->m_pDataCtrl->GetNodeKey();

	for(int i=0 ; i<nNodeSize ; ++i)
	{
		T_NODE_K NodeK;
		T_NODE_D NodeD;

		T_MESH_NODE_D MeshNoded = arMeshNode[i];
		if(IsSameNode(m_MashNodeD1, MeshNoded))
		{
			NodeK = m_NodeList[0];    
			ConvertOrgNodeData(MeshNoded, NodeD);
		}
		else if(IsSameNode(m_MashNodeD2, MeshNoded))    
		{
			NodeK = m_NodeList[1];   
			ConvertOrgNodeData(MeshNoded, NodeD);
		}
		else if(IsSameNode(m_MashNodeD3, MeshNoded))
		{
			NodeK = m_NodeList[2];
			ConvertOrgNodeData(MeshNoded, NodeD);
		}
		else if(IsSameNode(m_MashNodeD4, MeshNoded))
		{
			NodeK = m_NodeList[3];
			ConvertOrgNodeData(MeshNoded, NodeD);
		}
		else
		{
			ConvertOrgNodeData(MeshNoded, NodeD);
			if(m_pDoc->m_pAttrCtrl->ExistNode(NodeD.x, NodeD.y, NodeD.z, NodeK))
			{
				m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD);
			}
			else
			{
				NodeK = NewNodeK++;        
			}      
		}

		arNodeK.Add(NodeK);
		arNodeD.Add(NodeD);

		m_arNodeK[i] = NodeK;
		m_arNodeD[i] = NodeD;
	}  

	//////////////////////////////////////////////////////////////////////
	//Elem
	CArray<T_ELEM_K, T_ELEM_K> arElemK; //DB에 입력할 요소
	arElemK.RemoveAll(); arElemK.SetSize(nElemSize);
	CArray<T_ELEM_D, T_ELEM_D&> arElemD; //DB에 입력할 요소
	arElemD.RemoveAll(); arElemD.SetSize(nElemSize);

	T_ELEM_D ElemD; ElemD.Initialize();
	ElemD.eltyp = GetCurElemTypeID();
	if(!GetPropIDS(ElemD.eltyp, ElemD.elmat, ElemD.elpro)) return;

	if(m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp))
	{
		if(m_bDOF)
		{
			if(m_nThickThin == 1)
				ElemD.nSubType = 3;
			else
				ElemD.nSubType = 2;
		}
		else
		{
			if(m_nThickThin == 1)
				ElemD.nSubType = 1;
			else
				ElemD.nSubType = 0;
		}
	}
	else if(m_pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp))
	{
		if(m_bDOF)
		{
			ElemD.nSubType = 1;
		}
		else
		{
			ElemD.nSubType = 0;
		}
	}
	else if(m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
	{
		ElemD.nSubType = m_nMembPlate+1;
		ElemD.nWallId = m_nWallID;
	}

	T_ELEM_K NewElemK = m_pDoc->m_pDataCtrl->GetElemKey();
	for(int i=0 ; i<nElemSize ; ++i)
	{
		arElemK[i] = NewElemK++;    

		T_MESH_ELEM_D MeshElemD = arMeshElem[i];
		int nSize = MeshElemD.aNodeK.GetSize();
		for(int n=0 ; n<nSize ; ++n)
		{
			ElemD.elnod[n] = m_arNodeK[MeshElemD.aNodeK[n]];
		}

		arElemD[i] = ElemD;
	}


	///////////////////////////////////////////////////////////////////////
	// Mado
	T_MADO_D MadoD; MadoD.Initialize();
	m_EditDomainName.GetWindowText(MadoD.strName);
	MadoD.eltyp = GetCurElemTypeID();
	MadoD.elmat=0;
	MadoD.elpro=0;
	//if(!GetPropIDS(MadoD.eltyp, MadoD.elmat, MadoD.elpro)) return;
	if(m_pDoc->m_pAttrCtrl->IsPlate(MadoD.eltyp))
	{
		if(m_bDOF)
		{
			if(m_nThickThin == 1)
				MadoD.nSubType = 3;
			else
				MadoD.nSubType = 2;
		}
		else
		{
			if(m_nThickThin == 1)
				MadoD.nSubType = 1;
			else
				MadoD.nSubType = 0;
		}
	}
	else if(m_pDoc->m_pAttrCtrl->IsPlstrs(MadoD.eltyp))
	{
		if(m_bDOF)
		{
			MadoD.nSubType = 1;
		}
		else
		{
			MadoD.nSubType = 0;
		}
	}

	///////////////////////////////////////////////////////////////////////
	// Sbdo
	T_SBDO_D SbdoD; SbdoD.Initialize();
	SbdoD.strName = _T("[1]");
	if(!GetDomainType(SbdoD.nType)) return;

	///////////////////////////////////////////////////////////////////////
	// Del, Div Line Elem
	BOOL bDelLineElem = m_ChkDelLineElem.GetCheck();
	BOOL bSubDivLineElem = m_ChkSubDivLineElem.GetCheck();

	////////////////////////////////////////////////////////////////////////
	if(!m_pDoc->m_pDataCtrl->AddNodeElemMadoSbdo(arNodeK, arNodeD, arElemK, arElemD, 
		MadoD, SbdoD,    
		m_NodeList, bDelLineElem, bSubDivLineElem) ) 
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_FAILED_CREATE_MESH));    
	}

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(!pIGM) {ASSERT(0); return;}
	
	pIGM->ResetTempElem();
	pIGM->SetModelingModeST(IG_STATE_INITALL);
	pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
	m_EditNodeList.ImAssocWindowST();
	
	IUsageCounter::Use(_T("MESHM"));
}

//노드들의 Z 좌표를 비교하여 모두 같으면 Slab, 하나라도 다르면 Wall Type으로 정의
BOOL CMeshMapDlg::GetDomainType(int& nType)
{
	int nNodeKSize = m_NodeList.GetSize();
	if(nNodeKSize > 3)
	{
		T_NODE_K NodeK1 = 0;
		T_NODE_K NodeK2 = 0;
		T_NODE_K NodeK3 = 0;
		T_NODE_D NodeD1; NodeD1.Initialize();
		T_NODE_D NodeD2; NodeD2.Initialize();
		T_NODE_D NodeD3; NodeD3.Initialize();
		NodeK1 = m_NodeList[0];
		NodeK2 = m_NodeList[1];
		NodeK3 = m_NodeList[2];
		if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeD1)) return FALSE;
		if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeD2)) return FALSE;
		if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK3, NodeD3)) return FALSE;

		double xyz1[3]; xyz1[0]=NodeD1.x; xyz1[1]=NodeD1.y; xyz1[2]=NodeD1.z;
		double xyz2[3]; xyz2[0]=NodeD2.x; xyz2[1]=NodeD2.y; xyz2[2]=NodeD2.z;
		double xyz3[3]; xyz3[0]=NodeD3.x; xyz3[1]=NodeD3.y; xyz3[2]=NodeD3.z;
		double dTempVector[3];
		CMathFunc::mathCross_product_3d(xyz1, xyz2, xyz3, dTempVector);
		double dNormalizeVector[3];
		CMathFunc::mathNormalize(dTempVector, dNormalizeVector);
		double Tol = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit()*10;
		if(fabs(dNormalizeVector[2]-0) < Tol)
			nType = 3;//Wall
		else
			nType = 1;//Slab

		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMeshMapDlg::IsSameNode(const T_MESH_NODE_D& OrgNodeD, const T_MESH_NODE_D& NodeD)
{
	if(OrgNodeD.x==NodeD.x && OrgNodeD.y==NodeD.y && OrgNodeD.z==NodeD.z)
		return TRUE;
	else
		return FALSE;  
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CMeshMapDlg::GetPropIDS(int nElemType, int& nMatID, int& nThkID)
{
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	CString strT, strM;
	m_EditThik.GetWindowText(strT);
	m_EditMatl.GetWindowText(strM);
	
	if (pDoc->m_pAttrCtrl->IsNeedMatl(nElemType))
		GetINumber(strM,nMatID);
	else nMatID = 0;  

	if (pDoc->m_pAttrCtrl->IsNeedThik(nElemType))
		GetINumber(strT,nThkID);
	else nThkID = 0;

	return TRUE;
}



void CMeshMapDlg::InitData()
{
	m_Edit12DirDivision.SetEditUnit(2);
	m_Edit14DirDivision.SetEditUnit(2);
	m_Edit12DirBiasRatio.SetEditUnit(1);
	m_Edit14DirBiasRatio.SetEditUnit(1);
	m_RadioQuad.SetCheck(TRUE);
	
	m_ChkSubDivLineElem.SetCheck(TRUE);
}

void CMeshMapDlg::InitPropertyCtrls()
{
	m_CobxMatl.SetEditBoxPoint(&m_EditMatl);
	m_CobxThik.SetEditBoxPoint(&m_EditThik);

	m_EditMatl.SetComboBoxPoint(&m_CobxMatl);	
	m_EditThik.SetComboBoxPoint(&m_CobxThik);

	if (!m_bPropInit)
	{
		m_EditMatl.SetEditBoxText(1);
		m_EditThik.SetEditBoxText(1);
		m_bPropInit = TRUE;
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_CobxMatl);
	CDlgUtil::CobxAdjustListBoxWidth(m_CobxThik);
}

void CMeshMapDlg::InitElemTypeCombo()
{
	m_ElemTypeIDs.RemoveAll();
	
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	// 추가할 Element Type을 Element Type Array에 설정한다. 
	// Array에 Add한 순서대로 List에 추가된다. Item Data는 Element 
	// Type으로 설정된다. 	
#ifndef _TOWER	
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetPlateID());			//plate	       
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetPlstrsID());		//plane	stress 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetPlstrnID());		//plane	strain 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetAxisymID());		//axisymmetric 	
	/*#ifndef _CIVIL
	  m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetWallID());			//wall         
	#endif*/
#endif

	int nETypes = m_ElemTypeIDs.GetSize();
	for(int i = 0 ; i < nETypes ; i++)
	{
		// m_ComboElemType은 Sort Item Option을 반드시 제거할것...
		//m_ComboElemType.AddString(pDoc->m_pAttrCtrl->GetEltypName(m_ElemTypeIDs[i]));    
		m_ComboElemType.AddString(CDBLib::GetEltypName(m_ElemTypeIDs[i]));    
		m_ComboElemType.SetItemData(i,(DWORD)m_ElemTypeIDs[i]);
	}

	ChangeElemTypeComboWidth();

	m_ComboElemType.SetCurSel(m_nCurETypeIndex);
	
	if(m_nCurETypeIndex != -1)
	{
		int nEType;
		nEType = (int)m_ComboElemType.GetItemData(m_nCurETypeIndex);  
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_ComboElemType);
}


int  CMeshMapDlg::GetCurElemTypeID()
{
	m_nCurETypeIndex = m_ComboElemType.GetCurSel();  
	return m_ElemTypeIDs[m_nCurETypeIndex];
}

void CMeshMapDlg::OnSelchangeTmElemTypeCombo() 
{
	int nElemType    = GetCurElemTypeID();
	if(m_pDoc->m_pAttrCtrl->IsPlstrn(nElemType) || m_pDoc->m_pAttrCtrl->IsAxisym(nElemType))
	{
		GetDlgItem(IDC_TM_MESH_MAP_THICK_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_THIC_EDIT         )->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_THIC_COMBO        )->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_THIC_BUTTON       )->EnableWindow(FALSE);
		
	}
	else
	{
		GetDlgItem(IDC_TM_MESH_MAP_THICK_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_THIC_EDIT         )->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_THIC_COMBO        )->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_THIC_BUTTON       )->EnableWindow(TRUE);
	}

	//Wall Type일 경우 Triangle Mesh 는 허용이 안된다
	if(m_pDoc->m_pAttrCtrl->IsWall(nElemType))
	{
		m_RadioTri.SetCheck(FALSE);
		m_RadioTri.EnableWindow(FALSE);
		m_RadioQuad.SetCheck(TRUE);
	}
	else
		m_RadioTri.EnableWindow(TRUE);

	if(m_pDoc->m_pAttrCtrl->IsPlate(nElemType) ||
		m_pDoc->m_pAttrCtrl->IsPlstrs(nElemType) ||
		m_pDoc->m_pAttrCtrl->IsWall(nElemType) )
		GetDlgItem(IDC_TM_ELEM_BUTTON)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_TM_ELEM_BUTTON)->EnableWindow(FALSE);
}

void CMeshMapDlg::OnElemTypeButton() 
{
	int nElemType    = GetCurElemTypeID();
	if(m_pDoc->m_pAttrCtrl->IsPlate(nElemType))
	{
		CMeshElemSubTypePlateDlg dlg;
		dlg.SetData(m_nThickThin, m_bDOF);
		if(IDOK == dlg.DoModal())
		{
			dlg.GetData(m_nThickThin, m_bDOF);
		}
	}
	else if(m_pDoc->m_pAttrCtrl->IsPlstrs(nElemType))
	{
		CMeshElemSubTypePlstDlg dlg;
		dlg.SetData(m_bDOF);
		if(IDOK == dlg.DoModal())
		{
			dlg.GetData(m_bDOF);
		}
	}
	else if(m_pDoc->m_pAttrCtrl->IsWall(nElemType))
	{
		CMeshElemSubTypeWallDlg dlg;
		dlg.SetData(m_nMembPlate, m_nWallIDType, m_nWallID);
		if(IDOK == dlg.DoModal())
		{
			dlg.GetData(m_nMembPlate, m_nWallIDType, m_nWallID);
		}
	}
	else
		ASSERT(0);
}

void CMeshMapDlg::OnTmMatButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));
}

void CMeshMapDlg::OnTmThickButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_THICKNESS,0));
}

BOOL CMeshMapDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;  
	return TRUE;
}

void CMeshMapDlg::SetTempElemDataByCurSetting()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(pIGM)
	{
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
		pIGM->SetTempElemData(GetCurElemTypeID(), /*MaxNodeNum*/4);
		pIGM->SetTempElemNodeList(NList, GetCurElemTypeID(), /*MaxNodeNum*/4, pIGM);
	}
}

void CMeshMapDlg::OnSetfocusTmNodesEdit() 
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	//pIGM->SetTempElemErrorFlag(TRUE);
	//pIGM->CursorClearHide();
	//pIGM->ResetTempElem();
	//pIGM->SetTempElemErrorFlag(FALSE);
	if(pIGM)
	{   
		SetTempElemDataByCurSetting();  
		pIGM->SetTempElemFirstDrawFlag();
		//pIGM->SetTempElemErrorFlag(TRUE);
		//pIGM->SetTempElemErrorFlag(FALSE);
		
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
		
		if(NList.GetSize() != 0)
		{/*
			m_EditNodeList.SetWindowText(_T(""));
			pIGM->ResetTempElem();
			I_GENModelBase::SetModelingModeST(IG_STATE_ELEM_ADD);*/
		}
		
		pIGM->SetTempElemErrorFlag(TRUE);
		pIGM->CursorClearHide();
		pIGM->CancelAllTempElem();    
		m_EditNodeList.SetWindowText(_T(""));    
		//m_pDoc->UpdateAllViews(NULL, D_UPDATE_BUFFER_AFTER, NULL);     
	}

}

/*********************************************************************
	 *
	 *
 *****  Model View에서의 Mouse Event Notify를 받는다. 
	***
	 */
LRESULT CMeshMapDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_EditNodeList.GetSafeHwnd() == (HWND)lParam && 
		 pIGM->GetModelingModeST() == IG_STATE_ELEM_ADD)
	{
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);

		CString str = _T("");
		CString Tstr;

		int nCount = NList.GetSize();
		if(nCount == 1)
			m_EditNodeList.SetWindowText(_T(""));
		
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
		 m_EditNodeList.SetWindowText(str);
		 
		 /*if(pIGM->IsTempNodeComplete())
		 {
			 //Element Creation !!!
			 //_CreateElement();
			 //I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
			 //pIGM->SetTempElemData(nElemType,nMaxNodeNum);
			 
			 //m_wndNodeListEdit.SetFocus();
			 pIGM->ResetTempElem();
			 pIGM->SetModelingModeST(IG_STATE_INITALL);
			 pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
			 m_EditNodeList.ImAssocWindowST();
			 //m_wndNodeListEdit.SetFocus();
		 }*/
		}
	}
	//I_GENModelBase::SetModelingModeST(IG_STATE_ELEM_ADD);
	return 0L;
}

LRESULT CMeshMapDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_EditNodeList.GetSafeHwnd() == (HWND)lParam)
	{
		//_CreateElement(); 
		m_EditNodeList.SetWindowText(_T(""));
	}
	return 0L;
}

LRESULT CMeshMapDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
{
	if(m_EditNodeList.GetSafeHwnd() == (HWND)lParam)
	{
		
		//SetTempElemDataByCurSetting();
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(!pIGM) return 0L;
		
		m_EditNodeList.SetWindowText(_T(""));
		//pIGM->ResetTempElem();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);

		//SetTempElemDataByCurSetting();
		//pIGM->HideTempElem
		//pIGM->EndTempElem(
		//I_GENModelBase::SetModelingModeST(IG_STATE_INITALL); 
	}
	return 0L;
}

//=====================================================================
// CheckArg
//=====================================================================
BOOL CMeshMapDlg::CheckArg(const BOOL bSimul) 
{
	CString errorMsg;

	// Checking Invalid Node # & auguments
	CString strNodes;
	m_EditNodeList.GetWindowText(strNodes);
	m_NodeList.RemoveAll();
	GetNodeList(strNodes,m_NodeList);
	if(m_NodeList.GetSize() != 4) return FALSE;

	// [2010-04-13] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
	// Mesh가 한평면인지 검사하는 Tol를 1cm로 수정
	double Tol = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit()*10;
	BOOL bPlane = TRUE;
	double dPlanePoint1[3];
	double dPlanePoint2[3];
	double dPlanePoint3[3];
	T_NODE_D Node1; Node1.Initialize();
	T_NODE_D Node2; Node2.Initialize();
	T_NODE_D Node3; Node3.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[0], Node1)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[1], Node2)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[2], Node3)) return FALSE;
	dPlanePoint1[0] = Node1.x;  dPlanePoint1[1] = Node1.y;  dPlanePoint1[2] = Node1.z;  
	dPlanePoint2[0] = Node2.x;  dPlanePoint2[1] = Node2.y;  dPlanePoint2[2] = Node2.z;  
	dPlanePoint3[0] = Node3.x;  dPlanePoint3[1] = Node3.y;  dPlanePoint3[2] = Node3.z;
	
	int nNodeSize = m_NodeList.GetSize();
	for(int i=3 ; i<nNodeSize ; ++i)
	{   
		double dPoint[3];
		T_NODE_D Node; Node.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[i], Node)) return FALSE;
		dPoint[0] = Node.x;  dPoint[1] = Node.y;  dPoint[2] = Node.z;
		if(!CMathFunc::mathIncludePointInPlane(dPlanePoint1, dPlanePoint2, dPlanePoint3, dPoint, Tol))
		{
			bPlane = FALSE;
			break;
		}
	}
	if(!bPlane)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_co_planar_node));
		return FALSE;
	}

	if(m_Edit12DirDivision.GetEditValue() < 1 || m_Edit14DirDivision.GetEditValue() < 1)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_ERR_MESH_SIZE_DIVIDE));
		return FALSE;
	}
	if(m_Edit12DirBiasRatio.GetEditValue() <= 0 || m_Edit14DirBiasRatio.GetEditValue() <= 0) 
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_ERR_MESH_BIAS_RATIO));
		return FALSE;
	}

	// Checking Duplication of Nodes
	if( m_NodeList[0]==m_NodeList[1] || m_NodeList[0]==m_NodeList[2] || m_NodeList[0]==m_NodeList[3] ||
			m_NodeList[1]==m_NodeList[2] || m_NodeList[1]==m_NodeList[3] ||
			m_NodeList[2]==m_NodeList[3])
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));    
		return FALSE;
	}
	
	// Making Node Data
	if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[0], m_NodeD1)) return FALSE;
	ConvertMashNodeData(m_NodeD1, m_MashNodeD1);
	if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[1], m_NodeD2)) return FALSE;
	ConvertMashNodeData(m_NodeD2, m_MashNodeD2);
	if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[2], m_NodeD3)) return FALSE;
	ConvertMashNodeData(m_NodeD3, m_MashNodeD3);
	if(!m_pDoc->m_pAttrCtrl->GetNode(m_NodeList[3], m_NodeD4)) return FALSE;
	ConvertMashNodeData(m_NodeD4, m_MashNodeD4);
	
	// Checking Alignment
	if(!CheckAlign())
	{
		AfxMessageBox(_LS(IDS_TM_ERR_ALIGNEDED_NODE));
		return FALSE;
	}

	// Checking Nodal Crossity
	if(!CheckCross()) 
	{
		AfxMessageBox(_LS(IDS_TM_ERR_CROSS_NODE));
		return FALSE;
	}

	// Domain Name Check
	CString strName=_T("");
	m_EditDomainName.GetWindowText(strName);
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.GetLength() == 0)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_Domain_name_));
		return FALSE;
	}
	
	return TRUE;
}

void CMeshMapDlg::ConvertMashNodeData(T_NODE_D& OrgNodeD, T_MESH_NODE_D& MeshNodeD)
{
	MeshNodeD.Initialize();
	MeshNodeD.x = OrgNodeD.x;
	MeshNodeD.y = OrgNodeD.y;
	MeshNodeD.z = OrgNodeD.z;
}
void CMeshMapDlg::ConvertOrgNodeData(T_MESH_NODE_D& MeshNodeD, T_NODE_D& OrgNodeD)
{
	OrgNodeD.Initialize();
	OrgNodeD.x = MeshNodeD.x;
	OrgNodeD.y = MeshNodeD.y;
	OrgNodeD.z = MeshNodeD.z;
}

//=====================================================================
// CheckAlign
//=====================================================================
BOOL CMeshMapDlg::CheckAlign()
{
	double Vec1To2[3], Vec2To3[3], Vec3To4[3], Vec4To1[3];

	Vec1To2[0] = m_MashNodeD2.x - m_MashNodeD1.x;
	Vec1To2[1] = m_MashNodeD2.y - m_MashNodeD1.y;
	Vec1To2[2] = m_MashNodeD2.z - m_MashNodeD1.z;
	CMathFunc::mathNormalize(Vec1To2, Vec1To2);

	Vec2To3[0] = m_MashNodeD3.x - m_MashNodeD2.x;
	Vec2To3[1] = m_MashNodeD3.y - m_MashNodeD2.y;
	Vec2To3[2] = m_MashNodeD3.z - m_MashNodeD2.z;
	CMathFunc::mathNormalize(Vec2To3, Vec2To3);

	Vec3To4[0] = m_MashNodeD4.x - m_MashNodeD3.x;
	Vec3To4[1] = m_MashNodeD4.y - m_MashNodeD3.y;
	Vec3To4[2] = m_MashNodeD4.z - m_MashNodeD3.z;
	CMathFunc::mathNormalize(Vec3To4, Vec3To4);

	Vec4To1[0] = m_MashNodeD1.x - m_MashNodeD4.x;
	Vec4To1[1] = m_MashNodeD1.y - m_MashNodeD4.y;
	Vec4To1[2] = m_MashNodeD1.z - m_MashNodeD4.z;
	CMathFunc::mathNormalize(Vec4To1, Vec4To1);

	double dAng = CMathFunc::mathCrossAngle(Vec1To2, Vec2To3);
	if(abs(abs(dAng) - 180) < 1.0e-3 || abs(dAng) < 1.0e-3)
		return FALSE;

	dAng = CMathFunc::mathCrossAngle(Vec2To3, Vec3To4);
	if(abs(abs(dAng) - 180) < 1.0e-3 || abs(dAng) < 1.0e-3)
		return FALSE;

	dAng = CMathFunc::mathCrossAngle(Vec3To4, Vec4To1);
	if(abs(abs(dAng) - 180) < 1.0e-3 || abs(dAng) < 1.0e-3)
		return FALSE;

	dAng = CMathFunc::mathCrossAngle(Vec4To1, Vec1To2);
	if(abs(abs(dAng) - 180) < 1.0e-3 || abs(dAng) < 1.0e-3)
		return FALSE;

	return TRUE;

	// MQC 23971
	// 4개의 선 중 인접한 2개가 선상에 잇는지를 체크하는 에러 조건인 것 같음
	// 아래 코드로 돌렸을 때 3D 상에서 계산이 잘못되서 에러가 아닌데 에러로 판단 하는 상황이 있어서 각도로 체크하도록 수정 함
	/*
	double dx, dy, dz;
	double dX, dY, dZ;
	double Inner, absA, absB;

//1 2 3 체크
	dX = m_MashNodeD1.x - m_MashNodeD2.x;
	dY = m_MashNodeD1.y - m_MashNodeD2.y;
	dZ = m_MashNodeD1.z - m_MashNodeD2.z;

	dx = m_MashNodeD1.x - m_MashNodeD3.x;
	dy = m_MashNodeD1.y - m_MashNodeD3.y;
	dz = m_MashNodeD1.z - m_MashNodeD3.z;

	Inner = (dx*dX + dy*dY + dz*dZ)*(dx*dX + dy*dY + dz*dZ);
	absA = dx*dx + dy*dy + dz*dz;
	absB = dX*dX + dY*dY + dZ*dZ;
	
	if ((Inner/absA/absB-1)*(Inner/absA/absB-1) < 1.0e-7)
		return FALSE;

//2 3 4 체크
	dX = m_MashNodeD2.x - m_MashNodeD3.x;
	dY = m_MashNodeD2.y - m_MashNodeD3.y;
	dZ = m_MashNodeD2.z - m_MashNodeD3.z;

	dx = m_MashNodeD2.x - m_MashNodeD4.x;
	dy = m_MashNodeD2.y - m_MashNodeD4.y;
	dz = m_MashNodeD2.z - m_MashNodeD4.z;

	Inner = (dx*dX + dy*dY + dz*dZ)*(dx*dX + dy*dY + dz*dZ);
	absA = dx*dx + dy*dy + dz*dz;
	absB = dX*dX + dY*dY + dZ*dZ;
	
	if ((Inner/absA/absB-1)*(Inner/absA/absB-1) < 1.0e-7)
		return FALSE;

//1 3 4 체크
	dX = m_MashNodeD1.x - m_MashNodeD3.x;
	dY = m_MashNodeD1.y - m_MashNodeD3.y;
	dZ = m_MashNodeD1.z - m_MashNodeD3.z;

	dx = m_MashNodeD1.x - m_MashNodeD4.x;
	dy = m_MashNodeD1.y - m_MashNodeD4.y;
	dz = m_MashNodeD1.z - m_MashNodeD4.z;

	Inner = (dx*dX + dy*dY + dz*dZ)*(dx*dX + dy*dY + dz*dZ);
	absA = dx*dx + dy*dy + dz*dz;
	absB = dX*dX + dY*dY + dZ*dZ;
	
	if ((Inner/absA/absB-1)*(Inner/absA/absB-1) < 1.0e-7)
		return FALSE;

//1 2 4 체크
	dX = m_MashNodeD1.x - m_MashNodeD2.x;
	dY = m_MashNodeD1.y - m_MashNodeD2.y;
	dZ = m_MashNodeD1.z - m_MashNodeD2.z;

	dx = m_MashNodeD1.x - m_MashNodeD4.x;
	dy = m_MashNodeD1.y - m_MashNodeD4.y;
	dz = m_MashNodeD1.z - m_MashNodeD4.z;

	Inner = (dx*dX + dy*dY + dz*dZ)*(dx*dX + dy*dY + dz*dZ);
	absA = dx*dx + dy*dy + dz*dz;
	absB = dX*dX + dY*dY + dZ*dZ;
	
	if ((Inner/absA/absB-1)*(Inner/absA/absB-1) < 1.0e-7)
		return FALSE;
		*/
	return TRUE;
}

//=====================================================================
// CheckCross
//=====================================================================
BOOL CMeshMapDlg::CheckCross()
{
	/*gp_Pnt Point1(m_NodeD1.x, m_NodeD1.y, m_NodeD1.z);
	gp_Pnt Point2(m_NodeD2.x, m_NodeD2.y, m_NodeD2.z);
	gp_Pnt Point3(m_NodeD3.x, m_NodeD3.y, m_NodeD3.z);
	gp_Pnt Point4(m_NodeD4.x, m_NodeD4.y, m_NodeD4.z);

	gp_Vec Vec12(Point1, Point2);
	gp_Vec Vec23(Point2, Point3);
	gp_Vec Vec34(Point3, Point4);
	gp_Vec Vec41(Point4, Point1);

	gp_Vec Norm[4];

	Norm[0] = Vec12 ^ Vec23 ;
	Norm[1] = Vec23 ^ Vec34 ;
	Norm[2] = Vec34 ^ Vec41 ;
	Norm[3] = Vec41 ^ Vec12 ;

	for (int i=0 ; i<4 ; i++)
	{
		if ( i!= 3 )
		{
			if ( Norm[i].Angle(Norm[i+1]) > M_PI_2*4.0/3.0 )
				return FALSE;
		}
		else if ( i == 3 )
		{
			if (Norm[i].Angle(Norm[0]) > M_PI_2*4.0/3.0  )
				return FALSE;
		}
	}*/

	return TRUE;
}

void CMeshMapDlg::OnDelectLineElem() 
{
	if(m_ChkDelLineElem.GetCheck())
	{
		GetDlgItem(IDC_TM_DIV_LINE_ELEM_CHECK)->EnableWindow(FALSE);    
	}
	else
	{
		GetDlgItem(IDC_TM_DIV_LINE_ELEM_CHECK)->EnableWindow(TRUE);    
	}
}

void CMeshMapDlg::OnTmDomainButton() 
{
	CreateOrActivateCMD2Dlg((CDBDoc*)m_pDoc, CMainDomainDlg::IDD);
}

void CMeshMapDlg::ChangeElemTypeComboWidth()
{
	int nCount = m_ComboElemType.GetCount();
	if(nCount<1) return;
	
	CString data = _T("");
	m_ComboElemType.GetLBText(0, data);
	
	CDC* pDC = GetDC();  
	CSize max, cur;
	max = pDC->GetTextExtent(data);
	m_ComboElemType.SetDroppedWidth(max.cx);
	
	for(int i=1; i<nCount; i++)
	{
		m_ComboElemType.GetLBText(i, data);
		cur = pDC->GetTextExtent(data); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if(max.cx > m_ComboElemType.GetDroppedWidth()) m_ComboElemType.SetDroppedWidth(max.cx);
	
	ReleaseDC(pDC);
}

void CMeshMapDlg::UpdateBuffer()
{  
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
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
		case UR_MADO_ADD: case UR_MADO_DEL:
			SetDomainName();
		default:
			break;
		}
	}
}

void CMeshMapDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_UNIT:   
		break;
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:    
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}  
}