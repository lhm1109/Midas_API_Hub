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

double CDrawFunc::GetDistanceFromIntersectPointToPolyine(T_POLYLINE PolyD, POINT pointD)
{
	double dDis = 0.0;
	int nSize = PolyD.arPoint.GetSize();

	if(nSize == 0) return 0.0;
	if(nSize == 1) return sqrt((double)((pointD.x - PolyD.arPoint[0].x) * (pointD.x - PolyD.arPoint[0].x) + (pointD.y - PolyD.arPoint[0].y) * (pointD.y - PolyD.arPoint[0].y)));

	double line_i[3], line_j[3], point[3];
	line_i[2] = line_j[2] = point[2] = 0.0;

	point[0] = (double)(pointD.x);
	point[1] = (double)(pointD.y);
	for(int i = 0; i < nSize; i++)
	{
		if(i % 2 == 0)
		{
			line_j[0] = (double)(PolyD.arPoint[i].x);
			line_j[1] = (double)(PolyD.arPoint[i].y);
		}
		else 
		{
			line_i[0] = (double)(PolyD.arPoint[i].x);
			line_i[1] = (double)(PolyD.arPoint[i].y);
		}

		if(i == 0)
		{
			line_i[0] = (double)(PolyD.arPoint[nSize - 1].x);
			line_i[1] = (double)(PolyD.arPoint[nSize - 1].y);

			dDis = CMathFunc::mathDistanceFromIntersectPointToLine(line_i, line_j, point);
		}
		else 
		{
			dDis = min(dDis, CMathFunc::mathDistanceFromIntersectPointToLine(line_i, line_j, point));
		}
	}
	return dDis;
}

void CDrawFunc::GetAutoAxisInfo(int nWidth, int nHeight, int nFontSize, int nDecimal, double dMaxValue, double dMinValue, T_CHART_INFO_AXIS& AxisInfo, BOOL bCrossLabel)
{
	BOOL bMaxValueAuto   = AxisInfo.bMaxValueAuto;
	BOOL bMinValueAuto   = AxisInfo.bMinValueAuto;
	BOOL bMajorSpaceAuto = AxisInfo.bMajorSpaceAuto;
	BOOL bMinorSpaceAuto = AxisInfo.bMinorSpaceAuto;

	int    nSpaceType;// Type 0:1간격, 1:2간격, 2:2.5간격, 3:5간격
	double dCalcSpace;
	double dALMaxValue = bMaxValueAuto ? dMaxValue : AxisInfo.dMaxValue;
	double dALMinValue = bMinValueAuto ? dMinValue : AxisInfo.dMinValue;

	TCHAR strMax[64], strMin[64];  

	if(nDecimal < 0) 
	{
		wsprintf(strMax, _T("%g"), dMaxValue);
		wsprintf(strMin, _T("%g"), dMinValue);
	}
	else 
	{
		wsprintf(strMax, GetDoubleStringFormat(nDecimal), dMaxValue);
		wsprintf(strMin, GetDoubleStringFormat(nDecimal), dMinValue);
	}

	int nLebelLength = (int)(max(nFontSize * 1.5, max(lstrlen(strMax), lstrlen(strMin)) * nFontSize / (bCrossLabel ? 4 : 2)));

	if(bMaxValueAuto)
	{					
		if(dMaxValue < 0.0 && dMaxValue + 3.0 * (dMaxValue-dMinValue) > 0.0)	
		{ 
			AxisInfo.dMaxValue = dALMaxValue = 0.0; 
			bMaxValueAuto = FALSE;
		}
	}
	if(bMinValueAuto)
	{					
		if(dMinValue > 0.0 && dMinValue - 3.0 * (dMaxValue-dMinValue) < 0.0)	
		{
			AxisInfo.dMinValue = dALMinValue = 0.0; 
			bMinValueAuto = FALSE;
		}
	}

	if(bMajorSpaceAuto)
	{
		if(bMinorSpaceAuto)	
		{	
			double dMinorMax = (nWidth >= nHeight) ? 40.0 : (nHeight == 0 ? 40.0 : 40.0 * nWidth / nHeight);				
			dMinorMax = min(dMinorMax, nWidth/(nLebelLength / 3.0));
			AxisInfo.dMinorSpace = FindAutoSpace((dALMaxValue - dALMinValue) / dMinorMax, nSpaceType); 			
			bMinorSpaceAuto=FALSE;			
		}

		dCalcSpace = FindAutoSpace(AxisInfo.dMinorSpace, nSpaceType); 		

		int nRotaionMax = (dALMaxValue - dALMinValue) * dCalcSpace*nWidth == 0.0 ? 1 : (int)(((double)nLebelLength) / (((double)nWidth) / ((dALMaxValue - dALMinValue) / dCalcSpace))) + 1;

		if(nRotaionMax < 1) nRotaionMax = 1000;//nRotaionMax값이 너무 커서 음수로 변환되었을 경우 처리

		for(int i = 0; i < nRotaionMax; i++)
		{
			AxisInfo.dMajorSpace = GetIncreaseDiv(nSpaceType, i) * dCalcSpace;
			if(((double)nWidth) / ((dALMaxValue - dALMinValue) / AxisInfo.dMajorSpace) > 1.5 * nLebelLength) break;
		}
		bMajorSpaceAuto = FALSE;
	}
	else if(bMinorSpaceAuto) 
	{
		dCalcSpace = FindAutoSpace(AxisInfo.dMajorSpace, nSpaceType); 

		if     (nSpaceType == 0) { AxisInfo.dMinorSpace = dCalcSpace / 5.0; }
		else if(nSpaceType == 1) { AxisInfo.dMinorSpace = dCalcSpace / 4.0; }
		else if(nSpaceType == 2) { AxisInfo.dMinorSpace = dCalcSpace / 5.0; }
		else                      { AxisInfo.dMinorSpace = dCalcSpace / 5.0; }
		bMinorSpaceAuto = FALSE;
	}

	if(AxisInfo.dMajorSpace == 0.0) { ASSERT(0);  AxisInfo.dMajorSpace = 1; }

	if(bMaxValueAuto)
	{		
		dALMaxValue = dMaxValue+0.05*(dMaxValue-dMinValue);
		if     (dALMaxValue == 0) AxisInfo.dMaxValue = AxisInfo.dMajorSpace;
		else if(dALMaxValue >  0) AxisInfo.dMaxValue = AxisInfo.dMajorSpace * ((int)(dALMaxValue / AxisInfo.dMajorSpace) + (1.05*(dMaxValue-dMinValue)/AxisInfo.dMajorSpace < 5.0 ? 1 :  2));
		else                       AxisInfo.dMaxValue = AxisInfo.dMajorSpace * ((int)(dALMaxValue / AxisInfo.dMajorSpace) + (1.05*(dMaxValue-dMinValue)/AxisInfo.dMajorSpace < 5.0 ? 0 :  1));
		bMaxValueAuto = FALSE;
	}

	if(bMinValueAuto)
	{
		dALMinValue = dMinValue-0.05*(dMaxValue-dMinValue);
		if     (dALMinValue == 0) AxisInfo.dMinValue = -AxisInfo.dMajorSpace;
		else if(dALMinValue >  0) AxisInfo.dMinValue = AxisInfo.dMajorSpace * ((int)(dALMinValue / AxisInfo.dMajorSpace) - (1.05*(dMaxValue-dMinValue)/AxisInfo.dMajorSpace < 5.0 ? 0 :  1));
		else                       AxisInfo.dMinValue = AxisInfo.dMajorSpace * ((int)(dALMinValue / AxisInfo.dMajorSpace) - (1.05*(dMaxValue-dMinValue)/AxisInfo.dMajorSpace < 5.0 ? 1 :  2));
		bMinValueAuto = FALSE;
	}
}

double CDrawFunc::GetIncreaseDiv(int nType, int nTime)
{
	int nMulty = nTime / 3;

	double dCur = 1.0;
	if(nType == 0)
	{
		// minor 1간격 		
		if(nTime%3 == 0)      dCur = 5.0;		
		else if(nTime%3 == 1) dCur = 10.0;		
		else                  dCur = 20.0;		
	}
	else if(nType == 1) 
	{
		// minor 2간격		
		if(nTime%3 == 0)      dCur = 5.0;		
		else if(nTime%3 == 1) dCur = 10.0;		
		else                   dCur = 25.0;		
	}
	else if(nType == 2) 
	{
		// minor 2.5간격	
		if(nTime%3 == 0)      dCur = 4.0;		
		else if(nTime%3 == 1) dCur = 8.0;		
		else                   dCur = 20.0;		
	}
	else        
	{
		// minor 5간격
		if(nTime%3 == 0)      dCur = 4.0;		
		else if(nTime%3 == 1) dCur = 10.0;		
		else                   dCur = 20.0;		
	}

	return dCur * pow(10.0, nMulty);		
}

void CDrawFunc::GetHatchingSubData(EN_HATCHING_TYPE nHType, int nSpace, BOOL& bPoint, BOOL& bLine, int& nDLine, BOOL& bZigZag, int& nZigZagType, int& nLineDir,  
								   int& nLineStartPosi, int& nPointStartPosi, int& nLineSpace, int& nPointSpace, int& nPointSpace_Sub, T_LINE& PointSize)
{
	nLineStartPosi = 0;
	nPointStartPosi = 0;

	bPoint  = FALSE;
	bLine   = FALSE;
	nDLine  = 1;
	bZigZag = FALSE;
	nZigZagType = 22;
	nLineDir = 0; // 1:수평, 2:수직, 3:45도 우측에서 아래로, 4:45도 좌측에서 아래로, 5:십자가, 6:X자, 11:수평원통, 12:수직원통, 0:ETC
	nLineSpace      = nSpace;
	nPointSpace     = nSpace;
	nPointSpace_Sub = nSpace;

	PointSize.P1.x = -max(1, nLineSpace/5); 
	PointSize.P1.y = -max(1, (int)(nLineSpace/7.5));  
	PointSize.P2.x =  max(1, nLineSpace/5); 
	PointSize.P2.y =  max(1, (int)(nLineSpace/7.5));  

	switch(nHType)
	{
	case EN_HATCHING_TYPE_HORIZONTAL:// 수평으로 빗금
		nLineDir  = 1;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_VERTICAL: // 수직으로 빗금
		nLineDir  = 2;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_BDIAGONAL: // 45도 우측에서 아래로 빗금
		nLineDir  = 3;
		bLine = TRUE;
		break;  
	case EN_HATCHING_TYPE_FDIAGONAL: // 45도 좌측에서 아래로 빗금
		nLineDir  = 4;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_CROSS: // 십자가형태
		nLineDir  = 5;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_DIAGCROSS: // X자형태
		nLineDir  = 6;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_DOTS: // 점(원형)을 지그제그로 배치			
		bPoint  = TRUE;
		bZigZag = TRUE;
		break;
	case EN_HATCHING_TYPE_DOTS_B: // 굵은 점(원형)을 지그제그로 배치			
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = -max(1, nLineSpace/15);
		PointSize.P1.y = -max(1, nLineSpace/15);  
		PointSize.P2.x =  max(1, nLineSpace/15); 
		PointSize.P2.y =  max(1, nLineSpace/15);  
		break;
	case EN_HATCHING_TYPE_RECTDOTS: // 점(사각형3:2)을 지그제그로 배치		
		bPoint  = TRUE;
		bZigZag = TRUE;
		break;
	case EN_HATCHING_TYPE_CIRCLE: // 원을 지그제그로 배치		
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = -max(1, nLineSpace/5);
		PointSize.P1.y = -max(1, nLineSpace/5);  
		PointSize.P2.x =  max(1, nLineSpace/5);
		PointSize.P2.y =  max(1, nLineSpace/5);  
		break;
	case EN_HATCHING_TYPE_DUPLEX_HORIZONTAL: // 이중으로 된 수평으로 빗금
		nLineDir   = 1;
		bLine  = TRUE;
		nDLine = 2;
		nLineStartPosi = nLineSpace/2;
		break;
	case EN_HATCHING_TYPE_DUPLEX_VERTICAL: // 이중으로 된 수직으로 빗금
		nLineDir   = 2;
		bLine  = TRUE;
		nDLine = 2;
		nLineStartPosi = nLineSpace/2;
		break;
	case EN_HATCHING_TYPE_DUPLEX_BDIAGONAL: // 이중으로 된 45도 우측에서 아래로 빗금
		nLineDir   = 3;
		bLine  = TRUE;
		nDLine = 2;
		break;  
	case EN_HATCHING_TYPE_DUPLEX_FDIAGONAL: // 이중으로 된 45도 좌측에서 아래로 빗금
		nLineDir   = 4;
		bLine  = TRUE;
		nDLine = 2;
		break;
	case EN_HATCHING_TYPE_DUPLEX_CROSS: // 이중으로 된 십자가형태
		nLineDir   = 5;
		bLine  = TRUE;
		nDLine = 2;
		break;
	case EN_HATCHING_TYPE_DUPLEX_DIAGCROSS: // 이중으로 된 X자형태
		nLineDir   = 6;
		bLine  = TRUE;
		nDLine = 2;
		break;
	case EN_HATCHING_TYPE_DUPLEX_CIRCLE: // 크기가 다른 원을 이중으로 배치	
		bPoint  = TRUE;
		bZigZag = TRUE;			
		PointSize.P1.x = -max(1, nLineSpace/5);
		PointSize.P1.y = -max(1, nLineSpace/5);  
		PointSize.P2.x =  max(1, nLineSpace/5);
		PointSize.P2.y =  max(1, nLineSpace/5);  
		break;
	case EN_HATCHING_TYPE_PLUS: // '+'기호를 지그제그로 배치	
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = -max(1, nLineSpace/5); 
		PointSize.P1.y = -max(1, nLineSpace/5);  
		PointSize.P2.x =  max(1, nLineSpace/5);
		PointSize.P2.y =  max(1, nLineSpace/5);  
		break;
	case EN_HATCHING_TYPE_MINUS: // '-'기호를 지그제그로 배치	
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = -max(1, nLineSpace/5);
		PointSize.P1.y = 0;  
		PointSize.P2.x =  max(1, nLineSpace/5); 
		PointSize.P2.y = 0;  
		break;
	case EN_HATCHING_TYPE_CHECKER: // 체크무늬(ex 체크판)	
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = 0;       
		PointSize.P1.y = 0;  
		PointSize.P2.x = nSpace;  
		PointSize.P2.y = ((m_bEMF) ? -1 : 1) * nSpace;
		break;
	case EN_HATCHING_TYPE_BRICK: // 벽둘무늬
		nLineDir   = 1;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		nPointSpace = (int)(1.5*nPointSpace);
		PointSize.P1.x = 0; 
		PointSize.P1.y = 0;  
		PointSize.P2.x = 0; 
		PointSize.P2.y = ((m_bEMF) ? -1 : 1) * nSpace;
		break;
	case EN_HATCHING_TYPE_GRAVEL: // 자갈무늬
		bLine = FALSE;
		bPoint  = TRUE;
		bZigZag = FALSE;
		PointSize.P1.x = -2*nPointSpace; 
		PointSize.P1.y = -2*nPointSpace;  
		PointSize.P2.x =  2*nPointSpace; 
		PointSize.P2.y =  2*nPointSpace;  
		nPointSpace = nPointSpace_Sub = 4 * nPointSpace;			
		break;
	case EN_HATCHING_TYPE_HORIZONTAL_CYLINDER: // 수직 원통모양(수직선을 양옆으로 촘촘하게 배치)		
		nLineDir  = 11;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_VERTICAL_CYLINDER: // 수평 원통모양(수직선을 아래위로 촘촘하게 배치)
		nLineDir  = 12;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_HORIZONTAL_CYLINDER_SQ: // 수직 원통모양(수직선을 양옆으로 촘촘하게 배치)-간격격차증가	
		nLineDir  = 11;
		nDLine    = 11;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_VERTICAL_CYLINDER_SQ: // 수평 원통모양(수직선을 아래위로 촘촘하게 배치)-간격격차증가
		nLineDir  = 12;
		nDLine    = 11;
		bLine = TRUE;
		break;
	case EN_HATCHING_TYPE_CIRCLE_EQUAL: // 원 + '='기호가 지그제그로 배치
		bPoint  = TRUE;
		bZigZag = TRUE;			
		PointSize.P1.x = -max(1, nLineSpace/5);  
		PointSize.P1.y = -max(1, nLineSpace/5);  
		PointSize.P2.x =  max(1, nLineSpace/5); 
		PointSize.P2.y =  max(1, nLineSpace/5);  
		break;
	case EN_HATCHING_TYPE_PLUS_BDIAGONAL: // 1줄 BDIAGONAL와 '+'기호가 반복
		nLineDir   = 3;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		nLineStartPosi = nPointSpace;
		PointSize.P1.x = -max(1, nLineSpace/5);
		PointSize.P1.y = -max(1, nLineSpace/5);  
		PointSize.P2.x =  max(1, nLineSpace/5); 
		PointSize.P2.y =  max(1, nLineSpace/5);  			
		nLineSpace  *= 2;
		break;
	case EN_HATCHING_TYPE_PLUS_BDIAGONAL2: // 2줄 BDIAGONAL와 '+'기호가 반복
		nLineDir   = 3;
		bLine  = TRUE;
		nDLine = 2;
		bPoint  = TRUE;
		bZigZag = TRUE;
		nLineStartPosi = nPointSpace;
		PointSize.P1.x = -max(1, nLineSpace/5);
		PointSize.P1.y = -max(1, nLineSpace/5);  
		PointSize.P2.x =  max(1, nLineSpace/5); 
		PointSize.P2.y =  max(1, nLineSpace/5);  
		nLineSpace  *= 2;
		break;
	case EN_HATCHING_TYPE_PLUS_BDIAGONAL3: // 3줄 BDIAGONAL와 '+'기호가 반복
		nLineDir   = 3;
		bLine  = TRUE;
		nDLine = 3;
		bPoint  = TRUE;
		bZigZag = TRUE;
		nLineStartPosi = nPointSpace;
		PointSize.P1.x = -max(1, nLineSpace/5); 
		PointSize.P1.y = -max(1, nLineSpace/5);  
		PointSize.P2.x =  max(1, nLineSpace/5); 
		PointSize.P2.y =  max(1, nLineSpace/5);  
		nLineSpace  *= 2;
		break;
	case EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL: // 수평으로 빗금 + 점(사각형3:2)
		nLineDir   = 1;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		break;
	case EN_HATCHING_TYPE_RECTDOTS_VERTICAL: // 수직으로 빗금 + 점(사각형3:2)
		nLineDir   = 2;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		break;
	case EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL: // 45도 우측에서 아래로 빗금 + 점(사각형3:2)
		nLineDir   = 3;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		break;
	case EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL:  // 45도 좌측에서 아래로 빗금 + 점(사각형3:2) 
		nLineDir   = 4;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		break;
	case EN_HATCHING_TYPE_RECTDOTS_HORIZONTAL_S: // 수평으로 빗금 + 점(사각형3:2) Small
		nLineDir   = 1;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;		
		PointSize.P1.x = -max(1, nLineSpace/8); 
		PointSize.P1.y = -max(1, (int)(nLineSpace/12));  
		PointSize.P2.x =  max(1, nLineSpace/8); 
		PointSize.P2.y =  max(1, (int)(nLineSpace/12));  
		break;
	case EN_HATCHING_TYPE_RECTDOTS_VERTICAL_S: // 수직으로 빗금 + 점(사각형3:2) Small
		nLineDir   = 2;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = -max(1, nLineSpace/8);
		PointSize.P1.y = -max(1, (int)(nLineSpace/12));  
		PointSize.P2.x =  max(1, nLineSpace/8); 
		PointSize.P2.y =  max(1, (int)(nLineSpace/12));  
		break;
	case EN_HATCHING_TYPE_RECTDOTS_BDIAGONAL_S: // 45도 우측에서 아래로 빗금 + 점(사각형3:2) Small
		nLineDir   = 3;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = -max(1, nLineSpace/8);
		PointSize.P1.y = -max(1, (int)(nLineSpace/12));  
		PointSize.P2.x =  max(1, nLineSpace/8); 
		PointSize.P2.y =  max(1, (int)(nLineSpace/12));  
		break;
	case EN_HATCHING_TYPE_RECTDOTS_FDIAGONAL_S:  // 45도 좌측에서 아래로 빗금 + 점(사각형3:2) Small 
		nLineDir   = 4;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		PointSize.P1.x = -max(1, nLineSpace/8); 
		PointSize.P1.y = -max(1, (int)(nLineSpace/12));  
		PointSize.P2.x =  max(1, nLineSpace/8); 
		PointSize.P2.y =  max(1, (int)(nLineSpace/12));  
		break;
	case EN_HATCHING_TYPE_BDIAGONAL3_FDIAGONAL: // 45도 우측에서 아래로 빗금 3 + 45도 좌측에서 아래로 빗금
		nLineDir   = 3;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		nZigZagType = 6;
		nPointSpace     = nSpace/2;
		nPointSpace_Sub = nSpace/2;
		nLineSpace      = nPointSpace*2;
		PointSize.P1.x = 0;            
		PointSize.P1.y = 0;  
		PointSize.P2.x = nPointSpace; 
		PointSize.P2.y = nPointSpace * (m_bEMF ? -1 : 1);  
		break;
	case EN_HATCHING_TYPE_FDIAGONAL3_BDIAGONAL: // 45도 좌측에서 아래로 빗금 3 + 45도 우측에서 아래로 빗금
		nLineDir   = 4;
		bLine  = TRUE;
		bPoint  = TRUE;
		bZigZag = TRUE;
		nZigZagType = 16;
		nPointSpace     = nSpace/2;
		nPointSpace_Sub = nSpace/2;
		nLineSpace      = nPointSpace*2;
		PointSize.P1.x = 0;             
		PointSize.P1.y = 0;  
		PointSize.P2.x = -nPointSpace;  
		PointSize.P2.y = nPointSpace * (m_bEMF ? -1 : 1);  
		break;
	case EN_HATCHING_TYPE_GRAVEL_DOTS: // 자갈무늬 + 재부점
		bLine = FALSE;
		bPoint  = TRUE;
		bZigZag = FALSE;
		PointSize.P1.x = -2*nPointSpace; 
		PointSize.P1.y = -2*nPointSpace;  
		PointSize.P2.x =  2*nPointSpace; 
		PointSize.P2.y =  2*nPointSpace;  
		nPointSpace = nPointSpace_Sub = 4*nPointSpace;			
		break;
	default:
		ASSERT(0);
		break;
	}
}

BOOL CDrawFunc::GetCrossLine_Rect(T_LINE& Rect, POINT sPoint, int nDir, T_LINE& CrossLine, int nRectLineWidth)
{
	// nDir (1:수평, 2:수직, 3:45도 우측에서 아래로, 4:45도 좌측에서 아래로)
	int nXmax = max(Rect.P1.x, Rect.P2.x);
	int nXmin = min(Rect.P1.x, Rect.P2.x);
	int nYmax = max(Rect.P1.y, Rect.P2.y);
	int nYmin = min(Rect.P1.y, Rect.P2.y);

	if(!m_bRectTrPolyDraw)
	{
		nXmax -= max(1, nRectLineWidth/2);
		if(m_bEMF) nYmin += max(1, nRectLineWidth/2);
		else       nYmax -= max(1, nRectLineWidth/2);
	}

	if(nXmax==nXmin || nYmax==nYmin) return FALSE;

	if(nDir == 1)
	{
		if(nYmax <= sPoint.y || nYmin >= sPoint.y) return FALSE;
		CrossLine.P1.y = CrossLine.P2.y = sPoint.y;
		CrossLine.P1.x = nXmin;
		CrossLine.P2.x = nXmax;
	}
	else if(nDir == 2)
	{
		if(nXmax <= sPoint.x || nXmin >= sPoint.x) return FALSE;
		CrossLine.P1.x = CrossLine.P2.x = sPoint.x;
		CrossLine.P1.y = (m_bEMF) ? nYmax : nYmin;
		CrossLine.P2.y = (m_bEMF) ? nYmin : nYmax;
	}
	else if(nDir == 3 || nDir == 4)
	{
		int nMaxRectSize = max(1, max(nXmax-nXmin, nYmax-nYmin));
		double pCurrLine[2][2];
		double pBound1[2], pBound2[2], pCross[2]; 
		pCurrLine[0][0] = sPoint.x;
		pCurrLine[0][1] = sPoint.y;
		if(nDir == 3)
		{			
			pCurrLine[1][0] = sPoint.x - nMaxRectSize;
			pCurrLine[1][1] = sPoint.y + ((m_bEMF) ? -nMaxRectSize : nMaxRectSize);
		}
		else 
		{			
			pCurrLine[1][0] = sPoint.x + nMaxRectSize;
			pCurrLine[1][1] = sPoint.y + ((m_bEMF) ? -nMaxRectSize : nMaxRectSize);
		}

		int nFindCount = 0;
		for(int i = 0; i < 4; i++)
		{			
			if     (i==0) { pBound1[0]=nXmax;  pBound1[1]=nYmax;  pBound2[0]=nXmax;  pBound2[1]=nYmin; } // Xmax
			else if(i==1) { pBound1[0]=nXmin;  pBound1[1]=nYmax;  pBound2[0]=nXmin;  pBound2[1]=nYmin; } // Xmin
			else if(i==2) { pBound1[0]=nXmin;  pBound1[1]=nYmax;  pBound2[0]=nXmax;  pBound2[1]=nYmax; } // Ymax
			else if(i==3) { pBound1[0]=nXmin;  pBound1[1]=nYmin;  pBound2[0]=nXmax;  pBound2[1]=nYmin; } // Ymin

			if(CMathFunc::mathLineSegCross2D(pCurrLine, pBound1, pBound2, pCross) == 1)
			{
				if(nFindCount == 0)
				{
					CrossLine.P1.x = (long)(pCross[0]);
					CrossLine.P1.y = (long)(pCross[1]);
					nFindCount++;
				}
				else
				{
					if(CrossLine.P1.x == (long)(pCross[0]) && CrossLine.P1.y == (long)(pCross[1])) continue;
					CrossLine.P2.x = (long)(pCross[0]);
					CrossLine.P2.y = (long)(pCross[1]);
					nFindCount++;
					break;
				}
			}
		}
		if(nFindCount != 2) return FALSE;
	}
	else return FALSE;

	return TRUE;
}

BOOL CDrawFunc::GetCrossArc_Rect(T_LINE& Rect,  POINT cPoint, int nRadius, nrT_ARC& CrossArcList, int nRectLineWidth)
{
	int nXmax = max(Rect.P1.x, Rect.P2.x);
	int nXmin = min(Rect.P1.x, Rect.P2.x);
	int nYmax = max(Rect.P1.y, Rect.P2.y);
	int nYmin = min(Rect.P1.y, Rect.P2.y);

	if(!m_bRectTrPolyDraw)
	{
		nXmax -= max(1, nRectLineWidth/2);
		if(m_bEMF) nYmin += max(1, nRectLineWidth/2);
		else       nYmax -= max(1, nRectLineWidth/2);
	}

	if(nXmax == nXmin || nYmax == nYmin) return FALSE;

	if(cPoint.x <= nXmin-nRadius || cPoint.x >= nXmax+nRadius || cPoint.y <= nYmin-nRadius || cPoint.y >= nYmax+nRadius) return FALSE;

	CrossArcList.RemoveAll();

	BOOL bContXmin =  cPoint.x < nXmin+nRadius;
	BOOL bContXmax =  cPoint.x > nXmax-nRadius;
	BOOL bContYmin =  cPoint.y < nYmin+nRadius;
	BOOL bContYmax =  cPoint.y > nYmax-nRadius;

	if(!bContXmin && !bContXmax && !bContYmin && !bContYmax)
	{ 
		T_ARC CrossArc;  
		CrossArc.StartAngle = 0.0;  
		CrossArc.SweepAngle = 360.0;
		CrossArcList.Add(CrossArc);
		return TRUE;
	}

	double dXmax = nXmax;
	double dXmin = nXmin;
	double dYmax = nYmax;
	double dYmin = nYmin;
	nrT_POINT arPoint;
	T_POINT   DgnPointD;
	double dDis;
	if(bContXmin)
	{
		dDis = sqrt((double)(nRadius*nRadius - (nXmin-cPoint.x)*(nXmin-cPoint.x)));
		DgnPointD.x = dXmin;
		DgnPointD.y = cPoint.y + dDis;
		if(DgnPointD.y > dYmin && DgnPointD.y < dYmax) arPoint.Add(DgnPointD);
		DgnPointD.y = cPoint.y - dDis;
		if(DgnPointD.y > dYmin && DgnPointD.y < dYmax) arPoint.Add(DgnPointD);
	}
	if(bContXmax)
	{
		dDis = sqrt((double)(nRadius*nRadius - (nXmax-cPoint.x)*(nXmax-cPoint.x)));
		DgnPointD.x = dXmax;
		DgnPointD.y = cPoint.y + dDis;
		if(DgnPointD.y > dYmin && DgnPointD.y < dYmax) arPoint.Add(DgnPointD);
		DgnPointD.y = cPoint.y - dDis;
		if(DgnPointD.y > dYmin && DgnPointD.y < dYmax) arPoint.Add(DgnPointD);
	}
	if(bContYmin)
	{
		dDis = sqrt((double)(nRadius*nRadius - (nYmin-cPoint.y)*(nYmin-cPoint.y)));
		DgnPointD.x = cPoint.x + dDis;
		DgnPointD.y = dYmin;
		if(DgnPointD.x > dXmin && DgnPointD.x < dXmax) arPoint.Add(DgnPointD);
		DgnPointD.x = cPoint.x - dDis;
		if(DgnPointD.x > dXmin && DgnPointD.x < dXmax) arPoint.Add(DgnPointD);
	}
	if(bContYmax)
	{
		dDis = sqrt((double)(nRadius*nRadius - (nYmax-cPoint.y)*(nYmax-cPoint.y)));
		DgnPointD.x = cPoint.x + dDis;
		DgnPointD.y = dYmax;
		if(DgnPointD.x > dXmin && DgnPointD.x < dXmax) arPoint.Add(DgnPointD);
		DgnPointD.x = cPoint.x - dDis;
		if(DgnPointD.x > dXmin && DgnPointD.x < dXmax) arPoint.Add(DgnPointD);
	}

	int nSize, i, j;	
	CArray<double, double> dAngleList;
	double dtrang = CMathFunc::m_trrad;	
	double vector[2], vectorn[2];	
	double dAngle;
	nSize = arPoint.GetSize();
	dAngleList.SetSize(nSize);
	int nCount = 0;

	for(i = 0; i < nSize; i++)
	{
		vector[0] = arPoint[i].x - cPoint.x;
		vector[1] = (m_bEMF ? arPoint[i].y - cPoint.y : cPoint.y - arPoint[i].y);//vector[1] = cPoint.y - arPoint[i].y;

		if(!CMathFunc::mathNormalize2D(vector, vectorn))
		{
			dAngleList.RemoveAt(dAngleList.GetSize()-1);
			continue;
		}
		dAngle = CMathFunc::mathCrossAngle2DSign(1.0, 0.0, vectorn[0], vectorn[1]);
		if(dAngle < 0) dAngle += 360.0;
		dAngleList[nCount] = dAngle; 
		nCount++;
	}

	nSize = dAngleList.GetSize();
	if(nSize < 2) return FALSE;
	if(nSize % 2 == 1) return FALSE;
	for(i = 0; i < nSize - 1; i++)
	{
		for(j = i + 1; j < nSize ; j++)
		{
			if(dAngleList[i] > dAngleList[j])
			{
				dAngle = dAngleList[i];
				dAngleList[i] = dAngleList[j];
				dAngleList[j] = dAngle;				
			}
		}
	}

	if(fabs(dAngleList[0] - dAngleList[nSize-1]) < 0.1)
	{ 
		dAngleList.RemoveAt(nSize-1);
		dAngleList.RemoveAt(0);
	}
	for(i = nSize - 2; i >= 0; i--)
	{		
		if(fabs(dAngleList[i] - dAngleList[i+1]) < 0.1)
		{
			dAngleList.RemoveAt(i);
			dAngleList.RemoveAt(i + 1);
			i--;
		}		
	}

	BOOL bArcEx = FALSE;
	if(dAngleList[0] != 0.0) 
	{ 
		bArcEx = (cPoint.x+nRadius >= nXmin && cPoint.x+nRadius <= nXmax && cPoint.y >= nYmin && cPoint.y <= nYmax); 
	}
	else
	{
		if(m_bEMF) 
		{
			bArcEx = (cPoint.x+nRadius >= nXmin && cPoint.x+nRadius <= nXmax && cPoint.y-1 >= nYmin && cPoint.y-1 <= nYmax); 
		}
		else 
		{
			bArcEx = (cPoint.x+nRadius >= nXmin && cPoint.x+nRadius <= nXmax && cPoint.y+1 >= nYmin && cPoint.y+1 <= nYmax); 
		}
	}


	T_ARC CrossArc;
	CrossArc.CenterP = cPoint;
	CrossArc.Radius  = nRadius;
	for(i = 0; i < nSize ; i++)
	{
		if(bArcEx)
		{
			if(i==0) 
			{
				CrossArc.StartAngle = (float)(dAngleList[nSize-1]);
				CrossArc.SweepAngle = (float)(dAngleList[i] + (360.0-CrossArc.StartAngle));
			}
			else     
			{
				CrossArc.StartAngle = (float)(dAngleList[i-1]);
				CrossArc.SweepAngle = (float)(dAngleList[i] - CrossArc.StartAngle);
			}
			CrossArcList.Add(CrossArc);
			bArcEx = FALSE;
		}
		else 
		{
			bArcEx = TRUE;		
		}
	}

	return (CrossArcList.GetSize() > 0);
}

BOOL CDrawFunc::GetCrossPolyline_Rect(T_LINE& Rect, nrT_POLYLINE& arPoly, nrT_POLYLINE& arPolygonOut, nrT_POLYLINE& arPolylineOut, int nRectLineWidth)
{
	arPolygonOut.RemoveAll();
	arPolylineOut.RemoveAll();

	int nXmax = max(Rect.P1.x, Rect.P2.x);
	int nXmin = min(Rect.P1.x, Rect.P2.x);
	int nYmax = max(Rect.P1.y, Rect.P2.y);
	int nYmin = min(Rect.P1.y, Rect.P2.y);

	if(!m_bRectTrPolyDraw)
	{
		nXmax -= max(1, nRectLineWidth/2);

		if(m_bEMF)
		{
			nYmin += max(1, nRectLineWidth/2);
		}
		else
		{
			nYmax -= max(1, nRectLineWidth/2);
		}
	}

	if(nXmax == nXmin || nYmax == nYmin) return FALSE;

	int i, j, k, m, nSize, nPolySize, nCrossSize, nCurrXmax, nCurrXmin, nCurrYmax, nCurrYmin, nCrossID, nCrossNextID;
	CArray<int, int> arCrossID;
	nrPOINT arCrossPoint;
	T_POLYLINE PolyD, CurrPolyD;
	POINT PointD1, PointD2, CrossPointD; //, CurrPointD;

	double pXmax1[2], pXmax2[2], pXmin1[2], pXmin2[2], pYmax1[2], pYmax2[2], pYmin1[2], pYmin2[2], pCurr1[2], pCurr2[2], pBound1[2], pBound2[2], pCross[2]; 
	double pCurrLine[2][2];
	pXmax1[0] = pXmax2[0] = nXmax;  pXmax1[1] = nYmin;  pXmax2[1] = nYmax;
	pXmin1[0] = pXmin2[0] = nXmin;  pXmin1[1] = nYmin;  pXmin2[1] = nYmax;
	pYmax1[1] = pYmax2[1] = nYmax;  pYmax1[0] = nXmin;  pYmax2[0] = nXmax;
	pYmin1[1] = pYmin2[1] = nYmin;  pYmin1[0] = nXmin;  pYmin2[0] = nXmax;

	nSize = arPoly.GetSize();
	for(i = 0; i < nSize ; i++)
	{
		PolyD = arPoly[i];
		nPolySize = PolyD.arPoint.GetSize();
		if(nPolySize <= 0) continue;
		nCurrXmax = nCurrXmin = PolyD.arPoint[0].x;
		nCurrYmax = nCurrYmin = PolyD.arPoint[0].y;

		for(j = 1; j < nPolySize ; j++)
		{
			nCurrXmax = max(nCurrXmax, PolyD.arPoint[j].x);
			nCurrXmin = min(nCurrXmin, PolyD.arPoint[j].x);
			nCurrYmax = max(nCurrYmax, PolyD.arPoint[j].y);
			nCurrYmin = min(nCurrYmin, PolyD.arPoint[j].y);
		}

		if(nXmax >= nCurrXmax && nXmin <= nCurrXmin && nYmax >= nCurrYmax && nYmin <= nCurrYmin)
		{
			arPolygonOut.Add(PolyD);
		}
		else if(nXmax <= nCurrXmin || nXmin >= nCurrXmax || nYmax <= nCurrYmin || nYmin >= nCurrYmax)
		{
			continue;
		}
		else 
		{
			arCrossID.RemoveAll();

			for(j = 0; j < nPolySize ; j++)
			{
				PointD1 = PolyD.arPoint[j];

				if(j == nPolySize-1)
				{
					PointD2 = PolyD.arPoint[0];
				}
				else
				{
					PointD2 = PolyD.arPoint[j + 1];
				}

				if(nXmax == PointD1.x && (nYmax >= PointD1.y && nYmin <= PointD1.y)) 
				{
					arCrossID.Add(j); 
					continue; 
				}
				else if(nXmin==PointD1.x && (nYmax>=PointD1.y && nYmin<=PointD1.y)) 
				{ 
					arCrossID.Add(j); 
					continue; 
				}
				else if(nYmax==PointD1.y && (nXmax>=PointD1.x && nXmin<=PointD1.x))
				{ 
					arCrossID.Add(j); 
					continue; 
				}
				else if(nYmin==PointD1.y && (nXmax>=PointD1.x && nXmin<=PointD1.x))
				{ 
					arCrossID.Add(j);
					continue; 
				}
				else
				{
					pCurrLine[0][0] = pCurr1[0] = PointD1.x;  
					pCurrLine[0][1] = pCurr1[1] = PointD1.y; 
					pCurrLine[1][0] = pCurr2[0] = PointD2.x; 
					pCurrLine[1][1] = pCurr2[1] = PointD2.y; 
					arCrossPoint.RemoveAll();

					for(k = 0; k < 4; k++)
					{
						if     (k == 0) { pBound1[0] = pXmax1[0];  pBound1[1] = pXmax1[1];  pBound2[0] = pXmax2[0];  pBound2[1] = pXmax2[1]; }
						else if(k == 1) { pBound1[0] = pXmin1[0];  pBound1[1] = pXmin1[1];  pBound2[0] = pXmin2[0];  pBound2[1] = pXmin2[1]; }
						else if(k == 2) { pBound1[0] = pYmax1[0];  pBound1[1] = pYmax1[1];  pBound2[0] = pYmax2[0];  pBound2[1] = pYmax2[1]; }
						else if(k == 3) { pBound1[0] = pYmin1[0];  pBound1[1] = pYmin1[1];  pBound2[0] = pYmin2[0];  pBound2[1] = pYmin2[1]; }

						if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, pCurr1, pCurr2) == 1)
						{
							if(CMathFunc::mathLineSegCross2D(pCurrLine, pBound1, pBound2, pCross) == 1)
							{
								CrossPointD.x = (int)(pCross[0]);  
								CrossPointD.y = (int)(pCross[1]);

								if(CrossPointD.x == PointD1.x && CrossPointD.y == PointD1.y) 
								{
									arCrossID.Add(j); 
									continue; 
								}
								if(CrossPointD.x == PointD2.x && CrossPointD.y == PointD2.y) continue;

								arCrossPoint.Add(CrossPointD);	
								continue;
							}
						}
					}	

					int nCrossSize = arCrossPoint.GetSize();

					if(nCrossSize == 1)
					{
						j++; 
						nPolySize++;

						PolyD.arPoint.InsertAt(j, arCrossPoint[0]);
						arCrossID.Add(j);		
					}
					else if(nCrossSize > 1)
					{
						double dDisK, dDisM;						

						for(k = 0; k < nCrossSize - 1; k++)
						{						
							for(m = k; m < nCrossSize; m++)
							{
								dDisK = (PointD1.x - arCrossPoint[k].x) * (PointD1.x - arCrossPoint[k].x) + (PointD1.y - arCrossPoint[k].y) * (PointD1.y - arCrossPoint[k].y);
								dDisM = (PointD1.x - arCrossPoint[m].x) * (PointD1.x - arCrossPoint[m].x) + (PointD1.y - arCrossPoint[m].y) * (PointD1.y - arCrossPoint[m].y);								

								if(dDisK > dDisM)
								{
									CrossPointD = arCrossPoint[k];
									arCrossPoint[k] = arCrossPoint[m];
									arCrossPoint[m] = CrossPointD;
								}
							}
						}
						for(k = 0; k < nCrossSize; k++)
						{
							j++; 
							nPolySize++;

							PolyD.arPoint.InsertAt(j, arCrossPoint[k]);
							arCrossID.Add(j);		
						}
					}
				}
			}
			nCrossSize = (int)arCrossID.GetSize();
			if(nCrossSize % 2 == 1 || nCrossSize < 2) continue;

			for(j = 0; j < nCrossSize - 1 ; j++)
			{
				for(k = j + 1; k < nCrossSize ; k++)
				{
					if(arCrossID[j] > arCrossID[k])
					{
						nCrossID = arCrossID[j];
						arCrossID[j] = arCrossID[k];
						arCrossID[k] = nCrossID;				
					}
				}
			}

			if(arCrossID[0] == 0) 
			{
				PointD1 = PolyD.arPoint[nPolySize - 1];
			}
			else 
			{
				PointD1 = PolyD.arPoint[arCrossID[0] - 1];
			}

			PointD2 = PolyD.arPoint[arCrossID[0]];

			double dCurrX = (PointD1.x + PointD2.x)/2.0;
			double dCurrY = (PointD1.y + PointD2.y)/2.0;

			BOOL bLineEx = FALSE;

			if(nXmax > dCurrX && nXmin < dCurrX && nYmax > dCurrY && nYmin < dCurrY)
			{
				bLineEx = FALSE;
			}
			else 
			{
				bLineEx = TRUE;
			}

			for(j = 0; j < nCrossSize; j++)
			{			
				if(bLineEx)
				{				
					CurrPolyD.Init();
					nCrossID = arCrossID[j];	

					if(j == nCrossSize - 1)
					{
						for(k = nCrossID; k < nPolySize ; k++)
						{
							CurrPolyD.arPoint.Add(PolyD.arPoint[k]);
						}

						nCrossID = arCrossID[0];			

						for(k = 0; k <= nCrossID ; k++)
						{
							CurrPolyD.arPoint.Add(PolyD.arPoint[k]);		
						}
					}
					else 
					{
						nCrossNextID = arCrossID[j + 1];			

						for(k = nCrossID; k <= nCrossNextID ; k++)
						{
							CurrPolyD.arPoint.Add(PolyD.arPoint[k]);
						}
					}

					if(CurrPolyD.arPoint.GetSize() > 0) arPolylineOut.Add(CurrPolyD);
					bLineEx = FALSE;
				}
				else 
				{
					bLineEx = TRUE;
				}
			}			
		}
	}

	return (arPolygonOut.GetSize() > 0 || arPolylineOut.GetSize() > 0);
}

BOOL CDrawFunc::GetCrossLine_Poly(T_POLYLINE& PolyD, POINT sPoint, int nDir, int nXmax, int nXmin, int nYmax, int nYmin, nrT_LINE& arCrossLine)
{
	// nDir (1:, 2:, 3:, 4:)
	int nMaxWidth = max(nXmax - nXmin, nYmax - nYmin);
	if(nMaxWidth <= 0) return FALSE;
	if(sPoint.x > nXmax+nMaxWidth || sPoint.x < nXmin-nMaxWidth || sPoint.y > nYmax+nMaxWidth || sPoint.y < nYmin-nMaxWidth) 
	{ 
		ASSERT(0);
		return FALSE;
	}

	T_LINE LineD;
	if(nDir == 1)
	{
		//수평
		LineD.P1.x = sPoint.x-nMaxWidth;
		LineD.P2.x = sPoint.x+nMaxWidth;
		LineD.P1.y = LineD.P2.y = sPoint.y;
	}
	else if(nDir == 2)
	{
		//수직
		LineD.P1.x = LineD.P2.x = sPoint.x;
		LineD.P1.y = sPoint.y-nMaxWidth;
		LineD.P2.y = sPoint.y+nMaxWidth;		
	}
	else if(nDir == 3)
	{
		//45도 우측에서 아래로
		LineD.P1.x = sPoint.x-nMaxWidth;
		LineD.P2.x = sPoint.x+nMaxWidth;
		LineD.P1.y = m_bEMF ? sPoint.y-nMaxWidth : sPoint.y+nMaxWidth;
		LineD.P2.y = m_bEMF ? sPoint.y+nMaxWidth : sPoint.y-nMaxWidth;
	}
	else if(nDir == 4)
	{
		//45도 좌측에서 아래로
		LineD.P1.x = sPoint.x-nMaxWidth;
		LineD.P2.x = sPoint.x+nMaxWidth;
		LineD.P1.y = m_bEMF ? sPoint.y+nMaxWidth : sPoint.y-nMaxWidth;
		LineD.P2.y = m_bEMF ? sPoint.y-nMaxWidth : sPoint.y+nMaxWidth;
	}
	else 
	{
		ASSERT(0); 
		return FALSE;
	}

	return GetCrossLine_Poly(PolyD, LineD, arCrossLine);
}

BOOL CDrawFunc::GetCrossLine_Poly(T_POLYLINE& PolyD, T_LINE LineD, nrT_LINE& arCrossLine)
{
	CArray<int, int> arCrossID;
	return GetCrossLine_Poly(PolyD, LineD, arCrossLine, arCrossID);
}

BOOL CDrawFunc::GetCrossLine_Poly(T_POLYLINE& PolyD, T_LINE LineD, nrT_LINE& arCrossLine, CArray<int, int>& arCrossID)
{
	int nSize = PolyD.arPoint.GetSize();
	arCrossLine.RemoveAll();
	arCrossID.RemoveAll();
	double dLine_i[2], dLine_j[2], pBound1[2], pBound2[2], pCross[2];
	double pCurrLine[2][2];

	pCurrLine[0][0] = dLine_i[0] = (double)(LineD.P1.x);  
	pCurrLine[0][1] = dLine_i[1] = (double)(LineD.P1.y);
	pCurrLine[1][0] = dLine_j[0] = (double)(LineD.P2.x);  
	pCurrLine[1][1] = dLine_j[1] = (double)(LineD.P2.y);

	int i, j;
	for(i = 0; i < nSize; i++)
	{
		if(i == 0)
		{
			pBound1[0] = (double)(PolyD.arPoint[nSize-1].x); 
			pBound1[1] = (double)(PolyD.arPoint[nSize-1].y);
		}

		if(i%2 == 0)
		{
			pBound2[0] = (double)(PolyD.arPoint[i].x);  
			pBound2[1] = (double)(PolyD.arPoint[i].y); 
		}
		else 
		{ 
			pBound1[0] = (double)(PolyD.arPoint[i].x);
			pBound1[1] = (double)(PolyD.arPoint[i].y);
		}

		if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, dLine_i, dLine_j) >= 0) 
		{
			arCrossID.Add(i); 
		}
	}

	nrT_POINT arCrossPoint;
	T_POINT   pointR;
	nSize = arCrossID.GetSize();
	arCrossPoint.SetSize(nSize+2);
	arCrossPoint[0].x = (double)(LineD.P1.x);
	arCrossPoint[0].y = (double)(LineD.P1.y);
	arCrossPoint[1].x = (double)(LineD.P2.x);
	arCrossPoint[1].y = (double)(LineD.P2.y);
	int nCount = 2;
	int nPointSize = nSize;

	for(i = 0; i < nSize ; i++)
	{
		if(arCrossID[i] == 0)
		{
			pBound1[0] = (double)(PolyD.arPoint[nSize-1].x);
			pBound1[1] = (double)(PolyD.arPoint[nSize-1].y); 
		}
		else 
		{
			pBound1[0] = (double)(PolyD.arPoint[arCrossID[i]-1].x);  
			pBound1[1] = (double)(PolyD.arPoint[arCrossID[i]-1].y);
		}

		pBound2[0] = (double)(PolyD.arPoint[arCrossID[i]].x); 
		pBound2[1] = (double)(PolyD.arPoint[arCrossID[i]].y);

		if(CMathFunc::mathLineSegCross2D(pCurrLine, pBound1, pBound2, pCross) == 1)
		{
			arCrossPoint[nCount].x = pCross[0];
			arCrossPoint[nCount].y = pCross[1];
			nCount++;						
		}
		else 
		{
			ASSERT(0);
			arCrossPoint.RemoveAt(nPointSize-1);
			nPointSize--;
		}
	}
	// 좌표거리별로 정렬
	nSize = arCrossPoint.GetSize();
	CArray<double, double> arDis;
	double dDis;
	arDis.SetSize(nSize);

	for(i = 0; i < nSize; i++)
	{
		arDis[i] = sqrt((LineD.P1.x-arCrossPoint[i].x)*(LineD.P1.x-arCrossPoint[i].x) + (LineD.P1.y-arCrossPoint[i].y)*(LineD.P1.y-arCrossPoint[i].y));		
	}

	for(i = 0; i < nSize - 1; i++)
	{
		for(j = i; j < nSize; j++)
		{
			if(arDis[i] > arDis[j])
			{
				pointR = arCrossPoint[i];
				arCrossPoint[i] = arCrossPoint[j];
				arCrossPoint[j] = pointR;
				dDis = arDis[i];
				arDis[i] = arDis[j];
				arDis[j] = dDis;
			}
		}
	}

	// 동일 좌표 제거
	for(i = nSize - 1; i > 0; i--)
	{
		if(fabs(arDis[i]-arDis[i-1]) < m_Zero)
		{
			arCrossPoint.RemoveAt(i);
		}
	}

	nPointSize = PolyD.arPoint.GetSize();
	double targetPt[2];
	double (*coor2D)[2];
	coor2D = new double[nPointSize][2];  		
	for(i = 0; i < nPointSize; i++)
	{
		coor2D[i][0] = (double)(PolyD.arPoint[i].x);
		coor2D[i][1] = (double)(PolyD.arPoint[i].y);
	}

	nSize = arCrossPoint.GetSize();
	T_LINE CurrLine;

	for(i = 0; i < nSize - 1; i++)
	{
		targetPt[0] = (arCrossPoint[i].x + arCrossPoint[i+1].x) / 2.0;
		targetPt[1] = (arCrossPoint[i].y + arCrossPoint[i+1].y) / 2.0;

		if(CMathFunc::mathIsInsidePoint2D(targetPt, nPointSize, coor2D, FALSE))
		{
			CurrLine.P1.x = (long)(arCrossPoint[i].x);
			CurrLine.P1.y = (long)(arCrossPoint[i].y);
			CurrLine.P2.x = (long)(arCrossPoint[i+1].x);
			CurrLine.P2.y = (long)(arCrossPoint[i+1].y);
			arCrossLine.Add(CurrLine);
		}
	}

	delete [] coor2D;
	return (arCrossLine.GetSize() > 0);
}

BOOL CDrawFunc::GetCrossArc_Poly(T_POLYLINE& PolyD,  POINT cPoint, int nRadius, nrT_ARC& CrossArcList)
{	
	CrossArcList.RemoveAll();

	CArray<double, double> arAngleList;
	double dAngle, dSweepAngle, dCrossAngle, dInnerAngle, dDis;
	double pCurrLine[2][2], pBound1[2], pBound2[2], pCross[2], point[3], vector[2], vectorn[2];
	int i, j, nSize, nPointSize;

	nSize = PolyD.arPoint.GetSize();

	// y축이 상향이 +방향인 좌표계로 변환
	nrT_POINT arPoint;
	arPoint.SetSize(nSize);
	for(i = 0; i < nSize ; i++)
	{
		arPoint[i].x = (double)(PolyD.arPoint[i].x);
		arPoint[i].y = (double)(PolyD.arPoint[i].y) * (m_bEMF ? 1.0 : -1.0);
	}

	//
	double dtrang = CMathFunc::m_trrad;
	double dtrrad = CMathFunc::m_trrad;
	pCurrLine[0][0] = point[0] = (double)(cPoint.x);
	pCurrLine[0][1] = point[1] = (double)(cPoint.y);

	for(i = 0; i < nSize ; i++)
	{
		pBound1[0] = arPoint[i].x; 
		pBound1[1] = arPoint[i].y;

		if(i==nSize-1) { pBound2[0] = arPoint[0].x;    pBound2[1] = arPoint[0].y;   }
		else            { pBound2[0] = arPoint[i+1].x;  pBound2[1] = arPoint[i+1].y; }

		vector[0] = pBound2[0]-pBound1[0];  
		vector[1] = pBound2[1]-pBound1[1];

		if(!CMathFunc::mathNormalize2D(vector, vectorn)) continue;

		// 경계선의 직각방향선
		dCrossAngle = CMathFunc::mathCrossAngle2DSign(1.0, 0.0, vectorn[1], vectorn[0]); // 경계선의 직각방향
		dDis = CMathFunc::mathDistanceFromIntersectPointToLine(pBound1, pBound2, point);

		if(dDis >= nRadius) continue;

		if(dDis < m_Zero)
		{
			dAngle = dCrossAngle+90.0;			
			if(dAngle < 0)   dAngle += 360.0;
			if(dAngle >=360) dAngle -= 360.0;
			arAngleList.Add(dAngle);
			dAngle = dCrossAngle-90.0;			
			if(dAngle < 0)   dAngle += 360.0;
			if(dAngle >=360) dAngle -= 360.0;
			arAngleList.Add(dAngle);			
		}
		else 
		{		
			dInnerAngle = acos(dDis/(double)(nRadius)) * dtrang;
			pCurrLine[1][0] = pCurrLine[0][0] * cos(dCrossAngle+dInnerAngle);
			pCurrLine[1][1] = pCurrLine[0][1] * sin(dCrossAngle+dInnerAngle);

			if(CMathFunc::mathLineSegCross2D(pCurrLine, pBound1, pBound2, pCross) == 1)
			{
				vector[0] = pCross[0]-point[0];  
				vector[1] = pCross[1]-point[1];  

				if(!CMathFunc::mathNormalize2D(vector, vectorn)) continue;

				dAngle = CMathFunc::mathCrossAngle2DSign(1.0, 0.0, vectorn[0], vectorn[1]);

				if(dAngle < 0)   dAngle += 360.0;
				if(dAngle >=360) dAngle -= 360.0;

				arAngleList.Add(dAngle);
			}

			pCurrLine[1][0] = pCurrLine[0][0] * cos(dCrossAngle-dInnerAngle);
			pCurrLine[1][1] = pCurrLine[0][1] * sin(dCrossAngle-dInnerAngle);

			if(CMathFunc::mathLineSegCross2D(pCurrLine, pBound1, pBound2, pCross) == 1)
			{
				vector[0] = pCross[0]-point[0];  
				vector[1] = pCross[1]-point[1];  

				if(!CMathFunc::mathNormalize2D(vector, vectorn)) continue;

				dAngle = CMathFunc::mathCrossAngle2DSign(1.0, 0.0, vectorn[0], vectorn[1]);

				if(dAngle < 0)   dAngle += 360.0;
				if(dAngle >=360) dAngle -= 360.0;

				arAngleList.Add(dAngle);
			}
		}		
	}


	// 정렬 및 중복각도 제거
	nSize = arAngleList.GetSize();
	for(i = 0; i < nSize - 1; i++)
	{
		for(j = i + 1; j < nSize ; j++)
		{
			if(arAngleList[i] > arAngleList[j])
			{
				dAngle = arAngleList[i];
				arAngleList[i] = arAngleList[j];
				arAngleList[j] = dAngle;				
			}
		}
	}

	nSize = arAngleList.GetSize();
	for(i = nSize - 1; i >= 1; i--)
	{		
		if(fabs(arAngleList[i] - arAngleList[i-1]) < 0.1)
		{
			arAngleList.RemoveAt(i);
		}		
	}

	// Arc생성
	nPointSize = arPoint.GetSize();
	double targetPt[2];
	double (*coor2D)[2];
	coor2D = new double[nPointSize][2];  		

	for(i = 0; i < nPointSize ; i++)
	{
		coor2D[i][0] = arPoint[i].x;
		coor2D[i][1] = arPoint[i].y;
	}

	nSize = arAngleList.GetSize();	
	T_ARC CurrArc;
	CurrArc.CenterP = cPoint;

	for(i = 0; i < nSize ; i++)
	{
		dAngle = arAngleList[i];
		if(i == nSize - 1)
		{
			dSweepAngle = arAngleList[0] + 360.0 - dAngle;
		}
		else           
		{
			dSweepAngle = arAngleList[i + 1] - dAngle;
		}
		targetPt[0] = point[0] + cos((dAngle+dSweepAngle/2.0)*dtrrad);
		targetPt[1] = point[1] + sin((dAngle+dSweepAngle/2.0)*dtrrad);

		if(CMathFunc::mathIsInsidePoint2D(targetPt, nPointSize, coor2D, FALSE))
		{
			CurrArc.StartAngle = (float)(dAngle);
			CurrArc.SweepAngle = (float)(dSweepAngle);
			CrossArcList.Add(CurrArc);
		}
	}

	delete [] coor2D;

	return (CrossArcList.GetSize() > 0);
}

BOOL CDrawFunc::GetCrossPolyline_Poly(T_POLYLINE& PolyD_In, nrT_POLYLINE& arPoly, nrT_POLYLINE& arPolygonOut, nrT_POLYLINE& arPolylineOut, BOOL bOnlyPolygon)
{	
	arPolygonOut.RemoveAll();
	arPolylineOut.RemoveAll();

	int i, nXmax, nXmin, nYmax, nYmin;
	int nOutPolySize = PolyD_In.arPoint.GetSize();
	for(i = 0; i < nOutPolySize; i++)
	{
		if(i == 0) 
		{ 
			nXmax = nXmin = PolyD_In.arPoint[i].x; 
			nYmax = nYmin = PolyD_In.arPoint[i].y; 
		}
		else 
		{
			nXmax = max(nXmax, PolyD_In.arPoint[i].x);
			nXmin = min(nXmin, PolyD_In.arPoint[i].x);
			nYmax = max(nYmax, PolyD_In.arPoint[i].y);
			nYmin = min(nYmin, PolyD_In.arPoint[i].y);
		}
	}

	if(nXmax == nXmin || nYmax == nYmin) return FALSE;

	double targetPt[2];
	double (*coor2D)[2];
	coor2D = new double[nOutPolySize][2];  	 		
	for(i = 0; i < nOutPolySize ; i++)
	{
		coor2D[i][0] = (double)(PolyD_In.arPoint[i].x);
		coor2D[i][1] = (double)(PolyD_In.arPoint[i].y);
	}	

	int nSize, j, k, m, n, nPolySize, nCrossSize, nCurrXmax, nCurrXmin, nCurrYmax, nCurrYmin, nCrossID, nCrossNextID;
	nrT_EX_CROSS_ID arCrossID;
	T_EX_CROSS_ID Cross_ID;
	CArray<int, int> arCrossPointOutID;
	nrPOINT arCrossPoint;
	T_POLYLINE PolyD, CurrPolyD;
	POINT PointD1, PointD2, CrossPointD, CurrPointD;

	double pCurr1[2], pCurr2[2], pBound1[2], pBound2[2], pCross[2]; 
	double pCurrLine[2][2];

	nSize = arPoly.GetSize();
	for(i = 0; i < nSize ; i++)
	{		
		PolyD = arPoly[i];
		nPolySize = PolyD.arPoint.GetSize();

		if(nPolySize <= 0) continue;

		nCurrXmax = nCurrXmin = PolyD.arPoint[0].x;
		nCurrYmax = nCurrYmin = PolyD.arPoint[0].y;

		for(j = 1; j < nPolySize ; j++)
		{
			nCurrXmax = max(nCurrXmax, PolyD.arPoint[j].x);
			nCurrXmin = min(nCurrXmin, PolyD.arPoint[j].x);
			nCurrYmax = max(nCurrYmax, PolyD.arPoint[j].y);
			nCurrYmin = min(nCurrYmin, PolyD.arPoint[j].y);
		}

		if(nXmax <= nCurrXmin || nXmin >= nCurrXmax || nYmax <= nCurrYmin || nYmin >= nCurrYmax) continue;

		arCrossID.RemoveAll();
		for(j = 0; j < nPolySize ; j++)
		{
			PointD1 = PolyD.arPoint[j];

			if(j == nPolySize - 1) 
			{
				PointD2 = PolyD.arPoint[0];
			}
			else    
			{
				PointD2 = PolyD.arPoint[j + 1];
			}

			pCurrLine[0][0] = pCurr1[0] = PointD1.x; 
			pCurrLine[0][1] = pCurr1[1] = PointD1.y; 
			pCurrLine[1][0] = pCurr2[0] = PointD2.x; 
			pCurrLine[1][1] = pCurr2[1] = PointD2.y; 

			arCrossPoint.RemoveAll();
			arCrossPointOutID.RemoveAll();

			for(k = 0; k < nOutPolySize ; k++)
			{
				pBound1[0] = coor2D[k][0]; 
				pBound1[1] = coor2D[k][1];

				if(nOutPolySize - 1 == 0) 
				{ 
					pBound2[0] = coor2D[0][0]; 
					pBound2[1] = coor2D[0][1];
				}
				else                    
				{
					pBound2[0] = coor2D[i+1][0];
					pBound2[1] = coor2D[i+1][1]; 
				}

				if(CMathFunc::mathIntersect_ccw2D(pBound1, pBound2, pCurr1, pCurr2) == 1)
				{
					if(CMathFunc::mathLineSegCross2D(pCurrLine, pBound1, pBound2, pCross) == 1)
					{
						CrossPointD.x = (int)(pCross[0]);  CrossPointD.y = (int)(pCross[1]);

						if(CrossPointD.x == PointD1.x && CrossPointD.y == PointD1.y) 
						{
							Cross_ID.nID = j;  Cross_ID.nBoundID = k;  
							arCrossID.Add(Cross_ID);  
							continue; 
						}

						if(CrossPointD.x == PointD2.x && CrossPointD.y == PointD2.y) continue;

						arCrossPoint.Add(CrossPointD);	
						arCrossPointOutID.Add(k);
						continue;
					}
				}
			}	

			int nCrossSize = arCrossPoint.GetSize();
			if(nCrossSize == 1)
			{
				j++; 
				nPolySize++;

				PolyD.arPoint.InsertAt(j, arCrossPoint[0]);
				Cross_ID.nID = j; 
				Cross_ID.nBoundID = arCrossPointOutID[0];
				arCrossID.Add(Cross_ID);
			}
			else if(nCrossSize > 1)
			{
				double dDisK, dDisM;						

				for(k = 0; k < nCrossSize - 1; k++)
				{						
					for(m = k; m < nCrossSize; m++)
					{
						dDisK = (PointD1.x-arCrossPoint[k].x)*(PointD1.x-arCrossPoint[k].x) + (PointD1.y-arCrossPoint[k].y)*(PointD1.y-arCrossPoint[k].y);
						dDisM = (PointD1.x-arCrossPoint[m].x)*(PointD1.x-arCrossPoint[m].x) + (PointD1.y-arCrossPoint[m].y)*(PointD1.y-arCrossPoint[m].y);								
						if(dDisK > dDisM)
						{
							CrossPointD = arCrossPoint[k];
							arCrossPoint[k] = arCrossPoint[m];
							arCrossPoint[m] = CrossPointD;
						}
					}
				}
				for(k = 0; k < nCrossSize ; k++)
				{
					j++;  
					nPolySize++;

					PolyD.arPoint.InsertAt(j, arCrossPoint[k]);

					Cross_ID.nID = j;
					Cross_ID.nBoundID = arCrossPointOutID[k];
					arCrossID.Add(Cross_ID);
				}
			}
		}
		nCrossSize = arCrossID.GetSize();

		if(nCrossSize < 2)
		{
			int nChkID = 0;

			if(nCrossSize != 0)
			{ 
				if(nChkID < nPolySize - 1)
				{
					nChkID = arCrossID[0].nID + 1;
				}
			}

			targetPt[0] = PolyD.arPoint[nChkID].x;
			targetPt[1] = PolyD.arPoint[nChkID].y;

			if(CMathFunc::mathIsInsidePoint2D(targetPt, nOutPolySize, coor2D, TRUE))
			{
				arPolygonOut.Add(PolyD);
				continue;
			}
			else 
			{ 
				continue;
			}
		}
		if(nCrossSize % 2 == 1) continue;

		for(j = 0; j < nCrossSize - 1; j++)
		{
			for(k = j + 1; k < nCrossSize; k++)
			{
				if(arCrossID[j].nID > arCrossID[k].nID)
				{
					Cross_ID = arCrossID[j];
					arCrossID[j] = arCrossID[k];
					arCrossID[k] = Cross_ID;		
				}
			}
		}

		if(arCrossID[0].nID == 0) PointD1 = PolyD.arPoint[nPolySize - 1];
		else                       PointD1 = PolyD.arPoint[arCrossID[0].nID - 1];

		PointD2 = PolyD.arPoint[arCrossID[0].nID];
		targetPt[0] = (PointD1.x + PointD2.x) / 2.0;
		targetPt[1] = (PointD1.y + PointD2.y) / 2.0;

		BOOL bLineEx = (!CMathFunc::mathIsInsidePoint2D(targetPt, nOutPolySize, coor2D, TRUE));

		nrT_EX_POLYLINE arCurrPolyEx;

		for(j = 0; j < nCrossSize ; j++)
		{			
			if(bLineEx)
			{				
				CurrPolyD.Init();
				nCrossID = arCrossID[j].nID;	
				if(j == nCrossSize - 1)
				{
					for(k = nCrossID; k < nPolySize ; k++)
					{
						CurrPolyD.arPoint.Add(PolyD.arPoint[k]);
					}
					nCrossNextID = arCrossID[0].nID;			
					for(k = 0; k <= nCrossNextID ; k++)
					{
						CurrPolyD.arPoint.Add(PolyD.arPoint[k]);		
					}
				}
				else 
				{
					nCrossNextID = arCrossID[j + 1].nID;			
					for(k = nCrossID; k <= nCrossNextID; k++)
					{
						CurrPolyD.arPoint.Add(PolyD.arPoint[k]);
					}
				}

				if(CurrPolyD.arPoint.GetSize()>0) 
				{
					if(!bOnlyPolygon) 
					{ 
						arPolylineOut.Add(CurrPolyD); 
					}
					else 
					{
						T_EX_POLYLINE CurrPolyEx;
						CurrPolyEx.Poly = CurrPolyD;
						CurrPolyEx.StartID = arCrossID[j];
						CurrPolyEx.EndID = (j == nCrossSize-1) ? arCrossID[0] : arCrossID[j+1];	
						arCurrPolyEx.Add(CurrPolyEx);
					}
				}
				bLineEx = FALSE;
			}
			else 
			{
				bLineEx = TRUE;				
			}
		}		

		if(bOnlyPolygon)
		{
			nrT_EX_CROSS_ID arBoundID;			
			nrT_EX_POLYLINE arCurrPolyExBound;
			T_EX_POLYLINE CurrPolyEx;
			nCrossSize = arCrossID.GetSize();
			arBoundID.SetSize(nCrossSize);

			for(j = 0; j < nCrossSize; j++)
			{
				arBoundID[j] = arCrossID[j];
			}

			double dDisP1, dDisP2;
			// Bound Poly의 ID로 정렬			
			for(j = 0; j < nCrossSize - 1; j++)
			{
				for(k = j + 1; k < nCrossSize; k++)
				{
					if(arCrossID[j].nBoundID == arCrossID[k].nBoundID)
					{
						CurrPointD = PolyD_In.arPoint[arCrossID[k].nBoundID];
						PointD1 = PolyD.arPoint[arCrossID[j].nID];
						PointD2 = PolyD.arPoint[arCrossID[k].nID];
						dDisP1  = (CurrPointD.x-PointD1.x)*(CurrPointD.x-PointD1.x)+(CurrPointD.y-PointD1.y)*(CurrPointD.y-PointD1.y);
						dDisP2  = (CurrPointD.x-PointD2.x)*(CurrPointD.x-PointD2.x)+(CurrPointD.y-PointD2.y)*(CurrPointD.y-PointD2.y);
						if(dDisP1 > dDisP2)
						{
							Cross_ID = arBoundID[j];
							arBoundID[j] = arBoundID[k];
							arBoundID[k] = Cross_ID;	
						}						
					}
					if(arCrossID[j].nBoundID > arCrossID[k].nBoundID)
					{
						Cross_ID = arBoundID[j];
						arBoundID[j] = arBoundID[k];
						arBoundID[k] = Cross_ID;		
					}
				}
			}

			// Bound Poly의 경계선 산출
			double (*coor2D_Curr)[2];
			nPolySize = PolyD.arPoint.GetSize();
			coor2D_Curr = new double[nPolySize][2];  	 		

			for(j = 0; j < nPolySize ; j++)
			{
				coor2D_Curr[j][0] = (double)(PolyD.arPoint[j].x);
				coor2D_Curr[j][1] = (double)(PolyD.arPoint[j].y);
			}	

			if(arBoundID[0].nID == 0) PointD1 = PolyD.arPoint[nPolySize - 1];
			else                       PointD1 = PolyD.arPoint[arBoundID[0].nID - 1];

			PointD2 = PolyD.arPoint[arBoundID[0].nID];
			targetPt[0] = (PointD1.x + PointD2.x)/2.0;
			targetPt[1] = (PointD1.y + PointD2.y)/2.0;
			bLineEx = (!CMathFunc::mathIsInsidePoint2D(targetPt, nPolySize, coor2D_Curr, TRUE));
			delete [] coor2D_Curr;

			for(j = 0; j < nCrossSize ; j++)
			{	
				if(bLineEx)
				{				
					CurrPolyD.Init();
					nCrossID = arBoundID[j].nBoundID;	
					CurrPolyD.arPoint.Add(PolyD.arPoint[arBoundID[j].nID]);
					
					if(j == nCrossSize - 1)
					{
						for(k = nCrossID + 1; k < nOutPolySize ; k++)
						{
							CurrPolyD.arPoint.Add(PolyD_In.arPoint[k]);
						}
						nCrossNextID = arBoundID[0].nBoundID;			
						for(k = 0; k <= nCrossNextID ; k++)
						{
							CurrPolyD.arPoint.Add(PolyD_In.arPoint[k]);		
						}

						PointD2 = PolyD.arPoint[arBoundID[0].nID];
					}
					else 
					{
						nCrossNextID = arBoundID[j + 1].nBoundID;			
						for(k = nCrossID + 1; k <= nCrossNextID ; k++)
						{
							CurrPolyD.arPoint.Add(PolyD.arPoint[k]);
						}

						PointD2 = PolyD.arPoint[arBoundID[j+1].nID];
					}
					PointD1 = CurrPolyD.arPoint[CurrPolyD.arPoint.GetSize() - 1];
					
					if(PointD1.x != PointD2.x && PointD1.y != PointD2.y)
					{
						CurrPolyD.arPoint.Add(PointD2);
					}

					if(CurrPolyD.arPoint.GetSize() > 0) 
					{						
						CurrPolyEx;
						CurrPolyEx.Poly = CurrPolyD;
						CurrPolyEx.StartID = arBoundID[j];
						CurrPolyEx.EndID   = (j == nCrossSize-1) ? arBoundID[0] : arBoundID[j+1];	
						arCurrPolyExBound.Add(CurrPolyEx);						
					}
					bLineEx = FALSE;
				}
				else bLineEx = TRUE;				
			}

			int nCurrPolySize  = arCurrPolyEx.GetSize();
			int nBoundPolySize = arCurrPolyExBound.GetSize();
			CurrPolyD.Init();
			BOOL bCurrFind;			
			for(j = 0; j < nCurrPolySize ; j++)
			{
				CurrPolyEx = arCurrPolyEx[j];
				bCurrFind = FALSE;

				if(CurrPolyEx.Poly.arPoint.GetSize() < 1) continue;

				for(k=0 ; k<nCurrPolySize ; k++)// 검색 최대 반복횟수는 남아 있는 Curr PolyLine의 수
				{
					if(bCurrFind)
					{
						// arCurrPolyEx에서 연결될 Polyline을 검색
						for(m = j + 1; m < nCurrPolySize ; m++)
						{
							if(CurrPolyEx.EndID.nID == arCurrPolyEx[m].StartID.nID)
							{
								nPolySize = arCurrPolyEx[m].Poly.arPoint.GetSize();

								for(n = 1; n < nPolySize - 1; n++)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyEx[m].Poly.arPoint[n]);
								}
								if(CurrPolyEx.StartID.nID != arCurrPolyEx[m].EndID.nID)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyEx[m].Poly.arPoint[nPolySize-1]);
								}
								CurrPolyEx.EndID = arCurrPolyEx[m].EndID;
								arCurrPolyEx.RemoveAt(m);
								nCurrPolySize--;
								break;
							}
							else if(CurrPolyEx.EndID.nID == arCurrPolyEx[m].EndID.nID)
							{
								nPolySize = arCurrPolyEx[m].Poly.arPoint.GetSize();
								for(n=nPolySize-2 ; n>=1 ; n--)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyEx[m].Poly.arPoint[n]);
								}
								if(CurrPolyEx.StartID.nID != arCurrPolyEx[m].StartID.nID)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyEx[m].Poly.arPoint[0]);
								}
								CurrPolyEx.EndID = arCurrPolyEx[m].StartID;
								arCurrPolyEx.RemoveAt(m);
								nCurrPolySize--;
								break;
							}
						}
						bCurrFind = FALSE;
					}
					else 
					{
						// arCurrPolyExBound 연결될 Polyline을 검색
						for(m = 0; m < nBoundPolySize ; m++)
						{
							if(CurrPolyEx.EndID.nID == arCurrPolyExBound[m].StartID.nID)
							{
								nPolySize = arCurrPolyExBound[m].Poly.arPoint.GetSize();
								for(n=1 ; n<nPolySize-1 ; n++)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyExBound[m].Poly.arPoint[n]);
								}
								if(CurrPolyEx.StartID.nID != arCurrPolyExBound[m].EndID.nID)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyExBound[m].Poly.arPoint[nPolySize-1]);
								}
								CurrPolyEx.EndID = arCurrPolyExBound[m].EndID;
								arCurrPolyExBound.RemoveAt(m);
								nBoundPolySize--;
								break;
							}
							else if(CurrPolyEx.EndID.nID == arCurrPolyExBound[m].EndID.nID)
							{
								nPolySize = arCurrPolyExBound[m].Poly.arPoint.GetSize();
								for(n = nPolySize - 2; n >= 1; n--)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyExBound[m].Poly.arPoint[n]);
								}
								if(CurrPolyEx.StartID.nID != arCurrPolyExBound[m].StartID.nID)
								{
									CurrPolyEx.Poly.arPoint.Add(arCurrPolyExBound[m].Poly.arPoint[0]);
								}
								CurrPolyEx.EndID = arCurrPolyExBound[m].StartID;
								arCurrPolyExBound.RemoveAt(m);
								nBoundPolySize--;
								break;
							}
						}
						bCurrFind = TRUE;
					}

					PointD1 = CurrPolyEx.Poly.arPoint[0];
					PointD2 = CurrPolyEx.Poly.arPoint[CurrPolyEx.Poly.arPoint.GetSize() - 1];

					if(CurrPolyEx.StartID.nID == CurrPolyEx.EndID.nID || (PointD1.x==PointD2.x && PointD1.y==PointD2.y))
					{
						arPolygonOut.Add(CurrPolyEx.Poly);
						break;
					}
				}
			}			
		}
	}


	delete [] coor2D;
	return (arPolygonOut.GetSize() > 0 || arPolylineOut.GetSize() > 0);
}

void CDrawFunc::GetGravelData(int nWidth, int nHeight, nrT_POLYLINE& arPoly, nrPOINT& arPoint, int& nXmax, int& nXmin, int& nYmax, int& nYmin)
{
	arPoly.RemoveAll();
	arPoint.RemoveAll();
	nXmax = nXmin = nYmax = nYmin = 0;

	int nNonWidth = 20;
	int nNonHeight = 20;
	arPoly.SetSize(6);
	arPoint.SetSize(45);
	T_POLYLINE PolyD;

	// Polygon
	PolyD.arPoint.SetSize(13);
	PolyD.arPoint[ 0].x =  -1;  PolyD.arPoint[ 0].y = -11;
	PolyD.arPoint[ 1].x =   0;  PolyD.arPoint[ 1].y = -11;
	PolyD.arPoint[ 2].x =   1;  PolyD.arPoint[ 2].y = -10;
	PolyD.arPoint[ 3].x =   1;  PolyD.arPoint[ 3].y =  -9;
	PolyD.arPoint[ 4].x =   0;  PolyD.arPoint[ 4].y =  -8;
	PolyD.arPoint[ 5].x =  -2;  PolyD.arPoint[ 5].y =  -7;
	PolyD.arPoint[ 6].x =  -3;  PolyD.arPoint[ 6].y =  -6;
	PolyD.arPoint[ 7].x =  -4;  PolyD.arPoint[ 7].y =  -4;
	PolyD.arPoint[ 8].x =  -6;  PolyD.arPoint[ 8].y =  -3;
	PolyD.arPoint[ 9].x =  -8;  PolyD.arPoint[ 9].y =  -4;
	PolyD.arPoint[10].x =  -9;  PolyD.arPoint[10].y =  -5;
	PolyD.arPoint[11].x =  -9;  PolyD.arPoint[11].y =  -7;
	PolyD.arPoint[12].x =  -7;  PolyD.arPoint[12].y =  -9;
	arPoly[0] = PolyD;

	PolyD.arPoint.SetSize(14);
	PolyD.arPoint[ 0].x =   3;  PolyD.arPoint[ 0].y = -9;
	PolyD.arPoint[ 1].x =   5;  PolyD.arPoint[ 1].y = -9;
	PolyD.arPoint[ 2].x =   7;  PolyD.arPoint[ 2].y = -8;
	PolyD.arPoint[ 3].x =   8;  PolyD.arPoint[ 3].y = -7;
	PolyD.arPoint[ 4].x =   8;  PolyD.arPoint[ 4].y = -5;
	PolyD.arPoint[ 5].x =   7;  PolyD.arPoint[ 5].y = -3;
	PolyD.arPoint[ 6].x =   5;  PolyD.arPoint[ 6].y =  0;
	PolyD.arPoint[ 7].x =   3;  PolyD.arPoint[ 7].y =  1;
	PolyD.arPoint[ 8].x =   1;  PolyD.arPoint[ 8].y =  1;
	PolyD.arPoint[ 9].x =  -1;  PolyD.arPoint[ 9].y =  0;
	PolyD.arPoint[10].x =  -3;  PolyD.arPoint[10].y = -4;
	PolyD.arPoint[11].x =  -3;  PolyD.arPoint[11].y = -6;
	PolyD.arPoint[12].x =  -2;  PolyD.arPoint[12].y = -7;
	PolyD.arPoint[13].x =   0;  PolyD.arPoint[13].y = -8;
	arPoly[1] = PolyD;

	PolyD.arPoint.SetSize(8);
	PolyD.arPoint[ 0].x =   9;  PolyD.arPoint[ 0].y = -6;
	PolyD.arPoint[ 1].x =  11;  PolyD.arPoint[ 1].y = -5;
	PolyD.arPoint[ 2].x =  12;  PolyD.arPoint[ 2].y = -3;
	PolyD.arPoint[ 3].x =  12;  PolyD.arPoint[ 3].y = -2;
	PolyD.arPoint[ 4].x =  10;  PolyD.arPoint[ 4].y =  0;
	PolyD.arPoint[ 5].x =   9;  PolyD.arPoint[ 5].y =  0;
	PolyD.arPoint[ 6].x =   7;  PolyD.arPoint[ 6].y = -2;
	PolyD.arPoint[ 7].x =   8;  PolyD.arPoint[ 7].y = -5;
	arPoly[2] = PolyD;

	PolyD.arPoint.SetSize(17);
	PolyD.arPoint[ 0].x =  -6;  PolyD.arPoint[ 0].y = -3;
	PolyD.arPoint[ 1].x =  -5;  PolyD.arPoint[ 1].y = -3;
	PolyD.arPoint[ 2].x =  -3;  PolyD.arPoint[ 2].y = -2;
	PolyD.arPoint[ 3].x =  -2;  PolyD.arPoint[ 3].y = -1;
	PolyD.arPoint[ 4].x =  -1;  PolyD.arPoint[ 4].y =  1;
	PolyD.arPoint[ 5].x =   0;  PolyD.arPoint[ 5].y =  2;
	PolyD.arPoint[ 6].x =   2;  PolyD.arPoint[ 6].y =  3;
	PolyD.arPoint[ 7].x =   3;  PolyD.arPoint[ 7].y =  4;
	PolyD.arPoint[ 8].x =   3;  PolyD.arPoint[ 8].y =  5;
	PolyD.arPoint[ 9].x =   2;  PolyD.arPoint[ 9].y =  6;
	PolyD.arPoint[10].x =   0;  PolyD.arPoint[10].y =  7;
	PolyD.arPoint[11].x =  -1;  PolyD.arPoint[11].y =  7;
	PolyD.arPoint[12].x =  -4;  PolyD.arPoint[12].y =  6;
	PolyD.arPoint[13].x =  -7;  PolyD.arPoint[13].y =  4;
	PolyD.arPoint[14].x =  -9;  PolyD.arPoint[14].y =  2;
	PolyD.arPoint[15].x =  -9;  PolyD.arPoint[15].y =  0;
	PolyD.arPoint[16].x =  -8;  PolyD.arPoint[16].y = -2;
	arPoly[3] = PolyD;

	PolyD.arPoint.SetSize(10);
	PolyD.arPoint[ 0].x =  -9;  PolyD.arPoint[ 0].y =  4;
	PolyD.arPoint[ 1].x =  -8;  PolyD.arPoint[ 1].y =  4;
	PolyD.arPoint[ 2].x =  -6;  PolyD.arPoint[ 2].y =  5;
	PolyD.arPoint[ 3].x =  -4;  PolyD.arPoint[ 3].y =  7;
	PolyD.arPoint[ 4].x =  -3;  PolyD.arPoint[ 4].y =  9;
	PolyD.arPoint[ 5].x =  -4;  PolyD.arPoint[ 5].y = 10;
	PolyD.arPoint[ 6].x =  -6;  PolyD.arPoint[ 6].y = 10;
	PolyD.arPoint[ 7].x =  -9;  PolyD.arPoint[ 7].y =  9;
	PolyD.arPoint[ 8].x = -10;  PolyD.arPoint[ 8].y =  7;
	PolyD.arPoint[ 9].x = -10;  PolyD.arPoint[ 9].y =  5;
	arPoly[4] = PolyD;

	PolyD.arPoint.SetSize(15);
	PolyD.arPoint[ 0].x =   7;  PolyD.arPoint[ 0].y = -1;
	PolyD.arPoint[ 1].x =   9;  PolyD.arPoint[ 1].y =  0;
	PolyD.arPoint[ 2].x =  10;  PolyD.arPoint[ 2].y =  2;
	PolyD.arPoint[ 3].x =   9;  PolyD.arPoint[ 3].y =  5;
	PolyD.arPoint[ 4].x =   9;  PolyD.arPoint[ 4].y =  6;
	PolyD.arPoint[ 5].x =  11;  PolyD.arPoint[ 5].y = 10;
	PolyD.arPoint[ 6].x =  10;  PolyD.arPoint[ 6].y = 12;
	PolyD.arPoint[ 7].x =   8;  PolyD.arPoint[ 7].y = 12;
	PolyD.arPoint[ 8].x =   5;  PolyD.arPoint[ 8].y = 11;
	PolyD.arPoint[ 9].x =   3;  PolyD.arPoint[ 9].y = 10;
	PolyD.arPoint[10].x =   2;  PolyD.arPoint[10].y =  8;
	PolyD.arPoint[11].x =   2;  PolyD.arPoint[11].y =  7;
	PolyD.arPoint[12].x =   3;  PolyD.arPoint[12].y =  5;
	PolyD.arPoint[13].x =   4;  PolyD.arPoint[13].y =  3;
	PolyD.arPoint[14].x =   5;  PolyD.arPoint[14].y =  0;
	arPoly[5] = PolyD;
	// Point
	arPoint[ 0].x =  -1;  arPoint[ 0].y = -10;  
	arPoint[ 1].x =   5;  arPoint[ 1].y = -10;  
	arPoint[ 2].x =   9;  arPoint[ 2].y =  -9;  
	arPoint[ 3].x =  -4;  arPoint[ 3].y =  -8;  
	arPoint[ 4].x =  -2;  arPoint[ 4].y =  -8;  
	arPoint[ 5].x =   4;  arPoint[ 5].y =  -8;  
	arPoint[ 6].x =  -7;  arPoint[ 6].y =  -7;  
	arPoint[ 7].x =   0;  arPoint[ 7].y =  -7;  
	arPoint[ 8].x =  -5;  arPoint[ 8].y =  -6;  
	arPoint[ 9].x =  -7;  arPoint[ 9].y =  -5;  
	arPoint[10].x =   5;  arPoint[10].y =  -5;  
	arPoint[11].x =   7;  arPoint[11].y =  -5;  
	arPoint[12].x =   9;  arPoint[12].y =  -5;  
	arPoint[13].x =  -6;  arPoint[13].y =  -4;  
	arPoint[14].x =  -2;  arPoint[14].y =  -4;  
	arPoint[15].x =   2;  arPoint[15].y =  -4;  
	arPoint[16].x =  -9;  arPoint[16].y =  -3;  
	arPoint[17].x =  -1;  arPoint[17].y =  -2;  
	arPoint[18].x =   1;  arPoint[18].y =  -2;  
	arPoint[19].x =   5;  arPoint[19].y =  -2;  
	arPoint[20].x =   8;  arPoint[20].y =  -2;  
	arPoint[21].x = -10;  arPoint[21].y =  -1;  
	arPoint[22].x =  -8;  arPoint[22].y =  -1;  
	arPoint[23].x =  -3;  arPoint[23].y =   0;  
	arPoint[24].x =   1;  arPoint[24].y =   0;  
	arPoint[25].x =   3;  arPoint[25].y =   0;  
	arPoint[26].x =  -6;  arPoint[26].y =   1;  
	arPoint[27].x =   7;  arPoint[27].y =   1;  
	arPoint[28].x =  -7;  arPoint[28].y =   3;  
	arPoint[29].x =  -1;  arPoint[29].y =   3;  
	arPoint[30].x =  -5;  arPoint[30].y =   4;  
	arPoint[31].x =   2;  arPoint[31].y =   4;  
	arPoint[32].x =   5;  arPoint[32].y =   4;  
	arPoint[33].x =  -9;  arPoint[33].y =   5;  
	arPoint[34].x =  -3;  arPoint[34].y =   5;  
	arPoint[35].x =   1;  arPoint[35].y =   5;  
	arPoint[36].x =   8;  arPoint[36].y =   5;  
	arPoint[37].x =  -1;  arPoint[37].y =   6;  
	arPoint[38].x =  -9;  arPoint[38].y =   7;  
	arPoint[39].x =   3;  arPoint[39].y =   7;  
	arPoint[40].x =  -5;  arPoint[40].y =   8;  
	arPoint[41].x =   5;  arPoint[41].y =   8;  
	arPoint[42].x =   9;  arPoint[42].y =   8;  
	arPoint[43].x =  -7;  arPoint[43].y =   9;  
	arPoint[44].x =   7;  arPoint[44].y =   9;  

	// Scale조정 및 중복좌표제거
	double dFactorX = (double)(nWidth)/(double)(nNonWidth);
	double dFactorY = (double)(nHeight)/(double)(nNonHeight) * (m_bEMF ? -1.0 : 1.0);
	int nSize, nPolySize, i, j, k;
	POINT PointD;
	nSize = arPoly.GetSize();

	for(i = nSize - 1; i >= 0 ; i--)
	{
		nPolySize = arPoly[i].arPoint.GetSize();

		for(j = nPolySize - 1; j >= 0 ; j--)
		{
			PointD = arPoly[i].arPoint[j];

			arPoly[i].arPoint[j].x = (int)(PointD.x * dFactorX);
			arPoly[i].arPoint[j].y = (int)(PointD.y * dFactorY);

			for(k = j + 1; k < nPolySize; k++)
			{
				if(arPoly[i].arPoint[j].x == arPoly[i].arPoint[k].x && arPoly[i].arPoint[j].y == arPoly[i].arPoint[k].y)
				{
					arPoly[i].arPoint.RemoveAt(j);
					nPolySize--;
					break;
				}
			}
		}
		if(nPolySize <= 0) arPoly.RemoveAt(i);
	}

	nSize = arPoint.GetSize();
	for(i = nSize - 1; i >= 0 ; i--)
	{
		PointD = arPoint[i];

		arPoint[i].x = (int)(PointD.x * dFactorX);
		arPoint[i].y = (int)(PointD.y * dFactorY);

		for(j = i + 1; j < nSize ; j++)
		{
			if(arPoint[i].x == arPoint[j].x && arPoint[i].y == arPoint[j].y)
			{
				arPoint.RemoveAt(i);
				nSize--;
				break;
			}
		}
	}

	// Max, Min값 산출	
	nSize = arPoly.GetSize();
	for(i = 0; i < nSize ; i++)
	{
		nPolySize = arPoly[i].arPoint.GetSize();
		
		for(j = 0; j < nPolySize ; j++)
		{
			nXmax = max(nXmax, arPoly[i].arPoint[j].x);
			nXmin = min(nXmin, arPoly[i].arPoint[j].x);
			nYmax = max(nYmax, arPoly[i].arPoint[j].y);
			nYmin = min(nYmin, arPoly[i].arPoint[j].y);
		}
	}
	nSize = arPoint.GetSize();
	for(i = 0; i < nSize ; i++)
	{
		nXmax = max(nXmax, arPoint[i].x);
		nXmin = min(nXmin, arPoint[i].x);
		nYmax = max(nYmax, arPoint[i].y);
		nYmin = min(nYmin, arPoint[i].y);
	}
}

TCHAR* CDrawFunc::GetDoubleStringFormat(int nDecimal)
{
	static TCHAR szFormat[20];
	wsprintf(szFormat, _T("%s%d%s"), _T("%0."), nDecimal, _T("f"));
	return szFormat;  
}

void CDrawFunc::GetArcPolylineR(T_POINT startP, T_POINT originP, double angle, int nCount, T_POLYLINE_R& arcPolyline)
{
	int nSize = abs(nCount) + 1;
	arcPolyline.arPoint.SetSize(nSize);
	arcPolyline.arPoint[0] = startP;

	double xy[2];
	double org[2];  
	org[0] = originP.x;
	org[1] = originP.y;
	double dSign = 1.0;
	T_POINT pointR;

	for(int i = 1; i < nSize ; i++)
	{
		xy[0]  = startP.x;
		xy[1]  = startP.y;
		math_lib::rotate_2d(xy, org, (double)i*angle);

		pointR.x = xy[0];  
		pointR.y = xy[1];
		arcPolyline.arPoint[i] = pointR;
	}
	return;
}

void CDrawFunc::GetArcPolylineR(T_ARC_R& ArcD, int nCount, T_POLYLINE_R& arcPolyline)
{
	T_POINT startP, originP; 
	double angle,  startAngle_rad;
	angle = ArcD.SweepAngle / ((double)nCount);
	startAngle_rad = math_lib::radian(ArcD.StartAngle);

	originP = ArcD.CenterP;
	startP.Init();	
	startP.x = originP.x + ArcD.Radius * cos(startAngle_rad);
	startP.y = originP.y + ArcD.Radius * sin(startAngle_rad);

	GetArcPolylineR(startP, originP, angle, nCount, arcPolyline);
}

void CDrawFunc::GetHorizontalDimensionR(T_POLYLINE_R& dgnPolyR, nrT_DIMENSION_R& arDimR, double dPosition_Y, int nDecimal, double dRatio, int nSortType)
{
	CArray<double, double> arPosi;
	int nCount = dgnPolyR.arPoint.GetSize();
	if(nCount < 2) 
	{
		arDimR.RemoveAll();
		return;
	}
	arPosi.SetSize(nCount);
	for(int i = 0; i < nCount ; ++i)
	{
		arPosi[i] = dgnPolyR.arPoint[i].x;
	}

	GetHorizontalDimensionR(arPosi, arDimR, dPosition_Y, nDecimal, dRatio, nSortType);
}

void CDrawFunc::GetHorizontalDimensionR(CArray<double, double>& arPosi, nrT_DIMENSION_R& arDimR, double dPosition_Y, int nDecimal, double dRatio, int nSortType)
{
	arDimR.RemoveAll();
	// nSortType==0 : 작은 숫자 순으로 sorting , nSortType==1 : 큰 숫자 순으로 sorting 
	int nCount = arPosi.GetSize();
	if(nCount < 2) return;
	if(nDecimal < 0) nDecimal = 0;

	double dTemp;
	int i, j;

	for(i = 0; i < (nCount-1) ; ++i)
	{
		for(j = i + 1; j < nCount ; ++j)
		{
			if((nSortType == 0 && arPosi[i] > arPosi[j]) || (nSortType == 1 && arPosi[i] < arPosi[j])) 
			{ 
				dTemp = arPosi[i];  
				arPosi[i] = arPosi[j]; 
				arPosi[j] = dTemp;
			}
		}
	}
	//
	nCount = nCount-1;
	arDimR.SetSize(nCount);

	TCHAR szTemp[64];

	for(i = 0; i < nCount ; ++i)
	{
		T_DIMENSION_R dgnDimR;
		dgnDimR.P1.x = arPosi[i];
		dgnDimR.P2.x = arPosi[i+1];
		dgnDimR.P1.y = dgnDimR.P2.y = dPosition_Y;    
		double dLength = (double)(math_lib::round_off((arPosi[i+1]-arPosi[i])*pow(10.0, nDecimal)))/pow(10.0, nDecimal);

		wsprintf(szTemp, GetDoubleStringFormat(nDecimal), fabs(dLength*dRatio));
		dgnDimR.szText = szTemp; 
		arDimR[i] = dgnDimR;
	}
}

void CDrawFunc::GetVerticalDimensionR(T_POLYLINE_R& dgnPolyR, nrT_DIMENSION_R& arDimR, double dPosition_X, int nDecimal, double dRatio, int nSortType)
{
	CArray<double, double> arPosi;
	int nCount = dgnPolyR.arPoint.GetSize();
	if(nCount < 2) 
	{
		arDimR.RemoveAll();
		return;
	}
	arPosi.SetSize(nCount);
	for(int i = 0; i < nCount ; ++i)
	{
		arPosi[i] = dgnPolyR.arPoint[i].y;
	}

	GetVerticalDimensionR(arPosi, arDimR, dPosition_X, nDecimal, dRatio, nSortType);
}

void CDrawFunc::GetVerticalDimensionR(CArray<double, double>& arPosi, nrT_DIMENSION_R& arDimR, double dPosition_X, int nDecimal, double dRatio, int nSortType)
{
	arDimR.RemoveAll();
	// nSortType==0 : 작은 숫자 순으로 sorting , nSortType==1 : 큰 숫자 순으로 sorting 
	int nCount = arPosi.GetSize();
	if(nCount < 2) return;
	if(nDecimal < 0) nDecimal = 0;

	double dTemp;
	int i, j;

	for(i = 0; i < (nCount-1) ; ++i)
	{
		for(j = i + 1; j < nCount ; ++j)
		{
			if((nSortType==0 && arPosi[i] > arPosi[j]) || (nSortType==1 && arPosi[i] < arPosi[j])) 
			{ 
				dTemp = arPosi[i]; 
				arPosi[i] = arPosi[j];  
				arPosi[j] = dTemp;
			}
		}
	}
	//
	nCount = nCount-1;
	arDimR.SetSize(nCount);
	TCHAR szTemp[64];

	for(i = 0; i < nCount ; ++i)
	{
		T_DIMENSION_R dgnDimR;
		dgnDimR.P1.y = arPosi[i];
		dgnDimR.P2.y = arPosi[i+1];
		dgnDimR.P1.x = dgnDimR.P2.x = dPosition_X;    
		double dLength = (double)(math_lib::round_off((arPosi[i+1]-arPosi[i])*pow(10.0, nDecimal)))/pow(10.0, nDecimal);

		wsprintf(szTemp, GetDoubleStringFormat(nDecimal), fabs(dLength*dRatio));
		dgnDimR.szText = szTemp;
		arDimR[i] = dgnDimR;
	}
}

BOOL CDrawFunc::GetArcPolyline_Radius(double dStartP[2], double dEndP[2], double dRadius, BOOL bInside, int nPointCount, BOOL bPointSide, nrT_POINT& arPoint)
{	
	arPoint.RemoveAll();
	T_POINT pointR;  

	if(bPointSide && nPointCount <= 2)
	{
		if(nPointCount == 2)
		{		
			arPoint.SetSize(2);
			pointR.x = dStartP[0];   
			pointR.y = dStartP[1];
			arPoint[0] = pointR;
			pointR.x = dEndP[0]; 
			pointR.y = dEndP[1];
			arPoint[1] = pointR;

			return TRUE;		
		}
		return FALSE;
	}
	else if(!bPointSide && nPointCount <= 0)
	{
		return FALSE;
	}

	double line[2][2];
	line[0][0] = dStartP[0];  
	line[0][1] = dStartP[1];
	line[1][0] = dEndP[0];   
	line[1][1] = dEndP[1];
	double dBulge = CMathFunc::GetBulge(line, dRadius, bInside);
	double dLen   = CMathFunc::mathLength(dStartP[0], dStartP[1], dEndP[0], dEndP[1]);

	if(dLen < CMathFunc::m_NormalZero)
	{
		if(bPointSide)
		{
			arPoint.SetSize(1);
			pointR.x = dStartP[0];   
			pointR.y = dStartP[1];
			arPoint[0] = pointR;
		}
		return FALSE;
	}

	if(fabs(dBulge) < CMathFunc::m_NormalZero)
	{			
		GetLinePolyline(dStartP, dEndP, TRUE, nPointCount, 0.0, bPointSide, arPoint);
		return TRUE;
	}

	return GetArcPolyline_Bulge(dStartP, dEndP, dBulge, nPointCount, bPointSide, arPoint);
}

BOOL CDrawFunc::GetArcPolyline_Bulge(double dStartP[2], double dEndP[2], double dBulge, int nPointCount, BOOL bPointSide, nrT_POINT& arPoint)
{
	arPoint.RemoveAll();
	T_POINT pointR; 
	if(bPointSide && nPointCount <= 2)
	{
		if(nPointCount == 2)
		{		
			arPoint.SetSize(2);
			pointR.x = dStartP[0];  
			pointR.y = dStartP[1];
			arPoint[0] = pointR;
			pointR.x = dEndP[0]; 
			pointR.y = dEndP[1];
			arPoint[1] = pointR;

			return TRUE;		
		}
		return FALSE;
	}
	else if(!bPointSide && nPointCount <= 0)
	{
		return FALSE;
	}

	double line[2][2];
	double dCenterP[2], vector1[2], vector2[2];

	line[0][0] = dStartP[0];   
	line[0][1] = dStartP[1];
	line[1][0] = dEndP[0];   
	line[1][1] = dEndP[1];

	CMathFunc::GetCenter_Bulge(line, dBulge, dCenterP);

	CMathFunc::mathNormalize(dStartP[0]-dCenterP[0], dStartP[1]-dCenterP[1], vector1[0], vector1[1]);
	CMathFunc::mathNormalize(dEndP[0]-dCenterP[0],   dEndP[1]-dCenterP[1],   vector2[0], vector2[1]);

	double dAngle = fabs(CMathFunc::mathCrossAngle2DSign(vector1[0], vector1[1], vector2[0], vector2[1]));
	double dLen   = CMathFunc::mathLength(dStartP[0], dStartP[1], dEndP[0], dEndP[1]);

	if(dLen / 2.0 < dBulge) dAngle = 360.0 - dAngle;

	int nCount = (bPointSide) ? nPointCount-1 : nPointCount+1;
	if(nCount <= 0) return FALSE;

	double dAngleD = dAngle/((double)nCount) * (dBulge > 0.0 ? 1.0 : -1.0);

	GetArcPolyline(dStartP, dCenterP, dAngleD, nCount, bPointSide, arPoint);
	return TRUE;	
}

void CDrawFunc::GetArcPolyline(double dStartP[2], double dCenterP[2], double angle, int nCount, BOOL bPointSide, nrT_POINT& arPoint)
{
	arPoint.RemoveAll();

	T_POINT pointR;  
	pointR.x = dStartP[0];
	pointR.y = dStartP[1];

	int nSize = abs(nCount) + 1;
	arPoint.SetSize(nSize);
	arPoint[0] = pointR;	

	double xy[2];
	for(int i = 1; i < nSize ; i++)
	{
		xy[0]  = dStartP[0];
		xy[1]  = dStartP[1];
		math_lib::rotate_2d(xy, dCenterP, (double)i*angle);
		T_POINT pointR;
		pointR.x = xy[0];   
		pointR.y = xy[1];
		arPoint[i] = pointR;
	}

	if(!bPointSide)
	{
		if(nSize <= 2)
		{
			arPoint.RemoveAll();
		}
		else
		{
			arPoint.RemoveAt(nSize-1);
			arPoint.RemoveAt(0);
		}
	}

	return;
}

void CDrawFunc::GetArcPolyline(double dStartP[2], double dPassP[2], double dEndP[2], BOOL bNum, int nPointCount, double dPointCTC, BOOL bPointSide, nrT_POINT& arPoint)
{
	arPoint.RemoveAll();

	double CenterP[2];
	double dRadius, dStartAngle, dSweepAngle;
	int nChk = CMathFunc::mathArcForm3Point(dStartP, dPassP, dEndP, CenterP, dRadius, dStartAngle, dSweepAngle);

	if(nChk == 0)
	{
		double dLength = CMathFunc::mathLength(dStartP[0], dStartP[1], dEndP[0], dEndP[1]);
		if(dLength < CMathFunc::m_NormalZero)
		{
			T_POINT dgnPoint; 
			dgnPoint.x = dStartP[0]; 
			dgnPoint.y = dStartP[1];
			arPoint.Add(dgnPoint);
			return;
		}
		int    nRbarNumR = (bNum ? (bPointSide ? nPointCount : nPointCount+2) : (dPointCTC<=0.0 ? 0:(int)(dLength/dPointCTC)+1));
		double dRbarCTCR = (bNum ? (nRbarNumR<=1 ? 0.0:dLength/(nRbarNumR-1))   : dPointCTC);
		double lineVector[2];
		CMathFunc::mathNormalize(dEndP[0]-dStartP[0], dEndP[1]-dStartP[1], lineVector[0], lineVector[1]);

		int nCount = 0;
		int nSize = bPointSide ? nRbarNumR : nRbarNumR-2;
		if(nSize <= 0) return;
		arPoint.SetSize(nSize);
		for(int i = 0 ; i < nRbarNumR ; i++)
		{
			if(i==0           && !bPointSide) continue;
			if(i==nRbarNumR-1 && !bPointSide) continue;

			arPoint[nCount].x = dStartP[0] + i * dRbarCTCR * lineVector[0];
			arPoint[nCount].y = dStartP[1] + i * dRbarCTCR * lineVector[1];
			nCount++;
		}
	}
	else if(nChk == 1)
	{
		double dpi     = 4.0 * atan(1.0);
		double dtrrad  = dpi / 180;
		double dLength = fabs(2.0 * dpi * dRadius * dSweepAngle/360.0);
		
		if(dLength < CMathFunc::m_NormalZero)
		{
			T_POINT dgnPoint; 
			dgnPoint.x = dStartP[0];  
			dgnPoint.y = dStartP[1];
			arPoint.Add(dgnPoint);
			return;
		}
		int    nRbarNumR  = (bNum ? (bPointSide ? nPointCount:nPointCount+2) : (dPointCTC<=0.0 ? 0:(int)(dLength/dPointCTC)+1));

		if(nRbarNumR >= 1000 || nRbarNumR <= 0) return;//1000개 까지 제한함

		double dRbarCTCR = (bNum ? (nRbarNumR <= 1 ? 0.0 : dLength / (nRbarNumR - 1)) : dPointCTC);
		double dAngleDiv = dLength == 0.0 ? 0.0 : dSweepAngle * dRbarCTCR / dLength;

		int nCount = 0;
		int nSize = bPointSide ? nRbarNumR : nRbarNumR-2;
		if(nSize <= 0) return;

		arPoint.SetSize(nSize);

		for(int i = 0; i < nRbarNumR ; i++)
		{
			if(i==0           && !bPointSide) continue;
			if(i==nRbarNumR-1 && !bPointSide) continue;

			arPoint[nCount].x = CenterP[0] + dRadius * cos((dStartAngle + i*dAngleDiv)*dtrrad);
			arPoint[nCount].y = CenterP[1] + dRadius * sin((dStartAngle + i*dAngleDiv)*dtrrad);
			nCount++;
		}
	}
}

void CDrawFunc::GetLinePolyline(double dStartP[2], double dEndP[2], BOOL bNum, int nPointCount, double dPointCTC, BOOL bPointSide, nrT_POINT& arPoint)
{	
	arPoint.RemoveAll();
	double dLength = CMathFunc::mathLength(dStartP[0], dStartP[1], dEndP[0], dEndP[1]);
	
	if(dLength < CMathFunc::m_NormalZero)
	{
		T_POINT dgnPoint; 
		dgnPoint.x = dStartP[0];
		dgnPoint.y = dStartP[1];
		arPoint.Add(dgnPoint);
		return;
	}
	int    nPointNumR  = (bNum ? (bPointSide ? nPointCount:nPointCount+2) : (dPointCTC<=0.0 ? 0:(int)(dLength/dPointCTC)+1));
	double dPointCTCR = (bNum ? (nPointNumR<=1 ? 0.0:dLength/(nPointNumR-1))   : dPointCTC);
	double lineVector[2];

	CMathFunc::mathNormalize(dEndP[0]-dStartP[0], dEndP[1]-dStartP[1], lineVector[0], lineVector[1]);

	int nCount = 0;
	int nSize = bPointSide ? nPointNumR : nPointNumR-2;
	if(nSize <= 0) return;
	arPoint.SetSize(nSize);	
	for(int i = 0; i < nPointNumR ; i++)
	{
		if(i == 0               && !bPointSide) continue;
		if(i == nPointNumR - 1 && !bPointSide) continue;

		arPoint[nCount].x = dStartP[0] + i * dPointCTCR * lineVector[0];
		arPoint[nCount].y = dStartP[1] + i * dPointCTCR * lineVector[1];
		nCount++;
	}
}

void CDrawFunc::GetCirclePolyline(double dStartP[2], double dRadius, BOOL bNum, int nPointCount, double dPointCTC, nrT_POINT& arPoint)
{
	arPoint.RemoveAll();
	
	//선표시
	if(fabs(dRadius) < CMathFunc::m_NormalZero)
	{
		T_POINT dgnPoint; 
		dgnPoint.x = dStartP[0];   
		dgnPoint.y = dStartP[1];
		arPoint.Add(dgnPoint);
		return;
	}

	//철근표시
	double dpi   = 4.0 * atan(1.0);
	double dtrrad = dpi / 180;
	double dLength = 2.0 * dpi * dRadius;
	int    nPointNumR  = (bNum ? nPointCount : (dPointCTC <= 0.0 ? 0 : (int)(dLength / dPointCTC)));

	if(nPointNumR >= 1000 || nPointNumR <= 0) return;//1000개 까지 제한함

	double dPointCTCR = (bNum ? (nPointNumR <= 0 ? 0.0:dLength / nPointNumR) : dPointCTC);
	double dAngleDiv = dLength == 0.0 ? 0.0 : -360.0 * dPointCTCR / dLength;

	if(nPointNumR <= 0) return;

	arPoint.SetSize(nPointNumR);

	for(int i = 0; i < nPointNumR; i++)
	{
		arPoint[i].x = dStartP[0] + dRadius * cos((90.0 + i * dAngleDiv) * dtrrad);
		arPoint[i].y = dStartP[1] + dRadius * sin((90.0 + i * dAngleDiv) * dtrrad);
	}
}
