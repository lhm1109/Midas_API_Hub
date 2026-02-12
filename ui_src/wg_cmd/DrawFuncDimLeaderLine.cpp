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

// DrawDimLeaderLine ////////////////////////////////////

// T_DIM_FORMAT을 이용하여 DC에 지시선을 그림
void CDrawFunc::DrawDimLeaderLine(HDC DC, T_DIM_FORMAT dimFormat, T_LEADERLINE& dgnAngle)
{
	DrawDimLeaderLine(DC, dimFormat.fontColor, dimFormat.lineColor, dimFormat.fontSize, dimFormat.szFont.GetBuffer(), dimFormat.Width, dimFormat.dRWidth, dimFormat.dRHeight, dimFormat.arrowType, 
		dimFormat.dRTextSpace, dimFormat.dRDimSpace, dimFormat.dRGuideIn, dimFormat.dRGuideOut, dimFormat.dimType, dimFormat.bSubDimType, dimFormat.dimType_Sub, dgnAngle, dimFormat.bFontScale, dimFormat.bGuideLine_P1, dimFormat.bGuideLine_P2, dimFormat.bFontHalfWidth);
}

// T_DIM_FORMAT을 이용하여 DC에 지시선을 그림
void CDrawFunc::DrawDimLeaderLine(HDC DC, T_DIM_FORMAT dimFormat, nrT_LEADERLINE& arAngle)
{
	DrawDimLeaderLine(DC, dimFormat.fontColor, dimFormat.lineColor, dimFormat.fontSize, dimFormat.szFont.GetBuffer(), dimFormat.Width, dimFormat.dRWidth, dimFormat.dRHeight, dimFormat.arrowType, 
		dimFormat.dRTextSpace, dimFormat.dRDimSpace, dimFormat.dRGuideIn, dimFormat.dRGuideOut, dimFormat.dimType, dimFormat.bSubDimType, dimFormat.dimType_Sub, arAngle, dimFormat.bFontScale, dimFormat.bGuideLine_P1, dimFormat.bGuideLine_P2, dimFormat.bFontHalfWidth);
}

void CDrawFunc::DrawDimLeaderLine(HDC DC, COLORREF fontColor, COLORREF lineColor, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, T_LEADERLINE& dgnLeader, BOOL bFontScale, BOOL bGuideLine_P1, BOOL bGuideLine_P2, BOOL bFontHalfWidth)
{
	nrT_LEADERLINE arDimLeader;
	arDimLeader.SetSize(1);
	arDimLeader[0] = dgnLeader;

	DrawDimLeaderLine(DC, fontColor, lineColor, fontSize, szFont, Width, dRWidth, dRHeight, arrowType, dRTextSpace, dRDimSpace, dRGuideIn, dRGuideOut, dimType, bSubDimType, dimType_Sub, arDimLeader, bFontScale, bGuideLine_P1, bGuideLine_P2, bFontHalfWidth);  
}

void CDrawFunc::DrawDimLeaderLine(HDC DC, COLORREF fontColor, COLORREF lineColor, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, nrT_LEADERLINE& arLeader, BOOL bFontScale, BOOL bGuideLine_P1, BOOL bGuideLine_P2, BOOL bFontHalfWidth)
{
	// 적용 불가한 치수선 Text 형식은 치수선 중앙으로 조정함 
	if(dimType == EN_DIM_TYPE_OUT_START || dimType == EN_DIM_TYPE_OUT_END || dimType == EN_DIM_TYPE_OUT_LEFT  || dimType == EN_DIM_TYPE_OUT_RIGHT) dimType = EN_DIM_TYPE_CENTER;	
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
	int i, k, nCount, nPolySize, nZeroCount, nLeaderCount;
	nCount = arLeader.GetSize();

	CArray<SIZE, SIZE> arTextSize;  
	arTextSize.SetSize(nCount);  

	nZeroCount = 0;
	SIZE textSize;
	for(i = 0; i < nCount ; ++i)
	{
		textSize.cx = 0;
		textSize.cy = 0;
		if(arLeader[i].szText != _T(""))
		{
			// m_bEMF이면 m_hAttribDC 를 넣어주어야 한다. 
			::GetTextExtentPoint32(DC, arLeader[i].szText, arLeader[i].szText.GetLength(), &textSize);
		}
		textSize.cx = textSize.cx + fontSize / 2;
		arTextSize[i] = textSize;    
		nPolySize = arLeader[i].arPoint.GetSize();
		if(nPolySize < 2)  
		{
			nZeroCount++; 
			continue;
		}  
	}

	POINT EP0, EP1, EP2, PText;
	//
	//                         ●PText
	//         
	//           ●──●‥‥‥‥‥‥‥● 
	//          ／EP0  EP1             EP2
	//    […] ／ 
	//        ／  
	//       ／   
	//      ▶    
	//     ○    
	//     
	nrT_POLYLINE arPolyline;

	nLeaderCount = nCount - nZeroCount;
	arPolyline.SetSize(nLeaderCount);

	nLeaderCount = 0;
	for(i = 0; i < nCount ; ++i)
	{
		SIZE& textSize = arTextSize[i];    

		nPolySize = arLeader[i].arPoint.GetSize();

		if(nPolySize < 2)  continue;

		EP0 = arLeader[i].arPoint[nPolySize - 2];
		EP1 = arLeader[i].arPoint[nPolySize - 1];

		//단위 벡터 생성
		Vertex vertex;    
		if(EP0.x == EP1.x && EP0.y == EP1.y)
		{
			vertex.uv[0] = 1.0;
			vertex.uv[1] = 0.0;
		}
		else
		{
			double dLength = sqrt(pow((double)(EP0.x - EP1.x), 2.0) + pow((double)(EP0.y - EP1.y), 2.0));
			vertex.uv[0] = (double)(EP1.x-EP0.x)/dLength;
			vertex.uv[1] = (double)(EP1.y-EP0.y)/dLength;
		}

		EP2.x = EP1.x + math_lib::round_off((2.0*dUnitSize*dRTextSpace + (double)textSize.cx)*vertex.uv[0]);
		EP2.y = EP1.y + math_lib::round_off((2.0*dUnitSize*dRTextSpace + (double)textSize.cx)*vertex.uv[1]);      

		T_POLYLINE dgnPolyline;
		PText.x = (EP1.x + EP2.x)/2;
		PText.y = (EP1.y + EP2.y)/2;
		if(dimType == EN_DIM_TYPE_CENTER)
		{
			dgnPolyline.arPoint.SetSize(nPolySize);      
		}
		else
		{
			dgnPolyline.arPoint.SetSize(nPolySize+1);
			dgnPolyline.arPoint[nPolySize] = EP2;
			double dSpace = (dUnitSize * dRTextSpace + (double)(fontSize) / 2.0) * (m_bEMF ? -1.0 : 1.0) * (arLeader[i].bStrDir ? 1.0 : -1.0);
			if(dimType == EN_DIM_TYPE_BOTTOM) dSpace *= (-1.0);
			PText.x = PText.x + math_lib::round_off(dSpace*vertex.uv[1]);
			PText.y = PText.y - math_lib::round_off(dSpace*vertex.uv[0]);
		}

		for(k = 0; k < nPolySize; ++k)
		{
			dgnPolyline.arPoint[k] = arLeader[i].arPoint[k];
		}

		arPolyline[i] = dgnPolyline;

		int nEscapement = 0;
		if(EP0.x != EP1.x || EP0.y != EP1.y)
		{
			double dLength = sqrt(pow((double)(EP2.x - EP1.x), 2.0) + pow((double)(EP2.y - EP1.y), 2.0));
			nEscapement= math_lib::round_off(10.0 * math_lib::angle_2d((double)(EP2.x - EP1.x), (double)(EP2.y - EP1.y), dLength, 0.0));

			if(!arLeader[i].bStrDir) nEscapement += 1800;
		}   
		if(m_bEMF) nEscapement *= -1;
		// Text는 개별적으로 그림
		T_TEXT dgnText;
		dgnText.szText = arLeader[i].szText;
		dgnText.PPosition = PText;
		DrawText(DC, fontColor, lineColor, FALSE, fontSize, szFont, EN_POSITION_TYPE_CENTER_VCENTER, dgnText, nEscapement, bFontHalfWidth);      
	}

	// 지시선 그림
	DrawArrowPolyline(DC, PS_SOLID, lineColor, lineColor, FALSE, Width, dRWidth, dRHeight, arrowType, EN_ARROW_TYPE_NONE, arPolyline);

	::SelectObject(DC, pOldFont);
	if(Font) ::DeleteObject(Font);  
}