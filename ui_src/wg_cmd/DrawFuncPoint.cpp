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

// DrawPoint ////////////////////////////////////

// T_DRAW_FORMAT 이용하여 DC에 Point을 그림
void CDrawFunc::DrawPoint(HDC DC, T_DRAW_FORMAT Format, int x, int y)
{ 
	DrawPoint(DC, Format.lineColor, Format.Width, x, y, Format.pointType, Format.bFill, Format.fillColor);
};
// T_DRAW_FORMAT 이용하여 DC에 Point을 그림
void CDrawFunc::DrawPoint(HDC DC, T_DRAW_FORMAT Format, POINT Point)
{ 
	DrawPoint(DC, Format.lineColor, Format.Width, Point, Format.pointType, Format.bFill, Format.fillColor);
};
// T_DRAW_FORMAT 이용하여 DC에 Point을 그림
void CDrawFunc::DrawPoint(HDC DC, T_DRAW_FORMAT Format, nrPOINT& arPoint)
{ 
	DrawPoint(DC, Format.lineColor, Format.Width, arPoint, Format.pointType, Format.bFill, Format.fillColor); 
};

void CDrawFunc::DrawPoint(HDC DC, COLORREF PointColor, int Width, int x, int y, EN_POINT_TYPE nType, BOOL bFill, COLORREF fillColor)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrPOINT arPoint;
	arPoint.SetSize(1);
	POINT dgnPoint;
	dgnPoint.x = x;  dgnPoint.y = y;
	arPoint[0] = dgnPoint;

	DrawPoint(DC, PointColor, Width, arPoint, nType, bFill, fillColor);
}

void CDrawFunc::DrawPoint(HDC DC, COLORREF PointColor, int Width, POINT dgnPoint, EN_POINT_TYPE nType, BOOL bFill, COLORREF fillColor)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrPOINT arPoint;
	arPoint.SetSize(1);
	arPoint[0] = dgnPoint;
	DrawPoint(DC, PointColor, Width, arPoint, nType, bFill, fillColor);
}

void CDrawFunc::DrawPoint(HDC DC, COLORREF PointColor, int Width, nrPOINT& arPoint, EN_POINT_TYPE nType, BOOL bFill, COLORREF fillColor)
{
	int i ;

	if(Width == 0) return;
	if(Width == 1)
	{
		//for(auto itr = arPoint.Data.begin(); itr != arPoint.Data.end(); itr++)
		for( i = 0 ; i < arPoint.GetSize(); i++)

		{
			::SetPixel(DC, arPoint[i].x, arPoint[i].y, PointColor);
		}
	}
	if(Width == 2)
	{
		POINT currPoint;
		
		//for(auto itr = arPoint.Data.begin(); itr != arPoint.Data.end(); itr++)
		for(i = 0 ; i < arPoint.GetSize(); i++)
		{
			currPoint = arPoint[i];

			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.x++;           
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.y++;           
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.x--;           
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
		}
	}
	if(Width == 3)
	{
		POINT currPoint;
	
		//for(auto itr = arPoint.Data.begin(); itr != arPoint.Data.end(); itr++)
		for(i = 0 ; i < arPoint.GetSize(); i++)
		{
			currPoint = arPoint[i]; 

			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.x++;           
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.y++;          
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.x--;          
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.x--;          
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.y--;          
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.y--;          
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.x++;          
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);
			currPoint.x++;          
			::SetPixel(DC, currPoint.x, currPoint.y, PointColor);			
		}
	}
	else
	{
		if(nType == EN_POINT_TYPE_DOT)
		{
			HPEN    LinePen; 
			HBRUSH  FillBrush;	

			LinePen = CreatePen(PS_SOLID, 1, PointColor);
			FillBrush = CreateSolidBrush(PointColor);

			HGDIOBJ pOldPen = ::SelectObject(DC, LinePen);
			HGDIOBJ pOldBrh = ::SelectObject(DC, FillBrush);

			int dr = (Width) / 2;
			//for(auto itr = arPoint.Data.begin(); itr != arPoint.Data.end(); itr++)
			for( i = 0 ; i < arPoint.GetSize(); i++)
			{    
				int x1 = arPoint[i].x - dr;
				int y1 = arPoint[i].y - dr;
				int x2 = x1 + Width;
				int y2 = y1 + Width;
				::Ellipse(DC, x1, y1, x2, y2);
			}		

			::SelectObject(DC, pOldPen);
			::SelectObject(DC, pOldBrh);

			if(LinePen)   ::DeleteObject(LinePen);
			if(FillBrush) ::DeleteObject(FillBrush);
		}
		else if(nType != EN_POINT_TYPE_NONE)
		{			
			BOOL bLineMark = (nType == EN_POINT_TYPE_CROSS || nType == EN_POINT_TYPE_STAR || nType == EN_POINT_TYPE_PLUS || nType == EN_POINT_TYPE_MINUS ||
				nType == EN_POINT_TYPE_CROSS_B || nType == EN_POINT_TYPE_STAR_B || nType == EN_POINT_TYPE_PLUS_B || nType == EN_POINT_TYPE_MINUS_B);

			int nLineWidth = 1;
			if(nType == EN_POINT_TYPE_RECT_B || nType == EN_POINT_TYPE_DIAMOND_B || nType == EN_POINT_TYPE_TRIANGLE_B || nType == EN_POINT_TYPE_CIRCLE_B || 
				nType == EN_POINT_TYPE_CROSS_B || nType == EN_POINT_TYPE_STAR_B || nType == EN_POINT_TYPE_PLUS_B || nType == EN_POINT_TYPE_MINUS_B)
			{
				nLineWidth = max(1, Width / 4);
			}

			int nSize = arPoint.GetSize();  			
			int dr = Width / 2;

			for(int i = 0; i < nSize; i++)
			{    
				if(nType == EN_POINT_TYPE_RECT || nType == EN_POINT_TYPE_RECT_B || (bFill && bLineMark))
				{					
					int x1 = arPoint[i].x - dr;
					int y1 = arPoint[i].y - dr;
					int x2 = x1 + Width;
					int y2 = y1 + Width;
					DrawRect(DC, PS_SOLID, (bLineMark ? fillColor : PointColor), fillColor, bFill, (bLineMark ? 0 : nLineWidth), x1, y1, x2, y2);  
				}
				else if(nType == EN_POINT_TYPE_RECT32)
				{					
					int x1 = arPoint[i].x - dr;
					int y1 = arPoint[i].y - (int)(Width/3.0);
					int x2 = x1 + Width;
					int y2 = y1 + (int)(Width/3.0*2.0);
					DrawRect(DC, PS_SOLID, PointColor, fillColor, bFill, nLineWidth, x1, y1, x2, y2);  
				}
				else if(nType == EN_POINT_TYPE_DIAMOND || nType == EN_POINT_TYPE_DIAMOND_B)
				{
					T_POLYLINE dgnPoly;
					dgnPoly.arPoint.SetSize(4);
					dgnPoly.arPoint[0].x = arPoint[i].x;      
					dgnPoly.arPoint[0].y = arPoint[i].y - dr;
					dgnPoly.arPoint[1].x = arPoint[i].x - dr;  
					dgnPoly.arPoint[1].y = arPoint[i].y;
					dgnPoly.arPoint[2].x = arPoint[i].x;      
					dgnPoly.arPoint[2].y = arPoint[i].y + dr;
					dgnPoly.arPoint[3].x = arPoint[i].x + dr; 
					dgnPoly.arPoint[3].y = arPoint[i].y;
					DrawPolygon(DC, PS_SOLID, PointColor, fillColor, bFill, nLineWidth, dgnPoly, PS_GEOMETRIC|PS_ENDCAP_FLAT|PS_JOIN_ROUND);						
				}
				else if(nType == EN_POINT_TYPE_TRIANGLE || nType == EN_POINT_TYPE_TRIANGLE_B)
				{
					T_POLYLINE dgnPoly;
					dgnPoly.arPoint.SetSize(3);
					dgnPoly.arPoint[0].x = arPoint[i].x;      
					dgnPoly.arPoint[0].y = arPoint[i].y + (int)(Width * sqrt(3.0) / 3.0) * (m_bEMF ?  1 : -1);
					dgnPoly.arPoint[1].x = arPoint[i].x - dr;  
					dgnPoly.arPoint[1].y = arPoint[i].y + (int)(Width * sqrt(3.0) / 6.0) * (m_bEMF ? -1 :  1);
					dgnPoly.arPoint[2].x = arPoint[i].x + dr; 
					dgnPoly.arPoint[2].y = dgnPoly.arPoint[1].y;
					DrawPolygon(DC, PS_SOLID, PointColor, fillColor, bFill, nLineWidth, dgnPoly, PS_GEOMETRIC|PS_ENDCAP_FLAT|PS_JOIN_ROUND);		
				}
				else if(nType == EN_POINT_TYPE_CIRCLE || nType == EN_POINT_TYPE_CIRCLE_B)
				{
					T_CIRCLE dgnCircle;
					dgnCircle.CenterP = arPoint[i];
					dgnCircle.Radius  = dr;
					DrawCircle(DC, PS_SOLID, PointColor, fillColor, bFill, nLineWidth, dgnCircle);  
				}

				if(bLineMark)
				{				
					nrT_LINE arLine;
					T_LINE dgnLine;
					if(nType == EN_POINT_TYPE_CROSS || nType == EN_POINT_TYPE_STAR || nType == EN_POINT_TYPE_CROSS_B || nType == EN_POINT_TYPE_STAR_B)
					{
						// 'x'
						dgnLine.P1.x = arPoint[i].x - dr;
						dgnLine.P1.y = arPoint[i].y - dr;
						dgnLine.P2.x = arPoint[i].x + dr;
						dgnLine.P2.y = arPoint[i].y + dr;
						arLine.Add(dgnLine);
						dgnLine.P1.x = arPoint[i].x + dr; 
						dgnLine.P1.y = arPoint[i].y - dr;
						dgnLine.P2.x = arPoint[i].x - dr;
						dgnLine.P2.y = arPoint[i].y + dr;
						arLine.Add(dgnLine);
					}
					if(nType == EN_POINT_TYPE_STAR || nType == EN_POINT_TYPE_PLUS || nType == EN_POINT_TYPE_STAR_B || nType == EN_POINT_TYPE_PLUS_B)
					{
						// _T("|")						
						dgnLine.P1.x = arPoint[i].x; 
						dgnLine.P1.y = arPoint[i].y - dr;
						dgnLine.P2.x = arPoint[i].x;
						dgnLine.P2.y = arPoint[i].y + dr;
						arLine.Add(dgnLine);
					}
					if(nType == EN_POINT_TYPE_PLUS || nType == EN_POINT_TYPE_MINUS || nType == EN_POINT_TYPE_PLUS_B || nType == EN_POINT_TYPE_MINUS_B)
					{
						// _T("-")						
						dgnLine.P1.x = arPoint[i].x - dr; 
						dgnLine.P1.y = arPoint[i].y;
						dgnLine.P2.x = arPoint[i].x + dr; 
						dgnLine.P2.y = arPoint[i].y;
						arLine.Add(dgnLine);
					}

					if(arLine.GetSize() > 0) DrawLine(DC, PS_SOLID, PointColor, nLineWidth, arLine, PS_GEOMETRIC|PS_ENDCAP_ROUND);
					else ASSERT(0);
				}	
			}
		}
	}
}