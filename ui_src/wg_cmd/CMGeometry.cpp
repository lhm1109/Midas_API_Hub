// CMGeometry.cpp: implementation of the CCMGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGeometry.h"

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CCMGeom2D
//////////////////////////////////////////////////////////////////////

CCMGeom2D::CCMGeom2D()
{
	m_nWhoAmI = ID_Geom::Instance().GTYPE_NONE;
}

CCMGeom2D::CCMGeom2D(COLORREF PenColor, COLORREF BrushColor)
{
	CCMGeom2D::PenColor = PenColor;
	CCMGeom2D::BrushColor = BrushColor;
	m_nWhoAmI = ID_Geom::Instance().GTYPE_NONE;
}

CCMGeom2D::~CCMGeom2D()
{

}

int CCMGeom2D::WhoAmI()
{
	return m_nWhoAmI;
}

void CCMGeom2D::Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
										 COLORREF PenColor, COLORREF BrushColor)
{
	pDC; DrawArea; scale; offset; PenColor; BrushColor;
}

SCMRect2D CCMGeom2D::GetMBR()
{
	SCMRect2D mbr;
	return mbr;
}

void CCMGeom2D::Transfer(SCMPoint2D& offset)
{
	offset;
}

void CCMGeom2D::Rotate(double angle)
{
	angle;
}

void CCMGeom2D::Mirror(int axis)
{
	axis;
}

int CCMGeom2D::RoundUp(double dVal)
{
	if(dVal > 0.0)
	   return (int)floor(dVal+(double)0.5);
	else
	   return (int)ceil(dVal-(double)0.5);
}

//////////////////////////////////////////////////////////////////////
// CCMLine
//////////////////////////////////////////////////////////////////////
CCMLine::CCMLine()
{
	m_bNullBrush = FALSE;
	m_nPtList = 0;
	m_nWhoAmI = ID_Geom::Instance().GTYPE_LINE;
}

CCMLine::CCMLine(COLORREF PenColor, COLORREF BrushColor)
		: CCMGeom2D(PenColor, BrushColor) 
{ 
	m_bNullBrush = FALSE; m_nPtList = 0; 
	m_nWhoAmI = ID_Geom::Instance().GTYPE_LINE;
}

CCMLine::~CCMLine()
{

}

void CCMLine::SetNullBrush(BOOL bNullBrush)
{
	m_bNullBrush = bNullBrush;
}

//////////////////////
// Overridables
void CCMLine::Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
											COLORREF PenColor, COLORREF BrushColor)
{
	if (scale <= 0.0000001) return;

	CPoint *pt = new CPoint[2];
	SCMPoint2D v;

	// change real to screen coordinates
	v = m_pt1;
	pt[0].x=RoundUp(v.x*scale)+(int)offset.x;
	pt[0].y=RoundUp(v.y*scale)+(int)offset.y;
	v = m_pt2;
	pt[1].x=RoundUp(v.x*scale)+(int)offset.x;
	pt[1].y=RoundUp(v.y*scale)+(int)offset.y;

	CPen pen(PS_SOLID, 1, PenColor);
	int nSavedDC = pDC->SaveDC();

	pDC->SelectObject(&pen);
	if (m_nPtList == 0)
	{
		pDC->MoveTo(pt[0].x, pt[0].y);
	  pDC->LineTo(pt[1].x, pt[1].y);
	}
	else
	{
		double dDeltaX = pt[1].x - pt[0].x;
		double dDeltaY = pt[1].y - pt[0].y;
		double dPitchX = (m_nPtList > 1) ? dDeltaX / (m_nPtList-1) : 0.0;
		double dPitchY = (m_nPtList > 1) ? dDeltaY / (m_nPtList-1) : 0.0;
		CRect r;
		for (int i = 0; i < m_nPtList; i++)
		{
			r.left    = pt[0].x-1 + (int)((dPitchX)*i);
			r.top     = pt[0].y-1 + (int)((dPitchY)*i);;
			r.right   = pt[0].x+1 + (int)((dPitchX)*i);;
			r.bottom  = pt[0].y+1 + (int)((dPitchY)*i);;
			r.NormalizeRect();
			pDC->Ellipse(r);

		}
	}
	pDC->RestoreDC(nSavedDC);
	delete[] pt;
}

SCMRect2D CCMLine::GetMBR()
{
	SCMRect2D mbr;

	mbr.min.x = mbr.max.x = m_pt1.x;
	mbr.min.y = mbr.max.y = m_pt1.y;
	if (m_pt2.x > mbr.max.x) mbr.max.x = m_pt2.x;
	else if (m_pt2.x < mbr.min.x) mbr.min.x = m_pt2.x;
	if (m_pt2.y > mbr.max.y) mbr.max.y = m_pt2.y;
	else if (m_pt2.y < mbr.min.y) mbr.min.y = m_pt2.y;
	return mbr;
}

void CCMLine::Transfer(SCMPoint2D& offset)
{
	m_pt1.x += offset.x;
	m_pt1.y += offset.y;
	m_pt2.x += offset.x;
	m_pt2.y += offset.y;
}

void CCMLine::Rotate(double angle)
{
	angle = angle*M_PI/180.0;
	double sin_v = sin(angle);
	double cos_v = cos(angle);
	SCMPoint2D tmp;
	tmp = m_pt1;
	m_pt1.x = tmp.x*cos_v - tmp.y*sin_v;
	m_pt1.y = tmp.x*sin_v + tmp.y*cos_v;
	tmp = m_pt2;
	m_pt2.x = tmp.x*cos_v - tmp.y*sin_v;
	m_pt2.y = tmp.x*sin_v + tmp.y*cos_v;
}

void CCMLine::Mirror(int axis)
{
	switch(axis)
	{
	case 0: // x axis mirror
		m_pt1.y = -m_pt1.y;
		m_pt2.y = -m_pt2.y;
		break;
	case 1: // y axis mirror
		m_pt1.x = -m_pt1.x;
		m_pt2.x = -m_pt2.x;
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// CCMPolygon
//////////////////////////////////////////////////////////////////////
CCMPolygon::CCMPolygon()
{
	m_bNullBrush = FALSE;
	m_nWhoAmI = ID_Geom::Instance().GTYPE_POLYGON;
}

CCMPolygon::CCMPolygon(COLORREF PenColor, COLORREF BrushColor)
		: CCMGeom2D(PenColor, BrushColor) 
{ 
	m_bNullBrush = FALSE;
	m_nWhoAmI = ID_Geom::Instance().GTYPE_POLYGON;   
}

CCMPolygon::~CCMPolygon()
{

}

void CCMPolygon::SetNullBrush(BOOL bNullBrush)
{
	m_bNullBrush = bNullBrush;
}

//////////////////////
// Overridables
void CCMPolygon::Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
											COLORREF PenColor, COLORREF BrushColor)
{
	if (scale <= 0.0000001) return;

	int num = m_aVertex.GetSize();
	CPoint *pt = new CPoint[num];
	SCMPoint2D v;

	// change real to screen coordinates
	for(int i = 0; i < num; i++)
	{
		v = m_aVertex[i];
		pt[i].x=RoundUp(v.x*scale)+(int)offset.x;
		pt[i].y=RoundUp(v.y*scale)+(int)offset.y;
	}

	CBrush brush(BrushColor);
	CPen pen(PS_SOLID, 1, PenColor);
	int nSavedDC = pDC->SaveDC();

	/*
	if(m_bNullBrush)
	{
		pDC->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	}
	else
	*/
	{
	  pDC->SelectObject(&brush);
	}
	
	pDC->SelectObject(&pen);
	pDC->Polygon(pt, num);
	pDC->RestoreDC(nSavedDC);
	delete[] pt;
}

SCMRect2D CCMPolygon::GetMBR()
{
	SCMRect2D mbr;
	if (m_aVertex.GetSize() < 1) return mbr;

	mbr.min.x = mbr.max.x = m_aVertex[0].x;
	mbr.min.y = mbr.max.y = m_aVertex[0].y;
	for (int i = 1; i < m_aVertex.GetSize(); i++)
	{
		if (m_aVertex[i].x > mbr.max.x) mbr.max.x = m_aVertex[i].x;
		else if (m_aVertex[i].x < mbr.min.x) mbr.min.x = m_aVertex[i].x;
		if (m_aVertex[i].y > mbr.max.y) mbr.max.y = m_aVertex[i].y;
		else if (m_aVertex[i].y < mbr.min.y) mbr.min.y = m_aVertex[i].y;
	}
	return mbr;
}

void CCMPolygon::Transfer(SCMPoint2D& offset)
{
	for (int i = 0; i < m_aVertex.GetSize(); i++)
	{
		m_aVertex[i].x += offset.x;
		m_aVertex[i].y += offset.y;
	}
}

void CCMPolygon::Rotate(double angle)
{
	angle = angle*M_PI/180.0;
	double sin_v = sin(angle);
	double cos_v = cos(angle);
	SCMPoint2D tmp;
	for (int i = 0; i < m_aVertex.GetSize(); i++)
	{
		tmp = m_aVertex[i];
		m_aVertex[i].x = tmp.x*cos_v - tmp.y*sin_v;
		m_aVertex[i].y = tmp.x*sin_v + tmp.y*cos_v;
	}
}

void CCMPolygon::Mirror(int axis)
{
	int i;
	switch(axis)
	{
	case 0: // x axis mirror
		for (i = 0; i < m_aVertex.GetSize(); i++)
			m_aVertex[i].y = -m_aVertex[i].y;
		break;
	case 1: // y axis mirror
		for (i = 0; i < m_aVertex.GetSize(); i++)
			m_aVertex[i].x = -m_aVertex[i].x;
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// CCMEllipse
//////////////////////////////////////////////////////////////////////
CCMEllipse::CCMEllipse()
{
	this->m_nWhoAmI = ID_Geom::Instance().GTYPE_ELLIPSE;
}

CCMEllipse::CCMEllipse(COLORREF PenColor, COLORREF BrushColor)
		: CCMGeom2D(PenColor, BrushColor) 
{ 
	m_nWhoAmI = ID_Geom::Instance().GTYPE_ELLIPSE;
}

CCMEllipse::~CCMEllipse()
{

}

//////////////////////
// Overridables
void CCMEllipse::Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
											COLORREF PenColor, COLORREF BrushColor)
{
	if (scale <= 0.0000001) return;

	SCMPoint2D v;
	CRect ellipse;

	// change real to screen coordinates
	v = m_Rect.min;
	ellipse.left=RoundUp(v.x*scale)+(int)offset.x;
	ellipse.top=RoundUp(v.y*scale)+(int)offset.y;
	v = m_Rect.max;
	ellipse.right=RoundUp(v.x*scale)+(int)offset.x;
	ellipse.bottom=RoundUp(v.y*scale)+(int)offset.y;

	CBrush brush(BrushColor);
	CPen pen(PS_SOLID, 1, PenColor);
	int nSavedDC = pDC->SaveDC();

	pDC->SelectObject(&brush);
	pDC->SelectObject(&pen);
	pDC->Ellipse(ellipse);
	pDC->RestoreDC(nSavedDC);
}

void CCMEllipse::Transfer(SCMPoint2D& offset)
{
	m_Rect.min.x += offset.x; m_Rect.min.y += offset.y;
	m_Rect.max.x += offset.x; m_Rect.max.y += offset.y;
}

void CCMEllipse::Rotate(double angle)
{
	angle = angle*M_PI/180.0;
	double sin_v = sin(angle);
	double cos_v = cos(angle);
	SCMPoint2D tmp;
	tmp = m_Rect.min;
	m_Rect.min.x = tmp.x*cos_v - tmp.y*sin_v;
	m_Rect.min.y = tmp.x*sin_v + tmp.y*cos_v;
	tmp = m_Rect.max;
	m_Rect.max.x = tmp.x*cos_v - tmp.y*sin_v;
	m_Rect.max.y = tmp.x*sin_v + tmp.y*cos_v;
}

void CCMEllipse::Mirror(int axis)
{
	switch(axis)
	{
	case 0: // x axis mirror
		m_Rect.min.y = -m_Rect.min.y;
		m_Rect.max.y = -m_Rect.max.y;
		break;
	case 1: // y axis mirror
		m_Rect.min.x = -m_Rect.min.x;
		m_Rect.max.x = -m_Rect.max.x;
	}
}


//////////////////////////////////////////////////////////////////////
// CCMGeom2DArray
//////////////////////////////////////////////////////////////////////
CCMGeom2DArray::CCMGeom2DArray()
{

}

CCMGeom2DArray::~CCMGeom2DArray()
{
	// delete geometry instances
	for (int i = 0; i < m_aGeom2D.GetSize(); i++)
		delete m_aGeom2D[i];
}

//////////////////////
// Operations
int CCMGeom2DArray::AddDel(CCMGeom2DArray& aGeom)
{
	for (int i = 0; i < aGeom.GetSize(); i++)
		m_aGeom2D.Add(aGeom.GetAt(i));
	aGeom.RemoveAll();
	return m_aGeom2D.GetSize();
}

void CCMGeom2DArray::Transfer(SCMPoint2D &offset)
{
	for (int i = 0; i < m_aGeom2D.GetSize(); i++)
		m_aGeom2D[i]->Transfer(offset);
}

void CCMGeom2DArray::Rotate(double angle)
{
	for (int i = 0; i < m_aGeom2D.GetSize(); i++)
		m_aGeom2D[i]->Rotate(angle);
}

void CCMGeom2DArray::Mirror(int axis)
{
	for (int i = 0; i < m_aGeom2D.GetSize(); i++)
		m_aGeom2D[i]->Mirror(axis);
}

void CCMGeom2DArray::Draw(CDC* pDC, CRect &rect, double scale, CPoint &offset)
{
	// move origin to center of mbr
	SCMRect2D mbr = GetMBR();
	Transfer(SCMPoint2D(-(mbr.max.x+mbr.min.x)/2.0, -(mbr.max.y+mbr.min.y)/2.0));
	CPoint newOffset = offset;
	newOffset.x += (mbr.max.x-mbr.min.x)/2.0*scale;
	newOffset.y += (mbr.max.y-mbr.min.y)/2.0*scale;

	// draw...
	COLORREF PenColor, BrushColor;
	for (int i = 0; i < m_aGeom2D.GetSize(); i++)
	{
		PenColor = m_aGeom2D[i]->PenColor;
		BrushColor = m_aGeom2D[i]->BrushColor;
		m_aGeom2D[i]->Draw(pDC, rect, scale, newOffset, PenColor, BrushColor);
	}
}

//////////////////////
// Overridables
SCMRect2D CCMGeom2DArray::GetMBR()
{
	SCMRect2D mbr, tmp;
	if (m_aGeom2D.GetSize() < 1) return mbr;

	mbr = m_aGeom2D[0]->GetMBR();
	for (int i = 1; i < m_aGeom2D.GetSize(); i++)
	{
		tmp = m_aGeom2D[i]->GetMBR();
		if (tmp.max.x > mbr.max.x) mbr.max.x = tmp.max.x;
		if (tmp.max.y > mbr.max.y) mbr.max.y = tmp.max.y;
		if (tmp.min.x < mbr.min.x) mbr.min.x = tmp.min.x;
		if (tmp.min.y < mbr.min.y) mbr.min.y = tmp.min.y;
	}
	return mbr;
}
