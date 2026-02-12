#pragma once

#include <math.h>

typedef char xBOOL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Colors

#define cBlack		RGB(  0,   0,   0)
#define cWhite		RGB(255, 255, 255)
#define clGray		RGB(220, 220, 220)
#define cllGray		RGB(230, 230, 230)
#define cGray		RGB(190, 190, 190)
#define cdGray		RGB(128, 128, 128)
#define cddGray		RGB(105, 105, 105)
#define cRed		RGB(255,   0,   0)
#define clRed		RGB(255,  80,  80)
#define cllRed		RGB(255, 150, 150)
#define cdRed		RGB(128,   0,   0)
#define cddRed		RGB( 80,   0,   0)
#define cdlRed		RGB(200,   0,   0)
#define cddlRed		RGB(180,   0,   0)
#define cGreen		RGB(  0, 255,   0)
#define clGreen		RGB( 80, 255,  80)
#define cllGreen	RGB(150, 255, 150)
#define cdGreen		RGB(  0, 128,   0)
#define cddGreen	RGB(  0,  80,   0)
#define cdlGreen	RGB(  0, 200,   0)
#define cddlGreen	RGB(  0, 180,   0)
#define cBlue		RGB(  0,   0, 255)
#define clBlue		RGB( 80,  80, 255)
#define cllBlue		RGB(150, 150, 255)
#define clllBlue	RGB(234, 254, 254)
#define cdBlue		RGB(  0,   0, 128)
#define cddBlue		RGB(  0,   0,  80)
#define cdlBlue		RGB(  0,   0, 200)
#define cddlBlue	RGB(  0,   0, 180)
#define cYellow		RGB(255, 255,   0)
#define clYellow	RGB(255, 255,  80)
#define cllYellow	RGB(255, 255, 150)
#define clllYellow	RGB(250, 250, 210)
#define cdYellow	RGB(128, 128,   0)
#define cddYellow	RGB( 80,  80,   0)
#define cdlYellow	RGB(200, 200,   0)
#define cddlYellow	RGB(180, 180,   0)
#define cCyan		RGB(  0, 255, 255)
#define clCyan		RGB( 80, 255, 255)
#define cllCyan		RGB(150, 255, 255)
#define cdCyan		RGB(  0, 128, 128)
#define cddCyan		RGB(  0,  80,  80)
#define cdlCyan		RGB(  0, 200, 200)
#define cddlCyan	RGB(  0, 180, 180)
#define cMagenta	RGB(255,   0, 255)
#define clMagenta	RGB(255,  80, 255)
#define cllMagenta	RGB(255, 150, 255)
#define cdMagenta	RGB(128,   0, 128)
#define cddMagenta	RGB( 80,   0,  80)
#define cdlMagenta	RGB(200,   0, 200)
#define cddlMagenta	RGB(180,   0, 180)
#define cPink		RGB(255, 128, 128)
#define cOrange		RGB(255, 128,   0)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math

#define ONE_EPSILON	2.220446049250313e-08
#ifndef PI
#define PI			3.1415926535897932384626433832795
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dimension Shape, Text Direction Flag 

#define	DIM_SHP_LINE		(1<<0)	// |--
#define	DIM_SHP_ARROW		(1<<1)	// <--
#define	DIM_SHP_TRI			(1<<2)	// <I--
#define	DIM_SHP_RECT		(1<<3)	// ㅁ--
#define	DIM_SHP_DIAMOND		(1<<4)	// <>--
#define	DIM_SHP_CIRCLE		(1<<5)	// O--
#define	DIM_IN_FILL			(1<<6)
#define	DIM_DIR_LT			(1<<7)
#define	DIM_DIR_RT			(1<<8)
#define	DIM_DIR_UP			(1<<9)
#define	DIM_DIR_DN			(1<<10)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Font Type Flag 

#define	FONT_TYPE_BOLD		(1<<0)
#define	FONT_TYPE_UNDER		(1<<1)
#define	FONT_TYPE_ITALIC	(1<<2)


// enum ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum EN_DRFORMAT_TYPE
{
	EN_DRFORMAT_TYPE_NONE,
	EN_DRFORMAT_TYPE_DRAW,
	EN_DRFORMAT_TYPE_TEXT,
	EN_DRFORMAT_TYPE_DIM,
	EN_DRFORMAT_TYPE_CHART
};

enum EN_DRCLASS_TYPE // 그림을 그릴 개별 정보를 관리하는 Class의 종류
{
	EN_DRCLASS_TYPE_NONE,
	EN_DRCLASS_TYPE_POINT,
	EN_DRCLASS_TYPE_LINE,
	EN_DRCLASS_TYPE_ARROWLINE,
	EN_DRCLASS_TYPE_CIRCLE,
	EN_DRCLASS_TYPE_DONUT,
	EN_DRCLASS_TYPE_ELLIPSE,
	EN_DRCLASS_TYPE_RECT,
	EN_DRCLASS_TYPE_ARC,
	EN_DRCLASS_TYPE_POLYLINE,
	EN_DRCLASS_TYPE_POLYGON,
	EN_DRCLASS_TYPE_ARROWPOLYLINE,
	EN_DRCLASS_TYPE_TEXT,
	EN_DRCLASS_TYPE_DIMENSION,
	EN_DRCLASS_TYPE_DIMANGLE,
	EN_DRCLASS_TYPE_DIMLEADERLINE,	
	EN_DRCLASS_TYPE_CHART
};

enum EN_ARROW_TYPE // DGN삽도를 위한 화살표 형상
{
	EN_ARROW_TYPE_NONE,   // 화살표 없음 
	EN_ARROW_TYPE_ARROW,  // 채워진 화살표
	EN_ARROW_TYPE_OPEN,   // 열린 화살표
	EN_ARROW_TYPE_DIAMOND,// 다이아몬드형 
	EN_ARROW_TYPE_OVAL    // 타원형
};

enum EN_POINT_TYPE // DGN삽도를 위한 Point 형상
{
	EN_POINT_TYPE_NONE,      // Point 없음 
	EN_POINT_TYPE_DOT,       // 채워진 원
	EN_POINT_TYPE_RECT,      // 사각형
	EN_POINT_TYPE_DIAMOND,   // 다이아몬드형 
	EN_POINT_TYPE_TRIANGLE,  // 정삼각형 	
	EN_POINT_TYPE_CIRCLE,    // 원형 	
	EN_POINT_TYPE_CROSS,     // 'x'
	EN_POINT_TYPE_STAR,      // '*'
	EN_POINT_TYPE_PLUS,      // '+'
	EN_POINT_TYPE_MINUS,     // '-'
	EN_POINT_TYPE_RECT_B,    // 사각형(Bold) 
	EN_POINT_TYPE_DIAMOND_B, // 다이아몬드형(Bold)  
	EN_POINT_TYPE_TRIANGLE_B,// 정삼각형(Bold)  	
	EN_POINT_TYPE_CIRCLE_B,  // 원형(Bold)  	
	EN_POINT_TYPE_CROSS_B,   // 'x'(Bold) 
	EN_POINT_TYPE_STAR_B,    // '*'(Bold) 
	EN_POINT_TYPE_PLUS_B,    // '+'(Bold) 
	EN_POINT_TYPE_MINUS_B,   // '-'(Bold) 
	EN_POINT_TYPE_RECT32     // 3:2사각형
};

enum EN_MARK_TYPE  // Report Draw의 Mark의 형태 
{
	EN_MARK_PIXEL,
	EN_MARK_RECT_FILL,  
	EN_MARK_RECT,
	EN_MARK_CIRCLE_FILL, 
	EN_MARK_CIRCLE,
	EN_MARK_TRI_UP_FILL,
	EN_MARK_TRI_UP,
	EN_MARK_TRI_DN_FILL,
	EN_MARK_TRI_DN,
	EN_MARK_TRI_LT_FILL,
	EN_MARK_TRI_LT,
	EN_MARK_TRI_RT_FILL,
	EN_MARK_TRI_RT,
	EN_MARK_DIAMOND_FILL,
	EN_MARK_DIAMOND
};

enum EN_DIM_TYPE // DGN삽도를 위한 치수선 Text위치 
{                                     // <치수선>             <각도>            <지시선>
	EN_DIM_TYPE_TOP,                  // 치수선 상단          각도 외부         종점뒤 연장선의 위
	EN_DIM_TYPE_CENTER,               // 치수선 중앙          각도 중앙         종점뒤
	EN_DIM_TYPE_BOTTOM,               // 치수선 하단          각도 내부         종점뒤 연정선의 아래
	EN_DIM_TYPE_OUT_LEFT,             // 상좌로 지시선        좌로 지시선       ×(종점뒤)
	EN_DIM_TYPE_OUT_RIGHT,            // 상우로 지시선        우로 지시선       ×(종점뒤)
	EN_DIM_TYPE_OUT_START,            // ×(치수선 중앙)      시점Guide위       ×(종점뒤) 
	EN_DIM_TYPE_OUT_END,              // ×(치수선 중앙)      종점Guide위       ×(종점뒤)
	// 아래 항목은 내부에 글자를 표현하지 못할때 Sub형식만 지원하는 열거체
	EN_DIM_SUB_TYPE_TOP_OUT_LEFT,     // 치수선 밖 좌측 상단  ×(각도 상단)     ×(종점뒤 연장선의 위)
	EN_DIM_SUB_TYPE_TOP_OUT_RIGHT,    // 치수선 밖 우측 상단 	×(각도 상단)     ×(종점뒤 연장선의 위)
	EN_DIM_SUB_TYPE_CENTER_OUT_LEFT,  // 치수선 밖 좌측 중앙  ×(각도 중앙)     ×(종점뒤)
	EN_DIM_SUB_TYPE_CENTER_OUT_RIGHT, // 치수선 밖 우측 중앙 	×(각도 중앙)     ×(종점뒤)
	EN_DIM_SUB_TYPE_BOTTOM_OUT_LEFT,  // 치수선 밖 좌측 하단  ×(각도 하단)     ×(종점뒤 연정선의 아래)
	EN_DIM_SUB_TYPE_BOTTOM_OUT_RIGHT  // 치수선 밖 우측 하단 	×(각도 하단)     ×(종점뒤 연정선의 아래)
};

enum EN_POSITION_TYPE
{  
	EN_POSITION_TYPE_LEFT_TOP = 1,
	EN_POSITION_TYPE_LEFT_VCENTER= 2,
	EN_POSITION_TYPE_LEFT_BOTTOM = 3,
	EN_POSITION_TYPE_CENTER_TOP = 4,
	EN_POSITION_TYPE_CENTER_VCENTER = 5,
	EN_POSITION_TYPE_CENTER_BOTTOM = 6, // Center Bottom 은 뭔가 버그가 있음.. 고쳐서 쓰거나 쓰지마세요.. 
	EN_POSITION_TYPE_RIGHT_TOP = 7,
	EN_POSITION_TYPE_RIGHT_VCENTER = 8,
	EN_POSITION_TYPE_RIGHT_BOTTOM = 9
};

enum EN_CHART_TYPE
{
	EN_CHART_TYPE_DISPERSAL              =   1 // 분산형
};

enum EN_CHART_DATANAME_POSI_TYPE // Chart Data의 이름표시위치
{
	EN_CHART_DATANAME_POSI_NONE          =   0, // 표시하지 않음
	EN_CHART_DATANAME_POSI_REMARK        =   1, // Remark내에 표시(Remark표현시)
	EN_CHART_DATANAME_POSI_START         =   2, // Data의 시작위치 표시
	EN_CHART_DATANAME_POSI_END           =   3, // Data의 끝 표시
};

enum EN_HATCHING_TYPE
{ 
	EN_HATCHING_TYPE_NONE                   =   0, // 그리지 않음
	EN_HATCHING_TYPE_HORIZONTAL             =   1, // 수평으로 빗금
	EN_HATCHING_TYPE_VERTICAL               =   2, // 수직으로 빗금
	EN_HATCHING_TYPE_BDIAGONAL              =   3, // 45도 우측에서 아래로 빗금  
	EN_HATCHING_TYPE_FDIAGONAL              =   4, // 45도 좌측에서 아래로 빗금
	EN_HATCHING_TYPE_CROSS                  =   5, // 십자가형태
	EN_HATCHING_TYPE_DIAGCROSS              =   6, // X자형태
	EN_HATCHING_TYPE_DOTS                   =   7, // 점(원형)을 지그제그로 배치 (EMF 출력시 점의크기(선두께)가 너무 작을 경우 표시 되지 않을 수 있음)
	EN_HATCHING_TYPE_DOTS_B                 =   8, // 굵은 점(원형)을 지그제그로 배치
	EN_HATCHING_TYPE_RECTDOTS               =   9, // 점(사각형3:2)을 지그제그로 배치
	EN_HATCHING_TYPE_CIRCLE                 =  10, // 원을 지그제그로 배치
	EN_HATCHING_TYPE_DUPLEX_HORIZONTAL      =  11, // 이중으로 된 수평으로 빗금
	EN_HATCHING_TYPE_DUPLEX_VERTICAL        =  12, // 이중으로 된 수직으로 빗금
	EN_HATCHING_TYPE_DUPLEX_BDIAGONAL       =  13, // 이중으로 된 45도 우측에서 아래로 빗금  
	EN_HATCHING_TYPE_DUPLEX_FDIAGONAL       =  14, // 이중으로 된 45도 좌측에서 아래로 빗금	
	EN_HATCHING_TYPE_DUPLEX_CROSS           =  15, // 이중으로 된 십자가형태
	EN_HATCHING_TYPE_DUPLEX_DIAGCROSS       =  16, // 이중으로 된 X자형태
	EN_HATCHING_TYPE_DUPLEX_CIRCLE          =  19, // 크기가 다른 원을 이중으로 배치
	EN_HATCHING_TYPE_PLUS                   =  21, // '+'기호를 지그제그로 배치
	EN_HATCHING_TYPE_MINUS                  =  22, // '-'기호를 지그제그로 배치
	EN_HATCHING_TYPE_CHECKER                =  23, // 체크무늬(ex 체크판)
	EN_HATCHING_TYPE_BRICK                  =  24, // 벽둘무늬
	EN_HATCHING_TYPE_GRAVEL                 =  25, // 자갈무늬
	EN_HATCHING_TYPE_HORIZONTAL_CYLINDER    =  26, // 수직 원통모양(수직선을 양옆으로 촘촘하게 배치)
	EN_HATCHING_TYPE_VERTICAL_CYLINDER      =  27, // 수평 원통모양(수직선을 아래위로 촘촘하게 배치)	
	EN_HATCHING_TYPE_HORIZONTAL_CYLINDER_SQ =  28, // 수직 원통모양(수직선을 양옆으로 촘촘하게 배치)-간격격차증가
	EN_HATCHING_TYPE_VERTICAL_CYLINDER_SQ   =  29, // 수평 원통모양(수직선을 아래위로 촘촘하게 배치)-간격격차증가
	EN_HATCHING_TYPE_CIRCLE_EQUAL           =  30, // 원 + '='기호가 지그제그로 배치
	EN_HATCHING_TYPE_PLUS_BDIAGONAL         = 101, // 1줄 BDIAGONAL와 '+'기호가 반복
	EN_HATCHING_TYPE_PLUS_BDIAGONAL2        = 102, // 2줄 BDIAGONAL와 '+'기호가 반복
	EN_HATCHING_TYPE_PLUS_BDIAGONAL3        = 103, // 3줄 BDIAGONAL와 '+'기호가 반복
	EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL    = 111, // 수평으로 빗금 + 점(사각형3:2)
	EN_HATCHING_TYPE_RECTDOTS_VERTICAL      = 112, // 수직으로 빗금 + 점(사각형3:2)
	EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL     = 113, // 45도 우측에서 아래로 빗금 + 점(사각형3:2)
	EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL     = 114, // 45도 좌측에서 아래로 빗금 + 점(사각형3:2)
	EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL_S  = 115, // 수평으로 빗금 + 점(사각형3:2) Small
	EN_HATCHING_TYPE_RECTDOTS_VERTICAL_S    = 116, // 수직으로 빗금 + 점(사각형3:2) Small
	EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL_S   = 117, // 45도 우측에서 아래로 빗금 + 점(사각형3:2) Small
	EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL_S   = 118, // 45도 좌측에서 아래로 빗금 + 점(사각형3:2) Small
	EN_HATCHING_TYPE_BDIAGONAL3_FDIAGONAL   = 121, // 45도 우측에서 아래로 빗금 3 + 45도 좌측에서 아래로 빗금
	EN_HATCHING_TYPE_FDIAGONAL3_BDIAGONAL   = 122, // 45도 좌측에서 아래로 빗금 3 + 45도 우측에서 아래로 빗금
	EN_HATCHING_TYPE_GRAVEL_DOTS            = 131, // 자갈무늬 + 내부점
};

enum EN_OVERLAPCHECK_TYPE
{
	EN_OVERLAP_NONE   = 0, 
	EN_OVERLAP_OBJECT = 1,
};

enum EN_MIRRORVIEW_TYPE
{
	EN_MIRRORVIEW_TYPE_NONE = 0,
	EN_MIRRORVIEW_TYPE_HORIZONTAL = 1,
	EN_MIRRORVIEW_TYPE_VERTICAL = 2,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef CArray<int, int>			nrINT;
typedef CArray<UINT, UINT>			nrUINT;
typedef CArray<double, double>		nrDOUBLE;
typedef CArray<float, float>		nrFLOAT;
typedef CArray<long, long>			nrLONG;
typedef CArray<BOOL, BOOL>			nrBOOL;
typedef CArray<POINT, POINT>		nrPOINT;
typedef CArray<SIZE, SIZE>			nrSIZE;
typedef CArray<RECT, RECT>			nrRECT;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline Function 

inline double WINAPI DEG_TO_RAD(double v) { return PI / 180.0 * v; }  // Radian = PI / 180 * Degree
inline double WINAPI RAD_TO_DEG(double v) { return v * 180.0 / PI; }  // Degree = Radian * 180 / PI

inline DWORD WINAPI GetReverseColor(DWORD c)
{
	BYTE  r = (BYTE)(255 - (int)GetRValue(c));
	BYTE  g = (BYTE)(255 - (int)GetGValue(c));
	BYTE  b = (BYTE)(255 - (int)GetBValue(c));
	DWORD rgb = (DWORD)((r) | ((WORD)(g) << 8) | ((DWORD)(b) << 16));

	return rgb;
}


template <int iSize>
struct TString
{
	TCHAR str[iSize];

	TString() 
	{ 
		str[0] = NULL; 
 	}
	TString(const CString& csStr)
	{
// 		_tcsncpy(str, iSize, (LPCTSTR)csStr, iSize-1);
// 		str[iSize-1] = NULL;
		_tcsncpy(str, (LPCTSTR)csStr, iSize-1);
	}
	TString(LPCTSTR lpszStr)
	{
// 		_tcsncpy(str, iSize, lpszStr, iSize-1);
// 		str[iSize-1] = NULL;
		_tcsncpy(str, lpszStr, iSize-1);
	}
	TString& operator = (const CString csStr)
	{
// 		_tcsncpy(str, iSize, (LPCTSTR)csStr, iSize-1);
// 		str[iSize-1] = NULL;
		_tcsncpy(str, (LPCTSTR)csStr, iSize-1);
		return *this;
	}
	TString& operator = (LPCTSTR lpszStr)
	{
// 		_tcsncpy(str, iSize, lpszStr, iSize-1);
// 		str[iSize-1] = NULL;
		_tcsncpy(str, lpszStr, iSize-1);
		return *this;
	}
	BOOL operator == (const TString& tsStr) const
	{
		if( _tcscmp( str, tsStr.str ) == 0 ) return TRUE;
		return FALSE;
	}
	BOOL operator == (const CString& cstrStr) const
	{
		if( _tcscmp( str, (LPCTSTR)cstrStr ) == 0 ) return TRUE;
		return FALSE;
	}
	BOOL operator == (LPCTSTR lpszStr) const
	{
		if( _tcscmp( str, lpszStr ) == 0 ) return TRUE;
		return FALSE;
	}
	BOOL operator != (const TString& tsStr) const
	{
		if( _tcscmp( str, tsStr.str ) == 0 ) return FALSE;
		return TRUE;
	}
	BOOL operator != (const CString& cstrStr) const
	{
		if( _tcscmp( str, (LPCTSTR)cstrStr ) == 0 ) return FALSE;
		return TRUE;
	}
	BOOL operator != (LPCTSTR lpszStr) const
	{
		if( _tcscmp( str, lpszStr ) == 0 ) return FALSE;
		return TRUE;
	}
	long GetLength() const
	{
		return (long)_tcslen( str );
	}
	BOOL IsEmpty() const
	{
		return (GetLength() == 0);
	}
	TCHAR* GetBuffer() 
	{
		return str;
	}
	operator LPCTSTR()
	{
		return (LPCTSTR)str;
	}

	int Find(TCHAR ch, int iStart = 0) const throw()
	{
		ASSERT(iStart >= 0);
		int nLength = GetLength();
		if(iStart < 0 || iStart >= nLength) return -1;
		LPCTSTR psz = _tcschr(str + iStart, ch);
		return psz == nullptr ? -1 : int(psz - str);
	}

	int Find(TString szSub, int iStart = 0) const throw()
	{
		ASSERT(iStart >= 0);
		if(szSub == _T("")) return -1;
		int nLength = GetLength();
		if(iStart < 0 || iStart > nLength) return -1;
		LPCTSTR psz = _tcsstr(str + iStart, szSub);
		return psz == nullptr ? -1 : int(psz - str);
	}

	// Trim
	TString TrimLeft(const TCHAR *targets = nullptr)
	{
		TCHAR* szStr = str;
		if (!szStr) return nullptr;
		if (!targets) targets = _T(" \t\r\n");
		while(*szStr)
		{
			if (!_tcschr(targets, *str)) return str;
			szStr++;
		}
		return str;
	}
	TString TrimRight(const TCHAR *targets = nullptr)
	{
		if (!str) return nullptr;
		TCHAR *end;
		if (!targets) targets = _T(" \t\n\r");
		end = str + _tcslen(str);
		while (end-- > str)
		{
			if (!_tcschr(targets, *end)) return str;
			*end = 0;
		}
		return str;
	}
	TString Trim(const TCHAR *targets = nullptr)
	{
		TrimRight(targets);
		return TrimLeft(targets);
	}

	// Replace 
	TString Replace(TCHAR chOld, TCHAR chNew)
	{
		if (!str) return str;
		TCHAR *str1 = str;
		while (*str1)
		{
			if (*str1 == chOld) *str1 = chNew;
			str1++;
		}
		return str;
	}
};

typedef TString<32>			TShortStr;
typedef TString<64>			TStr;
typedef TString<128>		TLongStr;
typedef TString<260>		TPathStr;
typedef TString<512>        TLongPathStr;
typedef TString<1024>       TLargeStr; 