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
// DrawDonut ////////////////////////////////////

// T_DRAW_FORMAT 이용하여 DC에 Donut을 그림
void CDrawFunc::DrawDonut(HDC DC, T_DRAW_FORMAT Format, T_DONUT Donut)
{ 
	if     (Format.bLine) DrawDonut(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, Donut, Format.nPenSubStyle);
	else if(Format.bFill) DrawDonut(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, Donut, Format.nPenSubStyle);
};

// T_DRAW_FORMAT 이용하여 DC에 Donut을 그림
void CDrawFunc::DrawDonut(HDC DC, T_DRAW_FORMAT Format, nrT_DONUT& arDonut)
{ 
	if     (Format.bLine) DrawDonut(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, arDonut, Format.nPenSubStyle); 
	else if(Format.bFill) DrawDonut(DC, PS_SOLID, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, arDonut, Format.nPenSubStyle); 
};

void CDrawFunc::DrawDonut(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, T_DONUT dgnDonut, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_DONUT arDonut;
	arDonut.SetSize(1);
	arDonut[0] = dgnDonut;

	DrawDonut(DC, nPenStyle, lineColor, fillColor, bFill, Width, arDonut, nPenSubStyle);
}

void CDrawFunc::DrawDonut(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, nrT_DONUT& arDonut, int nPenSubStyle)
{
	HGDIOBJ  pOldPen;
	HGDIOBJ  pOldBrh;
	HBRUSH  FillBrush;
	LOGBRUSH logbr;

	logbr.lbStyle = BS_NULL;
	FillBrush = ::CreateBrushIndirect(&logbr);

	pOldBrh = ::SelectObject(DC, FillBrush);

	int Thick, Radius, x1, y1, x2, y2;
	int i;

	if(bFill)
	{
		//for(auto itr = arDonut.Data.begin(); itr != arDonut.Data.end(); itr++) 
		for(i = 0 ; i < arDonut.GetSize(); i++)
		{
			T_DONUT* itr = &arDonut[i];
			Thick = itr->OutRadius - itr->InRadius;
			Radius = (itr->OutRadius + itr->InRadius) / 2;
			if(Thick == 0) continue;
			if(Radius == 0) continue;

			HPEN FillPen;
			FillPen = ::CreatePen(PS_SOLID, Thick, fillColor);
			pOldPen = ::SelectObject(DC, FillPen);

			x1 = itr->CenterP.x - Radius;
			y1 = itr->CenterP.y - Radius;
			x2 = itr->CenterP.x + Radius;
			y2 = itr->CenterP.y + Radius;
			::Ellipse(DC, x1, y1, x2, y2);

			::SelectObject(DC, pOldPen);
			if(FillPen) ::DeleteObject(FillPen);
		}
	}

	HPEN     LinePen;
	LOGBRUSH logBrush; 
	if(nPenSubStyle < 0 || nPenStyle!=PS_SOLID)
	{
		LinePen = CreatePen(nPenStyle, Width, lineColor);
	}
	else 
	{
		logBrush.lbStyle = nPenStyle; 
		logBrush.lbColor = lineColor; 
		// LinePen.CreatePen(nPenSubStyle, Width, &logBrush);
		::ExtCreatePen(nPenSubStyle, Width, &logBrush, 0, NULL); 
	} 
	pOldPen = ::SelectObject(DC, LinePen);
	//외곽선
	//for(auto itr = arDonut.Data.begin(); itr != arDonut.Data.end(); itr++) 
	for( i = 0 ; i < arDonut.GetSize(); i++)
	{
		T_DONUT* itr = &arDonut[i];
		if(itr->OutRadius == 0) continue;

		x1 = itr->CenterP.x - itr->OutRadius;
		y1 = itr->CenterP.y - itr->OutRadius;
		x2 = itr->CenterP.x + itr->OutRadius;
		y2 = itr->CenterP.y + itr->OutRadius;

		::Ellipse(DC, x1, y1, x2, y2);

		if(itr->InRadius == 0) continue;

		x1 = itr->CenterP.x - itr->InRadius;
		y1 = itr->CenterP.y - itr->InRadius;
		x2 = itr->CenterP.x + itr->InRadius;
		y2 = itr->CenterP.y + itr->InRadius;

		::Ellipse(DC, x1, y1, x2, y2);
	}

	::SelectObject(DC, pOldPen);
	::SelectObject(DC, pOldBrh);

	if(LinePen) ::DeleteObject(LinePen);
	if(FillBrush) ::DeleteObject(FillBrush);
}