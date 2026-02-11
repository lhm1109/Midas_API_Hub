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

// DrawArrow ////////////////////////////////////

// T_DRAW_FORMAT 이용하여 DC에 화살표를 그림
void CDrawFunc::DrawArrow(HDC DC, T_DRAW_FORMAT Format, int x1, int y1, int x2, int y2)
{ 
	if(Format.bLine) DrawArrow(DC, Format.nPenStyle, Format.lineColor, Format.Width, Format.dRWidth, Format.dRHeight, Format.startArrowType, Format.endArrowType, x1, y1, x2, y2, Format.nPenSubStyle); 
}

// T_DRAW_FORMAT 이용하여 DC에 화살표를 그림
void CDrawFunc::DrawArrow(HDC DC, T_DRAW_FORMAT Format, T_LINE Line)
{ 
	if(Format.bLine) DrawArrow(DC, Format.nPenStyle, Format.lineColor, Format.Width, Format.dRWidth, Format.dRHeight, Format.startArrowType, Format.endArrowType, Line, Format.nPenSubStyle); 
}

// T_DRAW_FORMAT 이용하여 DC에 화살표를 그림
void CDrawFunc::DrawArrow(HDC DC, T_DRAW_FORMAT Format, nrT_LINE& arLine)
{ 
	if(Format.bLine) DrawArrow(DC, Format.nPenStyle, Format.lineColor, Format.Width, Format.dRWidth, Format.dRHeight, Format.startArrowType, Format.endArrowType, arLine, Format.nPenSubStyle); 
}

void CDrawFunc::DrawArrow(HDC DC, int nPenStyle, COLORREF lineColor, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, int x1, int y1, int x2, int y2, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_LINE arLine;
	arLine.SetSize(1);
	T_LINE dgnLine;
	dgnLine.P1.x = x1;  dgnLine.P1.y = y1;
	dgnLine.P2.x = x2;  dgnLine.P2.y = y2;
	arLine[0] = dgnLine;
	DrawArrow(DC, nPenStyle, lineColor, Width, dRWidth, dRHeight, startArrowType, endArrowType, arLine, nPenSubStyle);
}

void CDrawFunc::DrawArrow(HDC DC, int nPenStyle, COLORREF lineColor, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, T_LINE dgnLine, int nPenSubStyle)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_LINE arLine;
	arLine.SetSize(1);
	arLine[0] = dgnLine;
	DrawArrow(DC, nPenStyle, lineColor, Width, dRWidth, dRHeight, startArrowType, endArrowType, arLine, nPenSubStyle);
}

void CDrawFunc::DrawArrow(HDC DC, int nPenStyle, COLORREF lineColor, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, nrT_LINE& arLine, int nPenSubStyle)
{
	nrT_LINE     arMainLine;
	nrT_CIRCLE   arCircle;
	nrT_POLYLINE arPolyline;
	nrT_POLYLINE arPolygon;
	int nCount = arLine.GetSize();
	arMainLine.SetSize(nCount);
	BOOL bCircle   = FALSE;
	BOOL bPolyline = FALSE;
	BOOL bPolygon  = FALSE;
	int nCountCircle   = 0;
	int nCountPolyline = 0;
	int nCountPolygon  = 0;

	if     (startArrowType == EN_ARROW_TYPE_ARROW)  { bPolygon = TRUE; nCountPolygon += nCount; }
	else if(startArrowType == EN_ARROW_TYPE_OPEN)   { bPolyline= TRUE; nCountPolyline+= nCount; }
	else if(startArrowType == EN_ARROW_TYPE_DIAMOND){ bPolygon = TRUE; nCountPolygon += nCount; }
	else if(startArrowType == EN_ARROW_TYPE_OVAL)   { bCircle  = TRUE; nCountCircle  += nCount; }

	if     (endArrowType   == EN_ARROW_TYPE_ARROW)  { bPolygon = TRUE; nCountPolygon += nCount; }
	else if(endArrowType   == EN_ARROW_TYPE_OPEN)   { bPolyline= TRUE; nCountPolyline+= nCount; }
	else if(endArrowType   == EN_ARROW_TYPE_DIAMOND){ bPolygon = TRUE; nCountPolygon += nCount; }
	else if(endArrowType   == EN_ARROW_TYPE_OVAL)   { bCircle  = TRUE; nCountCircle  += nCount; }

	arCircle.SetSize(nCountCircle);
	arPolyline.SetSize(nCountPolyline);
	arPolygon.SetSize(nCountPolygon);

	nCountCircle = nCountPolyline = nCountPolygon = 0; // 초기화

	Vertex vertex;

	T_LINE* itrMainLine = NULL;
	if(arMainLine.GetSize() > 0)
	{
		itrMainLine = &arMainLine[0];
	}
	
	int i;
	//for(auto itr = arLine.Data.begin(); itr != arLine.Data.end(); itr++, itrMainLine++) 
	for( i = 0 ; i < arLine.GetSize() ; i++)
	{
		T_LINE& dgnLine = arLine[i];
		
		double dLength = sqrt(pow((double)(dgnLine.P1.x - dgnLine.P2.x), 2.0) + pow((double)(dgnLine.P1.y - dgnLine.P2.y), 2.0));

		if(dLength <= 1.0e-10) 
		{ 
			vertex.uv[0] = vertex.uv[1] = 0.0;
		}
		else
		{
			vertex.uv[0] = (double)(dgnLine.P2.x - dgnLine.P1.x) / dLength;
			vertex.uv[1] = (double)(dgnLine.P2.y - dgnLine.P1.y) / dLength;
		}    

		// 주요선
		T_LINE dgnMainLine = dgnLine;
		if(startArrowType == EN_ARROW_TYPE_ARROW)
		{
			dgnMainLine.P1.x = dgnLine.P1.x + math_lib::round_off(((double)(Width) * dRWidth / 2.0) * vertex.uv[0]);
			dgnMainLine.P1.y = dgnLine.P1.y + math_lib::round_off(((double)(Width) * dRWidth / 2.0) * vertex.uv[1]);
		}																			    		   	      
		if(endArrowType == EN_ARROW_TYPE_ARROW)										  		   	      
		{																			    		   	      
			dgnMainLine.P2.x = dgnLine.P2.x - math_lib::round_off(((double)(Width) * dRWidth / 2.0) * vertex.uv[0]);
			dgnMainLine.P2.y = dgnLine.P2.y - math_lib::round_off(((double)(Width) * dRWidth / 2.0) * vertex.uv[1]);
		}
		
		*itrMainLine = dgnMainLine;

		// 시작화살표
		if(startArrowType == EN_ARROW_TYPE_ARROW || startArrowType == EN_ARROW_TYPE_OPEN)
		{
			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(3);      
			dgnPolyline.arPoint[0].x = dgnLine.P1.x + math_lib::round_off((double)(Width) * dRWidth * vertex.uv[0] - (double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[0].y = dgnLine.P1.y + math_lib::round_off((double)(Width) * dRWidth * vertex.uv[1] + (double)(Width) * dRHeight / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[1]   = dgnLine.P1;										  		      								   	 		   	 	   
			dgnPolyline.arPoint[2].x = dgnLine.P1.x + math_lib::round_off((double)(Width) * dRWidth * vertex.uv[0] + (double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[2].y = dgnLine.P1.y + math_lib::round_off((double)(Width) * dRWidth * vertex.uv[1] - (double)(Width) * dRHeight / 2.0 * vertex.uv[0]);

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
			dgnPolyline.arPoint[0].x = dgnLine.P1.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[0].y = dgnLine.P1.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[1].x = dgnLine.P1.x + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[1].y = dgnLine.P1.y - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[2].x = dgnLine.P1.x - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[2].y = dgnLine.P1.y - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[3].x = dgnLine.P1.x - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[3].y = dgnLine.P1.y + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[0]);

			arPolygon[nCountPolygon] = dgnPolyline;
			nCountPolygon++;
		}
		else if(startArrowType == EN_ARROW_TYPE_OVAL)   
		{
			T_CIRCLE dgnCircle;
			dgnCircle.CenterP = dgnLine.P1;
			dgnCircle.Radius  = math_lib::round_off((double)(Width)*dRHeight/2.0);

			arCircle[nCountCircle] = dgnCircle;
			nCountCircle++;
		}

		// 끝화살표
		if(endArrowType == EN_ARROW_TYPE_ARROW || endArrowType == EN_ARROW_TYPE_OPEN)
		{
			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(3);      
			dgnPolyline.arPoint[0].x = dgnLine.P2.x - math_lib::round_off((double)(Width) * dRWidth * vertex.uv[0] - (double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[0].y = dgnLine.P2.y - math_lib::round_off((double)(Width) * dRWidth * vertex.uv[1] + (double)(Width) * dRHeight / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[1]   = dgnLine.P2;										    	      								   	 		   	 	   
			dgnPolyline.arPoint[2].x = dgnLine.P2.x - math_lib::round_off((double)(Width) * dRWidth * vertex.uv[0] + (double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[2].y = dgnLine.P2.y - math_lib::round_off((double)(Width) * dRWidth * vertex.uv[1] - (double)(Width) * dRHeight / 2.0 * vertex.uv[0]);
		
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
			dgnPolyline.arPoint[0].x = dgnLine.P2.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[0].y = dgnLine.P2.x + math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[1].x = dgnLine.P2.x + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[1].y = dgnLine.P2.y - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[2].x = dgnLine.P2.x - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[0]);
			dgnPolyline.arPoint[2].y = dgnLine.P2.y - math_lib::round_off((double)(Width) * dRWidth / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[3].x = dgnLine.P2.x - math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[1]);
			dgnPolyline.arPoint[3].y = dgnLine.P2.y + math_lib::round_off((double)(Width) * dRHeight / 2.0 * vertex.uv[0]);

			arPolygon[nCountPolygon] = dgnPolyline;
			nCountPolygon++;
		}
		else if(endArrowType == EN_ARROW_TYPE_OVAL)   
		{
			T_CIRCLE dgnCircle;
			dgnCircle.CenterP = dgnLine.P2;
			dgnCircle.Radius = math_lib::round_off((double)(Width)*dRHeight/2.0);

			arCircle[nCountCircle] = dgnCircle;  
			nCountCircle++;
		}
	}

	// 선그리기
	DrawLine(DC, nPenStyle, lineColor, Width, arMainLine, nPenSubStyle);
	// 화살표 그리기
	if(bCircle)   DrawCircle(DC, PS_SOLID, lineColor, lineColor, TRUE, 0, arCircle);
	if(bPolygon)  DrawPolygon(DC, PS_SOLID, lineColor, lineColor, TRUE, 0, arPolygon);
	if(bPolyline) DrawPolyline(DC, PS_SOLID, lineColor, lineColor, TRUE, Width, arPolyline);
}