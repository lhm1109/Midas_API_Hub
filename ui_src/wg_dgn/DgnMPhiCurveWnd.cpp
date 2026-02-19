// DgnMPhiCurveWnd.cpp: implementation of the CDgnMPhiCurveWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnMPhiCurveWnd.h"
#include "DgnSectionDrawWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnMPhiCurveWnd::CDgnMPhiCurveWnd()
{
	m_nDrawType = 0;
	m_nCurveType = 0;
	m_pMpccD = NULL;
	m_pResultD = NULL;
}

CDgnMPhiCurveWnd::~CDgnMPhiCurveWnd()
{

}


BEGIN_MESSAGE_MAP(CDgnMPhiCurveWnd, CWnd)
	//{{AFX_MSG_MAP(CDgnMPhiCurveWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCS_SectionDrawWnd message handlers

void CDgnMPhiCurveWnd::OnPaint() 
{
	OnPaint_Wnd();
}

BOOL CDgnMPhiCurveWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CDgnMPhiCurveWnd::OnPaint_Wnd()
{
	CPaintDC dc(this); // device context for painting
	CRect canvas;
	this->GetClientRect(canvas);  
	GMemDC MemDC(&dc, canvas);

	if(m_nDrawType == 0 && m_pMpccD != NULL)
	{
		m_MPhiDesign.Draw_Section(&MemDC, canvas, m_pMpccD);
	}
	else if(m_nDrawType == 1 && m_pResultD != NULL)
	{
		m_MPhiDesign.Draw_MPhiCurve(&MemDC, canvas, m_pResultD, m_nCurveType);
	}	
}

BOOL CDgnMPhiCurveWnd::Initial(CWnd* pParentWnd)
{
	if(!SubclassWindow(pParentWnd->m_hWnd))
	 {
		 TRACE(_T("Fail To Subclassing\n"));
		 return FALSE;
	 }

	LONG dwStyle;
	dwStyle = GetWindowLongPtr(m_hWnd,GWL_STYLE);
	dwStyle |=  WS_CLIPCHILDREN;
	SetWindowLongPtr(m_hWnd,GWL_STYLE,dwStyle); 

	return TRUE;
}
