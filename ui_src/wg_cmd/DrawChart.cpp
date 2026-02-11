#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"
// #include "..\DgnBase\Profiler.h"

#include "DrawChart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawChart::CDrawChart()
{
	m_DataR.Init();
	m_Data.Init();
}

CDrawChart::~CDrawChart()
{
}

//-------------------------------------------
int CDrawChart::GetClassType()
{
	return EN_DRCLASS_TYPE_CHART;
}

int CDrawChart::GetFormatType()
{
	return EN_DRFORMAT_TYPE_CHART;
}

void CDrawChart::Draw(HDC DC, CDrawFunc* pDrawFunc, void* pDrawFormat)
{
	T_CHART_FORMAT DrawFormat = *((T_CHART_FORMAT*)pDrawFormat);
	pDrawFunc->DrawChart(DC, DrawFormat, m_Data.List); 
}

void CDrawChart::Convert(BOOL bCDC, CDrawFunc* pDrawFunc, T_PAPER_FORMAT* pPaperFormat, void* pDrawFormat)
{
	if(bCDC)
	{
		// 실제크기의 Data를 HDC용 Data로 변환
		T_CHART_FORMAT DrawFormat = *((T_CHART_FORMAT*)pDrawFormat);
		m_Data.Init();
		pDrawFunc->ConvertToArChart(*pPaperFormat, DrawFormat, m_DataR.List, m_Data.List);		
	}
	else
	{
		// HDC용 Data를 실제크기의 Data로 변환
		m_DataR.Init();
		pDrawFunc->ConvertToArChartR(*pPaperFormat, m_Data.List, m_DataR.List);
	}
}

void CDrawChart::GetCalcBoundaryR(double Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0.0;

	double dXmax, dXmin, dYmax, dYmin;
	double dXmaxU, dXminU, dYmaxU, dYminU;

	if(m_DataR.List.GetSize() <= 0) return;
	GetPossessionRegionR(m_DataR.List[0], dXmax, dXmin, dYmax, dYmin);

	auto no = m_DataR.List.GetSize();
	for(auto i = 1; i < no; i++) 
	{
		GetPossessionRegionR(m_DataR.List[i], dXmaxU, dXminU, dYmaxU, dYminU);

		dXmax = max(dXmax, dXmaxU);
		dXmin = min(dXmin, dXminU);
		dYmax = max(dYmax, dYmaxU);
		dYmin = min(dYmin, dYminU);		
	}	
	Boundary[0] = dXmin;
	Boundary[1] = dYmin;
	Boundary[2] = dXmax;
	Boundary[3] = dYmax;
}

void CDrawChart::GetCalcBoundary(int Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0;

	int iXmax, iXmin, iYmax, iYmin;
	int iXmaxU, iXminU, iYmaxU, iYminU;
	
	if(m_Data.List.GetSize() <= 0) return;

	GetPossessionRegion(m_Data.List[0], iXmax, iXmin, iYmax, iYmin);

	auto no = m_Data.List.GetSize();
	for(auto i = 1; i < no; i++) 
	{
		GetPossessionRegion(m_Data.List[i], iXmaxU, iXminU, iYmaxU, iYminU);

		iXmax = max(iXmax, iXmaxU);
		iXmin = min(iXmin, iXminU);
		iYmax = max(iYmax, iYmaxU);
		iYmin = min(iYmin, iYminU);		
	}	
	Boundary[0] = iXmin;	Boundary[1] = iYmin;
	Boundary[2] = iXmax;	Boundary[3] = iYmax;
}

void CDrawChart::Move(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat)
{
	double disVectorR[2];
	int    disVector[2];

	GetMoveVector(bCDC, Vector, pPaperFormat, disVectorR, disVector);

	if(disVectorR[0] != 0.0 || disVectorR[1] != 0.0)
	{
		auto no = m_DataR.List.GetSize();
		for(auto i = 0; i < no; i++) 
		{
			MoveDrawUintR(disVectorR[0], disVectorR[1], m_DataR.List[i]);
		}
	}

	if(disVector[0] != 0 || disVector[1] != 0)
	{
		auto no = m_Data.List.GetSize();
		for(auto i = 0; i < no; i++) 
		{
			MoveDrawUint(disVector[0],  disVector[1], m_Data.List[i]);
		}
	}
	m_Boundary[0] += disVector[0];
	m_Boundary[1] += disVector[1];
	m_Boundary[2] += disVector[0];
	m_Boundary[3] += disVector[1];	
}

BOOL CDrawChart::IsSelectCheck(POINT sPoint, BOOL bFill, int iTol)
{
	if(!m_bIsSelect) return FALSE;

	double dTol = (double)(iTol)+m_dZero;
	
	nrPOINT arPoint;
	arPoint.SetSize(4);

	auto no = m_Data.List.GetSize();
	int i, j;
	for(i = 0; i < no; i++) 
	{
		T_CHART& chartD = m_Data.List[i];

		for(j = 0; j < chartD.frame.arRect.GetSize(); j++) 
		{
			arPoint[0].x = chartD.frame.arRect[j].P1.x;   arPoint[0].y = chartD.frame.arRect[j].P1.y;
			arPoint[1].x = chartD.frame.arRect[j].P2.x;   arPoint[1].y = chartD.frame.arRect[j].P1.y;
			arPoint[2].x = chartD.frame.arRect[j].P2.x;   arPoint[2].y = chartD.frame.arRect[j].P2.y;
			arPoint[3].x = chartD.frame.arRect[j].P1.x;   arPoint[3].y = chartD.frame.arRect[j].P2.y;	

			if(IsPointOfPolyLine(arPoint, sPoint, TRUE, bFill, iTol)) return TRUE;
		}

		if(chartD.Info.bTitle)
		{
			for(j = 0; j < chartD.frame.arText.GetSize(); j++) 
			{	
				if(abs(chartD.frame.arText[j].PPosition.x - sPoint.x) <= dTol && abs(chartD.frame.arText[j].PPosition.y - sPoint.y) <= dTol)
				{
					return TRUE;				
				}
			}		
		}

		if(chartD.Info.bRemark)
		{			
			for(auto j = 0; j < chartD.remark.arRect.GetSize(); j++) 
			{
				arPoint[0].x = chartD.remark.arRect[j].P1.x;   arPoint[0].y = chartD.remark.arRect[j].P1.y;
				arPoint[1].x = chartD.remark.arRect[j].P2.x;   arPoint[1].y = chartD.remark.arRect[j].P1.y;
				arPoint[2].x = chartD.remark.arRect[j].P2.x;   arPoint[2].y = chartD.remark.arRect[j].P2.y;
				arPoint[3].x = chartD.remark.arRect[j].P1.x;   arPoint[3].y = chartD.remark.arRect[j].P2.y;	

				if(IsPointOfPolyLine(arPoint, sPoint, TRUE, bFill, iTol)) return TRUE;
			}
		}

		int j;
		if(chartD.Info.xAxis.bAxis)
		{		
			for(j = 0; j < chartD.xAxis.arLine.GetSize(); j++) 
			{
				if(IsPointOfLine(chartD.xAxis.arLine[j].P1, chartD.xAxis.arLine[j].P2, sPoint, iTol, TRUE)) return TRUE;
			}

			for(j = 0; j < chartD.xAxis.arText.GetSize(); j++) 
			{
				if(abs(chartD.xAxis.arText[j].PPosition.x-sPoint.x) <= dTol && abs(chartD.xAxis.arText[j].PPosition.y-sPoint.y) <= dTol) return TRUE;				
			}
		}

		if(chartD.Info.yAxis.bAxis)
		{		
			for(j = 0; j < chartD.yAxis.arLine.GetSize(); j++) 
			{
				if(IsPointOfLine(chartD.yAxis.arLine[j].P1, chartD.yAxis.arLine[j].P2, sPoint, iTol, TRUE)) 	return TRUE;
			}

			for(j = 0; j < chartD.yAxis.arText.GetSize(); j++) 
			{	
				if(abs(chartD.yAxis.arText[j].PPosition.x - sPoint.x) <= dTol && abs(chartD.yAxis.arText[j].PPosition.y - sPoint.y) <= dTol) return TRUE;				
			}		
		}
	}
	return FALSE;
}

BOOL CDrawChart::IsSnapCheck(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR)
{
	snapPoint.x = snapPoint.y = 0;
	snapPointR.Init();

	int  nListSize = m_Data.List.GetSize();
	BOOL bIsRData = (nListSize == m_DataR.List.GetSize());
	
	if(m_SnapType == 1)
	{	
		T_LINE_R tempRectR;  
		int nSize, nCurrSize, i, j;

		nSize = m_Data.List.GetSize();
		for(i = 0; i < nSize ; i++)
		{
			T_CHART& chartD = m_Data.List[i];

			nCurrSize = chartD.frame.arRect.GetSize();
			for(j = 0; j < nCurrSize; j++)
			{
				if(j == 0) 
				{ 
					if(IsSnapCheck_Rect(chartD.frame.arRect[j], m_DataR.List[i].framePosi, TRUE, currPoint, snapPoint, snapPointR)) return TRUE; 
				}
				else
				{ 
					if(IsSnapCheck_Rect(chartD.frame.arRect[j], tempRectR, FALSE, currPoint, snapPoint, snapPointR)) return TRUE;
				}
			}

			if(chartD.Info.bTitle)
			{			
				nCurrSize = chartD.frame.arText.GetSize();
				
				for(j = 0; j < nCurrSize; j++)
				{
					if(IsInSnapArea(chartD.frame.arText[j].PPosition, currPoint))
					{
						snapPoint = chartD.frame.arText[j].PPosition;
						
						if(j == 0) snapPointR = m_DataR.List[i].titlePosi;
						
						return TRUE;
					}
				}
			}

			if(chartD.Info.bRemark)
			{
				nCurrSize = chartD.remark.arRect.GetSize();
				for(j = 0; j < nCurrSize ; j++)
				{
					if(j == 0) 
					{
						if(IsSnapCheck_Rect(chartD.remark.arRect[j], m_DataR.List[i].remarkPosi, TRUE, currPoint, snapPoint, snapPointR)) return TRUE; 
					}
					else
					{
						if(IsSnapCheck_Rect(chartD.remark.arRect[j], tempRectR, FALSE, currPoint, snapPoint, snapPointR)) return TRUE; 
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL CDrawChart::GetCenterPoint(int nID, POINT& CenterP)
{
	if(nID >= 0 && nID < m_Data.List.GetSize())
	{
		int iXmax, iXmin, iYmax, iYmin;
		GetPossessionRegion(m_Data.List[nID], iXmax, iXmin, iYmax, iYmin);
		CenterP.x = (iXmax + iXmin) / 2;
		CenterP.y = (iYmax + iYmin) / 2;
		return TRUE;
	}
	CenterP.x = CenterP.y = 0;
	return FALSE;
}

