#if !defined(__DgnSeisFailureModeEditBtn_H__)
#define __DgnSeisFailureModeEditBtn_H__


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

struct FailureModeGraphD
{
	FailureModeGraphD()
	{
		Init();
	}
	void Init()
	{
		aX.clear();
		aY.clear();
	}
	std::vector<double> aX;
	std::vector<double> aY;
};

struct FailureModeGraphShearD
{
	FailureModeGraphShearD()
	{
		Init();
	}
	void Init()
	{
		aX.clear();
		aY.clear();
	}
	std::vector<double> aX;
	std::vector<double> aY;  
};

struct FailureModeDrawD
{
	FailureModeDrawD()
	{
		Init();
	}
	void Init()
	{
		aData.clear();
		ShearData.Init();
		bLimit = FALSE;
		dLimit = 0.0;;
	}
	std::vector<FailureModeGraphD> aData; // FailureModeGraphD 를 배열로 가지고 있지만 실제로는 1개 밖에 안 들어옴.. 처음 기획은 선이 여러개 였음.. 그래서 그 구조 그대로 유지함.. 선이 여러개 들어오는 상황도 코드는 넣었으나 테스트 해보진 않았음
	FailureModeGraphShearD ShearData;
	BOOL bLimit;
	double dLimit;
};

struct FMEditBtnLineDraw
{
	FMEditBtnLineDraw()
	{
		Init();
	}
	void Init()
	{
		aX.clear();
		aY.clear();
		Color = RGB(0,0,0);    
		nLineType = 0;
	}
	std::vector<double> aX;
	std::vector<double> aY;
	COLORREF Color;    
	int nLineType; // 0:두꺼운선, 1:선, 2:점선
};

struct FMEditBtnDotDraw
{
	FMEditBtnDotDraw()
	{
		Init();
	}
	void Init()
	{
		dX = 0.0;
		dY = 0.0;
	}
	double dX;
	double dY;
};

struct FMEditBtnRectDraw
{
	FMEditBtnRectDraw()
	{
		Init();
	}
	void Init()
	{
		dX = 0.0;
		dY = 0.0;
	}
	double dX;
	double dY;
};

struct FailureModeGraphTxtDraw
{
	FailureModeGraphTxtDraw()
	{
		Init();
	}
	void Init()
	{
		nGuideType = 1;
		strX1 = _T("x");
		strY1 = _T("y");
		strX2 = _T("");
		strY2 = _T("");
	}
	int nGuideType; // 0 : GuideText를 쓰지 않음, 1 : strX1과 strY1만 씀. 2 : string을 모두 써서 A=B 형태로 출력 함
	CString strX1;
	CString strY1;
	CString strX2;
	CString strY2;
};

struct FMEditBtnGuideTxtDraw
{
	FMEditBtnGuideTxtDraw()
	{
		Init();
	}
	void Init()
	{
		dX = 0.0;
		dY = 0.0;

		bDrawXGuideLine = TRUE;
		bDrawYGuideLine = TRUE;

		TxtColor = RGB(236, 0, 0);  
		GuideColor = RGB(236, 0, 0);      
		GuideTxt.Init();
	}
	double dX;  
	double dY;
	
	BOOL bDrawXGuideLine;
	BOOL bDrawYGuideLine;

	COLORREF TxtColor;  
	COLORREF GuideColor;
	FailureModeGraphTxtDraw GuideTxt;
};

class CUnitCtrl;
class CDgnSeisFailureModeEditBtn : public CDrawEditBtn
{
// Construction
public:
	CDgnSeisFailureModeEditBtn();   // standard constructor
	virtual ~CDgnSeisFailureModeEditBtn();

	virtual void MakeDrawUnit(BOOL bFit = TRUE);
	BOOL SetData(FailureModeDrawD& Data, BOOL bFit = TRUE);    
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
	void MakeDrawContour();
	void MakeDrawValLine();  
	void MakeDrawDot();
	void MakeDrawRect();
	void MakeDrawAxisValLine();
	void MakeDrawAxisVal();
	void MakeDrawGuideTextDelta(double dX, double dY, COLORREF Color, double dLeftInc = 0.0, double dRightInc = 0.0, double dTopInc = 0.0, double dBotInc = 0.0);
	void MakeDrawGuideText();
	void MakeDrawGuideTextOld();
	void MakeDrawGuideLineOld();
	void MakeDrawFlame();
	void MakeDrawGuideBox();
	void MakeDrawGuideBoxAll();
	void MakeDrawGuideBoxEachOne(int nType);
	void MakeDrawData();
	void MakeDrawDataLimitLine();
	void MakeDrawAxisText();
	BOOL FindIntersect(double dP1X, double dP1Y, double dP2X, double dP2Y, std::vector<double>& aX, std::vector<double>& aY, double &dXOut, double &dYOut );
	void SetAxisValAndMax();  
	void MakeAxisValType2(const int nDivNum, double dMaxVal, std::vector<double>& aAxisVal);
	void MakeAxisValType1(const int nDivNum, double dMaxVal, std::vector<double>& aAxisVal);
	void DrawDataFitToFixSize();
	FailureModeGraphTxtDraw GetTxtDraw(int nType, int nIdx, BOOL bIntersect);
	BOOL DataChangeByLimit();
	//void MakeDrawValue();

	CArray<T_POINT3D, T_POINT3D&> m_aSnapVertex;
	
	int		m_nSectSnapType; // Section정보에 Snap을 지원할지 여부  
	BOOL  m_bMoveOrigin;
	
	int m_nDisplayType; // 0 : Line, 1 : Fill
	
	std::vector<BOOL> m_aChangeByLimit;
	std::vector<double> m_aOrgLastX;
	std::vector<double> m_aOrgLastY;

	std::vector<double> m_aAxisValX;
	std::vector<double> m_aAxisValY;
	std::vector<double> m_aAxisValXDisp;
	std::vector<double> m_aAxisValYDisp;

	double m_dMaxX;
	double m_dMaxY;
	COLORREF m_ColorFlame;
	COLORREF m_ColorAxisValLine;
	COLORREF m_ColorGuide;
	COLORREF m_ColorLimit;
	COLORREF m_ColorGuideTxt;
	COLORREF m_ColorBending;
	COLORREF m_ColorShear;

	int m_nLastFormatID;

	double m_dValAreaWidth;
	double m_dValAreaHeight;

	double m_dFlameMargineX;
	double m_dFlameMargineY;
	//double m_dTotalHeight;
	//double m_dTotalWidth;

	double m_dBoundBox_Left;
	double m_dBoundBox_Right;
	double m_dBoundBox_Top;
	double m_dBoundBox_Bottom;
	
	BOOL m_bBending;
	BOOL m_bShear;
	int m_nYValExpType; // Y최대값이 string 으로 몇 글자 인지에 대한 Type.. Y축에 적히는 글씨를 얼마나 좌로 밀어낼지를 결정함..  0=3글자이하, 1=4~5글자, 2=6글자(지수표현)

	FailureModeDrawD m_Data;
	std::vector<FMEditBtnLineDraw> m_aLineDraw;
	std::vector<FMEditBtnDotDraw> m_aDotDraw;
	std::vector<FMEditBtnRectDraw> m_aRectDraw;
	std::vector<FMEditBtnGuideTxtDraw> m_aGuideTxtDraw;
};

#endif // !defined(__DgnSeisFailureModeEditBtn_H__)
