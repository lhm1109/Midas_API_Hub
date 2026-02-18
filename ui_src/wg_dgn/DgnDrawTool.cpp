// DgnDrawTool.cpp: implementation of the CDgnDrawTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnDrawTool.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_SCALE				0.75
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnDrawTool::CDgnDrawTool()
{
	m_aDrawD.RemoveAll();
	m_aPointDrawD.RemoveAll();
	m_aSelectDrawD.RemoveAll();

	m_dMaxX = m_dMaxZ = -9999999999.9;
	m_dMinX = m_dMinZ =  9999999999.9;
	m_dZero = 1.E-07;

	m_dMouseScale = 1.0;
	m_MousePosiPoint = CPoint(0,0);

	m_bElem = FALSE;

	m_SelNodeK1 = m_SelNodeK2 = 0;
	m_SelElemK = 0;
}

CDgnDrawTool::~CDgnDrawTool()
{

}


BEGIN_MESSAGE_MAP(CDgnDrawTool, CWnd)
	//{{AFX_MSG_MAP(CDgnDrawTool)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDgnDrawTool::Create(CWnd *pParentWnd, const RECT& rect, UINT nID)
{
	CREATESTRUCT cs;
	LPCTSTR lpClass = AfxRegisterWndClass(CS_OWNDC);
	cs.lpszClass = lpClass;
	cs.style = WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE;
	CWnd::PreCreateWindow(cs);
	return CWnd::Create(lpClass, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, rect, pParentWnd, nID, NULL);
}

void CDgnDrawTool::FitToParent(CRect rect)
{
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE);
}

BOOL CDgnDrawTool::Init(CWnd* pParentWnd)
{ 
	if(SubclassWindow(pParentWnd->m_hWnd))
	{
		TRACE(_T("Success To Subclassing\n"));
		return FALSE;
	}
	LONG lstyle=WS_CLIPCHILDREN | WS_VISIBLE;
	::SetWindowLongPtr(m_hWnd, GWL_STYLE, lstyle);

	return TRUE;
}

void CDgnDrawTool::ClearBkgnd(CDC* pDC)
{
	CBrush BackBrush(RGB(160,160,160)); // white
	CBrush* pOldBrush = pDC->SelectObject(&BackBrush);
	CRect rect;
	GetClientRect(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	BackBrush.DeleteObject();
}

double CDgnDrawTool::CalcScale(CRect rect, double dX, double dY, double dZoomRat/*0.8*/)
{
	double dScale=0.0;
	double dScale1=0.0, dScale2=0.0;
	if(fabs(dX) > m_dZero && fabs(dY) > m_dZero)
	{
		dScale1 = (rect.Width() /dX) * dZoomRat;
		dScale2 = (rect.Height()/dY) * dZoomRat;
	}
	else if(fabs(dX) < m_dZero && fabs(dY) > m_dZero)
	{
		dScale2 = (rect.Height()/dY) * dZoomRat;
		dScale1 = dScale2;
	}
	else if(fabs(dX) > m_dZero && fabs(dY) < m_dZero)
	{
		dScale1 = (rect.Width() /dX) * dZoomRat;
		dScale2 = dScale1;
	}
	else
	{
		dScale1 = 0.0;
		dScale2 = 0.0;
	}
	dScale = min(dScale1, dScale2);

	return dScale;
}

CPoint CDgnDrawTool::CalcOriginPoint(CRect rect, double dScale, double dX, double dY)
{
	CPoint orPoint;
	// *표는 편의를 위해 변환시킨 기준좌표 위치 (원래위치는 $)
	// ---------------------- //
	// |    $    |          | //
	// |         |          | //
	// |---------+----------| //
	// |         |          | //
	// |    *    |          | //
	// ---------------------- //
	orPoint.x = long((rect.Width() -dScale*dX)/2.0);
	// Y좌표를 +로 한 이유는
	// 중립축을 기준으로 위아래를 바꾸기 위해서 
	orPoint.y = long((rect.Height()+dScale*dY)/2.0);
	
	return orPoint;
}

void CDgnDrawTool::ClearBorder()
{
 CClientDC dc(this);
	ClearBkgnd(&dc);
	dc.SetBkMode(TRANSPARENT);
}

void CDgnDrawTool::Draw()
{
	m_dMaxX = m_dMaxZ = -9999999999.9;
	m_dMinX = m_dMinZ =  9999999999.9;

	int nSize = m_aDrawD.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		ABLD_DR DrawD;
		DrawD.Initialize();
		DrawD = m_aDrawD[i];

		m_dMaxX = max(m_dMaxX, DrawD.x1);
		m_dMaxX = max(m_dMaxX, DrawD.x2);
		m_dMinX = min(m_dMinX, DrawD.x1);
		m_dMinX = min(m_dMinX, DrawD.x2);

		m_dMaxZ = max(m_dMaxZ, DrawD.z1);
		m_dMaxZ = max(m_dMaxZ, DrawD.z2);
		m_dMinZ = min(m_dMinZ, DrawD.z1);
		m_dMinZ = min(m_dMinZ, DrawD.z2);
	}

	nSize = m_aPointDrawD.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		ABLD_DR DrawD;
		DrawD.Initialize();
		DrawD = m_aPointDrawD[i];

		m_dMaxX = max(m_dMaxX, DrawD.x1);
		m_dMinX = min(m_dMinX, DrawD.x1);
		
		m_dMaxZ = max(m_dMaxZ, DrawD.z1);
		m_dMinZ = min(m_dMinZ, DrawD.z1);    
	}

	nSize = m_aSelectDrawD.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		ABLD_DR DrawD;
		DrawD.Initialize();
		DrawD = m_aSelectDrawD[i];

		m_dMaxX = max(m_dMaxX, DrawD.x1);
		m_dMaxX = max(m_dMaxX, DrawD.x2);
		m_dMinX = min(m_dMinX, DrawD.x1);
		m_dMinX = min(m_dMinX, DrawD.x2);

		m_dMaxZ = max(m_dMaxZ, DrawD.z1);
		m_dMaxZ = max(m_dMaxZ, DrawD.z2);
		m_dMinZ = min(m_dMinZ, DrawD.z1);
		m_dMinZ = min(m_dMinZ, DrawD.z2);
	}

	m_aDrawD.FreeExtra();
	m_aPointDrawD.FreeExtra();
	m_aSelectDrawD.FreeExtra();

	DrawMember();
}

void CDgnDrawTool::SelectNode(UINT NodeK1, UINT NodeK2)
{
	m_SelNodeK1 = NodeK1;
	m_SelNodeK2 = NodeK2;
	m_SelElemK  = 0;

	m_bElem = FALSE;

	Draw();
}

void CDgnDrawTool::SelectElem(UINT ElemK)
{
	m_SelNodeK1 = 0;
	m_SelNodeK2 = 0;
	m_SelElemK  = ElemK;

	m_bElem = TRUE;

	Draw();
}

void CDgnDrawTool::DrawMember()
{
	CClientDC dc(this);
	ClearBkgnd(&dc);
	dc.SetBkMode(TRANSPARENT);
	CRect rect;
	GetClientRect(rect);

	double dist_x = m_dMaxX - m_dMinX;
	double dist_y = m_dMaxZ - m_dMinZ;

	// Calculate Scale.
	double dScale = CalcScale(rect, dist_x, dist_y, D_SCALE);
	dScale *= m_dMouseScale;

	// Calculate Starting Point.
	CPoint orPoint = CalcOriginPoint(rect, dScale, dist_x, dist_y);
	orPoint += m_MousePosiPoint;
	rect.DeflateRect(0,0,0,0);

	ABLD_DR DrawD; DrawD.Initialize();
	

	int nPenWidth = 1;
	COLORREF crColor;

	CPen Pen,*pOldPen;
	CRect Rect;

	int nDrawSize = m_aDrawD.GetSize();
	for(int i=0; i<nDrawSize; ++i)
	{
		DrawD = m_aDrawD[i];

		// element
		nPenWidth = 1;
		crColor   = RGB(255,255,0);
		Pen.CreatePen(PS_SOLID,nPenWidth,crColor);
		pOldPen = (CPen*)dc.SelectObject(&Pen);
		dc.MoveTo(int((DrawD.x1-m_dMinX)*dScale+orPoint.x), int(orPoint.y-(DrawD.z1-m_dMinZ)*dScale));
		dc.LineTo(int((DrawD.x2-m_dMinX)*dScale+orPoint.x), int(orPoint.y-(DrawD.z2-m_dMinZ)*dScale));
		dc.SelectObject(pOldPen);
		Pen.DeleteObject();   
	}

	int nSelectDrawSize = m_aSelectDrawD.GetSize();
	for(int i=0; i<nSelectDrawSize; ++i)
	{
		DrawD = m_aSelectDrawD[i];

		// Select element
		nPenWidth = 2;
		crColor   = RGB(0,255,255);
		Pen.CreatePen(PS_SOLID,nPenWidth,crColor);
		pOldPen = (CPen*)dc.SelectObject(&Pen);
		dc.MoveTo(int((DrawD.x1-m_dMinX)*dScale+orPoint.x), int(orPoint.y-(DrawD.z1-m_dMinZ)*dScale));		
		dc.LineTo(int((DrawD.x2-m_dMinX)*dScale+orPoint.x), int(orPoint.y-(DrawD.z2-m_dMinZ)*dScale));
		dc.SelectObject(pOldPen);
		Pen.DeleteObject();   
	}
	
	int nPointSize = m_aPointDrawD.GetSize();
	for(int i=0; i<nPointSize; ++i)
	{
		DrawD = m_aPointDrawD[i];
		
		nPenWidth = 2;
		crColor   = RGB(255,0,0);
		Pen.CreatePen(PS_SOLID,nPenWidth,crColor);
	  pOldPen = (CPen*)dc.SelectObject(&Pen);
		dc.MoveTo(int((DrawD.x1-m_dMinX)*dScale+orPoint.x), int(orPoint.y-(DrawD.z1-m_dMinZ)*dScale));
		Rect.top		= int(orPoint.y-(DrawD.z1-m_dMinZ)*dScale-2);	
	  Rect.bottom	= int(orPoint.y-(DrawD.z1-m_dMinZ)*dScale+2);	
	  Rect.left		= int((DrawD.x1-m_dMinX)*dScale+orPoint.x-2);
	  Rect.right	= int((DrawD.x1-m_dMinX)*dScale+orPoint.x+2);
	  dc.Ellipse(Rect);
		dc.SelectObject(pOldPen);
		Pen.DeleteObject();	
	}

	if(nDrawSize > 0 && nPointSize > 0)
	{
		//Center
		nPenWidth = 2;
		crColor   = RGB(0,0,255);
		Pen.CreatePen(PS_SOLID,nPenWidth,crColor);
		pOldPen = (CPen*)dc.SelectObject(&Pen);
		dc.MoveTo(int((0-m_dMinX)*dScale+orPoint.x), int(orPoint.y-(0-m_dMinZ)*dScale));
		Rect.top		= int(orPoint.y-(0-m_dMinZ)*dScale-2);	
		Rect.bottom	= int(orPoint.y-(0-m_dMinZ)*dScale+2);	
		Rect.left		= int((0-m_dMinX)*dScale+orPoint.x-2);
		Rect.right	= int((0-m_dMinX)*dScale+orPoint.x+2);
		dc.Ellipse(Rect);
		dc.SelectObject(pOldPen);
		Pen.DeleteObject();
	}

	DrawPointNum(&dc, orPoint, dScale);  
}


void CDgnDrawTool::DrawPointNum(CDC* pDC, CPoint orPoint, double dScale)
{
	CFont Font, *pOldFont;
	Font.CreateFont(13,0,0,0,0,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
					        CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T("Arial"));

	pOldFont=(CFont*)pDC->SelectObject(&Font);

	CString strNodeNo=_T("");

	ABLD_DR DrawD; 
	DrawD.Initialize();
	int nSize = m_aPointDrawD.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		DrawD = m_aPointDrawD[i];
		strNodeNo.Format(_T("%d"), DrawD.nNode1);
		double dX = DrawD.x1 - m_dMinX;
		double dZ = DrawD.z1 - m_dMinZ;
		pDC->TextOut(int(dX*dScale+orPoint.x),	int(orPoint.y-dZ*dScale-12), strNodeNo);	    
	}	
}

void CDgnDrawTool::DrawNodeNum(CDC* pDC, CPoint orPoint, double dScale)
{
	CFont Font, *pOldFont;
	Font.CreateFont(13,0,0,0,0,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
					        CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T("Arial"));

	pOldFont=(CFont*)pDC->SelectObject(&Font);

	CString strNodeNo=_T("");

	ABLD_DR DrawD; DrawD.Initialize();
	int nSize = m_aDrawD.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		DrawD = m_aDrawD[i];
		strNodeNo.Format(_T("%d"), DrawD.nNode1);
		double dX = DrawD.x1 - m_dMinX;
		double dZ = DrawD.z1 - m_dMinZ;
		pDC->TextOut(int(dX*dScale+orPoint.x),	int(orPoint.y-dZ*dScale-12), strNodeNo);	

		strNodeNo.Format(_T("%d"), DrawD.nNode2);
		dX = DrawD.x2 - m_dMinX;
		dZ = DrawD.z2 - m_dMinZ;
		pDC->TextOut(int(dX*dScale+orPoint.x),	int(orPoint.y-dZ*dScale-12), strNodeNo);	
	}
	pDC->SelectObject(pOldFont);	
	Font.DeleteObject();
}

void CDgnDrawTool::DrawElemNum(CDC* pDC, CPoint orPoint, double dScale)
{
	CFont Font, *pOldFont;
	Font.CreateFont(13,0,0,0,0,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
					        CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T("Arial"));
	pOldFont=(CFont*)pDC->SelectObject(&Font);
	
	CString strElemNo = _T("");
	ABLD_DR DrawD; DrawD.Initialize();
	int nSize = m_aDrawD.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		DrawD = m_aDrawD[i];
		strElemNo.Format(_T("%d"), DrawD.nElem);
	  double dStaX = DrawD.x1;
	  double dStaZ = DrawD.z1;
	  double dEndX = DrawD.x2;
	  double dEndZ = DrawD.z2;
	  double dPi   = 4.0*atan(1.0);	// dPi.
	  double dSlope=0.0;
	  if(dEndX==dStaX)	dSlope = dPi/2.0;
	  else							dSlope = atan((dEndZ-dStaZ)/(dEndX-dStaX));	// Radian.

	  int iX=0, iZ=0;
			   if(dSlope >= -dPi/2.0 && dSlope < -dPi/2.5)	{iX =  2;	iZ =  -6;}	// -90 ~ -75 Deg.
	  else if(dSlope >= -dPi/2.5 && dSlope < -dPi/12.)	{iX =  0;	iZ = -12;}	// -75 ~ -15 Deg.
	  else if(dSlope >= -dPi/12. && dSlope <= dPi/12.)	{iX = -3;	iZ = -12;}	// -15 ~ +15 Deg.	
	  else if(dSlope >   dPi/12. && dSlope <= dPi/2.5)	{iX = -6;	iZ = -12;}	// +30 ~ +75 Deg.
	  else if(dSlope >   dPi/2.5 && dSlope <= dPi/2.0)	{iX =  2;	iZ =  -6;}	// +75 ~ +90 Deg.

		double dX = (dStaX+dEndX)/2.0 - m_dMinX;
	  double dZ = (dStaZ+dEndZ)/2.0 - m_dMinZ;
	  
	  BOOL bOkLeng	 = (sqrt(pow(dStaX-dEndX,2)+pow(dStaZ-dEndZ,2)) > 0.0);
	  BOOL bRealElem = (DrawD.nElem >= 0);
	  if(bOkLeng && bRealElem)
			pDC->TextOut(int(dX*dScale+orPoint.x)+iX, int(orPoint.y-dZ*dScale)+iZ, strElemNo);
	}
	
	// Restore Font.
	pDC->SelectObject(pOldFont);
	Font.DeleteObject();
}

void CDgnDrawTool::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Draw Member.
	Draw();
}

void CDgnDrawTool::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDn = TRUE;	
	m_MouseDownPoint = point;
	SetCapture();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CDgnDrawTool::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDn = FALSE;	
	m_MouseDownPoint = CPoint(0,0);
	ReleaseCapture();	
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CDgnDrawTool::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bRButtonDn = TRUE;	
	m_MouseDownPoint = point;
	SetCapture();

	CWnd::OnRButtonDown(nFlags, point);
}

void CDgnDrawTool::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bRButtonDn = FALSE;	
	m_MouseDownPoint = CPoint(0,0);
	ReleaseCapture();	
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CDgnDrawTool::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(GetCapture()==this)
	{
		if(m_bLButtonDn)
		{
			CPoint ClickPoint(0,0);
			ClickPoint.x = point.x - m_MouseDownPoint.x;
			ClickPoint.y = point.y - m_MouseDownPoint.y;
			m_MousePosiPoint += ClickPoint;
			Invalidate(TRUE);
			m_MouseDownPoint = point;
		}
		else if(m_bRButtonDn)
		{
			double dScaleX = (m_MouseDownPoint.x-point.x)/(double)200;
			double dScaleY = (m_MouseDownPoint.y-point.y)/(double)200;
			double dScale = (fabs(dScaleX) > fabs(dScaleY) ? dScaleX : dScaleY);
			m_dMouseScale += dScale;
			if(m_dMouseScale < 0.5) m_dMouseScale = 0.5;
			Invalidate(TRUE);
			m_MouseDownPoint = point;
		}
	}	
	CWnd::OnMouseMove(nFlags, point);
}