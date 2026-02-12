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

// DrawDimAngle ////////////////////////////////////

// T_DIM_FORMAT을 이용하여 DC에 Angle치수선을 그림
void CDrawFunc::DrawDimAngle(HDC DC, T_DIM_FORMAT dimFormat, T_ANGLE dgnAngle)
{
	DrawDimAngle(DC, dimFormat.fontColor, dimFormat.lineColor, dimFormat.fontSize, dimFormat.szFont.GetBuffer(), dimFormat.Width, dimFormat.dRWidth, dimFormat.dRHeight, dimFormat.arrowType, 
		dimFormat.dRTextSpace, dimFormat.dRDimSpace, dimFormat.dRGuideIn, dimFormat.dRGuideOut, dimFormat.dimType, dimFormat.bSubDimType, dimFormat.dimType_Sub, dgnAngle, dimFormat.bFontScale, dimFormat.bMainLine, dimFormat.bGuideLine_P1, dimFormat.bGuideLine_P2, dimFormat.bFontHalfWidth);
}

// T_DIM_FORMAT을 이용하여 DC에 Angle치수선을 그림
void CDrawFunc::DrawDimAngle(HDC DC, T_DIM_FORMAT dimFormat, nrT_ANGLE& arAngle)
{
	DrawDimAngle(DC, dimFormat.fontColor, dimFormat.lineColor, dimFormat.fontSize, dimFormat.szFont.GetBuffer(), dimFormat.Width, dimFormat.dRWidth, dimFormat.dRHeight, dimFormat.arrowType, 
		dimFormat.dRTextSpace, dimFormat.dRDimSpace, dimFormat.dRGuideIn, dimFormat.dRGuideOut, dimFormat.dimType, dimFormat.bSubDimType, dimFormat.dimType_Sub, arAngle, dimFormat.bFontScale, dimFormat.bMainLine, dimFormat.bGuideLine_P1, dimFormat.bGuideLine_P2, dimFormat.bFontHalfWidth);
}

void CDrawFunc::DrawDimAngle(HDC DC, COLORREF fontColor, COLORREF lineColor, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, T_ANGLE dgnAngle, BOOL bFontScale, BOOL bMainLine, BOOL bGuideLine_P1, BOOL bGuideLine_P2, BOOL bFontHalfWidth)
{
	nrT_ANGLE arDimAngle;
	arDimAngle.SetSize(1);
	arDimAngle[0] = dgnAngle;
	DrawDimAngle(DC, fontColor, lineColor, fontSize, szFont, Width, dRWidth, dRHeight, arrowType, dRTextSpace, dRDimSpace, dRGuideIn, dRGuideOut, dimType, bSubDimType, dimType_Sub, arDimAngle, bFontScale, bMainLine, bGuideLine_P1, bGuideLine_P2, bFontHalfWidth);  
}

void CDrawFunc::DrawDimAngle(HDC DC, COLORREF fontColor, COLORREF lineColor, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, nrT_ANGLE& arAngle, BOOL bFontScale, BOOL bMainLine, BOOL bGuideLine_P1, BOOL bGuideLine_P2, BOOL bFontHalfWidth)
{
	if(dimType == EN_DIM_SUB_TYPE_TOP_OUT_LEFT    || dimType == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT   ) dimType = EN_DIM_TYPE_TOP;
	if(dimType == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT || dimType == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT) dimType = EN_DIM_TYPE_CENTER;
	if(dimType == EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT || dimType == EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT) dimType = EN_DIM_TYPE_BOTTOM;
	if(dimType_Sub == EN_DIM_SUB_TYPE_TOP_OUT_LEFT    || dimType_Sub == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT   ) dimType_Sub = EN_DIM_TYPE_TOP;
	if(dimType_Sub == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT || dimType_Sub == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT) dimType_Sub = EN_DIM_TYPE_CENTER;
	if(dimType_Sub == EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT || dimType_Sub == EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT) dimType_Sub = EN_DIM_TYPE_BOTTOM;

	// Font는 문자열의 크기를 구하기 위한 용도로만 사용됨
	HFONT Font;
	Font = ::CreateFont(fontSize, (bFontHalfWidth ? fontSize/2 : 0), 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, szFont);
	HGDIOBJ pOldFont = ::SelectObject(DC, Font);

	double dUnitSize = 1;
	if(bFontScale) dUnitSize = (double)(fontSize);
	int i, nCount, nArcCount, nHalfArcCount, nGuideCount, nPolylineCount, nSubTypCount, nZeroCount;
	nCount = arAngle.GetSize();

	double dEMF = m_bEMF ? -1.0 : 1.0;

	CArray<SIZE, SIZE>   arTextSize;   
	arTextSize.SetSize(nCount);
	CArray<double, double> arSweepAngle; 
	arSweepAngle.SetSize(nCount);  
	CArray<double, double> arTextAngle;  
	arTextAngle.SetSize(nCount);  
	nSubTypCount = 0; nZeroCount = 0;
	double dRadius = dUnitSize * (dRDimSpace + dRGuideIn);//각도치수선의 반지름
	SIZE textSize;

	for(i = 0; i < nCount; i++) 
	{
		T_ANGLE& dgnAngle = arAngle[i];
		
		textSize.cx = 0;
		textSize.cy = 0;

		if(dgnAngle.szText != _T("")) 
		{
			// m_bEMF이면 m_hAttribDC 를 넣어주어야 한다. 
			::GetTextExtentPoint32(DC, dgnAngle.szText, dgnAngle.szText.GetLength(), &textSize);
		}

		textSize.cx = textSize.cx + fontSize / 2;
		double dSweepAngle = (double)dgnAngle.SweepAngle;    
		double dB, dH;
		dH = (double)(textSize.cy) / 2.0;
		dB = max(m_Zero, sqrt(max(0.0, dRadius * dRadius - dH * dH)));
		double dTextAngle = 2.0 * math_lib::degree(atan(dH / dB));    
		arTextSize[i] = textSize;
		arSweepAngle[i] = dSweepAngle;    
		arTextAngle[i] = dTextAngle;

		if(fabs(dSweepAngle) < m_Zero) 
		{
			nZeroCount++; 
			continue;
		}

		if(bSubDimType && dimType != EN_DIM_TYPE_OUT_START && dimType != EN_DIM_TYPE_OUT_END && fabs(dTextAngle) > fabs(dSweepAngle)) 
		{
			nSubTypCount++;    
		}
	}

	int nGuideNum = 0; 
	if(bGuideLine_P1) ++nGuideNum;
	if(bGuideLine_P2) ++nGuideNum;

	nGuideCount = nGuideNum * (nCount - nZeroCount);
	nArcCount       = 0;
	nHalfArcCount   = 0;
	nPolylineCount  = 0;

	if(dimType == EN_DIM_TYPE_CENTER) nHalfArcCount = 2 * (nCount - nZeroCount - nSubTypCount);  
	else                               nArcCount = nCount - nZeroCount - nSubTypCount;  

	if(dimType == EN_DIM_TYPE_OUT_LEFT || dimType == EN_DIM_TYPE_OUT_RIGHT) nPolylineCount = nCount - nZeroCount - nSubTypCount;

	if(dimType_Sub == EN_DIM_TYPE_CENTER) nHalfArcCount = nHalfArcCount + 2 * nSubTypCount;  
	else                                    nArcCount = nArcCount + nSubTypCount; 

	if(dimType_Sub == EN_DIM_TYPE_OUT_LEFT || dimType_Sub == EN_DIM_TYPE_OUT_RIGHT) nPolylineCount = nPolylineCount + nSubTypCount;

	nrT_LINE arLine;
	nrT_ARC arArc, arHalfArc;
	nrT_POLYLINE arPolyline;
	arLine.SetSize(nGuideCount);
	arArc.SetSize(nArcCount);
	arHalfArc.SetSize(nHalfArcCount);
	arPolyline.SetSize(nPolylineCount);

	nArcCount = nHalfArcCount = nGuideCount = nPolylineCount = 0; //초기화

	POINT CP, PGS1, PGS2, PGS3, PGE1, PGE2, PGE3, PText, PS1, PS2, PS3;

	//                ●PGE3
	//               ／                   ●PText
	//              ／
	//         PGE2●          PS2 ●──────● PS3
	//            ／ ↖         _／
	//           ／    ○    _／
	//          ／         ／
	//         ／    PS1 ●         
	//   PGE1 ●             
	//                     ○  HalfArcθ=SubAngle
	//                     ↓
	//   CP●    ●────●──●  
	//           PGS1     PGS2  PGS3

	double dB, dH;
	dH = (double)Width * dRWidth / 2.0;
	dB = max(m_Zero, sqrt(max(0.0, dRadius*dRadius - dH*dH)));
	double dArrowSpaceAngle  = 2.0 * math_lib::degree( atan(dH/dB) );    
	EN_DIM_TYPE dimType_Use;

	for(i = 0; i < nCount; i++) 
	{
		T_ANGLE& dgnAngle = arAngle[i];
		SIZE&    textSize = arTextSize[i];    

		// 단위 벡터 생성
		Vertex vertex_S, vertex_C, vertex_E;
		double dSweepAngle = arSweepAngle[i];
		double dTextAngle  = arTextAngle[i];

		if(fabs(dSweepAngle) < m_Zero) continue;

		double dStartAngle_rad  = math_lib::radian(dgnAngle.StartAngle);
		double dEndAngle_rad    = math_lib::radian(dgnAngle.StartAngle + dgnAngle.SweepAngle);
		double dCenterAngle_rad = math_lib::radian(dgnAngle.StartAngle + dgnAngle.SweepAngle / 2.0);
		vertex_S.uv[0] = cos(dStartAngle_rad);
		vertex_S.uv[1] = sin(dStartAngle_rad);
		vertex_C.uv[0] = cos(dCenterAngle_rad);
		vertex_C.uv[1] = sin(dCenterAngle_rad);
		vertex_E.uv[0] = cos(dEndAngle_rad);
		vertex_E.uv[1] = sin(dEndAngle_rad);

		if(bSubDimType && dimType != EN_DIM_TYPE_OUT_START && dimType != EN_DIM_TYPE_OUT_END && fabs(dTextAngle) > fabs(dSweepAngle)) 
		{
			dimType_Use = dimType_Sub;
		}
		else
		{
			dimType_Use = dimType;
		}

		CP = dgnAngle.CenterP;

		PGS1.x = CP.x + math_lib::round_off(dUnitSize * dRDimSpace * vertex_S.uv[0]);
		PGS1.y = CP.y - math_lib::round_off(dUnitSize * dRDimSpace * vertex_S.uv[1]) * (long)dEMF;
		PGS2.x = CP.x + math_lib::round_off(dRadius * vertex_S.uv[0]);
		PGS2.y = CP.y - math_lib::round_off(dRadius * vertex_S.uv[1]) * (long)dEMF;
		PGS3.x = CP.x + math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex_S.uv[0]);
		PGS3.y = CP.y - math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex_S.uv[1]) * (long)dEMF;
		PGE1.x = CP.x + math_lib::round_off(dUnitSize * dRDimSpace * vertex_E.uv[0]);
		PGE1.y = CP.y - math_lib::round_off(dUnitSize * dRDimSpace * vertex_E.uv[1]) * (long)dEMF;
		PGE2.x = CP.x + math_lib::round_off(dRadius * vertex_E.uv[0]);
		PGE2.y = CP.y - math_lib::round_off(dRadius * vertex_E.uv[1]) * (long)dEMF;
		PGE3.x = CP.x + math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex_E.uv[0]);
		PGE3.y = CP.y - math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex_E.uv[1]) * (long)dEMF;

		double dSubAngle = (fabs(dSweepAngle) - fabs(dTextAngle)) / 2.0;//Text와 PM1, PM2 사이의 공간을 제외한 한쪽호의 각도

		if(dSubAngle <= dArrowSpaceAngle) dSubAngle = (-1.0) * (double)(3 * dArrowSpaceAngle);// 공간 확보가 되지 않는 경우 밖에 화살표를 만듬

		double dRealTextSpace, dRealTextAngle = 0.0;

		if(dimType_Use == EN_DIM_TYPE_TOP)    
		{
			dRealTextSpace = dRadius + (dUnitSize*dRTextSpace + (double)(textSize.cx)*0.5) * (dRadius < 0.0 ? -1.0 : 1.0);
			PText.x = CP.x + math_lib::round_off(dRealTextSpace * vertex_C.uv[0]);
			PText.y = CP.y - math_lib::round_off(dRealTextSpace * vertex_C.uv[1]) * (long)dEMF;
			dRealTextAngle = (dgnAngle.StartAngle + dgnAngle.SweepAngle/2.0) * dEMF;
		}
		else if(dimType_Use == EN_DIM_TYPE_CENTER) 
		{
			dRealTextSpace = dRadius;
			PText.x = CP.x + math_lib::round_off(dRealTextSpace * vertex_C.uv[0]);
			PText.y = CP.y - math_lib::round_off(dRealTextSpace * vertex_C.uv[1]) * (long)dEMF;
			dRealTextAngle = (dgnAngle.StartAngle + dgnAngle.SweepAngle/2.0) * dEMF;
		}
		else if(dimType_Use == EN_DIM_TYPE_BOTTOM) 
		{
			dRealTextSpace = dRadius - (dUnitSize * dRTextSpace + (double)(textSize.cx) * 0.5) * (dRadius < 0.0 ? -1.0 : 1.0);
			PText.x = CP.x + math_lib::round_off(dRealTextSpace * vertex_C.uv[0]);
			PText.y = CP.y - math_lib::round_off(dRealTextSpace * vertex_C.uv[1]) * (long)dEMF;
			dRealTextAngle = (dgnAngle.StartAngle + dgnAngle.SweepAngle/2.0) * dEMF;
		}
		else if(dimType_Use == EN_DIM_TYPE_OUT_LEFT || dimType_Use == EN_DIM_TYPE_OUT_RIGHT)
		{       
			double dSpace, dEndShift, dGuideMaxRadius; // 지시선의 꺽이는 점까지의 거리,  꺽이는 점에서 수평으로 움직일 거리,  GuideLine의 최외각까지의 반지름
			dGuideMaxRadius = dRadius + dUnitSize * dRGuideOut * (dRadius * dRGuideOut >= 0.0 ? 1.0 : 0.0); // GuideOut은 Arc방향과 같을 경우에만 추가함
			dSpace    = dGuideMaxRadius + 4.0 * dUnitSize * dRTextSpace * (dRadius < 0.0 ? -1.0 : 1.0);      
			dEndShift = textSize.cx + dUnitSize * 4.0 * dRTextSpace;

			if(dimType_Use == EN_DIM_TYPE_OUT_LEFT) 
			{ 
				dEndShift *= (-1.0); 
			}

			PS1.x = CP.x + math_lib::round_off(dRadius*vertex_C.uv[0]);
			PS1.y = CP.y - math_lib::round_off(dRadius*vertex_C.uv[1]) * (long)dEMF;
			PS2.x = CP.x + math_lib::round_off(dSpace*vertex_C.uv[0]);
			PS2.y = CP.y - math_lib::round_off(dSpace*vertex_C.uv[1]) * (long)dEMF;
			PS3.x = PS2.x + math_lib::round_off(dEndShift);
			PS3.y = PS2.y;      

			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(3);
			dgnPolyline.arPoint[0] = PS1;
			dgnPolyline.arPoint[1] = PS2;
			dgnPolyline.arPoint[2] = PS3;

			arPolyline[nPolylineCount] = dgnPolyline; 
			++nPolylineCount;

			PText.x = (long)((PS2.x + PS3.x) / 2.0);
			PText.y = PS2.y - math_lib::round_off( dUnitSize*(dRTextSpace) + (double)textSize.cy/2.0 ) * (long)dEMF;
			dRealTextAngle = 0.0;
		}
		else if(dimType_Use == EN_DIM_TYPE_OUT_START)
		{
			dRealTextSpace = dRadius + (dUnitSize*dRTextSpace + (double)(textSize.cx)*0.5) * (dRadius < 0.0 ? -1.0 : 1.0);
			double dSpace  = dUnitSize*dRTextSpace + (double)(textSize.cy)*0.5;
			PText.x = CP.x + math_lib::round_off(dRealTextSpace*vertex_S.uv[0] - dSpace*vertex_S.uv[1]);
			PText.y = CP.y - math_lib::round_off(dRealTextSpace*vertex_S.uv[1] + dSpace*vertex_S.uv[0]) * (long)dEMF;     
			dRealTextAngle = dgnAngle.StartAngle * dEMF;      
		}
		else if(dimType_Use == EN_DIM_TYPE_OUT_END)
		{
			dRealTextSpace = dRadius + (dUnitSize*dRTextSpace + (double)(textSize.cx)*0.5) * (dRadius < 0.0 ? -1.0 : 1.0);
			double dSpace  = dUnitSize*dRTextSpace + (double)(textSize.cy)*0.5;
			PText.x = CP.x + math_lib::round_off(dRealTextSpace*vertex_E.uv[0] - dSpace*vertex_E.uv[1]);
			PText.y = CP.y - math_lib::round_off(dRealTextSpace*vertex_E.uv[1] + dSpace*vertex_E.uv[0]) * (long)dEMF;      
			dRealTextAngle = (dgnAngle.StartAngle+dgnAngle.SweepAngle) * dEMF;      
		}
		else ASSERT(0);

		int nEscapement = math_lib::round_off(10.0 * dRealTextAngle);
		if(m_bEMF) nEscapement *= -1;

		// Text는 개별적으로 그림
		T_TEXT dgnText;
		dgnText.szText = dgnAngle.szText;
		dgnText.PPosition = PText;
		DrawText(DC, fontColor, lineColor, FALSE, fontSize, szFont, EN_POSITION_TYPE_CENTER_VCENTER, dgnText, nEscapement, bFontHalfWidth);  

		T_ARC  dgnArc;
		T_LINE dgnLine;

		if(bGuideLine_P1)
		{
			dgnLine.P1 = PGS1;  
			dgnLine.P2 = PGS3;
			arLine[nGuideCount] = dgnLine;
			++nGuideCount;
		}
		if(bGuideLine_P2)
		{
			dgnLine.P1 = PGE1;  
			dgnLine.P2 = PGE3;
			arLine[nGuideCount] = dgnLine;
			++nGuideCount;
		}
	
		if(dimType_Use == EN_DIM_TYPE_CENTER)
		{
			dgnArc.CenterP = CP;
			dgnArc.Radius  = math_lib::round_off(dRadius);
			dgnArc.StartAngle = dgnAngle.StartAngle;
			if(dSweepAngle >= 0.0)
			{
				dgnArc.SweepAngle = (float)( dSubAngle);
			}
			else
			{
				dgnArc.SweepAngle = (float)(-dSubAngle);
			}
			arHalfArc[nHalfArcCount] = dgnArc;  
			++nHalfArcCount;

			dgnArc.CenterP = CP;
			dgnArc.Radius  = math_lib::round_off(dRadius);
			dgnArc.StartAngle = dgnAngle.StartAngle + dgnAngle.SweepAngle;
			if(dSweepAngle >= 0.0)
			{
				dgnArc.SweepAngle = (float)(-dSubAngle);
			}
			else
			{
				dgnArc.SweepAngle = (float)( dSubAngle);
			}
			arHalfArc[nHalfArcCount] = dgnArc;
			++nHalfArcCount;
		}
		else
		{
			dgnArc.CenterP = CP;
			dgnArc.Radius  = math_lib::round_off(dRadius);
			dgnArc.StartAngle = dgnAngle.StartAngle;
			dgnArc.SweepAngle = dgnAngle.SweepAngle;
			arArc[nArcCount] = dgnArc;
			++nArcCount;
		}    
	}
	// 안내선 그림
	DrawLine(DC, PS_SOLID, lineColor, Width, arLine);
	// 화살표 그림
	if(bMainLine)
	{
		DrawArc(DC, PS_SOLID, lineColor, lineColor, FALSE, Width, dRWidth, dRHeight, arrowType, EN_ARROW_TYPE_NONE, arHalfArc);
		DrawArc(DC, PS_SOLID, lineColor, lineColor, FALSE, Width, dRWidth, dRHeight, arrowType, arrowType, arArc);
	}
	// 지시선 그림
	DrawPolyline(DC, PS_SOLID, lineColor, lineColor, FALSE, Width, arPolyline);

	::SelectObject(DC, pOldFont);
	if(Font) ::DeleteObject(Font);  
}
