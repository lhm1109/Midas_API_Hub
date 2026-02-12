#if !defined(AFX_EXTRAVIEW_H__2F475E64_6A50_11D2_A486_00A0249CC658__INCLUDED_)
#define AFX_EXTRAVIEW_H__2F475E64_6A50_11D2_A486_00A0249CC658__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExtraView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestView form view
class CTestView : public CFormViewInBar
{
protected:
	CTestView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTestView)
//enum { IDD = IDD_LCG_Extra };
// Form Data
public:
	//{{AFX_DATA(CTestView)
	enum { IDD = IDD_RCC };
	CMouseEdit	m_wndCoord;
	CButtonBitmap	m_btnEnd;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void _AlignEndButton();
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTestView)
	afx_msg void OnLCGExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTrOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTRAVIEW_H__2F475E64_6A50_11D2_A486_00A0249CC658__INCLUDED_)
