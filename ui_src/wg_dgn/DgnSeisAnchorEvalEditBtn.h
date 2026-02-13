#if !defined(__DgnSeisAnchorEvalEditBtn_H__)
#define __DgnSeisAnchorEvalEditBtn_H__


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
class CDgnSeisAnchorEvalEditBtn : public CDrawEditBtn
{
// Construction
public:
	CDgnSeisAnchorEvalEditBtn();   // standard constructor
	virtual ~CDgnSeisAnchorEvalEditBtn();

	virtual void MakeDrawUnit(int nRatType[4], BOOL bFit = TRUE);
	BOOL SetData(std::vector<std::vector<T_BRPR_D>>& aArBearingData, T_ANEV_D& Data, BOOL bUser, BOOL bFit = TRUE);
	void Redraw(BOOL bFit = TRUE);    
	void SetDrawType(int nDrawType, int nDir, BOOL bTrans);
	void GetDrawOKInfo(BOOL& bDrawOK) { bDrawOK = m_bDrawOK;}

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
	void FullyRedraw(BOOL bFit);
		
	BOOL MakeDrawCopying(BOOL bFit);
	void MakeDrawCopyingSide(BOOL bFill, BOOL bFit);
	void ConvtoTransScale(CArray<double, double>& aX, CArray<double, double>& aY, double dCopingCenterX, double dCopingCenterY,CArray<double, double>& aModX, CArray<double, double>& aModY);
	void ConvtoTransScale(double dX, double dY, double dCopingCenterX, double dCopingCenterY, double& dModX, double& dModY);
	void ConvtoPreviewScale(CArray<double, double>& aX, CArray<double, double>& aY, double dCopingCenterX, double dCopingCenterY, double dPreviewScale, T_POINT3D& MovePos, CArray<double, double>& aMiniX, CArray<double, double>& aMiniY);
	void ConvtoPreviewScale(double dX, double dY, double dCopingCenterX, double dCopingCenterY, double dPreviewScale, T_POINT3D& MovePos, double& dMiniX, double& dMiniY);	
	void MakeDrawBrgPreview(BOOL bFit);
	void MakeDrawRefLine();
	void MakeDrawAnchor();
	void MakeDrawBearing(BOOL bFill);
	void MakeDrawPryoutProjXLine();
	void MakeDrawPryoutProjLine();
	void MakeDrawConcFailProjLine();
	void SetDimensionMembVal();
	void MakeDrawDimensionLine();
	void MakeDrawBasicDim(int nFormatIDLine, int nFormatIDArrow, int nFormatIDTxt, T_POINT2D Line[2], double dGap, CString& strVal, BOOL bRight);
	BOOL IsCa1DimCopingSize();
	void MakeDrawSideAnchorAndDim(int nCa1BearingIIdx, int nCa1BearingJIdx, int nCa1AnchorIIdx, int nCa1AnchorJIdx, double dCa1Recal);
	void MakeDrawSideAnchor(int nDir, int nCa1BearingIIdx, int nCa1BearingJIdx, int nCa1AnchorIIdx, int nCa1AnchorJIdx, double dBearingThickness,double PosCa1Dim[2]);

	void ConvColorToGray(COLORREF& Color);
	void ConvDrawToGray();
	void MakeDrawProjDimensionLine();	
	void SetRepresentConcFailBc();
	COLORREF SetColorBright(COLORREF Color, COLORREF val);

	//void MakeDrawDeviantLine();
	//void MakeDrawDeviantLine(int nIdx, double dGap);
	double ConvWndPosToCoord(int nWndPos);

	double SetFloor(double dVal);
	void Setha();
	BOOL IsFixAnchorExist();
	//void MakeDrawValue();

	CArray<T_POINT3D, T_POINT3D&> m_aSnapVertex;
	T_POINT2D m_BotCopingStPos, m_LeftCopingStPos;
	T_POINT2D m_SideCopingLineForBot[2], m_SideCopingLineForLeft[2];
	T_POINT2D m_CaCalcPos[2];

	T_POLYGON_R_GROUP m_aDeviantLine; // 사각형일 때만 동작하도록 코드를 넣었음

	BOOL  m_bMoveOrigin;
	
	COLORREF m_ColorCopying;	
	COLORREF m_ColorBearing;
	COLORREF m_ColorBearingLine;
	COLORREF m_ColorAnchor;
	COLORREF m_ColorDimensionLine;	
	COLORREF m_ColorConcFailProjLine;
	COLORREF m_ColorPryoutProjLine;
	COLORREF m_ColorPryoutProjLineRep;
	COLORREF m_ColorRefLine;
	COLORREF m_ColorRefLine2;
	COLORREF m_ColorConcFailFill;
	COLORREF m_ColorConcFailLine2;
	COLORREF m_ColorPryoutXProjLineIn;
	COLORREF m_ColorPryoutXProjLineOut;


	int m_nLastFormatID;

	double m_dBoundBox_Left;
	double m_dBoundBox_Right;
	double m_dBoundBox_Top;
	double m_dBoundBox_Bottom;
	double m_dDimGap;	
	double m_dCopingSideGap;

	BOOL m_bUser;
	T_ANEV_D m_Data;
	std::vector<std::vector<T_BRPR_D>> m_aArBearingData;		
	CDgnSeisAnchorShapeMgr m_ShapeMgr;
	CArray<double, double> m_aMaxSax;

	BOOL m_bTrans;
	int m_nDir;
	int m_nDrawType; // 0:치수, 1:ConcFail, 2:Prayout, 3:Preview
	BOOL m_bDrawOK;

/*        [ 이전 ]                       [변경 후]

             (UP)                           (R) 
       -------------                     ----------- 
       |			|                   |           |
   (L) |			| (R)          (UP) |           |--> (Down)
       |	  |	    |                   |           | 
       -------|-----                    |           | 
              |                          -----------
       	      V                              (L)
       	    (Down)
*/
	int m_nDownUp;	    // Down: 0, Up: 1     //Down: 교축 정방향, UP : 교축 역방향
	int m_nLeftRight;	// Left: 0, Right: 1  //L   : 교직 정방향, R  : 교직 역방향
	int m_nDownUpPry;		// Down: 0, Up: 1 - Pryout
	int m_nLeftRightPry;	// Left: 0, Right: 1 - Pryout

	TConcFailBcCalc m_LongiConcFailBc;
	TConcFailBcCalc m_TransConcFailBc;

	double m_dHa;
	double m_dDimLenShort, m_dDimLenLong, m_dDimLenTailGap, m_dDimLendTxtGap, m_dDimFrontGap, m_dDimOverlap, m_GapCri;		

	double m_dCopingCenterX, m_dCopingCenterY;
	BOOL m_bDrawDir;
};

#endif // !defined(__DgnSeisAnchorEvalEditBtn_H__)
