// DgnDrawBase_DrawView_Impl.cpp: implementation of the CDgnDrawBase_DrawView_Impl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnDrawBase_DrawView_Impl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnDrawBase_DrawView_Connector_Impl::CDgnDrawBase_DrawView_Connector_Impl()
{
	
}

CDgnDrawBase_DrawView_Connector_Impl::~CDgnDrawBase_DrawView_Connector_Impl()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnDrawBase_DrawView_Impl::CDgnDrawBase_DrawView_Impl(UINT resID, CWnd* pParent /*=NULL*/)
	: CDgnDrawBase_DrawView(resID, pParent)
{
	
}

CDgnDrawBase_DrawView_Impl::~CDgnDrawBase_DrawView_Impl()
{

}

void CDgnDrawBase_DrawView_Impl::DoDataExchange(CDataExchange* pDX)
{
	CDgnDrawBase_DrawView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnDrawBase_DrawView_Impl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnDrawBase_DrawView_Impl, CDgnDrawBase_DrawView)
	//{{AFX_MSG_MAP(CDgnDrawBase_DrawView_Impl)
	ON_WM_PAINT()	
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
BOOL CDgnDrawBase_DrawView_Impl::OnInitialDialog(CWnd* pParentWnd, UINT nID_PlaceHolder)
{ return CDgnDrawBase_DrawView::On_InitDialog( pParentWnd, nID_PlaceHolder); }
void CDgnDrawBase_DrawView_Impl::SetFit()
{ CDgnDrawBase_DrawView::Set_Fit(); }
void CDgnDrawBase_DrawView_Impl::SetMode(int nMode)
{ CDgnDrawBase_DrawView::Set_Mode(nMode); }
int  CDgnDrawBase_DrawView_Impl::GetMode() 
{return m_nMode;};	
void CDgnDrawBase_DrawView_Impl::SetSnap(BOOL bObjectSnap, BOOL bGridSnap, int nSnapDis)
{ CDgnDrawBase_DrawView::Set_Snap(bObjectSnap, bGridSnap, nSnapDis); }
void CDgnDrawBase_DrawView_Impl::SetAxisMode(int nAxisMode, int nSize, int nWidth, BOOL bCapital)
{ CDgnDrawBase_DrawView::Set_AxisMode(nAxisMode, nSize, nWidth, bCapital); }
void CDgnDrawBase_DrawView_Impl::SetAxisLocal(double dOrgPoint[2], double dAxisVector1[2], double dAxisVector2[2])
{ CDgnDrawBase_DrawView::Set_AxisLocal(dOrgPoint, dAxisVector1, dAxisVector2); }
void CDgnDrawBase_DrawView_Impl::SetGridMode(int nGridMode, double dInterval_x, double dInterval_y)
{ CDgnDrawBase_DrawView::Set_GridMode(nGridMode, dInterval_x, dInterval_y); }

void CDgnDrawBase_DrawView_Impl::Set_WndConnector(CDgnDrawBase_DrawView_Connector* pConnector)
{ CDgnDrawBase_DrawView::Set_WndConnector(pConnector); }
void CDgnDrawBase_DrawView_Impl::SetWndCoordText(CBCGPStatic* pText)
{ CDgnDrawBase_DrawView::Set_WndCoordText(pText); }
void CDgnDrawBase_DrawView_Impl::SetWndModeText(CBCGPStatic* pText)
{ CDgnDrawBase_DrawView::Set_WndModeText(pText); }
void CDgnDrawBase_DrawView_Impl::SetWndPointEdit(CEdit* pEdit)
{ CDgnDrawBase_DrawView::Set_WndPointEdit(pEdit); }
void CDgnDrawBase_DrawView_Impl::SetWndLineEdit(CEdit* pStartEdit, CEdit* pEndEdit)
{ CDgnDrawBase_DrawView::Set_WndLineEdit(pStartEdit, pEndEdit); }	
void CDgnDrawBase_DrawView_Impl::SetWndArcEdit(CEdit* pStartEdit, CEdit* pPassEdit, CEdit* pEndEdit)
{ CDgnDrawBase_DrawView::Set_WndArcEdit(pStartEdit, pPassEdit, pEndEdit); }
void CDgnDrawBase_DrawView_Impl::SetWndCircleEdit(CEdit* pStartEdit, CEdit* pEndEdit, CEdit* pRadiusEdit)
{ CDgnDrawBase_DrawView::Set_WndCircleEdit(pStartEdit, pEndEdit, pRadiusEdit); }

void CDgnDrawBase_DrawView_Impl::Zoom(CPoint CurrP, double dZoomRatio)
{ CDgnDrawBase_DrawView::Zoom_Ratio(CurrP, dZoomRatio); }
void CDgnDrawBase_DrawView_Impl::ZoomOut(CPoint CurrP)
{ CDgnDrawBase_DrawView::Zoom_Out(CurrP); }
void CDgnDrawBase_DrawView_Impl::ZoomIn(CPoint CurrP)
{ CDgnDrawBase_DrawView::Zoom_In(CurrP); }
void CDgnDrawBase_DrawView_Impl::ZoomOut()
{ CDgnDrawBase_DrawView::Zoom_Out(); }
void CDgnDrawBase_DrawView_Impl::ZoomIn()
{ CDgnDrawBase_DrawView::Zoom_In(); }

// SelectKey
int CDgnDrawBase_DrawView_Impl::Get_SelectedKeyList(CArray<UINT, UINT>& arKey)
{ return CDgnDrawBase_DrawView::Get_SelectedKeyList(arKey); }
int CDgnDrawBase_DrawView_Impl::Get_SelectedLinkKeyList(CArray<UINT, UINT>& arLinkKey)
{ return CDgnDrawBase_DrawView::Get_SelectedLinkKeyList(arLinkKey); }
void CDgnDrawBase_DrawView_Impl::Set_SelectedKeyList(CArray<UINT, UINT>& arKey, BOOL bReDraw)
{ CDgnDrawBase_DrawView::Set_SelectedKeyList(arKey, bReDraw); }
void CDgnDrawBase_DrawView_Impl::Set_SelectedLinkKeyList(CArray<UINT, UINT>& arLinkKey, BOOL bReDraw)
{ CDgnDrawBase_DrawView::Set_SelectedLinkKeyList(arLinkKey, bReDraw); }

// CurrUnit를 직접 입력	
void CDgnDrawBase_DrawView_Impl::ClearCurrUnit()
{ CDgnDrawBase_DrawView::Clear_CurrUnit(); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Point()
{ CDgnDrawBase_DrawView::Set_CurrUnit_Point(); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Point(double dStartP[2])
{ CDgnDrawBase_DrawView::Set_CurrUnit_Point(dStartP); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Line(int nCount)
{ CDgnDrawBase_DrawView::Set_CurrUnit_Line(nCount); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Line(double dStartP[2], double dEndP[2], int nCount)
{ CDgnDrawBase_DrawView::Set_CurrUnit_Line(dStartP, dEndP, nCount); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Arc(int nCount)
{ CDgnDrawBase_DrawView::Set_CurrUnit_Arc(nCount); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Arc(double dStartP[2], double dPassP[2], double dEndP[2], int nCount)
{ CDgnDrawBase_DrawView::Set_CurrUnit_Arc(dStartP, dPassP, dEndP, nCount); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Circle(int nCount, int nType)
{ CDgnDrawBase_DrawView::Set_CurrUnit_Circle(nCount, nType); }
void CDgnDrawBase_DrawView_Impl::SetCurrUnit_Circle(double dStartP[2], double dEndP[2], int nCount)
{ CDgnDrawBase_DrawView::Set_CurrUnit_Circle(dStartP, dEndP, nCount); }
/////////////////////////////////////////////////////////////////////////////
void CDgnDrawBase_DrawView_Impl::GetGenuineCoord(CPoint CurrP, double& dXR, double& dYR)
{ CDgnDrawBase_DrawView::Get_GenuineCoord(CurrP, dXR, dYR); }
void CDgnDrawBase_DrawView_Impl::GetDisplayCoord(double dXR, double dYR, CPoint& CurrP)
{ CDgnDrawBase_DrawView::Get_DisplayCoord(dXR, dYR, CurrP); }
CString CDgnDrawBase_DrawView_Impl::GetCoordText(double dX, double dY)
{ return CDgnDrawBase_DrawView::Get_CoordText(dX, dY); }
BOOL CDgnDrawBase_DrawView_Impl::GetCoordFromEdit(CEdit* pEdit, double& dX, double& dY)
{ return CDgnDrawBase_DrawView::Get_CoordFromEdit(pEdit, dX, dY); }

BOOL CDgnDrawBase_DrawView_Impl::GetOpjectSnapPoint(CPoint currPoint, CPoint& snapPoint, T_DGN_POINT& snapPointR)
{ return CDgnDrawBase_DrawView::Get_OpjectSnapPoint(currPoint, snapPoint, snapPointR); }
BOOL CDgnDrawBase_DrawView_Impl::GetGridSnapPoint(CPoint currPoint, CPoint& snapPoint, T_DGN_POINT& snapPointR)
{ return CDgnDrawBase_DrawView::Get_GridSnapPoint(currPoint, snapPoint, snapPointR); }
/////////////////////////////////////////////////////////////////////////////
// CDgnDrawBase_DrawView_Impl message handlers
void CDgnDrawBase_DrawView_Impl::OnPaint() 
{
	On_Paint();	
}
BOOL CDgnDrawBase_DrawView_Impl::OnEraseBkgnd(CDC* pDC) 
{
	return On_EraseBkgnd(pDC);
}

// Mouse ///////////////////////////////////////////////////////////////////////
void CDgnDrawBase_DrawView_Impl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	On_LButtonDown(nFlags, point) ;
}
void CDgnDrawBase_DrawView_Impl::OnLButtonUp(UINT nFlags, CPoint point)
{
	On_LButtonUp(nFlags, point);
}
void CDgnDrawBase_DrawView_Impl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	On_LButtonDblClk(nFlags, point);	
}
void CDgnDrawBase_DrawView_Impl::OnMouseMove(UINT nFlags, CPoint point)
{
	On_MouseMove(nFlags, point);
}
BOOL CDgnDrawBase_DrawView_Impl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{	
	return On_MouseWheel(nFlags, zDelta, pt);
}
void CDgnDrawBase_DrawView_Impl::OnMButtonDown(UINT nFlags, CPoint point)
{
	On_MButtonDown(nFlags, point);
}
void CDgnDrawBase_DrawView_Impl::OnMButtonUp(UINT nFlags, CPoint point)
{
	On_MButtonUp(nFlags, point);	
}
void CDgnDrawBase_DrawView_Impl::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	On_MButtonDblClk(nFlags, point);
}
void CDgnDrawBase_DrawView_Impl::OnRButtonDown(UINT nFlags, CPoint point)
{
	On_RButtonDown(nFlags, point);
}
void CDgnDrawBase_DrawView_Impl::OnRButtonUp(UINT nFlags, CPoint point)
{
	On_RButtonUp(nFlags, point);
}
void CDgnDrawBase_DrawView_Impl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	On_RButtonDblClk(nFlags, point);
}
