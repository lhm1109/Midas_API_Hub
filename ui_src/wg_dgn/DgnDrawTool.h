// DgnDrawTool.h: interface for the CDgnDrawTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNDRAWTOOL_H__)
#define __DGNDRAWTOOL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ABLD_DR
{
	UINT nElem;
	UINT nNode1, nNode2;
	double x1, z1;
	double x2, z2;
	
	void Initialize()
	{
		nElem = nNode1 = nNode2 = 0;
		x1 = z1 = x2 = z2 = 0.0;
	}
};

class CDgnDrawTool  : public CWnd
{
public:
	CDgnDrawTool();
	virtual ~CDgnDrawTool();

	BOOL Create(CWnd *pParentWnd, const RECT& rect, UINT nID);
	BOOL Init(CWnd* pParentWnd);
	void FitToParent(CRect rect);
	void Draw();

	void SelectNode(UINT NodeK1, UINT NodeK2);
	void SelectElem(UINT ElemK);

	CArray<ABLD_DR, ABLD_DR&> m_aDrawD;
	CArray<ABLD_DR, ABLD_DR&> m_aPointDrawD;
	CArray<ABLD_DR, ABLD_DR&> m_aSelectDrawD;
	
protected:
	BOOL   m_bElem;
	
	double m_dMaxX, m_dMinX;
	double m_dMaxZ, m_dMinZ;
	double m_dZero;

	UINT   m_SelNodeK1, m_SelNodeK2;
	UINT   m_SelElemK;

	BOOL   m_bLButtonDn, m_bRButtonDn;
	CPoint m_MouseDownPoint;
	CPoint m_MousePosiPoint;
	double m_dMouseScale;

protected:
	void DrawMember();
	
	void ClearBorder();
	void ClearBkgnd(CDC* pDC);
	double CalcScale(CRect rect, double dX, double dY, double dZoomRat=0.8);
	CPoint CalcOriginPoint(CRect rect, double dScale, double dX, double dY);

	void DrawNodeNum(CDC* pDC, CPoint orPoint, double dScale);
	void DrawPointNum(CDC* pDC, CPoint orPoint, double dScale);
	void DrawElemNum(CDC* pDC, CPoint orPoint, double dScale);
	
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnDrawTool)
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CDgnDrawTool)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__DGNDRAWTOOL_H__)
