#pragma once

#include "DrawBase.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDrawRect : public CDrawBase  
{
public:
	CDrawRect();
	virtual ~CDrawRect();

public:
	T_RECT_R_GROUP   m_DataR;
	T_RECT_GROUP     m_Data;
	T_HATCHING_GROUP m_Hatching;

	// Overrides
public:
	virtual int  GetClassType();
	virtual int  GetFormatType();
	
	virtual void Draw(HDC DC, CDrawFunc* pDrawFunc, void* pDrawFormat);
	
	virtual void Convert(BOOL bCDC, CDrawFunc* pDrawFunc, T_PAPER_FORMAT* pPaperFormat, void* pDrawFormat);
	
	virtual void GetCalcBoundaryR(double Boundary[4]);// Boundary[0]: x_min, [1]:y_min, [2]:x_max, [3]:y_max	
	virtual void GetCalcBoundary(int    Boundary[4]);// Boundary[0]: x_min, [1]:y_min, [2]:x_max, [3]:y_max	
	
	virtual void Move(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat); // Vector[0]: x, [1]:y	
	
	virtual BOOL IsSelectCheck(POINT sPoint, BOOL bFill=FALSE, int iTol=1); // 
	virtual BOOL IsSnapCheck(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR);
	virtual BOOL GetCenterPoint(int nID, POINT& CenterP);

};

#include "HeaderPost.h"