#pragma once 

#include "DrawData.h"

#include "HeaderPre.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_PAPER_FORMAT

struct __MY_EXT_CLASS__ T_PAPER_FORMAT
{
	POINT  OrgPoint; // CDC상의 좌표원점이 될 위치
	double ScaleX;   // 실제 좌표와 CDC상의 좌표와의 X방향 Scale
	double ScaleY;   // 실제 좌표와 CDC상의 좌표와의 Y방향 Scale

	T_PAPER_FORMAT()	{ Init(); }
	T_PAPER_FORMAT(const T_PAPER_FORMAT& pData) { *this = pData; }
	T_PAPER_FORMAT& operator = (const T_PAPER_FORMAT& pData);

	void Init();
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DRAW_FORMAT

struct __MY_EXT_CLASS__ T_DRAW_FORMAT
{
	COLORREF          lineColor;       // 선색
	COLORREF          fillColor;       // 내부색
	COLORREF          hatchingColor;   // hatching 선색
	BOOL              bFill;           // 내부를 채울지 여부
	BOOL              bLine;           // Line을 표시할지 여부
	BOOL              bHatching;       // 내부에 Hatching처리를 할지 여부 (현재(09.06.01) Rect, Polyline, Poligon만 사용가능)
	int               nPenStyle;       // 선형식(PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME)
	int               nPenSubStyle;    // end cap, join부에 대한 세부 형식(-1일 경우 nPenSubStyle사용안함. nPenStyle이 PS_SOLID형식이때만 적용됨, PS_ENDCAP_XXX, PS_JOIN_XXX... 등)
	int               Width;           // 선두께
	int               HatchingWidth;   // Hatching 선두께
	int               HatchingSpace;   // Hatching 간격
	double            dRWidth;         // 화살표 길이 비율(선두께에 대한)
	double            dRHeight;        // 화살표 높이 비율(선두께에 대한)
	EN_ARROW_TYPE    startArrowType;  // 시작점의 화살표 형식
	EN_ARROW_TYPE    endArrowType;    // 끝점의 화살표 형식
	EN_HATCHING_TYPE hatchingType;    // Hatching 형식
	EN_POINT_TYPE    pointType;       // Point 형식

	T_DRAW_FORMAT()	{ Init(); }
	T_DRAW_FORMAT(const T_DRAW_FORMAT& pData) { *this = pData; }
	T_DRAW_FORMAT& operator = (const T_DRAW_FORMAT& pData);

	void Init();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_TEXT_FORMAT

struct __MY_EXT_CLASS__ T_TEXT_FORMAT
{
	COLORREF          fontColor;    // 글자색
	COLORREF          fillColor;    // 배경색
	BOOL              bFill;        // 배경색을 채울지 여부
	int               Size;         // 글자크기
	BOOL              bHalfWidth;   // 폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	TShortStr         szFont;      // 글꼴
	EN_POSITION_TYPE positionType; // 기준점의 위치 형식
	int               nEscapement;  // 글자의 회전각(x축에서 부터 반시계방향, 1 = 1/10도 , ex) 90도->900) 

	T_TEXT_FORMAT() { Init(1); }
	T_TEXT_FORMAT(int nLang) { Init(nLang); }
	T_TEXT_FORMAT(const T_TEXT_FORMAT& pData)	{ *this = pData; }
	T_TEXT_FORMAT& operator = (const T_TEXT_FORMAT& pData);

	void Init(int nLanguage = 1);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIM_FORMAT

struct __MY_EXT_CLASS__ T_DIM_FORMAT
{  
	BOOL               bFontScale;    // 치수선을 그릴때 FontSize에 대한 비율로 그릴지의 여부 
	BOOL               bMainLine;     // MainLine을 그릴지 여부
	BOOL               bGuideLine_P1; // P1측의 GuideLine을 그릴지 여부
	BOOL               bGuideLine_P2; // P2측의 GuideLine을 그릴지 여부
	COLORREF           fontColor;     // 글자색
	COLORREF           lineColor;     // 선색
	int                Width;         // 선두께
	double             dRWidth;       // 화살표 길이 비율(선두께에 대한)
	double             dRHeight;      // 화살표 높이 비율(선두께에 대한)
	EN_ARROW_TYPE      arrowType;     // 화살표 형식
	double             dRTextSpace;   // 문자열 아래의 여유 공간 or 공간비율(항상 '+')(bFontScale == TRUE일때 글자 크기에 대한) 
	double             dRDimSpace;    // 해당 좌표에서 떨어지 거리 or 공간비율(bFontScale == TRUE일때 글자 크기에 대한) 
	double             dRGuideIn;     // 안쪽 안내선의 길이 or 공간비율(bFontScale == TRUE일때 글자 크기에 대한) 
	double             dRGuideOut;    // 바깥 안내선의 길이 or 공간비율(bFontScale == TRUE일때 글자 크기에 대한) 
	EN_DIM_TYPE		   dimType;       // 치수선 표시 형식
	BOOL               bSubDimType;   // 치수선의 길이가 문자열의 길이보다 작을 경우 dimType_Sub를 사용할지 여부
	EN_DIM_TYPE		   dimType_Sub;   // 문자열 길이를 확보하지 못할경우 사용될 하위 치수선 형식
	int                fontSize;      // 글자 크기
	BOOL               bFontHalfWidth;// 폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)
	TShortStr          szFont;       // 글꼴
	double             dOutTypeRate_L;// 문자열 길이를 확보하지 못할경우 좌측 바깥쪽으로 치수선을 그릴경우 바깥쪽으로 표현할 선의 길이 비율(화살표 길이에 대한 배수 음수일때은 기본값 2.0이 적용됨)
	double             dOutTypeRate_R;// 문자열 길이를 확보하지 못할경우 우측 바깥쪽으로 치수선을 그릴경우 바깥쪽으로 표현할 선의 길이 비율(화살표 길이에 대한 배수 음수일때은 기본값 2.0이 적용됨)

	//                                       bFontScale 가      TRUE일때               FALSE일때
	//
	//                      TEXT                       ＿
	//  │                   ↕dRTextSpace*FontSize │   ↕dRGuideOut*FontSize   or    dRGuideOut
	//  ├─────────────────────┤ ─
	//  │                                          │   ↑dRGuideIn*FontSize    or    dRGuideIn
	//  │                                          │ ＿↓
	//                                                   ↕dRDimSpace*FontSize   or    dRDimSpace
	//  ●P1                                      P2● ─
	//
	// dRGuideOut, dRGuideIn, dRDimSpace를 '-'값을 사용하면 아래쪽에 내려간 치수선을 만들 수 있다.


	T_DIM_FORMAT() { Init(); }
	T_DIM_FORMAT(int nLang) { Init(nLang); }
	T_DIM_FORMAT(const T_DIM_FORMAT& pData){*this = pData;}
	T_DIM_FORMAT& operator = (const T_DIM_FORMAT& pData);

	void Init(int nLanguage = 1);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_FORMAT_FRAME

struct __MY_EXT_CLASS__ T_CHART_FORMAT_FRAME
{
	COLORREF          lineColor;  // 테두리선색
	COLORREF          fillColor;  // 내부색
	BOOL              bLine;      // 테두리선을 표시할지 여부
	BOOL              bFill;      // 내부를 채울지 여부
	int               nPenStyle;  // 테두리선형식(PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME)
	int               Width;      // 테두리선두께
	COLORREF          fontColor;  // 글자색 (Title or Remark)
	int               fontSize;   // 글자크기	(Title or Remark)
	TShortStr         szFont;     // 글꼴 (Title or Remark)

	T_CHART_FORMAT_FRAME()	{ Init(1); }
	T_CHART_FORMAT_FRAME(int nLang) { Init(nLang); }
	T_CHART_FORMAT_FRAME(const T_CHART_FORMAT_FRAME& pData) { *this = pData; }
	T_CHART_FORMAT_FRAME& operator = (const T_CHART_FORMAT_FRAME& pData);

	void Init(int nLanguage = 1);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_FORMAT_AXIS

struct __MY_EXT_CLASS__ T_CHART_FORMAT_AXIS
{
	COLORREF          lineColor;   // 축 선색
	int               nPenStyle;   // 축 선형식(PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME)
	int               Width;       // 축 선두께
	COLORREF          majorGridColor;     // 주눈금선 선색
	int               nMajorGridPenStyle; // 주눈금선 선형식(PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME)
	int               nMajorGridWidth;    // 주눈금선 선두께
	COLORREF          minorGridColor;     // 보조눈금선 선색
	int               nMinorGridPenStyle; // 보조눈금선 선형식(PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME)
	int               nMinorGridWidth;    // 보조눈금선 선두께
	COLORREF          fontColor;   // 글자색
	int               fontSize;    // 글자크기	
	TShortStr         szFont;     // 글꼴  
	COLORREF          fontColor_Title;// Title글자색
	int               fontSize_Title; // Title글자크기	
	TShortStr         szFont_Title;  // Title글꼴  
	int               nEscapement; // 글자의 회전각(x축에서 부터 반시계방향, 1 = 1/10도 , ex) 90도->900) ※90도 초과는 지원하지 않음

	T_CHART_FORMAT_AXIS()	{ Init(1); }
	T_CHART_FORMAT_AXIS(int nLang) { Init(nLang); }
	T_CHART_FORMAT_AXIS(const T_CHART_FORMAT_AXIS& pData) { *this = pData; }
	T_CHART_FORMAT_AXIS& operator = (const T_CHART_FORMAT_AXIS& pData);

	void Init(int nLanguage = 1);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_FORMAT

struct __MY_EXT_CLASS__ T_CHART_FORMAT // Chart 틀의 표시형식 (색, 선형식, 글자크기 위주 )
{
	T_CHART_FORMAT_FRAME frame;
	T_CHART_FORMAT_FRAME remark;
	T_CHART_FORMAT_AXIS  xAxis;
	T_CHART_FORMAT_AXIS  yAxis;	
	BOOL                 bHalfWidth;   // 폰트의 폭의 크기를 높이의 1/2을 사용할지 여부(FALSE:default값적용, TRUE:높이의 1/2적용)

	T_CHART_FORMAT() { Init(1); }
	T_CHART_FORMAT(int nLang) { Init(nLang); }
	T_CHART_FORMAT(const T_CHART_FORMAT& pData) { *this = pData; }
	T_CHART_FORMAT& operator = (const T_CHART_FORMAT& pData);

	void Init(int nLanguage = 1);
};

#include "HeaderPost.h"