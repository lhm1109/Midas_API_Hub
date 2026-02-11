#pragma once

#include "DrawDataFormat.h"
#include "DrawDataObject.h"

#include "HeaderPre.h"

///////////////////////////////////////////////////////////////////////
//  CDrawFunc                                            
//  그림을 그리기 위한 기초 Class로 HDC에 직접 그림을 그릴때 사용    

#define   ARCPOLY_MINSIZE   3   // Polyline을 Pathline으로 변환시 추가되는 호좌표의 초소개수
#define   ARCPOLY_MAXANGLE  5.0 // Polyline을 Pathline으로 변환시 최대각도

class __MY_EXT_CLASS__ CDrawFunc  
{
public:
	CDrawFunc();
	virtual ~CDrawFunc();

protected:
	BOOL   m_bEMF;
	int    m_nMapMode;
	double m_Zero;
	int    m_nFontAddSize_RZero;
	BOOL   m_bRectTrPolyDraw; // DC에 Rect를 그릴때 Polygon으로 그릴지 여부
	// ※ 일반적으로 Dlg상에 DC에 HDC::Rectangle함수로 사각형을 그리는 것과 HDC::LineTo로 사각형을 그리는 것과 크기에서 차이가 발생합니다.
	//    동일 산상의 직선과 사각형이 일치하게(겹치게) 나타나지 않는 이유가 해당 사항입니다. 
	//    그차이는 선의 두께 정도로 차이가 발생합니다.
	//    따라서 HDC::Rectangle대신 Polygon정보로 변환하여 그려주는 옵션입니다.
	// Chart기능사용시에는 m_bRectTrPolyDraw==TRUE하여 작업을 수행하고 기존값으로 돌려줍니다.
	// Hatching기능사용시에는 m_bRectTrPolyDraw옵션여부에 따라 영역이 달라지므로 주의하여야 함
public:  
	static int mathArcForm3Point(double p1[2], double p2[2], double p3[2], double CenterP[2], double& dRadius, double& dStartAngle, double& dSweepAngle, double& dBulge);
	static double mathDistanceFromIntersectPointToLine2D(double line_i[2], double line_j[2], double point[2], int& nState);
	static BOOL IsOverLap_Polyline(const int nData1, double polyLine1[][2], const int nData2, double polyLine2[][2]);

	void ResetVariables();
	BOOL IsEMF()				{return m_bEMF;}
	void SetIsEMF(BOOL bEMF)	{ m_bEMF = bEMF; }	
	BOOL IsRectTrPolyDraw()		{ return m_bRectTrPolyDraw; }
	void SetIsRectTrPolyDraw(BOOL bRectTrPolyDraw) { m_bRectTrPolyDraw = bRectTrPolyDraw; }
	void SetMapMode(int nMapMode) { m_nMapMode = nMapMode; }
	void SetFontAddSize_RZero(int nFontAddSize_RZero) { m_nFontAddSize_RZero = nFontAddSize_RZero; }

	// Point ---------------------------
	//   DC:그림을 그릴 객체   PointColor:점색깔   Width:점두께   x,y:점의 좌표
	void DrawPoint(HDC DC, COLORREF clrPoint, int Width, int x, int y, EN_POINT_TYPE nType = EN_POINT_TYPE_DOT, BOOL bFill = FALSE, COLORREF clrFill = 0);
	//   DC:그림을 그릴 객체   PointColor:전색깔   Width:점두께   Point : 점의 좌표
	void DrawPoint(HDC DC, COLORREF clrPoint, int Width, POINT Point, EN_POINT_TYPE nType = EN_POINT_TYPE_DOT, BOOL bFill = FALSE, COLORREF clrFill = 0);  
	//   DC:그림을 그릴 객체   nPenStyle:선모양   PointColor:선색깔   Width:선두께   arLine:선의 시작점과 끝점의 좌표 리스트
	void DrawPoint(HDC DC, COLORREF clrPoint, int Width, nrPOINT& arPoint, EN_POINT_TYPE nType = EN_POINT_TYPE_DOT, BOOL bFill = FALSE, COLORREF clrFill = 0);
	// T_DRAW_FORMAT 이용하여 DC에 Point을 그림
	void DrawPoint(HDC DC, T_DRAW_FORMAT Format, int x, int y);
	// T_DRAW_FORMAT 이용하여 DC에 Point을 그림
	void DrawPoint(HDC DC, T_DRAW_FORMAT Format, POINT Point);
	// T_DRAW_FORMAT 이용하여 DC에 Point을 그림
	void DrawPoint(HDC DC, T_DRAW_FORMAT Format, nrPOINT& arPoint);

	// Line ----------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   clrline:선색깔   Width:선두께   x1,y1,x2,y2:선의 시작점과 끝점의 좌표   nPenSubStyle:end cap, joint 형식
	void DrawLine(HDC DC, int nPenStyle, COLORREF clrline, int Width, int x1, int y1, int x2, int y2, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   clrline:선색깔   Width:선두께   Line : 선의 시작점과 끝점의 좌표   nPenSubStyle:end cap, joint 형식
	void DrawLine(HDC DC, int nPenStyle, COLORREF clrline, int Width, T_LINE Line, int nPenSubStyle = -1);  
	//   DC:그림을 그릴 객체   nPenStyle:선모양   clrline:선색깔   Width:선두께   arLine:선의 시작점과 끝점의 좌표 리스트   nPenSubStyle:end cap, joint 형식
	void DrawLine(HDC DC, int nPenStyle, COLORREF clrline, int Width, nrT_LINE& arLine, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 Line을 그림
	void DrawLine(HDC DC, T_DRAW_FORMAT Format, int Width, int x1, int y1, int x2, int y2);
	// T_DRAW_FORMAT 이용하여 DC에 Line을 그림
	void DrawLine(HDC DC, T_DRAW_FORMAT Format, T_LINE Line);
	// T_DRAW_FORMAT 이용하여 DC에 Line을 그림
	void DrawLine(HDC DC, T_DRAW_FORMAT Format, nrT_LINE& arLine);

	// Circle --------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   clrFill:채우기색깔   bFill:채우기여부   Width:선두께   Circle:원의 위치정보   nPenSubStyle:end cap, joint 형식
	void DrawCircle(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, T_CIRCLE Circle, int nPenSubStyle = -1);
	// DC에 여러개의 Circle을 그림
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   arCircle:원의 위치정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawCircle(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, nrT_CIRCLE& arCircle, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 Circle을 그림
	void DrawCircle(HDC DC, T_DRAW_FORMAT Format, T_CIRCLE Circle);
	// T_DRAW_FORMAT 이용하여 DC에 Circle을 그림
	void DrawCircle(HDC DC, T_DRAW_FORMAT Format, nrT_CIRCLE& arCircle);

	// Donut --------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   Donut:원의 위치정보   nPenSubStyle:end cap, joint 형식
	void DrawDonut(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, T_DONUT Donut, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   arDonut:원의 위치정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawDonut(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, nrT_DONUT& arDonut, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 Donut을 그림
	void DrawDonut(HDC DC, T_DRAW_FORMAT Format, T_DONUT Donut);
	// T_DRAW_FORMAT 이용하여 DC에 Donut을 그림
	void DrawDonut(HDC DC, T_DRAW_FORMAT Format, nrT_DONUT& arDonut);

	// Ellipse -------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   clrFill:채우기색깔   bFill:채우기여부   Width:선두께   x1,y1,x2,y2:타원에 접하는 사격형의 좌표정보   nPenSubStyle:end cap, joint 형식
	void DrawEllipse (HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, int x1, int y1, int x2, int y2, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   clrFill:채우기색깔   bFill:채우기여부   Width:선두께   Line:타원에 접하는 사격형의 좌표정보   nPenSubStyle:end cap, joint 형식
	void DrawEllipse (HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, T_LINE Line, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   clrFill:채우기색깔   bFill:채우기여부   Width:선두께   arLine:타원에 접하는 사격형의 좌표정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawEllipse(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, nrT_LINE& arLine, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 타원을 그림
	void DrawEllipse(HDC DC, T_DRAW_FORMAT Format, int x1, int y1, int x2, int y2);
	// T_DRAW_FORMAT 이용하여 DC에 타원을 그림
	void DrawEllipse(HDC DC, T_DRAW_FORMAT Format, T_LINE Line);
	// T_DRAW_FORMAT 이용하여 DC에 타원을 그림
	void DrawEllipse(HDC DC, T_DRAW_FORMAT Format, nrT_LINE& arLine);

	// Arc -----------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   Arc:호의 위치정보   nPenSubStyle:end cap, joint 형식
	void DrawArc(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startType, EN_ARROW_TYPE endType, T_ARC Arc, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   arArc:호의 위치정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawArc(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startType, EN_ARROW_TYPE endType, nrT_ARC& arArc, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 호를 그림
	void DrawArc(HDC DC, T_DRAW_FORMAT Format, T_ARC Arc);
	// T_DRAW_FORMAT 이용하여 DC에 호를 그림
	void DrawArc(HDC DC, T_DRAW_FORMAT Format, nrT_ARC& arArc);

	// Rect ----------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   x1,y1,x2,y2:사격형의 좌표정보   nPenSubStyle:end cap, joint 형식
	void DrawRect(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, int x1, int y1, int x2, int y2, int nPenSubStyle = -1);  
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   Line:사격형의 좌표정보   nPenSubStyle:end cap, joint 형식
	void DrawRect(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, T_LINE Line, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   Line:사격형의 좌표정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawRect(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, nrT_LINE& arLine, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 사각형을 그림
	void DrawRect(HDC DC, T_DRAW_FORMAT Format, int x1, int y1, int x2, int y2);
	// T_DRAW_FORMAT 이용하여 DC에 사각형을 그림
	void DrawRect(HDC DC, T_DRAW_FORMAT Format, T_LINE Line);
	// T_DRAW_FORMAT 이용하여 DC에 사각형을 그림
	void DrawRect(HDC DC, T_DRAW_FORMAT Format, nrT_LINE& arLine);

	// Polygon -------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   PolyLine:다각형 구성 좌표정보   nPenSubStyle:end cap, joint 형식
	void DrawPolygon(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, T_POLYLINE PolyLine, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   PolyLine:다각형 구성 좌표정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawPolygon(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, nrT_POLYLINE& arPolyLine, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 다각형을 그림
	void DrawPolygon(HDC DC, T_DRAW_FORMAT Format, T_POLYLINE PolyLine);
	// T_DRAW_FORMAT 이용하여 DC에 다각형을 그림
	void DrawPolygon(HDC DC, T_DRAW_FORMAT Format, nrT_POLYLINE& arPolyLine);

	// Polyline ------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   PolyLine:꺽은선 구성 좌표정보   nPenSubStyle:end cap, joint 형식
	void DrawPolyline(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, T_POLYLINE PolyLine, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   PolyLine:꺽은선 구성 좌표정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawPolyline(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, nrT_POLYLINE& arPolyLine, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 꺽은선을 그림
	void DrawPolyline(HDC DC, T_DRAW_FORMAT Format, T_POLYLINE PolyLine);
	// T_DRAW_FORMAT 이용하여 DC에 꺽은선을 그림
	void DrawPolyline(HDC DC, T_DRAW_FORMAT Format, nrT_POLYLINE& arPolyLine);

	// Image ---------------------------
	//   DC:r그림을 그릴 객체, img : 그려질 이미지
	void DrawImage(HDC DC, HBITMAP img, RECT rtCanvas, BOOL bStreach = FALSE);

	// Text ----------------------------
	//   DC:그림을 그릴 객체   fontColor:글색깔   fillColor:바탕색깔   Size:글자크기   strFont:Font종류   enPosiType:글자기준점의 위치   dgnText:글자정보   nEscapement:글자 회전각
	void DrawText(HDC DC, COLORREF fontColor, COLORREF clrFill, BOOL bFill, int fontSize, TCHAR* szFont, EN_POSITION_TYPE enPosiType, T_TEXT dgnText, int nEscapement = 0, BOOL bHalfWidth = FALSE);
	//   DC:그림을 그릴 객체   fontColor:글색깔   fillColor:바탕색깔   Size:글자크기   strFont:Font종류   enPosiType:글자기준점의 위치   arText:글자정보 리스트   nEscapement:글자 회전각
	void DrawText(HDC DC, COLORREF fontColor, COLORREF clrFill, BOOL bFill, int fontSize, TCHAR* szFont, EN_POSITION_TYPE enPosiType, nrT_TEXT& arText, int nEscapement = 0, BOOL bHalfWidth = FALSE);
	// T_TEXT_FORMAT 이용하여 DC에 Text를 그림
	void DrawText(HDC DC, T_TEXT_FORMAT textFormat, T_TEXT dgnText);
	// T_TEXT_FORMAT 이용하여 DC에 Text를 그림
	void DrawText(HDC DC, T_TEXT_FORMAT textFormat, nrT_TEXT& arText);

	// Arrow ----------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   startArrowType:시작점 화살표모양   endArrowType:끝점 화살표모양   x1,y1:시작점 좌표   x2,y2:끝점좌료   nPenSubStyle:end cap, joint 형식
	void DrawArrow(HDC DC, int nPenStyle, COLORREF clrLine, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startType, EN_ARROW_TYPE endType, int x1, int y1, int x2, int y2, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   startArrowType:시작점 화살표모양   endArrowType:끝점 화살표모양   Line:화살표의 시작점과 끝점 정보   nPenSubStyle:end cap, joint 형식
	void DrawArrow(HDC DC, int nPenStyle, COLORREF clrLine, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startType, EN_ARROW_TYPE endType, T_LINE Line, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   startArrowType:시작점 화살표모양   endArrowType:끝점 화살표모양   arLine:화살표의 시작점과 끝점 정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawArrow(HDC DC, int nPenStyle, COLORREF clrLine, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startType, EN_ARROW_TYPE endType, nrT_LINE& arLine, int nPenSubStyle = -1);
	// T_DRAW_FORMAT 이용하여 DC에 화살표를 그림
	void DrawArrow(HDC DC, T_DRAW_FORMAT Format, int x1, int y1, int x2, int y2);
	// T_DRAW_FORMAT 이용하여 DC에 화살표를 그림
	void DrawArrow(HDC DC, T_DRAW_FORMAT Format, T_LINE Line);
	// T_DRAW_FORMAT 이용하여 DC에 화살표를 그림
	void DrawArrow(HDC DC, T_DRAW_FORMAT Format, nrT_LINE& arLine);

	// ArrowPolyline ------------------------
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   startArrowType:시작점 화살표모양   endArrowType:끝점 화살표모양   PolyLine:꺽은선 구성 좌표정보   nPenSubStyle:end cap, joint 형식
	void DrawArrowPolyline(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, T_POLYLINE PolyLine, int nPenSubStyle = -1);
	//   DC:그림을 그릴 객체   nPenStyle:선모양   lineColor:선색깔   fillColor:채우기색깔   bFill:채우기여부   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   startArrowType:시작점 화살표모양   endArrowType:끝점 화살표모양   PolyLine:꺽은선 구성 좌표정보 리스트   nPenSubStyle:end cap, joint 형식
	void DrawArrowPolyline(HDC DC, int nPenStyle, COLORREF clrLine, COLORREF clrFill, BOOL bFill, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE startArrowType, EN_ARROW_TYPE endArrowType, nrT_POLYLINE& arPolyLine, int nPenSubStyle = -1);
	// T_DGN_DRAW_FORMAT 이용하여 pDC에 화살표를 가진 꺽은선을 그림
	void DrawArrowPolyline(HDC DC, T_DRAW_FORMAT drawFormat, T_POLYLINE dgnPolyLine);
	// T_DGN_DRAW_FORMAT 이용하여 pDC에 화살표를 가진 여러개의 꺽은선을 그림
	void DrawArrowPolyline(HDC DC, T_DRAW_FORMAT drawFormat, nrT_POLYLINE& arPolyLine);

	// Dimension ----------------------------
	//   DC:그림을 그릴 객체   lineColor:선색깔   fontSize:글자크기   strFont:Font종류   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   arrowType:화살표모양   
	//   dRTextSpace: Text와 선과의 떨어진 비율   dRDimSpace: 치수선과 치수점과 떨어진 비율   dRGuideIn:지시선의 내측길이의 비율   dRGuideOut:지시선의 외측길이의 비율   dimType:치수의 기입위치 종류   
	//   bSubDimType:문자열이 길경우 하위 치수선을 사용할지 여부   dimType_Sub:문자열이 길경우 사용할 하위치수선   dgnDimension:치수정보   bFontScale:FontSize에 대한 비율로 그릴지의 여부   bGuideLine_P1,bGuideLine_P2:GuideLine을 그릴지의 여부
	//   bFontHalfWidth:폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	void DrawDimension(HDC DC, COLORREF fontColor, COLORREF clrLine, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, 
		double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, T_DIMENSION dgnDimension, BOOL bFontScale = TRUE, BOOL bMainLine = TRUE, BOOL bGuideLine_P1 = TRUE, BOOL bGuideLine_P2 = TRUE, BOOL bFontHalfWidth = FALSE, double dOutTypeRate_L = 2.0, double dOutTypeRate_R = 2.0);
	//   DC:그림을 그릴 객체   lineColor:선색깔   fontSize:글자크기   strFont:Font종류   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   arrowType:화살표모양   
	//   dRTextSpace: Text와 선과의 떨어진 비율   dRDimSpace: 치수선과 치수점과 떨어진 비율   dRGuideIn:지시선의 내측길이의 비율   dRGuideOut:지시선의 외측길이의 비율   dimType:치수의 기입위치 종류   
	//   bSubDimType:문자열이 길경우 하위 치수선을 사용할지 여부   dimType_Sub:문자열이 길경우 사용할 하위치수선   arDimension:치수정보 리스트   bFontScale:FontSize에 대한 비율로 그릴지의 여부   bGuideLine_P1,bGuideLine_P2:GuideLine을 그릴지의 여부
	//   bFontHalfWidth:폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	void DrawDimension(HDC DC, COLORREF fontColor, COLORREF clrLine, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, 
		double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, nrT_DIMENSION& arDimension, BOOL bFontScale = TRUE, BOOL bMainLine = TRUE, BOOL bGuideLine_P1 = TRUE, BOOL bGuideLine_P2 = TRUE, BOOL bFontHalfWidth = FALSE, double dOutTypeRate_L = 2.0, double dOutTypeRate_R = 2.0);
	// T_DIM_FORMAT을 이용하여 DC에 치수선을 그림
	void DrawDimension(HDC DC, T_DIM_FORMAT dimFormat, T_DIMENSION dgnDimension);
	// T_DIM_FORMAT을 이용하여 DC에 치수선을 그림
	void DrawDimension(HDC DC, T_DIM_FORMAT dimFormat, nrT_DIMENSION& arDimension);

	// Dim_Angle ----------------------------
	//   DC:그림을 그릴 객체   lineColor:선색깔   fontSize:글자크기   strFont:Font종류   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   arrowType:화살표모양   
	//   dRTextSpace: Text와 선과의 떨어진 비율   dRDimSpace: 치수선과 치수점과 떨어진 비율   dRGuideIn:지시선의 내측길이의 비율   dRGuideOut:지시선의 외측길이의 비율   dimType:치수의 기입위치 종류   
	//   bSubDimType:문자열이 길경우 하위 치수선을 사용할지 여부   dimType_Sub:문자열이 길경우 사용할 하위치수선   dgnAngle:치수정보   bFontScale:FontSize에 대한 비율로 그릴지의 여부   bGuideLine_P1,bGuideLine_P2:GuideLine을 그릴지의 여부
	//   bFontHalfWidth:폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	void DrawDimAngle(HDC DC, COLORREF fontColor, COLORREF clrLine, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, 
		double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, T_ANGLE dgnAngle, BOOL bFontScale = TRUE, BOOL bMainLine = TRUE, BOOL bGuideLine_P1 = TRUE, BOOL bGuideLine_P2 = TRUE, BOOL bFontHalfWidth = FALSE);
	//   DC:그림을 그릴 객체   lineColor:선색깔   fontSize:글자크기   strFont:Font종류   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   arrowType:화살표모양   
	//   dRTextSpace: Text와 선과의 떨어진 비율   dRDimSpace: 치수선과 치수점과 떨어진 비율   dRGuideIn:지시선의 내측길이의 비율   dRGuideOut:지시선의 외측길이의 비율   dimType:치수의 기입위치 종류   
	//   bSubDimType:문자열이 길경우 하위 치수선을 사용할지 여부   dimType_Sub:문자열이 길경우 사용할 하위치수선   arAngle:치수정보 리스트   bFontScale:FontSize에 대한 비율로 그릴지의 여부   bGuideLine_P1,bGuideLine_P2:GuideLine을 그릴지의 여부
	//   bFontHalfWidth:폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	void DrawDimAngle(HDC DC, COLORREF fontColor, COLORREF clrLine, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, 
		double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, nrT_ANGLE& arAngle, BOOL bFontScale = TRUE, BOOL bMainLine = TRUE, BOOL bGuideLine_P1 = TRUE, BOOL bGuideLine_P2 = TRUE, BOOL bFontHalfWidth = FALSE);
	// T_DIM_FORMAT을 이용하여 DC에 Angle치수선을 그림
	void DrawDimAngle(HDC DC, T_DIM_FORMAT dimFormat, T_ANGLE dgnAngle);
	// T_DIM_FORMAT을 이용하여 DC에 Angle치수선을 그림
	void DrawDimAngle(HDC DC, T_DIM_FORMAT dimFormat, nrT_ANGLE& arAngle);
	
	// Dim_Leader ----------------------------
	//   DC:그림을 그릴 객체   lineColor:선색깔   fontSize:글자크기   strFont:Font종류   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   arrowType:화살표모양   
	//   dRTextSpace: Text와 선과의 떨어진 비율   dRDimSpace: 치수선과 치수점과 떨어진 비율   dRGuideIn:지시선의 내측길이의 비율   dRGuideOut:지시선의 외측길이의 비율   dimType:치수의 기입위치 종류   
	//   bSubDimType:문자열이 길경우 하위 치수선을 사용할지 여부   dimType_Sub:문자열이 길경우 사용할 하위치수선   dgnAngle:치수정보   bFontScale:FontSize에 대한 비율로 그릴지의 여부   bGuideLine_P1,bGuideLine_P2:GuideLine을 그릴지의 여부
	//   bFontHalfWidth:폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	// ※REMARK : 하위 치수선이 없음 따라서 bSubDimType, dimType_Sub은 사용되지 않음	
	void DrawDimLeaderLine(HDC DC, COLORREF fontColor, COLORREF clrLine, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, 
		double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, T_LEADERLINE& dgnAngle, BOOL bFontScale = TRUE, BOOL bGuideLine_P1 = TRUE, BOOL bGuideLine_P2 = TRUE, BOOL bFontHalfWidth = FALSE);
	//   DC:그림을 그릴 객체   lineColor:선색깔   fontSize:글자크기   strFont:Font종류   Width:선두께   dRWidth:화살표폭의 선두께에 대한 비율   dRHeight:화살표높이의 선두께에 대한 비율   arrowType:화살표모양   
	//   dRTextSpace: Text와 선과의 떨어진 비율   dRDimSpace: 치수선과 치수점과 떨어진 비율   dRGuideIn:지시선의 내측길이의 비율   dRGuideOut:지시선의 외측길이의 비율   dimType:치수의 기입위치 종류   
	//   bSubDimType:문자열이 길경우 하위 치수선을 사용할지 여부   dimType_Sub:문자열이 길경우 사용할 하위치수선   arAngle:치수정보 리스트   bFontScale:FontSize에 대한 비율로 그릴지의 여부   bGuideLine_P1,bGuideLine_P2:GuideLine을 그릴지의 여부
	//   bFontHalfWidth:폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	// ※REMARK : 하위 치수선이 없음 따라서 bSubDimType, dimType_Sub은 사용되지 않음
	void DrawDimLeaderLine(HDC DC, COLORREF fontColor, COLORREF clrLine, int fontSize, TCHAR* szFont, int Width, double dRWidth, double dRHeight, EN_ARROW_TYPE arrowType, 
		double dRTextSpace, double dRDimSpace, double dRGuideIn, double dRGuideOut, EN_DIM_TYPE dimType, BOOL bSubDimType, EN_DIM_TYPE dimType_Sub, nrT_LEADERLINE& arAngle, BOOL bFontScale = TRUE, BOOL bGuideLine_P1 = TRUE, BOOL bGuideLine_P2 = TRUE, BOOL bFontHalfWidth = FALSE);
	// T_DIM_FORMAT을 이용하여 DC에 지시선을 그림
	void DrawDimLeaderLine(HDC DC, T_DIM_FORMAT dimFormat, T_LEADERLINE& dgnAngle);
	// T_DIM_FORMAT을 이용하여 DC에 지시선을 그림
	void DrawDimLeaderLine(HDC DC, T_DIM_FORMAT dimFormat, nrT_LEADERLINE& arAngle);

	// Chart ----------------------------
	// T_CHART_FORMAT을 이용하여 DC에 Chart를 그림
	void DrawChart(HDC DC, T_CHART_FORMAT chartFormat, T_CHART& dgnChart);
	// T_CHART_FORMAT을 이용하여 DC에 Chart를 그림
	void DrawChart(HDC DC, T_CHART_FORMAT chartFormat, nrT_CHART& arChart);

	// Hatching ------------------------
	//   DC:그림을 그릴 객체   HatchingType:Hatching형식   HatchingColor:선색깔   bHatching:Hatching여부   Width:선두께   dgnHatching:Hatching 구성 정보
	void DrawHatching(HDC DC, EN_HATCHING_TYPE HatchingType, COLORREF HatchingColor, BOOL bHatching, int Width, T_HATCHING_D& dgnHatching);
	// T_DRAW_FORMAT 이용하여 DC에 Hatching정보를 그림
	void DrawHatching(HDC DC, T_DRAW_FORMAT Format, T_HATCHING_D& dgnHatching);  
	
	// 변환 ---------------------------------------------------------------------------------------------------
	void ConvertPaperFormatMirror(T_PAPER_FORMAT& Format, RECT Canvas, int nMirror_Org, int nMirror_Change);
	void ConvertToPOINT          (T_PAPER_FORMAT Format, T_POINT        point_In,      POINT&          point_Out);
	void ConvertToTPoint         (T_PAPER_FORMAT Format, POINT          point_In,		 T_POINT&        point_Out);
	void ConvertToLine           (T_PAPER_FORMAT Format, T_LINE_R       line_In,       T_LINE&         line_Out);
	void ConvertToLineR          (T_PAPER_FORMAT Format, T_LINE         line_In,       T_LINE_R&       line_Out);
	void ConvertToCircle         (T_PAPER_FORMAT Format, T_CIRCLE_R     circle_In,     T_CIRCLE&       circle_Out);  
	void ConvertToCircleR        (T_PAPER_FORMAT Format, T_CIRCLE       circle_In,     T_CIRCLE_R&     circle_Out);
	void ConvertToDonut			 (T_PAPER_FORMAT Format, T_DONUT_R      donut_In,      T_DONUT&        donut_Out);  
	void ConvertToDonutR		 (T_PAPER_FORMAT Format, T_DONUT        donut_In,      T_DONUT_R&      donut_Out);
	void ConvertToArc            (T_PAPER_FORMAT Format, T_ARC_R        arc_In,        T_ARC&          arc_Out);
	void ConvertToArcR           (T_PAPER_FORMAT Format, T_ARC          arc_In,        T_ARC_R&        arc_Out);
	void ConvertToPolyline       (T_PAPER_FORMAT Format, T_POLYLINE_R   polyline_In,   T_POLYLINE&     polyline_Out);
	void ConvertToPolylineR      (T_PAPER_FORMAT Format, T_POLYLINE     polyline_In,   T_POLYLINE_R&   polyline_Out);
	void ConvertToPolygon        (T_PAPER_FORMAT Format, T_POLYLINE_R   polygon_In,    T_POLYLINE&     polygon_Out);
	void ConvertToPolygonR       (T_PAPER_FORMAT Format, T_POLYLINE     polygon_In,    T_POLYLINE_R&   polygon_Out);
	void ConvertToText           (T_PAPER_FORMAT Format, T_TEXT_R       text_In,       T_TEXT&         text_Out);
	void ConvertToTextR          (T_PAPER_FORMAT Format, T_TEXT         text_In,       T_TEXT_R&       text_Out);
	void ConvertToDimension      (T_PAPER_FORMAT Format, T_DIMENSION_R  dimension_In,  T_DIMENSION&    dimension_Out);
	void ConvertToDimensionR     (T_PAPER_FORMAT Format, T_DIMENSION    dimension_In,  T_DIMENSION_R&  dimension_Out);
	void ConvertToAngle          (T_PAPER_FORMAT Format, T_ANGLE_R      angle_In,      T_ANGLE&        angle_Out);  
	void ConvertToAngleR         (T_PAPER_FORMAT Format, T_ANGLE        angle_In,      T_ANGLE_R&      angle_Out);
	void ConvertToLeaderLine     (T_PAPER_FORMAT Format, T_LEADERLINE_R leader_In,     T_LEADERLINE&   leader_Out);  
	void ConvertToLeaderLineR    (T_PAPER_FORMAT Format, T_LEADERLINE   leader_In,     T_LEADERLINE_R& leader_Out);
	void ConvertToChart          (T_PAPER_FORMAT Format, T_CHART_FORMAT chartFormat,   T_CHART_R chart_In, T_CHART& chart_Out);
	void ConvertToChartR         (T_PAPER_FORMAT Format, T_CHART        chart_In,      T_CHART_R&      chart_Out);
	void ConvertToArPOINT       (T_PAPER_FORMAT Format, nrT_POINT& arPoint_In,			nrPOINT& arPoint_Out);
	void ConvertToArTPoint      (T_PAPER_FORMAT Format, nrPOINT& arPoint_In,				nrT_POINT& arPoint_Out);
	void ConvertToArLine        (T_PAPER_FORMAT Format, nrT_LINE_R& arLine_In,			nrT_LINE& arLine_Out);
	void ConvertToArLineR       (T_PAPER_FORMAT Format, nrT_LINE& arLine_In,				nrT_LINE_R& arLine_Out);
	void ConvertToArCircle      (T_PAPER_FORMAT Format, nrT_CIRCLE_R& arCircle_In,		nrT_CIRCLE& arCircle_Out);
	void ConvertToArCircleR     (T_PAPER_FORMAT Format, nrT_CIRCLE& arCircle_In,			nrT_CIRCLE_R& arCircle_Out);
	void ConvertToArDonut       (T_PAPER_FORMAT Format, nrT_DONUT_R& arDonut_In,			nrT_DONUT& arDonut_Out);
	void ConvertToArDonutR      (T_PAPER_FORMAT Format, nrT_DONUT& arDonut_In,			nrT_DONUT_R& arDonut_Out);
	void ConvertToArArc         (T_PAPER_FORMAT Format, nrT_ARC_R& arArc_In,				nrT_ARC& arArc_Out);
	void ConvertToArArcR        (T_PAPER_FORMAT Format, nrT_ARC& arArc_In,				nrT_ARC_R& arArc_Out);
	void ConvertToArPolyline    (T_PAPER_FORMAT Format, nrT_POLYLINE_R& arPolyline_In,	nrT_POLYLINE& arPolyline_Out);
	void ConvertToArPolylineR   (T_PAPER_FORMAT Format, nrT_POLYLINE& arPolyline_In,		nrT_POLYLINE_R& arPolyline_Out);
	void ConvertToArPolygon     (T_PAPER_FORMAT Format, nrT_POLYLINE_R& arPolygon_In,		nrT_POLYLINE& arPolygon_Out);
	void ConvertToArPolygonR    (T_PAPER_FORMAT Format, nrT_POLYLINE& arPolygon_In,		nrT_POLYLINE_R& arPolygon_Out);
	void ConvertToArText        (T_PAPER_FORMAT Format, nrT_TEXT_R& arText_In,			nrT_TEXT& arText_Out);
	void ConvertToArTextR       (T_PAPER_FORMAT Format, nrT_TEXT& arText_In,				nrT_TEXT_R& arText_Out);
	void ConvertToArDimension   (T_PAPER_FORMAT Format, nrT_DIMENSION_R& arDimension_In,	nrT_DIMENSION& arDimension_Out);
	void ConvertToArDimensionR  (T_PAPER_FORMAT Format, nrT_DIMENSION& arDimension_In,	nrT_DIMENSION_R& arDimension_Out);
	void ConvertToArAngle       (T_PAPER_FORMAT Format, nrT_ANGLE_R& arAngle_In,			nrT_ANGLE& arAngle_Out);
	void ConvertToArAngleR      (T_PAPER_FORMAT Format, nrT_ANGLE& arAngle_In,			nrT_ANGLE_R& arAngle_Out);
	void ConvertToArLeaderLine  (T_PAPER_FORMAT Format, nrT_LEADERLINE_R& arLeader_In,	nrT_LEADERLINE& arLeader_Out);
	void ConvertToArLeaderLineR (T_PAPER_FORMAT Format, nrT_LEADERLINE& arLeader_In,		nrT_LEADERLINE_R& arLeader_Out);	
	void ConvertToArChart       (T_PAPER_FORMAT Format, T_CHART_FORMAT chartFormat,		nrT_CHART_R& arChart_In, nrT_CHART& arChart_Out);	
	void ConvertToArChartR      (T_PAPER_FORMAT Format, nrT_CHART& arChart_In,			nrT_CHART_R& arChart_Out);	
	void ConvertToPathline      (T_POLYLINE_R polyline, T_POLYLINE_R& pathline, BOOL bClose);

	void MakeChartData          (T_PAPER_FORMAT Format, T_CHART_FORMAT chartFormat, T_CHART_R& Chart_In, T_CHART& Chart_Out);
	void MakeHatchingData       (T_DRAW_FORMAT Format,   T_LINE			Rect_In,		T_HATCHING_D&   Hatching_Out);
	void MakeHatchingData       (T_DRAW_FORMAT Format,   T_POLYLINE 	Polyline_In,    T_HATCHING_D&   Hatching_Out);	
	void MakeArHatchingData     (T_DRAW_FORMAT Format,   nrT_LINE&		arRect_In,		nrT_HATCHING_D& arHatching_Out);
	void MakeArHatchingData     (T_DRAW_FORMAT Format,   nrT_POLYLINE& arPolyline_In, nrT_HATCHING_D& arHatching_Out);

	// 영역 ----------------------------------------------------------------------------------------------------
	void GetPossessionRegion(T_LINE_R&       LineD,       double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegion(T_CIRCLE_R&     CircleD,     double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegion(T_DONUT_R&      DonutD,      double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegion(T_ARC_R&        ArcD,        double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegion(T_POLYLINE_R&   PolylineD,   double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	//※주의 : 현재(07.12.17) Text크기 및 치수선의 지시선의 크기는 미고려하고 있습니다.
	void GetPossessionRegion(T_TEXT_R&       TextD,       double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegion(T_DIMENSION_R&  DimensionD,  double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegion(T_ANGLE_R&      AngleD,      double& dXmax, double& dXmin, double& dYmax, double& dYmin);
	void GetPossessionRegion(T_LEADERLINE_R& LeaderLineD, double& dXmax, double& dXmin, double& dYmax, double& dYmin);

	// Chart -------------------------------
	// 축설정에서 Auto로 된 부분의 값을 채워줌
	//   nWidth, nFontSize, nDecimal: 그래프가 그려질 HDC상의 크기, Label글자크기, 글자표현 소수점자리
	//   dMaxValue, dMinValue :  Data의 최대값 최소값
	//   bCrossLabel : 축과 Label이 직교에 가까운지 여부 (직교하는 경우 자동 Lable 간격 설정시 문자열의 재약을 완화시킴)
	// ><AxisInfo : 축설정
	void   GetAutoAxisInfo(int nWidth, int nHeight, int nFontSize, int nDecimal, double dMaxValue, double dMinValue, T_CHART_INFO_AXIS& AxisInfo, BOOL bCrossLabel=FALSE);
	double FindAutoSpace(double dBaseSpace, int& nType); // nType 0:1간격, 1:2간격, 2:2.5간격, 3:5간격
	double GetIncreaseDiv(int nType, int nTime);// minor형식의 증배시킬 값을 넘겨줌  nType:minor형식(1,2,2.5,5간격), nTime:만족하지 못하였을때 재요청 횟수
	// 두 HDC상의 위치에 비례하여 실제값을 HDC상의 값으로 계산하여 넘겨줌
	int  ConvertToChartValue(int nP1, int nP2, double dP1, double dP2, double dValue);
	// 해당 범위(Rect) 밖의 정보는 Cutting함 Line의 경우 교차되어 벗어 나는 
	void Cutting_OutRect(T_LINE Rect, int nPointSize, nrPOINT& arPoint_In, nrT_LINE& arLine_In, nrPOINT& arPoint_Out, nrT_LINE& arLine_Out);
	
	// Hatching ----------------------------
	// Hatching 형식을 세부적인 형식으로 변환
	//   nType, nSpace : Format에 정의 된 형식 및 간격
	// < bPoint, bLine, nDLine, bZigZag, nZigZagType : Point정보가 필요한지 여부, Line정보가 필요한지 여부, Line의 중첩갯수, Point가 ZigZag배치인지 여부, ZigZag배치배치형식
	// < nLineDir : 선의방향(1:수평, 2:수직, 3:45도 우측에서 아래로, 4:45도 좌측에서 아래로, 5:십자가, 6:X자, 11:수평원통, 12:수직원통, 0:ETC)
	// < nLineStartPosi, int& nPointStartPosi : Line과 Point의 시작점에서 떨어진 거리
	// < nLineSpace, nPointSpace, nPointSpace_Sub : Line간격, Point수평간격, Point수직간격
	// < PointSize : Point크기
	void GetHatchingSubData(EN_HATCHING_TYPE nType, int nSpace, BOOL& bPoint, BOOL& bLine, int& nDLine, BOOL& bZigZag, int& nZigZagType, int& nLineDir,  
		int& nLineStartPosi, int& nPointStartPosi, int& nLineSpace, int& nPointSpace, int& nPointSpace_Sub, T_LINE& PointSize);

	// - Rectangular
	// 사각형(Rect)안에 해당 방향(sPoint, nDir)을 가로지르는 선분을 계산하여 넘겨줌
	//   Rect : 경계사각형
	//   sPoint, nDir : 기준점 및 선의방향(1:수평, 2:수직, 3:45도 우측에서 아래로, 4:45도 좌측에서 아래로)
	// < CrossLine 경계내부를 가로 지르는 선분
	//   nRectLineWidth : 경계사각형의 선의 두께(m_bRectTrPolyDraw==FALSE시에 사각형이 작게 그려지는 현상 고려)
	BOOL GetCrossLine_Rect(T_LINE& Rect, POINT sPoint, int nDir, T_LINE& CrossLine, int nRectLineWidth);
	// 사각형(Rect)안에 원을 그릴때 사각형 내부의 호를 계산하여 넘겨줌
	BOOL GetCrossArc_Rect(T_LINE& Rect,  POINT cPoint, int nRadius, nrT_ARC& CrossArcList, int nRectLineWidth);
	// 사각형(Rect)안에 Polygon을 그릴때 사각형 내부에 들어가는 Polygon과 Polyline을 계산하여 넘겨줌
	BOOL GetCrossPolyline_Rect(T_LINE& Rect, nrT_POLYLINE& arPoly, nrT_POLYLINE& arPolygonOut, nrT_POLYLINE& arPolylineOut, int nRectLineWidth);

	// - Polyline
	// Polyline안에 해당 방향(sPoint, nDir)을 가로지르는 선분을 계산하여 넘겨줌
	//   nDir : 선의방향(1:수평, 2:수직, 3:45도 우측에서 아래로, 4:45도 좌측에서 아래로)
	BOOL GetCrossLine_Poly(T_POLYLINE& PolyD, POINT sPoint, int nDir, int nXmax, int nXmin, int nYmax, int nYmin, nrT_LINE& arCrossLine);
	// Polyline안에 해당 선분을 그릴때 내부의 선분들을 계산하여 넘겨줌
	BOOL GetCrossLine_Poly(T_POLYLINE& PolyD, T_LINE LineD, nrT_LINE& arCrossLine);
	BOOL GetCrossLine_Poly(T_POLYLINE& PolyD, T_LINE LineD, nrT_LINE& arCrossLine, CArray<int, int>& arCrossID);
	// Polyline안에 원을 그릴때 Polyline내부의 호를 계산하여 넘겨줌
	BOOL GetCrossArc_Poly(T_POLYLINE& PolyD,  POINT cPoint, int nRadius, nrT_ARC& CrossArcList);
	// Polyline안에 Polygon을 그릴때 Polyline 내부에 들어가는 Polygon과 Polyline을 계산하여 넘겨줌
	//   bOnlyPolygon : 영역안의 Polyline을 경계를 포함한 Polygon으로 변경하여 넘겨줄지 여부
	BOOL GetCrossPolyline_Poly(T_POLYLINE& PolyD, nrT_POLYLINE& arPoly, nrT_POLYLINE& arPolygonOut, nrT_POLYLINE& arPolylineOut, BOOL bOnlyPolygon);

	// 1개 구역에 대한 자갈에 대한 Polygon정보와 Point정보를 넘겨줌
	//   nWidth, nHeight : 1개 구역의 크기
	// < arPoly, arPoint : Scale적용된 1개구역의 자갈정보
	// < nXmax, nXmin, nYmax, nYmin : 1개구역을 이루는 좌표들이 영역(타구역과 정첩되는 Polygon이 있으므로...)
	void GetGravelData(int nWidth, int nHeight, nrT_POLYLINE& arPoly, nrPOINT& arPoint, int& nXmax, int& nXmin, int& nYmax, int& nYmin);

	// E.T.C ----------------------------
	// '\n'을 포함하고 있는 문자열을 가진경우 복수의 T_TEXT형식으로 변환하여줌
	void ConvertNextLineText(int fontSize, EN_POSITION_TYPE enPosiType, int nEscapement, nrT_TEXT& arTextOrg, nrT_TEXT& arTextCng);
	// 수식정보('`', '^', '$')를 포함하고 있는 문자열을 가진경우 복수의 T_TEXT형식으로 변환하여줌
	void ConvertMathText(HDC DC, int fontSize, int fontSize_sub, EN_POSITION_TYPE enPosiType, int nEscapement, nrT_TEXT& arTextOrg, nrT_TEXT& arTextCng, nrT_TEXT& arTextCng_sub);

	// 점과 polyline과의 최단 거리
	double GetDistanceFromIntersectPointToPolyine(T_POLYLINE PolyD, POINT pointD);

	// emf생성시 해당 크기를 emf상의 가로 크기로 변환하여줌 (MM_HIMETRIC일때)
	//   iMode   : dLength가 Pixel단위인지 mm단위인지 여부(0:변환없음 1:Pixel 2:mm(Office 2003이하)  3:Pixel 4:mm(Office 2007이상 및 기타))
	//   dLength : 그림의 가로 Pixel 또는 mm
	//   RETURN  : 그림을 그릴 공간에서의 길이
	int ConvertToMapLength(int iMode, double dLength);

	// 자릿수에 따른 포맷 컨트롤 문자열을 넘겨줌
	static TCHAR* GetDoubleStringFormat(int nDecimal);

	// Arc를 구성하는 Polyline을 만들어 줌
	//   startP : 시작점
	//   originP : 회전중심점
	//   angle   : 1점당 회전각(단위 :˚)
	//   nCount  : angle을 몇번 회전시킬지 여부 (총회전각=angle × nCount,  좌표점의 수=|nCount|+1)
	//   arcPolyline : Arc를 이루는 Polyline을 넘겨 받을 변수
	// ※REMARK  : 회전은 반시계 방향으로 이루어지며 시계방향으로 회전시킬때에는 angle을 음수를 적용하면 된다. 
	static void GetArcPolylineR(T_POINT startP, T_POINT originP, double angle, int nCount, T_POLYLINE_R& arcPolyline);
	static void GetArcPolylineR(T_ARC_R& ArcD, int nCount, T_POLYLINE_R& arcPolyline);

	// Arc를 구성하는 Polyline을 만들어 줌
	//   dStartP : 시작점
	//   dCenterP: 회전중심점
	//   angle   : 1점당 회전각(단위 :˚)
	//   nCount  : angle을 몇번 회전시킬지 여부 (총회전각=angle × nCount,  좌표점의 수=|nCount|+1)
	//   bPointSide : 양끝점을 포함시킬지 여부
	// < arPoint : Arc를 이루는 Polyline을 넘겨 받을 변수
	// ※REMARK  : 회전은 반시계 방향으로 이루어지며 시계방향으로 회전시킬때에는 angle을 음수를 적용하면 된다. 
	void GetArcPolyline(double dStartP[2], double dCenterP[2], double angle, int nCount, BOOL bPointSide, nrT_POINT& arPoint);
	void GetArcPolyline(double dStartP[2], double dPassP[2], double dEndP[2], BOOL bNum, int nPointCount, double dPointCTC, BOOL bPointSide, nrT_POINT& arPoint);
	void GetLinePolyline(double dStartP[2], double dEndP[2], BOOL bNum, int nPointCount, double dPointCTC, BOOL bSideRebar, nrT_POINT& arPoint);
	void GetCirclePolyline(double dStartP[2], double dRadius, BOOL bNum, int nPointCount, double dPointCTC, nrT_POINT& arPoint);

	// Arc를 구성하는 Polyline을 만들어 줌(dRadius와 bInside를 이용)
	//   dStartP, dEndP : 시작점 끝점
	//   dRadius : 호의 반지름(+:중심점이 진행방향의 좌측에 존재, -:중심점이 진행방향의 우측에 존재)
	//   bInside : 호의 각도가 180이하인지 이상인지 여부(TRUE일때 180각 이하의 호를 만들어 줌)
	//   nInCount: 시작점과 끝점사이에 추가할 좌표의 개수(arPoint의 크기는 양끝점 포함시 nInCount+2개 값을 가짐)
	//   bPointSide : 양끝점을 포함시킬지 여부
	// < arPoint : Arc를 이루는 Polyline을 넘겨 받을 변수
	BOOL GetArcPolyline_Radius(double dStartP[2], double dEndP[2], double dRadius, BOOL bInside, int nPointCount, BOOL bPointSide, nrT_POINT& arPoint);
	// Arc를 구성하는 Polyline을 만들어 줌(dBulge를 이용)
	BOOL GetArcPolyline_Bulge (double dStartP[2], double dEndP[2], double dBulge, int nPointCount, BOOL bPointSide, nrT_POINT& arPoint);

	static void GetHorizontalDimensionR(T_POLYLINE_R& dgnPolyR, nrT_DIMENSION_R& arDimR, double dPosition_Y, int nDecimal, double dRatio, int nSortType);
	static void GetHorizontalDimensionR(nrDOUBLE& arPosi, nrT_DIMENSION_R& arDimR, double dPosition_Y, int nDecimal, double dRatio, int nSortType);
	static void GetVerticalDimensionR  (T_POLYLINE_R& dgnPolyR, nrT_DIMENSION_R& arDimR, double dPosition_X, int nDecimal, double dRatio, int nSortType);
	static void GetVerticalDimensionR  (nrDOUBLE& arPosi, nrT_DIMENSION_R& arDimR, double dPosition_X, int nDecimal, double dRatio, int nSortType);	
};




#include "HeaderPost.h"
