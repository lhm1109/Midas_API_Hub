#pragma once

#include "DrawDataObject.h"

#include "HeaderPre.h"

// struct //////////////////////////////////////////////////////////////////
// - List를 구성하는 구조체는 "DgnFigureDrawFunc.h"에 정의 되어 있음
/*               삽도상의 정보              실제 크기의 정보
Point         :  T_POINT_GROUP             T_POINT_R_GROUP          
Line          :  T_LINE_GROUP              T_LINE_R_GROUP          
ArrowLine     :  T_ARROWLINE_GROUP         T_ARROWLINE_R_GROUP     
Circle        :  T_CIRCLE_GROUP            T_CIRCLE_R_GROUP
Donut         :  T_DONUT_GROUP             T_DONUT_R_GROUP
Ellipse       :  T_ELLIPSE_GROUP           T_ELLIPSE_R_GROUP       
Rectangle     :  T_RECT_GROUP              T_RECT_R_GROUP          
Arc           :  T_ARC_GROUP               T_ARC_R_GROUP           
Polyline      :  T_POLYLINE_GROUP          T_POLYLINE_R_GROUP      
Polygon       :  T_POLYGON_GROUP           T_POLYGON_R_GROUP       
ArrowPolyline :  T_ARROWPOLYLINE_GROUP     T_ARROWPOLYLINE_R_GROUP 
Text          :  T_TEXT_GROUP              T_TEXT_R_GROUP          
Dimension     :  T_DIMENSION_GROUP         T_DIMENSION_R_GROUP     
Angle         :  T_DIMANGLE_GROUP          T_DIMANGLE_R_GROUP      
LeaderLine    :  T_DIMLEADERLINE_GROUP     T_DIMLEADERLINE_R_GROUP 
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POINT_GROUP 삽도상의 Point Group 정보

struct __MY_EXT_CLASS__ T_POINT_GROUP
{
	nrPOINT List;// 구성요소 리스트

	T_POINT_GROUP()	{}
	T_POINT_GROUP(const T_POINT_GROUP& pData) { *this = pData; }
	T_POINT_GROUP& operator = (const T_POINT_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POINT_R_GROUP 실제크기의 Line Group 정보

struct __MY_EXT_CLASS__ T_POINT_R_GROUP
{
	nrT_POINT List; // 구성요소 리스트
		
	T_POINT_R_GROUP() {}
	T_POINT_R_GROUP(const T_POINT_R_GROUP& pData) { *this = pData; }
	T_POINT_R_GROUP& operator = (const T_POINT_R_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_LINE_GROUP 삽도상의 Line Group 정보

struct __MY_EXT_CLASS__ T_LINE_GROUP
{
	nrT_LINE List;// 구성요소 리스트
		
	T_LINE_GROUP()	{}
	T_LINE_GROUP(const T_LINE_GROUP& pData)	 { *this = pData; }
	T_LINE_GROUP& operator = (const T_LINE_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_LINE_R_GROUP 실제크기의 Line Group 정보

struct __MY_EXT_CLASS__ T_LINE_R_GROUP
{
	nrT_LINE_R List;// 구성요소 리스트
		
	T_LINE_R_GROUP() { }
	T_LINE_R_GROUP(const T_LINE_R_GROUP& pData) { *this = pData; }
	T_LINE_R_GROUP& operator = (const T_LINE_R_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARROWLINE_GROUP 삽도상의 ArrowLine Group 정보

struct __MY_EXT_CLASS__ T_ARROWLINE_GROUP
{
	nrT_LINE List;// 구성요소 리스트
		
	T_ARROWLINE_GROUP()	{}
	T_ARROWLINE_GROUP(const T_ARROWLINE_GROUP& pData) { *this = pData; }
	T_ARROWLINE_GROUP& operator = (const T_ARROWLINE_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARROWLINE_R_GROUP 실제크기의 ArrowLine Group 정보

struct __MY_EXT_CLASS__ T_ARROWLINE_R_GROUP
{
	nrT_LINE_R List;// 구성요소 리스트
		
	T_ARROWLINE_R_GROUP()	{}
	T_ARROWLINE_R_GROUP(const T_ARROWLINE_R_GROUP& pData) { *this = pData; }
	T_ARROWLINE_R_GROUP& operator = (const T_ARROWLINE_R_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CIRCLE_GROUP 삽도상의 Circle Group 정보

struct __MY_EXT_CLASS__ T_CIRCLE_GROUP
{
	nrT_CIRCLE List;// 구성요소 리스트
		
	T_CIRCLE_GROUP()	{}
	T_CIRCLE_GROUP(const T_CIRCLE_GROUP& pData)	 { *this = pData; }
	T_CIRCLE_GROUP& operator = (const T_CIRCLE_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CIRCLE_R_GROUP 실제크기의 Circle Group 정보

struct __MY_EXT_CLASS__ T_CIRCLE_R_GROUP
{
	nrT_CIRCLE_R List;// 구성요소 리스트
		
	T_CIRCLE_R_GROUP()	{}
	T_CIRCLE_R_GROUP(const T_CIRCLE_R_GROUP& pData) { *this = pData; }
	T_CIRCLE_R_GROUP& operator = (const T_CIRCLE_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DONUT_GROUP 삽도상의 Donut Group 정보

struct __MY_EXT_CLASS__ T_DONUT_GROUP
{
	nrT_DONUT List;// 구성요소 리스트
		
	T_DONUT_GROUP()	{}
	T_DONUT_GROUP(const T_DONUT_GROUP& pData) { *this = pData; }
	T_DONUT_GROUP& operator = (const T_DONUT_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DONUT_R_GROUP 실제크기의 Donut Group 정보

struct __MY_EXT_CLASS__ T_DONUT_R_GROUP
{
	nrT_DONUT_R List;// 구성요소 리스트
		
	T_DONUT_R_GROUP()	{}
	T_DONUT_R_GROUP(const T_DONUT_R_GROUP& pData) { *this = pData; }
	T_DONUT_R_GROUP& operator = (const T_DONUT_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ELLIPSE_GROUP 삽도상의 Ellipse Group 정보

struct __MY_EXT_CLASS__ T_ELLIPSE_GROUP
{
	nrT_LINE List;// 구성요소 리스트
		
	T_ELLIPSE_GROUP()	{}
	T_ELLIPSE_GROUP(const T_ELLIPSE_GROUP& pData) { *this = pData; }
	T_ELLIPSE_GROUP& operator = (const T_ELLIPSE_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ELLIPSE_R_GROUP 실제크기의 Ellipse Group 정보

struct __MY_EXT_CLASS__ T_ELLIPSE_R_GROUP
{
	nrT_LINE_R List;// 구성요소 리스트
		
	T_ELLIPSE_R_GROUP()	{}
	T_ELLIPSE_R_GROUP(const T_ELLIPSE_R_GROUP& pData) { *this = pData; }
	T_ELLIPSE_R_GROUP& operator = (const T_ELLIPSE_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_RECT_GROUP 삽도상의 Rectangle Group 정보

struct __MY_EXT_CLASS__ T_RECT_GROUP
{
	nrT_LINE List;// 구성요소 리스트
		
	T_RECT_GROUP()	{}
	T_RECT_GROUP(const T_RECT_GROUP& pData)	 { *this = pData; }
	T_RECT_GROUP& operator = (const T_RECT_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_RECT_R_GROUP 실제크기의 Rectangle Group 정보

struct __MY_EXT_CLASS__ T_RECT_R_GROUP
{
	nrT_LINE_R List;// 구성요소 리스트
		
	T_RECT_R_GROUP() {}
	T_RECT_R_GROUP(const T_RECT_R_GROUP& pData) { *this = pData; }
	T_RECT_R_GROUP& operator = (const T_RECT_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARC_GROUP 삽도상의 Arc Group 정보

struct __MY_EXT_CLASS__ T_ARC_GROUP
{
	nrT_ARC List;// 구성요소 리스트
	
	T_ARC_GROUP()	{}
	T_ARC_GROUP(const T_ARC_GROUP& pData) { *this = pData; }
	T_ARC_GROUP& operator = (const T_ARC_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARC_R_GROUP 실제크기의 Arc Group 정보

struct __MY_EXT_CLASS__ T_ARC_R_GROUP
{
	nrT_ARC_R List;// 구성요소 리스트
		
	T_ARC_R_GROUP()	{}
	T_ARC_R_GROUP(const T_ARC_R_GROUP& pData) { *this = pData; }
	T_ARC_R_GROUP& operator = (const T_ARC_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYLINE_GROUP  삽도상의 Polyline Group 정보

struct __MY_EXT_CLASS__ T_POLYLINE_GROUP
{
	nrT_POLYLINE List;// 구성요소 리스트
		
	T_POLYLINE_GROUP()	{}
	T_POLYLINE_GROUP(const T_POLYLINE_GROUP& pData)	 { *this = pData; }
	T_POLYLINE_GROUP& operator = (const T_POLYLINE_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYLINE_R_GROUP 실제크기의 Polyline Group 정보

struct __MY_EXT_CLASS__ T_POLYLINE_R_GROUP
{
	nrT_POLYLINE_R List;// 구성요소 리스트
		
	T_POLYLINE_R_GROUP()	{}
	T_POLYLINE_R_GROUP(const T_POLYLINE_R_GROUP& pData)	 { *this = pData; }
	T_POLYLINE_R_GROUP& operator = (const T_POLYLINE_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYGON_GROUP 삽도상의 Polygon Group 정보

struct __MY_EXT_CLASS__ T_POLYGON_GROUP
{
	nrT_POLYLINE List;// 구성요소 리스트
		
	T_POLYGON_GROUP() {}
	T_POLYGON_GROUP(const T_POLYGON_GROUP& pData) { *this = pData; }
	T_POLYGON_GROUP& operator = (const T_POLYGON_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYGON_R_GROUP 실제크기의 Polygon Group 정보

struct __MY_EXT_CLASS__ T_POLYGON_R_GROUP
{
	nrT_POLYLINE_R List;// 구성요소 리스트
		
	T_POLYGON_R_GROUP()	{}
	T_POLYGON_R_GROUP(const T_POLYGON_R_GROUP& pData) { *this = pData; }
	T_POLYGON_R_GROUP& operator = (const T_POLYGON_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARROWPOLYLINE_GROUP 삽도상의 ArrowPolyline Group 정보

struct __MY_EXT_CLASS__ T_ARROWPOLYLINE_GROUP
{
	nrT_POLYLINE List;// 구성요소 리스트
		
	T_ARROWPOLYLINE_GROUP()	{}
	T_ARROWPOLYLINE_GROUP(const T_ARROWPOLYLINE_GROUP& pData) { *this = pData; }
	T_ARROWPOLYLINE_GROUP& operator = (const T_ARROWPOLYLINE_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARROWPOLYLINE_R_GROUP 실제크기의 ArrowPolyline Group 정보

struct __MY_EXT_CLASS__ T_ARROWPOLYLINE_R_GROUP
{
	nrT_POLYLINE_R List;// 구성요소 리스트
		
	T_ARROWPOLYLINE_R_GROUP()	{}
	T_ARROWPOLYLINE_R_GROUP(const T_ARROWPOLYLINE_R_GROUP& pData) { *this = pData; }
	T_ARROWPOLYLINE_R_GROUP& operator = (const T_ARROWPOLYLINE_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_TEXT_GROUP  삽도상의 Text Group 정보

struct __MY_EXT_CLASS__ T_TEXT_GROUP
{
	nrT_TEXT List;// 구성요소 리스트
		
	T_TEXT_GROUP()	{}
	T_TEXT_GROUP(const T_TEXT_GROUP& pData)	 { *this = pData; }
	T_TEXT_GROUP& operator = (const T_TEXT_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_TEXT_R_GROUP 실제크기의 Text Group 정보

struct __MY_EXT_CLASS__ T_TEXT_R_GROUP
{
	nrT_TEXT_R List;// 구성요소 리스트
		
	T_TEXT_R_GROUP()	{}
	T_TEXT_R_GROUP(const T_TEXT_R_GROUP& pData)	 { *this = pData; }
	T_TEXT_R_GROUP& operator = (const T_TEXT_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMENSION_GROUP 삽도상의 치수선 Group 정보

struct __MY_EXT_CLASS__ T_DIMENSION_GROUP
{
	nrT_DIMENSION List;// 구성요소 리스트
	
	T_DIMENSION_GROUP()	{}
	T_DIMENSION_GROUP(const T_DIMENSION_GROUP& pData) { *this = pData; }
	T_DIMENSION_GROUP& operator = (const T_DIMENSION_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMENSION_R_GROUP 실제크기의 치수선 Group 정보

struct __MY_EXT_CLASS__ T_DIMENSION_R_GROUP
{
	nrT_DIMENSION_R List;// 구성요소 리스트
		
	T_DIMENSION_R_GROUP()	{}
	T_DIMENSION_R_GROUP(const T_DIMENSION_R_GROUP& pData) { *this = pData; }
	T_DIMENSION_R_GROUP& operator = (const T_DIMENSION_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMANGLE_GROUP 삽도상의 각도치수선 Group 정보

struct __MY_EXT_CLASS__ T_DIMANGLE_GROUP
{
	nrT_ANGLE List;// 구성요소 리스트
		
	T_DIMANGLE_GROUP()	{}
	T_DIMANGLE_GROUP(const T_DIMANGLE_GROUP& pData) { *this = pData; }
	T_DIMANGLE_GROUP& operator = (const T_DIMANGLE_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMANGLE_R_GROUP 실제크기의 각도치수선 Group 정보

struct __MY_EXT_CLASS__ T_DIMANGLE_R_GROUP
{
	nrT_ANGLE_R List;// 구성요소 리스트
		
	T_DIMANGLE_R_GROUP()	{}
	T_DIMANGLE_R_GROUP(const T_DIMANGLE_R_GROUP& pData)	 { *this = pData; }
	T_DIMANGLE_R_GROUP& operator = (const T_DIMANGLE_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMLEADERLINE_GROUP 삽도상의 지시선 Group 정보

struct __MY_EXT_CLASS__ T_DIMLEADERLINE_GROUP
{
	nrT_LEADERLINE List;// 구성요소 리스트

	T_DIMLEADERLINE_GROUP()	{}
	T_DIMLEADERLINE_GROUP(const T_DIMLEADERLINE_GROUP& pData) { *this = pData; }
	T_DIMLEADERLINE_GROUP& operator = (const T_DIMLEADERLINE_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMLEADERLINE_R_GROUP  실제크기의 지시선 Group 정보

struct __MY_EXT_CLASS__ T_DIMLEADERLINE_R_GROUP
{
	nrT_LEADERLINE_R List;// 구성요소 리스트
	
	T_DIMLEADERLINE_R_GROUP()	{}
	T_DIMLEADERLINE_R_GROUP(const T_DIMLEADERLINE_R_GROUP& pData) { *this = pData; }
	T_DIMLEADERLINE_R_GROUP& operator = (const T_DIMLEADERLINE_R_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_GROUP 삽도상의 Chart Group 정보

struct __MY_EXT_CLASS__ T_CHART_GROUP
{
	nrT_CHART List;// 구성요소 리스트

	T_CHART_GROUP()	{}
	T_CHART_GROUP(const T_CHART_GROUP& pData) { *this = pData; }
	T_CHART_GROUP& operator = (const T_CHART_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_R_GROUP  실제크기의 Chart Group 정보

struct __MY_EXT_CLASS__ T_CHART_R_GROUP
{
	nrT_CHART_R List;// 구성요소 리스트

	T_CHART_R_GROUP()	{}
	T_CHART_R_GROUP(const T_CHART_R_GROUP& pData) { *this = pData; }
	T_CHART_R_GROUP& operator = (const T_CHART_R_GROUP& pData)
	{
		List.Copy(pData.List);
		return *this;
	}

	void Init() { List.RemoveAll(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_HATCHING_GROUP

struct __MY_EXT_CLASS__ T_HATCHING_GROUP // 
{
	nrT_HATCHING_D List;// 구성요소 리스트

	T_HATCHING_GROUP()	{}
	T_HATCHING_GROUP(const T_HATCHING_GROUP& pData) { *this = pData; }
	T_HATCHING_GROUP& operator = (const T_HATCHING_GROUP& pData)
	{
		List.Copy(pData.List); 
		return *this;
	}

	void Init() { List.RemoveAll(); }
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_OVERLAP_K

#define T_ED_OVERLAP_KEY unsigned int

union T_ED_OVERLAP_K
{
	T_ED_OVERLAP_KEY keymap;
	struct
	{
		unsigned int entity : 20; // 1048576까지
		unsigned int subID  : 12; //    4096까지
	} key;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_OVERLAP_RES_D

struct __MY_EXT_CLASS__ T_ED_OVERLAP_RES_D
{
	int   nCount;
	POINT CenterP;

	CArray<T_ED_OVERLAP_KEY, T_ED_OVERLAP_KEY> arKey;

	T_ED_OVERLAP_RES_D()	{}
	T_ED_OVERLAP_RES_D(const T_ED_OVERLAP_RES_D& pData) { *this = pData; }
	T_ED_OVERLAP_RES_D& operator = (const T_ED_OVERLAP_RES_D& pData)
	{
		nCount  = pData.nCount;
		CenterP = pData.CenterP;
		arKey.Copy(pData.arKey);
		return *this;
	}

	void Init()
	{
		nCount = 0; 
		CenterP.x = 0;
		CenterP.y = 0; 
		arKey.RemoveAll(); 
	}

	void Sum(T_ED_OVERLAP_RES_D& pData, int nSumType);
};

#include "HeaderPost.h"


