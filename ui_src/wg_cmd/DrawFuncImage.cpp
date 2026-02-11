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

// Draw_image ///////////////////////////////////

void CDrawFunc::DrawImage(HDC DC, HBITMAP img, RECT rtCanvas, BOOL bStreach)
{
	BITMAP bmpInfo;
	::GetObject(img, sizeof(BITMAP), &bmpInfo);

	RECT rtImg;
	rtImg.left = 0;
	rtImg.top  = 0;
	rtImg.right = bmpInfo.bmWidth;
	rtImg.bottom = bmpInfo.bmHeight;

	HDC MemDC;
	MemDC = ::CreateCompatibleDC(DC);

	HGDIOBJ pOldBitmap = ::SelectObject(MemDC, img);

	// HBITMAP
	LONG imgWidth = rtImg.right - rtImg.left;
	LONG ImgHeight = rtImg.bottom - rtImg.top;
	LONG Width = rtCanvas.right - rtCanvas.left;
	LONG Height = rtCanvas.bottom - rtCanvas.top;

	if(!bStreach)
	{
		int nWidth = Width; 
		int nHeight = Height; 

		double dWidthR = (double)nWidth / (double)imgWidth; 
		double dHeightR = (double)nHeight / (double)ImgHeight;

		double dCriter = (dWidthR < dHeightR) ? dWidthR : dHeightR;

		double dWidth = (double)imgWidth * dCriter;
		double dHeight = (double)ImgHeight * dCriter;

		double dMarginHor = ((double)Width - dWidth) / 2.0;
		double dMarginVer = ((double)Height - dHeight) / 2.0;

		::StretchBlt(DC, (int)dMarginHor, (int)dMarginVer, (int)dWidth, (int)dHeight, MemDC, 0, 0, imgWidth, ImgHeight, SRCCOPY);
	}
	else
	{
		::StretchBlt(DC, 0, 0, Width, Height, MemDC, 0, 0, imgWidth, ImgHeight, SRCCOPY );
	}

	::SelectObject(MemDC, pOldBitmap);
}
