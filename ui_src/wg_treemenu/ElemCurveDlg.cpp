// ElemCurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemCurveDlg.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_dbRes2.h"
#include "..\wg_base\I_GENStateDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemCurveDlg dialog

CElemCurveDlg::CElemCurveDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemCurveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemCurveDlg)
	m_bIntersectNode = TRUE;
	m_bIntersectElem = TRUE;
	m_nDivOpt = 0;
	m_nArcEndPoint = 0;
	m_nElementType = 1;
	//}}AFX_DATA_INIT
	m_bPropInit = FALSE;

	// 컨트롤 배치 준비
	aCtrlIDAll.Add(IDC_TM_ENDOPT_STATIC);
	aCtrlIDAll.Add(IDC_TM_ENDOPT_P3_RADIO);
	aCtrlIDAll.Add(IDC_TM_ENDOPT_ANGLE_RADIO);
	aCtrlIDAll.Add(IDC_TM_ENDOPT_ANGLE_CBO);
	aCtrlIDAll.Add(IDC_TM_ANGLE_UNIT);
	aCtrlIDAll.Add(IDC_TM_ELEMCURVE_SEMIELLIPSE_CHK);
	aCtrlIDAll.Add(IDC_TM_ELEMCURVE_DIVIDE);
	aCtrlIDAll.Add(IDC_TM_ELEMCURVE_DIVIDE_RDO1);
	aCtrlIDAll.Add(IDC_TM_ELEMCURVE_DIVIDE_RDO2);
	aCtrlIDAll.Add(IDC_TM_POINT_STATIC1);
	aCtrlIDAll.Add(IDC_TM_POINT_STATIC2);
	aCtrlIDAll.Add(IDC_TM_POINT_STATIC3);
	aCtrlIDAll.Add(IDC_TM_POINT_STATIC4);
	aCtrlIDAll.Add(IDC_TM_POINT_EDIT1);
	aCtrlIDAll.Add(IDC_TM_POINT_EDIT2);
	aCtrlIDAll.Add(IDC_TM_POINT_EDIT3);
	aCtrlIDAll.Add(IDC_TM_POINT_EDIT4);
	aCtrlIDAll.Add(IDC_TM_POINT_UNIT1);
	aCtrlIDAll.Add(IDC_TM_POINT_UNIT2);
	aCtrlIDAll.Add(IDC_TM_POINT_UNIT3);
	aCtrlIDAll.Add(IDC_TM_POINT_UNIT4);
	
	aCtrlID12.Add(IDC_TM_ENDOPT_STATIC);
	aCtrlID12.Add(IDC_TM_ENDOPT_P3_RADIO);
	aCtrlID12.Add(IDC_TM_ENDOPT_ANGLE_RADIO);
	aCtrlID12.Add(IDC_TM_ENDOPT_ANGLE_CBO);
	aCtrlID12.Add(IDC_TM_ANGLE_UNIT);

	aCtrlAngle.Add(IDC_TM_ENDOPT_ANGLE_CBO);
	aCtrlAngle.Add(IDC_TM_ANGLE_UNIT);

	aCtrlID5.Add(IDC_TM_ELEMCURVE_SEMIELLIPSE_CHK);

	aCtrlID6.Add(IDC_TM_ELEMCURVE_DIVIDE);
	aCtrlID6.Add(IDC_TM_ELEMCURVE_DIVIDE_RDO1);
	aCtrlID6.Add(IDC_TM_ELEMCURVE_DIVIDE_RDO2);

	aCtrlIDBase16.Add(IDC_TM_POINT_STATIC1);
	aCtrlIDBase16.Add(IDC_TM_POINT_STATIC2);
	aCtrlIDBase16.Add(IDC_TM_POINT_STATIC3);
	aCtrlIDBase16.Add(IDC_TM_POINT_EDIT1);
	aCtrlIDBase16.Add(IDC_TM_POINT_EDIT2);
	aCtrlIDBase16.Add(IDC_TM_POINT_EDIT3);
	aCtrlIDBase16.Add(IDC_TM_POINT_UNIT1);
	aCtrlIDBase16.Add(IDC_TM_POINT_UNIT2);
	aCtrlIDBase16.Add(IDC_TM_POINT_UNIT3);

	aCtrlIDBase7.Add(IDC_TM_POINT_STATIC4);
	aCtrlIDBase7.Add(IDC_TM_POINT_EDIT4);
	aCtrlIDBase7.Add(IDC_TM_POINT_UNIT4);

	m_pBitmap = 0;
}

CElemCurveDlg::~CElemCurveDlg()
{
	if(m_pBitmap) delete m_pBitmap;
}

void CElemCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemCurveDlg)
	DDX_Control(pDX, IDC_TM_ENDOPT_ANGLE_CBO, m_cboAngle);
	DDX_Control(pDX, IDC_TM_ELEMCURVE_SEMIELLIPSE_CHK, m_chkSemiEllipse);
	DDX_Control(pDX, IDC_TM_ANGLE_UNIT, m_AngleUnit);
	DDX_Control(pDX, IDC_TM_CURVESEG_EDIT, m_edtSeqNum);
	DDX_Control(pDX, IDC_TM_ELEMCURVE_SECT_NO_EDIT, m_SecNoEdit);
	DDX_Control(pDX, IDC_TM_ELEMCURVE_SECT_NAME_CBO, m_SecNameCbo);
	DDX_Control(pDX, IDC_TM_ELEMCURVE_MAT_NAME_CBO, m_MatNameCbo);
	DDX_Control(pDX, IDC_TM_ELEMCURVE_MAT_NO_EDIT, m_MatNoEdit);
	DDX_Control(pDX, IDC_TM_POINT_UNIT4, m_PointUnit4);
	DDX_Control(pDX, IDC_TM_POINT_UNIT3, m_PointUnit3);
	DDX_Control(pDX, IDC_TM_POINT_UNIT2, m_PointUnit2);
	DDX_Control(pDX, IDC_TM_POINT_UNIT1, m_PointUnit1);
	DDX_Control(pDX, IDC_TM_POINT_EDIT4, m_PointEdit4);
	DDX_Control(pDX, IDC_TM_POINT_EDIT3, m_PointEdit3);
	DDX_Control(pDX, IDC_TM_POINT_EDIT2, m_PointEdit2);
	DDX_Control(pDX, IDC_TM_POINT_EDIT1, m_PointEdit1);
	DDX_Control(pDX, IDC_TM_ELEMCURVE_ORI_BANGLE_CBO, m_BAngle);
	DDX_Control(pDX, IDC_TM_ELEMCURVE_ORI_BANGLE_UNIT, m_BAngleUnit);
	DDX_Control(pDX, IDC_TM_CURVETYPE_CBO, m_CurveType);
	DDX_Check(pDX, IDC_TM_INTSECT_NODE_CHECK, m_bIntersectNode);
	DDX_Check(pDX, IDC_TM_INTSECT_ELEM_CHECK, m_bIntersectElem);
	DDX_Radio(pDX, IDC_TM_ELEMCURVE_DIVIDE_RDO1, m_nDivOpt);
	DDX_Radio(pDX, IDC_TM_ENDOPT_P3_RADIO, m_nArcEndPoint);
	DDX_Radio(pDX, IDC_TM_ELEMCURVE_TYPE_TRUSS, m_nElementType);
	DDX_Control(pDX, IDC_TM_CURVE_BMP, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CElemCurveDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CElemCurveDlg)
	ON_CBN_SELCHANGE(IDC_TM_CURVETYPE_CBO, OnSelchangeTmCurvetypeCbo)
	ON_BN_CLICKED(IDC_TM_ELEMCURVE_MAT_BTN, OnTmMatButton)
	ON_BN_CLICKED(IDC_TM_ELEMCURVE_SECT_BTN, OnTmSecButton)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_CURVESEG_SPIN, OnDeltaposTmCurvesegSpin)
	ON_BN_CLICKED(IDC_TM_ENDOPT_P3_RADIO, OnTmEndoptP3Radio)
	ON_BN_CLICKED(IDC_TM_ENDOPT_ANGLE_RADIO, OnTmEndoptAngleRadio)
	ON_EN_UPDATE(IDC_TM_CURVESEG_EDIT, OnUpdateTmCurvesegEdit)
	ON_BN_CLICKED(IDC_TM_ELEMCURVE_SEMIELLIPSE_CHK, OnTmElemcurveSemiellipseChk)
	ON_CBN_EDITCHANGE(IDC_TM_ENDOPT_ANGLE_CBO, OnEditchangeTmEndoptAngleCbo)
	ON_CBN_SELCHANGE(IDC_TM_ENDOPT_ANGLE_CBO, OnSelchangeTmEndoptAngleCbo)
	ON_CBN_EDITUPDATE(IDC_TM_ENDOPT_ANGLE_CBO, OnEditupdateTmEndoptAngleCbo)
	ON_CBN_SELENDOK(IDC_TM_ENDOPT_ANGLE_CBO, OnSelendokTmEndoptAngleCbo)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_PLUGIN_OPERATION_END  , OnPlugInOperationEnd )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemCurveDlg message handlers


BOOL CElemCurveDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	// Curve Type에 항목 입력
	const TCHAR* strCurveType[] = {_LS(IDS_WG_TREEMENU_Arc_by_3_Points),
																_LS(IDS_WG_TREEMENU_Arc_by_Center_and_2_Points),
		                            _LS(IDS_WG_TREEMENU_Circle_by_3_Points), 
																_LS(IDS_WG_TREEMENU_Circle_by_Center_and_2_Points),
															  _LS(IDS_WG_TREEMENU_Ellipse_by_Center_and_2_Points), 
																_LS(IDS_WG_TREEMENU_Parabolic_Curve_by_3_Points),
															  _LS(IDS_WG_TREEMENU_Cubic_Curve_by_4_Points)
																};
	for(int i=0; i < 7; i++)
		m_CurveType.AddString(strCurveType[i]);

	m_CurveType.SetCurSel(0);
	//OnSelchangeTmCurvetypeCbo();
	CurveTypeCtrlMan();

	// Beta Angle 값 초기화
	m_BAngle.AddString(_T("0"));
	m_BAngle.AddString(_LS(IDS_WG_TREEMENU_90));
	m_BAngle.AddString(_LS(IDS_WG_TREEMENU_180));
	m_BAngle.SetCurSel(0);


	// Angle 값 초기화
	CString strAngle;
	for(int i=1; 45*i < 360; i++)
	{
//		if(45*i == 180) continue;  // Arc 그릴때 180도 가 선택이 되면 DataCtrl에서 못그린다. 
																 // 다시 살림. 아무 방향으로 그리도록 함.		
		strAngle.Format(_LS(IDS_WG_TREEMENU__d), 45*i);
		m_cboAngle.AddString(strAngle);
	}
	m_cboAngle.SetCurSel(0);

	// Unit 관련 데이타 초기화
	/*
	m_PointEdit1.SetAttUcsPos();
	m_PointEdit1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_PointEdit1.SetNextLink(&m_PointEdit2);

	m_PointEdit2.SetAttUcsPos();
	m_PointEdit2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_PointEdit2.SetNextLink(&m_PointEdit3);

	m_PointEdit3.SetAttUcsPos();
	m_PointEdit3.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_PointEdit3.SetNextLink(NULL);
	
	m_PointEdit4.SetAttUcsPos();
	m_PointEdit4.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_PointEdit4.SetNextLink(NULL);
	*/

	m_edtSeqNum.SetRange(4, SHRT_MAX);
	m_edtSeqNum.SetInteger(TRUE);

	m_PointUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PointUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PointUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PointUnit4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	

	m_AngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_BAngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);

	// Property 관련 컨트롤 초기화
	InitPropertyCtrls();
	InitEditValue();

	// MNET:XXXX-HSSHIM-20140819. 대화상자에 없는 Control인데, 여기서 사용해서 에러..
	// 	// 나머지 컨트롤 초기화
	// 	CButton* pBtn = (CButton *)GetDlgItem(IDC_TM_ENDOPT1_RADIO12);
	// 	pBtn->SetCheck(TRUE);

	UpdateData(FALSE);

	AlignControls();

	SetPlugInParam();
	m_PointEdit1.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemCurveDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// Reference ctrl 좌표 구하기
	GetDlgItem(IDC_TM_ENDOPT_STATIC)->GetWindowRect(rRef);

	// MNET:XXXX-HSSHIM-20140819. IDC_TM_DEFOPT_CHECK1가 리소스에 없음.
	//GetDlgItem(IDC_TM_DEFOPT_CHECK1)->GetWindowRect(rToMove);
	GetDlgItem(IDC_TM_ELEMCURVE_SEMIELLIPSE_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(10);
	CtrlMoveDistY(aCtrlID5, nDistY);

	// MNET:XXXX-HSSHIM-20140819. IDC_TM_DEFOPT_STATIC가 리소스에 없음.
	//GetDlgItem(IDC_TM_DEFOPT_STATIC)->GetWindowRect(rToMove);
	GetDlgItem(IDC_TM_ELEMCURVE_DIVIDE)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(aCtrlID6, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_INTSECT_NODE_CHECK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(5);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);  
}

void CElemCurveDlg::CurveTypeCtrlMan()
{
		// TODO: Add your control notification handler code here
	
	CDlgUtil::CtrlShowHide(this,aCtrlIDAll, FALSE);
	int i = m_CurveType.GetCurSel();
	
	/*  By L.C.G
	if(i != 6)  // MouseEdit 컨트롤의 링크를 조절한다.
	  m_PointEdit3.SetNextLink(NULL);
	else
	  m_PointEdit3.SetNextLink(&m_PointEdit4);
	*/

	switch(i)
	{
	case 0:  // Arc by 3 points
		CDlgUtil::CtrlShowHide(this, aCtrlID12, TRUE);
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase16, TRUE);
		((CBCGPStatic*)GetDlgItem(IDC_TM_ENDOPT_P3_RADIO))->SetWindowText(_LS(IDS_WG_TREEMENU_P3));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC1))->SetWindowText(_LS(IDS_WG_TREEMENU_P1));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC2))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC3))->SetWindowText(_LS(IDS_WG_TREEMENU_P3));
		break;
	case 1:  // Arc by Center and 2 points
		CDlgUtil::CtrlShowHide(this, aCtrlID12, TRUE);
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase16, TRUE);
		((CBCGPStatic*)GetDlgItem(IDC_TM_ENDOPT_P3_RADIO))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC1))->SetWindowText(_T("C"));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC2))->SetWindowText(_LS(IDS_WG_TREEMENU_P1));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC3))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		break;
	case 2:  // Circle by 3 points
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase16, TRUE);
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC1))->SetWindowText(_LS(IDS_WG_TREEMENU_P1));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC2))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC3))->SetWindowText(_LS(IDS_WG_TREEMENU_P3));
		break;
	case 3:  // Circle by Center and 2 points
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase16, TRUE);
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC1))->SetWindowText(_T("C"));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC2))->SetWindowText(_LS(IDS_WG_TREEMENU_P1));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC3))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		break;
	case 4:  // Ellipse by Center and 2 points
		CDlgUtil::CtrlShowHide(this, aCtrlID5, TRUE);
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase16, TRUE);
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC1))->SetWindowText(_T("C"));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC2))->SetWindowText(_LS(IDS_WG_TREEMENU_P1));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC3))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		break;
	case 5:  // Parabolic Curve by 3 points
		CDlgUtil::CtrlShowHide(this, aCtrlID6, TRUE);
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase16, TRUE);
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC1))->SetWindowText(_LS(IDS_WG_TREEMENU_P1));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC2))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC3))->SetWindowText(_LS(IDS_WG_TREEMENU_P3));
		break;
	case 6:  // Cubic Curve by 4 points
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase16, TRUE);
		CDlgUtil::CtrlShowHide(this, aCtrlIDBase7, TRUE);
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC1))->SetWindowText(_LS(IDS_WG_TREEMENU_P1));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC2))->SetWindowText(_LS(IDS_WG_TREEMENU_P2));
		((CBCGPStatic*)GetDlgItem(IDC_TM_POINT_STATIC3))->SetWindowText(_LS(IDS_WG_TREEMENU_P3));
		break;
	}

	

	/* By L.C.G
	switch(i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		m_PointEdit4.SetLButtonDownNotifyWindow(NULL);
		m_PointEdit4.SetEnterNotifyWindow(NULL);
		m_PointEdit3.SetLButtonDownNotifyWindow(this);
		m_PointEdit3.SetEnterNotifyWindow(this);
		break;
	case 6:
		m_PointEdit3.SetLButtonDownNotifyWindow(NULL);
		m_PointEdit3.SetEnterNotifyWindow(NULL);
		m_PointEdit4.SetLButtonDownNotifyWindow(this);
		m_PointEdit4.SetEnterNotifyWindow(this);
		break;
	} 
	*/

//  GotoDlgCtrl(&m_PointEdit1);
	InitEditValue();
	ChangeBitmap(i);

	
	SetPlugInParam();
}

void CElemCurveDlg::OnSelchangeTmCurvetypeCbo() 
{
	CurveTypeCtrlMan();
	m_PointEdit1.SetFocus();
}

void CElemCurveDlg::OnTmMatButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));	
}

void CElemCurveDlg::OnTmSecButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_SECTION,0));
}

void CElemCurveDlg::Execute()
{
	CreateElemCurve();
}

void CElemCurveDlg::ChangeBitmap(int i)
{
#if(0)
	if(m_pBitmap) delete m_pBitmap;

	UINT aBitmapID[] = { 
		IDB_TM_CURVE1,  IDB_TM_CURVE2, IDB_TM_CURVE3, IDB_TM_CURVE4, 
		IDB_TM_CURVE5,  IDB_TM_CURVE6,  IDB_TM_CURVE7   };      

	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_CURVE_BMP);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[i]);
	pImage->SetBitmap(HBITMAP(*m_pBitmap)); 
#else
	CString aBitmapPath[] = {
		_T("SVG\\illustration\\Dialog\\tm_curv1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_curv2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_curv3.svg"),
		_T("SVG\\illustration\\Dialog\\tm_curv4.svg"),
		_T("SVG\\illustration\\Dialog\\tm_curv5.svg"),
		_T("SVG\\illustration\\Dialog\\tm_curv6.svg"),
		_T("SVG\\illustration\\Dialog\\tm_curv7.svg")
	};
	m_wndPicture.SetImage(aBitmapPath[i]);
#endif
}

void CElemCurveDlg::InitPropertyCtrls()
{
	m_MatNameCbo.SetEditBoxPoint(&m_MatNoEdit);
	m_SecNameCbo.SetEditBoxPoint(&m_SecNoEdit);

	m_MatNoEdit.SetComboBoxPoint(&m_MatNameCbo);
	m_SecNoEdit.SetComboBoxPoint(&m_SecNameCbo);

	CDlgUtil::CtrlEnableDisable(this, aCtrlAngle, FALSE);

	if (!m_bPropInit)
	{
		m_MatNoEdit.SetEditBoxText(1);
		m_SecNoEdit.SetEditBoxText(1);
		m_bPropInit = TRUE;
	}
}

void CElemCurveDlg::OnTmIntstTol() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}

void CElemCurveDlg::OnDeltaposTmCurvesegSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString strTemp;
	if(pNMUpDown->iDelta < 0)
	{
		m_edtSeqNum.GetWindowText(strTemp);		
		strTemp.Format(_LS(IDS_WG_TREEMENU__d), _ttoi(strTemp)+1);
		m_edtSeqNum.SetWindowText(strTemp);
	}
	else
	{
		m_edtSeqNum.GetWindowText(strTemp);		
		if(_ttoi(strTemp) > 4)
		{
			strTemp.Format(_LS(IDS_WG_TREEMENU__d), _ttoi(strTemp) - 1);
			m_edtSeqNum.SetWindowText(strTemp);
		}
	}

	*pResult = 0;

	// By L.C.G
	SetPlugInParam();
}

void CElemCurveDlg::InitEditValue()
{
	m_PointEdit1.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_PointEdit2.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_PointEdit3.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_PointEdit4.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));

	m_edtSeqNum.SetWindowText(_T("8"));

}

LRESULT CElemCurveDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IDC_TM_POINT_EDIT3)
	{
		if(m_PointEdit3.GetSafeHwnd() == (HWND)lParam)
		{
			CreateElemCurve();			
			m_PointEdit3.ImNotAssocWindowST();
			m_PointEdit3.DisConnectAssocWindowST();
			I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
		}
	}
	else if(wParam == IDC_TM_POINT_EDIT4)
	{
		if(m_PointEdit4.GetSafeHwnd() == (HWND)lParam)
		{
			CreateElemCurve();
			m_PointEdit4.ImNotAssocWindowST();
			m_PointEdit4.DisConnectAssocWindowST();
			I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
		}
	}

	return 0L;
}

LRESULT CElemCurveDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IDC_TM_POINT_EDIT3)
	{
		if(m_PointEdit3.GetSafeHwnd() == (HWND)lParam)
		{
			CreateElemCurve();
			m_PointEdit3.ImNotAssocWindowST();
			m_PointEdit3.DisConnectAssocWindowST();
			I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
		}
	}
	else if(wParam == IDC_TM_POINT_EDIT4)
	{
		if(m_PointEdit4.GetSafeHwnd() == (HWND)lParam)
		{
			CreateElemCurve();
			m_PointEdit4.ImNotAssocWindowST();
			m_PointEdit4.DisConnectAssocWindowST();
			I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
		}
	}
	return 0L;
}

void CElemCurveDlg::CreateElemCurve()
{
	UpdateData(TRUE);
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	m_CurvedElem.Initialize();
	if(!GetParameter(m_CurvedElem))
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
		return;
	}

	if(!pDoc->m_pDataCtrl->CurvedElem(m_CurvedElem, m_bIntersectNode, m_bIntersectElem))
		return;
}

BOOL CElemCurveDlg::GetParameter(T_CURVEDELEM &CurvedElem)
{
	CString strTemp;

	CurvedElem.nType = m_CurveType.GetCurSel() + 1;
	CurvedElem.nEltype = m_nElementType + 1;
	m_MatNoEdit.GetKey(CurvedElem.KeyMatl);
	m_SecNoEdit.GetKey(CurvedElem.KeySect);

	m_BAngle.GetWindowText(strTemp);
	if(!GetFloatNumber(strTemp, CurvedElem.dBetaAngle)) return FALSE;
	m_edtSeqNum.GetWindowText(strTemp);
	if(!GetINumber(strTemp, CurvedElem.nSegment)) return FALSE;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	switch(CurvedElem.nType)
	{
		case 1:
			m_Arc3p.Initialize();
			if(!GetEditPosition(m_PointEdit1, m_Arc3p.p1[0], m_Arc3p.p1[1], m_Arc3p.p1[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit2, m_Arc3p.p2[0], m_Arc3p.p2[1], m_Arc3p.p2[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit3, m_Arc3p.p3[0], m_Arc3p.p3[1], m_Arc3p.p3[2])) return FALSE;
			m_Arc3p.bEndAngle = m_nArcEndPoint;  
			if(m_Arc3p.bEndAngle)
			{
				m_cboAngle.GetWindowText(strTemp);
				if(!GetFloatNumber(strTemp, m_Arc3p.dAngle)) return FALSE;
			}
			CurvedElem.Arc3p = m_Arc3p;
			break;
		case 2:
			m_Arc2p1c.Initialize();
			if(!GetEditPosition(m_PointEdit1, m_Arc2p1c.pc[0], m_Arc2p1c.pc[1], m_Arc2p1c.pc[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit2, m_Arc2p1c.p1[0], m_Arc2p1c.p1[1], m_Arc2p1c.p1[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit3, m_Arc2p1c.p2[0], m_Arc2p1c.p2[1], m_Arc2p1c.p2[2])) return FALSE;
			m_Arc2p1c.bEndAngle = m_nArcEndPoint;
			if(m_Arc2p1c.bEndAngle)
			{
				m_cboAngle.GetWindowText(strTemp);
				if(!GetFloatNumber(strTemp, m_Arc2p1c.dAngle)) return FALSE;
			}
			CurvedElem.Arc2p1c = m_Arc2p1c;
			break;
		case 3:
			m_Cir3p.Initialize();
			if(!GetEditPosition(m_PointEdit1, m_Cir3p.p1[0], m_Cir3p.p1[1], m_Cir3p.p1[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit2, m_Cir3p.p2[0], m_Cir3p.p2[1], m_Cir3p.p2[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit3, m_Cir3p.p3[0], m_Cir3p.p3[1], m_Cir3p.p3[2])) return FALSE;
			CurvedElem.Cir3p = m_Cir3p;
			break;
		case 4:
			m_Cir2p1c.Initialize();
			if(!GetEditPosition(m_PointEdit1, m_Cir2p1c.pc[0], m_Cir2p1c.pc[1], m_Cir2p1c.pc[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit2, m_Cir2p1c.p1[0], m_Cir2p1c.p1[1], m_Cir2p1c.p1[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit3, m_Cir2p1c.p2[0], m_Cir2p1c.p2[1], m_Cir2p1c.p2[2])) return FALSE;
			CurvedElem.Cir2p1c = m_Cir2p1c;
			break;
		case 5:
			m_Elp2p1c.Initialize();
			if(!GetEditPosition(m_PointEdit1, m_Elp2p1c.pc[0], m_Elp2p1c.pc[1], m_Elp2p1c.pc[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit2, m_Elp2p1c.p1[0], m_Elp2p1c.p1[1], m_Elp2p1c.p1[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit3, m_Elp2p1c.p2[0], m_Elp2p1c.p2[1], m_Elp2p1c.p2[2])) return FALSE;
			m_Elp2p1c.bTrim = m_chkSemiEllipse.GetCheck();
			CurvedElem.Elp2p1c = m_Elp2p1c;
			break;
		case 6:
			m_Par3p.Initialize();
			if(!GetEditPosition(m_PointEdit1, m_Par3p.p1[0], m_Par3p.p1[1], m_Par3p.p1[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit2, m_Par3p.p2[0], m_Par3p.p2[1], m_Par3p.p2[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit3, m_Par3p.p3[0], m_Par3p.p3[1], m_Par3p.p3[2])) return FALSE;
			m_Par3p.bProjection = m_nDivOpt;
			pIGM->GetWcsVector(1.0, 0.0, 0.0, m_Par3p.axisvector[0], m_Par3p.axisvector[1], m_Par3p.axisvector[2]);
			CurvedElem.Par3p = m_Par3p;
			break;
		case 7:
			m_Cub4p.Initialize();
			if(!GetEditPosition(m_PointEdit1, m_Cub4p.p1[0], m_Cub4p.p1[1], m_Cub4p.p1[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit2, m_Cub4p.p2[0], m_Cub4p.p2[1], m_Cub4p.p2[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit3, m_Cub4p.p3[0], m_Cub4p.p3[1], m_Cub4p.p3[2])) return FALSE;
			if(!GetEditPosition(m_PointEdit4, m_Cub4p.p4[0], m_Cub4p.p4[1], m_Cub4p.p4[2])) return FALSE;
			CurvedElem.Cub4p = m_Cub4p;
			break;
	}

	return TRUE;
}

BOOL CElemCurveDlg::GetEditPosition(CMouseEdit& edtCtrl, double& Px, double& Py, double& Pz)
{  
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	double UX,UY,UZ;

	if(edtCtrl.IsKeyPressed())
	{
		CString strPos;
		edtCtrl.GetWindowText(strPos);
		if(!GetPosition(strPos,UX,UY,UZ)) return FALSE;
		pIGM->GetWcsPos(UX,UY,UZ,Px,Py,Pz); // UCS Data를 WCS Data로 변환 한다.
	}
	else
	{
		edtCtrl.GetCurMousePosReal(Px,Py,Pz,UX,UY,UZ);
	}
	return TRUE;  
}

void CElemCurveDlg::OnTmEndoptP3Radio() 
{
	CDlgUtil::CtrlEnableDisable(this, aCtrlAngle, FALSE);
	
	// By L.C.G
	SetPlugInParam();
}

void CElemCurveDlg::OnTmEndoptAngleRadio() 
{
	CDlgUtil::CtrlEnableDisable(this, aCtrlAngle, TRUE);	

	// By L.C.G
	SetPlugInParam();
}


	/*********************************************************************
	 *
	 *
 *****  Plug In Functions... By L.C.G.
	***
	 */
LRESULT CElemCurveDlg::OnPlugInOperationEnd(WPARAM wParam, LPARAM lParam) 
{
	 if(m_PointEdit3.GetSafeHwnd() == (HWND)lParam)
	 {
			CreateElemCurve();
			m_PointEdit3.ImNotAssocWindowST();
			m_PointEdit3.DisConnectAssocWindowST();
			I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	 }

	 if(m_PointEdit4.GetSafeHwnd() == (HWND)lParam)
	 {
			CreateElemCurve();
			m_PointEdit4.ImNotAssocWindowST();
			m_PointEdit4.DisConnectAssocWindowST();
			I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	 }
	
	 return 0L;
}

BOOL  CElemCurveDlg::SetPlugInParam()
{
	UpdateData();

	CString strTemp;
	int nWhat = m_CurveType.GetCurSel();
	int nSubType = 0;
	double dAngle = 0.0;
	int nSegment = 0;
	
	m_edtSeqNum.GetWindowText(strTemp);
	if(!GetINumber(strTemp, nSegment)) return FALSE;
	
	switch(nWhat)
	{
		case 0:
			nSubType = m_nArcEndPoint;  
			m_cboAngle.GetWindowText(strTemp);
			if(!GetFloatNumber(strTemp,dAngle)) return FALSE;
			break;
		case 1:
			nSubType = m_nArcEndPoint;  
			m_cboAngle.GetWindowText(strTemp);
			if(!GetFloatNumber(strTemp,dAngle)) return FALSE;
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			if(m_chkSemiEllipse.GetCheck())
			  nSubType = 1;
			else 
			  nSubType = 0;
			break;
		case 5:
			nSubType = m_nDivOpt;
			break;
		case 6:

			break;
	}

	CArray<CMouseEdit*,CMouseEdit*> MEArr;
 
	if(nWhat == 6)
	{
		MEArr.Add(&m_PointEdit1);
	  MEArr.Add(&m_PointEdit2);
		MEArr.Add(&m_PointEdit3);
		MEArr.Add(&m_PointEdit4);
	}
	else
	{
		MEArr.Add(&m_PointEdit1);
	  MEArr.Add(&m_PointEdit2);
		MEArr.Add(&m_PointEdit3);
	}

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->SetMouseEditPlugInForCurvedElem(MEArr,this,nWhat,nSegment,nSubType,dAngle);

	return TRUE;
}

void   CElemCurveDlg::EndPlugInParam()
{
	I_GENModelBase::GetCurMySelfST()->ClearMouseEditPlugIn();
}

void CElemCurveDlg::OnUpdateTmCurvesegEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CMenuBarChildDlg::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	SetPlugInParam();
	
}

void CElemCurveDlg::OnTmElemcurveSemiellipseChk() 
{
	// TODO: Add your control notification handler code here
	SetPlugInParam();
}

void CElemCurveDlg::OnEditchangeTmEndoptAngleCbo() 
{
	// TODO: Add your control notification handler code here
	SetPlugInParam();
}

void CElemCurveDlg::OnSelchangeTmEndoptAngleCbo() 
{
	// TODO: Add your control notification handler code here
	//SetPlugInParam();
	//GSaveHistoryNF("OnSelchangeTmEndoptAngleCbo");
	int nCur = m_cboAngle.GetCurSel();
	//GSaveHistoryFormatNF("Angle %d",nCur);
	switch(nCur)
	{
	case 0:
		m_cboAngle.SetWindowText(_LS(IDS_WG_TREEMENU_45));
		break;
	case 1:
		m_cboAngle.SetWindowText(_LS(IDS_WG_TREEMENU_90));
		break;
	case 2:
		m_cboAngle.SetWindowText(_LS(IDS_WG_TREEMENU_135));
		break;
	case 3:
		m_cboAngle.SetWindowText(_LS(IDS_WG_TREEMENU_180));
		break;
	case 4:
		m_cboAngle.SetWindowText(_LS(IDS_WG_TREEMENU_225));
		break;
	case 5:
		m_cboAngle.SetWindowText(_LS(IDS_WG_TREEMENU_270));
		break;
	case 6:
		m_cboAngle.SetWindowText(_LS(IDS_WG_TREEMENU_315));
		break;
	}
	
	SetPlugInParam();

}

void CElemCurveDlg::OnEditupdateTmEndoptAngleCbo() 
{
	// TODO: Add your control notification handler code here
	//SetPlugInParam();
}

void CElemCurveDlg::OnSelendokTmEndoptAngleCbo() 
{
	// TODO: Add your control notification handler code here
	//SetPlugInParam();
}
