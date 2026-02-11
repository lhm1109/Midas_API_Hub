#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"

#include "DrawData.h"
#include "DrawFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDrawFunc

// DrawHatching ////////////////////////////////////

void CDrawFunc::DrawHatching(HDC DC, T_DRAW_FORMAT Format, T_HATCHING_D& dgnHatching)
{ 
	DrawHatching(DC, Format.hatchingType, Format.hatchingColor, Format.bHatching, Format.HatchingWidth, dgnHatching);

}

void CDrawFunc::MakeHatchingData(T_DRAW_FORMAT drawFormat, T_LINE Rect_In, T_HATCHING_D& Hatching_Out)
{
	Hatching_Out.Init();
	EN_HATCHING_TYPE nHType = drawFormat.hatchingType;

	if(nHType == EN_HATCHING_TYPE_NONE) return;
	if(drawFormat.HatchingSpace <= 0) return;

	int nXmax = max(Rect_In.P1.x, Rect_In.P2.x);
	int nXmin = min(Rect_In.P1.x, Rect_In.P2.x);
	int nYmax = max(Rect_In.P1.y, Rect_In.P2.y);
	int nYmin = min(Rect_In.P1.y, Rect_In.P2.y);


	BOOL bPoint, bLine, bZigZag; 
	int  nZigZagType;
	int  nLineDir; // 선의방향(1:수평, 2:수직, 3:45도 우측에서 아래로, 4:45도 좌측에서 아래로, 5:십자가, 6:X자, 11:수평원통, 12:수직원통, 0:ETC)
	int  nDLine, nLineStartPosi, nPointStartPosi, nLineSpace, nPointSpace, nPointSpace_Sub;
	T_LINE PointSize;
	POINT pointD;

	GetHatchingSubData(nHType, drawFormat.HatchingSpace, bPoint, bLine, nDLine, bZigZag, nZigZagType, nLineDir,  
		nLineStartPosi, nPointStartPosi, nLineSpace, nPointSpace, nPointSpace_Sub, PointSize);

	CMap<int, int&, POINT, POINT&>				PointList;
	CMap<int, int&, T_LINE, T_LINE&>			LineList;
	CMap<int, int&, T_LINE, T_LINE&>			RectList;
	CMap<int, int&, T_CIRCLE, T_CIRCLE&>		CircleList;
	CMap<int, int&, T_ARC, T_ARC&>				ArcList;
	CMap<int, int&, T_POLYLINE, T_POLYLINE&>	PolygonList;
	CMap<int, int&, T_POLYLINE, T_POLYLINE&>	PolylineList;

	int i, j, nSize, nSizeX, nSizeY, nSpaceX, nSpaceY, nStartX;	
	int nPointCount    = 0;
	int nLineCount     = 0;	
	int nRectCount     = 0;
	int nCircleCount   = 0;
	int nArcCount      = 0;
	int nPolygonCount  = 0;
	int nPolylineCount = 0;
	if(bPoint)
	{
		nSizeX = (int)((abs(Rect_In.P1.x - Rect_In.P2.x) - nPointStartPosi) / nPointSpace) + 1;
		if(abs(Rect_In.P1.x - Rect_In.P2.x) <= nPointStartPosi) nSizeX = 0;
		nSizeY = (int)(abs(Rect_In.P1.y - Rect_In.P2.y) / nPointSpace_Sub) + 1;

		if(nHType==EN_HATCHING_TYPE_RECTDOTS || nHType==EN_HATCHING_TYPE_CIRCLE || nHType==EN_HATCHING_TYPE_PLUS || nHType==EN_HATCHING_TYPE_MINUS || 
			nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL2 || EN_HATCHING_TYPE_GRAVEL_DOTS || 
			nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL || nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL || 
			nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL_S || 
			nHType==EN_HATCHING_TYPE_BDIAGONAL3_FDIAGONAL || nHType==EN_HATCHING_TYPE_FDIAGONAL3_BDIAGONAL)
		{ 
			nSizeX++; 
			nSizeY++; 
		}

		POINT StartPoint, CurrPoint;
		StartPoint.x = nXmin + nPointStartPosi;
		StartPoint.y = (m_bEMF) ? nYmax : nYmin;
		nSpaceX = nPointSpace;
		nSpaceY = ((m_bEMF) ? -1 : 1) * nPointSpace_Sub;

		nZigZagType = max(2, nZigZagType);
		BOOL bZigPlus = (nZigZagType<10);
		int  nZigDiv  = max(2, nZigZagType%10);

		for(i = 0; i < nSizeX ; i++)
		{
			CurrPoint.x =  StartPoint.x + i*nSpaceX;
			for(j = 0; j < nSizeY ; j++)
			{
				if(bZigZag) 
				{
					if(nZigZagType <= 2)
					{ 
						if(i % 2 != j % 2) continue; 
					}
					else
					{
						if(bZigPlus)
						{
							if((i+j)%nZigDiv != 0) continue;
						}
						else
						{
							if((i-j)%nZigDiv != 0) continue;
						}
					}
				}
				CurrPoint.y =  StartPoint.y + j*nSpaceY;
				PointList.SetAt(nPointCount, CurrPoint);
				nPointCount++;
			}
		}
	}

	if(bLine)
	{		
		if(nLineDir == 1 || nLineDir == 5)
		{
			// 수평선											
			nSizeY = (int)((abs(Rect_In.P1.y - Rect_In.P2.y) - nLineStartPosi) / nLineSpace) + 1;
			if(abs(Rect_In.P1.y - Rect_In.P2.y) <= nLineStartPosi) nSizeY = 0;

			POINT StartPoint, CurrPoint;
			T_LINE CurrLine;
			StartPoint.x = nXmin;
			StartPoint.y = (m_bEMF) ? nYmax - nLineStartPosi : nYmin + nLineStartPosi;

			int nCurrLineSpace = (m_bEMF) ? -nLineSpace : nLineSpace;
			CurrPoint.x = StartPoint.x;
			if(nDLine == 1)
			{	
				for(i = 0; i < nSizeY; i++)
				{
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 1, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++; 
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i = 0; i < nSizeY ; i++)
				{
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace - nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 1, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++; 
					}
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 1, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
				}
			}
			else if(nDLine == 3)
			{	
				for(i = 0; i < nSizeY ; i++)
				{
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 1, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace - nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 1, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine);
						nLineCount++;
					}
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 1, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++; 
					}
				}
			}
		}

		if(nLineDir == 2 || nLineDir == 5)
		{
			// 수직선	
			nSizeX = (int)((abs(Rect_In.P1.x - Rect_In.P2.x) - nLineStartPosi) / nLineSpace) + 1;
			if(abs(Rect_In.P1.x - Rect_In.P2.x) <= nLineStartPosi) nSizeX = 0;
			POINT StartPoint, CurrPoint;
			T_LINE CurrLine;
			StartPoint.x = nXmin + nLineStartPosi;
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			if(nDLine == 1)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;

					if(GetCrossLine_Rect(Rect_In, CurrPoint, 2, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine);
						nLineCount++;
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;

					if(GetCrossLine_Rect(Rect_In, CurrPoint, 2, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 2, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
				}
			}		
			else if(nDLine == 3)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 2, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 2, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 2, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine);
						nLineCount++; 
					}
				}
			}		
		}

		if(nLineDir == 3 || nLineDir == 6)
		{
			// 45도 우측에서 아래로 빗금			
			nSizeX = (int)((abs(Rect_In.P1.x - Rect_In.P2.x)+abs(Rect_In.P1.y - Rect_In.P2.y) - nLineStartPosi) / nLineSpace) + 1;			
			if((abs(Rect_In.P1.x - Rect_In.P2.x)+abs(Rect_In.P1.y - Rect_In.P2.y)) <= nLineStartPosi) nSizeX = 0;
			POINT StartPoint, CurrPoint;
			T_LINE CurrLine;
			StartPoint.x = nXmin + nLineStartPosi;
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			if(nDLine == 1)
			{	
				for(i = 0; i < nSizeX; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 3, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++; 
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i = 0; i < nSizeX; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 3, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine);
						nLineCount++; 
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 3, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine);
						nLineCount++;
					}
				}
			}
			else if(nDLine == 3)
			{	
				for(i = 0 ; i < nSizeX; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 3, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 3, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++; 
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 3, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++; 
					}
				}
			}
		}

		if(nLineDir == 4 || nLineDir == 6)
		{
			// 45도 좌측에서 아래로 빗금
			nStartX = (int)((abs(Rect_In.P1.y - Rect_In.P2.y) + nLineStartPosi) / nLineSpace);
			nSizeX  = (int)((abs(Rect_In.P1.x - Rect_In.P2.x) - nLineStartPosi) / nLineSpace) + 1;			
			if(abs(Rect_In.P1.x - Rect_In.P2.x) <= nLineStartPosi) nSizeX = 0;
			POINT StartPoint, CurrPoint;
			T_LINE CurrLine;
			StartPoint.x = nXmin + nLineStartPosi;
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			if(nDLine == 1)
			{	
				for(i = -nStartX; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 4, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine);  
						nLineCount++;
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i = -nStartX; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 4, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 4, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++; 
					}
				}
			}
			else if(nDLine == 3)
			{	
				for(i = -nStartX; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 4, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine);  
						nLineCount++; 
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 4, CurrLine, drawFormat.Width))
					{ 
						LineList.SetAt(nLineCount, CurrLine);
						nLineCount++;
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Rect(Rect_In, CurrPoint, 4, CurrLine, drawFormat.Width))
					{
						LineList.SetAt(nLineCount, CurrLine); 
						nLineCount++;
					}
				}
			}
		}
		if(nLineDir == 11)
		{
			// 수평선(원주)											
			nSizeY = (int)((abs(Rect_In.P1.y - Rect_In.P2.y)) / nLineSpace) + 1;

			POINT StartPoint, CurrPoint;
			T_LINE CurrLine;
			StartPoint.x = nXmin;
			StartPoint.y = (int)((nYmax + nYmin) / 2);

			CurrPoint.x = StartPoint.x;
			int nCurrFactor = (m_bEMF) ? -(nYmax - nYmin) : (nYmax - nYmin);
			double dAngleSpace = (nSizeY - 1) == 0 ? 0.0 : 1.0 / (nSizeY - 1) * 180.0 * CMathFunc::m_trrad;

			double dAngleFactor;
			BOOL   bSqrt = (nDLine >= 10);
			for(i = 0; i < nSizeY ; i++)
			{
				dAngleFactor = cos(i*dAngleSpace);
				if(bSqrt) dAngleFactor = sqrt(fabs(dAngleFactor)) * (dAngleFactor >= 0.0 ? 1.0 : -1.0);

				CurrPoint.y = StartPoint.y + (int)(nCurrFactor / 2.0 * dAngleFactor);

				if(GetCrossLine_Rect(Rect_In, CurrPoint, 1, CurrLine, drawFormat.Width))
				{ 
					LineList.SetAt(nLineCount, CurrLine); 
					nLineCount++;
				}
			}
		}

		if(nLineDir == 12)
		{
			// 수직선(원주)		
			nSizeX = (int)((abs(Rect_In.P1.x - Rect_In.P2.x)) / nLineSpace) + 1;

			POINT StartPoint, CurrPoint;
			T_LINE CurrLine;
			StartPoint.x = (int)((nXmax + nXmin) / 2);
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			int nCurrFactor = (nXmax - nXmin);
			double dAngleSpace = (nSizeX-1) == 0 ? 0.0 : 1.0 / (nSizeX - 1) * 180.0 * CMathFunc::m_trrad;

			double dAngleFactor;
			BOOL   bSqrt = (nDLine >= 10);

			for(i = 0; i < nSizeX; i++)
			{
				dAngleFactor = cos(i * dAngleSpace);
				if(bSqrt) dAngleFactor = sqrt(fabs(dAngleFactor)) * (dAngleFactor>=0.0 ? 1.0 : -1.0);

				CurrPoint.x = StartPoint.x + (int)(nCurrFactor / 2.0 * dAngleFactor);
				if(GetCrossLine_Rect(Rect_In, CurrPoint, 2, CurrLine, drawFormat.Width))
				{ 
					LineList.SetAt(nLineCount, CurrLine);
					nLineCount++;
				}
			}			
		}
	}


	int Index;
	POSITION Pos;
	// Point
	if(nHType == EN_HATCHING_TYPE_DOTS)
	{
		// Dot
		Index = 0;		
		Pos = PointList.GetStartPosition();

		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);
			if(pointD.x > nXmax || pointD.x < nXmin || pointD.y > nYmax || pointD.y < nYmin)
			{
				PointList.RemoveKey(Index);
			}
		}
	}
	else if(nHType == EN_HATCHING_TYPE_DOTS_B || nHType == EN_HATCHING_TYPE_CIRCLE || nHType == EN_HATCHING_TYPE_DUPLEX_CIRCLE || nHType == EN_HATCHING_TYPE_CIRCLE_EQUAL)
	{
		// Circle	
		T_CIRCLE CurrCircle;
		nrT_ARC arArc;

		int nRadius = max(1, (int)(abs(PointSize.P1.y - PointSize.P2.y) / 2));
		CurrCircle.Radius = nRadius;

		Index = 0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);

			if(pointD.x >= nXmin+nRadius && pointD.x <= nXmax-nRadius && pointD.y >= nYmin+nRadius && pointD.y <= nYmax-nRadius)
			{
				CurrCircle.CenterP = pointD;
				CircleList.SetAt(nCircleCount, CurrCircle); 
				nCircleCount++;
			}
			else if(pointD.x > nXmin-nRadius && pointD.x < nXmax+nRadius && pointD.y > nYmin-nRadius && pointD.y < nYmax+nRadius)			
			{
				if(GetCrossArc_Rect(Rect_In, pointD, nRadius, arArc, drawFormat.Width))
				{ 
					for(auto i = 0; i < arArc.GetSize(); i++) 
					{ 
						ArcList.SetAt(nArcCount, arArc[i]);
						nArcCount++; 
					}
				}
			}	
		}
		if(nHType == EN_HATCHING_TYPE_DUPLEX_CIRCLE)
		{
			nRadius /= 2;
			CurrCircle.Radius = nRadius;
			Index = 0;	
			Pos = PointList.GetStartPosition();

			while(Pos)
			{		
				PointList.GetNextAssoc(Pos, Index, pointD);
				pointD.x += nPointSpace;

				if(pointD.x >= nXmin+nRadius && pointD.x <= nXmax-nRadius && pointD.y >= nYmin+nRadius && pointD.y <= nYmax-nRadius)
				{					
					CurrCircle.CenterP = pointD;
					CircleList.SetAt(nCircleCount, CurrCircle);
					nCircleCount++;
				}
				else if(pointD.x > nXmin-nRadius && pointD.x < nXmax+nRadius && pointD.y > nYmin-nRadius && pointD.y < nYmax+nRadius)			
				{
					if(GetCrossArc_Rect(Rect_In, pointD, nRadius, arArc, drawFormat.Width))
					{ 
						for(auto i = 0; i < arArc.GetSize(); i++) 
						{ 
							ArcList.SetAt(nArcCount, arArc[i]); 
							nArcCount++;
						}
					}
				}	
			}
		}
		else if(nHType == EN_HATCHING_TYPE_CIRCLE_EQUAL)
		{			
			nRadius /= 2;
			T_LINE CurrLineH;
			Index=0;		Pos = PointList.GetStartPosition();
			while(Pos)
			{		
				PointList.GetNextAssoc(Pos, Index, pointD);
				pointD.x += nPointSpace;

				CurrLineH.P1.x = max(nXmin, pointD.x-nRadius);
				CurrLineH.P2.x = min(nXmax-1, pointD.x+nRadius);
				CurrLineH.P1.y = CurrLineH.P2.y = pointD.y - nRadius;				
				if((CurrLineH.P2.x-CurrLineH.P1.x) > 0 && (CurrLineH.P1.y > nYmin && CurrLineH.P1.y < nYmax))
				{
					LineList.SetAt(nLineCount, CurrLineH);  nLineCount++;
				}				

				CurrLineH.P1.y = CurrLineH.P2.y = pointD.y + nRadius;				
				if((CurrLineH.P2.x-CurrLineH.P1.x) > 0 && (CurrLineH.P1.y > nYmin && CurrLineH.P1.y < nYmax))
				{
					LineList.SetAt(nLineCount, CurrLineH);  nLineCount++;
				}				
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType==EN_HATCHING_TYPE_RECTDOTS          || nHType==EN_HATCHING_TYPE_CHECKER             || 
		nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL   || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL   ||
		nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL    || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL  ||
		nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL_S || 
		nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL_S  || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL_S)
	{
		// Rect
		int nCurrXmax = max(PointSize.P1.x, PointSize.P2.x);
		int nCurrXmin = min(PointSize.P1.x, PointSize.P2.x);
		int nCurrYmax = max(PointSize.P1.y, PointSize.P2.y);
		int nCurrYmin = min(PointSize.P1.y, PointSize.P2.y);
		T_LINE CurrLine;

		Index=0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);
			CurrLine.P1.x = min(nXmax, pointD.x+nCurrXmax);
			CurrLine.P1.y = min(nYmax, pointD.y+nCurrYmax);
			CurrLine.P2.x = max(nXmin, pointD.x+nCurrXmin);
			CurrLine.P2.y = max(nYmin, pointD.y+nCurrYmin);

			if((CurrLine.P1.x-CurrLine.P2.x) > 0 && (CurrLine.P1.y-CurrLine.P2.y) > 0)
			{
				RectList.SetAt(nRectCount, CurrLine);  nRectCount++;
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_PLUS || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL2 || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL3)
	{
		// '+'
		int nCurrXmax = max(PointSize.P1.x, PointSize.P2.x);
		int nCurrXmin = min(PointSize.P1.x, PointSize.P2.x);
		int nCurrYmax = max(PointSize.P1.y, PointSize.P2.y);
		int nCurrYmin = min(PointSize.P1.y, PointSize.P2.y);
		T_LINE CurrLineV, CurrLineH;

		Index = 0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);			
			CurrLineH.P1.x = max(nXmin, pointD.x+nCurrXmin);
			CurrLineH.P2.x = min(nXmax-1, pointD.x+nCurrXmax);
			CurrLineH.P1.y = CurrLineH.P2.y = pointD.y;

			CurrLineV.P1.x = CurrLineV.P2.x = pointD.x;
			CurrLineV.P1.y = m_bEMF ? min(nYmax, pointD.y+nCurrYmax)   : max(nYmin, pointD.y+nCurrYmin);
			CurrLineV.P2.y = m_bEMF ? max(nYmin+1, pointD.y+nCurrYmin) : min(nYmax-1, pointD.y+nCurrYmax);

			if((CurrLineH.P2.x-CurrLineH.P1.x) > 0 && (CurrLineH.P1.y > nYmin && CurrLineH.P1.y < nYmax))
			{
				LineList.SetAt(nLineCount, CurrLineH);  nLineCount++;
			}
			if((m_bEMF ? (CurrLineV.P1.y-CurrLineV.P2.y) : (CurrLineV.P2.y-CurrLineV.P1.y)) > 0 && (CurrLineV.P1.x > nXmin && CurrLineV.P1.x < nXmax))
			{
				LineList.SetAt(nLineCount, CurrLineV);  nLineCount++;
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_MINUS)
	{
		// '-'		
		int nCurrXmax = max(PointSize.P1.x, PointSize.P2.x);
		int nCurrXmin = min(PointSize.P1.x, PointSize.P2.x);
		T_LINE CurrLineH;

		Index=0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);			
			CurrLineH.P1.x = max(nXmin, pointD.x+nCurrXmin);
			CurrLineH.P2.x = min(nXmax-1, pointD.x+nCurrXmax);
			CurrLineH.P1.y = CurrLineH.P2.y = pointD.y;

			if((CurrLineH.P2.x-CurrLineH.P1.x) > 0 && (CurrLineH.P1.y > nYmin && CurrLineH.P1.y < nYmax))
			{
				LineList.SetAt(nLineCount, CurrLineH);  nLineCount++;
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_BRICK)
	{
		// '｜'		
		int nCurrYmax = max(PointSize.P1.y, PointSize.P2.y);
		int nCurrYmin = min(PointSize.P1.y, PointSize.P2.y);
		T_LINE CurrLineV;

		Index=0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);	
			CurrLineV.P1.x = CurrLineV.P2.x = pointD.x;
			CurrLineV.P1.y = m_bEMF ? min(nYmax, pointD.y+nCurrYmax)   : max(nYmin, pointD.y+nCurrYmin);
			CurrLineV.P2.y = m_bEMF ? max(nYmin+1, pointD.y+nCurrYmin) : min(nYmax-1, pointD.y+nCurrYmax);

			if((m_bEMF ? (CurrLineV.P1.y-CurrLineV.P2.y) : (CurrLineV.P2.y-CurrLineV.P1.y)) > 0 && (CurrLineV.P1.x > nXmin && CurrLineV.P1.x < nXmax))
			{
				LineList.SetAt(nLineCount, CurrLineV);  nLineCount++;
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_BDIAGONAL3_FDIAGONAL || nHType == EN_HATCHING_TYPE_FDIAGONAL3_BDIAGONAL)
	{
		T_LINE CurrLine;
		double dXslope = (PointSize.P2.x == PointSize.P1.x) ? 0.0 : (double)(PointSize.P2.y-PointSize.P1.y)/(double)(PointSize.P2.x-PointSize.P1.x);
		double dYslope = (PointSize.P2.y == PointSize.P1.y) ? 0.0 : (double)(PointSize.P2.x-PointSize.P1.x)/(double)(PointSize.P2.y-PointSize.P1.y);

		int nGapXmax, nGapYmax, nGapYmin; 
		nGapXmax = nGapYmax = nGapYmin = 0;
		if(!m_bRectTrPolyDraw)
		{ 
			nGapXmax = -1 * max(1, drawFormat.Width/2);  
			if(m_bEMF) nGapYmin = max(1, drawFormat.Width/2); 
			else       nGapYmax = -1 * max(1, drawFormat.Width/2);
		}

		Index = 0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);			

			CurrLine.P1.x = pointD.x + PointSize.P1.x;
			CurrLine.P1.y = pointD.y + PointSize.P1.y;
			CurrLine.P2.x = pointD.x + PointSize.P2.x;
			CurrLine.P2.y = pointD.y + PointSize.P2.y;

			if(CurrLine.P1.x > nXmax+nGapXmax) { CurrLine.P1.y -= (int)(dXslope*(CurrLine.P1.x-(nXmax+nGapXmax)));  CurrLine.P1.x = nXmax+nGapXmax;}
			if(CurrLine.P1.y > nYmax+nGapYmax) { CurrLine.P1.x -= (int)(dYslope*(CurrLine.P1.y-(nYmax+nGapYmax)));  CurrLine.P1.y = nYmax+nGapYmax;}
			if(CurrLine.P1.x < nXmin)			  { CurrLine.P1.y -= (int)(dXslope*(CurrLine.P1.x-(nXmin)));			CurrLine.P1.x = nXmin;}
			if(CurrLine.P1.y < nYmin+nGapYmin) { CurrLine.P1.x -= (int)(dYslope*(CurrLine.P1.y-(nYmin+nGapYmin)));  CurrLine.P1.y = nYmin+nGapYmin;}

			if(CurrLine.P2.x > nXmax+nGapXmax) { CurrLine.P2.y -= (int)(dXslope*(CurrLine.P2.x-(nXmax+nGapXmax)));  CurrLine.P2.x = nXmax+nGapXmax;}
			if(CurrLine.P2.y > nYmax+nGapYmax) { CurrLine.P2.x -= (int)(dYslope*(CurrLine.P2.y-(nYmax+nGapYmax)));  CurrLine.P2.y = nYmax+nGapYmax;}
			if(CurrLine.P2.x < nXmin)			  { CurrLine.P2.y -= (int)(dXslope*(CurrLine.P2.x-(nXmin)));			CurrLine.P2.x = nXmin;}
			if(CurrLine.P2.y < nYmin+nGapYmin) { CurrLine.P2.x -= (int)(dYslope*(CurrLine.P2.y-(nYmin+nGapYmin)));  CurrLine.P2.y = nYmin+nGapYmin;}

			if((CurrLine.P1.x == CurrLine.P2.x) &&  (CurrLine.P1.y == CurrLine.P2.y)) continue;				

			if(max(CurrLine.P1.x, CurrLine.P2.x) > nXmax) continue;
			if(max(CurrLine.P1.y, CurrLine.P2.y) > nYmax) continue;
			if(min(CurrLine.P1.x, CurrLine.P2.x) < nXmin) continue;
			if(min(CurrLine.P1.y, CurrLine.P2.y) < nYmin) continue;

			LineList.SetAt(nLineCount, CurrLine);  nLineCount++;			
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_GRAVEL || nHType == EN_HATCHING_TYPE_GRAVEL_DOTS)
	{		
		nrT_POLYLINE arGravelPoly;
		nrPOINT		arGravelPoint;
		nrT_POLYLINE arCurrGravelPoly;
		nrT_POLYLINE arCurrPolygon;
		nrT_POLYLINE arCurrPolyline;
		T_POLYLINE CurrPoly;
		POINT CurrPoint;
		int nGravelXmax, nGravelXmin, nGravelYmax, nGravelYmin;
		int nCurrXmax, nCurrXmin, nCurrYmax, nCurrYmin;
		GetGravelData(nPointSpace, nPointSpace_Sub, arGravelPoly, arGravelPoint, nGravelXmax, nGravelXmin, nGravelYmax, nGravelYmin);

		nrPOINT arPointList;
		nSize = (int)PointList.GetCount();
		arPointList.SetSize(nSize);
		Index=0;		
		Pos = PointList.GetStartPosition();
		int nCount=0;
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);	
			arPointList[nCount] = pointD;
			nCount++;		
		}
		PointList.RemoveAll();
		nPointCount = 0;

		int nPolygonSize, nPointSize, nCurrPolygonSize, nCurrPolylineSize;
		nPolygonSize = arGravelPoly.GetSize();
		nPointSize   = arGravelPoint.GetSize();

		for(i = 0; i < nSize; i++)
		{
			POINT& pointD = arPointList[i];

			nCurrXmax = pointD.x + nGravelXmax;
			nCurrXmin = pointD.x + nGravelXmin;
			nCurrYmax = pointD.y + nGravelYmax;
			nCurrYmin = pointD.y + nGravelYmin;

			if(nXmax >= nCurrXmax && nXmin <= nCurrXmin && nYmax >= nCurrYmax && nYmin <= nCurrYmin)
			{
				for(j = 0; j < nPolygonSize; j++)
				{ 
					CurrPoly = arGravelPoly[j];
					CurrPoly.Move(pointD.x, pointD.y);
					PolygonList.SetAt(nPolygonCount, CurrPoly); 
					nPolygonCount++;
				}
			}
			else 
			{
				arCurrGravelPoly.RemoveAll();
				arCurrGravelPoly.SetSize(nPolygonSize);

				for(j = 0; j < nPolygonSize; j++)
				{ 
					CurrPoly = arGravelPoly[j];
					CurrPoly.Move(pointD.x, pointD.y);
					arCurrGravelPoly[j] = CurrPoly;				
				}

				if(GetCrossPolyline_Rect(Rect_In, arCurrGravelPoly, arCurrPolygon, arCurrPolyline, drawFormat.Width))
				{
					nCurrPolygonSize = arCurrPolygon.GetSize();

					for(j = 0; j < nCurrPolygonSize; j++)
					{
						PolygonList[nPolygonCount] = arCurrPolygon[j]; 
						nPolygonCount++;
					}

					nCurrPolylineSize = arCurrPolyline.GetSize();

					for(j = 0; j < nCurrPolylineSize; j++)
					{
						PolylineList[nPolylineCount] = arCurrPolyline[j]; 
						nPolylineCount++;
					}
				}
			}
		}
		if(nHType == EN_HATCHING_TYPE_GRAVEL_DOTS)
		{
			T_CIRCLE CurrCircle;
			nrT_ARC arArc;

			int nRadius = max(1, (int)(drawFormat.HatchingSpace / 15));
			CurrCircle.Radius = nRadius;
			int nArcSize, k;

			for(i = 0; i < nSize; i++)
			{
				POINT& pointD = arPointList[i];

				for(j = 0; j < nPointSize ; j++)
				{
					CurrPoint = arGravelPoint[j];
					CurrPoint.x += pointD.x;
					CurrPoint.y += pointD.y;
					if(CurrPoint.x >= nXmin+nRadius && CurrPoint.x <= nXmax-nRadius && CurrPoint.y >= nYmin+nRadius && CurrPoint.y <= nYmax-nRadius)
					{
						CurrCircle.CenterP = CurrPoint;
						CircleList.SetAt(nCircleCount, CurrCircle);  nCircleCount++;
					}
					else if(CurrPoint.x > nXmin-nRadius && CurrPoint.x < nXmax+nRadius && CurrPoint.y > nYmin-nRadius && CurrPoint.y < nYmax+nRadius)			
					{
						if(GetCrossArc_Rect(Rect_In, CurrPoint, nRadius, arArc, drawFormat.Width))
						{ 
							nArcSize = arArc.GetSize();

							for(k = 0; k < nArcSize; k++)
							{ 
								ArcList.SetAt(nArcCount, arArc[k]);
								nArcCount++; 
							}
						}
					}	
				}
			}
		}
	}

	// Hatching정보 입력
	T_LINE     lineD;
	T_CIRCLE   CircleD;
	T_ARC      ArcD;
	T_POLYLINE PolyD;
	// - Point
	nSize = (int)PointList.GetCount();
	Hatching_Out.arPoint.SetSize(nSize);
	Index=0;		
	Pos = PointList.GetStartPosition();
	nPointCount = 0;

	while(Pos)
	{		
		PointList.GetNextAssoc(Pos, Index, pointD);	
		Hatching_Out.arPoint[nPointCount] = pointD;
		nPointCount++;
	}
	// - Line
	nSize = (int)LineList.GetCount();
	Hatching_Out.arLine.SetSize(nSize);
	Index=0;		
	Pos = LineList.GetStartPosition();
	nLineCount = 0;
	while(Pos)
	{		
		LineList.GetNextAssoc(Pos, Index, lineD);	
		Hatching_Out.arLine[nLineCount] = lineD;
		nLineCount++;
	}
	// - Rect
	nSize = (int)RectList.GetCount();
	Hatching_Out.arRect.SetSize(nSize);
	Index=0;		
	Pos = RectList.GetStartPosition();
	nRectCount = 0;
	while(Pos)
	{		
		RectList.GetNextAssoc(Pos, Index, lineD);	
		Hatching_Out.arRect[nRectCount] = lineD;
		nRectCount++;
	}
	// - Circle
	nSize = (int)CircleList.GetCount();
	Hatching_Out.arCircle.SetSize(nSize);
	Index=0;		
	Pos = CircleList.GetStartPosition();
	nCircleCount = 0;
	while(Pos)
	{		
		CircleList.GetNextAssoc(Pos, Index, CircleD);	
		Hatching_Out.arCircle[nCircleCount] = CircleD;
		nCircleCount++;
	}
	// - Arc
	nSize = (int)ArcList.GetCount();
	Hatching_Out.arArc.SetSize(nSize);
	Index=0;	
	Pos = ArcList.GetStartPosition();
	nArcCount = 0;
	while(Pos)
	{		
		ArcList.GetNextAssoc(Pos, Index, ArcD);	
		Hatching_Out.arArc[nArcCount] = ArcD;
		nArcCount++;
	}
	// - Polygon
	nSize = (int)PolygonList.GetCount();
	Hatching_Out.arPolygon.SetSize(nSize);
	Index=0;	
	Pos = PolygonList.GetStartPosition();
	nPolygonCount = 0;
	while(Pos)
	{		
		PolygonList.GetNextAssoc(Pos, Index, PolyD);	
		Hatching_Out.arPolygon[nPolygonCount] = PolyD;
		nPolygonCount++;
	}
	// - Polyline
	nSize = (int)PolylineList.GetCount();
	Hatching_Out.arPolyline.SetSize(nSize);
	Index=0;	
	Pos = PolylineList.GetStartPosition();
	nPolylineCount = 0;
	while(Pos)
	{		
		PolylineList.GetNextAssoc(Pos, Index, PolyD);	
		Hatching_Out.arPolyline[nPolylineCount] = PolyD;
		nPolylineCount++;
	}
}

void CDrawFunc::MakeHatchingData(T_DRAW_FORMAT drawFormat, T_POLYLINE Polyline_In, T_HATCHING_D& Hatching_Out)
{	
	Hatching_Out.Init();
	ASSERT(0);   // Check!!! SHIN 현재 Test중임
	return;

	EN_HATCHING_TYPE nHType = drawFormat.hatchingType;
	if(nHType==EN_HATCHING_TYPE_NONE) return;
	if(drawFormat.HatchingSpace <= 0) return;

	BOOL bPoint, bLine, bZigZag; 
	int  nZigZagType;
	int  nLineDir; // 선의방향(1:수평, 2:수직, 3:45도 우측에서 아래로, 4:45도 좌측에서 아래로, 5:십자가, 6:X자, 11:수평원통, 12:수직원통, 0:ETC)
	int  nDLine, nLineStartPosi, nPointStartPosi, nLineSpace, nPointSpace, nPointSpace_Sub;
	T_LINE PointSize;
	POINT pointD;

	GetHatchingSubData(nHType, drawFormat.HatchingSpace, bPoint, bLine, nDLine, bZigZag, nZigZagType, nLineDir, nLineStartPosi, nPointStartPosi, nLineSpace, nPointSpace, nPointSpace_Sub, PointSize);

	CMap<int, int&, POINT, POINT&>				PointList;
	CMap<int, int&, T_LINE, T_LINE&>			LineList;
	CMap<int, int&, T_LINE, T_LINE&>			RectList;
	CMap<int, int&, T_CIRCLE, T_CIRCLE&>		CircleList;
	CMap<int, int&, T_ARC, T_ARC&>				ArcList;
	CMap<int, int&, T_POLYLINE, T_POLYLINE&>	PolygonList;
	CMap<int, int&, T_POLYLINE, T_POLYLINE&>	PolylineList;

	int i, j, nSize, nSizeX, nSizeY, nSpaceX, nSpaceY, nStartX;	
	int nPointCount    = 0;
	int nLineCount     = 0;	
	int nRectCount     = 0;
	int nCircleCount   = 0;
	int nArcCount      = 0;
	int nPolygonCount  = 0;
	int nPolylineCount = 0;

	int nXmax, nXmin, nYmax, nYmin;
	nSize = Polyline_In.arPoint.GetSize();
	for(i = 0; i < nSize ; i++)
	{
		if(i==0) 
		{ 
			nXmax = nXmin = Polyline_In.arPoint[i].x;
			nYmax = nYmin = Polyline_In.arPoint[i].y;
		}
		else 
		{
			nXmax = max(nXmax, Polyline_In.arPoint[i].x);
			nXmin = min(nXmin, Polyline_In.arPoint[i].x);
			nYmax = max(nYmax, Polyline_In.arPoint[i].y);
			nYmin = min(nYmin, Polyline_In.arPoint[i].y);
		}
	}

	if(bPoint)
	{
		nSizeX = (int)(((nXmax - nXmin) - nPointStartPosi) / nPointSpace) + 1;
		if((nXmax - nXmin) <= nPointStartPosi) nSizeX = 0;
		nSizeY = (int)((nYmax - nYmin) / nPointSpace_Sub) + 1;

		if(nHType==EN_HATCHING_TYPE_RECTDOTS || nHType==EN_HATCHING_TYPE_CIRCLE || nHType==EN_HATCHING_TYPE_PLUS || nHType==EN_HATCHING_TYPE_MINUS || 
			nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL2 || EN_HATCHING_TYPE_GRAVEL_DOTS || 
			nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL || nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL || 
			nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL_S || 
			nHType==EN_HATCHING_TYPE_BDIAGONAL3_FDIAGONAL || nHType==EN_HATCHING_TYPE_FDIAGONAL3_BDIAGONAL)
		{
			nSizeX++; 
			nSizeY++; 
		}

		POINT StartPoint, CurrPoint;
		StartPoint.x = nXmin + nPointStartPosi;
		StartPoint.y = (m_bEMF) ? nYmax : nYmin;
		nSpaceX = nPointSpace;
		nSpaceY = ((m_bEMF) ? -1 : 1) * nPointSpace_Sub;

		nZigZagType = max(2, nZigZagType);
		BOOL bZigPlus = (nZigZagType<10);
		int  nZigDiv  = max(2, nZigZagType%10);

		for(i = 0; i < nSizeX; i++)
		{
			CurrPoint.x = StartPoint.x + i * nSpaceX;

			for(j = 0; j < nSizeY ; j++)
			{
				if(bZigZag) 
				{
					if(nZigZagType <= 2)
					{ 
						if(i % 2 != j % 2) continue;
					}
					else
					{
						if(bZigPlus)
						{
							if((i + j) % nZigDiv != 0) continue;
						}
						else
						{
							if((i - j) % nZigDiv != 0) continue;
						}
					}
				}
				CurrPoint.y =  StartPoint.y + j*nSpaceY;
				PointList.SetAt(nPointCount, CurrPoint);
				nPointCount++;
			}
		}
	}

	if(bLine)
	{		
		if(nLineDir == 1 || nLineDir == 5)
		{
			// 수평선											
			nSizeY = (int)(((nYmax - nYmin) - nLineStartPosi) / nLineSpace) + 1;
			if((nYmax - nYmin) <= nLineStartPosi) nSizeY = 0;

			POINT StartPoint, CurrPoint;
			nrT_LINE arLineD;
			StartPoint.x = nXmin;
			StartPoint.y = (m_bEMF) ? nYmax - nLineStartPosi : nYmin + nLineStartPosi;

			int nCurrLineSpace = (m_bEMF) ? -nLineSpace : nLineSpace;
			CurrPoint.x = StartPoint.x;
			if(nDLine == 1)
			{	
				for(i = 0; i < nSizeY; i++)
				{
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 1, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++;
						}
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i = 0; i < nSizeY ; i++)
				{
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 1, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++; 
						}
					}
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 1, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++;
						}
					}
				}
			}
			else if(nDLine == 3)
			{	
				for(i=0 ; i<nSizeY ; i++)
				{
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 1, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++; 
						}
					}
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 1, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++;
						}
					}
					CurrPoint.y = StartPoint.y + i*nCurrLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 1, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++;
						}
					}
				}
			}
		}

		if(nLineDir == 2 || nLineDir == 5)
		{
			// 수직선	
			nSizeX = (int)(((nXmax - nXmin) - nLineStartPosi) / nLineSpace) + 1;
			if((nXmax - nXmin) <= nLineStartPosi) nSizeX = 0;
			POINT StartPoint, CurrPoint;			
			nrT_LINE arLineD;
			StartPoint.x = nXmin + nLineStartPosi;
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			if(nDLine == 1)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 2, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize ; j++)
						{
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++; 
						}
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 2, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize ; j++)
						{
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++; 
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 2, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++; 
						}
					}
				}
			}		
			else if(nDLine == 3)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 2, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++;
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 2, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++; 
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 2, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++; 
						}
					}
				}
			}		
		}

		if(nLineDir == 3 || nLineDir == 6)
		{
			// 45도 우측에서 아래로 빗금			
			nSizeX = (int)(((nXmax-nXmin) + (nYmax-nYmin) - nLineStartPosi) / nLineSpace) + 1;			
			if(((nXmax-nXmin) + (nYmax - nYmin)) <= nLineStartPosi) nSizeX = 0;
			POINT StartPoint, CurrPoint;
			nrT_LINE arLineD;
			StartPoint.x = nXmin + nLineStartPosi;
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			if(nDLine == 1)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 3, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++;
						}
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i = 0; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 3, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++;
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 3, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize ; j++)
						{
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++;
						}
					}
				}
			}
			else if(nDLine == 3)
			{	
				for(i=0 ; i<nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 3, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++;
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 3, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j=0 ; j<nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++;
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 3, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++;
						}
					}
				}
			}
		}

		if(nLineDir == 4 || nLineDir == 6)
		{
			// 45도 좌측에서 아래로 빗금
			nStartX = (int)(((nYmax - nYmin) + nLineStartPosi) / nLineSpace);
			nSizeX  = (int)(((nXmax - nXmin) - nLineStartPosi) / nLineSpace) + 1;			

			if((nXmax - nXmin) <= nLineStartPosi) nSizeX = 0;

			POINT StartPoint, CurrPoint;
			nrT_LINE arLineD;

			StartPoint.x = nXmin + nLineStartPosi;
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			if(nDLine == 1)
			{	
				for(i = -nStartX; i < nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 4, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++; 
						}
					}
				}
			}
			else if(nDLine == 2)
			{	
				for(i=-nStartX ; i<nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 4, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);  
							nLineCount++; 
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 4, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++; 
						}
					}
				}
			}
			else if(nDLine == 3)
			{	
				for(i=-nStartX ; i<nSizeX ; i++)
				{
					CurrPoint.x = StartPoint.x + i*nLineSpace;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 4, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]); 
							nLineCount++; 
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace - nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 4, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++;
						}
					}
					CurrPoint.x = StartPoint.x + i*nLineSpace + nLineSpace/10;
					if(GetCrossLine_Poly(Polyline_In, CurrPoint, 4, nXmax, nXmin, nYmax, nYmin, arLineD))
					{
						nSize = arLineD.GetSize();
						for(j = 0; j < nSize ; j++)
						{ 
							LineList.SetAt(nLineCount, arLineD[j]);
							nLineCount++; 
						}
					}
				}
			}
		}
		if(nLineDir == 11)
		{
			// 수평선(원주)											
			nSizeY = (int)(((nYmax - nYmin)) / nLineSpace) + 1;

			POINT StartPoint, CurrPoint;
			nrT_LINE arLineD;

			StartPoint.x = nXmin;
			StartPoint.y = (int)((nYmax + nYmin) / 2);

			CurrPoint.x = StartPoint.x;
			int nCurrFactor = (m_bEMF) ? -(nYmax-nYmin) : (nYmax-nYmin);
			double dAngleSpace = (nSizeY-1)==0 ? 0.0 : 1.0 / (nSizeY - 1) * 180.0 * CMathFunc::m_trrad;

			double dAngleFactor;
			BOOL   bSqrt = (nDLine >= 10);
			for(i = 0; i < nSizeY ; i++)
			{
				dAngleFactor = cos(i*dAngleSpace);
				if(bSqrt) dAngleFactor = sqrt(fabs(dAngleFactor)) * (dAngleFactor>=0.0 ? 1.0 : -1.0);

				CurrPoint.y = StartPoint.y + (int)(nCurrFactor/2.0*dAngleFactor);
				if(GetCrossLine_Poly(Polyline_In, CurrPoint, 1, nXmax, nXmin, nYmax, nYmin, arLineD))
				{
					nSize = (int)arLineD.GetSize();
					for(j=0 ; j<nSize ; j++)
					{ LineList.SetAt(nLineCount, arLineD[j]);  nLineCount++; }
				}
			}
		}

		if(nLineDir == 12)
		{
			// 수직선(원주)		
			nSizeX = (int)(((nXmax - nXmin)) / nLineSpace) + 1;

			POINT StartPoint, CurrPoint;
			nrT_LINE arLineD;

			StartPoint.x = (int)((nXmax + nXmin) / 2);
			StartPoint.y = (m_bEMF) ? nYmax : nYmin;

			CurrPoint.y = StartPoint.y;
			int nCurrFactor = (nXmax-nXmin);
			double dAngleSpace = (nSizeX-1)==0 ? 0.0 : 1.0/(nSizeX-1) * 180.0 * CMathFunc::m_trrad;

			double dAngleFactor;
			BOOL   bSqrt = (nDLine >= 10);

			for(i = 0; i < nSizeX ; i++)
			{
				dAngleFactor = cos(i*dAngleSpace);
				if(bSqrt) dAngleFactor = sqrt(fabs(dAngleFactor)) * (dAngleFactor>=0.0 ? 1.0 : -1.0);

				CurrPoint.x = StartPoint.x + (int)(nCurrFactor/2.0*dAngleFactor);
				if(GetCrossLine_Poly(Polyline_In, CurrPoint, 2, nXmax, nXmin, nYmax, nYmin, arLineD))
				{
					nSize = arLineD.GetSize();
					for(j = 0; j < nSize; j++)
					{
						LineList.SetAt(nLineCount, arLineD[j]); 
						nLineCount++; 
					}
				}
			}			
		}
	}

	int nPolyPointSize = Polyline_In.arPoint.GetSize();
	double targetPt[2];
	double (*coor2D)[2];
	coor2D = new double[nPolyPointSize][2];  		
	for(i = 0; i < nPolyPointSize ; i++)
	{
		coor2D[i][0] = (double)(Polyline_In.arPoint[i].x);
		coor2D[i][1] = (double)(Polyline_In.arPoint[i].y);
	}
	int Index;
	POSITION Pos;
	// Point
	if(nHType == EN_HATCHING_TYPE_DOTS)
	{
		// Dot
		Index=0;	
		Pos = PointList.GetStartPosition();

		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);
			targetPt[0] = (double)(pointD.x);  
			targetPt[1] = (double)(pointD.y);
			if(!CMathFunc::mathIsInsidePoint2D(targetPt, nPolyPointSize, coor2D, TRUE)) PointList.RemoveKey(Index);			
		}
	}
	else if(nHType == EN_HATCHING_TYPE_DOTS_B || nHType == EN_HATCHING_TYPE_CIRCLE || nHType == EN_HATCHING_TYPE_DUPLEX_CIRCLE || nHType == EN_HATCHING_TYPE_CIRCLE_EQUAL)
	{
		// Circle	
		T_CIRCLE CurrCircle;
		nrT_ARC arArc;

		int nRadius = max(1, (int)(abs(PointSize.P1.y-PointSize.P2.y) / 2));
		CurrCircle.Radius = nRadius;

		Index = 0;	
		Pos = PointList.GetStartPosition();

		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);

			if(nRadius <= GetDistanceFromIntersectPointToPolyine(Polyline_In, pointD))
			{
				targetPt[0] = (double)(pointD.x);  
				targetPt[1] = (double)(pointD.y);

				if(CMathFunc::mathIsInsidePoint2D(targetPt, nPolyPointSize, coor2D, TRUE))
				{
					CurrCircle.CenterP = pointD;
					CircleList.SetAt(nCircleCount, CurrCircle);  nCircleCount++;
				}
			}
			else 
			{
				if(GetCrossArc_Poly(Polyline_In, pointD, nRadius, arArc))
				{ 
					nSize = (int)arArc.GetSize();
					for(i = 0; i < nSize ; i++)
					{ 
						ArcList.SetAt(nArcCount, arArc[i]);  
						nArcCount++; 
					}
				}
			}	
		}
		if(nHType==EN_HATCHING_TYPE_DUPLEX_CIRCLE)
		{
			nRadius /= 2;
			CurrCircle.Radius = nRadius;
			Index = 0;		
			Pos = PointList.GetStartPosition();
			while(Pos)
			{		
				PointList.GetNextAssoc(Pos, Index, pointD);
				pointD.x += nPointSpace;				

				if(nRadius <= GetDistanceFromIntersectPointToPolyine(Polyline_In, pointD))
				{
					targetPt[0] = (double)(pointD.x);  
					targetPt[1] = (double)(pointD.y);

					if(CMathFunc::mathIsInsidePoint2D(targetPt, nPolyPointSize, coor2D, TRUE))
					{
						CurrCircle.CenterP = pointD;
						CircleList.SetAt(nCircleCount, CurrCircle);  nCircleCount++;
					}
				}
				else 
				{
					if(GetCrossArc_Poly(Polyline_In, pointD, nRadius, arArc))
					{ 
						nSize = (int)arArc.GetSize();
						for(i = 0; i < nSize ; i++)
						{ 
							ArcList.SetAt(nArcCount, arArc[i]); 
							nArcCount++; 
						}
					}
				}	
			}
		}
		else if(nHType==EN_HATCHING_TYPE_CIRCLE_EQUAL)
		{			
			nRadius /= 2;
			T_LINE CurrLineH;			
			nrT_LINE arLineD;
			Index=0;		
			Pos = PointList.GetStartPosition();

			while(Pos)
			{		
				PointList.GetNextAssoc(Pos, Index, pointD);
				pointD.x += nPointSpace;

				CurrLineH.P1.x = pointD.x-nRadius;
				CurrLineH.P2.x = pointD.x+nRadius;
				CurrLineH.P1.y = CurrLineH.P2.y = pointD.y - nRadius;		

				if(GetCrossLine_Poly(Polyline_In, CurrLineH, arLineD))
				{
					nSize = arLineD.GetSize();
					for(i = 0; i < nSize ; i++)
					{ 
						LineList.SetAt(nLineCount, arLineD[i]); 
						nLineCount++; 
					}
				}
				CurrLineH.P1.y = CurrLineH.P2.y = pointD.y + nRadius;				
				if(GetCrossLine_Poly(Polyline_In, CurrLineH, arLineD))
				{
					nSize = arLineD.GetSize();
					for(i = 0; i < nSize ; i++)
					{ 
						LineList.SetAt(nLineCount, arLineD[i]); 
						nLineCount++;
					}
				}		
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType==EN_HATCHING_TYPE_RECTDOTS          || nHType==EN_HATCHING_TYPE_CHECKER             || 
		nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL   || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL   ||
		nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL    || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL  ||
		nHType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL_S || nHType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL_S || 
		nHType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL_S  || nHType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL_S)
	{
		// Rect
		int nCurrXmax = max(PointSize.P1.x, PointSize.P2.x);
		int nCurrXmin = min(PointSize.P1.x, PointSize.P2.x);
		int nCurrYmax = max(PointSize.P1.y, PointSize.P2.y);
		int nCurrYmin = min(PointSize.P1.y, PointSize.P2.y);

		nrT_POLYLINE arRectPoly;
		T_POLYLINE CurrPoly;
		nSize = (int)PointList.GetCount();
		arRectPoly.SetSize(nSize);
		CurrPoly.arPoint.SetSize(4);
		CurrPoly.arPoint[0].x = nCurrXmin;  CurrPoly.arPoint[0].y = nCurrYmin;
		CurrPoly.arPoint[1].x = nCurrXmax;  CurrPoly.arPoint[1].y = nCurrYmin;
		CurrPoly.arPoint[2].x = nCurrXmax;  CurrPoly.arPoint[2].y = nCurrYmax;
		CurrPoly.arPoint[3].x = nCurrXmin;  CurrPoly.arPoint[3].y = nCurrYmax;

		for(i = 0; i < nSize ; i++)
		{
			arRectPoly[i] = CurrPoly; 
		}

		int nCurrCount = 0;
		Index=0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);			
			arRectPoly[nCurrCount].Move(pointD.x, pointD.y);			
			nCurrCount++;
		}

		nrT_POLYLINE arCurrPolygon;
		nrT_POLYLINE arCurrPolyline;

		if(GetCrossPolyline_Poly(Polyline_In, arRectPoly, arCurrPolygon, arCurrPolyline, TRUE))
		{
			nSize = arCurrPolygon.GetSize();
			for(i = 0; i < nSize ; i++)
			{
				PolygonList.SetAt(nPolygonCount, arCurrPolygon[i]); 
				nPolygonCount++;
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType==EN_HATCHING_TYPE_PLUS || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL2 || nHType==EN_HATCHING_TYPE_PLUS_BDIAGONAL3)
	{
		// '+'
		int nCurrXmax = max(PointSize.P1.x, PointSize.P2.x);
		int nCurrXmin = min(PointSize.P1.x, PointSize.P2.x);
		int nCurrYmax = max(PointSize.P1.y, PointSize.P2.y);
		int nCurrYmin = min(PointSize.P1.y, PointSize.P2.y);
		T_LINE CurrLineV, CurrLineH;		
		nrT_LINE arLineD;

		Index=0;
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);			
			CurrLineH.P1.x = pointD.x+nCurrXmin;
			CurrLineH.P2.x = pointD.x+nCurrXmax;
			CurrLineH.P1.y = CurrLineH.P2.y = pointD.y;

			CurrLineV.P1.x = CurrLineV.P2.x = pointD.x;
			CurrLineV.P1.y = m_bEMF ? pointD.y+nCurrYmax : pointD.y+nCurrYmin;
			CurrLineV.P2.y = m_bEMF ? pointD.y+nCurrYmin : pointD.y+nCurrYmax;

			if(GetCrossLine_Poly(Polyline_In, CurrLineH, arLineD))
			{
				nSize = arLineD.GetSize();
				for(i = 0; i<nSize ; i++)
				{
					LineList.SetAt(nLineCount, arLineD[j]); 
					nLineCount++;
				}
			}

			if(GetCrossLine_Poly(Polyline_In, CurrLineV, arLineD))
			{
				nSize = arLineD.GetSize();
				for(i=0 ; i<nSize ; i++)
				{ 
					LineList.SetAt(nLineCount, arLineD[j]); 
					nLineCount++; 
				}
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_MINUS)
	{
		// '-'		
		int nCurrXmax = max(PointSize.P1.x, PointSize.P2.x);
		int nCurrXmin = min(PointSize.P1.x, PointSize.P2.x);
		T_LINE CurrLineH;	
		nrT_LINE arLineD;

		Index=0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);			
			CurrLineH.P1.x = pointD.x+nCurrXmin;
			CurrLineH.P2.x = pointD.x+nCurrXmax;
			CurrLineH.P1.y = CurrLineH.P2.y = pointD.y;

			if(GetCrossLine_Poly(Polyline_In, CurrLineH, arLineD))
			{
				nSize = arLineD.GetSize();
				for(i=0 ; i<nSize ; i++)
				{ 
					LineList.SetAt(nLineCount, arLineD[j]); 
					nLineCount++;
				}
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_BRICK)
	{
		// '｜'		
		int nCurrYmax = max(PointSize.P1.y, PointSize.P2.y);
		int nCurrYmin = min(PointSize.P1.y, PointSize.P2.y);
		T_LINE CurrLineV;
		nrT_LINE arLineD;

		Index=0;		
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);	
			CurrLineV.P1.x = CurrLineV.P2.x = pointD.x;
			CurrLineV.P1.y = m_bEMF ? pointD.y+nCurrYmax : pointD.y+nCurrYmin;
			CurrLineV.P2.y = m_bEMF ? pointD.y+nCurrYmin : pointD.y+nCurrYmax;

			if(GetCrossLine_Poly(Polyline_In, CurrLineV, arLineD))
			{
				nSize = arLineD.GetSize();
				for(i = 0; i < nSize ; i++)
				{
					LineList.SetAt(nLineCount, arLineD[j]); 
					nLineCount++;
				}
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType==EN_HATCHING_TYPE_BDIAGONAL3_FDIAGONAL || nHType==EN_HATCHING_TYPE_FDIAGONAL3_BDIAGONAL)
	{
		T_LINE CurrLine;
		nrT_LINE arLineD;

		double dXslope = (PointSize.P2.x==PointSize.P1.x) ? 0.0 : (double)(PointSize.P2.y-PointSize.P1.y)/(double)(PointSize.P2.x-PointSize.P1.x);
		double dYslope = (PointSize.P2.y==PointSize.P1.y) ? 0.0 : (double)(PointSize.P2.x-PointSize.P1.x)/(double)(PointSize.P2.y-PointSize.P1.y);

		int nGapXmax, nGapYmax, nGapYmin; 
		nGapXmax = nGapYmax = nGapYmin = 0;
		if(!m_bRectTrPolyDraw)
		{ 
			nGapXmax = -1 * max(1, drawFormat.Width/2);  
			if(m_bEMF) nGapYmin = max(1, drawFormat.Width/2); 
			else       nGapYmax = -1 * max(1, drawFormat.Width/2);
		}

		Index=0;	
		Pos = PointList.GetStartPosition();
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);			

			CurrLine.P1.x =pointD.x + PointSize.P1.x;
			CurrLine.P1.y =pointD.y + PointSize.P1.y;
			CurrLine.P2.x =pointD.x + PointSize.P2.x;
			CurrLine.P2.y =pointD.y + PointSize.P2.y;

			if(CurrLine.P1.x > nXmax+nGapXmax) { CurrLine.P1.y -= (int)(dXslope*(CurrLine.P1.x-(nXmax+nGapXmax)));  CurrLine.P1.x = nXmax+nGapXmax;}
			if(CurrLine.P1.y > nYmax+nGapYmax) { CurrLine.P1.x -= (int)(dYslope*(CurrLine.P1.y-(nYmax+nGapYmax)));  CurrLine.P1.y = nYmax+nGapYmax;}
			if(CurrLine.P1.x < nXmin         ) { CurrLine.P1.y -= (int)(dXslope*(CurrLine.P1.x-(nXmin         )));  CurrLine.P1.x = nXmin         ;}
			if(CurrLine.P1.y < nYmin+nGapYmin) { CurrLine.P1.x -= (int)(dYslope*(CurrLine.P1.y-(nYmin+nGapYmin)));  CurrLine.P1.y = nYmin+nGapYmin;}

			if(CurrLine.P2.x > nXmax+nGapXmax) { CurrLine.P2.y -= (int)(dXslope*(CurrLine.P2.x-(nXmax+nGapXmax)));  CurrLine.P2.x = nXmax+nGapXmax;}
			if(CurrLine.P2.y > nYmax+nGapYmax) { CurrLine.P2.x -= (int)(dYslope*(CurrLine.P2.y-(nYmax+nGapYmax)));  CurrLine.P2.y = nYmax+nGapYmax;}
			if(CurrLine.P2.x < nXmin         ) { CurrLine.P2.y -= (int)(dXslope*(CurrLine.P2.x-(nXmin         )));  CurrLine.P2.x = nXmin         ;}
			if(CurrLine.P2.y < nYmin+nGapYmin) { CurrLine.P2.x -= (int)(dYslope*(CurrLine.P2.y-(nYmin+nGapYmin)));  CurrLine.P2.y = nYmin+nGapYmin;}

			if((CurrLine.P1.x == CurrLine.P2.x) &&  (CurrLine.P1.y == CurrLine.P2.y)) continue;				

			if(max(CurrLine.P1.x, CurrLine.P2.x) > nXmax) continue;
			if(max(CurrLine.P1.y, CurrLine.P2.y) > nYmax) continue;
			if(min(CurrLine.P1.x, CurrLine.P2.x) < nXmin) continue;
			if(min(CurrLine.P1.y, CurrLine.P2.y) < nYmin) continue;

			if(GetCrossLine_Poly(Polyline_In, CurrLine, arLineD))
			{
				nSize = arLineD.GetSize();
				for(i = 0; i < nSize ; i++)
				{ 
					LineList.SetAt(nLineCount, arLineD[j]);  
					nLineCount++;
				}
			}
		}
		PointList.RemoveAll();
	}
	else if(nHType == EN_HATCHING_TYPE_GRAVEL || nHType == EN_HATCHING_TYPE_GRAVEL_DOTS)
	{		
		nrT_POLYLINE arGravelPoly;
		nrPOINT		arGravelPoint;
		nrT_POLYLINE arCurrGravelPoly;
		nrT_POLYLINE arCurrPolygon;
		nrT_POLYLINE arCurrPolyline;
		T_POLYLINE CurrPoly;
		int nGravelXmax, nGravelXmin, nGravelYmax, nGravelYmin;

		GetGravelData(nPointSpace, nPointSpace_Sub, arGravelPoly, arGravelPoint, nGravelXmax, nGravelXmin, nGravelYmax, nGravelYmin);

		nrPOINT arPointList;
		nSize = (int)PointList.GetCount();
		arPointList.SetSize(nSize);
		Index=0;		
		Pos = PointList.GetStartPosition();
		int nCount=0;
		while(Pos)
		{		
			PointList.GetNextAssoc(Pos, Index, pointD);	
			arPointList[nCount] = pointD;
			nCount++;		
		}
		PointList.RemoveAll();
		nPointCount = 0;

		int nPolygonSize, nPointSize, nCurrPolygonSize, nCurrPolylineSize;
		nPolygonSize = arGravelPoly.GetSize();
		nPointSize   = arGravelPoint.GetSize();
		for(i = 0; i < nSize ; i++)
		{
			pointD = arPointList[i];

			arCurrGravelPoly.RemoveAll();
			arCurrGravelPoly.SetSize(nPolygonSize);
			for(j = 0; j < nPolygonSize ; j++)
			{ 
				CurrPoly = arGravelPoly[j];
				CurrPoly.Move(pointD.x, pointD.y);
				arCurrGravelPoly[j] = CurrPoly;
			}

			if(GetCrossPolyline_Poly(Polyline_In, arCurrGravelPoly, arCurrPolygon, arCurrPolyline, FALSE))
			{
				nCurrPolygonSize = arCurrPolygon.GetSize();
				for(j = 0; j < nCurrPolygonSize ; j++)
				{
					PolygonList.SetAt(nPolygonCount, arCurrPolygon[j]); 
					nPolygonCount++;
				}
				nCurrPolylineSize = arCurrPolyline.GetSize();
				for(j = 0; j < nCurrPolylineSize ; j++)
				{
					PolylineList.SetAt(nPolylineCount, arCurrPolyline[j]); 
					nPolylineCount++;
				}
			}			
		}
		if(nHType==EN_HATCHING_TYPE_GRAVEL_DOTS)
		{		
			T_CIRCLE CurrCircle;
			nrT_ARC arArc;

			int nRadius = max(1, (int)(drawFormat.HatchingSpace / 15));
			CurrCircle.Radius = nRadius;

			for(i = 0; i < nSize ; i++)
			{
				pointD = arPointList[i];

				if(nRadius <= GetDistanceFromIntersectPointToPolyine(Polyline_In, pointD))
				{
					targetPt[0] = (double)(pointD.x);  
					targetPt[1] = (double)(pointD.y);

					if(CMathFunc::mathIsInsidePoint2D(targetPt, nPolyPointSize, coor2D, TRUE))
					{
						CurrCircle.CenterP = pointD;
						CircleList.SetAt(nCircleCount, CurrCircle);  nCircleCount++;
					}
				}
				else 
				{
					if(GetCrossArc_Poly(Polyline_In, pointD, nRadius, arArc))
					{ 
						nSize = arArc.GetSize();
						for(i=0 ; i < nSize ; i++)
						{ 
							ArcList.SetAt(nArcCount, arArc[i]); 
							nArcCount++;
						}
					}
				}	
			}
		}
	}
	delete [] coor2D;

	// Hatching정보 입력
	T_LINE     lineD;
	T_CIRCLE   CircleD;
	T_ARC      ArcD;
	T_POLYLINE PolyD;
	// - Point
	nSize = (int)PointList.GetCount();
	Hatching_Out.arPoint.SetSize(nSize);
	Index=0;		
	Pos = PointList.GetStartPosition();
	nPointCount = 0;
	while(Pos)
	{		
		PointList.GetNextAssoc(Pos, Index, pointD);	
		Hatching_Out.arPoint[nPointCount] = pointD;
		nPointCount++;
	}
	// - Line
	nSize = (int)LineList.GetCount();
	Hatching_Out.arLine.SetSize(nSize);
	Index=0;	
	Pos = LineList.GetStartPosition();
	nLineCount = 0;
	while(Pos)
	{		
		LineList.GetNextAssoc(Pos, Index, lineD);	
		Hatching_Out.arLine[nLineCount] = lineD;
		nLineCount++;
	}
	// - Rect
	nSize = (int)RectList.GetCount();
	Hatching_Out.arRect.SetSize(nSize);
	Index=0;	
	Pos = RectList.GetStartPosition();
	nRectCount = 0;
	while(Pos)
	{		
		RectList.GetNextAssoc(Pos, Index, lineD);	
		Hatching_Out.arRect[nRectCount] = lineD;
		nRectCount++;
	}
	// - Circle
	nSize = (int)CircleList.GetCount();
	Hatching_Out.arCircle.SetSize(nSize);
	Index=0;	
	Pos = CircleList.GetStartPosition();
	nCircleCount = 0;
	while(Pos)
	{		
		CircleList.GetNextAssoc(Pos, Index, CircleD);	
		Hatching_Out.arCircle[nCircleCount] = CircleD;
		nCircleCount++;
	}
	// - Arc
	nSize = (int)ArcList.GetCount();
	Hatching_Out.arArc.SetSize(nSize);
	Index=0;	
	Pos = ArcList.GetStartPosition();
	nArcCount = 0;
	while(Pos)
	{		
		ArcList.GetNextAssoc(Pos, Index, ArcD);	
		Hatching_Out.arArc[nArcCount] = ArcD;
		nArcCount++;
	}
	// - Polygon
	nSize = (int)PolygonList.GetCount();
	Hatching_Out.arPolygon.SetSize(nSize);
	Index=0;		
	Pos = PolygonList.GetStartPosition();
	nPolygonCount = 0;
	while(Pos)
	{		
		PolygonList.GetNextAssoc(Pos, Index, PolyD);	
		Hatching_Out.arPolygon[nPolygonCount] = PolyD;
		nPolygonCount++;
	}
	// - Polyline
	nSize = (int)PolylineList.GetCount();
	Hatching_Out.arPolyline.SetSize(nSize);
	Index=0;		
	Pos = PolylineList.GetStartPosition();
	nPolylineCount = 0;
	while(Pos)
	{		
		PolylineList.GetNextAssoc(Pos, Index, PolyD);	
		Hatching_Out.arPolyline[nPolylineCount] = PolyD;
		nPolylineCount++;
	}
}

void CDrawFunc::MakeArHatchingData(T_DRAW_FORMAT drawFormat, nrT_LINE& arRect_In, nrT_HATCHING_D& arHatching_Out)
{	
	arHatching_Out.RemoveAll();
	if(!drawFormat.bHatching) return;

	int nCount = arRect_In.GetSize();
	arHatching_Out.SetSize(nCount);
	for(int i = 0; i < nCount ; ++i)
	{
		T_HATCHING_D dgnData;
		MakeHatchingData(drawFormat, arRect_In[i], dgnData);
		arHatching_Out[i] = dgnData;
	}
}

void CDrawFunc::MakeArHatchingData(T_DRAW_FORMAT drawFormat, nrT_POLYLINE& arPolyline_In, nrT_HATCHING_D& arHatching_Out)
{
	arHatching_Out.RemoveAll();
	if(!drawFormat.bHatching) return;

	int nCount = arPolyline_In.GetSize();
	arHatching_Out.SetSize(nCount);

	for(int i = 0; i < nCount ; ++i)
	{
		T_HATCHING_D dgnData;
		MakeHatchingData(drawFormat, arPolyline_In[i], dgnData);
		arHatching_Out[i] = dgnData;
	}
}

void CDrawFunc::DrawHatching(HDC DC, EN_HATCHING_TYPE HatchingType, COLORREF HatchingColor, BOOL bHatching, int Width, T_HATCHING_D& dgnHatching)
{
	if(!bHatching) return;

	BOOL bFill = (HatchingType==EN_HATCHING_TYPE_DOTS || HatchingType==EN_HATCHING_TYPE_DOTS_B || HatchingType==EN_HATCHING_TYPE_RECTDOTS || HatchingType==EN_HATCHING_TYPE_CHECKER || 
		HatchingType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL   || HatchingType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL   ||
		HatchingType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL    || HatchingType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL  ||
		HatchingType==EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL_S || HatchingType==EN_HATCHING_TYPE_RECTDOTS_VERTICAL_S ||
		HatchingType==EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL_S  || HatchingType==EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL_S);	
	int  nLineType = PS_SOLID;
	int  nPointLineType = PS_SOLID;
	BOOL bFill_Circle = (HatchingType==EN_HATCHING_TYPE_GRAVEL_DOTS)? TRUE : bFill;
	BOOL bFill_Arc    = (HatchingType==EN_HATCHING_TYPE_GRAVEL_DOTS)? TRUE : bFill;
	int  Width_Rect   = (HatchingType==EN_HATCHING_TYPE_CHECKER    )?    0 : Width;

	DrawPoint(DC, HatchingColor, Width, dgnHatching.arPoint);
	DrawLine(DC, nLineType, HatchingColor, Width, dgnHatching.arLine, PS_GEOMETRIC|PS_ENDCAP_FLAT);
	DrawRect(DC, nPointLineType, HatchingColor, HatchingColor, bFill, Width_Rect, dgnHatching.arRect);
	DrawCircle(DC, nPointLineType, HatchingColor, HatchingColor, bFill_Circle, Width, dgnHatching.arCircle);
	DrawArc(DC, nPointLineType, HatchingColor, HatchingColor, bFill_Arc, Width, 0.0, 0.0, EN_ARROW_TYPE_NONE, EN_ARROW_TYPE_NONE, dgnHatching.arArc);
	DrawPolygon(DC, nPointLineType, HatchingColor, HatchingColor, bFill, Width, dgnHatching.arPolygon);
	DrawPolyline(DC, nPointLineType, HatchingColor, HatchingColor, bFill, Width, dgnHatching.arPolyline);	
}
