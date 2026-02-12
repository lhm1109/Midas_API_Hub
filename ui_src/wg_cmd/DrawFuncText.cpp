#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathLib.h"
#include "../wg_db/MathFunc.h"

#include "DrawData.h"
#include "DrawFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDrawFunc

// DrawText ////////////////////////////////////

// T_TEXT_FORMAT 이용하여 DC에 Text를 그림
void CDrawFunc::DrawText(HDC DC, T_TEXT_FORMAT textFormat, T_TEXT dgnText)
{ 
	DrawText(DC, textFormat.fontColor, textFormat.fillColor, textFormat.bFill, textFormat.Size, textFormat.szFont.GetBuffer(), textFormat.positionType, dgnText, textFormat.nEscapement, textFormat.bHalfWidth); 
};

// T_TEXT_FORMAT 이용하여 DC에 Text를 그림
void CDrawFunc::DrawText(HDC DC, T_TEXT_FORMAT textFormat, nrT_TEXT& arText)
{
	DrawText(DC, textFormat.fontColor, textFormat.fillColor, textFormat.bFill, textFormat.Size, textFormat.szFont.GetBuffer(), textFormat.positionType, arText, textFormat.nEscapement, textFormat.bHalfWidth); 
};

void CDrawFunc::DrawText(HDC DC, COLORREF fontColor, COLORREF fillColor, BOOL bFill, int fontSize, TCHAR* szFont, EN_POSITION_TYPE enPosiType, T_TEXT dgnText, int nEscapement, BOOL bHalfWidth)
{
	// 겹지정된 함수를 사용하기위한 변환함수임
	nrT_TEXT arText;
	arText.SetSize(1);
	arText[0] = dgnText;
	DrawText(DC, fontColor, fillColor, bFill, fontSize, szFont, enPosiType, arText, nEscapement, bHalfWidth);
}

void CDrawFunc::DrawText(HDC DC, COLORREF fontColor, COLORREF fillColor, BOOL bFill, int fontSize, TCHAR* szFont, EN_POSITION_TYPE enPosiType, nrT_TEXT& arTextOrg, int nEscapement, BOOL bHalfWidth)
{
	if(fontSize <= 0) return;

	int fontSize_sub = (int)(0.6 * fontSize);

	HGDIOBJ  pOldBrh;
	HPEN     LinePen; 
	HBRUSH   FillBrush; 
	HFONT    Font;
	COLORREF oldTextColor;
	COLORREF oldBkColor;

	long nEMF = m_bEMF ? -1 : 1;

	LinePen = ::CreatePen(PS_SOLID, 1, fontColor);
	FillBrush = ::CreateSolidBrush(fillColor);

	if(nEscapement == 0) fontSize += m_nFontAddSize_RZero;
	Font = ::CreateFont(fontSize, (bHalfWidth? fontSize / 2 : 0), nEscapement * nEMF, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, szFont);

	oldTextColor = ::SetTextColor(DC, fontColor);

	if(bFill) oldBkColor = ::SetBkColor(DC, fillColor);
	else       oldBkColor = RGB(0, 0 ,0);

	HGDIOBJ pOldPen = ::SelectObject(DC, LinePen);
	HGDIOBJ pOldFont = ::SelectObject(DC, Font);
	if(bFill) pOldBrh = ::SelectObject(DC, FillBrush);

	nrT_TEXT arTextLine, arText, arText_sub;
	ConvertNextLineText(fontSize, enPosiType, nEscapement, arTextOrg, arTextLine);
	ConvertMathText(DC, fontSize, fontSize_sub, enPosiType, nEscapement, arTextLine, arText, arText_sub);

	SIZE textSize;
	int  i;
	//for(auto itr = arText.Data.begin(); itr != arText.Data.end(); itr++)
	for(i = 0 ; i < arText.GetSize(); i++)
	{ 
		T_TEXT* itr = &arText[i];
		textSize.cx = 0;
		textSize.cy = 0;

		if(itr->szText != _T(""))
		{
			// m_bEMF이면 m_hAttribDC 를 넣어주어야 한다. 
			::GetTextExtentPoint32(DC, itr->szText, itr->szText.GetLength(), &textSize);
		}

		double dFactor[2];
		dFactor[0] = cos(nEscapement / 10.0 * CMathFunc::m_trrad);
		dFactor[1] = sin(nEscapement / 10.0 * CMathFunc::m_trrad);
		POINT LeftTopPoint;    

		if(enPosiType == EN_POSITION_TYPE_LEFT_TOP) 
		{
			LeftTopPoint.x = itr->PPosition.x;  
			LeftTopPoint.y = itr->PPosition.y; 
		}
		else if(enPosiType == EN_POSITION_TYPE_LEFT_VCENTER) 
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cy/2.0 * dFactor[0]) * nEMF); 
		}
		else if(enPosiType == EN_POSITION_TYPE_LEFT_BOTTOM) 
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cy * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cy * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_CENTER_TOP)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx/2.0*dFactor[0]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx/2.0*dFactor[1]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_CENTER_VCENTER)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx/2.0*dFactor[0] - textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (+textSize.cx/2.0*dFactor[1] - textSize.cy/2.0 * dFactor[0]) * nEMF); 
		}
		else if(enPosiType == EN_POSITION_TYPE_CENTER_BOTTOM)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx/2.0*dFactor[0] - textSize.cy * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx/2.0*dFactor[1] + textSize.cy * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_RIGHT_TOP)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx*dFactor[0]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx*dFactor[1]) * (m_bEMF? 1 : -1)); 
		}
		else if(enPosiType == EN_POSITION_TYPE_RIGHT_VCENTER)
		{
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx*dFactor[0] - textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx*dFactor[1] + textSize.cy/2.0 * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_RIGHT_BOTTOM)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx*dFactor[0] - textSize.cy * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx*dFactor[1] + textSize.cy * dFactor[0]) * nEMF); 
		}
		else ASSERT(0);

		if(bFill) ::SetBkMode(DC, OPAQUE);
		else       ::SetBkMode(DC, TRANSPARENT);

		::TextOut(DC, LeftTopPoint.x, LeftTopPoint.y, itr->szText, itr->szText.GetLength());
	}

	if(Font) ::DeleteObject(Font);
	Font = CreateFont(fontSize_sub, (bHalfWidth? fontSize/2 : 0), nEscapement*(m_bEMF? -1 : 1), 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, szFont);
	pOldFont = ::SelectObject(DC, Font);

	//for(auto itr = arText_sub.Data.begin(); itr != arText_sub.Data.end(); itr++) 
	for(i = 0 ; i < arText_sub.GetSize(); i++)
	{ 
		T_TEXT* itr = &arText_sub[i];
		textSize.cx = 0;
		textSize.cy = 0;

		if(itr->szText != _T("")) 
		{
			// m_bEMF이면 m_hAttribDC 를 넣어주어야 한다. 
			::GetTextExtentPoint32(DC, itr->szText, itr->szText.GetLength(), &textSize);
		}

		double dFactor[2];
		dFactor[0] = cos(nEscapement/10.0 * CMathFunc::m_trrad);
		dFactor[1] = sin(nEscapement/10.0 * CMathFunc::m_trrad);
		POINT LeftTopPoint;    

		if(enPosiType == EN_POSITION_TYPE_LEFT_TOP) 
		{
			LeftTopPoint.x = itr->PPosition.x;  
			LeftTopPoint.y = itr->PPosition.y; 
		}
		else if(enPosiType == EN_POSITION_TYPE_LEFT_VCENTER) 
		{
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cy / 2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (textSize.cy / 2.0 * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_LEFT_BOTTOM) 
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cy * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (textSize.cy * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_CENTER_TOP)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx / 2.0 * dFactor[0]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx / 2.0 * dFactor[1]) * nEMF); }
		else if(enPosiType == EN_POSITION_TYPE_CENTER_VCENTER)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx / 2.0 * dFactor[0] - textSize.cy / 2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx / 2.0 * dFactor[1] + textSize.cy / 2.0 * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_CENTER_BOTTOM)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx / 2.0 * dFactor[0] - textSize.cy * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx / 2.0 * dFactor[1] + textSize.cy * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_RIGHT_TOP)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx * dFactor[0]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx * dFactor[1]) * (m_bEMF? 1 : -1));
		}
		else if(enPosiType == EN_POSITION_TYPE_RIGHT_VCENTER)
		{
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx * dFactor[0] - textSize.cy / 2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx * dFactor[1] + textSize.cy / 2.0 * dFactor[0]) * nEMF);
		}
		else if(enPosiType == EN_POSITION_TYPE_RIGHT_BOTTOM)
		{ 
			LeftTopPoint.x = math_lib::round_off(itr->PPosition.x + (-textSize.cx * dFactor[0] - textSize.cy * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(itr->PPosition.y + (-textSize.cx * dFactor[1] + textSize.cy * dFactor[0]) * nEMF);
		}
		else ASSERT(0);

		if(bFill) ::SetBkMode(DC, OPAQUE);
		else       ::SetBkMode(DC, TRANSPARENT);

		::TextOut(DC, LeftTopPoint.x, LeftTopPoint.y, itr->szText, itr->szText.GetLength());
	}

	::SetTextColor(DC, oldTextColor);
	::SetBkColor(DC, oldBkColor);
	::SelectObject(DC, pOldPen);
	::SelectObject(DC, pOldFont);
	if(bFill) ::SelectObject(DC, pOldBrh);

	if(LinePen)   ::DeleteObject(LinePen);
	if(FillBrush) ::DeleteObject(FillBrush);
	if(Font)       ::DeleteObject(Font);
}