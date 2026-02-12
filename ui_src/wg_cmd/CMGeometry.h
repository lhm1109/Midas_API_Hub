// CMGeometry.h: interface for the CCMGeometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMGEOMETRY_H__370CEDC1_23C5_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMGEOMETRY_H__370CEDC1_23C5_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "HeaderPre.h"
///////////////////////////////////////////////////////////////////
struct SCMPoint2D
{
	SCMPoint2D() { x = y = 0.0; }
	SCMPoint2D(double x, double y) { this->x = x; this->y = y; }
	double x, y;
};

struct __MY_EXT_CLASS__ SCMRect2D
{
	SCMRect2D() {}
	SCMRect2D(double minx, double miny, double maxx, double maxy)
	{ min.x = minx; min.y = miny; max.x = maxx; max.y = maxy; }
	double Width() { return max.x - min.x; }
	double Height() { return max.y - min.y; }
	SCMPoint2D min, max;
};

class ID_Geom
{
public:
	int GTYPE_NONE;
	int GTYPE_POINT;
	int GTYPE_LINE;
	int GTYPE_POLYGON;
	int GTYPE_ELLIPSE;
protected:
	ID_Geom()
	{
		GTYPE_NONE  = -1;
		GTYPE_POINT = 1;
	  GTYPE_LINE  = 2;
	  GTYPE_POLYGON = 3;
		GTYPE_ELLIPSE = 4;
	}
public:
	~ID_Geom()
	{

	}
	
	static ID_Geom& Instance()
	{
		static ID_Geom theIDGeom;
		return theIDGeom;
	}
};
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// class CCMGeom2D
class __MY_EXT_CLASS__ CCMGeom2D
{
protected:
	int m_nWhoAmI;
public:
	CCMGeom2D();
	CCMGeom2D(COLORREF PenColor, COLORREF BrushColor);
	virtual ~CCMGeom2D();

	int WhoAmI();
// Attributes
	COLORREF PenColor, BrushColor;

// Overridables
public:
	virtual void Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
										COLORREF PenColor, COLORREF BrushColor);
	virtual SCMRect2D GetMBR();
	virtual void Transfer(SCMPoint2D& offset);
	virtual void Rotate(double angle);
	virtual void Mirror(int axis);

// implementations
protected:
	int RoundUp(double dVal);
};

///////////////////////////////////////////////////////////////////
// class CCMLine
class CCMLine:public CCMGeom2D
{
public:
	CCMLine();
	CCMLine(COLORREF PenColor, COLORREF BrushColor);
	
	virtual ~CCMLine();

	BOOL m_bNullBrush;
	void SetNullBrush(BOOL bNullBrush);
// Attributes
public:
	SCMPoint2D m_pt1, m_pt2;
	int m_nPtList;

// Operations
public:
	void Set(SCMPoint2D &pt1, SCMPoint2D& pt2) 
	{ m_pt1 = pt1; m_pt2 = pt2; }
	void SetPtList(int nPtList) { m_nPtList = nPtList; }

// Overridables
public:
	virtual void Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
										COLORREF PenColor, COLORREF BrushColor);
	virtual SCMRect2D GetMBR();
	virtual void Transfer(SCMPoint2D& offset);
	virtual void Rotate(double angle);
	virtual void Mirror(int axis);

// Implementations
protected:

};

///////////////////////////////////////////////////////////////////
// class CCMPolygon
class CCMPolygon:public CCMGeom2D
{
public:
	CCMPolygon();
	CCMPolygon(COLORREF PenColor, COLORREF BrushColor);

	virtual ~CCMPolygon();

	BOOL m_bNullBrush;
	void SetNullBrush(BOOL bNullBrush);
// Attributes
public:
	CArray<SCMPoint2D, SCMPoint2D&> m_aVertex;

// Operations
public:
	int Add(SCMPoint2D &pt) { return m_aVertex.Add(pt); }

// Overridables
public:
	virtual void Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
										COLORREF PenColor, COLORREF BrushColor);
	virtual SCMRect2D GetMBR();
	virtual void Transfer(SCMPoint2D& offset);
	virtual void Rotate(double angle);
	virtual void Mirror(int axis);


// Implementations
protected:

};

///////////////////////////////////////////////////////////////////
// class CCMEllipse
class CCMEllipse:public CCMGeom2D
{
public:
	CCMEllipse();
	CCMEllipse(COLORREF PenColor, COLORREF BrushColor);

	virtual ~CCMEllipse();

// Attributes
public:
	SCMRect2D m_Rect;

// Operations
public:
	void SetRect(SCMPoint2D min, SCMPoint2D max)
	{ m_Rect.min = min; m_Rect.max = max; }

// Overridables
public:
	virtual void Draw(CDC* pDC, CRect &DrawArea, double scale, CPoint &offset,
										COLORREF PenColor, COLORREF BrushColor);
	virtual SCMRect2D GetMBR() { return m_Rect; }
	virtual void Transfer(SCMPoint2D& offset); 
	virtual void Rotate(double angle);
	virtual void Mirror(int axis);

// Implementations
protected:

};

///////////////////////////////////////////////////////////////////
// class CCMGeom2DArray
class __MY_EXT_CLASS__ CCMGeom2DArray
{
public:
	CCMGeom2DArray();
	virtual ~CCMGeom2DArray();

// Attributes
public:
	CArray<CCMGeom2D*, CCMGeom2D*> m_aGeom2D;

// Operations
public:
	int GetSize() { return m_aGeom2D.GetSize(); }
	int Add(CCMGeom2D* geom) { return m_aGeom2D.Add(geom); }
	int AddDel(CCMGeom2DArray& aGeom);
	CCMGeom2D* GetAt(int nIndex) { return m_aGeom2D.GetAt(nIndex); }
	CCMGeom2D* operator [] (int nIndex) { return m_aGeom2D[nIndex]; }
	void RemoveAll() { m_aGeom2D.RemoveAll(); }
	void RemoveAt(int nIndex, int nCount=1) { m_aGeom2D.RemoveAt(nIndex, nCount); }

	void Transfer(SCMPoint2D &offset);
	void Rotate(double angle);
	void Mirror(int axis);
	void Draw(CDC* pDC, CRect &rect, double scale, CPoint &offset);

// Overridables
public:
	SCMRect2D GetMBR();

// Implementations
protected:

};

#include "HeaderPost.h"

#endif // !defined(AFX_CMGEOMETRY_H__370CEDC1_23C5_11D3_92DE_0000C0B0E6B3__INCLUDED_)
