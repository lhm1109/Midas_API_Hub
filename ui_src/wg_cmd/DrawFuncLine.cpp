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
// DrawLine

// T_DRAW_FORMAT 이용하여 DC에 Line을 그림
void CDrawFunc::DrawLine(HDC DC, T_DRAW_FORMAT Format, int Width, int x1, int y1, int x2, int y2)
{ 
	if(Format.bLine) DrawLine(DC, Format.nPenStyle, Format.lineColor, Format.Width, x1, y1, x2, y2, Format.nPenSubStyle);
}
// T_DRAW_FORMAT 이용하여 DC에 Line을 그림
void CDrawFunc::DrawLine(HDC DC, T_DRAW_FORMAT Format, T_LINE Line)
{
	if(Format.bLine) DrawLine(DC, Format.nPenStyle, Format.lineColor, Format.Width, Line, Format.nPenSubStyle); 
}
// T_DRAW_FORMAT 이용하여 DC에 Line을 그림
void CDrawFunc::DrawLine(HDC DC, T_DRAW_FORMAT Format, nrT_LINE& arLine)
{
	if(Format.bLine) DrawLine(DC, Format.nPenStyle, Format.lineColor, Format.Width, arLine, Format.nPenSubStyle); 
}

void CDrawFunc::DrawLine(HDC DC, int nPenStyle, COLORREF lineColor,int Width, int x1, int y1, int x2, int y2, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_LINE arLine;
	arLine.SetSize(1);
	T_LINE dgnLine;
	dgnLine.P1.x = x1;  
	dgnLine.P1.y = y1;
	dgnLine.P2.x = x2; 
	dgnLine.P2.y = y2;
	arLine[0] = dgnLine;

	DrawLine(DC, nPenStyle, lineColor, Width, arLine, nPenSubStyle);
}

void CDrawFunc::DrawLine(HDC DC, int nPenStyle, COLORREF lineColor, int Width, T_LINE dgnLine, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_LINE arLine;
	arLine.SetSize(1);
	arLine[0] = dgnLine;
	DrawLine(DC, nPenStyle, lineColor, Width, arLine, nPenSubStyle);
}

void CDrawFunc::DrawLine(HDC DC, int nPenStyle, COLORREF lineColor, int Width, nrT_LINE& arLine, int nPenSubStyle)
{
	HPEN     LinePen;
	LOGBRUSH logBrush; 

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
	HGDIOBJ pOldPen = ::SelectObject(DC, LinePen);

	int nSize = arLine.GetSize();  
	nrPOINT arPoint;
	arPoint.SetSize(nSize);

	for(int i = 0; i < nSize; i++)
	{
		arPoint[i] = arLine[i].P2;
		
		if(arLine[i].P1.x == arLine[i].P2.x && arLine[i].P1.y == arLine[i].P2.y) continue;
		
		::MoveToEx(DC, arLine[i].P1.x, arLine[i].P1.y, NULL);
		::LineTo(DC, arLine[i].P2.x, arLine[i].P2.y);
	}
	::SelectObject(DC, pOldPen);

	if(LinePen) ::DeleteObject(LinePen);

	if(Width == 1)
	{
		DrawPoint(DC, lineColor, Width, arPoint);//선의 끝부분이 조금 작게 찍히는 부분을 수정(Width==1일때 1Pixel작게 찍히는 부분을 보완)
	}
}
