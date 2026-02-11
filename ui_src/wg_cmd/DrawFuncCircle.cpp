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

// DrawCircle ////////////////////////////////////

// T_DRAW_FORMAT 이용하여 DC에 Circle을 그림
void CDrawFunc::DrawCircle(HDC DC, T_DRAW_FORMAT Format, T_CIRCLE Circle)
{ 
	if     (Format.bLine) DrawCircle(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, Circle, Format.nPenSubStyle); 
	else if(Format.bFill) DrawCircle(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, Circle, Format.nPenSubStyle); 
};

// T_DRAW_FORMAT 이용하여 DC에 Circle을 그림
void CDrawFunc::DrawCircle(HDC DC, T_DRAW_FORMAT Format, nrT_CIRCLE& arCircle)
{ 
	if     (Format.bLine) DrawCircle(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, arCircle, Format.nPenSubStyle); 
	else if(Format.bFill) DrawCircle(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, arCircle, Format.nPenSubStyle); 
};

void CDrawFunc::DrawCircle(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, T_CIRCLE dgnCircle, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_CIRCLE arCircle;
	arCircle.SetSize(1);
	arCircle[0] = dgnCircle;
	DrawCircle(DC, nPenStyle, lineColor, fillColor, bFill, Width, arCircle, nPenSubStyle);
}

void CDrawFunc::DrawCircle(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, nrT_CIRCLE& arCircle, int nPenSubStyle)
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
	HBRUSH   FillBrush;
	LOGBRUSH logbr;
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

	//for(auto i = arCircle.Data.begin(); i != arCircle.Data.end(); i++)
	for(i = 0 ; i < arCircle.GetSize(); i++)
	{
		T_CIRCLE* itr = &arCircle[i];
		if(itr->Radius == 0) continue;

		int x1 = itr->CenterP.x - itr->Radius;
		int y1 = itr->CenterP.y - itr->Radius;
		int x2 = itr->CenterP.x + itr->Radius;
		int y2 = itr->CenterP.y + itr->Radius;

		::Ellipse(DC, x1, y1, x2, y2);
	}

	::SelectObject(DC, pOldPen);
	::SelectObject(DC, pOldBrh);

	if(LinePen)   ::DeleteObject(LinePen);
	if(FillBrush) ::DeleteObject(FillBrush);
}