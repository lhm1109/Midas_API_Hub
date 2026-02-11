#if !defined(__SeisCvlForceDirDlgDrawEditBtn_H__)
#define __SeisCvlForceDirDlgDrawEditBtn_H__


#if _MSC_VER > 1000
#pragma once
#endif

#include "..\wg_cmd\DrawEditBtn.h"

#include "..\wg_db\DB_ST_DT.h"

#include <map>

#include "..\wg_db\UserDefType.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnDraw_SectViewPannel CDrawEditBtn
//
/////////////////////////////////////////////////////////////////////////////

enum DRAW_EDIT_BTN_TYPE
{
	DRAW_EDIT_BTN_TYPE_YZ = 0,
	DRAW_EDIT_BTN_TYPE_XY,
	DRAW_EDIT_BTN_TYPE_XZ
};

class CUnitCtrl;
class CSeisCvlForceDirDlgDrawEditBtn : public CDrawEditBtn
{
// Construction
public:
	CSeisCvlForceDirDlgDrawEditBtn();   // standard constructor
	virtual ~CSeisCvlForceDirDlgDrawEditBtn();

	virtual void MakeDrawUnit(BOOL bFit = TRUE);  
	BOOL SetData(std::vector<double>& aX, std::vector<double>& aY, BOOL bFit = TRUE);
	double FitToFixSize(double dMax, double dVal, double dFixSize);    
	void Redraw(BOOL bFit = TRUE);    
	
protected:
	DECLARE_MESSAGE_MAP()
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void MakeDrawSubClass();
	virtual void SetDrawFormat();
	virtual void StartCurrUnitSelect(CPoint CurrP); // LButtonDown
	virtual void CompleteCurrUnitSelect(CPoint CurrP); // LButtonUp
	virtual void InProgressCurrUnitSelect(CPoint CurrP);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	void ResetSnapPos();
	void CalcBoundingBox(double dX, double dY, double* dLeft = NULL, double* dRight = NULL, double* dTop = NULL, double* dBottom = NULL);
	void GetPosByCPoint(CPoint& cPt, double& dPtX, double& dPtY, int& nSnapX, int& nSnapY, BOOL bSnap = FALSE);
	void MakeCurrLinePreview(double dPtX, double dPtY, int nSnapX, int nSnapY);  
	void SetMaxVal(std::vector<double>& aX, std::vector<double>& aY);
	void MakeDrawContour();
	void MakeDrawElem();
	void MakeDrawValue();
	void Fit();
	void MakeContourLine(std::vector<double>& aX, std::vector<double>& aY, std::vector<COLORREF>& aColorDiv, int nType, std::vector<T_POINT3D>& aPnt1, std::vector<T_POINT3D>& aPnt2, std::vector<COLORREF>& aColor);    
	void MakeContourPoly(std::vector<double>& aX, std::vector<double>& aY, std::vector<COLORREF>& aColorDiv, std::vector<std::vector<T_POINT3D>>& aArPoly, std::vector<COLORREF>& aColor);

	CArray<T_POINT3D, T_POINT3D&> m_aSnapVertex;
	
	int		m_nSectSnapType; // Section정보에 Snap을 지원할지 여부  
	BOOL  m_bMoveOrigin;
	
	int m_nDisplayType; // 0 : Line, 1 : Fill

	double m_dWidthX;  
	double m_dHeightY;

	int m_nLastFormatID;

	double m_dContourWidth;
	double m_dContourHeight;

	double m_dTotalHeight;
	double m_dTotalWidth;

	double m_dBoundBox_Left;
	double m_dBoundBox_Right;
	double m_dBoundBox_Top;
	double m_dBoundBox_Bottom;
	
	std::vector<double> m_aX;
	std::vector<double> m_aY;
};

#endif // !defined(__SeisCvlForceDirDlgDrawEditBtn_H__)