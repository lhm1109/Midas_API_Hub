// NodeCreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeCreateDlg.h"

#include "..\wg_db\wg_dbRes2.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeCreateDlg dialog
CNodeCreateDlg::CNodeCreateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeCreateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeCreateDlg)
	m_bIsCreateSamePos = TRUE;
	m_strCoordXYZ = _LS(IDS_WG_TREEMENU_0___0___0);
	m_strDisXYZ = _LS(IDS_WG_TREEMENU_0__0__0_);
	m_nNCopy = 0;
	m_bIsIntersectFrameElem = TRUE;
	//}}AFX_DATA_INIT
}


void CNodeCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeCreateDlg)
	DDX_Control(pDX, IDC_TM_DISTUNIT0, m_DistUnit0);
	DDX_Control(pDX, IDC_TM_DIST_UNIT2, m_DistUnit2);
	DDX_Control(pDX, IDC_TM_DIST_XYZ , m_wndDistXYZ);
	DDX_Control(pDX, IDC_TM_COORD_XYZ, m_wndCoordXYZ);
	DDX_Check(pDX, IDC_TM_CHECK_SAME_POS, m_bIsCreateSamePos);
	DDX_Text(pDX, IDC_TM_COORD_XYZ, m_strCoordXYZ);
	DDX_Text(pDX, IDC_TM_DIST_XYZ, m_strDisXYZ);
	DDX_Control(pDX, IDC_TM_NCOPY, m_spinNCopy);
	DDX_Check(pDX, IDC_TM_CHECK_INTS_FRAME, m_bIsIntersectFrameElem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeCreateDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CNodeCreateDlg)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ITER_SPIN, OnDeltaposTmIterSpin)
	ON_BN_CLICKED(IDC_TM_INTSECT_TOL, OnTmIntsectTol)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeCreateDlg message handlers

BOOL CNodeCreateDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_wndCoordXYZ.SetAttUcsPos();
	m_wndCoordXYZ.SetModeToUse(MOUSEEDIT_USE_NODE_CREATE);
	m_wndCoordXYZ.SetEnterNotifyWindow(this);
	m_wndCoordXYZ.SetLButtonDownNotifyWindow(this);

	m_wndDistXYZ.SetAttUcsDistance();
	m_wndDistXYZ.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	//m_wndDistXYZ.SetLButtonDownNotifyWindow(this);
		
	m_DistUnit0.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_spinNCopy.SetRange(0, SHRT_MAX);
	m_spinNCopy.SetValue(m_nNCopy);
	m_spinNCopy.SetInteger(TRUE);
	
	//CMouseEdit::ImAssocWindowST();
	GotoDlgCtrl(&m_wndCoordXYZ);
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeCreateDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	if(::IsWindow(m_hWnd))
	{
		GotoDlgCtrl(&m_wndCoordXYZ);
	}
}

BOOL CNodeCreateDlg::GetParameter(_NodeCreate& CS)
{
	m_bIsParamError = FALSE;
	CS.InitErrorFlag();
	
	UpdateData(TRUE);
	
	MyParser Parser;
	
	CString strNCopy;
	m_spinNCopy.GetWindowText(strNCopy);
	if(FALSE == Parser.ParsingINumberArg  (strNCopy)   )
		 m_bIsParamError = CS.IsNCopyError   = TRUE;
	else
	{
		if(FALSE == Parser.GetINumberData     (CS.NCopyNum)  )
		 m_bIsParamError = CS.IsNCopyError   = TRUE;
	}
	
	if(m_wndCoordXYZ.IsKeyPressed())
	{
		if(FALSE == Parser.ParsingPositionArg (m_strCoordXYZ))
			 m_bIsParamError = CS.IsNodePosError = TRUE;
		else
			if(FALSE == Parser.GetPositionData    (CS.Nx,CS.Ny,CS.Nz)     )
				m_bIsParamError = CS.IsNodePosError = TRUE;
	}
	else // Key In이 발생하지 않았을 경우 Parsing을 하지 않고 CMouseEdit의 Real Data를 설정한다. 
	{
		double wx,wy,wz,ux,uy,uz;
		m_wndCoordXYZ.GetCurMousePosReal(wx,wy,wz,ux,uy,uz);

		CS.Nx = ux; CS.Ny = uy ; CS.Nz = uz;
	}
	
	if(m_wndDistXYZ.IsKeyPressed())
	{
		if(FALSE == Parser.ParsingPositionArg (m_strDisXYZ)  )
			 m_bIsParamError = CS.IsDistError    = TRUE;
		else
			if(FALSE == Parser.GetPositionData    (CS.Dx,CS.Dy,CS.Dz)     )
			 m_bIsParamError = CS.IsDistError    = TRUE;
	}
	else // Key In이 발생하지 않았을 경우 Parsing을 하지 않고 CMouseEdit의 Real Data를 설정한다 
	{
		double dwx,dwy,dwz,dux,duy,duz;  
		m_wndDistXYZ.GetCurMousePosDist(dwx, dwy, dwz, dux, duy, duz);
		CS.Dx = dux ; CS.Dy = duy ; CS.Dz = duz;
	}
	
	return (!m_bIsParamError);
}

void CNodeCreateDlg::InitOption()
{
	m_strCoordXYZ = _LS(IDS_WG_TREEMENU_0___0___0);
	m_strDisXYZ = _LS(IDS_WG_TREEMENU_0__0__0_);
	m_nNCopy = 0;
	UpdateData(FALSE);
}

// StructNodeDlg에서 호출된다 
void CNodeCreateDlg::Execute()
{
	_NodeCreate CS;
	if(GetParameter(CS))
	{
		//if(AddNode(CS));
			//InitOption();
		AddNode(CS);
	}
}

void CNodeCreateDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	// Document Pointer를 획득하고 Tolerance 설정 Dialog를 생성한다. 
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

LRESULT CNodeCreateDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	// lParam == HWND ==> Message를 보낸 CMouseEdit의 HWND ...
	if(m_wndCoordXYZ.GetSafeHwnd() == (HWND)lParam)
	{
		_NodeCreate CS;
		
		if(GetParameter(CS))
		{
			CButton* pExecButton = GetExecButton();
			if(pExecButton)
			{
				//Execute Button의 Push 효과를 낸다. 
			}
			
			//if(AddNode(CS))  ;
				//InitOption();
			AddNode(CS);
		}
	}
	return 0L;
}

LRESULT CNodeCreateDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(m_wndCoordXYZ.GetSafeHwnd() == (HWND)lParam)
	{
		if(pIGM->GetModelingModeST() == IG_STATE_NODE_ADD)
		{
			_NodeCreate CS;
			if(GetParameter(CS))
			{
				CButton* pExecButton = GetExecButton();
				if(pExecButton)
				{
				// Execute Button의 Push 효과를 낸다. 
				}
				pIGM->CursorHide();
				AddNode(CS);
				pIGM->CursorCurPointDraw();
				
				pIGM->SetModelingModeST(IG_STATE_INITALL);

				pIGM->SetModelingModeST(IG_STATE_NODE_ADD);
				//m_wndCoordXYZ.SetFocus();
			}
		}
	}
	return 0L;
}

BOOL CNodeCreateDlg::AddNode(_NodeCreate &CS)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(pIGM)
	{
		// Ucs좌표로 주어진 Point Data를 Wcs좌표로 변환한다. 
		T_NODE_D NodePos;
		
		pIGM->GetWcsPos(CS.Nx,CS.Ny,CS.Nz,CS.Nx,CS.Ny,CS.Nz);
		pIGM->GetWcsVector(CS.Dx,CS.Dy,CS.Dz,CS.Dx,CS.Dy,CS.Dz);
		
		NodePos.x = CS.Nx;
		NodePos.y = CS.Ny;
		NodePos.z = CS.Nz;
	
		CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		(pDoc->m_pDataCtrl)->AddNode(0,NodePos,CS.NCopyNum,CS.Dx,CS.Dy,CS.Dz,m_bIsCreateSamePos,
																																				 m_bIsIntersectFrameElem);
		return TRUE;
	}
	return FALSE;
}

// void CNodeCreateDlg::OnDeltaposTmIterSpin(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	int NCopy;
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 	UpdateData(TRUE);
// 	MyParser Parser;
// 	
// 	if(Parser.ParsingINumberArg(m_strNCopy))
// 	{
// 		Parser.GetINumberData(NCopy);
// 	}
// 	else
// 	{
// 		NCopy = 0;
// 	}
// 	
// 	if(pNMUpDown->iDelta < 0)
// 	{
// 		
// 			NCopy += 1;
// 	}
// 	else
// 	{
// 		if(NCopy > 0)
// 			 NCopy -= 1;
// 	}
// 	
// 	m_spinNCopy.SetValue(NCopy);
// 	
// 	UpdateData(FALSE);
// 	*pResult = 0;
// }

void CNodeCreateDlg::OnTmIntsectTol() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));
}
