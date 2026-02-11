// SectForResultantForceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SectForResultantForceDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_ControlEx.h"   // for CFormulaEdit
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\I_GENStateDefine.h"    // for IG_STATE_ELEM_ADD, IG_STATE_INITALL

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
//#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\UserDefType.h"   // for T_POINT3D
#include "..\wg_db\LdfsUtil.h"      // for T_POINT3D
#include "..\wg_db\GeomFunc.h"      // for CGeomFunc
#include "..\wg_db\wg_db_SectUtil.h"
#include "SectRForcGrid.h"
#include "SectRVbemGrid.h"
#include "StructElementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_RFORC_MODE_STRUCT_GROUP     0   // Structure Group
#define D_RFORC_MODE_POLYGON_SELECT   1   // Polygon Select
#define D_RFORC_MODE_ELEMENTS_SELECT  2   // Elements Select

/////////////////////////////////////////////////////////////////////////////
// CSectForResultantForceDlg dialog


CSectForResultantForceDlg::CSectForResultantForceDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSectForResultantForceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSectForResultantForceDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_pGridLoc = NULL;
	m_pGridVBeam = NULL;

	m_dTolerance = 0.01;
	m_bIsComplete = TRUE;

	m_arXYZ[0].RemoveAll(); 
	m_arXYZ[1].RemoveAll(); 
	m_arXYZ[2].RemoveAll(); 

	m_dZVec[0]=m_dZVec[1]=m_dZVec[2]=0.0;

	m_aCtrlCoord.RemoveAll();
	m_aCtrlCoord.Add(IDC_TM_SECT_RFOC_POS_STC     );
	m_aCtrlCoord.Add(IDC_TM_SECT_RFOC_POS_EDT     );
	m_aCtrlCoord.Add(IDC_TM_SECT_RFOC_POS_LIST_EDT);
	m_aCtrlCoord.FreeExtra();

	m_aCtrlTol.RemoveAll();
	m_aCtrlTol.Add(IDC_TM_SECT_RFOC_TOL_STC);
	m_aCtrlTol.Add(IDC_TM_SECT_RFOC_TOL_EDT);
	m_aCtrlTol.Add(IDC_TM_SECT_RFOC_TOL_UNT);
	m_aCtrlTol.FreeExtra();

	m_aCtrlDiv.RemoveAll();
	m_aCtrlDiv.Add(IDC_TM_SECT_RFOC_DIV_STC);
	m_aCtrlDiv.Add(IDC_TM_SECT_RFOC_DIV_EDT);
	m_aCtrlDiv.FreeExtra();

	m_aCtrlName.RemoveAll();
	m_aCtrlName.Add(IDC_TM_SECT_RFOC_NAME_STC);
	m_aCtrlName.Add(IDC_TM_SECT_RFOC_NAME_EDT);
	m_aCtrlName.FreeExtra();

	m_aCtrlGrup.RemoveAll();
	m_aCtrlGrup.Add(IDC_TM_SECT_RFOC_GRUP_STC);
	m_aCtrlGrup.Add(IDC_TM_SECT_RFOC_GRUP_CMB);
	m_aCtrlGrup.FreeExtra();
}


void CSectForResultantForceDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectForResultantForceDlg)
	DDX_Control(pDX, IDC_TM_SECT_RFOC_MODE_CMB, m_cmbMode);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_TOL_EDT, m_edtTolerance);	
	DDX_Control(pDX, IDC_TM_SECT_RFOC_TOL_UNT, m_untTolerance);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_DIV_EDT, m_edtDiv);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_VSEC_GRP, m_grpCoord);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_POS_EDT, m_edtInputCoord);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_POS_LIST_EDT, m_edtInput);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_ZVEC_CHK, m_chkZVec);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_ZVEC_EDT, m_edtZVec);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_NAME_EDT, m_edtName);	 
	DDX_Control(pDX, IDC_TM_SECT_RFOC_GRUP_CMB, m_wndGrupCobx);  
	
	DDX_Control(pDX, IDC_TM_SECT_RFOC_LOC_GRD, *m_pGridLoc);
	DDX_Control(pDX, IDC_TM_SECT_RFOC_VBEAM_GRD, *m_pGridVBeam);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSectForResultantForceDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSectForResultantForceDlg)
	ON_CBN_SELCHANGE(IDC_TM_SECT_RFOC_MODE_CMB,  OnSelchangeCmbMode)
	ON_BN_CLICKED(IDC_TM_SECT_RFOC_ADD_BTN, OnTmBtnAdd)
	ON_BN_CLICKED(IDC_TM_SECT_RFOC_MOD_BTN, OnTmBtnMod)
	ON_BN_CLICKED(IDC_TM_SECT_RFOC_DEL_BTN, OnTmBtnDel)
	ON_BN_CLICKED(IDC_TM_SECT_RFOC_ZVEC_CHK, OnTmChkZVector)
	ON_CBN_SELCHANGE(IDC_TM_SECT_RFOC_GRUP_CMB, OnSelchangeTmGrupCmb)
	ON_BN_CLICKED(IDC_TM_SECT_RFOC_CONVERT_BTN, ConvertToVsec)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE, OnMouseEditEscape)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_CANCEL_FROM_MVIEW, OnMViewCancel)
	ON_REGISTERED_MESSAGE(CSectRForcGrid::WM_SECT_RFORC_GRID_VSEC_CLICKED, OnVsecGridClicked)
	ON_REGISTERED_MESSAGE(CSectRVbemGrid::WM_SECT_RFORC_GRID_VBEM_CLICKED, OnVbemGridClicked)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectForResultantForceDlg message handlers

BOOL CSectForResultantForceDlg::OnInitDialog() 
{
	m_pGridLoc = new CSectRForcGrid(this);
	m_pGridVBeam = new CSectRVbemGrid(this);

	CMenuBarChildDlg::OnInitDialog();

	AlignControls();
	InitControls();
	
	EnableDisableCtrls();
	ShowHideCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSectForResultantForceDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;

	// No. of Division
	GetDlgItem(IDC_TM_SECT_RFOC_TOL_STC)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlDiv[0])->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top  - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlDiv, nDistX, nDistY);

	// Structure Group
	GetDlgItem(IDC_TM_SECT_RFOC_NAME_STC)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlGrup[0])->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top  - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlGrup, nDistX, nDistY);
}

void CSectForResultantForceDlg::ShowHideCtrls()
{
	int nMode = GetMode();

	// No. of Division
	BOOL bUseDiv = (nMode == D_RFORC_MODE_POLYGON_SELECT)? FALSE : TRUE;
	CDlgUtil::CtrlShowHide(this, m_aCtrlTol, !bUseDiv);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDiv,  bUseDiv);

	// Structure Group
	BOOL bUseGrup = (nMode == D_RFORC_MODE_STRUCT_GROUP)? TRUE : FALSE;
	CDlgUtil::CtrlShowHide(this, m_aCtrlName, !bUseGrup);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGrup,  bUseGrup);
}

void CSectForResultantForceDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	 	//InitTsgrData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		//InitTsgrData();
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

void CSectForResultantForceDlg::OnDestroy() 
{
	m_grpCoord.SetFoldState(FALSE);
	CMenuBarChildDlg::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CSectForResultantForceDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	return TRUE;
}

int CSectForResultantForceDlg::GetMode()
{
	DWORD dwMode = D_RFORC_MODE_ELEMENTS_SELECT;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbMode, dwMode);

	return dwMode;
}

void CSectForResultantForceDlg::OnSelchangeCmbMode() 
{
	ShowHideCtrls();
	EnableDisableCtrls();

	int nMode = GetMode();
	BOOL bFold = (nMode == D_RFORC_MODE_POLYGON_SELECT)? FALSE : TRUE;
	m_grpCoord.SetFoldState(bFold);
}

void CSectForResultantForceDlg::EnableDisableCtrls()
{
	int nMode = GetMode();

	// Coord. Group Box
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCoord, nMode==D_RFORC_MODE_POLYGON_SELECT);

	// Direction Vector
	BOOL bEnableZVec = (m_chkZVec.GetCheck() != 0)? TRUE : FALSE;
	m_edtZVec.EnableWindow(bEnableZVec);

}

void CSectForResultantForceDlg::OnTmChkZVector()
{
	BOOL bEnableZVec = (m_chkZVec.GetCheck() != 0)? TRUE : FALSE;
	if (bEnableZVec)  m_edtZVec.SetFocus();

	EnableDisableCtrls();
}

// dZVec : [out] Direction Vector
// arXYZ : [in]  Polygon
BOOL CSectForResultantForceDlg::GetDirVectorFromPolygon(double dZVec[3], CArray<double,double> arXYZ[3])
{
	int nCountCoods = m_arXYZ[0].GetSize();
	if (nCountCoods < 3) { ASSERT(0);  return FALSE; }

	int iPosCur = 0;

	BOOL bGetPt1 = FALSE;
	double pt0[3] = {0.0, };
	pt0[0] = arXYZ[0][iPosCur];  pt0[1] = arXYZ[1][iPosCur];  pt0[2] = arXYZ[2][iPosCur];
	iPosCur++;

	double pt1[3] = {0.0, };
	for (int i=iPosCur; i<nCountCoods; ++i)
	{
		pt1[0] = arXYZ[0][iPosCur];  pt1[1] = arXYZ[1][iPosCur];  pt1[2] = arXYZ[2][iPosCur];
		iPosCur++;

		// 동일점 검사
		if (CMathFunc::mathLength(pt0[0], pt0[1], pt0[2],  pt1[0], pt1[1], pt1[2]) > m_dTolerance)  { bGetPt1 = TRUE;  break; }
	}
	if (!bGetPt1)  { ASSERT(0);  return FALSE; }

	double dVec01[3] = {0.0, };
	dVec01[0] = pt1[0] - pt0[0];
	dVec01[1] = pt1[1] - pt0[1];
	dVec01[2] = pt1[2] - pt0[2];
	if(!CMathFunc::mathNormalize(dVec01, dVec01)) { ASSERT(0);  return FALSE; }
	
	double dVec12[3] = {0.0, };
	double pt2[3] = {0.0, };
	BOOL bGetPt2 = FALSE;
	for (int i=iPosCur; i<nCountCoods; ++i)
	{
		pt2[0] = arXYZ[0][iPosCur];  pt2[1] = arXYZ[1][iPosCur];  pt2[2] = arXYZ[2][iPosCur];
		iPosCur++;

		// 동일점 검사
		if (CMathFunc::mathLength(pt1[0], pt1[1], pt1[2],  pt2[0], pt2[1], pt2[2]) < m_dTolerance)  continue;    

		// 직선 연결 검사
		dVec12[0] = pt2[0] - pt1[0];
		dVec12[1] = pt2[1] - pt1[1];
		dVec12[2] = pt2[2] - pt1[2];
		if(!CMathFunc::mathNormalize(dVec12, dVec12)) { ASSERT(0);  return FALSE; }

		double dAng = CMathFunc::mathCrossAngle(dVec01, dVec12);
		if (fabs(dAng      ) < 1.0) continue;
		if (fabs(dAng-180.0) < 1.0) continue;

		bGetPt2 = TRUE;
		break;
	}
	if (!bGetPt2)  { ASSERT(0);  return FALSE; }

	CMathFunc::mathNormal(dVec01, dVec12, dZVec);
	if(!CMathFunc::mathNormalize(dZVec, dZVec)) { ASSERT(0);  return FALSE; }

	return TRUE;
}

// 제일 바깥쪽 경계를 이루는 두점을 반환
BOOL CSectForResultantForceDlg::GetCutPoints2DfromPolygon(T_POINT2D& pt2D_CutP1, T_POINT2D& pt2D_CutP2, CArray<double,double> arXYZ[3])
{
	int nCountCoods = m_arXYZ[0].GetSize();
	if (nCountCoods < 2) { ASSERT(0);  return FALSE; }

	int iPosCur = 0;
	double dMinX = arXYZ[0][iPosCur];    int iPosMinX = iPosCur;     
	double dMaxX = arXYZ[0][iPosCur];    int iPosMaxX = iPosCur;     
	double dMinY = arXYZ[1][iPosCur];    int iPosMinY = iPosCur;     
	double dMaxY = arXYZ[1][iPosCur];    int iPosMaxY = iPosCur;     
	iPosCur++;

	for (int i=iPosCur; i<nCountCoods; ++i)
	{
		if (dMinX > arXYZ[0][iPosCur]) { dMinX = arXYZ[0][iPosCur];  iPosMinX = iPosCur; }
		if (dMaxX < arXYZ[0][iPosCur]) { dMaxX = arXYZ[0][iPosCur];  iPosMaxX = iPosCur; }
		if (dMinY > arXYZ[1][iPosCur]) { dMinY = arXYZ[1][iPosCur];  iPosMinY = iPosCur; }
		if (dMaxY < arXYZ[1][iPosCur]) { dMaxY = arXYZ[1][iPosCur];  iPosMaxY = iPosCur; }
		iPosCur++;
	}

	int iPos1 = iPosMinY;   // 시작점
	int iPos2 = iPosMaxY;   // 종료점
	if (iPos1 == iPos2)
	{
		iPos1 = iPosMinX;   // 시작점
		iPos2 = iPosMaxX;   // 종료점
	}

	pt2D_CutP1.x = arXYZ[0][iPos1];
	pt2D_CutP1.y = arXYZ[1][iPos1];

	pt2D_CutP2.x = arXYZ[0][iPos2];
	pt2D_CutP2.y = arXYZ[1][iPos2];

	return TRUE;
}

void CSectForResultantForceDlg::Dlg2Data()
{
	if(m_chkZVec.GetCheck() != 0) // use user direction
	{
		double dZVec[3] = {0.0, };
		GetVecFromMouseEdit(m_edtZVec, dZVec);

		// set data
		m_dZVec[0]=dZVec[0];
		m_dZVec[1]=dZVec[1];
		m_dZVec[2]=dZVec[2];
	}
}

BOOL CSectForResultantForceDlg::GetDataVsec_fromPolygon(T_VSEC_D& VsecD, int iDir, LPCTSTR lpszName)
{
	VsecD.Initialize();
	if (m_arXYZ[0].GetSize() == 0)  { ASSERT(0);  return FALSE; }

	CString strName = lpszName;
	if (strName.IsEmpty())  { ASSERT(0);  return FALSE; }

	CString strPostFix;
	strPostFix.Format(_T("_%s"), (iDir == 0)? _T("I") : _T("J"));
	strName += strPostFix;

	// Get Selected Elem list
	CArray<T_ELEM_K,T_ELEM_K> aElemK_cutting;   aElemK_cutting.RemoveAll();
	int nElem = GetCuttingElemList(aElemK_cutting);
	if (nElem == 0)   { ASSERT(0);  return FALSE; }   // 

	double dZVec[3] = {0.0, };
	if(m_chkZVec.GetCheck() != 0) // use user direction
	{
		dZVec[0] = m_dZVec[0];
		dZVec[1] = m_dZVec[1];
		dZVec[2] = m_dZVec[2];
	}
	else  // auto direction
	{
		if (!GetDirVectorFromPolygon(dZVec, m_arXYZ))  { ASSERT(0);  return FALSE; }
	}

	T_POINT3D pt3DOnCuttingPlane;    // Polygon 상의 한 점.
	pt3DOnCuttingPlane.x = m_arXYZ[0][0];
	pt3DOnCuttingPlane.y = m_arXYZ[1][0];
	pt3DOnCuttingPlane.z = m_arXYZ[2][0];

	// get start/end position
	T_NODE_D NodeD_start;
	NodeD_start.x = pt3DOnCuttingPlane.x;
	NodeD_start.y = pt3DOnCuttingPlane.y;
	NodeD_start.z = pt3DOnCuttingPlane.z;
	T_NODE_D NodeD_end;
	NodeD_end.x = NodeD_start.x + dZVec[0];
	NodeD_end.y = NodeD_start.y + dZVec[1];
	NodeD_end.z = NodeD_start.z + dZVec[2];

	// get cutting line
	T_POINT2D pt2D_CutP1;
	T_POINT2D pt2D_CutP2;

	if (!GetCutPoints2DfromPolygon(pt2D_CutP1, pt2D_CutP2, m_arXYZ))  { ASSERT(0);  return FALSE; }
	// cutting direction : dZVec 방향의 요소가 선택될 수 있도록 처리하기 위함
	double dCuttingDir[2] = {0.0, };
	dCuttingDir[0] = pt2D_CutP2.x-pt2D_CutP1.x;
	dCuttingDir[1] = pt2D_CutP2.y-pt2D_CutP1.y;
	if (!CMathFunc::mathNormalize2D(dCuttingDir, dCuttingDir))  { ASSERT(0);  return FALSE; }

	// ccw 검사 (=-1:cw, =0:수평, =1:ccw)
	int iCCW = CMathFunc::math_ccw(0.0, 0.0,  dCuttingDir[0], dCuttingDir[1],  dZVec[0], dZVec[1]);
	if (iCCW != -1)  // cutting 방향을 뒤집어준다.
	{
		T_POINT2D pt2D_temp = pt2D_CutP1;
		pt2D_CutP1 = pt2D_CutP2;
		pt2D_CutP2 = pt2D_temp;
	}

	CLdfsUtil utilLdfs;
//   // get nomal vector (by 2D)
//   T_POINT3D pt3D_normalVec;   pt3D_normalVec.Set(0.0, 0.0, 0.0);
//   if (!utilLdfs.GetNormal2D(pt3D_normalVec, NodeD_start, NodeD_end))  { ASSERT(0);  return FALSE; }
// 
//   if (!GetCuttingLine2D(pt2D_CutP1, pt2D_CutP2, pt3DOnCuttingPlane, pt3D_normalVec))  { ASSERT(0);  return FALSE; }

	// 합성 단면의 도심 (Global 기준)
	T_POINT3D ptCentG;    ptCentG.Set(0.0, 0.0, 0.0);
	double dArea_sum = 0.0;
	CArray<T_CENTROID_BASE,T_CENTROID_BASE&> aCentBase;   
	CArray<T_VSEC_D, T_VSEC_D&> aVsecD;   aVsecD.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> aElemK_bound;  aElemK_bound.RemoveAll();
	CArray<T_NODE_K,T_NODE_K> aNodeK_bound;  aNodeK_bound.RemoveAll();

	//int iDir = 0;
	//int iDir = 1;
	// int nModeTol : Tolerance 비교를 각도로 할지, 거리로할지 (=0: 거리, =1: 각도) 
	// (Polygon 타입은 단면정의라서 거리가 좋고, Elem 타입은 단면이 없기 때문에 각도가 좋음)
	int nModeTol = 0;
	if (!utilLdfs.GetLdfsSectInfo(aElemK_cutting, pt2D_CutP1, pt2D_CutP2, iDir, nModeTol, dZVec,  aCentBase, ptCentG, dArea_sum, aElemK_bound, aNodeK_bound))  { ASSERT(0);  return FALSE; }

	// get vsec data
	if (!GetDataVsec_fromElemNode(VsecD, aElemK_bound, aNodeK_bound, ptCentG, dZVec, strName))  { ASSERT(0);  return FALSE; }

	return TRUE;
}

BOOL CSectForResultantForceDlg::GetDataVsec_fromElemNode(T_VSEC_D& VsecD, 
																										 const CArray<T_ELEM_K,T_ELEM_K>& aElemK, const CArray<T_NODE_K,T_NODE_K>& aNodeK, 
																										 const T_POINT3D& ptCentG, const double dZVec[3],
																										 LPCTSTR lpszName)
{
	VsecD.Initialize();
	if (aElemK.GetSize() == 0)   { ASSERT(0);  return FALSE; }   // 

	int nCentCalcType = 0; // =0:Auto, =1:User
	VsecD.strName = lpszName;
	VsecD.arKeyElem.Copy(aElemK);
	VsecD.arKeyNode.Copy(aNodeK);
	VsecD.nCentCalcType = nCentCalcType;  // 도심 계산 방법 (=0: Auto, =1: User)
	VsecD.dCentX = ptCentG.x;      // centroid X (Global)
	VsecD.dCentY = ptCentG.y;      // centroid Y (Global)
	VsecD.dCentZ = ptCentG.z;      // centroid Z (Global)

	VsecD.dNormalVecX = dZVec[0];
	VsecD.dNormalVecY = dZVec[1];
	VsecD.dNormalVecZ = dZVec[2];

	return TRUE;
}

// cutting 면
// pt2D_CutP1 : [out] cutting 면의 시작점
// pt2D_CutP2 : [out] cutting 면의 끝점
BOOL CSectForResultantForceDlg::GetCuttingLine2D(T_POINT2D& pt2D_CutP1, T_POINT2D& pt2D_CutP2, 
																								 const T_POINT3D& ptDiv, const T_POINT3D& pt3D_normalVec, const double dCuttingScale)
{
	// cutting line (p1)
	pt2D_CutP1.x = ptDiv.x + (-1.0)*dCuttingScale * pt3D_normalVec.x;
	pt2D_CutP1.y = ptDiv.y + (-1.0)*dCuttingScale * pt3D_normalVec.y;

	// cutting line (p2)
	pt2D_CutP2.x = ptDiv.x + dCuttingScale * pt3D_normalVec.x;
	pt2D_CutP2.y = ptDiv.y + dCuttingScale * pt3D_normalVec.y;

	return TRUE;
}

void CSectForResultantForceDlg::OnTmBtnAdd() 
{ 
	if(m_pDoc->m_pAttrCtrl->GetCountStag() > 0 && m_pDoc->IsPostMode())
	{
		AfxMessageBox(_LS(IDS_TM_VSEC_EDIT_PRE_ONLY));
		return;
	}

	Dlg2Data();

	CDataCtrl* pDataCtrl = m_pDoc->m_pDataCtrl;    ASSERT(pDataCtrl);
	T_VSEC_D VsecD;

	int nMode = GetMode();
	if (nMode == D_RFORC_MODE_POLYGON_SELECT)
	{
		if(!m_bIsComplete)return; 

		CString strName;
		m_edtName.GetWindowText(strName);   strName.Trim();
		if (strName.IsEmpty())  { AfxMessageBox(_LS(IDS_TM_SECT_RFORC_NAME_MISS));  m_edtName.SetFocus();  return; }

		CArray<T_VSEC_D, T_VSEC_D&> aVsecD;   aVsecD.RemoveAll();

		// get sect data
		int iDir = 1;  // j-Dir
		if (GetDataVsec_fromPolygon(VsecD, iDir, strName)) { aVsecD.Add(VsecD); }

		iDir = 0;  // i-Dir
		if (GetDataVsec_fromPolygon(VsecD, iDir, strName)) { aVsecD.Add(VsecD); }

		if (aVsecD.GetSize() == 0)
		{
			//"Virtual Section을 구성하는 요소를 찾을 수 없습니다. direction vector를 입력 후 다시 시도해 주세요."
			AfxMessageBox(_LS(IDS_TM_VSEC_NOT_FOUND_ELEMENTS_TRY_AGAIN));  ASSERT(0);  return; 
		}
		
		// add vsec to DB
		CArray<T_VSEC_K, T_VSEC_K> aVsecK;   aVsecK.RemoveAll();
		if (!pDataCtrl->AddVsec(aVsecD, FALSE, &aVsecK))  { ASSERT(0);  return; }
	}
	else  // D_RFORC_MODE_STRUCT_GROUP, D_RFORC_MODE_ELEMENTS_SELECT
	{
		CLdfsUtil utilLdfs;

		// Get Selected Elem list
		CArray<T_ELEM_K,T_ELEM_K> aElemK_sel;   aElemK_sel.RemoveAll();
		int nElem = 0;
		if (nMode == D_RFORC_MODE_STRUCT_GROUP) GetSelectedGrupElemKeyList(aElemK_sel);
		else nElem = GetSelectedElemKeyList(aElemK_sel);

		double dZVec[3] = {0.0, };
		if(m_chkZVec.GetCheck() != 0) // use user direction
		{
			GetVecFromMouseEdit(m_edtZVec, dZVec);

			SetZVec(dZVec[0], dZVec[1], dZVec[2]);
		}
		else  // auto direction
		{
			BOOL bUseStartEnd = FALSE;
			if (!utilLdfs.GetDirVectorFromElements(dZVec, aElemK_sel, bUseStartEnd))  { ASSERT(0);  return; }
		}

		// get start/end position
		T_NODE_D NodeD_start;
		T_NODE_D NodeD_end;
		if (!utilLdfs.GetStartEndPosition2DFromElemKList(aElemK_sel, NodeD_start, NodeD_end, dZVec))  { ASSERT(0);  return; }
		double dLenStartEnd = CMathFunc::mathLength(NodeD_start.x, NodeD_start.y, NodeD_start.z,  NodeD_end.x, NodeD_end.y, NodeD_end.z);

		// get divide point list
		int nDiv = 0;
		if (!CFormulaEdit::GetEditValue(&m_edtDiv, nDiv)) nDiv = 0;

		CArray<T_POINT3D,T_POINT3D&> aPt3DList_div;   aPt3DList_div.RemoveAll();
		if (!utilLdfs.GetDividePtList(aPt3DList_div, NodeD_start, NodeD_end, nDiv))  { ASSERT(0);  return; }

		// get nomal vector (by 2D)
		T_POINT3D pt3D_normalVec;   pt3D_normalVec.Set(0.0, 0.0, 0.0);
		if (!utilLdfs.GetNormal2D(pt3D_normalVec, NodeD_start, NodeD_end))  { ASSERT(0);  return; }

		// cutting 면
		T_POINT2D pt2D_CutP1;  pt2D_CutP1.Set(0.0, 0.0);    // cutting line (p1)
		T_POINT2D pt2D_CutP2;  pt2D_CutP2.Set(0.0, 0.0);    // cutting line (p2)

		// 합성 단면의 도심 (Global 기준)
		T_POINT3D ptCentG;    ptCentG.Set(0.0, 0.0, 0.0);
		double dArea_sum = 0.0;
		CArray<T_CENTROID_BASE,T_CENTROID_BASE&> aCentBase;   
		CArray<T_VSEC_D, T_VSEC_D&> aVsecD;   aVsecD.RemoveAll();
		
		CString strName = _T("");
		if (nMode == D_RFORC_MODE_STRUCT_GROUP) { strName = GetSelectedGrupName(); }
		else  { m_edtName.GetWindowText(strName); }

		strName.Trim();
		if (strName.IsEmpty())  { AfxMessageBox(_LS(IDS_TM_SECT_RFORC_NAME_MISS));  m_edtName.SetFocus();  return; }

		int nPoints_div = aPt3DList_div.GetSize();
		for (int i=0; i<nPoints_div; ++i)
		{
			T_POINT3D& ptDiv = aPt3DList_div[i];
			//if (!GetCuttingLine2D(pt2D_CutP1, pt2D_CutP2, ptDiv, pt3D_normalVec, 1.2*dLenStartEnd))  { ASSERT(0);  return; }
			if (!GetCuttingLine2D(pt2D_CutP1, pt2D_CutP2, ptDiv, pt3D_normalVec, 100.0*dLenStartEnd))  { ASSERT(0);  return; }

			// Get Cutting Elem list
			CArray<T_ELEM_K,T_ELEM_K> aElemK_cutting;
			GetElemListByCutting(aElemK_cutting, pt2D_CutP1, pt2D_CutP2, aElemK_sel);
			
			// 가상보의 각 단면에 대한 loop
			for (int iDir=1; iDir>=0; --iDir)   // i + j 방향 두번을 찾음
			{
				if (i==0 && iDir==1) continue;   // 시작위치(i=0)는 i단면(k=0)만 찾음
				if (nPoints_div != 1)
				{
					if (i==nPoints_div-1 && iDir==0) continue;   // 마지막위치는 j단면(k=1)만 찾음
				}

				CArray<T_ELEM_K,T_ELEM_K> aElemK_bound;  aElemK_bound.RemoveAll();
				CArray<T_NODE_K,T_NODE_K> aNodeK_bound;  aNodeK_bound.RemoveAll();

				// int nModeTol : Tolerance 비교를 각도로 할지, 거리로할지 (=0: 거리, =1: 각도) 
				// (Polygon 타입은 단면정의라서 거리가 좋고, Elem 타입은 단면이 없기 때문에 각도가 좋음)
				int nModeTol = 0;
				if (nDiv < 2) nModeTol = 1;   // 양 끝단만 생성하는 경우는 각도방식으로 가도 될 듯.

				if (!utilLdfs.GetLdfsSectInfo(aElemK_cutting, pt2D_CutP1, pt2D_CutP2, iDir, nModeTol, dZVec,  aCentBase, ptCentG, dArea_sum, aElemK_bound, aNodeK_bound))  { ASSERT(0);  return; }

				if (aElemK_bound.GetSize() == 0)
				{
					//[Error] Selected elements are not valid
					GSaveHistoryFormatNF(_LS(IDS_TM_VSEC_NOT_FOUND_ELEMENTS));  ASSERT(0);  return; 
				}

				// get vsec data
				if (!GetDataVsec_fromElemNode(VsecD, aElemK_bound, aNodeK_bound, ptCentG, dZVec, strName))  { ASSERT(0);  return; }
				CString strPostFix;
				strPostFix.Format(_T("_%d_%s"), i+1, (iDir == 0)? _T("I") : _T("J"));
				VsecD.strName += strPostFix;
				aVsecD.Add(VsecD);
			}
		}

		// add vsec to DB
		CArray<T_VSEC_K, T_VSEC_K> aVsecK;   aVsecK.RemoveAll();
		if (!pDataCtrl->AddVsec(aVsecD, FALSE, &aVsecK))  { return; }

		// add vbem to DB
		CArray<T_VBEM_D, T_VBEM_D&> aVbemD;   aVbemD.RemoveAll();
		T_VBEM_D VbemD;   VbemD.Initialize();
		for (int i=0; i<aVsecK.GetSize()-1; i+=2)
		{
			VbemD.Vsec1 = aVsecK[i  ];
			VbemD.Vsec2 = aVsecK[i+1];
			aVbemD.Add(VbemD);
		}
		if (aVbemD.GetSize() > 0)
		{
			if (!pDataCtrl->AddVbem(aVbemD))  { return; }
		}
	}
}

void CSectForResultantForceDlg::OnTmBtnDel() 
{
	if(m_pDoc->m_pAttrCtrl->GetCountStag() > 0 && m_pDoc->IsPostMode())
	{
		AfxMessageBox(_LS(IDS_TM_VSEC_EDIT_PRE_ONLY));
		return;
	}

	CArray<T_VSEC_K,T_VSEC_K> aVsecK_sel;   aVsecK_sel.RemoveAll();
	if (m_pGridLoc) m_pGridLoc->GetSelectedVsecKList(aVsecK_sel);
	if (aVsecK_sel.GetSize() == 0)  { ASSERT(0);  return; }

	m_pDoc->m_pDataCtrl->DelVsec(aVsecK_sel);

	ResetPolygonCoord();
}

void CSectForResultantForceDlg::OnTmBtnMod() 
{
	if(m_pDoc->m_pAttrCtrl->GetCountStag() > 0 && m_pDoc->IsPostMode())
	{
		AfxMessageBox(_LS(IDS_TM_VSEC_EDIT_PRE_ONLY));
		return;
	}

	CArray<T_VSEC_K,T_VSEC_K> aVsecK_sel;   aVsecK_sel.RemoveAll();
	if (m_pGridLoc) m_pGridLoc->GetSelectedVsecKList(aVsecK_sel);
	if (aVsecK_sel.GetSize() == 0)  { ASSERT(0);  return; }

	Dlg2Data();
	int nVsec = aVsecK_sel.GetSize();

	// get selected elem
	CArray<T_ELEM_K,T_ELEM_K> aElemK_sel;   aElemK_sel.RemoveAll();
	GetSelectedElemKeyList(aElemK_sel);
	if (aElemK_sel.GetSize() == 0)  { AfxMessageBox(_LS(IDS_TM_VSEC_NOT_SELECTED_NODE_ELEM));  return; }

	// get selected node
	CArray<T_NODE_K,T_NODE_K> aNodeK_sel;   aNodeK_sel.RemoveAll();
	GetSelectedNodeKeyList(aNodeK_sel);
	if (aNodeK_sel.GetSize() == 0)  { AfxMessageBox(_LS(IDS_TM_VSEC_NOT_SELECTED_NODE_ELEM));  return; }

	CMapEx<T_NODE_K,T_NODE_K,int,int> mNodeK;   mNodeK.RemoveAll();
	mNodeK.InitHashTable(97);
	for (int i=0; i<aNodeK_sel.GetSize(); ++i)  mNodeK.SetAt(aNodeK_sel[i], 0);

	// check single node or elem
	CString strMsg;
	T_ELEM_D ElemD;
	for (int i=0; i<aElemK_sel.GetSize(); ++i)
	{
		if (!m_pDoc->m_pAttrCtrl->GetElem(aElemK_sel[i], ElemD))  { ASSERT(0);  return; }
		int nNode = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);

		BOOL bNodeUsed = FALSE;
		for(int k=0; k<nNode; ++k)
		{
			if (mNodeK.Exist(ElemD.elnod[k]))
			{
				bNodeUsed = TRUE;
				mNodeK.SetAt(ElemD.elnod[k], 1);    // used node
			}
		}
		if (!bNodeUsed)
		{ 
			strMsg.Format(_LS(IDS_TM_VSEC_ELEM_WITHOUT_NODE_IS_ASSIGNED__d), aElemK_sel[i]);
			AfxMessageBox(strMsg);  
			return; 
		}
	}
	T_NODE_K NodeK = 0;
	int nUsed = 0;
	POSITION pos = mNodeK.GetStartPosition();
	while(pos)
	{
		mNodeK.GetNextAssoc(pos, NodeK, nUsed);
		if (nUsed == 0) 
		{ 
			strMsg.Format(_LS(IDS_TM_VSEC_NODE_WITHOUT_ELEM_IS_ASSIGNED__d), NodeK);
			AfxMessageBox(strMsg);  

			// remove node
			int nSize = aNodeK_sel.GetSize();
			for (int i = nSize - 1; i >= 0; --i)
			{
				if(NodeK == aNodeK_sel[i])
					aNodeK_sel.RemoveAt(i);
			}
		}
	}

	// MQC 4899-12. Modify시 Direction을 변경해 준다.
	CArray<T_VSEC_D,T_VSEC_D&> aVsecD;  aVsecD.RemoveAll();
	aVsecD.SetSize(nVsec);
	for (int i=0; i<nVsec; ++i)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetVsec(aVsecK_sel[i], aVsecD[i]))  { ASSERT(0);  return; }

		if(m_chkZVec.GetCheck() != 0) // user input
		{
			aVsecD[i].dNormalVecX = m_dZVec[0];
			aVsecD[i].dNormalVecY = m_dZVec[1];
			aVsecD[i].dNormalVecZ = m_dZVec[2];
		}
		
		aVsecD[i].nCentCalcType = 0;    // auto 계산으로 변경
		aVsecD[i].arKeyElem.Copy(aElemK_sel);
		aVsecD[i].arKeyNode.Copy(aNodeK_sel);

		if (aVsecD[i].nCentCalcType == 0)    // 도심 자동계산
		{
			CLdfsUtil utilLdfs;
			T_COMPSECT_INFO infoSect;   infoSect.Initialize();
			if (utilLdfs.GetCompSectInforFromVsec(aVsecD[i], infoSect))  { aVsecD[i] = infoSect.VsecD; }
		}
	}

	if (!m_pDoc->m_pDataCtrl->ModifyVsec(aVsecK_sel, aVsecD))  { return; }

	ResetPolygonCoord();
}

void CSectForResultantForceDlg::InitControls()
{
	// init combo
	m_cmbMode.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbMode, _LS(IDS_TM_SECT_RFORC_Structure_Group), D_RFORC_MODE_STRUCT_GROUP   );   // Structure Group
	CDlgUtil::CobxAddItem(m_cmbMode, _LS(IDS_TM_SECT_RFORC_Polygon_Select ), D_RFORC_MODE_POLYGON_SELECT );   // Polygon Select
	CDlgUtil::CobxAddItem(m_cmbMode, _LS(IDS_TM_SECT_RFORC_Elements_Select), D_RFORC_MODE_ELEMENTS_SELECT);   // Elements Select
	CDlgUtil::CobxSetCurSelItemData(m_cmbMode, D_RFORC_MODE_ELEMENTS_SELECT);  // Polygon Select

	// tolerance
	m_edtTolerance.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTolerance.SetUnitType(D_UNITSYS_BASE_LENGTH);

	// No. of Division
	m_edtDiv.SetWindowText(_T("1"));

	// Coord. GroupBox
	m_grpCoord.InitControl(this, IDD, IDC_TM_SECT_RFOC_VSEC_GRP,TRUE,TRUE,1,1);
	m_grpCoord.SetFoldState(TRUE);

	// Coord. Edit box
	m_edtInputCoord.SetAttWcsPos();
	m_edtInputCoord.SetEscNotifyWindow(this);
	m_edtInputCoord.SetLButtonDownNotifyWindow(this);
	m_edtInputCoord.SetMViewCancelNotifyWindow(this);
	m_edtInputCoord.SetModeToUse(MOUSEEDIT_USE_GET_EDGESEL_POS);

	// Direction Vector
	m_edtZVec.SetAttWcsDistance();
	m_edtZVec.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	//m_edtZVec.SetLButtonDownNotifyWindow(this);

	// Grid
	InitGrid();

	// init data
	m_dTolerance = M_InitValueCurUnit(0.1, KN, MM, D_UNITSYS_BASE_LENGTH);     // 0.1mm
	//-------------------------------------------------------------
	// 단위에 따른 default 값 변경
	T_UNIT_INDEX CurrentUnitIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurrentUnitIndex);
	if (CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)  m_dTolerance = M_InitValueCurUnit(0.0003, KN, FT, D_UNITSYS_BASE_LENGTH);     // 0.0003ft
	if (CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)  m_dTolerance = M_InitValueCurUnit(0.0039, KN, IN, D_UNITSYS_BASE_LENGTH);     // 0.0039in
	//-------------------------------------------------------------

	m_edtTolerance.SetEditUnit(m_dTolerance);
	m_chkZVec.SetCheck(0);
}

void CSectForResultantForceDlg::InitGrid()
{
	ASSERT(m_pGridLoc);
	ASSERT(m_pGridVBeam);

	m_pGridLoc->Initialize();
	m_pGridVBeam->Initialize();
}

void CSectForResultantForceDlg::PostNcDestroy() 
{
	if(m_pGridLoc)  { delete m_pGridLoc;  m_pGridLoc = NULL; }

	CMenuBarChildDlg::PostNcDestroy();
}

void CSectForResultantForceDlg::ResetPolygonCoord()
{
	m_edtInput.SetWindowText(_T(""));  // Point List를 저장하는 MultiLine Edit Control    
	//I_GENModelBase::GetCurMySelfST()->ClearFaceEdgeSelData(); // LDFS - gykim    
	I_GENModelBase::GetCurMySelfST()->ClearFaceEdgeSelData(FALSE); // LDFS - gykim    
	m_arXYZ[0].RemoveAll(); 
	m_arXYZ[1].RemoveAll(); 
	m_arXYZ[2].RemoveAll(); 
	m_dZVec[0]=m_dZVec[1]=m_dZVec[2]=0.0;

	SetSelFaceEdgeMode(FALSE);  

	I_GENModelBase::GetCurMySelfST()->MakeCurFaceEdgeSelData(FALSE); // ldfs - gykim    
	//I_GENModelBase::GetCurMySelfST()->ResetSelFaceEdge();

	m_bIsComplete = TRUE;
}

BOOL CSectForResultantForceDlg::GetVecFromMouseEdit(CMouseEdit& editMouse, double dZVec[3])
{
	double VWX,VWY,VWZ,VUX,VUY,VUZ;
	if(editMouse.IsKeyPressed())
	{
		CString strTemp;   editMouse.GetWindowText(strTemp);
		if(!CStrParser::GetDirectionVector(strTemp,VWX,VWY,VWZ)) return FALSE;//Error
	}
	else
	{
		editMouse.GetCurMousePosReal(VWX,VWY,VWZ,VUX,VUY,VUZ);
	}

	dZVec[0] = VWX;
	dZVec[1] = VWY;
	dZVec[2] = VWZ;
	CMathFunc::mathNormalize(dZVec, dZVec);

	return TRUE;
}

LRESULT CSectForResultantForceDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_edtInputCoord.GetSafeHwnd() == (HWND)lParam)
	{
		UpdateData(TRUE); 
		if(m_bIsComplete)
		{
			ResetPolygonCoord();
		}

		CString strTemp,strParse,strResult;
		m_edtInput.GetWindowText(strParse);
		m_edtInputCoord.GetWindowText(strTemp);

		// MNET:XXXX-HSSHIM-20080116. MouseEdit에서 좌표를 읽어서 사용하도록 수정함. 
		double VWX,VWY,VWZ,VUX,VUY,VUZ;
		if(m_edtInputCoord.IsKeyPressed())
		{
			if(!CStrParser::GetDirectionVector(strTemp,VUX,VUY,VUZ))return 0L;//Error
		}
		else
		{
			m_edtInputCoord.GetCurMousePosReal(VWX,VWY,VWZ,VUX,VUY,VUZ);
		}

		double x,y,z;
		CArray<double,double>CoordList;
		// BOOL bIsSuccess = CStrParser::GetPosition(strTemp,x,y,z);
		x = VUX;    y = VUY;    z = VUZ;
		int nCoord = CoordList.GetSize(); 
		//if(bIsSuccess)//for(int i = 0 ; i < nCoord ; i+= 3)
		{
			m_arXYZ[0].Add(x);
			m_arXYZ[1].Add(y);
			m_arXYZ[2].Add(z);
		}

		if(strParse.GetLength() != 0)
			strResult.Format(_T("\r\n[%s]"),strTemp);
		else
			strResult.Format(_T("[%s]"),strTemp);

		strParse += strResult;
		m_edtInput.SetWindowText(strParse);

		int nLine = m_edtInput.GetLineCount();
		m_edtInput.LineScroll(nLine);
		if(I_GENModelBase::GetCurMySelfST()->IsCompleteSelFaceEdge())
		{       
			if(m_chkZVec.GetCheck() != 0) // Polygon Select Mode... + z-vector checked
			{
				double dZVec[3] = {0.0, };
				GetVecFromMouseEdit(m_edtZVec, dZVec);

				I_GENModelBase::GetCurMySelfST()->SetSelFaceEdgeZDirVector(dZVec[0],dZVec[1],dZVec[2]); // Local ZVector 설정

				SetZVec(VWX, VWY, VWZ);
			}
			else
			{
				// get from polgon
				int nCountCoods = m_arXYZ[0].GetSize();
				if(nCountCoods > 2)
				{
					double dVec1[3] = {0.0,};
					dVec1[0] = m_arXYZ[0][1] - m_arXYZ[0][0];
					dVec1[1] = m_arXYZ[1][1] - m_arXYZ[1][0];
					dVec1[2] = m_arXYZ[2][1] - m_arXYZ[2][0];
					CMathFunc::mathNormalize(dVec1, dVec1);

					double dVec2[3] = {0.0,};
					dVec2[0] = m_arXYZ[0][2] - m_arXYZ[0][1];
					dVec2[1] = m_arXYZ[1][2] - m_arXYZ[1][1];
					dVec2[2] = m_arXYZ[2][2] - m_arXYZ[2][1];
					CMathFunc::mathNormalize(dVec2, dVec2);

					// normal
					double dZVec[3] = {0.0, };
					CMathFunc::mathNormal(dVec1, dVec2, dZVec);
					SetZVec(dZVec[0], dZVec[1], dZVec[2]);
				}
			}

			//I_GENModelBase::GetCurMySelfST()->MakeCurFaceEdgeSelData();      // ldfs - gykim
			I_GENModelBase::GetCurMySelfST()->MakeCurFaceEdgeSelData(FALSE); // ldfs - gykim    
			I_GENModelBase::GetCurMySelfST()->ResetSelFaceEdge();

			m_bIsComplete = TRUE;

			CalcCentroidAuto_PolyGon();
		}
		else
		{
			m_bIsComplete = FALSE;
		}
	}

	return 0L;
}

LRESULT CSectForResultantForceDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

LRESULT CSectForResultantForceDlg::OnMouseEditEscape(WPARAM wParam, LPARAM lParam)
{
	return OnMViewCancel(wParam, lParam);
}

LRESULT CSectForResultantForceDlg::OnMViewCancel(WPARAM wParam, LPARAM lParam)
{
	if(m_edtInputCoord.GetSafeHwnd() == (HWND)lParam)
	{        
		ResetPolygonCoord(); 

		I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);     
		CMouseEdit::ImNotAssocWindowAllForceST();
	}
	else if (m_edtZVec.GetSafeHwnd() == (HWND)lParam)
	{
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(!pIGM) return 0L;
	}

	return 0L;
}

LRESULT CSectForResultantForceDlg::OnVsecGridClicked(WPARAM wParam, LPARAM lParam)
{
	T_VSEC_K VsecK = (T_VSEC_K) wParam;

	CArray<T_VSEC_K,T_VSEC_K> aVsecK_sel;   aVsecK_sel.RemoveAll();
	if (m_pGridLoc) m_pGridLoc->GetSelectedVsecKList(aVsecK_sel);
	if (aVsecK_sel.GetSize() == 0)  { aVsecK_sel.Add(VsecK); }
	if (VsecK == 0) VsecK = aVsecK_sel[0];

	CAttrCtrl2* pAttrCtrl2 = m_pDoc->m_pAttrCtrl2;  ASSERT(pAttrCtrl2);
	T_VSEC_D VsecD;
	CMapEx<T_ELEM_K,T_ELEM_K, int,int> mElemK;    mElemK.RemoveAll();   mElemK.InitHashTable(997);
	CMapEx<T_NODE_K,T_NODE_K, int,int> mNodeK;    mNodeK.RemoveAll();   mNodeK.InitHashTable(997);
	for (int i=0; i<aVsecK_sel.GetSize(); ++i)
	{
		if (pAttrCtrl2->GetVsec(aVsecK_sel[i], VsecD))
		{
			for (int k=0; k<VsecD.arKeyElem.GetSize(); ++k) mElemK.SetAt(VsecD.arKeyElem[k], 0);
			for (int k=0; k<VsecD.arKeyNode.GetSize(); ++k) mNodeK.SetAt(VsecD.arKeyNode[k], 0);
		}
	}

	CArray<T_ELEM_K,T_ELEM_K> aElemK;   aElemK.RemoveAll();
	CArray<T_NODE_K,T_NODE_K> aNodeK;   aNodeK.RemoveAll();
	mElemK.GetSortedKeyListforUintK(aElemK);
	mNodeK.GetSortedKeyListforUintK(aNodeK);

	// select elem of Vsec
	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	pIGM->UnselectAllElem();    pIGM->AddSelectedElemKeyList(aElemK);
	pIGM->UnselectAllNode();    pIGM->AddSelectedNodeKeyList(aNodeK);

	if (pAttrCtrl2->GetVsec(VsecK, VsecD))
	{
		// show Direction Vector
		SetZVec(VsecD.dNormalVecX, VsecD.dNormalVecY, VsecD.dNormalVecZ);
	}

	return 0L;
}

LRESULT CSectForResultantForceDlg::OnVbemGridClicked(WPARAM wParam, LPARAM lParam)
{
	T_VBEM_K VbemK = (T_VBEM_K) wParam;

	CArray<T_VBEM_K,T_VBEM_K> aVbemK_sel;   aVbemK_sel.RemoveAll();
	if (m_pGridVBeam) m_pGridVBeam->GetSelectedVbemKList(aVbemK_sel);
	if (aVbemK_sel.GetSize() == 0)  { aVbemK_sel.Add(VbemK); }
	if (VbemK == 0) VbemK = aVbemK_sel[0];

	CMapEx<T_ELEM_K,T_ELEM_K, int,int> mElemK;    mElemK.RemoveAll();   mElemK.InitHashTable(997);
	CMapEx<T_NODE_K,T_NODE_K, int,int> mNodeK;    mNodeK.RemoveAll();   mNodeK.InitHashTable(997);

	CAttrCtrl2* pAttrCtrl2 = m_pDoc->m_pAttrCtrl2;  ASSERT(pAttrCtrl2);
	T_VBEM_D VbemD;
	for (int i=0; i<aVbemK_sel.GetSize(); ++i)
	{
		if (pAttrCtrl2->GetVbem(aVbemK_sel[i], VbemD))
		{
			T_VSEC_D VsecD;
			if (pAttrCtrl2->GetVsec(VbemD.Vsec1, VsecD))  
			{
				for (int k=0; k<VsecD.arKeyElem.GetSize(); ++k) mElemK.SetAt(VsecD.arKeyElem[k], 0);
				for (int k=0; k<VsecD.arKeyNode.GetSize(); ++k) mNodeK.SetAt(VsecD.arKeyNode[k], 0);
			}
			else  ASSERT(0);

			if (pAttrCtrl2->GetVsec(VbemD.Vsec2, VsecD))  
			{
				for (int k=0; k<VsecD.arKeyElem.GetSize(); ++k) mElemK.SetAt(VsecD.arKeyElem[k], 0);
				for (int k=0; k<VsecD.arKeyNode.GetSize(); ++k) mNodeK.SetAt(VsecD.arKeyNode[k], 0);
			}
			else  ASSERT(0);
		}
	}

	CArray<T_ELEM_K,T_ELEM_K> aElemK;   aElemK.RemoveAll();
	CArray<T_NODE_K,T_NODE_K> aNodeK;   aNodeK.RemoveAll();
	mElemK.GetSortedKeyListforUintK(aElemK);
	mNodeK.GetSortedKeyListforUintK(aNodeK);

	// select elem of Vsec
	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	pIGM->UnselectAllElem();    pIGM->AddSelectedElemKeyList(aElemK);
	pIGM->UnselectAllNode();    pIGM->AddSelectedNodeKeyList(aNodeK);

	return 0L;
}

void CSectForResultantForceDlg::SetSelFaceEdgeMode(BOOL bInit)
{
	// (1) Face Select Mode (2) Edge Select Mode (3) Edge Select By Polygon (4) Face & Edge Select By Polygon
	int nSelFaceEdgeMode = 3;
	I_GENModelBase::GetCurMySelfST()->SetSelFaceEdgeMode(nSelFaceEdgeMode, bInit);	

	//m_edtInputCoord.SetModeToUse(MOUSEEDIT_USE_GET_FACESEL_POS);
	m_edtInputCoord.SetModeToUse(MOUSEEDIT_USE_GET_EDGESEL_POS);
}

void CSectForResultantForceDlg::CalcCentroidAuto()
{
	int nMode = GetMode();
	if (nMode != D_RFORC_MODE_STRUCT_GROUP   && 
			nMode != D_RFORC_MODE_POLYGON_SELECT && 
			nMode != D_RFORC_MODE_ELEMENTS_SELECT)  { ASSERT(0);  return; }

	if (nMode == D_RFORC_MODE_POLYGON_SELECT)
	{
		CalcCentroidAuto_PolyGon();
	}
	else
	{ 
		ASSERT(0);  
	}
}

// iDir =0: X, =1: Y, =2: Z
BOOL CSectForResultantForceDlg::IsElemOutofMinMax(T_ELEM_D& ElemD, double dMin, double dMax, int iDir, double dTol)
{
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;   ASSERT(pAttrCtrl);

	T_NODE_D NodeD;
	int nPos_prev = 0;
	BOOL bElemCross = FALSE;
	int NumNode=pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);

	// about x 
	nPos_prev = 0;
	bElemCross = FALSE;
	for(int i=0; i<NumNode; i++)
	{
		pAttrCtrl->GetNode(ElemD.elnod[i], NodeD);

		double dValue = 0.0;
		if (iDir == 0) dValue = NodeD.x;
		if (iDir == 1) dValue = NodeD.y;
		if (iDir == 2) dValue = NodeD.z;

		int nPos_cur = 0;
		if (dValue < dMin -dTol)  nPos_cur = -1;   // min보다 작으면
		else if (dValue > dMax+dTol)  nPos_cur = 1;  // max 보다 크면
		else { bElemCross = TRUE;  break; }      // min과 max 사이에 들어오면

		int nPos_comp = 0;
		if (nPos_prev != 0) nPos_comp = nPos_cur*nPos_prev;
		if (nPos_comp < 0)  { bElemCross = TRUE;  break; }  // 부호가 반대이면 (min/max 양쪽에 걸치면)

		nPos_prev = nPos_cur;
	}
	if (!bElemCross)  return TRUE;     // 교차 불가
	return FALSE;
}

BOOL CSectForResultantForceDlg::IsElemOutofMinMax(T_ELEM_D& ElemD, double dMinX, double dMaxX, double dMinY, double dMaxY, double dMinZ, double dMaxZ, double dTol)
{
	if (IsElemOutofMinMax(ElemD, dMinX, dMaxX, 0, dTol)) return TRUE;
	if (IsElemOutofMinMax(ElemD, dMinY, dMaxY, 1, dTol)) return TRUE;
	if (IsElemOutofMinMax(ElemD, dMinZ, dMaxZ, 2, dTol)) return TRUE;

	return FALSE;
}

// 요소의 절점이 convex hull 영역에 들어오는지 검사
BOOL CSectForResultantForceDlg::IsElemIntersectWithConvexHull(T_ELEM_D& ElemD, T_POLYLINE2D& ConvxHull, double dTol)
{
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;   ASSERT(pAttrCtrl);

	T_POINT2D pnt;

	// Cross 검사용 ccw를 위한 기준 line 설정
	int iCCW_prev = 0;
	BOOL bElemCross = FALSE;
	T_POINT2D pt2D_CutP1, pt2D_CutP2;
	if (ConvxHull.GetSize() > 0) { pt2D_CutP1 = ConvxHull[0];   pt2D_CutP2 = pt2D_CutP1; }
	for (int i=1; i<ConvxHull.GetSize(); ++i)
	{
		// 
		if (pt2D_CutP2.x < ConvxHull[i].x - dTol) pt2D_CutP2 = ConvxHull[i];
		else 
		{
			// 더 멀리 있는 점
			double dDist1 = CMathFunc::mathLength(pt2D_CutP1.x, pt2D_CutP1.y,  pt2D_CutP2.x, pt2D_CutP2.y);
			double dDist2 = CMathFunc::mathLength(pt2D_CutP1.x, pt2D_CutP1.y,  ConvxHull[i].x, ConvxHull[i].y);
			if (dDist1 < dDist2)  pt2D_CutP2 = ConvxHull[i];
		}
	}

	BOOL bExistInnerNode = FALSE;
	T_NODE_D NodeD;
	int NumNode=pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);
	for(int i=0; i<NumNode; i++)
	{
		pAttrCtrl->GetNode(ElemD.elnod[i], NodeD);

		// a point for check
		pnt.x = NodeD.x;
		pnt.y = NodeD.y;

		// check inside node
		if (CGeomFunc::CheckInsideConvexHull(ConvxHull, pnt, dTol)) { bExistInnerNode = TRUE;  break; }

		// check cross by ccw
		int iCCW_cur = CMathFunc::math_ccw(pt2D_CutP1.x, pt2D_CutP1.y,  pt2D_CutP2.x, pt2D_CutP2.y,  NodeD.x, NodeD.y,  dTol);
		if (iCCW_cur == 0) { bExistInnerNode = TRUE;  break; }    // convex hull의 최외곽 두점을 이은 선상에 있다는 것은 Convex Hull 안에 있다는 것...^^
		if (iCCW_prev != 0 && (iCCW_cur*iCCW_prev < 0)) { bElemCross = TRUE;  break; }  // 부호가 반대이면 cross

		iCCW_prev = iCCW_cur;
	}
	if (bExistInnerNode) return TRUE;
	if (bElemCross     )  return TRUE;     // ConvxHull과 교차

	return FALSE;
}

void CSectForResultantForceDlg::GetMBR3D(const CArray<double,double> arXYZ[3], double& dMaxX,double& dMinX, double& dMaxY,double& dMinY, double& dMaxZ,double& dMinZ)
{
	dMaxX = -DBL_MAX;
	dMinX =  DBL_MAX;
	dMaxY = -DBL_MAX;
	dMinY =  DBL_MAX;
	dMaxZ = -DBL_MAX;
	dMinZ =  DBL_MAX;

	int nCountCoods = arXYZ[0].GetSize();
	for (int i=0; i<nCountCoods; ++i)
	{
		if (dMaxX < arXYZ[0][i]) dMaxX = arXYZ[0][i];
		if (dMinX > arXYZ[0][i]) dMinX = arXYZ[0][i];

		if (dMaxY < arXYZ[1][i]) dMaxY = arXYZ[1][i];
		if (dMinY > arXYZ[1][i]) dMinY = arXYZ[1][i];

		if (dMaxZ < arXYZ[2][i]) dMaxZ = arXYZ[2][i];
		if (dMinZ > arXYZ[2][i]) dMinZ = arXYZ[2][i];
	}
}

// cutting 영역에 들어오는 elem list
int CSectForResultantForceDlg::GetElemListByCutting(CArray<UINT,UINT>& raElemK_cutting, 
																										const T_POINT2D& pt2D_CutP1, const T_POINT2D& pt2D_CutP2, 
																										const CArray<UINT,UINT>& aElemK_candi)
{
	CArray<double,double> arXYZ[3];
	arXYZ[0].RemoveAll(); 
	arXYZ[1].RemoveAll(); 
	arXYZ[2].RemoveAll(); 

	// start
	arXYZ[0].Add(pt2D_CutP1.x);
	arXYZ[1].Add(pt2D_CutP1.y);
	arXYZ[2].Add(-DBL_MAX);

	// end
	arXYZ[0].Add(pt2D_CutP2.x);
	arXYZ[1].Add(pt2D_CutP2.y);
	arXYZ[2].Add( DBL_MAX);

	// start (for polygon)
	arXYZ[0].Add(pt2D_CutP1.x);
	arXYZ[1].Add(pt2D_CutP1.y);
	arXYZ[2].Add(-DBL_MAX);

	return GetCuttingElemList(raElemK_cutting, arXYZ, aElemK_candi);
}

// 사용자가 지정한 cutting 영역에 들어오는 elem list
int CSectForResultantForceDlg::GetCuttingElemList(CArray<UINT,UINT>& raElemK_sel, const CArray<double,double> arXYZ[3], const CArray<UINT,UINT>& aElemK_candi)
{
	raElemK_sel.RemoveAll();

	int nCountCoods = arXYZ[0].GetSize();
	ASSERT(arXYZ[1].GetSize() == nCountCoods);
	ASSERT(arXYZ[2].GetSize() == nCountCoods);
	if(nCountCoods < 3)  { /*ASSERT(0);*/  return 0; }

	// cutting 면이 하나의 평면을 이루는지 체크

	// MBR 비교 (1차 필터)
	double dMaxX = 0.0;
	double dMinX = 0.0;
	double dMaxY = 0.0;
	double dMinY = 0.0;
	double dMaxZ = 0.0;
	double dMinZ = 0.0;
	GetMBR3D(arXYZ, dMaxX,dMinX, dMaxY,dMinY, dMaxZ,dMinZ);

	// make convex hull (2차 필터)
	CArray<T_POINT2D, T_POINT2D&> aPoints;  aPoints.RemoveAll();
	T_POINT2D pt2D;   pt2D.Set(0.0, 0.0);
	for (int i=0; i<nCountCoods; ++i) { pt2D.Set(arXYZ[0][i], arXYZ[1][i]);  aPoints.Add(pt2D); }
	if (aPoints.GetSize() <  2)  { ASSERT(0);  return 0; }

	T_POLYLINE2D ConvxHull;   ConvxHull.RemoveAll();
	if (!CGeomFunc::MakeConvexHoll( &aPoints, &ConvxHull))  { ASSERT(0);  return 0; }

	// Active elem (0차 필터). Polygon 타입은 Active된 요소만을 대상으로 한다.
	CArray<T_ELEM_K,T_ELEM_K> aElemK_active;  aElemK_active.RemoveAll();
	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();   ASSERT(pIGM);
	pIGM->GetActiveElemKeys(aElemK_active);
	CMapEx<T_ELEM_K,T_ELEM_K, int,int> mElemK_active;   mElemK_active.RemoveAll(); 
	mElemK_active.InitHashTable(HASHSIZEELEM);
	for (int i=0; i<aElemK_active.GetSize(); ++i) mElemK_active.SetAt(aElemK_active[i], 0);

	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;   ASSERT(pAttrCtrl);
	//double dTol = m_pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit();
	double dTol = M_InitValueCurUnit(0.05001, KN, M, D_UNITSYS_BASE_LENGTH);     // 5cm

	T_ELEM_K ElemK = 0;
	T_ELEM_D ElemD;
	for (int i=0; i<aElemK_candi.GetSize(); ++i)
	{
		ElemK = aElemK_candi[i];

		// 0차 is active
		if (!mElemK_active.Exist(ElemK)) continue;

		if (!pAttrCtrl->GetElem(ElemK, ElemD))  { ASSERT(0);  return FALSE; }

		// 1차 MBR 비교
		if (IsElemOutofMinMax(ElemD, dMinX, dMaxX, dMinY, dMaxY, dMinZ, dMaxZ, dTol)) continue;

		// 2차 convex hull 영역 비교
		if (!IsElemIntersectWithConvexHull(ElemD, ConvxHull, dTol)) continue;

		raElemK_sel.Add(ElemK);
	}

	// 2. 각 꼭지점과 cutting 면의 위치 비교 : cutting 면 양쪽에 있거나, cutting면 기준으로 한쪽에 있어야 함.

	// element type filtering
	CLdfsUtil utilLDFS;
	utilLDFS.FilterByCanElemType(raElemK_sel);

	return raElemK_sel.GetSize();
}


// 사용자가 지정한 cutting 영역에 들어오는 elem list
int CSectForResultantForceDlg::GetCuttingElemList(CArray<UINT,UINT>& raElemK_sel)
{
	CArray<T_ELEM_K,T_ELEM_K> aElemK_candi;  aElemK_candi.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemK_candi, FALSE);
	
	return GetCuttingElemList(raElemK_sel, m_arXYZ, aElemK_candi);
}

// get start/end position
BOOL CSectForResultantForceDlg::GetStartEndPosition(T_NODE_D& NodeD_start, T_NODE_D& NodeD_end, T_POINT3D& pt3DOnCuttingPlane)
{
	// Get z-Vector
	double dZVec[3] = {0.0, };
	dZVec[0]=m_dZVec[0];    dZVec[1]=m_dZVec[1];    dZVec[2]=m_dZVec[2];

	// cutting plane 상의 한 점
	double dMaxX = 0.0;
	double dMinX = 0.0;
	double dMaxY = 0.0;
	double dMinY = 0.0;
	double dMaxZ = 0.0;
	double dMinZ = 0.0;
	GetMBR3D(m_arXYZ, dMaxX,dMinX, dMaxY,dMinY, dMaxZ,dMinZ);

	//T_POINT3D pt3DOnCuttingPlane;
	pt3DOnCuttingPlane.x = 0.5*(dMaxX+dMinX);
	pt3DOnCuttingPlane.y = 0.5*(dMaxY+dMinY);
	pt3DOnCuttingPlane.z = 0.5*(dMaxZ+dMinZ);

	//T_NODE_D NodeD_start;
	NodeD_start.x = pt3DOnCuttingPlane.x;
	NodeD_start.y = pt3DOnCuttingPlane.y;
	NodeD_start.z = pt3DOnCuttingPlane.z;

	//T_NODE_D NodeD_end;
	NodeD_end.x = NodeD_start.x + dZVec[0];
	NodeD_end.y = NodeD_start.y + dZVec[1];
	NodeD_end.z = NodeD_start.z + dZVec[2];

	return TRUE;
}

int CSectForResultantForceDlg::GetSelectedElemKeyList(CArray<UINT,UINT>& raElemK_sel)
{
	// select elem
	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	pIGM->GetSelectedElemKeyList(raElemK_sel);

	CLdfsUtil utilLDFS;
	utilLDFS.FilterByCanElemType(raElemK_sel);

	return raElemK_sel.GetSize();
}

int CSectForResultantForceDlg::GetSelectedNodeKeyList(CArray<UINT,UINT>& raNodeK_sel)
{
	// select node
	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	pIGM->GetSelectedNodeKeyList(raNodeK_sel);

	return raNodeK_sel.GetSize();
}

int CSectForResultantForceDlg::GetSelectedGrupElemKeyList(CArray<UINT,UINT>& raElemK_sel)
{
	UpdateData(TRUE); 

	T_GRUP_K GrupK = 0;
	T_GRUP_D GrupD;
	if (!m_wndGrupCobx.GetSelectedGrup(GrupK))  { ASSERT(0);  return 0; }
	if (!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD))  { ASSERT(0);  return 0; }

	raElemK_sel.Copy(GrupD.arKeyElem);
	return raElemK_sel.GetSize();
}

CString CSectForResultantForceDlg::GetSelectedGrupName()
{
	CString strGrupName = _T("");
	T_GRUP_K GrupK = 0;
	T_GRUP_D GrupD;
	if (!m_wndGrupCobx.GetSelectedGrup(GrupK))  { ASSERT(0);  return strGrupName; }
	if (!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD))  { ASSERT(0);  return strGrupName; }

	strGrupName = GrupD.GroupName;
	return strGrupName;
}

void CSectForResultantForceDlg::CalcCentroidAuto_PolyGon()
{
	int nCountCoods = m_arXYZ[0].GetSize();
	if (nCountCoods < 3) return;

	// Get z-Vector
	double dZVec[3] = {0.0, };
	dZVec[0]=m_dZVec[0];    dZVec[1]=m_dZVec[1];    dZVec[2]=m_dZVec[2];

	// Get Selected Elem list
	CArray<T_ELEM_K,T_ELEM_K> aElemK_sel;   aElemK_sel.RemoveAll();
	int nElem = GetCuttingElemList(aElemK_sel);

	// select elem
	{
		I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
		pIGM->UnselectAllElem();

		pIGM->AddSelectedElemKeyList(aElemK_sel);
	}

	// get start/end position
	T_NODE_D NodeD_start;
	T_NODE_D NodeD_end;
	T_POINT3D pt3DOnCuttingPlane;
	if (!GetStartEndPosition(NodeD_start, NodeD_end, pt3DOnCuttingPlane))  { ASSERT(0);  return /*FALSE*/; }

	CLdfsUtil utilLdfs;

	// get nomal vector (by 2D)
	T_POINT3D pt3D_normalVec;   pt3D_normalVec.Set(0.0, 0.0, 0.0);
	if (!utilLdfs.GetNormal2D(pt3D_normalVec, NodeD_start, NodeD_end))  { ASSERT(0);  return /*FALSE*/; }

	// Get Cutting Line
	double dCuttingScale = 100000;
	T_POINT2D pt2D_CutP1;  pt2D_CutP1.Set(0.0, 0.0);    // cutting line (p1)
	pt2D_CutP1.x = pt3DOnCuttingPlane.x + (-1.0)*dCuttingScale * pt3D_normalVec.x;
	pt2D_CutP1.y = pt3DOnCuttingPlane.y + (-1.0)*dCuttingScale * pt3D_normalVec.y;
	T_POINT2D pt2D_CutP2;  pt2D_CutP2.Set(0.0, 0.0);    // cutting line (p2)
	pt2D_CutP2.x = pt3DOnCuttingPlane.x + dCuttingScale * pt3D_normalVec.x;
	pt2D_CutP2.y = pt3DOnCuttingPlane.y + dCuttingScale * pt3D_normalVec.y;

	// Calc Centroid
	// 합성 단면의 도심 (Global 기준)
	T_POINT3D ptCentG;    ptCentG.Set(0.0, 0.0, 0.0);
	double dArea_sum = 0.0;
	const double dTol = 1.0e-9;
	CArray<T_CENTROID_BASE,T_CENTROID_BASE&> aCentBase;   

	// 가상보의 각 단면에 대한 loop
	//for (int iDir=1; iDir>=0; --iDir)   // i + j 방향 두번을 찾음
	int iDir=1;
	{
		aCentBase.RemoveAll();

		// get centroid info
		// int nModeTol : Tolerance 비교를 각도로 할지, 거리로할지 (=0: 거리, =1: 각도) 
		// Polygon 타입은 단면정의라서 거리가 좋고, Elem 타입은 단면이 없기 때문에 각도가 좋음
		int nModeTol = 0;
		if (!utilLdfs.GetCentroid_Composite(aCentBase, ptCentG, dArea_sum, aElemK_sel, pt2D_CutP1, pt2D_CutP2, iDir, dTol, nModeTol, dZVec)) { ASSERT(0);  return /*FALSE*/; }
		//PrintCentroid_Composite(aCentBase, ptCentG, dArea_sum, iDir);   // print centroid for debug

		CMapEx<UINT,UINT, int,int> mNodeK_bound;     mNodeK_bound.RemoveAll();      mNodeK_bound.InitHashTable(17);   // elem boundary로 사용할 node
		CMapEx<UINT,UINT, int,int> mElemK_bound;     mElemK_bound.RemoveAll();      mElemK_bound.InitHashTable(97);   // elem boundary로 사용할 elem
		CMapEx<UINT,UINT, int,int> mElemK_Exclude;   mElemK_Exclude.RemoveAll();    mElemK_Exclude.InitHashTable(97); // Boundary 제외 요소

		// Get Boundary elem/node info of virtual section
		for (int k=0; k<aCentBase.GetSize(); ++k) 
		{ 
			// 각 요소를 대상으로 boundary elem/node 정보를 파악
			if (!utilLdfs.GetBoundaryInfo(aCentBase[k].ElemK, mNodeK_bound, mElemK_bound, mElemK_Exclude, pt2D_CutP1, pt2D_CutP2, iDir, dTol, nModeTol)) continue;
		}

		// Boundary 제외 요소 적용.
		T_ELEM_K ElemK_exclude = 0;
		int nDummy = 0;
		POSITION pos = mElemK_Exclude.GetStartPosition();
		while(pos)
		{
			mElemK_Exclude.GetNextAssoc(pos, ElemK_exclude, nDummy);
			mElemK_bound.RemoveKey(ElemK_exclude);
		}
		CArray<T_ELEM_K,T_ELEM_K> aElemK_bound;  aElemK_bound.RemoveAll();      mElemK_bound.GetSortedKeyListforUintK(aElemK_bound);
		CArray<T_NODE_K,T_NODE_K> aNodeK_bound;  aNodeK_bound.RemoveAll();      mNodeK_bound.GetSortedKeyListforUintK(aNodeK_bound);
	}
}

void CSectForResultantForceDlg::SetZVec(double dX, double dY, double dZ)
{
	// set data
	m_dZVec[0]=dX;
	m_dZVec[1]=dY;
	m_dZVec[2]=dZ;
	CMathFunc::mathNormalize(m_dZVec, m_dZVec);

	// update display
	CString strVec;   strVec.Format(_T("%g, %g, %g"), m_dZVec[0]+0.0, m_dZVec[1]+0.0, m_dZVec[2]+0.0);
	m_edtZVec.MySetWindowText(strVec);
}

void CSectForResultantForceDlg::OnSelchangeTmGrupCmb() 
{
//   UpdateData(TRUE); 
// 
//   T_GRUP_K GrupK = 0;
//   T_GRUP_D GrupD;
//   if (!m_wndGrupCobx.GetSelectedGrup(GrupK))  { ASSERT(0);  return; }
//   if (!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD))  { ASSERT(0);  return; }
// 
//   CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
//   arKeyElem.Copy(GrupD.arKeyElem);
//   ASSERT(0);
}

LRESULT CSectForResultantForceDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	//Dialog Size변경
	CRect rectParent;
	GetWindowRect(rectParent);
	((CStructElementDlg*)(GetParent()->GetParent()))->SetHolderSize(rectParent.Height());

	return 0L;
}

void CSectForResultantForceDlg::ConvertToVsec()
{
	CAttrCtrl2* pAttrCtrl2 = m_pDoc->m_pAttrCtrl2;
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;

	// 1. Get all existing VBEM keys
	CArray<T_VBEM_K, T_VBEM_K> aVbemK;
	aVbemK.RemoveAll();
	pAttrCtrl2->GetVbemKeyList(aVbemK);
	CArray<T_SECT_D, T_SECT_D&> arrSectT2;
	arrSectT2.RemoveAll();
	CArray<T_SECV_D, T_SECV_D&> arrSectVDT2;
	arrSectVDT2.RemoveAll();
	for (int i = 0; i < aVbemK.GetSize(); ++i)
	{
		T_VBEM_K vbemKey = aVbemK[i];

		// ----- Get VBEM data -----
		T_VBEM_D VbemD;
		if (!pAttrCtrl2->GetVbem(vbemKey, VbemD))
			continue;

		// ----- Get related VSEC keys -----
		T_VSEC_D VsecD1, VsecD2;

		if (!pAttrCtrl2->GetVsec(VbemD.Vsec1, VsecD1)) continue;
		if (!pAttrCtrl2->GetVsec(VbemD.Vsec2, VsecD2)) continue;

		// ----- Collect Elements and Nodes from both VSECs -----
		CMapEx<T_ELEM_K, T_ELEM_K, int, int> mElemK;
		mElemK.RemoveAll();
		mElemK.InitHashTable(997);

		CMapEx<T_NODE_K, T_NODE_K, int, int> mNodeK;
		mNodeK.RemoveAll();
		mNodeK.InitHashTable(997);

		for (int k = 0; k < VsecD1.arKeyElem.GetSize(); ++k)
			mElemK.SetAt(VsecD1.arKeyElem[k], 0);

		for (int k = 0; k < VsecD2.arKeyElem.GetSize(); ++k)
			mElemK.SetAt(VsecD2.arKeyElem[k], 0);

		for (int k = 0; k < VsecD1.arKeyNode.GetSize(); ++k)
			mNodeK.SetAt(VsecD1.arKeyNode[k], 0);

		for (int k = 0; k < VsecD2.arKeyNode.GetSize(); ++k)
			mNodeK.SetAt(VsecD2.arKeyNode[k], 0);

		// ----- Separate slab / girder -----
		CArray<T_ELEM_K, T_ELEM_K> arSlabElem, arGirderElem;
		CArray<T_NODE_K, T_NODE_K> arSlabNode, arGirderNode;

		POSITION pos = mElemK.GetStartPosition();
		T_ELEM_K elemK;
		int dummy;

		T_ELEM_D ElemD;
		T_THIK_D Sectdata;
		ElemD.Initialize();
		Sectdata.Initialize();
		T_SECT_K SectK = 0;
		T_THIK_D Thickness;
		double tc = 0;

		while (pos)
		{
			mElemK.GetNextAssoc(pos, elemK, dummy);

			if (!pAttrCtrl->GetElem(elemK, ElemD))
				continue;

			
			if (ElemD.eltyp == PLATE_EL)
			{
				arSlabElem.Add(elemK);
				pAttrCtrl->GetThik(ElemD.elpro, Thickness);
				tc = Thickness.Value.ThickIn;
				for (int n = 0; n < 8; ++n)
					if (ElemD.elnod[n] != 0)
						arSlabNode.Add(ElemD.elnod[n]);
			}
			else if (ElemD.eltyp == BEAM_EL)
			{
				arGirderElem.Add(elemK);
				SectK = ElemD.elpro;
				for (int n = 0; n < 8; ++n)
					if (ElemD.elnod[n] != 0)
						arGirderNode.Add(ElemD.elnod[n]);
			}
		}

		// ----- Create Section Data Exactly Like InsertVsecVbem -----
		T_SECT_D SectT2;
		if (!VirtualSection_PSC_I(
			SectK,
			arSlabNode,
			arGirderNode,
			arSlabElem,
			arGirderElem,
			VsecD1,
			SectT2,
			tc))
		{
			TRACE(_T("VirtualSection_PSC_I failed for SectK = %d\n"), SectK);

			continue;
		}
		T_SECV_D SecvD;
		SecvD.ConvertToSecv(SectT2);

		arrSectVDT2.Add(SecvD);
	}
	CArray<T_VBEM_K, T_VBEM_K> aExistingVbemK;
	aExistingVbemK.RemoveAll();
	CString strCmd = _T("Convert To Virtual Section");
	m_pDoc->StartEditDB(strCmd, 0, TRUE);
	m_pDoc->m_pAttrCtrl2->GetVbemKeyList(aExistingVbemK);

	std::map<int, std::vector<T_ELEM_K>> mapVbemToGirderElem;
	for (int i = 0; i < aExistingVbemK.GetSize(); ++i)
	{
		T_VBEM_K VbemK = aExistingVbemK[i];
		T_SECV_K SecvK = VbemK;

		T_SECV_D SecvD = arrSectVDT2[i];

		CString strPostFix0;
		strPostFix0.Format(_T("VBc_D%d"), i);

		SecvD.SName = strPostFix0;

		CString strPostFix;
		strPostFix.Format(_T("_%d"), SecvK);

		SecvD.SName += strPostFix;

		if (!m_pDoc->m_pEditData->AddSecv(SecvK, SecvD))
		{
			ASSERT(0);
			//return FALSE;
		}

		T_SEVO_D OData = SecvD;

		if (!m_pDoc->m_pEditData->AddSevo(SecvK, OData))
		{
			ASSERT(0);
			//return FALSE;
		}
		/*auto it = mapVbemToGirderElem.find(VbemK);
		if (it != mapVbemToGirderElem.end())
		{
			m_mapSecvToElem[SecvK].insert(
				m_mapSecvToElem[SecvK].end(),
				it->second.begin(),
				it->second.end()
			);
		}*/
	}
	m_pDoc->CloseEditDB();
}

BOOL CSectForResultantForceDlg::VirtualSection_PSC_I(T_SECT_K SectK,
	CArray<T_NODE_K, T_NODE_K>& arSlabNode, CArray<T_NODE_K, T_NODE_K>& arGirderNode,
	CArray<T_ELEM_K, T_ELEM_K>& arSlabElem, CArray<T_ELEM_K, T_ELEM_K>& arGirderElem,
	T_VSEC_D& VsecD, T_SECT_D& SectT2,double tc)
{
	//if (arSlabNode.GetSize() == 0) { ASSERT(0); return FALSE; }
	if (arGirderNode.GetSize() == 0) { ASSERT(0); return FALSE; }
	//if (arSlabElem.GetSize() == 0) { ASSERT(0); return FALSE; }
	if (arGirderElem.GetSize() == 0) { ASSERT(0); return FALSE; }

	T_SECT_D SectT1;
	if (!m_pDoc->m_pAttrCtrl->GetSect(SectK, SectT1)) {}//ASSERT(0); // Type 1

	BOOL bTapered = FALSE;
	BOOL bSection = FALSE;

	if (SectT1.nStype == D_SECT_TYPE_TAPERED && SectT1.SectBefore.nStype == D_SECT_TYPE_COMPO_CI)
	{ /*OK*/
		bTapered = TRUE;
	}
	else if (SectT1.nStype == D_SECT_TYPE_COMPO_CI)
	{ /*OK*/
	}
	else if (SectT1.nStype == D_SECT_TYPE_PSC)
	{ /*OK*/
	}
	else
	{
		//ASSERT(0);
		return FALSE;
	}

	if (bTapered) return FALSE;

	SectT2.Initialize();

	SectT2 = SectT1;
	SectT2.nStype = D_SECT_TYPE_COMPO_CI;
	SectT2.SectBefore.Shape = D_SECT_SHAPE_COMPO_CI;
	SectT2.SectBefore.nStype = D_SECT_TYPE_COMPO_CI;

	// Get Matl Data
	T_ELEM_D ElemS; ElemS.Initialize();
	T_ELEM_D ElemG; ElemG.Initialize();
	if (arSlabElem.GetSize() > 0) {
		if (!m_pDoc->m_pAttrCtrl->GetElem(arSlabElem[0], ElemS)) {
			ASSERT(0); return FALSE;
		}
	}
	if (!m_pDoc->m_pAttrCtrl->GetElem(arGirderElem[0], ElemG)) {
		ASSERT(0); return FALSE;
	}

	T_MATL_D MatlS;  MatlS.Initialize();
	T_MATL_D MatlG;  MatlG.Initialize();
	if (arSlabElem.GetSize() > 0) {
		if (!m_pDoc->m_pAttrCtrl->GetMatl(ElemS.elmat, MatlS)) {
			ASSERT(0); return FALSE;
		}
	}
	if (!m_pDoc->m_pAttrCtrl->GetMatl(ElemG.elmat, MatlG)) {
		ASSERT(0); return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	T_NODE_D NodeD;
	double dMaxS = -DBL_MAX;
	double dMinS = DBL_MAX;
	double hMaxS = -DBL_MAX;
	double hMinS = DBL_MAX;
	if (arSlabNode.GetSize() > 0) {
		for (int i = 0; i < arSlabNode.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetNode(arSlabNode[i], NodeD);

			if (dMaxS < NodeD.y) dMaxS = NodeD.y;
			if (dMinS > NodeD.y) dMinS = NodeD.y;
			if (hMaxS < NodeD.z) hMaxS = NodeD.z;
			if (hMinS > NodeD.z) hMinS = NodeD.z;
		}
	}
	T_NODE_D NodeG;
	double dMaxG = -DBL_MAX; // Left to Pos
	double dMinG = DBL_MAX;
	double hMaxG = -DBL_MAX; // Left to Pos
	double hMinG = DBL_MAX;
	double dCentY;
	for (int i = 0; i < arGirderNode.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetNode(arGirderNode[i], NodeG);

		if (dMaxG < NodeG.y) dMaxG = NodeG.y;
		if (dMinG > NodeG.y) dMinG = NodeG.y;
		if (hMaxG < NodeG.z) hMaxG = NodeG.z;
		if (hMinG > NodeG.z) hMinG = NodeG.z;
	}

		dCentY = (dMaxG + dMinG) / 2.0;
	//////////////////////////////////////////////////////////////////////////
	if (tc == 0) {
		SectT2.SectAfter.SectJ.Size[0] = SectT1.SectAfter.SectJ.Size[0];
		SectT2.SectAfter.SectJ.Size[1] = SectT1.SectAfter.SectJ.Size[1];
		SectT2.SectAfter.SectJ.Size[2] = SectT1.SectAfter.SectJ.Size[2];
	}
	else {
		SectT2.SectAfter.SectJ.Size[0] = dMaxS - dMinS;
		SectT2.SectAfter.SectJ.Size[1] = tc;
		SectT2.SectAfter.SectJ.Size[2] = hMaxS - hMaxG - tc / 2;
	}
	double Bc = SectT2.SectAfter.SectJ.Size[0];

	double dTop, dBot, dSg; // 계산되어 나오는 변수
	//CSectUtil::CompStlGSymmetricSectAutoCalc(D_SECT_TYPE_COMPO_CI, Bc, B1, B2, B3, B4, 0.0, tw, t1, FALSE, dSg, dTop, dBot, SectT2);
	//double orgdis = (Bc - (dB1 + dB2)) / 2;
	
	if (tc == 0 ) {
		dSg = SectT1.SectAfter.SectJ.Size[3];
	}
	else {
		dSg = dMaxS - (dMaxG + Bc / 2);
	}

	SectT2.SectAfter.SectJ.Size[3] = dSg;

	CSectUtil::CalcSectData(SectT2);

	return TRUE;
}