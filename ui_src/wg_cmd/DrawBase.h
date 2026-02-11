#pragma once 

#include "DrawData.h"
#include "DrawDataFormat.h"
#include "DrawDataGroup.h"
#include "DrawFunc.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDrawBase  
{
public:
	CDrawBase();
	virtual ~CDrawBase();

public:
	UINT   m_DrawFormatKey;	
	UINT   m_LinkKey;       // 연결된 Data의 Key
	BOOL   m_bIsSelect;     // 선택 가능여부
	int    m_nSelectSubType;// 선택항목의 상세기능(0:Move불가 1:Move가능)
	BOOL   m_bIsOverLapChk; // Overlap 검토여부
	UINT   m_SnapType;      // 0:Snap미지원, 1:좌표점 Snap지원	
	double m_dZero;
	int    m_Boundary[4];

	static UINT m_nSnapDis;

public:
	static UINT GetSnapDis();
	static void SetSnapDis(UINT nSnapDis);

	virtual int  GetClassType() = 0;
	virtual int  GetFormatType() = 0;

	virtual void Draw(HDC DC, CDrawFunc* pDrawFunc, void* pDrawFormat) = 0;

	virtual void Convert(BOOL bCDC, CDrawFunc* pDrawFunc, T_PAPER_FORMAT* pPaperFormat, void* pDrawFormat) = 0;
	virtual void Move(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat) = 0; // Vector[0]: x, [1]:y

	virtual void GetCalcBoundaryR(double Boundary[4]) = 0;// Boundary[0]: x_min, [1]:y_min, [2]:x_max, [3]:y_max	
	virtual void GetCalcBoundary(int    Boundary[4]) = 0;// Boundary[0]: x_min, [1]:y_min, [2]:x_max, [3]:y_max	

	virtual BOOL IsSelectCheck(POINT sPoint, BOOL bFill=FALSE, int iTol=1) = 0; // 
	virtual BOOL IsSnapCheck(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR) = 0;	
	virtual BOOL GetCenterPoint(int nID, POINT& CenterP) = 0; // 실제 그림객체중 nID(0~)번째 객체의 중심점 좌표를 넘겨줌

	BOOL IsInSnapArea(POINT P1, POINT P2);
	BOOL IsSnapCheck_Rect(const T_LINE& rectD, const T_LINE_R& rectD_R, BOOL bIsRData, POINT currPoint, POINT& snapPoint, T_POINT& snapPointR);

	void GetBoundary(int& iXmin, int& iYmin, int& iXmax, int& iYmax, BOOL bReCalc = FALSE);
	// 입력받은 영역(Boundary)에 자진의 영역의 포함 여부 검토
	//   Boundary : 영역 ([0]: x_min, [1]:y_min, [2]:x_max, [3]:y_max)
	//   RETURN   : 포함여부 (-1:영역이 겹치지 않는 경우, 0:자신의Boundary와 Boundary의 일부가 겹치는 경우, 1:자신의Boundary가 Boundary내부에 있는 경우, 2:자신의Boundary내부에 Boundary가 있는 경우, 3:영역이 일치는 경우
	int  GetBoundaryIncludeR(double Boundary[4]); 
	int  GetBoundaryInclude (int    Boundary[4], BOOL bReCalc = FALSE); 

	void DrawOutLine(HDC DC, CDrawFunc* pDrawFunc, T_DRAW_FORMAT* pDrawFormat, int nOffset=0); // Boundary에 따른 외곽 라인을 그림

	void GetMoveVector(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat, double disVectorR[2], int disVector[2]);
	//
	BOOL IsPointOfLine(POINT p1, POINT p2, POINT tp, int nTol=1, BOOL isOnLine=TRUE);

	BOOL IsPointOfPolyLine (nrPOINT& arPoint, POINT tp, BOOL bClose=FALSE, BOOL bFill=FALSE, int nTol=1);
	BOOL IsPointOfPolyLineR(nrT_POINT& arPoint, T_POINT tp, BOOL bClose=FALSE, BOOL bFill=FALSE);

	void GetArcPolyline(T_ARC& ArcD, int nCount, nrPOINT& arPoint);
	void GetArcPolyline(POINT startP, POINT originP, double angle, int nCount, nrPOINT& arPoint);	
	void GetArcPolylineR(T_ARC_R& ArcD, int nCount, T_POLYLINE_R& arcPolyline);
	void GetArcPolylineR(T_POINT startP, T_POINT originP, double angle, int nCount, T_POLYLINE_R& arcPolyline);

	// 영역 ----------------------------------------------------------------------------------------------------
	void GetPossessionRegionR(T_POINT&        PointD,      double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_LINE_R&       LineD,       double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_CIRCLE_R&     CircleD,     double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_DONUT_R&      DonutD,      double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_ARC_R&        ArcD,        double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_POLYLINE_R&   PolylineD,   double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	//※주의 : 현재(07.12.17) Text크기 및 치수선의 지시선의 크기는 미고려하고 있습니다.
	void GetPossessionRegionR(T_TEXT_R&       TextD,       double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_DIMENSION_R&  DimensionD,  double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_ANGLE_R&      AngleD,      double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_LEADERLINE_R& LeaderLineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegionR(T_CHART_R&      ChartD,      double& dXmax, double& dXmin, double& dYmax, double& dYmin);

	void GetPossessionRegion (POINT&             PointD,      int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_LINE&         LineD,       int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_CIRCLE&       CircleD,     int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_DONUT&        DonutD,      int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_ARC&          ArcD,        int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_POLYLINE&     PolylineD,   int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	//※주의 : 현재(07.12.17) Text크기 및 치수선의 지시선의 크기는 미고려하고 있습니다.
	void GetPossessionRegion (T_TEXT&         TextD,       int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_DIMENSION&    DimensionD,  int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_ANGLE&        AngleD,      int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_LEADERLINE&   LeaderLineD, int& iXmax, int& iXmin, int& iYmax, int& iYmin);
	void GetPossessionRegion (T_CHART&        ChartD,      int& iXmax, int& iXmin, int& iYmax, int& iYmin);

	// 이동 ----------------------------------------------------------------------------------------------------
	void MoveDrawUintR(double disX, double disY, T_POINT&        PointD     );
	void MoveDrawUintR(double disX, double disY, T_LINE_R&       LineD      );
	void MoveDrawUintR(double disX, double disY, T_CIRCLE_R&     CircleD    );
	void MoveDrawUintR(double disX, double disY, T_DONUT_R&      DonutD     );
	void MoveDrawUintR(double disX, double disY, T_ARC_R&        ArcD       );
	void MoveDrawUintR(double disX, double disY, T_POLYLINE_R&   PolylineD  );
	void MoveDrawUintR(double disX, double disY, T_TEXT_R&       TextD      );
	void MoveDrawUintR(double disX, double disY, T_DIMENSION_R&  DimensionD );
	void MoveDrawUintR(double disX, double disY, T_ANGLE_R&      AngleD     );
	void MoveDrawUintR(double disX, double disY, T_LEADERLINE_R& LeaderLineD);
	void MoveDrawUintR(double disX, double disY, T_CHART_R&      ChartD     );

	void MoveDrawUint (int disX,    int disY,    POINT&         PointD     );
	void MoveDrawUint (int disX,    int disY,    T_LINE&         LineD      );
	void MoveDrawUint (int disX,    int disY,    T_CIRCLE&       CircleD    );
	void MoveDrawUint (int disX,    int disY,    T_DONUT&        DonutD     );
	void MoveDrawUint (int disX,    int disY,    T_ARC&          ArcD       );
	void MoveDrawUint (int disX,    int disY,    T_POLYLINE&     PolylineD  );
	void MoveDrawUint (int disX,    int disY,    T_TEXT&         TextD      );
	void MoveDrawUint (int disX,    int disY,    T_DIMENSION&    DimensionD );
	void MoveDrawUint (int disX,    int disY,    T_ANGLE&        AngleD     );
	void MoveDrawUint (int disX,    int disY,    T_LEADERLINE&   LeaderLineD);
	void MoveDrawUint (int disX,    int disY,    T_CHART&        ChartD     );
	void MoveDrawUint (int disX,    int disY,    T_HATCHING_D&   HatchingD  );

	// Overlap -------------------------------------------------------------------------------------------------
	static BOOL IsOverLapDrawUnit_Point   (int nOverlapType, int iTol, POINT&      DataA, POINT&      DataB);
	static BOOL IsOverLapDrawUnit_Line    (int nOverlapType, int iTol, T_LINE&     DataA, T_LINE&     DataB);
	static BOOL IsOverLapDrawUnit_Rect    (int nOverlapType, int iTol, T_LINE&     DataA, T_LINE&     DataB);
	static BOOL IsOverLapDrawUnit_Ellipse (int nOverlapType, int iTol, T_LINE&     DataA, T_LINE&     DataB);
	static BOOL IsOverLapDrawUnit_Circle  (int nOverlapType, int iTol, T_CIRCLE&   DataA, T_CIRCLE&   DataB);
	static BOOL IsOverLapDrawUnit_Donut   (int nOverlapType, int iTol, T_DONUT&    DataA, T_DONUT&    DataB);
	static BOOL IsOverLapDrawUnit_Polyline(int nOverlapType, int iTol, T_POLYLINE& DataA, T_POLYLINE& DataB, BOOL bClose = FALSE, BOOL bFill = FALSE);

};

#include "HeaderPost.h"