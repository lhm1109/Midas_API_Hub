#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"
// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"

#include "DrawEditBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CDrawEditBtn::AxisDraw()
{
	DrawAxisUnit(&m_DrawCtrl, m_nAxisMode, m_nAxisSize, m_nAxisWidth, m_bCapital, m_AxisDrawFormat, m_bCDC_AxisOrgPoint, m_AxisOrgPoint, m_AxisOrgPointR, m_dAxisVector1, m_dAxisVector2);
}

void CDrawEditBtn::GridDraw()
{
	if(m_DrawCtrl.m_PaperFormat.ScaleX == 0.0 || m_DrawCtrl.m_PaperFormat.ScaleY == 0.0) return;

	if(m_nGridMode == EN_DRAW_GRIDMODE_DOT)
	{
		double dCDCInterval[2] = { m_dGridInterval[0] / m_DrawCtrl.m_PaperFormat.ScaleX, m_dGridInterval[1] / m_DrawCtrl.m_PaperFormat.ScaleY };

		// 한칸의 간격이 5Pixel보다 작다면 그리지 않음
		if(dCDCInterval[0] < 5.0 || dCDCInterval[1] < 5.0) return; 

		CPoint OrgP = m_DrawCtrl.m_PaperFormat.OrgPoint;

		CRect GridSize; //Grid의 갯수크기
		GridSize.left   = (int)(((double)m_Canvas.left   - (double)OrgP.x) / dCDCInterval[0]);
		GridSize.right  = (int)(((double)m_Canvas.right  - (double)OrgP.x) / dCDCInterval[0]);
		GridSize.top    = (int)(((double)m_Canvas.top    - (double)OrgP.y) / dCDCInterval[1]);
		GridSize.bottom = (int)(((double)m_Canvas.bottom - (double)OrgP.y) / dCDCInterval[0]);

		T_POINT_GROUP dotGroup;
		CPoint        dotUnit;

		int nSizeX, nSizeY, nGapX, nGapY;
		nSizeX = nSizeY = nGapX = nGapY = 0;
		nSizeX = GridSize.Width() ;
		nSizeY = GridSize.Height(); 

		if(GridSize.left > 0)		  nGapX++;
		else if(GridSize.right > 0)  nSizeX++;
		if(GridSize.top > 0)         nGapY++;
		else if(GridSize.bottom > 0) nSizeY++;

		if(nSizeX * nSizeY == 0) return; 
		dotGroup.List.SetSize(nSizeX*nSizeY);
		int nCount = 0;

		for(int i = 0; i < nSizeX ; i++)
		{
			for(int j = 0; j < nSizeY; j++)
			{
				dotUnit.x = OrgP.x + (int)((GridSize.left + i + nGapX) * dCDCInterval[0]);
				dotUnit.y = OrgP.y + (int)((GridSize.top + j + nGapY) * dCDCInterval[1]);
				dotGroup.List[nCount] = dotUnit;
				nCount++;
			}
		}
		m_DrawCtrl.DrawUnit(m_GridDrawFormat, dotGroup);
	}
}

void CDrawEditBtn::SnapDraw()
{
	m_DrawCtrl.DrawUnit(m_SnapDrawFormat, m_SnapDrawGroup);
}

void CDrawEditBtn::HighlightDraw()
{
	m_DrawCtrl.DrawUnit(m_HighlightDrawFormat, m_HighlightDrawGroup);
}

void CDrawEditBtn::SnapRemoveDrawData()
{
	m_SnapDrawGroup.Init();
}

void CDrawEditBtn::HighlightRemoveDrawData()
{
	m_HighlightDrawGroup.Init();
}

void CDrawEditBtn::SelectDraw()
{
	m_DrawCtrl.DrawUnit(m_SelectAreaDrawFormat, m_SelectAreaDrawGroup);
}

void CDrawEditBtn::SelectRemoveDrawData()
{
	m_SelectAreaDrawGroup.Init();
}

void CDrawEditBtn::OverlapDraw()
{
	if(m_nOverlapType != EN_OVERLAP_NONE) m_DrawCtrl.DrawUnit(m_OverlapDimFormat, m_OverlapDrawGroup);
}

void CDrawEditBtn::OverlapRemoveDrawData()
{
	m_OverlapDrawGroup.Init();
}

void CDrawEditBtn::CurrRemoveDrawData()
{	
	m_CurrDrawClass_Point.m_Data.Init();  
	m_CurrDrawClass_Point.m_DataR.Init();
	m_CurrDrawClass_Line.m_Data.Init();  
	m_CurrDrawClass_Line.m_DataR.Init();
	m_CurrDrawClass_ArrowLine.m_Data.Init();  
	m_CurrDrawClass_ArrowLine.m_DataR.Init();
	m_CurrDrawClass_Circle.m_Data.Init();  
	m_CurrDrawClass_Circle.m_DataR.Init();
	m_CurrDrawClass_Donut.m_Data.Init();  
	m_CurrDrawClass_Donut.m_DataR.Init();
	m_CurrDrawClass_Ellipse.m_Data.Init(); 
	m_CurrDrawClass_Ellipse.m_DataR.Init();
	m_CurrDrawClass_Rect.m_Data.Init(); 
	m_CurrDrawClass_Rect.m_DataR.Init();
	m_CurrDrawClass_Arc.m_Data.Init();  
	m_CurrDrawClass_Arc.m_DataR.Init();
	m_CurrDrawClass_Polyline.m_Data.Init();   
	m_CurrDrawClass_Polyline.m_DataR.Init();
	m_CurrDrawClass_Polygon.m_Data.Init();  
	m_CurrDrawClass_Polygon.m_DataR.Init();
	m_CurrDrawClass_ArrowPolyline.m_Data.Init(); 
	m_CurrDrawClass_ArrowPolyline.m_DataR.Init();
	m_CurrDrawClass_Text.m_Data.Init();  
	m_CurrDrawClass_Text.m_DataR.Init();
	m_CurrDrawClass_Dimension.m_Data.Init(); 
	m_CurrDrawClass_Dimension.m_DataR.Init();
	m_CurrDrawClass_DimAngle.m_Data.Init();  
	m_CurrDrawClass_DimAngle.m_DataR.Init();
	m_CurrDrawClass_DimLeaderline.m_Data.Init();  
	m_CurrDrawClass_DimLeaderline.m_DataR.Init();
}

void CDrawEditBtn::CurrConvertDrawData(BOOL bCDC)
{
	m_CurrDrawClass_Point.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat_Point));
	m_CurrDrawClass_Line.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_ArrowLine.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Circle.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Donut.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Ellipse.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Rect.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Arc.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Polyline.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Polygon.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_ArrowPolyline.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Text.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrTextFormat));
	m_CurrDrawClass_Dimension.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDimFormat));
	m_CurrDrawClass_DimAngle.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDimFormat));
	m_CurrDrawClass_DimLeaderline.Convert(bCDC, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDimFormat));
}
void CDrawEditBtn::CurrDraw()
{
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat_Point, m_CurrDrawClass_Point.m_Data) ;
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Line.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_ArrowLine.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Circle.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Donut.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Ellipse.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Rect.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Arc.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Polyline.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_Polygon.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDrawFormat, m_CurrDrawClass_ArrowPolyline.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrTextFormat, m_CurrDrawClass_Text.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDimFormat,  m_CurrDrawClass_Dimension.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDimFormat,  m_CurrDrawClass_DimAngle.m_Data);
	m_DrawCtrl.DrawUnit(m_CurrDimFormat,  m_CurrDrawClass_DimLeaderline.m_Data);	
}

void CDrawEditBtn::AxisOrgPointConvertDrawData(BOOL bCDC)
{
	AxisOrgPointConvertDrawData(bCDC, &m_DrawCtrl, m_AxisOrgPoint, m_AxisOrgPointR);
}

void CDrawEditBtn::AxisOrgPointConvertDrawData(BOOL bCDC, CDrawCtrl* pDrawCtrl, CPoint& AxisOrgPoint, T_POINT& AxisOrgPointR)
{
	// ※주의 : m_AxisOrgPoint은 화면상의 좌표원점이 아니라 m_DrawCtrl.m_PaperFormat.OrgPoint부터의 상대거리임
	if(bCDC)  
	{		
		pDrawCtrl->m_DrawFunc.ConvertToPOINT(pDrawCtrl->m_PaperFormat, AxisOrgPointR, AxisOrgPoint);
		AxisOrgPoint.x = AxisOrgPoint.x - pDrawCtrl->m_PaperFormat.OrgPoint.x;
		AxisOrgPoint.y = AxisOrgPoint.y - pDrawCtrl->m_PaperFormat.OrgPoint.y;
	}
	else
	{
		CPoint AxisOrgPointTemp;
		AxisOrgPointTemp.x = AxisOrgPoint.x + pDrawCtrl->m_PaperFormat.OrgPoint.x;
		AxisOrgPointTemp.x = AxisOrgPoint.y + pDrawCtrl->m_PaperFormat.OrgPoint.y;
		pDrawCtrl->m_DrawFunc.ConvertToTPoint(pDrawCtrl->m_PaperFormat, AxisOrgPointTemp, AxisOrgPointR);
	}
}

void CDrawEditBtn::DrawAxisUnit(CDrawCtrl* pDrawCtrl, int nAxisMode, int nAxisSize, int nAxisWidth, BOOL bCapital, T_DRAW_FORMAT AxisDrawFormat,
								BOOL bCDC_AxisOrgPoint, CPoint AxisOrgPoint, T_POINT AxisOrgPointR, double dAxis_Vector1[2], double dAxis_Vector2[2])
{	
	BOOL bLocalAxis = (nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L);

	if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ ||
	   nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L)
	{
		// 원점 & 축방향
		CPoint OrgP;            
		OrgP.x = OrgP.y = 0;
		double dAxisVector1[2]; 
		dAxisVector1[0] = dAxisVector1[1] = 0.0;
		double dAxisVector2[2];
		dAxisVector2[0] = dAxisVector2[1] = 0.0;

		if(!bLocalAxis)
		{
			OrgP = pDrawCtrl->m_PaperFormat.OrgPoint;
			if(pDrawCtrl->m_DrawFunc.IsEMF()) OrgP.y *= -1;
			dAxisVector1[0] = 1.0;  
			dAxisVector1[1] = 0.0;
			dAxisVector2[0] = 0.0;  
			dAxisVector2[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0);
		}
		else 
		{
			if(!bCDC_AxisOrgPoint) AxisOrgPointConvertDrawData(TRUE, pDrawCtrl, AxisOrgPoint, AxisOrgPointR); //입력받은 원점 좌표가 실제좌표계인 경우 CDC좌표기준으로 바꾸어 준다.
			OrgP.x = pDrawCtrl->m_PaperFormat.OrgPoint.x + AxisOrgPoint.x;
			OrgP.y = pDrawCtrl->m_PaperFormat.OrgPoint.y + AxisOrgPoint.y;
			dAxisVector1[0] = dAxis_Vector1[0];
			dAxisVector1[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0) * dAxis_Vector1[1];
			dAxisVector2[0] = dAxis_Vector2[0]; 
			dAxisVector2[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0) * dAxis_Vector2[1];
		}

		if(m_DrawCtrl.m_nMirrorMode == EN_MIRRORVIEW_TYPE_HORIZONTAL) { dAxisVector1[0] *= -1.0;  dAxisVector2[0] *= -1.0; }
		if(m_DrawCtrl.m_nMirrorMode == EN_MIRRORVIEW_TYPE_VERTICAL  ) { dAxisVector1[1] *= -1.0;  dAxisVector2[1] *= -1.0; }


		int nCount = 0;

		T_LINE_GROUP lineGroup;
		T_LINE       lineUnit;
		lineGroup.List.RemoveAll();	
		//원점
		lineUnit.P1.x = OrgP.x - long(dAxisVector2[0] * (nAxisWidth / 4)); 
		lineUnit.P1.y = OrgP.y - long(dAxisVector2[1] * (nAxisWidth / 4));		
		lineUnit.P2.x = OrgP.x + long(dAxisVector2[0] * (nAxisWidth / 4)); 
		lineUnit.P2.y = OrgP.y + long(dAxisVector2[1] * (nAxisWidth / 4));
		lineGroup.List.Add(lineUnit);
		lineUnit.P1.x = OrgP.x - long(dAxisVector1[0] * (nAxisWidth / 4));  
		lineUnit.P1.y = OrgP.y - long(dAxisVector1[1] * (nAxisWidth / 4));		
		lineUnit.P2.x = OrgP.x + long(dAxisVector1[0] * (nAxisWidth / 4));  
		lineUnit.P2.y = OrgP.y + long(dAxisVector1[1] * (nAxisWidth / 4));
		lineGroup.List.Add(lineUnit);

		CPoint TextOrgP;
		double dDiagVectorTR[2]; 
		dDiagVectorTR[0] = 1.0;  
		dDiagVectorTR[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0);
		double dDiagVectorTL[2]; 
		dDiagVectorTL[0] = -1.0;
		dDiagVectorTL[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0);
		double dDownVector[2]; 
		dDownVector[0] = 0.0;
		dDownVector[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? -1.0 : 1.0);
		// X
		if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ ||
		   nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L)
		{
			TextOrgP.x = OrgP.x + long(dAxisVector1[0] * (nAxisSize-nAxisWidth / 2 - nAxisWidth / 4));
			TextOrgP.y = OrgP.y + long(dAxisVector1[1] * (nAxisSize-nAxisWidth / 2 - nAxisWidth / 4));			

			if(bLocalAxis)
			{
				if(pDrawCtrl->m_DrawFunc.IsEMF())
				{ 
					dDiagVectorTR[0] = dAxisVector1[0]        - dAxisVector1[1]; 
					dDiagVectorTR[1] = dAxisVector1[1]        + dAxisVector1[0];
					dDiagVectorTL[0] = (-1.0)*dAxisVector1[0] - dAxisVector1[1];  
					dDiagVectorTL[1] = (-1.0)*dAxisVector1[1] + dAxisVector1[0];
				}
				else
				{ 
					dDiagVectorTR[0] = dAxisVector1[0] +        dAxisVector1[1];   
					dDiagVectorTR[1] = dAxisVector1[1]        - dAxisVector1[0];
					dDiagVectorTL[0] = (-1.0)*dAxisVector1[0] + dAxisVector1[1];  
					dDiagVectorTL[1] = (-1.0)*dAxisVector1[1] - dAxisVector1[0];
				}
			}

			int halfText = (int)((double)nAxisWidth / 4.0 * (bCapital ? 1.0 : 0.5));

			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);    
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTR[0]);  
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTR[1]);
			lineGroup.List.Add(lineUnit);
			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);     
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTL[0]); 
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTL[1]);
			lineGroup.List.Add(lineUnit);
		}
		// Y
		if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ   ||
			nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L)
		{
			double dDiagVectorBR_Min[2];

			if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L)
			{
				TextOrgP.x = OrgP.x + long(dAxisVector1[0] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));
				TextOrgP.y = OrgP.y + long(dAxisVector1[1] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));	

				if(bLocalAxis)
				{
					if(pDrawCtrl->m_DrawFunc.IsEMF())
					{ 
						dDiagVectorTR[0] = dAxisVector1[0] - dAxisVector1[1];    
						dDiagVectorTR[1] = dAxisVector1[1] + dAxisVector1[0];
						dDiagVectorTL[0] = (-1.0) * dAxisVector1[0] - dAxisVector1[1];   
						dDiagVectorTL[1] = (-1.0) * dAxisVector1[1] + dAxisVector1[0];
						dDownVector[0] = dAxisVector1[1];                            
						dDownVector[1] = (-1.0) * dAxisVector1[0];
						dDiagVectorBR_Min[0] = (-1.0) * dAxisVector1[0] + (3.0) * dAxisVector1[1];   
						dDiagVectorBR_Min[1] = (-1.0) * dAxisVector1[1] - (3.0) * dAxisVector1[0]; 
					}
					else
					{ 
						dDiagVectorTR[0] = dAxisVector1[0] + dAxisVector1[1];  
						dDiagVectorTR[1] = dAxisVector1[1] - dAxisVector1[0];
						dDiagVectorTL[0] = (-1.0) * dAxisVector1[0] + dAxisVector1[1];  
						dDiagVectorTL[1] = (-1.0) * dAxisVector1[1] - dAxisVector1[0];
						dDownVector[0] = (-1.0) * dAxisVector1[1];                  
						dDownVector[1] = dAxisVector1[0];
						dDiagVectorBR_Min[0] = (-1.0) * dAxisVector1[0] - (3.0) * dAxisVector1[1]; 
						dDiagVectorBR_Min[1] = (-1.0) * dAxisVector1[1] + (3.0) * dAxisVector1[0]; 
					}
				}
			}
			else
			{
				TextOrgP.x = OrgP.x + long(dAxisVector2[0] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));
				TextOrgP.y = OrgP.y + long(dAxisVector2[1] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));	

				if(bLocalAxis)
				{
					if(pDrawCtrl->m_DrawFunc.IsEMF())
					{ 
						dDiagVectorTR[0] = dAxisVector2[0] + dAxisVector2[1]; 
						dDiagVectorTR[1] = dAxisVector2[1] - dAxisVector2[0]; 
						dDiagVectorTL[0] = dAxisVector2[0] - dAxisVector2[1]; 
						dDiagVectorTL[1] = dAxisVector2[1] + dAxisVector2[0];
						dDiagVectorBR_Min[0] = (-3.0)*dAxisVector2[0] - dAxisVector2[1];  
						dDiagVectorBR_Min[1] = (-3.0)*dAxisVector2[1] + dAxisVector2[0]; 
					}
					else
					{ 
						dDiagVectorTR[0] = dAxisVector2[0] - dAxisVector2[1];  
						dDiagVectorTR[1] = dAxisVector2[1] + dAxisVector2[0]; 
						dDiagVectorTL[0] = dAxisVector2[0] + dAxisVector2[1]; 
						dDiagVectorTL[1] = dAxisVector2[1] - dAxisVector2[0];
						dDiagVectorBR_Min[0] = (-3.0)*dAxisVector2[0] + dAxisVector2[1]; 
						dDiagVectorBR_Min[1] = (-3.0)*dAxisVector2[1] - dAxisVector2[0]; 
					}
					dDownVector[0] = (-1.0) * dAxisVector2[0];       
					dDownVector[1] = (-1.0) * dAxisVector2[1];
				}
			}

			int halfText = (int)((double)nAxisWidth / 4.0 * (bCapital ? 1.0 : 0.5));
			if(bCapital)
			{
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);    
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);			
				lineUnit.P2.x = TextOrgP.x;                                       
				lineUnit.P2.y = TextOrgP.y;
				lineGroup.List.Add(lineUnit);
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);   
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);
				lineUnit.P2.x = TextOrgP.x;                                      
				lineUnit.P2.y = TextOrgP.y;
				lineGroup.List.Add(lineUnit);
				lineUnit.P1.x = TextOrgP.x;                                       
				lineUnit.P1.y = TextOrgP.y;
				lineUnit.P2.x = TextOrgP.x + long(halfText * dDownVector[0]);  
				lineUnit.P2.y = TextOrgP.y + long(halfText * dDownVector[1]);
				lineGroup.List.Add(lineUnit);
			}
			else
			{
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);   
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);			
				lineUnit.P2.x = TextOrgP.x + long(halfText * dDownVector[0]);   
				lineUnit.P2.y = TextOrgP.y + long(halfText * dDownVector[1]);
				lineGroup.List.Add(lineUnit);
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);     
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);
				lineUnit.P2.x = TextOrgP.x + long(halfText * dDiagVectorBR_Min[0]); 
				lineUnit.P2.y = TextOrgP.y + long(halfText * dDiagVectorBR_Min[1]);
				lineGroup.List.Add(lineUnit);
			}
		}
		//Z
		if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ ||
		   nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L)
		{
			TextOrgP.x = OrgP.x + long(dAxisVector2[0] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));
			TextOrgP.y = OrgP.y + long(dAxisVector2[1] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));	

			if(bLocalAxis)
			{
				if(pDrawCtrl->m_DrawFunc.IsEMF())
				{ 
					dDiagVectorTR[0] = dAxisVector2[0] + dAxisVector2[1]; 
					dDiagVectorTR[1] = dAxisVector2[1] - dAxisVector2[0]; 
					dDiagVectorTL[0] = dAxisVector2[0] - dAxisVector2[1]; 
					dDiagVectorTL[1] = dAxisVector2[1] + dAxisVector2[0];
				}
				else
				{ 
					dDiagVectorTR[0] = dAxisVector2[0] - dAxisVector2[1]; 
					dDiagVectorTR[1] = dAxisVector2[1] + dAxisVector2[0]; 
					dDiagVectorTL[0] = dAxisVector2[0] + dAxisVector2[1]; 
					dDiagVectorTL[1] = dAxisVector2[1] - dAxisVector2[0];
				}
			}

			int halfText = (int)((double)nAxisWidth/4.0 * (bCapital ? 1.0 : 0.5));
			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);   
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);		
			lineUnit.P2.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);   
			lineUnit.P2.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);
			lineGroup.List.Add(lineUnit);		     
			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);   
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTR[0]);  
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTR[1]);
			lineGroup.List.Add(lineUnit);		     
			lineUnit.P1.x = TextOrgP.x - long(halfText * dDiagVectorTR[0]); 
			lineUnit.P1.y = TextOrgP.y - long(halfText * dDiagVectorTR[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTL[0]); 
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTL[1]);
			lineGroup.List.Add(lineUnit);		    
		}

		pDrawCtrl->DrawUnit(AxisDrawFormat, lineGroup);

		T_POLYGON_GROUP polygonGroup;
		T_POLYLINE      polygonUnit;
		CPoint          pointUnit;		
		polygonGroup.List.SetSize(2);
		// 1개축이 5각형으로 된 X,Y축 Polyline
		double dDirVector[2];
		double dCrossVector[2];
		dDirVector[0]   = dAxisVector1[0]; 
		dDirVector[1]   = dAxisVector1[1];
		dCrossVector[0] = dAxisVector1[1]; 
		dCrossVector[1] = dAxisVector1[0] * (-1.0);

		polygonUnit.arPoint.SetSize(7);
		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[0] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[1] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[2] = pointUnit;
		pointUnit.x = OrgP.x + long(nAxisSize * dDirVector[0]);                    
		pointUnit.y = OrgP.y + long(nAxisSize * dDirVector[1]);
		polygonUnit.arPoint[3] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[4] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[5] = pointUnit;
		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[6] = pointUnit;
		polygonGroup.List[0] = polygonUnit;

		dDirVector[0]   = dAxisVector2[0];  
		dDirVector[1]   = dAxisVector2[1];
		dCrossVector[0] = dAxisVector2[1]; 
		dCrossVector[1] = dAxisVector2[0] * (-1.0);

		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[0] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[1] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[2] = pointUnit;
		pointUnit.x = OrgP.x + long(nAxisSize * dDirVector[0]);                    
		pointUnit.y = OrgP.y + long(nAxisSize * dDirVector[1]);
		polygonUnit.arPoint[3] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[4] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[5] = pointUnit;
		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[6] = pointUnit;
		polygonGroup.List[1] = polygonUnit;

		pDrawCtrl->DrawUnit(AxisDrawFormat, polygonGroup);
	}	
}