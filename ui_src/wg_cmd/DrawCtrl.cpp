#include "Stdafx.h"
#include "wg_cmd.h"

// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"
#include "../wg_db/MathFunc.h"

// #include "../NS_opt/IOptionDB.h"
// #include "../NS_opt/OptionDBStruct.h"


#include <FLOAT.H>
#include "DrawCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawCtrl::CDrawCtrl()
{
	m_pMessageStatic = NULL;
	m_PaperFormat.Init();
	m_DrawFunc.ResetVariables();
	RemoveDrawData();	

	m_nMirrorMode    = EN_MIRRORVIEW_TYPE_NONE;
	m_SelectDrawType = 0;
	m_HideType   = 0;
	m_nSelectDis = 1;

	
	m_arDrawFormat.InitHashTable(50);
	m_arTextFormat.InitHashTable(50);
	m_arDimFormat.InitHashTable(50);
	m_arChartFormat.InitHashTable(50);
}

CDrawCtrl::~CDrawCtrl()
{
	RemoveDrawData();
}

void CDrawCtrl::GetMinMax(double& dXmin, double& dXmax, double& dYmin, double& dYmax)
{
	BOOL bFirst = TRUE;
	double Boundary [4] = {0.0, 0.0, 0.0, 0.0};
	double BoundaryU[4] = {0.0, 0.0, 0.0, 0.0};

	CDrawBase* pDrawClass=NULL;
	for(UINT nIndex = 1; nIndex <= m_FinalEntityNum; nIndex++)
	{
		pDrawClass = GetDrawClass(nIndex);
		if (pDrawClass != NULL)
		{
			if (bFirst)
			{
				pDrawClass->GetCalcBoundaryR(Boundary);
				bFirst = FALSE;
			}			
			else
			{
				pDrawClass->GetCalcBoundaryR(BoundaryU);
				Boundary[0] = min(Boundary[0], BoundaryU[0]);
				Boundary[1] = min(Boundary[1], BoundaryU[1]);
				Boundary[2] = max(Boundary[2], BoundaryU[2]);
				Boundary[3] = max(Boundary[3], BoundaryU[3]);
			}
		}						
	}

	dXmin = Boundary[0]; dXmax = Boundary[2];
	dYmin = Boundary[1]; dYmax = Boundary[3];
}

//////////////////////////////////////////////////////////////////////
void CDrawCtrl::Copy(CDrawCtrl* pDrawCtrl)
{
	RemoveDrawData(TRUE, TRUE);

	m_hDC            = pDrawCtrl->m_hDC;
	m_pMessageStatic = pDrawCtrl->m_pMessageStatic;
	m_PaperFormat    = pDrawCtrl->m_PaperFormat;

	m_SelectDrawType    = pDrawCtrl->m_SelectDrawType;
	m_HideType          = pDrawCtrl->m_HideType;
	m_SelectDrawFormat  = pDrawCtrl->m_SelectDrawFormat;    
	m_SelectTextFormat  = pDrawCtrl->m_SelectTextFormat;    
	m_SelectDimFormat   = pDrawCtrl->m_SelectDimFormat;      
	m_SelectChartFormat = pDrawCtrl->m_SelectChartFormat; 
	m_nSelectDis        = pDrawCtrl->m_nSelectDis;

	m_FinalEntityNum    = pDrawCtrl->m_FinalEntityNum;

	UINT Index;
	POSITION Pos;
	T_DRAW_FORMAT      DrawFormat;
	T_TEXT_FORMAT      TextFormat;
	T_DIM_FORMAT       DimFormat;
	T_CHART_FORMAT     ChartFormat;
	
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

	CDrawPoint        *pNew_Point = NULL;
	CDrawLine         *pNew_Line = NULL;
	CDrawArrowLine    *pNew_ArrowLine = NULL;
	CDrawCircle       *pNew_Circle = NULL;
	CDrawDonut        *pNew_Donut = NULL;
	CDrawEllipse      *pNew_Ellipse = NULL;
	CDrawRect         *pNew_Rect = NULL;
	CDrawArc          *pNew_Arc = NULL;
	CDrawPolyline     *pNew_Polyline = NULL;
	CDrawPolygon      *pNew_Polygon = NULL;
	CDrawArrowPolyline*pNew_ArrowPolyline = NULL;
	CDrawText         *pNew_Text = NULL;
	CDrawDimension    *pNew_Dimension = NULL;
	CDrawDimAngle     *pNew_DimAngle = NULL;
	CDrawDimLeaderline*pNew_DimLeaderline = NULL;
	CDrawChart        *pNew_Chart = NULL;
	
	// Format
	Index = 0;	
	Pos = pDrawCtrl->m_arDrawFormat.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arDrawFormat.GetNextAssoc(Pos, Index, DrawFormat);
		m_arDrawFormat.SetAt(Index, DrawFormat);		
	}
	Index = 0;		
	Pos = pDrawCtrl->m_arTextFormat.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arTextFormat.GetNextAssoc(Pos, Index, TextFormat);
		m_arTextFormat.SetAt(Index, TextFormat);		
	}
	Index = 0;	
	Pos = pDrawCtrl->m_arDimFormat.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arDimFormat.GetNextAssoc(Pos, Index, DimFormat);
		m_arDimFormat.SetAt(Index, DimFormat);		
	}
	Index = 0;	
	Pos = pDrawCtrl->m_arChartFormat.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arChartFormat.GetNextAssoc(Pos, Index, ChartFormat);
		m_arChartFormat.SetAt(Index, ChartFormat);		
	}

	// DrawUnit
	Index=0;	
	Pos = pDrawCtrl->m_arPoint.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arPoint.GetNextAssoc(Pos, Index, pDC_Point);
		pNew_Point = new CDrawPoint; 
		(*pNew_Point) = (*pDC_Point);
		m_arPoint.SetAt(Index, pNew_Point);		
	}	
	Index=0;	
	Pos = pDrawCtrl->m_arLine.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arLine.GetNextAssoc(Pos, Index, pDC_Line);
		pNew_Line = new CDrawLine; 
		(*pNew_Line) = (*pDC_Line);
		m_arLine.SetAt(Index, pNew_Line);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arArrowLine.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arArrowLine.GetNextAssoc(Pos, Index, pDC_ArrowLine);
		pNew_ArrowLine = new CDrawArrowLine; 
		(*pNew_ArrowLine) = (*pDC_ArrowLine);
		m_arArrowLine.SetAt(Index, pNew_ArrowLine);		
	}
	Index=0;		
	Pos = pDrawCtrl->m_arCircle.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arCircle.GetNextAssoc(Pos, Index, pDC_Circle);
		pNew_Circle = new CDrawCircle; 
		(*pNew_Circle) = (*pDC_Circle);
		m_arCircle.SetAt(Index, pNew_Circle);		
	}
	Index=0;		
	Pos = pDrawCtrl->m_arDonut.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arDonut.GetNextAssoc(Pos, Index, pDC_Donut);
		pNew_Donut = new CDrawDonut; 
		(*pNew_Donut) = (*pDC_Donut);
		m_arDonut.SetAt(Index, pNew_Donut);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arEllipse.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arEllipse.GetNextAssoc(Pos, Index, pDC_Ellipse);
		pNew_Ellipse = new CDrawEllipse; 
		(*pNew_Ellipse) = (*pDC_Ellipse);
		m_arEllipse.SetAt(Index, pNew_Ellipse);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arRect.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arRect.GetNextAssoc(Pos, Index, pDC_Rect);
		pNew_Rect = new CDrawRect; 
		(*pNew_Rect) = (*pDC_Rect);
		m_arRect.SetAt(Index, pNew_Rect);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arArc.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arArc.GetNextAssoc(Pos, Index, pDC_Arc);
		pNew_Arc = new CDrawArc;
		(*pNew_Arc) = (*pDC_Arc);
		m_arArc.SetAt(Index, pNew_Arc);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arPolyline.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arPolyline.GetNextAssoc(Pos, Index, pDC_Polyline);
		pNew_Polyline = new CDrawPolyline;
		(*pNew_Polyline) = (*pDC_Polyline);
		m_arPolyline.SetAt(Index, pNew_Polyline);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arPolygon.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arPolygon.GetNextAssoc(Pos, Index, pDC_Polygon);
		pNew_Polygon = new CDrawPolygon; 
		(*pNew_Polygon) = (*pDC_Polygon);
		m_arPolygon.SetAt(Index, pNew_Polygon);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arArrowPolyline.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arArrowPolyline.GetNextAssoc(Pos, Index, pDC_ArrowPolyline);
		pNew_ArrowPolyline = new CDrawArrowPolyline; 
		(*pNew_ArrowPolyline) = (*pDC_ArrowPolyline);
		m_arArrowPolyline.SetAt(Index, pNew_ArrowPolyline);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arText.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arText.GetNextAssoc(Pos, Index, pDC_Text);
		pNew_Text = new CDrawText;
		(*pNew_Text) = (*pDC_Text);
		m_arText.SetAt(Index, pNew_Text);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arDimension.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arDimension.GetNextAssoc(Pos, Index, pDC_Dimension);
		pNew_Dimension = new CDrawDimension;
		(*pNew_Dimension) = (*pDC_Dimension);
		m_arDimension.SetAt(Index, pNew_Dimension);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arDimAngle.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arDimAngle.GetNextAssoc(Pos, Index, pDC_DimAngle);
		pNew_DimAngle = new CDrawDimAngle;
		(*pNew_DimAngle) = (*pDC_DimAngle);
		m_arDimAngle.SetAt(Index, pNew_DimAngle);		
	}
	Index=0;	
	Pos = pDrawCtrl->m_arDimLeaderline.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arDimLeaderline.GetNextAssoc(Pos, Index, pDC_DimLeaderline);
		pNew_DimLeaderline = new CDrawDimLeaderline;
		(*pNew_DimLeaderline) = (*pDC_DimLeaderline);
		m_arDimLeaderline.SetAt(Index, pNew_DimLeaderline);		
	}
	Index = 0;	
	Pos = pDrawCtrl->m_arChart.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arChart.GetNextAssoc(Pos, Index, pDC_Chart);
		pNew_Chart = new CDrawChart;
		(*pNew_Chart) = (*pDC_Chart);
		m_arChart.SetAt(Index, pNew_Chart);		
	}

	// Selected Key
	UINT SelectedKey;
	Index = 0;	
	Pos = pDrawCtrl->m_arSelectedDrawUnitKey.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arSelectedDrawUnitKey.GetNextAssoc(Pos, Index, SelectedKey);
		m_arSelectedDrawUnitKey.SetAt(Index, SelectedKey);		
	}
	// Hide Key
	UINT HideKey;
	Index = 0;		
	Pos = pDrawCtrl->m_arHideDrawUnitKey.GetStartPosition();
	while(Pos)
	{		
		pDrawCtrl->m_arHideDrawUnitKey.GetNextAssoc(Pos, Index, HideKey);
		m_arHideDrawUnitKey.SetAt(Index, HideKey);		
	}
}

void CDrawCtrl::MessageOut(TCHAR* strMsg)
{
	if(m_pMessageStatic != NULL)
	{
		m_pMessageStatic->SetWindowText(strMsg);		
	}
}

void CDrawCtrl::RemoveDrawData(BOOL bDrawFormat, BOOL bDrawUnit, BOOL bPaperFormat)
{
	m_FinalEntityNum = 0;

	if(bPaperFormat) m_PaperFormat.Init();

	if(bDrawFormat)
	{	
		m_arDrawFormat.RemoveAll();
		m_arTextFormat.RemoveAll();
		m_arDimFormat.RemoveAll();
		m_arChartFormat.RemoveAll();
		m_SelectDrawFormat.Init();
		m_SelectTextFormat.Init();
		m_SelectDimFormat.Init();
		m_SelectChartFormat.Init();
	}

	if(bDrawUnit)
	{
		m_arSelectedDrawUnitKey.RemoveAll();
		m_arHideDrawUnitKey.RemoveAll();

		UINT Index;
		POSITION Pos;
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

		// Point
		Index = 0;	
		Pos = m_arPoint.GetStartPosition();
		while(Pos)
		{		
			m_arPoint.GetNextAssoc(Pos, Index, pDC_Point);
			if(pDC_Point != NULL) delete pDC_Point;
		}
		m_arPoint.RemoveAll();
		// Line
		Index = 0;		
		Pos = m_arLine.GetStartPosition();
		while(Pos)
		{		
			m_arLine.GetNextAssoc(Pos, Index, pDC_Line);
			if(pDC_Line != NULL) delete pDC_Line;
		}
		m_arLine.RemoveAll();
		// ArrowLine
		Index = 0;		
		Pos = m_arArrowLine.GetStartPosition();
		while(Pos)
		{		
			m_arArrowLine.GetNextAssoc(Pos, Index, pDC_ArrowLine);
			if(pDC_ArrowLine != NULL) delete pDC_ArrowLine;
		}
		m_arArrowLine.RemoveAll();
		// Circle
		Index = 0;	
		Pos = m_arCircle.GetStartPosition();
		while(Pos)
		{		
			m_arCircle.GetNextAssoc(Pos, Index, pDC_Circle);
			if(pDC_Circle != NULL) delete pDC_Circle;
		}
		m_arCircle.RemoveAll();
		// Donut
		Index = 0;	
		Pos = m_arDonut.GetStartPosition();
		while(Pos)
		{		
			m_arDonut.GetNextAssoc(Pos, Index, pDC_Donut);
			if(pDC_Donut != NULL) delete pDC_Donut;
		}
		m_arDonut.RemoveAll();
		// Ellipse
		Index = 0;	
		Pos = m_arEllipse.GetStartPosition();
		while(Pos)
		{		
			m_arEllipse.GetNextAssoc(Pos, Index, pDC_Ellipse);
			if(pDC_Ellipse != NULL) delete pDC_Ellipse;
		}
		m_arEllipse.RemoveAll();
		// Rect
		Index = 0;	
		Pos = m_arRect.GetStartPosition();
		while(Pos)
		{		
			m_arRect.GetNextAssoc(Pos, Index, pDC_Rect);
			if(pDC_Rect != NULL) delete pDC_Rect;
		}
		m_arRect.RemoveAll();
		// Arc
		Index = 0;	
		Pos = m_arArc.GetStartPosition();
		while(Pos)
		{		
			m_arArc.GetNextAssoc(Pos, Index, pDC_Arc);
			if(pDC_Arc != NULL) delete pDC_Arc;
		}
		m_arArc.RemoveAll();
		// Polyline
		Index = 0;	
		Pos = m_arPolyline.GetStartPosition();
		while(Pos)
		{		
			m_arPolyline.GetNextAssoc(Pos, Index, pDC_Polyline);
			if(pDC_Polyline != NULL) delete pDC_Polyline;
		}
		m_arPolyline.RemoveAll();
		// Polygon
		Index=0;	
		Pos = m_arPolygon.GetStartPosition();
		while(Pos)
		{		
			m_arPolygon.GetNextAssoc(Pos, Index, pDC_Polygon);
			if(pDC_Polygon != NULL) delete pDC_Polygon;
		}
		m_arPolygon.RemoveAll();
		// ArrowPolyline
		Index = 0;	
		Pos = m_arArrowPolyline.GetStartPosition();
		while(Pos)
		{		
			m_arArrowPolyline.GetNextAssoc(Pos, Index, pDC_ArrowPolyline);
			if(pDC_ArrowPolyline != NULL) delete pDC_ArrowPolyline;
		}
		m_arArrowPolyline.RemoveAll();
		// Text
		Index = 0;	
		Pos = m_arText.GetStartPosition();
		while(Pos)
		{		
			m_arText.GetNextAssoc(Pos, Index, pDC_Text);
			if(pDC_Text != NULL) delete pDC_Text;
		}
		m_arText.RemoveAll();
		// Dimension
		Index = 0;	
		Pos = m_arDimension.GetStartPosition();
		while(Pos)
		{		
			m_arDimension.GetNextAssoc(Pos, Index, pDC_Dimension);
			if(pDC_Dimension != NULL) delete pDC_Dimension;
		}
		m_arDimension.RemoveAll();
		// DimAngle
		Index = 0;	
		Pos = m_arDimAngle.GetStartPosition();
		while(Pos)
		{		
			m_arDimAngle.GetNextAssoc(Pos, Index, pDC_DimAngle);
			if(pDC_DimAngle != NULL) delete pDC_DimAngle;
		}
		m_arDimAngle.RemoveAll();
		// DimLeaderline
		Index = 0;	
		Pos = m_arDimLeaderline.GetStartPosition();
		while(Pos)
		{		
			m_arDimLeaderline.GetNextAssoc(Pos, Index, pDC_DimLeaderline);
			if(pDC_DimLeaderline != NULL) delete pDC_DimLeaderline;
		}
		m_arDimLeaderline.RemoveAll();
		// Chart
		Index = 0;	
		Pos = m_arChart.GetStartPosition();
		while(Pos)
		{		
			m_arChart.GetNextAssoc(Pos, Index, pDC_Chart);
			if(pDC_Chart != NULL) delete pDC_Chart;
		}
		m_arChart.RemoveAll();
	}
}

void CDrawCtrl::ConvertDrawData(BOOL bCDC)
{	
// 	IProductInfo* pProdInfo = IProductInfo::Instance();
// 	int nOutLanguage = pProdInfo->GetLanguageID();
	int nOutLanguage = 0x01;//pProdInfo->GetLanguageID();

	// Format
	T_DRAW_FORMAT  DrawFormat; 
	T_TEXT_FORMAT  TextFormat(nOutLanguage);
	T_DIM_FORMAT   DimFormat(nOutLanguage);
	T_CHART_FORMAT ChartFormat(nOutLanguage);

	int nDrawFormatType;
	int iXmin, iYmin, iXmax, iYmax;
	CDrawBase* pDrawClass;
	
	for(UINT iIndex = 1; iIndex <= m_FinalEntityNum; iIndex++)
	{
		pDrawClass = GetDrawClass(iIndex);
		
		if(pDrawClass != NULL) 
		{
			nDrawFormatType = pDrawClass->GetFormatType();
			if(nDrawFormatType == EN_DRFORMAT_TYPE_DRAW)
			{	
				if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
				pDrawClass->Convert(bCDC, &m_DrawFunc, &m_PaperFormat, &DrawFormat);		
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_TEXT)
			{	
				if(!m_arTextFormat.Lookup(pDrawClass->m_DrawFormatKey, TextFormat)) TextFormat.Init();	
				pDrawClass->Convert(bCDC, &m_DrawFunc, &m_PaperFormat, &TextFormat);		
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_DIM)
			{
				if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat)) DimFormat.Init();	
				pDrawClass->Convert(bCDC, &m_DrawFunc, &m_PaperFormat, &DimFormat);		
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_CHART)
			{
				if(!m_arChartFormat.Lookup(pDrawClass->m_DrawFormatKey, ChartFormat)) ChartFormat.Init();	
				pDrawClass->Convert(bCDC, &m_DrawFunc, &m_PaperFormat, &ChartFormat);		
			}

			if(bCDC)pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);// CDC용 Data로 변환시에는 해당 정보의 m_Boundary값을 재설정 하여줌
		}		
	}
}

void CDrawCtrl::Draw()
{
	RECT DrawRect;
	memset(&DrawRect, 0, sizeof(RECT));
	Draw(DrawRect);
}

void CDrawCtrl::Draw(RECT DrawRect)
{
	// Format
	T_DRAW_FORMAT  DrawFormat;
	T_TEXT_FORMAT  TextFormat;
	T_DIM_FORMAT   DimFormat; 
	T_CHART_FORMAT ChartFormat;
	
	CDrawBase* pDrawClass = NULL;

	BOOL  bUnBoundary = (DrawRect.left == 0 && DrawRect.top == 0 && DrawRect.right == 0 && DrawRect.bottom == 0);
	int   Boundary[4] = { (int)DrawRect.left, (int)DrawRect.top, (int)DrawRect.right, (int)DrawRect.bottom };

	int nDrawFormatType;
	BOOL bSelected;
	UINT dummy;

	for(UINT iIndex = 1; iIndex <= m_FinalEntityNum; iIndex++)
	{
		pDrawClass = GetDrawClass(iIndex);

		if(pDrawClass != NULL)
		{
			if(!bUnBoundary)
			{
				if(pDrawClass->GetBoundaryInclude(Boundary, FALSE) == -1) continue;
			}

			if(m_SelectDrawType != 0) bSelected = m_arSelectedDrawUnitKey.Lookup(iIndex, dummy);
			else                      bSelected = FALSE;

			if(m_HideType == 1)
			{ 
				if(m_arHideDrawUnitKey.Lookup(iIndex, dummy)) continue;
			}

			nDrawFormatType = pDrawClass->GetFormatType();			
		
			if(nDrawFormatType == EN_DRFORMAT_TYPE_DRAW)
			{				
				if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
				
				if(bSelected)
				{
					if(m_SelectDrawType == 2) DrawFormat=m_SelectDrawFormat;
					else if(m_SelectDrawType > 2) 
					{
						DrawFormat.lineColor = m_SelectDrawFormat.lineColor;
						DrawFormat.fillColor = m_SelectDrawFormat.fillColor;
						if(m_SelectDrawType > 3)
						{
							if(pDrawClass->GetClassType() != EN_DRCLASS_TYPE_POINT) 
							{ DrawFormat.Width = m_SelectDrawFormat.Width; }
							else                                            
							{
								T_DRAW_FORMAT DrawFormat_Out = DrawFormat;								
								DrawFormat_Out.Width = max(DrawFormat.Width+2, DrawFormat.Width+m_SelectDrawFormat.Width*2);
								pDrawClass->Draw(m_hDC, &m_DrawFunc, &DrawFormat_Out);																
								DrawFormat.lineColor = DrawFormat.fillColor;
							}
						}
					}					
				}
				pDrawClass->Draw(m_hDC, &m_DrawFunc, &DrawFormat);
				if(bSelected && m_SelectDrawType == 1) pDrawClass->DrawOutLine(m_hDC, &m_DrawFunc, &m_SelectDrawFormat, 2);
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_TEXT)
			{
				if(!m_arTextFormat.Lookup(pDrawClass->m_DrawFormatKey, TextFormat)) TextFormat.Init();	
				
				if(bSelected)
				{
					if(m_SelectDrawType == 2) 
					{
						TextFormat=m_SelectTextFormat;
					}
					else if(m_SelectDrawType > 2) 
					{
						TextFormat.fontColor = m_SelectTextFormat.fontColor;
						TextFormat.fillColor = m_SelectTextFormat.fillColor;
					}					
				}
				pDrawClass->Draw(m_hDC, &m_DrawFunc, &TextFormat);
				if(bSelected && m_SelectDrawType == 1) pDrawClass->DrawOutLine(m_hDC, &m_DrawFunc, &m_SelectDrawFormat, 2);
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_DIM)
			{
				if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat)) DimFormat.Init();	
				
				if(bSelected)
				{
					if(m_SelectDrawType == 2) 
					{
						DimFormat=m_SelectDimFormat;
					}
					else if(m_SelectDrawType > 2) 
					{
						DimFormat.fontColor = m_SelectDimFormat.fontColor;
						DimFormat.lineColor = m_SelectDimFormat.lineColor;
						if(m_SelectDrawType > 3) DimFormat.Width = m_SelectDimFormat.Width;
					}					
				}
				pDrawClass->Draw(m_hDC, &m_DrawFunc, &DimFormat);
				if(bSelected && m_SelectDrawType == 1) pDrawClass->DrawOutLine(m_hDC, &m_DrawFunc, &m_SelectDrawFormat, 2);
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_CHART)
			{
				if(!m_arChartFormat.Lookup(pDrawClass->m_DrawFormatKey, ChartFormat)) ChartFormat.Init();	
				
				if(bSelected)
				{
					// 0:표현안함, 1:외곽사각형표시, 2:Format변경, 3:색만변경, 4:색+선두깨변경
					if(m_SelectDrawType == 2)
					{
						ChartFormat=m_SelectChartFormat;
					}
					else if(m_SelectDrawType > 2) 
					{
						ChartFormat.frame.lineColor      = m_SelectChartFormat.frame.lineColor;
						ChartFormat.frame.fillColor      = m_SelectChartFormat.frame.fillColor;
						ChartFormat.frame.fontColor      = m_SelectChartFormat.frame.fontColor;
						ChartFormat.remark.lineColor     = m_SelectChartFormat.remark.lineColor;
						ChartFormat.remark.fillColor     = m_SelectChartFormat.remark.fillColor;
						ChartFormat.remark.fontColor     = m_SelectChartFormat.remark.fontColor;
						ChartFormat.xAxis.lineColor      = m_SelectChartFormat.xAxis.lineColor;
						ChartFormat.xAxis.majorGridColor = m_SelectChartFormat.xAxis.majorGridColor;
						ChartFormat.xAxis.minorGridColor = m_SelectChartFormat.xAxis.minorGridColor;
						ChartFormat.xAxis.fontColor      = m_SelectChartFormat.xAxis.fontColor;
						ChartFormat.xAxis.fontColor_Title= m_SelectChartFormat.xAxis.fontColor_Title;
						ChartFormat.yAxis.lineColor      = m_SelectChartFormat.yAxis.lineColor;
						ChartFormat.yAxis.majorGridColor = m_SelectChartFormat.yAxis.majorGridColor;
						ChartFormat.yAxis.minorGridColor = m_SelectChartFormat.yAxis.minorGridColor;
						ChartFormat.yAxis.fontColor      = m_SelectChartFormat.yAxis.fontColor;
						ChartFormat.yAxis.fontColor_Title= m_SelectChartFormat.yAxis.fontColor_Title;
						
						if(m_SelectDrawType > 3) 
						{
							ChartFormat.frame.Width           = m_SelectChartFormat.frame.Width;
							ChartFormat.remark.Width          = m_SelectChartFormat.remark.Width;
							ChartFormat.xAxis.Width           = m_SelectChartFormat.xAxis.Width;
							ChartFormat.xAxis.nMajorGridWidth = m_SelectChartFormat.xAxis.nMajorGridWidth;
							ChartFormat.xAxis.nMinorGridWidth = m_SelectChartFormat.xAxis.nMinorGridWidth;							
							ChartFormat.yAxis.Width           = m_SelectChartFormat.yAxis.Width;
							ChartFormat.yAxis.nMajorGridWidth = m_SelectChartFormat.yAxis.nMajorGridWidth;
							ChartFormat.yAxis.nMinorGridWidth = m_SelectChartFormat.yAxis.nMinorGridWidth;
						}
					}					
				}
				pDrawClass->Draw(m_hDC, &m_DrawFunc, &ChartFormat);
				if(bSelected && m_SelectDrawType==1) pDrawClass->DrawOutLine(m_hDC, &m_DrawFunc, &m_SelectDrawFormat, 2);
			}
		}		
	}
}

void CDrawCtrl::AutoPaperFormat(RECT Canvas, double dTopMargin, double dBottomMargin, double dLeftMargin, double dRightMargin)
{
	double dXmax = 0.0, dYmax = 0.0, dXmin = 0.0, dYmin = 0.0;
	GetMinMax(dXmin, dXmax, dYmin, dYmax);

	SetPaperFormat(Canvas, dTopMargin, dBottomMargin, dLeftMargin, dRightMargin, dXmax, dXmin, dYmax, dYmin);	
}

void CDrawCtrl::AdjustMarginToFitImage(RECT Canvas, double dMargin, double dImageB, double dImageH, BOOL bDimT, BOOL bDimB, BOOL bDimL, BOOL bDimR)
{
	double dZero = 1.0e-7;
	ASSERT(dMargin < 0.5 + dZero);
	if (dMargin < dZero) dMargin = 0.01;

	double dMDim = dMargin/2.0;
	if (dMDim < 0.05) dMDim = 0.05;

	double dMT = bDimT ? dMargin + dMDim : dMargin;
	double dMB = bDimB ? dMargin + dMDim : dMargin;
	double dML = bDimL ? dMargin + dMDim : dMargin;
	double dMR = bDimR ? dMargin + dMDim : dMargin;

	double dCanvasB = (double)(Canvas.right - Canvas.left);
	double dCanvasH = (double)(Canvas.top - Canvas.bottom);

	if (dCanvasB > dCanvasH)
	{
		double dCanvasBHR = dCanvasB / dCanvasH;
		
		if (dImageB<dImageH)
		{
			if (bDimT) dMT *= dCanvasBHR;
			if (bDimB) dMB *= dCanvasBHR;
		}
		else
		{
			if (dCanvasBHR > 1.5)
			{
				if (bDimT) dMT += dMDim;
				if (bDimB) dMB += dMDim;
			}
			else if (dCanvasBHR > 1.0)
			{
				if (bDimT) dMT *= dCanvasBHR;
				if (bDimB) dMB *= dCanvasBHR;
			}
		}
	}
	else
	{
		double dCanvasHBR = dCanvasH / dCanvasB;
		if (dImageB>dImageH || dCanvasHBR > 1.5)
		{
			if (bDimL) dML *= dCanvasHBR;
			if (bDimR) dMR *= dCanvasHBR;
		}
		else
		{
			if (dCanvasHBR > 1.5)
			{
				if (bDimL) dML += dMDim;
				if (bDimR) dMR += dMDim;
			}
			else if (dCanvasHBR > 1.0)
			{
				if (bDimL) dML *= dCanvasHBR;
				if (bDimR) dMR *= dCanvasHBR;
			}
		}
	}

	AutoPaperFormat(Canvas, dMT, dMB, dML, dMR);
}

void CDrawCtrl::SetPaperFormat(RECT Canvas, double dTopMargin, double dBottomMargin, double dLeftMargin, double dRightMargin, 
											  double dWorkXmax, double dWorkXmin, double dWorkYmax, double dWorkYmin)
{
	LONG width = Canvas.right - Canvas.left;
	LONG height = Canvas.bottom - Canvas.top;

	m_PaperFormat.Init();
	double dWorkCanvasWidth  = width * (1.0 - dLeftMargin - dRightMargin);
	double dWorkCanvasHeight = height * (1.0 - dTopMargin - dBottomMargin);
	double dWorkWidth  = dWorkXmax - dWorkXmin;
	double dWorkHeight = dWorkYmax - dWorkYmin;

	if(dWorkCanvasWidth <= 0 || dWorkCanvasHeight <= 0) return;
	
	if(dWorkWidth == 0 && dWorkHeight == 0)
	{
		m_PaperFormat.OrgPoint.x = (long)(width  / 2.0 - dWorkXmax);
		m_PaperFormat.OrgPoint.y = (long)(height / 2.0 - dWorkYmax);
		m_PaperFormat.ScaleX = m_PaperFormat.ScaleY = 1.0;
		return;
	}

	double Fx = 0.;
	double Fy = 0.;

	if(dWorkWidth != 0 && dWorkHeight != 0)
	{
		Fx =  dWorkCanvasWidth / dWorkWidth;
		Fy =  dWorkCanvasHeight / dWorkHeight;
	}
	else if(dWorkWidth != 0 && dWorkHeight == 0)
	{
		Fx =  dWorkCanvasWidth / dWorkWidth;
		Fy =  Fx;
	}
	else if(dWorkWidth == 0 && dWorkHeight != 0)
	{
		Fy =  dWorkCanvasHeight / dWorkHeight;
		Fx =  Fy;
	}

	double dScaleF = min(Fx, Fy);
	if(dScaleF==0.0) return;

	m_PaperFormat.ScaleX = m_PaperFormat.ScaleY = 1.0 / dScaleF;

	if(!m_DrawFunc.IsEMF())
	{
		m_PaperFormat.OrgPoint.x = (long)Canvas.left + (long)(width * dLeftMargin + dWorkCanvasWidth / 2.0 - (dWorkXmax + dWorkXmin) / 2.0 * dScaleF);
		m_PaperFormat.OrgPoint.y = (long)Canvas.top  + (long)(height * dTopMargin + dWorkCanvasHeight / 2.0 + (dWorkYmax + dWorkYmin) / 2.0 * dScaleF);
	}
	else
	{
		m_PaperFormat.OrgPoint.x = (long)Canvas.left + (long)(width * dLeftMargin + dWorkCanvasWidth / 2.0 - (dWorkXmax + dWorkXmin) / 2.0 * dScaleF);
		m_PaperFormat.OrgPoint.y = (long)Canvas.top  + (long)(height * dTopMargin + dWorkCanvasHeight / 2.0 + (dWorkYmax + dWorkYmin) / 2.0 * dScaleF);
	}  

	m_DrawFunc.ConvertPaperFormatMirror(m_PaperFormat, Canvas, EN_MIRRORVIEW_TYPE_NONE, m_nMirrorMode);
}

CDrawBase* CDrawCtrl::GetDrawClass(UINT key)
{
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

	if(m_arPoint.Lookup(key, pDC_Point))					{ return (CDrawBase*)(pDC_Point);}
	if(m_arLine.Lookup(key, pDC_Line))						{ return (CDrawBase*)(pDC_Line);}
	if(m_arArrowLine.Lookup(key, pDC_ArrowLine))			{ return (CDrawBase*)(pDC_ArrowLine);}
	if(m_arCircle.Lookup(key, pDC_Circle))					{ return (CDrawBase*)(pDC_Circle);}
	if(m_arDonut.Lookup(key, pDC_Donut))					{ return (CDrawBase*)(pDC_Donut);}
	if(m_arEllipse.Lookup(key, pDC_Ellipse))				{ return (CDrawBase*)(pDC_Ellipse);}
	if(m_arRect.Lookup(key, pDC_Rect))						{ return (CDrawBase*)(pDC_Rect);}
	if(m_arArc.Lookup(key, pDC_Arc))						{ return (CDrawBase*)(pDC_Arc);}
	if(m_arPolyline.Lookup(key, pDC_Polyline))			{ return (CDrawBase*)(pDC_Polyline);}
	if(m_arPolygon.Lookup(key, pDC_Polygon))				{ return (CDrawBase*)(pDC_Polygon);}
	if(m_arArrowPolyline.Lookup(key, pDC_ArrowPolyline)) { return (CDrawBase*)(pDC_ArrowPolyline);}
	if(m_arText.Lookup(key, pDC_Text))						{ return (CDrawBase*)(pDC_Text);}
	if(m_arDimension.Lookup(key, pDC_Dimension))			{ return (CDrawBase*)(pDC_Dimension);}
	if(m_arDimAngle.Lookup(key, pDC_DimAngle))			{ return (CDrawBase*)(pDC_DimAngle);}
	if(m_arDimLeaderline.Lookup(key, pDC_DimLeaderline)) { return (CDrawBase*)(pDC_DimLeaderline);}
	if(m_arChart.Lookup(key, pDC_Chart))					{ return (CDrawBase*)(pDC_Chart);}

	return NULL;
}

BOOL CDrawCtrl::FindLinkKeyFromDrawKey(UINT DrawKey, UINT& LinkKey)
{
	CDrawBase* pDrawClass = GetDrawClass(DrawKey);
	if(pDrawClass == NULL) return FALSE;
	LinkKey = pDrawClass->m_LinkKey;
	
	return TRUE;
}

BOOL CDrawCtrl::FindDrawKeyFromLinkKey(UINT  LinkKey, nrUINT& arDrawKey)
{
	arDrawKey.RemoveAll();
	CDrawBase* pDrawClass = NULL;
	
	for(UINT iIndex = 1; iIndex <= m_FinalEntityNum; iIndex++)
	{
		pDrawClass = GetDrawClass(iIndex);
		
		if(pDrawClass == NULL)
		{
			if(pDrawClass->m_LinkKey == LinkKey) arDrawKey.Add(iIndex);				
		}
	}
	return (arDrawKey.GetSize() > 0);
}


UINT CDrawCtrl::GetSelectDis()
{
	return m_nSelectDis;
}

void CDrawCtrl::SetSelectDis(UINT nSelectDis)
{
	m_nSelectDis = nSelectDis;
}

UINT CDrawCtrl::GetSnapDis()
{
	return CDrawBase::GetSnapDis();
}

void CDrawCtrl::SetSnapDis(UINT nSnapDis)
{
	CDrawBase::SetSnapDis(nSnapDis);
}

BOOL CDrawCtrl::GetOpjectSnapPoint(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR)
{
	CDrawBase* pDrawClass = NULL;

	for(UINT iIndex = m_FinalEntityNum; iIndex > 0; iIndex--)
	{
		pDrawClass = GetDrawClass(iIndex);
		if(pDrawClass != NULL)
		{
			if(pDrawClass->IsSnapCheck(currPoint, snapPoint, snapPointR)) return TRUE;
		}
	}
	return FALSE;
}

BOOL CDrawCtrl::GetOpjectHighlightLine(CPoint currPoint, nrLine &aHighlightLine)
{	
	UINT SelectKey;
	if(GetSelectDrawUnitKey(currPoint, SelectKey))
	{
		if(!IsSelected(SelectKey))
		{			
// 			nrUINT arConnectedKey;
// 			double dOffsetMergeLine = 1.0;
// 			GetConnectedLineKey(SelectKey,arConnectedKey, dOffsetMergeLine);
// 
// 			CDrawLine* pConnectedDrawLineClass = NULL;
// 			for(int i = 0 ; i < arConnectedKey.GetSize() ; i++)
// 			{
// 				if(!m_arLine.Lookup(arConnectedKey[i], pConnectedDrawLineClass))
// 				{
// 					ASSERT(0);
// 					continue;
// 				}
// 
// 				if(pConnectedDrawLineClass->m_Data.List.GetSize() != 1)
// 					ASSERT(0);
// 
// 				aHighlightLine.Add(pConnectedDrawLineClass->m_Data.List[0]);
// 			}	

			CDrawLine* pDrawLineClass = NULL;
			
			if(m_arLine.Lookup(SelectKey, pDrawLineClass))
			{
				int i;
				int nSize = pDrawLineClass->m_Data.List.GetSize();
				aHighlightLine.SetSize(nSize);
				for(i = 0 ; i < nSize ; i++)
				{
					aHighlightLine[i] = pDrawLineClass->m_Data.List[i];
				}
			}
			else
			{
				aHighlightLine.RemoveAll();
			}				
		}
	}

	return TRUE;
}


BOOL CDrawCtrl::ChkMaxDrawUnit()
{
	if(m_FinalEntityNum >= INT_MAX)
	{
		TCHAR strMsg[100];
		wsprintf(strMsg, _T("The allowed maximum number of additional Unit already reached !"));
		MessageOut(strMsg);
		return FALSE;
	}
	return TRUE;
}


////////////////////////////////
BOOL CDrawCtrl::ModifyDrawFormat(UINT key, int nFormatKey)
{
	CDrawBase* pDrawClass = GetDrawClass(key);
	if(pDrawClass == NULL) return FALSE;

	pDrawClass->m_DrawFormatKey = nFormatKey;
	return TRUE;
}

BOOL CDrawCtrl::ModifyLinkKey(UINT key, int nLinkKey)
{
	CDrawBase* pDrawClass = GetDrawClass(key);
	if(pDrawClass == NULL) return FALSE;

	pDrawClass->m_LinkKey = nLinkKey;
	return TRUE;
}

///////////////////////////////////////////////////////////
void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_POINT_GROUP& rData)
{	
	m_DrawFunc.DrawPoint(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_LINE_GROUP& rData)
{	
	m_DrawFunc.DrawLine(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_ARROWLINE_GROUP& rData)
{	
	m_DrawFunc.DrawArrow(m_hDC, DrawFormat, rData.List); 
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_CIRCLE_GROUP& rData)
{
	m_DrawFunc.DrawCircle(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_DONUT_GROUP& rData)
{	
	m_DrawFunc.DrawDonut(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_ELLIPSE_GROUP& rData)
{
	m_DrawFunc.DrawEllipse(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_RECT_GROUP& rData)
{
	m_DrawFunc.DrawRect(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_ARC_GROUP& rData)
{
	m_DrawFunc.DrawArc(m_hDC, DrawFormat, rData.List); 
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_POLYLINE_GROUP& rData)
{
	m_DrawFunc.DrawPolyline(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_POLYGON_GROUP& rData)
{
	m_DrawFunc.DrawPolygon(m_hDC, DrawFormat, rData.List); 
}

void CDrawCtrl::DrawUnit(T_DRAW_FORMAT DrawFormat, T_ARROWPOLYLINE_GROUP& rData)
{	
	m_DrawFunc.DrawArrowPolyline(m_hDC, DrawFormat, rData.List); 
}

void CDrawCtrl::DrawUnit(T_TEXT_FORMAT DrawFormat, T_TEXT_GROUP& rData)
{	
	m_DrawFunc.DrawText(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DIM_FORMAT DrawFormat, T_DIMENSION_GROUP& rData)
{
	m_DrawFunc.DrawDimension(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DIM_FORMAT DrawFormat, T_DIMANGLE_GROUP& rData)
{
	m_DrawFunc.DrawDimAngle(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(T_DIM_FORMAT DrawFormat, T_DIMLEADERLINE_GROUP& rData)
{
	m_DrawFunc.DrawDimLeaderLine(m_hDC, DrawFormat, rData.List); 
}

void CDrawCtrl::DrawUnit(T_CHART_FORMAT DrawFormat, T_CHART_GROUP& rData)
{
	m_DrawFunc.DrawChart(m_hDC, DrawFormat, rData.List);
}

void CDrawCtrl::DrawUnit(HBITMAP img, RECT rtCanvas)
{
	m_DrawFunc.DrawImage(m_hDC, img, rtCanvas);
}

void CDrawCtrl::SetTextUnit_CurrFormatSizeRatio(double dRatio)
{
	UINT Index = 0;		
	POSITION Pos = m_arText.GetStartPosition();
	CDrawText *pDC_Text = NULL;
	int i;
	while(Pos)
	{		
		m_arText.GetNextAssoc(Pos, Index, pDC_Text);
		
		int no = pDC_Text->m_Data.List.GetSize();
		for(i = 0; i < no; i++) 
		{
			pDC_Text->m_Data.List[i].CurrFormat.Size = (int)(pDC_Text->m_Data.List[i].CurrFormat.Size * dRatio); 
		}

		int no2 = pDC_Text->m_DataR.List.GetSize();
		for(i = 0; i < no2; i++) 
		{
			pDC_Text->m_DataR.List[i].CurrFormat.Size = (int)(pDC_Text->m_DataR.List[i].CurrFormat.Size * dRatio); 
		}		
	}
}

void CDrawCtrl::SetTextUnit_FormatSizeRatio(double dRatio, BOOL bLineWidth, BOOL bTextFont, BOOL bDimFont, BOOL bChartFont)
{
	if(dRatio <= 0.0) return;

	T_DRAW_FORMAT  DrawFormat;
	T_TEXT_FORMAT  TextFormat;
	T_DIM_FORMAT   DimFormat;
	T_CHART_FORMAT ChartFormat;

	UINT Index = 0;		
	POSITION Pos;

	if(bTextFont)
	{
		Pos = m_arTextFormat.GetStartPosition();
		
		while(Pos)
		{		
			m_arTextFormat.GetNextAssoc(Pos, Index, TextFormat);
			TextFormat.Size = int(TextFormat.Size * dRatio);
			m_arTextFormat.SetAt(Index, TextFormat);
		}
		m_SelectTextFormat.Size = int(m_SelectTextFormat.Size * dRatio);
	}

	if(bDimFont)
	{
		Pos = m_arDimFormat.GetStartPosition();
		while(Pos)
		{		
			m_arDimFormat.GetNextAssoc(Pos, Index, DimFormat);
			DimFormat.fontSize = int(DimFormat.fontSize * dRatio);
			if(!DimFormat.bFontScale)
			{
				DimFormat.dRTextSpace = int(DimFormat.dRTextSpace * dRatio);
				DimFormat.dRDimSpace  = int(DimFormat.dRDimSpace  * dRatio); 
				DimFormat.dRGuideIn   = int(DimFormat.dRGuideIn   * dRatio);  
				DimFormat.dRGuideOut  = int(DimFormat.dRGuideOut  * dRatio); 
			}
			m_arDimFormat.SetAt(Index, DimFormat);
		}
		m_SelectDimFormat.fontSize = int(m_SelectDimFormat.fontSize * dRatio);	
		if(!m_SelectDimFormat.bFontScale)
		{
			m_SelectDimFormat.dRTextSpace = int(m_SelectDimFormat.dRTextSpace* dRatio);
			m_SelectDimFormat.dRDimSpace  = int(m_SelectDimFormat.dRDimSpace * dRatio); 
			m_SelectDimFormat.dRGuideIn   = int(m_SelectDimFormat.dRGuideIn  * dRatio);  
			m_SelectDimFormat.dRGuideOut  = int(m_SelectDimFormat.dRGuideOut * dRatio); 
		}
	}

	if(bChartFont)
	{
		Pos = m_arChartFormat.GetStartPosition();
		while(Pos)
		{		
			m_arChartFormat.GetNextAssoc(Pos, Index, ChartFormat);
			ChartFormat.frame.fontSize       = int(ChartFormat.frame.fontSize      * dRatio);
			ChartFormat.remark.fontSize      = int(ChartFormat.remark.fontSize     * dRatio); 
			ChartFormat.xAxis.fontSize       = int(ChartFormat.xAxis.fontSize      * dRatio); 
			ChartFormat.xAxis.fontSize_Title = int(ChartFormat.xAxis.fontSize_Title* dRatio);	
			ChartFormat.yAxis.fontSize       = int(ChartFormat.yAxis.fontSize      * dRatio); 
			ChartFormat.yAxis.fontSize_Title = int(ChartFormat.yAxis.fontSize_Title* dRatio); 
			m_arChartFormat.SetAt(Index, ChartFormat);
		}

		m_SelectChartFormat.frame.fontSize       = int(m_SelectChartFormat.frame.fontSize      * dRatio);
		m_SelectChartFormat.remark.fontSize      = int(m_SelectChartFormat.remark.fontSize     * dRatio);
		m_SelectChartFormat.xAxis.fontSize       = int(m_SelectChartFormat.xAxis.fontSize      * dRatio);
		m_SelectChartFormat.xAxis.fontSize_Title = int(m_SelectChartFormat.xAxis.fontSize_Title* dRatio);	
		m_SelectChartFormat.yAxis.fontSize       = int(m_SelectChartFormat.yAxis.fontSize      * dRatio);
		m_SelectChartFormat.yAxis.fontSize_Title = int(m_SelectChartFormat.yAxis.fontSize_Title* dRatio);
	}

	if(bLineWidth)
	{
		// Draw
		Pos = m_arDrawFormat.GetStartPosition();
		while(Pos)
		{		
			m_arDrawFormat.GetNextAssoc(Pos, Index, DrawFormat);
			DrawFormat.Width         = int(DrawFormat.Width        * dRatio);
			DrawFormat.HatchingWidth = int(DrawFormat.HatchingWidth* dRatio);
			DrawFormat.HatchingSpace = int(DrawFormat.HatchingSpace* dRatio);
			m_arDrawFormat.SetAt(Index, DrawFormat);
		}
		m_SelectDrawFormat.Width         = int(m_SelectDrawFormat.Width        * dRatio);
		m_SelectDrawFormat.HatchingWidth = int(m_SelectDrawFormat.HatchingWidth* dRatio);
		m_SelectDrawFormat.HatchingSpace = int(m_SelectDrawFormat.HatchingSpace* dRatio);

		// Dim
		Pos = m_arDimFormat.GetStartPosition();
		while(Pos)
		{		
			m_arDimFormat.GetNextAssoc(Pos, Index, DimFormat);
			DimFormat.Width = int(DimFormat.Width * dRatio);
			m_arDimFormat.SetAt(Index, DimFormat);
		}
		m_SelectDimFormat.Width = int(m_SelectDimFormat.Width * dRatio);

		// Chart
		Pos = m_arChartFormat.GetStartPosition();
		while(Pos)
		{		
			m_arChartFormat.GetNextAssoc(Pos, Index, ChartFormat);
			ChartFormat.frame.Width           = int(ChartFormat.frame.Width           * dRatio);
			ChartFormat.remark.Width          = int(ChartFormat.remark.Width          * dRatio);
			ChartFormat.xAxis.Width           = int(ChartFormat.xAxis.Width           * dRatio);
			ChartFormat.xAxis.nMajorGridWidth = int(ChartFormat.xAxis.nMajorGridWidth * dRatio);		
			ChartFormat.xAxis.nMinorGridWidth = int(ChartFormat.xAxis.nMinorGridWidth * dRatio);		
			ChartFormat.yAxis.Width           = int(ChartFormat.yAxis.Width           * dRatio);
			ChartFormat.yAxis.nMajorGridWidth = int(ChartFormat.yAxis.nMajorGridWidth * dRatio);
			ChartFormat.yAxis.nMinorGridWidth = int(ChartFormat.yAxis.nMinorGridWidth * dRatio);
			m_arChartFormat.SetAt(Index, ChartFormat);
		}
		m_SelectChartFormat.frame.Width           = int(m_SelectChartFormat.frame.Width           * dRatio);
		m_SelectChartFormat.remark.Width          = int(m_SelectChartFormat.remark.Width          * dRatio);
		m_SelectChartFormat.xAxis.Width           = int(m_SelectChartFormat.xAxis.Width           * dRatio);
		m_SelectChartFormat.xAxis.nMajorGridWidth = int(m_SelectChartFormat.xAxis.nMajorGridWidth * dRatio);
		m_SelectChartFormat.xAxis.nMinorGridWidth = int(m_SelectChartFormat.xAxis.nMinorGridWidth * dRatio);
		m_SelectChartFormat.yAxis.Width           = int(m_SelectChartFormat.yAxis.Width           * dRatio);
		m_SelectChartFormat.yAxis.nMajorGridWidth = int(m_SelectChartFormat.yAxis.nMajorGridWidth * dRatio);
		m_SelectChartFormat.yAxis.nMinorGridWidth = int(m_SelectChartFormat.yAxis.nMinorGridWidth * dRatio);

		CDrawChart* pDC_Chart;
		Pos = m_arChart.GetStartPosition();
		
		while(Pos)
		{		
			int i, j;
			m_arChart.GetNextAssoc(Pos, Index, pDC_Chart);
			
			int no = pDC_Chart->m_Data.List.GetSize();
			for(i = 0; i < no; i++) 
			{ 
				pDC_Chart->m_Data.List[i].Info.xAxis.dNotchHeightRate = (int)(pDC_Chart->m_Data.List[i].Info.xAxis.dNotchHeightRate * dRatio); 
				pDC_Chart->m_Data.List[i].Info.yAxis.dNotchHeightRate = (int)(pDC_Chart->m_Data.List[i].Info.yAxis.dNotchHeightRate * dRatio); 
				
				auto no2 = pDC_Chart->m_Data.List[i].arData.GetSize();
				for(j = 0; j < no2; j++) 
				{
					pDC_Chart->m_Data.List[i].arData[j].Info.lineWidth = (int)(pDC_Chart->m_Data.List[i].arData[j].Info.lineWidth * dRatio); 
					pDC_Chart->m_Data.List[i].arData[j].Info.markSize  = (int)(pDC_Chart->m_Data.List[i].arData[j].Info.markSize  * dRatio); 
				}
			}

			no = pDC_Chart->m_DataR.List.GetSize();
			for(i = 0; i < no; i++) 
			{ 
				pDC_Chart->m_DataR.List[i].Info.xAxis.dNotchHeightRate = (int)(pDC_Chart->m_DataR.List[i].Info.xAxis.dNotchHeightRate * dRatio); 
				pDC_Chart->m_DataR.List[i].Info.yAxis.dNotchHeightRate = (int)(pDC_Chart->m_DataR.List[i].Info.yAxis.dNotchHeightRate * dRatio); 
				
				int no2 = pDC_Chart->m_DataR.List[i].arData.GetSize();
				for(j = 0; j < no2; j++) 
				{
					pDC_Chart->m_DataR.List[i].arData[j].Info.lineWidth = (int)(pDC_Chart->m_DataR.List[i].arData[j].Info.lineWidth * dRatio); 
					pDC_Chart->m_DataR.List[i].arData[j].Info.markSize  = (int)(pDC_Chart->m_DataR.List[i].arData[j].Info.markSize  * dRatio); 
				}
			}		
		}
	}

	int nDrawFormatType;
	CDrawBase* pDrawClass = NULL;
	
	for(UINT iIndex = 1; iIndex <= m_FinalEntityNum; iIndex++)
	{
		pDrawClass = GetDrawClass(iIndex);
		
		if(pDrawClass != NULL) 
		{
			nDrawFormatType = pDrawClass->GetFormatType();
			if(nDrawFormatType == EN_DRFORMAT_TYPE_DIM)
			{
				if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat)) DimFormat.Init();	
				pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DimFormat);		
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_CHART)
			{
				if(!m_arChartFormat.Lookup(pDrawClass->m_DrawFormatKey, ChartFormat)) ChartFormat.Init();	
				pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &ChartFormat);		
			}
		}		
	}	

	return;
}


BOOL CDrawCtrl::IsOverLap(const T_POINT tpA, const T_POINT tpB)
{
	if(abs(tpA.x - tpB.x) < 0.00001 && abs(tpA.y - tpB.y) < 0.00001)
		return TRUE;

	return FALSE;
}

double CDrawCtrl::GetAngle(const T_POINT &ptStP1, const T_POINT &ptStP2, const T_POINT &ptEndP1, const T_POINT &ptEndP2)
{
	double Vec1[3], Vec2[3]; 

	GetFrmDirectionVector(ptStP1 ,  ptStP2, Vec1[0], Vec1[1], Vec1[2]);
	GetFrmDirectionVector(ptEndP1, ptEndP2, Vec2[0], Vec2[1], Vec2[2]);

	double dAngle = CMathFunc::mathCrossAngle(Vec1, Vec2);
	return dAngle;
}

void CDrawCtrl::GetFrmDirectionVector(const T_POINT &ptSt, const T_POINT &ptEnd, double& Ux, double& Uy, double& Uz )
{
	double Coor_i[3], Coor_j[3], Vec[3], NormVec[3];
	Coor_i[0]=ptSt.x;	  Coor_i[1]=ptSt.y;		Coor_i[2]=0.0;
	Coor_j[0]=ptEnd.x;	  Coor_j[1]=ptEnd.y;	Coor_j[2]=0.0;

	for(int i= 0; i < 3; i++) Vec[i] = (Coor_j[i] - Coor_i[i]);
	CMathFunc::mathNormalize(Vec, NormVec);
	Ux = NormVec[0];
	Uy = NormVec[1];
	Uz = NormVec[2];
}