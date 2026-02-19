// DgnQuadTree.h: interface for the CDgnQuadTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNQUADTREE_H__654222ED_85EA_4812_AC38_B54215758949__INCLUDED_)
#define AFX_DGNQUADTREE_H__654222ED_85EA_4812_AC38_B54215758949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Dgn_Object.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnDataPool
class CDgnDataPool
{
// Construction
public:
	CDgnDataPool();
	virtual ~CDgnDataPool();

// attributes
public:
	CArray < CDgn_Object*, CDgn_Object* > m_objectList;

	CDgnDataPool* m_pNext;
	CDgnDataPool* m_pPrev;

// operations
	INT Add(CDgn_Object* gObject);
	INT GetSize() { return m_objectList.GetSize(); }
	CDgn_Object* GetAt(UINT index) { return m_objectList.GetAt(index); }
	void RemoveAll() { m_objectList.RemoveAll(); }
	void RemoveAt( int nIndex, int nCount = 1 ) { m_objectList.RemoveAt(nIndex, nCount); }
};

/////////////////////////////////////////////////////////////////////////////
// CDgn_Object
const int    CEQN_MIN_OBJECT_NUM		=	16;
const double CEQN_DEFAULT_SIZE_X		= 100.0;
const double CEQN_DEFAULT_SIZE_Y		= 100.0;
const int    CEQN_DEFAULT_OBJECT_NUM= 128;

class CDgnQuadNode
{
// Construction
public:
	CDgnQuadNode();
	CDgnQuadNode(double dSizeX, double dSizeY, INT nMax);
	CDgnQuadNode(CDgnQuadNode* pParent);
	virtual ~CDgnQuadNode();
private:
	//void InitClassMembers();

public:
	void InitClassMembers();
	
// Attributes
public:
	static	  double m_dSizeMinX, m_dSizeMinY;	// 사각형의 초기 크기
	static		double m_dSizeX, m_dSizeY;	// 사각형의 초기 크기
	static		INT	m_nMaxObjectNum;		// Quad당 저장할 수 있는 최대 갯수

	Point3D			m_minP;				// 영역 최소 좌표
	Point3D			m_maxP;				// 영역 최대 좌표
	CDgnQuadNode*	m_pParentNode;		// 부모 노드를 가르키는 포인터			Y
										//										|---+---+
	CDgnQuadNode*	m_childNode[4];		// 자식 노드 포인터 배열				| 2	| 3	|
										// 0 : left bottom, 1 : right bottom	|---+---|
										// 2 : left top, 3 : right top			| 0	| 1	|
	CDgnDataPool		m_dataPool;			// 데이타 저장 공간						+--------> X

// Operations
public:
	INT				Insert(CDgn_Object* gObject);
	INT				Delete(CDgn_Object* gObject);
	CDgnQuadNode*	FindNodeContainingPoint(Point3D point);
	CDgn_Object*  GetNearestObjectWithinArea(DgnRect& rect, CDgn_Object* pObject);
	INT				GetObjectsWithinArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsIntersectArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsWithinPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsIntersectPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsIntersectLine(DgnLine& line, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
//  void			Draw(CDC* pDC);

private:
	INT	Split(CDgn_Object* gObject);
	INT Merge();
	INT	Distribute(CDgn_Object* gObject);
	INT Extend(Point3D* pt);
	INT GetChildNodeIndexContainingObject(CDgn_Object* gObject);
	INT GetChildNodeIndexContainingObject(Point3D point);
	INT GetObjectsUsingOperator(INT nOperator, DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT GetObjectsUsingOperator(INT nOperator, DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT GetObjectsUsingOperator(INT nOperator, DgnLine& line, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);

	enum {CEQN_WITHIN, CEQN_INTERSECT};
};

/////////////////////////////////////////////////////////////////////////////
// CDgnQuadTree

class CDgnQuadTree  
{
public:
	CDgnQuadTree();
	virtual ~CDgnQuadTree();

	// Attributes
public:
	CDgnQuadNode* m_pNode;
	
// Operations
public:
	void      Initialize();
	INT				Insert(CDgn_Object* gObject);
	INT				Delete(CDgn_Object* gObject);
	CDgn_Object*	Find(Point3D point);
	CDgnDataPool*		FindNode(Point3D point);
	CDgn_Object*  GetNearestObjectWithinArea(DgnRect& rect);
	INT				GetObjectsWithinArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsIntersectArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsWithinPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsIntersectPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
	INT				GetObjectsIntersectLine(DgnLine& line, CArray <CDgn_Object*, CDgn_Object*>& ObjSet);
//	void	Draw(CDC* pDC) { if (m_pNode) m_pNode->Draw(pDC); }

	BOOL      MakePlateQuadTree(const CArray<UINT, UINT>& aKeyList);
	BOOL      CalcIntersectElem(double Pt1[3],double Pt2[3],CArray<UINT,UINT> &arElemK);
	BOOL      CalcInAreaElem(DgnRect& rect, double dZ, CArray<UINT,UINT> &arElemK, double Tolerance);

private:
	BOOL      IsIntersect2D(T_NODE_D& P1, T_NODE_D& P2, CArray<T_NODE_D, T_NODE_D&>& arPolygon);

};

#endif // !defined(AFX_DGNQUADTREE_H__654222ED_85EA_4812_AC38_B54215758949__INCLUDED_)
