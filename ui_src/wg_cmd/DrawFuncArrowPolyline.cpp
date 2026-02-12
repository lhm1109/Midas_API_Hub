#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"

#include "DrawData.h"
#include "DrawFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDrawFunc

// DrawArrowPolyline ////////////////////////////////

// T_DGN_DRAW_FORMAT 이용하여 pDC에 화살표를 가진 꺽은선을 그림
void CDrawFunc::DrawArrowPolyline(HDC DC, T_DRAW_FORMAT drawFormat, T_POLYLINE dgnPolyLine)
{ 
	if(drawFormat.bLine) 
	{
		DrawArrowPolyline(DC, drawFormat.nPenStyle, drawFormat.lineColor, drawFormat.fillColor, drawFormat.bFill, drawFormat.Width, drawFormat.dRWidth, drawFormat.dRHeight, drawFormat.startArrowType, drawFormat.endArrowType, dgnPolyLine, drawFormat.nPenSubStyle); 
	}
	else if(drawFormat.bFill)
	{
		DrawPolyline(DC, PS_SOLID, drawFormat.fillColor, drawFormat.fillColor, drawFormat.bFill, 0, dgnPolyLine, drawFormat.nPenSubStyle); 
	}
};

// T_DGN_DRAW_FORMAT 이용하여 pDC에 화살표를 가진 여러개의 꺽은선을 그림
void CDrawFunc::DrawArrowPolyline(HDC DC, T_DRAW_FORMAT drawFormat, nrT_POLYLINE& arPolyLine)
{ 
	if(drawFormat.bLine)
	{
		DrawArrowPolyline(DC, drawFormat.nPenStyle, drawFormat.lineColor, drawFormat.fillColor, drawFormat.bFill, drawFormat.Width, drawFormat.dRWidth, drawFormat.dRHeight, drawFormat.startArrowType, drawFormat.endArrowType, arPolyLine, drawFormat.nPenSubStyle); 
	}
	else if(drawFormat.bFill)
	{
		DrawPolyline(DC, PS_SOLID, drawFormat.fillColor, drawFormat.fillColor, drawFormat.bFill, 0, arPolyLine, drawFormat.nPenSubStyle); 
	}
};

void CDrawFunc::DrawArrowPolyline(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, T_POLYLINE dgnPolyLine, int nPenSubStyle)
{
	int nSize = dgnPolyLine.arPoint.GetSize();
	T_LINE dgnLine;

	if(nSize == 2)
	{
		dgnLine.P1 = dgnPolyLine.arPoint[0];
		dgnLine.P2 = dgnPolyLine.arPoint[1];
		
		DrawArrow(DC, nPenStyle, lineColor, Width, dRWidth, dRHeight, startArrowType, endArrowType, dgnLine, nPenSubStyle);
	}
	if(nSize > 2)
	{
		DrawPolyline(DC, nPenStyle, lineColor, fillColor, bFill, Width, dgnPolyLine, nPenSubStyle);
		
		if(startArrowType != EN_ARROW_TYPE_NONE)
		{
			dgnLine.P1 = dgnPolyLine.arPoint[0];
			dgnLine.P2 = dgnPolyLine.arPoint[1];
			
			DrawArrow(DC, nPenStyle, lineColor, Width, dRWidth, dRHeight, startArrowType, EN_ARROW_TYPE_NONE, dgnLine, nPenSubStyle);
		}
		if(endArrowType != EN_ARROW_TYPE_NONE)
		{
			dgnLine.P1 = dgnPolyLine.arPoint[nSize - 2];
			dgnLine.P2 = dgnPolyLine.arPoint[nSize - 1];
			
			DrawArrow(DC, nPenStyle, lineColor, Width, dRWidth, dRHeight, EN_ARROW_TYPE_NONE, endArrowType, dgnLine, nPenSubStyle);
		}
	}
}

void CDrawFunc::DrawArrowPolyline(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, nrT_POLYLINE& arPolyLine, int nPenSubStyle)
{
	int i;
	//for(auto i = arPolyLine.Data.begin(); i != arPolyLine.Data.end(); i++) 
	for( i = 0 ; i < arPolyLine.GetSize(); i++)
	{
		T_POLYLINE* itr = &arPolyLine[i];
		DrawArrowPolyline(DC, nPenStyle, lineColor, fillColor, bFill, Width, dRWidth, dRHeight, startArrowType, endArrowType, (*itr), nPenSubStyle);
	}
}