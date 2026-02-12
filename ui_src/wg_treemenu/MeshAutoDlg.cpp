// MeshAutoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MeshAutoDlg.h"
#include "MeshElemSubTypePlateDlg.h"
#include "MeshElemSubTypePlstDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\MathLib.h"
#include "..\wg_db\qsort.h"
#include "..\wg_db\AlgoFindConner.h"
#include "..\wg_db\BaseMeshTool.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AutoIDMaker.h"
#include "..\wg_db\ViewBuff.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\IUsageCounter.h"

#include "..\wg_base\wg_base_Compfunc.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_strParser.h"
#include "..\wg_base\Profiler.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd2\wg_cmd2_DomainDlgs.h"
#include "..\wg_cmd2\ExportFuncCMD2.h"

#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\arrayUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshAutoDlg dialog

int CMeshAutoDlg::m_nCurETypeIndex = 0;

CMeshAutoDlg::CMeshAutoDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CMeshAutoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeshAutoDlg)
	m_bInnerDomain      = FALSE;
	m_bInteriorNode     = TRUE ;
	m_nOption_Node      = 0    ;
	m_bInteriorLine     = TRUE ;
	m_nOption_Line      = 0    ;
	m_bBndrConnect      = TRUE ;
	m_nOption_Size      = 0    ;
	m_bDeleteSrcLine    = FALSE;
	m_bSubdivideSrcLine = TRUE ;
	m_bPropInit         = FALSE;  
	//}}AFX_DATA_INIT
	m_aKeyList.clear();
	m_mapNodeKeyList.InitHashTable(1000003);
	m_mapElemKeyList.InitHashTable(1000003);
	m_aInteriorNode.RemoveAll();
	m_aInteriorElem.RemoveAll();
	m_mapSeedElemK.RemoveAll();
	m_mapSeedElemK.InitHashTable(1000003);
	m_nThickThin = 0;
	m_bDOF       = TRUE;
	m_nMembPlate = 0;  
	m_bChangeMeshSize = FALSE;
	m_dMinMeshSize=0;
	m_nChangeMethod=0;
	m_dMaxBoundarySize=-DBL_MAX; 
	m_bStartNodeInput = FALSE;
	m_bShowMsg        = FALSE;

	m_CcpaK.RemoveAll();
	m_DpaaK.RemoveAll();
	m_MasterNodeK.RemoveAll();

	m_trang = 180. / (4.0*atan(1.0));
	m_NormalZero = 1.e-15;
}

CMeshAutoDlg::~CMeshAutoDlg()
{  
}  

void CMeshAutoDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshAutoDlg)
	DDX_Radio(pDX, IDC_TM_MESH_AUTO_NODE_AUTO_RADIO  , m_nOption_Node );
	DDX_Radio(pDX, IDC_TM_MESH_AUTO_LINE_AUTO_RADIO  , m_nOption_Line );
	DDX_Radio(pDX, IDC_TM_MESH_AUTO_SIZE_LENGTH_RADIO, m_nOption_Size );
	
	DDX_Check(pDX, IDC_TM_MESH_AUTO_INNER_DOMAIN_CHK         , m_bInnerDomain     );
	DDX_Check(pDX, IDC_TM_MESH_AUTO_INTERIOR_NODE_CHK        , m_bInteriorNode    );
	DDX_Check(pDX, IDC_TM_MESH_AUTO_INTERIOR_LINE_CHK        , m_bInteriorLine    );
	DDX_Check(pDX, IDC_TM_MESH_AUTO_BOUNDARY_CONNECTIVITY_CHK, m_bBndrConnect     );
	DDX_Check(pDX, IDC_TM_MESH_AUTO_DELETE_SRC_CHK           , m_bDeleteSrcLine   );
	DDX_Check(pDX, IDC_TM_MESH_AUTO_SUBDIVIDE_SRC_CHK        , m_bSubdivideSrcLine);
		
	DDX_Control(pDX, IDC_TM_MESH_AUTO_SELECT_NODE_EDT, m_wndSelectNodeListEdit);
	DDX_Control(pDX, IDC_TM_MESH_AUTO_SELECT_ELEM_EDT, m_wndSelectElemListEdit);
	DDX_Control(pDX, IDC_TM_MESH_AUTO_NODE_EDT     , m_wndNodeListEdit    );
	DDX_Control(pDX, IDC_TM_MESH_AUTO_ELEM_EDT     , m_wndElemListEdit    );
	DDX_Control(pDX, IDC_TM_MESH_AUTO_SIZE_EDT     , m_edtMeshSize        );
	DDX_Control(pDX, IDC_TM_MESH_AUTO_SIZE_STC     , m_untMeshSize        );

	DDX_Control(pDX, IDC_TM_ELEM_TYPE_COMBO , m_ctrlCobxEType);
	DDX_Control(pDX, IDC_TM_THICK_NAME_COMBO, m_ctrlCobxThik );
	DDX_Control(pDX, IDC_TM_MAT_NAME_COMBO  , m_ctrlCobxMatl );
	DDX_Control(pDX, IDC_TM_THICK_NO_EDIT   , m_ctrlEditThik );
	DDX_Control(pDX, IDC_TM_MAT_NO_EDIT     , m_ctrlEditMatl );  

	DDX_Control(pDX, IDC_TM_MESH_AUTO_METHOD_COMBO, m_cmbMethod);
	DDX_Control(pDX, IDC_TM_MESH_AUTO_TYPE_COMBO  , m_cmbType  );  
	DDX_Control(pDX, IDC_TM_DIR_PT1_EDIT, m_wndDirPt1);
	//}}AFX_DATA_MAP	  
}

BEGIN_MESSAGE_MAP(CMeshAutoDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CMeshAutoDlg)
	ON_EN_SETFOCUS(IDC_TM_MESH_AUTO_SELECT_NODE_EDT, OnSetfocusTmSelectNodeEdit)
	ON_EN_SETFOCUS(IDC_TM_MESH_AUTO_ELEM_EDT, OnSetfocusTmElemEdit)
	ON_EN_SETFOCUS(IDC_TM_MESH_AUTO_NODE_EDT, OnSetfocusTmNodeEdit)	
	ON_BN_CLICKED(IDC_TM_ELEM_BUTTON, OnElemTypeButton)	
	ON_BN_CLICKED(IDC_TM_MATL_BUTTON, OnTmMatButton)
	ON_BN_CLICKED(IDC_TM_THIC_BUTTON, OnTmThickButton)
	ON_BN_CLICKED(IDC_TM_DOMAIN_BUTTON, OnTmDomainButton)	
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE_COMBO, OnSelchangeTmElemTypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_MESH_AUTO_METHOD_COMBO, OnSelchangeMethodCombo)
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_INTERIOR_NODE_CHK, OnClickInteriorNodeCheck)
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_NODE_AUTO_RADIO, OnClickInteriorNodeRadio)
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_NODE_USER_RADIO, OnClickInteriorNodeRadio)  
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_LINE_AUTO_RADIO, OnClickInteriorLineRadio)
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_LINE_USER_RADIO, OnClickInteriorLineRadio)    
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_INTERIOR_LINE_CHK, OnClickInteriorLineCheck)	
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_SIZE_LENGTH_RADIO, OnClickMeshSizeRadio)
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_SIZE_DIVISION_RADIO, OnClickMeshSizeRadio)
	ON_BN_CLICKED(IDC_TM_MESH_AUTO_DELETE_SRC_CHK, OnClickDeleteSrcCheck)  
	//}}AFX_MSG_MAP  
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshAutoDlg message handlers

BOOL CMeshAutoDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);  

#ifndef _MGEN_CH
	GetDlgItem(IDC_TM_DIRECTION_FRAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_TM_DIR_PT1_EDIT)->EnableWindow(FALSE);
#else
	GetDlgItem(IDC_TM_DIRECTION_FRAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_TM_DIR_PT1_EDIT)->EnableWindow(TRUE);
#endif
	// MQC 11863 : Include Interior Lines 체크해도 누락되는 요소 발생
	// m_dTol은 좀 큰 감이 있어서 수정한 함수에서는 따로 Tolerance 사용(m_dTol * 1.e-4)
	m_dLengthTol = 1.e-7 / m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	
	InitPropertyCtrls();
	InitElemTypeCombo();

	I_GENModelBase::SetModelingModeST(IG_STATE_RESET);

	m_wndSelectNodeListEdit.SetAttNothing();
	m_wndSelectNodeListEdit.SetModeToUse(MOUSEEDIT_USE_NODE_LIST_INPUT);
	m_wndSelectNodeListEdit.SetEscNotifyWindow(this);
	m_wndSelectNodeListEdit.SetEnterNotifyWindow(this);
	m_wndSelectNodeListEdit.SetLButtonDownNotifyWindow(this);
	SetTempElemDataByCurSetting();

	I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();

	m_wndSelectElemListEdit.Connect(SC_ID_ELEM, &m_wndSelectElemListEdit);

	m_cmbMethod.AddString(_LS(IDS_TM_Nodes)          );
	m_cmbMethod.AddString(_LS(IDS_TM_Line_Elements)  );
	m_cmbMethod.AddString(_LS(IDS_TM_Planar_Elements));
	m_cmbMethod.SetCurSel(1);
	ChangeMethod();
	
	m_wndNodeListEdit.SetAttNothing();
	m_wndNodeListEdit.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndNodeListEdit.SetEscNotifyWindow(this);
	m_wndNodeListEdit.SetEnterNotifyWindow(this);
	m_wndNodeListEdit.SetLButtonDownNotifyWindow(this);

	m_wndElemListEdit.SetAttNothing();
	m_wndElemListEdit.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndElemListEdit.SetEscNotifyWindow(this);
	m_wndElemListEdit.SetEnterNotifyWindow(this);
	m_wndElemListEdit.SetLButtonDownNotifyWindow(this);

	m_cmbType.AddString(_LS(IDS_TM_Quadrilateral));
	m_cmbType.AddString(_LS(IDS_TM_Quad_Triangle));
	m_cmbType.AddString(_LS(IDS_TM_Triangle)     );
	m_cmbType.SetCurSel(0);
	
	EnableInterierNode(m_bInteriorNode);
	EnableInterierLine(m_bInteriorLine);
	
	OnClickMeshSizeRadio();    
		
	double dMinSize = 1000.0;
	dMinSize = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dMinSize);
	m_edtMeshSize.SetEditUnit(dMinSize);
	UpdateData(FALSE);

	GotoDlgCtrl(&m_wndSelectElemListEdit);

	m_dTol = m_pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit();
	// [2010-04-13] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
	// Mesh가 한평면인지 검사하는 Tol를 1cm로 수정    
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	m_dPlaneTol = CDBLib::Get_SlabSamePlaneTol(CurIndex.nBase_Length); 

	SetDomainName();

	m_wndDirPt1.SetAttUcsDistance();
	m_wndDirPt1.SetModeToUse(MOUSEEDIT_USE_SET_DIST);

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeshAutoDlg::SetDomainName()
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

void CMeshAutoDlg::InitPropertyCtrls()
{
	m_ctrlCobxMatl.SetEditBoxPoint(&m_ctrlEditMatl);
	m_ctrlCobxThik.SetEditBoxPoint(&m_ctrlEditThik);
	
	m_ctrlEditMatl.SetComboBoxPoint(&m_ctrlCobxMatl);
	m_ctrlEditThik.SetComboBoxPoint(&m_ctrlCobxThik);
	
	if (!m_bPropInit)
	{
		m_ctrlEditMatl.SetEditBoxText(1);
		m_ctrlEditThik.SetEditBoxText(1);
		m_bPropInit = TRUE;
	}
}

void CMeshAutoDlg::InitElemTypeCombo()
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
#endif
	
	int nETypes = m_ElemTypeIDs.GetSize();
	for(int i=0; i<nETypes; i++)
	{
		// m_ComboElemType은 Sort Item Option을 반드시 제거할것...
		m_ctrlCobxEType.AddString(CDBLib::GetEltypName(m_ElemTypeIDs[i]));    
		m_ctrlCobxEType.SetItemData(i,(DWORD)m_ElemTypeIDs[i]);
	}
	
	ChangeElemTypeComboWidth();

	m_ctrlCobxEType.SetCurSel(m_nCurETypeIndex);
	
	if(m_nCurETypeIndex != -1)
	{
		int nEType;
		nEType = (int)m_ctrlCobxEType.GetItemData(m_nCurETypeIndex);  
	}
}

int  CMeshAutoDlg::GetCurElemTypeID()
{
	m_nCurETypeIndex = m_ctrlCobxEType.GetCurSel();  
	return m_ElemTypeIDs[m_nCurETypeIndex];
}

void CMeshAutoDlg::SetTempElemDataByCurSetting()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(pIGM)
	{
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
		pIGM->SetTempElemData(GetCurElemTypeID(), D_NODE_MAXNUM);
		pIGM->SetTempElemNodeList(NList, GetCurElemTypeID(), D_NODE_MAXNUM, pIGM);
	}
}

class CMyProfilePrinter : public CProfilePrinterAdaptor
{
public:
	CMyProfilePrinter() {}
	virtual ~CMyProfilePrinter() {}
	virtual void Print(LPCTSTR lpszText) 
	{ 
		GSaveHistoryFormatNF(lpszText); 
	}
};

void CMeshAutoDlg::MakeSeedElemKList()
{
	m_mapSeedElemK.RemoveAll();
	m_mapSeedElemK.InitHashTable(1000003);
		
	std::vector<T_ELEM_K> aElemKList;

	CArray<T_DOEL_K, T_DOEL_K> aDoelK;
	m_pDoc->m_pAttrCtrl->GetDoelKeyList(aDoelK);

	//////////////////////////////////////////////////////////////////////////
	// Main Domain에 속한 Elem List
	// AutoMesh Option만 구매한 제품은 Main Domain에 속한 Elem만 판단.
	std::vector<T_MADO_K> aMadoKList;
	m_pDoc->m_pAttrCtrl->GetMadoKeyList(aMadoKList);
	for(int i=0; i<aMadoKList.size(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElemListMadoK(aMadoKList[i], aElemKList, aDoelK);
		for(int j=0; j<aElemKList.size(); j++)
			m_mapSeedElemK.SetAt(aElemKList[j], aElemKList[j]);
	}
#ifdef _MGEN_CH
	m_mapSeedElemK.RemoveAll();
#endif

	BOOL bApproval = FALSE;
	if(CProduct::IsMovingType(D_PRODUCT_MOVING_ORG) || CProduct::IsMovingType(D_PRODUCT_MOVING_CH))
		bApproval = m_pDoc->AllowCommand(D_OPTN_ID_SLAB_STRIP);
	else
		bApproval = m_pDoc->AllowCommand(D_OPTN_ID_SL_WA_DGN);

	if(!bApproval) return;
	
	//////////////////////////////////////////////////////////////////////////
	// Sub Domain에 속한 Elem List
	// Slab&Wall Design Option을 구매한 제품은 Main Domain 또는 Sub Domain에 속한 Elem를 판별하여 Seed 정보를 구성.
	std::vector<T_SBDO_K> aSbdoKList;
	m_pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoKList);
	for(int i=0; i<aSbdoKList.size(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoKList[i], aElemKList, aDoelK);
		for(int j=0; j<aElemKList.size(); j++)
			m_mapSeedElemK.SetAt(aElemKList[j], aElemKList[j]);
	}
}

void CMeshAutoDlg::Execute()
{
	clock_t start = clock();

	CWaitCursor Cursor;

	MakeSeedElemKList();

	m_dTol = m_pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit();

	// [2010-04-13] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
	// Mesh가 한평면인지 검사하는 Tol를 1cm로 수정  
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	m_dPlaneTol = CDBLib::Get_SlabSamePlaneTol(CurIndex.nBase_Length);
	
	if(!(CheckArg()))	return;
	
	m_bShowMsg=TRUE;  // 주두/지판이 있을경우 영역이 겹칠때 Warning 메시지를 출력할지 여부
	/*
	CMyProfilePrinter rpt;
	CProfileMgr::ClearProfileDataST();
	CProfileMgr::StartProfilingST();
	*/

	m_nChangeMethod=0;

	CString strWarnMsg, ustr;

	m_dMinMeshSize=0;
	m_dMaxBoundarySize=-DBL_MAX; 
	m_bChangeMeshSize = FALSE;

	AutoMeshArg.Mesher        = D_MESHER_DELAUNAY;
	AutoMeshArg.MesherType    = m_cmbType.GetCurSel();
	AutoMeshArg.nInnerDomain  = (m_bInnerDomain)? 1 : 0;
	if(m_bInteriorLine || m_bInteriorNode) AutoMeshArg.bIncInterEdge = TRUE;
	else                                   AutoMeshArg.bIncInterEdge = FALSE;  
	AutoMeshArg.bRelax        = TRUE;  
	AutoMeshArg.dRefineFactor = 1;
	AutoMeshArg.theSizeInfo.nConstraintType = m_nOption_Size;
	AutoMeshArg.theSizeInfo.bEmpty = FALSE;
	AutoMeshArg.nPropID = 1;
	if(m_nOption_Size == 0 )	  
	{
		AutoMeshArg.theSizeInfo.dSize = m_edtMeshSize.GetEditValue();
		double dMinSize = 1.0;
		dMinSize = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dMinSize);
		if(CCompFunc::CompReal(AutoMeshArg.theSizeInfo.dSize, dMinSize) != 1)
		{     
			CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, ustr);  
			strWarnMsg.Format(_LS(IDS_TM_ERR_MESH_SIZE_LENGTH),dMinSize,ustr);
			m_pDoc->DisplayHistoryMessage(strWarnMsg);             
			return;
		}

		double dWarnSize = 10.0;
		dWarnSize = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dWarnSize);
		if(CCompFunc::CompReal(AutoMeshArg.theSizeInfo.dSize, dWarnSize) != 1)
		{     
			CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, ustr);  
			strWarnMsg.Format(_LS(IDS_TM_WARN_MESH_SIZE_LENGTH__g__s),AutoMeshArg.theSizeInfo.dSize,ustr);
			int nAnswer = AfxMessageBox(strWarnMsg, MB_YESNO);
			if(nAnswer == IDNO) return;
		}
	}
	else if(m_nOption_Size == 1 ) 
	{
		AutoMeshArg.theSizeInfo.nDivision = (int)m_edtMeshSize.GetEditValue();
		if(AutoMeshArg.theSizeInfo.nDivision <= 1)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_ERR_MESH_SIZE_DIVIDE));
			return;
		}
		CString strDiv;
		strDiv.Format(_T("%d"), AutoMeshArg.theSizeInfo.nDivision);
		m_edtMeshSize.SetWindowText(strDiv);
	}  

	m_aEdge.clear();
	m_aSelectNode.RemoveAll();
			
	// Node Method
	if(m_cmbMethod.GetCurSel() == 0)
	{  
		MeshByNode();
	}
	// Line Method
	else if(m_cmbMethod.GetCurSel() == 1)
	{
		MeshByLine();
	}
	// Planar Method
	else if(m_cmbMethod.GetCurSel() == 2)
	{    
		// Planar Method는 선택된 Element를 돌면서 mesh한다.
		MeshByPlanarElement();
	}
	else
	{
		ASSERT(0);
	}

	if (CProduct::GetTestEnvValue(_ULS(MeshAutoTime)) == _ULS(yes))
	{
		clock_t end = clock();
		double dET = (double)(end - start) / CLOCKS_PER_SEC;
		GSaveHistoryFormatNF(_T("CMeshAutoDlg::Execute time is %g"), dET);
	}

	//CProfileMgr::PrintST((CProfilePrinterAdaptor*)&rpt);	

	IUsageCounter::Use(_T("MESHA"));
}

void CMeshAutoDlg::AddEdge(std::vector<T_LINE_G>& aEdge, T_LINE_G& srcLineG)
{
	if(aEdge.size() == 0)
	{
		aEdge.push_back(srcLineG);
		return;
	}

	int nCount = (int)aEdge.size();
	for (int i = 0; i < nCount; i++)
	{
		if (srcLineG.IsSameLine(aEdge[i], m_dTol, FALSE)) return;
	}

	double dLine1[2][3];
	double dLine2[2][3];
	double dCross[3];

	CArray<T_POINT3D,T_POINT3D&> aDividePoint;
	T_POINT3D dividePt;
	dLine1[0][0] = srcLineG.p1.x;
	dLine1[0][1] = srcLineG.p1.y;
	dLine1[0][2] = srcLineG.p1.z;

	dLine1[1][0] = srcLineG.p2.x;
	dLine1[1][1] = srcLineG.p2.y;
	dLine1[1][2] = srcLineG.p2.z;

	T_LINE_G addEdge;
	std::vector<T_LINE_G> aAddEdge; aAddEdge.reserve(nCount);

	double dDistToLine=0, dDistToPoint1=0, dDistToPoint2=0;
	bool bIncludeStart, bIncludeEnd, bIncludeStart_1, bIncludeEnd_1;
	for(int i=0; i<nCount; i++)
	{ 
		addEdge.Initialize();
		dLine2[0][0] = aEdge[i].p1.x;
		dLine2[0][1] = aEdge[i].p1.y;
		dLine2[0][2] = aEdge[i].p1.z;

		dLine2[1][0] = aEdge[i].p2.x;
		dLine2[1][1] = aEdge[i].p2.y;
		dLine2[1][2] = aEdge[i].p2.z;
		bool bIntersect = math_lib::intersect_line_line_3d(dLine1[0], dLine1[1], dLine2[0], dLine2[1], dCross);
		if(bIntersect == true)
		{
			// 기존 Line이 나눠진 Edge
			addEdge.p1   = aEdge[i].p1;
			addEdge.p2.x = dCross[0];
			addEdge.p2.y = dCross[1];
			addEdge.p2.z = dCross[2];
			if(addEdge.GetLineLength() > m_dTol)
				aAddEdge.push_back(addEdge);
			
			addEdge.p1.x = dCross[0];
			addEdge.p1.y = dCross[1];
			addEdge.p1.z = dCross[2];
			addEdge.p2   = aEdge[i].p2;
			if(addEdge.GetLineLength() > m_dTol)
				aAddEdge.push_back(addEdge);

			dividePt.x = dCross[0];
			dividePt.y = dCross[1];   
			dividePt.z = dCross[2];
			aDividePoint.Add(dividePt);      
		}
		else // 평행일 경우
		{ 
			dDistToLine = mathDistanceToLine(dLine2[0], dLine2[1], dLine1[0]);
			if(dDistToLine < m_dTol)
			{
				dDistToPoint1 = fabs(mathDistanceToPoint(dLine2[0], dLine1[0]));
				dDistToPoint2 = fabs(mathDistanceToPoint(dLine2[1], dLine1[0]));
				if(dDistToPoint1 > m_dTol && dDistToPoint2 > m_dTol)
					bIncludeStart = true;
				else
					bIncludeStart = false;
			}
			else
				bIncludeStart = false;

			dDistToLine = mathDistanceToLine(dLine2[0], dLine2[1], dLine1[1]);
			if(dDistToLine < m_dTol) 
			{
				dDistToPoint1 = fabs(mathDistanceToPoint(dLine2[0], dLine1[1]));
				dDistToPoint2 = fabs(mathDistanceToPoint(dLine2[1], dLine1[1]));
				if(dDistToPoint1 > m_dTol && dDistToPoint2 > m_dTol)
					bIncludeEnd = true;
				else
					bIncludeEnd = false;
			}
			else
				bIncludeEnd = false;
			
			if(bIncludeStart && bIncludeEnd)
			{
				// aEdge[i]에 srcLineG가 포함되는 경우
				double dDist_p1 = fabs(mathDistanceToPoint(dLine2[0], dLine1[0])); 
				double dDist_p2 = fabs(mathDistanceToPoint(dLine2[0], dLine1[1]));
				if(dDist_p1 < dDist_p2)
				{
					addEdge.p1 = aEdge[i].p1;
					addEdge.p2 = srcLineG.p1;            
					if(addEdge.GetLineLength() > m_dTol)
						aAddEdge.push_back(addEdge);

					addEdge.p1 = srcLineG.p2;
					addEdge.p2 = aEdge[i].p2;         
					if(addEdge.GetLineLength() > m_dTol)
						aAddEdge.push_back(addEdge);
				}
				else
				{
				addEdge.p1 = aEdge[i].p1;
					addEdge.p2 = srcLineG.p2;            
				if (addEdge.GetLineLength() > m_dTol)
					aAddEdge.push_back(addEdge);

					addEdge.p1 = srcLineG.p1;
				addEdge.p2 = aEdge[i].p2;
				if (addEdge.GetLineLength() > m_dTol)
					aAddEdge.push_back(addEdge);
			}
			}
			else if(bIncludeStart)
			{
				// 기존에 있던 edge가 추가되는 edge에 의해서 나눠질때 추가되는 edge와 겹치는 부분을 제외한 부분만 추가한다.                  
				if( fabs(mathDistanceToPoint(dLine2[0], dLine1[1])) < m_dTol )
				{
					addEdge.p1 = srcLineG.p1;
					addEdge.p2 = aEdge[i].p2;            
					if(addEdge.GetLineLength() > m_dTol)
						aAddEdge.push_back(addEdge);
				}
				else if( fabs(mathDistanceToPoint(dLine2[1], dLine1[1])) < m_dTol )
				{
					addEdge.p1 = aEdge[i].p1;
					addEdge.p2 = srcLineG.p1;            
					if(addEdge.GetLineLength() > m_dTol)
						aAddEdge.push_back(addEdge);
				}
				else
				{          
					dDistToLine = mathDistanceToLine(dLine1[0], dLine1[1], dLine2[0]);
					if(dDistToLine < m_dTol) 
					{
						addEdge.p1 = srcLineG.p1;
						addEdge.p2 = aEdge[i].p2;         
						if(addEdge.GetLineLength() > m_dTol)
							aAddEdge.push_back(addEdge);

						aDividePoint.Add(aEdge[i].p1);
						aDividePoint.Add(srcLineG.p2);
					}
					else        
					{
						addEdge.p1 = aEdge[i].p1;
						addEdge.p2 = srcLineG.p1;            
						if(addEdge.GetLineLength() > m_dTol)
							aAddEdge.push_back(addEdge);

						aDividePoint.Add(aEdge[i].p2);    
						aDividePoint.Add(srcLineG.p2);
					}
				}  
			}
			else if(bIncludeEnd)
			{
				if( fabs(mathDistanceToPoint(dLine2[0], dLine1[0])) < m_dTol )
				{
					addEdge.p1 = srcLineG.p2;
					addEdge.p2 = aEdge[i].p2;         
					if(addEdge.GetLineLength() > m_dTol)
						aAddEdge.push_back(addEdge);
				}
				else if( fabs(mathDistanceToPoint(dLine2[1], dLine1[0])) < m_dTol )
				{
					addEdge.p1 = aEdge[i].p1;
					addEdge.p2 = srcLineG.p2;            
					if(addEdge.GetLineLength() > m_dTol)
						aAddEdge.push_back(addEdge);
				}
				else
				{           
					dDistToLine = mathDistanceToLine(dLine1[0], dLine1[1], dLine2[0]);
					if(dDistToLine < m_dTol) 
					{            
						addEdge.p1 = srcLineG.p2;
						addEdge.p2 = aEdge[i].p2;         
						if(addEdge.GetLineLength() > m_dTol)
							aAddEdge.push_back(addEdge);

						aDividePoint.Add(aEdge[i].p1);   
						aDividePoint.Add(srcLineG.p2);
					}
					else        
					{
						addEdge.p1 = aEdge[i].p1;
						addEdge.p2 = srcLineG.p2;            
						if(addEdge.GetLineLength() > m_dTol)
							aAddEdge.push_back(addEdge);

						aDividePoint.Add(aEdge[i].p2);   
						aDividePoint.Add(srcLineG.p2);
					}
				}
			}
			else
			{
				dDistToLine = mathDistanceToLine(dLine1[0], dLine1[1], dLine2[0]);
				if(dDistToLine < m_dTol)
				{
					dDistToPoint1 = fabs(mathDistanceToPoint(dLine1[0], dLine2[0]));
					dDistToPoint2 = fabs(mathDistanceToPoint(dLine1[1], dLine2[0]));
					if(dDistToPoint1 > m_dTol && dDistToPoint2 > m_dTol)
						bIncludeStart_1 = true;
					else
						bIncludeStart_1 = false;          
				}
				else    
					bIncludeStart_1 = false;
				
				dDistToLine = mathDistanceToLine(dLine1[0], dLine1[1], dLine2[1]);
				if(dDistToLine < m_dTol)
				{
					dDistToPoint1 = fabs(mathDistanceToPoint(dLine1[0], dLine2[1]));
					dDistToPoint2 = fabs(mathDistanceToPoint(dLine1[1], dLine2[1]));
					if(dDistToPoint1 > m_dTol && dDistToPoint2 > m_dTol)
						bIncludeEnd_1 = true;
					else
						bIncludeEnd_1 = false;          
				}
				else        
					bIncludeEnd_1 = false;
				
				if(bIncludeStart_1 && bIncludeEnd_1)
				{
					// srcLineG에 aEdge[i]가 포함되는 경우
					aDividePoint.Add(aEdge[i].p1);
					aDividePoint.Add(aEdge[i].p2); 
					aDividePoint.Add(srcLineG.p2); 
				}        
				else if(bIncludeStart_1)
				{          
					aDividePoint.Add(aEdge[i].p1);          
					aDividePoint.Add(aEdge[i].p2);          
				}
				else if(bIncludeEnd_1)
				{
					aDividePoint.Add(aEdge[i].p2);
					aDividePoint.Add(srcLineG.p2);
				}
				else
					aAddEdge.push_back(aEdge[i]);
			}
		}        
	}  

	double p1[3];
	CArray<int, int>       aIndex;   
	CArray<double, double> aDist; 
	for(int i=0; i<aDividePoint.GetSize(); i++)
	{    
		p1[0] = aDividePoint[i].x;  p1[1] = aDividePoint[i].y;  p1[2] = aDividePoint[i].z;
		double dDist = mathDistanceToPoint(dLine1[0], p1);    
		if(dDist > m_dTol)
		{
			aDist.Add(dDist);  
			aIndex.Add(i);  
		}
	}

	if(aDist.GetSize() < 2)
	{
		if(srcLineG.GetLineLength() > m_dTol)
			aAddEdge.push_back(srcLineG);
	}
	else
	{
		CQSort::QSortDblWithIndex(aIndex.GetData(), aDist.GetData(), aDist.GetSize());
		
		addEdge.p1 = srcLineG.p1;
		addEdge.p2 = aDividePoint[aIndex[0]];
		if(addEdge.GetLineLength() > m_dTol)
			aAddEdge.push_back(addEdge);
		
		int nIndexSize = aIndex.GetSize();
		for(int i=0; i<nIndexSize-1; i++)
		{
			addEdge.p1 = aDividePoint[aIndex[i]];
			addEdge.p2 = aDividePoint[aIndex[i+1]];
			if(addEdge.GetLineLength() > m_dTol)
				aAddEdge.push_back(addEdge);
		}    
	}
	
	aEdge.clear();
	aEdge.resize(aAddEdge.size());
	std::copy(aAddEdge.begin(), aAddEdge.end(), aEdge.begin());
}

void CMeshAutoDlg::DivideEdgeByIncludeNode(std::vector<T_LINE_G>& aEdge, CArray<T_POINT3D,T_POINT3D&>& aNode)
{
M_PROFILE('CMeshAutoDlg::DivideEdgeByIncludeNode');
	int i, j;
	double dLength = 0; // Node와 Edge 사이의 거리
	double line_i[3];
	double line_j[3];
	double point[3];
	double p1[3]; 
	double p2[3];

	CMapEx<int,int,int,int> mapDelNodeIndex; mapDelNodeIndex.InitHashTable(1000003);
	std::vector<T_LINE_G> aTempEdge; aTempEdge.reserve(aEdge.size());
	CArray<T_POINT3D,T_POINT3D&> aIncludeNode;
	CArray<T_POINT3D,T_POINT3D&> aIncludeNode_Sort;
	CArray<double, double> aDist;   
	CArray<int, int>       aIndex;  
	T_LINE_G  LineG;  
	T_POINT3D point3D;
	for(int i=0; i<aEdge.size(); i++)
	{
		LineG.Initialize();
		LineG = aEdge[i];

		double StartLength = mathLength(LineG.p1.x, LineG.p1.y, LineG.p1.z);
		double EndLength   = mathLength(LineG.p2.x, LineG.p2.y, LineG.p2.z);
		if(EndLength < StartLength)
		{
			line_i[0] = LineG.p2.x; line_i[1] = LineG.p2.y; line_i[2] = LineG.p2.z;
			line_j[0] = LineG.p1.x;	line_j[1] = LineG.p1.y;	line_j[2] = LineG.p1.z;
		}
		else
		{
			line_i[0] = LineG.p1.x;	line_i[1] = LineG.p1.y;	line_i[2] = LineG.p1.z;
			line_j[0] = LineG.p2.x;	line_j[1] = LineG.p2.y;	line_j[2] = LineG.p2.z;
		}
		
		aDist.RemoveAll();
		aIndex.RemoveAll();
		int nIndex = 0;
		aIncludeNode.RemoveAll();    
		aIncludeNode_Sort.RemoveAll();
		for(j=0; j<aNode.GetSize(); j++)
		{
			point3D = aNode[j];
			// Edge의 시작과 끝의 위치점이면 안됨
			if( (point3D.x==LineG.p1.x && point3D.y==LineG.p1.y && point3D.z==LineG.p1.z) ||
					(point3D.x==LineG.p2.x && point3D.y==LineG.p2.y && point3D.z==LineG.p2.z) )
				continue;
			
			point[0] = point3D.x; point[1] = point3D.y; point[2] = point3D.z;      
			dLength = mathDistanceToLine(line_i, line_j, point);      
			if(dLength > m_dTol) continue;            

			aIncludeNode.Add(point3D);
			p1[0] = LineG.p1.x; p1[1] = LineG.p1.y; p1[2] = LineG.p1.z;
			p2[0] = point3D.x ; p2[1] = point3D.y ; p2[2] = point3D.z ;      
			double dDist = mathDistanceToPoint(p1, p2);
			aDist.Add(dDist);
			aIndex.Add(nIndex);
			nIndex++;

			mapDelNodeIndex.SetAt(j, j);
		}  

		int nIncludeCount = aIncludeNode.GetSize();
		if(nIncludeCount <= 0  && LineG.GetLineLength() > m_dTol)
		{
			aTempEdge.push_back(LineG);
			continue;
		}

		// p1에서부터 거리별로 정렬    
		CQSort::QSortDblWithIndex(aIndex.GetData(), aDist.GetData(), aDist.GetSize());
		for(j=0; j<nIncludeCount; j++)    
			aIncludeNode_Sort.Add(aIncludeNode[aIndex[j]]);    

		T_LINE_G  LineG_temp;
		LineG_temp.Set(LineG.p1, aIncludeNode_Sort[0]); 
		if(LineG_temp.GetLineLength() > m_dTol ) aTempEdge.push_back(LineG_temp);
		
		for(j=0; j<nIncludeCount; j++)
		{      
			LineG_temp.Initialize();
			if(j == nIncludeCount-1)      
				LineG_temp.Set(aIncludeNode_Sort[j], LineG.p2);            
			else
				LineG_temp.Set(aIncludeNode_Sort[j], aIncludeNode_Sort[j+1]);     
			if(LineG_temp.GetLineLength() > m_dTol ) aTempEdge.push_back(LineG_temp); 
		}
	}

	aIndex.RemoveAll();
	mapDelNodeIndex.GetKeyList(aIndex);
	CQSort::QSortInt(aIndex.GetData(), aIndex.GetSize());
	for(i=aIndex.GetSize()-1; i>=0; i--)  
		aNode.RemoveAt(aIndex[i]);  

	aEdge.clear();
	std::copy(aTempEdge.begin(), aTempEdge.end(), std::back_inserter(aEdge));
}

BOOL CMeshAutoDlg::GetDomainType(int& nType, double dPlanePoint[3][3])
{
	// KR의 경우 Wall이 Sub-Domain Type에서 제외 되기 때문에 아래와 같이 처리함.
	// wg_cmd2 > SubDomainDlg.cpp > CSubDomainDlg::OnInitDialog() 함수 수정시 코드 삭제할 것...!
	if(CProduct::IsMovingType(D_PRODUCT_MOVING_ORG))
	{
		nType = 1;
		return TRUE;
	}

#ifndef _CIVIL
	if (CDBLib::IsCodeForMeshedDesignAIJ_JP())
	{
		nType = 1;
		return TRUE;
	}
#endif

	double dTempVector[3];
	mathCross_product_3d(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dTempVector);
	double dNormalizeVector[3];
	mathNormalize(dTempVector[0], dTempVector[1], dTempVector[2], dNormalizeVector[0], dNormalizeVector[1], dNormalizeVector[2]);

	// 경사 45도를 기준으로 Wall과 Slab 타입을 구분함.
	// Meshed Wall의 경우 Plate 부재력으로 설계하기 때문에 어느 각도라도 설계가 가능하지만,
	// MQC-10329 요청에 따라 자동으로 구분해서 판단하도록 처리함.
	if(fabs(dNormalizeVector[2]) < 0.7071)    // 0.7071 =  1/sqrt(2.0). 경사 45도일때의 평면 법선벡터의 z값.
		nType = 3;//Wall
	else
		nType = 1;//Slab
	
	return TRUE;
}

void CMeshAutoDlg::OnElemTypeButton() 
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
	else
		ASSERT(0);
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CMeshAutoDlg::GetPropIDS(int nElemType, int& nMatID, int& nThkID)
{
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	CString strT, strM;
	m_ctrlEditThik.GetWindowText(strT);
	m_ctrlEditMatl.GetWindowText(strM);
	
	if (pDoc->m_pAttrCtrl->IsNeedMatl(nElemType))
		GetINumber(strM,nMatID);
	else nMatID = 0;  
	
	if (pDoc->m_pAttrCtrl->IsNeedThik(nElemType))
		GetINumber(strT,nThkID);
	else nThkID = 0;
	
	return TRUE;
}

BOOL CMeshAutoDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;  
	return TRUE;
}

void CMeshAutoDlg::OnSelchangeTmElemTypeCombo() 
{ 
	int nElemType = GetCurElemTypeID(); 
	if(m_pDoc->m_pAttrCtrl->IsPlstrn(nElemType) || m_pDoc->m_pAttrCtrl->IsAxisym(nElemType))
	{
		GetDlgItem(IDC_TM_MESH_AUTO_THICK_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_THICK_NO_EDIT      )->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_THICK_NAME_COMBO   )->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_THIC_BUTTON        )->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_ELEM_BUTTON        )->EnableWindow(FALSE);    
	}
	else
	{
		GetDlgItem(IDC_TM_MESH_AUTO_THICK_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_THICK_NO_EDIT      )->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_THICK_NAME_COMBO   )->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_THIC_BUTTON        )->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_ELEM_BUTTON        )->EnableWindow(TRUE);
	}
}

void CMeshAutoDlg::OnTmMatButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));
}

void CMeshAutoDlg::OnTmThickButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_THICKNESS,0));
}

void CMeshAutoDlg::OnTmDomainButton() 
{
	CreateOrActivateCMD2Dlg((CDBDoc*)m_pDoc, CMainDomainDlg::IDD);
}

void CMeshAutoDlg::OnClickInteriorNodeRadio()
{
	UpdateData(TRUE);
	if(m_nOption_Node == 0)
		m_wndNodeListEdit.EnableWindow(FALSE);  
	else
	{
		m_wndNodeListEdit.EnableWindow(TRUE);  
		GotoDlgCtrl(&m_wndNodeListEdit);
		OnSetfocusTmNodeEdit();
	}
}

void CMeshAutoDlg::OnClickInteriorLineRadio()
{
	UpdateData(TRUE);
	if(m_nOption_Line == 0)
		m_wndElemListEdit.EnableWindow(FALSE);  
	else
	{
		m_wndElemListEdit.EnableWindow(TRUE);  
		GotoDlgCtrl(&m_wndElemListEdit);
		OnSetfocusTmElemEdit();
	}
}

void CMeshAutoDlg::OnClickMeshSizeRadio()
{
	UpdateData(TRUE);
	if(m_nOption_Size == 0)
	{
		m_untMeshSize.ShowWindow(SW_SHOW);
		m_edtMeshSize.SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	else
	{
		m_untMeshSize.ShowWindow(SW_HIDE); 
		m_edtMeshSize.SetUnitType(D_UNITSYS_NONE);
	}
}

void CMeshAutoDlg::OnClickDeleteSrcCheck()
{
	/*
	UpdateData(TRUE);
	if(m_bDeleteSrcLine) 
	{
		GetDlgItem(IDC_TM_MESH_AUTO_SUBDIVIDE_SRC_CHK)->EnableWindow(FALSE);
		m_bSubdivideSrcLine = FALSE;
		UpdateData(FALSE);      
	}
	else
		GetDlgItem(IDC_TM_MESH_AUTO_SUBDIVIDE_SRC_CHK)->EnableWindow(TRUE);
	*/
}

void CMeshAutoDlg::OnClickInteriorNodeCheck()
{
	UpdateData(TRUE);
	EnableInterierNode(m_bInteriorNode);    
}

void CMeshAutoDlg::OnClickInteriorLineCheck()
{
	UpdateData(TRUE);
	EnableInterierLine(m_bInteriorLine);  
}

void CMeshAutoDlg::EnableInterierNode(BOOL bEnable)
{
	GetDlgItem(IDC_TM_MESH_AUTO_NODE_AUTO_RADIO)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_MESH_AUTO_NODE_USER_RADIO)->EnableWindow(bEnable); 
	m_wndNodeListEdit.EnableWindow(bEnable);
	if(bEnable)
	{
		GotoDlgCtrl(&m_wndNodeListEdit);
		OnClickInteriorNodeRadio();    
	}
}

void CMeshAutoDlg::EnableInterierLine(BOOL bEnable)
{
	GetDlgItem(IDC_TM_MESH_AUTO_LINE_AUTO_RADIO)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_MESH_AUTO_LINE_USER_RADIO)->EnableWindow(bEnable);
	m_wndElemListEdit.EnableWindow(bEnable);
	if(bEnable)
	{
		GotoDlgCtrl(&m_wndElemListEdit);
		OnClickInteriorLineRadio();   
	}  
}

void CMeshAutoDlg::OnSelchangeMethodCombo()
{  
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	ChangeMethod();
	if(m_cmbMethod.GetCurSel() == 0)
		I_GENModelBase::SetModelingModeST(IG_STATE_RESET);
	else
		I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
	m_wndSelectElemListEdit.SetWindowText(_T(""));
}

void CMeshAutoDlg::ChangeMethod()
{
	UpdateData(TRUE);
	// Line
	if(m_cmbMethod.GetCurSel() == 1)
	{
		m_wndSelectNodeListEdit.ShowWindow(SW_HIDE);
		m_wndSelectElemListEdit.ShowWindow(SW_SHOW);   
		GotoDlgCtrl(&m_wndSelectElemListEdit);    
		GetDlgItem(IDC_TM_MESH_AUTO_DELETE_SRC_CHK)->SetWindowText(_LS(IDS_TM_Delete_Source_Line_Elem));         
		GetDlgItem(IDC_TM_MESH_AUTO_SUBDIVIDE_SRC_CHK)->SetWindowText(_LS(IDS_TM_Subdivide_Source_Line_Elem));   
		
		// Set Select Mode
	}
	else
	{     
		// node
		if(m_cmbMethod.GetCurSel() == 0)
		{
			m_wndSelectNodeListEdit.ShowWindow(SW_SHOW);
			m_wndSelectElemListEdit.ShowWindow(SW_HIDE);      
			GotoDlgCtrl(&m_wndSelectNodeListEdit);
			
		}
		// planar
		else
		{
			m_wndSelectNodeListEdit.ShowWindow(SW_HIDE);
			m_wndSelectElemListEdit.ShowWindow(SW_SHOW);       
			GotoDlgCtrl(&m_wndSelectElemListEdit);
			
			// Set Select Mode
			
		}    
		GetDlgItem(IDC_TM_MESH_AUTO_DELETE_SRC_CHK)->SetWindowText(_LS(IDS_TM_Delete_Boundary_Line_Elem));        
		GetDlgItem(IDC_TM_MESH_AUTO_SUBDIVIDE_SRC_CHK)->SetWindowText(_LS(IDS_TM_Subdivide_Boundary_Line_Elem));  
	}    
}

void CMeshAutoDlg::OnSetfocusTmSelectNodeEdit() 
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
		
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
 
		pIGM->SetTempElemErrorFlag(TRUE);
		pIGM->CursorClearHide();
		pIGM->CancelAllTempElem();    
		m_wndSelectNodeListEdit.SetWindowText(_T(""));    
	}

	/*
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

			// Reset Select Mode
			
			// Set Node Snap Mode

			//I_GENModelBase::SetModelingModeST(IG_STATE_ELEM_ADD);
		}
	}
	*/
}

void CMeshAutoDlg::OnSetfocusTmNodeEdit() 
{
	/*
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	if(pIGM)
	{   
		SetTempElemDataByCurSetting();  
		pIGM->SetTempElemFirstDrawFlag();
		
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
	
		pIGM->SetTempElemErrorFlag(TRUE);
		pIGM->CursorClearHide();
		pIGM->CancelAllTempElem();    
		m_wndNodeListEdit.SetWindowText(_T(""));     
		
		// Reset Select Mode
			
		// Set Node Snap Mode
	}  
	*/
}

void CMeshAutoDlg::OnSetfocusTmElemEdit() 
{
	/*
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	if(pIGM)
	{     
		pIGM->SetTempElemFirstDrawFlag();
		
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
		
		pIGM->SetTempElemErrorFlag(TRUE);
		pIGM->CursorClearHide();
		pIGM->CancelAllTempElem();    
		m_wndElemListEdit.SetWindowText(_T(""));    
		
		// Set Elem Snap Mode

	}
	*/
}

LRESULT CMeshAutoDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{  
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int,unsigned int> NList;
	CString str = _T("");
	CString Tstr;
	int nCount = 0;

	if(m_wndSelectNodeListEdit.GetSafeHwnd() == (HWND)lParam && 
		 pIGM->GetModelingModeST() == IG_STATE_ELEM_ADD)
	{    
		CArray<unsigned int,unsigned int> NList_Save;
		pIGM->GetTempElemNodeList(NList);
		nCount = NList.GetSize();
		if(m_bStartNodeInput)
		{   
			m_bStartNodeInput = FALSE;                
			if(nCount > 1)
			{ 
				NList_Save.Add(NList[nCount-1]);
				pIGM->SetTempElemData(GetCurElemTypeID(), D_NODE_MAXNUM);
				pIGM->SetTempElemNodeList(NList_Save, GetCurElemTypeID(), D_NODE_MAXNUM, pIGM);                
			}      
		}
		pIGM->GetTempElemNodeList(NList);       
		nCount = NList.GetSize();

		if(nCount == 1)
			m_wndSelectNodeListEdit.SetWindowText(_T(""));
		
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
			m_wndSelectNodeListEdit.SetWindowText(str);  
		}

		if(nCount > 1)
		{
			if(NList[0] == NList[nCount-1])        
				m_bStartNodeInput = TRUE;           
		}    
	}  
	else if(m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam && 
					pIGM->GetModelingModeST() == IG_STATE_ELEM_ADD)
	{
		/*
		pIGM->GetTempElemNodeList(NList);
		
		nCount = NList.GetSize();
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
		}
		*/
	}
	else if(m_wndElemListEdit.GetSafeHwnd() == (HWND)lParam && 
					pIGM->GetModelingModeST() == IG_STATE_ELEM_ADD)
	{
		/*
		pIGM->GetTempElemNodeList(NList);
		
		nCount = NList.GetSize();
		if(nCount == 1)
			m_wndElemListEdit.SetWindowText(_T(""));
		
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
			m_wndElemListEdit.SetWindowText(str);  
		}
		*/
	}

	return 0L;

	/*
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_wndSelectNodeListEdit.GetSafeHwnd() == (HWND)lParam && 
		 pIGM->GetModelingModeST() == IG_STATE_ELEM_ADD)
	{
		CArray<unsigned int,unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);

		CString str = _T("");
		CString Tstr;

		int nCount = NList.GetSize();
		if(nCount == 1)
			m_wndSelectNodeListEdit.SetWindowText(_T(""));
		
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
		 m_wndSelectNodeListEdit.SetWindowText(str);
		}
	}
	

	return 0L;
	*/
}

//=====================================================================
// CheckArg
//=====================================================================
BOOL CMeshAutoDlg::CheckArg(const BOOL bSimul) 
{
	// Domain Name Check
	CString strName = _T("");
	GetDlgItem(IDC_TM_DOMAIN_NAME_EDIT)->GetWindowText(strName);
	strName.TrimLeft();
	strName.TrimRight();
	if (strName.GetLength() == 0)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_Domain_name_));
		return FALSE;
	}

	m_aKeyList.clear();
	m_mapNodeKeyList.RemoveAll();
	m_mapElemKeyList.RemoveAll();

	m_CcpaK.RemoveAll();
	m_DpaaK.RemoveAll();
	m_MasterNodeK.RemoveAll();

	int i, j, temp=0;  

	CString errorMsg;  
	CString strKeyList;
	if(m_cmbMethod.GetCurSel() == 0) // node
	{
		m_wndSelectNodeListEdit.GetWindowText(strKeyList);
		GetNodeList(strKeyList,m_aKeyList);    
		
		std::vector<int> aTempKeyList;
		// Inner Node와 중복되는 것을 막기 위한 map
		for(i=0; i<m_aKeyList.size(); i++)  
		{
			if(m_mapNodeKeyList.Lookup(m_aKeyList[i], temp)) continue;
			m_mapNodeKeyList.SetAt(m_aKeyList[i], temp); 
			aTempKeyList.push_back(m_aKeyList[i]);
		}
		// Node 개수가 3개 이상이어야지만 평면을 구성할 수 있음.
		if(aTempKeyList.size() < 3) 
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
			return FALSE;
		}    
		m_aKeyList.clear();
		m_aKeyList.resize(aTempKeyList.size());
		std::copy(aTempKeyList.begin(), aTempKeyList.end(), m_aKeyList.begin());
	}
	else
	{
		m_wndSelectElemListEdit.GetWindowText(strKeyList); 
		m_wndSelectElemListEdit.SelectByStr(strKeyList);    
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		CArray<UINT,UINT> aTempKey;
		pIGM->GetSelectedElemKeyList(aTempKey);
		if(aTempKey.GetSize() > 0)
		{
			m_aKeyList.reserve(aTempKey.GetSize());
			for(i=0; i<aTempKey.GetSize(); i++)     
				m_aKeyList.push_back((int)aTempKey[i]);      
		}
		// Inner Line과 중복되는 것을 막기 위한 map
		for(i=0; i<m_aKeyList.size(); i++)
			m_mapElemKeyList.SetAt(m_aKeyList[i], temp);    

		int nTempK = 0;
		m_aKeyList.clear();
		m_aKeyList.reserve(m_mapElemKeyList.GetSize());

		if(m_cmbMethod.GetCurSel() == 2) // planar
		{
			POSITION pos = m_mapElemKeyList.GetStartPosition();
			while (pos)
			{
				m_mapElemKeyList.GetNextKey(pos, nTempK);
				m_aKeyList.push_back(nTempK);
			}

			if(m_aKeyList.size() < 1)
			{
				m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_elem_list_));
				return FALSE;
			}      
		}
		else // line
		{
			T_ELEM_D ElemD;
			POSITION pos = m_mapElemKeyList.GetStartPosition();
			while (pos)
			{
				m_mapElemKeyList.GetNextKey(pos, nTempK);

				if (!m_pDoc->m_pAttrCtrl->GetElem(nTempK, ElemD)) continue;
				if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) continue;

				m_aKeyList.push_back(nTempK);
			}

			// line 개수가 3개 이상이어야지만 평면을 구성할 수 있음.
			if(m_aKeyList.size() < 3) 
			{
				m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_elem_list_));
				return FALSE;
			}      
		}

		// Elem의 node를 m_mapNodeKeyList에 등록   
		T_ELEM_D ElemD;    
		for(i=0; i<m_aKeyList.size(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetElem(m_aKeyList[i], ElemD)) {ASSERT(0); return FALSE;}              
			int nNodeNum = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp,ElemD.elnod);
			for(j=0; j<nNodeNum; j++)
			{
				m_mapNodeKeyList.SetAt(ElemD.elnod[j], temp);
			}
		}
	} 

	return TRUE;
}

BOOL CMeshAutoDlg::GetSamePlaneElementEdge(std::vector<T_LINE_G>& aEdge, T_ELEM_D& ElemD, std::vector<T_LINE_G>& arIntersectEdge)
{
	arIntersectEdge.clear();
	double dPlanePoint[3][3]={0};
	if(!GetPlanePoint(aEdge, dPlanePoint)) return FALSE;

	T_NODE_D NodeD1, NodeD2;
	double dPoint[3];
	T_POINT3D ptStart, ptEnd;
	T_LINE_G LineG;
	int nNodeNum = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp,ElemD.elnod);
	arIntersectEdge.reserve(nNodeNum*0.25);
	for(int i=0; i<nNodeNum; i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[i], NodeD1)) { ASSERT(0); return FALSE; }

		dPoint[0] = NodeD1.x;  dPoint[1] = NodeD1.y;  dPoint[2] = NodeD1.z;  
		if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol)) continue;
		
		if(i == nNodeNum-1)
		{
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD2)) { ASSERT(0); return FALSE; }      
		}
		else
		{
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[i+1], NodeD2)) { ASSERT(0); return FALSE; }
		}

		dPoint[0] = NodeD2.x;  dPoint[1] = NodeD2.y;  dPoint[2] = NodeD2.z; 
		if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol)) continue;
		     
		ptStart.x = NodeD1.x;
		ptStart.y = NodeD1.y;
		ptStart.z = NodeD1.z;

		ptEnd.x = NodeD2.x;
		ptEnd.y = NodeD2.y;
		ptEnd.z = NodeD2.z;

		LineG.Set(ptStart, ptEnd);
		arIntersectEdge.push_back(LineG);
	}

	if(arIntersectEdge.size() > 0) 
		return TRUE;

	return FALSE;
}

BOOL CMeshAutoDlg::GetPlanePoint(std::vector<T_LINE_G>& aEdge, double dPlanePoint[3][3])
{
	double vec1[3], vec2[3];
	double dOriAngle=0, dTempAngle=0, dAngleTol = 0.1;   
	int nEdgeSize = aEdge.size();
	
	dPlanePoint[0][0] = aEdge[0].p1.x;
	dPlanePoint[0][1] = aEdge[0].p1.y;
	dPlanePoint[0][2] = aEdge[0].p1.z;
	
	dPlanePoint[1][0] = aEdge[0].p2.x;
	dPlanePoint[1][1] = aEdge[0].p2.y;
	dPlanePoint[1][2] = aEdge[0].p2.z;
	
	// 기준이 되는 vec
	vec1[0] = aEdge[0].p2.x - aEdge[0].p1.x;
	vec1[1] = aEdge[0].p2.y - aEdge[0].p1.y;
	vec1[2] = aEdge[0].p2.z - aEdge[0].p1.z;
	
	int nIndex = 0;
	BOOL bPlane = FALSE;
	for(int i=1; i<nEdgeSize; i++)
	{
		vec2[0] = aEdge[i].p2.x - aEdge[i].p1.x;
		vec2[1] = aEdge[i].p2.y - aEdge[i].p1.y;
		vec2[2] = aEdge[i].p2.z - aEdge[i].p1.z;
		
		dOriAngle = mathCrossAngleNormalize(vec1,vec2);
		if( CCompFunc::CompRealTol(dOriAngle, 0  , dAngleTol) != 0 && 
				CCompFunc::CompRealTol(dOriAngle, 180, dAngleTol) != 0 && 
				CCompFunc::CompRealTol(dOriAngle, 360, dAngleTol) != 0 ) 
		{
			vec2[0] = aEdge[i].p2.x - aEdge[0].p1.x;
			vec2[1] = aEdge[i].p2.y - aEdge[0].p1.y;
			vec2[2] = aEdge[i].p2.z - aEdge[0].p1.z;
			
			if(vec2[0] == 0 && vec2[1] == 0 && vec2[2] == 0)
			{
				dPlanePoint[2][0] = aEdge[i].p1.x;
				dPlanePoint[2][1] = aEdge[i].p1.y;
				dPlanePoint[2][2] = aEdge[i].p1.z;          
			}
			else
			{
				dTempAngle = mathCrossAngleNormalize(vec1,vec2);
				
				if( CCompFunc::CompRealTol(dTempAngle, 0  , dAngleTol) != 0 && 
						CCompFunc::CompRealTol(dTempAngle, 180, dAngleTol) != 0 && 
						CCompFunc::CompRealTol(dTempAngle, 360, dAngleTol) != 0 )
				{
					dPlanePoint[2][0] = aEdge[i].p2.x;
					dPlanePoint[2][1] = aEdge[i].p2.y;
					dPlanePoint[2][2] = aEdge[i].p2.z;  
				}
				else
				{
					vec2[0] = aEdge[i].p1.x - aEdge[0].p1.x;
					vec2[1] = aEdge[i].p1.y - aEdge[0].p1.y;
					vec2[2] = aEdge[i].p1.z - aEdge[0].p1.z;

					dTempAngle = mathCrossAngleNormalize(vec1,vec2);

					if( CCompFunc::CompRealTol(dTempAngle, 0  , dAngleTol) != 0 && 
						CCompFunc::CompRealTol(dTempAngle, 180, dAngleTol) != 0 && 
						CCompFunc::CompRealTol(dTempAngle, 360, dAngleTol) != 0 )
					{
						dPlanePoint[2][0] = aEdge[i].p1.x;
						dPlanePoint[2][1] = aEdge[i].p1.y;
						dPlanePoint[2][2] = aEdge[i].p1.z;  
					}
					else
					{
						continue;
					}
				}                     
			}

			bPlane = TRUE;
			break;     
		}
	}

	return bPlane;
}

BOOL CMeshAutoDlg::ConfirmInPlane(std::vector<T_LINE_G>& aEdge, CArray<T_POINT3D,T_POINT3D&>& aNode, double PlaneLocalVector[3][3], double dPlanePoint[3][3])
{    
	BOOL bPlane = GetPlanePoint(aEdge, dPlanePoint);
		
	if(!bPlane)
		return FALSE;

	double dPoint[3]; 
	for(int i=1; i<aEdge.size(); ++i)
	{          
		dPoint[0] = aEdge[i].p1.x;  dPoint[1] = aEdge[i].p1.y;  dPoint[2] = aEdge[i].p1.z;
		if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dPlaneTol))
		{
			bPlane = FALSE;
			return FALSE;
		}
	}
	
	for(int i=0; i<aNode.GetSize(); ++i)
	{          
		dPoint[0] = aNode[i].x;  dPoint[1] = aNode[i].y;  dPoint[2] = aNode[i].z;
		if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dPlaneTol))
		{
			bPlane = FALSE;
			return FALSE;
		}
	}    
	
	if(!m_pDoc->calcPlaneLocalVector(3, dPlanePoint, 0.0, PlaneLocalVector)) { ASSERT(0); } 
	
	return TRUE;
}

BOOL CMeshAutoDlg::CheckInteriorNode()
{
	return TRUE;
}

BOOL CMeshAutoDlg::CheckInteriorEdge()
{
	return TRUE;
}

LRESULT CMeshAutoDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wndSelectNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{    
		m_wndSelectNodeListEdit.SetWindowText(_T(""));
	}
	else if(m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		//m_wndNodeListEdit.SetWindowText("");
	}
	else if(m_wndElemListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		//m_wndElemListEdit.SetWindowText("");
	}
	
	return 0L;
}

LRESULT CMeshAutoDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
{
	if(m_wndSelectNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{        
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(!pIGM) return 0L;
				
		m_wndSelectNodeListEdit.SetWindowText(_T(""));    
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);    
	}
	else if (m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(!pIGM) return 0L;
		
		//m_wndNodeListEdit.SetWindowText("");    
		//pIGM->SetModelingModeST(IG_STATE_INITALL);
		//pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
	}
	else if (m_wndElemListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(!pIGM) return 0L;
		
		//m_wndElemListEdit.SetWindowText("");    
		//pIGM->SetModelingModeST(IG_STATE_INITALL);
		//pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
	}
	return 0L;
}

void CMeshAutoDlg::ChangeElemTypeComboWidth()
{
	int nCount = m_ctrlCobxEType.GetCount();
	if(nCount<1) return;
	
	CString data = _T("");
	m_ctrlCobxEType.GetLBText(0, data);
	
	CDC* pDC = GetDC();  
	CSize max, cur;
	max = pDC->GetTextExtent(data);
	m_ctrlCobxEType.SetDroppedWidth(max.cx);
	
	for(int i=1; i<nCount; i++)
	{
		m_ctrlCobxEType.GetLBText(i, data);
		cur = pDC->GetTextExtent(data); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if(max.cx > m_ctrlCobxEType.GetDroppedWidth()) m_ctrlCobxEType.SetDroppedWidth(max.cx);
	
	ReleaseDC(pDC);
}

// dTargetLine이 dBoundLine과 같지 않고 포함되는지 여부 
BOOL CMeshAutoDlg::IsInsideEdge(double dBoundLine1[2], double dBoundLine2[2], double dTargetLine1[2], double dTargetLine2[2])
{
	if(!mathIsPointOfLine2D(dBoundLine1, dBoundLine2, dTargetLine1, TRUE) ||
	   !mathIsPointOfLine2D(dBoundLine1, dBoundLine2, dTargetLine2, TRUE) )
		 return FALSE;

	T_LINE_G Line1, Line2;
	Line1.Set(T_POINT3D(dBoundLine1[0] , dBoundLine1[1] , 0), T_POINT3D(dBoundLine2[0] , dBoundLine2[1] , 0));
	Line2.Set(T_POINT3D(dTargetLine1[0], dTargetLine1[1], 0), T_POINT3D(dTargetLine2[0], dTargetLine2[1], 0));

	if(Line1.IsSameLine(Line2, m_dTol, FALSE))
		return FALSE;

	return TRUE;  
}

void CMeshAutoDlg::MeshByPlanarElement()
{
	T_ELEM_D AddElemD; AddElemD.Initialize();
	AddElemD.eltyp = GetCurElemTypeID();
	if (!GetPropIDS(AddElemD.eltyp, AddElemD.elmat, AddElemD.elpro)) return;

	int i, j, nNodeNum, temp=0;
	T_POINT3D ptStart, ptEnd;
	T_ELEM_D  ElemD;
	T_NODE_D  NodeD1, NodeD2;  
	T_LINE_G  LineG;
	
	std::vector<T_ELEM_K> arElemK; 
	std::vector<T_ELEM_D> arElemD;
	std::vector<T_NODE_K> arNodeK; 
	std::vector<T_NODE_D> arNodeD;     
	std::vector<T_NODE_D> aHoopNodeData;

	CArray<int,int> aDomainElemCount;
	std::vector<T_ELEM_K> aDelElemK;
	CArray<CArray<int,int>*, CArray<int,int>*> aIncludeSbdoIndex; 
	CArray<T_SBDO_ELEMLIST_D, T_SBDO_ELEMLIST_D&> aSbdoElemList;

	T_NODE_K NewNodeK = m_pDoc->m_pDataCtrl->GetNodeKey(); 
	T_ELEM_K NewElemK = m_pDoc->m_pDataCtrl->GetElemKey();
	
	if(m_pDoc->m_pAttrCtrl->IsPlate(AddElemD.eltyp))
	{
		if(m_bDOF)
		{
			if(m_nThickThin == 1)
				AddElemD.nSubType = 3;
			else
				AddElemD.nSubType = 2;
		}
		else
		{
			if(m_nThickThin == 1)
				AddElemD.nSubType = 1;
			else
				AddElemD.nSubType = 0;
		}
	}
	else if(m_pDoc->m_pAttrCtrl->IsPlstrs(AddElemD.eltyp))
	{
		if(m_bDOF)
			AddElemD.nSubType = 1;
		else
			AddElemD.nSubType = 0;
	}
	else
		ASSERT(0);   
	
	int nNodeStartIndex=0;
	int nElemStartIndex=0;

	BOOL bDel = FALSE;
	int  nAnswer = IDNO;
	CArray<T_PRES_K,T_PRES_K>  aPresK; 
	CArray<T_PRES_D,T_PRES_D&> aPresD; 
	double PlandLocalVec[3][3];
	CArray<UINT,UINT> aFailurePlane; aFailurePlane.RemoveAll();
	CArray<UINT,UINT> aFailureMesh;  aFailureMesh.RemoveAll();
	aDelElemK.reserve(m_aKeyList.size());
	for(i=0; i<m_aKeyList.size(); i++)
	{ 
		m_mapElemKeyList.RemoveAll();     
		m_mapNodeKeyList.RemoveAll(); 
		m_aEdge.clear();
		m_aSelectNode.RemoveAll();

		ElemD.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(m_aKeyList[i],ElemD);
		if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp)) continue;
		nNodeNum = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp,ElemD.elnod);

		if(nAnswer == IDNO)
		{            
			if(!m_pDoc->calcPlaneLocalVector(m_aKeyList[i], PlandLocalVec)) continue;

			m_pDoc->m_pAttrCtrl->GetPres(m_aKeyList[i], aPresK, aPresD);    
			for(j=0; j<aPresD.GetSize(); j++)
			{
				if(aPresD[j].LoadType != 1 || aPresD[j].nSubLoadType != 0)
					bDel = TRUE;      
				else
				{
					double p1=aPresD[j].Force[1];
					double p2=aPresD[j].Force[2];
					double p3=aPresD[j].Force[3];
					double p4=aPresD[j].Force[4];
					if(p1 == p2 && p1 == p3 && p1 == p4)
						continue;     

					if(nNodeNum == 4)
					{
						if( (p1 == p2 && p3 == p4) || (p1 == p4 && p2 == p3) )
						{
							if(aPresD[j].LoadDirectionNew == 3) // local z
								continue;
							else if(aPresD[j].LoadDirectionNew == 4) // Global X
							{
								if(PlandLocalVec[0][0] == 0 && PlandLocalVec[0][1] == 0 && fabs(PlandLocalVec[0][2]) == 1)
									continue;            
							}
							else if(aPresD[j].LoadDirectionNew == 5) // Global Y
							{
								if(PlandLocalVec[1][0] == 0 && PlandLocalVec[1][1] == 0 && fabs(PlandLocalVec[1][2]) == 1)
									continue;            
							}
							else if(aPresD[j].LoadDirectionNew == 6) // Global Z
							{
								if(PlandLocalVec[2][0] == 0 && PlandLocalVec[2][1] == 0 && fabs(PlandLocalVec[2][2]) == 1)
									continue;                            
							}
							else if(aPresD[j].LoadDirectionNew == 7)  // vector
							{
								double dVecAngle = mathCrossAngleNormalize(PlandLocalVec[2], aPresD[j].Vector);          
								if( CCompFunc::CompRealTol(dVecAngle, 0  , 0.1) == 0 ||  
										CCompFunc::CompRealTol(dVecAngle, 180, 0.1) == 0 || 
										CCompFunc::CompRealTol(dVecAngle, 360, 0.1) == 0 ) 
								{
									continue;
								}
							}
						}
					}
					else
					{
						if(p1 == p2 && p1 == p3)
						{
							if(aPresD[j].LoadDirectionNew == 3) // local z
								continue;
							else if(aPresD[j].LoadDirectionNew == 6) // Global z
							{
								if(PlandLocalVec[2][0] == 0 && PlandLocalVec[2][1] == 0 && fabs(PlandLocalVec[2][2]) == 1)
									continue;                            
							}
							else if(aPresD[j].LoadDirectionNew == 7)  // vector
							{
								if(fabs(aPresD[j].Vector[2]) == 1)
									continue;
							}
						}
					}          
					bDel = TRUE;
				}
				if(bDel)
				{
					nAnswer = AfxMessageBox(_LS(IDS_WG_TM_Warning__Del_Pressure_load), MB_YESNO);       
					if(nAnswer == IDYES) break;
					else                 return;
				}
			}      
		}
		
		for(j=0; j<nNodeNum; j++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j], NodeD1)) { ASSERT(0); return; }
			ptStart.x = NodeD1.x;
			ptStart.y = NodeD1.y;
			ptStart.z = NodeD1.z;
			
			// Planar Element의 마지막 Node와 첫번째 Node를 연결하여 Edge 생성
			if(j==nNodeNum-1)
			{
				if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD2)) { ASSERT(0); return; }  
			}
			else
			{
				if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j+1], NodeD2)) { ASSERT(0); return; }      
			}
			ptEnd.x = NodeD2.x;
			ptEnd.y = NodeD2.y;
			ptEnd.z = NodeD2.z;
			
			LineG.Set(ptStart, ptEnd);
			AddEdge(m_aEdge, LineG);   
			
			m_mapNodeKeyList.SetAt(ElemD.elnod[j], temp);    
		}
		if(m_aEdge.size() <= 0) {ASSERT(0); continue;}

		if(m_bBndrConnect)  
			DivideEdge(m_aEdge);  

		m_mapElemKeyList.SetAt(m_aKeyList[i], temp);

		CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&> aMeshElemD;
		CArray<T_MESH_NODE_D,T_MESH_NODE_D&> aMeshNodeD;
		std::vector<T_NODE_D> aHoopNodeData_Temp;
		std::vector<int> aDomainElemCount;
		double dPlanePoint[3][3]={0};
		int nRet = CreateMeshByEdge(aMeshElemD, aMeshNodeD, aDomainElemCount, dPlanePoint, aHoopNodeData_Temp);
		if(nRet == -1)
		{
			aFailurePlane.Add(m_aKeyList[i]);
			continue;   
		}
		else if(nRet == -2)
		{
			aFailureMesh.Add(m_aKeyList[i]);
			continue;
		}
		else if(nRet == 0)
		{
			ASSERT(0);
			return;
		}

		if(aHoopNodeData_Temp.size() > 0)
			std::copy(aHoopNodeData_Temp.begin(), aHoopNodeData_Temp.end(), std::back_inserter(aHoopNodeData));
		
		std::vector<T_ELEM_K> arElemK_Temp; 
		std::vector<T_ELEM_D> arElemD_Temp;
		std::vector<T_NODE_K> arNodeK_Temp; 
		std::vector<T_NODE_D> arNodeD_Temp; 
		MakeNodeElemList_SubDomain(aMeshElemD, aMeshNodeD, arElemK_Temp, arElemD_Temp, arNodeK_Temp, arNodeD_Temp, aDomainElemCount, nNodeStartIndex, nElemStartIndex);

		int nTempNodeKSize = arNodeK_Temp.size();
		int nTempElemKSize = arElemK_Temp.size();

		nNodeStartIndex += nTempNodeKSize;
		nElemStartIndex += nTempElemKSize;

		if(nTempNodeKSize > 0)
		{
			std::copy(arNodeK_Temp.begin(), arNodeK_Temp.end(), std::back_inserter(arNodeK));
			std::copy(arNodeD_Temp.begin(), arNodeD_Temp.end(), std::back_inserter(arNodeD));
		}
		if(nTempElemKSize > 0)
		{
			std::copy(arElemK_Temp.begin(), arElemK_Temp.end(), std::back_inserter(arElemK));
			std::copy(arElemD_Temp.begin(), arElemD_Temp.end(), std::back_inserter(arElemD));
		}
		else
			continue;      
				
		CArray<int,int>* aSbdoIndex = new CArray<int,int>;
		CArray<T_SBDO_ELEMLIST_D, T_SBDO_ELEMLIST_D&> aSbdoElemList_Temp;
		MakeSubDomainData(aDomainElemCount, dPlanePoint, arElemK_Temp, aSbdoElemList_Temp);    
		for(j=0; j<aSbdoElemList_Temp.GetSize(); j++)  
		{
			aSbdoIndex->Add(aSbdoElemList.GetSize());

			int nSbdoName = j+aSbdoElemList.GetSize()+1;
			aSbdoElemList_Temp[j].SbdoD.strName.Format(_T("[%d]"), nSbdoName);
			aSbdoElemList.Add(aSbdoElemList_Temp[j]);
		}
		
		// plate method일 경우 mesh한 원본 element를 지운다.  
		aDelElemK.push_back(m_aKeyList[i]);
		aIncludeSbdoIndex.Add(aSbdoIndex);

#ifdef _MGEN_CH
		if (m_aEdge.size() <= 0)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_elem_list_));

			for (int i = 0; i < aIncludeSbdoIndex.GetSize(); i++)
			{
				if (aIncludeSbdoIndex[i] != NULL)
				{
					delete aIncludeSbdoIndex[i];
					aIncludeSbdoIndex[i] = NULL;
				}
			}
			aIncludeSbdoIndex.RemoveAll();
			return;
		}
#endif
	}  

#ifndef _MGEN_CH
	if(m_aEdge.size() <= 0) 
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_elem_list_));

		for(int i=0; i<aIncludeSbdoIndex.GetSize(); i++)
		{
			if(aIncludeSbdoIndex[i] != NULL)
			{
				delete aIncludeSbdoIndex[i];
				aIncludeSbdoIndex[i] = NULL;
			}
		}
		aIncludeSbdoIndex.RemoveAll();
		return;
	}  
#endif

	T_MADO_D MadoD; 
	MakeMainDomainData(MadoD);  

	CString strWarnMsg;
	T_3COOR Pt; Pt.Initialize();
#ifdef _MGEN_CH
	if(!Get3Point(m_wndDirPt1, Pt.x, Pt.y, Pt.z))
		return;
#endif
	if(!m_pDoc->m_pDataCtrl->AddNodeElemMadoSbdoList(arNodeK, arNodeD, arElemK, arElemD, MadoD, aSbdoElemList, aHoopNodeData, m_bDeleteSrcLine, m_bSubdivideSrcLine, aDelElemK, aIncludeSbdoIndex, m_DpaaK, m_CcpaK, m_MasterNodeK, &Pt) ) 
	{
		for(i=0; i<aIncludeSbdoIndex.GetSize(); i++)
		{
			if(aIncludeSbdoIndex[i] != NULL)
			{        
				delete aIncludeSbdoIndex[i];
				aIncludeSbdoIndex[i] = NULL;
			}
		}
		aIncludeSbdoIndex.RemoveAll();
		for(i=0; i<aFailurePlane.GetSize(); i++)
		{
			strWarnMsg.Format(_LS(IDS_WG_TREEMENU_Error___Invalid_co_planar_nodes_of_Elem__d), aFailurePlane[i]);
			m_pDoc->DisplayHistoryMessage(strWarnMsg);       
		}
		for(i=0; i<aFailureMesh.GetSize(); i++)
		{
			strWarnMsg.Format(_LS(IDS_WG_TREEMENU_Error___failed_create_mesh_Elem__d), aFailureMesh[i]);
			m_pDoc->DisplayHistoryMessage(strWarnMsg);       
		}
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_FAILED_CREATE_MESH));    
		return;
	}  

	for(i=0; i<aIncludeSbdoIndex.GetSize(); i++)
	{
		if(aIncludeSbdoIndex[i] != NULL)
		{
			delete aIncludeSbdoIndex[i];
			aIncludeSbdoIndex[i] = NULL;
		}    
	}
	aIncludeSbdoIndex.RemoveAll();  
	for(i=0; i<aFailurePlane.GetSize(); i++)
	{
		strWarnMsg.Format(_LS(IDS_WG_TREEMENU_Error___Invalid_co_planar_nodes_of_Elem__d), aFailurePlane[i]);
		m_pDoc->DisplayHistoryMessage(strWarnMsg);       
	}
	for(i=0; i<aFailureMesh.GetSize(); i++)
	{
		strWarnMsg.Format(_LS(IDS_WG_TREEMENU_Error___failed_create_mesh_Elem__d), aFailureMesh[i]);
		m_pDoc->DisplayHistoryMessage(strWarnMsg);       
	}   
	EndMesh();
}


BOOL CMeshAutoDlg::Get3Point(CMouseEdit& pt, double& Px, double& Py, double& Pz)
{
	CString strPos;
	pt.GetWindowText(strPos);
	return GetPosition(strPos, Px, Py, Pz);
}

void CMeshAutoDlg::MeshByLine()
{
	if (m_aKeyList.size() < 3)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_elem_list_));
		return;
	}

	T_POINT3D ptStart, ptEnd;
	T_NODE_D NodeD, NodeD1, NodeD2;
	T_ELEM_D ElemD;
	T_LINE_G LineG;
	
	for (int i = 0; i < m_aKeyList.size(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetElem(m_aKeyList[i], ElemD)) continue;
		if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) { ASSERT(0); return; }
		if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
		if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }

		ptStart.Set(NodeD1.x, NodeD1.y, NodeD1.z);
		ptEnd.Set(NodeD2.x, NodeD2.y, NodeD2.z);

		LineG.Set(ptStart, ptEnd);
		AddEdge(m_aEdge, LineG);
	}

	if (m_aEdge.size() <= 0)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_elem_list_));
		return;
	}

	int i, j, k, n, nNodeNum, temp = 0;
	CString strKeyList;

	//////////////////////////////////////////////////////////////////////////
	// Interior Line - User  

	// Node를 판단하기 전에 Line을 먼저 찾아서 등록해야 한다.
	if (m_bInteriorLine && m_nOption_Line == 1)
	{
		m_wndElemListEdit.GetWindowText(strKeyList);
		std::vector<UINT> aKeyList;
		GetNodeListRemoveDup(strKeyList, aKeyList);

		std::vector<T_LINE_G> aIntersectEdge;
		aIntersectEdge.reserve(aKeyList.size()*0.5);
		for (int i = 0; i < aKeyList.size(); i++)
		{
			// Line Method일 경우 사용자가 Select Edit에서 선택한 Edge와 중복된 것이 있으면 skip
			if (m_cmbMethod.GetCurSel() == 1 && m_mapElemKeyList.Lookup(aKeyList[i], temp)) continue;
			if (!m_pDoc->m_pAttrCtrl->GetElem(aKeyList[i], ElemD)) { ASSERT(0); return; }

			nNodeNum = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);

			// 선택한 Inner Line Element를 Edge 정보에 포함
			if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
			{
				if (!GetSamePlaneElementEdge(m_aEdge, ElemD, aIntersectEdge))
				{
					m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_co_planar_line));
					return;
				}

				if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
				ptStart.Set(NodeD1.x, NodeD1.y, NodeD1.z);

				if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }
				ptEnd.Set(NodeD2.x, NodeD2.y, NodeD2.z);

				LineG.Set(ptStart, ptEnd);
				AddEdge(m_aEdge, LineG);
			}
			else if (m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
			{
				// plane요소가 사용자가 선택한 평면과 같은 평면에 있는 부분을 찾아서 edge를 만든다.
				if (!GetSamePlaneElementEdge(m_aEdge, ElemD, aIntersectEdge))
				{
					m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_co_planar_line));
					return;
				}

				for (j = 0; j < aIntersectEdge.size(); j++)
					AddEdge(m_aEdge, aIntersectEdge[j]);
			}
			else
				continue;

			// Elem의 node를 m_mapNodeKeyList에 등록        
			for (j = 0; j < nNodeNum; j++)
				m_mapNodeKeyList.SetAt(ElemD.elnod[j], temp);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Interior Node - User
	T_POINT3D ptNode;
	if (m_bInteriorNode && m_nOption_Node == 1)
	{
		m_wndNodeListEdit.GetWindowText(strKeyList);
		std::vector<UINT> aKeyList;
		GetNodeListRemoveDup(strKeyList, aKeyList);

		for (int i = 0; i < aKeyList.size(); i++)
		{
			if (m_mapNodeKeyList.Lookup(aKeyList[i], temp)) continue;

			if (!m_pDoc->m_pAttrCtrl->GetNode(aKeyList[i], NodeD1)) { ASSERT(0); return; }
			ptNode.Set(NodeD1.x, NodeD1.y, NodeD1.z);

			m_aSelectNode.Add(ptNode);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 평면을 구성하는 Normal vec를 XY평면의 Normal vec로 변환하여 2D 좌표계 구성 
	// 1. Plane Normal vec와 XY 평면의 Normal vec를 cross 하여 회전축 생성
	// 2. Plane Normal vec와 XY 평면의 Normal vec의 각도만큼 회전
	// 3. mesh가 끝난후 다시 원복 
	double PlaneLocalVector[3][3] = { 0 };
	double dPlanePoint[3][3] = { 0 };
	if (!ConfirmInPlane(m_aEdge, m_aSelectNode, PlaneLocalVector, dPlanePoint))
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_co_planar_line));
		return;
	}

	double dXYPlane[3] = { 0,0,1 };
	double dAxisVec[3];
	mathCross(dXYPlane, PlaneLocalVector[2], dAxisVec);
	double dRotAngle = mathCrossAngleNormalize(dXYPlane, PlaneLocalVector[2]);

	// Edge 변환
	for (int i = 0; i < m_aEdge.size(); i++)
	{
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], m_aEdge[i].p1.x, m_aEdge[i].p1.y, m_aEdge[i].p1.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], m_aEdge[i].p2.x, m_aEdge[i].p2.y, m_aEdge[i].p2.z);
	}
	// Node 변환
	for (int i = 0; i < m_aSelectNode.GetSize(); i++)
	{
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], m_aSelectNode[i].x, m_aSelectNode[i].y, m_aSelectNode[i].z);
	}
	//////////////////////////////////////////////////////////////////////////

	// 여기서 line list들로 부터 Boundary list 들을 뽑아 주두/지판의 Edge를 추가해야 함..
	CArray<T_LINE_G, T_LINE_G&> aEdge_Drop;  // 주두에 대한 Edge
	CArray<T_LINE_G, T_LINE_G&> aEdge_Colm;  // 지판에 대한 Edge
	MakeEdgeByDropPanelNColmCapital(aEdge_Drop, aEdge_Colm, dPlanePoint, dRotAngle, dAxisVec);

	std::vector<T_LINE_G> aEdge_copy; // 주두 지판 영역을 넣기 전의 line
	aEdge_copy.resize(m_aEdge.size());
	std::copy(m_aEdge.begin(), m_aEdge.end(), aEdge_copy.begin());

	std::vector<T_LINE_G> aBoundaryEdge;
	std::vector<T_LINE_G> aBoundaryEdge_temp;
	int nPathCount = FindClosePath();
	for (int i = 0; i < nPathCount; i++)
	{
		// 일직선상의 부재는 하나로 통합하여 다각형을 만든다. 4각형이 아니면 생성X
		MakePolyLineMergingStraightLine(m_aClosedPath[i].aNodeId, aBoundaryEdge_temp);

		// Boundary Edge 변환
		for (k = 0; k < aBoundaryEdge_temp.size(); k++)
		{
			mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aBoundaryEdge_temp[k].p1.x, aBoundaryEdge_temp[k].p1.y, aBoundaryEdge_temp[k].p1.z);
			mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aBoundaryEdge_temp[k].p2.x, aBoundaryEdge_temp[k].p2.y, aBoundaryEdge_temp[k].p2.z);
		}
		if (aBoundaryEdge_temp.size() > 0) aBoundaryEdge.insert(aBoundaryEdge.end(), aBoundaryEdge_temp.begin(), aBoundaryEdge_temp.end());
	}

	// 주두/지판에 따른 Edge 추가
	CMapEx<int, int, BOOL, BOOL> mEdgeConnectBndr;
	MakeEdgeConnectInfo(aEdge_copy, aBoundaryEdge, mEdgeConnectBndr, aEdge_Drop, aEdge_Colm);
	if (!AddEdgeByDropPanelNColmCapital(m_aEdge, aEdge_copy, aBoundaryEdge, aEdge_Drop, aEdge_Colm, m_aSelectNode, mEdgeConnectBndr)) { ASSERT(0); return; }

	if (m_aSelectNode.GetSize() > 0)
		DivideEdgeByIncludeNode(m_aEdge, m_aSelectNode);

	// [MQC 9341] AutoMesh할 때 엣지의 Seed가 중복되어서 들어가는 경우가 생김(주두나 지판 영역의 선과 m_aEdge가 겹치는 경우)
	// 그 때 겹치는 것을 빼도록 함 2014-12-16 : JWKWON
	RemoveRedundentEdge(m_aEdge);

	CArray<double, double&> aSeedSize;
	MakeSeedSizeInfo(m_aEdge, aSeedSize, dRotAngle, dAxisVec);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Perform Mesh
	CAutoMesh_PlanarArea Mesher;
	int NbIndividual = 1;
	int nIth = 0;
	CArray<T_POLYLINE3D*, T_POLYLINE3D*> aDomainPolyLine;
	std::vector<int> aDomainElemCount; aDomainElemCount.clear();
	m_nChangeMethod = Mesher.AutoMesh_PlanarArea(m_aEdge, m_aSelectNode, AutoMeshArg, NbIndividual, nIth, aDomainElemCount, m_dMinMeshSize, aSeedSize, aDomainPolyLine);
	if (m_nChangeMethod == -1)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_FAILED_CREATE_MESH));
		return;
	}
	else if (m_nChangeMethod == 1 && m_nChangeMethod == 2)
	{
		AutoMeshArg.MesherType = D_MESHTYPE_TRIAQUAD;
	}
	CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD = Mesher.AllElems();
	CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD = Mesher.AllNodes();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////   
	double dPoint[3];
	std::vector<T_NODE_K> aIncludeNodeKeyList;
	std::vector<T_NODE_D> aIncludeNodeDataList;
	std::vector<T_NODE_K> aAllNodeKeyList;
	if ((m_bInteriorLine && m_nOption_Line == 0) || (m_bInteriorNode && m_nOption_Node == 0))
	{
		m_pDoc->m_pAttrCtrl->GetNodeKeyList(aAllNodeKeyList);
		for (int i = 0; i < aAllNodeKeyList.size(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNode(aAllNodeKeyList[i], NodeD)) { ASSERT(0); return; }
			dPoint[0] = NodeD.x;  dPoint[1] = NodeD.y;  dPoint[2] = NodeD.z;

			// 전체 노드중에서 평면과 일치하는 node를 걸러낸다.
			if (!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol)) continue;

			// Mesh된 Domain의 2D 좌표계로 변환.
			mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], NodeD.x, NodeD.y, NodeD.z);

			aIncludeNodeDataList.push_back(NodeD);
			aIncludeNodeKeyList.push_back(aAllNodeKeyList[i]);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	int nDomainCount = aDomainPolyLine.GetSize();

	//////////////////////////////////////////////////////////////////////////
	// Interior Line - Auto
	BOOL bRemesh = FALSE;
	int nData;
	double p1[2], p2[2];
	if (m_bInteriorLine && m_nOption_Line == 0)
	{
		double dLine[2][2] = { 0 };

		// Domain의 Boundary에 포함되는지 판단.     
		for (int i = 0; i < nDomainCount; i++)
		{
			nData = aDomainPolyLine[i]->GetSize();
			double(*dPolyLine)[2];
			dPolyLine = new double[nData][2];
			for (j = 0; j < nData; j++)
			{
				dPolyLine[j][0] = aDomainPolyLine[i]->GetAt(j).x;
				dPolyLine[j][1] = aDomainPolyLine[i]->GetAt(j).y;
			}

			if (nData < 3)
			{
				delete[]dPolyLine;
				continue;
			}
			for (j = 0; j < aIncludeNodeDataList.size(); j++)
			{
				p1[0] = aIncludeNodeDataList[j].x;
				p1[1] = aIncludeNodeDataList[j].y;
				if (!CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect)) continue;

				std::vector<T_ELEM_K>  aConnectedElemK;
				// 해당 node에 연결된 element를 찾음.
				m_pDoc->m_pAttrCtrl->GetConnectedElem(aIncludeNodeKeyList[j], aConnectedElemK);
				for (k = 0; k < aConnectedElemK.size(); k++)
				{
					if (m_mapElemKeyList.Lookup(aConnectedElemK[k], temp)) continue;

					ElemD.Initialize();
					if (!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElemK[k], ElemD))
					{
						ASSERT(0);
						delete[]dPolyLine;
						return;
					}
					nNodeNum = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);

					// Element의 node중에서 convex hull에 포함되는 다른 node가 있는지 검사
					if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
					{
						// 먼저 두점이 3D 상에서 같은 평면인지 검사 
						if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1))
						{
							ASSERT(0);
							delete[]dPolyLine;
							return;
						}
						dPoint[0] = NodeD1.x;  dPoint[1] = NodeD1.y;  dPoint[2] = NodeD1.z;
						if (!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
							continue;

						if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2))
						{
							ASSERT(0);
							delete[]dPolyLine;
							return;
						}
						dPoint[0] = NodeD2.x;  dPoint[1] = NodeD2.y;  dPoint[2] = NodeD2.z;
						if (!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
							continue;

						// Domain의 2D 좌표계로 변환. 
						mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], NodeD1.x, NodeD1.y, NodeD1.z);
						p1[0] = NodeD1.x;
						p1[1] = NodeD1.y;

						mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], NodeD2.x, NodeD2.y, NodeD2.z);
						p2[0] = NodeD2.x;
						p2[1] = NodeD2.y;

						// 2D Domain안에 포함되는 지 검사
						if (CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect) && CMathFunc::mathIsInsidePoint2D_Tol(p2, nData, dPolyLine, m_dTol, m_bBndrConnect))
						{
							dLine[0][0] = p1[0]; dLine[0][1] = p1[1];
							dLine[1][0] = p2[0]; dLine[1][1] = p2[1];
							double dInsideLength = CMathFunc::mathsInsideLength_Tol(dLine, nData, dPolyLine, m_dLengthTol);
							if (dInsideLength <= m_dTol) continue;

							ptStart.Set(p1[0], p1[1], NodeD1.z);
							ptEnd.Set(p2[0], p2[1], NodeD2.z);

							LineG.Set(ptStart, ptEnd);
							AddEdge(m_aEdge, LineG);

							m_mapNodeKeyList.SetAt(ElemD.elnod[0], temp);
							m_mapNodeKeyList.SetAt(ElemD.elnod[1], temp);
							m_mapElemKeyList.SetAt(aConnectedElemK[k], temp);
							bRemesh = TRUE;
						}
					}
					else if (m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
					{
						for (n = 0; n < nNodeNum; n++)
						{
							if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[n], NodeD1))
							{
								ASSERT(0);
								delete[]dPolyLine;
								return;
							}
							dPoint[0] = NodeD1.x;  dPoint[1] = NodeD1.y;  dPoint[2] = NodeD1.z;
							if (!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
								continue;

							int nNodeIndex;
							if (n == nNodeNum - 1)
							{
								nNodeIndex = 0;
								if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[nNodeIndex], NodeD2))
								{
									ASSERT(0);
									delete[]dPolyLine;
									return;
								}
							}
							else
							{
								nNodeIndex = n + 1;
								if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[nNodeIndex], NodeD2))
								{
									ASSERT(0);
									delete[]dPolyLine;
									return;
								}
							}
							dPoint[0] = NodeD2.x;  dPoint[1] = NodeD2.y;  dPoint[2] = NodeD2.z;
							if (!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
								continue;

							mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], NodeD1.x, NodeD1.y, NodeD1.z);
							p1[0] = NodeD1.x;
							p1[1] = NodeD1.y;

							mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], NodeD2.x, NodeD2.y, NodeD2.z);
							p2[0] = NodeD2.x;
							p2[1] = NodeD2.y;

							if (CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect) &&
								CMathFunc::mathIsInsidePoint2D_Tol(p2, nData, dPolyLine, m_dTol, m_bBndrConnect))
							{
								dLine[0][0] = p1[0]; dLine[0][1] = p1[1];
								dLine[1][0] = p2[0]; dLine[1][1] = p2[1];

								double dInsideLength = CMathFunc::mathsInsideLength_Tol(dLine, nData, dPolyLine, m_dLengthTol);
								if (dInsideLength <= m_dTol) continue;

								ptStart.Set(p1[0], p1[1], NodeD1.z);
								ptEnd.Set(p2[0], p2[1], NodeD2.z);

								LineG.Set(ptStart, ptEnd);
								AddEdge(m_aEdge, LineG);

								m_mapNodeKeyList.SetAt(ElemD.elnod[n], temp);
								m_mapNodeKeyList.SetAt(ElemD.elnod[nNodeIndex], temp);
								m_mapElemKeyList.SetAt(aConnectedElemK[k], temp);
								bRemesh = TRUE;
							}
						}
					}
					else
						continue;
				}
			}
			delete[]dPolyLine;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// Interior Node - Auto
	if (m_bInteriorNode && m_nOption_Node == 0)
	{
		for (int i = 0; i < nDomainCount; i++)
		{
			nData = aDomainPolyLine[i]->GetSize();
			double(*dPolyLine)[2];
			dPolyLine = new double[nData][2];
			for (j = 0; j < aDomainPolyLine[i]->GetSize(); j++)
			{
				dPolyLine[j][0] = aDomainPolyLine[i]->GetAt(j).x;
				dPolyLine[j][1] = aDomainPolyLine[i]->GetAt(j).y;
			}

			if (nData < 3)
			{
				delete[]dPolyLine;
				continue;
			}

			for (j = 0; j < aIncludeNodeDataList.size(); j++)
			{
				if (m_mapNodeKeyList.Lookup(aIncludeNodeKeyList[j], temp)) continue;

				p1[0] = aIncludeNodeDataList[j].x;
				p1[1] = aIncludeNodeDataList[j].y;
				if (!CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect)) continue;

				m_mapNodeKeyList.SetAt(aIncludeNodeKeyList[j], temp);
				ptNode.Set(aIncludeNodeDataList[j].x, aIncludeNodeDataList[j].y, aIncludeNodeDataList[j].z);
				m_aSelectNode.Add(ptNode);
				bRemesh = TRUE;
			}
			delete[]dPolyLine;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Remesh
	//int nPolyNum = aDomainPolyLine.GetSize();

	if (bRemesh)
	{
		if (m_aSelectNode.GetSize() > 0)
			DivideEdgeByIncludeNode(m_aEdge, m_aSelectNode);

		MakeSeedSizeInfo(m_aEdge, aSeedSize, dRotAngle, dAxisVec);

		aMeshNodeD.RemoveAll();
		aMeshElemD.RemoveAll();
		aDomainElemCount.clear();

		for (i = nDomainCount - 1; i >= 0; i--)
		{
			if (aDomainPolyLine[i] != NULL)
			{
				aDomainPolyLine[i]->RemoveAll();
				delete aDomainPolyLine[i];
				aDomainPolyLine[i] = NULL;
			}
		}
		aDomainPolyLine.RemoveAll();

		int nChangeMethod_temp = Mesher.AutoMesh_PlanarArea(m_aEdge, m_aSelectNode, AutoMeshArg, NbIndividual, nIth, aDomainElemCount, m_dMinMeshSize, aSeedSize, aDomainPolyLine);
		if (nChangeMethod_temp == -1)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_FAILED_CREATE_MESH));
			return;
		}
		else if (m_nChangeMethod == 0)
			m_nChangeMethod = nChangeMethod_temp;
	}


	// 내부 도메인 안만들기 옵션일때에는 주두/지판에 대한 Mesh를 다시 만들어준다.
	if (!m_bInnerDomain)
	{
		std::vector<T_LINE_G> aBoundaryEdge;
		// 주두/지판에 따른 Edge 추가
		std::vector<T_LINE_G> aEdge2;
		CArray<T_POINT3D, T_POINT3D&> aSelectNode; aSelectNode.RemoveAll();
		for (int i = 0; i < nPathCount; i++)
		{
			// 일직선상의 부재는 하나로 통합하여 다각형을 만든다. 4각형이 아니면 생성X
			MakePolyLineMergingStraightLine(m_aClosedPath[i].aNodeId, aBoundaryEdge);

			CMapEx<int, int, BOOL, BOOL> mEdgeConnectBndr;
			MakeEdgeConnectInfo(aEdge_copy, aBoundaryEdge, mEdgeConnectBndr, aEdge_Drop, aEdge_Colm);

			// Boundary Edge 변환
			for (k = 0; k < aBoundaryEdge.size(); k++)
			{
				mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aBoundaryEdge[k].p1.x, aBoundaryEdge[k].p1.y, aBoundaryEdge[k].p1.z);
				mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aBoundaryEdge[k].p2.x, aBoundaryEdge[k].p2.y, aBoundaryEdge[k].p2.z);
			}

			// 주두/지판에 따른 Edge 추가
			if (!AddEdgeByDropPanelNColmCapital(aEdge2, aEdge_copy, aBoundaryEdge, aEdge_Drop, aEdge_Colm, aSelectNode, mEdgeConnectBndr, TRUE)) return;
		}

		if (aSelectNode.GetSize() > 0)
			DivideEdgeByIncludeNode(aEdge2, aSelectNode);

		if (aEdge2.size() > 0)
		{
			DivideEdge(aEdge2);

			//       CArray<T_POINT3D,T_POINT3D&> aSelectNode2; aSelectNode2.RemoveAll();

			CAutoMesh_PlanarArea Mesher2;
			int NbIndividual2 = 1;
			int nIth2 = 0;

			CArray<double, double&> aSeedSize2;
			//MakeSeedSizeInfo(aEdge2, aSeedSize2, dRotAngle, dAxisVec);


			// Mesh    
			AutoMeshArg.nInnerDomain = 1;
			int nSaveChangeMethod2 = m_nChangeMethod;
			std::vector<int> aDomainElemCount2; aDomainElemCount2.clear();
			CArray<T_POLYLINE3D*, T_POLYLINE3D*> aDomainPolyLine2;
			m_nChangeMethod = Mesher2.AutoMesh_PlanarArea(aEdge2, aSelectNode, AutoMeshArg, NbIndividual2, nIth2, aDomainElemCount2, m_dMinMeshSize, aSeedSize, aDomainPolyLine2);
			if (m_nChangeMethod == -1)  return;

			CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aElemD2 = Mesher2.AllElems();
			CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aNodeD2 = Mesher2.AllNodes();

			//       // 원래의 3D 좌표계로 변환
			//       for(int i=0; i<aNodeD2.GetSize(); i++)
			//       {
			//         CMathFunc::mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
			//           aNodeD2[i].x, aNodeD2[i].y, aNodeD2[i].z);
			//       }

						// 추가되는 Node2이기 때문에 aElemD2에서 생성된 Nodek를 수정해준다.
			int nExistNodeCount = aMeshNodeD.GetSize();
			for (int i = 0; i < aElemD2.GetSize(); i++)
			{
				int nNodeSize = aElemD2[i].aNodeK.GetSize();
				for (int j = 0; j < nNodeSize; j++)
				{
					int nNodeK = aElemD2[i].aNodeK[j];
					aElemD2[i].aNodeK[j] = nExistNodeCount + nNodeK;
				}
			}

			if (aElemD2.GetSize() > 0)
				aMeshElemD.Append(aElemD2);
			if (aNodeD2.GetSize() > 0)
				aMeshNodeD.Append(aNodeD2);
		}

	}

	std::vector<T_NODE_D> aHoopNodeData;
	if (m_bDeleteSrcLine || m_bSubdivideSrcLine)
		MakeHoopNode(aDomainPolyLine, aHoopNodeData);
	for (int i = 0; i < aHoopNodeData.size(); i++)
	{
		mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aHoopNodeData[i].x, aHoopNodeData[i].y, aHoopNodeData[i].z);
	}

	nDomainCount = aDomainPolyLine.GetSize();
	for (i = nDomainCount - 1; i >= 0; i--)
	{
		if (aDomainPolyLine[i] != NULL)
		{
			aDomainPolyLine[i]->RemoveAll();
			delete aDomainPolyLine[i];
			aDomainPolyLine[i] = NULL;
		}
	}
	aDomainPolyLine.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 원래의 3D 좌표계로 변환
	for (int i = 0; i < aMeshNodeD.GetSize(); i++)
	{
		mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aMeshNodeD[i].x, aMeshNodeD[i].y, aMeshNodeD[i].z);
	}
	//////////////////////////////////////////////////////////////////////////

	if (!AddNodeElemMadoSbdoList(aMeshElemD, aMeshNodeD, aDomainElemCount, dPlanePoint, aHoopNodeData))
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_FAILED_CREATE_MESH));
		return;
	}

	EndMesh();
}

void CMeshAutoDlg::MeshByNode()
{
	T_POINT3D ptStart, ptEnd;
	T_NODE_D  NodeD1, NodeD2;  
	T_LINE_G  LineG;  

	for(int i=0; i<m_aKeyList.size(); i++)
	{	
		if(!m_pDoc->m_pAttrCtrl->GetNode(m_aKeyList[i], NodeD1)) { ASSERT(0); return; }
		ptStart.x = NodeD1.x;
		ptStart.y = NodeD1.y;
		ptStart.z = NodeD1.z;
		
		if(i==m_aKeyList.size()-1) // 마지막 node와 첫번째 node를 연결하여 Edge 구성
		{
			if(!m_pDoc->m_pAttrCtrl->GetNode(m_aKeyList[0], NodeD2)) { ASSERT(0); return; }      
		}
		else
		{
			if(!m_pDoc->m_pAttrCtrl->GetNode(m_aKeyList[i+1], NodeD2)) { ASSERT(0); return; }           
		}
		ptEnd.x = NodeD2.x;
		ptEnd.y = NodeD2.y;
		ptEnd.z = NodeD2.z;
		
		LineG.Set(ptStart, ptEnd);
		AddEdge(m_aEdge, LineG);      
	}
	
	if(m_aEdge.size() <= 0) 
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
		return;
	}  

	if(m_bBndrConnect)  
		DivideEdge(m_aEdge);  

	CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&> aMeshElemD;
	CArray<T_MESH_NODE_D,T_MESH_NODE_D&> aMeshNodeD;
	std::vector<T_NODE_D> aHoopNodeData;
	std::vector<int> aDomainElemCount;
	double dPlanePoint[3][3]={0};
	int nRet = CreateMeshByEdge(aMeshElemD, aMeshNodeD, aDomainElemCount, dPlanePoint, aHoopNodeData);
	if(nRet == -1)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Error___Invalid_co_planar_node));
		return;   
	}
	else if(nRet == -2)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_FAILED_CREATE_MESH));
		return;   
	}
	else if(nRet == 0)
	{
		ASSERT(0);
		return;
	}

	if (!AddNodeElemMadoSbdoList(aMeshElemD, aMeshNodeD, aDomainElemCount, dPlanePoint, aHoopNodeData))
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_TM_FAILED_CREATE_MESH));
		return;
	}

	EndMesh();
}

BOOL CMeshAutoDlg::AddNodeElemMadoSbdoList(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD, std::vector<int>& aDomainElemCount, double dPlanePoint[3][3], std::vector<T_NODE_D>& aHoopNodeData)
{
	std::vector<T_ELEM_K> arElemK;
	std::vector<T_ELEM_D> arElemD;
	std::vector<T_NODE_K> arNodeK;
	std::vector<T_NODE_D> arNodeD;
	MakeNodeElemList_SubDomain(aMeshElemD, aMeshNodeD, arElemK, arElemD, arNodeK, arNodeD, aDomainElemCount);

	T_MADO_D MadoD;
	MakeMainDomainData(MadoD);

	CArray<T_SBDO_ELEMLIST_D, T_SBDO_ELEMLIST_D&> aSbdoElemList;
	MakeSubDomainData(aDomainElemCount, dPlanePoint, arElemK, aSbdoElemList);
	
	T_3COOR Pt; Pt.Initialize();
#ifdef _MGEN_CH
	if (!Get3Point(m_wndDirPt1, Pt.x, Pt.y, Pt.z))
		return FALSE;
#else
	// Planar Elem은 별도 함수로 처리함. Line과 Node만 이곳에서 처리됨.
	if (aMeshElemD.GetSize() >= 1)
	{
		T_MESH_ELEM_D& crFirstElemD = aMeshElemD[0];
		if (crFirstElemD.aNodeK.GetSize() >= 3)
		{
			const T_NODE_D& DataNode1 = arNodeD[crFirstElemD.aNodeK[0]];
			const T_NODE_D& DataNode2 = arNodeD[crFirstElemD.aNodeK[1]];

			Pt.Set((DataNode1.x - DataNode2.x), (DataNode1.y - DataNode2.y), (DataNode1.z - DataNode2.z));
			CMathFunc::mathNormalize(Pt.x, Pt.y, Pt.z, Pt.x, Pt.y, Pt.z);
		}
	}
#endif

	M_PROFILE('AddNodeElemMadoSbdoList');
	std::vector<T_ELEM_K> aDelElemK; aDelElemK.clear();
	CArray<CArray<int, int>*, CArray<int, int>*> aIncludeSbdoIndex; aIncludeSbdoIndex.RemoveAll();

	if (!m_pDoc->m_pDataCtrl->AddNodeElemMadoSbdoList(arNodeK, arNodeD, arElemK, arElemD, MadoD, aSbdoElemList, aHoopNodeData, m_bDeleteSrcLine, m_bSubdivideSrcLine, aDelElemK, aIncludeSbdoIndex, m_DpaaK, m_CcpaK, m_MasterNodeK, &Pt))
	{
		return FALSE;
	}	

	return TRUE;
}

int CMeshAutoDlg::CreateMeshByEdge(CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& arMeshElemD, CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& arMeshNodeD, std::vector<int>& aDomainElemCount, double dPlanePoint[3][3], std::vector<T_NODE_D>& aHoopNodeData)
{  
	int i, j, k, nNodeNum, nData, temp=0;
	T_POINT3D ptStart, ptEnd, ptNode;
	T_ELEM_D  ElemD;
	T_NODE_D  NodeD, NodeD1, NodeD2;  
	T_LINE_G  LineG;
	CString   strKeyList;
	T_POLYLINE2D Polyline2D;
	double p1[2], p2[2];
	double dPoint[3];
	m_dMinMeshSize=0;
	m_dMaxBoundarySize=-DBL_MAX; 

	if(!GetPlanePoint(m_aEdge, dPlanePoint)) {ASSERT(0); return 0;}
	
	CArray<T_NODE_K,T_NODE_K>  aIncludeNodeKeyList;
	CArray<T_NODE_K,T_NODE_K>  aAllNodeKeyList;
	CArray<T_NODE_D,T_NODE_D&> aIncludeNodeDataList; 
	
	CArray<int,int> aKeyList;
	CMapEx<int,int,int,int> mapUserElemKeyList;
	CMapEx<int,int,int,int> mapUserNodeKeyList;
	
	strKeyList = _T("");
	aKeyList.RemoveAll();
	m_wndElemListEdit.GetWindowText(strKeyList);    
	GetNodeList(strKeyList,aKeyList);    
	for(int i=0; i<aKeyList.GetSize(); i++)  
		mapUserElemKeyList.SetAt(aKeyList[i], temp);     
	
	strKeyList = _T("");
	aKeyList.RemoveAll();        
	m_wndNodeListEdit.GetWindowText(strKeyList);
	GetNodeList(strKeyList,aKeyList);    
	for(int i=0; i<aKeyList.GetSize(); i++)  
		mapUserNodeKeyList.SetAt(aKeyList[i], temp);   
	
	// 평면을 구성하는 Normal vec를 XY평면의 Normal vec로 변환하여 2D 좌표계 구성 
	// 1. Plane Normal vec와 XY 평면의 Normal vec를 cross 하여 회전축 생성
	// 2. Plane Normal vec와 XY 평면의 Normal vec의 각도만큼 회전
	// 3. mesh가 끝난후 다시 원복 
	double PlaneLocalVector[3][3]={0};
	if(!ConfirmInPlane(m_aEdge, m_aSelectNode, PlaneLocalVector, dPlanePoint))  
		return -1;      
		
	double dXYPlane[3] = {0,0,1};  
	double dAxisVec[3];
	mathCross(dXYPlane, PlaneLocalVector[2], dAxisVec);
	double dRotAngle = mathCrossAngleNormalize(dXYPlane,PlaneLocalVector[2]);
	
	mathNormalize(dAxisVec[0], dAxisVec[1], dAxisVec[2], dAxisVec[0], dAxisVec[1], dAxisVec[2]);
	std::vector<T_LINE_G> aBoundaryEdge;
	aBoundaryEdge.reserve(m_aEdge.size());
	// Edge 변환
	for(int i=0; i<m_aEdge.size(); i++)
	{
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], m_aEdge[i].p1.x, m_aEdge[i].p1.y, m_aEdge[i].p1.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], m_aEdge[i].p2.x, m_aEdge[i].p2.y, m_aEdge[i].p2.z);
		
		// Boundary 저장
		aBoundaryEdge.push_back(m_aEdge[i]);
	}
	// Node 변환
	for(int i=0; i<m_aSelectNode.GetSize(); i++)
	{    
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], m_aSelectNode[i].x, m_aSelectNode[i].y, m_aSelectNode[i].z);
	} 
	
	if(m_bInteriorLine || m_bInteriorNode)
	{
		m_pDoc->m_pAttrCtrl->GetNodeKeyList(aAllNodeKeyList);
		for(int i=0; i<aAllNodeKeyList.GetSize(); i++)
		{
			//if(m_mapNodeKeyList.Lookup(aAllNodeKeyList[i], temp)) continue;
		
			if(!m_pDoc->m_pAttrCtrl->GetNode(aAllNodeKeyList[i], NodeD)) {ASSERT(0); return 0;}
			dPoint[0] = NodeD.x;  dPoint[1] = NodeD.y;  dPoint[2] = NodeD.z;
		
			// 전체 노드중에서 평면과 일치하는 node를 걸러낸다.
			if(mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
			{
				aIncludeNodeDataList.Add(NodeD);
				aIncludeNodeKeyList.Add(aAllNodeKeyList[i]);
			}      
		}

		// Mesh된 Domain의 2D 좌표계로 변환.
		for(int i=0; i<aIncludeNodeDataList.GetSize(); i++)
		{              
			mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aIncludeNodeDataList[i].x, aIncludeNodeDataList[i].y, aIncludeNodeDataList[i].z);
		} 
	}

	CMap<UINT,UINT,T_LINE_G,T_LINE_G&> mapIncludeEdge;
	if(m_bInteriorLine)
	{ 
		double dLine[2][2] = {0}; 

		// Domain의 Boundary에 포함되는지 판단.           
		nData = aBoundaryEdge.size(); 
		double (*dPolyLine)[2];		
		dPolyLine = new double[nData][2];     
		for(int i=0; i<nData; i++)
		{
			dPolyLine[i][0] = aBoundaryEdge[i].p2.x;
			dPolyLine[i][1] = aBoundaryEdge[i].p2.y;
		}

		if(nData < 3) 
		{
			delete []dPolyLine; 
			return 0;
		}
		for(int i=0; i<aIncludeNodeDataList.GetSize(); i++)
		{
			p1[0] = aIncludeNodeDataList[i].x;
			p1[1] = aIncludeNodeDataList[i].y;  
			
			if(!CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect)) continue;
			
			std::vector<T_ELEM_K>  aConnectedElemK;
			// 해당 node에 연결된 element를 찾음.
			m_pDoc->m_pAttrCtrl->GetConnectedElem(aIncludeNodeKeyList[i], aConnectedElemK);
			for(j=0; j<aConnectedElemK.size(); j++)
			{
				T_ELEM_K Elemkk = aConnectedElemK[j];
				if(m_mapElemKeyList.Lookup(aConnectedElemK[j], temp)) continue;            

				ElemD.Initialize();
				if(!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElemK[j], ElemD)) 
				{              
					ASSERT(0);
					delete []dPolyLine;
					return 0;
				}              
				nNodeNum = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp,ElemD.elnod); 
				
				// Element의 node중에서 convex hull에 포함되는 다른 node가 있는지 검사
				if(m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
				{
					// 먼저 두점이 3D 상에서 같은 평면인지 검사 
					if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1))
					{                
						ASSERT(0);
						delete []dPolyLine;
						return 0;
					}         
					dPoint[0] = NodeD1.x;  dPoint[1] = NodeD1.y;  dPoint[2] = NodeD1.z;  
					if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
						continue;

					if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) 
					{                
						ASSERT(0);
						delete []dPolyLine;
						return 0;
					}        
					dPoint[0] = NodeD2.x;  dPoint[1] = NodeD2.y;  dPoint[2] = NodeD2.z;  
					if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
						continue;              

					// Domain의 2D 좌표계로 변환. 
					mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
																	NodeD1.x, NodeD1.y, NodeD1.z);
					p1[0] = NodeD1.x;
					p1[1] = NodeD1.y;             
				
					mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
																	NodeD2.x, NodeD2.y, NodeD2.z);
					p2[0] = NodeD2.x;
					p2[1] = NodeD2.y;
				
					// 2D Domain안에 포함되는 지 검사
					if(CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect) &&
						 CMathFunc::mathIsInsidePoint2D_Tol(p2, nData, dPolyLine, m_dTol, m_bBndrConnect) )
					{
						dLine[0][0] = p1[0]; dLine[0][1] = p1[1];
						dLine[1][0] = p2[0]; dLine[1][1] = p2[1];
						double dInsideLength = CMathFunc::mathsInsideLength_Tol(dLine, nData, dPolyLine, m_dLengthTol);              
						if(dInsideLength <= m_dTol) continue;

						ptStart.x = p1[0];
						ptStart.y = p1[1];
						ptStart.z = NodeD1.z;

						ptEnd.x = p2[0];
						ptEnd.y = p2[1];
						ptEnd.z = NodeD2.z;                

						BOOL bAdd = FALSE;
						LineG.Set(ptStart, ptEnd);              
						if(m_nOption_Line == 0) 
						{
							AddEdge(m_aEdge, LineG);
							bAdd = TRUE;
						}
						else
						{
							if(mapUserElemKeyList.Lookup(aConnectedElemK[j], temp))
							{
								AddEdge(m_aEdge, LineG);
								bAdd = TRUE;
							}
							else
								bAdd = FALSE;
						}
						if(bAdd)
						{
							m_mapNodeKeyList.SetAt(ElemD.elnod[0], temp);
							m_mapNodeKeyList.SetAt(ElemD.elnod[1], temp);  
							m_mapElemKeyList.SetAt(aConnectedElemK[j], temp);
						}
					}
				}
				else if(m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
				{
					for(k=0; k<nNodeNum; k++)
					{              
						if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[k], NodeD1))
						{                  
							ASSERT(0);
							delete []dPolyLine;
							return 0;
						}  
						dPoint[0] = NodeD1.x;  dPoint[1] = NodeD1.y;  dPoint[2] = NodeD1.z;  
						if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
							continue;                

						int nNodeIndex;                
						if(k == nNodeNum-1)
						{
							nNodeIndex = 0;
							if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[nNodeIndex], NodeD2)) 
							{                    
								ASSERT(0);
								delete []dPolyLine;
								return 0;
							}  
						}
						else
						{
							nNodeIndex = k+1;
							if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[nNodeIndex], NodeD2)) 
							{                    
								ASSERT(0);
								delete []dPolyLine;
								return 0;
							}  
						}
						dPoint[0] = NodeD2.x;  dPoint[1] = NodeD2.y;  dPoint[2] = NodeD2.z;  
						if(!mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
							continue;                                          
					
						mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
																	NodeD1.x, NodeD1.y, NodeD1.z);
						p1[0] = NodeD1.x;
						p1[1] = NodeD1.y;             
			 
						mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
																	NodeD2.x, NodeD2.y, NodeD2.z);
						p2[0] = NodeD2.x;
						p2[1] = NodeD2.y;
				
						if(CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect) &&
							 CMathFunc::mathIsInsidePoint2D_Tol(p2, nData, dPolyLine, m_dTol, m_bBndrConnect) )
						{ 
							dLine[0][0] = p1[0]; dLine[0][1] = p1[1];
							dLine[1][0] = p2[0]; dLine[1][1] = p2[1];
							double dInsideLength = CMathFunc::mathsInsideLength_Tol(dLine, nData, dPolyLine, m_dLengthTol);
							if(dInsideLength <= m_dTol) continue;

							ptStart.x = p1[0];
							ptStart.y = p1[1];
							ptStart.z = NodeD1.z;
													 
							ptEnd.x = p2[0];
							ptEnd.y = p2[1];
							ptEnd.z = NodeD2.z;
					
							BOOL bAdd = FALSE;
							LineG.Set(ptStart, ptEnd);  
							if(m_nOption_Line == 0) 
							{
								AddEdge(m_aEdge, LineG);                  
								bAdd = TRUE;
							}
							else
							{
								if(mapUserElemKeyList.Lookup(aConnectedElemK[j], temp))
								{
									AddEdge(m_aEdge, LineG);
									bAdd = TRUE;
								}
								else
									bAdd = FALSE;
							}

							if(bAdd)
							{
								m_mapNodeKeyList.SetAt(ElemD.elnod[k], temp);
								m_mapNodeKeyList.SetAt(ElemD.elnod[nNodeIndex], temp); 
								m_mapElemKeyList.SetAt(aConnectedElemK[j], temp);
							}
						}
					}              
				}     
				else
					continue;
			}
		}
		delete []dPolyLine;   
		//////////////////////////////////////////////////////////////////////////
	}

	if(m_bInteriorNode)
	{ 
		// Domain의 Boundary에 포함되는지 판단.
		nData = aBoundaryEdge.size(); 
		double (*dPolyLine)[2];		
		dPolyLine = new double[nData][2];     
		for(int i=0; i<nData; i++)
		{
			dPolyLine[i][0] = aBoundaryEdge[i].p2.x;
			dPolyLine[i][1] = aBoundaryEdge[i].p2.y;
		}
		
		if(nData < 3) 
		{
			delete []dPolyLine; 
			return 0;
		}
		
		for(int i=0; i<aIncludeNodeDataList.GetSize(); i++)
		{
			if(m_mapNodeKeyList.Lookup(aIncludeNodeKeyList[i], temp)) continue;
			
			p1[0] = aIncludeNodeDataList[i].x;
			p1[1] = aIncludeNodeDataList[i].y;                 
			if(CMathFunc::mathIsInsidePoint2D_Tol(p1, nData, dPolyLine, m_dTol, m_bBndrConnect))
			{
				m_mapNodeKeyList.SetAt(aIncludeNodeKeyList[i], temp);
				
				ptNode.x = aIncludeNodeDataList[i].x;
				ptNode.y = aIncludeNodeDataList[i].y;
				ptNode.z = aIncludeNodeDataList[i].z;
				
				if(m_nOption_Node == 0)
					m_aSelectNode.Add(ptNode);          
				else
				{
					if(mapUserNodeKeyList.Lookup(aIncludeNodeKeyList[i], temp))            
						m_aSelectNode.Add(ptNode);   
				}
			}        
		}
		delete []dPolyLine;     
	}      

	// 주두/지판에 대한 edge를 만든다.
	CArray<T_LINE_G,T_LINE_G&> aEdge_Drop;  // 주두에 대한 Edge
	CArray<T_LINE_G,T_LINE_G&> aEdge_Colm;  // 지판에 대한 Edge
	MakeEdgeByDropPanelNColmCapital(aEdge_Drop, aEdge_Colm, dPlanePoint, dRotAngle, dAxisVec);

	// 주두/지판에 따른 Edge 추가
	std::vector< T_LINE_G> aEdge_copy; // 주두 지판 영역을 넣기 전의 line
	aEdge_copy.reserve(m_aEdge.size());
	for (int i = 0; i < m_aEdge.size(); i++)
		aEdge_copy.push_back(m_aEdge[i]);
	CMapEx<int, int, BOOL, BOOL> mEdgeConnectBndr;
	MakeEdgeConnectInfo(aEdge_copy, aBoundaryEdge, mEdgeConnectBndr, aEdge_Drop, aEdge_Colm);
	if(!AddEdgeByDropPanelNColmCapital(m_aEdge, aEdge_copy, aBoundaryEdge, aEdge_Drop, aEdge_Colm, m_aSelectNode, mEdgeConnectBndr)) return 0;


	if(m_aSelectNode.GetSize() > 0)
		DivideEdgeByIncludeNode(m_aEdge, m_aSelectNode);  

	// [MQC 9341] AutoMesh할 때 엣지의 Seed가 중복되어서 들어가는 경우가 생김(주두나 지판 영역의 선과 m_aEdge가 겹치는 경우)
	// 그 때 겹치는 것을 빼도록 함 2014-12-16 : JWKWON
	RemoveRedundentEdge(m_aEdge);

	CAutoMesh_PlanarArea Mesher;
	int NbIndividual = 1;
	int nIth = 0;

	CArray<double,double&> aSeedSize;
	MakeSeedSizeInfo(m_aEdge, aSeedSize, dRotAngle, dAxisVec);

	// Mesh    
	int nSaveChangeMethod = m_nChangeMethod;
	aDomainElemCount.clear();  
	CArray<T_POLYLINE3D*,T_POLYLINE3D*> aDomainPolyLine;
	m_nChangeMethod = Mesher.AutoMesh_PlanarArea(m_aEdge, m_aSelectNode, AutoMeshArg, NbIndividual, nIth, aDomainElemCount, m_dMinMeshSize, aSeedSize, aDomainPolyLine);
	if(m_nChangeMethod == -1)  
		return -2;    

	// elem을 생성하는 부분에서 삼각형을 만들수 있으므로.
	if(nSaveChangeMethod == 2)
		m_nChangeMethod = nSaveChangeMethod;
	
	if(m_bDeleteSrcLine || m_bSubdivideSrcLine)      
		MakeHoopNode(aDomainPolyLine, aHoopNodeData);  
	for(int i=0; i<aHoopNodeData.size(); i++)
	{
		mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
													aHoopNodeData[i].x, aHoopNodeData[i].y, aHoopNodeData[i].z);    
	}  
	
	int nDomainCount = aDomainPolyLine.GetSize();
	for(i=nDomainCount-1; i>=0; i--)
	{
		if(aDomainPolyLine[i] != NULL)
		{
			aDomainPolyLine[i]->RemoveAll();
			delete aDomainPolyLine[i]; 
			aDomainPolyLine[i] = NULL;
		}
	}
	aDomainPolyLine.RemoveAll();

	CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& aElemD = Mesher.AllElems();
	CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& aNodeD = Mesher.AllNodes();  

	// 원래의 3D 좌표계로 변환
	for(int i=0; i<aNodeD.GetSize(); i++)
	{
		mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
			aNodeD[i].x, aNodeD[i].y, aNodeD[i].z);
	}

	arMeshElemD.SetSize(aElemD.GetSize());
	for(int i=0; i<aElemD.GetSize(); i++)    
		arMeshElemD.SetAt(i, aElemD.GetAt(i));
	
	arMeshNodeD.SetSize(aNodeD.GetSize());
	for(int i=0; i<aNodeD.GetSize(); i++)
		arMeshNodeD.SetAt(i, aNodeD.GetAt(i)); 
	
	// 내부 도메인 안만들기 옵션일때에는 주두/지판에 대한 Mesh를 다시 만들어준다.
	if(!m_bInnerDomain)
	{
		std::vector<T_LINE_G> aEdge2;
		CArray<T_POINT3D,T_POINT3D&> aSelectNode; aSelectNode.RemoveAll();
		// 주두/지판에 따른 Edge 추가
		if(!AddEdgeByDropPanelNColmCapital(aEdge2, aEdge_copy, aBoundaryEdge, aEdge_Drop, aEdge_Colm, aSelectNode, mEdgeConnectBndr, TRUE)) return 0;

		if(aSelectNode.GetSize() > 0)
			DivideEdgeByIncludeNode(aEdge2, aSelectNode); 

		if(aEdge2.size() > 2)
		{
			DivideEdge(aEdge2);  

//       CArray<T_POINT3D,T_POINT3D&> aSelectNode2; aSelectNode2.RemoveAll();
		
			CAutoMesh_PlanarArea Mesher2;
			int NbIndividual2 = 1;
			int nIth2 = 0;

			CArray<double,double&> aSeedSize2;
			//MakeSeedSizeInfo(aEdge2, aSeedSize2, dRotAngle, dAxisVec);


			// Mesh    
			AutoMeshArg.nInnerDomain=1;
			int nSaveChangeMethod2 = m_nChangeMethod;
			std::vector<int> aDomainElemCount2; aDomainElemCount2.clear();  
			CArray<T_POLYLINE3D*,T_POLYLINE3D*> aDomainPolyLine2;
			m_nChangeMethod = Mesher2.AutoMesh_PlanarArea(aEdge2, aSelectNode, AutoMeshArg, NbIndividual2, nIth2, aDomainElemCount2, m_dMinMeshSize, aSeedSize, aDomainPolyLine2);
			if(m_nChangeMethod == -1)  
				return -2;    

			CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& aElemD2 = Mesher2.AllElems();
			CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& aNodeD2 = Mesher2.AllNodes();  

			// 원래의 3D 좌표계로 변환
			for(int i=0; i<aNodeD2.GetSize(); i++)
			{
				mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2],
					aNodeD2[i].x, aNodeD2[i].y, aNodeD2[i].z);
			}

			// 추가되는 Node2이기 때문에 aElemD2에서 생성된 Nodek를 수정해준다.
			int nExistNodeCount = arMeshNodeD.GetSize();
			for(int i=0; i<aElemD2.GetSize(); i++)
			{
				int nNodeSize = aElemD2[i].aNodeK.GetSize();
				for (int j=0; j<nNodeSize; j++)
				{
					int nNodeK = aElemD2[i].aNodeK[j];
					aElemD2[i].aNodeK[j] = nExistNodeCount+nNodeK;
				}
			}

			if(aElemD2.GetSize() > 0)
				arMeshElemD.Append(aElemD2);
			if(aNodeD2.GetSize() > 0)
				arMeshNodeD.Append(aNodeD2);
		}
	}

	return 1;
}

void CMeshAutoDlg::MakeHoopNode(CArray<T_POLYLINE3D*,T_POLYLINE3D*>& aPolyLine, std::vector<T_NODE_D>& raHoopNodeData)
{
	raHoopNodeData.clear();
	raHoopNodeData.reserve(aPolyLine.GetSize() * 2);
	T_NODE_D NodeD;
	for(int i=0; i<aPolyLine.GetSize(); i++)
	{
		T_POLYLINE3D* pPolyLine = aPolyLine[i];
		int nCount = pPolyLine->GetSize();
		for (int j=0; j<nCount; j++)
		{
			NodeD.x = pPolyLine->GetAt(j).x;
			NodeD.y = pPolyLine->GetAt(j).y;
			NodeD.z = pPolyLine->GetAt(j).z;
			raHoopNodeData.push_back(NodeD);
			
			if(j == nCount-1)
			{
				NodeD.x = pPolyLine->GetAt(0).x;
				NodeD.y = pPolyLine->GetAt(0).y;
				NodeD.z = pPolyLine->GetAt(0).z;
			}
			else
			{
				NodeD.x = pPolyLine->GetAt(j+1).x;
				NodeD.y = pPolyLine->GetAt(j+1).y;
				NodeD.z = pPolyLine->GetAt(j+1).z;
			}
			raHoopNodeData.push_back(NodeD);
		}
	}  
}

void CMeshAutoDlg::MakeMainDomainData(T_MADO_D& MadoD)
{
	///////////////////////////////////////////////////////////////////////
	// Mado
	MadoD.Initialize();
	GetDlgItem(IDC_TM_DOMAIN_NAME_EDIT)->GetWindowText(MadoD.strName);
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
}

void CMeshAutoDlg::MakeNodeElemList(CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& aMeshNodeD,
																		CArray<T_ELEM_K, T_ELEM_K>& arElemK, CArray<T_ELEM_D, T_ELEM_D&>& arElemD,
																		CArray<T_NODE_K, T_NODE_K>& arNodeK, CArray<T_NODE_D, T_NODE_D&>& arNodeD,
																		std::vector<int>& aDomainElemCount)
{
	int i, j;
	
	T_NODE_K NewNodeK = m_pDoc->m_pDataCtrl->GetNodeKey();
	CMap<int,int,int,int> mapNodeK; 
	CMap<int,int,int,int> mapElemNodeK; 
	for(int i=0; i<aMeshNodeD.GetSize(); i++)
	{
		T_NODE_D NodeD;
		NodeD.x = aMeshNodeD[i].x;
		NodeD.y = aMeshNodeD[i].y;
		NodeD.z = aMeshNodeD[i].z;
		// 중복된 node 검사
		T_NODE_K NodeK = m_pDoc->m_pDataCtrl->GetSamePointNodeFlag(NodeD);  
		if(NodeK == 0)
		{  
			arNodeK.Add(NewNodeK);
			arNodeD.Add(NodeD);
			NodeK = NewNodeK;
			NewNodeK++;   
			
			mapElemNodeK.SetAt(NodeK,i);
		}    
		// mesh에서 사용자가 선택한 node는 키가 0부터 +1씩 하여 들어오므로 map을 생성하여
		// Elem의 node정보를 찾을 때 사용.
		mapNodeK.SetAt(i,NodeK);    
	}
	
	//////////////////////////////////////////////////////////////////////
	//Elem
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
			ElemD.nSubType = 1;
		else
			ElemD.nSubType = 0;
	}  
	
	CArray<int,int> aIncraseIndex;
	BOOL bAddElem;
	int nNodeKey;
	double dPoint[4][3];   
	T_ELEM_K NewElemK = m_pDoc->m_pDataCtrl->GetElemKey();
	for(i=0 ; i<aMeshElemD.GetSize() ; ++i)
	{
		for(j=0; j<4; j++) ElemD.elnod[j] = 0;       
		T_MESH_ELEM_D MeshElemD = aMeshElemD[i];
		int nSize = MeshElemD.aNodeK.GetSize();
		for(int n=0 ; n<nSize ; ++n)    
		{
			if(!mapNodeK.Lookup(MeshElemD.aNodeK[n], ElemD.elnod[n])) {ASSERT(0);}
		}        

		bAddElem = TRUE;
		// 4절점을 가지는 삼각형 요소가 만들어져서 이 부분을 추가  
		// 두개의 삼각형 Element로 나눠준다.
		if(nSize == 4)
		{
			T_ELEM_D TempElemD = ElemD;
			for(int k=0; k<nSize; k++)
			{
				if(!mapElemNodeK.Lookup(ElemD.elnod[k], nNodeKey))
				{
					T_NODE_D NodeD;
					m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[k], NodeD);
					dPoint[k][0] = NodeD.x;
					dPoint[k][1] = NodeD.y;        
					dPoint[k][2] = NodeD.z;    
				}
				else
				{
					dPoint[k][0] = aMeshNodeD[nNodeKey].x;
					dPoint[k][1] = aMeshNodeD[nNodeKey].y;        
					dPoint[k][2] = aMeshNodeD[nNodeKey].z;    
				}       
			}

			double dV0_1_2, dV1_2_3, dV2_3_0, dV3_0_1;
			double dVecTemp[3];         
			
			// 0-1-2
			double dVec_0_1[3];         
			dVecTemp[0] = dPoint[1][0] - dPoint[0][0];
			dVecTemp[1] = dPoint[1][1] - dPoint[0][1];
			dVecTemp[2] = dPoint[1][2] - dPoint[0][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_0_1[0], dVec_0_1[1], dVec_0_1[2]);
			
			double dVec_1_2[3];
			dVecTemp[0] = dPoint[2][0] - dPoint[1][0];
			dVecTemp[1] = dPoint[2][1] - dPoint[1][1];
			dVecTemp[2] = dPoint[2][2] - dPoint[1][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_1_2[0], dVec_1_2[1], dVec_1_2[2]);

			// 1-2-3, 2-3-0
			double dVec_2_3[3];
			dVecTemp[0] = dPoint[3][0] - dPoint[2][0];
			dVecTemp[1] = dPoint[3][1] - dPoint[2][1];
			dVecTemp[2] = dPoint[3][2] - dPoint[2][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_2_3[0], dVec_2_3[1], dVec_2_3[2]);

			// 3-0-1
			double dVec_3_0[3];
			dVecTemp[0] = dPoint[0][0] - dPoint[3][0];
			dVecTemp[1] = dPoint[0][1] - dPoint[3][1];
			dVecTemp[2] = dPoint[0][2] - dPoint[3][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_3_0[0], dVec_3_0[1], dVec_3_0[2]);
			
			dV0_1_2 = fabs(dVec_0_1[0]*dVec_1_2[0] + dVec_0_1[1]*dVec_1_2[1] + dVec_0_1[2]*dVec_1_2[2]); 
			dV1_2_3 = fabs(dVec_1_2[0]*dVec_2_3[0] + dVec_1_2[1]*dVec_2_3[1] + dVec_1_2[2]*dVec_2_3[2]);
			dV2_3_0 = fabs(dVec_2_3[0]*dVec_3_0[0] + dVec_2_3[1]*dVec_3_0[1] + dVec_2_3[2]*dVec_3_0[2]);
			dV3_0_1 = fabs(dVec_3_0[0]*dVec_0_1[0] + dVec_3_0[1]*dVec_0_1[1] + dVec_3_0[2]*dVec_0_1[2]);
			
			if(fabs(dV0_1_2-1) <= m_dTol)            
			{ 
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[0]; 
				TempElemD.elnod[1] = ElemD.elnod[1];          
				TempElemD.elnod[2] = ElemD.elnod[3];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[2]; 
				TempElemD.elnod[1] = ElemD.elnod[1];          
				TempElemD.elnod[2] = ElemD.elnod[3];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;            
			}
			else if(fabs(dV1_2_3-1) <= m_dTol)      
			{
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[1]; 
				TempElemD.elnod[1] = ElemD.elnod[2];          
				TempElemD.elnod[2] = ElemD.elnod[0];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[3]; 
				TempElemD.elnod[1] = ElemD.elnod[2];          
				TempElemD.elnod[2] = ElemD.elnod[0];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;            
			}
			else if(fabs(dV2_3_0-1) <= m_dTol)            
			{
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[2]; 
				TempElemD.elnod[1] = ElemD.elnod[3];          
				TempElemD.elnod[2] = ElemD.elnod[1];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[0]; 
				TempElemD.elnod[1] = ElemD.elnod[3];          
				TempElemD.elnod[2] = ElemD.elnod[1];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;            
			}
			else if(fabs(dV3_0_1-1) <= m_dTol)      
			{
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[3]; 
				TempElemD.elnod[1] = ElemD.elnod[0];          
				TempElemD.elnod[2] = ElemD.elnod[2];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[1]; 
				TempElemD.elnod[1] = ElemD.elnod[0];          
				TempElemD.elnod[2] = ElemD.elnod[2];
				arElemK.Add(NewElemK); 
				arElemD.Add(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;             
			}
		}
		
		if(!bAddElem)  
		{
			m_nChangeMethod = 2;
			int nFindIndex = FindIndexDomainElemCount(i, aDomainElemCount);
			if(nFindIndex == -1) ASSERT(0);
			else                 aIncraseIndex.Add(nFindIndex);
			continue;    
		}
	
		arElemK.Add(NewElemK); 
		arElemD.Add(ElemD);
		NewElemK++;
	}  

	for(int i=0; i<aIncraseIndex.GetSize(); i++)  
		aDomainElemCount[aIncraseIndex[i]] += 1;
}

// Node 중복체크 안하고 무조건 생성
void CMeshAutoDlg::MakeNodeElemList_SubDomain(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD, std::vector<T_ELEM_K>& arElemK, std::vector<T_ELEM_D>& arElemD,
											  std::vector<T_NODE_K>& arNodeK, std::vector<T_NODE_D>& arNodeD, std::vector<int>& aDomainElemCount, int nNodeStartIndex/*=0*/, int nElemStartIndex/*=0*/)
{
	M_PROFILE('CMeshAutoDlg::MakeNodeElemList_SubDomain');

	T_ELEM_D ElemD; ElemD.Initialize();
	ElemD.eltyp = GetCurElemTypeID();
	if (!GetPropIDS(ElemD.eltyp, ElemD.elmat, ElemD.elpro)) return;

	int i, j;

	arNodeK.clear();
	arNodeD.clear();
	arNodeK.reserve(aMeshNodeD.GetSize());
	arNodeD.reserve(aMeshNodeD.GetSize());
	
	T_NODE_D NodeD;
	T_NODE_K NewNodeK = m_pDoc->m_pDataCtrl->GetNodeKey()+nNodeStartIndex;
	for(int i=0; i<aMeshNodeD.GetSize(); i++)
	{
		NodeD.x = aMeshNodeD[i].x;
		NodeD.y = aMeshNodeD[i].y;
		NodeD.z = aMeshNodeD[i].z;
		 
		arNodeK.push_back(NewNodeK);
		arNodeD.push_back(NodeD);
		NewNodeK++;         
	}
	
	//////////////////////////////////////////////////////////////////////
	//Elem
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
			ElemD.nSubType = 1;
		else
			ElemD.nSubType = 0;
	}  
	
	arElemK.clear();
	arElemD.clear();
	arElemK.reserve(aMeshElemD.GetSize());
	arElemD.reserve(aMeshElemD.GetSize());

	std::vector<int> aIncraseIndex;
	BOOL bAddElem;
	double dPoint[4][3];   
	T_ELEM_K NewElemK = m_pDoc->m_pDataCtrl->GetElemKey()+nElemStartIndex;
	for(i=0 ; i<aMeshElemD.GetSize() ; ++i)
	{
		int nSize = aMeshElemD[i].aNodeK.GetSize();
		for(j=0 ; j<nSize ; ++j)    
			ElemD.elnod[j] = arNodeK[aMeshElemD[i].aNodeK[j]];

		if(nSize<4) ElemD.elnod[3] = 0;

		bAddElem = TRUE;
		// 4절점을 가지는 삼각형 요소가 만들어져서 이 부분을 추가  
		// 두개의 삼각형 Element로 나눠준다.
		if(nSize == 4)
		{
			T_ELEM_D TempElemD = ElemD;
			for(j=0; j<nSize; j++)
			{
				dPoint[j][0] = arNodeD[aMeshElemD[i].aNodeK[j]].x;
				dPoint[j][1] = arNodeD[aMeshElemD[i].aNodeK[j]].y;        
				dPoint[j][2] = arNodeD[aMeshElemD[i].aNodeK[j]].z;                   
			}

			double dV0_1_2, dV1_2_3, dV2_3_0, dV3_0_1;
			double dVecTemp[3];         
			
			// 0-1-2
			double dVec_0_1[3];         
			dVecTemp[0] = dPoint[1][0] - dPoint[0][0];
			dVecTemp[1] = dPoint[1][1] - dPoint[0][1];
			dVecTemp[2] = dPoint[1][2] - dPoint[0][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_0_1[0], dVec_0_1[1], dVec_0_1[2]);
			
			double dVec_1_2[3];
			dVecTemp[0] = dPoint[2][0] - dPoint[1][0];
			dVecTemp[1] = dPoint[2][1] - dPoint[1][1];
			dVecTemp[2] = dPoint[2][2] - dPoint[1][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_1_2[0], dVec_1_2[1], dVec_1_2[2]);

			// 1-2-3, 2-3-0
			double dVec_2_3[3];
			dVecTemp[0] = dPoint[3][0] - dPoint[2][0];
			dVecTemp[1] = dPoint[3][1] - dPoint[2][1];
			dVecTemp[2] = dPoint[3][2] - dPoint[2][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_2_3[0], dVec_2_3[1], dVec_2_3[2]);

			// 3-0-1
			double dVec_3_0[3];
			dVecTemp[0] = dPoint[0][0] - dPoint[3][0];
			dVecTemp[1] = dPoint[0][1] - dPoint[3][1];
			dVecTemp[2] = dPoint[0][2] - dPoint[3][2];      
			mathNormalize(dVecTemp[0], dVecTemp[1], dVecTemp[2], dVec_3_0[0], dVec_3_0[1], dVec_3_0[2]);
			
			dV0_1_2 = fabs(dVec_0_1[0]*dVec_1_2[0] + dVec_0_1[1]*dVec_1_2[1] + dVec_0_1[2]*dVec_1_2[2]); 
			dV1_2_3 = fabs(dVec_1_2[0]*dVec_2_3[0] + dVec_1_2[1]*dVec_2_3[1] + dVec_1_2[2]*dVec_2_3[2]);
			dV2_3_0 = fabs(dVec_2_3[0]*dVec_3_0[0] + dVec_2_3[1]*dVec_3_0[1] + dVec_2_3[2]*dVec_3_0[2]);
			dV3_0_1 = fabs(dVec_3_0[0]*dVec_0_1[0] + dVec_3_0[1]*dVec_0_1[1] + dVec_3_0[2]*dVec_0_1[2]);
			
			if(fabs(dV0_1_2-1) <= 1.0e-4)            
			{ 
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[0]; 
				TempElemD.elnod[1] = ElemD.elnod[1];          
				TempElemD.elnod[2] = ElemD.elnod[3];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[2]; 
				TempElemD.elnod[1] = ElemD.elnod[1];          
				TempElemD.elnod[2] = ElemD.elnod[3];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;            
			}
			else if(fabs(dV1_2_3-1) <= 1.0e-4)      
			{
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[1]; 
				TempElemD.elnod[1] = ElemD.elnod[2];          
				TempElemD.elnod[2] = ElemD.elnod[0];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[3]; 
				TempElemD.elnod[1] = ElemD.elnod[2];          
				TempElemD.elnod[2] = ElemD.elnod[0];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;            
			}
			else if(fabs(dV2_3_0-1) <= 1.0e-4)            
			{
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[2]; 
				TempElemD.elnod[1] = ElemD.elnod[3];          
				TempElemD.elnod[2] = ElemD.elnod[1];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[0]; 
				TempElemD.elnod[1] = ElemD.elnod[3];          
				TempElemD.elnod[2] = ElemD.elnod[1];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;            
			}
			else if(fabs(dV3_0_1-1) <= 1.0e-4)      
			{
				TempElemD.elnod[3] = 0;         
				
				TempElemD.elnod[0] = ElemD.elnod[3]; 
				TempElemD.elnod[1] = ElemD.elnod[0];          
				TempElemD.elnod[2] = ElemD.elnod[2];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;
				
				TempElemD.elnod[0] = ElemD.elnod[1]; 
				TempElemD.elnod[1] = ElemD.elnod[0];          
				TempElemD.elnod[2] = ElemD.elnod[2];
				arElemK.push_back(NewElemK); 
				arElemD.push_back(TempElemD);
				NewElemK++;          
				bAddElem = FALSE;             
			}
		}
		
		if(!bAddElem)  
		{
			m_nChangeMethod = 2;
			int nFindIndex = FindIndexDomainElemCount(i, aDomainElemCount);
			if(nFindIndex == -1) ASSERT(0);
			else                 aIncraseIndex.push_back(nFindIndex);
			continue;    
		}
	
		arElemK.push_back(NewElemK); 
		arElemD.push_back(ElemD);
		NewElemK++;
	}  

	for(int i=0; i<aIncraseIndex.size(); i++) 
		aDomainElemCount[aIncraseIndex[i]] += 1;
}

int CMeshAutoDlg::FindIndexDomainElemCount(int nIndex, std::vector<int>& aDomainElemCount)
{
	int nFindIndex=-1;
	int nSum = 0;
	for (int i=0; i<aDomainElemCount.size(); i++)
	{
		nSum += aDomainElemCount[i];
		if(nIndex <= nSum-1)
		{
			nFindIndex = i;
			break;
		}
	}
	return nFindIndex;
}

void CMeshAutoDlg::MakeSubDomainData(std::vector<int>& aDomainElemCount, double dPlanePoint[3][3], std::vector<T_ELEM_K>& arElemK, CArray<T_SBDO_ELEMLIST_D, T_SBDO_ELEMLIST_D&>& arSbdoElemList)
{
	arSbdoElemList.RemoveAll();
	
	int nElemIndex = 0;
	CString strSbdoName;
	T_SBDO_ELEMLIST_D aSbdoList;   
	for (int i=0; i<aDomainElemCount.size(); i++)
	{
		aSbdoList.Initialize();    
		strSbdoName.Format(_T("[%d]"), i+1);
		aSbdoList.SbdoD.strName = strSbdoName;    
		for(int j=0; j<aDomainElemCount[i]; j++)
		{
			if(j+nElemIndex >= arElemK.size()) {ASSERT(0); return;}
			aSbdoList.ElemKList.Add(arElemK[j+nElemIndex]);
		}
		nElemIndex += aDomainElemCount[i];
		if(!GetDomainType(aSbdoList.SbdoD.nType, dPlanePoint)) continue;
		arSbdoElemList.Add(aSbdoList);
	}  
}

void CMeshAutoDlg::EndMesh()
{
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);  
	
	CString strWarnMsg, ustr;
	strWarnMsg = _T("");
	ustr       = _T("");
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, ustr);  
	// Mesh Size가 변경되고 나서 method가 변경되는 경우가 있으므로 size관련 message를 먼저 출력한다.
	if(m_bChangeMeshSize)
	{    
		strWarnMsg.Format(_LS(IDS_WG_TREEMENU_Warning___Mesh_Size_Changed__g__s), m_dMaxBoundarySize, ustr);
		m_pDoc->DisplayHistoryMessage(strWarnMsg);        
	}
	if(m_nChangeMethod == 1)
	{ 
		strWarnMsg.Format(_LS(IDS_WG_TREEMENU_Warning___Mesh_Method_Changed__g__s), m_dMinMeshSize, ustr);
		m_pDoc->DisplayHistoryMessage(strWarnMsg);
	}
	else if(m_nChangeMethod == 2)   
		m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_TREEMENU_Warning___Mesh_Method_Changed_Seed_Info));  

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(!pIGM) {ASSERT(0); return;}

	if(m_cmbMethod.GetCurSel() == 0)
	{
		pIGM->ResetTempElem();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
		m_wndNodeListEdit.ImAssocWindowST();
	}
}

void CMeshAutoDlg::MakeSeedSizeInfo(std::vector<T_LINE_G>& aEdge, CArray<double,double&>& aSeedSize, const double& dRotAngle, const double dAxisVec[3])
{
	if(!m_bBndrConnect) return; 
	
	M_PROFILE('CMeshAutoDlg::MakeSeedSizeInfo');

	aSeedSize.RemoveAll();
	int i, j; 
	//////////////////////////////////////////////////////////////////////////
	// Edge 변환
	for (i = 0; i < aEdge.size(); i++)
	{
		mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aEdge[i].p1.x, aEdge[i].p1.y, aEdge[i].p1.z);
		mathRotate(dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aEdge[i].p2.x, aEdge[i].p2.y, aEdge[i].p2.z);
	}

	UINT nTemp;
	CArray<T_ELEM_K, T_ELEM_K> aConnectElemKList;
	T_NODE_K NodeK1, NodeK2;   
	T_NODE_D NodeD1, NodeD2; 
	for(i=0; i<aEdge.size(); i++)
	{
		double dSeedSize = 0;
		NodeD1.x = aEdge[i].p1.x;
		NodeD1.y = aEdge[i].p1.y;
		NodeD1.z = aEdge[i].p1.z;    
		NodeK1 = m_pDoc->m_pDataCtrl->GetSamePointNodeFlag(NodeD1);

		NodeD2.x = aEdge[i].p2.x;
		NodeD2.y = aEdge[i].p2.y;
		NodeD2.z = aEdge[i].p2.z;    
		NodeK2 = m_pDoc->m_pDataCtrl->GetSamePointNodeFlag(NodeD2);

		if(NodeK1 == 0 || NodeK2 == 0) 
		{
			aSeedSize.Add(dSeedSize);
			continue;
		}

		aConnectElemKList.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(NodeK1, aConnectElemKList);
		CMap<UINT,UINT,UINT,UINT> mapConnectElemK;
		for(j=0; j<aConnectElemKList.GetSize(); j++)
			mapConnectElemK.SetAt(aConnectElemKList[j], aConnectElemKList[j]); 

		aConnectElemKList.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(NodeK2, aConnectElemKList);
		for(j=0; j<aConnectElemKList.GetSize(); j++)
		{
			// Node1과 Node2에 둘다 연결이 되어 있고 Sub Domain에 속해 있는 Elem
			if(mapConnectElemK.Lookup(aConnectElemKList[j], nTemp) && m_mapSeedElemK.Lookup(aConnectElemKList[j], nTemp))
			{
				T_ELEM_D ElemD; ElemD.Initialize();
				m_pDoc->m_pAttrCtrl->GetElem(aConnectElemKList[j], ElemD);
				if(m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
				{
					dSeedSize = 1;
				}
				break;
			}
		}
		aSeedSize.Add(dSeedSize);
	}  

	//////////////////////////////////////////////////////////////////////////
	// Edge 변환
	for (i = 0; i < aEdge.size(); i++)
	{
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aEdge[i].p1.x, aEdge[i].p1.y, aEdge[i].p1.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], aEdge[i].p2.x, aEdge[i].p2.y, aEdge[i].p2.z);
	}
}

void CMeshAutoDlg::DivideEdge(std::vector<T_LINE_G>& aEdge)
{
	int i, j;
	T_NODE_D NodeD1, NodeD2;
	std::vector<T_LINE_G> aTempEdge; aTempEdge.reserve(aEdge.size());
	CArray<T_NODE_K,T_NODE_K> aNodeKeyList;
	double dX1, dX2, dY1, dY2, dZ1, dZ2;
	T_LINE_G LineG;
	T_POINT3D ptStart, ptEnd; 
	for(i=0; i<aEdge.size(); i++)
	{
		dX1 = aEdge[i].p1.x;
		dY1 = aEdge[i].p1.y;
		dZ1 = aEdge[i].p1.z;
		
		dX2 = aEdge[i].p2.x;
		dY2 = aEdge[i].p2.y;
		dZ2 = aEdge[i].p2.z;
		
		aNodeKeyList.RemoveAll();    
		m_pDoc->m_pDataCtrl->GetNodeBy2Point(dX1, dY1, dZ1, dX2, dY2, dZ2, m_dTol, aNodeKeyList, TRUE);
		if(aNodeKeyList.GetSize() <= 2)
		{
			aTempEdge.push_back(aEdge[i]);
			continue;
		}
		for(j=0; j<aNodeKeyList.GetSize()-1; j++)
		{
			NodeD1.Initialize();
			m_pDoc->m_pAttrCtrl->GetNode(aNodeKeyList[j], NodeD1);
			m_pDoc->m_pAttrCtrl->GetNode(aNodeKeyList[j+1], NodeD2);
			
			LineG.Initialize();
			ptStart.x = NodeD1.x;
			ptStart.y = NodeD1.y;
			ptStart.z = NodeD1.z;
			
			ptEnd.x = NodeD2.x;
			ptEnd.y = NodeD2.y;
			ptEnd.z = NodeD2.z;
						
			LineG.Set(ptStart, ptEnd);
			if(LineG.GetLineLength() <= m_dTol) continue;
			aTempEdge.push_back(LineG);
		}
	}

	if(aTempEdge.size() <= 0) return;
		
	aEdge.clear();
	aEdge.resize(aTempEdge.size());
	std::copy(aTempEdge.begin(), aTempEdge.end(), aEdge.begin());
}

BOOL CMeshAutoDlg::MakeEdgeByDropPanelNColmCapital(CArray<T_LINE_G,T_LINE_G&>& aEdge_Drop, CArray<T_LINE_G,T_LINE_G&>& aEdge_Colm, double dPlanePoint[3][3], double dRotAngle, double dAxisVec[3])
{
	m_DpaaK.RemoveAll();
	m_CcpaK.RemoveAll();
	m_MasterNodeK.RemoveAll();

	aEdge_Drop.RemoveAll();
	aEdge_Colm.RemoveAll();

	BOOL bWarningMessage=FALSE;
	CArray<T_DPAA_K, T_DPAA_K> aDpaaK;
	m_pDoc->m_pAttrCtrl2->GetDpaaKeyList(aDpaaK);

	int nDpaaCount = aDpaaK.GetSize();
	for (int i=0; i<nDpaaCount; i++)
	{
		T_DPAA_D DpaaD;
		if(!m_pDoc->m_pAttrCtrl2->GetDpaa(aDpaaK[i], DpaaD)) { ASSERT(0); continue; }

		T_DPAN_D DpanD;
		if(!m_pDoc->m_pAttrCtrl2->GetDpan(DpaaD.DpanK, DpanD)) { ASSERT(0); continue; }

		T_ELEM_D ElemD;
		if(!m_pDoc->m_pAttrCtrl->GetElem(aDpaaK[i], ElemD)) { ASSERT(0); continue; }

		T_NODE_D NodeD1, NodeD2, NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1))  { ASSERT(0); continue; }
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2))  { ASSERT(0); continue; }

		T_NODE_K NodeK_used=0;
		BOOL bNode1IsTop = NodeD1.z>NodeD2.z ? TRUE:FALSE;
		BOOL bUseTop=FALSE, bUseBottom=FALSE;

		// 1. 기둥의 2 절점 중 Mesh에 포함될 점을 찾음
		// Top
		if(DpanD.nLocation==0 || DpanD.nLocation==2)
		{
			// 위쪽 절점과 영역 포함여부 비교
			double dPoint[3];
			dPoint[0] = bNode1IsTop ? NodeD1.x:NodeD2.x; 
			dPoint[1] = bNode1IsTop ? NodeD1.y:NodeD2.y; 
			dPoint[2] = bNode1IsTop ? NodeD1.z:NodeD2.z;
			if(mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
			{
				bUseTop=TRUE;
				if(bNode1IsTop)
				{
					NodeD = NodeD1;    
					NodeK_used = ElemD.elnod[0];
				}
				else
				{
					NodeD = NodeD2;    
					NodeK_used = ElemD.elnod[1];
				}
			}
		}

		// Bottom
		if(DpanD.nLocation==1 || DpanD.nLocation==2)
		{
			// 아래쪽 절점과 영역 포함여부 비교
			double dPoint[3];
			dPoint[0] = bNode1IsTop ? NodeD2.x:NodeD1.x; 
			dPoint[1] = bNode1IsTop ? NodeD2.y:NodeD1.y; 
			dPoint[2] = bNode1IsTop ? NodeD2.z:NodeD1.z;
			if(mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
			{
				bUseBottom=TRUE;
				ASSERT(!bUseTop); // Mesh 영역은 수평이기 때문에 Top,Bottom 둘 중 하나만 사용될 것이다.
				if(bNode1IsTop)
				{
					NodeD = NodeD2;    
					NodeK_used = ElemD.elnod[1];
				}
				else
				{
					NodeD = NodeD1;    
					NodeK_used = ElemD.elnod[0];
				}
			}
		}

		if(NodeK_used==0) 
			continue;

		// 2. 기둥의 절점을 중심으로 4각형을 만든다.
		// [MQC 10267] 기둥이 생성되는 B, H 값이랑 주두/지판에서 생성되는 B/H 값이 서로 반대임
		T_POINT3D p1, p2, p3, p4;
		p1.x = NodeD.x - DpanD.dH1; p1.y = NodeD.y - DpanD.dB1; p1.z = NodeD.z;
		p2.x = NodeD.x + DpanD.dH2; p2.y = NodeD.y - DpanD.dB1; p2.z = NodeD.z;
		p3.x = NodeD.x + DpanD.dH2; p3.y = NodeD.y + DpanD.dB2; p3.z = NodeD.z;
		p4.x = NodeD.x - DpanD.dH1; p4.y = NodeD.y + DpanD.dB2; p4.z = NodeD.z;

		double dAngle = ElemD.angle;
		if(DpanD.bAngle)
		{
			dAngle = DpanD.dAngle;
		}

		CMathFunc::mathRotate(dAngle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p1.x, p1.y, p1.z); //임의 축에 대한 회전, angle:회전각[deg], px,py,pz:회전축상의 한점, ux,uy,uz:회전축 벡터, rx,ry,rz:회전대상 좌표
		CMathFunc::mathRotate(dAngle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p2.x, p2.y, p2.z);
		CMathFunc::mathRotate(dAngle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p3.x, p3.y, p3.z); //임의 축에 대한 회전, angle:회전각[deg], px,py,pz:회전축상의 한점, ux,uy,uz:회전축 벡터, rx,ry,rz:회전대상 좌표
		CMathFunc::mathRotate(dAngle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p4.x, p4.y, p4.z);

		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p1.x, p1.y, p1.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p2.x, p2.y, p2.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p3.x, p3.y, p3.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p4.x, p4.y, p4.z);


		T_LINE_G NewLine1; NewLine1.Initialize(); NewLine1.Set(p1, p2);  aEdge_Drop.Add(NewLine1);
		T_LINE_G NewLine2; NewLine2.Initialize(); NewLine2.Set(p2, p3);  aEdge_Drop.Add(NewLine2);
		T_LINE_G NewLine3; NewLine3.Initialize(); NewLine3.Set(p3, p4);  aEdge_Drop.Add(NewLine3);
		T_LINE_G NewLine4; NewLine4.Initialize(); NewLine4.Set(p4, p1);  aEdge_Drop.Add(NewLine4);

		if(DpanD.bThik) m_DpaaK.Add(aDpaaK[i]);
	}

	CArray<T_CCPA_K, T_CCPA_K> aCcpaK;
	m_pDoc->m_pAttrCtrl2->GetCcpaKeyList(aCcpaK);
	int nCcpaCount = aCcpaK.GetSize();
	for (int i=0; i<nCcpaCount; i++)
	{
		T_CCPA_D CcpaD;
		if(!m_pDoc->m_pAttrCtrl2->GetCcpa(aCcpaK[i], CcpaD)) { ASSERT(0); continue; }

		T_CCPT_D CcptD;
		if(!m_pDoc->m_pAttrCtrl2->GetCcpt(CcpaD.CcptK, CcptD)) { ASSERT(0); continue; }

		T_ELEM_D ElemD;
		if(!m_pDoc->m_pAttrCtrl->GetElem(aCcpaK[i], ElemD)) { ASSERT(0); continue; }

		// 기둥에 대한 절점 중에서 Plane 에 속한 점을 찾는다.
		T_NODE_D NodeD1, NodeD2, NodeD;
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1))  { ASSERT(0); continue; }
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2))  { ASSERT(0); continue; }

		T_NODE_K NodeK_used=0;
		BOOL bNode1IsTop = NodeD1.z>NodeD2.z ? TRUE:FALSE;
		BOOL bUseTop=FALSE, bUseBottom=FALSE;
		
		// 1. 기둥의 2 절점 중 Mesh에 포함될 점을 찾음
		// Top
		if(CcptD.nLocation==0 || CcptD.nLocation==2)
		{
			// 위쪽 절점과 영역 포함여부 비교
			double dPoint[3];
			dPoint[0] = bNode1IsTop ? NodeD1.x:NodeD2.x; 
			dPoint[1] = bNode1IsTop ? NodeD1.y:NodeD2.y; 
			dPoint[2] = bNode1IsTop ? NodeD1.z:NodeD2.z;
			if(mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
			{
				bUseTop=TRUE;
				if(bNode1IsTop)
				{
					NodeD = NodeD1;    
					NodeK_used = ElemD.elnod[0];
				}
				else
				{
					NodeD = NodeD2;    
					NodeK_used = ElemD.elnod[1];
				}
			}
		}

		// Bottom
		if(CcptD.nLocation==1 || CcptD.nLocation==2)
		{
			// 아래쪽 절점과 영역 포함여부 비교
			double dPoint[3];
			dPoint[0] = bNode1IsTop ? NodeD2.x:NodeD1.x; 
			dPoint[1] = bNode1IsTop ? NodeD2.y:NodeD1.y; 
			dPoint[2] = bNode1IsTop ? NodeD2.z:NodeD1.z;
			if(mathIncludePointInPlane(dPlanePoint[0], dPlanePoint[1], dPlanePoint[2], dPoint, m_dTol))
			{
				bUseBottom=TRUE;
				ASSERT(!bUseTop); // Mesh 영역은 수평이기 때문에 Top,Bottom 둘 중 하나만 사용될 것이다.
				if(bNode1IsTop)
				{
					NodeD = NodeD2;    
					NodeK_used = ElemD.elnod[1];
				}
				else
				{
					NodeD = NodeD1;    
					NodeK_used = ElemD.elnod[0];
				}
			}
		}

		if(NodeK_used==0) 
			continue;

		// 2. 기둥의 절점을 중심으로 4각형을 만든다.
		// [MQC 10267] 기둥이 생성되는 B, H 값이랑 주두/지판에서 생성되는 B/H 값이 서로 반대임
		T_POINT3D p1, p2, p3, p4;
		p1.x = NodeD.x - CcptD.dH1; p1.y = NodeD.y - CcptD.dB1; p1.z = NodeD.z;
		p2.x = NodeD.x + CcptD.dH2; p2.y = NodeD.y - CcptD.dB1; p2.z = NodeD.z;
		p3.x = NodeD.x + CcptD.dH2; p3.y = NodeD.y + CcptD.dB2; p3.z = NodeD.z;
		p4.x = NodeD.x - CcptD.dH1; p4.y = NodeD.y + CcptD.dB2; p4.z = NodeD.z;

		CMathFunc::mathRotate(ElemD.angle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p1.x, p1.y, p1.z); //임의 축에 대한 회전, angle:회전각[deg], px,py,pz:회전축상의 한점, ux,uy,uz:회전축 벡터, rx,ry,rz:회전대상 좌표
		CMathFunc::mathRotate(ElemD.angle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p2.x, p2.y, p2.z);
		CMathFunc::mathRotate(ElemD.angle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p3.x, p3.y, p3.z); //임의 축에 대한 회전, angle:회전각[deg], px,py,pz:회전축상의 한점, ux,uy,uz:회전축 벡터, rx,ry,rz:회전대상 좌표
		CMathFunc::mathRotate(ElemD.angle, NodeD.x, NodeD.y, NodeD.z, 0., 0., 1., p4.x, p4.y, p4.z);

		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p1.x, p1.y, p1.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p2.x, p2.y, p2.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p3.x, p3.y, p3.z);
		mathRotate(-dRotAngle, dAxisVec[0], dAxisVec[1], dAxisVec[2], p4.x, p4.y, p4.z);

		T_LINE_G NewLine1; NewLine1.Initialize(); NewLine1.Set(p1, p2);  aEdge_Colm.Add(NewLine1);
		T_LINE_G NewLine2; NewLine2.Initialize(); NewLine2.Set(p2, p3);  aEdge_Colm.Add(NewLine2);
		T_LINE_G NewLine3; NewLine3.Initialize(); NewLine3.Set(p3, p4);  aEdge_Colm.Add(NewLine3);
		T_LINE_G NewLine4; NewLine4.Initialize(); NewLine4.Set(p4, p1);  aEdge_Colm.Add(NewLine4);

		// auto Rigid Rink를 만들기 위한 정보를 저장함..
		if(CcptD.bAutoRigid)
		{
			m_CcpaK.Add(aCcpaK[i]);
			m_MasterNodeK.Add(NodeK_used);
		}
	}

	return TRUE;
}

BOOL CMeshAutoDlg::AddEdgeByDropPanelNColmCapital(std::vector<T_LINE_G>& aEdge, const std::vector<T_LINE_G>& aEdge_org, std::vector<T_LINE_G>& aBoundaryEdge,
																									CArray<T_LINE_G,T_LINE_G&>& aEdge_Drop, CArray<T_LINE_G,T_LINE_G&>& aEdge_Colm, CArray<T_POINT3D,T_POINT3D&>& aSelectNode, 
																									CMapEx<int, int, BOOL, BOOL>& mEdgeConnectBndr, BOOL bSecond/*=FALSE*/)
{
//   m_CcpaK.RemoveAll();
//   m_DpaaK.RemoveAll();
//   m_MasterNodeK.RemoveAll();

	int nData = aBoundaryEdge.size(); 
	double (*dPolyLine)[2];		
	dPolyLine = new double[nData][2];     
	for(int i=0; i<nData; i++)
	{
		dPolyLine[i][0] = aBoundaryEdge[i].p2.x;
		dPolyLine[i][1] = aBoundaryEdge[i].p2.y;
	}

	CArray<T_POINT3D,T_POINT3D&> aSelectNode_temp; aSelectNode_temp.RemoveAll();

//   CArray<T_LINE_G,T_LINE_G&> aEdge_temp;
//   aEdge_temp.Copy(aEdge_org);

	double pInts[3] = { 0.0, 0.0, 0.0 };
	BOOL bWarningMessage=FALSE;
	int nDpaaCount = aEdge_Drop.GetSize();
	if (nDpaaCount%4 != 0) { ASSERT(0); return FALSE; }
	for (int i=0; i<nDpaaCount; i+=4)
	{
		T_LINE_G& rLine1 = aEdge_Drop[i];
		T_LINE_G& rLine2 = aEdge_Drop[i+1];
		T_LINE_G& rLine3 = aEdge_Drop[i+2];
		T_LINE_G& rLine4 = aEdge_Drop[i+3];

		// 3. 4각형 각각의 점이 모두 바운더리 안에 들어오는 것들만 포함해서 mesh를 한다.
		double p1_1[2], p2_1[2], p3_1[2], p4_1[2];
		p1_1[0] = rLine1.p1.x; p1_1[1] = rLine1.p1.y;
		p2_1[0] = rLine2.p1.x; p2_1[1] = rLine2.p1.y;
		p3_1[0] = rLine3.p1.x; p3_1[1] = rLine3.p1.y;
		p4_1[0] = rLine4.p1.x; p4_1[1] = rLine4.p1.y;
		BOOL bInP1 = CMathFunc::mathIsInsidePoint2D_Tol(p1_1, nData, dPolyLine, m_dTol, m_bBndrConnect);
		BOOL bInP2 = CMathFunc::mathIsInsidePoint2D_Tol(p2_1, nData, dPolyLine, m_dTol, m_bBndrConnect);
		BOOL bInP3 = CMathFunc::mathIsInsidePoint2D_Tol(p3_1, nData, dPolyLine, m_dTol, m_bBndrConnect);
		BOOL bInP4 = CMathFunc::mathIsInsidePoint2D_Tol(p4_1, nData, dPolyLine, m_dTol, m_bBndrConnect);

		if(bInP1 && bInP2 && bInP3 && bInP4)
		{
			// 4개의 점이 다 들어와야 한다..
			CArray<T_LINE_G,T_LINE_G&> aEdge_temp_new; aEdge_temp_new.RemoveAll();
			aEdge_temp_new.Add(rLine1);
			aEdge_temp_new.Add(rLine2);
			aEdge_temp_new.Add(rLine3);
			aEdge_temp_new.Add(rLine4);

			// 교차하는 점이 있는 경우에는 line을 나누어 주어야 함..
			BOOL bExistIntersect=FALSE;
			for (int k=0; k<aEdge_org.size(); k++)
			{
				for (int l=0; l<aEdge_temp_new.GetSize(); l++)
				{
					if(!IsInterSectTwoLine(aEdge_org[k], aEdge_temp_new[l], pInts)) continue;
					
					T_POINT3D IntersectPt;
					IntersectPt.x = pInts[0];
					IntersectPt.y = pInts[1];
					IntersectPt.z = pInts[2];
					if(!bSecond) aSelectNode_temp.Add(IntersectPt);

					if(mEdgeConnectBndr.Exist(k)) bExistIntersect=TRUE;
					break;
					
				}
			}

			if(bSecond && bExistIntersect) continue;  // 교차점이 있는 경우는 하나의 첫번째에 같이 되기 때문에 넣지 않는다..

			AddEdge(aEdge, rLine1);
			AddEdge(aEdge, rLine2);
			AddEdge(aEdge, rLine3);
			AddEdge(aEdge, rLine4);

		}
		else if(bInP1 || bInP2 || bInP3 || bInP4) bWarningMessage = TRUE; // 부분적으로 걸쳐 있는 것이 있으면 Warning Message를 출력한다..
		else continue;
	}

	int nCcpaCount = aEdge_Colm.GetSize();
	if (nCcpaCount%4 != 0) { ASSERT(0); return FALSE; }
	for (int i=0; i<nCcpaCount; i+=4)
	{
		T_LINE_G& rLine1 = aEdge_Colm[i];
		T_LINE_G& rLine2 = aEdge_Colm[i+1];
		T_LINE_G& rLine3 = aEdge_Colm[i+2];
		T_LINE_G& rLine4 = aEdge_Colm[i+3];

		// 3. 4각형 각각의 점이 모두 바운더리 안에 들어오는 것들만 포함해서 mesh를 한다.
		double p1_1[2], p2_1[2], p3_1[2], p4_1[2];
		p1_1[0] = rLine1.p1.x; p1_1[1] = rLine1.p1.y;
		p2_1[0] = rLine2.p1.x; p2_1[1] = rLine2.p1.y;
		p3_1[0] = rLine3.p1.x; p3_1[1] = rLine3.p1.y;
		p4_1[0] = rLine4.p1.x; p4_1[1] = rLine4.p1.y;
		BOOL bInP1 = CMathFunc::mathIsInsidePoint2D_Tol(p1_1, nData, dPolyLine, m_dTol, m_bBndrConnect);
		BOOL bInP2 = CMathFunc::mathIsInsidePoint2D_Tol(p2_1, nData, dPolyLine, m_dTol, m_bBndrConnect);
		BOOL bInP3 = CMathFunc::mathIsInsidePoint2D_Tol(p3_1, nData, dPolyLine, m_dTol, m_bBndrConnect);
		BOOL bInP4 = CMathFunc::mathIsInsidePoint2D_Tol(p4_1, nData, dPolyLine, m_dTol, m_bBndrConnect);

		if(bInP1 && bInP2 && bInP3 && bInP4)
		{
			// 4개의 점이 다 들어와야 한다..
			CArray<T_LINE_G,T_LINE_G&> aEdge_temp_new; aEdge_temp_new.RemoveAll();
			aEdge_temp_new.Add(rLine1);
			aEdge_temp_new.Add(rLine2);
			aEdge_temp_new.Add(rLine3);
			aEdge_temp_new.Add(rLine4);

			// 교차하는 점이 있는 경우에는 line을 나누어 주어야 함..
			BOOL bExistIntersect=FALSE;
			for (int k=0; k<aEdge_org.size(); k++)
			{
				for (int l=0; l<aEdge_temp_new.GetSize(); l++)
				{
					double pInts[3]={0.0, 0.0, 0.0};
					if(!IsInterSectTwoLine(aEdge_org[k], aEdge_temp_new[l], pInts)) continue;
					
					T_POINT3D IntersectPt;
					IntersectPt.x = pInts[0];
					IntersectPt.y = pInts[1];
					IntersectPt.z = pInts[2];
//           if(aSelectNode.GetSize() > 0)IntersectPt.z = aSelectNode[0].z;
//           else IntersectPt.z = 0.0;
					if(!bSecond) aSelectNode_temp.Add(IntersectPt);

					if(mEdgeConnectBndr.Exist(k)) bExistIntersect=TRUE;
					break;
					
				}
			}

			if(bSecond && bExistIntersect) continue;  // 교차점이 있는 경우는 하나의 첫번째에 같이 되기 때문에 넣지 않는다..

			AddEdge(aEdge, rLine1);
			AddEdge(aEdge, rLine2);
			AddEdge(aEdge, rLine3);
			AddEdge(aEdge, rLine4);
		}
		else if(bInP1 || bInP2 || bInP3 || bInP4) bWarningMessage = TRUE; // 부분적으로 걸쳐 있는 것이 있으면 Warning Message를 출력한다..
		else continue;
	}

	delete []dPolyLine; 

	if(bWarningMessage && m_bShowMsg)
	{
		m_bShowMsg=FALSE;
		if(AfxMessageBox(_LS(IDS_TM_WARNING_AUtO_MESH_DROPPANEL), MB_YESNO|MB_ICONQUESTION) != IDYES) return FALSE;
	}

	// 중복되지 않은 절점만 더한다.
	CArray<T_POINT3D,T_POINT3D&> aSelectNode_add; aSelectNode_add.RemoveAll();
	aSelectNode_temp.Append(aSelectNode);
	if(aSelectNode_temp.GetSize() > 0)
	{
		for (int i=0; i<aSelectNode_temp.GetSize(); i++)
		{
			BOOL bExist=FALSE;
			T_POINT3D& rPt_temp = aSelectNode_temp[i];
			for(int j=0; j<aSelectNode_add.GetSize(); j++)
			{
				T_POINT3D& rPt_sel = aSelectNode_add[j];
				if(fabs(rPt_temp.x - rPt_sel.x)<m_dTol && fabs(rPt_temp.y - rPt_sel.y)<m_dTol && fabs(rPt_temp.z - rPt_sel.z)<m_dTol)
				{
					bExist=TRUE;
					break;
				}
			}
			if(!bExist) aSelectNode_add.Add(rPt_temp);
		}
	}

	if(aSelectNode_add.GetSize() > 0)
	{
		aSelectNode.RemoveAll();
		aSelectNode.Copy(aSelectNode_add);
	}
	return TRUE;
}

void CMeshAutoDlg::MakeEdgeConnectInfo(const std::vector<T_LINE_G>& aEdge_copy, std::vector<T_LINE_G>& aBoundaryEdge, CMapEx<int, int, BOOL, BOOL>& mEdgeConnectBndr, CArray<T_LINE_G,T_LINE_G&>& aEdge_Drop, CArray<T_LINE_G,T_LINE_G&>& aEdge_Colm)
{
	std::vector<T_LINE_G> aBoundaryEdge_temp;
	aBoundaryEdge_temp.resize(aBoundaryEdge.size());
	std::copy(aBoundaryEdge.begin(), aBoundaryEdge.end(), aBoundaryEdge_temp.begin());

	int nDropSize = aEdge_Drop.GetSize();
	for (int i=0; i<nDropSize; i++)
	{
		T_LINE_G& rLine = aEdge_Drop[i];
		if(IsConnectBounary(rLine, aBoundaryEdge_temp)) aBoundaryEdge_temp.push_back(rLine);
	}

	int nColmSize = aEdge_Colm.GetSize();
	for (int i=0; i<nColmSize; i++)
	{
		T_LINE_G& rLine = aEdge_Colm[i];
		if(IsConnectBounary(rLine, aBoundaryEdge_temp)) aBoundaryEdge_temp.push_back(rLine);
	}

	// 1. 각 line을 돌면서 Boundary에 연결되어 있는지 확인함.
	int nEdgeCount = aEdge_copy.size();
	for (int i=0; i<nEdgeCount; i++)
	{
		if(IsConnectBounary(aEdge_copy[i], aBoundaryEdge_temp)) { mEdgeConnectBndr.SetAt(i, TRUE); }
	}

	// 2. Boundary에 연결되어 있지 않는 것들 중에서 연결되어 있는 line이 존재하는지 확인한다.
	for (int i=0; i<nEdgeCount; i++)
	{
		if(mEdgeConnectBndr.Exist(i)) continue;

		CMapEx<int, int, int, int> mUseEdge; mUseEdge.InitHashTable(nEdgeCount);
		mUseEdge.SetAt(i,1);
		if(DetermineConnectionLine(aEdge_copy, aEdge_copy[i], mEdgeConnectBndr, mUseEdge))
		{
			mEdgeConnectBndr.SetAt(i, TRUE);
		}
	}
}

BOOL CMeshAutoDlg::DetermineConnectionLine(const std::vector<T_LINE_G>& aEdge_copy, const T_LINE_G& rLineG, CMapEx<int, int, BOOL, BOOL>& mEdgeConnectBndr, CMapEx<int, int, int, int>& mUseEdge)
{
	int nEdgeCount = aEdge_copy.size();
	double pInts[3] = { 0., };
	for (int i = 0; i < nEdgeCount; i++)
	{
		if (mUseEdge.Exist(i)) continue;
		
		if (IsInterSectTwoLine(rLineG, aEdge_copy[i], pInts))
		{
			if (mEdgeConnectBndr.Exist(i)) { mEdgeConnectBndr.SetAt(i, TRUE); return TRUE; }
			else
			{
				mUseEdge.SetAt(i, 1);
				if (DetermineConnectionLine(aEdge_copy, aEdge_copy[i], mEdgeConnectBndr, mUseEdge)) { mEdgeConnectBndr.SetAt(i, TRUE); return TRUE; }
			}
		}
	}

	return FALSE;
}

BOOL CMeshAutoDlg::IsInterSectTwoLine(const T_LINE_G& rLine1, const T_LINE_G& rLine2, double pInts[3])
{
	double pl1_i[3] = {rLine1.p1.x, rLine1.p1.y, rLine1.p1.z};
	double pl1_j[3] = {rLine1.p2.x, rLine1.p2.y, rLine1.p2.z};

	double pl2_i[3] = {rLine2.p1.x, rLine2.p1.y, rLine2.p1.z};
	double pl2_j[3] = {rLine2.p2.x, rLine2.p2.y, rLine2.p2.z};

	double dblDistance = 0.0;
	return mathIntersectLine2(pl1_i, pl1_j, pl2_i, pl2_j, m_dTol, dblDistance, pInts); // 두개의 선분의 교점
}

BOOL CMeshAutoDlg::IsConnectBounary(const T_LINE_G& rLine1, const std::vector<T_LINE_G>& aBoundaryEdge)
{
	double pl1_i[3] = {rLine1.p1.x, rLine1.p1.y, rLine1.p1.z};
	double pl1_j[3] = {rLine1.p2.x, rLine1.p2.y, rLine1.p2.z};

	double pInts[3] = { 0. };
	double dblDistance = 0.0;

	for (int i=0; i<aBoundaryEdge.size(); i++)
	{
		const T_LINE_G& rLine2 = aBoundaryEdge[i];
		double pl2_i[3] = {rLine2.p1.x, rLine2.p1.y, rLine2.p1.z};
		double pl2_j[3] = {rLine2.p2.x, rLine2.p2.y, rLine2.p2.z};
				
		if(mathIntersectLine2(pl1_i, pl1_j, pl2_i, pl2_j, m_dTol, dblDistance, pInts)) return TRUE;

	}
	return FALSE;
}

void CMeshAutoDlg::RemoveRedundentEdge(std::vector<T_LINE_G>& aEdge)
{
	for (int i=aEdge.size()-1; i>-1; i--)
	{
		BOOL bDel=FALSE;
		T_LINE_G& rLine_src = aEdge[i];
		for (int j=aEdge.size()-1; j>-1; j--)
		{
			if(i==j) continue;
			const T_LINE_G& rLine_tgt = aEdge[j];

			if(rLine_src.IsSameLine(rLine_tgt, m_dTol, FALSE))
			{
				bDel=TRUE;
				break;
			}
		}
		if(bDel) aEdge.erase(aEdge.begin()+i);
	}
}

void CMeshAutoDlg::UpdateBuffer()
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

void CMeshAutoDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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


int CMeshAutoDlg::GF_GetHashSizeByCount(int nDataCount1)
{
	int nDataCount = nDataCount1 * 2 / 10;
	if (nDataCount < 10) return 11;
	if (nDataCount < 50) return 53;
	if (nDataCount < 100) return 101;
	if (nDataCount < 300) return 307;
	if (nDataCount < 600) return 601;
	if (nDataCount < 1000) return 1009;
	if (nDataCount < 3000) return 3001;
	if (nDataCount < 5000) return 5003;
	if (nDataCount < 7000) return 7001;
	if (nDataCount < 10000) return 10007;
	if (nDataCount < 30000) return 30011;
	if (nDataCount < 50000) return 50021;
	if (nDataCount < 70000) return 70001;
	if (nDataCount <100000) return 100003;
	return 150001;
}

int CMeshAutoDlg::FindClosePath()
{
	int nFindType = D_CPF_FIND_TYPE_INNER;
	m_mMembK_checked.RemoveAll();
	m_mEdge.RemoveAll();
	m_mNode.RemoveAll();
	m_mNode_outcycle.RemoveAll();    // 외곽 cycle을 형성하는 edge의 연결 정보만 갖는 임시 node map
	m_mEdge_erase.RemoveAll();  // 폐구간 생성에 이미 사용된 Edge ID
	m_aClosedPath.RemoveAll();    

	T_ELEM_D ElemD;
	int nCount = m_aKeyList.size();
	for (int i=0; i<nCount; i++)
	{
		T_ELEM_K ElemK = m_aKeyList[i];
		ElemD.Initialize();
		if (m_mMembK_checked.Exist(ElemK))  continue;

		m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD);
		if(!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) continue;

		if (!MakeGraphFromMembK(ElemK))  return 0;

		if(m_mNode.GetCount() == 0)   continue;
		if (!RemoveCaltileverFromGraph())  return 0;
		if (m_mEdge.GetCount() < 2)  continue;

		// 바깥쪽 Cycle을 찾는다. (시계 방향으로 찾는다)
		CList<UINT,UINT> listEdgeOfOuterCycle;    listEdgeOfOuterCycle.RemoveAll();
		UINT nNodeId_start = NULL;
		UINT nEdgeId_start = NULL;
		if (!MakeOutCyclePath(listEdgeOfOuterCycle, nNodeId_start, nEdgeId_start))  return FALSE;
		ASSERT(listEdgeOfOuterCycle.GetCount() > 1);

		// 외곽 cycle에 있는 edge는 시계방향으로 돌도록 edge 방향을 잡아준다. 
		// unit cycle을 생성할 때 반시계 방향으로 돌기 위한 방향이 필요하다.
		if (!UpdateCycleEdgeDirection(listEdgeOfOuterCycle, nNodeId_start))  return FALSE;

		// 바깥 cycle에 대한 node map을 따로 만들어준다.
		// MakeUnitCyclePath() 처리시 edge-disjoint union 연산을 하기 위한 용도 
		if (!MakeOutCycleNodeMap(listEdgeOfOuterCycle))  return FALSE;

		// 양파 껍질 벗기듯 unit-cycle을 찾아서 등록한다. (반시계방향으로 찾는다)
		if (!MakeUnitCyclePath(nFindType))  continue;
	}
	return m_aClosedPath.GetSize();
}

void CMeshAutoDlg::InitializeGraph()
{
	m_mEdge.RemoveAll();
	m_mNode.RemoveAll();
	m_mNode_outcycle.RemoveAll();
	m_mEdge_erase.RemoveAll();

	m_mEdge.InitHashTable(9199 );
	m_mNode.InitHashTable(9199 );
	m_mNode_outcycle.InitHashTable(29);
	m_mEdge_erase.InitHashTable(29);

}

// 외곽 cycle에 있는 edge는 시계방향으로 돌도록 edge 방향을 잡아준다. 
// unit cycle을 생성할 때 반시계 방향으로 돌기 위한 방향이 필요하다.
BOOL CMeshAutoDlg::UpdateCycleEdgeDirection(CList<UINT,UINT>& listEdgeOfCycle, UINT nNodeId_start)
{
	UINT nNodeId_pre = nNodeId_start;
	UINT nEdgeId = NULL;
	T_CPF_EDGE edge;
	POSITION pos = listEdgeOfCycle.GetHeadPosition();
	while(pos)
	{
		nEdgeId = listEdgeOfCycle.GetNext(pos);
		if (!m_mEdge.Lookup(nEdgeId, edge))  { ASSERT(0); return FALSE; }

		if (edge.node1 != nNodeId_pre)  
		{
			edge.ReverseNode();   
			if (edge.node1 != nNodeId_pre)  { ASSERT(0); return FALSE; }

			if (!AddOrReplaceEdge(edge))  { ASSERT(0); return FALSE; }
		}

		nNodeId_pre = edge.node2;
	}

	return TRUE;
}

// outer cycle용 node map에 edge 연결정보를 추가하는 함수 
BOOL CMeshAutoDlg::RegisterEdge2Node_outerCycle(UINT nEdgeId)
{
	T_CPF_EDGE edge;   edge.Initialize();
	if (!m_mEdge.Lookup(nEdgeId, edge))  { ASSERT(0); return FALSE; }

	if (!RegisterEdge2Node_outerCycle_sub(nEdgeId, edge.node1))  { ASSERT(0); return FALSE; }
	if (!RegisterEdge2Node_outerCycle_sub(nEdgeId, edge.node2))  { ASSERT(0); return FALSE; }

	return TRUE;
}

BOOL CMeshAutoDlg::RegisterEdge2Node_outerCycle_sub(UINT nEdgeId, UINT nNodeId)
{
	T_CPF_NODE node;   node.Initialize();
	if (!m_mNode_outcycle.Lookup(nNodeId, node))   
	{
		// m_mNode에서 가져다가 새로 하나 만든다.
		node.Initialize();
		if (!m_mNode.Lookup(nNodeId, node))    { ASSERT(0); return FALSE; }

		node.listEdge.RemoveAll();    // 이때 연결된 edge 정보는 reset 시켜야 한다. 
	}

	// node에 edge 연결 정보 추가 
	node.AddEdge(nEdgeId); 

	// m_mNode_outcycle에 node-edge의 연결 정보 업데이트 
	m_mNode_outcycle.SetAt(node.id, node);

	return TRUE;
}
BOOL CMeshAutoDlg::MakeOutCycleNodeMap(const CList<UINT,UINT>& listEdgeOfOuterCycle)
{
	if (listEdgeOfOuterCycle.GetCount() < 2)    { ASSERT(0); return FALSE; }

	CMapEx<UINT,UINT, int,int> mUsedOutEdge;   // OutCycle이 동일 경로를 다시 지나가면 Disjoint 시키기 위한 용도. 
	// O-O 모양으로 생긴 Out Cycle을 분리하기 위함.
	mUsedOutEdge.RemoveAll();
	mUsedOutEdge.InitHashTable(GF_GetHashSizeByCount(listEdgeOfOuterCycle.GetCount()));

	UINT nEdgeId = NULL;
	T_CPF_EDGE edge;   edge.Initialize();
	T_CPF_NODE node;   node.Initialize();
	POSITION pos = listEdgeOfOuterCycle.GetHeadPosition();
	while(pos)
	{
		nEdgeId = listEdgeOfOuterCycle.GetNext(pos);
		if (!mUsedOutEdge.Exist(nEdgeId))
		{
			if (!RegisterEdge2Node_outerCycle(nEdgeId))  { ASSERT(0); return FALSE; }
		}
		else
		{
			// OutCycle이 동일 경로를 다시 지나가면 Disjoint 시킴 
			ASSERT(nEdgeId);

			if (m_mEdge.Lookup(nEdgeId, edge))
			{
				// edge 삭제 
				if (!RemoveEdge(nEdgeId))  return FALSE;

				if (m_mNode_outcycle.Lookup(edge.node1, node) && node.listEdge.GetSize() == 0)  
				{ 
					m_mNode_outcycle.RemoveKey(edge.node1); 
				}

				if (m_mNode_outcycle.Lookup(edge.node2, node) && node.listEdge.GetSize() == 0)  
				{ 
					m_mNode_outcycle.RemoveKey(edge.node2); 
				}

				if (m_mNode.Lookup(edge.node1, node) && node.listEdge.GetSize() == 0)  
				{ 
					m_mNode.RemoveKey(edge.node1); 
				}

				if (m_mNode.Lookup(edge.node2, node) && node.listEdge.GetSize() == 0)  
				{ 
					m_mNode.RemoveKey(edge.node2); 
				}
			}
		}

		mUsedOutEdge.SetAt(nEdgeId, 0);
	}

	return TRUE;
}

// 양파 껍질 벗기듯 unit-cycle을 찾아서 등록한다. (반시계방향으로 찾는다)
// m_mNode_outcycle를 사용.
BOOL CMeshAutoDlg::MakeUnitCyclePath(int nFindType/*=D_CPF_FIND_TYPE_INNER*/)
{
	POSITION pos           = NULL;
	UINT     nNodeId_iter  = NULL;
	UINT     nNodeId_start = NULL;
	UINT     nEdgeId_start = NULL;
	T_CPF_NODE node_iter ;    node_iter .Initialize();
	T_CPF_EDGE edge_start;    edge_start.Initialize();
	CList<UINT,UINT> listEdgeOfUnitCycle;   listEdgeOfUnitCycle.RemoveAll();
	CList<UINT,UINT> listNodeOfUnitCycle;   listNodeOfUnitCycle.RemoveAll();
	BOOL bFirstChance = TRUE;
	int nMaxLoopCount = m_mNode.GetCount();
	int iLoop = 0;
	while (m_mNode_outcycle.GetCount() > 0)
	{
		// 무한루프 방지용.
		if (iLoop > nMaxLoopCount)  break;
		iLoop++;

		// 시작 node 및 edge를 설정한다. 
		nNodeId_iter = NULL;
		node_iter.Initialize();
//     if (bFirstChance && m_bUseStartNode)
//     {
//       nNodeId_iter = m_idNodeStart;
//       if (!m_mNode_outcycle.Lookup(nNodeId_iter, node_iter))    nNodeId_iter = NULL;
//     }
		bFirstChance = FALSE;

		if (nNodeId_iter == NULL)
		{
			pos = m_mNode_outcycle.GetStartPosition();
			m_mNode_outcycle.GetNextAssoc(pos, nNodeId_iter, node_iter);
		}

		if (node_iter.listEdge.GetSize() < 2)  { ASSERT(0); return FALSE; }
		ASSERT(node_iter.listEdge.GetSize() == 2);    // 외곽 node(m_mNode_outcycle)는 항상 2점만 물고 있어야 한다. 

		nEdgeId_start = node_iter.listEdge[0];
		if (!m_mEdge.Lookup(nEdgeId_start, edge_start)) { ASSERT(0); return FALSE; }

		// nNodeId_iter를 시작점으로 폐구간을 형성하기 위한 조치.
		// edge_start.node2 -> edge_start.node1 방향이 반시계방향이기 때문에 
		// edge_start.node2를 시작점으로 폐구간을 찾게된다. 
		if (edge_start.node2 != nNodeId_iter)
		{
			nEdgeId_start = node_iter.listEdge[1];
			if (!m_mEdge.Lookup(nEdgeId_start, edge_start)) { ASSERT(0); return FALSE; }
		}

		// 반시계방향으로 돌기 위해 외곽 cycle의 반대방향으로 돈다.
		// 이미 앞에서 UpdateCycleEdgeDirection()를 통해서 외곽 cycle은 시계 방향으로 설정이 되어 있어야 한다. 
		// (반시계 방향으로 돌면서) 반시계 방향의 edge를 찾아서 unit cycle을 만든다. 
		// 반시계방향 = edge_start.node2 -> edge_start.node1 방향
		listEdgeOfUnitCycle.RemoveAll();
		listNodeOfUnitCycle.RemoveAll();
		nNodeId_start = edge_start.node2;
		int nRet = GetCyclePathCCW(edge_start, nNodeId_start, listEdgeOfUnitCycle, &listNodeOfUnitCycle, TRUE, ((nFindType==D_CPF_FIND_TYPE_OUTER)? m_mNode_outcycle : m_mNode));
		if (nRet == 0)  { ASSERT(0); return FALSE; }
		//     else if (nRet == 2)  { ASSERT(0); return TRUE;  }   // loop cycle을 찾기는 했으나 형상이 정상적이지 않은 경우 
		//                                                         // loop가 시작 node로 돌아오지 않고 중간에서 만난 경우 

		// cycle의 회전방향 검사 
		if (listNodeOfUnitCycle.GetCount() > 2) 
		{
			// 반시계 방향의 cycle이 안 나온 경우 회전방향을 바꿔준다. 
			if (!CheckCyclePathCCW(listNodeOfUnitCycle))   ReverseCycle(listEdgeOfUnitCycle, listNodeOfUnitCycle);
		}

		if (listEdgeOfUnitCycle.GetCount() < 2)  { ASSERT(0); return FALSE; }

		// 찾아낸 unit cycle을 등록한다.
		T_CPF_CYCLE_PATH cycle;
		cycle.Set(0, listEdgeOfUnitCycle, listNodeOfUnitCycle);
		m_aClosedPath.Add(cycle);

		// unit cycle에 있는 edge는 반시계방향으로 돌도록 edge 방향을 잡아준다. 
		// DisjointUnionEdge_outerCycle() 연산시 outer cycle의 path를 갱신하는데, 
		// 이때도 outer cycle의 방향을 시계방향으로 유지시켜주기 위함임. 
		if (!UpdateCycleEdgeDirection(listEdgeOfUnitCycle, listNodeOfUnitCycle.GetHead()))  return FALSE;

		// unit cycle과 outer cycle을 edge-disjoint union 연산해서 outer cycle을 갱신한다. 
		// 이때 edge와의 연결이 남지 않은 (m_mNode_outcycle 상의)node는 삭제된다.
		DisjointUnionEdge_outerCycle(listEdgeOfUnitCycle);

		// cantilever edge가 발생하면 삭제한다. 
		RemoveCantileverNode_connectedOnly(listNodeOfUnitCycle);
	}

	return TRUE;
}

// 시작점은 유지시키면서 회전 방향을 바꿔준다. 
BOOL CMeshAutoDlg::ReverseCycle(CList<UINT,UINT>& rlistEdgeOfCycle, CList<UINT,UINT>& rlistNodeOfCycle)
{
	if (rlistEdgeOfCycle.GetCount() < 2)    { ASSERT(0); return FALSE; }
	if (rlistNodeOfCycle.GetCount() < 2)    { ASSERT(0); return FALSE; }

	// reverse node
	CList<UINT,UINT> listNodeOfCycle_temp;    listNodeOfCycle_temp.RemoveAll();
	listNodeOfCycle_temp.AddTail(rlistNodeOfCycle.RemoveHead());
	while(!rlistNodeOfCycle.IsEmpty())
	{
		listNodeOfCycle_temp.AddTail(rlistNodeOfCycle.RemoveTail());
	}

	// reverse edge
	CList<UINT,UINT> listEdgeOfCycle_temp;    listEdgeOfCycle_temp.RemoveAll();
	// listEdgeOfCycle_temp.AddTail(rlistEdgeOfCycle.RemoveHead());
	while(!rlistEdgeOfCycle.IsEmpty())
	{
		listEdgeOfCycle_temp.AddTail(rlistEdgeOfCycle.RemoveTail());
	}

	// 반환값 
	rlistEdgeOfCycle.RemoveAll();
	POSITION posEdge = listEdgeOfCycle_temp.GetHeadPosition();
	while (posEdge)  {  rlistEdgeOfCycle.AddTail( listEdgeOfCycle_temp.GetNext(posEdge) );  }

	rlistNodeOfCycle.RemoveAll();
	POSITION posNode = listNodeOfCycle_temp.GetHeadPosition();
	while (posNode)  {  rlistNodeOfCycle.AddTail( listNodeOfCycle_temp.GetNext(posNode) );  }

	return TRUE;
}
// unit cycle과 outer cycle을 edge-disjoint union 연산해서 
// outer cycle의 node-edge 연결관계를 갱신한다.
BOOL CMeshAutoDlg::DisjointUnionEdge_outerCycle(const CList<UINT,UINT>& listEdge)
{
	BOOL bDisconnect = FALSE;
	UINT nEdgeId = NULL;
	T_CPF_EDGE edge;   edge.Initialize();
	POSITION pos = listEdge.GetHeadPosition();
	while(pos)
	{
		nEdgeId = listEdge.GetNext(pos);
		if (!m_mEdge.Lookup(nEdgeId, edge))  { ASSERT(0); return FALSE; }

		// node가 edge.id를 가지고 있으면 삭제하고, 없으면 추가하는 연산.
		if (!DisjointUnionEdge(edge.node1, edge.id, m_mNode_outcycle, bDisconnect))  { ASSERT(0); return FALSE; }
		if (!DisjointUnionEdge(edge.node2, edge.id, m_mNode_outcycle, bDisconnect))  { ASSERT(0); return FALSE; }
	}

	return TRUE;
}

// nodeId가 cantilever node이면 node 및 연결된 edge를 삭제한다. 
BOOL CMeshAutoDlg::RemoveCantileverNode_connectedOnly(const CList<UINT,UINT>& listNode)
{
	UINT nodeId = 0;
	POSITION pos = listNode.GetHeadPosition();
	while(pos)
	{
		nodeId = listNode.GetNext(pos);
		if (!m_mNode.Exist(nodeId))   continue;
		if (!RemoveCantileverNode_connectedOnly( nodeId ))  return FALSE;
	}

	return TRUE;
}

// nodeId가 cantilever node이면 node 및 연결된 edge를 삭제한다. 
BOOL CMeshAutoDlg::RemoveCantileverNode_connectedOnly(UINT nodeId)
{
	T_CPF_NODE node;
	if (!m_mNode.Lookup(nodeId, node))  { ASSERT(0); return TRUE; }
	ASSERT(nodeId == node.id);

	// node에 연결된 edge 갯수를 센다. 
	int nCountEdge = node.GetConnectedEdgeCount();

	if (nCountEdge == 0)            // _tremove free node 
	{ 
		RemoveNode(nodeId);  
	}
	else if (nCountEdge == 1)       // edeg가 1개이면 cantilever node
	{ 
		// edge로 연결된 다음 node로 이동해서 cantilever를 삭제한다. 
		UINT nodeId_next = 0;
		T_CPF_EDGE edge;
		if (m_mEdge.Lookup(node.listEdge[0], edge))
		{
			if (edge.node1 == nodeId)   nodeId_next = edge.node2;
			else if (edge.node2 == nodeId)   nodeId_next = edge.node1;
			else  { ASSERT(0); return FALSE; }

			// edge 삭제 
			RemoveEdge(edge.id);
			RemoveNode(nodeId);

			RemoveCantileverNode_connectedOnly(nodeId_next);
		}
	}

	return TRUE;
}

// nodeId가 edgeId를 가지고 있으면 연결을 끊고, 없으면 edgeId를 연결하는 연산.
// 이 연산과정에서 edge가 남지 않은 node는 삭제한다. 
// @param rbDisconnect : [out] 연결이 끊겼는지 여부.
BOOL CMeshAutoDlg::DisjointUnionEdge(UINT nodeId, UINT edgeId, T_CPF_NODE_MAP& mapNode, BOOL& rbDisconnect)
{
	rbDisconnect = FALSE;
	T_CPF_NODE node;   node.Initialize();
	if (mapNode.Lookup(nodeId, node))  
	{ 
		for (int i=node.listEdge.GetSize()-1; i>=0; --i)
		{
			if (node.listEdge[i] == edgeId)
			{
				node.listEdge.RemoveAt(i);    // 연결을 끊는다. 

				rbDisconnect = TRUE;
				m_mEdge_erase.SetAt(edgeId, 0);
				RemoveEdge(edgeId);     // edge 삭제 
				break;
			}
		}

		// 끊어지는 edge가 없으면 edge를 추가 연결한다. 
		if (!rbDisconnect) 
		{ 
			// 이미 삭제되었던 edge가 아니라면 
			if (!m_mEdge_erase.Exist(edgeId)) node.AddEdge(edgeId); 
		}

		if (node.listEdge.GetSize() > 0)    mapNode.SetAt(node.id, node);   // 갱신 
		else                                mapNode.RemoveKey(node.id);     // 연결 edge가 안남으면 node 삭제 
	}
	else
	{
		// boundary상에 없던 node가 들어오면 mapNode에 node 추가 
		if (!RegisterEdge2Node_outerCycle_sub(edgeId, nodeId))  return FALSE;
	}

	return TRUE;
}
// 반시계방향을 돌고 있는 node list인가? 
//NewellsMethod, 회전 방향 판단 
BOOL CMeshAutoDlg::CheckCyclePathCCW(const CList<UINT,UINT>& listNodeOfUnitCycle)
{
	int nCountNode = listNodeOfUnitCycle.GetCount();
	if (nCountNode < 3)   { ASSERT(0); return FALSE; }

	CArray<T_CPF_NODE, T_CPF_NODE&> aNode;   aNode.RemoveAll();

	UINT idNode_temp = 0;
	T_CPF_NODE node_temp;   node_temp.Initialize();
	aNode.SetSize(nCountNode+1);

	int index = 0;
	POSITION posNode = listNodeOfUnitCycle.GetHeadPosition();
	while (posNode)
	{
		idNode_temp = listNodeOfUnitCycle.GetNext(posNode);
		if (!m_mNode.Lookup(idNode_temp, node_temp))  { ASSERT(0);  continue; }
		aNode[index] = node_temp;
		index++;
	}
	aNode[nCountNode] = aNode[0];

	ASSERT(nCountNode > 2);
	double nor_z=0.;
	for (int i=0; i<nCountNode; ++i)  { nor_z = nor_z + (aNode[i].x-aNode[i+1].x)*(aNode[i].y+aNode[i+1].y); }
	if (nor_z < 0.0)  return FALSE;

	return TRUE;  
}

BOOL CMeshAutoDlg::MakeGraphFromMembK(T_KEY ElemK)
{
	T_ELEM_D ElemD;   ElemD.Initialize();
	m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD);
	if(!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) return FALSE;

	MakeGraphFromPontK(ElemD.elnod[0]);

	return TRUE;
}
BOOL CMeshAutoDlg::MakeGraphFromPontK(T_KEY PontK)
{
	InitializeGraph();

	CList<T_KEY,T_KEY> listQueuePontK;    // 재귀 함수를 호출하는 경우 depth가 너무 깊어질 거 같아서 queue를 사용함. 
	listQueuePontK.RemoveAll();
	listQueuePontK.AddTail(PontK);

	T_NODE_K PontK1 = NULL;
	T_NODE_K PontK2 = NULL;
	T_NODE_K PontK_iter = NULL;
	std::vector<UINT> aElemK, aElemK_All;   
	T_ELEM_D ElemD;   ElemD.Initialize();

	CMapEx<UINT,UINT,int,int> mPontK_CurPlaneChecked;
	CMapEx<UINT,UINT,int,int> mMembK_CurPlaneChecked;

	double P1[3], P2[3], P3[3];
	double dA, dB, dC, dH;    // Plane Equation
	BOOL bFindPlane = FALSE;
	
	while(listQueuePontK.GetCount() > 0)
	{
		PontK_iter = listQueuePontK.RemoveHead();
		if(mPontK_CurPlaneChecked.Exist(PontK_iter))    continue;
		mPontK_CurPlaneChecked.SetAt(PontK_iter,1);

		// point에 연결된 Member들 
		aElemK_All.clear();
		m_pDoc->m_pAttrCtrl->GetConnectedElem(PontK_iter, aElemK_All);

		aElemK.clear();
		CArrayUtil::IntersectUINT(aElemK_All, m_aKeyList, aElemK);

		// 연결된 Member를 Edge에 등록 
		int nCount = (int)aElemK.size();
		for (int i=0; i<nCount; ++i)
		{
			T_ELEM_K ElemK = aElemK[i];
			if(mMembK_CurPlaneChecked.Exist(ElemK))   continue;

			if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))  { ASSERT(0); return FALSE; }
			
			T_NODE_D NodeD1, NodeD2;
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) {ASSERT(0); return FALSE;}
			if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) {ASSERT(0); return FALSE;}
			

			// 처음 두개의 edge로 평면을 구성하고 3번째 부터는 같은 평면에 존재해야만 추가한다.
			int nEdgeCount = m_mEdge.GetCount();
			if(nEdgeCount == 0)
			{
				P1[0] = NodeD1.x;      P1[1] = NodeD1.y;     P1[2] = NodeD1.z;
				P2[0] = NodeD2.x;      P2[1] = NodeD2.y;     P2[2] = NodeD2.z;
			}
			else
			{
				T_NODE_D OtherPontD = (ElemD.elnod[0] == PontK_iter)?   NodeD2:      NodeD1;
				P3[0] = OtherPontD.x;      P3[1] = OtherPontD.y;     P3[2] = OtherPontD.z;

				if(bFindPlane == FALSE)
				{
					// 이미 등록 된 경로인지
					if(IsAlreadyExistPath(ElemD))   continue;

					// 처음 엣지와 일직선상의 부재가 아닐 때만 평면식을 구한다.
					double dAxis1[3] = {0.0};
					mathNormalize(P2[0]-P1[0], P2[1]-P1[1], P2[2]-P1[2], dAxis1[0], dAxis1[1], dAxis1[2]);
					double dAxis2[3] = {0.0};
					mathNormalize(P3[0]-P2[0], P3[1]-P2[1], P3[2]-P2[2], dAxis2[0], dAxis2[1], dAxis2[2]);
					double dAngle = mathCrossAngle(dAxis1, dAxis2);    // +-0~180
					if(fabs(dAngle) > 1 && fabs(dAngle) < 179)
					{
						// 평면의 방정식을 구한다. (+/- 구분 X)
						mathPlaneEquation(P1, P2, P3, dA, dB, dC, dH);
						if(math_lib::is_zero(dC))   continue;            // X-Y 평면에 수직인 평면은 스킵
						bFindPlane = TRUE;
					}
				}
				else
				{
					double dDist = mathDistanceToPlane(dA, dB, dC, dH, P3[0], P3[1], P3[2]);
					if(fabs(dDist) > 1.0E-6)   continue;
				}
			}

			// Member를 Edge에 등록 
			AddMember2Edge(ElemK, ElemD);

			//----------------------------------------------------
			// edge(Member)로 연결되는 다른 node(point)를 queue에 넣는다. 
			PontK1 = ElemD.elnod[0];
			PontK2 = ElemD.elnod[1];
			ASSERT(PontK1 != PontK2);

			if (PontK1 == PontK_iter)         listQueuePontK.AddTail(PontK2);
			else if (PontK2 == PontK_iter)    listQueuePontK.AddTail(PontK1);
			else                              { ASSERT(0); return FALSE; }
			//----------------------------------------------------

			m_mMembK_checked.SetAt(ElemK, 0);
			mMembK_CurPlaneChecked.SetAt(ElemK, 1);
		}
	}

	return TRUE;
}

BOOL CMeshAutoDlg::IsAlreadyExistPath(const T_ELEM_D& ElemD)
{
	BOOL bExistPath = FALSE;

	T_KEY_LIST aNodeK;
	int nNode = m_mNode.GetKeyList(aNodeK);
	if(nNode < 2)      {  ASSERT(0);		return FALSE;	}

	if(m_mNode.Exist(ElemD.elnod[0]))    aNodeK.Add(ElemD.elnod[1]);
	else  aNodeK.Add(ElemD.elnod[0]);

	int nPathSize = m_aClosedPath.GetSize();
	for(int k = 0; k < nPathSize; k++)
	{
		T_CPF_CYCLE_PATH CPath = m_aClosedPath[k];
		int nNodeSize = CPath.aNodeId.GetSize();
		CMapEx<UINT,UINT,int,int> mNodeID;
		for(int m = 0; m < nNodeSize; m++)    mNodeID.SetAt(CPath.aNodeId[m], 1);

		if(mNodeID.Exist(aNodeK[0]) && mNodeID.Exist(aNodeK[1]) && mNodeID.Exist(aNodeK[2]))
		{
			bExistPath = TRUE;
			break;
		}
	}

	return bExistPath;
}

BOOL CMeshAutoDlg::AddMember2Edge(T_ELEM_K ElemK, T_ELEM_D& ElemD)
{
	// 이미 edge로 등록 되었는지 검사.
	T_CPF_EDGE edge;    edge.Initialize();
	if (m_mEdge.Lookup(ElemK, edge))  { ASSERT(0); return FALSE; }

	// edge 생성/추가 
	edge.Initialize();
	edge.id = ElemK;
	edge.node1 = ElemD.elnod[0];
	edge.node2 = ElemD.elnod[1];
	if (edge.node1 == edge.node2)  { ASSERT(0); return FALSE; }

	T_NODE_D NodeD1, NodeD2;
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) {ASSERT(0); return FALSE;}
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) {ASSERT(0); return FALSE;}

	edge.cx = (NodeD1.x+NodeD2.x)/2.0;
	edge.cy = (NodeD1.y+NodeD2.y)/2.0;

	// 동일한 edge가 있는지 검사 : 동일한 위치에 edge가 있다면 skip
	if (ExistSameEdge(edge))  return TRUE;


	if (!AddOrReplaceEdge(edge))  { ASSERT(0); return FALSE; }

	if (!RegisterEdge2Node(edge.id, edge.node1))  { ASSERT(0); return FALSE; }
	if (!RegisterEdge2Node(edge.id, edge.node2))  { ASSERT(0); return FALSE; }

	return TRUE;
}
// 같은 edge인가?
// 같은 edge의 판단은 양 끝 node가 동일한지와, edge 중간점이 동일한지를 비교.
// 방향은 비교하지 않는다. 
BOOL CMeshAutoDlg::ExistSameEdge(const T_CPF_EDGE& edge)
{
	T_CPF_NODE node;
	if (!m_mNode.Lookup(edge.node1, node))  return FALSE;
	if (!m_mNode.Lookup(edge.node2, node))  return FALSE;

	BOOL bExistSameEdge = FALSE;
	double dTol = 1.0E-6; //M_InitValueCurUnit(4.0e-2, KN, M, D_UNITSYS_BASE_LENGTH);
	T_CPF_EDGE edge_comp;   edge_comp.Initialize();
	int nCountEdge = node.listEdge.GetSize();
	for (int i=0; i<nCountEdge; ++i)
	{
		if (!m_mEdge.Lookup(node.listEdge[i], edge_comp))  { ASSERT(0); return FALSE; }

		if (edge_comp.id == edge.id)  continue;     // 동일한 edge인 경우 

		// 다른 node를 물고 있는 경우 
		if ((edge_comp.node1 != edge.node1) && (edge_comp.node1 != edge.node2)) continue;     // node1
		if ((edge_comp.node2 != edge.node1) && (edge_comp.node2 != edge.node2)) continue;     // node2

		// line 중간점이 다른 경우 
		if (fabs(edge.cx-edge_comp.cx) > dTol) continue;
		if (fabs(edge.cy-edge_comp.cy) > dTol) continue;

		bExistSameEdge = TRUE;
		m_mMembK_checked.SetAt(edge.id, 0);
		break;
	}

	return bExistSameEdge;
}

BOOL CMeshAutoDlg::RegisterEdge2Node(UINT edgeId, UINT nodeId)
{
	T_CPF_NODE node;   node.Initialize();
	if (!GetOrMakeNode(nodeId, node))  { ASSERT(0); return FALSE; }

	// node에 edge 연결 정보 추가 
	node.AddEdge(edgeId);

	// node 정보 갱신 
	AddOrReplaceNode(node);

	return TRUE;
}

BOOL CMeshAutoDlg::AddOrReplaceEdge(T_CPF_EDGE& edge)
{
	if (edge.node1 == edge.node2)   { ASSERT(0); return FALSE; }

	m_mEdge.SetAt(edge.id, edge);

	return TRUE;
}

// nodeId를 가지고 해당 node의 T_CPF_NODE 객체 반환
// nodeId의 해당 node가 없다면 정보를 생성해서 반환 
BOOL CMeshAutoDlg::GetOrMakeNode(UINT nodeId, T_CPF_NODE& node)
{
	node.Initialize();

	if (!m_mNode.Lookup(nodeId, node))
	{
		T_NODE_K PontK = nodeId;
		T_NODE_D PontD;   PontD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetNode(PontK, PontD))  { ASSERT(0); return FALSE; }

		// 없다면 node 추가 
		node.Initialize();
		node.id = nodeId;
		node.x  = PontD.x;
		node.y  = PontD.y;
	}

	return TRUE;
}



BOOL CMeshAutoDlg::AddOrReplaceNode(T_CPF_NODE& node)
{
	m_mNode.SetAt(node.id, node);

	return TRUE;
}

// 한 node에서 부터 연결된 edge를 뒤지면서 edge가 끝단 node(edge가 하나만 연결된 node)로 연결되면 
// node 및 edge를 지우면서 진행 한다. 
BOOL CMeshAutoDlg::RemoveCaltileverFromGraph()
{
	if (m_mNode.GetCount() == 0)  { ASSERT(0); return FALSE; } 

	CMapEx<UINT,UINT, int,int> mNodeId_passed;    // 테스트해 본 node map 
	mNodeId_passed.RemoveAll();
	mNodeId_passed.InitHashTable(GF_GetHashSizeByCount(m_mNode.GetCount()));

	CArray<UINT,UINT> aNodeId;    aNodeId.RemoveAll();
	int nCountNode = m_mNode.GetKeyList(aNodeId);
	for (int i=0; i<nCountNode; ++i)
	{
		if (!m_mNode.Exist(aNodeId[i]))  continue;    // RemoveCaltieverFromGraph_sub()에서 node가 삭제될 수 있음.

		if (!RemoveCaltieverFromGraph_sub(aNodeId[i], mNodeId_passed))  return FALSE;
	}

	if (mNodeId_passed.GetCount() != nCountNode)  { ASSERT(0); return FALSE; }  // 모든 node에 대해서 검사했는가?

	return TRUE;
}

// cantilever edge 및 node 삭제 (재귀함수)
BOOL CMeshAutoDlg::RemoveCaltieverFromGraph_sub(UINT nodeId, CMapEx<UINT,UINT, int,int>& mNodeId_passed)
{
	// 거쳐간 node는 skip
	if (mNodeId_passed.Exist(nodeId))  return TRUE;

	// passed node map에 등록한다. 
	mNodeId_passed.SetAt(nodeId, 0);

	T_CPF_NODE node;
	if (!m_mNode.Lookup(nodeId, node))  { return TRUE; }
	ASSERT(nodeId == node.id);

	// node에 연결된 edge 갯수를 센다. 
	int nCountEdge = node.GetConnectedEdgeCount();

	// edeg가 1개이면 cantilever node
	if (nCountEdge == 0)        { ASSERT(0); return FALSE; }
	else if (nCountEdge == 1)   
	{ 
		// edge로 연결된 다음 node로 이동해서 cantilever를 삭제한다. 
		UINT nodeId_next = 0;
		T_CPF_EDGE edge;
		if (m_mEdge.Lookup(node.listEdge[0], edge))
		{
			if (edge.node1 == nodeId)   nodeId_next = edge.node2;
			else if (edge.node2 == nodeId)   nodeId_next = edge.node1;
			else  { ASSERT(0); return FALSE; }
		}

		// cantilever node 삭제 
		RemoveCantileverNode(node.id, &mNodeId_passed);

		// 다음 node 검사.
		RemoveCaltieverFromGraph_sub(nodeId_next, mNodeId_passed);
	}
	else if (nCountEdge > 1)
	{
		// edge로 연결된 다음 node로 이동해서 cantilever를 삭제한다. 
		T_CPF_EDGE edge;
		for (int i=0; i<nCountEdge; ++i)
		{
			if (!m_mEdge.Lookup(node.listEdge[i], edge))   continue;   // 삭제 과정에서 이미 삭제되어 있을 수 있다. 

			if      (edge.node1 == nodeId)   RemoveCaltieverFromGraph_sub(edge.node2, mNodeId_passed);
			else if (edge.node2 == nodeId)   RemoveCaltieverFromGraph_sub(edge.node1, mNodeId_passed);
			else  { ASSERT(0); return FALSE; }
		}
	}
	else { ASSERT(0); return FALSE; }

	return TRUE;
}

// cantilever node를 삭제하면서 연결된 다른 cantilever node도 쭈~욱~ 삭제한다. 
BOOL CMeshAutoDlg::RemoveCantileverNode(UINT nodeId, CMapEx<UINT,UINT, int,int>* pmNodeId_passed/*=NULL*/)
{
	T_CPF_NODE nodeOld;
	if (!m_mNode.Lookup(nodeId, nodeOld))  { return TRUE; }

	if (nodeOld.listEdge.GetSize() > 1)  return TRUE;   // cantilever node 가 아니라면 
	ASSERT(nodeOld.listEdge.GetSize() <= 1);            // cantilever node

	// cantilever node를 재귀적으로 삭제한다. -------

	if (pmNodeId_passed)  pmNodeId_passed->SetAt(nodeId, 0);

	// node 삭제 
	RemoveNode(nodeId);

	if (nodeOld.listEdge.GetSize() > 0)
	{
		UINT edgeId_canti = nodeOld.listEdge[0];    ASSERT(edgeId_canti);
		T_CPF_EDGE edge;
		if (m_mEdge.Lookup(edgeId_canti, edge))
		{
			// node에 연결된 cantilever edge 삭제 
			RemoveEdge(edgeId_canti);

			// edge와 연결되어 있던 cantilever node 삭제 
			if      (edge.node1 == nodeId)  RemoveCantileverNode(edge.node2, pmNodeId_passed);
			else if (edge.node2 == nodeId)  RemoveCantileverNode(edge.node1, pmNodeId_passed);
			else    { ASSERT(0); return FALSE; }
		}
	}

	return TRUE;
}
BOOL CMeshAutoDlg::RemoveNode(UINT nodeId)
{
	// node 삭제 
	m_mNode.RemoveKey(nodeId);
	m_mNode_outcycle.RemoveKey(nodeId);

	return TRUE;
}
BOOL CMeshAutoDlg::RemoveEdge(UINT edgeId)
{
	T_CPF_EDGE edgeOld;
	if (!m_mEdge.Lookup(edgeId, edgeOld))  return TRUE;

	// edge 삭제 
	m_mEdge.RemoveKey(edgeId);

	// edge가 연결하고 있던 node에서 edge 정보 삭제 
	T_CPF_NODE node;
	if (m_mNode.Lookup(edgeOld.node1, node))  { node.RemoveEdge(edgeOld.id);  AddOrReplaceNode(node); }
	if (m_mNode.Lookup(edgeOld.node2, node))  { node.RemoveEdge(edgeOld.id);  AddOrReplaceNode(node); }

	if (m_mNode_outcycle.Lookup(edgeOld.node1, node))  { node.RemoveEdge(edgeOld.id);  m_mNode_outcycle.SetAt(edgeOld.node1, node); }
	if (m_mNode_outcycle.Lookup(edgeOld.node2, node))  { node.RemoveEdge(edgeOld.id);  m_mNode_outcycle.SetAt(edgeOld.node2, node); }

	return TRUE;
}

BOOL CMeshAutoDlg::MakeOutCyclePath(CList<UINT,UINT>& rlistEdgeOfOuterCycle, UINT& rnNodeId_start,UINT& rnEdgeId_start)
{
	rlistEdgeOfOuterCycle.RemoveAll();
	rnNodeId_start = NULL;
	rnEdgeId_start = NULL;

	// cycle을 형성하려면 node가 2개 이상이어야 한다. 
	if (m_mNode.GetCount() < 2)  { ASSERT(0); return FALSE; }

	// 바깥쪽에 있는 node들 중에서 임의의 한개 node를 얻어온다.
	// 바깥쪽 cycle을 찾기위한 첫 node 
	T_CPF_NODE nodeStart;   nodeStart.Initialize();
	GetFirstNodeForOuterCycle(nodeStart, m_mNode);
	rnNodeId_start = nodeStart.id;

	// 시계방향으로 돌면서 가장 바깥쪽 cycle을 그릴 수 있는 연결 Path를 찾는다.
	T_CPF_EDGE edgeOuterCW_start;   edgeOuterCW_start.Initialize();  // 시계방향으로 돌기위한 가장 바깥쪽 path 
	if (!GetFirstEdgeForOuterCycle(nodeStart, edgeOuterCW_start))  { ASSERT(0); return FALSE; }
	rnEdgeId_start = edgeOuterCW_start.id;

	// (시계 방향으로 돌면서) 반시계 방향의 edge를 찾아서 외곽 cycle을 만든다. 
	BOOL bSmallCycle=FALSE;   // 최대한 큰 cycle을 만든다. 
	if (GetCyclePathCCW(edgeOuterCW_start, nodeStart.id, rlistEdgeOfOuterCycle, NULL, bSmallCycle, m_mNode) != 1)  { ASSERT(0); return FALSE; }

	return TRUE;
}

#define DO_NOTHING()  0 // 말 그대로 아무 것도 안 하는 것 : 코드 가독성 효과

// 바깥쪽에 있는 node들 중에서 임의의 한개 node를 얻어온다.
// 바깥쪽 cycle을 찾기위한 첫 node 
BOOL CMeshAutoDlg::GetFirstNodeForOuterCycle(T_CPF_NODE& rNodeOut, CMapEx<UINT,UINT, T_CPF_NODE,T_CPF_NODE&>& mNode)    
{
	// cycle을 형성하려면 node가 2개 이상이어야 한다. 
	if (mNode.GetCount() < 2)  { ASSERT(0); return FALSE; }

	// node 중에서 제일 바깥쪽 좌상단 node를 찾는다. 
	UINT nNodeID_iter = NULL;
	T_CPF_NODE node_iter;   node_iter.Initialize();
	POSITION pos = mNode.GetStartPosition();

	// 첫 node 
	if (pos)   { mNode.GetNextAssoc(pos, nNodeID_iter, node_iter);  rNodeOut = node_iter; }

	double dTol = 1.0e-9;
	int nCompX = 0;
	int nCompY = 0;
	while (pos)
	{
		mNode.GetNextAssoc(pos, nNodeID_iter, node_iter);

		nCompX = CCompFunc::CompRealTol(node_iter.x, rNodeOut.x, dTol);
		if (nCompX < 0)  rNodeOut = node_iter;
		else if (nCompX > 0)  DO_NOTHING();
		else    // if (nCompX == 0)
		{
			nCompY = CCompFunc::CompRealTol(node_iter.y, rNodeOut.y, dTol);
			if (nCompY > 0)  rNodeOut = node_iter;
		}
	}

	return TRUE;
}

// 시계방향으로 돌면서 가장 바깥쪽 cycle을 그릴 수 있는 연결 Path를 찾는다.
// @param rEdgeOuterCW : 시계방향으로 돌기위한 첫번째 가장 바깥쪽 path 
BOOL CMeshAutoDlg::GetFirstEdgeForOuterCycle(const T_CPF_NODE& nodeStart, T_CPF_EDGE& rEdgeOuterCW)
{
	int nCountEdge = nodeStart.listEdge.GetSize();
	if (nCountEdge < 2)  { ASSERT(0); return FALSE; }   // cantilever를 제거한 상태에서 들어와야 한다. 

	if (!m_mEdge.Lookup(nodeStart.listEdge[0], rEdgeOuterCW))  { ASSERT(0); return FALSE; }
	double dAngle = GetPathAngleCCW(nodeStart.x-1.0, nodeStart.y,
		nodeStart.x, nodeStart.y,
		rEdgeOuterCW.cx, rEdgeOuterCW.cy);

	int nCCW = 1;   // 1:CCW, -1:CW, 0:수평
	T_CPF_EDGE edge_comp;   edge_comp.Initialize();
	for (int i=1; i<nCountEdge; ++i)
	{
		if (!m_mEdge.Lookup(nodeStart.listEdge[i], edge_comp))  { ASSERT(0); return FALSE; }

		nCCW = CMathFunc::math_ccw(nodeStart.x, nodeStart.y,  rEdgeOuterCW.cx, rEdgeOuterCW.cy,  edge_comp.cx, edge_comp.cy);
		if (nCCW == 1)
		{
			// 제일 바깥쪽 좌상단 -> 우상단 방향의 가장 바깥쪽 edge
			double dAngle_comp = GetPathAngleCCW(nodeStart.x-1.0, nodeStart.y,
				nodeStart.x, nodeStart.y,
				edge_comp.cx, edge_comp.cy);

			if (dAngle_comp > dAngle) rEdgeOuterCW = edge_comp;
		}
	}

	return TRUE;
}

// @return : 0 - 못 찾음.
//           1 - 정상적으로 찾음.
//           2 - 시작 node를 벗어나서 찾음. (올챙이 모양의 loop)
int CMeshAutoDlg::GetCyclePathCCW(const T_CPF_EDGE& edge_start, UINT nNodeID_start, CList<UINT,UINT>& rlistEdgeOfCycle, CList<UINT,UINT>* plistNodeOfCycle, BOOL bSmallCycle, const T_CPF_NODE_MAP& m_mNode_working)
{
	T_CPF_EDGE edge_prev = edge_start;

	// cycle에 edge 추가 
	rlistEdgeOfCycle.AddTail(edge_prev.id);
	if (plistNodeOfCycle)  plistNodeOfCycle->AddTail(nNodeID_start);

	int nUsedCount = 0;
	CMapEx<UINT,UINT, int,int> mEdge_used;    mEdge_used.RemoveAll();
	mEdge_used.SetAt(edge_prev.id, 1);

	UINT nodeID_joint = edge_prev.GetOppositeNode(nNodeID_start);

	T_CPF_EDGE edge_next;    edge_next.Initialize();
	BOOL bOpened      = TRUE ;    // path가 열려있는가?
	while(bOpened)
	{
		// 시작점으로 돌아왔는가? 
		if (nodeID_joint == nNodeID_start)    { bOpened = FALSE;  break; }

		// ccw방향 다음 edge를 얻어온다. 
		if (!GetNextEdgeCCW(edge_prev, nodeID_joint, edge_next, bSmallCycle, m_mNode_working)) return 0;

		//----------------------------------------------
		// MNET:XXXX-HSSHIM-20081023
		// 시작 node로 돌아오지 않는 경우의 예외처리 (하나의 보 밑에 분할된 벽이 있는 경우)
		// 이미 한바퀴 돌았다면 (시작 node로 안 이어지면서 cycle이 형성되는 경우)
		if (mEdge_used.Lookup(edge_next.id, nUsedCount))
		{
			if (bSmallCycle || nUsedCount > 2)   // 내부 폐구간을 찾을때 || 왕복해서 지나간 다음에도 계속 지나려고 할 때 
			{
				UINT edgeId_remove = rlistEdgeOfCycle.RemoveHead();
				if (plistNodeOfCycle)  plistNodeOfCycle->RemoveHead();
				while (edgeId_remove != edge_next.id && rlistEdgeOfCycle.GetCount() > 0)
				{
					edgeId_remove = rlistEdgeOfCycle.RemoveHead();
					if (plistNodeOfCycle)  plistNodeOfCycle->RemoveHead();
				}

				//       rlistEdgeOfCycle.AddHead(edge_next.id);
				//       if (plistNodeOfCycle)  plistNodeOfCycle->AddHead(nodeID_joint);

				if (rlistEdgeOfCycle.GetCount() > 1)  return 2;
				else                                  return 0;
			}

			nUsedCount++;
			mEdge_used.SetAt(edge_next.id, nUsedCount);
		}
		else mEdge_used.SetAt(edge_next.id, 0);
		//----------------------------------------------

		// cycle에 edge 추가 
		rlistEdgeOfCycle.AddTail(edge_next.id);
		if (plistNodeOfCycle)  plistNodeOfCycle->AddTail(nodeID_joint);

		nodeID_joint = edge_next.GetOppositeNode(nodeID_joint);
		edge_prev = edge_next;
	}
	ASSERT(!bOpened);

	return 1;
}

// @param bSmallCycle : 얇은 Loop를 걸러내기 위한 파라미터 
//                      보/벽의 intersect가 제대로 되어 있지않은 경우 Cycle 자체가 line일 수 있다. 
BOOL CMeshAutoDlg::GetNextEdgeCCW(const T_CPF_EDGE& edge_prev, UINT nodeID_joint, T_CPF_EDGE& rEdge_next, BOOL bSmallCycle, const T_CPF_NODE_MAP& m_mNode_working)
{
	T_CPF_NODE nodeJoint;
	if (!m_mNode_working.Lookup(nodeID_joint, nodeJoint))  { ASSERT(0); return FALSE; }

	int nCountEdge = nodeJoint.listEdge.GetSize();
	if (nCountEdge < 2)  { ASSERT(0); return FALSE; }   // cantilever를 제거한 상태에서 들어와야 한다. 

	double dAngle_next   = 0.0;     // edgeOuterCW_prev에서 rEdgeOuterCW_next로의 사이각 
	double dAngle        = 0.0;
	BOOL   bSetFirstEdge = FALSE;   // 연결되는 다음 Edge를 설정했는지 여부.
	int    nCCW          = 1;       // 1:CCW, -1:CW, 0:수평
	T_CPF_EDGE edge_comp;   edge_comp.Initialize();
	for (int i=0; i<nCountEdge; ++i)
	{
		if (nodeJoint.listEdge[i] == edge_prev.id)  continue;

		if (!m_mEdge.Lookup(nodeJoint.listEdge[i], edge_comp))  { ASSERT(0); return FALSE; }

		// 첫번째 다음 path 
		if (!bSetFirstEdge)
		{
			rEdge_next = edge_comp;
			if (nCountEdge == 2)  break;      // 속도 개선을 위해. 분기가 아니면 더이상 진행할 필요가 없다. 

			bSetFirstEdge = TRUE;
			dAngle_next = GetPathAngleCCW(edge_prev.cx, edge_prev.cy,
				nodeJoint.x, nodeJoint.y,
				rEdge_next.cx, rEdge_next.cy);
			continue;
		}

		dAngle = GetPathAngleCCW(edge_prev.cx, edge_prev.cy,
			nodeJoint.x, nodeJoint.y,
			edge_comp.cx, edge_comp.cy);

		//     // dAngle이 0인 경우의 예외처리 (edge경로가 왔다가 되돌아가는 경우)
		//     // 주로 벽과 보가 같이 있는 구간에서 분할이 제대로 안된 경우에 나타남. 
		//     if (bSmallCycle)
		//     {
		//       double dTol = 0.1;    // 0.1 degree 
		//       double dCheckAng = (dAngle > 180.0)? (dAngle-360.0) : dAngle;
		//       if (fabs(dCheckAng) < dTol)  dAngle = dAngle_next +1.0e-9;    // dAngle > dAngle_next 상태로 만들어서 아래 코드가 작동하게 한다. 
		//     }

		if (dAngle > dAngle_next)   { rEdge_next = edge_comp;  dAngle_next = dAngle;  }
	}

	return TRUE;
}

// a -> b -> c 로의 각도를 0 ~ 360도 사이 값으로 반환.
// 각도는 반시계 방향으로 계산 
double CMeshAutoDlg::GetPathAngleCCW(double a0, double a1,  double b0, double b1,  double c0, double c1)
{
	double v1[2] = {0.0};   v1[0] = a0-b0;   v1[1] = a1-b1;
	double v2[2] = {0.0};   v2[0] = c0-b0;   v2[1] = c1-b1;

	double u1x, u1y, u2x, u2y;    // 단위 벡터 
	CMathFunc::mathNormalize(v1[0], v1[1],  u1x, u1y);
	CMathFunc::mathNormalize(v2[0], v2[1],  u2x, u2y);

	double dAngle = CMathFunc::mathCrossAngle2D(u1x, u1y,  u2x, u2y);
	// CMathFunc::mathCrossAngle2D()에서 부호 판정을 해주지 않아서 여기서 함.
	int nCCW = CMathFunc::math_ccw(a0, a1,  b0, b1,  c0, c1);   // 1:CCW, -1:CW, 0:수평

	if (nCCW == 1) dAngle = 360.0 - dAngle;

	return dAngle;
}

void CMeshAutoDlg::MakePolyLineMergingStraightLine(CArray<UINT,UINT>& aNodeK, std::vector<T_LINE_G>& aBoundaryEdge)
{
	aBoundaryEdge.clear();

	int nPont = aNodeK.GetSize();
	CArray<T_NODE_D, T_NODE_D&> aNodeD;     aNodeD.SetSize(nPont);
	for(int k = 0; k < nPont; k++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetNode(aNodeK[k], aNodeD[k]))     {  ASSERT(0);		return;	}
	}

	aBoundaryEdge.reserve(nPont + 1);

	for(int k = 1; k < nPont; k++)
	{
		T_LINE_G NewLine;
		NewLine.p1.x = aNodeD[k-1].x;
		NewLine.p1.y = aNodeD[k-1].y;
		NewLine.p1.z = aNodeD[k-1].z;
		
		
		NewLine.p2.x = aNodeD[k].x;
		NewLine.p2.y = aNodeD[k].y;
		NewLine.p2.z = aNodeD[k].z;

		aBoundaryEdge.push_back(NewLine);
	}

	T_LINE_G LastLine;
	LastLine.p1.x = aNodeD[nPont-1].x;
	LastLine.p1.y = aNodeD[nPont-1].y;
	LastLine.p1.z = aNodeD[nPont-1].z;


	LastLine.p2.x = aNodeD[0].x;
	LastLine.p2.y = aNodeD[0].y;
	LastLine.p2.z = aNodeD[0].z;
	aBoundaryEdge.push_back(LastLine);
}

void CMeshAutoDlg::GetNodeListRemoveDup(CString& strNodeList, std::vector<UINT>& aNodeList)
{
	GetNodeList(strNodeList, aNodeList);

	int nTemp = 0;
	CMapEx<int, int, int, int> mapKeyList;
	mapKeyList.RemoveAll();
	mapKeyList.InitHashTable((int)aNodeList.size());
	for (int i = 0; i < aNodeList.size(); i++)
		mapKeyList.SetAt(aNodeList[i], nTemp);
	
	aNodeList.clear();
	aNodeList.reserve(mapKeyList.GetSize());
	POSITION pos = mapKeyList.GetStartPosition();
	while (pos)
	{
		mapKeyList.GetNextKey(pos, nTemp);
		aNodeList.push_back(nTemp);
	}
}

double CMeshAutoDlg::mathDistanceToLine(double line_i[3], double line_j[3], double point[3])
{
	double a, b, c, l, m, n, x, y, z;
	double dx, dy, dz, px, py, pz;

	x = point[0], y = point[1], z = point[2];
	a = line_i[0], b = line_i[1], c = line_i[2];
	dx = line_j[0] - a, dy = line_j[1] - b, dz = line_j[2] - c;

	double dLength = mathLength(dx, dy, dz);
	mathNormalize(dx, dy, dz, l, m, n);
	double t;
	if (fabs(l) < m_NormalZero && fabs(m) < m_NormalZero && fabs(n) < m_NormalZero) t = 0.0;
	else t = ((x - a)*l + (y - b)*m + (z - c)*n) / (l*l + m * m + n * n);
	if (t <= 0.)
	{
		px = line_i[0], py = line_i[1], pz = line_i[2];
	}
	else if (t >= dLength)
	{
		px = line_j[0], py = line_j[1], pz = line_j[2];
	}
	else
	{
		px = a + t * l;
		py = b + t * m;
		pz = c + t * n;
	}
	return mathLength(point[0] - px, point[1] - py, point[2] - pz);
}

double CMeshAutoDlg::mathDistanceToPoint(double p1[3], double p2[3])
{
	double dx = p1[0] - p2[0];
	double dy = p1[1] - p2[1];
	double dz = p1[2] - p2[2];
	return sqrt(dx*dx + dy * dy + dz * dz);
}

BOOL CMeshAutoDlg::mathIntersectLine2(double pl1_i[3], double pl1_j[3], double pl2_i[3], double pl2_j[3], double Tolerance, double& dblDistance, double pInts[3])
{
	pInts[0] = 0.;
	pInts[1] = 0.;
	pInts[2] = 0.;

	double u1x = pl1_j[0] - pl1_i[0];
	double u1y = pl1_j[1] - pl1_i[1];
	double u1z = pl1_j[2] - pl1_i[2];
	double u1, v1, w1;
	if (!mathNormalize(u1x, u1y, u1z, u1, v1, w1))return FALSE;

	double u2x = pl2_j[0] - pl2_i[0];
	double u2y = pl2_j[1] - pl2_i[1];
	double u2z = pl2_j[2] - pl2_i[2];
	double u2, v2, w2;
	if (!mathNormalize(u2x, u2y, u2z, u2, v2, w2))return FALSE;

	double i21_x = pl2_i[0] - pl1_i[0];
	double i21_y = pl2_i[1] - pl1_i[1];
	double i21_z = pl2_i[2] - pl1_i[2];

	double vw = v1 * w2 - v2 * w1;
	double wu = w1 * u2 - w2 * u1;
	double uv = u1 * v2 - u2 * v1;

	double d = sqrt((vw)*(vw)+(wu)*(wu)+(uv)*(uv));
	//if(fabs(d) < 1.e-15)return FALSE;
	//if(fabs(d) < 1.e-7)return FALSE;
	//if(fabs(d) < 1.e-4)return FALSE;
	if (d < 1.e-6)return FALSE;

	dblDistance = fabs(((i21_x)*(vw)-(i21_y)*(-wu) + (i21_z)*(uv)) / d);
	if (dblDistance > Tolerance)return FALSE;

	double t = 0.0;
	double eps = 1.e-7;

	double q1 = uv;
	double q2 = -wu;
	double q3 = vw;

	double maxq = __max(__max(fabs(q1), fabs(q2)), fabs(q3));
	if (maxq < eps)return FALSE;
	if (maxq == fabs(q1))      t = (u2*(-i21_y) - v2 * (-i21_x)) / q1;
	else if (maxq == fabs(q2)) t = (u2*(-i21_z) - w2 * (-i21_x)) / q2;
	else if (maxq == fabs(q3)) t = (v2*(-i21_z) - w2 * (-i21_y)) / q3;
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}

	// L1 선분 내에 있는지 검사
	if (t < -(1e-8 + Tolerance)) return FALSE;

	double dL1Length = mathLength(u1x, u1y, u1z);
	double dDiff = dL1Length + 1e-8;
	if (t > dDiff)
	{
		if (fabs(t - dDiff) > Tolerance)
			return FALSE;
	}


	q1 = -uv;
	q2 = wu;
	q3 = -vw;

	maxq = __max(__max(fabs(q1), fabs(q2)), fabs(q3));
	if (maxq < eps)return FALSE;
	if (maxq == fabs(q1))      t = (u1*(i21_y)-v1 * (i21_x)) / q1;
	else if (maxq == fabs(q2)) t = (u1*(i21_z)-w1 * (i21_x)) / q2;
	else if (maxq == fabs(q3)) t = (v1*(i21_z)-w1 * (i21_y)) / q3;
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}

	// L2 선분 내에 있는지 검사
	if (t < -(1e-8 + Tolerance)) return FALSE;

	double dL2Length = mathLength(u2x, u2y, u2z);
	dDiff = dL2Length + 1e-8;
	if (t > dDiff)
	{
		if (fabs(t - dDiff) > Tolerance)
			return FALSE;
	}


	// pl2선상의 교점을 구한다.
	pInts[0] = pl2_i[0] + u2 * t;
	pInts[1] = pl2_i[1] + v2 * t;
	pInts[2] = pl2_i[2] + w2 * t;

	return TRUE;
}

BOOL CMeshAutoDlg::mathNormalize(double dx, double dy, double dz, double& dxn, double& dyn, double& dzn)
{
	double Length = mathLength(dx, dy, dz);
	dxn = 0.;
	dyn = 0.;
	dzn = 0.;
	if (Length < m_NormalZero)return FALSE;
	dxn = dx / Length;
	dyn = dy / Length;
	dzn = dz / Length;
	return TRUE;
}

double CMeshAutoDlg::mathLength(double dx, double dy, double dz)
{
	return sqrt(dx*dx + dy * dy + dz * dz);
}

double CMeshAutoDlg::mathCrossAngle(double Vector1[3], double Vector2[3])
{
	// Normalized 된 벡터만 써야한다.
	double v = mathDot(Vector1, Vector2);
	double r = mathAcos(v);
	return r * m_trang;
}

double CMeshAutoDlg::mathDot(double xyz1[3], double xyz2[3])
{
	double dblDot = xyz1[0] * xyz2[0] + xyz1[1] * xyz2[1] + xyz1[2] * xyz2[2];
	if (dblDot > 1.)dblDot = 1.;
	if (dblDot < -1.)dblDot = -1.;
	return dblDot;
}

double CMeshAutoDlg::mathAcos(double x)
{
	if (x < -1)x = -1.;
	if (x > 1)x = 1.;
	return acos(x);
};

void CMeshAutoDlg::mathCross_product_3d(const double xyz0[3], const double xyz1[3], const double xyz2[3], double result_vector[3])
{
	double vector01[3], vector02[3];
	vector01[0] = xyz1[0] - xyz0[0];
	vector01[1] = xyz1[1] - xyz0[1];
	vector01[2] = xyz1[2] - xyz0[2];
	vector02[0] = xyz2[0] - xyz0[0];
	vector02[1] = xyz2[1] - xyz0[1];
	vector02[2] = xyz2[2] - xyz0[2];
	result_vector[0] = vector01[1] * vector02[2] - vector02[1] * vector01[2];
	result_vector[1] = vector02[0] * vector01[2] - vector01[0] * vector02[2];
	result_vector[2] = vector01[0] * vector02[1] - vector02[0] * vector01[1];
}

BOOL CMeshAutoDlg::mathIncludePointInPlane(double p1[3], double p2[3], double p3[3], double point[3], double Tol)
{
	double a, b, c, h;
	double dDistance = 0.;
	if (!mathPlaneEquation(p1, p2, p3, a, b, c, h)) return FALSE;

	dDistance = mathDistanceToPlane(a, b, c, h, point[0], point[1], point[2]);
	if (fabs(dDistance) < Tol)
		return TRUE;
	else
		return FALSE;
}

BOOL CMeshAutoDlg::mathPlaneEquation(double p1[3], double p2[3], double p3[3], double& a, double& b, double& c, double & h)
{
	a = 0., b = 0., c = 0., h = 0.;

	double aa, bb, cc, dd;
	aa = p1[1] * (p2[2] - p3[2]) - p2[1] * (p1[2] - p3[2]) + p3[1] * (p1[2] - p2[2]);
	bb = p1[0] * (p3[2] - p2[2]) + p2[0] * (p1[2] - p3[2]) + p3[0] * (p2[2] - p1[2]);
	cc = p1[0] * (p2[1] - p3[1]) - p2[0] * (p1[1] - p3[1]) + p3[0] * (p1[1] - p2[1]);
	dd = mathSqrt(aa*aa + bb * bb + cc * cc);
	if (dd < m_NormalZero)return FALSE;
	a = aa / dd;
	b = bb / dd;
	c = cc / dd;
	h = -(a*p1[0] + b * p1[1] + c * p1[2]);
	return TRUE;
}

double CMeshAutoDlg::mathDistanceToPlane(double a, double b, double c, double h, double x, double y, double z)
{
	double aa = fabs(a*x + b * y + c * z + h);
	double dd = mathSqrt(a*a + b * b + c * c);
	if (dd < m_NormalZero)return -1.;
	return aa / dd;
}

double CMeshAutoDlg::mathSqrt(double x)
{
	if (x < 0)x = 0.;
	return sqrt(x);
};

BOOL CMeshAutoDlg::mathIsPointOfLine2D(double bound1[2], double bound2[2], double targetPt[2], BOOL isOnLine)
{
	double dNormalZero = m_NormalZero * 10000.0;
	double dTol = max(mathLength(bound1[0] - bound2[0], bound1[1] - bound2[1]), m_NormalZero);
	if (fabs(targetPt[0] - bound1[0]) < dNormalZero*dTol && fabs(targetPt[1] - bound1[1]) < dNormalZero*dTol)
		return TRUE;
	if (fabs(targetPt[0] - bound2[0]) < dNormalZero*dTol && fabs(targetPt[1] - bound2[1]) < dNormalZero*dTol)
		return TRUE;

	double dTemp = 0.0;

	double vector1[2];	
	vector1[0] = bound1[0] - targetPt[0];
	vector1[1] = bound1[1] - targetPt[1];
	mathNormalize(vector1[0], vector1[1], dTemp, vector1[0], vector1[1], dTemp);
	double vector2[2];
	vector2[0] = bound2[0] - targetPt[0];
	vector2[1] = bound2[1] - targetPt[1];
	mathNormalize(vector2[0], vector2[1], dTemp, vector2[0], vector2[1], dTemp);

	// 외적(vector product , ^ )이 0이면 두 벡터는 평행하다.// SHIN (2006.2.4) e-15로 했을경우 오차범위가 너무 좁음   
	if (fabs(mathCross2D(vector1, vector2)) < 1.0e-10 && !(fabs(vector1[0] - vector2[0]) < dNormalZero*dTol && fabs(vector1[1] - vector2[1]) < dNormalZero*dTol))
		return TRUE;

	return FALSE;
}

double CMeshAutoDlg::mathCross2D(double vector1[2], double vector2[2])
{
	return vector1[0] * vector2[1] - vector1[1] * vector2[0];
}

void CMeshAutoDlg::mathCross(double xyz1[3], double xyz2[3], double xyzout[3])
{
	double xyztemp[3];
	xyztemp[0] = xyz1[1] * xyz2[2] - xyz1[2] * xyz2[1];
	xyztemp[1] = xyz1[2] * xyz2[0] - xyz1[0] * xyz2[2];
	xyztemp[2] = xyz1[0] * xyz2[1] - xyz1[1] * xyz2[0];
	xyzout[0] = xyztemp[0];
	xyzout[1] = xyztemp[1];
	xyzout[2] = xyztemp[2];
}

double CMeshAutoDlg::mathCrossAngleNormalize(double Vector1[3], double Vector2[3])
{
	double d1 = Vector1[0] * Vector1[0] + Vector1[1] * Vector1[1] + Vector1[2] * Vector1[2];
	double d2 = Vector2[0] * Vector2[0] + Vector2[1] * Vector2[1] + Vector2[2] * Vector2[2];
	d1 = sqrt(d1);
	d2 = sqrt(d2);
	double dTol = 1.0e-10;
	if (fabs(d1) < dTol) { ASSERT(0); return 0; }
	if (fabs(d2) < dTol) { ASSERT(0); return 0; }
	double NV1[3], NV2[3];
	for (int i = 0; i < 3; i++)
	{
		NV1[i] = Vector1[i] / d1;
		NV2[i] = Vector2[i] / d2;
	}
	return mathCrossAngle(NV1, NV2);
}

void CMeshAutoDlg::mathRotate(double angle, double ux, double uy, double uz, double& rx, double& ry, double& rz)
{
	if (angle == 0.)return;

	double d1 = sqrt(uy*uy + uz * uz);
	double phi = 0.;
	if (d1 >= 1.e-15)
	{
		phi = asin(uy / d1)*m_trang;
		if (uz < 0.)phi = 180. - phi;
	}

	double d2 = sqrt(ux*ux + uy * uy + uz * uz);
	double theta = 0.;
	if (d2 >= 1.e-15)theta = asin(ux / d2)*m_trang;

	if (phi != 0.)CMathFunc::mathRotateX(phi, rx, ry, rz);
	if (theta != 0.)CMathFunc::mathRotateY(-theta, rx, ry, rz);
	CMathFunc::mathRotateZ(angle, rx, ry, rz);
	if (theta != 0.)CMathFunc::mathRotateY(theta, rx, ry, rz);
	if (phi != 0.)CMathFunc::mathRotateX(-phi, rx, ry, rz);

	if (fabs(rx) < 1.e-10)rx = 0.;
	if (fabs(ry) < 1.e-10)ry = 0.;
	if (fabs(rz) < 1.e-10)rz = 0.;
}

void CMeshAutoDlg::mathRotate(double angle, double px, double py, double pz, double ux, double uy, double uz, double& rx, double& ry, double& rz)
{
	if (angle == 0.)return;
	double rxx = rx - px;
	double ryy = ry - py;
	double rzz = rz - pz;
	mathRotate(angle, ux, uy, uz, rxx, ryy, rzz);
	rx = rxx + px;
	ry = ryy + py;
	rz = rzz + pz;
	if (fabs(rx) < 1.e-10)rx = 0.;
	if (fabs(ry) < 1.e-10)ry = 0.;
	if (fabs(rz) < 1.e-10)rz = 0.;
	return;
}