#if !defined(__DgnSeisBearingPropEditBtn_H__)
#define __DgnSeisBearingPropEditBtn_H__


#if _MSC_VER > 1000
#pragma once
#endif

#include "..\wg_cmd\DrawEditBtn.h"

#include "..\wg_db\DB_ST_DT.h"

#include <map>

#include "..\wg_db\UserDefType.h"
#include "DgnSeisAnchorShapeMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnDraw_SectViewPannel CDrawEditBtn
//
/////////////////////////////////////////////////////////////////////////////

class CUnitCtrl;
class CDgnSeisBearingPropEditBtn : public CDrawEditBtn
{
// Construction
public:
	CDgnSeisBearingPropEditBtn();   // standard constructor
	virtual ~CDgnSeisBearingPropEditBtn();

	virtual void MakeDrawUnit(BOOL bFit = TRUE);	
	BOOL SetData(T_BRPR_D& Data, BOOL bFit = TRUE);    		
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
	void MakeDrawContour();	
	void FullyRedraw(BOOL bFit);
		
	void MakeDrawBearing();
	void MakeDrawAnchor();

	void GetAnchorPos(std::vector<T_POINT2D> &aAnchorPos);

	double ConvWndPosToCoord(int nWndPos);
	void MakeDrawDimensionLine();
	//void MakeDrawValue();

	CArray<T_POINT3D, T_POINT3D&> m_aSnapVertex;
	
	BOOL  m_bMoveOrigin;
	
	COLORREF m_ColorBearingBody;	
	COLORREF m_ColorBearingLine;	
	COLORREF m_ColorBolt;
	COLORREF m_ColorDimensionLine;	

	int m_nLastFormatID;

	double m_dBoundBox_Left;
	double m_dBoundBox_Right;
	double m_dBoundBox_Top;
	double m_dBoundBox_Bottom;
	
	double m_dBearingWidth;
	double m_dBearingHeight;

	CDgnSeisAnchorShapeMgr m_ShapeMgr; // 이걸 써서 그리도록 리팩토링 해야 하는데.. 시간이 없어서 그만.. .ㅠㅠ

	T_BRPR_D m_Data;
};

#endif // !defined(__DgnSeisBearingPropEditBtn_H__)
