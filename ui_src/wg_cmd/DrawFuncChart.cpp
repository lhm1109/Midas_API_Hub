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

// DrawChart ////////////////////////////////////

// T_CHART_FORMAT을 이용하여 DC에 Chart를 그림
void CDrawFunc::DrawChart(HDC DC, T_CHART_FORMAT chartFormat, nrT_CHART& arChart)
{
	int no = arChart.GetSize(); 
	for(auto i = 0; i < no; i++)  DrawChart(DC, chartFormat, arChart[i]);
}

void CDrawFunc::MakeChartData(T_PAPER_FORMAT paperFormat, T_CHART_FORMAT chartFormat, T_CHART_R& Chart_In, T_CHART& Chart_Out)
{
	Chart_Out.Init();
	if(Chart_In.Info.nType == EN_CHART_TYPE_DISPERSAL)
	{
		POINT  titlePosi;
		T_LINE framePosi;
		T_LINE remarkPosi;
		ConvertToPOINT(paperFormat, Chart_In.titlePosi, titlePosi);
		ConvertToLine(paperFormat, Chart_In.framePosi, framePosi);
		ConvertToLine(paperFormat, Chart_In.remarkPosi, remarkPosi);

		int nXmax = max(framePosi.P1.x, framePosi.P2.x);
		int nXmin = min(framePosi.P1.x, framePosi.P2.x);
		int nYmax = max(framePosi.P1.y, framePosi.P2.y);
		int nYmin = min(framePosi.P1.y, framePosi.P2.y);

		Chart_Out.Info = Chart_In.Info;

		int nRmSize, i, j;
		// 축정보 자동설정정보 채우기 ------------------------------------------------
		BOOL bxAxisAuto = (Chart_Out.Info.xAxis.bMaxValueAuto || Chart_Out.Info.xAxis.bMinValueAuto || Chart_Out.Info.xAxis.bMajorSpaceAuto || Chart_Out.Info.xAxis.bMinorSpaceAuto);
		BOOL byAxisAuto = (Chart_Out.Info.yAxis.bMaxValueAuto || Chart_Out.Info.yAxis.bMinValueAuto || Chart_Out.Info.yAxis.bMajorSpaceAuto || Chart_Out.Info.yAxis.bMinorSpaceAuto);

		if(bxAxisAuto || byAxisAuto)
		{
			double dDataXmax, dDataXmin, dDataYmax, dDataYmin;
			dDataXmax = dDataXmin = dDataYmax = dDataYmin = 0.0;
			BOOL bFirst = TRUE;

			auto no = Chart_In.arData.GetSize();
			for(i = 0; i < no; i++)
			{
				auto no2 = Chart_In.arData[i].arPoint.GetSize();
				for(j = 0; j < no2; j++) 
				{
					if(bFirst) 
					{ 
						dDataXmax = dDataXmin = Chart_In.arData[i].arPoint[j].x;		
						dDataYmax = dDataYmin = Chart_In.arData[i].arPoint[j].y;
						bFirst = FALSE;
					}
					else 
					{
						dDataXmax = max(dDataXmax, Chart_In.arData[i].arPoint[j].x);
						dDataXmin = min(dDataXmin, Chart_In.arData[i].arPoint[j].x);
						dDataYmax = max(dDataYmax, Chart_In.arData[i].arPoint[j].y);
						dDataYmin = min(dDataYmin, Chart_In.arData[i].arPoint[j].y);
					}
				}
			}

			if(bxAxisAuto) GetAutoAxisInfo((nXmax-nXmin), (nYmax-nYmin), chartFormat.xAxis.fontSize, Chart_In.Info.xAxis.nDecimal, dDataXmax, dDataXmin, Chart_Out.Info.xAxis, (abs(chartFormat.xAxis.nEscapement)>850));
			if(byAxisAuto) GetAutoAxisInfo((nYmax-nYmin), (nXmax-nXmin), chartFormat.yAxis.fontSize, Chart_In.Info.yAxis.nDecimal, dDataYmax, dDataYmin, Chart_Out.Info.yAxis, (abs(chartFormat.yAxis.nEscapement)< 50));			
		}
		// x축의 표시 위치
		if(Chart_Out.Info.xAxis.bCrossPosiAuto) 
		{
			if(Chart_Out.Info.yAxis.dMaxValue >= 0.0 && Chart_Out.Info.yAxis.dMinValue <= 0.0) Chart_Out.Info.xAxis.dCrossPosi = 0.0;
			else Chart_Out.Info.xAxis.dCrossPosi = (Chart_Out.Info.yAxis.dMaxValue <= 0.0) ? Chart_Out.Info.yAxis.dMaxValue : Chart_Out.Info.yAxis.dMinValue;
		}
		else if(Chart_Out.Info.xAxis.dCrossPosi > Chart_Out.Info.yAxis.dMaxValue) Chart_Out.Info.xAxis.dCrossPosi = Chart_Out.Info.yAxis.dMaxValue;
		else if(Chart_Out.Info.xAxis.dCrossPosi < Chart_Out.Info.yAxis.dMinValue) Chart_Out.Info.xAxis.dCrossPosi = Chart_Out.Info.yAxis.dMinValue;
		// y축의 표시 위치
		if(Chart_Out.Info.yAxis.bCrossPosiAuto) 
		{
			if(Chart_Out.Info.xAxis.dMaxValue >= 0.0 && Chart_Out.Info.xAxis.dMinValue <= 0.0) Chart_Out.Info.yAxis.dCrossPosi = 0.0;
			else Chart_Out.Info.yAxis.dCrossPosi = (Chart_Out.Info.xAxis.dMaxValue <= 0.0) ? Chart_Out.Info.xAxis.dMaxValue : Chart_Out.Info.xAxis.dMinValue;
		}
		else if(Chart_Out.Info.yAxis.dCrossPosi > Chart_Out.Info.xAxis.dMaxValue) Chart_Out.Info.yAxis.dCrossPosi = Chart_Out.Info.xAxis.dMaxValue;
		else if(Chart_Out.Info.yAxis.dCrossPosi < Chart_Out.Info.xAxis.dMinValue) Chart_Out.Info.yAxis.dCrossPosi = Chart_Out.Info.xAxis.dMinValue;
		//------------------------------------------------

		double dLeftValue   = (Chart_Out.Info.xAxis.bReverse)  ? Chart_Out.Info.xAxis.dMaxValue : Chart_Out.Info.xAxis.dMinValue;
		double dRightValue  = (!Chart_Out.Info.xAxis.bReverse) ? Chart_Out.Info.xAxis.dMaxValue : Chart_Out.Info.xAxis.dMinValue;
		double dBottomValue = (Chart_Out.Info.yAxis.bReverse)  ? Chart_Out.Info.yAxis.dMaxValue : Chart_Out.Info.yAxis.dMinValue;
		double dTopValue    = (!Chart_Out.Info.yAxis.bReverse) ? Chart_Out.Info.yAxis.dMaxValue : Chart_Out.Info.yAxis.dMinValue;
		int    nLeftValue   = nXmin;
		int    nRightValue  = nXmax;
		int    nBottomValue = m_bEMF ? nYmin : nYmax;
		int    nTopValue    = m_bEMF ? nYmax : nYmin;

		T_TEXT dgnText;
		T_LINE dgnLine, dgnLine_Sub;
		POINT dgnPoint;
		TStr  strStringFormat;
		nRmSize = 0;

		auto no = Chart_In.arData.GetSize();
		for(i = 0; i < no; i++) 
		{ 
			if(Chart_In.arData[i].Info.nNameType == EN_CHART_DATANAME_POSI_REMARK) nRmSize++; 
		}
		int nYSign = (m_bEMF) ? -1 : 1;
		int nCount, nCrossPosi, nStartNotch, nEndNotch, nLabelPosi;	
		double dLabelValue;
		// Frame  
		dgnText.Init();
		dgnText.szText = Chart_Out.Info.szTitle;
		dgnText.PPosition = titlePosi;
		Chart_Out.frame.arText.Add(dgnText);		
		Chart_Out.frame.arRect.Add(framePosi);
		// Remark
		if(abs(remarkPosi.P1.y - remarkPosi.P2.y) < chartFormat.remark.fontSize*nRmSize)
		{
			LONG nDelta = LONG((chartFormat.remark.fontSize*nRmSize - abs(remarkPosi.P1.y - remarkPosi.P2.y))/2.0);
			if(remarkPosi.P1.y > remarkPosi.P2.y) { remarkPosi.P1.y += nDelta;   remarkPosi.P2.y -= nDelta; }
			else                                      { remarkPosi.P1.y -= nDelta;   remarkPosi.P2.y += nDelta; }
		}
		Chart_Out.remark.arRect.Add(remarkPosi);
		Chart_Out.remark.arText.SetSize(nRmSize);
		int nRemarkXPois = min(remarkPosi.P1.x, remarkPosi.P2.x) + 3*chartFormat.remark.fontSize;
		int nRemarkYPois = (int)((remarkPosi.P1.y+remarkPosi.P2.y)/2) - (int)(nYSign*(nRmSize-1)*chartFormat.remark.fontSize/2);
		nCount = 0;
		for(i = 0; i < no; i++) 
		{
			if(Chart_In.arData[i].Info.nNameType != EN_CHART_DATANAME_POSI_REMARK) continue;
			Chart_Out.remark.arText[nCount].Init();
			Chart_Out.remark.arText[nCount].szText = Chart_In.arData[i].Info.szName;
			Chart_Out.remark.arText[nCount].PPosition.x = nRemarkXPois;
			Chart_Out.remark.arText[nCount].PPosition.y = nRemarkYPois + nYSign*nCount*chartFormat.remark.fontSize;			
			nCount++;
		}
		// x-Axis
		nCount = 0;
		int nMajorSize, nMinorSize;
		double dMinValue_MajorRev = (int)(Chart_Out.Info.xAxis.dMinValue/Chart_Out.Info.xAxis.dMajorSpace)*Chart_Out.Info.xAxis.dMajorSpace;
		double dMinValue_MinorRev = (int)(Chart_Out.Info.xAxis.dMinValue/Chart_Out.Info.xAxis.dMinorSpace)*Chart_Out.Info.xAxis.dMinorSpace;

		if(Chart_Out.Info.xAxis.nNotchCtrlPointType != 3)
		{
			// Level의 기준점을 낮은쪽, 높은쪽 기준으로 할때
			nMajorSize = (int)((Chart_Out.Info.xAxis.dMaxValue - Chart_Out.Info.xAxis.dMinValue)/Chart_Out.Info.xAxis.dMajorSpace)+1;
			nMinorSize = (int)((Chart_Out.Info.xAxis.dMaxValue - Chart_Out.Info.xAxis.dMinValue)/Chart_Out.Info.xAxis.dMinorSpace)+1;
		}
		else 
		{
			// Level의 기준점을 원점으로 할때
			nMajorSize = (int)(Chart_Out.Info.xAxis.dMaxValue/Chart_Out.Info.xAxis.dMajorSpace) - (int)(Chart_Out.Info.xAxis.dMinValue/Chart_Out.Info.xAxis.dMajorSpace);
			if(Chart_Out.Info.xAxis.dMaxValue>=0.0 && Chart_Out.Info.xAxis.dMinValue<=0.0) nMajorSize++;
			nMinorSize = (int)(Chart_Out.Info.xAxis.dMaxValue/Chart_Out.Info.xAxis.dMinorSpace) - (int)(Chart_Out.Info.xAxis.dMinValue/Chart_Out.Info.xAxis.dMinorSpace);
			if(Chart_Out.Info.xAxis.dMaxValue>=0.0 && Chart_Out.Info.xAxis.dMinValue<=0.0) nMinorSize++;
		}

		Chart_Out.xAxis.arLine.SetSize(1 + (Chart_Out.Info.xAxis.nMajorNotchType==0 ? 0 : nMajorSize) + (Chart_Out.Info.xAxis.nMinorNotchType==0 ? 0 : nMinorSize));
		if(nMajorSize > 0) 
		{ 
			if(Chart_Out.Info.xAxis.bMajorGrid)           Chart_Out.xAxis.arMajorGrid.SetSize(nMajorSize);  
			if(Chart_Out.Info.xAxis.nNotchLabelType != 0) Chart_Out.xAxis.arText.SetSize(nMajorSize); 
		}
		if(nMinorSize > 0) 
		{
			if(Chart_Out.Info.xAxis.bMinorGrid) Chart_Out.xAxis.arMinorGrid.SetSize(nMinorSize);
		}

		nCrossPosi = ConvertToChartValue(nTopValue, nBottomValue, dTopValue, dBottomValue, Chart_Out.Info.xAxis.dCrossPosi);
		dgnLine.P1.x = nXmin;  dgnLine.P2.x = nXmax;  				
		dgnLine.P1.y = dgnLine.P2.y = nCrossPosi;
		Chart_Out.xAxis.arLine[nCount] = dgnLine;
		nCount++;

		nStartNotch = nEndNotch =0;
		double dNotchRate = (Chart_Out.Info.xAxis.dNotchHeightRate <= 0) ? 1.0/3.0 : Chart_Out.Info.xAxis.dNotchHeightRate;

		if(Chart_Out.Info.xAxis.nMajorNotchType == 1) 
		{ 
			nEndNotch = nYSign*max(chartFormat.xAxis.Width*2, (int)(chartFormat.xAxis.fontSize*dNotchRate)) * (Chart_Out.Info.yAxis.bReverse  ? -1 : 1); 
		} 
		if(Chart_Out.Info.xAxis.nMajorNotchType == 2) 
		{
			nEndNotch = (-1)*nYSign*max(chartFormat.xAxis.Width*2, (int)(chartFormat.xAxis.fontSize*dNotchRate)) * (Chart_Out.Info.yAxis.bReverse  ? -1 : 1);
		} 
		if(Chart_Out.Info.xAxis.nMajorNotchType == 3) 
		{ 
			nStartNotch = nYSign*max(chartFormat.xAxis.Width*2, (int)(chartFormat.xAxis.fontSize*dNotchRate)); 
			nEndNotch = (-1)*nStartNotch; 
		} 

		strStringFormat = (Chart_Out.Info.xAxis.nDecimal < 0) ? _T("%g") : GetDoubleStringFormat(Chart_Out.Info.xAxis.nDecimal);

		if(Chart_Out.Info.xAxis.nNotchLabelType != 3)
		{
			if(Chart_Out.Info.yAxis.bReverse == (Chart_Out.Info.xAxis.nNotchLabelType != 1)) 
			{ 
				nLabelPosi = nTopValue - (int)(nYSign*(dNotchRate+1.0)*chartFormat.xAxis.fontSize);
			}
			else                                                                       
			{
				nLabelPosi = nBottomValue + (int)(nYSign*(dNotchRate+1.0)*chartFormat.xAxis.fontSize);
			}
		}
		else nLabelPosi = nCrossPosi + (int)(nYSign*(dNotchRate+1.0)*chartFormat.xAxis.fontSize)*(Chart_Out.Info.yAxis.bReverse  ? -1 : 1);

		for(i = 0; i < nMajorSize ; i++) 
		{		
			if(Chart_Out.Info.xAxis.nNotchCtrlPointType == 1)
			{
				// Level의 기준점을 높은쪽으로 할때
				dLabelValue = (Chart_Out.Info.xAxis.dMaxValue - i*Chart_Out.Info.xAxis.dMajorSpace);
			}
			else if(Chart_Out.Info.xAxis.nNotchCtrlPointType == 2)
			{
				// Level의 기준점을 낮은쪽으로 할때
				dLabelValue = (Chart_Out.Info.xAxis.dMinValue + i*Chart_Out.Info.xAxis.dMajorSpace);
			}
			else if(Chart_Out.Info.xAxis.nNotchCtrlPointType == 3)
			{
				// Level의 기준점을 원점으로 할때
				dLabelValue = (dMinValue_MajorRev + i*Chart_Out.Info.xAxis.dMajorSpace);
			}
			else ASSERT(0);			

			dgnLine_Sub.P1.x = dgnLine_Sub.P2.x = dgnText.PPosition.x = ConvertToChartValue(nLeftValue, nRightValue, dLeftValue, dRightValue, dLabelValue);

			if(Chart_Out.Info.xAxis.nMajorNotchType != 0)
			{			
				dgnLine_Sub.P1.y = nCrossPosi + nStartNotch;
				dgnLine_Sub.P2.y = nCrossPosi + nEndNotch;
				Chart_Out.xAxis.arLine[nCount] = dgnLine_Sub;
				nCount++;
			}
			if(Chart_Out.Info.xAxis.bMajorGrid)
			{
				dgnLine_Sub.P1.y = nYmin; 
				dgnLine_Sub.P2.y = nYmax;
				Chart_Out.xAxis.arMajorGrid[i] = dgnLine_Sub; 
			}
			if(Chart_Out.Info.xAxis.nNotchLabelType != 0)
			{
				TCHAR szTemp[128];
				wsprintf(szTemp, strStringFormat, dLabelValue);
				dgnText.szText = szTemp;
				dgnText.PPosition.y = nLabelPosi;			
				Chart_Out.xAxis.arText[i] = dgnText;
			}
		}

		if(Chart_Out.Info.xAxis.nMinorNotchType == 1) 
		{
			nEndNotch = nYSign*max(chartFormat.xAxis.Width*2, (int)(chartFormat.xAxis.fontSize*dNotchRate)) * (Chart_Out.Info.yAxis.bReverse  ? -1 : 1); 
		} 
		if(Chart_Out.Info.xAxis.nMinorNotchType == 2) 
		{
			nEndNotch = (-1)*nYSign*max(chartFormat.xAxis.Width*2, (int)(chartFormat.xAxis.fontSize*dNotchRate)) * (Chart_Out.Info.yAxis.bReverse  ? -1 : 1);
		} 
		if(Chart_Out.Info.xAxis.nMinorNotchType == 3) 
		{
			nStartNotch = nYSign*max(chartFormat.xAxis.Width*2, (int)(chartFormat.xAxis.fontSize*dNotchRate)); 
			nEndNotch = (-1)*nStartNotch; 
		} 		

		nStartNotch = (int)(nStartNotch*2/3);
		nEndNotch   = (int)(nEndNotch*2/3);

		for(i = 0; i < nMinorSize ; i++) 
		{					
			if(Chart_Out.Info.xAxis.nNotchCtrlPointType == 1)
			{// Level의 기준점을 높은쪽으로 할때
				dLabelValue = (Chart_Out.Info.xAxis.dMaxValue - i*Chart_Out.Info.xAxis.dMinorSpace);
			}
			else if(Chart_Out.Info.xAxis.nNotchCtrlPointType == 2)
			{// Level의 기준점을 낮은쪽으로 할때
				dLabelValue = (Chart_Out.Info.xAxis.dMinValue + i*Chart_Out.Info.xAxis.dMinorSpace);
			}
			else if(Chart_Out.Info.xAxis.nNotchCtrlPointType == 3)
			{// Level의 기준점을 원점으로 할때
				dLabelValue = (dMinValue_MinorRev + i*Chart_Out.Info.xAxis.dMinorSpace);
			}
			else ASSERT(0);			

			dLabelValue = (Chart_Out.Info.xAxis.dMinValue + i*Chart_Out.Info.xAxis.dMinorSpace);
			dgnLine_Sub.P1.x = dgnLine_Sub.P2.x = ConvertToChartValue(nLeftValue, nRightValue, dLeftValue, dRightValue, dLabelValue);

			if(Chart_Out.Info.xAxis.nMinorNotchType != 0)
			{
				dgnLine_Sub.P1.y = nCrossPosi + nStartNotch;
				dgnLine_Sub.P2.y = nCrossPosi + nEndNotch;
				Chart_Out.xAxis.arLine[nCount] = dgnLine_Sub;
				nCount++;
			}
			if(Chart_Out.Info.xAxis.bMinorGrid)
			{
				dgnLine_Sub.P1.y = nYmin;
				dgnLine_Sub.P2.y = nYmax;
				Chart_Out.xAxis.arMinorGrid[i] = dgnLine_Sub;
			}
		}

		// 축제목
		if(Chart_Out.Info.xAxis.bAxisName && Chart_Out.Info.xAxis.szName != _T(""))
		{
			Chart_Out.xAxis.title.szText = Chart_Out.Info.xAxis.szName;
			Chart_Out.xAxis.title.PPosition.x = (int)((nXmax+nXmin) / 2);

			int nLabelSize = (int)Chart_Out.xAxis.arText.GetSize();					
			int nLabelSpace = (chartFormat.xAxis.fontSize + chartFormat.xAxis.fontSize_Title)/2;

			if(chartFormat.xAxis.nEscapement != 0 && nLabelSize > 0)
			{				
				TLongStr& strLabel1 = Chart_Out.xAxis.arText[0].szText;
				TLongStr& strLabel2 = Chart_Out.xAxis.arText[nLabelSize-1].szText;
				int nLabelWidth = chartFormat.xAxis.fontSize * max(strLabel1.GetLength(), strLabel2.GetLength())/2;// 숫자이므로 간략식으로 처리

				nLabelPosi += (int)fabs(nLabelWidth*sin(chartFormat.xAxis.nEscapement/10.0 * CMathFunc::m_trrad)) * (m_bEMF ? -1 : 1);
			}
			else if(chartFormat.xAxis.nEscapement == 0)
			{ 
				nLabelSpace = (int)(1.3*nLabelSpace); 
			}
			Chart_Out.xAxis.title.PPosition.y = (m_bEMF ? min(nYmin-chartFormat.xAxis.fontSize_Title/2, nLabelPosi)-nLabelSpace : max(nYmax+chartFormat.xAxis.fontSize_Title/2, nLabelPosi)+nLabelSpace);
		}
		// y-Axis
		nCount = 0;
		dMinValue_MajorRev = (int)(Chart_Out.Info.yAxis.dMinValue/Chart_Out.Info.yAxis.dMajorSpace)*Chart_Out.Info.yAxis.dMajorSpace;
		dMinValue_MinorRev = (int)(Chart_Out.Info.yAxis.dMinValue/Chart_Out.Info.yAxis.dMinorSpace)*Chart_Out.Info.yAxis.dMinorSpace;
		if(Chart_Out.Info.yAxis.nNotchCtrlPointType != 3)
		{
			// Level의 기준점을 낮은쪽, 높은쪽 기준으로 할때
			nMajorSize = (int)((Chart_Out.Info.yAxis.dMaxValue - Chart_Out.Info.yAxis.dMinValue)/Chart_Out.Info.yAxis.dMajorSpace)+1;
			nMinorSize = (int)((Chart_Out.Info.yAxis.dMaxValue - Chart_Out.Info.yAxis.dMinValue)/Chart_Out.Info.yAxis.dMinorSpace)+1;
		}
		else 
		{
			// Level의 기준점을 원점으로 할때
			nMajorSize = (int)(Chart_Out.Info.yAxis.dMaxValue/Chart_Out.Info.yAxis.dMajorSpace) - (int)(Chart_Out.Info.yAxis.dMinValue/Chart_Out.Info.yAxis.dMajorSpace);
			if(Chart_Out.Info.yAxis.dMaxValue>=0.0 && Chart_Out.Info.yAxis.dMinValue<=0.0) nMajorSize++;
			nMinorSize = (int)(Chart_Out.Info.yAxis.dMaxValue/Chart_Out.Info.yAxis.dMinorSpace) - (int)(Chart_Out.Info.yAxis.dMinValue/Chart_Out.Info.yAxis.dMinorSpace);
			if(Chart_Out.Info.yAxis.dMaxValue>=0.0 && Chart_Out.Info.yAxis.dMinValue<=0.0) nMinorSize++;
		}

		Chart_Out.yAxis.arLine.SetSize(1 + (Chart_Out.Info.yAxis.nMajorNotchType==0 ? 0 : nMajorSize) + (Chart_Out.Info.yAxis.nMinorNotchType==0 ? 0 : nMinorSize));

		if(nMajorSize > 0) 
		{ 
			if(Chart_Out.Info.yAxis.bMajorGrid)           Chart_Out.yAxis.arMajorGrid.SetSize(nMajorSize);  
			if(Chart_Out.Info.yAxis.nNotchLabelType != 0) Chart_Out.yAxis.arText.SetSize(nMajorSize); 
		}

		if(nMinorSize > 0) 
		{
			if(Chart_Out.Info.yAxis.bMinorGrid) Chart_Out.yAxis.arMinorGrid.SetSize(nMinorSize); 
		}

		nCrossPosi = ConvertToChartValue(nLeftValue, nRightValue, dLeftValue, dRightValue, Chart_Out.Info.yAxis.dCrossPosi);
		dgnLine.P1.y = nYmin; 
		dgnLine.P2.y = nYmax;  				
		dgnLine.P1.x = dgnLine.P2.x = nCrossPosi;
		Chart_Out.yAxis.arLine[nCount] = dgnLine;
		nCount++;

		nStartNotch = nEndNotch =0;		
		dNotchRate = (Chart_Out.Info.yAxis.dNotchHeightRate <= 0) ? 1.0/3.0 : Chart_Out.Info.yAxis.dNotchHeightRate;
		if(Chart_Out.Info.yAxis.nMajorNotchType == 1) 
		{
			nEndNotch = (-1)*max(chartFormat.yAxis.Width*2, (int)(chartFormat.yAxis.fontSize*dNotchRate)) * (Chart_Out.Info.xAxis.bReverse  ? -1 : 1);
		} 
		if(Chart_Out.Info.yAxis.nMajorNotchType == 2) 
		{
			nEndNotch = max(chartFormat.yAxis.Width*2, (int)(chartFormat.yAxis.fontSize*dNotchRate) * (Chart_Out.Info.xAxis.bReverse  ? -1 : 1)); 
		} 
		if(Chart_Out.Info.yAxis.nMajorNotchType == 3) 
		{
			nStartNotch = max(chartFormat.yAxis.Width*2, (int)(chartFormat.yAxis.fontSize*dNotchRate));  
			nEndNotch = (-1)*nStartNotch;
		} 
		strStringFormat = (Chart_Out.Info.yAxis.nDecimal < 0) ? _T("%g") : GetDoubleStringFormat(Chart_Out.Info.yAxis.nDecimal);

		if(Chart_Out.Info.yAxis.nNotchLabelType != 3)
		{
			if(Chart_Out.Info.xAxis.bReverse == (Chart_Out.Info.yAxis.nNotchLabelType != 1)) nLabelPosi = nRightValue + (int)((dNotchRate+1.0)*chartFormat.xAxis.fontSize); 
			else                                                                                nLabelPosi = nLeftValue - (int)((dNotchRate+1.0)*chartFormat.xAxis.fontSize);
		}
		else nLabelPosi = nCrossPosi - (int)((dNotchRate+1.0)*chartFormat.yAxis.fontSize)*(Chart_Out.Info.xAxis.bReverse  ? -1 : 1);

		for(i = 0; i < nMajorSize ; i++) 
		{			
			if(Chart_Out.Info.yAxis.nNotchCtrlPointType == 1)
			{
				// Level의 기준점을 높은쪽으로 할때
				dLabelValue = (Chart_Out.Info.yAxis.dMaxValue - i*Chart_Out.Info.yAxis.dMajorSpace);
			}
			else if(Chart_Out.Info.yAxis.nNotchCtrlPointType == 2)
			{
				// Level의 기준점을 낮은쪽으로 할때
				dLabelValue = (Chart_Out.Info.yAxis.dMinValue + i*Chart_Out.Info.yAxis.dMajorSpace);
			}
			else if(Chart_Out.Info.yAxis.nNotchCtrlPointType == 3)
			{
				// Level의 기준점을 원점으로 할때
				dLabelValue = (dMinValue_MajorRev + i*Chart_Out.Info.yAxis.dMajorSpace);
			}
			else
				ASSERT(0);						

			dgnLine_Sub.P1.y = dgnLine_Sub.P2.y = dgnText.PPosition.y = ConvertToChartValue(nTopValue, nBottomValue, dTopValue, dBottomValue, dLabelValue);

			if(Chart_Out.Info.yAxis.nMajorNotchType != 0)
			{	
				dgnLine_Sub.P1.x = nCrossPosi + nStartNotch;
				dgnLine_Sub.P2.x = nCrossPosi + nEndNotch;
				Chart_Out.yAxis.arLine[nCount] = dgnLine_Sub;
				nCount++;
			}
			if(Chart_Out.Info.yAxis.bMajorGrid)
			{
				dgnLine_Sub.P1.x = nXmin;  dgnLine_Sub.P2.x = nXmax;
				Chart_Out.yAxis.arMajorGrid[i] = dgnLine_Sub;
			}
			if(Chart_Out.Info.yAxis.nNotchLabelType != 0)
			{
				TCHAR szTemp[128];
				wsprintf(szTemp, strStringFormat, dLabelValue);
				dgnText.szText = szTemp;
				dgnText.PPosition.x = nLabelPosi;					
				Chart_Out.yAxis.arText[i] = dgnText;
			}
		}

		if(Chart_Out.Info.yAxis.nMinorNotchType == 1) 
		{ 
			nEndNotch = (-1)*max(chartFormat.yAxis.Width*2, (int)(chartFormat.yAxis.fontSize*dNotchRate)) * (Chart_Out.Info.xAxis.bReverse  ? -1 : 1); 
		} 
		if(Chart_Out.Info.yAxis.nMinorNotchType == 2) 
		{ 
			nEndNotch = max(chartFormat.yAxis.Width*2, (int)(chartFormat.yAxis.fontSize*dNotchRate) * (Chart_Out.Info.xAxis.bReverse  ? -1 : 1)); 
		} 
		if(Chart_Out.Info.yAxis.nMinorNotchType == 3) 
		{
			nStartNotch = max(chartFormat.yAxis.Width*2, (int)(chartFormat.yAxis.fontSize*dNotchRate)); 
			nEndNotch = (-1)*nStartNotch; 
		} 

		nStartNotch = (int)(nStartNotch * 2 / 3);
		nEndNotch   = (int)(nEndNotch * 2 / 3);

		for(i = 0; i < nMinorSize ; i++) 
		{	
			if(Chart_Out.Info.yAxis.nNotchCtrlPointType == 1)
			{
				// Level의 기준점을 높은쪽으로 할때
				dLabelValue = (Chart_Out.Info.yAxis.dMaxValue - i * Chart_Out.Info.yAxis.dMinorSpace);
			}
			else if(Chart_Out.Info.yAxis.nNotchCtrlPointType == 2)
			{
				// Level의 기준점을 낮은쪽으로 할때
				dLabelValue = (Chart_Out.Info.yAxis.dMinValue + i * Chart_Out.Info.yAxis.dMinorSpace);
			}
			else if(Chart_Out.Info.yAxis.nNotchCtrlPointType == 3)
			{
				// Level의 기준점을 원점으로 할때
				dLabelValue = (dMinValue_MinorRev + i * Chart_Out.Info.yAxis.dMinorSpace);
			}
			else ASSERT(0);			

			dgnLine_Sub.P1.y = dgnLine_Sub.P2.y = ConvertToChartValue(nTopValue, nBottomValue, dTopValue, dBottomValue, dLabelValue);

			if(Chart_Out.Info.yAxis.nMinorNotchType != 0)
			{	
				dgnLine_Sub.P1.x = nCrossPosi + nStartNotch;
				dgnLine_Sub.P2.x = nCrossPosi + nEndNotch;
				Chart_Out.yAxis.arLine[nCount] = dgnLine_Sub;
				nCount++;
			}
			if(Chart_Out.Info.yAxis.bMinorGrid)
			{
				dgnLine_Sub.P1.x = nXmin; 
				dgnLine_Sub.P2.x = nXmax;
				Chart_Out.yAxis.arMinorGrid[i] =  dgnLine_Sub;
			}
		}		

		// 축제목
		if(Chart_Out.Info.yAxis.bAxisName && Chart_Out.Info.yAxis.szName != _T(""))
		{
			Chart_Out.yAxis.title.szText = Chart_Out.Info.yAxis.szName;
			Chart_Out.yAxis.title.PPosition.y = (int)((nYmax+nYmin)/2);

			int nLabelSize = (int)Chart_Out.yAxis.arText.GetSize();

			int nLabelSpace = (chartFormat.yAxis.fontSize + chartFormat.yAxis.fontSize_Title)/2;

			if(chartFormat.yAxis.nEscapement == 900 || chartFormat.yAxis.nEscapement == -900)
			{ 
				nLabelSpace = (int)(1.3*nLabelSpace);
			}
			else if(nLabelSize > 0)
			{				
				int nLabelWidth = chartFormat.yAxis.fontSize * max(Chart_Out.yAxis.arText[0].szText.GetLength(), Chart_Out.yAxis.arText[nLabelSize-1].szText.GetLength())/2;// 숫자이므로 간략식으로 처리

				nLabelPosi -= (int)fabs(nLabelWidth*cos(chartFormat.yAxis.nEscapement/10.0*CMathFunc::m_trrad));
			}			
			Chart_Out.yAxis.title.PPosition.x = (int)(min(nXmin-chartFormat.yAxis.fontSize_Title/2, nLabelPosi)-nLabelSpace);
		}
		// Graph
		nRemarkXPois = min(remarkPosi.P1.x, remarkPosi.P2.x) + (int)(chartFormat.remark.fontSize/2);
		nRemarkYPois = (int)((remarkPosi.P1.y+remarkPosi.P2.y)/2) - (int)(nYSign*(nRmSize-1)*chartFormat.remark.fontSize/2) ;

		int nPointSize, nSize = Chart_In.arData.GetSize();		
		Chart_Out.arData.SetSize(nSize);		
		nCount = 0;

		for(i = 0; i < nSize; i++)
		{
			Chart_Out.arData[i].Init();
			Chart_Out.arData[i].Info = Chart_In.arData[i].Info;
			nPointSize = Chart_In.arData[i].arPoint.GetSize();

			if(nPointSize <= 0) continue;

			Chart_Out.arData[i].arPoint.SetSize(nPointSize);
			if(nPointSize >= 2) Chart_Out.arData[i].arLine.SetSize(nPointSize - 1);

			for(j = 0; j < nPointSize; j++)
			{
				dgnPoint.x = ConvertToChartValue(nLeftValue, nRightValue, dLeftValue, dRightValue,  Chart_In.arData[i].arPoint[j].x);
				dgnPoint.y = ConvertToChartValue(nTopValue, nBottomValue, dTopValue,  dBottomValue, Chart_In.arData[i].arPoint[j].y);
				Chart_Out.arData[i].arPoint[j] = dgnPoint;

				if(j < nPointSize-1) Chart_Out.arData[i].arLine[j  ].P1 = dgnPoint;
				if(j > 0)              Chart_Out.arData[i].arLine[j-1].P2 = dgnPoint;
			}

			if(Chart_Out.arData[i].Info.nNameType == EN_CHART_DATANAME_POSI_REMARK)
			{			
				Chart_Out.arData[i].remarkLine.P1.x = nRemarkXPois;
				Chart_Out.arData[i].remarkLine.P1.y = nRemarkYPois + nYSign*nCount*chartFormat.remark.fontSize;	
				Chart_Out.arData[i].remarkLine.P2.x = nRemarkXPois + 2*chartFormat.remark.fontSize;
				Chart_Out.arData[i].remarkLine.P2.y = nRemarkYPois + nYSign*nCount*chartFormat.remark.fontSize;	

				Chart_Out.arData[i].remarkPoint.x = nRemarkXPois + 1*chartFormat.remark.fontSize;
				Chart_Out.arData[i].remarkPoint.y = nRemarkYPois + nYSign*nCount*chartFormat.remark.fontSize;		
				nCount++;
			}
		}
	}
	else ASSERT(0);
}

void CDrawFunc::DrawChart(HDC DC, T_CHART_FORMAT chartFormat, T_CHART& dgnChart)
{
	int i;

	// frame(바탕) -> axis(minorGrid-majorGrid-frame(테두리)-axis) -> graph -> remark 순으로 그려준다.
	// Line과 Rect형식이 일치할 수 있도록 m_bRectTrPolyDraw 옵션을 켜준다.
	BOOL bRectTrPolyDraw_Old = m_bRectTrPolyDraw;
	m_bRectTrPolyDraw = TRUE;

	// frame
	if     (chartFormat.frame.bLine) DrawRect(DC, chartFormat.frame.nPenStyle, chartFormat.frame.lineColor, chartFormat.frame.fillColor, chartFormat.frame.bFill, chartFormat.frame.Width, dgnChart.frame.arRect);  
	else if(chartFormat.frame.bFill) DrawRect(DC, PS_SOLID, chartFormat.frame.fillColor, chartFormat.frame.fillColor, chartFormat.frame.bFill, 0, dgnChart.frame.arRect);

	if(dgnChart.frame.arText.GetSize() > 0)
	{
		DrawText(DC, chartFormat.frame.fontColor, RGB(255,255,255), FALSE, chartFormat.frame.fontSize, chartFormat.frame.szFont.GetBuffer(), EN_POSITION_TYPE_CENTER_VCENTER, dgnChart.frame.arText);
	}

	// Axis 
	// - minor Grid
	if(dgnChart.Info.xAxis.bAxis)
	{
		if(dgnChart.Info.xAxis.bMinorGrid && dgnChart.xAxis.arMinorGrid.GetSize() > 0)
		{
			DrawLine(DC, chartFormat.xAxis.nMinorGridPenStyle, chartFormat.xAxis.minorGridColor, chartFormat.xAxis.nMinorGridWidth, dgnChart.xAxis.arMinorGrid);
		}
	}
	if(dgnChart.Info.yAxis.bAxis)
	{
		if(dgnChart.Info.yAxis.bMinorGrid && dgnChart.yAxis.arMinorGrid.GetSize() > 0)
		{
			DrawLine(DC, chartFormat.yAxis.nMinorGridPenStyle, chartFormat.yAxis.minorGridColor, chartFormat.yAxis.nMinorGridWidth, dgnChart.yAxis.arMinorGrid);
		}
	}
	// - major Grid
	if(dgnChart.Info.xAxis.bAxis)
	{
		if(dgnChart.Info.xAxis.bMajorGrid && dgnChart.xAxis.arMajorGrid.GetSize() > 0)
		{
			DrawLine(DC, chartFormat.xAxis.nMajorGridPenStyle, chartFormat.xAxis.majorGridColor, chartFormat.xAxis.nMajorGridWidth, dgnChart.xAxis.arMajorGrid);
		}
	}
	if(dgnChart.Info.yAxis.bAxis)
	{
		if(dgnChart.Info.yAxis.bMajorGrid && dgnChart.yAxis.arMajorGrid.GetSize() > 0)
		{
			DrawLine(DC, chartFormat.yAxis.nMajorGridPenStyle, chartFormat.yAxis.majorGridColor, chartFormat.yAxis.nMajorGridWidth, dgnChart.yAxis.arMajorGrid);
		}
	}
	// - frame 테두리
	if(chartFormat.frame.bLine) DrawRect(DC, chartFormat.frame.nPenStyle, chartFormat.frame.lineColor, chartFormat.frame.fillColor, FALSE, chartFormat.frame.Width, dgnChart.frame.arRect);  
	// - xAxis 
	if(dgnChart.Info.xAxis.bAxis)
	{
		if(dgnChart.xAxis.arLine.GetSize() > 0)
		{
			DrawLine(DC, chartFormat.xAxis.nPenStyle, chartFormat.xAxis.lineColor, chartFormat.xAxis.Width, dgnChart.xAxis.arLine);
		}

		EN_POSITION_TYPE LabelPosi = EN_POSITION_TYPE_CENTER_VCENTER;
		if(chartFormat.xAxis.nEscapement != 0)
		{
			if(abs(chartFormat.xAxis.nEscapement) > 900) 
			{ 
				chartFormat.xAxis.nEscapement = (chartFormat.xAxis.nEscapement > 0) ? 900 : -900; 
			}

			if((dgnChart.Info.xAxis.nNotchLabelType==1 && !dgnChart.Info.yAxis.bReverse) || (dgnChart.Info.xAxis.nNotchLabelType!=1 && dgnChart.Info.yAxis.bReverse ))
			{
				// Lable이 축의 상단에 있는 경우				
				if(chartFormat.xAxis.nEscapement > 0) LabelPosi = EN_POSITION_TYPE_LEFT_VCENTER;
				else								    LabelPosi = EN_POSITION_TYPE_RIGHT_VCENTER;
			}
			else 
			{
				// Lable이 축의 하단에 있는 경우
				if(chartFormat.xAxis.nEscapement > 0) LabelPosi = EN_POSITION_TYPE_RIGHT_VCENTER;
				else									LabelPosi = EN_POSITION_TYPE_LEFT_VCENTER;
			}			
		}

		if(dgnChart.xAxis.arText.GetSize() > 0)
		{
			DrawText(DC, chartFormat.xAxis.fontColor, RGB(255,255,255), FALSE, chartFormat.xAxis.fontSize, chartFormat.xAxis.szFont.GetBuffer(), LabelPosi, dgnChart.xAxis.arText, chartFormat.xAxis.nEscapement);
		}

		if(dgnChart.Info.xAxis.bAxisName) 
		{
			DrawText(DC, chartFormat.xAxis.fontColor_Title, RGB(255,255,255), FALSE, chartFormat.xAxis.fontSize_Title, chartFormat.xAxis.szFont_Title.GetBuffer(), EN_POSITION_TYPE_CENTER_VCENTER, dgnChart.xAxis.title);
		}
	}

	// - yAxis 
	if(dgnChart.Info.yAxis.bAxis)
	{
		if(dgnChart.yAxis.arLine.GetSize() > 0)
		{
			DrawLine(DC, chartFormat.yAxis.nPenStyle, chartFormat.yAxis.lineColor, chartFormat.yAxis.Width, dgnChart.yAxis.arLine);
		}

		if(abs(chartFormat.yAxis.nEscapement) > 900) 
		{ chartFormat.yAxis.nEscapement = (chartFormat.yAxis.nEscapement > 0) ? 900 : -900; }
		EN_POSITION_TYPE LabelPosi = EN_POSITION_TYPE_CENTER_VCENTER;

		if((dgnChart.Info.yAxis.nNotchLabelType==1 && !dgnChart.Info.xAxis.bReverse) || (dgnChart.Info.yAxis.nNotchLabelType!=1 && dgnChart.Info.xAxis.bReverse))
		{
			// Lable이 축의 우측에 있는 경우
			if(chartFormat.yAxis.nEscapement != 900 && chartFormat.yAxis.nEscapement != -900) LabelPosi = EN_POSITION_TYPE_LEFT_VCENTER;
		}
		else 
		{
			// Lable이 축의 좌측에 있는 경우
			if(chartFormat.yAxis.nEscapement != 900 && chartFormat.yAxis.nEscapement != -900) LabelPosi = EN_POSITION_TYPE_RIGHT_VCENTER;
		}					

		if(dgnChart.yAxis.arText.GetSize() > 0)
		{
			DrawText(DC, chartFormat.yAxis.fontColor, RGB(255,255,255), FALSE, chartFormat.yAxis.fontSize, chartFormat.yAxis.szFont.GetBuffer(), LabelPosi, dgnChart.yAxis.arText, chartFormat.yAxis.nEscapement);
		}

		if(dgnChart.Info.yAxis.bAxisName)
		{
			DrawText(DC, chartFormat.yAxis.fontColor_Title, RGB(255,255,255), FALSE, chartFormat.yAxis.fontSize_Title, chartFormat.yAxis.szFont_Title.GetBuffer(), EN_POSITION_TYPE_CENTER_VCENTER, dgnChart.yAxis.title, 900);
		}
	}

	// Graph
	if(dgnChart.Info.nType == EN_CHART_TYPE_DISPERSAL)
	{
		int nSize = dgnChart.arData.GetSize();
		for(int i = 0; i < nSize; i++)
		{		
			T_CHART_DATA& chartData = dgnChart.arData[i];	

			if(!dgnChart.Info.bOutDataCut || dgnChart.frame.arRect.GetSize()==0)
			{					
				if(chartData.Info.bLine)		
				{ 
					if(chartData.arLine.GetSize() > 0)
					{
						DrawLine(DC, chartData.Info.lineStyle, chartData.Info.lineColor, chartData.Info.lineWidth, chartData.arLine, PS_GEOMETRIC|PS_ENDCAP_ROUND);
					}
				}
				if(chartData.Info.bMark)
				{ 
					if(chartData.arPoint.GetSize() > 0) 
					{
						DrawPoint(DC, chartData.Info.markLineColor, chartData.Info.markSize, chartData.arPoint, chartData.Info.markType, chartData.Info.bFillMark, chartData.Info.markFillColor);
					}
				}						
			}
			else 
			{
				// Frame영역범위 밖의 정보는 Cutting하여 표시함
				nrPOINT arPoint;
				nrT_LINE arLine;				

				Cutting_OutRect(dgnChart.frame.arRect[0], chartData.Info.markSize, chartData.arPoint, chartData.arLine, arPoint, arLine);

				if(chartData.Info.bLine)		
				{ 
					if(arLine.GetSize() > 0) 
					{
						DrawLine(DC, chartData.Info.lineStyle, chartData.Info.lineColor, chartData.Info.lineWidth, arLine, PS_GEOMETRIC|PS_ENDCAP_ROUND); 
					}
				}
				if(chartData.Info.bMark)
				{ 
					if(arPoint.GetSize() > 0) 
					{
						DrawPoint(DC, chartData.Info.markLineColor, chartData.Info.markSize, arPoint, chartData.Info.markType, chartData.Info.bFillMark, chartData.Info.markFillColor); 
					}
				}						
			}
		}
	}
	else 
	{
		ASSERT(0);
	}

	// Remark
	if(dgnChart.Info.bRemark)
	{
		if(chartFormat.remark.bLine) 
		{
			DrawRect(DC, chartFormat.remark.nPenStyle, chartFormat.remark.lineColor, chartFormat.remark.fillColor, chartFormat.remark.bFill, chartFormat.remark.Width, dgnChart.remark.arRect);  
		}
		else if(chartFormat.remark.bFill)
		{
			DrawRect(DC, PS_SOLID, chartFormat.remark.fillColor, chartFormat.remark.fillColor, chartFormat.remark.bFill, 0, dgnChart.remark.arRect);
		}

		if(dgnChart.remark.arText.GetSize() > 0)
		{
			DrawText(DC, chartFormat.remark.fontColor, RGB(255,255,255), FALSE, chartFormat.remark.fontSize, chartFormat.remark.szFont.GetBuffer(), EN_POSITION_TYPE_LEFT_VCENTER, dgnChart.remark.arText);
		}

		if(dgnChart.Info.nType == EN_CHART_TYPE_DISPERSAL)
		{
			//for(auto itr = dgnChart.arData.Data.begin(); itr != dgnChart.arData.Data.end(); itr++)
			for(i = 0 ; i < dgnChart.arData.GetSize(); i++)
			{						
				T_CHART_DATA* itr = &dgnChart.arData[i];
				if(itr->Info.nNameType != EN_CHART_DATANAME_POSI_REMARK) continue;

				if(itr->Info.bLine)		
				{
					DrawLine(DC, itr->Info.lineStyle, itr->Info.lineColor, itr->Info.lineWidth, itr->remarkLine, PS_GEOMETRIC|PS_ENDCAP_ROUND);
				}
				if(itr->Info.bMark)
				{
					DrawPoint(DC, itr->Info.markLineColor, min(chartFormat.remark.fontSize, itr->Info.markSize), itr->remarkPoint, itr->Info.markType, itr->Info.bFillMark, itr->Info.markFillColor);
				}					
			}
		}
		else ASSERT(0);
	}

	// Remark외에 Data Name을 표시하는 경우
	T_TEXT DgnTextD;  

	//for(auto itr = dgnChart.arData.Data.begin(); itr != dgnChart.arData.Data.end(); itr++)
	for(i = 0 ; i < dgnChart.arData.GetSize(); i++)
	{
		T_CHART_DATA* itr = &dgnChart.arData[i];
		if(itr->Info.nNameType == EN_CHART_DATANAME_POSI_START && itr->arPoint.GetSize() > 0)		
		{
			DgnTextD.PPosition = itr->arPoint[0]; 
		}
		else if(itr->Info.nNameType == EN_CHART_DATANAME_POSI_END && itr->arPoint.GetSize() > 0)		
		{
			DgnTextD.PPosition = itr->arPoint[itr->arPoint.GetSize() - 1];
		}
		else continue;

		DgnTextD.szText = itr->Info.szName;
		DrawText(DC, chartFormat.remark.fontColor, RGB(255,255,255), FALSE, chartFormat.remark.fontSize, chartFormat.remark.szFont.GetBuffer(), itr->Info.nNamePosiType, DgnTextD);
	}

	// m_bRectTrPolyDraw옵션을 기존값으로 반환한다.
	m_bRectTrPolyDraw = bRectTrPolyDraw_Old;
}