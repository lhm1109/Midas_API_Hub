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

// DrawPolyline ////////////////////////////////////

// T_DRAW_FORMAT 이용하여 DC에 꺽은선을 그림
void CDrawFunc::DrawPolyline(HDC DC, T_DRAW_FORMAT Format, T_POLYLINE PolyLine)
{ 
	if(Format.bLine)
	{
		DrawPolyline(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, PolyLine, Format.nPenSubStyle); 
	}
	else if(Format.bFill)
	{
		DrawPolyline(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, PolyLine, Format.nPenSubStyle); 
	}
};

// T_DRAW_FORMAT 이용하여 DC에 꺽은선을 그림
void CDrawFunc::DrawPolyline(HDC DC, T_DRAW_FORMAT Format, nrT_POLYLINE& arPolyLine)
{ 
	if(Format.bLine) 
	{
		DrawPolyline(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, arPolyLine, Format.nPenSubStyle); 
	}
	else if(Format.bFill)
	{
		DrawPolyline(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, arPolyLine, Format.nPenSubStyle); 
	}
};

void CDrawFunc::DrawPolyline(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, T_POLYLINE dgnPolyLine, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_POLYLINE arPolyLine;

	arPolyLine.SetSize(1);
	arPolyLine[0] = dgnPolyLine;

	DrawPolyline(DC, nPenStyle, lineColor, fillColor, bFill, Width, arPolyLine, nPenSubStyle);
}

void CDrawFunc::DrawPolyline(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, nrT_POLYLINE& arPolyLine, int nPenSubStyle)
{
	HPEN     LinePen;
	LOGBRUSH logBrush; 
	HBRUSH   FillBrush;
	LOGBRUSH logbr;

	if(nPenSubStyle < 0 || nPenStyle != PS_SOLID)
	{
		LinePen = ::CreatePen(nPenStyle, Width, lineColor);
	}
	else 
	{
		logBrush.lbStyle = nPenStyle; 
		logBrush.lbColor = lineColor; 
		LinePen = ::ExtCreatePen(nPenSubStyle, Width, &logBrush, 0, NULL);		
	} 

	logbr.lbStyle = BS_NULL;
	if(bFill)
	{
		FillBrush = ::CreateSolidBrush(fillColor);
	}
	else
	{
		FillBrush = ::CreateBrushIndirect(&logbr);
	}

	HGDIOBJ pOldPen = ::SelectObject(DC, LinePen);
	HGDIOBJ pOldBrh = ::SelectObject(DC, FillBrush);
	
	int i;
	//for(auto i = arPolyLine.Data.begin(); i != arPolyLine.Data.end(); i++)
	for(i = 0 ; i < arPolyLine.GetSize(); i++)
	{   
		T_POLYLINE* itr = &arPolyLine[i];
		::Polyline(DC, itr->arPoint.GetData(), itr->arPoint.GetSize());
	}

	::SelectObject(DC, pOldPen);
	::SelectObject(DC, pOldBrh);

	if(LinePen)   ::DeleteObject(LinePen);
	if(FillBrush) ::DeleteObject(FillBrush);
}