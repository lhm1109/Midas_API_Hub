#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"

#include "DrawBase.h"
#include "DrawFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UINT CDrawBase::m_nSnapDis = 2;

CDrawBase::CDrawBase()
{
	m_DrawFormatKey = 0;
	m_bIsSelect = FALSE;
	m_nSelectSubType = 1;
	m_bIsOverLapChk = FALSE;
	m_SnapType  = 0;
	m_LinkKey   = 0;
	m_dZero     = 0.000001;
	m_Boundary[0] = m_Boundary[1] = m_Boundary[2] = m_Boundary[3] = 0;
}

CDrawBase::~CDrawBase()
{
}

UINT CDrawBase::GetSnapDis() { return m_nSnapDis; }

void CDrawBase::SetSnapDis(UINT nSnapDis) { m_nSnapDis = nSnapDis; }

BOOL CDrawBase::IsInSnapArea(POINT P1, POINT P2)
{
	return (abs(P1.x - P2.x) <= (long)m_nSnapDis && abs(P1.y - P2.y) <= (long)m_nSnapDis);
}

BOOL CDrawBase::IsSnapCheck_Rect(const T_LINE& rectD, const T_LINE_R& rectD_R, BOOL bIsRData, POINT currPoint, POINT& snapPoint, T_POINT& snapPointR)
{
	POINT p1;

	if(IsInSnapArea(rectD.P1, currPoint))
	{
		snapPoint = rectD.P1;
		if(bIsRData) 
		{
			snapPointR.x = rectD_R.P1.x;
			snapPointR.y = rectD_R.P1.y;
			//snapPointR = rectD_R.P1;
		}
		return TRUE;
	}

	if(IsInSnapArea(rectD.P2, currPoint))
	{
		snapPoint = rectD.P2;
		if(bIsRData) 
		{
			snapPointR.x = rectD_R.P2.x;
			snapPointR.y = rectD_R.P2.y;
			//snapPointR = rectD_R.P2;
		}
		return TRUE;
	}		

	p1.x = rectD.P1.x;
	p1.y = rectD.P2.y;
	if(IsInSnapArea(p1, currPoint))
	{
		snapPoint = p1;
		if(bIsRData)
		{	
			snapPointR.x = rectD_R.P1.x;
			snapPointR.y = rectD_R.P2.y;
		}
		return TRUE;
	}

	p1.x = rectD.P2.x;
	p1.y = rectD.P1.y;
	if(IsInSnapArea(p1, currPoint))
	{
		snapPoint = p1;
		if(bIsRData)
		{	
			snapPointR.x = rectD_R.P2.x;
			snapPointR.y = rectD_R.P1.y;
		}
		return TRUE;
	}
	return FALSE;
}

void CDrawBase::GetBoundary(int& iXmin, int& iYmin, int& iXmax, int& iYmax, BOOL bReCalc)
{	
	if(bReCalc)
	{
		GetCalcBoundary(m_Boundary);
	}

	iXmin = m_Boundary[0];
	iYmin = m_Boundary[1];
	iXmax = m_Boundary[2];
	iYmax = m_Boundary[3];	
}

int CDrawBase::GetBoundaryIncludeR(double Boundary[4])
{
	//포함여부 (-1:영역이 겹치지 않는 경우, 0:자신의Boundary와 Boundary의 일부가 겹치는 경우, 1:자신의Boundary가 Boundary내부에 있는 경우, 2:자신의Boundary내부에 Boundary가 있는 경우, 3:영역이 일치는 경우
	double myBoundary[4];	
	GetCalcBoundaryR(myBoundary);

	if(fabs(myBoundary[0]-Boundary[0]) < m_dZero && fabs(myBoundary[1]-Boundary[1]) < m_dZero && fabs(myBoundary[2]-Boundary[2]) < m_dZero && fabs(myBoundary[3]-Boundary[3]) < m_dZero)
	{
		return 3;// 3:영역이 일치는 경우
	}

	if(myBoundary[0]<=Boundary[0] && myBoundary[1]<=Boundary[1] && myBoundary[2]>=Boundary[2] && myBoundary[3]>=Boundary[3])
	{
		return 2;// 2:자신의Boundary내부에 Boundary가 있는 경우
	}

	if(myBoundary[0]>=Boundary[0] && myBoundary[1]>=Boundary[1] && myBoundary[2]<=Boundary[2] && myBoundary[3]<=Boundary[3])
	{
		return 1;// 1:자신의Boundary가 Boundary내부에 있는 경우
	}

	if((myBoundary[0]>Boundary[2] || Boundary[0]>myBoundary[2]) && (myBoundary[1]>Boundary[3] || Boundary[1]>myBoundary[3]))
	{
		return -1;// -1:영역이 겹치지 않는 경우
	}

	return 0;// 0:자신의Boundary와 Boundary의 일부가 겹치는 경우
}

int CDrawBase::GetBoundaryInclude(int Boundary[4], BOOL bReCalc)
{
	//포함여부 (-1:영역이 겹치지 않는 경우, 0:자신의Boundary와 Boundary의 일부가 겹치는 경우, 1:자신의Boundary가 Boundary내부에 있는 경우, 2:자신의Boundary내부에 Boundary가 있는 경우, 3:영역이 일치는 경우
	int myBoundary[4];	
	
	if(bReCalc)
	{
		GetCalcBoundary(myBoundary);
	}
	else
	{
		memcpy(myBoundary, m_Boundary, sizeof(int) * 4);
	}

	if(abs(myBoundary[0]-Boundary[0]) < m_dZero && abs(myBoundary[1]-Boundary[1]) < m_dZero && abs(myBoundary[2]-Boundary[2]) < m_dZero && abs(myBoundary[3]-Boundary[3]) < m_dZero)
	{
		return 3;// 3:영역이 일치는 경우
	}

	if(myBoundary[0]<=Boundary[0] && myBoundary[1]<=Boundary[1] && myBoundary[2]>=Boundary[2] && myBoundary[3]>=Boundary[3])
	{
		return 2;// 2:자신의Boundary내부에 Boundary가 있는 경우
	}

	if(myBoundary[0]>=Boundary[0] && myBoundary[1]>=Boundary[1] && myBoundary[2]<=Boundary[2] && myBoundary[3]<=Boundary[3])
	{
		return 1;// 1:자신의Boundary가 Boundary내부에 있는 경우
	}

	if((myBoundary[0]>Boundary[2] || Boundary[0]>myBoundary[2]) || (myBoundary[1]>Boundary[3] || Boundary[1]>myBoundary[3]))
	{
		return -1;// -1:영역이 겹치지 않는 경우
	}

	return 0;// 0:자신의Boundary와 Boundary의 일부가 겹치는 경우
}

void CDrawBase::DrawOutLine(HDC DC, CDrawFunc* pDrawFunc, T_DRAW_FORMAT* pDrawFormat, int nOffset)
{
	T_LINE LineD;

	LineD.P1.x = min(m_Boundary[0], m_Boundary[2]) - nOffset;
	LineD.P1.y = min(m_Boundary[1], m_Boundary[3]) - nOffset;
	LineD.P2.x = max(m_Boundary[0], m_Boundary[2]) + nOffset;
	LineD.P2.y = max(m_Boundary[1], m_Boundary[3]) + nOffset;

	pDrawFunc->DrawRect(DC, *pDrawFormat, LineD); 
}

void CDrawBase::GetMoveVector(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat, double disVectorR[2], int disVector[2])
{
	disVectorR[0] = disVectorR[1] = 0.0;
	disVector[0]  = disVector[1]  = 0;

	if(bCDC)
	{
		disVector[0] = int(Vector[0]);
		disVector[1] = int(Vector[1]);
		
		if(pPaperFormat != NULL)
		{	
			disVectorR[0] = Vector[0] * pPaperFormat->ScaleX;	
			disVectorR[1] = (-1.0) * Vector[1] * pPaperFormat->ScaleY;
		}
		else
		{
			disVectorR[0] = disVectorR[1] = 0.0; 
		}
	}
	else
	{
		disVectorR[0] = Vector[0];	
		disVectorR[1] = Vector[1];
		
		if(pPaperFormat != NULL)
		{
			disVector[0] = math_lib::round_off((pPaperFormat->ScaleX==0.0)? 0.0 : Vector[0]/pPaperFormat->ScaleX);			
			disVector[1] = math_lib::round_off((pPaperFormat->ScaleY==0.0)? 0.0 : (-1.0)*Vector[1]/pPaperFormat->ScaleY); 
		}
		else
		{
			disVector[0] = disVector[1] = 0;
		}
	}
}

BOOL CDrawBase::IsPointOfLine(POINT p1, POINT p2, POINT tp, int nTol, BOOL isOnLine)
{
	// SHIN (2006.2.4) e-15로 했을경우 오차범위가 너무 좁음  // SHIN (2006.2.28) 오차범위를 선분의 길이의 e-15배로 수정// SHIN (2006.6.28) 오차범위를 선분의 길이의 e-11배로 수정 // SHIN (2010.7.27) 0.5 Pixel로 수정
	double dTol = (double)(nTol)+0.5;
	double bound1[2]   = {(double)(p1.x), (double)(p1.y)};
	double bound2[2]   = {(double)(p2.x), (double)(p2.y)};
	double targetPt[2] = {(double)(tp.x), (double)(tp.y)};

	if (fabs(targetPt[0]-bound1[0]) <= dTol && fabs(targetPt[1]-bound1[1]) <= dTol) return isOnLine;
	if (fabs(targetPt[0]-bound2[0]) <= dTol && fabs(targetPt[1]-bound2[1]) <= dTol) return isOnLine;

	double vector1[2];
	vector1[0] = bound1[0] - targetPt[0];
	vector1[1] = bound1[1] - targetPt[1];
	CMathFunc::mathNormalize2D(vector1, vector1);
	double vector2[2];
	vector2[0] = bound2[0] - targetPt[0];
	vector2[1] = bound2[1] - targetPt[1];
	CMathFunc::mathNormalize2D(vector2, vector2);
	double vector3[2];
	vector3[0] = bound2[0] - bound2[0];
	vector3[1] = bound2[1] - bound2[1];
	CMathFunc::mathNormalize2D(vector2, vector2);

	double dDis    = CMathFunc::mathDistanceToLine2D(bound1[0], bound1[1], bound2[0], bound2[1], targetPt[0], targetPt[1]);
	double dAngle1 = CMathFunc::mathCrossAngle2DSign(vector1[0], vector1[1], vector3[0], vector3[1]);
	double dAngle2 = CMathFunc::mathCrossAngle2DSign(vector2[0], vector2[1], (-1.0)*vector3[0], (-1.0)*vector3[1]);
	// 
	if (fabs(dDis)<=dTol && (fabs(dAngle1)-90) <= m_dZero && (fabs(dAngle2)-90) <= m_dZero) return TRUE;

	return FALSE;
}

BOOL CDrawBase::IsPointOfPolyLine(nrPOINT& arPoint, POINT tp, BOOL bClose, BOOL bFill, int nTol)
{
	int i, nSize;
	nSize = arPoint.GetSize();

	if(nSize <= 0) return FALSE;

	if(nSize == 1)
	{
		if (abs(tp.x - arPoint[0].x) <= nTol && abs(tp.y - arPoint[0].y) <= nTol) return TRUE;
		return FALSE;		
	}

	for(i = 0; i < nSize-1 ; i++)
	{			
		if(IsPointOfLine(arPoint[i], arPoint[i+1], tp, nTol, TRUE)) return TRUE; 
	}

	if(bClose)
	{
		if(IsPointOfLine(arPoint[0], arPoint[nSize-1], tp, nTol, TRUE)) return TRUE; 
	}

	if(bFill)
	{
		double targetPt[2] = {(double)(tp.x), (double)(tp.y)};

		double (*coor2D)[2];
		coor2D = new double[nSize][2];  		
		for(i = 0; i < nSize; i++)
		{
			coor2D[i][0] = arPoint[i].x;
			coor2D[i][1] = arPoint[i].y;
		}

		BOOL bChk = CMathFunc::mathIsInsidePoint2D(targetPt, nSize, coor2D, TRUE);

		delete [] coor2D;

		return bChk;
	}

	return FALSE;
}
BOOL CDrawBase::IsPointOfPolyLineR(nrT_POINT& arPoint, T_POINT tp, BOOL bClose, BOOL bFill)
{
	int i, nSize;
	double dTol = m_dZero;
	nSize = arPoint.GetSize();

	if(nSize <= 0) return FALSE;

	if(nSize == 1)
	{
		if (abs(tp.x - arPoint[0].x) <= dTol && abs(tp.y - arPoint[0].y) <= dTol) return TRUE;
		return FALSE;		
	}

	if(bFill)
	{
		double targetPt[2] = {(double)(tp.x), (double)(tp.y)};

		double (*coor2D)[2];
		coor2D = new double[nSize][2];  		
		for(i = 0; i < nSize ; i++)
		{
			coor2D[i][0] = arPoint[i].x;
			coor2D[i][1] = arPoint[i].y;
		}

		BOOL bChk = CMathFunc::mathIsInsidePoint2D(targetPt, nSize, coor2D, TRUE);

		delete [] coor2D;

		return bChk;
	}
	else
	{
		double bound1[2], bound2[2];
		double targetPt[2] = {tp.x, tp.y};

		for(i = 0; i < nSize - 1; i++)
		{			
			bound1[0] = arPoint[i].x;    bound1[1] = arPoint[i].y;
			bound2[0] = arPoint[i+1].x;  bound2[1] = arPoint[i+1].y;
			if(CMathFunc::mathIsPointOfLine2D(bound1, bound2, targetPt, TRUE)) return TRUE; 
		}
		if(bClose)
		{
			bound1[0] = arPoint[0].x;        bound1[0] = arPoint[i].y;
			bound2[0] = arPoint[nSize-1].x;  bound2[1] = arPoint[nSize-1].y;
			if(CMathFunc::mathIsPointOfLine2D(bound1, bound2, targetPt, TRUE)) return TRUE; 
		}
	}

	return FALSE;
}

void CDrawBase::GetArcPolyline(T_ARC& ArcD, int nCount, nrPOINT& arPoint)
{
	POINT  startP, originP; 
	double angle,  startAngle_rad;

	angle = ArcD.SweepAngle / ((double)nCount);
	startAngle_rad = math_lib::radian(ArcD.StartAngle);

	originP = ArcD.CenterP;		
	startP.x = originP.x + math_lib::round_off(ArcD.Radius * cos(startAngle_rad));
	startP.y = originP.y + math_lib::round_off(ArcD.Radius * sin(startAngle_rad));

	GetArcPolyline(startP, originP, angle, nCount, arPoint);	
}

void CDrawBase::GetArcPolyline(POINT startP, POINT originP, double angle, int nCount, nrPOINT& arPoint)
{	
	int nSize = abs(nCount) + 1;

	arPoint.RemoveAll();
	arPoint.SetSize(nSize);
	arPoint[0] = startP;

	double xy[2];
	double org[2];  
	org[0] = originP.x;
	org[1] = originP.y;
	double dSign = 1.0;
	
	for(int i = 1; i < nSize; i++)
	{
		xy[0]  = startP.x;
		xy[1]  = startP.y;
		math_lib::rotate_2d(xy, org, (double)i * angle);
		POINT pointR;
		pointR.x =  math_lib::round_off(xy[0]);  
		pointR.y = math_lib::round_off(xy[1]);
		arPoint[i] = pointR;
	}

	return;
}

void CDrawBase::GetArcPolylineR(T_ARC_R& ArcD, int nCount, T_POLYLINE_R& arcPolyline)
{
	CDrawFunc::GetArcPolylineR(ArcD, nCount, arcPolyline);
}

void CDrawBase::GetArcPolylineR(T_POINT startP, T_POINT originP, double angle, int nCount, T_POLYLINE_R& arcPolyline)
{
	CDrawFunc::GetArcPolylineR(startP, originP, angle, nCount, arcPolyline);
}

///////////////////////////////////
void CDrawBase::GetPossessionRegionR(T_POINT& PointD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = dXmin = PointD.x;
	dYmax = dYmin = PointD.y;
}

void CDrawBase::GetPossessionRegionR(T_LINE_R& LineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = max(LineD.P1.x, LineD.P2.x);
	dXmin = min(LineD.P1.x, LineD.P2.x);
	dYmax = max(LineD.P1.y, LineD.P2.y);
	dYmin = min(LineD.P1.y, LineD.P2.y);
}

void CDrawBase::GetPossessionRegionR(T_CIRCLE_R& CircleD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = CircleD.CenterP.x + CircleD.Radius;
	dXmin = CircleD.CenterP.x - CircleD.Radius;
	dYmax = CircleD.CenterP.y + CircleD.Radius;
	dYmin = CircleD.CenterP.y - CircleD.Radius;
}

void CDrawBase::GetPossessionRegionR(T_DONUT_R& DonutD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = DonutD.CenterP.x + DonutD.OutRadius;
	dXmin = DonutD.CenterP.x - DonutD.OutRadius;
	dYmax = DonutD.CenterP.y + DonutD.OutRadius;
	dYmin = DonutD.CenterP.y - DonutD.OutRadius;
}

void CDrawBase::GetPossessionRegionR(T_ARC_R& ArcD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	T_POLYLINE_R PolylineD;
	int nDiv = max(2, (int)(fabs(ArcD.SweepAngle/5.0)));
	GetArcPolylineR(ArcD, nDiv, PolylineD);

	GetPossessionRegionR(PolylineD, dXmax, dXmin, dYmax, dYmin);
}

void CDrawBase::GetPossessionRegionR(T_POLYLINE_R& PolylineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = dXmin = dYmax = dYmin = 0.0;
	if(! PolylineD.arPoint.GetSize()) return;

	dXmax = dXmin = PolylineD.arPoint[0].x;
	dYmax = dYmin = PolylineD.arPoint[0].y;

	int i ;
	//for(auto itr = PolylineD.arPoint.Data.begin() + 1; itr != PolylineD.arPoint.Data.end(); itr++) 
	for(i = 1 ; i < PolylineD.arPoint.GetSize(); i++)
	{
		T_POINT& PointRData = PolylineD.arPoint[i];

		dXmax = max(dXmax, PointRData.x);
		dXmin = min(dXmin, PointRData.x);
		dYmax = max(dYmax, PointRData.y);
		dYmin = min(dYmin, PointRData.y);
	}
}

void CDrawBase::GetPossessionRegionR(T_TEXT_R& TextD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(07.12.17) Text크기는 미고려
	dXmax = dXmin = TextD.PPosition.x;
	dYmax = dYmin = TextD.PPosition.y;
}

void CDrawBase::GetPossessionRegionR(T_DIMENSION_R& DimensionD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(07.12.17) Text 및 지시선 미고려
	dXmax = max(DimensionD.P1.x, DimensionD.P2.x);
	dXmin = min(DimensionD.P1.x, DimensionD.P2.x);
	dYmax = max(DimensionD.P1.y, DimensionD.P2.y);
	dYmin = min(DimensionD.P1.y, DimensionD.P2.y);
}

void CDrawBase::GetPossessionRegionR(T_ANGLE_R& AngleD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(07.12.17) Text 및 지시선 미고려
	dXmax = dXmin = AngleD.CenterP.x;
	dYmax = dYmin = AngleD.CenterP.y;
}

void CDrawBase::GetPossessionRegionR(T_LEADERLINE_R& LeaderLineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = dXmin = dYmax = dYmin = 0.0;
	if(! LeaderLineD.arPoint.GetSize()) return;

	//현재(07.12.17) Text크기는 미고려
	dXmax = dXmin = LeaderLineD.arPoint[0].x;
	dYmax = dYmin = LeaderLineD.arPoint[0].y;

	int i;
	//for(auto itr = LeaderLineD.arPoint.Data.begin() + 1; itr != LeaderLineD.arPoint.Data.end(); itr++) 
	for(i = 1 ; i < LeaderLineD.arPoint.GetSize(); i++)
	{
		T_POINT& PointRData = LeaderLineD.arPoint[i];

		dXmax = max(dXmax, PointRData.x);
		dXmin = min(dXmin, PointRData.x);
		dYmax = max(dYmax, PointRData.y);
		dYmin = min(dYmin, PointRData.y);
	}
}

void CDrawBase::GetPossessionRegionR(T_CHART_R& ChartD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(09.06.06) Text크기는 미고려	
	dXmax = max(ChartD.framePosi.P1.x, ChartD.framePosi.P2.x);
	dXmin = min(ChartD.framePosi.P1.x, ChartD.framePosi.P2.x);
	dYmax = max(ChartD.framePosi.P1.y, ChartD.framePosi.P2.y);
	dYmin = min(ChartD.framePosi.P1.y, ChartD.framePosi.P2.y);

	if(ChartD.Info.bTitle)
	{
		dXmax = max(dXmax, ChartD.titlePosi.x);
		dXmin = min(dXmin, ChartD.titlePosi.x);
		dYmax = max(dYmax, ChartD.titlePosi.y);
		dYmin = min(dYmin, ChartD.titlePosi.y);
	}

	if(ChartD.Info.bRemark)
	{
		dXmax = max(dXmax, max(ChartD.remarkPosi.P1.x, ChartD.remarkPosi.P2.x));
		dXmin = min(dXmin, min(ChartD.remarkPosi.P1.x, ChartD.remarkPosi.P2.x));
		dYmax = max(dYmax, max(ChartD.remarkPosi.P1.y, ChartD.remarkPosi.P2.y));
		dYmin = min(dYmin, min(ChartD.remarkPosi.P1.y, ChartD.remarkPosi.P2.y));
	}
}

//------------
void CDrawBase::GetPossessionRegion(POINT& PointD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	iXmax = iXmin = PointD.x;
	iYmax = iYmin = PointD.y;
}

void CDrawBase::GetPossessionRegion(T_LINE& LineD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	iXmax = max(LineD.P1.x, LineD.P2.x);
	iXmin = min(LineD.P1.x, LineD.P2.x);
	iYmax = max(LineD.P1.y, LineD.P2.y);
	iYmin = min(LineD.P1.y, LineD.P2.y);
}

void CDrawBase::GetPossessionRegion(T_CIRCLE& CircleD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	iXmax = CircleD.CenterP.x + CircleD.Radius;
	iXmin = CircleD.CenterP.x - CircleD.Radius;
	iYmax = CircleD.CenterP.y + CircleD.Radius;
	iYmin = CircleD.CenterP.y - CircleD.Radius;
}

void CDrawBase::GetPossessionRegion(T_DONUT& DonutD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	iXmax = DonutD.CenterP.x + DonutD.OutRadius;
	iXmin = DonutD.CenterP.x - DonutD.OutRadius;
	iYmax = DonutD.CenterP.y + DonutD.OutRadius;
	iYmin = DonutD.CenterP.y - DonutD.OutRadius;
}

void CDrawBase::GetPossessionRegion(T_ARC& ArcD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	T_POLYLINE PolylineD;
	int nDiv = max(2, (int)(fabs(ArcD.SweepAngle / 5.0)));
	GetArcPolyline(ArcD, nDiv, PolylineD.arPoint);
	GetPossessionRegion(PolylineD, iXmax, iXmin, iYmax, iYmin);
}

void CDrawBase::GetPossessionRegion(T_POLYLINE& PolylineD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	iXmax = iXmin = iYmax = iYmin = 0;
	if(! PolylineD.arPoint.GetSize()) return;

	iXmax = iXmin = PolylineD.arPoint[0].x;
	iYmax = iYmin = PolylineD.arPoint[0].y;

	int i;
	//for(auto itr = PolylineD.arPoint.Data.begin() + 1; itr != PolylineD.arPoint.Data.end(); itr++) 
	for(i = 1 ; i < PolylineD.arPoint.GetSize(); i++)
	{
		POINT& PointData = PolylineD.arPoint[i]; 
		
		iXmax = max(iXmax, PointData.x);
		iXmin = min(iXmin, PointData.x);
		iYmax = max(iYmax, PointData.y);
		iYmin = min(iYmin, PointData.y);
	}
}

void CDrawBase::GetPossessionRegion(T_TEXT& TextD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	//현재(07.12.17) Text크기는 미고려
	iXmax = iXmin = TextD.PPosition.x;
	iYmax = iYmin = TextD.PPosition.y;
}

void CDrawBase::GetPossessionRegion(T_DIMENSION& DimensionD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	//현재(07.12.17) Text 및 지시선 미고려
	iXmax = max(DimensionD.P1.x, DimensionD.P2.x);
	iXmin = min(DimensionD.P1.x, DimensionD.P2.x);
	iYmax = max(DimensionD.P1.y, DimensionD.P2.y);
	iYmin = min(DimensionD.P1.y, DimensionD.P2.y);
}

void CDrawBase::GetPossessionRegion(T_ANGLE& AngleD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	//현재(07.12.17) Text 및 지시선 미고려
	iXmax = iXmin = AngleD.CenterP.x;
	iYmax = iYmin = AngleD.CenterP.y;
}

void CDrawBase::GetPossessionRegion(T_LEADERLINE& LeaderLineD,int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	//현재(07.12.17) Text크기는 미고려
	iXmax = iXmin = iYmax = iYmin = 0;

	if(! LeaderLineD.arPoint.GetSize()) return;

	iXmax = iXmin = LeaderLineD.arPoint[0].x;
	iYmax = iYmin = LeaderLineD.arPoint[0].y;

	int i ;
	//for(auto itr = LeaderLineD.arPoint.Data.begin() + 1; itr != LeaderLineD.arPoint.Data.end(); itr++) 
	for(i = 1 ; i < LeaderLineD.arPoint.GetSize(); i++)
	{
		POINT& PointData = LeaderLineD.arPoint[i];
		
		iXmax = max(iXmax, PointData.x);
		iXmin = min(iXmin, PointData.x);
		iYmax = max(iYmax, PointData.y);
		iYmin = min(iYmin, PointData.y);
	}
}

void CDrawBase::GetPossessionRegion(T_CHART& ChartD, int& iXmax, int& iXmin, int& iYmax, int& iYmin)
{
	//현재(07.12.17) Text크기는 미고려
	iXmax = iXmin = iYmax = iYmin = 0;
	int iCurrXmax, iCurrXmin, iCurrYmax, iCurrYmin;
	
	if(ChartD.frame.arRect.GetSize())
	{
		GetPossessionRegion(ChartD.frame.arRect[0], iXmax, iXmin, iYmax, iYmin);
	}

	int no = ChartD.frame.arRect.GetSize();
	int i;
	//for(auto itr = ChartD.frame.arRect.Data.begin() + 1; itr !=ChartD.frame.arRect.Data.end(); itr++) 
	for(i = 1 ; i < ChartD.frame.arRect.GetSize(); i++)
	{
		GetPossessionRegion(ChartD.frame.arRect[i], iCurrXmax, iCurrXmin, iCurrYmax, iCurrYmin);

		iXmax = max(iXmax, iCurrXmax);
		iXmin = min(iXmin, iCurrXmin);
		iYmax = max(iYmax, iCurrYmax);
		iYmin = min(iYmin, iCurrYmin);
	}

	if(ChartD.Info.bTitle)
	{
		//for(auto itr = ChartD.frame.arText.Data.begin(); itr !=ChartD.frame.arText.Data.end(); itr++) 
		for(i = 0 ; i < ChartD.frame.arText.GetSize(); i++)
		{
			GetPossessionRegion(ChartD.frame.arText[i], iCurrXmax, iCurrXmin, iCurrYmax, iCurrYmin);

			iXmax = max(iXmax, iCurrXmax);
			iXmin = min(iXmin, iCurrXmin);
			iYmax = max(iYmax, iCurrYmax);
			iYmin = min(iYmin, iCurrYmin);
		}
	}

	if(ChartD.Info.bRemark)
	{
		//for(auto itr = ChartD.remark.arRect.Data.begin(); itr !=ChartD.remark.arRect.Data.end(); itr++) 
		for(i = 0 ; i < ChartD.remark.arRect.GetSize(); i++)
		{
			GetPossessionRegion(ChartD.remark.arRect[i], iCurrXmax, iCurrXmin, iCurrYmax, iCurrYmin);
			
			iXmax = max(iXmax, iCurrXmax);
			iXmin = min(iXmin, iCurrXmin);
			iYmax = max(iYmax, iCurrYmax);
			iYmin = min(iYmin, iCurrYmin);
		}
	}
}
///////////////////////////////////

void CDrawBase::MoveDrawUintR(double disX, double disY, T_POINT& PointD)
{
	PointD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_LINE_R& LineD)
{
	LineD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_CIRCLE_R& CircleD)
{
	CircleD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_DONUT_R& DonutD)
{
	DonutD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_ARC_R& ArcD)
{
	ArcD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_POLYLINE_R& PolylineD)
{
	PolylineD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_TEXT_R& TextD)
{
	TextD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_DIMENSION_R& DimensionD)
{
	DimensionD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_ANGLE_R& AngleD)
{
	AngleD.CenterP.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_LEADERLINE_R& LeaderLineD)
{
	LeaderLineD.Move(disX, disY);
}
void CDrawBase::MoveDrawUintR(double disX, double disY, T_CHART_R& ChartD)
{
	ChartD.Move(disX, disY);
}
//---------------
void CDrawBase::MoveDrawUint(int disX, int disY, POINT& PointD)
{
	PointD.x += disX;
	PointD.y += disY;
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_LINE& LineD)
{
	LineD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_CIRCLE& CircleD)
{
	CircleD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_DONUT& DonutD)
{
	DonutD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_ARC& ArcD)
{
	ArcD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_POLYLINE& PolylineD)
{
	PolylineD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_TEXT& TextD)
{
	TextD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_DIMENSION& DimensionD)
{
	DimensionD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_ANGLE& AngleD)
{
	AngleD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_LEADERLINE& LeaderLineD)
{
	LeaderLineD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_CHART& ChartD)
{
	ChartD.Move(disX, disY);
}
void CDrawBase::MoveDrawUint(int disX, int disY, T_HATCHING_D& HatchingD)
{
	HatchingD.Move(disX, disY);
}
//------------------------

BOOL CDrawBase::IsOverLapDrawUnit_Point(int nOverlapType, int iTol, POINT& DataA, POINT& DataB)
{
	if(nOverlapType == EN_OVERLAP_NONE) return FALSE;

	iTol = max(0, iTol - 1); // 기본두께는 배제함

	if(DataA.x == DataB.x && DataA.y == DataB.y) return TRUE; // 전체 일치

	double dLength = CMathFunc::mathLength(DataA.x, DataA.y, DataB.x, DataB.y);

	if(nOverlapType == EN_OVERLAP_OBJECT)
	{
		if(dLength <= iTol) return TRUE;
	}
	return FALSE;
}

BOOL CDrawBase::IsOverLapDrawUnit_Line(int nOverlapType, int iTol, T_LINE& DataA, T_LINE& DataB)
{	
	if(nOverlapType == EN_OVERLAP_NONE) return FALSE;

	iTol = max(0, iTol - 1); // 기본두께는 배제함

	if(DataA.P1.x == DataB.P1.x && DataA.P1.y == DataB.P1.y && DataA.P2.x == DataB.P2.x && DataA.P2.y == DataB.P2.y) return TRUE; // 전체 일치
	if(DataA.P1.x == DataB.P2.x && DataA.P1.y == DataB.P2.y && DataA.P2.x == DataB.P1.x && DataA.P2.y == DataB.P1.y) return TRUE; // 전체 일치

	if(nOverlapType == EN_OVERLAP_OBJECT)
	{		
		double dLengthA = CMathFunc::mathLength(DataA.P1.x, DataA.P1.y, DataA.P2.x, DataA.P2.y);
		double dLengthB = CMathFunc::mathLength(DataB.P1.x, DataB.P1.y, DataB.P2.x, DataB.P2.y);

		if(dLengthA >= dLengthB)
		{
			int    nState1, nState2;
			double line_i[2], line_j[2], point1[2], point2[2], dLength1, dLength2;

			line_i[0] = (double)(DataA.P1.x);  line_i[1] = (double)(DataA.P1.y);
			line_j[0] = (double)(DataA.P2.x);  line_j[1] = (double)(DataA.P2.y);
			point1[0] = (double)(DataB.P1.x);  point1[1] = (double)(DataB.P1.y);
			point2[0] = (double)(DataB.P2.x);  point2[1] = (double)(DataB.P2.y);

			dLength1 = CDrawFunc::mathDistanceFromIntersectPointToLine2D(line_i, line_j, point1, nState1);
			dLength2 = CDrawFunc::mathDistanceFromIntersectPointToLine2D(line_i, line_j, point2, nState2);

			if(max(fabs(dLength1), fabs(dLength2)) <= iTol)
			{
				if(nState1 == 1 || nState2 == 1) return TRUE;
			}
		}		
	}
	return FALSE;
}

BOOL CDrawBase::IsOverLapDrawUnit_Rect(int nOverlapType, int iTol, T_LINE& DataA, T_LINE& DataB)
{
	if(nOverlapType == EN_OVERLAP_NONE) return FALSE;

	iTol = max(0, iTol - 1); // 기본두께는 배제함

	T_LINE Data_A, Data_B;

	Data_A.P1.x = min(DataA.P1.x, DataA.P2.x);
	Data_A.P2.x = max(DataA.P1.x, DataA.P2.x);	
	Data_A.P1.y = min(DataA.P1.y, DataA.P2.y);
	Data_A.P2.y = max(DataA.P1.y, DataA.P2.y);

	Data_B.P1.x = min(DataB.P1.x, DataB.P2.x);
	Data_B.P2.x = max(DataB.P1.x, DataB.P2.x);	
	Data_B.P1.y = min(DataB.P1.y, DataB.P2.y);
	Data_B.P2.y = max(DataB.P1.y, DataB.P2.y);		

	if(Data_A.P1.x == Data_B.P1.x && Data_A.P1.y == Data_B.P1.y && Data_A.P2.x == Data_B.P2.x && Data_A.P2.y == Data_B.P2.y) return TRUE; // 전체 일치

	if(nOverlapType == EN_OVERLAP_OBJECT)
	{			
		double dLengthA = CMathFunc::mathLength(DataA.P1.x, DataA.P1.y, DataA.P2.x, DataA.P2.y);
		double dLengthB = CMathFunc::mathLength(DataB.P1.x, DataB.P1.y, DataB.P2.x, DataB.P2.y);

		if(dLengthA < dLengthB)
		{
			T_LINE Data_Temp;
			Data_Temp = Data_A;
			Data_A = Data_B;
			Data_B = Data_Temp;
		}
		Data_A.P1.x -= iTol;
		Data_A.P2.x += iTol;		
		Data_A.P1.y	-= iTol;
		Data_A.P2.y += iTol;

		if(Data_A.P1.x <= Data_B.P1.x && Data_A.P2.x >= Data_B.P1.x && Data_A.P1.y <= Data_B.P1.y && Data_A.P2.y >= Data_B.P1.y) return TRUE;
		if(Data_A.P1.x <= Data_B.P2.x && Data_A.P2.x >= Data_B.P2.x && Data_A.P1.y <= Data_B.P2.y && Data_A.P2.y >= Data_B.P2.y) return TRUE;
	}
	return FALSE;	
}

BOOL CDrawBase::IsOverLapDrawUnit_Ellipse(int nOverlapType, int iTol, T_LINE& DataA, T_LINE& DataB)
{
	if(nOverlapType == EN_OVERLAP_NONE) return FALSE;

	iTol = max(0, iTol - 1); // 기본두께는 배제함

	T_LINE Data_A, Data_B;

	Data_A.P1.x = min(DataA.P1.x, DataA.P2.x);
	Data_A.P2.x = max(DataA.P1.x, DataA.P2.x);	
	Data_A.P1.y = min(DataA.P1.y, DataA.P2.y);
	Data_A.P2.y = max(DataA.P1.y, DataA.P2.y);

	Data_B.P1.x = min(DataB.P1.x, DataB.P2.x);
	Data_B.P2.x = max(DataB.P1.x, DataB.P2.x);	
	Data_B.P1.y = min(DataB.P1.y, DataB.P2.y);
	Data_B.P2.y = max(DataB.P1.y, DataB.P2.y);		

	if(Data_A.P1.x == Data_B.P1.x && Data_A.P1.y == Data_B.P1.y && Data_A.P2.x == Data_B.P2.x && Data_A.P2.y == Data_B.P2.y) return TRUE; // 전체 일치

	if(nOverlapType == EN_OVERLAP_OBJECT)
	{			
		// 실제 떨어진 거리를 사용하지 않고 가상으로 반지름 평균값 사용. 실제 값을 사용시에 수정요망
		double dLength = CMathFunc::mathLength((DataA.P1.x + DataA.P2.x) / 2.0, (DataA.P1.y + DataA.P2.y) / 2.0, (DataB.P1.x + DataB.P2.x) / 2.0, (DataB.P1.y + DataB.P2.y) / 2.0);
		double dR_A = (abs(DataA.P1.x - DataA.P2.x) / 2.0 + abs(DataA.P1.y - DataA.P2.y) / 2.0) / 2.0;
		double dR_B = (abs(DataB.P1.x - DataB.P2.x) / 2.0 + abs(DataB.P1.y - DataB.P2.y) / 2.0) / 2.0;

		if(dLength - (dR_A + dR_B) <= iTol) return TRUE;
	}
	return FALSE;	
}

BOOL CDrawBase::IsOverLapDrawUnit_Circle(int nOverlapType, int iTol, T_CIRCLE& DataA, T_CIRCLE& DataB)
{
	if(nOverlapType == EN_OVERLAP_NONE) return FALSE;
	
	if(DataA.CenterP.x == DataB.CenterP.x && DataA.CenterP.y == DataB.CenterP.y && DataA.Radius == DataB.Radius) return TRUE; // 전체 일치

	iTol = max(0, iTol - 1); // 기본두께는 배제함

	if(nOverlapType == EN_OVERLAP_OBJECT)
	{
		double dLength = CMathFunc::mathLength(DataA.CenterP.x, DataA.CenterP.y, DataB.CenterP.x, DataB.CenterP.y);
		if(dLength - (DataA.Radius + DataB.Radius) <= iTol) return TRUE;		
	}
	return FALSE;		
}

BOOL CDrawBase::IsOverLapDrawUnit_Donut(int nOverlapType, int iTol, T_DONUT& DataA, T_DONUT& DataB)
{
	if(nOverlapType == EN_OVERLAP_NONE) return FALSE;

	if(DataA.CenterP.x == DataB.CenterP.x && DataA.CenterP.y == DataB.CenterP.y && DataA.OutRadius == DataB.OutRadius && DataA.InRadius == DataB.InRadius) return TRUE; // 전체 일치

	iTol = max(0, iTol - 1); // 기본두께는 배제함

	if(nOverlapType == EN_OVERLAP_OBJECT)
	{
		double dLength = CMathFunc::mathLength(DataA.CenterP.x, DataA.CenterP.y, DataB.CenterP.x, DataB.CenterP.y);
		if(dLength - (DataA.OutRadius + DataB.OutRadius) <= iTol) return TRUE;		
	}
	return FALSE;		
}

BOOL CDrawBase::IsOverLapDrawUnit_Polyline(int nOverlapType, int iTol, T_POLYLINE& DataA, T_POLYLINE& DataB, BOOL bClose, BOOL bFill)
{
	if(nOverlapType == EN_OVERLAP_NONE) return FALSE;

	iTol = max(0, iTol-1); // 기본두께는 배제함

	int nSizeA = DataA.arPoint.GetSize();
	int nSizeB = DataB.arPoint.GetSize();

	if(nSizeA == nSizeB)
	{
		BOOL bChk = TRUE;
		for(int i = 0; i < nSizeA; i++)
		{
			if(DataA.arPoint[i].x != DataB.arPoint[i].x || DataA.arPoint[i].y != DataB.arPoint[i].y) 
			{ bChk = FALSE;  break; }
		}
		if(bChk) return TRUE;
	}

	if(nOverlapType == EN_OVERLAP_OBJECT)
	{
		if(bFill)
		{
			int i;
			double (*coor2DA)[2];
			double (*coor2DB)[2];
			coor2DA = new double[nSizeA][2];  		
			coor2DB = new double[nSizeB][2];  	
			
			for(i = 0; i < nSizeA; i++)
			{
				coor2DA[i][0] = DataA.arPoint[i].x;
				coor2DA[i][1] = DataA.arPoint[i].y;
			}
			for(i = 0; i < nSizeB; i++)
			{
				coor2DB[i][0] = DataB.arPoint[i].x;
				coor2DB[i][1] = DataB.arPoint[i].y;
			}
			BOOL bChk = CDrawFunc::IsOverLap_Polyline(nSizeA, coor2DA, nSizeB, coor2DB);

			delete [] coor2DA;
			delete [] coor2DB;
			return bChk;			
		}
		else 
		{
			T_LINE LineDataA, LineDataB;
			for(int i = 0 ; i < nSizeA ; i++)
			{	
				LineDataA.P1 = DataA.arPoint[i];
				if(i == (nSizeA-1) && !bClose) break;

				if(i == (nSizeA-1)) 
				{
					LineDataA.P2 = DataA.arPoint[0];
				}
				else
				{
					LineDataA.P2 = DataA.arPoint[i+1];
				}

				for(int j = 0; j < nSizeB; j++)
				{
					LineDataB.P1 = DataB.arPoint[i];
					if(i == (nSizeB-1) && !bClose) break;

					if(i == (nSizeB-1))
					{
						LineDataB.P2 = DataB.arPoint[0];
					}
					else 
					{
						LineDataB.P2 = DataB.arPoint[i+1];
					}

					if(IsOverLapDrawUnit_Line(nOverlapType, iTol, LineDataA, LineDataB)) return TRUE;
				}
			}
		}
	}
	return FALSE;		
}