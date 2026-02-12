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

// DrawDimension ////////////////////////////////////

// T_DIM_FORMAT을 이용하여 DC에 치수선을 그림
void CDrawFunc::DrawDimension(HDC DC, T_DIM_FORMAT dimFormat, T_DIMENSION dgnDimension)
{
	DrawDimension(DC, dimFormat.fontColor, dimFormat.lineColor, dimFormat.fontSize, dimFormat.szFont.GetBuffer(), dimFormat.Width, dimFormat.dRWidth, dimFormat.dRHeight, dimFormat.arrowType, 
		dimFormat.dRTextSpace, dimFormat.dRDimSpace, dimFormat.dRGuideIn, dimFormat.dRGuideOut, dimFormat.dimType, dimFormat.bSubDimType, dimFormat.dimType_Sub, dgnDimension, dimFormat.bFontScale, dimFormat.bMainLine, dimFormat.bGuideLine_P1, dimFormat.bGuideLine_P2, dimFormat.bFontHalfWidth, dimFormat.dOutTypeRate_L, dimFormat.dOutTypeRate_R);
}

// T_DIM_FORMAT을 이용하여 DC에 치수선을 그림
void CDrawFunc::DrawDimension(HDC DC, T_DIM_FORMAT dimFormat, nrT_DIMENSION& arDimension)
{
	DrawDimension(DC, dimFormat.fontColor, dimFormat.lineColor, dimFormat.fontSize, dimFormat.szFont.GetBuffer(), dimFormat.Width, dimFormat.dRWidth, dimFormat.dRHeight, dimFormat.arrowType, 
		dimFormat.dRTextSpace, dimFormat.dRDimSpace, dimFormat.dRGuideIn, dimFormat.dRGuideOut, dimFormat.dimType, dimFormat.bSubDimType, dimFormat.dimType_Sub, arDimension, dimFormat.bFontScale, dimFormat.bMainLine, dimFormat.bGuideLine_P1, dimFormat.bGuideLine_P2, dimFormat.bFontHalfWidth, dimFormat.dOutTypeRate_L, dimFormat.dOutTypeRate_R);
}

void CDrawFunc::DrawDimension(HDC DC, COLORREF fontColor, COLORREF lineColor, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, T_DIMENSION dgnDimension, BOOL bFontScale, BOOL bMainLine, BOOL bGuideLine_P1, BOOL bGuideLine_P2, BOOL bFontHalfWidth, double dOutTypeRate_L, double dOutTypeRate_R)
{
	nrT_DIMENSION arDimension;
	arDimension.SetSize(1);
	arDimension[0] = dgnDimension;
	DrawDimension(DC, fontColor, lineColor, fontSize, szFont, Width, dRWidth, dRHeight, arrowType, dRTextSpace, dRDimSpace, dRGuideIn, dRGuideOut, dimType, bSubDimType, dimType_Sub, arDimension, bFontScale, bMainLine, bGuideLine_P1, bGuideLine_P2, bFontHalfWidth, dOutTypeRate_L, dOutTypeRate_R);  
}

void CDrawFunc::DrawDimension(HDC DC, COLORREF fontColor, COLORREF lineColor, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, nrT_DIMENSION& arDimension, BOOL bFontScale, BOOL bMainLine, BOOL bGuideLine_P1, BOOL bGuideLine_P2, BOOL bFontHalfWidth, double dOutTypeRate_L, double dOutTypeRate_R)
{
	// 적용 불가한 치수선 Text 형식은 치수선 중앙으로 조정함 
	if(dimType == EN_DIM_TYPE_OUT_START || dimType == EN_DIM_TYPE_OUT_END) dimType = EN_DIM_TYPE_CENTER;
	if(dimType == EN_DIM_SUB_TYPE_TOP_OUT_LEFT    || dimType == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT   ) dimType = EN_DIM_TYPE_TOP;
	if(dimType == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT || dimType == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT) dimType = EN_DIM_TYPE_CENTER;
	if(dimType == EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT || dimType == EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT) dimType = EN_DIM_TYPE_BOTTOM;
	if(dimType_Sub == EN_DIM_TYPE_OUT_START || dimType_Sub == EN_DIM_TYPE_OUT_END) dimType_Sub = EN_DIM_TYPE_CENTER;
	if(dOutTypeRate_L < 0.0) dOutTypeRate_L = 2.0;
	if(dOutTypeRate_R < 0.0) dOutTypeRate_R = 2.0;
	// Font는 문자열의 크기를 구하기 위한 용도로만 사용됨
	HFONT Font;	
	Font = ::CreateFont(fontSize, (bFontHalfWidth ? fontSize/2 : 0), 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, szFont);
	HGDIOBJ pOldFont = ::SelectObject(DC, Font);
	//  
	double dUnitSize = 1;
	if(bFontScale) dUnitSize = (double)(fontSize);
	int i, nCount, nArrowCount, nHalfArrowCount, nGuideCount, nPolylineCount, nSubTypCount, nZeroCount;
	nCount = arDimension.GetSize();

	SIZE textSize;
	CArray<SIZE, SIZE>   arTextSize;
	arTextSize.SetSize(nCount);
	CArray<double, double> arDimLength;
	arDimLength.SetSize(nCount);  
	nSubTypCount = 0;
	nZeroCount = 0;	

	double dEMF = m_bEMF ? -1.0 : 1.0;
	
	for(i = 0; i < nCount; i++) 
	{
		T_DIMENSION& dgnDim = arDimension[i]; 
			
		textSize.cx = 0;
		textSize.cy = 0;

		if(dgnDim.szText != _T(""))
		{
			// m_bEMF이면 m_hAttribDC 를 넣어주어야 한다. 
			::GetTextExtentPoint32(DC, dgnDim.szText, dgnDim.szText.GetLength(), &textSize); 			
		}

		double dLength = sqrt(pow((double)(dgnDim.P1.x - dgnDim.P2.x), 2.0) + pow((double)(dgnDim.P1.y - dgnDim.P2.y), 2.0));
		arTextSize[i] = textSize;
		arDimLength[i] = dLength;    

		if(fabs(dLength) < m_Zero && !dgnDim.bDefaultAngle) 
		{
			nZeroCount++; 
			continue;
		}
		if(bSubDimType && (textSize.cx + (double)(Width)*dRWidth) > dLength) nSubTypCount++;    
	}

	int nGuideNum = 0; 
	if(bGuideLine_P1) ++nGuideNum;
	if(bGuideLine_P2) ++nGuideNum;

	nGuideCount = nGuideNum * (nCount - nZeroCount);
	nArrowCount = 0;
	nHalfArrowCount = 0;
	nPolylineCount  = 0;
	if(dimType == EN_DIM_TYPE_CENTER) 
	{
		nHalfArrowCount = 2 * (nCount-nZeroCount-nSubTypCount);  
	}
	else                                        
	{
		nArrowCount = nCount-nZeroCount-nSubTypCount;  
	}

	if(dimType == EN_DIM_TYPE_OUT_LEFT || dimType == EN_DIM_TYPE_OUT_RIGHT) 
	{
		nPolylineCount = nCount-nZeroCount-nSubTypCount;
	}

	if(dimType_Sub == EN_DIM_TYPE_CENTER || 
		dimType_Sub == EN_DIM_SUB_TYPE_TOP_OUT_LEFT    || dimType_Sub == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT ||
		dimType_Sub == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT || dimType_Sub == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT ||
		dimType_Sub == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT || dimType_Sub == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT )
	{
		nHalfArrowCount = nHalfArrowCount + 2 * nSubTypCount;  
	}
	else
	{
		nArrowCount = nArrowCount + nSubTypCount; 
	}

	if(dimType_Sub == EN_DIM_TYPE_OUT_LEFT || dimType_Sub == EN_DIM_TYPE_OUT_RIGHT) 
	{
		nPolylineCount = nPolylineCount + nSubTypCount;
	}

	nrT_LINE arLine, arArrow, arHalfArrow;
	nrT_POLYLINE arPolyline;
	arLine.SetSize(nGuideCount);
	arArrow.SetSize(nArrowCount);
	arHalfArrow.SetSize(nHalfArrowCount);
	arPolyline.SetSize(nPolylineCount);

	nArrowCount = nHalfArrowCount = nGuideCount = nPolylineCount = 0; //초기화

	POINT PG11, PG12, PG13, PG21, PG22, PG23, PM1, PM2, PText, PS1, PS2, PS3;
	//
	//                                   ●PText
	//                                   
	//                        PS2 ●──────● PS3
	//                           /
	// PG13●                   /                ●PG23
	//     │                  /                 │
	// PG12●──────●   ●   ●──────●PG22
	//     │            PM1  PS1  PM2           │
	//     │                                    │
	// PG11●                                    ●PG21
	//
	//   P1●                                    ●P2
	if(m_bEMF)
	{
		dRDimSpace *= -1;
		dRGuideIn  *= -1;
		dRGuideOut *= -1;
	}
	Vertex      vertex;
	EN_DIM_TYPE dimType_Use;
	
	for(i = 0; i < nCount; i++)
	{
		T_DIMENSION& dgnDim = arDimension[i]; 		
		SIZE& textSize = arTextSize[i];

		//단위 벡터 생성
		double& dLength = arDimLength[i];

		if(fabs(dLength) < m_Zero && !dgnDim.bDefaultAngle) continue;

		if(dLength <= m_Zero) 
		{ 
			if(!dgnDim.bDefaultAngle)	
			{
				vertex.uv[0] = vertex.uv[1] = 0.0;
			}
			else 
			{
				double dAngle_rad  = math_lib::radian(dgnDim.dDefaultAngle);
				vertex.uv[0] = cos(dAngle_rad);
				vertex.uv[1] = sin(dAngle_rad) * dEMF;

				if(fabs(vertex.uv[0]) < m_Zero) 
				{ 
					vertex.uv[0] = 0.0; 
					vertex.uv[1] = (vertex.uv[1] < 0.0) ? -1.0 : 1.0; 
				}
				if(fabs(vertex.uv[1]) < m_Zero)
				{
					vertex.uv[1] = 0.0;
					vertex.uv[0] = (vertex.uv[0] < 0.0) ? -1.0 : 1.0; 
				}
			}
		}
		else
		{
			if(dLength <= 2 && dgnDim.bDefaultAngle)
			{
				double dAngle_rad = math_lib::radian(dgnDim.dDefaultAngle);
				vertex.uv[0] = cos(dAngle_rad);
				vertex.uv[1] = sin(dAngle_rad) * dEMF;

				if(fabs(vertex.uv[0]) < m_Zero) 
				{ 
					vertex.uv[0] = 0.0; 
					vertex.uv[1] = (vertex.uv[1] < 0.0) ? -1.0 : 1.0; 
				}
				if(fabs(vertex.uv[1]) < m_Zero)
				{ 
					vertex.uv[1] = 0.0; 
					vertex.uv[0] = (vertex.uv[0] < 0.0) ? -1.0 : 1.0;
				}
			}
			else 
			{
				vertex.uv[0] = (double)(dgnDim.P2.x-dgnDim.P1.x)/dLength;
				vertex.uv[1] = (double)(dgnDim.P2.y-dgnDim.P1.y)/dLength;
			}
		}    

		if(bSubDimType && (textSize.cx + (double)(Width)*dRWidth) > dLength)
		{
			dimType_Use = dimType_Sub;
		}
		else
		{
			dimType_Use = dimType;
		}

		PG11.x = dgnDim.P1.x + math_lib::round_off(dUnitSize * dRDimSpace * vertex.uv[1]);
		PG11.y = dgnDim.P1.y - math_lib::round_off(dUnitSize * dRDimSpace * vertex.uv[0]);
		PG12.x = dgnDim.P1.x + math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[1]);
		PG12.y = dgnDim.P1.y - math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[0]);
		PG13.x = dgnDim.P1.x + math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex.uv[1]);
		PG13.y = dgnDim.P1.y - math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex.uv[0]);
		PG21.x = dgnDim.P2.x + math_lib::round_off(dUnitSize * dRDimSpace * vertex.uv[1]);
		PG21.y = dgnDim.P2.y - math_lib::round_off(dUnitSize * dRDimSpace * vertex.uv[0]);
		PG22.x = dgnDim.P2.x + math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[1]);
		PG22.y = dgnDim.P2.y - math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[0]);
		PG23.x = dgnDim.P2.x + math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex.uv[1]);
		PG23.y = dgnDim.P2.y - math_lib::round_off(dUnitSize * (dRDimSpace + dRGuideIn + dRGuideOut) * vertex.uv[0]);

		double dSubLength_L = (dLength - (double)(textSize.cx + fontSize/2))/2.0;//Text와 PM1, PM2 사이의 공간을 fontSize/2만큼의 공간을 확보
		double dSubLength_R = dSubLength_L;

		if(dSubLength_L <= Width*dRWidth) 
		{
			dSubLength_L = (-1.0)*(double)(dOutTypeRate_L*Width*dRWidth);// 공간 확보가 되지 않는 경우 밖에 화살표를 만듬
			dSubLength_R = (-1.0)*(double)(dOutTypeRate_R*Width*dRWidth);// 공간 확보가 되지 않는 경우 밖에 화살표를 만듬
		}

		PM1.x  = dgnDim.P1.x + math_lib::round_off( dSubLength_L * vertex.uv[0] + dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[1]);
		PM1.y  = dgnDim.P1.y + math_lib::round_off( dSubLength_L * vertex.uv[1] - dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[0]);
		PM2.x  = dgnDim.P2.x + math_lib::round_off(-dSubLength_R * vertex.uv[0] + dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[1]);
		PM2.y  = dgnDim.P2.y + math_lib::round_off(-dSubLength_R * vertex.uv[1] - dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[0]);
		if(dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_LEFT || dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT)
		{
			PM1.x  = dgnDim.P1.x + math_lib::round_off( min(dSubLength_L, (dSubLength_L - textSize.cx)) * vertex.uv[0] + dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[1]);
			PM1.y  = dgnDim.P1.y + math_lib::round_off( min(dSubLength_L, (dSubLength_L - textSize.cx)) * vertex.uv[1] - dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[0]);
		}
		else if(dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT || dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT)
		{
			PM2.x  = dgnDim.P2.x + math_lib::round_off(-1.0*(min(dSubLength_R, (dSubLength_R - textSize.cx))) * vertex.uv[0] + dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[1]);
			PM2.y  = dgnDim.P2.y + math_lib::round_off(-1.0*(min(dSubLength_R, (dSubLength_R - textSize.cx))) * vertex.uv[1] - dUnitSize * (dRDimSpace + dRGuideIn) * vertex.uv[0]);
		}

		double dRealTextSpace = 0.0, dRealTextShift = 0.0;
		
		if     (dimType_Use == EN_DIM_TYPE_CENTER) {dRealTextSpace = dUnitSize * (dRDimSpace + dRGuideIn); }		
		else if(dimType_Use == EN_DIM_TYPE_TOP)    {dRealTextSpace = dUnitSize * (dRDimSpace + dRGuideIn) + (dUnitSize*dRTextSpace+(double)(fontSize)*0.5)*dEMF;}
		else if(dimType_Use == EN_DIM_TYPE_BOTTOM) {dRealTextSpace = dUnitSize * (dRDimSpace + dRGuideIn) - (dUnitSize*dRTextSpace+(double)(fontSize)*0.5)*dEMF;}
		else if(dimType_Use == EN_DIM_TYPE_OUT_LEFT || dimType_Use == EN_DIM_TYPE_OUT_RIGHT)
		{       
			double dSpace, dStartShift, dEndShift;
			if((!m_bEMF && dRGuideIn >= 0) || (m_bEMF && dRGuideIn <= 0))
			{ 
				double dRGuideOutMax = m_bEMF ? min(dRGuideOut,0.0) : max(dRGuideOut,0.0);
				dSpace      = dUnitSize*(dRDimSpace+dRGuideIn+dRGuideOutMax+dRTextSpace*dEMF);
				dStartShift = dUnitSize*(dRGuideOutMax+dRTextSpace*dEMF)*dEMF;
			}
			else               
			{
				double dRGuideOutMin = m_bEMF ? max(dRGuideOut,0.0) : min(dRGuideOut,0.0);
				dSpace      = dUnitSize*(dRDimSpace+dRGuideIn+dRGuideOutMin)-(dUnitSize*2.0*dRTextSpace+(double)(fontSize)*1.0)*dEMF;
				dStartShift = (dUnitSize*(-dRGuideOutMin)+(dUnitSize*2.0*dRTextSpace+(double)(fontSize)*1.0)*dEMF)*dEMF;
			}
			dEndShift = dStartShift + textSize.cx + dUnitSize*4.0*dRTextSpace;

			if(dimType_Use == EN_DIM_TYPE_OUT_LEFT) 
			{
				dStartShift *= (-1.0);
				dEndShift *= (-1.0);
			}

			PS1.x = math_lib::round_off((double)(dgnDim.P1.x + dgnDim.P2.x) / 2.0 + dUnitSize*(dRDimSpace+dRGuideIn)*vertex.uv[1]);
			PS1.y = math_lib::round_off((double)(dgnDim.P1.y + dgnDim.P2.y) / 2.0 - dUnitSize*(dRDimSpace+dRGuideIn)*vertex.uv[0]);
			PS2.x = math_lib::round_off((double)(dgnDim.P1.x + dgnDim.P2.x) / 2.0 + dStartShift*vertex.uv[0] + dSpace*vertex.uv[1]);
			PS2.y = math_lib::round_off((double)(dgnDim.P1.y + dgnDim.P2.y) / 2.0 + dStartShift*vertex.uv[1] - dSpace*vertex.uv[0]);
			PS3.x = math_lib::round_off((double)(dgnDim.P1.x + dgnDim.P2.x) / 2.0 + dEndShift*vertex.uv[0]   + dSpace*vertex.uv[1]);
			PS3.y = math_lib::round_off((double)(dgnDim.P1.y + dgnDim.P2.y) / 2.0 + dEndShift*vertex.uv[1]   - dSpace*vertex.uv[0]);

			dRealTextSpace = dSpace + (dUnitSize*dRTextSpace+(double)(fontSize)*0.5)*(m_bEMF ? -1.0 : 1.0);
			dRealTextShift = (dStartShift + dEndShift)/2.0;

			T_POLYLINE dgnPolyline;
			dgnPolyline.arPoint.SetSize(3);
			dgnPolyline.arPoint[0] = PS1;
			dgnPolyline.arPoint[1] = PS2;
			dgnPolyline.arPoint[2] = PS3;

			arPolyline[nPolylineCount] = dgnPolyline;
			++nPolylineCount;
		}
		else if(dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_LEFT || dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT) 
		{
			dRealTextSpace = dUnitSize*(dRDimSpace+dRGuideIn) + (dUnitSize*dRTextSpace+(double)(fontSize)*0.5)*(m_bEMF ? -1.0 : 1.0);

			if(bSubDimType && (textSize.cx + (double)(Width)*dRWidth) > dLength)
			{
				if(dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_LEFT)
				{ 
					dRealTextShift = -1.0*(dLength/2.0 + max(fabs(dSubLength_L), (fabs(dSubLength_L) + textSize.cx/2)));
				}
				else if(dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT)
				{ 
					dRealTextShift = dLength/2.0 + max(fabs(dSubLength_R), (fabs(dSubLength_R) + textSize.cx/2)); 
				}
			}
		}
		else if(dimType_Use == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT || dimType_Use == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT) 
		{
			dRealTextSpace = dUnitSize*(dRDimSpace+dRGuideIn);

			if(bSubDimType && (textSize.cx + (double)(Width)*dRWidth) > dLength)
			{
				if(dimType_Use == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT)
				{ 
					dRealTextShift = -1.0*(dLength/2.0 + fabs(dSubLength_L) + textSize.cx/2 + fontSize/4);
				}
				else if(dimType_Use == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT)
				{
					dRealTextShift = dLength/2.0 + fabs(dSubLength_R) + textSize.cx/2 + fontSize/4;
				}
			}
		}
		else if(dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT || dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT) 
		{
			dRealTextSpace = dUnitSize*(dRDimSpace+dRGuideIn)-(dUnitSize*dRTextSpace+(double)(fontSize)*0.5)*dEMF;

			if(bSubDimType && (textSize.cx + (double)(Width)*dRWidth) > dLength)
			{
				if(dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT)
				{ 
					dRealTextShift = -1.0*(dLength/2.0 + max(fabs(dSubLength_L), (fabs(dSubLength_L) + textSize.cx/2)));
				}
				else if(dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT)
				{
					dRealTextShift = dLength/2.0 + max(fabs(dSubLength_R), (fabs(dSubLength_R) + textSize.cx/2));
				}
			}
		}
		else
		{
			ASSERT(0);
		}
		PText.x = math_lib::round_off((double)(dgnDim.P1.x+dgnDim.P2.x)/2.0 + dRealTextShift*vertex.uv[0] + dRealTextSpace*vertex.uv[1]);
		PText.y = math_lib::round_off((double)(dgnDim.P1.y+dgnDim.P2.y)/2.0 + dRealTextShift*vertex.uv[1] - dRealTextSpace*vertex.uv[0]);

		int nEscapement = 0;
		if(dLength <= m_Zero) 
		{ 
			if(!dgnDim.bDefaultAngle) nEscapement = math_lib::round_off(10.0 * dgnDim.dDefaultAngle);
			else                       nEscapement = 0;
		}
		else 
		{
			if(dLength <= 2 && dgnDim.bDefaultAngle) 
			{
				nEscapement = math_lib::round_off(-10.0 * dgnDim.dDefaultAngle);
			}
			else                                     
			{
				//nEscapement = math_lib::round_off(10.0 * math_lib::angle_2d((double)(dgnDim.P2.x-dgnDim.P1.x), (double)(dgnDim.P2.y-dgnDim.P1.y), dLength, 0.0));
				//방향을 반대로 한다.(상동)
				nEscapement = math_lib::round_off(-10.0 * math_lib::angle_2d((double)(dgnDim.P2.x-dgnDim.P1.x), (double)(dgnDim.P2.y-dgnDim.P1.y), dLength, 0.0));
			}
		}
		if(m_bEMF) nEscapement *= -1;

		// Text는 개별적으로 그림
		T_TEXT  dgnText;
		dgnText.szText = dgnDim.szText;
		dgnText.PPosition = PText;
		DrawText(DC, fontColor, lineColor, FALSE, fontSize, szFont, EN_POSITION_TYPE_CENTER_VCENTER, dgnText, nEscapement, bFontHalfWidth);

		T_LINE dgnLine;

		if(bGuideLine_P1)
		{
			dgnLine.P1 = PG11; 
			dgnLine.P2 = PG13;
			arLine[nGuideCount] = dgnLine;
			++nGuideCount;
		}
		if(bGuideLine_P2)
		{
			dgnLine.P1 = PG21;  
			dgnLine.P2 = PG23;
			arLine[nGuideCount] = dgnLine;
			++nGuideCount;
		}
		if(dimType_Use == EN_DIM_TYPE_CENTER || 
			dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_LEFT    || dimType_Use == EN_DIM_SUB_TYPE_TOP_OUT_RIGHT    ||
			dimType_Use == EN_DIM_SUB_TYPE_CENTER_OUT_LEFT || dimType_Use == EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT ||
			dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT || dimType_Use == EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT )
		{
			dgnLine.P1 = PG12;   
			dgnLine.P2 = PM1;
			arHalfArrow[nHalfArrowCount] = dgnLine;
			++nHalfArrowCount;
			dgnLine.P1 = PG22;  
			dgnLine.P2 = PM2;
			arHalfArrow[nHalfArrowCount] = dgnLine;
			++nHalfArrowCount;            
		}
		else
		{
			dgnLine.P1 = PG12;
			dgnLine.P2 = PG22;
			arArrow[nArrowCount] = dgnLine;
			++nArrowCount;
		}    
	}
	// 안내선 그림
	DrawLine(DC, PS_SOLID, lineColor, Width, arLine);
	// 화살표 그림
	if(bMainLine)
	{
		DrawArrow(DC, PS_SOLID, lineColor, Width, dRWidth, dRHeight, arrowType, EN_ARROW_TYPE_NONE, arHalfArrow);
		DrawArrow(DC, PS_SOLID, lineColor, Width, dRWidth, dRHeight, arrowType, arrowType, arArrow);
	}
	// 지시선 그림
	DrawPolyline(DC, PS_SOLID, lineColor, lineColor, FALSE, Width, arPolyline);

	::SelectObject(DC, pOldFont);
	if(Font) ::DeleteObject(Font);
}
