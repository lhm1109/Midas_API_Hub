#if !defined(AFX_FontEdit_H__716C9520_916D_4227_A476_6FA87ACC434B__INCLUDED_)
#define AFX_FontEdit_H__716C9520_916D_4227_A476_6FA87ACC434B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontEdit.h : header file
//

#define RED        RGB(127,  0,  0)
#define GREEN      RGB(  0,127,  0)
#define BLUE       RGB(  0,  0,127)
#define LIGHTRED   RGB(255,  0,  0)
#define LIGHTGREEN RGB(  0,255,  0)
#define LIGHTBLUE  RGB(  0,  0,255)
#define BLACK      RGB(  0,  0,  0)
#define WHITE      RGB(255,255,255)
#define GRAY       RGB(192,192,192)


/////////////////////////////////////////////////////////////////////////////
// CFontEdit window

class CFontEdit : public CEdit
{
// Construction
public:
	CFontEdit();

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontEdit)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
public:	
	void SetBkColor(COLORREF crColor); // This Function is to set the BackGround Color for the Text and the Edit Box.
	void SetTextColor(COLORREF crColor); // This Function is to set the Color for the Text.
	BOOL SetReadOnly(BOOL flag = TRUE);
	void SetFont(LPCTSTR szFontName);
	void SetStyle(BOOL bBold=0, BOOL bItalic=0, BOOL bUnderline=0);
	void SetAlign(int nAlign);
	
	virtual ~CFontEdit();

	// Generated message map functions
protected:

	CFont    *m_pFont;
	CBrush m_brBkgnd; // Holds Brush Color for the Edit Box
	COLORREF m_crBkColor; // Holds the Background Color for the Text
	COLORREF m_crTextColor; // Holds the Color for the Text

	CString m_strFont;
	BOOL    m_bBold;
	BOOL    m_bItalic;
	BOOL    m_bUnderline;
	int     m_nAlign; //0:left 1:center 2:right

	BOOL   CreateFont();
	// Generated message map functions
protected:
	//{{AFX_MSG(CFontEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FontEdit_H__716C9520_916D_4227_A476_6FA87ACC434B__INCLUDED_)
