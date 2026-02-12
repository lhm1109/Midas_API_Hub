#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"

#include "DrawData.h"
#include "DrawFunc.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDrawFunc

CDrawFunc::CDrawFunc()
{
	ResetVariables();  
}

CDrawFunc::~CDrawFunc()
{

}

void CDrawFunc::ResetVariables()
{
	m_bEMF = FALSE;
	m_nMapMode = MM_HIMETRIC;
	m_Zero = 10e-10;
	m_nFontAddSize_RZero = 0;
	m_bRectTrPolyDraw = FALSE;
}

//////////////////////////////////////////////////////////////////////////

void CDrawFunc::GetPossessionRegion(T_LINE_R& LineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = max(LineD.P1.x, LineD.P2.x);
	dXmin = min(LineD.P1.x, LineD.P2.x);
	dYmax = max(LineD.P1.y, LineD.P2.y);
	dYmax = min(LineD.P1.y, LineD.P2.y);
}

void CDrawFunc::GetPossessionRegion(T_CIRCLE_R& CircleD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = CircleD.CenterP.x + CircleD.Radius;
	dXmin = CircleD.CenterP.x - CircleD.Radius;
	dYmax = CircleD.CenterP.y + CircleD.Radius;
	dYmax = CircleD.CenterP.y - CircleD.Radius;
}

void CDrawFunc::GetPossessionRegion(T_DONUT_R& DonutD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	dXmax = DonutD.CenterP.x + DonutD.OutRadius;
	dXmin = DonutD.CenterP.x - DonutD.OutRadius;
	dYmax = DonutD.CenterP.y + DonutD.OutRadius;
	dYmax = DonutD.CenterP.y - DonutD.OutRadius;
}

void CDrawFunc::GetPossessionRegion(T_ARC_R& ArcD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	T_POLYLINE_R PolylineD;
	GetArcPolylineR(ArcD, 18, PolylineD);

	GetPossessionRegion(PolylineD, dXmax, dXmin, dYmax, dYmin);
}

void CDrawFunc::GetPossessionRegion(T_POLYLINE_R& PolylineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	if(! PolylineD.arPoint.GetSize())
	{
		dXmax = dXmin = dYmax = dYmin = 0.0;
		return;
	}

	dXmax = dXmin = PolylineD.arPoint[0].x;
	dYmax = dYmin = PolylineD.arPoint[0].y;

	int i;

	//for(auto itr = PolylineD.arPoint.Data.begin() + 1; itr != PolylineD.arPoint.Data.end(); itr++) 
	for( i = 1; i < PolylineD.arPoint.GetSize() ;i++)
	{
		T_POINT& PointRData = PolylineD.arPoint[i]; 

		dXmax = max(dXmax, PointRData.x);
		dXmin = min(dXmin, PointRData.x);
		dYmax = max(dYmax, PointRData.y);
		dYmax = min(dYmax, PointRData.y);
	}
}

void CDrawFunc::GetPossessionRegion(T_TEXT_R& TextD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(07.12.17) Text크기는 미고려
	dXmax = dXmin = TextD.PPosition.x;
	dYmax = dYmin = TextD.PPosition.y;
}

void CDrawFunc::GetPossessionRegion(T_DIMENSION_R& DimensionD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(07.12.17) Text 및 지시선 미고려
	dXmax = max(DimensionD.P1.x, DimensionD.P2.x);
	dXmin = min(DimensionD.P1.x, DimensionD.P2.x);
	dYmax = max(DimensionD.P1.y, DimensionD.P2.y);
	dYmax = min(DimensionD.P1.y, DimensionD.P2.y);
}

void CDrawFunc::GetPossessionRegion(T_ANGLE_R& AngleD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(07.12.17) Text 및 지시선 미고려
	dXmax = dXmin = AngleD.CenterP.x;
	dYmax = dYmin = AngleD.CenterP.y;
}

void CDrawFunc::GetPossessionRegion(T_LEADERLINE_R& LeaderLineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	//현재(07.12.17) Text크기는 미고려
	if(! LeaderLineD.arPoint.GetSize())
	{
		dXmax = dXmin = dYmax = dYmin = 0.0;
		return;
	}

	dXmax = dXmin = LeaderLineD.arPoint[0].x;
	dYmax = dYmin = LeaderLineD.arPoint[0].y;

	int i;
	//for(auto itr = LeaderLineD.arPoint.Data.begin() + 1; itr != LeaderLineD.arPoint.Data.end(); itr++) 
	for( i = 1 ; i < LeaderLineD.arPoint.GetSize() ;i++)
	{
		T_POINT& PointRData = LeaderLineD.arPoint[i];

		dXmax = max(dXmax, PointRData.x);
		dXmin = min(dXmin, PointRData.x);
		dYmax = max(dYmax, PointRData.y);
		dYmax = min(dYmax, PointRData.y);
	}
}

double CDrawFunc::FindAutoSpace(double dBaseSpace, int& nType)
{
	nType = 0;
	dBaseSpace = fabs(dBaseSpace);
	double dSpace = 1.0;
	if(dBaseSpace == 0.0) return dSpace;

	std::wstring strSpace;
	TCHAR szTemp[64];
	_stprintf(szTemp, _T("%g"), dBaseSpace);
	strSpace = std::wstring(szTemp);

	std::string::size_type nFindID = strSpace.find('E');

	if(nFindID == std::wstring::npos)
	{
		nFindID = strSpace.find('e');
	}
		
	if(nFindID >= 0)
	{
		std::wstring temp = _T("");
		for(int i = int(strSpace.length() - nFindID - 1); i < lstrlen(strSpace.c_str()); i++)
		{
			temp += strSpace[i];
		}
		
		dSpace = pow(10.0, _ttoi(temp.c_str()));
	}
	else 
	{
		int i, nSize;
		
		nFindID = strSpace.find('.');
		
		if(nFindID == std::string::npos)
		{
			dSpace = pow(10.0, double(strSpace.length() - 1));
		}
		else           
		{
			if(dBaseSpace >= 1.0)
			{
				dSpace = pow(10.0, double(nFindID - 1));
			}
			else 
			{
				nSize = (int)strSpace.length();
				for(i = (int)nFindID + 1; i < nSize; i++)
				{
					if(strSpace[i] == '0') continue;
					dSpace = pow(10.0, -1.0 * (i - nFindID));
					break;
				}			
			}
		}
	}

	if(dBaseSpace < 1.5 * dSpace)       { nType = 0;  return dSpace; }     // 1단위
	else if(dBaseSpace < 2.25 * dSpace) { nType = 1;  return 2.0 * dSpace; } // 2단위
	else if(dBaseSpace < 3.75 * dSpace) { nType = 2;  return 2.5 * dSpace; } // 2.5단위
	else if(dBaseSpace < 7.5 * dSpace)  { nType = 3;  return 5.0 * dSpace; } // 2.5단위

	nType = 1;  
	return 10.0 * dSpace;// 1단위
}

void CDrawFunc::Cutting_OutRect(T_LINE Rect, int nPointSize, nrPOINT& arPoint_In, nrT_LINE& arLine_In, nrPOINT& arPoint_Out, nrT_LINE& arLine_Out)
{
	arPoint_Out.RemoveAll();
	arLine_Out.RemoveAll();
	CArray<int, int> arInCheck;
	int nXmax = max(Rect.P1.x, Rect.P2.x);
	int nXmin = min(Rect.P1.x, Rect.P2.x);
	int nYmax = max(Rect.P1.y, Rect.P2.y);
	int nYmin = min(Rect.P1.y, Rect.P2.y);

	int i, nSize, nCount;
	// Point
	nSize = arPoint_In.GetSize();
	arInCheck.SetSize(nSize);
	nCount = 0;
	for(i = 0; i < nSize ; i++)
	{
		if(arPoint_In[i].x > nXmax || arPoint_In[i].x < nXmin || arPoint_In[i].y > nYmax || arPoint_In[i].y < nYmin)	
		{
			arInCheck[i] = 0; 
		}
		else 
		{ 
			arInCheck[i] = 1;
			nCount++; 
		}
	}

	arPoint_Out.SetSize(nCount);
	nCount = 0;
	for(i = 0; i < nSize; i++)
	{
		if(arInCheck[i] == 1)
		{
			arPoint_Out[nCount] = arPoint_In[i];
			nCount++;
		}
	}
	// Line
	nSize = arLine_In.GetSize();
	arInCheck.SetSize(arLine_In.GetSize());
	nCount = 0;
	BOOL bInP1, bInP2;
	double pCurrLine[2][2];
	double pBound1[2], pBound2[2], pCurr1[2], pCurr2[2], pCross[2];
	for(i = 0; i < nSize ; i++)
	{
		bInP1 = !(arLine_In[i].P1.x > nXmax || arLine_In[i].P1.x < nXmin || arLine_In[i].P1.y > nYmax || arLine_In[i].P1.y < nYmin);
		bInP2 = !(arLine_In[i].P2.x > nXmax || arLine_In[i].P2.x < nXmin || arLine_In[i].P2.y > nYmax || arLine_In[i].P2.y < nYmin);
		
		if(bInP1 && bInP2) 
		{
			arInCheck[i] = 1; 
			nCount++; 
			continue;
		}
		else if(bInP1 || bInP2)
		{ 
			arInCheck[i] = 2; 
			nCount++;  
			continue; 
		}
		else 
		{ 
			pCurr1[0]  = arLine_In[i].P1.x;  
			pCurr1[1]  = arLine_In[i].P1.y; 
			pCurr2[0]  = arLine_In[i].P2.x; 
			pCurr2[1]  = arLine_In[i].P2.y;
			pBound1[0] = nXmin;             
			pBound1[1] = nYmin;             
			pBound2[0] = nXmax;         
			pBound2[1] = nYmin;
			
			if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, pCurr1, pCurr2) >=0 ) 
			{ 
				arInCheck[i] = 2;
				nCount++; 
				continue;
			}
			pBound1[1] = nYmax;         
			pBound2[1] = nYmax;
			if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, pCurr1, pCurr2) >=0 )
			{ 
				arInCheck[i] = 2; 
				nCount++; 
				continue;
			}
			pBound1[0] = nXmin;         
			pBound1[1] = nYmin;    
			pBound2[0] = nXmin;         
			pBound2[1] = nYmax;
			if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, pCurr1, pCurr2) >=0 )
			{ 
				arInCheck[i] = 2;  
				nCount++; 
				continue; 
			}
			pBound1[0] = nXmax;     
			pBound2[0] = nXmax;     
			if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, pCurr1, pCurr2) >=0 )
			{ 
				arInCheck[i] = 2; 
				nCount++;
				continue;
			}					
		}
		arInCheck[i] = 0;
	}

	int nOffset = max(2, min(nPointSize, (int)(min(nXmax - nXmin, nYmax - nYmin) / 20)));
	nXmax += nOffset;  nXmin -= nOffset;  nYmax += nOffset;  nYmin -= nOffset;
	arLine_Out.SetSize(nCount);
	nCount = 0;
	for(i = 0; i < nSize ; i++)
	{
		if(arInCheck[i] == 1)
		{
			arLine_Out[nCount] = arLine_In[i]; 
			nCount++; 
		}
		else if(arInCheck[i] == 2)
		{
			bInP1 = !(arLine_In[i].P1.x > nXmax || arLine_In[i].P1.x < nXmin || arLine_In[i].P1.y > nYmax || arLine_In[i].P1.y < nYmin);
			bInP2 = !(arLine_In[i].P2.x > nXmax || arLine_In[i].P2.x < nXmin || arLine_In[i].P2.y > nYmax || arLine_In[i].P2.y < nYmin);

			if(bInP1) arLine_Out[nCount].P1 = arLine_In[i].P1;
			if(bInP2) arLine_Out[nCount].P2 = arLine_In[i].P2;
			
			if(!bInP1 || !bInP2)
			{				
				pCurr1[0]  = arLine_In[i].P1.x;
				pCurr1[1]  = arLine_In[i].P1.y; 
				pCurr2[0]  = arLine_In[i].P2.x; 
				pCurr2[1]  = arLine_In[i].P2.y;
				pCurrLine[0][0] = arLine_In[i].P1.x; 
				pCurrLine[0][1] = arLine_In[i].P1.y; 
				pCurrLine[1][0] = arLine_In[i].P2.x; 
				pCurrLine[1][1] = arLine_In[i].P2.y;
				
				for(int j = 0; j < 4; j++)
				{				
					if     (j == 0) { pBound1[0] = nXmin;  pBound1[1] = nYmin;  pBound2[0] = nXmax;  pBound2[1] = nYmin; }
					else if(j == 1) { pBound1[0] = nXmin;  pBound1[1] = nYmax;  pBound2[0] = nXmax;  pBound2[1] = nYmax; }
					else if(j == 2) { pBound1[0] = nXmin;  pBound1[1] = nYmin;  pBound2[0] = nXmin;  pBound2[1] = nYmax; }
					else if(j == 3) { pBound1[0] = nXmax;  pBound1[1] = nYmin;  pBound2[0] = nXmax;  pBound2[1] = nYmax; }
					
					if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, pCurr1, pCurr2) >=0 )
					{					
						if(CMathFunc::mathLineSegCross2D(pCurrLine, pBound1, pBound2, pCross) == 1)
						{ 
							if(!bInP1) 
							{
								arLine_Out[nCount].P1.x = (int)(pCross[0]);  
								arLine_Out[nCount].P1.y = (int)(pCross[1]); 
								bInP1 = TRUE;
							}
							if(!bInP2) 
							{
								arLine_Out[nCount].P2.x = (int)(pCross[0]); 
								arLine_Out[nCount].P2.y = (int)(pCross[1]); 
								bInP2 = TRUE;
							}
							if(bInP1 && bInP2) continue;
						}
					}
				}

				if(!bInP1 || !bInP2) 
				{
					ASSERT(0); // 실패하였음
					if(!bInP1) arLine_Out[nCount].P1 = arLine_Out[nCount].P2;
					if(!bInP2) arLine_Out[nCount].P2 = arLine_Out[nCount].P1;
				}
				nCount++;
			}
			else 
			{
				nCount++;
			}
		}
	}	
}


double CDrawFunc::mathDistanceFromIntersectPointToLine2D(double line_i[2], double line_j[2], double point[2], int& nState)
{	
	double NormalZero = 1E-10;
	double a, b, l, m, x, y;
	double dx, dy, px, py;
	
	x=point[0], y=point[1];
	a=line_i[0], b=line_i[1];
	dx=line_j[0]-a, dy=line_j[1]-b;
	
	double dLength = CMathFunc::mathLength(dx, dy);
	CMathFunc::mathNormalize(dx, dy, l, m);
	double t;
	if (fabs(l) < NormalZero && fabs(m) < NormalZero) t = 0.0;
	else t=((x-a)*l+(y-b)*m)/(l*l+m*m);
	
	if(t < 0.)           { nState = 0; }
	else if(t > dLength) { nState = 2; }  
	else                 { nState = 1; }  
	
	px=a+t*l;
	py=b+t*m;
	
	return CMathFunc::mathLength(point[0]-px, point[1]-py);
}

BOOL CDrawFunc::IsOverLap_Polyline(const int nData1, double polyLine1[][2], const int nData2, double polyLine2[][2])
{
	if(nData1==0 || nData2==0) return FALSE;
	
	int i, j;
	// 교차 검토
	double p1Org[2], p2Org[2], p3Org[2], p4Org[2];
	for(i = 0; i < nData1 ; i++)
	{
		p1Org[0] = polyLine1[i][0];
		p1Org[1] = polyLine1[i][1];
		p2Org[0] = polyLine1[(i==nData1-1 ? 0 : i)][0];
		p2Org[1] = polyLine1[(i==nData1-1 ? 0 : i)][1];
		
		for(j = 0; j < nData2; j++)
		{
			p3Org[0] = polyLine2[j][0];
			p3Org[1] = polyLine2[j][1];
			p4Org[0] = polyLine2[(j == nData2 - 1 ? 0 : j)][0];
			p4Org[1] = polyLine2[(j == nData2 - 1 ? 0 : j)][1];
			
			if(CMathFunc::mathIntersect_ccw2D(p1Org, p2Org, p3Org, p4Org) >= 0) return TRUE; //교차하거나 포함되는 경우
		}
	}
	
	// 내부점 검토
	double dxmax1, dxmin1, dymax1, dymin1;
	double dxmax2, dxmin2, dymax2, dymin2;
	
	for(i = 0; i < nData1 ; i++)
	{
		if(i == 0) 
		{
			dxmax1 = dxmin1 = polyLine1[i][0]; 
			dymax1 = dymin1 = polyLine1[i][1];
		}
		else 
		{
			dxmax1 = max(dxmax1, polyLine1[i][0]);
			dxmin1 = min(dxmin1, polyLine1[i][0]);
			dymax1 = max(dymax1, polyLine1[i][1]);
			dymin1 = min(dymin1, polyLine1[i][1]);
		}
	}
	for(i = 0; i < nData2 ; i++)
	{
		if(i == 0) 
		{ 
			dxmax2 = dxmin2 = polyLine2[i][0]; 
			dymax2 = dymin2 = polyLine2[i][1]; 
		}
		else 
		{
			dxmax2 = max(dxmax2, polyLine2[i][0]);
			dxmin2 = min(dxmin2, polyLine2[i][0]);
			dymax2 = max(dymax2, polyLine2[i][1]);
			dymin2 = min(dymin2, polyLine2[i][1]);
		}
	}
	
	double p1[2];
	if(max(dxmax1-dxmin1, dymax1-dymin1) > max(dxmax2-dxmin2, dymax2-dymin2) || (nData1>1 && nData2==1))
	{
		for(i = 0; i < nData2 ; i++)
		{
			p1[0] = polyLine2[i][0]; 
			p1[1] = polyLine2[i][1];
			if(CMathFunc::mathIsInsidePoint2D(p1, nData1, polyLine1, TRUE)) return TRUE;
		}
	}
	else if(nData2 > 1)
	{
		for(i = 0; i < nData1 ; i++)
		{
			p1[0] = polyLine1[i][0];
			p1[1] = polyLine1[i][1];
			if(CMathFunc::mathIsInsidePoint2D(p1, nData2, polyLine2, TRUE)) return TRUE;
		}
	}
	return FALSE;
}

int CDrawFunc::mathArcForm3Point(double p1[2], double p2[2], double p3[2], double CenterP[2], double& dRadius, double& dStartAngle, double& dSweepAngle, double& dBulge)
{
	dBulge = 0.0;
	int nChk = CMathFunc::mathArcForm3Point(p1, p2, p3, CenterP, dRadius, dStartAngle, dSweepAngle);
	dBulge = dRadius * (1.0 - cos(fabs(dSweepAngle)*CMathFunc::m_trrad));
	if(dSweepAngle < 0.0) dBulge *= (-1.0);
	return nChk;
}