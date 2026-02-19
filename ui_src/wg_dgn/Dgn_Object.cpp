// Dgn_Object.cpp: implementation of the CDgn_Object class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_Object.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Point3D
Point3D::Point3D()
{
}

Point3D::~Point3D()
{
}

int Point3D::operator == (Point3D& point )
{
	if(fabs(x - point.x) >= LGLDBL_EPSILON) return 0;
	if(fabs(y - point.y) >= LGLDBL_EPSILON) return 0;
	if(fabs(z - point.z) >= LGLDBL_EPSILON) return 0;
	
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CDgn_Object

CDgn_Object::CDgn_Object()
{
	//m_bDoNotDraw = FALSE;
	m_bSelected = FALSE;
}

CDgn_Object::~CDgn_Object()
{
}

/////////////////////////////////////////////////////////////////////////////
// DgnOB_Point

DgnOB_Point::DgnOB_Point()
{
	m_nID = GOB_POINT;
}

DgnOB_Point::~DgnOB_Point()
{
}
/*
void DgnOB_Point::Draw(CDC* pDC, COLORREF nColor)
{

	if (m_bDoNotDraw)
	{
		if (nColor == (COLORREF)RGB(255, 0, 0))
			AfxMessageBox(_T("Oh my God!"));
		return;
	}
	CPen pen(PS_SOLID, 1, nColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo((INT)m_ptPos.x-2, (INT)m_ptPos.y-2);
	pDC->LineTo((INT)m_ptPos.x+2, (INT)m_ptPos.y+2);
	pDC->MoveTo((INT)m_ptPos.x+2, (INT)m_ptPos.y-2);
	pDC->LineTo((INT)m_ptPos.x-2, (INT)m_ptPos.y+2);

	pDC->SelectObject(pOldPen);
	return;
}
*/

Point3D* DgnOB_Point::GetPointForQT(int Index)
{
	if (Index != 0) return NULL;
	return &m_ptPos;
}

LDOUBLE DgnOB_Point::NearDist(Point3D FromP)
{
	LDOUBLE Dx,Dy;
	//,Dz;
	Dx = FromP.x - m_ptPos.x;
	Dy = FromP.y - m_ptPos.y;
	//Dz = 0.0;
	return sqrt(Dx *Dx + Dy*Dy);
}

/////////////////////////////////////////////////////////////////////////////
// DgnOB_Line

DgnOB_Line::DgnOB_Line()
{
	m_nID = GOB_LINE;
}

DgnOB_Line::~DgnOB_Line()
{
}

/*
void DgnOB_Line::Draw(CDC* pDC, COLORREF nColor)
{

	if (m_bDoNotDraw)
	{
		if (nColor == (COLORREF)RGB(255, 0, 0))
			AfxMessageBox(_T("Oh my God!"));
		return;
	}
	CPen pen(PS_SOLID, 1, nColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo((INT)m_ptPos1.x, (INT)m_ptPos1.y);
	pDC->LineTo((INT)m_ptPos2.x, (INT)m_ptPos2.y);

	pDC->SelectObject(pOldPen);
	return;
}
*/

Point3D* DgnOB_Line::GetPointForQT(int Index)
{
	switch(Index)
	{
	case 0:
		return &m_ptPos1;
	case 1:
		return &m_ptPos2;
	default:
		return NULL;
	}
}

LDOUBLE DgnOB_Line::NearDist(Point3D FromP)
{
	double 	a,b,c,a1,b1,c1;
	double	x,y,x1,y1,x2,y2;
	double 	Temp_d,Temp_1,Temp_2,Test;
	double	Temp_3;
	
	x = FromP.x;
	y = FromP.y;

	x1 = m_ptPos1.x;
	y1 = m_ptPos1.y;
	
	x2 = m_ptPos2.x;
	y2 = m_ptPos2.y;

	a=y2-y1;
	b=x1-x2;
	c=x2*y1-x1*y2;
	
	a1=b;
	b1=-a;
	c1=a*y-b*x;
	
	Test=(a1*x1+b1*y1+c1)*(a1*x2+b1*y2+c1);
	
	if (Test<0)
	{	
		Temp_3=(double)(a*x+b*y+c);Temp_3=Temp_3*Temp_3;
		Temp_d=(long)(Temp_3/(double)(a*a+b*b));
	}
	else
	{	
		Temp_1=((x1-x)*(x1-x)+(y1-y)*(y1-y));
		Temp_2=((x2-x)*(x2-x)+(y2-y)*(y2-y));
		Temp_d= (Temp_1<Temp_2) ? Temp_1 : Temp_2 ;
	}
	
	return Temp_d;
}

///////////////////////////////////////////////////////////////////////////
//// DgnOB_Polygon
DgnOB_Polygon::DgnOB_Polygon()
{
	m_nID = GOB_PLINE;
}

DgnOB_Polygon::DgnOB_Polygon(UINT nKey)
{
	m_nID = GOB_PLINE;
	m_nKey = nKey; 
}

Point3D* DgnOB_Polygon::GetPointForQT(int Index)
{
	if(Index>m_VertexList.GetSize()-1)
	{
		ASSERT(0);
		return 0; 
	}
	return m_VertexList[Index];
}

/*
void DgnOB_Polygon::Draw(CDC* pDC, COLORREF nColor)
{

	if (m_bDoNotDraw)
	{
		if (nColor == (COLORREF)RGB(255, 0, 0))
			AfxMessageBox(_T("Oh my God!"));
		return;
	}
	CPen pen(PS_SOLID, 1, nColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

	//pDC->MoveTo((INT)m_ptPos1.x, (INT)m_ptPos1.y);
	//pDC->LineTo((INT)m_ptPos2.x, (INT)m_ptPos2.y);
	Point3D *pSt,*pEd;
	for(int i=0; i<m_VertexList.GetSize(); i++)
	{
		if(i==m_VertexList.GetSize()-1)
		{
			pSt = m_VertexList[i];
			pEd = m_VertexList[0];
		}
		else
		{
			pSt = m_VertexList[i];
			pEd = m_VertexList[i+1];
		}
		pDC->MoveTo((INT)pSt->x, (INT)pSt->y);
		pDC->LineTo((INT)pEd->x, (INT)pEd->y);
	}

	pDC->SelectObject(pOldPen);
	return;
}
*/

DgnOB_Polygon::~DgnOB_Polygon()
{
	// Vertext List 정리 
	ClearVertexList();	
}


void DgnOB_Polygon::ClearVertexList()
{
	int Size = m_VertexList.GetSize();
	for(int i = 0; i < Size ; i++)
	{
		delete m_VertexList[i];
	}
	m_VertexList.RemoveAll();

}
/***************************************************
@@	현재 Polygon을 이루는 Vertex의 갯수를 반환한다. 
@@	Polygon의 적합성 여부는 개선사항 
**/
int DgnOB_Polygon::AddVertex(Point3D point)
{
	Point3D *pVertex = new Point3D();
	(*pVertex) = point;
	m_VertexList.Add(pVertex);
	return m_VertexList.GetSize();
}

int DgnOB_Polygon::AddVertex(LDOUBLE X,LDOUBLE Y, LDOUBLE Z)
{
	Point3D *pVertex = new Point3D();
	pVertex->Set(X,Y,Z);
	m_VertexList.Add(pVertex);
	return m_VertexList.GetSize();
}


/*************************************************************************************
@@	GViewOperation에서 Copy된 Routine
**/
LBOOL GetLineIntersectPoint(Point3D P1,Point3D P2, Point3D P3,Point3D P4,
											  Point3D& IntersectPoint)
{
	
	LDOUBLE A,B,C,D;
	LDOUBLE T,V;
	LDOUBLE DivT,DivV;
	
	A = P4.x - P3.x;
	B = P2.x - P1.x;
	C = P4.y - P3.y;
	D = P2.y - P1.y;

	DivT = A*D - B*C; 
	DivV = B*C - A*D;
	
	if(DivT != 0.0 && DivV != 0.0)
	{
		T = (A*P3.y + C*P1.x - C*P3.x - A*P1.y) / DivT;
		V = (B*P1.y + D*P3.x - B*P3.y - D*P1.x) / DivV;

		if( (T > -LGL_LINE_INTERSECT_EPSILON && T < 1.0 + LGL_LINE_INTERSECT_EPSILON) &&
			(V > -LGL_LINE_INTERSECT_EPSILON && V < 1.0 + LGL_LINE_INTERSECT_EPSILON))
		{
			IntersectPoint.x = P1.x + B * T;
			IntersectPoint.y = P1.y + D * T;
			IntersectPoint.z = 0.0;
	
			return TRUE;
		}
	}
	IntersectPoint.Set(0,0,0);
	return FALSE;
}


LBOOL GetLineIntersectPoint(DgnLine L1, DgnLine L2, Point3D& IntersectPoint)
{
	return GetLineIntersectPoint(L1.P1,L1.P2,L2.P1,L2.P2,IntersectPoint);

}


LBOOL DgnOB_Polygon::IsInner(Point3D point)
{
	//폴리곤 외곽의 Rectangle을 설정한다.
	//폴리곤 에 완전히 포함되지 않고 y 좌표가 검사하려는 point와 같은 한점을 설정한다.(Op)
	//Op와 point로 이루어지는 직선을 정의 하고 
	//이점이 Polygon을 이루는 직선과 만나는 점을 검사한다. 
	//만나는 정점의 갯수가 홀수이면 내부에 존재 
	//만나는 정점의 갯수가 짝수이면 외부에 존재 
	//Vertext에서 정점이 생기면 나머지 Vertex에 연결된 나머지 두정점과 의 관계를 고려  짝수 또는 
	// 홀수를 결정한다. 나머지 두정점이 만나는 정점을 기준으로 같은 방향( 현재는 Y좌표 기준)이면 
	// 짝수 만남 , 다른 방향이면 홀수만남으로 취급한다.
	//최종적으로 만남 Test가 완료된후 홀수 만남이면 내부에 존재 
	//짝수 만남이면 외부에 존재하는 것으로 판단한다. 
	
	Point3D MinP,MaxP,TempP,OutterP;
	//OutterP는 최외곽 점을 정의 한다. 
		int VertexNum = m_VertexList.GetSize();
	//다각형의 조건을 만족하지 않으면 포함 테스트를 수행하지 않는다. 
	if(VertexNum < 3) return FALSE;
	
	MinP = *m_VertexList[0];
	MaxP = *m_VertexList[0];	
	MinP.z = 0.0;
	MaxP.z = 0.0;

	for(int i = 1; i < VertexNum; i++)
	{
		TempP = *m_VertexList[i];
		
		if(MinP.x > TempP.x) MinP.x = TempP.x;
		if(MinP.y > TempP.y) MinP.y = TempP.y;
		if(MaxP.x < TempP.x) MaxP.x = TempP.x;
		if(MaxP.y < TempP.y) MaxP.y = TempP.y;
	}

	DgnRect OR;
	OR.Set(MinP.x,MinP.y,MaxP.x,MaxP.y);
	//폴리곤에 완전히 포함 되지 않는 point는 포함 테스트를 생략한다.
	if(!OR.IsInner(point)) return FALSE;

	//폴리곤에 완전히 포함 되지 않는 외부의 한점을 정의 한다. 
	OutterP.x = MinP.x - 100;
	OutterP.y = point.y;
	OutterP.z = 0.0;

	
	LBOOL bIsInner = FALSE;
	
	
	
	DgnLine LToTest,TempL;
	Point3D IntersectPoint; 
	// 폴리곤 외부의 점과 포함테스트를 수행할 점으로 이루어진 직선을 정의 한다. 
	// 현재는 X축에 평행한 직선으로 정의 된다. 
	// 차후 Intersect Test 최적화를 수행할것 
	// 현재는 일반적인 Line Intersect Test를 수행하고 있다. 
	LToTest.Set(OutterP,point);
	LBOOL bIntersectVertex;
	for(int i = 0 ; i < VertexNum ; i ++)
	{
	  if(i == VertexNum -1)
		  TempL.Set(*m_VertexList[i],*m_VertexList[0]);
	  else
	  	  TempL.Set(*m_VertexList[i],*m_VertexList[i+1]);
	  if(bIntersectVertex = GetLineIntersectPoint(LToTest,TempL,IntersectPoint))
	  {
		  if (fabs(point.y -TempL.P1.y) <= LGL_LINE_INTERSECT_EPSILON)
			  bIntersectVertex = FALSE;
	  }
	  
	  //if(TRUE == GetLineIntersectPoint(LToTest,TempL,IntersectPoint))
	  if (bIntersectVertex)
	  {
			//산출된 교점이 Polygon 의Vertex와  일치하는지 검사한다. 
		    //Vertex와 일치하면 교점에 대한 방향판단으로 짝수, 홀수 검사를 수행한다. 
			for(int j = 0 ; j < VertexNum ; j ++)
			{
				if( fabs(IntersectPoint.y - m_VertexList[j]->y) <= LGL_LINE_INTERSECT_EPSILON)
				{
					if(fabs(IntersectPoint.x - m_VertexList[j]->x) <= LGL_LINE_INTERSECT_EPSILON)
					{
						//폴리곤의 정점에서 만났을때 앞뒤 정점과 의 관계를 검사 한다. (현재는 Y좌표 기준)
						//같은 방향이면 짝수로 다른 방향 이면 홀수로 취급한다. 
						LDOUBLE Fy,My,Ny;
						if(j == 0) 
							Fy = m_VertexList[VertexNum-1]->y;
						else
							Fy = m_VertexList[j -1]->y;
						if(j == VertexNum -1)
							Ny = m_VertexList[0]->y;
						else
							Ny = m_VertexList[j+1]->y;
						
						My = IntersectPoint.y;
						//My = point.y;

						//if((Fy < My && Ny < My) ||(Fy > My && Ny > My))
						if((Fy < My && Ny < My) ||(Fy > My && Ny > My))
						{
							//짝수 만남 
							//짝수 만남은 bIsInner Flag에 영향을 미치지 않으므로 두번에 걸치 !연산으로 
							//처리한다. 
							bIsInner = !bIsInner;
						}
						// 홀수 만남에 대한 bIsInner Flag의 설정은 loop외부에서 처리한다. 
					}
				}
			}
			bIsInner =  !bIsInner;
	  }
	}
	return bIsInner;
}

/*
LBOOL DgnOB_Polygon::IsInner(DgnOB_Line Line)
{

}
	
int DgnOB_Polygon::RemoveRecentVertex()
{
	//CArray
	int Size = m_VertexList.GetSize();
	
	Point3D *pVertex = m_VertexList[Size-1]

	m_VertexList.RemoveAt(Size-1);

	return Size-1;

}
*/

LBOOL DgnOB_Polygon::IsWithin(Point3D point)
{
	return IsInner(point);
}


LBOOL DgnOB_Polygon::IsWithin(DgnOB_Line Line)
{
	Point3D P1,P2,IntersectPoint;
	int VertexNum = m_VertexList.GetSize();
	DgnLine TempL,TestLine;
	TestLine.P1 = *Line.GetPointForQT(0);
	TestLine.P2 = *Line.GetPointForQT(1);


	P1 = *Line.GetPointForQT(0);
	P2 = *Line.GetPointForQT(1);

	if(IsInner(P1) && IsInner(P2)) return TRUE;
	//주어진 Line이 폴리곤을 이루는 Line과 교차하면 
	// 주어진 Line은 명백하게 Polygon에 완전히 포함되지 않는다. 
	for(int i = 0 ; i < VertexNum ; i ++)
	{
	  if(i == VertexNum -1)
		  TempL.Set(*m_VertexList[i],*m_VertexList[0]);
	  else
	  	  TempL.Set(*m_VertexList[i],*m_VertexList[i+1]);
	  
	  if(TRUE == GetLineIntersectPoint(TestLine,TempL,IntersectPoint))	return FALSE;
	}
	
	return FALSE;
}

LBOOL DgnOB_Polygon::IsWithin(CDgn_Object* pObject)
{
	switch(pObject->WhoAmI())
	{
	case GOB_POINT:
		return IsWithin(*(pObject->GetPointForQT(0)));
	case GOB_LINE:
		return IsWithin(*((DgnOB_Line*)pObject));
	}
	return FALSE;
}

LBOOL DgnOB_Polygon::IsIntersect(CDgn_Object* pObject)
{
	switch(pObject->WhoAmI())
	{
	case GOB_POINT:
		return IsWithin(*(pObject->GetPointForQT(0)));
	case GOB_LINE:
		return IsIntersect(*((DgnOB_Line*)pObject));
	}
	return FALSE;
}

LBOOL DgnOB_Polygon::IsIntersect(DgnOB_Line Line)
{
	Point3D P1,P2,IntersectPoint;
	int VertexNum = m_VertexList.GetSize();
	DgnLine TempL,TestLine;
	TestLine.P1 = *Line.GetPointForQT(0);
	TestLine.P2 = *Line.GetPointForQT(1);

	P1 = *Line.GetPointForQT(0);
	P2 = *Line.GetPointForQT(1);

	if(IsInner(P1)) return TRUE;
	if(IsInner(P2)) return TRUE;
	
	//주어진 Line이 폴리곤을 이루는 Line과 교차하면 
	// 주어진 Line은 명백하게 Polygon에 반드시 교차한다. 
	for(int i = 0 ; i < VertexNum ; i ++)
	{
	  if(i == VertexNum -1)
		  TempL.Set(*m_VertexList[i],*m_VertexList[0]);
	  else
	  	  TempL.Set(*m_VertexList[i],*m_VertexList[i+1]);
	  
	  if(TRUE == GetLineIntersectPoint(TestLine,TempL,IntersectPoint))	return TRUE;
	}

	return FALSE;
}

LBOOL DgnOB_Polygon::IsWithin(DgnRect Rect)
{
	Point3D LB,LT,RT,RB;

	LB.Set(Rect.left,Rect.bottom,0.0);
	LT.Set(Rect.left,Rect.top,0.0);
	RT.Set(Rect.right,Rect.top,0.0);
	RB.Set(Rect.right,Rect.bottom,0.0);

	int VertexNum = m_VertexList.GetSize();
	
	// 폴리곤의 정점이 주어진 Rect에 포함되면 
	// Rect는 Polygon에 완전히 포함되지 않는다 . 
	for(int i = 0 ; i < VertexNum ; i++)
	{
		if(TRUE == Rect.IsInner(*m_VertexList[i])) return FALSE;
	}
	
	if(IsInner(LB) && IsInner(LT) && IsInner(RT) && IsInner(RB)) return TRUE;
	
	return FALSE;
}


LBOOL DgnOB_Polygon::IsIntersect(DgnRect Rect)
{
	Point3D LB,LT,RT,RB;

	LB.Set(Rect.left,Rect.bottom,0.0);
	LT.Set(Rect.left,Rect.top,0.0);
	RT.Set(Rect.right,Rect.top,0.0);
	RB.Set(Rect.right,Rect.bottom,0.0);

	int VertexNum = m_VertexList.GetSize();
	
	// 폴리곤의 정점이 주어진 Rect에 포함되면 
	// Rect는 Polygon에 반드시 Intersect 된다 . 
	for(int i = 0 ; i < VertexNum ; i++)
	{
		if( TRUE == Rect.IsInner(*m_VertexList[i])) return TRUE;
	}
	if(IsInner(LB)) return TRUE;
	if(IsInner(LT)) return TRUE;
	if(IsInner(RT)) return TRUE;
	if(IsInner(RB)) return TRUE;

	
	Point3D IntersectPoint;
	
	DgnLine L1,L2,L3,L4,TempL;
	L1.Set(LB,RB);
	L2.Set(RB,RT);
	L3.Set(RT,LT);
	L4.Set(LT,LB);
	//주어진 L1,L2,L3,L4중 하나가  폴리곤을 이루는 Line과 교차하면 
	//주어진 Rect는 명백하게 Polygon에 반드시 교차한다. 
	for(int i = 0 ; i < VertexNum ; i ++)
	{
	  if(i == VertexNum -1)
		  TempL.Set(*m_VertexList[i],*m_VertexList[0]);
	  else
	  	  TempL.Set(*m_VertexList[i],*m_VertexList[i+1]);
	  
	  if(TRUE == GetLineIntersectPoint(L1,TempL,IntersectPoint))	return TRUE;
	  if(TRUE == GetLineIntersectPoint(L2,TempL,IntersectPoint))	return TRUE;
	  if(TRUE == GetLineIntersectPoint(L3,TempL,IntersectPoint))	return TRUE;
	  if(TRUE == GetLineIntersectPoint(L4,TempL,IntersectPoint))	return TRUE;
	}

		
	return FALSE;
}

Point3D * DgnOB_Polygon::GetVertex(int Index)
{
	if( Index < 0 || Index > m_VertexList.GetSize()-1) return NULL;
	return m_VertexList[Index];
}

LINT    DgnOB_Polygon:: GetVertexNum()
{
	return m_VertexList.GetSize();
}


/////////////////////////////////////////////////////////////////////////////
// DgnRect

DgnRect::DgnRect()
{
	left   = 0.0;
	top    = 0.0;
	right  = 0.0;
	bottom = 0.0;
}

DgnRect::DgnRect(LDOUBLE Left, LDOUBLE Bottom, LDOUBLE Right, LDOUBLE Top)
{
	left   = Left;
	bottom = Bottom;
	right  = Right;
	top    = Top;
}

DgnRect::~DgnRect()
{

}

void DgnRect::Set(LDOUBLE Left, LDOUBLE Bottom, LDOUBLE Right, LDOUBLE Top)
{
	left   = Left;
	bottom = Bottom;
	right  = Right;
	top    = Top;
}

LDOUBLE DgnRect::Width()
{
	return fabs(right - left);
}

LDOUBLE DgnRect::Height()
{
	return fabs(top - bottom);
}

LBOOL DgnRect::IsInner(Point3D point)
{
	NormalizeRect();
	if(left <= point.x && bottom <= point.y &&
		top >= point.y && right  >= point.x) return TRUE;
	return FALSE;
}

/*****************************************************************************
@@	포함 테스트를 수행하기 위한 Flag를 설정한다. 
@@	Line Clipping알고리즘에서 
**/
void _SetIntersectFlag(DgnRect &Window,LINT &P1Flag,LINT &P2Flag,Point3D P1,Point3D P2)
{
	/********************************************************************
	@@	B4 | B3 | B2 | B1 
	@@	-----------------------------------------------------------------
	@@	B1 : 선분의 끝점이 Window의 위쪽에 놓이는 경우         : true
	@@	B2 : 선분의 끝점이 Window의 아래쪽에 놓이는 경우      : true
	@@	B3 : 선분의 끝점이 Window의 오른쪽에 놓이는 경우      : true
	@@	B4 : 선분의 끝점이 Window의 왼쪽에 놓이는 경우         : true 
	@@  -----------------------------------------------------------------
	@@	Bit Field로 구성함이 합당하나 현재는 Hex Field로 구성되어 있다.
	*/
	memset(&P1Flag,0,sizeof(LINT)); // P1Flag 초기화 
	memset(&P2Flag,0,sizeof(LINT)); // P2Flag 초기화  
	/******************************************************************************
	@@	주어진 점 P1과 P2에 대한 Clipping Flag를 설정한다 
	*/
	if(P1.x < Window.left)    
		P1Flag |= 0x1000;				//	B1 : P1이 Window의 왼쪽에 놓이는 경우  
	if(P1.x > Window.right)      
		P1Flag |= 0x0100;				//	B2 : P1이 Window의 오른쪽에 놓이는 경우
	if(P1.y > Window.top)		
		P1Flag |= 0x0001;				//	B3 : P1이 Window의 위쪽에 놓이는 경우
	if(P1.y < Window.bottom)
		P1Flag |= 0x0010;				//	B4 : P1이 Window의 아래쪽에 놓이는 경우  

	
	if(P2.x < Window.left)
		P2Flag |= 0x1000;				//	B1 : P2가 Window의 왼쪽에 놓이는 경우  
	if(P2.x > Window.right)
		P2Flag |= 0x0100;				//	B2 : P2가 Window의 오른쪽에 놓이는 경우
	if(P2.y > Window.top)
		P2Flag |= 0x0001;				//	B3 : P2가 Window의 위쪽에 놓이는 경우
	if(P2.y < Window.bottom)
		P2Flag |= 0x0010;				//	B4 : P2가 Window의 아래쪽에 놓이는 경우  
}

LBOOL DgnRect::IsIntersect(CDgn_Object* pObject)
{
	LINT P1Flag,P2Flag;
	
	Point3D *pPoint1, *pPoint2;
	Point3D pt1, pt2;
	LINT nGeomType = pObject->WhoAmI();
	pPoint1 = pObject->GetPointForQT(0);
	pt1.x = pPoint1->x;
	pt1.y = pPoint1->y;
	NormalizeRect();
	switch(nGeomType)
	{
	case GOB_POINT:
		if(left <= pt1.x && bottom <= pt1.y &&
			top >= pt1.y && right  >= pt1.x) return TRUE;
		return FALSE;
	case GOB_LINE:
		pPoint2 = pObject->GetPointForQT(1);
		pt2.x = pPoint2->x;
		pt2.y = pPoint2->y;
		_SetIntersectFlag(*this, P1Flag, P2Flag, pt1, pt2);
		if( (P1Flag & P2Flag)  != 0x0000 ) return FALSE;
		return TRUE;

	//KYE-HONG-2003-03-12 : 필요없음 
	/*
	case GOB_PLINE:
		{
			int i,nStIdx,nEdIdx; 
			int nPolyNum = ((DgnOB_Polygon *)pObject)->GetVertexNum();
			CArray<Point3D*,Point3D*> arPolyPt;
			BOOL bInterSect=FALSE; 
			for(i=0; i< nPolyNum; i++)
				arPolyPt.Add(((DgnOB_Polygon *)pObject)->GetVertex(i));
			for(i=0; i< nPolyNum; i++)
			{
				if(i==nPolyNum-1) { nStIdx=i; nEdIdx=0;}
				else              { nStIdx=i; nEdIdx=i+1;}
				pt1.x = arPolyPt[nStIdx]->x;
				pt1.y = arPolyPt[nStIdx]->y;
				pt2.x = arPolyPt[nEdIdx]->x;
				pt2.y = arPolyPt[nEdIdx]->y;

				_SetIntersectFlag(*this, P1Flag, P2Flag, pt1, pt2);
		    if( (P1Flag & P2Flag)  != 0x0000 ) return FALSE;
			}
		} 
		break; 
	*/    
	}
	return TRUE;
}
/******************************************************************************
@@	Line이 완전히 포함될때 TRUE를 반환한다.
@@	제시되는 Line은 2D Line Object임을 가정한다. 
**/
LBOOL DgnRect::IsWithin(CDgn_Object* pObject)
{
	LINT P1Flag,P2Flag;
	
	Point3D *pPoint1, *pPoint2;
	Point3D pt1, pt2;
	LINT nGeomType = pObject->WhoAmI();
	pPoint1 = pObject->GetPointForQT(0);
	pt1.x = pPoint1->x;
	pt1.y = pPoint1->y;
	NormalizeRect();
	switch(nGeomType)
	{
	case GOB_POINT:
		if(left <= pt1.x && bottom <= pt1.y &&
			top >= pt1.y && right  >= pt1.x) return TRUE;
		return FALSE;
	case GOB_LINE:
		pPoint2 = pObject->GetPointForQT(1);
		pt2.x = pPoint2->x;
		pt2.y = pPoint2->y;
		_SetIntersectFlag(*this,P1Flag,P2Flag,pt1,pt2);
		if(P1Flag == 0x0000 && P2Flag == 0x0000) return TRUE;
		return FALSE;
	}

	return FALSE;
}


void DgnRect::NormalizeRect()
{
	LDOUBLE Temp;
	
	if(left > right)
	{
		Temp  = left;
		left  = right;
		right = Temp;
	}

	if(bottom > top)
	{
		Temp   = bottom;
		bottom = top;
		top    = Temp;
	}
}

/**********************************************************************
@@	DgnLine
@@	Viewing 변환에서 사용될 Line Object
@@	Grid 생성시 
*/
DgnLine::DgnLine()
{
}

DgnLine::~DgnLine()
{

}

DgnLine& DgnLine::operator = (const DgnLine& Source)
{
	 	memcpy(&P1.x,&Source.P1.x,sizeof(LDOUBLE));
	memcpy(&P1.y,&Source.P1.y,sizeof(LDOUBLE));
	memcpy(&P1.z,&Source.P1.z,sizeof(LDOUBLE));
	
	memcpy(&P2.x,&Source.P2.x,sizeof(LDOUBLE));
	memcpy(&P2.y,&Source.P2.y,sizeof(LDOUBLE));
	memcpy(&P2.z,&Source.P2.z,sizeof(LDOUBLE));
	
	return (*this);
}

void DgnLine::Set(const Point3D& p1,const Point3D& p2)
{
	P1 = p1;
	P2 = p2;
}

CString DgnLine::GetString(CString Format)
{
	CString StrTemp;
	StrTemp.Format(Format,
		P1.x,P1.y,P1.z,P2.x,P2.y,P2.z);
	
	return StrTemp;
}

void DgnLine::SetZero()
{
	P1.Set(0.0,0.0,0.0);
	P2.Set(0.0,0.0,0.0);
}

LBOOL	DgnLine::IsIntersect(CDgn_Object* pObject)
{
	ASSERT(0);
	return FALSE; 
}

/***********************************************************
@@	현재 Line을 구성하는 점들이 유효한 Line을 구성하는 조건
@@	인지를 검사한다. 즉 Line을 구성하는 두점이 같은 점이면 
@@	Line은 유효하지 않은 Line으로 판정한다. 
**/
LBOOL DgnLine::IsValid()
{
	if(P1 == P2 ) return FALSE;
	return TRUE;
}

/******************************************************************
@@	현재 Line을 지정된 Delta값 만큼 이동시킨다. 
**/
LDOUBLE DgnLine::GetMinX(LDOUBLE CurMinX)
{
	LDOUBLE MinX;
	MinX = CurMinX;

	MinX = min(P1.x,MinX);
	MinX = min(P2.x,MinX);

	return MinX;
}

LDOUBLE DgnLine::GetMinY(LDOUBLE CurMinY)
{
	LDOUBLE MinY;
	MinY = CurMinY;

	MinY = min(P1.y,MinY);
	MinY = min(P2.y,MinY);

	return MinY;
}

LDOUBLE DgnLine::GetMaxX(LDOUBLE CurMaxX)
{
	LDOUBLE MaxX;
	MaxX = CurMaxX;

	MaxX = max(P1.x,MaxX);
	MaxX = max(P2.x,MaxX);

	return MaxX;
}

LDOUBLE DgnLine::GetMaxY(LDOUBLE CurMaxY)
{
	LDOUBLE MaxY;
	MaxY = CurMaxY;

	MaxY = max(P1.y,MaxY);
	MaxY = max(P2.y,MaxY);

	return MaxY;
}
