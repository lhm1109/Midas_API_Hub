#include "Stdafx.h"
#include "wg_cmd.h"

// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"
#include "../wg_db/MathFunc.h"

#include <FLOAT.H>
#include "DrawCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDrawCtrl::DelDrawUnit(UINT key)
{
	m_arSelectedDrawUnitKey.RemoveKey(key);
	m_arHideDrawUnitKey.RemoveKey(key);

	CDrawPoint        *pDC_Point = NULL;
	CDrawLine         *pDC_Line = NULL;
	CDrawArrowLine    *pDC_ArrowLine = NULL;
	CDrawCircle       *pDC_Circle = NULL;
	CDrawDonut        *pDC_Donut = NULL;
	CDrawEllipse      *pDC_Ellipse = NULL;
	CDrawRect         *pDC_Rect = NULL;
	CDrawArc          *pDC_Arc = NULL;
	CDrawPolyline     *pDC_Polyline = NULL;
	CDrawPolygon      *pDC_Polygon = NULL;
	CDrawArrowPolyline*pDC_ArrowPolyline = NULL;
	CDrawText         *pDC_Text = NULL;
	CDrawDimension    *pDC_Dimension = NULL;
	CDrawDimAngle     *pDC_DimAngle = NULL;
	CDrawDimLeaderline*pDC_DimLeaderline = NULL;
	CDrawChart        *pDC_Chart = NULL;

	if(m_arPoint.Lookup(key, pDC_Point))			{ delete pDC_Point;  m_arPoint.RemoveKey(key); }
	if(m_arLine.Lookup(key, pDC_Line))				{ delete pDC_Line;  m_arLine.RemoveKey(key); }
	if(m_arArrowLine.Lookup(key, pDC_ArrowLine))	{ delete pDC_ArrowLine;  m_arArrowLine.RemoveKey(key); }
	if(m_arCircle.Lookup(key, pDC_Circle))			{ delete pDC_Circle;  m_arCircle.RemoveKey(key); }
	if(m_arDonut.Lookup(key, pDC_Donut))		 	{ delete pDC_Donut;  m_arDonut.RemoveKey(key); }
	if(m_arEllipse.Lookup(key, pDC_Ellipse))		{ delete pDC_Ellipse;  m_arEllipse.RemoveKey(key); }
	if(m_arRect.Lookup(key, pDC_Rect))				{ delete pDC_Rect;  m_arRect.RemoveKey(key); }
	if(m_arArc.Lookup(key, pDC_Arc))				{ delete pDC_Arc;  m_arArc.RemoveKey(key); }
	if(m_arPolyline.Lookup(key, pDC_Polyline))	{ delete pDC_Polyline; m_arPolyline.RemoveKey(key); }
	if(m_arPolygon.Lookup(key, pDC_Polygon))		{ delete pDC_Polygon;  m_arPolygon.RemoveKey(key); }
	if(m_arArrowPolyline.Lookup(key, pDC_ArrowPolyline))	{ delete pDC_ArrowPolyline;  m_arArrowPolyline.RemoveKey(key); }
	if(m_arText.Lookup(key, pDC_Text))				{ delete pDC_Text;  m_arText.RemoveKey(key); }
	if(m_arDimension.Lookup(key, pDC_Dimension))  { delete pDC_Dimension;  m_arDimension.RemoveKey(key); }
	if(m_arDimAngle.Lookup(key, pDC_DimAngle))  	{ delete pDC_DimAngle;  m_arDimAngle.RemoveKey(key); }
	if(m_arDimLeaderline.Lookup(key, pDC_DimLeaderline))	{ delete pDC_DimLeaderline;  m_arDimLeaderline.RemoveKey(key); }
	if(m_arChart.Lookup(key, pDC_Chart))			{ delete pDC_Chart;  m_arChart.RemoveKey(key); }

	if(m_FinalEntityNum == key) m_FinalEntityNum--;
}

void CDrawCtrl::DelDrawUnit(nrUINT& arKey)
{
	nrUINT arKey_Sort;
	// 작은순서로 배열
	int nSize, i, j;
	UINT TempKey;

	nSize = arKey.GetSize();

	if(nSize <= 0) return;
	if(nSize == 1) 	
	{
		arKey_Sort.SetSize(1);
		arKey_Sort[0] = arKey[0];	
	}

	for(i = 0; i < nSize - 1; i++)
	{
		for(j = i + 1; j < nSize; j++)
		{
			if(arKey_Sort[i] > arKey_Sort[j])
			{
				TempKey       = arKey_Sort[i];
				arKey_Sort[i] = arKey_Sort[j];
				arKey_Sort[j] = TempKey;
			}
		}
	}

	for(i = 0; i < nSize ; i++)
	{
		DelDrawUnit(arKey_Sort[i]);
	}
}
