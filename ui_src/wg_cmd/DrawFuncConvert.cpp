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

// 변환 ----------------------------------------------------------------------------------------------
void CDrawFunc::ConvertPaperFormatMirror(T_PAPER_FORMAT& paperFormat, RECT Canvas, int nMirror_Org, int nMirror_Change)
{
	BOOL bChangeH, bChangeV;
	bChangeH = ((nMirror_Org == EN_MIRRORVIEW_TYPE_HORIZONTAL) != (nMirror_Change == EN_MIRRORVIEW_TYPE_HORIZONTAL));
	bChangeV = ((nMirror_Org == EN_MIRRORVIEW_TYPE_VERTICAL  ) != (nMirror_Change == EN_MIRRORVIEW_TYPE_VERTICAL  ));

	if(bChangeH)
	{
		paperFormat.ScaleX *= -1.0;
		paperFormat.OrgPoint.x = (long)(Canvas.left + Canvas.right - paperFormat.OrgPoint.x);
	}
	if(bChangeV)
	{
		paperFormat.ScaleY *= -1.0;
		paperFormat.OrgPoint.x = (long)(Canvas.top + Canvas.bottom - paperFormat.OrgPoint.y);
	}
}

void CDrawFunc::ConvertToPOINT(T_PAPER_FORMAT paperFormat, T_POINT point_In, POINT& point_Out)
{
	point_Out.x = paperFormat.OrgPoint.x + math_lib::round_off(point_In.x / paperFormat.ScaleX);  
	point_Out.y = paperFormat.OrgPoint.y - math_lib::round_off(point_In.y / paperFormat.ScaleY);
	if(m_bEMF) point_Out.y *= (-1);
}

void CDrawFunc::ConvertToTPoint(T_PAPER_FORMAT paperFormat, POINT point_In, T_POINT& point_Out)
{
	point_Out.x = (double)(point_In.x - paperFormat.OrgPoint.x) * paperFormat.ScaleX;
	point_Out.y = (-1.0)*(double)(point_In.y - paperFormat.OrgPoint.y) * paperFormat.ScaleY;

	if(m_bEMF) point_Out.y *= (-1.0);
}

void CDrawFunc::ConvertToLine(T_PAPER_FORMAT paperFormat, T_LINE_R line_In, T_LINE& line_Out)
{
	ConvertToPOINT(paperFormat, line_In.P1, line_Out.P1);
	ConvertToPOINT(paperFormat, line_In.P2, line_Out.P2);

	if(line_In.P1.x != line_In.P2.x || line_In.P1.y != line_In.P2.y)
	{	
		double dTol = fabs(fabs(line_In.P1.x-line_In.P2.x) - fabs(line_In.P1.y-line_In.P2.y)) / max(fabs(line_In.P1.x-line_In.P2.x), fabs(line_In.P1.y-line_In.P2.y));

		if(dTol < m_Zero)
		{
			if(abs(line_Out.P1.x-line_Out.P2.x) != abs(line_Out.P1.y-line_Out.P2.y))
			{
				line_Out.P2.y = (long)(line_Out.P1.y + abs(line_Out.P1.x-line_Out.P2.x)*((line_Out.P2.y-line_Out.P1.y < 0) ? -1 : 1));
			}
		}
	}
}

void CDrawFunc::ConvertToLineR(T_PAPER_FORMAT paperFormat, T_LINE line_In, T_LINE_R& line_Out)
{
	ConvertToTPoint(paperFormat, line_In.P1, line_Out.P1);
	ConvertToTPoint(paperFormat, line_In.P2, line_Out.P2);    
}

void CDrawFunc::ConvertToCircle(T_PAPER_FORMAT paperFormat, T_CIRCLE_R circle_In, T_CIRCLE& circle_Out)
{
	ConvertToPOINT(paperFormat, circle_In.CenterP, circle_Out.CenterP);
	circle_Out.Radius = math_lib::round_off(circle_In.Radius / ((fabs(paperFormat.ScaleX) + fabs(paperFormat.ScaleY)) / 2.0));
}

void CDrawFunc::ConvertToCircleR(T_PAPER_FORMAT paperFormat, T_CIRCLE circle_In, T_CIRCLE_R& circle_Out)
{
	ConvertToTPoint(paperFormat, circle_In.CenterP, circle_Out.CenterP);
	circle_Out.Radius = (double)(circle_In.Radius) * ((fabs(paperFormat.ScaleX) + fabs(paperFormat.ScaleY)) / 2.0);
}

void CDrawFunc::ConvertToDonut(T_PAPER_FORMAT paperFormat, T_DONUT_R donut_In, T_DONUT& donut_Out)
{
	ConvertToPOINT(paperFormat, donut_In.CenterP, donut_Out.CenterP);
	donut_Out.OutRadius = math_lib::round_off(donut_In.OutRadius / ((paperFormat.ScaleX + paperFormat.ScaleY) / 2.0));
	donut_Out.InRadius  = math_lib::round_off(donut_In.InRadius  / ((paperFormat.ScaleX + paperFormat.ScaleY) / 2.0));
}

void CDrawFunc::ConvertToDonutR(T_PAPER_FORMAT paperFormat, T_DONUT donut_In, T_DONUT_R& donut_Out)
{
	ConvertToTPoint(paperFormat, donut_In.CenterP, donut_Out.CenterP);
	donut_Out.OutRadius = (double)(donut_In.OutRadius) * ((paperFormat.ScaleX + paperFormat.ScaleY) / 2.0);
	donut_Out.InRadius  = (double)(donut_In.InRadius ) * ((paperFormat.ScaleX + paperFormat.ScaleY) / 2.0);
}																									    

void CDrawFunc::ConvertToArc(T_PAPER_FORMAT paperFormat, T_ARC_R arc_In, T_ARC& arc_Out)
{
	ConvertToPOINT(paperFormat, arc_In.CenterP, arc_Out.CenterP);
	arc_Out.Radius = math_lib::round_off(arc_In.Radius / ((paperFormat.ScaleX+paperFormat.ScaleY) / 2.0));
	arc_Out.StartAngle = (float)arc_In.StartAngle;
	arc_Out.SweepAngle = (float)arc_In.SweepAngle;	
}

void CDrawFunc::ConvertToArcR(T_PAPER_FORMAT paperFormat, T_ARC arc_In, T_ARC_R& arc_Out)
{
	ConvertToTPoint(paperFormat, arc_In.CenterP, arc_Out.CenterP);
	arc_Out.Radius = (double)(arc_In.Radius) * ((paperFormat.ScaleX + paperFormat.ScaleY) / 2.0);
	arc_Out.StartAngle = (double)arc_In.StartAngle;
	arc_Out.SweepAngle = (double)arc_In.SweepAngle;
}

void CDrawFunc::ConvertToPolyline(T_PAPER_FORMAT paperFormat, T_POLYLINE_R  polyline_In, T_POLYLINE& polyline_Out)
{
	T_POLYLINE_R pathline;
	ConvertToPathline(polyline_In, pathline, FALSE);

	int nCount = pathline.arPoint.GetSize();
	polyline_Out.arPoint.SetSize(nCount);

	for(int i = 0 ; i < nCount ; ++i)
	{
		ConvertToPOINT(paperFormat, pathline.arPoint[i], polyline_Out.arPoint[i]);
	}
}

void CDrawFunc::ConvertToPolylineR(T_PAPER_FORMAT paperFormat, T_POLYLINE polyline_In, T_POLYLINE_R& polyline_Out)
{
	int nCount = polyline_In.arPoint.GetSize();
	polyline_Out.arPoint.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToTPoint(paperFormat, polyline_In.arPoint[i], polyline_Out.arPoint[i]);		
	}
}
void CDrawFunc::ConvertToPolygon(T_PAPER_FORMAT paperFormat, T_POLYLINE_R polygon_In, T_POLYLINE& polygon_Out)
{
	T_POLYLINE_R pathline;
	ConvertToPathline(polygon_In, pathline, TRUE);

	int nCount = pathline.arPoint.GetSize();
	polygon_Out.arPoint.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToPOINT(paperFormat, pathline.arPoint[i], polygon_Out.arPoint[i]);
	}
}

void CDrawFunc::ConvertToPolygonR(T_PAPER_FORMAT paperFormat, T_POLYLINE polygon_In, T_POLYLINE_R& polygon_Out)
{
	int nCount = polygon_In.arPoint.GetSize();
	polygon_Out.arPoint.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToTPoint(paperFormat, polygon_In.arPoint[i], polygon_Out.arPoint[i]);
	}
}

void CDrawFunc::ConvertToText(T_PAPER_FORMAT paperFormat, T_TEXT_R text_In, T_TEXT& text_Out)
{
	text_Out.szText      = text_In.szText;
	text_Out.bCurrFormat = text_In.bCurrFormat;
	text_Out.CurrFormat  = text_In.CurrFormat;

	ConvertToPOINT(paperFormat, text_In.PPosition, text_Out.PPosition);
}

void CDrawFunc::ConvertToTextR(T_PAPER_FORMAT paperFormat, T_TEXT text_In, T_TEXT_R& text_Out)
{
	text_Out.szText      = text_In.szText;
	text_Out.bCurrFormat = text_In.bCurrFormat;
	text_Out.CurrFormat  = text_In.CurrFormat;

	ConvertToTPoint(paperFormat, text_In.PPosition, text_Out.PPosition);
}

void CDrawFunc::ConvertToDimension(T_PAPER_FORMAT paperFormat, T_DIMENSION_R dimension_In, T_DIMENSION& dimension_Out)
{
	dimension_Out.Init();
	dimension_Out.szText = dimension_In.szText;
	ConvertToPOINT(paperFormat, dimension_In.P1, dimension_Out.P1);
	ConvertToPOINT(paperFormat, dimension_In.P2, dimension_Out.P2);   
	double dLength_R = sqrt(pow((double)(dimension_In.P1.x  - dimension_In.P2.x ), 2.0) + pow((double)(dimension_In.P1.y  - dimension_In.P2.y ), 2.0));
	double dLength   = sqrt(pow((double)(dimension_Out.P1.x - dimension_Out.P2.x), 2.0) + pow((double)(dimension_Out.P1.y - dimension_Out.P2.y), 2.0));

	if(dLength <= 2 && dLength_R > m_Zero)
	{
		dimension_Out.bDefaultAngle = TRUE;
		dimension_Out.dDefaultAngle = -1.0 * math_lib::angle_2d((double)(dimension_In.P2.x-dimension_In.P1.x), (double)(dimension_In.P2.y-dimension_In.P1.y), dLength_R, 0.0);
	}
}

void CDrawFunc::ConvertToDimensionR(T_PAPER_FORMAT paperFormat, T_DIMENSION dimension_In, T_DIMENSION_R& dimension_Out)
{
	dimension_Out.Init();
	dimension_Out.szText = dimension_In.szText;
	ConvertToTPoint(paperFormat, dimension_In.P1, dimension_Out.P1);
	ConvertToTPoint(paperFormat, dimension_In.P2, dimension_Out.P2);   
}

void CDrawFunc::ConvertToAngle(T_PAPER_FORMAT paperFormat, T_ANGLE_R angle_In, T_ANGLE& angle_Out)
{
	angle_Out.szText = angle_In.szText;
	ConvertToPOINT(paperFormat, angle_In.CenterP, angle_Out.CenterP);
	angle_Out.StartAngle = (float)angle_In.StartAngle;
	angle_Out.SweepAngle = (float)angle_In.SweepAngle;  
}

void CDrawFunc::ConvertToAngleR(T_PAPER_FORMAT paperFormat, T_ANGLE angle_In, T_ANGLE_R& angle_Out)
{
	angle_Out.szText = angle_In.szText;
	ConvertToTPoint(paperFormat, angle_In.CenterP, angle_Out.CenterP);
	angle_Out.StartAngle = (double)angle_In.StartAngle;
	angle_Out.SweepAngle = (double)angle_In.SweepAngle;  
}

void CDrawFunc::ConvertToLeaderLine(T_PAPER_FORMAT paperFormat, T_LEADERLINE_R leader_In, T_LEADERLINE& leader_Out)  
{
	leader_Out.szText = leader_In.szText;
	leader_Out.bStrDir = leader_In.bStrDir;

	int nCount = leader_In.arPoint.GetSize();
	leader_Out.arPoint.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToPOINT(paperFormat, leader_In.arPoint[i], leader_Out.arPoint[i]);
	}
}

void CDrawFunc::ConvertToLeaderLineR(T_PAPER_FORMAT paperFormat, T_LEADERLINE leader_In, T_LEADERLINE_R& leader_Out)
{
	leader_Out.szText = leader_In.szText;
	leader_Out.bStrDir = leader_In.bStrDir;

	int nCount = leader_In.arPoint.GetSize();
	leader_Out.arPoint.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToTPoint(paperFormat, leader_In.arPoint[i], leader_Out.arPoint[i]);
	}
}

void CDrawFunc::ConvertToChart(T_PAPER_FORMAT paperFormat, T_CHART_FORMAT chartFormat, T_CHART_R chart_In, T_CHART& chart_Out)
{
	MakeChartData(paperFormat, chartFormat, chart_In, chart_Out);
}

void CDrawFunc::ConvertToChartR(T_PAPER_FORMAT paperFormat, T_CHART chart_In, T_CHART_R& chart_Out)
{
	chart_Out.Init();

	if(chart_In.Info.nType != EN_CHART_TYPE_DISPERSAL) 
	{
		ASSERT(0); 
		return; 
	}

	int nXmax, nXmin, nYmax, nYmin;
	nXmax = nXmin = nYmax = nYmin = 0;

	if(chart_In.frame.arRect.GetSize() > 0)
	{		
		ConvertToTPoint(paperFormat, chart_In.frame.arRect[0].P1, chart_Out.framePosi.P1);
		ConvertToTPoint(paperFormat, chart_In.frame.arRect[0].P2, chart_Out.framePosi.P2);  		

		nXmax = max(chart_In.frame.arRect[0].P1.x, chart_In.frame.arRect[0].P2.x);
		nXmin = min(chart_In.frame.arRect[0].P1.x, chart_In.frame.arRect[0].P2.x);
		nYmax = max(chart_In.frame.arRect[0].P1.y, chart_In.frame.arRect[0].P2.y);
		nYmin = min(chart_In.frame.arRect[0].P1.y, chart_In.frame.arRect[0].P2.y);
	}

	if(chart_In.frame.arText.GetSize() > 0)
	{
		ConvertToTPoint(paperFormat, chart_In.frame.arText[0].PPosition, chart_Out.titlePosi);
	}

	if(chart_In.remark.arRect.GetSize() > 0)
	{		
		ConvertToTPoint(paperFormat, chart_In.remark.arRect[0].P1, chart_Out.remarkPosi.P1);
		ConvertToTPoint(paperFormat, chart_In.remark.arRect[0].P2, chart_Out.remarkPosi.P2);    
	}

	chart_Out.Info = chart_In.Info;

	int i, j, nSize, nPointSize;
	nSize = chart_In.arData.GetSize();
	chart_Out.arData.SetSize(nSize);	

	for(i = 0 ; i < nSize ; i++) 
	{			
		chart_Out.arData[i].Info = chart_In.arData[i].Info;

		nPointSize = chart_In.arData[i].arPoint.GetSize();
		chart_Out.arData[i].arPoint.SetSize(nPointSize);

		for(j = 0; j < nPointSize; j++) chart_Out.arData[i].arPoint[j].Init();
	}

	BOOL bXZero = (chart_Out.framePosi.P1.x == chart_Out.framePosi.P2.x);
	BOOL bYZero = (chart_Out.framePosi.P1.y == chart_Out.framePosi.P2.y);

	if(bXZero && bYZero)
	{
		double dLeft, dRight, dTop, dBottom;
		dLeft   = (chart_Out.Info.xAxis.bReverse) ?  chart_Out.Info.xAxis.dMaxValue : chart_Out.Info.xAxis.dMinValue;
		dRight  = (!chart_Out.Info.xAxis.bReverse) ? chart_Out.Info.xAxis.dMaxValue : chart_Out.Info.xAxis.dMinValue;

		dBottom = (chart_Out.Info.yAxis.bReverse) ?  chart_Out.Info.xAxis.dMaxValue : chart_Out.Info.xAxis.dMinValue;
		dTop    = (!chart_Out.Info.yAxis.bReverse) ? chart_Out.Info.xAxis.dMaxValue : chart_Out.Info.xAxis.dMinValue;

		for(i = 0; i < nSize; i++)
		{
			nPointSize = chart_In.arData[i].arPoint.GetSize();

			for(j = 0; j < nPointSize; j++)
			{			
				chart_Out.arData[i].arPoint[j].x = (bXZero) ? 0.0 : dLeft + (chart_In.arData[i].arPoint[j].x - nXmin) / (dRight-dLeft);
				chart_Out.arData[i].arPoint[j].y = (bXZero) ? 0.0 : dTop  + ((m_bEMF) ? (nYmax - chart_In.arData[i].arPoint[j].y) : (chart_In.arData[i].arPoint[j].y - nYmin)) / (dTop - dBottom);
			}
		}
	}	
}

//Array 변환---------------------------------------------------------------------------

void CDrawFunc::ConvertToArPOINT(T_PAPER_FORMAT paperFormat, nrT_POINT& arPoint_In, nrPOINT& arPoint_Out)
{
	int nCount = arPoint_In.GetSize();
	arPoint_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToPOINT(paperFormat, arPoint_In[i], arPoint_Out[i]);
	}
}

void CDrawFunc::ConvertToArTPoint(T_PAPER_FORMAT paperFormat, nrPOINT& arPoint_In, nrT_POINT& arPoint_Out)
{
	int nCount = arPoint_In.GetSize();
	arPoint_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToTPoint(paperFormat, arPoint_In[i], arPoint_Out[i]);
	}
}

void CDrawFunc::ConvertToArLine(T_PAPER_FORMAT paperFormat, nrT_LINE_R& arLine_In, nrT_LINE& arLine_Out)
{
	int nCount = arLine_In.GetSize();
	arLine_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToLine(paperFormat, arLine_In[i], arLine_Out[i]);
	}
}

void CDrawFunc::ConvertToArLineR(T_PAPER_FORMAT paperFormat, nrT_LINE& arLine_In, nrT_LINE_R& arLine_Out)
{
	int nCount = arLine_In.GetSize();
	arLine_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToLineR(paperFormat, arLine_In[i], arLine_Out[i]);
	}
}

void CDrawFunc::ConvertToArCircle(T_PAPER_FORMAT paperFormat, nrT_CIRCLE_R& arCircle_In, nrT_CIRCLE& arCircle_Out)
{
	int nCount = arCircle_In.GetSize();
	arCircle_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToCircle(paperFormat, arCircle_In[i], arCircle_Out[i]);
	}
}

void CDrawFunc::ConvertToArCircleR(T_PAPER_FORMAT paperFormat, nrT_CIRCLE& arCircle_In, nrT_CIRCLE_R& arCircle_Out)
{
	int nCount = arCircle_In.GetSize();
	arCircle_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToCircleR(paperFormat, arCircle_In[i], arCircle_Out[i]);
	}
}

void CDrawFunc::ConvertToArDonut(T_PAPER_FORMAT paperFormat, nrT_DONUT_R& arDonut_In, nrT_DONUT& arDonut_Out)
{
	int nCount = arDonut_In.GetSize();
	arDonut_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToDonut(paperFormat, arDonut_In[i], arDonut_Out[i]);
	}
}

void CDrawFunc::ConvertToArDonutR(T_PAPER_FORMAT paperFormat, nrT_DONUT& arDonut_In, nrT_DONUT_R& arDonut_Out)
{
	int nCount = arDonut_In.GetSize();
	arDonut_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToDonutR(paperFormat, arDonut_In[i], arDonut_Out[i]);		
	}
}

void CDrawFunc::ConvertToArArc(T_PAPER_FORMAT paperFormat, nrT_ARC_R& arArc_In, nrT_ARC& arArc_Out)
{
	int nCount = arArc_In.GetSize();
	arArc_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToArc(paperFormat, arArc_In[i], arArc_Out[i]);		
	}
}

void CDrawFunc::ConvertToArArcR(T_PAPER_FORMAT paperFormat, nrT_ARC& arArc_In, nrT_ARC_R& arArc_Out)
{
	int nCount = arArc_In.GetSize();
	arArc_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToArcR(paperFormat, arArc_In[i], arArc_Out[i]);		
	}
}

void CDrawFunc::ConvertToArPolyline(T_PAPER_FORMAT paperFormat, nrT_POLYLINE_R& arPolyline_In, nrT_POLYLINE& arPolyline_Out)
{
	int nCount = arPolyline_In.GetSize();
	arPolyline_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToPolyline(paperFormat, arPolyline_In[i], arPolyline_Out[i]);
	}
}

void CDrawFunc::ConvertToArPolylineR(T_PAPER_FORMAT paperFormat, nrT_POLYLINE& arPolyline_In, nrT_POLYLINE_R& arPolyline_Out)
{
	int nCount = arPolyline_In.GetSize();
	arPolyline_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToPolylineR(paperFormat, arPolyline_In[i], arPolyline_Out[i]);
	}
}

void CDrawFunc::ConvertToArPolygon(T_PAPER_FORMAT paperFormat, nrT_POLYLINE_R& arPolygon_In, nrT_POLYLINE& arPolygon_Out)
{
	int nCount = arPolygon_In.GetSize();
	arPolygon_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToPolygon(paperFormat, arPolygon_In[i], arPolygon_Out[i]);
	}
}

void CDrawFunc::ConvertToArPolygonR(T_PAPER_FORMAT paperFormat, nrT_POLYLINE& arPolygon_In, nrT_POLYLINE_R& arPolygon_Out)
{
	int nCount = arPolygon_In.GetSize();
	arPolygon_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToPolygonR(paperFormat, arPolygon_In[i], arPolygon_Out[i]);		
	}
}

void CDrawFunc::ConvertToArText(T_PAPER_FORMAT paperFormat, nrT_TEXT_R& arText_In, nrT_TEXT& arText_Out)
{
	int nCount = arText_In.GetSize();
	arText_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToText(paperFormat, arText_In[i], arText_Out[i]);
	}
}

void CDrawFunc::ConvertToArTextR(T_PAPER_FORMAT paperFormat, nrT_TEXT& arText_In, nrT_TEXT_R& arText_Out)
{
	int nCount = arText_In.GetSize();
	arText_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToTextR(paperFormat, arText_In[i], arText_Out[i]);
	}
}

void CDrawFunc::ConvertToArDimension(T_PAPER_FORMAT paperFormat, nrT_DIMENSION_R& arDimension_In, nrT_DIMENSION& arDimension_Out)
{
	int nCount = arDimension_In.GetSize();
	arDimension_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		ConvertToDimension(paperFormat, arDimension_In[i], arDimension_Out[i]);
	}
}

void CDrawFunc::ConvertToArDimensionR(T_PAPER_FORMAT paperFormat, nrT_DIMENSION& arDimension_In, nrT_DIMENSION_R& arDimension_Out)
{
	int nCount = arDimension_In.GetSize();
	arDimension_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToDimensionR(paperFormat, arDimension_In[i], arDimension_Out[i]);
	}
}

void CDrawFunc::ConvertToArAngle(T_PAPER_FORMAT paperFormat, nrT_ANGLE_R& arAngle_In, nrT_ANGLE& arAngle_Out)
{
	int nCount = arAngle_In.GetSize();
	arAngle_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToAngle(paperFormat, arAngle_In[i], arAngle_Out[i]);
	}
}

void CDrawFunc::ConvertToArAngleR(T_PAPER_FORMAT paperFormat, nrT_ANGLE& arAngle_In, nrT_ANGLE_R& arAngle_Out)
{
	int nCount = arAngle_In.GetSize();
	arAngle_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToAngleR(paperFormat, arAngle_In[i], arAngle_Out[i]);		
	}
}

void CDrawFunc::ConvertToArLeaderLine(T_PAPER_FORMAT paperFormat, nrT_LEADERLINE_R& arLeader_In, nrT_LEADERLINE& arLeader_Out)
{
	int nCount = arLeader_In.GetSize();
	arLeader_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToLeaderLine(paperFormat, arLeader_In[i], arLeader_Out[i]);		
	}
}

void CDrawFunc::ConvertToArLeaderLineR(T_PAPER_FORMAT paperFormat, nrT_LEADERLINE& arLeader_In, nrT_LEADERLINE_R& arLeader_Out)
{
	int nCount = arLeader_In.GetSize();
	arLeader_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToLeaderLineR(paperFormat, arLeader_In[i], arLeader_Out[i]);
	}
}

void CDrawFunc::ConvertToArChart(T_PAPER_FORMAT paperFormat, T_CHART_FORMAT chartFormat, nrT_CHART_R& arChart_In, nrT_CHART& arChart_Out)
{
	int nCount = arChart_In.GetSize();
	arChart_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToChart(paperFormat, chartFormat, arChart_In[i], arChart_Out[i]);
	}
}

void CDrawFunc::ConvertToArChartR(T_PAPER_FORMAT paperFormat, nrT_CHART& arChart_In, nrT_CHART_R& arChart_Out)
{
	int nCount = arChart_In.GetSize();
	arChart_Out.SetSize(nCount);

	for(int i = 0; i < nCount; ++i)
	{
		ConvertToChartR(paperFormat, arChart_In[i], arChart_Out[i]);
	}
}

//
void CDrawFunc::ConvertToPathline(T_POLYLINE_R polyline, T_POLYLINE_R& pathline, BOOL bClose)
{
	int i, j, nSize, nCount, nAddP;	
	T_POLYLINE_R arcPolyline; 
	T_POINT      originP;      

	nSize = polyline.arPoint.GetSize();

	if(nSize < 2)
	{
		pathline = polyline;
		return;
	}
	nCount = 0;
	double line[2][2];
	double centerP[2], vector1[2], vector2[2];
	double dAngle, dAngleD, dLen;

	for(i = 0; i < nSize - 1; i++)
	{
		T_POINT& P1 = polyline.arPoint[i];
		T_POINT& P2 = polyline.arPoint[i + 1];

		dLen = CMathFunc::mathLength(P1.x, P1.y, P2.x, P2.y);
		nCount++;

		if(P1.bulge != 0.0 && dLen > m_Zero)
		{
			line[0][0] = P1.x;  line[0][1] = P1.y;
			line[1][0] = P2.x;  line[1][1] = P2.y;
			CMathFunc::GetCenter_Bulge(line, P1.bulge, centerP);
			CMathFunc::mathNormalize(P1.x-centerP[0], P1.y-centerP[1], vector1[0], vector1[1]);
			CMathFunc::mathNormalize(P2.x-centerP[0], P2.y-centerP[1], vector2[0], vector2[1]);
			dAngle = fabs(CMathFunc::mathCrossAngle2DSign(vector1[0], vector1[1], vector2[0], vector2[1]));

			if(dLen / 2.0 < P1.bulge)
			{
				dAngle = 360.0 - dAngle;
			}
			nCount += max(ARCPOLY_MINSIZE, (int)(dAngle / ARCPOLY_MAXANGLE)-1);
		}
	}
	nCount++;// 마지막 좌표점
	if(bClose)
	{
		T_POINT& P1 = polyline.arPoint[nSize - 1];
		T_POINT& P2 = polyline.arPoint[0];

		dLen = CMathFunc::mathLength(P1.x, P1.y, P2.x, P2.y);

		if(P1.bulge != 0.0 && dLen > m_Zero)
		{
			line[0][0] = P1.x;  line[0][1] = P1.y;
			line[1][0] = P2.x;  line[1][1] = P2.y;
			CMathFunc::GetCenter_Bulge(line, P1.bulge, centerP);
			CMathFunc::mathNormalize(P1.x-centerP[0], P1.y-centerP[1], vector1[0], vector1[1]);
			CMathFunc::mathNormalize(P2.x-centerP[0], P2.y-centerP[1], vector2[0], vector2[1]);
			dAngle = fabs(CMathFunc::mathCrossAngle2DSign(vector1[0], vector1[1], vector2[0], vector2[1]));
			if(dLen/2.0 < P1.bulge)
			{
				dAngle = 360.0 - dAngle;
			}
			nCount += max(ARCPOLY_MINSIZE, (int)(dAngle / ARCPOLY_MAXANGLE)-1);
		}
	}

	//
	pathline.arPoint.SetSize(nCount);
	nCount = 0;
	for(i = 0; i < nSize - 1; i++)
	{
		T_POINT& P1 = polyline.arPoint[i];
		T_POINT& P2 = polyline.arPoint[i + 1];

		dLen = CMathFunc::mathLength(P1.x, P1.y, P2.x, P2.y);

		pathline.arPoint[nCount] = P1;
		nCount++;

		if(P1.bulge != 0.0 && dLen > m_Zero)
		{
			line[0][0] = P1.x;  line[0][1] = P1.y;
			line[1][0] = P2.x;  line[1][1] = P2.y;
			CMathFunc::GetCenter_Bulge(line, P1.bulge, centerP);
			CMathFunc::mathNormalize(P1.x-centerP[0], P1.y-centerP[1], vector1[0], vector1[1]);
			CMathFunc::mathNormalize(P2.x-centerP[0], P2.y-centerP[1], vector2[0], vector2[1]);
			dAngle = fabs(CMathFunc::mathCrossAngle2DSign(vector1[0], vector1[1], vector2[0], vector2[1]));
			if(dLen/2.0 < P1.bulge)
			{
				dAngle = 360.0 - dAngle;
			}
			nAddP = max(ARCPOLY_MINSIZE, (int)(dAngle / ARCPOLY_MAXANGLE)-1);
			dAngleD = dAngle/((double)nAddP+1.0) * (P1.bulge > 0.0 ? 1.0 : -1.0);
			originP.x = centerP[0];
			originP.y = centerP[1];

			GetArcPolylineR(P1, originP, dAngleD, nAddP+1, arcPolyline);
			for(j = 1; j <= nAddP ; j++)
			{
				pathline.arPoint[nCount] = arcPolyline.arPoint[j];
				nCount++;
			}
		}
	}
	pathline.arPoint[nCount] = polyline.arPoint[nSize - 1];
	nCount++;

	if(bClose)
	{
		T_POINT& P1 = polyline.arPoint[nSize - 1];
		T_POINT& P2 = polyline.arPoint[0];

		dLen = CMathFunc::mathLength(P1.x, P1.y, P2.x, P2.y);

		if(P1.bulge != 0.0 && dLen > m_Zero)
		{
			line[0][0] = P1.x;  line[0][1] = P1.y;
			line[1][0] = P2.x;  line[1][1] = P2.y;
			CMathFunc::GetCenter_Bulge(line, P1.bulge, centerP);
			CMathFunc::mathNormalize(P1.x-centerP[0], P1.y-centerP[1], vector1[0], vector1[1]);
			CMathFunc::mathNormalize(P2.x-centerP[0], P2.y-centerP[1], vector2[0], vector2[1]);
			dAngle = fabs(CMathFunc::mathCrossAngle2DSign(vector1[0], vector1[1], vector2[0], vector2[1]));
			if(dLen/2.0 < P1.bulge)
			{
				dAngle = 360.0 - dAngle;
			}
			nAddP = max(ARCPOLY_MINSIZE, (int)(dAngle / ARCPOLY_MAXANGLE)-1);
			dAngleD = dAngle / ((double)nAddP + 1.0) * (P1.bulge > 0.0 ? 1.0 : -1.0);
			originP.x = centerP[0];
			originP.y = centerP[1];

			GetArcPolylineR(P1, originP, dAngleD, nAddP+1, arcPolyline);
			for(j = 1; j <= nAddP ; j++)
			{
				pathline.arPoint[nCount] = arcPolyline.arPoint[j]; 
				nCount++;
			}
		}
	}
}

void CDrawFunc::ConvertNextLineText(int fontSize, EN_POSITION_TYPE enPosiType, int nEscapement, nrT_TEXT& arTextOrg, nrT_TEXT& arTextCng)
{
	arTextCng.RemoveAll();
	int i, j, k, nSize, nCount, nLength;
	nSize = arTextOrg.GetSize();
	CArray<int, int> arLineSize;
	arLineSize.SetSize(nSize);

	std::wstring szTemp; 

	for(i = 0; i < nSize; i++) 
	{
		nCount = 1;
		szTemp = arTextOrg[i].szText; 

		if(szTemp.find('\n') == std::wstring::npos)
		{
			arLineSize[i] = nCount;
		}
		else 
		{
			nLength = arTextOrg[i].szText.GetLength();
			for(j = 0 ; j < nLength ; j++)
			{ 
				if(arTextOrg[i].szText[j] == '\n') nCount++;
			}
			arLineSize[i] = nCount;
		}
	}
	nCount = 0;
	for(i = 0; i < nSize ; i++)
	{
		nCount += arLineSize[i];
	}
	arTextCng.SetSize(nCount);

	std::wstring::size_type nFindID;
	nCount = 0;
	POINT StartPoint;
	double dVactor[2];
	double dtrrad = CMathFunc::m_trrad;
	dVactor[0] = fontSize * cos(dtrrad * (nEscapement - 900) / 10);
	dVactor[1] = fontSize * sin(dtrrad * (nEscapement - 900) / 10) * (m_bEMF ? 1.0 : -1.0);

	for(i = 0; i < nSize ; i++)
	{
		if(arLineSize[i] <= 1)
		{
			arTextCng[nCount] = arTextOrg[i]; 
			nCount++; 
		}
		else 
		{
			if(enPosiType == EN_POSITION_TYPE_LEFT_TOP || enPosiType == EN_POSITION_TYPE_CENTER_TOP || enPosiType == EN_POSITION_TYPE_RIGHT_TOP)
			{ 
				StartPoint = arTextOrg[i].PPosition;
			}
			else if(enPosiType == EN_POSITION_TYPE_LEFT_VCENTER || enPosiType == EN_POSITION_TYPE_CENTER_VCENTER || enPosiType == EN_POSITION_TYPE_RIGHT_VCENTER)
			{ 
				StartPoint.x = arTextOrg[i].PPosition.x - (int)(0.5 * (arLineSize[i]-1) * dVactor[0]); 
				StartPoint.y = arTextOrg[i].PPosition.y - (int)(0.5 * (arLineSize[i]-1) * dVactor[1]); 
			}
			else if(enPosiType == EN_POSITION_TYPE_LEFT_BOTTOM || enPosiType == EN_POSITION_TYPE_CENTER_BOTTOM || enPosiType == EN_POSITION_TYPE_RIGHT_BOTTOM)
			{ 
				StartPoint.x = arTextOrg[i].PPosition.x - (int)((arLineSize[i] - 1) * dVactor[0]); 
				StartPoint.y = arTextOrg[i].PPosition.y - (int)((arLineSize[i] - 1) * dVactor[1]); 
			}

			szTemp = arTextOrg[i].szText;
			for(j = 0; j < arLineSize[i] ; j++)
			{
				if(j == arLineSize[i]-1)
				{
					arTextCng[nCount].szText = szTemp.c_str();
				}
				else 
				{
					nFindID = szTemp.find('\n');
					if(nFindID >= 0)
					{
						nLength = (int)szTemp.length();

						std::wstring temp = _T("");

						//arTextCng[nCount].szText = szText.Left(nFindID);
						for(k = 0; k < (int)nFindID; k++)
						{
							temp += szTemp[k];
						}
						arTextCng[nCount].szText = temp.c_str();

						//szText = szText.Right(nLength - (nFindID+1));
						temp = _T("");
						for(k = nLength - ((int)nFindID + 1); k < lstrlen(szTemp.c_str()); k++)
						{
							temp += szTemp[k];
						}
						szTemp = temp;						
					}
					else
					{
						ASSERT(0);
						arTextCng[nCount].szText = _T("");						
					}
				}
				arTextCng[nCount].PPosition.x = (int)(StartPoint.x + j * dVactor[0]); 	
				arTextCng[nCount].PPosition.y = (int)(StartPoint.y + j * dVactor[1]); 	
				nCount++;
			}
		}
	}

}

void CDrawFunc::ConvertMathText(HDC DC, int fontSize, int fontSize_sub, EN_POSITION_TYPE enPosiType, int nEscapement, nrT_TEXT& arTextOrg, nrT_TEXT& arTextCng, nrT_TEXT& arTextCng_sub)
{	
	arTextCng.RemoveAll();
	arTextCng_sub.RemoveAll();

	int    i, j, nUSize, nCount, nCount_non, nCount_sub, nTotal_non, nTotal_sub, nLength;
	double dSubScale = (double)(fontSize_sub) / (double)(fontSize);

	nrINT        arStrSize;
	std::wstring szTemp;

	arStrSize.SetSize(arTextOrg.GetSize());

	nTotal_non = nTotal_sub = 0;
	//int* itrStrSize = arStrSize.Data.begin();
	int* itrStrSize = arStrSize.GetData();
	
	//for(auto itr = arTextOrg.Data.begin(); itr != arTextOrg.Data.end(); itr++, itrStrSize++) 
	for(i = 0 ; i < arTextOrg.GetSize(); i++, itrStrSize++)
	{		
		T_TEXT* itr = &arTextOrg[i];
		szTemp = itr->szText;

		if(szTemp.find('`') == std::wstring::npos && szTemp.find('^') == std::wstring::npos)
		{
			nTotal_non++;
			*itrStrSize = 1;
		}
		else 
		{			
			nCount_non = nCount_sub = 0; 
			nLength = itr->szText.GetLength();
			BOOL bEndMark = TRUE;
			int  nStartPosi = 0;

			for(j = 0; j < nLength; j++)
			{
				if(bEndMark && (itr->szText[j] == '`' || itr->szText[j] == '^'))
				{ 
					if(j - nStartPosi > 0)
					{
						nCount_non++;
					}
					bEndMark = FALSE;  
					nStartPosi = j + 1;
				}
				else if(!bEndMark && itr->szText[j] == '$')
				{
					if(j - nStartPosi > 0) 
					{
						nCount_sub++;
					}
					bEndMark = TRUE; 
					nStartPosi = j + 1;
				}
				else if(j == nLength - 1 && j - nStartPosi >= 0)
				{
					if(bEndMark) nCount_non++;
					else          nCount_sub++;					
				}
			}
			nTotal_non += nCount_non;
			nTotal_sub += nCount_sub;
			*itrStrSize = nCount_non + nCount_sub;
		}
	}
	arTextCng.SetSize(nTotal_non);
	arTextCng_sub.SetSize(nTotal_sub);

	T_TEXT			 DgnText;
	CArray<TLongStr, TLongStr> arString; //문자열
	nrDOUBLE		 arWidth;  //문자열 폭
	nrINT			 arSubType;//0:Non, 1:위첨자, 2:아래첨자

	nTotal_non = nTotal_sub =0;
	int nEMFSign = (m_bEMF? 1 : -1);

	//itrStrSize = arStrSize.Data.begin();
	itrStrSize = arStrSize.GetData();

	//for(auto itr = arTextOrg.Data.begin(); itr != arTextOrg.Data.end(); itr++) 
	for(i = 0 ; i < arTextOrg.GetSize(); i++)
	{		
		T_TEXT* itr = &arTextOrg[i];
		szTemp = itr->szText;

		if(szTemp.find('`') == std::wstring::npos && szTemp.find('^') == std::wstring::npos)
		{
			arTextCng[nTotal_non] = *itr;
			nTotal_non++;
		}
		else 
		{		
			nUSize = *itrStrSize;
			arString.SetSize(nUSize);
			arWidth.SetSize(nUSize);			
			arSubType.SetSize(nUSize);

			nCount = 0; 
			nLength = itr->szText.GetLength();
			BOOL bEndMark = TRUE;
			int  nStartPosi = 0;
			int  nEndPosi = 0;
			BOOL bRecNon, bRecSub, bUpdateStartPosi;
			SIZE size;
			size.cx = 0; size.cy = 0;

			for(j = 0; j < nLength ; j++)
			{
				bRecNon = bRecSub = bUpdateStartPosi = FALSE;
				if(bEndMark && (itr->szText[j] == '`' || itr->szText[j] == '^'))
				{ 
					if(j - nStartPosi > 0) 
					{ 
						bRecNon = TRUE; 
						nEndPosi = j - 1;
					}
					bEndMark = FALSE; 
					bUpdateStartPosi = TRUE;
				}
				else if(!bEndMark && itr->szText[j] == '$')
				{
					if(j-nStartPosi > 0)
					{
						bRecSub = TRUE; 
						nEndPosi = j - 1;
					}
					bEndMark = TRUE; 
					bUpdateStartPosi = TRUE;
				}
				else if(j == nLength - 1 && j - nStartPosi >= 0)
				{
					if(bEndMark) 
					{
						bRecNon = TRUE; 
						nEndPosi = j;
					}
					else         
					{
						bRecSub = TRUE; 
						nEndPosi = j;
					}				
				}

				if(bRecNon || bRecSub)
				{
					// arString[nCount] = szTemp.Mid(nStartPosi, (nEndPosi-nStartPosi+1));
					int nEndPos = nEndPosi - nStartPosi + 1;

					std::wstring temp = _T("");
					for(int k = nStartPosi; k < nEndPos; k++)
					{
						temp += szTemp[k];
					}
					arString[nCount] = temp.c_str();

					//size = (m_bEMF ? DC->GetOutputTextExtent(arString[nCount]) : DC->GetTextExtent(arString[nCount]));
					::GetTextExtentPoint32(DC, arString[nCount], arString[nCount].GetLength(), &size);

					arWidth[nCount] = (bRecNon ? 1.0 : dSubScale) * size.cx;
					if(bRecNon) 
					{
						arSubType[nCount] = 0;
					}
					else if(nStartPosi > 0)
					{
						if     (itr->szText[nStartPosi - 1] == '^') arSubType[nCount] = 1;
						else if(itr->szText[nStartPosi - 1] == '`') arSubType[nCount] = 2;
						else 
						{ 
							ASSERT(0);
							arSubType[nCount] = 0; 
						}
					}
					else
					{
						ASSERT(0);
						arSubType[nCount] = 0; 
					}
					nCount++;
				}

				if(bUpdateStartPosi) nStartPosi = j + 1;
			}

			double dTotalWidth = 0.0;
			for(j = 0; j < nUSize; j++) dTotalWidth += arWidth[j];

			double dFactor[2];
			dFactor[0] = cos(nEscapement / 10.0 * CMathFunc::m_trrad);
			dFactor[1] = sin(nEscapement / 10.0 * CMathFunc::m_trrad);
			SIZE textSize;
			if(itr->szText != _T("")) 
			{
				//	textSize = m_bEMF ? DC->GetOutputTextExtent(arTextOrg[i].szText) : DC->GetTextExtent(arTextOrg[i].szText);
				::GetTextExtentPoint32(DC, itr->szText, itr->szText.GetLength(), &textSize);
			}
			POINT LeftTopPoint;    

			if(enPosiType == EN_POSITION_TYPE_LEFT_TOP) 
			{
				LeftTopPoint.x = itr->PPosition.x;  			
				LeftTopPoint.y = itr->PPosition.y; 
			}
			else if(enPosiType == EN_POSITION_TYPE_LEFT_VCENTER) 
			{ 
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cy / 2.0 * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (textSize.cy / 2.0 * dFactor[0]) * nEMFSign);
			}
			else if(enPosiType == EN_POSITION_TYPE_LEFT_BOTTOM) 
			{ 
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cy * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (textSize.cy * dFactor[0]) * nEMFSign); 
			}
			else if(enPosiType == EN_POSITION_TYPE_CENTER_TOP)
			{ 
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-dTotalWidth / 2.0 * dFactor[0]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-dTotalWidth / 2.0 * dFactor[1]) * nEMFSign); 
			}
			else if(enPosiType == EN_POSITION_TYPE_CENTER_VCENTER)
			{ 
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-dTotalWidth / 2.0 * dFactor[0] - textSize.cy / 2.0 * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-dTotalWidth / 2.0 * dFactor[1] + textSize.cy / 2.0 * dFactor[0]) * nEMFSign);
			}
			else if(enPosiType == EN_POSITION_TYPE_CENTER_BOTTOM )
			{ 
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-dTotalWidth / 2.0 * dFactor[0] - textSize.cy * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-dTotalWidth / 2.0 * dFactor[1] + textSize.cy * dFactor[0]) * nEMFSign);
			}
			else if(enPosiType == EN_POSITION_TYPE_RIGHT_TOP)
			{ 
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-dTotalWidth * dFactor[0]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-dTotalWidth * dFactor[1]) * nEMFSign); }
			else if(enPosiType == EN_POSITION_TYPE_RIGHT_VCENTER)
			{
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-dTotalWidth * dFactor[0] - textSize.cy / 2.0 * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-dTotalWidth * dFactor[1] + textSize.cy / 2.0 * dFactor[0]) * nEMFSign); 
			}
			else if(enPosiType == EN_POSITION_TYPE_RIGHT_BOTTOM)
			{ 
				LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-dTotalWidth * dFactor[0] - textSize.cy * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-dTotalWidth * dFactor[1] + textSize.cy * dFactor[0]) * nEMFSign);
			}
			else ASSERT(0);

			double dHeight = (double)size.cy;
			double dHeight_cur;
			double dWidth = 0.0;
			double dPosi[2];

			for(j = 0; j < nUSize ; j++)
			{
				DgnText.Init();
				DgnText.szText = arString[j];
				//좌상단점 산출
				dPosi[0] = LeftTopPoint.x + dWidth * dFactor[0];
				dPosi[1] = LeftTopPoint.y + dWidth * dFactor[1] * nEMFSign;

				if(arSubType[j] == 2)
				{
					//아래첨자만 수직으로 내려줌	
					dPosi[0] = dPosi[0] + dHeight * (1.0 - dSubScale) * dFactor[1];
					dPosi[1] = dPosi[1] - dHeight * (1.0 - dSubScale) * dFactor[0] * nEMFSign;
				}
				dHeight_cur = (arSubType[j] == 0) ? dHeight : dHeight*dSubScale;

				//해당위치의 기준점으로 변경
				if(enPosiType == EN_POSITION_TYPE_LEFT_TOP) 
				{
					DgnText.PPosition.x = math_lib::round_off(dPosi[0]);  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1]);
				}
				else if(enPosiType == EN_POSITION_TYPE_LEFT_VCENTER) 
				{
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-dHeight_cur / 2.0 * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (dHeight_cur / 2.0 * dFactor[0]) * nEMFSign);
				}
				else if(enPosiType==EN_POSITION_TYPE_LEFT_BOTTOM) 
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-dHeight_cur * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (dHeight_cur * dFactor[0]) * nEMFSign);
				}
				else if(enPosiType==EN_POSITION_TYPE_CENTER_TOP)
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j] / 2.0 * dFactor[0]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j] / 2.0 * dFactor[1]) * nEMFSign); 
				}
				else if(enPosiType==EN_POSITION_TYPE_CENTER_VCENTER)
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j] / 2.0 * dFactor[0] - dHeight_cur / 2.0 * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j] / 2.0 * dFactor[1] + dHeight_cur / 2.0 * dFactor[0]) * nEMFSign); 
				}
				else if(enPosiType==EN_POSITION_TYPE_CENTER_BOTTOM)
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j] / 2.0 * dFactor[0] - dHeight_cur * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j] / 2.0 * dFactor[1] + dHeight_cur * dFactor[0]) * nEMFSign);
				}
				else if(enPosiType==EN_POSITION_TYPE_RIGHT_TOP)
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]*dFactor[0]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]*dFactor[1]) * nEMFSign); 
				}
				else if(enPosiType==EN_POSITION_TYPE_RIGHT_VCENTER)
				{
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]*dFactor[0] - dHeight_cur / 2.0 * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]*dFactor[1] + dHeight_cur / 2.0 * dFactor[0]) * nEMFSign);
				}
				else if(enPosiType==EN_POSITION_TYPE_RIGHT_BOTTOM)
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]*dFactor[0] - dHeight_cur * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]*dFactor[1] + dHeight_cur * dFactor[0]) * nEMFSign); 
				}
				else ASSERT(0);

				if(arSubType[j] == 0)
				{
					arTextCng[nTotal_non] = DgnText;
					nTotal_non ++;
				}
				else 
				{
					arTextCng_sub[nTotal_sub] = DgnText;
					nTotal_sub ++;
				}
				dWidth += arWidth[j];
			}
		}
	}
}

int CDrawFunc::ConvertToChartValue(int nP1, int nP2, double dP1, double dP2, double dValue)
{
	return (dP1 == dP2) ? nP1 : nP1 + (int)((nP1 - nP2) / (dP1 - dP2) * (dValue - dP1));
}

int CDrawFunc::ConvertToMapLength(int iMode, double dLength)
{	
	if(m_bEMF)
	{
		// Emf 출력인 경우	
		double dLength_factor = (iMode == 1 || iMode == 2 ? 3.0 / 4.0 : 1.0);// Office2007일경우 1.0을 사용 2003이하는 3/4

		if(iMode == 1 || iMode == 3)
		{
			//Pixel단위						
			double dLength_mm = dLength * 0.2635;// 곱하여진 값은 Office상에 픽셀과 길이의 비로 계산된 값임
			double dLength_in = dLength_mm / 25.4;

			switch(m_nMapMode)
			{
			case MM_HIENGLISH://0.001 inch
				return  math_lib::round_off(dLength_in * 1000.0 * dLength_factor);
			case MM_HIMETRIC://0.01 mm
				return  math_lib::round_off(dLength_mm * 100.0 * dLength_factor);
			case MM_LOENGLISH://0.01 inch
				return  math_lib::round_off(dLength_in * 100.0 * dLength_factor);
			case MM_LOMETRIC://0.1 mm
				return  math_lib::round_off(dLength_mm * 10.0 * dLength_factor);
			case MM_TEXT:// 1 Pixel
				return math_lib::round_off(dLength);
			case MM_TWIPS:// 1/20 Pixel
				return math_lib::round_off(dLength/20.0);
			default:// MM_ISOTROPIC는 지원하지 않음
				ASSERT(0);
				break;
			}			
		}
		else if(iMode == 2 || iMode == 4)
		{
			//mm단위					
			double dLength_pixel = dLength / 0.2635;
			double dLength_mm    = dLength;
			double dLength_in    = dLength_mm / 25.4;

			switch(m_nMapMode)
			{
			case MM_HIENGLISH://0.001 inch
				return  math_lib::round_off(dLength_in * 1000.0 * dLength_factor);
			case MM_HIMETRIC://0.01 mm
				return  math_lib::round_off(dLength * 100.0 * dLength_factor);
			case MM_LOENGLISH://0.01 inch
				return  math_lib::round_off(dLength_in * 100.0 * dLength_factor);
			case MM_LOMETRIC://0.1 mm
				return  math_lib::round_off(dLength * 10.0 * dLength_factor);
			case MM_TEXT:// 1 Pixel
				return math_lib::round_off(dLength_pixel);
			case MM_TWIPS:// 1/20 Pixel
				return math_lib::round_off(dLength_pixel/20.0);
			default:// MM_ISOTROPIC는 지원하지 않음				
				break;
			}			
		}
	}
	else
	{
		// Bmp 출력인 경우
		if(iMode == 1 || iMode == 3)//Pixel단위
		{
			return  math_lib::round_off(dLength);
		}
		else if(iMode == 2 || iMode == 4)//mm단위
		{
			return  math_lib::round_off(dLength/0.2645);		
		}
	}
	ASSERT(0);//해당사항이 없을 경우
	return math_lib::round_off(dLength);
}