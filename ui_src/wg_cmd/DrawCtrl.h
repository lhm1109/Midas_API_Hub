#pragma once

#include <set>
#include <list>
#include <algorithm>

#include "DrawBase.h"
#include "DrawPoint.h"        
#include "DrawLine.h"         
#include "DrawArrowLine.h"    
#include "DrawCircle.h"      
#include "DrawDonut.h"        
#include "DrawEllipse.h"      
#include "DrawRect.h"         
#include "DrawArc.h"          
#include "DrawPolyline.h"     
#include "DrawPolygon.h"      
#include "DrawArrowPolyline.h"
#include "DrawText.h"         
#include "DrawDimension.h"    
#include "DrawDimAngle.h"     
#include "DrawDimLeaderline.h" 
#include "DrawChart.h"

#include "HeaderPre.h"

/*=========================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////
CDrawCtrl

실제 좌표정보로 그림을 그릴때 사용
(실제 좌표 정보를 HDC좌표정보롤 변환하여 CDrawFunc를 이용하여 그림을 그림)
//////////////////////////////////////////////////////////////////////////////////////////


실제크기의 Draw정보(m_arLineR∼m_arDimLeaderlineR)는 가상의 정보로 Draw()명령을 주어도 그려지지 않는다.
삽도상의 Draw정보(m_arLine∼m_arDimLeaderline)만이 Draw()명령시 HDC에 그림을 그린다.

따라서 일반적인 작업순서는 아래와 같다.
실제크기의 Draw정보들을 생성 -> ConvertToCDC()를 실행하여 삽도상의 Draw정보로 변환  -> Draw()
(변환시에는 동일 ID의 Draw객체는 덮어 씀)

실제크기의 Draw정보, 삽도상의 Draw정보 및 그림형식은 모두 public멤버이므로 조작 후 원하는 시점에 Draw()를 
호출하며 HDC에 입력정보를 그린다.
=========================================================================================================*/

#define D_MERGE_LINE_ANG		1E-5

typedef CArray<T_LINE_R, T_LINE_R&>	nrLineR;
typedef CArray<T_LINE, T_LINE&>		nrLine;

class __MY_EXT_CLASS__ CDrawCtrl  
{
public:
	CDrawCtrl();
	virtual ~CDrawCtrl();

	CDrawFunc m_DrawFunc;// 실제 삽도를 그리는 객체
	
	// 실제의 정보와 HDC상의 정보간의 Scale정보 
	T_PAPER_FORMAT m_PaperFormat;

	// Member Variable /////////////////////////////////
	// HDC에 그릴때 그림객체들의 형식 (Key번호는 각 Group의 FormatKey)
	CMap<UINT, UINT, T_DRAW_FORMAT, T_DRAW_FORMAT&>	m_arDrawFormat;
	CMap<UINT, UINT, T_TEXT_FORMAT, T_TEXT_FORMAT&>	m_arTextFormat;
	CMap<UINT, UINT, T_DIM_FORMAT, T_DIM_FORMAT&>	m_arDimFormat;
	CMap<UINT, UINT, T_CHART_FORMAT, T_CHART_FORMAT&>	m_arChartFormat;

	int m_nMirrorMode;     // Mirror
	int m_SelectDrawType;  // 0:표현안함, 1:외곽사각형표시, 2:Format변경, 3:색만변경, 4:색+선두깨변경	
	int m_HideType;        // 0:Hide적용안함, 1:Hide기능사용

	T_DRAW_FORMAT  m_SelectDrawFormat;    
	T_TEXT_FORMAT  m_SelectTextFormat;    
	T_DIM_FORMAT   m_SelectDimFormat;      
	T_CHART_FORMAT m_SelectChartFormat;   
	
protected:
	HDC      m_hDC;
	CBCGPStatic* m_pMessageStatic;

private:
	UINT m_nSelectDis; // Select시 여유 간격

private:
	UINT m_FinalEntityNum; //마지막 Draw 객체의 번호
	// 실제크기의 Draw정보 : 가상의 정보로 실제 삽도상에 그리기 위해서는 일반 Draw정보로 전환하여야 한다.
	//                       (Key번호는 그림을 그릴 순서)
	CMap<UINT, UINT, CDrawPoint*, CDrawPoint*>			m_arPoint;
	CMap<UINT, UINT, CDrawLine*, CDrawLine*>			m_arLine;
	CMap<UINT, UINT, CDrawArrowLine*, CDrawArrowLine*>	m_arArrowLine;
	CMap<UINT, UINT, CDrawCircle*, CDrawCircle*>		m_arCircle;
	CMap<UINT, UINT, CDrawDonut*, CDrawDonut*>			m_arDonut;
	CMap<UINT, UINT, CDrawEllipse*, CDrawEllipse*>		m_arEllipse;
	CMap<UINT, UINT, CDrawRect*, CDrawRect*>			m_arRect;
	CMap<UINT, UINT, CDrawArc*, CDrawArc*>				m_arArc;
	CMap<UINT, UINT, CDrawPolyline*, CDrawPolyline*>			m_arPolyline;
	CMap<UINT, UINT, CDrawPolygon*, CDrawPolygon*>				m_arPolygon;
	CMap<UINT, UINT, CDrawArrowPolyline*, CDrawArrowPolyline*>	m_arArrowPolyline;
	CMap<UINT, UINT, CDrawText*, CDrawText*>					m_arText;
	CMap<UINT, UINT, CDrawDimension*, CDrawDimension*>			m_arDimension;
	CMap<UINT, UINT, CDrawDimAngle*, CDrawDimAngle*>			m_arDimAngle;
	CMap<UINT, UINT, CDrawDimLeaderline*, CDrawDimLeaderline*>	m_arDimLeaderline;
	CMap<UINT, UINT, CDrawChart*, CDrawChart*>					m_arChart;

	
	CMap<UINT, UINT, UINT, UINT> m_arHideDrawUnitKey;
	CMap<UINT, UINT, UINT, UINT> m_arSelectedDrawUnitKey;

public:
	// Member Function /////////////////////////////////
	void SetHDC(HDC& DC) { m_hDC = DC; };
	HDC  GetDC()         { return m_hDC;};

	void GetMinMax(double& dXmin, double& dXmax, double& dYmin, double& dYmax);
	void Copy(CDrawCtrl* pDrawCtrl);
	
	void MessageOut(TCHAR* strMsg);
	void RemoveDrawData(BOOL bDrawFormat = TRUE, BOOL bDrawUnit = TRUE, BOOL bPaperFormat = TRUE);
	void ConvertDrawData(BOOL bCDC);
	void Draw();
	void Draw(RECT DrawRect);
	void AdjustMarginToFitImage(RECT Canvas, double dMargin, double dImageB, double dImageH, BOOL bDimT, BOOL bDimB, BOOL bDimL, BOOL bDimR);
	void AutoPaperFormat(RECT Canvas, double dTopMargin, double dBottomMargin, double dLeftMargin, double dRightMargin);
	void SetPaperFormat(RECT Canvas, double dTopMargin, double dBottomMargin, double dLeftMargin, double dRightMargin, 	double dWorkXmax, double dWorkXmin, double dWorkYmax, double dWorkYmin);	

	UINT GetFinalEntityNum() { return m_FinalEntityNum; }
	CDrawBase* GetDrawClass(UINT key);

	BOOL FindLinkKeyFromDrawKey(UINT DrawKey, UINT& LinkKey);// DrawKey가 가르키는 DrawUnit로 부터 LinkKey를 찾아 넘겨줌
	BOOL FindDrawKeyFromLinkKey(UINT LinkKey, nrUINT& arDrawKey);// 동일한 LinkKey를 가지는 모든 DrawUnit의 Key를 찾아 넘겨줌

	void RotateDrawData(double dRotate, double dOffsetx, double dOffsety, UINT& nSt);
	void RotateData(double dRotate, double dOffsetx, double dOffsety, long& x, long& y);
	void RotateData(double dRotate, double dOffsetx, double dOffsety, double& x, double& y);

	// Select Draw Unit
	int  GetIncludeKeyList(POINT Boundary1, POINT Boundary2, nrUINT& arKey, BOOL bIsSelect=FALSE);//사각형 내의 DrawUnit Key를 찾음 (bIsSelect: Select가 가능한 부재에 대해서만 추가할지 여부) 
	int  GetSelectedKeyList(nrUINT& arKey, BOOL bMoveOnly = FALSE);	
	int  GetSelectedLinkKeyList(nrUINT& arLinkKey, BOOL bMoveOnly = FALSE);
	void SetSelectedKeyList(nrUINT& arKey);
	void SetSelectedLinkKeyList(nrUINT& arLinkKey);
	void RemoveAllSelectedKey();
	void AddSelectedKey(UINT key);
	void AddSelectedKey(nrUINT& arKey);
	void DelSelectedKey(UINT key);
	void DelSelectedKey(nrUINT& arKey);
	void ReSelectedKey(UINT key);
	void ReSelectedKey(nrUINT& arKey);
	BOOL IsSelected(UINT key);
	void GetConnectedLineKey(UINT keySrc, nrUINT &arKeyConnected, double dAngle);	
	void GetConnectedLineKey(UINT keySrc, int nSrcIdx, CArray<CDrawLine*, CDrawLine*> &arPLine, CArray<UINT, UINT> &arKey, nrUINT &arKeyConnected, double dAngle);		
	void GetConnectedLineKeyOneDir(int nSrcIdx, std::set<UINT> &setConnectedKey, std::list<UINT> &stlistConKey,CArray<CDrawLine*, CDrawLine*> &arPLine, CArray<UINT, UINT> &arKey,
		BOOL bDirectForward ,double dAngle);	
	
	void GetMergedConnectedLine(nrUINT &arkeySrc, CArray<nrLineR, nrLineR&> &arMergedConnectedLine, double dMergeAng);
	void MergeLineByAngle(const nrUINT& arLineKey, nrLineR &arMergedLine,double dMergeAng);

	// Hide Draw Unit
	int  GetHideKeyList(nrUINT& arKey);	
	int  GetHideLinkKeyList(nrUINT& arLinkKey);
	void SetHideKeyList(nrUINT& arKey);
	void SetHideLinkKeyList(nrUINT& arLinkKey);
	void RemoveAllHideKey();
	void AddHideKey(UINT key);
	void AddHideKey(nrUINT& arKey);
	void DelHideKey(UINT key);
	void DelHideKey(nrUINT& arKey);
	BOOL IsHideKey(UINT key);

	UINT GetSelectDis();
	void SetSelectDis(UINT nSelectDis);
	UINT GetSnapDis();
	void SetSnapDis(UINT nSnapDis);
	BOOL GetOpjectSnapPoint(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR);
	BOOL GetSelectDrawUnitKey(POINT currPoint, UINT& SelectKey);

	BOOL GetOpjectHighlightLine(CPoint currPoint, nrLine &aHighlightLine);

	void MoveDrawUnitR(UINT key, double disX, double disY);
	void MoveDrawUnitR(nrUINT& arKey, double disX, double disY);
	void MoveDrawUnit(UINT key, int disX, int disY);
	void MoveDrawUnit(nrUINT& arKey, int disX, int disY);

	void DelDrawUnit(UINT key);
	void DelDrawUnit(nrUINT& arKey);

	UINT AddDrawUnit(int nFormatKey, T_POINT_GROUP           &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_LINE_GROUP            &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ARROWLINE_GROUP       &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_CIRCLE_GROUP          &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DONUT_GROUP           &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ELLIPSE_GROUP         &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_RECT_GROUP            &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ARC_GROUP             &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_POLYLINE_GROUP        &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_POLYGON_GROUP         &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ARROWPOLYLINE_GROUP   &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_TEXT_GROUP            &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DIMENSION_GROUP       &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DIMANGLE_GROUP        &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DIMLEADERLINE_GROUP   &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_CHART_GROUP           &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, int iOffsetX = 0, int iOffsetY = 0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);

	UINT AddDrawUnit(int nFormatKey, T_POINT_R_GROUP         &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_LINE_R_GROUP          &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ARROWLINE_R_GROUP     &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_CIRCLE_R_GROUP        &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DONUT_R_GROUP         &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ELLIPSE_R_GROUP       &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_RECT_R_GROUP          &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ARC_R_GROUP           &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_POLYLINE_R_GROUP      &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_POLYGON_R_GROUP       &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_ARROWPOLYLINE_R_GROUP &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_TEXT_R_GROUP          &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DIMENSION_R_GROUP     &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DIMANGLE_R_GROUP      &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_DIMLEADERLINE_R_GROUP &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);
	UINT AddDrawUnit(int nFormatKey, T_CHART_R_GROUP         &rData, BOOL bConvert = FALSE, int LinkKey = 0, int nSnapType = 0, BOOL bIsSelect = FALSE, double dOffsetX = 0.0, double dOffsetY = 0.0, BOOL bIsOverlapChk = FALSE, int nSelectSubType = 1);

	BOOL ModifyDrawFormat(UINT key, int nFormatKey);
	BOOL ModifyLinkKey(UINT key, int nLinkKey);

	BOOL ModifyDrawUnit(UINT key, T_POINT_GROUP           &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_LINE_GROUP            &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ARROWLINE_GROUP       &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_CIRCLE_GROUP          &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DONUT_GROUP           &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ELLIPSE_GROUP         &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_RECT_GROUP            &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ARC_GROUP             &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_POLYLINE_GROUP        &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_POLYGON_GROUP         &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ARROWPOLYLINE_GROUP   &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_TEXT_GROUP            &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DIMENSION_GROUP       &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DIMANGLE_GROUP        &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DIMLEADERLINE_GROUP   &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_CHART_GROUP           &rData, BOOL bConvert = FALSE);

	BOOL ModifyDrawUnit(UINT key, T_POINT_R_GROUP         &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_LINE_R_GROUP          &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ARROWLINE_R_GROUP     &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_CIRCLE_R_GROUP        &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DONUT_R_GROUP         &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ELLIPSE_R_GROUP       &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_RECT_R_GROUP          &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ARC_R_GROUP           &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_POLYLINE_R_GROUP      &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_POLYGON_R_GROUP       &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_ARROWPOLYLINE_R_GROUP &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_TEXT_R_GROUP          &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DIMENSION_R_GROUP     &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DIMANGLE_R_GROUP      &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_DIMLEADERLINE_R_GROUP &rData, BOOL bConvert = FALSE);
	BOOL ModifyDrawUnit(UINT key, T_CHART_R_GROUP         &rData, BOOL bConvert = FALSE);

	// 정보를 저정하지 않고 HDC에 직접 그릴경우
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_POINT_GROUP        &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_LINE_GROUP         &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_ARROWLINE_GROUP    &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_CIRCLE_GROUP       &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_DONUT_GROUP        &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_ELLIPSE_GROUP      &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_RECT_GROUP         &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_ARC_GROUP          &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_POLYLINE_GROUP     &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_POLYGON_GROUP      &rData);
	void DrawUnit(T_DRAW_FORMAT  DrawFormat, T_ARROWPOLYLINE_GROUP&rData);
	void DrawUnit(T_TEXT_FORMAT  DrawFormat, T_TEXT_GROUP         &rData);
	void DrawUnit(T_DIM_FORMAT   DrawFormat, T_DIMENSION_GROUP    &rData);
	void DrawUnit(T_DIM_FORMAT   DrawFormat, T_DIMANGLE_GROUP     &rData);
	void DrawUnit(T_DIM_FORMAT   DrawFormat, T_DIMLEADERLINE_GROUP&rData);
	void DrawUnit(T_CHART_FORMAT DrawFormat, T_CHART_GROUP        &rData);
	void DrawUnit(HBITMAP img, RECT rtCanvas);

public:	
	void OverlapChecking_Object(int nOverlapType, T_DIMLEADERLINE_GROUP& OverlapDrawGroup, T_DIM_FORMAT& DimFormat);

	void SetTextUnit_CurrFormatSizeRatio(double dRatio); // 모든 Text DrawUnit의 CurrFormat의 Size에 dRatio값을 곱하여 줍니다.(CDrawEditBtn::Make_EmfFile시에 Canvas와 Emf파일의 크기사이의 조정시에 필요합니다.)
	void SetTextUnit_FormatSizeRatio(double dRatio, BOOL bLineWidth=TRUE, BOOL bTextFont=TRUE, BOOL bDimFont=TRUE, BOOL bChartFont=TRUE); // 모든 사용하는 Format의 Fond Size 및 LineWidth에 dRatio값을 곱하여 줍니다.(CDrawEditBtn::Make_EmfFile시에 Canvas와 Emf파일의 크기사이의 조정시에 필요합니다.)

protected:
	BOOL ChkMaxDrawUnit(); // Unit수가 m_FinalEntityNum이 표현할수 있는 수의 범위를 넘어 셨는지 여부를 검토
	BOOL IsOverLap(const T_POINT tpA, const T_POINT tpB);
	double GetAngle(const T_POINT &ptStP1, const T_POINT &ptStP2, const T_POINT &ptEndP1, const T_POINT &ptEndP2);
	void GetFrmDirectionVector(const T_POINT &ptSt, const T_POINT &ptEnd, double& Ux, double& Uy, double& Uz );

	//void InsertOverlapKey(CMap<T_OVERLAP_KEY, T_OVERLAP_KEY, UINT, UINT>& mapOverlap, CMap<UINT, UINT, T_OVERLAP_RES_D, T_OVERLAP_RES_D&>& mapOverlapRes, UINT& DrawKey1, int& ListID1, UINT& DrawKey2, int& ListID2, UINT& nNewKey);
	void InsertOverlapKey(CMap<T_ED_OVERLAP_KEY, T_ED_OVERLAP_KEY,UINT, UINT>& mapOverlap, CMap<UINT, UINT, T_ED_OVERLAP_RES_D, T_ED_OVERLAP_RES_D>& mapOverlapRes, UINT& DrawKey1, int& ListID1, UINT& DrawKey2, int& ListID2, UINT& nNewKey);	
	
};

#include "HeaderPost.h"
