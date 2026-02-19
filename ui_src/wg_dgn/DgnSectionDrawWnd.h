#if !defined(AFX_DGNSECTIONDRAWWND_H__952CA3D8_5416_4BA4_B414_36918C00E494__INCLUDED_)
#define AFX_DGNSECTIONDRAWWND_H__952CA3D8_5416_4BA4_B414_36918C00E494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSectionDrawWnd.h : header file
//

#include "DgnSectionDrawFunc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSectionDrawWnd window
#include "HeaderPre.h"
class GMemDC : public CDC 
{
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		  m_pDC;
	CRect		  m_rcBounds;
public:
	GMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap = new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap);
		m_pDC = pDC;
		m_rcBounds = rcBounds;

		// 바탕색 : white
		CBrush  bckBrh(RGB(255,255,255));
		CBrush* pOldBrh = SelectObject(&bckBrh);
		PatBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(),m_rcBounds.Height(), PATCOPY);
		SelectObject(pOldBrh);
		bckBrh.DeleteObject();
	}
	~GMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
					this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		SelectObject(m_oldBitmap);
		if (m_bitmap != NULL) delete m_bitmap;
	}
	
	GMemDC* operator->() { return this;     }
	operator GMemDC*()   { return this;     }
	CBitmap* GetBitmap() { return m_bitmap; }
};


class __MY_EXT_CLASS__ CDgnSectionDrawWnd : public CWnd
{
public:
	CDgnSectionDrawWnd();
	virtual ~CDgnSectionDrawWnd();

	BOOL Initial(CWnd* pParentWnd);

	void Set_TextSize(int textSize);
	void Draw_Shape(int nType, T_SECT_K SectK, BOOL bDim=FALSE, BOOL bFill=FALSE);
	void Draw_ChkRebar(T_SECT_K SectK, int nIMJ, T_RCHK_D RchkD, BOOL bDim=FALSE, BOOL bFill=TRUE);
	void Draw_ChkRebar(T_SECT_K SectK, int nIMJ, T_CPCO_D CpcoD, BOOL bDim=FALSE, BOOL bFill=TRUE);// (SHIN '06.03.20 추가)


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSectionDrawWnd)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	BOOL bShape;
	BOOL bRebar;
	// 공통 변수
	int       m_nRebarDataType;// 철근정보의 Data형식 (0:Data없음, 1:T_RCHK_D, 2:T_CPCO_D)
	int       m_nType;
	int       m_nTextSize;
	BOOL      m_bDimShape; 
	BOOL      m_bDimRebar;
	BOOL      m_bFillShape;
	BOOL      m_bFillRebar;
	T_SECT_K  m_SectK;
	// For Rebar
	int       m_nIMJ;
	T_RCHK_D  m_RchkD;
	T_CPCO_D  m_CpcoD;

	CDgnSectionDrawFunc  m_DrawUtil;

	//{{AFX_MSG(CDgnSectionDrawWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSECTIONDRAWWND_H__952CA3D8_5416_4BA4_B414_36918C00E494__INCLUDED_)
