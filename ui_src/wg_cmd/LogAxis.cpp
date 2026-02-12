// LogAxis.cpp: implementation of the CLogAxis class.
//
//////////////////////////////////////////////////////////////////////

#include <string.h>

#include "stdafx.h"
//#include "sgsv02.h"
#include "LogAxis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogAxis::CLogAxis()
{
	m_IsCreated = FALSE;
	m_TickNum = 0;
	m_LabelSlope = 450;
	m_TickSlope = 3150;

	m_TickText.RemoveAll();
	m_TickPos.RemoveAll();
}

CLogAxis::~CLogAxis()
{
	m_TickText.RemoveAll();
	m_TickPos.RemoveAll();
}

void CLogAxis::AddLabel(CString lbl, CPoint pos)
{
	m_TickText.SetAtGrow(m_TickNum, lbl);
	m_TickPos.SetAtGrow(m_TickNum, pos);
	m_TickNum++;
}

void CLogAxis::DrawAllLabels(CDC *pDC)
{
	double	disp_xres, disp_yres, print_xres, print_yres;
	HDC		hDispDC;
	CPen	pen, *pOldPen;
	LOGFONT lf;
	CFont	SmallFont, BigFont, *pOldFont;

	hDispDC = ::GetDC(NULL);
	disp_xres = ((double) GetDeviceCaps(hDispDC, HORZRES))/((double) GetDeviceCaps(hDispDC, HORZSIZE));
	disp_yres = ((double) GetDeviceCaps(hDispDC, VERTRES))/((double) GetDeviceCaps(hDispDC, VERTSIZE));
	print_xres = ((double) pDC->GetDeviceCaps(HORZRES))/((double) pDC->GetDeviceCaps(HORZSIZE));
	print_yres = ((double) pDC->GetDeviceCaps(VERTRES))/((double) pDC->GetDeviceCaps(VERTSIZE));

	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pOldPen = (CPen *) pDC->SelectObject(&pen);
	pDC->MoveTo(m_AxisP[0]);
	pDC->LineTo(m_AxisP[1]);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	if (pDC->IsPrinting()) {
		lf.lfHeight			= (int) (13.0*print_yres/disp_yres);
		lf.lfWidth			= (int) (6.0*print_xres/disp_xres);
	}
	else {
		lf.lfHeight			= 13;
		lf.lfWidth			= 6;
	}
	lf.lfEscapement		= m_TickSlope;
	lf.lfOrientation	= 0;
	lf.lfWeight			= FW_NORMAL;
	lf.lfItalic			= FALSE;
	lf.lfUnderline		= FALSE;
	lf.lfStrikeOut		= FALSE;
	lf.lfCharSet		= DEFAULT_CHARSET;
	lf.lfOutPrecision	= OUT_CHARACTER_PRECIS;
	lf.lfClipPrecision	= CLIP_CHARACTER_PRECIS;
	lf.lfQuality		= DEFAULT_QUALITY;
	lf.lfPitchAndFamily	= DEFAULT_PITCH|FF_DONTCARE;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	SmallFont.CreateFontIndirect(&lf);

	if (pDC->IsPrinting()) {
		lf.lfHeight			= (int) (14.0*print_yres/disp_yres);
		lf.lfWidth			= (int) (6.0*print_xres/disp_xres);
	}
	else {
		lf.lfHeight			= 14;
		lf.lfWidth			= 6;
	}
	lf.lfWeight = FW_NORMAL;
	lf.lfEscapement = m_LabelSlope;
	BigFont.CreateFontIndirect(&lf);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkColor(RGB(255, 255, 255));	
	pDC->SetBkMode(TRANSPARENT);
//	pDC->SetBkMode(OPAQUE);

	pOldFont = (CFont *) pDC->SelectObject(&BigFont);
	if (m_LabelSlope < 1800) {
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		pDC->TextOut(m_AxisP[0].x, m_AxisP[0].y, m_LabelText);
	}
	else {
		pDC->SetTextAlign(TA_RIGHT | TA_TOP);
		pDC->TextOut(m_AxisP[0].x, m_AxisP[0].y, m_LabelText);
	}

	pDC->SelectObject(&SmallFont);
	if (m_TickSlope < 1800)
		pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
	else
		pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
	for (int i=0;i<m_TickNum;i++) {
		if (m_TickSlope < 1800)
			pDC->TextOut(m_TickPos[i].x, m_TickPos[i].y, m_TickText[i]);
		else
			pDC->TextOut(m_TickPos[i].x, m_TickPos[i].y, m_TickText[i]);
	}

	pDC->SelectObject(pOldFont);
	SmallFont.DeleteObject();
	BigFont.DeleteObject();
	::ReleaseDC(NULL, hDispDC);
	
}

CPoint CLogAxis::FindCrossPoint(CPoint s, CPoint e)
{
	double	a, c1, c2, c3;
	double	x1, x2, y1, y2,
			x11, x22, y11, y22;
	CPoint	p;

	x1 = (double) m_AxisP[0].x;
	x2 = (double) m_AxisP[1].x;
	y1 = (double) m_AxisP[0].y;
	y2 = (double) m_AxisP[1].y;
	x11 = (double) s.x;
	x22 = (double) e.x;
	y11 = (double) s.y;
	y22 = (double) e.y;

	a  = (y2-y1)*(x22-x11) - (y22-y11)*(x2-x1);
	c1 = x1*(x22-x11)*(y2-y1);
	c2 = x11*(x2-x1)*(y22-y11);
	c3 = (x2-x1)*(x22-x11)*(y1-y11);

	p.x = (int) ((c1-c2-c3)/a);
	p.y = (int) (((y2-y1)*(p.x-x1))/(x2-x1)) + y1;

	return p;
	
}

void CLogAxis::initialize()
{
	m_IsCreated = FALSE;
	m_TickNum = 0;
	m_TickPos.RemoveAll();
	m_TickText.RemoveAll();
}
