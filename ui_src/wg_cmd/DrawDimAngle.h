#pragma once

#include "DrawBase.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDrawDimAngle : public CDrawBase  
{
public:
	CDrawDimAngle();
	virtual ~CDrawDimAngle();

public:
	T_DIMANGLE_R_GROUP m_DataR;
	T_DIMANGLE_GROUP   m_Data;

	// Overrides ※주의:현재(08.03.4) 치수선의 영역및 선택 위치는 해당 점에 대해서만 고려
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