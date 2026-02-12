// StldHprsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldHprsDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
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
// CStldHprsDlg dialog


CStldHprsDlg::CStldHprsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldHprsDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pBitmap = 0;
	m_pLoadTypeBitmap = 0;
	//{{AFX_DATA_INIT(CStldHprsDlg)
	m_nSelectionMethod = 1;
	m_nOption = 0;
	m_dFactorA = 0;
	m_dFactorB = 0;
	m_dFactorC = 0;
	m_nCurveType = 0;
	//}}AFX_DATA_INIT
	m_aCtrlOption.Add(IDC_TM_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_OPT_REPLACE);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE);

	m_aCtrlDisable.Add(IDC_TM_ELEM_TYPE);
	m_aCtrlDisable.Add(IDC_TM_LOADED_EDGE);
	m_aCtrlDisable.Add(IDC_TM_GRAD_DIR);
	m_aCtrlDisable.Add(IDC_TM_REF_LEVEL);
	m_aCtrlDisable.Add(IDC_TM_CONST_INTEN);
	m_aCtrlDisable.Add(IDC_TM_GRAD_INTEN);
	m_aCtrlDisable.Add(IDC_TM_STATIC2       );
	m_aCtrlDisable.Add(IDC_TM_SELECT_BY_NODE);
	m_aCtrlDisable.Add(IDC_TM_SELECT_BY_ELEM);
	m_aCtrlDisable.Add(IDC_TM_DIR_TITLE2);
	m_aCtrlDisable.Add(IDC_TM_DIR_COMBO2);
	//m_aCtrlDisable.Add(IDC_TM_EDGE_FACE);


	m_arCtrlDirection.Add(IDC_TM_DIR_TITLE2);
	m_arCtrlDirection.Add(IDC_TM_DIR_COMBO2);

	m_arCtrlProject.Add(IDC_TM_PROJ_TITLE);
	m_arCtrlProject.Add(IDC_TM_PROJ_YES);
	m_arCtrlProject.Add(IDC_TM_PROJ_NO);

	m_arLinearType.Add(IDC_TM_HPRS_STATIC1);
	m_arLinearType.Add(IDC_TM_CONST_INTEN);
	m_arLinearType.Add(IDC_TM_CONST_INTEN_UNIT);
	m_arLinearType.Add(IDC_TM_HPRS_STATIC2);
	m_arLinearType.Add(IDC_TM_GRAD_INTEN);
	m_arLinearType.Add(IDC_TM_GRAD_INTEN_UNIT);

	m_arCurvedType.Add(IDC_TM_CURVE_MTD_RDO);
	m_arCurvedType.Add(IDC_TM_CURVE_MTD_RDO2);
	m_arCurvedType.Add(IDC_TM_CURVE_MTD_RDO3);
	m_arCurvedType.Add(IDC_TM_A_TEXT);
	m_arCurvedType.Add(IDC_TM_B_TEXT);
	m_arCurvedType.Add(IDC_TM_C_TEXT);
	m_arCurvedType.Add(IDC_TM_A_EDIT);
	m_arCurvedType.Add(IDC_TM_B_EDIT);
	m_arCurvedType.Add(IDC_TM_C_EDIT);
	m_arCurvedType.Add(IDC_TM_CURVE_UNIT_STATIC);
	m_arCurvedType.Add(IDC_TM_UP2);
}

CStldHprsDlg::~CStldHprsDlg()
{
	if (m_pBitmap) delete m_pBitmap;
	if (m_pLoadTypeBitmap) delete m_pLoadTypeBitmap;
}

void CStldHprsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldHprsDlg)
	DDX_Control(pDX, IDC_TM_DIR_COMBO2, m_cboDirection);
	DDX_Control(pDX, IDC_TM_REF_LEVEL_UNIT, m_wndRefLevelUnit);
	DDX_Control(pDX, IDC_TM_GRAD_INTEN_UNIT, m_wndGradIntenUnit);
	DDX_Control(pDX, IDC_TM_CONST_INTEN_UNIT, m_wndConstIntenUnit);
	DDX_Control(pDX, IDC_TM_UP2, m_wndCurvedIntenUnit);
	DDX_Control(pDX, IDC_TM_GRAD_INTEN, m_wndGradInten);
	DDX_Control(pDX, IDC_TM_CONST_INTEN, m_wndConstInten);
	DDX_Control(pDX, IDC_TM_REF_LEVEL, m_wndRefLevel);
	DDX_Control(pDX, IDC_TM_GRAD_DIR, m_wndGradDir);
	DDX_Control(pDX, IDC_TM_LOADED_EDGE_TITLE, m_wndDirTitle);
	DDX_Control(pDX, IDC_TM_LOADED_EDGE, m_wndDirCobx);
	DDX_Control(pDX, IDC_TM_ELEM_TYPE, m_wndElemType);
	DDX_Control(pDX, IDC_TM_LOAD_TYPE, m_wndLoadType);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Text(pDX, IDC_TM_A_EDIT, m_dFactorA);
	DDX_Text(pDX, IDC_TM_B_EDIT, m_dFactorB);
	DDX_Text(pDX, IDC_TM_C_EDIT, m_dFactorC);
	DDX_Radio(pDX, IDC_TM_SELECT_BY_NODE, m_nSelectionMethod);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_TM_CURVE_MTD_RDO, m_nCurveType);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);

	DDX_Control(pDX, IDC_TM_EDGE_FACE, m_wndEdgeFacePicture);
	DDX_Control(pDX, IDC_LOADTYPE_IMG, m_wndLoadTypePicture);

	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStldHprsDlg message handlers
void CStldHprsDlg::SetDirection(int nElemType)
{
	CString csItem;

	switch(nElemType)
	{
	case 0: // Plate
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Direction___));
		m_wndDirCobx.ResetContent();
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Local_x));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Local_y));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Local_z));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Global_X));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
		break;
	case 1: // Plane Stress
	case 2: // Plane Strain
	case 3: // Axisymmetric
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Pressure_Edge__));
		m_wndDirCobx.ResetContent();
		for(int i=0; i < 4; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Edge___d), i+1);
			m_wndDirCobx.AddString(csItem);
		}
		break;
	case 4: // 8 Nodes Solid (Face 6)
	case 5: // 6 Nodes Solid (Face 5)
	case 6: // 4 Nodes Solid (Face 4)
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Pressure_Face__));
		m_wndDirCobx.ResetContent();
		for(int i=0; i < 10-nElemType; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Face___d), i+1);
			m_wndDirCobx.AddString(csItem);
		}
		break;
	}
}

void CStldHprsDlg::ChangeBitmap(int nElemType)
{
	CString aEdgeFacePicture[] = {
		_T("SVG\\illustration\\Dialog\\tm_hprs2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_hprs1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_hprs1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_hprs1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_hprs5.svg"),
	};
	m_wndEdgeFacePicture.SetImage(aEdgeFacePicture[nElemType]);
}

void CStldHprsDlg::GetElemByType(int nOption, int nElemType, CArray<UINT, UINT> &aSelKey)
{
	int i;
	T_ELEM_D ElemData;
	int nNodeNum;

	if (nOption == 2) // delete
	{
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (ElemData.eltyp == PLATE_EL || 
					ElemData.eltyp == PLSTRS_EL ||
					ElemData.eltyp == PLSTRN_EL ||
					ElemData.eltyp == AXISYM_EL ||
					ElemData.eltyp == SOLID_EL) continue;
			aSelKey.RemoveAt(i);
		}
		return;
	}

	switch(nElemType)
	{
	case 0:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp)) aSelKey.RemoveAt(i);
		}
		break;
	case 1:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlstrs(ElemData.eltyp)) aSelKey.RemoveAt(i);
		}
		break;
	case 2:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlstrn(ElemData.eltyp)) aSelKey.RemoveAt(i);
		}
		break;
	case 3:
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsAxisym(ElemData.eltyp)) aSelKey.RemoveAt(i);
		}
		break;
	case 4:
	case 5:
	case 6:
		if (nElemType == 4) nNodeNum = 8;
		else if (nElemType == 5) nNodeNum = 6;
		else nNodeNum = 4;
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsSolid(ElemData.eltyp)) aSelKey.RemoveAt(i);
			else 
			{
				if (m_pDoc->m_pAttrCtrl->GetNumNode(ElemData.eltyp, ElemData.elnod) != nNodeNum)
					aSelKey.RemoveAt(i);
			}
		}
		break;
	}
}


BEGIN_MESSAGE_MAP(CStldHprsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldHprsDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE, OnChangeElemType)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_CBN_SELCHANGE(IDC_TM_GRAD_DIR, OnChangeGradDir)
	ON_CBN_SELCHANGE(IDC_TM_LOADED_EDGE, OnSelchangeTmLoadedEdge)
	ON_CBN_SELCHANGE(IDC_TM_DIR_COMBO2, OnSelchangeTmDirCombo2)
	ON_CBN_SELCHANGE(IDC_TM_LOAD_TYPE, OnSelchangeTmLoadType)
	ON_BN_CLICKED(IDC_TM_SELECT_BY_NODE, OnTmSelectionMethod)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_SELECT_BY_ELEM, OnTmSelectionMethod)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_CURVE_MTD_RDO, OnTmCurveMtdRdo)
	ON_BN_CLICKED(IDC_TM_CURVE_MTD_RDO2, OnTmCurveMtdRdo)
	ON_BN_CLICKED(IDC_TM_CURVE_MTD_RDO3, OnTmCurveMtdRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldHprsDlg message handlers
void CStldHprsDlg::DistPrjCtrl()
{
	CRect rRef1, rRef2;

	GetDlgItem(IDC_TM_DIR_TITLE2)->GetWindowRect(rRef1);
	GetDlgItem(IDC_TM_PROJ_TITLE)->GetWindowRect(rRef2);

	m_nDistPrjCtrl = rRef2.top - rRef1.top;
}

BOOL CStldHprsDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();

	m_nSelectionMethod = 1;
	m_nOption = 0;

	UpdateData(FALSE);
	
	// TODO: Add extra initialization here
	
	m_wndElemType.ResetContent();
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Plate));
 // m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Plane_Stress));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Plane_Strain));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Axisymmetric));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Solid));
	
	/*
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_8_Nodes_Solid));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_6_Nodes_Solid));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_4_Nodes_Solid));
	*/

	m_wndGradDir.AddString(_LS(IDS_WG_TREEMENU_Global___X_));
	m_wndGradDir.AddString(_LS(IDS_WG_TREEMENU_Global___Y_));
	m_wndGradDir.AddString(_LS(IDS_WG_TREEMENU_Global___Z_));
	m_wndGradDir.SetCurSel(2);  // Default : Global (-Z)

	m_cboDirection.ResetContent();
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Normal));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
	m_cboDirection.SetCurSel(0);

	m_wndLoadType.AddString(_LS(IDS_WG_TREEMENU_Linear_Loads));
	m_wndLoadType.AddString(_LS(IDS_WG_TREEMENU_Curved_Loads));
	m_wndLoadType.SetCurSel(0);

	((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);  
	CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, FALSE);
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);   // Project 항목의 기본값.

	// Reference Level
	m_wndRefLevel.SetAttWcsPos();
	m_wndRefLevel.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);  // Default : Global (-Z)
	m_wndRefLevel.SetWindowText(_T("0"));

	// Set Unit
	//m_wndRefLevel.SetUnitType(CUnitCtrl::m_HPRS_UNIT.dReferenceLevel);
	m_wndConstInten.SetUnitType(CUnitCtrl::m_HPRS_UNIT.dConstantIntensity);
	m_wndGradInten.SetUnitType(CUnitCtrl::m_HPRS_UNIT.dGradientIntensity);

	m_wndRefLevelUnit.SetUnitType(CUnitCtrl::m_HPRS_UNIT.dReferenceLevel);
	m_wndConstIntenUnit.SetUnitType(CUnitCtrl::m_HPRS_UNIT.dConstantIntensity);
	m_wndGradIntenUnit.SetUnitType(CUnitCtrl::m_HPRS_UNIT.dGradientIntensity);

	m_wndCurvedIntenUnit.SetUnitType(CUnitCtrl::m_HPRS_UNIT.dConstantIntensity);
	
	// Set Default Values
	//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, 0);  // Default : Add

	int nElemType = 0;
	m_wndElemType.SetCurSel(nElemType); // default : plate
	if(nElemType > 0) nElemType ++;  // Plane Stress 빼기 .^^

	// Load Type에 의한 Control들을 동일한 위치에 이동시킴
	CRect rRef;
	CRect rMove;
	GetDlgItem(IDC_TM_HPRS_STATIC1)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_CURVE_MTD_RDO)->GetWindowRect(rMove);
	int nDispY = rRef.top - rMove.top + globalUtils.ScaleByDPI(2);
	CDlgUtil::CtrlMoveDistXY(this, m_arCurvedType, 0, nDispY);
	ChangeLoadTypeMan();
	
	// Enable/Disable/Show/Hide/Align controls
	ChangeBitmap(nElemType);
	SetDirection(nElemType);
	m_wndDirCobx.SetCurSel(2);  // default : local (+z)

	SelectionMethodMan();

	//UpdateData(FALSE);

	CheckMode(TRUE);
	 // Added by BugBoy   Hydrostatic Pressure Load 의 Edge나 Face를 나타내기 위해서.
	 // 2000.8.25
	DistPrjCtrl(); // Pressure Load 와 마찬가지로 Project Ctrl들을 이동시키기 위한 거리계산.
	m_bUpDown = FALSE;
	ChangePrjCtrlPosition(TRUE);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_EXECUTE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(12);
	MoveWindow(r);

	OnTmCurveMtdRdo();

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldHprsDlg::ChangeLoadTypeMan()
{
	int nIndex = m_wndLoadType.GetCurSel();

	BOOL bShow = (nIndex==0)? TRUE:FALSE;
	CDlgUtil::CtrlShowHide(this, m_arLinearType, bShow);
	CDlgUtil::CtrlShowHide(this, m_arCurvedType, !bShow);

#if 0
	UINT aBitmapID[] = { 
		IDB_TM_HPRS0, IDB_TM_HPRS6,
	};

	if (m_pLoadTypeBitmap != 0) delete m_pLoadTypeBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_LOADTYPE_IMG);
	m_pLoadTypeBitmap = new CBitmap;
	m_pLoadTypeBitmap->LoadBitmap(aBitmapID[nIndex]);
	pImage->SetBitmap(HBITMAP(*m_pLoadTypeBitmap));
#else
	//IDB_TM_HPRS0            BITMAP                  "res\\tm_hprs0.bmp"
	//IDB_TM_HPRS6            BITMAP                  "res\\tm_hprs6.bmp"
	CString aLoadTypePicture[] = 
	{
		_T("SVG\\illustration\\Dialog\\tm_hprs0.svg"),
		_T("SVG\\illustration\\Dialog\\tm_hprs6.svg")
	};
	m_wndLoadTypePicture.SetImage(aLoadTypePicture[nIndex]);
#endif
}

void CStldHprsDlg::OnTmExecute() 
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	CArray<int, int> arIFaceOrEdge;
	int nRealElemType = this->GetElemTypeExternal();
	int nElemType = m_wndElemType.GetCurSel();
	if(nElemType > 0) nElemType ++;

	// 선택된 Element를 Filtering 한다.  
	// Select By Node Mode가 추가됨에 따라. Node에 의한 Select 대상 Object설정
	// 요구...
	GetElemByType(m_nOption, nRealElemType, aSelKey,arIFaceOrEdge);  //Face Or Edge 
	// By Node 와 By Element구분에 의한 후보 Element 가 설정 된 상태 .. aSelKey
	if (aSelKey.GetSize() == 0) 
	{
		CString csElemTypeName;
		if (m_nOption == 2) // Delete
			csElemTypeName = _LS(IDS_WG_TREEMENU_pressure);
		else
			m_wndElemType.GetWindowText(csElemTypeName);
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected__s_element_), csElemTypeName);
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_HPRS_D data;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;

	if (nElemType == 0) data.LoadType = 1;
	else if (nElemType == 1 || nElemType == 2 || nElemType == 3) data.LoadType = 2;
	else data.LoadType = 3;

	// Edge Face (540에서 Pressure 변경되면서 같이 변경:Plate Face 인 경우 Face=1)
	if (data.LoadType == 1) data.nEdgeFace = 1;
	else
		data.nEdgeFace = m_wndDirCobx.GetCurSel() + 1;
	
	// Curved Type
	data.nLoadType = m_wndLoadType.GetCurSel();
	if (data.nLoadType == 1)
	{
		data.nCurveType = m_nCurveType;
		data.dFactor_a = m_dFactorA;
		data.dFactor_b = m_dFactorB;
		data.dFactor_c = m_dFactorC;
	}

	// Direction
	int nDir=0;
	if(data.LoadType == 1)
		nDir = m_wndDirCobx.GetCurSel()+1;  // LX, LY, LZ, GX, GY, GZ
	else if(data.LoadType == 2) nDir = 0; // Normal
	else if(data.LoadType == 3)
	{
		nDir = m_cboDirection.GetCurSel()+1;
		// 540에서 Pressure 변경되면서 같이 변경
		// Solid의 경우 Normal:1->0, GX:2->4, GY:3->5, GZ:4-6
		if (nDir == 1) nDir = 0;
		else nDir += 2;
	}

	data.LoadDirectionNew = nDir;

	// Projection
	if(data.LoadType != 2)
		data.Projection = ((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->GetCheck(); 

	data.nGradientDirection = m_wndGradDir.GetCurSel()+1;
	// *^^* 여기 수정
	//data.dReferenceLevel = m_wndRefLevel.GetEditValue();
	if (!GetReferenceLevel(data.dReferenceLevel))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_reference_level_));
		return;
	}
	data.dConstantIntensity = m_wndConstInten.GetEditValue();
	data.dGradientIntensity = m_wndGradInten.GetEditValue();

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	if(nRealElemType >= 0 && nRealElemType <= 8) // By Element
	{
		BOOL bSuccess=FALSE;
		if (m_nOption == 0 || m_nOption == 1) // Add or Replace
		{
			
			data.GroupKey = LdgrK;
			
			bSuccess = m_pDoc->m_pDataCtrl->AddHprs(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
		}
		else if (m_nOption == 2)  // delete
			bSuccess = m_pDoc->m_pDataCtrl->DelHprs(aSelKey, nLoadCaseKey,LdgrK);	

		if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else // By Node
	{
		
		BOOL bSuccess = TRUE;
		
		if (m_nOption == 0 || m_nOption == 1) // Add or Replace
		{
			int nElem  = aSelKey.GetSize();
			int nFE    = arIFaceOrEdge.GetSize();
			ASSERT( nElem == nFE );
			CArray<T_HPRS_D,T_HPRS_D> Datas;
		 
			data.GroupKey = LdgrK;
			
			for( int i = 0 ; i < nElem ; i++)
			{
				data.nEdgeFace = arIFaceOrEdge[i];
				Datas.Add(data);
			}
			m_pDoc->m_pDataCtrl->AddHprs(aSelKey,Datas,(m_nOption==0)?FALSE:TRUE);

			/*
			for( int i = 0 ; i < nElem ; i++)
			{
				data.nEdgeFace = arIFaceOrEdge[i];
				bTemp = m_pDoc->m_pDataCtrl->AddHprs(aSelKey[i],data,(m_nOption==0)?FALSE:TRUE);
				bSuccess &= bTemp;
			}
			*/
		}
		else if (m_nOption == 2)  // delete
			bSuccess = m_pDoc->m_pDataCtrl->DelHprs(aSelKey, nLoadCaseKey,LdgrK);	

		if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}




/*

	BOOL bSuccess;
	if (nOption == 0 || nOption == 1) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddHprs(aSelKey, data, (nOption==0)?FALSE:TRUE);	
	else if (nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelHprs(aSelKey, nLoadCaseKey);	

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);

*/

	/*









	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	int nElemType = m_wndElemType.GetCurSel();

	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);
	GetElemByType(nOption, nElemType, aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		CString csElemTypeName;
		if (nOption == 2) // Delete
			csElemTypeName = _LS(IDS_WG_TREEMENU_pressure);
		else
			m_wndElemType.GetWindowText(csElemTypeName);
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected__s_element_), csElemTypeName);
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_HPRS_D data;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;

	if (nElemType == 0) data.LoadType = 1;
	else if (nElemType == 1 || nElemType == 2 || nElemType == 3) data.LoadType = 2;
	else data.LoadType = 3;

	// Edge Face
	if(data.LoadType != 1)
		data.nEdgeFace = m_wndDirCobx.GetCurSel() + 1;
	
	// Direction
	int nDir=0;
	if(data.LoadType == 1)
		nDir = m_wndDirCobx.GetCurSel()+1;
	else if(data.LoadType == 3)
		nDir = m_cboDirection.GetCurSel()+1;

	if(nDir)
		data.LoadDirection = nDir;

	// Projection
	if(data.LoadType != 2)
		data.Projection = ((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->GetCheck(); 

	data.nGradientDirection = m_wndGradDir.GetCurSel()+1;
	// *^^* 여기 수정
	//data.dReferenceLevel = m_wndRefLevel.GetEditValue();
	if (!GetReferenceLevel(data.dReferenceLevel))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_reference_level_));
		return;
	}
	data.dConstantIntensity = m_wndConstInten.GetEditValue();
	data.dGradientIntensity = m_wndGradInten.GetEditValue();

	BOOL bSuccess;
	if (nOption == 0 || nOption == 1) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddHprs(aSelKey, data, (nOption==0)?FALSE:TRUE);	
	else if (nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelHprs(aSelKey, nLoadCaseKey);	

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	*/
}

void CStldHprsDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldHprsDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}


void CStldHprsDlg::OnChangeElemType() 
{
	// TODO: Add your control notification handler code here
	int nElemType = m_wndElemType.GetCurSel();
	if(nElemType > 0) nElemType ++;

	ChangeBitmap(nElemType);
	SetDirection(nElemType);



	int nDir;

	if(m_wndElemType.GetCurSel() == 0) nDir=2;  // Plate인 경우 Local z
	else nDir=0;
	m_wndDirCobx.SetCurSel(nDir);

	CheckMode(TRUE);
	 // Added by BugBoy   Hydrostatic Pressure Load 의 Edge나 Face를 나타내기 위해서.
	 // 2000.8.25
	ModifyProjectCtrl(nElemType);
}

void CStldHprsDlg::ModifyProjectCtrl(int nIndex)
{
	switch(nIndex)
	{
		case 0:   // plate			
			CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, FALSE);
			ChangePrjCtrlPosition(TRUE);
			if(m_wndDirCobx.GetCurSel() > 2)
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, TRUE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			else
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			break;
		case 1:   // plane stress
		case 2:   // plane strain
		case 3:   // Axisymmetric
			ChangePrjCtrlPosition(TRUE);
			CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);
			((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			break;
		case 4:   // solid 8
		case 5:   // solid 6
		case 6:   // solid 4
			CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, TRUE);
			ChangePrjCtrlPosition(FALSE);
			if(m_cboDirection.GetCurSel() > 0)
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, TRUE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			else
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			break;
	}
}

void CStldHprsDlg::ChangePrjCtrlPosition(BOOL bUpDown)  //bUpDown=1 위로, bUpDown=0 아래로.
{
	if(bUpDown && !m_bUpDown)
	{
		CDlgUtil::CtrlMoveDistY(this, m_arCtrlProject, -m_nDistPrjCtrl);
		m_bUpDown = TRUE;
	}
	else if(!bUpDown && m_bUpDown)
	{
		CDlgUtil::CtrlMoveDistY(this, m_arCtrlProject, m_nDistPrjCtrl);
		m_bUpDown = FALSE;
	}
}

void CStldHprsDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nCheck);

	if(nCheck == 2) //Delete 
	{
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else
	{
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}

	BOOL bEnable = (nCheck != 2) ? TRUE : FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, bEnable);

	UpdateData(TRUE);
	if (m_nSelectionMethod == 0)
		GetDlgItem(IDC_TM_LOADED_EDGE)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_TM_LOADED_EDGE)->EnableWindow(TRUE);
}

void CStldHprsDlg::OnChangeGradDir() 
{
	// TODO: Add your control notification handler code here
	int nDir = m_wndGradDir.GetCurSel();
	if (nDir == 0) m_wndRefLevel.SetModeToUse(MOUSEEDIT_USE_POS_X_ONLY);
	else if (nDir == 1) m_wndRefLevel.SetModeToUse(MOUSEEDIT_USE_POS_Y_ONLY);
	else m_wndRefLevel.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
}

void CStldHprsDlg::PostNcDestroy() 
{
 	CheckMode(FALSE);
	// Added By BugBoy For Display Edge or Face..
	// 2000.8.25
	
	CMenuBarChildDlg::PostNcDestroy();
}

void CStldHprsDlg::OnSelchangeTmLoadedEdge() 
{
	CheckMode(TRUE);
	 // Added by BugBoy   Hydrostatic Pressure Load 의 Edge나 Face를 나타내기 위해서.
	 // 2000.8.25
	int nElemType = m_wndElemType.GetCurSel();
	if(nElemType > 0)  nElemType ++;

	ModifyProjectCtrl(nElemType);
}

void CStldHprsDlg::CheckMode(BOOL bCheck)
{
	m_pDoc = CDBDoc::GetDocPoint();
	if(m_pDoc)
	{
		if(bCheck)
		{
			int nElemType = GetElemTypeExternal();
			m_pDoc->m_pViewCtrl->SetPressureInputMode(TRUE);
			m_pDoc->m_pViewCtrl->SetPressureInputParam(nElemType, m_wndDirCobx.GetCurSel());	
			
		}
		else
		{
			m_pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);  
			m_pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
		}
	}
}

void CStldHprsDlg::OnSelchangeTmDirCombo2() 
{
	int nElemType = m_wndElemType.GetCurSel();
	if(nElemType > 0) nElemType++;

	ModifyProjectCtrl(nElemType);
}




// 변경된 ElemType과 변경 되지 않은 ElemType을 모두 참조 해서.. Control 관리 할것..
int CStldHprsDlg::GetPlateSolidOptionMan()
{
	int nElemType = m_wndElemType.GetCurSel();  
	if(nElemType > 0) nElemType ++;







	//Plate Edge Select는 없다 


	
	
	

	//Plate Face Select : 0  Edge Select : 7
	//Solid All Select  : 8

	if(nElemType == 0)
	{
		
		/*
		CtrlShowHide(this->m_arCtrlPlateOpt, TRUE);
		CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
		//////////////////////////////////////////
		//Solid Option을 비활성화 ,  Plate Option을 활성화 시킨다.
		//Plate Option을 참조 nElemType을 설정..
		if(m_nPlateOption == 1)
			nElemType = 7;
		*/
	}
	else if( nElemType == 4)
	{
		/*
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
		*/
		nElemType = 8;
	}
	else
	{
		//CtrlShowHide(this->m_arCtrlPlateOpt, FALSE);
		//CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
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
	Ctrls.Add(IDC_TM_LOADED_EDGE_TITLE);
	Ctrls.Add(IDC_TM_LOADED_EDGE);
	
	if((nElemType == 8          && m_nSelectionMethod != 1) || 
		 (m_nSelectionMethod != 1 && nElemType <= 6 && nElemType >= 0) ||
		 (nElemType == 7 && m_nSelectionMethod != 1))
	{
		this->CtrlEnableDisable(Ctrls,FALSE);  
	}
	else 
	{
		this->CtrlEnableDisable(Ctrls,TRUE);   
	}
	
	return nElemType;
}

int CStldHprsDlg::GetElemTypeExternal()
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

	int nElemType = GetPlateSolidOptionMan();

	if(m_nSelectionMethod == 0 && nElemType == 7) nElemType = 9;
	if(m_nSelectionMethod == 0 && nElemType == 8) nElemType = 10;
	
	if(m_nSelectionMethod == 0 && nElemType == 1) nElemType = 11;  // stress Edge By Node
	if(m_nSelectionMethod == 0 && nElemType == 2) nElemType = 12;  // strain Edge By Node
	if(m_nSelectionMethod == 0 && nElemType == 3) nElemType = 13;  // axisymmetric Edge By Node
	
	if(m_nSelectionMethod == 0 && nElemType == 4) nElemType = 14;// 8 Node Solid Select By Node
	if(m_nSelectionMethod == 0 && nElemType == 5) nElemType = 15;// 6 Node Solid Select By Node
	if(m_nSelectionMethod == 0 && nElemType == 6) nElemType = 16;// 4 Node Solid Select By Node


	return nElemType;
}

void CStldHprsDlg::OnTmSelectionMethod() 
{
	UpdateData(TRUE);
	this->OnChangeElemType();
}

void CStldHprsDlg::SelectionMethodMan()
{
	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_TM_LOADED_EDGE_TITLE);
	Ctrls.Add(IDC_TM_LOADED_EDGE);
	if(m_nSelectionMethod == 1)
	{
		this->CtrlEnableDisable(Ctrls,TRUE);  
	}
	else
	{
		this->CtrlEnableDisable(Ctrls,FALSE);  
	}
}



void CStldHprsDlg::GetElemByType(int nOption, int nElemType, CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	
	if(nOption != 2) // Add Or Modify 
	{
		if(nElemType <= 8) //By Element 
		{
			pIGM->GetSelectedElemKeyList(aSelKey);
		}
		else
		{
			CArray<T_ELEM_K,T_ELEM_K> aElemList; // Active된 element list
			CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
			
			pIGM->GetSelectedNodeKeyList(aSelNodeKey);
			pIGM->GetActiveElemKeys(aElemList);
			
			if(nElemType == 10 || nElemType == 14 || nElemType == 15 || nElemType == 16)
			{
				 CDBDoc::GetDocPoint()->SelectFaceByNode(aElemList, 
																								 aSelNodeKey, 
																								 aSelKey,
																								 arFaceOrEdge);
			}else if( nElemType == 9 || nElemType == 11 || nElemType == 12 || nElemType == 13)
			{
				CDBDoc::GetDocPoint()->SelectEdgeByNode(aElemList, 
																								 aSelNodeKey, 
																								 aSelKey,
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
	int nNodeNum;
	
	if (nOption == 2) // delete
	{
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (ElemData.eltyp == PLATE_EL || 
					ElemData.eltyp == PLSTRS_EL ||
					ElemData.eltyp == PLSTRN_EL ||
					ElemData.eltyp == AXISYM_EL ||
					ElemData.eltyp == SOLID_EL) continue;
			aSelKey.RemoveAt(i);
		}
		return;
	}

	switch(nElemType)
	{
	case 0: // Plate Face By Element
	case 7: // Plate Edge By Element 
	case 9: // Plate Edge By Node
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 9)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 11: // Plane Stress Edge By Node
	case 1:  // Plane Stress Edge By Element 
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlstrs(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 11)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 12: // Plane Strain Edge By Node
	case 2:  // Plane Strain Edge By Element 
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlstrn(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 12)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 13: // AxiSymmetric Edge By Node
	case 3:  // AxiSymmetric Edge By Element 
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsAxisym(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 13)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 4:  // 8 Solid Face By Element 
	case 5:  // 6 Solid Face By Element 
	case 6:  // 4 Solid Face By Element 
	case 8:  // Solid   Face By Element
	case 10: // Solid   Face By Node 
	case 14: // 8 Solid Face By Node
	case 15: // 6 Solid Face By Node
	case 16: // 4 Solid Face By Node
		if      (nElemType == 4 || nElemType == 14) nNodeNum = 8;
		else if (nElemType == 5 || nElemType == 15) nNodeNum = 6;
		else if (nElemType == 6 || nElemType == 16) nNodeNum = 4;
		else    nNodeNum = 100; // All Solid 

		if( nElemType == 4 || nElemType == 5 || nElemType ==6 || nElemType == 8)// by Element
		{
			for (i = aSelKey.GetSize()-1; i >= 0; i--)
			{
				m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
				if (!m_pDoc->m_pAttrCtrl->IsSolid(ElemData.eltyp))aSelKey.RemoveAt(i);
				else 
				{
					if( nNodeNum <= 8 && nNodeNum >= 4)
					{
						if (m_pDoc->m_pAttrCtrl->GetNumNode(ElemData.eltyp, ElemData.elnod) != nNodeNum)
								aSelKey     .RemoveAt(i);
					}
				}
			}
		}
		else if( nElemType == 10 || nElemType == 14|| nElemType == 15|| nElemType ==16 ) // by Node
		{
			for (i = aSelKey.GetSize()-1; i >= 0; i--)
			{
				m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
				if( nNodeNum <= 8 && nNodeNum >= 4)
				{
					if (m_pDoc->m_pAttrCtrl->GetNumNode(ElemData.eltyp, ElemData.elnod) != nNodeNum)
					{
						aSelKey     .RemoveAt(i);
						arFaceOrEdge.RemoveAt(i);
					}
				}
			}
		}
		break;
	}
}

void CStldHprsDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

void CStldHprsDlg::OnSelchangeTmLoadType()
{
	ChangeLoadTypeMan();
}

void CStldHprsDlg::OnTmCurveMtdRdo() 
{
	UpdateData();
	GetDlgItem(IDC_TM_C_EDIT)->EnableWindow(m_nCurveType == 1 || m_nCurveType == 2);
}

BOOL CStldHprsDlg::GetReferenceLevel(double& rdRefLevel)
{
	rdRefLevel = 0.0;

	double dPosX = 0.0;  double dPosY = 0.0;  double dPosZ = 0.0;
	if (!m_wndRefLevel.GetCoordData(&dPosX, &dPosY, &dPosZ))  { ASSERT(0); return FALSE; }

	int nMouseEditMode = m_wndRefLevel.GetModeToUse();
			 if (nMouseEditMode == MOUSEEDIT_USE_POS_X_ONLY)   rdRefLevel = dPosX;
	else if (nMouseEditMode == MOUSEEDIT_USE_POS_Y_ONLY)   rdRefLevel = dPosY;
	else if (nMouseEditMode == MOUSEEDIT_USE_POS_Z_ONLY)   rdRefLevel = dPosZ;
	else
	{
		ASSERT(0);
		// MNET:3064-HSSHIM-20091227
		// 아래 코드로 들어오는 경우 소숫점 5자리 이하 자리가 반올림 되어 문제가 발생할 수 있음.
		return CFormulaEdit::GetEditValue(&m_wndRefLevel, rdRefLevel);
	}

	return TRUE;
}