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


////////////////////////////////////////////
UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_POINT_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawPoint* pDrawClass = new CDrawPoint;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;
	
	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = { (double)iOffsetX, (double)iOffsetY };
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	
	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arPoint.SetAt(m_FinalEntityNum, pDrawClass);
	
	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_LINE_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawLine* pDrawClass = new CDrawLine;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;	

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	
	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arLine.SetAt(m_FinalEntityNum, pDrawClass);
	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ARROWLINE_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawArrowLine* pDrawClass = new CDrawArrowLine;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arArrowLine.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_CIRCLE_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawCircle* pDrawClass = new CDrawCircle;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;
	
	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = { (double)iOffsetX, (double)iOffsetY };
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arCircle.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DONUT_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawDonut* pDrawClass = new CDrawDonut;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) 
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arDonut.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ELLIPSE_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawEllipse* pDrawClass = new CDrawEllipse;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arEllipse.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_RECT_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawRect* pDrawClass = new CDrawRect;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) 
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arRect.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ARC_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawArc* pDrawClass = new CDrawArc;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arArc.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_POLYLINE_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawPolyline* pDrawClass = new CDrawPolyline;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arPolyline.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_POLYGON_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawPolygon* pDrawClass = new CDrawPolygon;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arPolygon.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ARROWPOLYLINE_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawArrowPolyline* pDrawClass = new CDrawArrowPolyline;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) 
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arArrowPolyline.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_TEXT_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawText* pDrawClass = new CDrawText;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_TEXT_FORMAT TextFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arTextFormat.Lookup(pDrawClass->m_DrawFormatKey, TextFormat))
		{
			//TextFormat.Init(pProdInfo->GetLanguageID());	
			TextFormat.Init(0x01);	
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &TextFormat);
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arText.SetAt(m_FinalEntityNum, pDrawClass);
	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DIMENSION_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawDimension* pDrawClass = new CDrawDimension;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DIM_FORMAT DimFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat)) 
		{
			//DimFormat.Init(pProdInfo->GetLanguageID());	
			DimFormat.Init(0x01);	
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DimFormat);		
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arDimension.SetAt(m_FinalEntityNum, pDrawClass);
	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DIMANGLE_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawDimAngle* pDrawClass = new CDrawDimAngle;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DIM_FORMAT DimFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat)) 
		{
			//DimFormat.Init(pProdInfo->GetLanguageID());	
			DimFormat.Init(0x01);	
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DimFormat);		
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arDimAngle.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DIMLEADERLINE_GROUP   & rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawDimLeaderline* pDrawClass = new CDrawDimLeaderline;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_DIM_FORMAT DimFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat))
		{
			//DimFormat.Init(pProdInfo->GetLanguageID());	
			DimFormat.Init(0x01);	
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &DimFormat);		
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arDimLeaderline.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_CHART_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, int iOffsetX, int iOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	int iXmin, iYmin, iXmax, iYmax;
	CDrawChart* pDrawClass = new CDrawChart;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_Data = rData;

	if(iOffsetX != 0 || iOffsetY != 0)
	{
		double dVector[2] = {(double)iOffsetX, (double)iOffsetY};
		pDrawClass->Move(TRUE, dVector, NULL); 
	}
	pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

	if(bConvert)
	{
		T_CHART_FORMAT ChartFormat;
		if(!m_arChartFormat.Lookup(pDrawClass->m_DrawFormatKey, ChartFormat)) 
		{
			ChartFormat.Init();	
		}
		pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &ChartFormat);		
	}

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arChart.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

//////////////////////////
UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_POINT_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{	
	CDrawPoint* pDrawClass = new CDrawPoint;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arPoint.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_LINE_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawLine* pDrawClass = new CDrawLine;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) 
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arLine.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ARROWLINE_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawArrowLine* pDrawClass = new CDrawArrowLine;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) 
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	

	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arArrowLine.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_CIRCLE_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawCircle* pDrawClass = new CDrawCircle;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arCircle.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DONUT_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawDonut* pDrawClass = new CDrawDonut;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arDonut.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ELLIPSE_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawEllipse* pDrawClass = new CDrawEllipse;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arEllipse.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_RECT_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawRect* pDrawClass = new CDrawRect;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) 
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arRect.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ARC_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawArc* pDrawClass = new CDrawArc;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arArc.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_POLYLINE_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawPolyline* pDrawClass = new CDrawPolyline;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arPolyline.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_POLYGON_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawPolygon* pDrawClass = new CDrawPolygon;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat))
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arPolygon.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_ARROWPOLYLINE_R_GROUP & rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawArrowPolyline* pDrawClass = new CDrawArrowPolyline;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DRAW_FORMAT DrawFormat;
		if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) 
		{
			DrawFormat.Init();
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arArrowPolyline.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_TEXT_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawText* pDrawClass = new CDrawText;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_TEXT_FORMAT TextFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arTextFormat.Lookup(pDrawClass->m_DrawFormatKey, TextFormat)) 
		{
			//TextFormat.Init(pProdInfo->GetLanguageID());
			TextFormat.Init(0x01);
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &TextFormat);
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arText.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DIMENSION_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawDimension* pDrawClass = new CDrawDimension;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DIM_FORMAT DimFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat))
		{
			//DimFormat.Init(pProdInfo->GetLanguageID());	
			DimFormat.Init(0x01);	
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DimFormat);	
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arDimension.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DIMANGLE_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawDimAngle* pDrawClass = new CDrawDimAngle;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DIM_FORMAT DimFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat))
		{
			//DimFormat.Init(pProdInfo->GetLanguageID());	
			DimFormat.Init(0x01);	
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DimFormat);	
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	if(!ChkMaxDrawUnit()) return FALSE;
	m_arDimAngle.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_DIMLEADERLINE_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawDimLeaderline* pDrawClass = new CDrawDimLeaderline;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_DIM_FORMAT DimFormat;

		//IProductInfo* pProdInfo = IProductInfo::Instance();

		if(!m_arDimFormat.Lookup(pDrawClass->m_DrawFormatKey, DimFormat)) 
		{
			//DimFormat.Init(pProdInfo->GetLanguageID());	
			DimFormat.Init(0x01);	
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DimFormat);	
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	
	if(! ChkMaxDrawUnit()) return FALSE;
	m_arDimLeaderline.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}

UINT CDrawCtrl::AddDrawUnit(int nFormatKey, T_CHART_R_GROUP& rData, BOOL bConvert, int nLinkKey, int nSnapType, BOOL bIsSelect, double dOffsetX, double dOffsetY, BOOL bIsOverlapChk, int nSelectSubType)
{
	CDrawChart* pDrawClass = new CDrawChart;
	pDrawClass->m_DrawFormatKey = nFormatKey;
	pDrawClass->m_LinkKey = nLinkKey;
	pDrawClass->m_bIsSelect = bIsSelect;
	pDrawClass->m_nSelectSubType = nSelectSubType;
	pDrawClass->m_SnapType  = nSnapType;
	pDrawClass->m_bIsOverLapChk = bIsOverlapChk;
	pDrawClass->m_DataR = rData;	

	if(dOffsetX != 0.0 || dOffsetY != 0.0)
	{
		double dVector[2] = {dOffsetX, dOffsetY};
		pDrawClass->Move(FALSE, dVector, NULL); 
	}

	if(bConvert)
	{
		int iXmin, iYmin, iXmax, iYmax;
		T_CHART_FORMAT ChartFormat;
		if(!m_arChartFormat.Lookup(pDrawClass->m_DrawFormatKey, ChartFormat)) 
		{
			ChartFormat.Init();	
		}
		pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &ChartFormat);	
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
	}	
	m_FinalEntityNum++;
	
	if(! ChkMaxDrawUnit()) return FALSE;
	m_arChart.SetAt(m_FinalEntityNum, pDrawClass);

	return m_FinalEntityNum;
}
