#include "Stdafx.h"
#include "wg_cmd.h"

//#include "../NS_lib/IMSGInfo.h"

#include "DrawDataFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

T_PAPER_FORMAT& T_PAPER_FORMAT::operator = (const T_PAPER_FORMAT& pData) 
{
	memcpy(this, &pData, sizeof(T_PAPER_FORMAT));
	return *this;
}

void T_PAPER_FORMAT::Init() 
{ 
	OrgPoint.x = OrgPoint.y = 0;   
	ScaleX = ScaleY = 1.0;
}

T_DRAW_FORMAT& T_DRAW_FORMAT::operator = (const T_DRAW_FORMAT& pData)
{
	lineColor      = pData.lineColor     ;
	fillColor      = pData.fillColor     ;
	hatchingColor  = pData.hatchingColor ;
	nPenStyle      = pData.nPenStyle     ;
	nPenSubStyle   = pData.nPenSubStyle  ;
	bFill          = pData.bFill         ;
	bLine          = pData.bLine         ;
	bHatching      = pData.bHatching     ;
	Width          = pData.Width         ;
	HatchingWidth  = pData.HatchingWidth ;		
	HatchingSpace  = pData.HatchingSpace ;	
	dRWidth        = pData.dRWidth       ;
	dRHeight       = pData.dRHeight      ;
	startArrowType = pData.startArrowType;
	endArrowType   = pData.endArrowType  ;
	hatchingType   = pData.hatchingType  ;
	pointType      = pData.pointType     ;
	return *this;
}

void T_DRAW_FORMAT::Init()
{
	lineColor      = RGB(  0,  0,  0);
	fillColor      = RGB(255,255,255);
	hatchingColor  = RGB(  0,  0,  0);
	nPenStyle      = PS_SOLID;
	nPenSubStyle   = -1;
	bFill          = FALSE;
	bLine          = TRUE;
	bHatching      = FALSE;
	Width          = 1; 
	HatchingWidth  = 0;
	HatchingSpace  = 0;
	dRWidth        = 4.0;
	dRHeight       = 4.0;
	startArrowType = EN_ARROW_TYPE_ARROW;
	endArrowType   = EN_ARROW_TYPE_ARROW;
	hatchingType   = EN_HATCHING_TYPE_NONE;
	pointType      = EN_POINT_TYPE_DOT; 
}

T_TEXT_FORMAT& T_TEXT_FORMAT::operator = (const T_TEXT_FORMAT& pData)
{
	fontColor    = pData.fontColor   ;
	fillColor    = pData.fillColor   ;
	bFill        = pData.bFill       ;
	Size         = pData.Size        ;
	bHalfWidth   = pData.bHalfWidth  ;
	szFont       = pData.szFont     ;
	positionType = pData.positionType;  
	nEscapement  = pData.nEscapement ;
	return *this;
}

void T_TEXT_FORMAT::Init(int nLanguage)
{
	fontColor    = RGB(  0,  0,  0);
	fillColor    = RGB(255,255,255);
	bFill        = FALSE;
	Size         = 10; 
	bHalfWidth   = FALSE;		
	//szFont       = (nLanguage != _LANGUAGE_JP_) ? TEXT(_T("Arial")) : TEXT(_T("MS ゴシック")); // 일본어가 지원되지 않으므로 문자가 일본어일 경우 _T("MS ゴシック")사용
	szFont       = _T("Arial");
	positionType = EN_POSITION_TYPE_LEFT_TOP;
	nEscapement  = 0;
}

T_DIM_FORMAT& T_DIM_FORMAT::operator = (const T_DIM_FORMAT& pData)
{
	bFontScale    = pData.bFontScale   ;
	bMainLine     = pData.bMainLine    ;
	bGuideLine_P1 = pData.bGuideLine_P1;
	bGuideLine_P2 = pData.bGuideLine_P2;
	fontColor     = pData.fontColor    ;
	lineColor     = pData.lineColor    ;
	Width         = pData.Width        ;
	dRWidth       = pData.dRWidth      ;
	dRHeight      = pData.dRHeight     ;
	arrowType     = pData.arrowType    ;
	dRTextSpace   = pData.dRTextSpace  ;
	dRDimSpace    = pData.dRDimSpace   ;
	dRGuideIn     = pData.dRGuideIn    ;
	dRGuideOut    = pData.dRGuideOut   ;
	dimType       = pData.dimType      ;
	bSubDimType   = pData.bSubDimType  ;
	dimType_Sub   = pData.dimType_Sub  ;
	fontSize      = pData.fontSize     ;
	bFontHalfWidth= pData.bFontHalfWidth;
	szFont        = pData.szFont      ;
	dOutTypeRate_L= pData.dOutTypeRate_L;
	dOutTypeRate_R= pData.dOutTypeRate_R;
	return *this;
}  

void T_DIM_FORMAT::Init(int nLanguage)
{
	bFontScale    = TRUE;
	bMainLine     = TRUE;
	bGuideLine_P1 = TRUE;
	bGuideLine_P2 = TRUE;
	fontColor     = RGB(0,0,0);
	lineColor     = RGB(0,0,0);
	Width         = 1;
	dRWidth       = 4.0;
	dRHeight      = 4.0;
	arrowType     = EN_ARROW_TYPE_ARROW;
	dRTextSpace   = 0.2;
	dRDimSpace    = 0.5;
	dRGuideIn     = 1.5;
	dRGuideOut    = 0.5;
	dimType       = EN_DIM_TYPE_TOP;
	bSubDimType   = FALSE;
	dimType_Sub   = EN_DIM_TYPE_OUT_RIGHT;
	fontSize      = 10;
	bFontHalfWidth= FALSE;
	//szFont        = (nLanguage != _LANGUAGE_JP_) ? TEXT(_T("Arial")) : TEXT(_T("MS ゴシック")); // 일본어가 지원되지 않으므로 문자가 일본어일 경우 _T("MS ゴシック")사용
	szFont        = _T("Arial");
	dOutTypeRate_L= -1.0;
	dOutTypeRate_R= -1.0;
}

T_CHART_FORMAT_FRAME& T_CHART_FORMAT_FRAME::operator = (const T_CHART_FORMAT_FRAME& pData)
{
	lineColor = pData.lineColor;
	fillColor = pData.fillColor;
	bLine     = pData.bLine    ;
	bFill     = pData.bFill    ;
	nPenStyle = pData.nPenStyle;
	Width     = pData.Width    ;
	fontColor = pData.fontColor;
	fontSize  = pData.fontSize ;
	szFont    = pData.szFont  ;
	return *this;
}

void T_CHART_FORMAT_FRAME::Init(int nLanguage)
{
	lineColor = RGB(0,0,0);
	fillColor = RGB(0,0,0);
	bLine     = TRUE;    
	bFill     = FALSE;    
	nPenStyle = PS_SOLID;
	Width     = 1;    		
	fontColor = RGB(0,0,0);
	fontSize  = 10;
	//szFont    = (nLanguage != _LANGUAGE_JP_) ? TEXT(_T("Arial")) : TEXT(_T("MS ゴシック")); // 일본어가 지원되지 않으므로 문자가 일본어일 경우 _T("MS ゴシック")사용
	szFont    = _T("Arial"); 
}

T_CHART_FORMAT_AXIS& T_CHART_FORMAT_AXIS::operator = (const T_CHART_FORMAT_AXIS& pData)
{
	lineColor          = pData.lineColor         ;
	nPenStyle          = pData.nPenStyle         ;
	Width              = pData.Width             ;
	majorGridColor     = pData.majorGridColor    ;
	nMajorGridPenStyle = pData.nMajorGridPenStyle;
	nMajorGridWidth    = pData.nMajorGridWidth   ;
	minorGridColor     = pData.minorGridColor    ;
	nMinorGridPenStyle = pData.nMinorGridPenStyle;
	nMinorGridWidth    = pData.nMinorGridWidth   ;
	fontColor          = pData.fontColor         ;
	fontSize           = pData.fontSize          ;
	szFont             = pData.szFont           ;
	fontColor_Title    = pData.fontColor_Title   ;
	fontSize_Title     = pData.fontSize_Title    ;
	szFont_Title       = pData.szFont_Title     ;
	nEscapement        = pData.nEscapement       ;
	return *this;
}

void T_CHART_FORMAT_AXIS::Init(int nLanguage)
{
	lineColor          = RGB(0,0,0);
	nPenStyle          = PS_SOLID;
	Width              = 1;
	majorGridColor     = RGB(0,0,0);     
	nMajorGridPenStyle = PS_SOLID; 
	nMajorGridWidth    = 1;    
	minorGridColor     = RGB(0,0,0);
	nMinorGridPenStyle = PS_SOLID;
	nMinorGridWidth    = 1;
	fontColor          = RGB(0,0,0);
	fontSize           = 10;
	//szFont             = (nLanguage != _LANGUAGE_JP_) ? TEXT(_T("Arial")) : TEXT(_T("MS ゴシック")); // 일본어가 지원되지 않으므로 문자가 일본어일 경우 _T("MS ゴシック")사용
	szFont             = _T("Arial");
	fontColor_Title    = RGB(0,0,0);
	fontSize_Title     = 10;
	//szFont_Title       = (nLanguage != _LANGUAGE_JP_) ? TEXT(_T("Arial")) : TEXT(_T("MS ゴシック")); // 일본어가 지원되지 않으므로 문자가 일본어일 경우 _T("MS ゴシック")사용
	szFont_Title       = _T("Arial");
	nEscapement        = 0;
}


T_CHART_FORMAT& T_CHART_FORMAT::operator = (const T_CHART_FORMAT& pData)
{
	frame  = pData.frame;
	remark = pData.remark;
	xAxis  = pData.xAxis;
	yAxis  = pData.yAxis;
	bHalfWidth = pData.bHalfWidth;
	return *this;
}

void T_CHART_FORMAT::Init(int nLanguage)
{
	frame.Init(nLanguage);
	remark.Init(nLanguage);
	xAxis.Init(nLanguage);
	yAxis.Init(nLanguage);
	bHalfWidth = FALSE;
}