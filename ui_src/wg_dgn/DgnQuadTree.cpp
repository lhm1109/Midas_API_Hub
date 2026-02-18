// DgnQuadTree.cpp: implementation of the CDgnQuadTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnQuadTree.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDgnDataPool
CDgnDataPool::CDgnDataPool()
{
	m_pNext = 0;
	m_pPrev = 0;
}

CDgnDataPool::~CDgnDataPool()
{
	// Element는 지우지 않는다. 생성한 곳에서 지우기 때문
	int nObj = m_objectList.GetSize();
	for(int i=0; i<nObj; i++)
	{
		if(m_objectList[i] != NULL)
			delete m_objectList[i];
	}
	m_objectList.RemoveAll();
}
	
INT CDgnDataPool::Add(CDgn_Object* gObject)
{
	return m_objectList.Add(gObject);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnQuadNode

double CDgnQuadNode::m_dSizeX = 0.0;
double CDgnQuadNode::m_dSizeY = 0.0;
double CDgnQuadNode::m_dSizeMinX=0.0;
double CDgnQuadNode::m_dSizeMinY=0.0;
INT CDgnQuadNode::m_nMaxObjectNum = 0;

CDgnQuadNode::CDgnQuadNode()
{
	InitClassMembers();

	/*
	// static member variables 값이 설정되어 있지 않으면
	if (CDgnQuadNode::m_nMaxObjectNum < CEQN_MIN_OBJECT_NUM || 
		//CDgnQuadNode::m_dSizeX < CEQN_MIN_SIZE_X ||
		//CDgnQuadNode::m_dSizeY < CEQN_MIN_SIZE_Y)
		CDgnQuadNode::m_dSizeX < CDgnQuadNode::m_dSizeMinX ||
		CDgnQuadNode::m_dSizeY < CDgnQuadNode::m_dSizeMinY )
	{
		// Default 값 설정
		CDgnQuadNode::m_nMaxObjectNum = CEQN_DEFAULT_OBJECT_NUM;
		CDgnQuadNode::m_dSizeX = CEQN_DEFAULT_SIZE_X;
		CDgnQuadNode::m_dSizeY = CEQN_DEFAULT_SIZE_Y;
	}
	*/
	CDgnQuadNode::m_nMaxObjectNum = CEQN_DEFAULT_OBJECT_NUM;
}

CDgnQuadNode::CDgnQuadNode(double dSizeX, double dSizeY, INT nMax)
{
	InitClassMembers();

	/*
	// Initialize static member variables
	ASSERT(dSizeX >= CEQN_MIN_SIZE_X);
	ASSERT(dSizeY >= CEQN_MIN_SIZE_Y);
	ASSERT(nMax >= CEQN_MIN_OBJECT_NUM);

	CDgnQuadNode::m_dSizeX = dSizeX;
	CDgnQuadNode::m_dSizeY = dSizeY;
	CDgnQuadNode::m_nMaxObjectNum = nMax;
	*/
	CDgnQuadNode::m_nMaxObjectNum = nMax;
}

CDgnQuadNode::CDgnQuadNode(CDgnQuadNode* pParent)
{
	InitClassMembers();
	m_pParentNode = pParent;
}

void CDgnQuadNode::InitClassMembers()
{
	// 영역 초기화
	m_maxP.x = m_maxP.y = 0.0;
	m_minP.x = m_minP.y = 0.0;

	// 부모 Node pointer 초기화
	m_pParentNode = 0;

	// 자식 Node pointer 초기화
	for (INT i = 0; i < 4 ; i++)
		m_childNode[i] = 0;
}

CDgnQuadNode::~CDgnQuadNode()
{
	// Child Node를 모두 지운다.
	for (INT i = 0; i < 4; i++)
		delete m_childNode[i];
}

INT CDgnQuadNode::Insert(CDgn_Object* gObject)
{
	// Graphic Object가 Child Node에 포함되면 Child Node에 저장
	// 현재 노드에 포함되면 현재 노드에 저장
	// 현재 노드의 범위 밖에 있으면 확장한다.(Root의 경우)

	int nGeomType = gObject->WhoAmI();
	Point3D* pt = gObject->GetPointForQT(0);
	Point3D* pt2;

	int nPolyPtNum=0;
	CArray<Point3D*,Point3D*> arPolyPt;
		
	// gObject 타입이 선인 경우
	if (nGeomType == GOB_LINE) pt2 = gObject->GetPointForQT(1);

	// gObject 타입이 다각형인경우 
	int i; 
	Point3D mnPt,mxPt;
	if(nGeomType == GOB_PLINE)
	{
		nPolyPtNum=((DgnOB_Polygon *)gObject)->GetVertexNum();
		for(i=0; i<nPolyPtNum; i++)
		{
			arPolyPt.Add(((DgnOB_Polygon *)gObject)->GetVertex(i));
		}
		mnPt.x = DBL_MAX; 
		mnPt.y = DBL_MAX; 
		mxPt.x = -DBL_MAX; 
		mxPt.y = -DBL_MAX; 
		for(i=0; i<nPolyPtNum; i++)
		{
			mnPt.x = min(arPolyPt[i]->x,mnPt.x);
			mnPt.y = min(arPolyPt[i]->y,mnPt.y);
			mxPt.x = max(arPolyPt[i]->x,mxPt.x);
			mxPt.y = max(arPolyPt[i]->y,mxPt.y);
		}
	}

	// 최초 삽입, min, max를 설정하고 값을 저장
	if (m_minP.x == m_maxP.x && m_minP.y == m_maxP.y)
	{
		// 점인 경우
		if(nGeomType == GOB_POINT)
		{
			m_minP.x = pt->x - (CDgnQuadNode::m_dSizeX/2.0);
			m_minP.y = pt->y - (CDgnQuadNode::m_dSizeY/2.0);
			m_maxP.x = pt->x + (CDgnQuadNode::m_dSizeX/2.0);
			m_maxP.y = pt->y + (CDgnQuadNode::m_dSizeY/2.0);
		}
		// 선인 경우
		else if(nGeomType == GOB_LINE)
		{
			double nDistX, nDistY;
			nDistX = pt2->x - pt->x;
			nDistY = pt2->y - pt->y;
			if (nDistX < 0.0) nDistX = -nDistX;
			if (nDistY < 0.0) nDistY = -nDistY;
			if (nDistX < nDistY) nDistX = nDistY;
			else nDistY = nDistX;
			nDistX += CDgnQuadNode::m_dSizeX;
			nDistY += CDgnQuadNode::m_dSizeY;

			// 선을 포함하는 사각형을 영역을 계산
			m_minP.x = (pt->x + pt2->x - nDistX)/2.0;
			m_minP.y = (pt->y + pt2->y - nDistY)/2.0;
			m_maxP.x = (pt->x + pt2->x + nDistX)/2.0;
			m_maxP.y = (pt->y + pt2->y + nDistY)/2.0;
		}
		// 다각형인경우 
		else if (nGeomType == GOB_PLINE)
		{
			double nDistX, nDistY;
			nDistX = mxPt.x - mnPt.x;
			nDistY = mxPt.y - mnPt.y;
			if (nDistX < 0.0) nDistX = -nDistX;
			if (nDistY < 0.0) nDistY = -nDistY;
			if (nDistX < nDistY) nDistX = nDistY;
			else nDistY = nDistX;
			nDistX += CDgnQuadNode::m_dSizeX;
			nDistY += CDgnQuadNode::m_dSizeY;

			// 선을 포함하는 사각형을 영역을 계산
			m_minP.x = (mnPt.x + mxPt.x - nDistX)/2.0;
			m_minP.y = (mnPt.y + mxPt.y - nDistY)/2.0;
			m_maxP.x = (mnPt.x + mxPt.x + nDistX)/2.0;
			m_maxP.y = (mnPt.y + mxPt.y + nDistY)/2.0;
		}

		m_dataPool.Add(gObject);
		return 0;
	}

	BOOL bObjectInRect=FALSE;
	if (nGeomType == GOB_POINT)
	{
		bObjectInRect = (pt->x >= m_minP.x && pt->x < m_maxP.x &&
					pt->y >= m_minP.y && pt->y < m_maxP.y);
	}
	// 선인 경우
	else if (nGeomType == GOB_LINE)
	{
		bObjectInRect = (pt->x >= m_minP.x && pt->x < m_maxP.x &&
					pt->y >= m_minP.y && pt->y < m_maxP.y);
		bObjectInRect = bObjectInRect && 
						(pt2->x >= m_minP.x && pt2->x < m_maxP.x &&
						pt2->y >= m_minP.y && pt2->y < m_maxP.y);
	}
	
	// 다각형인 경우 
	else if(nGeomType == GOB_PLINE)
	{
		/*
		bObjectInRect=TRUE; 
		for(i=0; i<nPolyPtNum; i++)
		if( !(arPolyPt[i]->x >= m_minP.x && arPolyPt[i]->x < m_maxP.x &&
					arPolyPt[i]->y >= m_minP.y && arPolyPt[i]->y < m_maxP.y) )
		{
			bObjectInRect=FALSE; 
			break; 
		}
		*/
		
		bObjectInRect = (mnPt.x >= m_minP.x && mnPt.x < m_maxP.x &&
					mnPt.y >= m_minP.y && mnPt.y < m_maxP.y);
		bObjectInRect = bObjectInRect && 
						(mxPt.x >= m_minP.x && mxPt.x < m_maxP.x &&
						 mxPt.y >= m_minP.y && mxPt.y < m_maxP.y);
	}
		
	if(bObjectInRect)
	{
		if (m_childNode[0] != 0)	// child가 있는 경우
		{
			// child의 어느 부분에 있는지 검사
			int nIndex = GetChildNodeIndexContainingObject(gObject);

			// child에 포함되면 child에 추가, 아니면 현재노드에 추가
			if (nIndex >= 0 && nIndex < 4) return m_childNode[nIndex]->Insert(gObject);
			else return m_dataPool.Add(gObject);
		}

		// 데이타 풀에 추가
		if (m_dataPool.GetSize() < CDgnQuadNode::m_nMaxObjectNum)
			m_dataPool.Add(gObject);
		else Split(gObject);	// 노드를 분할한다.
		return 0;
	}
	
	// 현재 노드에 안 들어가는 경우 -> Extend 필요
	if (nGeomType == GOB_POINT) 
		Extend(pt);
	else if (nGeomType == GOB_LINE)
	{
		if (!(pt->x >= m_minP.x && pt->x < m_maxP.x && 
			  pt->y >= m_minP.y && pt->y < m_maxP.y))
			  Extend(pt);
		if (!(pt2->x >= m_minP.x && pt2->x < m_maxP.x && 
			  pt2->y >= m_minP.y && pt2->y < m_maxP.y))
			  Extend(pt2);
	}
	else if(nGeomType == GOB_PLINE)
	{
		/*
		for(i=0; i<nPolyPtNum; i++)
		if(!(arPolyPt[i]->x >= m_minP.x && arPolyPt[i]->x < m_maxP.x &&
				 arPolyPt[i]->y >= m_minP.y && arPolyPt[i]->y < m_maxP.y) )
		{
			Extend(arPolyPt[i]);
		}
		*/
		if (!(mnPt.x >= m_minP.x && mnPt.x < m_maxP.x && 
			  mnPt.y >= m_minP.y && mnPt.y < m_maxP.y))
			  Extend(&mnPt);
		if (!(mxPt.x >= m_minP.x && mxPt.x < m_maxP.x && 
			  mxPt.y >= m_minP.y && mxPt.y < m_maxP.y))
			  Extend(&mxPt);
	}
	return Insert(gObject);
}

INT CDgnQuadNode::Delete(CDgn_Object* gObject)
{
	int nGeomType = gObject->WhoAmI();
	Point3D* pt = gObject->GetPointForQT(0);
	Point3D* pt2;
	int nPolyPtNum=0;
	CArray<Point3D*,Point3D*> arPolyPt;

	if (nGeomType == GOB_LINE) pt2 = gObject->GetPointForQT(1);
	// gObject 타입이 다각형인경우 
	int i; 
	Point3D mnPt,mxPt;
	if(nGeomType == GOB_PLINE)
	{
		nPolyPtNum=((DgnOB_Polygon *)gObject)->GetVertexNum();
		for(i=0; i<nPolyPtNum; i++)
		{
			arPolyPt.Add(((DgnOB_Polygon *)gObject)->GetVertex(i));
		}
		mnPt.x = DBL_MAX; 
		mnPt.y = DBL_MAX; 
		mxPt.x = -DBL_MAX; 
		mxPt.y = -DBL_MAX; 
		for(i=0; i<nPolyPtNum; i++)
		{
			mnPt.x = min(arPolyPt[i]->x,mnPt.x);
			mnPt.y = min(arPolyPt[i]->y,mnPt.y);
			mxPt.x = max(arPolyPt[i]->x,mxPt.x);
			mxPt.y = max(arPolyPt[i]->y,mxPt.y);
		}
	}

	BOOL bObjectInRect=FALSE;
	if (nGeomType == GOB_POINT)
	{
	  bObjectInRect = (pt->x >= m_minP.x && pt->x < m_maxP.x &&
						  pt->y >= m_minP.y && pt->y < m_maxP.y);

	}
	// 선인 경우
	else if (nGeomType == GOB_LINE)
	{
		bObjectInRect = (pt->x >= m_minP.x && pt->x < m_maxP.x &&
						  pt->y >= m_minP.y && pt->y < m_maxP.y);
		bObjectInRect = bObjectInRect && 
						(pt2->x >= m_minP.x && pt2->x < m_maxP.x &&
						 pt2->y >= m_minP.y && pt2->y < m_maxP.y);
	}
	// 다각형인경우 
	else if(nGeomType == GOB_PLINE)
	{
		/*
		bObjectInRect=TRUE; 
		for(i=0; i<nPolyPtNum; i++)
		if( arPolyPt[i]->x < m_minP.x && arPolyPt[i]->x >= m_maxP.x &&
				arPolyPt[i]->y < m_minP.y && arPolyPt[i]->y >= m_maxP.y )
		{
			bObjectInRect=FALSE; 
			break; 
		} 
		*/
		bObjectInRect = (mnPt.x >= m_minP.x && mnPt.x < m_maxP.x &&
						  mnPt.y >= m_minP.y && mnPt.y < m_maxP.y);
		bObjectInRect = bObjectInRect && 
						(mxPt.x >= m_minP.x && mxPt.x < m_maxP.x &&
						 mxPt.y >= m_minP.y && mxPt.y < m_maxP.y);
	}

	// Quad Node안에 있으면 
	if (bObjectInRect)
	{
		INT nCount = m_dataPool.GetSize();
		INT i;

		// child Node에 있는지 검사
		if (m_childNode[0] != NULL)
		{
			int nIndex = GetChildNodeIndexContainingObject(gObject);
			if (nIndex >= 0 && nIndex < 4)
				return m_childNode[nIndex]->Delete(gObject);
		}

		// 현재 노드에 있는지 조사
		for (i = 0; i < nCount; i++)
		{
			if (gObject == m_dataPool.GetAt(i))	
			{
				m_dataPool.RemoveAt(i);
				if (nCount == 1 && m_childNode[0] == NULL && 
					m_pParentNode != NULL)
					m_pParentNode->Merge();
				return 0;
			}
		}
	}

	return -1;
}

CDgnQuadNode* CDgnQuadNode::FindNodeContainingPoint(Point3D point)
{
	if (point.x >= m_minP.x && point.x < m_maxP.x &&
		point.y >= m_minP.y && point.y < m_maxP.y)
	{
		if (m_childNode[0] == NULL) return this;
		int nIndex = GetChildNodeIndexContainingObject(point);
		return m_childNode[nIndex]->FindNodeContainingPoint(point);
	}
	return NULL;
}

CDgn_Object* CDgnQuadNode::GetNearestObjectWithinArea(DgnRect& rect, CDgn_Object* pObject)
{
	CDgn_Object* pGrObject = pObject;

	// rect와 node의 영역이 겹치는지 조사
	BOOL bOverlap;
	bOverlap = !(rect.left >= m_maxP.x || rect.right < m_minP.x ||
		         rect.bottom >= m_maxP.y || rect.top < m_minP.y);

	if (bOverlap)
	{
		INT i, nCount;
		Point3D center;
		CDgn_Object* pTempObject;
		double threshold, dx, dy, dist, minDist;

		nCount = m_dataPool.GetSize();
		center.x = (rect.left+rect.right)/2.0;
		center.y = (rect.top+rect.bottom)/2.0;
		dx = rect.right - center.x;
		dy = rect.top - center.y;
		threshold = sqrt(dx*dx + dy*dy);

		if (pObject == NULL) minDist = -1.0;
		else minDist = pObject->NearDist(center);

		// 현재 노드에서 가장 가까운 객체를 찾는다.
		for (i = 0; i < nCount; i++)
		{
			pTempObject = m_dataPool.GetAt(i);
			dist = pTempObject->NearDist(center);
			if (dist <= threshold && (minDist < 0.0 || dist < minDist))
			{
				minDist = dist;
				pGrObject = pTempObject;
			}
		}

		// 자식 노드에서 현재 노드에서 찾은 것 보다 더 가까운 객체를 찾는다.
		if (m_childNode[0] != NULL)
		{
			for (i = 0; i < 4; i++)
				pGrObject = m_childNode[i]->GetNearestObjectWithinArea(rect, pGrObject);
		}
	}
	return pGrObject;
}

INT CDgnQuadNode::GetObjectsWithinArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	return GetObjectsUsingOperator(CEQN_WITHIN, rect, ObjSet);
}

INT CDgnQuadNode::GetObjectsIntersectArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	return GetObjectsUsingOperator(CEQN_INTERSECT, rect, ObjSet);
}

INT CDgnQuadNode::GetObjectsWithinPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	return GetObjectsUsingOperator(CEQN_WITHIN, polygon, ObjSet);
}

INT CDgnQuadNode::GetObjectsIntersectPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	return GetObjectsUsingOperator(CEQN_INTERSECT, polygon, ObjSet);
}

INT CDgnQuadNode::GetObjectsIntersectLine(DgnLine& line, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	return GetObjectsUsingOperator(CEQN_INTERSECT, line, ObjSet);
}

INT CDgnQuadNode::GetObjectsUsingOperator(INT nOperator, DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	// rect와 node의 영역이 겹치는지 조사
	BOOL bOverlap;
	bOverlap = !(rect.left >= m_maxP.x || rect.right < m_minP.x ||
		         rect.bottom >= m_maxP.y || rect.top < m_minP.y);

	if (bOverlap)
	{
		CDgn_Object* pGrObject;
		INT i, nCount;
		BOOL ok;

		// 현재 노드에 있는 객체중 영역내에 있는 것을 결과 셋에 추가
		nCount = m_dataPool.GetSize();
		for (i = 0; i < nCount; i++)
		{
			pGrObject = m_dataPool.GetAt(i);

			/*
			switch(nOperator)
			{
			case CEQN_WITHIN:
				ok = rect.IsWithin(pGrObject);
				break;
			case CEQN_INTERSECT:
				ok = rect.IsIntersect(pGrObject);
				break;
			}
			*/
			ok=TRUE; 

			// 알고리즘 보강... 이거 뭔지 많이 부족한 함수당... 흠... // MNET:XXXX-HSSHIM-20091203
			if (nOperator == CEQN_INTERSECT) 
			{
				if (pGrObject->WhoAmI() == GOB_PLINE)
				{
					ok = ((DgnOB_Polygon*)pGrObject)->IsIntersect(rect);
				}
			}
			
			if (ok)	ObjSet.Add(pGrObject);
		}

		// 자식 노드에 있는 객체중 영역내에 있는 것을 결과 셋에 추가
		if (m_childNode[0] != NULL)
		{
			for (i = 0; i < 4; i++)
				m_childNode[i]->GetObjectsUsingOperator(nOperator, rect, ObjSet);
		}
	}
	return 0;
}

INT CDgnQuadNode::GetObjectsUsingOperator(INT nOperator, DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	// polygon과 node의 영역이 겹치는지 조사
	BOOL bOverlap;
	DgnRect gr(m_minP.x, m_minP.y, m_maxP.x, m_maxP.y);
	bOverlap = polygon.IsIntersect(gr);

	if (bOverlap)
	{
		CDgn_Object* pGrObject;
		INT i, nCount;
		BOOL ok;

		// 현재 노드에 있는 객체중 영역내에 있는 것을 결과 셋에 추가
		nCount = m_dataPool.GetSize();
		for (i = 0; i < nCount; i++)
		{
			pGrObject = m_dataPool.GetAt(i);

			/*계홍 
			switch(nOperator)
			{
			case CEQN_WITHIN:	// CEQN_ : CDgnQuadNode의 약자
				ok = polygon.IsWithin(pGrObject);
				break;
			case CEQN_INTERSECT:
				ok = polygon.IsIntersect(pGrObject);
				break;
			}
			*/
			ok=TRUE; 
			if (ok)	ObjSet.Add(pGrObject);
		}

		// 자식 노드에 있는 객체중 영역내에 있는 것을 결과 셋에 추가
		if (m_childNode[0] != NULL)
		{
			for (i = 0; i < 4; i++)
				m_childNode[i]->GetObjectsUsingOperator(nOperator, polygon, ObjSet);
		}
	}
	return 0;
}

INT CDgnQuadNode::GetObjectsUsingOperator(INT nOperator, DgnLine& line, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	// line과 node의 영역이 겹치는지 조사
	BOOL bOverlap;
	DgnRect gr(m_minP.x, m_minP.y, m_maxP.x, m_maxP.y);
	
	DgnOB_Line ObjLine;
	ObjLine.m_ptPos1.x = line.P1.x;
	ObjLine.m_ptPos1.y = line.P1.y;
	ObjLine.m_ptPos2.x = line.P2.x;
	ObjLine.m_ptPos2.y = line.P2.y;
	bOverlap = gr.IsIntersect(&ObjLine);
	//bOverlap = line.IsIntersect(gr);

	if (bOverlap)
	{
		CDgn_Object* pGrObject;
		INT i, nCount;
		BOOL ok;

		// 현재 노드에 있는 객체중 영역내에 있는 것을 결과 셋에 추가
		nCount = m_dataPool.GetSize();
		for (i = 0; i < nCount; i++)
		{
			pGrObject = m_dataPool.GetAt(i);

			/*계홍 
			switch(nOperator)
			{
			case CEQN_WITHIN:	// CEQN_ : CDgnQuadNode의 약자
				ok = polygon.IsWithin(pGrObject);
				break;
			case CEQN_INTERSECT:
				ok = polygon.IsIntersect(pGrObject);
				break;
			}
			*/
			ok=TRUE; 
			if (ok)	ObjSet.Add(pGrObject);
		}

		// 자식 노드에 있는 객체중 영역내에 있는 것을 결과 셋에 추가
		if (m_childNode[0] != NULL)
		{
			for (i = 0; i < 4; i++)
				m_childNode[i]->GetObjectsUsingOperator(nOperator, line, ObjSet);
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnQuadNode's Private Member Functions
INT CDgnQuadNode::Split(CDgn_Object* gObject)
{
	// 자식 노드를 만든다.
	for (INT i = 0; i < 4; i++)
	{
		m_childNode[i] = new CDgnQuadNode(this);
		ASSERT(m_childNode[i] != 0);
	}

	// min, max값 설정
	m_childNode[0]->m_minP.x = m_minP.x;
	m_childNode[0]->m_minP.y = m_minP.y;
	m_childNode[0]->m_maxP.x = (m_minP.x+m_maxP.x)/2.0;
	m_childNode[0]->m_maxP.y = (m_minP.y+m_maxP.y)/2.0;

	m_childNode[1]->m_minP.x = m_childNode[0]->m_maxP.x;
	m_childNode[1]->m_minP.y = m_childNode[0]->m_minP.y;
	m_childNode[1]->m_maxP.x = m_maxP.x;
	m_childNode[1]->m_maxP.y = m_childNode[0]->m_maxP.y;

	m_childNode[2]->m_minP.x = m_childNode[0]->m_minP.x;
	m_childNode[2]->m_minP.y = m_childNode[0]->m_maxP.y;
	m_childNode[2]->m_maxP.x = m_childNode[0]->m_maxP.x;
	m_childNode[2]->m_maxP.y = m_maxP.y;

	m_childNode[3]->m_minP.x = m_childNode[0]->m_maxP.x;
	m_childNode[3]->m_minP.y = m_childNode[0]->m_maxP.y;
	m_childNode[3]->m_maxP.x = m_maxP.x;
	m_childNode[3]->m_maxP.y = m_maxP.y;

	// 부모에 있는 데이타를 자식에 재분배한다.
	return Distribute(gObject);
}

INT CDgnQuadNode::Merge()
{
	INT		i, nCount=0;

	if (m_childNode[0] == NULL) return -1;

	for (i = 0; i < 4; i++)
	{
		if (m_childNode[i]->m_childNode[0] != NULL) break;
		nCount += m_childNode[i]->m_dataPool.GetSize();
	}
	// child가 있거나 child node안에 데이타가 있으면 merge 안함
	if (i != 4 || nCount != 0) return -1;

	for (i = 0; i < 4; i++)
	{
		delete m_childNode[i];
		m_childNode[i] = NULL;
	}
	
	if (m_pParentNode != 0) 
	{
		m_pParentNode->Merge();
	}
	
	return 0;
}

INT	CDgnQuadNode::Distribute(CDgn_Object* gObject)
{
	CDgn_Object* pObject;
	INT nCount = m_dataPool.GetSize();

	CArray <CDgn_Object*, CDgn_Object*> tempPool;

	// 현재 노드에 있는 것을 자식에게 재분배한다.
	INT nIndex;
	for (INT i = 0; i < nCount; i++)
	{
		pObject = m_dataPool.GetAt(i);
		nIndex =  GetChildNodeIndexContainingObject(pObject);
		
		if (nIndex >= 0 && nIndex < 4) m_childNode[nIndex]->m_dataPool.Add(pObject);
		else tempPool.Add(pObject);
	}

	// 현재 노드에 있는 데이타를 모두 지운다.
	m_dataPool.RemoveAll();

	// tempPool에 있는 데이타를 삽입하고 tempPool은 지운다.
	nCount = tempPool.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		pObject = tempPool.GetAt(i);
		m_dataPool.Add(pObject);
	}
	tempPool.RemoveAll();

	// 추가되는 gObject를 삽입할 Child Node를 찾는다.
	nIndex = GetChildNodeIndexContainingObject(gObject);

	// 만약 한 자식 노드에 모든 오브젝트가 집중되지 않았으면 삽입, 그렇지 않으면 다시 분할
	if (nIndex >= 0 && nIndex < 4)
	{
		if (m_childNode[nIndex]->m_dataPool.GetSize() < CDgnQuadNode::m_nMaxObjectNum)
			return m_childNode[nIndex]->m_dataPool.Add(gObject);
		else return m_childNode[nIndex]->Split(gObject);
	}
	// 자식 노드에 포함되지 않으면 현재 노드에 바로 삽입
	else return m_dataPool.Add(gObject);
}

INT CDgnQuadNode::Extend(Point3D* pt)
{
	INT nIndex, i;

	// 확장의 방향을 결정한다.
	if (pt->x >= m_maxP.x)
	{
		if (pt->y >= m_maxP.y) nIndex = 0;
		else nIndex = 2;
	}
	else if (pt->x < m_minP.x)
	{
		if (pt->y >= m_maxP.y) nIndex = 1;
		else nIndex = 3;
	}
	else
	{
		if (pt->y >= m_maxP.y) nIndex = 0;
		else nIndex = 2;
	}

	// child node를 4개 만든다.
	CDgnQuadNode* pChildNode[4];
	for (i = 0; i < 4; i++)
	{
		pChildNode[i] = new CDgnQuadNode(this);
		ASSERT(pChildNode[i] != 0);
	}

	// 현재 노드의 min, max 값을 재설정한다.
	Point3D size;
	size.x = m_maxP.x - m_minP.x;
	size.y = m_maxP.y - m_minP.y;
	switch(nIndex)
	{
	case 0:
		m_maxP.x += size.x; m_maxP.y += size.y;
		break;
	case 1:
		m_minP.x -= size.x; m_maxP.y += size.y;
		break;
	case 2:
		m_maxP.x += size.x; m_minP.y -= size.y;
		break;
	case 3:
		m_minP.x -= size.x; m_minP.y -= size.y;
		break;
	}

	// 새 자식 노드의 min, max 값을 재설정한다.
	pChildNode[0]->m_minP.x = m_minP.x;
	pChildNode[0]->m_minP.y = m_minP.y;
	pChildNode[0]->m_maxP.x = (m_minP.x+m_maxP.x)/2.0;
	pChildNode[0]->m_maxP.y = (m_minP.y+m_maxP.y)/2.0;

	pChildNode[1]->m_minP.x = pChildNode[0]->m_maxP.x;
	pChildNode[1]->m_minP.y = pChildNode[0]->m_minP.y;
	pChildNode[1]->m_maxP.x = m_maxP.x;
	pChildNode[1]->m_maxP.y = pChildNode[0]->m_maxP.y;

	pChildNode[2]->m_minP.x = pChildNode[0]->m_minP.x;
	pChildNode[2]->m_minP.y = pChildNode[0]->m_maxP.y;
	pChildNode[2]->m_maxP.x = pChildNode[0]->m_maxP.x;
	pChildNode[2]->m_maxP.y = m_maxP.y;

	pChildNode[3]->m_minP.x = pChildNode[0]->m_maxP.x;
	pChildNode[3]->m_minP.y = pChildNode[0]->m_maxP.y;
	pChildNode[3]->m_maxP.x = m_maxP.x;
	pChildNode[3]->m_maxP.y = m_maxP.y;

	// 부모에 있는 데이타를 한 자식에게 카피한다.
	INT nCount = m_dataPool.GetSize();
	for (i = 0; i < nCount; i++)
		pChildNode[nIndex]->m_dataPool.Add(m_dataPool.GetAt(i));
	for (i = 0; i < 4; i++)
	{
		pChildNode[nIndex]->m_childNode[i] = m_childNode[i];
		if (m_childNode[i] != 0) m_childNode[i]->m_pParentNode = pChildNode[nIndex];
	}

	// 부모의 값을 재 설정한다.
	m_dataPool.RemoveAll();
	for (i = 0; i < 4; i++)
		m_childNode[i] = pChildNode[i];

	return 0;
}

INT CDgnQuadNode::GetChildNodeIndexContainingObject(CDgn_Object* gObject)
{
	BOOL bObjectInRect;

	int nGeomType = gObject->WhoAmI();
	Point3D* pt = gObject->GetPointForQT(0);
	Point3D* pt2;
	if (nGeomType == GOB_LINE) pt2 = gObject->GetPointForQT(1);

	int nPolyPtNum=0;
	CArray<Point3D*,Point3D*> arPolyPt;
		
	// gObject 타입이 다각형인경우 
	int i; 
	Point3D mnPt,mxPt;
	if(nGeomType == GOB_PLINE)
	{
		nPolyPtNum=((DgnOB_Polygon *)gObject)->GetVertexNum();
		for(i=0; i<nPolyPtNum; i++)
		{
			arPolyPt.Add(((DgnOB_Polygon *)gObject)->GetVertex(i));
		}
		mnPt.x = DBL_MAX; 
		mnPt.y = DBL_MAX; 
		mxPt.x = -DBL_MAX; 
		mxPt.y = -DBL_MAX; 
		for(i=0; i<nPolyPtNum; i++)
		{
			mnPt.x = min(arPolyPt[i]->x,mnPt.x);
			mnPt.y = min(arPolyPt[i]->y,mnPt.y);
			mxPt.x = max(arPolyPt[i]->x,mxPt.x);
			mxPt.y = max(arPolyPt[i]->y,mxPt.y);
		}
	}

    INT j = 0;
	for (j = 0; j < 4; j++)
	{
		if (nGeomType == GOB_POINT)
		{
		  bObjectInRect = (pt->x >= m_childNode[j]->m_minP.x &&
						 pt->x < m_childNode[j]->m_maxP.x &&
						 pt->y >= m_childNode[j]->m_minP.y &&
						 pt->y < m_childNode[j]->m_maxP.y);
		}
		// 선인 경우
		if (nGeomType == GOB_LINE)
		{
			bObjectInRect = (pt->x >= m_childNode[j]->m_minP.x &&
						 pt->x < m_childNode[j]->m_maxP.x &&
						 pt->y >= m_childNode[j]->m_minP.y &&
						 pt->y < m_childNode[j]->m_maxP.y);
			bObjectInRect = bObjectInRect && 
							(pt2->x >= m_childNode[j]->m_minP.x &&
							pt2->x < m_childNode[j]->m_maxP.x &&
							pt2->y >= m_childNode[j]->m_minP.y &&
							pt2->y < m_childNode[j]->m_maxP.y);
		}
		// 다각형인 경우 
		else if(nGeomType == GOB_PLINE)
		{
			/*
			bObjectInRect=TRUE; 
			for(i=0; i<nPolyPtNum; i++)
			if( !(arPolyPt[i]->x >= m_childNode[j]->m_minP.x && arPolyPt[i]->x < m_childNode[j]->m_maxP.x &&
						arPolyPt[i]->y >= m_childNode[j]->m_minP.y && arPolyPt[i]->y < m_childNode[j]->m_maxP.y ))
			{
				bObjectInRect=FALSE; 
				break; 
			}
			*/
			bObjectInRect = (mnPt.x >= m_childNode[j]->m_minP.x &&
						           mnPt.x < m_childNode[j]->m_maxP.x &&
						           mnPt.y >= m_childNode[j]->m_minP.y &&
						           mnPt.y < m_childNode[j]->m_maxP.y);
			bObjectInRect = bObjectInRect && 
							        (mxPt.x >= m_childNode[j]->m_minP.x &&
							         mxPt.x < m_childNode[j]->m_maxP.x &&
							         mxPt.y >= m_childNode[j]->m_minP.y &&
											 mxPt.y < m_childNode[j]->m_maxP.y);
			
		}
		
		if (bObjectInRect) break;
	}
	return j;
}

INT CDgnQuadNode::GetChildNodeIndexContainingObject(Point3D point)
{
	BOOL bObjectInRect;
    INT j = 0;
	for (j = 0; j < 4; j++)
	{
		// 점인 경우
		bObjectInRect = (point.x >= m_childNode[j]->m_minP.x &&
						 point.x < m_childNode[j]->m_maxP.x &&
						 point.y >= m_childNode[j]->m_minP.y &&
						 point.y < m_childNode[j]->m_maxP.y);
		if (bObjectInRect) break;
	}
	return j;
}

// void CDgnQuadNode::Draw(CDC* pDC)
// {
// 	if (m_childNode[0] == 0)
// 	{
// 		CRect r;
// 		r.left = (long)m_minP.x;
// 		r.top = (long) m_minP.y;
// 		r.right = (long)m_maxP.x;
// 		r.bottom = (long)m_maxP.y;
// 		LOGBRUSH logBr;
// 		logBr.lbStyle = BS_HOLLOW;
// 		CBrush br;
// 		br.CreateBrushIndirect(&logBr);
// 		CBrush *oldBr = pDC->SelectObject(&br);
// 		pDC->Rectangle(&r);
// 		pDC->SelectObject(oldBr);
// 		return;
// 	}
// 	else
// 	{
// 		for (int i = 0; i < 4; i++)
// 		{
// 			m_childNode[i]->Draw(pDC);
// 		}
// 	}
// }

/////////////////////////////////////////////////////////////////////////////
// CDgnQuadTree

CDgnQuadTree::CDgnQuadTree()
{
	m_pNode = 0;
}

CDgnQuadTree::~CDgnQuadTree()
{
	if (m_pNode != 0) delete m_pNode;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnQuadTree message handlers

/////////////////////////////////////////////////////////////////////////////
// CDgnQuadTree's Public Member Functions
void CDgnQuadTree::Initialize()
{
	if (m_pNode != 0) delete m_pNode;
	m_pNode = 0;
	//if(m_pNode != 0)
	//    m_pNode->InitClassMembers();
}

INT CDgnQuadTree::Insert(CDgn_Object* gObject)
{

	if (m_pNode == 0)
	{
		m_pNode = new CDgnQuadNode();
		ASSERT(m_pNode != 0);
	}

	return m_pNode->Insert(gObject);
}

INT CDgnQuadTree::Delete(CDgn_Object* gObject)
{
	if (m_pNode == 0) return -1;
	return m_pNode->Delete(gObject);
}

CDgn_Object* CDgnQuadTree::Find(Point3D point)
{

	return NULL;
}


CDgnDataPool* CDgnQuadTree::FindNode(Point3D point)
{
	CDgnQuadNode* pNode;
	if (m_pNode == 0) return NULL;
	
	pNode = m_pNode->FindNodeContainingPoint(point);
	if (pNode == NULL) return NULL;
	return &(pNode->m_dataPool);
}

CDgn_Object* CDgnQuadTree::GetNearestObjectWithinArea(DgnRect& rect)
{
	if (m_pNode == NULL) return NULL;
	return m_pNode->GetNearestObjectWithinArea(rect, NULL);
}

INT	CDgnQuadTree::GetObjectsWithinArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	if (m_pNode == NULL) return 0;
	return m_pNode->GetObjectsWithinArea(rect, ObjSet);
}

INT	CDgnQuadTree::GetObjectsIntersectArea(DgnRect& rect, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	if (m_pNode == NULL) return 0;
	return m_pNode->GetObjectsIntersectArea(rect, ObjSet);
}

INT	CDgnQuadTree::GetObjectsWithinPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	if (m_pNode == NULL) return 0;
	return m_pNode->GetObjectsWithinPolygon(polygon, ObjSet);
}

INT	CDgnQuadTree::GetObjectsIntersectPolygon(DgnOB_Polygon& polygon, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	if (m_pNode == NULL) return 0;
	return m_pNode->GetObjectsIntersectPolygon(polygon, ObjSet);
}

INT	CDgnQuadTree::GetObjectsIntersectLine(DgnLine& line, CArray <CDgn_Object*, CDgn_Object*>& ObjSet)
{
	if (m_pNode == NULL) return 0;
	return m_pNode->GetObjectsIntersectLine(line, ObjSet);
}


//===================================================================================================================
#include "..\wg_db\mathfunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "DgnDataCtrl.h"
BOOL  CDgnQuadTree::MakePlateQuadTree(const CArray<unsigned int, unsigned int>& aPlateKeyList)
{
	Initialize();

	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	T_ELEM_D TmpElemD;
	T_NODE_D TmpNodeD;

	arElemK.RemoveAll();
	arElemK.Copy(aPlateKeyList);
	DgnOB_Polygon* pGrObject = NULL;

	double MdlXMin,MdlXMax,MdlYMin,MdlYMax,MdlZMax,MdlZMin;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->calcRangeModel(MdlXMin, MdlYMin, MdlZMin, MdlXMax, MdlYMax, MdlZMax);

	CDgnQuadNode::m_dSizeX    = fabs(MdlXMax-MdlXMin)/50.0; // 한축에 50개 
	CDgnQuadNode::m_dSizeY    = fabs(MdlYMax-MdlYMin)/50.0; // 한축에 50개 
	CDgnQuadNode::m_dSizeMinX = fabs(MdlXMax-MdlXMin)/10000.0;
	CDgnQuadNode::m_dSizeMinY = fabs(MdlYMax-MdlYMin)/10000.0;

	int nNodeNum;
	for(int nElem=0; nElem<arElemK.GetSize(); nElem++)
	{
		if(!pDoc->m_pAttrCtrl->GetElem(arElemK[nElem],TmpElemD))	{ASSERT(0);}
		if(!pDoc->m_pAttrCtrl->IsPlate(TmpElemD.eltyp))continue; // Plate아니면 통과 

		nNodeNum=0;
		for(int i=0; i<D_ELEM_MAXNOD; i++)
		{
			if(TmpElemD.elnod[i]==0) break;
			nNodeNum++;
		}
		pGrObject = NULL; 
		pGrObject = new DgnOB_Polygon(arElemK[nElem]);
		if(!pGrObject)
		{
			return FALSE; 
		}

		for(int nNode=0; nNode<nNodeNum; nNode++)
		{
			if(!pDoc->m_pAttrCtrl->GetNode(TmpElemD.elnod[nNode],TmpNodeD))	{continue;}
			pGrObject->AddVertex(TmpNodeD.x,TmpNodeD.y,TmpNodeD.z);
		}
		Insert(pGrObject);
	}
	return TRUE; 
}

BOOL  CDgnQuadTree::CalcIntersectElem(double Pt1[3],double Pt2[3],CArray<UINT,UINT> &arElemK)
{
	arElemK.RemoveAll(); 

	DgnLine Dgn_line;
	CArray<CDgn_Object*,CDgn_Object*> arObject;

	Dgn_line.P1.Set(Pt1[0],Pt1[1],Pt1[2]);
	Dgn_line.P2.Set(Pt2[0],Pt2[1],Pt2[2]);
	GetObjectsIntersectLine(Dgn_line,arObject);

	T_NODE_D CutlP1,CutlP2,TmpPt;
	CutlP1.Initialize();
	CutlP1.x = Pt1[0];
	CutlP1.y = Pt1[1];
	CutlP2.Initialize();
	CutlP2.x = Pt2[0];
	CutlP2.y = Pt2[1];

	CArray<T_NODE_D,T_NODE_D&> arPolyPt;
	Point3D *pObjectPt;
	for(int i=arObject.GetSize()-1; i>=0; i--)
	{
		arPolyPt.RemoveAll(); 
		for(int j=0; j<((DgnOB_Polygon *)arObject[i])->GetVertexNum(); j++)
		{
			pObjectPt=((DgnOB_Polygon *)arObject[i])->GetVertex(j);
			TmpPt.Initialize();
			TmpPt.x = pObjectPt->x;
			TmpPt.y = pObjectPt->y;
			TmpPt.z = pObjectPt->z;
			arPolyPt.Add(TmpPt);
		}
		if(!IsIntersect2D(CutlP1, CutlP2, arPolyPt))
			arObject.RemoveAt(i); 
	}
	for(int i=0; i<arObject.GetSize(); i++)
		arElemK.Add(((DgnOB_Polygon *)arObject[i])->m_nKey);

	return TRUE; 
}


BOOL  CDgnQuadTree::CalcInAreaElem(DgnRect& rect, double dZ, CArray<UINT,UINT> &arElemK, double Tolerance)
{
	arElemK.RemoveAll(); 

	CArray<CDgn_Object*,CDgn_Object*> arObject;

	GetObjectsWithinArea(rect,arObject);

	CArray<T_NODE_D,T_NODE_D&> arPolyPt;
	Point3D *pObjectPt;
	for(int i=arObject.GetSize()-1; i>=0; i--)
	{
		int nObj = ((DgnOB_Polygon *)arObject[i])->GetVertexNum();

		pObjectPt=((DgnOB_Polygon *)arObject[i])->GetVertex(nObj-1);
	}
	for(int i=0; i<arObject.GetSize(); i++)
		arElemK.Add(((DgnOB_Polygon *)arObject[i])->m_nKey);

	return TRUE; 
}

BOOL CDgnQuadTree::IsIntersect2D(T_NODE_D& P1, T_NODE_D& P2, CArray<T_NODE_D, T_NODE_D&>& arPolygon)
{
	int nNum = arPolygon.GetSize();
	double dp1[2] = {P1.x, P1.y};
	double dp2[2] = {P2.x, P2.y};

	double dPoly[4][2];
	BOOL bInterSect = FALSE;
	for(int i=0; i<nNum; i++)
	{
		T_NODE_D Pnt = arPolygon[i];
		dPoly[i][0] = Pnt.x;
		dPoly[i][1] = Pnt.y;

		int j = i==(nNum-1) ? 0 : i+1;
		T_NODE_D P3 = arPolygon[i];
		T_NODE_D P4 = arPolygon[j];

		double dp3[2] = {P3.x, P3.y};
		double dp4[2] = {P4.x, P4.y};    
		int nSect = CMathFunc::mathIntersect_ccw2D_Tol(dp1, dp2, dp3, dp4);

		if(nSect > -1)
		{
			bInterSect = TRUE;
			break;
		}
	}
	if(!bInterSect)
	{
		bInterSect = CMathFunc::mathIsInsidePoint2D(dp1, nNum, dPoly, TRUE);
		if(!bInterSect)
		{
			bInterSect = CMathFunc::mathIsInsidePoint2D(dp2, nNum, dPoly, TRUE);
		}
	}

	return bInterSect;
}