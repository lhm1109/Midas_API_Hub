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


void CDrawCtrl::RotateData(double dRotate, double dOffsetx, double dOffsety, long& x, long& y)
{
	double dx = (double)x;
	double dy = (double)y;
	double dz = 0.0;

	CMathFunc::mathRotate(dRotate, dOffsetx, 0.0, 0.0, 0.0, 0.0, 1.0, dx, dy, dz);

	x = (long)dx;
	y = (long)(dy + dOffsety);
}

void CDrawCtrl::RotateData(double dRotate, double dOffsetx, double dOffsety, double& x, double& y)
{
	double z = 0.0;

	CMathFunc::mathRotate(dRotate, dOffsetx, 0.0, 0.0, 0.0, 0.0, 1.0, x, y, z);
	y += dOffsety;
}

void CDrawCtrl::RotateDrawData(double dRotate, double dOffsetx, double dOffsety, UINT& nSt)
{	
	CDrawPoint         *pDC_Point = NULL;
	CDrawLine          *pDC_Line = NULL;
	CDrawArrowLine     *pDC_ArrowLine = NULL;
	CDrawCircle        *pDC_Circle = NULL;
	CDrawDonut         *pDC_Donut = NULL;
	CDrawEllipse       *pDC_Ellipse = NULL;
	CDrawRect          *pDC_Rect = NULL;
	CDrawArc           *pDC_Arc = NULL;
	CDrawPolyline      *pDC_Polyline = NULL;
	CDrawPolygon       *pDC_Polygon = NULL;
	CDrawArrowPolyline *pDC_ArrowPolyline = NULL;
	CDrawText          *pDC_Text = NULL;
	CDrawDimension     *pDC_Dimension = NULL;
	CDrawDimAngle      *pDC_DimAngle = NULL;
	CDrawDimLeaderline *pDC_DimLeaderline = NULL;
	CDrawChart         *pDC_Chart = NULL;

	if(nSt == 0)  nSt = 1;
	CDrawBase* pDrawClass = NULL;
	int i, j;

	for(UINT iIndex=nSt ; iIndex <= m_FinalEntityNum ; iIndex++)
	{
		pDrawClass = GetDrawClass(iIndex);
		if(pDrawClass == NULL) continue;

		UINT key = iIndex;
		if(m_arPoint.Lookup(key, pDC_Point))
		{	
			//for(auto i = pDC_Point->m_Data.List.Data.begin(); i != pDC_Point->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Point->m_Data.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Point->m_Data.List[i].x, pDC_Point->m_Data.List[i].y);
			}

			//for(auto i = pDC_Point->m_DataR.List.Data.begin(); i != pDC_Point->m_DataR.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Point->m_DataR.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Point->m_DataR.List[i].x, pDC_Point->m_DataR.List[i].y);
			}

			m_arPoint.SetAt(key, pDC_Point);
		}
		else if(m_arLine.Lookup(key, pDC_Line))
		{
			//for(auto i = pDC_Line->m_Data.List.Data.begin(); i != pDC_Line->m_Data.List.Data.end(); i++) 
			for(i = 0 ; i < pDC_Line->m_Data.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Line->m_Data.List[i].P1.x, pDC_Line->m_Data.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Line->m_Data.List[i].P2.x, pDC_Line->m_Data.List[i].P2.y);
			}

			//for(auto i = pDC_Line->m_DataR.List.Data.begin(); i != pDC_Line->m_DataR.List.Data.end(); i++) 
			for(int i = 0 ; i < pDC_Line->m_DataR.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Line->m_DataR.List[i].P1.x, pDC_Line->m_DataR.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Line->m_DataR.List[i].P2.x, pDC_Line->m_DataR.List[i].P2.y);
			}

			m_arLine.SetAt(key, pDC_Line);
		}
		else if(m_arArrowLine.Lookup(key, pDC_ArrowLine))
		{	
			//for(auto i = pDC_ArrowLine->m_Data.List.Data.begin(); i != pDC_ArrowLine->m_Data.List.Data.end(); i++) 
			for(i = 0 ; i < pDC_ArrowLine->m_Data.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_ArrowLine->m_Data.List[i].P1.x, pDC_ArrowLine->m_Data.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_ArrowLine->m_Data.List[i].P2.x, pDC_ArrowLine->m_Data.List[i].P2.y);
			}

			//for(auto i = pDC_ArrowLine->m_DataR.List.Data.begin(); i != pDC_ArrowLine->m_DataR.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_ArrowLine->m_DataR.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_ArrowLine->m_DataR.List[i].P1.x, pDC_ArrowLine->m_DataR.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_ArrowLine->m_DataR.List[i].P2.x, pDC_ArrowLine->m_DataR.List[i].P2.y);
			}

			m_arArrowLine.SetAt(key, pDC_ArrowLine);
		}
		else if(m_arCircle.Lookup(key, pDC_Circle))
		{
			//for(auto i = pDC_Circle->m_Data.List.Data.begin(); i != pDC_Circle->m_Data.List.Data.end(); i++) 
			for(i = 0 ; i < pDC_Circle->m_Data.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Circle->m_Data.List[i].CenterP.x, pDC_Circle->m_Data.List[i].CenterP.y);
			}

			//for(auto i = pDC_Circle->m_DataR.List.Data.begin(); i != pDC_Circle->m_DataR.List.Data.end(); i++) 
			for(i = 0 ; i < pDC_Circle->m_DataR.List.GetSize() ;i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Circle->m_DataR.List[i].CenterP.x, pDC_Circle->m_DataR.List[i].CenterP.y);
			}

			m_arCircle.SetAt(key, pDC_Circle);
		}
		else if(m_arDonut.Lookup(key, pDC_Donut))
		{
			continue;
		}
		else if(m_arEllipse.Lookup(key, pDC_Ellipse))
		{
			continue;
		}
		else if(m_arRect.Lookup(key, pDC_Rect))
		{
			//for(auto i = pDC_Rect->m_Data.List.Data.begin(); i != pDC_Rect->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Rect->m_Data.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Rect->m_Data.List[i].P1.x, pDC_Rect->m_Data.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Rect->m_Data.List[i].P2.x, pDC_Rect->m_Data.List[i].P2.y);
			}

			//for(auto i = pDC_Rect->m_DataR.List.Data.begin(); i != pDC_Rect->m_DataR.List.Data.end(); i++) 
			for(i = 0 ; i < pDC_Rect->m_DataR.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Rect->m_DataR.List[i].P1.x, pDC_Rect->m_DataR.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Rect->m_DataR.List[i].P2.x, pDC_Rect->m_DataR.List[i].P2.y);
			}

			m_arRect.SetAt(key, pDC_Rect);
		}
		else if(m_arArc.Lookup(key, pDC_Arc))
		{
			continue;
		}
		if(m_arPolyline.Lookup(key, pDC_Polyline))
		{
			//for(auto i = pDC_Polyline->m_Data.List.Data.begin(); i != pDC_Polyline->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Polyline->m_Data.List.GetSize() ; i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++)
				for( j = 0 ; j < pDC_Polyline->m_Data.List[i].arPoint.GetSize(); j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_Polyline->m_Data.List[i].arPoint[j].x, pDC_Polyline->m_Data.List[i].arPoint[j].y);
				}
			}

			//for(auto i = pDC_Polyline->m_DataR.List.Data.begin(); i != pDC_Polyline->m_DataR.List.Data.end(); i++)  
			for( i = 0 ; i < pDC_Polyline->m_DataR.List.GetSize(); i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++) 
				for( j = 0 ; j < pDC_Polyline->m_DataR.List[i].arPoint.GetSize(); j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_Polyline->m_DataR.List[i].arPoint[j].x, pDC_Polyline->m_DataR.List[i].arPoint[j].y);
				}
			}

			m_arPolyline.SetAt(key, pDC_Polyline);
		}
		else if(m_arPolygon.Lookup(key, pDC_Polygon)) 
		{	
//			for(auto i = pDC_Polygon->m_Data.List.Data.begin(); i != pDC_Polygon->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Polygon->m_Data.List.GetSize(); i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++) 
				for( j = 0 ; j < pDC_Polygon->m_Data.List[i].arPoint.GetSize() ; j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_Polygon->m_Data.List[i].arPoint[j].x, pDC_Polygon->m_Data.List[i].arPoint[j].y);
				}
			}

			//for(auto i = pDC_Polygon->m_DataR.List.Data.begin(); i != pDC_Polygon->m_DataR.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Polygon->m_DataR.List.GetSize() ;i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++) 
				for( j = 0 ; j < pDC_Polygon->m_DataR.List[i].arPoint.GetSize(); j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_Polygon->m_DataR.List[i].arPoint[j].x, pDC_Polygon->m_DataR.List[i].arPoint[j].y);
				}
			}

			m_arPolygon.SetAt(key, pDC_Polygon);
		}
		else if(m_arArrowPolyline.Lookup(key, pDC_ArrowPolyline))
		{	
//			for(auto i = pDC_ArrowPolyline->m_Data.List.Data.begin(); i != pDC_ArrowPolyline->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_ArrowPolyline->m_Data.List.GetSize() ; i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++) 
				for( j = 0 ; j < pDC_ArrowPolyline->m_Data.List[i].arPoint.GetSize(); j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_ArrowPolyline->m_Data.List[i].arPoint[j].x, pDC_ArrowPolyline->m_Data.List[i].arPoint[j].y);
				}
			}

			//for(auto i = pDC_ArrowPolyline->m_DataR.List.Data.begin(); i != pDC_ArrowPolyline->m_DataR.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_ArrowPolyline->m_DataR.List.GetSize(); i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++)
				for( j = 0 ; j < pDC_ArrowPolyline->m_DataR.List[i].arPoint.GetSize(); j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_ArrowPolyline->m_DataR.List[i].arPoint[j].x, pDC_ArrowPolyline->m_DataR.List[i].arPoint[j].y);
				}
			}

			m_arArrowPolyline.SetAt(key, pDC_ArrowPolyline) ;   
		}
		else if(m_arText.Lookup(key, pDC_Text))
		{
			//for(auto i = pDC_Text->m_Data.List.Data.begin(); i != pDC_Text->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Text->m_Data.List.GetSize() ;i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Text->m_Data.List[i].PPosition.x, pDC_Text->m_Data.List[i].PPosition.y);
			}

			//for(auto i = pDC_Text->m_DataR.List.Data.begin(); i != pDC_Text->m_DataR.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Text->m_DataR.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Text->m_DataR.List[i].PPosition.x, pDC_Text->m_DataR.List[i].PPosition.y);
			}

			m_arText.SetAt(key, pDC_Text);
		}
		else if(m_arDimension.Lookup(key, pDC_Dimension))
		{
			//for(auto i = pDC_Dimension->m_Data.List.Data.begin(); i != pDC_Dimension->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Dimension->m_Data.List.GetSize() ;i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Dimension->m_Data.List[i].P1.x, pDC_Dimension->m_Data.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Dimension->m_Data.List[i].P2.x, pDC_Dimension->m_Data.List[i].P2.y);
			}

			//for(auto i = pDC_Dimension->m_DataR.List.Data.begin(); i != pDC_Dimension->m_DataR.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_Dimension->m_DataR.List.GetSize() ;i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Dimension->m_DataR.List[i].P1.x, pDC_Dimension->m_DataR.List[i].P1.y);
				RotateData(dRotate, dOffsetx, dOffsety, pDC_Dimension->m_DataR.List[i].P2.x, pDC_Dimension->m_DataR.List[i].P2.y);
			}

			m_arDimension.SetAt(key, pDC_Dimension);
		}
		else if(m_arDimAngle.Lookup(key, pDC_DimAngle))
		{
			//for(auto i = pDC_DimAngle->m_Data.List.Data.begin(); i != pDC_DimAngle->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_DimAngle->m_Data.List.GetSize() ;i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_DimAngle->m_Data.List[i].CenterP.x, pDC_DimAngle->m_Data.List[i].CenterP.y);
			}

			//for(auto i = pDC_DimAngle->m_DataR.List.Data.begin(); i != pDC_DimAngle->m_DataR.List.Data.end(); i++) 
			for(i = 0 ; i < pDC_DimAngle->m_DataR.List.GetSize(); i++)
			{
				RotateData(dRotate, dOffsetx, dOffsety, pDC_DimAngle->m_DataR.List[i].CenterP.x, pDC_DimAngle->m_DataR.List[i].CenterP.y);
			}

			m_arDimAngle.SetAt(key, pDC_DimAngle);
		}
		else if(m_arDimLeaderline.Lookup(key, pDC_DimLeaderline))
		{
			//for(auto i = pDC_DimLeaderline->m_Data.List.Data.begin(); i != pDC_DimLeaderline->m_Data.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_DimLeaderline->m_Data.List.GetSize() ;i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++) 
				for( j = 0 ; j < pDC_DimLeaderline->m_Data.List[i].arPoint.GetSize() ; j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_DimLeaderline->m_Data.List[i].arPoint[j].x, pDC_DimLeaderline->m_Data.List[i].arPoint[j].y);
				}
			}

			//for(auto i = pDC_DimLeaderline->m_DataR.List.Data.begin(); i != pDC_DimLeaderline->m_DataR.List.Data.end(); i++) 
			for( i = 0 ; i < pDC_DimLeaderline->m_DataR.List.GetSize() ;i++)
			{
				//for(auto j = i->arPoint.Data.begin(); j != i->arPoint.Data.end(); j++) 
				for( j = 0 ; j < pDC_DimLeaderline->m_DataR.List[i].arPoint.GetSize() ;j++)
				{
					RotateData(dRotate, dOffsetx, dOffsety, pDC_DimLeaderline->m_DataR.List[i].arPoint[j].x, pDC_DimLeaderline->m_DataR.List[i].arPoint[j].y);
				}
			}

			m_arDimLeaderline.SetAt(key, pDC_DimLeaderline);
		}
		else if(m_arChart.Lookup(key, pDC_Chart))
		{	
			continue;
		}
	}

	nSt = m_FinalEntityNum;
}