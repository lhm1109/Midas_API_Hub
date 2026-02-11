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


BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_POINT_GROUP& rData, BOOL bConvert)
{
	CDrawPoint* pDrawClass = NULL;
	if(m_arPoint.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_LINE_GROUP& rData, BOOL bConvert)
{
	CDrawLine* pDrawClass = NULL;
	if(m_arLine.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ARROWLINE_GROUP& rData, BOOL bConvert)
{
	CDrawArrowLine* pDrawClass = NULL;
	if(m_arArrowLine.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_CIRCLE_GROUP& rData, BOOL bConvert)
{
	CDrawCircle* pDrawClass = NULL;
	if(m_arCircle.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DONUT_GROUP& rData, BOOL bConvert)
{
	CDrawDonut* pDrawClass = NULL;
	if(m_arDonut.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ELLIPSE_GROUP& rData, BOOL bConvert)
{
	CDrawEllipse* pDrawClass = NULL;

	if(m_arEllipse.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_RECT_GROUP& rData, BOOL bConvert)
{
	CDrawRect* pDrawClass = NULL;
	if(m_arRect.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ARC_GROUP& rData, BOOL bConvert)
{
	CDrawArc* pDrawClass = NULL;
	if(m_arArc.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_POLYLINE_GROUP& rData, BOOL bConvert)
{
	CDrawPolyline* pDrawClass = NULL;

	if(m_arPolyline.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_POLYGON_GROUP& rData, BOOL bConvert)
{
	CDrawPolygon* pDrawClass = NULL;
	if(m_arPolygon.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ARROWPOLYLINE_GROUP& rData, BOOL bConvert)
{
	CDrawArrowPolyline* pDrawClass = NULL;
	if(m_arArrowPolyline.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_TEXT_GROUP& rData, BOOL bConvert)
{
	CDrawText* pDrawClass = NULL;
	if(m_arText.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DIMENSION_GROUP& rData, BOOL bConvert)
{
	CDrawDimension* pDrawClass = NULL;
	if(m_arDimension.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DIMANGLE_GROUP& rData, BOOL bConvert)
{
	CDrawDimAngle* pDrawClass = NULL;
	if(m_arDimAngle.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DIMLEADERLINE_GROUP& rData, BOOL bConvert)
{
	CDrawDimLeaderline* pDrawClass = NULL;
	if(m_arDimLeaderline.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_CHART_GROUP& rData, BOOL bConvert)
{
	CDrawChart* pDrawClass = NULL;

	if(m_arChart.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_Data = rData;
		int iXmin, iYmin, iXmax, iYmax;
		pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);

		if(bConvert)
		{
			T_CHART_FORMAT ChartFormat;
			//IProductInfo* pProdInfo = IProductInfo::Instance();

			if(!m_arChartFormat.Lookup(pDrawClass->m_DrawFormatKey, ChartFormat))
			{
				//ChartFormat.Init(pProdInfo->GetLanguageID());
				ChartFormat.Init(0x01);
			}
			pDrawClass->Convert(FALSE, &m_DrawFunc, &m_PaperFormat, &ChartFormat);		
		}
		return TRUE;
	}
	return FALSE;
}

////////////////////////////////
BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_POINT_R_GROUP& rData, BOOL bConvert)
{
	CDrawPoint* pDrawClass = NULL;

	if(m_arPoint.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_LINE_R_GROUP& rData, BOOL bConvert)
{
	CDrawLine* pDrawClass = NULL;

	if(m_arLine.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;

		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ARROWLINE_R_GROUP& rData, BOOL bConvert)
{
	CDrawArrowLine* pDrawClass = NULL;

	if(m_arArrowLine.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_CIRCLE_R_GROUP& rData, BOOL bConvert)
{
	CDrawCircle* pDrawClass = NULL;
	if(m_arCircle.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DONUT_R_GROUP& rData, BOOL bConvert)
{
	CDrawDonut* pDrawClass = NULL;
	if(m_arDonut.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ELLIPSE_R_GROUP& rData, BOOL bConvert)
{
	CDrawEllipse* pDrawClass = NULL;
	if(m_arEllipse.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_RECT_R_GROUP& rData, BOOL bConvert)
{
	CDrawRect* pDrawClass = NULL;
	if(m_arRect.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ARC_R_GROUP& rData, BOOL bConvert)
{
	CDrawArc* pDrawClass = NULL;
	if(m_arArc.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_POLYLINE_R_GROUP& rData, BOOL bConvert)
{
	CDrawPolyline* pDrawClass = NULL;

	if(m_arPolyline.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_POLYGON_R_GROUP& rData, BOOL bConvert)
{
	CDrawPolygon* pDrawClass = NULL;
	if(m_arPolygon.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;
		if(bConvert)
		{
			int iXmin, iYmin, iXmax, iYmax;
			T_DRAW_FORMAT DrawFormat;
			if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
			pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &DrawFormat);
			pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
		}	
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_ARROWPOLYLINE_R_GROUP& rData, BOOL bConvert)
{
	CDrawArrowPolyline* pDrawClass = NULL;
	if(m_arArrowPolyline.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_TEXT_R_GROUP& rData, BOOL bConvert)
{
	CDrawText* pDrawClass = NULL;

	if(m_arText.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DIMENSION_R_GROUP& rData, BOOL bConvert)
{
	CDrawDimension* pDrawClass = NULL;

	if(m_arDimension.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DIMANGLE_R_GROUP& rData, BOOL bConvert)
{
	CDrawDimAngle* pDrawClass = NULL;
	if(m_arDimAngle.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;
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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_DIMLEADERLINE_R_GROUP& rData, BOOL bConvert)
{
	CDrawDimLeaderline* pDrawClass = NULL;
	if(m_arDimLeaderline.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

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
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawCtrl::ModifyDrawUnit(UINT key, T_CHART_R_GROUP & rData, BOOL bConvert)
{
	CDrawChart* pDrawClass = NULL;
	if(m_arChart.Lookup(key, pDrawClass))
	{
		if(pDrawClass == NULL) return FALSE;
		pDrawClass->m_DataR = rData;

		if(bConvert)
		{
			int iXmin, iYmin, iXmax, iYmax;
			T_CHART_FORMAT ChartFormat;
			//IProductInfo* pProdInfo = IProductInfo::Instance();

			if(!m_arChartFormat.Lookup(pDrawClass->m_DrawFormatKey, ChartFormat))
			{
				//ChartFormat.Init(pProdInfo->GetLanguageID());
				ChartFormat.Init(0x01);
			}
			pDrawClass->Convert(TRUE, &m_DrawFunc, &m_PaperFormat, &ChartFormat);
			pDrawClass->GetBoundary(iXmin, iYmin, iXmax, iYmax, TRUE);
		}	
		return TRUE;
	}
	return FALSE;
}
