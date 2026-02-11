#if !defined(__GSXBUTTON_H__)
#define __GSXBUTTON_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// gsxbutton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GSxButton window
#define	GSXBUTTON_CENTER	-1

class GSxButton : public CButton
{
// Construction
public:
	GSxButton();

// Attributes
protected:
	//	Positioning
	BOOL		m_bUseOffset;				
	CPoint		m_pointImage;
	CPoint		m_pointText;
	int			m_nImageOffsetFromBorder;
	int			m_nTextOffsetFromImage;

	//	Image
	UINT         m_MaskedBitmapID;
	HICON		m_hIcon;					
	HBITMAP		m_hBitmap;
	HBITMAP		m_hBitmapDisabled;
	int			m_nImageWidth, m_nImageHeight;
		COLORREF m_crTransparentMask;
	//	Color Tab
	char		m_bColorTab;				
	COLORREF	m_crColorTab;

	//	State
	BOOL	   m_bDefault;
	UINT		m_nOldAction;
	UINT		m_nOldState;
	
	BOOL       m_bIsTransparentBitmap;

// Operations
public:
	//	Positioning
	int		SetImageOffset( int nPixels ); 
	int		SetTextOffset( int nPixels );
	CPoint	SetImagePos( CPoint p );
	CPoint	SetTextPos( CPoint p );

	//	Image
	BOOL	SetIcon( UINT nID, int nWidth, int nHeight );
	BOOL	SetBitmap( UINT nID, int nWidth, int nHeight );
	BOOL	SetMaskedBitmap( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask );
	BOOL	HasImage() 
	{ 
		//return (BOOL)( m_hIcon != 0  | m_hBitmap != 0 );  //Original
		return (BOOL)( m_hIcon != 0  || m_hBitmap != 0 ); 
	}

	//	Color Tab
	void	SetColorTab(COLORREF crTab);

	//	State
	BOOL	SetDefaultButton( BOOL bState = TRUE );

protected:
	BOOL	SetBitmapCommon( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask, BOOL bUseMask );
	void	CheckPointForCentering( CPoint &p, int nWidth, int nHeight );
	void	Redraw();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GSxButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GSxButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(GSxButton)
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CButtonBitmap window
class CButtonBitmap : public GSxButton
{
// Construction
public:
	CButtonBitmap();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonBitmap)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonBitmap();
	 /*
	void SetBitmaps(UINT IDBitmapActive,UINT IDBitmapInActive,COLORREF mask);
		void SetActive();
		void SetInActive();
		BOOL GetCurrentStatus();
	*/
	// Generated message map functions
protected:
	//BOOL m_bCurrentStatus; 
	
	//UINT m_IDActiveBitmap;
	//UINT m_IDInActiveBitmap;
	//{{AFX_MSG(CButtonBitmap)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__GSXBUTTON_H__)
