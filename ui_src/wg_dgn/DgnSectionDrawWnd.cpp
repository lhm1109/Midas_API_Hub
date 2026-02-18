// DgnSectionDrawWnd.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSectionDrawWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSectionDrawWnd

CDgnSectionDrawWnd::CDgnSectionDrawWnd()
{
	bShape = FALSE;
	bRebar = FALSE;
	m_bDimRebar  = m_bDimShape  = FALSE;
	m_bFillRebar = m_bFillShape = TRUE;
	m_nRebarDataType = 0;// (SHIN '06.03.20 추가)
	m_nTextSize = 12;
}

CDgnSectionDrawWnd::~CDgnSectionDrawWnd()
{
}

BEGIN_MESSAGE_MAP(CDgnSectionDrawWnd, CWnd)
	//{{AFX_MSG_MAP(CDgnSectionDrawWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSectionDrawWnd message handlers

BOOL CDgnSectionDrawWnd::Initial(CWnd* pParentWnd)
{
	HWND hWnd = pParentWnd->GetSafeHwnd();
	CWnd* pSubclassWnd = NULL;
	if ((pSubclassWnd = FromHandlePermanent(pParentWnd->m_hWnd)) != NULL)
		pSubclassWnd->UnsubclassWindow();

	if(!SubclassWindow(hWnd))
	 {
		 TRACE(_T("Fail To Subclassing\n"));
		 return FALSE;
	 }

	LONG dwStyle;
	dwStyle = GetWindowLongPtr(m_hWnd,GWL_STYLE);
	dwStyle |=  WS_CLIPCHILDREN;
	SetWindowLongPtr(m_hWnd,GWL_STYLE,dwStyle); 

	CRect canvas;
	this->GetClientRect(canvas);
	m_DrawUtil.Set_Canvas(canvas, 0, 1, m_nTextSize, 4);

	return TRUE;
}

void CDgnSectionDrawWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect canvas;
	this->GetClientRect(canvas);  
	GMemDC MemDC(&dc, canvas);

	if(bShape) m_DrawUtil.Draw_Shape(&MemDC, m_nType, m_SectK, m_bDimShape, m_bFillShape, m_nIMJ);
	if(bRebar) 
	{
		if(m_nRebarDataType == 1)
			m_DrawUtil.Draw_ChkRebar(&MemDC, m_SectK, m_nIMJ, &m_RchkD, m_bDimRebar, m_bFillRebar);
		else if(m_nRebarDataType == 2)
			m_DrawUtil.Draw_ChkRebar(&MemDC, m_SectK, m_nIMJ, &m_CpcoD, m_bDimRebar, m_bFillRebar);
	}

}

BOOL CDgnSectionDrawWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CDgnSectionDrawWnd::Set_TextSize(int textSize)
{
	m_nTextSize = textSize;
	CRect canvas;
	this->GetClientRect(canvas);
	m_DrawUtil.Set_Canvas(canvas, 0, 1, m_nTextSize, 4);
}

void CDgnSectionDrawWnd::Draw_Shape(int nType, T_SECT_K SectK, BOOL bDim, BOOL bFill)
{
	m_nType      = nType;
	m_SectK      = SectK;
	m_bDimShape  = bDim; 
	m_bFillShape = bFill;
	bShape       = TRUE;

}

void CDgnSectionDrawWnd::Draw_ChkRebar(T_SECT_K SectK, int nIMJ, T_RCHK_D RchkD, BOOL bDim, BOOL bFill)
{
	m_nRebarDataType = 1;// (SHIN '06.03.20 추가)

	m_SectK      = SectK;
	m_nIMJ       = nIMJ;
	m_RchkD      = RchkD;
	m_bDimRebar  = bDim; 
	m_bFillRebar = bFill;
	bRebar       = TRUE;
}

// T_CPCO_D 용 (SHIN '06.03.20 추가)
void CDgnSectionDrawWnd::Draw_ChkRebar(T_SECT_K SectK, int nIMJ, T_CPCO_D CpcoD, BOOL bDim, BOOL bFill)
{
	m_nRebarDataType = 2;

	m_SectK      = SectK;
	m_nIMJ       = nIMJ;
	m_CpcoD      = CpcoD;
	m_bDimRebar  = bDim; 
	m_bFillRebar = bFill;
	bRebar       = TRUE;
}