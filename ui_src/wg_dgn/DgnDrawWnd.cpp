// DgnDrawWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DgnDrawWnd.h"

#include "Math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF Diffcolor_Red    =RGB(255,  0,  0);
const COLORREF Diffcolor_Green  =RGB(  0,255,  0);
const COLORREF Diffcolor_BLUE   =RGB(  0,  0,255);
const COLORREF Diffcolor_YELLOW =RGB(255,255,  0);
const COLORREF Diffcolor_DARKGRAY =RGB(169,169,169);
const COLORREF Diffcolor_GRAY     =RGB(128,128,128);

/////////////////////////////////////////////////////////////////////////////
// CDgnDrawWnd

CDgnDrawWnd::CDgnDrawWnd()
{
}

CDgnDrawWnd::~CDgnDrawWnd()
{
}


BEGIN_MESSAGE_MAP(CDgnDrawWnd, CWnd)
	//{{AFX_MSG_MAP(CDgnDrawWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnDrawWnd message handlers

BOOL CDgnDrawWnd::Initial(CWnd* pParentWnd)
{
	HWND hWnd = pParentWnd->GetSafeHwnd();
	CWnd* pSubclassWnd = NULL;
	if ((pSubclassWnd = FromHandlePermanent(pParentWnd->m_hWnd)) != NULL)
		pSubclassWnd->UnsubclassWindow();

	if(!SubclassWindow(hWnd))
	 {
		 TRACE(_T("Fail To Subclassing\n"));
		 return FALSE;
	 }

	LONG dwStyle;
	dwStyle = GetWindowLongPtr(m_hWnd,GWL_STYLE);
	dwStyle |=  WS_CLIPCHILDREN;
	SetWindowLongPtr(m_hWnd,GWL_STYLE,dwStyle); 

	return TRUE;
}

void CDgnDrawWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	COLORREF col=RGB(0,0,0);
	if(m_SectionType==eDGN_BEAM)
	{
		if(m_ShapeType==eDGN_RECT)		DrawRectangleBeam(&dc,col,m_Hc,m_Bc);
		else if(m_ShapeType==eDGN_TEE)			DrawTeeBeam(&dc,col,m_Hc,m_Bc,m_Hf,m_Bf);
	}
	else if(m_SectionType==eDGN_COLM)
	{
		if(m_ShapeType==eDGN_RECT)		DrawRectangleColumn(&dc,col,m_Hc,m_Bc);
		else if(m_ShapeType==eDGN_CIR)	DrawCircle(&dc,col,m_Hc);
		else if(m_ShapeType==eDGN_PIPE)		DrawCircle(&dc,col,m_Hc,m_Bc);
	}
	else if(m_SectionType==eSRC_BEAM)
	{
		if(m_ShapeType==eSRC_RHB)		DrawRHBShapeBeam(&dc,col,m_Hc,m_Bc);
	}
}

BOOL CDgnDrawWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush backBrush(RGB(255,255,255));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	GetClientRect(&rect);
	pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
	pDC->SelectObject(pOldBrush);
	backBrush.DeleteObject();

	return CWnd::OnEraseBkgnd(pDC);
}

void CDgnDrawWnd::DrawLine(CDC* pDC,COLORREF color,int Width,int x1,int y1,int x2,int y2)
{
	CPen pen(PS_SOLID,Width,color);
	int nSavedDC = pDC->SaveDC();
	pDC->SelectObject(&pen);

	//  Line을 그리자 !!!
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);

	pDC->RestoreDC(nSavedDC);
}

void CDgnDrawWnd::DrawCircle(CDC* pDC,COLORREF color,int Width,int x1,int y1,int x2,int y2)
{
	CPen pen(PS_SOLID,Width,color);
	int nSavedDC = pDC->SaveDC();
	pDC->SelectObject(&pen);

	//  Ellipse을 그리자 !!!
	pDC->Ellipse(x1,y1,x2,y2);

	pDC->RestoreDC(nSavedDC);
}

void CDgnDrawWnd::DrawRebar(CDC* pDC,COLORREF color,int Width,double d,double x,double y)
{
	//CPen pen(PS_SOLID,Width,color);
	CPen pen(PS_INSIDEFRAME,Width,color);
	int nSavedDC = pDC->SaveDC();
	pDC->SelectObject(&pen);

	double X,Y;
	X = d/2.;
	Y = d/2.;

	int x1,x2,y1,y2;
	x1 = (int)(x-X);
	x2 = (int)(x+X);
	y1 = (int)(y-Y);
	y2 = (int)(y+Y);

	pDC->Ellipse(x1,y1,x2,y2);

	pDC->RestoreDC(nSavedDC);
}

void CDgnDrawWnd::DrawRectangleBeam(CDC* pDC,COLORREF color,double hc,double bc)
{
	//  Rectangle을 그리자 !!!
	CRect rect;
	GetClientRect(&rect);
	//  영역의 중심을 계산하자 !!!
	double CenterX = (rect.right-rect.left)/2.;
	double CenterY = (rect.bottom-rect.top)/2.;
	//  Scale_Factor를 계산합니다.
	double Scale_Factor = Calc_ScaleFactor(rect,bc,hc,0.65);

	double X = bc*Scale_Factor/2.;
	double Y = hc*Scale_Factor/2.;

	int x1,x2,y1,y2;
	x1 = (int)(CenterX-X);
	x2 = (int)(CenterX+X);
	y1 = (int)(CenterY-Y);
	y2 = (int)(CenterY+Y);

	double D,D1=10.;
	if(m_Dt > m_Db) D  = m_Db;
	else            D  = m_Dt;

	double sLenX,sLenYb,sLenYt,n1x,n2x,n1y1,n2y1,n1y2,n2y2,RebarSize=6;
	sLenX  = (bc-2.*D)*Scale_Factor;
	sLenYb = (hc-2.*m_Db)*Scale_Factor;
	sLenYt = (hc-2.*m_Dt)*Scale_Factor;

	if(sLenX  < 0.0 || sLenYb < 0.0 || sLenYt < 0.0) return ;

	//  Rectangle의 외곽선을 그립니다.
	DrawLine(pDC,color,2,x1,y1,x2,y1);
	DrawLine(pDC,color,2,x2,y1,x2,y2);
	DrawLine(pDC,color,2,x2,y2,x1,y2);
	DrawLine(pDC,color,2,x1,y2,x1,y1);

	//  띠철근을 그립니다.
	x1 = (int)(CenterX-sLenX/2.-RebarSize/2.);
	x2 = (int)(CenterX+sLenX/2.+RebarSize/2.);
	y1 = (int)(CenterY-sLenYt/2.-RebarSize/2.);
	y2 = (int)(CenterY+sLenYb/2.+RebarSize/2.);

	DrawRoundRebar(pDC,color,1,x1,y1,x2,y2,(int)(RebarSize/2.));

	n1x  = CenterX-sLenX/2.;
	n2x  = CenterX+sLenX/2.;
	n1y1 = CenterY+sLenYb/2.;   //  Bottom
	n2y1 = CenterY-sLenYt/2.;   //  Top
	n1y2 = n1y1-D1;             //  Bottom
	n2y2 = n2y1+D1;             //  Top

	//  가로철근을 그립니다.
	//  Top-One Array의 철근을 그립니다.
	double nColDis = sLenX/(m_Nt-1);
	for(int i=1; i<=m_Nt; i++)
	{
		if(m_Nt != 1)
		{
			y1 = (int)n2y1;
			x1 = (int)(n1x+(i-1)*nColDis);
		}
		else
		{
			y1 = (int)n2y1;
			x1 = (int)CenterX;
		}
		DrawRebar(pDC,color,5,RebarSize,x1,y1);
	}

	if(m_TArrayType == 1)
	{
		//  Top-Two Array의 철근을 그립니다.
		nColDis = sLenX/(m_Nt2-1);
		for(int i=1; i<=m_Nt2; i++)
		{
			if(m_Nt2 != 1)
			{
				y1 = (int)n2y2;
				x1 = (int)(n1x+(i-1)*nColDis);
			}
			else
			{
				y1 = (int)n2y2;
				x1 = (int)CenterX;
			}
			if(m_bDiffColor)
				DrawRebar(pDC,Diffcolor_Red,5,RebarSize,x1,y1);
			else
				DrawRebar(pDC,color,5,RebarSize,x1,y1);        
		}
	}

	//  Bottom-One Array의 철근을 그립니다.
	nColDis = sLenX/(m_Nb-1);
	for(int i=1; i<=m_Nb; i++)
	{
		if(m_Nb != 1)
		{
			y1 = (int)n1y1;
			x1 = (int)(n1x+(i-1)*nColDis);
		}
		else
		{
			y1 = (int)n1y1;
			x1 = (int)CenterX;
		}
		DrawRebar(pDC,color,5,RebarSize,x1,y1);
	}

	if(m_BArrayType == 1)
	{
		//  Bottom-Two Array의 철근을 그립니다.
		nColDis = sLenX/(m_Nb2-1);
		for(int i=1; i<=m_Nb2; i++)
		{
			if(m_Nb2 != 1)
			{
				y1 = (int)n1y2;
				x1 = (int)(n1x+(i-1)*nColDis);
			}
			else
			{
				y1 = (int)n1y2;
				x1 = (int)CenterX;
			}
			if(m_bDiffColor)
				DrawRebar(pDC,Diffcolor_Red,5,RebarSize,x1,y1);
			else
				DrawRebar(pDC,color,5,RebarSize,x1,y1);
		}
	}

	if(m_Ns > 0)
	{
			double dns2 = (m_TArrayType == 1 && m_Nt2>0) ? n2y2 : n2y1;
			double dns1 = (m_BArrayType == 1 && m_Nb2>0) ? n1y2 : n1y1;
			double dStep = (dns1-dns2) / (m_Ns + 1);

			for(int i=1; i<=m_Ns; i++)
			{
					y1 = (int)(dns2 + dStep*i);
					x1 = (int)(n1x);
					DrawRebar(pDC,Diffcolor_GRAY,4,RebarSize,x1,y1);

					y1 = (int)(dns2 + dStep*i);
					x1 = (int)(n1x + sLenX);
					DrawRebar(pDC,Diffcolor_GRAY,4,RebarSize,x1,y1);
			}
	}

	//  좌표축을 그리자 !!!
	DrawMainAxis(pDC,color,1,rect,(int)CenterX,(int)CenterY);
}

void CDgnDrawWnd::DrawRHBShapeBeam(CDC* pDC,COLORREF color,double hc,double bc)
{
	//  Rectangle을 그리자 !!!
	CRect rect;
	GetClientRect(&rect);
	//  영역의 중심을 계산하자 !!!
	double CenterX = (rect.right-rect.left)/2.;
	double CenterY = (rect.bottom-rect.top)/2.;
	//  Scale_Factor를 계산합니다.
	double Scale_Factor = Calc_ScaleFactor(rect,bc,hc,0.65);

	double X = bc*Scale_Factor/2.;
	double Y = hc*Scale_Factor/2.;

	int x1,x2,y1,y2;
	x1 = (int)(CenterX-X);
	x2 = (int)(CenterX+X);
	y1 = (int)(CenterY-Y);
	y2 = (int)(CenterY+Y);

	//  Rectangle의 외곽선을 그립니다.
	DrawLine(pDC,color,2,x1,y1,x2,y1);
	DrawLine(pDC,color,2,x2,y1,x2,y2);
	DrawLine(pDC,color,2,x2,y2,x1,y2);
	DrawLine(pDC,color,2,x1,y2,x1,y1);

	double D,D1=10.;
	if(m_Dt > m_Db) D  = m_Db;
	else            D  = m_Dt;

	double sLenX,sLenYb,sLenYt,n1x,n2x,n1y1,n2y1,n1y2,n2y2,RebarSize=6;
	sLenX  = (bc-2.*D)*Scale_Factor;
	sLenYb = (hc-2.*m_Db)*Scale_Factor;
	sLenYt = (hc-2.*m_Dt)*Scale_Factor;

	//  띠철근을 그립니다.
	x1 = (int)(CenterX-sLenX/2.-RebarSize/2.);
	x2 = (int)(CenterX+sLenX/2.+RebarSize/2.);
	y1 = (int)(CenterY-sLenYt/2.-RebarSize/2.);
	y2 = (int)(CenterY+sLenYb/2.+RebarSize/2.);

	DrawRoundRebar(pDC,color,1,x1,y1,x2,y2,(int)(RebarSize/2.));

	n1x  = CenterX-sLenX/2.;
	n2x  = CenterX+sLenX/2.;
	n1y1 = CenterY+sLenYb/2.;   //  Bottom
	n2y1 = CenterY-sLenYt/2.;   //  Top
	n1y2 = n1y1-D1;             //  Bottom
	n2y2 = n2y1+D1;             //  Top

	//  가로철근을 그립니다.
	//  Top-One Array의 철근을 그립니다.
	double nColDis = sLenX/(m_Nt-1);
	for(int i=1; i<=m_Nt; i++)
	{
		if(m_Nt != 1)
		{
			y1 = (int)n2y1;
			x1 = (int)(n1x+(i-1)*nColDis);
		}
		else
		{
			y1 = (int)n2y1;
			x1 = (int)CenterX;
		}
		DrawRebar(pDC,color,5,RebarSize,x1,y1);
	}

	if(m_TArrayType == 1)
	{
		//  Top-Two Array의 철근을 그립니다.
		nColDis = sLenX/(m_Nt2-1);
		for(int i=1; i<=m_Nt2; i++)
		{
			if(m_Nt2 != 1)
			{
				y1 = (int)n2y2;
				x1 = (int)(n1x+(i-1)*nColDis);
			}
			else
			{
				y1 = (int)n2y2;
				x1 = (int)CenterX;
			}
			DrawRebar(pDC,color,5,RebarSize,x1,y1);
		}
	}

	//  Bottom-One Array의 철근을 그립니다.
	nColDis = sLenX/(m_Nb-1);
	for(int i=1; i<=m_Nb; i++)
	{
		if(m_Nb != 1)
		{
			y1 = (int)n1y1;
			x1 = (int)(n1x+(i-1)*nColDis);
		}
		else
		{
			y1 = (int)n1y1;
			x1 = (int)CenterX;
		}
		DrawRebar(pDC,color,5,RebarSize,x1,y1);
	}

	if(m_BArrayType == 1)
	{
		//  Bottom-Two Array의 철근을 그립니다.
		nColDis = sLenX/(m_Nb2-1);
		for(int i=1; i<=m_Nb2; i++)
		{
			if(m_Nb2 != 1)
			{
				y1 = (int)n1y2;
				x1 = (int)(n1x+(i-1)*nColDis);
			}
			else
			{
				y1 = (int)n1y2;
				x1 = (int)CenterX;
			}
			DrawRebar(pDC,color,5,RebarSize,x1,y1);
		}
	}

	//  H 형강을 그립니다.
	double HX1=0.0, HX2=0.0, HY=0.0, TW=0.0, TF=0.0, TF2=0.0;
	HX1 = m_B1*Scale_Factor/2.;
	HX2 = m_B2*Scale_Factor/2.;
	if(m_Tf2 == 0.0)
		HY = (m_H-2*m_Tf)*Scale_Factor/2.;
	else
		HY = (m_H-m_Tf-m_Tf2)*Scale_Factor/2.;

	TW = m_Tw*Scale_Factor;
	TF = m_Tf*Scale_Factor;
	TF2 = m_Tf2*Scale_Factor;

	int Hx0, Hx1,Hx2, Hx3,Hx4,Hy1,Hy2, Tw=0.0, Tf=0.0, Tf2=0.0;
	Hx0 = (int)(CenterX);
	Hx1 = (int)(CenterX-HX1);
	Hx2 = (int)(CenterX+HX1);
	Hx3 = (int)(CenterX-HX2);
	Hx4 = (int)(CenterX+HX2);
	Hy1 = (int)(CenterY-HY);
	Hy2 = (int)(CenterY+HY);
	Tw = (int)(TW);
	Tf = (int)(TF);
	Tf2 = (int)(TF2);
	if(Tw < 2) Tw = 2;
	if(Tf < 2) Tf = 2;
	if(Tf2 < 2) Tf2 = 2;

	DrawLine(pDC,color,Tf,Hx1,Hy1,Hx2,Hy1);
	DrawLine(pDC,color,Tf2,Hx3,Hy2,Hx4,Hy2);
	DrawLine(pDC,color,Tw,Hx0,Hy2,Hx0,Hy1);

	//  좌표축을 그리자 !!!
	DrawMainAxis(pDC,color,1,rect,(int)CenterX,(int)CenterY);
}

void CDgnDrawWnd::DrawRectangleColumn(CDC* pDC,COLORREF color,double hc,double bc)
{
	//  Rectangle을 그리자 !!!
	CRect rect;
	GetClientRect(&rect);
	//  영역의 중심을 계산하자 !!!
	double CenterX = (rect.right-rect.left)/2.;
	double CenterY = (rect.bottom-rect.top)/2.;
	//  Scale_Factor를 계산합니다.
	double Scale_Factor = Calc_ScaleFactor(rect,bc,hc,0.65);

	double X = bc*Scale_Factor/2.;
	double Y = hc*Scale_Factor/2.;

	int x1,x2,y1,y2;
	x1 = (int)(CenterX-X);
	x2 = (int)(CenterX+X);
	y1 = (int)(CenterY-Y);
	y2 = (int)(CenterY+Y);

	//  Rectangle의 외곽선을 그립니다.
	DrawLine(pDC,color,2,x1,y1,x2,y1);
	DrawLine(pDC,color,2,x2,y1,x2,y2);
	DrawLine(pDC,color,2,x2,y2,x1,y2);
	DrawLine(pDC,color,2,x1,y2,x1,y1);

	double sLenX,sLenY,n1x,n2x,n1y,n2y,RebarSize=6;
	sLenX = (bc-2.*m_D0)*Scale_Factor/2.;
	sLenY = (hc-2.*m_D0)*Scale_Factor/2.;

	//  띠철근을 그립니다.
	x1 = (int)(CenterX-sLenX-RebarSize/2.);
	x2 = (int)(CenterX+sLenX+RebarSize/2.);
	y1 = (int)(CenterY-sLenY-RebarSize/2.);
	y2 = (int)(CenterY+sLenY+RebarSize/2.);

	DrawRoundRebar(pDC,color,1,x1,y1,x2,y2,(int)(RebarSize/2.));

	//  세로철근을 그립니다.
	n1x = CenterX-sLenX;
	n2x = CenterX+sLenX;
	n1y = CenterY+sLenY;
	n2y = CenterY-sLenY;
	double nRowDis = sLenY*2./(m_Nrow-1);
	for(int i=0; i<m_Nrow; i++)
	{
		x1 = (int)n1x;
		x2 = (int)n2x;
		y1 = (int)(n1y-i*nRowDis);

		if(m_bDiffColor && (i==0 || i==(m_Nrow-1)))
		{
			DrawRebar(pDC,Diffcolor_Red,5,RebarSize,x1,y1);
			DrawRebar(pDC,Diffcolor_Red,5,RebarSize,x2,y1);
		}
		else
		{
			DrawRebar(pDC,color,5,RebarSize,x1,y1);
			DrawRebar(pDC,color,5,RebarSize,x2,y1);
		}
	}

	//  가로철근을 그립니다.
	int nCol=(m_Nqrb-2*m_Nrow)/2;
	double nColDis = sLenX*2./(nCol+1);
	for(int i=1; i<=nCol; i++)
	{
		y1 = (int)n1y;
		y2 = (int)n2y;
		x1 = (int)(n1x+i*nColDis);

		DrawRebar(pDC,color,5,RebarSize,x1,y1);
		DrawRebar(pDC,color,5,RebarSize,x1,y2);
	}

	//  좌표축을 그리자 !!!
	DrawMainAxis(pDC,color,1,rect,(int)CenterX,(int)CenterY);
}

void CDgnDrawWnd::DrawMainAxis(CDC* pDC,COLORREF color,int Width,CRect Rect,int Cx,int Cy)
{
	int D=10;
	DrawLine(pDC,color,Width,Rect.left+D,Cy,Rect.right-D,Cy);
	DrawLine(pDC,color,Width,Cx,Rect.bottom-D,Cx,Rect.top+D);

	DrawText(pDC,color,Width,Rect.right-15,Cy-10,_T("y"));
	DrawText(pDC,color,Width,Cx-3,Rect.top+3,_T("z"));
}

void CDgnDrawWnd::DrawText(CDC* pDC,COLORREF color,int Width,int x,int y,CString str)
{
	CPen pen(PS_SOLID,Width,color);
	int nSavedDC = pDC->SaveDC();
	pDC->SelectObject(&pen);

	//  String를 화면에 찍습니다. !!!
	pDC->TextOut(x,y,str);

	pDC->RestoreDC(nSavedDC);
}

void CDgnDrawWnd::DrawRoundRebar(CDC* pDC,COLORREF color,int Width,int X1,int Y1,int X2,int Y2,int ReSize)
{
	CPen pen(PS_SOLID,Width,color);
	int nSavedDC = pDC->SaveDC();
	pDC->SelectObject(&pen);

	//  Round Rectangle을 그리자 !!!
	pDC->RoundRect(X1,Y1,X2,Y2,ReSize,ReSize);

	pDC->RestoreDC(nSavedDC);
}

void CDgnDrawWnd::DrawCircle(CDC* pDC, COLORREF color, double dd, double db/*0.0*/)
{
	// Draw Circle/Pipe.
	CRect rect;
	GetClientRect(&rect);
	// Calculate Region.
	double CenterX = (rect.right-rect.left)/2.;
	double CenterY = (rect.bottom-rect.top)/2.;
	// Calculate Scale_Factor.
	double Scale_Factor = Calc_ScaleFactor(rect,0.,dd,0.65);

	double X = dd*Scale_Factor/2.;
	double Y = dd*Scale_Factor/2.;

	int x1,x2,y1,y2,RebarSize=6;
	x1 = (int)(CenterX-X);
	x2 = (int)(CenterX+X);
	y1 = (int)(CenterY-Y);
	y2 = (int)(CenterY+Y);

	// Draw outline of Circle.
	DrawCircle(pDC,color,2,x1,y1,x2,y2);
	// Add by ZINU.('01.8.16). For Pipe.
	if(db > 0.0)
	{
		double dXin = (dd-2.*db)*Scale_Factor/2.;
		double dYin = (dd-2.*db)*Scale_Factor/2.;
		int ix1in = (int)(CenterX-dXin);
		int ix2in = (int)(CenterX+dXin);
		int iy1in = (int)(CenterY-dYin);
		int iy2in = (int)(CenterY+dYin);
	  DrawCircle(pDC,color,2,ix1in,iy1in,ix2in,iy2in);
	}

	// Draw ties/spirals.
	double LenX = X-m_D0*Scale_Factor;

	x1 = (int)(CenterX-LenX-RebarSize/2.);
	x2 = (int)(CenterX+LenX+RebarSize/2.);
	y1 = (int)(CenterY-LenX-RebarSize/2.);
	y2 = (int)(CenterY+LenX+RebarSize/2.);

	DrawCircle(pDC,color,1,x1,y1,x2,y2);

	// Draw Main Bar.
	double Angle,radAngle,Pi=3.141592;
	Angle = 360./m_Nqrb;
	x1 = (int)(CenterX+LenX);
	y1 = (int)(CenterY);
	for(int i=0; i<m_Nqrb; i++)
	{
		radAngle = Angle*Pi*i/180.;
		x1 = (int)(CenterX+LenX*cos(radAngle));
		y1 = (int)(CenterY-LenX*sin(radAngle));

		DrawRebar(pDC,color,5,RebarSize,x1,y1);
	}
	// Draw Axis.
	DrawMainAxis(pDC,color,1,rect,(int)CenterX,(int)CenterY);
}

void CDgnDrawWnd::DrawTeeBeam(CDC* pDC,COLORREF color,double hc,double bc,double hf,double bf)
{
	//  Tee를 그리자 !!!
	CRect rect;
	GetClientRect(&rect);
	//  영역의 중심을 계산하자 !!!
	double CenterX = (rect.right-rect.left)/2.;
	double CenterY = (rect.bottom-rect.top)/2.;
	//  Scale_Factor를 계산합니다.
	double Scale_Factor = Calc_ScaleFactor(rect,bf,hc,0.65);

	double X1 = bf*Scale_Factor/2.;
	double X2 = bc*Scale_Factor/2.;
	double Y1 = hc*Scale_Factor/2.;
	double Y2 = hf*Scale_Factor;

	int x1,x2,x3,x4,y1,y2,y3;

	x1 = (int)(CenterX-X1);
	x2 = (int)(CenterX+X1);
	x3 = (int)(CenterX+X2);
	x4 = (int)(CenterX-X2);
	y1 = (int)(CenterY-Y1);
	y2 = (int)(y1+Y2);
	y3 = (int)(CenterY+Y1);

	//  Tee의 외곽선을 그립니다.
	DrawLine(pDC,color,2,x1,y1,x2,y1);
	DrawLine(pDC,color,2,x2,y1,x2,y2);
	DrawLine(pDC,color,2,x2,y2,x3,y2);
	DrawLine(pDC,color,2,x3,y2,x3,y3);
	DrawLine(pDC,color,2,x3,y3,x4,y3);
	DrawLine(pDC,color,2,x4,y3,x4,y2);
	DrawLine(pDC,color,2,x4,y2,x1,y2);
	DrawLine(pDC,color,2,x1,y2,x1,y1);

	double D,D1=10.;
	if(m_Dt > m_Db) D  = m_Db;
	else            D  = m_Dt;

	double sLenX,sLenYb,sLenYt,n1x,n2x,n1y1,n2y1,n1y2,n2y2,RebarSize=6;
	sLenX  = (bc-2.*D)*Scale_Factor;
	sLenYb = (hc-2.*m_Db)*Scale_Factor;
	sLenYt = (hc-2.*m_Dt)*Scale_Factor;

	//  띠철근을 그립니다.
	x1 = (int)(CenterX-sLenX/2.-RebarSize/2.);
	x2 = (int)(CenterX+sLenX/2.+RebarSize/2.);
	y1 = (int)(CenterY-sLenYt/2.-RebarSize/2.);
	y2 = (int)(CenterY+sLenYb/2.+RebarSize/2.);

	DrawRoundRebar(pDC,color,1,x1,y1,x2,y2,(int)(RebarSize/2.));

	n1x  = CenterX-sLenX/2.;
	n2x  = CenterX+sLenX/2.;
	n1y1 = CenterY+sLenYb/2.;   //  Bottom
	n2y1 = CenterY-sLenYt/2.;   //  Top
	n1y2 = n1y1-D1;             //  Bottom
	n2y2 = n2y1+D1;             //  Top

	//  가로철근을 그립니다.
	//  Top-One Array의 철근을 그립니다.
	double nColDis = sLenX/(m_Nt-1);
	for(int i=1; i<=m_Nt; i++)
	{
		if(m_Nt != 1)
		{
			y1 = (int)n2y1;
			x1 = (int)(n1x+(i-1)*nColDis);
		}
		else
		{
			y1 = (int)n2y1;
			x1 = (int)CenterX;
		}
		DrawRebar(pDC,color,5,RebarSize,x1,y1);
	}

	if(m_TArrayType == 1)
	{
		//  Top-Two Array의 철근을 그립니다.
		nColDis = sLenX/(m_Nt2-1);
		for(int i=1; i<=m_Nt2; i++)
		{
			if(m_Nt2 != 1)
			{
				y1 = (int)n2y2;
				x1 = (int)(n1x+(i-1)*nColDis);
			}
			else
			{
				y1 = (int)n2y2;
				x1 = (int)CenterX;
			}
			DrawRebar(pDC,color,5,RebarSize,x1,y1);
		}
	}

	//  Bottom-One Array의 철근을 그립니다.
	nColDis = sLenX/(m_Nb-1);
	for(int i=1; i<=m_Nb; i++)
	{
		if(m_Nb != 1)
		{
			y1 = (int)n1y1;
			x1 = (int)(n1x+(i-1)*nColDis);
		}
		else
		{
			y1 = (int)n1y1;
			x1 = (int)CenterX;
		}
		DrawRebar(pDC,color,5,RebarSize,x1,y1);
	}

	if(m_BArrayType == 1)
	{
		//  Bottom-Two Array의 철근을 그립니다.
		nColDis = sLenX/(m_Nb2-1);
		for(int i=1; i<=m_Nb2; i++)
		{
			if(m_Nb2 != 1)
			{
				y1 = (int)n1y2;
				x1 = (int)(n1x+(i-1)*nColDis);
			}
			else
			{
				y1 = (int)n1y2;
				x1 = (int)CenterX;
			}
			DrawRebar(pDC,color,5,RebarSize,x1,y1);
		}
	}

	if(m_Ns > 0)
	{
			double dns2 = (m_TArrayType == 1 && m_Nt2>0) ? n2y2 : n2y1;
			double dns1 = (m_BArrayType == 1 && m_Nb2>0) ? n1y2 : n1y1;
			double dStep = (dns1-dns2) / (m_Ns + 1);

			for(int i=1; i<=m_Ns; i++)
			{
					y1 = (int)(dns2 + dStep*i);
					x1 = (int)(n1x);
					DrawRebar(pDC,Diffcolor_GRAY,4,RebarSize,x1,y1);

					y1 = (int)(dns2 + dStep*i);
					x1 = (int)(n1x + sLenX);
					DrawRebar(pDC,Diffcolor_GRAY,4,RebarSize,x1,y1);
			}
	}

	//  좌표축을 그리자 !!!
	DrawMainAxis(pDC,color,1,rect,(int)CenterX,(int)CenterY);
}

double CDgnDrawWnd::Calc_ScaleFactor(CRect rect,double x,double y,double zoom_ratio/*0.8*/)
{
	double zoom   = zoom_ratio; // Default Value is 0.8
	double scale1 = 0.;
	double scale2 = 0.;

	if(x!=0. && y!=0.)
	{
		scale1=(rect.Width()/x)*zoom;
		scale2=(rect.Height()/y)*zoom;
	}
	else if(x==0. && y!=0.)
	{
		scale2=(rect.Height()/y)*zoom;
		scale1=scale2;
	}
	else if(x!=0. && y==0.)
	{
		scale1=(rect.Width()/x)*zoom;
		scale2=scale1;
	}
	double scale=min(scale1,scale2);

	return scale;
}

void CDgnDrawWnd::Column_DrawData(int Shape,double hc,double bc,int nqrb,int nrow,double d0, BOOL bCorner/*FALSE*/)
{
	m_ShapeType = Shape;
	m_Hc = hc;
	m_Bc = bc;
	m_Hf = 0.;
	m_Bf = 0.;
	m_Nqrb = nqrb;
	m_Nrow = nrow;
	m_D0   = d0;
	m_bDiffColor = bCorner;
}

void CDgnDrawWnd::Brace_DrawData(int Shape,double hc,double bc)
{
	m_ShapeType = Shape;
	m_Hc = hc;
	m_Bc = bc;
	m_Hf = 0.;
	m_Bf = 0.;
	m_bDiffColor = FALSE;
}

void CDgnDrawWnd::Beam_DrawData(int Shape,double hc,double bc,double hf,double bf,int TArray,int BArray,
								int Nb,int Nb2,int Nt,int Nt2,double Dt,double Db, int Ns, BOOL bnotSameTopBot/*FALSE*/)
{
	m_ShapeType = Shape;
	m_Hc = hc;
	m_Bc = bc;
	m_Hf = hf;
	m_Bf = bf;
	m_TArrayType = TArray;
	m_BArrayType = BArray;
	m_Nb  = Nb;
	m_Nb2 = Nb2;
	m_Nt  = Nt;
	m_Nt2 = Nt2;
	m_Dt  = Dt;
	m_Db  = Db;
	m_bDiffColor = bnotSameTopBot;

	m_Ns = Ns;
}

void CDgnDrawWnd::SRCBeam_DrawData(int Shape,double H, double B1, double B2, double Tw, double Tf, double Tf2,double hc,double bc,int TArray,int BArray,
																int Nb,int Nb2,int Nt,int Nt2,double Dt,double Db)
{
	m_ShapeType = Shape;

	m_H = H;
	m_B1 = B1;
	m_B2 = B2;
	m_Tw = Tw;
	m_Tf = Tf;
	m_Tf2 = Tf2;

	m_Hc = hc;
	m_Bc = bc;
	m_TArrayType = TArray;
	m_BArrayType = BArray;
	m_Nb  = Nb;
	m_Nb2 = Nb2;
	m_Nt  = Nt;
	m_Nt2 = Nt2;
	m_Dt  = Dt;
	m_Db  = Db;
	m_bDiffColor = FALSE;

	m_Ns = 0;
}
