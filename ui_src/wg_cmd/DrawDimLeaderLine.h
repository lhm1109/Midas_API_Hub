#pragma once

#include "DrawBase.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDrawDimLeaderline : public CDrawBase  
{
public:
	CDrawDimLeaderline();
	virtual ~CDrawDimLeaderline();

public:
	T_DIMLEADERLINE_R_GROUP m_DataR;
	T_DIMLEADERLINE_GROUP   m_Data;

	// Overrides ※주의:현재(08.03.4)화살표형상 및 Text에 대한 영역은 고려하지 않습니다.
public:
	virtual int  GetClassType();
	virtual int  GetFormatType();
	
	virtual void Draw(HDC DC, CDrawFunc* pDrawFunc, void* pDrawFormat);
	
	virtual void Convert(BOOL bCDC, CDrawFunc* pDrawFunc, T_PAPER_FORMAT* pPaperFormat, void* pDrawFormat);
	
	virtual void GetCalcBoundaryR(double Boundary[4]);// Boundary[0]: x_min, [1]:y_min, [2]:x_max, [3]:y_max	
	virtual void GetCalcBoundary (int    Boundary[4]);// Boundary[0]: x_min, [1]:y_min, [2]:x_max, [3]:y_max	
	
	virtual void Move(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat); // Vector[0]: x, [1]:y	
	
	virtual BOOL IsSelectCheck(POINT sPoint, BOOL bFill=FALSE, int iTol=1); // 
	virtual BOOL IsSnapCheck(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR);
	virtual BOOL GetCenterPoint(int nID, POINT& CenterP);

};

#include "HeaderPost.h"
