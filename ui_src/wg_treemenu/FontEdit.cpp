// FontEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "AMR.h"
#include "FontEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontEdit

CFontEdit::CFontEdit()
{
	m_crBkColor = WHITE;//::GetSysColor(COLOR_3DFACE); // Initializing background color to the system face color.
	m_crTextColor = BLACK; // Initializing text color to black
	m_brBkgnd.CreateSolidBrush(m_crBkColor); // Creating the Brush Color For the Edit Box Background

	m_pFont = NULL;
	
	m_strFont = _LSX(Arial);
	m_bBold   = FALSE;
	m_bItalic = FALSE;
	m_bUnderline = FALSE;

	m_nAlign = 0;
}

CFontEdit::~CFontEdit()
{
	if(m_pFont)
	{
		m_pFont->DeleteObject();
		delete m_pFont;
	}
}


BEGIN_MESSAGE_MAP(CFontEdit, CEdit)
	//{{AFX_MSG_MAP(CFontEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontEdit message handlers

HBRUSH CFontEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here	
	HBRUSH hbr;
	hbr = (HBRUSH)m_brBkgnd; // Passing a Handle to the Brush
	pDC->SetBkColor(m_crBkColor); // Setting the Color of the Text Background to the one passed by the Dialog
	pDC->SetTextColor(m_crTextColor); // Setting the Text Color to the one Passed by the Dialog

	if (nCtlColor)       // To get rid of compiler warning
			nCtlColor += 0;

	return hbr;

}

BOOL CFontEdit::PreTranslateMessage(MSG* pMsg) 
{ 
	CWnd *pWnd = GetFocus();

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{

	}
	
	return CEdit::PreTranslateMessage(pMsg);
}


void CFontEdit::SetTextColor(COLORREF crColor)
{
	m_crTextColor = crColor; // Passing the value passed by the dialog to the member varaible for Text Color
	RedrawWindow();
}

void CFontEdit::SetBkColor(COLORREF crColor)
{
	m_crBkColor = crColor; // Passing the value passed by the dialog to the member varaible for Backgound Color
	m_brBkgnd.DeleteObject(); // Deleting any Previous Brush Colors if any existed.
	m_brBkgnd.CreateSolidBrush(crColor); // Creating the Brush Color For the Edit Box Background
	RedrawWindow();
}

BOOL CFontEdit::SetReadOnly(BOOL flag)
{
	 if (flag == TRUE)
			SetBkColor(m_crBkColor);
	 else
			SetBkColor(WHITE);

	 return CEdit::SetReadOnly(flag);
}

void CFontEdit::SetFont(LPCTSTR szFontName)
{
	m_strFont = szFontName;

	CreateFont();

	CEdit::SetFont( m_pFont );

}

void CFontEdit::SetStyle(BOOL bBold, BOOL bItalic, BOOL bUnderline)
{
	m_bBold     = bBold;
	m_bItalic   = bItalic;
	m_bUnderline= bUnderline;

	CreateFont();
	
	CEdit::SetFont( m_pFont );
}

BOOL  CFontEdit::CreateFont()
{
	if(m_pFont)
	{
		m_pFont->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}

	int nWidth = m_bBold ? 12 : 8;
	
	m_pFont = new CFont();  

	int nBold = m_bBold ? FW_BOLD :FW_NORMAL;
	m_pFont->CreateFont(15, 8, 0, 0, nBold, m_bItalic , m_bUnderline , FALSE, DEFAULT_CHARSET
		, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_strFont);

	return TRUE;
}

void CFontEdit::SetAlign(int nAlign)
{
	m_nAlign = nAlign;


	long lStyle = GetWindowLongPtr(this->GetSafeHwnd(), GWL_STYLE);
	
	long cStyle;

	switch(nAlign)
	{
	case 0: cStyle = ES_LEFT   ; break;
	case 1: cStyle = ES_CENTER ; break;
	case 2: cStyle = ES_RIGHT  ; break;
	default: break;
	}
	//ES_LEFT
	lStyle &= ~(ES_LEFT|ES_CENTER|ES_RIGHT);
	lStyle |= cStyle;

	::SetWindowLongPtr(this->GetSafeHwnd() , GWL_STYLE, lStyle );

	RedrawWindow();
}


