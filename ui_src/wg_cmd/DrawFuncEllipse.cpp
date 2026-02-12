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

// DrawEllipse ////////////////////////////////////

// T_DRAW_FORMAT 이용하여 DC에 타원을 그림
void CDrawFunc::DrawEllipse(HDC DC, T_DRAW_FORMAT Format, int x1, int y1, int x2, int y2)
{ 
	if     (Format.bLine) DrawEllipse(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, x1, y1, x2, y2, Format.nPenSubStyle); 
	else if(Format.bFill) DrawEllipse(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, x1, y1, x2, y2, Format.nPenSubStyle); 
};

// T_DRAW_FORMAT 이용하여 DC에 타원을 그림
void CDrawFunc::DrawEllipse(HDC DC, T_DRAW_FORMAT Format, T_LINE Line)
{ 
	if     (Format.bLine) DrawEllipse(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, Line, Format.nPenSubStyle); 
	else if(Format.bFill) DrawEllipse(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, Line, Format.nPenSubStyle); 
};

// T_DRAW_FORMAT 이용하여 DC에 타원을 그림
void CDrawFunc::DrawEllipse(HDC DC, T_DRAW_FORMAT Format, nrT_LINE& arLine)
{
	if     (Format.bLine) DrawEllipse(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, arLine, Format.nPenSubStyle); 
	else if(Format.bFill) DrawEllipse(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, arLine, Format.nPenSubStyle); 
};

void CDrawFunc::DrawEllipse(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, int x1, int y1, int x2, int y2, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_LINE arLine;
	arLine.SetSize(1);
	T_LINE dgnLine;
	dgnLine.P1.x = x1;  dgnLine.P1.y = y1;
	dgnLine.P2.x = x2;  dgnLine.P2.y = y2;
	arLine[0] = dgnLine;
	DrawEllipse(DC, nPenStyle, lineColor, fillColor, bFill, Width, arLine, nPenSubStyle);
}

void CDrawFunc::DrawEllipse(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, T_LINE dgnLine, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_LINE arLine;
	arLine.SetSize(1);
	arLine[0] = dgnLine;

	DrawEllipse(DC, nPenStyle, lineColor, fillColor, bFill, Width, arLine, nPenSubStyle);
}

void CDrawFunc::DrawEllipse(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, nrT_LINE& arLine, int nPenSubStyle)
{
	HGDIOBJ  pOldBrh;
	HPEN     LinePen;
	LOGBRUSH logBrush; 
	if(nPenSubStyle < 0 || nPenStyle!=PS_SOLID)
	{
		LinePen = ::CreatePen(nPenStyle, Width, lineColor);
	}
	else 
	{
		logBrush.lbStyle = nPenStyle; 
		logBrush.lbColor = lineColor; 
		LinePen = ::ExtCreatePen(nPenSubStyle, Width, &logBrush, 0, NULL);		
	} 
	HBRUSH FillBrush;
	FillBrush = ::CreateSolidBrush(fillColor);
	HGDIOBJ pOldPen = ::SelectObject(DC, LinePen);
	if(bFill) pOldBrh = ::SelectObject(DC, FillBrush);

	int i;
	//for(auto itr = arLine.Data.begin(); itr != arLine.Data.end(); itr++)
	for(i = 0 ; i < arLine.GetSize(); i++)
	{   
		T_LINE* itr = &arLine[i];
		if(itr->P1.x == itr->P2.x && itr->P1.y == itr->P2.y) continue;

		::Ellipse(DC, itr->P1.x, itr->P1.y, itr->P2.x, itr->P2.y);		
	}

	::SelectObject(DC, pOldPen);
	if(bFill) ::SelectObject(DC, pOldBrh);

	if(LinePen) ::DeleteObject(LinePen);
	if(FillBrush) ::DeleteObject(FillBrush);
}