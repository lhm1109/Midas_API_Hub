// StldFbldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldFbldDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldFbldDlg dialog


CStldFbldDlg::CStldFbldDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldFbldDlg::IDD, pParent)
{
	m_pBitmap = 0;
	//{{AFX_DATA_INIT(CStldFbldDlg)
	m_csAngleA1 = _T("");
	m_csSubA2 = _T("");
	m_nProj = -1;
	m_csDescription = _T("");
	m_bCopyFloorLoad = FALSE;
	m_nAxis = -1;
	m_bConvertToBeamLoad = FALSE;
	//}}AFX_DATA_INIT
	m_ctrlLoadAngleA1.Add(IDC_TM_FBL_ANGLE_TITLE);
	m_ctrlLoadAngleA1.Add(IDC_TM_FBL_ANGLE_A1);
	m_ctrlLoadAngleA1.Add(IDC_TM_FBL_ANGLE_A1_UNIT);

	m_ctrlProjection.Add(IDC_TM_FBL_PROJ_TITLE);
	m_ctrlProjection.Add(IDC_TM_FBL_PROJ_YES);
	m_ctrlProjection.Add(IDC_TM_FBL_PROJ_NO);

	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_AXIS_TITLE);
	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_AXIS_X);
	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_AXIS_Y);
	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_AXIS_Z);
	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_DIST_TITLE);
	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_DIST_EDIT);
	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_DIST_UNIT);
	m_ctrlCopyFloorLoad.Add(IDC_TM_FBL_DIST_EXAM);

	m_ctrlLoadPolygon.Add(IDC_TM_FBL_SUB_NO);
	//m_ctrlLoadPolygon.Add(IDC_TM_FBL_SUB_NO_SPIN);
	m_ctrlLoadPolygon.Add(IDC_TM_FBL_SUB_A2);
	m_ctrlLoadPolygon.Add(IDC_TM_FBL_USW_EDIT);
}

CStldFbldDlg::~CStldFbldDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CStldFbldDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldFbldDlg)
	DDX_Control(pDX, IDC_TM_FBL_SUB_NO, m_wndSubNo);
	DDX_Control(pDX, IDC_TM_FBL_SUB_A2, m_wndSubA2);
	DDX_Control(pDX, IDC_TM_FBL_ANGLE_A1, m_wndAngleA1);
	DDX_Control(pDX, IDC_TM_FBL_DIST_EDIT, m_wndDist);
	DDX_Control(pDX, IDC_TM_FBL_USW_EDIT, m_wndUSW);
	DDX_Control(pDX, IDC_TM_FBL_NODE_LIST, m_wndNodeList);
	DDX_Control(pDX, IDC_TM_FBL_LOADTYPE, m_wndLoadType);
	DDX_Control(pDX, IDC_TM_FBL_DIST_UNIT, m_wndDistUnit);
	DDX_Control(pDX, IDC_TM_FBL_USW_UNIT, m_wndUSWUnit);
	DDX_Control(pDX, IDC_TM_FBL_ANGLE_A1_UNIT, m_wndAngleA1Unit);
	DDX_Control(pDX, IDC_TM_FBL_LOAD_DIR, m_wndLoadDir);
	DDX_Control(pDX, IDC_TM_FBL_DIST_TYPE, m_wndDistType);
	DDX_Text(pDX, IDC_TM_FBL_ANGLE_A1, m_csAngleA1);
	DDX_Text(pDX, IDC_TM_FBL_SUB_A2, m_csSubA2);
	DDX_Radio(pDX, IDC_TM_FBL_PROJ_YES, m_nProj);
	DDX_Text(pDX, IDC_TM_FBL_DESCRIPTION, m_csDescription);
	DDX_Check(pDX, IDC_TM_FBL_COPY_FBLD, m_bCopyFloorLoad);
	DDX_Radio(pDX, IDC_TM_FBL_AXIS_X, m_nAxis);
	DDX_Check(pDX, IDC_TM_FBL_CONVERT, m_bConvertToBeamLoad);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_TM_CHK_EXCLUDE, m_chkExclude);
	DDX_Control(pDX, IDC_TM_CHK_ALLOW_POLYGON, m_chkAllow);
	DDX_Control(pDX, IDC_TM_FBL_PICTURE, m_wndFbldTypePicture);
	//}}AFX_DATA_MAP
}

BOOL CStldFbldDlg::GetParameter(T_FBLA_D &data, BOOL &bChangeBeamLoad, BOOL &bCopyLoad, 
																double &ux, double &uy, double &uz, CArray<double, double> &aDist)
{
	if (!UpdateData(TRUE)) return FALSE;
	data.Initialize();
	if (!m_wndLoadType.GetSelectedFbld(data.LoadTypeKey))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Load_type_is_not_selected_));
		return FALSE;
	}
	data.nDistributionType = m_wndDistType.GetCurSel()+1;
  if (data.nDistributionType == D_FBLTYPE_ONEWAY)  // one way
	{
		if (!GetFloatNumber(m_csAngleA1, data.LoadAngle))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_load_angle_));
			return FALSE;
		}
	}
	data.bExcludeInnerElemArea = m_chkExclude.GetCheck() != 0;
	data.bAllowPolygonTypeUnitArea = m_chkAllow.GetCheck() != 0;
	if (!m_wndSubNo.GetEditValue(data.nSubBeam))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_number_of_sub_beam_));
		return FALSE;
	}
	if (!GetFloatNumber(m_csSubA2, data.SubBeamAngle))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalud_sub_beam_angle_));
		return FALSE;
	}
	// *^^* SubBeam 갯수가 0일 때는 Sub Beam Angle 을 0으로 한다.
	if (data.nSubBeam == 0) data.SubBeamAngle = 0.0;

	data.SubBeamUnitWeight = m_wndUSW.GetEditValue();
	data.LoadDirection = m_wndLoadDir.GetCurSel()+1;
	if (data.LoadDirection >= 4)  // GX, GY, GZ
		data.Projection = (m_nProj == 0) ? 1 : 0;
	data.Description = m_csDescription;

	CArray<UINT, UINT> aNodeList;
	CString csNodeList;
	int nSize;
	m_wndNodeList.GetWindowText(csNodeList);
	if (!GetNodeList(csNodeList, aNodeList))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
		return FALSE;
	}
	nSize = aNodeList.GetSize();
	if (nSize <= 2)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Not_enough_number_of_nodes_));
		return FALSE;
	}
	for (int i = 0; i < nSize; i++)
		data.nConnectivity[i] = aNodeList[i];

	bCopyLoad = m_bCopyFloorLoad;
	if (bCopyLoad)
	{
		double UX, UY, UZ;
		UX = 0.0;
		UY = 0.0;
		UZ = 0.0;

		switch(m_nAxis)
		{
		case 0: UX = 1.0; break; // x Axis...
		case 1: UY = 1.0; break; // y Axis...
		case 2: UZ = 1.0; break; // z Axis...
		}
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		pIGM->GetWcsVector(UX, UY, UZ, ux, uy, uz);

		CString csDist;
		m_wndDist.GetWindowText(csDist);
		if (!GetUEDistance(csDist, aDist))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_values_));
			return FALSE;
		}
	}
	bChangeBeamLoad = m_bConvertToBeamLoad;
	return TRUE;
}

void CStldFbldDlg::ChangeBitmap(int nBitmap)
{
	// Bitmap을 Oneway, Twoway로 바꾼다.
#if 0
	UINT aBitmapID[] = { IDB_TM_FBL_ONEWAY, IDB_TM_FBL_TWOWAY, IDB_TM_FBL_POLYCENT, IDB_TM_FBL_POLYLENG};

	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* FblImage = (CBCGPStatic*)GetDlgItem(IDC_TM_FBL_PICTURE);
	m_pBitmap = (CBitmap*)FblImage;
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nBitmap]);
	FblImage->SetBitmap(HBITMAP(*m_pBitmap));
#else
	//IDB_TM_FBL_ONEWAY       BITMAP                  "res\\tm_fbl_o.bmp"
	//IDB_TM_FBL_TWOWAY       BITMAP                  "res\\tm_fbl_t.bmp"
	//IDB_TM_FBL_POLYCENT     BITMAP                  "res\\tm_fbl_polycent.bmp"
	//IDB_TM_FBL_POLYLENG     BITMAP                  "res\\tm_fbl_polyleng.bmp"
	CString aFbldTypes[] = {
		_T("SVG\\illustration\\Dialog\\tm_fbl_o.svg"),
		_T("SVG\\illustration\\Dialog\\tm_fbl_t.svg"),
		_T("SVG\\illustration\\Dialog\\tm_fbl_polycent.svg"),
		_T("SVG\\illustration\\Dialog\\tm_fbl_polyleng.svg")
	};
	m_wndFbldTypePicture.SetImage(aFbldTypes[nBitmap]);
#endif
}

void CStldFbldDlg::Execute()
{
	OnTmExecute();
}

void CStldFbldDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_wndDistType.GetDC();
	for (int i=0;i < m_wndDistType.GetCount();i++)
	{
		m_wndDistType.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_wndDistType.ReleaseDC(pDC);

	//dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_wndDistType.GetDroppedWidth() < dx)
	{
		m_wndDistType.SetDroppedWidth(dx);
		ASSERT(m_wndDistType.GetDroppedWidth() == dx);
	}
}

BEGIN_MESSAGE_MAP(CStldFbldDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldFbldDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_CBN_SELCHANGE(IDC_TM_FBL_DIST_TYPE, OnSelchangeTmFblDistType)
	ON_CBN_SELCHANGE(IDC_TM_FBL_LOAD_DIR, OnSelchangeTmFblLoadDir)
	ON_BN_CLICKED(IDC_TM_FBL_COPY_FBLD, OnTmFblCopyFbld)
	ON_BN_CLICKED(IDC_TM_FBL_LOADTYPE_BTN, OnTmFblLoadtypeBtn)
	ON_EN_SETFOCUS(IDC_TM_FBL_NODE_LIST, OnSetfocusTmFblNodeList)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_FBL_SUB_NO_SPIN, OnDeltaposTmFblSubNoSpin)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnMouseEditEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldFbldDlg message handlers

BOOL CStldFbldDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	m_wndDistType.AddString(_LS(IDS_WG_TREEMENU_One_way));
	m_wndDistType.AddString(_LS(IDS_WG_TREEMENU_Two_way));
	m_wndDistType.AddString(_LS(IDS_WG_TREEMENU_Poly_Cent));
	m_wndDistType.AddString(_LS(IDS_WG_TREEMENU_Poly_Leng));
	AdjustComboListBox();
 
	m_wndLoadDir.AddString(_LS(IDS_WG_TREEMENU_Local_x));
	m_wndLoadDir.AddString(_LS(IDS_WG_TREEMENU_Local_y));
	m_wndLoadDir.AddString(_LS(IDS_WG_TREEMENU_Local_z));
	m_wndLoadDir.AddString(_LS(IDS_WG_TREEMENU_Global_X));
	m_wndLoadDir.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
	m_wndLoadDir.AddString(_LS(IDS_WG_TREEMENU_Global_Z));

	// set unit type
	m_wndAngleA1Unit.SetUnitType(CUnitCtrl::m_FBLA_UNIT.LoadAngle);
	m_wndUSW.SetUnitType(CUnitCtrl::m_FBLA_UNIT.SubBeamUnitWeight);
	m_wndUSWUnit.SetUnitType(CUnitCtrl::m_FBLA_UNIT.SubBeamUnitWeight);
	//m_wndDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	// Angle A1 and Sub A2
	m_wndAngleA1.AddString(_T("0"));
	m_wndAngleA1.AddString(_LS(IDS_WG_TREEMENU_90));
	m_wndSubA2.AddString(_T("0"));
	m_wndSubA2.AddString(_LS(IDS_WG_TREEMENU_90));

	// set default value
	m_wndDistType.SetCurSel(1); // Two way
	m_csAngleA1 = _T("0");
	m_wndSubNo.SetRange(0, SHRT_MAX);
	m_wndSubNo.SetValue(0);
	m_wndSubNo.SetInteger(TRUE);
	m_csSubA2 = _LS(IDS_WG_TREEMENU_90);
	m_wndUSW.SetWindowText(_T("0"));
	m_wndLoadDir.SetCurSel(5);  // Global Z
	m_nProj = 1;  // no
	m_csDescription = _T("");
	m_bCopyFloorLoad = FALSE;
	m_nAxis = 2;  // z
	m_wndDist.SetWindowText(_T("0"));
	m_bConvertToBeamLoad = FALSE;

	m_chkExclude.SetCheck(FALSE);
	m_chkAllow.SetCheck(FALSE);

	m_wndNodeList.SetAttNodeList();
	m_wndNodeList.SetLButtonDownNotifyWindow(this);
	m_wndNodeList.SetEnterNotifyWindow(this);
	m_wndNodeList.SetEscNotifyWindow(this);
	m_wndNodeList.SetModeToUse(MOUSEEDIT_USE_GET_FLOAD_NODE_LIST);
	m_wndNodeList.SetMaxNodeKeyNum(D_FBLA_MAXNOD);

	BOOL bEnable;
	bEnable = m_wndDistType.GetCurSel() == 0;
	CtrlEnableDisable(m_ctrlLoadAngleA1, bEnable);
	bEnable = (m_wndDistType.GetCurSel() < 2);
	CtrlEnableDisable(m_ctrlLoadPolygon, bEnable);
	ChangeBitmap(m_wndDistType.GetCurSel());
	bEnable = m_wndDistType.GetCurSel() == 0 || m_wndDistType.GetCurSel() == 1;
	m_chkExclude.EnableWindow(bEnable);
	bEnable = m_wndDistType.GetCurSel() == 1;
	m_chkAllow.EnableWindow(bEnable);

	bEnable = m_wndLoadDir.GetCurSel() >= 3;  // if GX, GY, GZ then true
	CtrlEnableDisable(m_ctrlProjection, bEnable);

	bEnable = m_bCopyFloorLoad;
	CtrlEnableDisable(m_ctrlCopyFloorLoad, bEnable);

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);


	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStldFbldDlg::Do()
{
	T_FBLA_D data;
	BOOL bChangeBeamLoad, bCopyLoad;
	double ux, uy, uz;
	ux = 0.0;
	uy = 0.0;
	uz = 0.0;   // Initialize. added by bugboy
	CArray<double, double> aDist;
	if (!GetParameter(data, bChangeBeamLoad, bCopyLoad, ux, uy, uz, aDist)) return FALSE;

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	data.GroupKey = LdgrK;

	if(m_pDoc->m_pDataCtrl->AddFbla(data, bChangeBeamLoad, bCopyLoad, ux, uy, uz, aDist))
	{
		return TRUE;
	}
	return FALSE;
}

void CStldFbldDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	Do();
}

void CStldFbldDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldFbldDlg::OnSelchangeTmFblDistType() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_ctrlLoadAngleA1, m_wndDistType.GetCurSel() == 0);
	CtrlEnableDisable(m_ctrlLoadPolygon, m_wndDistType.GetCurSel() < 2);

	if (m_wndDistType.GetCurSel() == 0 || m_wndDistType.GetCurSel() == 1)
		m_chkExclude.EnableWindow(TRUE);
	else
	{
		m_chkExclude.EnableWindow(FALSE);
		m_chkExclude.SetCheck(FALSE);
	}
	if (m_wndDistType.GetCurSel() == 1)
		m_chkAllow.EnableWindow(TRUE);
	else 
	{
		m_chkAllow.EnableWindow(FALSE);
		m_chkAllow.SetCheck(FALSE);
	}

	ChangeBitmap(m_wndDistType.GetCurSel());
}

void CStldFbldDlg::OnSelchangeTmFblLoadDir() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_ctrlProjection, m_wndLoadDir.GetCurSel() >= 3);
}

void CStldFbldDlg::OnTmFblCopyFbld() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_TM_FBL_COPY_FBLD);
	m_bCopyFloorLoad = pButton->GetCheck();
	CtrlEnableDisable(m_ctrlCopyFloorLoad, m_bCopyFloorLoad);
}

void CStldFbldDlg::OnTmFblLoadtypeBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_BLOAD_DEFFBL, 0));
}

LRESULT CStldFbldDlg::OnMouseEditEscape(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(pIGM)
	{
		pIGM->EndTempFLoad();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		m_wndNodeList.ClearNodeKeyList();
		pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
	}
	return 0L;
}

LRESULT CStldFbldDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(pIGM)
	{
		BOOL bRetVal = Do();
		pIGM->EndTempFLoad();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		//if(bRetVal)
		//{
		pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
		m_wndNodeList.ClearNodeKeyList();
		//}
	}

	return 0L;
}

LRESULT CStldFbldDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	if(pIGM)
	{
		if(pIGM->IsCompleteTempFLoad())
		{
			BOOL bRetVal = Do();
			pIGM->EndTempFLoad();
			m_wndNodeList.ClearNodeKeyList();
			pIGM->SetModelingModeST(IG_STATE_INITALL);
			//if(bRetVal)
			pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
			
		}
	}
	return 0L;
}

void CStldFbldDlg::OnSetfocusTmFblNodeList() 
{
	// TODO: Add your control notification handler code here
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(pIGM)
	{
		pIGM->EndTempFLoad();
		m_wndNodeList.ClearNodeKeyList();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		//if(bRetVal)
		pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
	}
}

void CStldFbldDlg::OnDeltaposTmFblSubNoSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nVal;
	CString csVal;

	m_wndSubNo.GetEditValue(nVal);
	nVal -= pNMUpDown->iDelta;
	if(nVal < 0) nVal = 0;
	
	csVal.Format(_LS(IDS_WG_TREEMENU__d),nVal);
	
	m_wndSubNo.SetWindowText(csVal);
	*pResult = 0;
}

void CStldFbldDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}


