#if !defined(__IEHPYIELDPROPGRID_H__)
#define __IEHPYIELDPROPGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpYieldPropGrid.h : header file
//
#include "CMGXGridWnd.h"
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropGrid window
class CIehpYieldPropDlgNew;
class CIehpYieldPropGrid : public CCMGXGridWnd
{
// Construction
public:
	CIehpYieldPropGrid(CIehpYieldPropDlgNew* pParent);
	virtual ~CIehpYieldPropGrid();

// Attributes
public:
	void SetDefaultData(BOOL bConc = FALSE) { m_bConc = FALSE; }// bConc; }

// Operations
public:
	void Initialize();
	void SetFormatMode(int nMode);    // nMode : (0) 1st   (1) 2nd   (2) 1st & 2nd   (3) 3rd
	void SetDataToGrid(double dP1st[][11], double dM1st[][11], double dP2nd[][11], double dM2nd[][11], double dP3rd[][11], double dM3rd[][11],BOOL bAssignProperties, BOOL bAuto=FALSE);
	BOOL GetGridToData(double dP1st[][11], double dM1st[][11], double dP2nd[][11], double dM2nd[][11], double dP3rd[][11], double dM3rd[][11]);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropGrid)
	//}}AFX_VIRTUAL
	BOOL SetValueMode(BOOL bValueMode);

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanPaste() { return FALSE; }
	virtual BOOL CanCut() { return FALSE; }

protected:
	int  GetColHeaderNum();
	void SetEnabled(BOOL bAuto, BOOL bConc, BOOL bAssignProperties=FALSE);
	void SetHeaderTitle();
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
	//void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void UpdateParentChart(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	// Generated message map functions
protected:
	//{{AFX_MSG(CIehpYieldPropGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bConc;
	CIehpYieldPropDlgNew* m_pParent;
	BOOL m_bValueMode;
};

/////////////////////////////////////////////////////////////////////////////
// Graph

#define D_COLOR_WHITE  RGB(255,255,255)	// White.
#define D_COLOR_BLUE   RGB(0,0,192)			// Blue.
#define D_COLOR_RED    RGB(192,0,0)			// Red.
#define D_COLOR_BLACK  RGB(0,0,0)				// Black.
#define D_COLOR_GREEN  RGB(0,128,0)			// Dark Green.
#define D_COLOR_MAGNT  RGB(128,0,128)		// Magenta.
#define D_COLOR_GRAY   RGB(164,160,164)	// Medium Gray.

class CIehpYieldPropGraph : public CWnd
{
public:
	CIehpYieldPropGraph();
	virtual ~CIehpYieldPropGraph();

public:
	void   SetDefaultData(int nMethod, double dx[], double dy[], double dOp, double dOm);
	void   SetDefaultData(int nMethod, double dV_P1st[][11], double dV_M1st[][11],
												double dV_P2nd[][11], double dV_M2nd[][11],
												double dV_P3rd[][11], double dV_M3rd[][11],
												double dV_P1st_aprx[][101], double dV_M1st_aprx[][101],
												double dV_P2nd_aprx[][101], double dV_M2nd_aprx[][101],
												double dV_P3rd_aprx[][101], double dV_M3rd_aprx[][101],
												double dV_MM1st_aprx[][51], double dV_MM2nd_aprx[][51], double dV_MM3rd_aprx[][51]);
	void   SetDefaultData(int nMethod, T_IEHP_DT& arMultiData, BOOL bPmmc=FALSE);
	void   DrawStrengthGraph();
	void   DrawSurfaceGraph();
	void   DrawMultiLinearGraph();
	void   SetDraw1stCurve(BOOL bSet) { m_bDraw1stCurve = bSet; }
	void   SetDraw3rdCurve(BOOL bSet) { m_bDraw3rdCurve = bSet; }

	BOOL   m_bPmmc;
	int    m_nMethod; // 0:Strength(except Clough) 1: Strength(Clough)
										// 2: Surface(P-My) 3: Surface(P-Mz) 4: Surface(My-Mz)
	BOOL   Create(CWnd *pParentWnd, const RECT& rect, UINT nID);
	void   FitToParent(CRect rect);
	void   ClearStrengthBkgrnd(CDC* pDC);
	void   ClearSurfaceBkgrnd(CDC* pDC);
	double CalcScale(CRect rect, double dX, double dY, double dZoomRat=0.8);
	CPoint CalcOriginPoint(CRect rect, double dScale, double dX, double dY);

	void   DrawText(CDC* pDC, CString str, double dx, double dy);

	void SetCrackData(double dV_Pcrst[][11], double dV_Mcrst[][11]);

protected:
	//{{AFX_MSG(CIehpYieldPropGrid)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	// Draw strength
	double m_dx[9];
	double m_dy[9];

	double m_dOp; // delta0(+): Slip Model
	double m_dOm; // delta0(-): Slip Model

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Crack for Japan
	bool   m_bDrawCrack; 
	double m_dV_Pcrst[2][11];  // Crack Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double m_dV_Mcrst[2][11];  // Crack Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	double m_dV_P1st[2][11];  // 1st Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double m_dV_M1st[2][11];  // 1st Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double m_dV_P2nd[2][11];  // 2nd Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double m_dV_M2nd[2][11];  // 2nd Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double m_dV_P3rd[2][11];  // 3rd Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double m_dV_M3rd[2][11];  // 3rd Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)

	double m_dV_P1st_aprx[2][101];  // 1st Approximated Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double m_dV_M1st_aprx[2][101];  // 1st Approximated Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double m_dV_P2nd_aprx[2][101];  // 2nd Approximated Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double m_dV_M2nd_aprx[2][101];  // 2nd Approximated Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double m_dV_P3rd_aprx[2][101];  // 3rd Approximated Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double m_dV_M3rd_aprx[2][101];  // 3rd Approximated Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)

	double m_dV_MM1st_aprx[2][51];  // 1st Approximated My - Mz Interaction Curve Coordinate (0: y-axis,  1: z-axis)
	double m_dV_MM2nd_aprx[2][51];  // 2nd Approximated My - Mz Interaction Curve Coordinate (0: y-axis,  1: z-axis)
	double m_dV_MM3rd_aprx[2][51];  // 3rd Approximated My - Mz Interaction Curve Coordinate (0: y-axis,  1: z-axis)

	T_IEHP_DT m_aMultiLinearData;   // Multi Linear Model
	BOOL m_bDraw1stCurve; // 1st 그려줄지 여부
	BOOL m_bDraw3rdCurve; // 3rd 그려줄지 여부
};

////////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__IEHPYIELDPROPGRID_H__)
