#if !defined(AFX_DGNDRAWWND_H__0BA8C439_1A81_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNDRAWWND_H__0BA8C439_1A81_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnDrawWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnDrawWnd window
/*
#define		BEAM				1
#define		COLUMN			2
#define		BRACE				3
#define		WALL				4

#define		RECTANGLE		1
#define		CIRCLE			2
#define		TEE					3
#define		PIPE				4 
*/
enum {eDGN_BEAM=1, eDGN_COLM=2, eDGN_BRCE=3, eDGN_WALL=4, eSRC_BEAM=5};
enum {eDGN_RECT=1, eDGN_CIR =2, eDGN_TEE =3, eDGN_PIPE=4, eSRC_RHB=5};

class CDgnDrawWnd : public CWnd
{
// Construction
public:
	CDgnDrawWnd();

// Attributes
public:
	int m_SectionType,m_ShapeType,m_Nqrb,m_Nrow;
	int m_TArrayType,m_BArrayType,m_Nb,m_Nb2,m_Nt,m_Nt2,m_Ns;
	double m_Hc,m_Bc,m_Hf,m_Bf,m_D0,m_Dt,m_Db;
	double m_H, m_B1, m_B2, m_Tw, m_Tf, m_Tf2;
	BOOL m_bDiffColor;


// Operations
public:
	BOOL Initial(CWnd* pParentWnd);
	void Column_DrawData(int Shape,double hc,double bc,int nqrb,int nrow,double d0, BOOL bCorner=FALSE);
	void Brace_DrawData(int Shape,double hc,double bc);
	void Beam_DrawData(int Shape,double hc,double bc,double hf,double bf,int TArray,int BArray,
										 int Nb,int Nb2,int Nt,int Nt2,double Dt,double Db, int Ns, BOOL bnotSameTopBot=FALSE);
	void SRCBeam_DrawData(int Shape,double H, double B1, double B2, double Tw, double Tf, double Tf2,double hc,double bc,int TArray,int BArray,
												int Nb,int Nb2,int Nt,int Nt2,double Dt,double Db);
	//////////////////////////////////////////////////////////////////////
	//  기본적인 그리기 함수입니다.
	void DrawText(CDC* pDC,COLORREF color,int Width,int x,int y,CString str);
	void DrawMainAxis(CDC* pDC,COLORREF color,int Width,CRect Rect,int Cx,int Cy);
	void DrawLine(CDC* pDC,COLORREF color,int Width,int x1,int y1,int x2,int y2);
	void DrawCircle(CDC* pDC,COLORREF color,int Width,int x1,int y1,int x2,int y2);
	void DrawRebar(CDC* pDC,COLORREF color,int Width,double d,double x,double y);
	void DrawRoundRebar(CDC* pDC,COLORREF color,int Width,int X1,int Y1,int X2,int Y2,int ReSize);
	//////////////////////////////////////////////////////////////////////
	//  실질적인 그림 그리기 작업함수입니다.
	void DrawRectangleBeam(CDC* pDC,COLORREF color,double hc,double bc);
	void DrawTeeBeam(CDC* pDC,COLORREF color,double hc,double bc,double hf,double bf);
	void DrawRectangleColumn(CDC* pDC,COLORREF color,double hc,double bc);
	void DrawCircle(CDC* pDC, COLORREF color, double dd, double db=0.0);
	void DrawRHBShapeBeam(CDC* pDC,COLORREF color,double hc,double bc);

	double Calc_ScaleFactor(CRect rect,double x,double y,double zoom_ratio=0.8);
	//////////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnDrawWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDgnDrawWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDgnDrawWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNDRAWWND_H__0BA8C439_1A81_11D4_888F_0000C0F30D4D__INCLUDED_)
