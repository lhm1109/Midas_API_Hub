// Dgn_Object.h: interface for the CDgn_Object class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGN_OBJECT_H__3835FF36_1E69_4280_8D55_E9F1C8ABBA37__INCLUDED_)
#define AFX_DGN_OBJECT_H__3835FF36_1E69_4280_8D55_E9F1C8ABBA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <float.h>
#include <afxtempl.h>

#define LDOUBLE double
#define LBOOL	  BOOL
#define LINT	  int

const double LGLDBL_EPSILON = DBL_EPSILON * 100;
const double LGL_LINE_INTERSECT_EPSILON = LGLDBL_EPSILON;

class Point3D
{
// Construction
public:
	Point3D();
	virtual ~Point3D();

// Attributes
public:
	double x, y, z;
// Operations
public:
	void Set(double newX, double newY, double newZ) { x = newX; y = newY; z = newZ; }
	int operator == (Point3D& point );

};

enum OBJECT_ID { GOB_NONE = 0, GOB_POINT, GOB_LINE, GOB_PLINE, GOB_ARC, GOB_CIRCLE, 
				 GOB_CLINE, GOB_STORY, GOB_BAY, GOB_BAY_SOURCE, GOB_FBAY,
			     GOB_FBAY_SOURCE};   

class DgnRect;
class DgnOB_Point;
class DgnOB_Line;
class Point3D;

class CDgn_Object
{
// Construction
public:
	CDgn_Object();
	virtual ~CDgn_Object();

// Attributes
public:
	// Variable for Debug
	//BOOL	m_bDoNotDraw;	
	BOOL	m_bSelected;

// Operations
public:
	//virtual void Draw(CDC* pDC, COLORREF nColor) { }
	OBJECT_ID WhoAmI() { return m_nID; }
	virtual Point3D* GetPointForQT(int Index) { return NULL; }
	virtual LDOUBLE NearDist(Point3D FromP) { return 0.0; }

//
protected:
	OBJECT_ID m_nID;
};

class DgnOB_Point : public CDgn_Object
{
// Construction
public:
	DgnOB_Point();
	virtual ~DgnOB_Point();

// Attributes
public:
	Point3D m_ptPos;

// Operations
	//virtual void Draw(CDC* pDC, COLORREF nColor);
	virtual Point3D* GetPointForQT(int Index);
	virtual LDOUBLE NearDist(Point3D FromP);
};

class DgnOB_Line : public CDgn_Object
{
// Construction
public:
	DgnOB_Line();
	virtual ~DgnOB_Line();

// Attributes
public:
	Point3D m_ptPos1, m_ptPos2;

// Operations
	//virtual void Draw(CDC* pDC, COLORREF nColor);
	virtual Point3D* GetPointForQT(int Index);
	virtual LDOUBLE NearDist(Point3D FromP);
};

class DgnOB_Polygon : public CDgn_Object
{
protected:
	CArray <Point3D*, Point3D*> m_VertexList;	
public:
	UINT m_nKey;

public:
	DgnOB_Polygon();
	DgnOB_Polygon(UINT nKey);
	virtual ~DgnOB_Polygon();
	void ClearVertexList();
	int AddVertex(Point3D point);
	int AddVertex(LDOUBLE X,LDOUBLE Y, LDOUBLE Z);
	//주어진 Index에 대한 Vertex Pointer를 반환한다.
	//부당한 Index가 주어지면 NULL을 반환한다. 
	Point3D * GetVertex(int Index);
	LINT     GetVertexNum();
	//Polygon의 생성조건에 부합되는지 여부를 검사 
	//즉 Vertex의 갯수가 3보다 작으면 Polygon을 생성할 수 없으므로 FALSE를 반환한다.  
	LBOOL  IsValid();

	// 주어진 point에 대한 포함 테스트를 수행한다.
	LBOOL IsInner(Point3D point);
	// 주어진 Object에 대한 포함 테스트및 걸침 테스트를 수행한다. 
	LBOOL IsWithin(Point3D point);
	LBOOL IsWithin(DgnOB_Line Line);
	LBOOL IsWithin(CDgn_Object* pObject);
	LBOOL IsIntersect(DgnOB_Line Line);
	LBOOL IsWithin(DgnRect Rect);
	LBOOL IsIntersect(DgnRect Rect);
	LBOOL IsIntersect(CDgn_Object* pObject);

	//virtual void Draw(CDC* pDC, COLORREF nColor);
	virtual Point3D* GetPointForQT(int Index);
};

/***********************************************
@@	LDOUBLE형 Rectangle 구조체 
@@	오른손 실 좌표계에서의 Rectangle 정의 
**/
class DgnRect
{
public:
	LDOUBLE left;
	LDOUBLE bottom;
	LDOUBLE right;
	LDOUBLE top;
	
	DgnRect();
	DgnRect(LDOUBLE Left, LDOUBLE Bottom, LDOUBLE Right, LDOUBLE Top);
	virtual ~DgnRect();

	void	Set(LDOUBLE Left, LDOUBLE Bottom, LDOUBLE Right, LDOUBLE Top );
	
	LBOOL	IsInner(Point3D Point);
	LDOUBLE Width();
	LDOUBLE Height();
	void	NormalizeRect();
	
	LBOOL	IsIntersect(CDgn_Object* pObject);
	LBOOL	IsWithin(CDgn_Object* pObject);
};

class DgnLine
{
public:
	DgnLine();
	virtual ~DgnLine();
	Point3D P1;
	Point3D P2;
	DgnLine& operator = (const DgnLine& Source);
	void Set(const Point3D& p1,const Point3D& p2);
	CString GetString(CString Format = _T("(%f, %f, %f), (%f, %f, %f)"));
	void SetZero();
	LBOOL IsValid();

	LDOUBLE GetMinX(LDOUBLE CurMinX);
	LDOUBLE GetMinY(LDOUBLE CurMinY);
	LDOUBLE GetMaxX(LDOUBLE CurMaxX);
	LDOUBLE GetMaxY(LDOUBLE CurMaxY);

	LBOOL	IsIntersect(CDgn_Object* pObject);
};

#endif // !defined(AFX_DGN_OBJECT_H__3835FF36_1E69_4280_8D55_E9F1C8ABBA37__INCLUDED_)
