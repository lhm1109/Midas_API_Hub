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
// DrawArc

// T_DRAW_FORMAT 이용하여 DC에 호를 그림
void CDrawFunc::DrawArc(HDC DC, T_DRAW_FORMAT Format, T_ARC Arc)
{ 
	if(Format.bLine)
	{
		DrawArc(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, Format.dRWidth, Format.dRHeight, Format.startArrowType, Format.endArrowType, Arc, Format.nPenSubStyle); 
	}
	else if(Format.bFill)
	{
		DrawArc(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, Format.dRWidth, Format.dRHeight, Format.startArrowType, Format.endArrowType, Arc, Format.nPenSubStyle); 
	}
};

// T_DRAW_FORMAT 이용하여 DC에 호를 그림
void CDrawFunc::DrawArc(HDC DC, T_DRAW_FORMAT Format, nrT_ARC& arArc)
{ 
	if(Format.bLine) 
	{
		DrawArc(DC, Format.nPenStyle, Format.lineColor, Format.fillColor, Format.bFill, Format.Width, Format.dRWidth, Format.dRHeight, Format.startArrowType, Format.endArrowType, arArc, Format.nPenSubStyle); 
	}
	else if(Format.bFill)
	{
		DrawArc(DC, PS_SOLID, Format.fillColor, Format.fillColor, Format.bFill, 0, Format.dRWidth, Format.dRHeight, Format.startArrowType, Format.endArrowType, arArc, Format.nPenSubStyle); 
	}
};

void CDrawFunc::DrawArc(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, T_ARC dgnArc, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_ARC arArc;
	arArc.SetSize(1);
	arArc[0] = dgnArc;

	DrawArc(DC, nPenStyle, lineColor, fillColor, bFill, Width, dRWidth, dRHeight, startArrowType, endArrowType, arArc, nPenSubStyle);
}

void CDrawFunc::DrawArc(HDC DC, int nPenStyle, COLORREF lineColor, COLORREF fillColor, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, nrT_ARC& arArc, int nPenSubStyle)
{
	HGDIOBJ  pOldBrh;
	HPEN     LinePen;
	LOGBRUSH logBrush; 
	if(nPenSubStyle < 0 || nPenStyle != PS_SOLID)
	{
		LinePen = CreatePen(nPenStyle, Width, lineColor);
	}
	else 
	{
		logBrush.lbStyle = nPenStyle; 
		logBrush.lbColor = lineColor; 
		LinePen = ::ExtCreatePen(nPenSubStyle, Width, &logBrush, 0, NULL);
	} 
	HBRUSH FillBrush;
	FillBrush = CreateSolidBrush(fillColor);
	HGDIOBJ pOldPen = ::SelectObject(DC, LinePen);
	if(bFill) pOldBrh = ::SelectObject(DC, FillBrush);

	double dStartAngle;
	double dSweepAngle;
	POINT  StartP, EndP;
	Vertex vertex_S, vertex_E;   
	double dStartP[2], dEndP[2];
	int    nPointCount;
	nrT_POINT arArcPoint;

	int i;
	//for(auto itr = arArc.Data.begin(); itr != arArc.Data.end(); itr++) 
	for( i = 0 ; i < arArc.GetSize() ; i++)
	{    
		T_ARC& dgnArc = arArc[i];

		if(dgnArc.Radius == 0 || fabs(dgnArc.SweepAngle) < m_Zero) continue;

		dStartAngle = dgnArc.StartAngle;
		dSweepAngle = dgnArc.SweepAngle;

		double dStartAngle_rad = math_lib::radian(dgnArc.StartAngle);    

		vertex_S.uv[0] = cos(dStartAngle_rad);
		vertex_S.uv[1] = sin(dStartAngle_rad); 
		StartP.x = dgnArc.CenterP.x + math_lib::round_off(dgnArc.Radius * vertex_S.uv[0]);
		StartP.y = dgnArc.CenterP.y - math_lib::round_off(dgnArc.Radius * vertex_S.uv[1]) * (long)(m_bEMF ? -1.0 : 1.0);

		if(bFill)
		{
			// HDC::AngleArc는 채우기가 되지 않으므로 Polygon으로 채워줌
			nPointCount = max(3, (int)(dSweepAngle));			
			double dEndAngle_rad = math_lib::radian(dgnArc.StartAngle+dgnArc.SweepAngle);
			vertex_E.uv[0] = cos(dEndAngle_rad);
			vertex_E.uv[1] = sin(dEndAngle_rad);

			dStartP[0] = (dgnArc.Radius * vertex_S.uv[0]);
			dStartP[1] = (dgnArc.Radius * vertex_S.uv[1]);
			dEndP[0]   = (dgnArc.Radius * vertex_E.uv[0]);
			dEndP[1]   = (dgnArc.Radius * vertex_E.uv[1]);

			if(GetArcPolyline_Radius(dStartP, dEndP, (double)(dgnArc.Radius)*(dSweepAngle<0 ? -1.0 : 1.0), (dSweepAngle<180.0), nPointCount, TRUE, arArcPoint))
			{
				nPointCount = arArcPoint.GetSize();

				if(nPointCount > 2)
				{
					T_POLYLINE polygonD;
					polygonD.arPoint.SetSize(nPointCount);

					for(int j = 0; j < nPointCount; j++)
					{
						polygonD.arPoint[j].x = dgnArc.CenterP.x + math_lib::round_off(arArcPoint[j].x);						
						polygonD.arPoint[j].y = dgnArc.CenterP.y + math_lib::round_off(arArcPoint[j].y) * (long)(m_bEMF ? 1.0 : -1.0);
					}
					DrawPolygon(DC, PS_SOLID, fillColor, fillColor, TRUE, 0, polygonD);
				}
			}			
		}

		::MoveToEx(DC, StartP.x, StartP.y, NULL);        
		if(dgnArc.Radius >= 0)
		{
			::AngleArc(DC, dgnArc.CenterP.x,  dgnArc.CenterP.y, dgnArc.Radius, (float)(dStartAngle*(m_bEMF ? -1.0 : 1.0)), (float)(dSweepAngle*(m_bEMF ? -1.0 : 1.0)));    
		}
		else
		{
			dStartAngle = dStartAngle+180.0;
			if(dStartAngle > 360 ) dStartAngle -= 360.0;
			::AngleArc(DC, dgnArc.CenterP.x,  dgnArc.CenterP.y,  (int)(abs(dgnArc.Radius)),  (float)(dStartAngle*(m_bEMF ? -1.0 : 1.0)),  (float)(dSweepAngle*(m_bEMF ? -1.0 : 1.0)));    
		}
	}

	::SelectObject(DC, pOldPen);
	if(bFill) ::SelectObject(DC, pOldBrh);

	if(LinePen) ::DeleteObject(LinePen);
	if(FillBrush) ::DeleteObject(FillBrush);

	// 화살표 그리기  
	nrT_CIRCLE   arCircle;
	nrT_POLYLINE arPolyline;
	nrT_POLYLINE arPolygon;

	BOOL bCircle   = FALSE;
	BOOL bPolyline = FALSE;
	BOOL bPolygon  = FALSE;
	int nCountCircle   = 0;
	int nCountPolyline = 0;
	int nCountPolygon  = 0;  
	int nSize = arArc.GetSize();

	if     (startArrowType == EN_ARROW_TYPE_ARROW)  {bPolygon = TRUE; nCountPolygon += nSize;}
	else if(startArrowType == EN_ARROW_TYPE_OPEN)   {bPolyline= TRUE; nCountPolyline+= nSize;}
	else if(startArrowType == EN_ARROW_TYPE_DIAMOND){bPolygon = TRUE; nCountPolygon += nSize;}
	else if(startArrowType == EN_ARROW_TYPE_OVAL)   {bCircle  = TRUE; nCountCircle  += nSize;}

	if     (endArrowType   == EN_ARROW_TYPE_ARROW)  {bPolygon = TRUE; nCountPolygon += nSize;}
	else if(endArrowType   == EN_ARROW_TYPE_OPEN)   {bPolyline= TRUE; nCountPolyline+= nSize;}
	else if(endArrowType   == EN_ARROW_TYPE_DIAMOND){bPolygon = TRUE; nCountPolygon += nSize;}
	else if(endArrowType   == EN_ARROW_TYPE_OVAL)   {bCircle  = TRUE; nCountCircle  += nSize;}

	arCircle.SetSize(nCountCircle);
	arPolyline.SetSize(nCountPolyline);
	arPolygon.SetSize(nCountPolygon);

	nCountCircle = nCountPolyline = nCountPolygon = 0; // 초기화

	//for(auto itr = arArc.Data.begin(); itr != arArc.Data.end(); itr++) 
	for( i = 0 ; i < arArc.GetSize() ; i++)
	{
		T_ARC& dgnArc = arArc[i];

		double dStartAngle_rad = math_lib::radian(dgnArc.StartAngle);
		double dEndAngle_rad   = math_lib::radian(dgnArc.StartAngle + dgnArc.SweepAngle);

		vertex_S.uv[0] = cos(dStartAngle_rad);
		vertex_S.uv[1] = sin(dStartAngle_rad);    
		vertex_E.uv[0] = cos(dEndAngle_rad);
		vertex_E.uv[1] = sin(dEndAngle_rad);

		StartP.x = dgnArc.CenterP.x + math_lib::round_off(dgnArc.Radius * vertex_S.uv[0]);
		StartP.y = dgnArc.CenterP.y - math_lib::round_off(dgnArc.Radius * vertex_S.uv[1]) * (long)(m_bEMF ? -1.0 : 1.0);
		EndP.x   = dgnArc.CenterP.x + math_lib::round_off(dgnArc.Radius * vertex_E.uv[0]);
		EndP.y   = dgnArc.CenterP.y - math_lib::round_off(dgnArc.Radius * vertex_E.uv[1]) * (long)(m_bEMF ? -1.0 : 1.0);

		if(dgnArc.SweepAngle * dgnArc.Radius >= 0)
		{
			vertex_S.uv[0] = -sin(dStartAngle_rad);
			vertex_S.uv[1] = -cos(dStartAngle_rad) * (m_bEMF ? -1.0 : 1.0);    
			vertex_E.uv[0] = -sin(dEndAngle_rad);
			vertex_E.uv[1] = -cos(dEndAngle_rad) * (m_bEMF ? -1.0 : 1.0);
		}
		else
		{
			vertex_S.uv[0] = sin(dStartAngle_rad);
			vertex_S.uv[1] = cos(dStartAngle_rad) * (m_bEMF ? -1.0 : 1.0);    
			vertex_E.uv[0] = sin(dEndAngle_rad);
			vertex_E.uv[1] = cos(dEndAngle_rad) * (m_bEMF ? -1.0 : 1.0);
		}

		// 시작화살표
		if(startArrowType == EN_ARROW_TYPE_ARROW || startArrowType == EN_ARROW_TYPE_OPEN)
		{
			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(3);      
			dgnPolyline.arPoint[0].x = StartP.x + math_lib::round_off((double)(Width) * dRWidth * vertex_S.uv[0] - (double)(Width) * dRHeight / 2.0 * vertex_S.uv[1]);
			dgnPolyline.arPoint[0].y = StartP.y + math_lib::round_off((double)(Width) * dRWidth * vertex_S.uv[1] + (double)(Width) * dRHeight / 2.0 * vertex_S.uv[0]);
			dgnPolyline.arPoint[1]   = StartP;
			dgnPolyline.arPoint[2].x = StartP.x + math_lib::round_off((double)(Width) * dRWidth * vertex_S.uv[0] + (double)(Width) * dRHeight / 2.0 * vertex_S.uv[1]);
			dgnPolyline.arPoint[2].y = StartP.y + math_lib::round_off((double)(Width) * dRWidth * vertex_S.uv[1] - (double)(Width) * dRHeight / 2.0 * vertex_S.uv[0]);

			if(startArrowType == EN_ARROW_TYPE_ARROW)
			{ 
				arPolygon[nCountPolygon] = dgnPolyline;
				nCountPolygon++; 
			}
			else
			{ 
				arPolyline[nCountPolyline] = dgnPolyline;
				nCountPolyline++; 
			}
		}
		else if(startArrowType == EN_ARROW_TYPE_DIAMOND)
		{
			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(4);
			dgnPolyline.arPoint[0].x = StartP.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_S.uv[0]);
			dgnPolyline.arPoint[0].y = StartP.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_S.uv[1]);
			dgnPolyline.arPoint[1].x = StartP.x + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_S.uv[1]);
			dgnPolyline.arPoint[1].y = StartP.y - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_S.uv[0]);
			dgnPolyline.arPoint[2].x = StartP.x - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_S.uv[0]);
			dgnPolyline.arPoint[2].y = StartP.y - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_S.uv[1]);
			dgnPolyline.arPoint[3].x = StartP.x - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_S.uv[1]);
			dgnPolyline.arPoint[3].y = StartP.y + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_S.uv[0]);

			arPolygon[nCountPolygon] = dgnPolyline;
			nCountPolygon++;
		}
		else if(startArrowType == EN_ARROW_TYPE_OVAL)   
		{
			T_CIRCLE dgnCircle;
			dgnCircle.CenterP = StartP;
			dgnCircle.Radius  = math_lib::round_off((double)(Width)*dRHeight/2.0);

			arCircle[nCountCircle] = dgnCircle;
			nCountCircle++;
		}

		// 끝화살표
		if(endArrowType == EN_ARROW_TYPE_ARROW || endArrowType == EN_ARROW_TYPE_OPEN)
		{
			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(3);      
			dgnPolyline.arPoint[0].x = EndP.x - math_lib::round_off((double)(Width)*dRWidth * vertex_E.uv[0] - (double)(Width) * dRHeight / 2.0 * vertex_E.uv[1]);
			dgnPolyline.arPoint[0].y = EndP.y - math_lib::round_off((double)(Width)*dRWidth * vertex_E.uv[1] + (double)(Width) * dRHeight / 2.0 * vertex_E.uv[0]);
			dgnPolyline.arPoint[1]   = EndP;												  	  									   
			dgnPolyline.arPoint[2].x = EndP.x - math_lib::round_off((double)(Width)*dRWidth * vertex_E.uv[0] + (double)(Width) * dRHeight / 2.0 * vertex_E.uv[1]);
			dgnPolyline.arPoint[2].y = EndP.y - math_lib::round_off((double)(Width)*dRWidth * vertex_E.uv[1] - (double)(Width) * dRHeight / 2.0 * vertex_E.uv[0]);

			if(endArrowType == EN_ARROW_TYPE_ARROW)
			{ 
				arPolygon[nCountPolygon] = dgnPolyline;   
				nCountPolygon++; 
			}
			else
			{ 
				arPolyline[nCountPolyline] = dgnPolyline; 
				nCountPolyline++; 
			}
		}
		else if(endArrowType == EN_ARROW_TYPE_DIAMOND)
		{
			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(4);
			dgnPolyline.arPoint[0].x = EndP.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_E.uv[0]);
			dgnPolyline.arPoint[0].y = EndP.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_E.uv[1]);
			dgnPolyline.arPoint[1].x = EndP.x + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_E.uv[1]);
			dgnPolyline.arPoint[1].y = EndP.y - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_E.uv[0]);
			dgnPolyline.arPoint[2].x = EndP.x - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_E.uv[0]);
			dgnPolyline.arPoint[2].y = EndP.y - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex_E.uv[1]);
			dgnPolyline.arPoint[3].x = EndP.x - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_E.uv[1]);
			dgnPolyline.arPoint[3].y = EndP.y + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex_E.uv[0]);

			arPolygon[nCountPolygon] = dgnPolyline; 
			nCountPolygon++;
		}
		else if(endArrowType == EN_ARROW_TYPE_OVAL)   
		{
			T_CIRCLE dgnCircle;
			dgnCircle.CenterP = EndP;
			dgnCircle.Radius = math_lib::round_off((double)(Width) * dRHeight / 2.0);

			arCircle[nCountCircle] = dgnCircle;
			nCountCircle++;
		}
	}

	//화살표 그리기
	if(bCircle)   DrawCircle(DC, PS_SOLID, lineColor, lineColor, TRUE, 0, arCircle);
	if(bPolygon)  DrawPolygon(DC, PS_SOLID, lineColor, lineColor, TRUE, 0, arPolygon);
	if(bPolyline) DrawPolyline(DC, PS_SOLID, lineColor, lineColor, TRUE, Width, arPolyline);
}
