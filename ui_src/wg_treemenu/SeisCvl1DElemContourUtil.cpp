// CSeisCvl1DElemContourUtil.cpp : implementation file
//

#include "stdafx.h"
#include "SeisCvl1DElemContourUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <map>

/////////////////////////////////////////////////////////////////////////////
// CSeisCvl1DElemContourUtil dialog
CSeisCvl1DElemContourUtil::CSeisCvl1DElemContourUtil()
{
	
}

BOOL CSeisCvl1DElemContourUtil::Get1DElemContour(std::vector<double>& aX, std::vector<double>& aY, int nGlobalDiv, int nValDiv, 
																								 std::vector<double>& aXGlobalDiv, std::vector<double>& aYGlobalDiv, std::vector<COLORREF>& aColorGlobalDiv,
																								 std::vector<double>& aXValDiv, std::vector<double>& aYValDiv, std::vector<COLORREF>& aColorValDiv)
{
	aXGlobalDiv.clear();
	aYGlobalDiv.clear();
	aColorGlobalDiv.clear();

	aXValDiv.clear();
	aYValDiv.clear();
	aColorValDiv.clear();

	for (int i = 0; i < aX.size() - 1; i++)
	{
		if(aX[i + 1] - aX[i] <= 0.0)
		{
			return FALSE; 
		}
	}

	if(aX.size() <= 2)
	{
		return FALSE;
	}

	if(aX.size() != aY.size())
	{
		return FALSE;
	}

	if(nGlobalDiv == 0)
	{
		return FALSE;
	}

	double dMinX = aX[0];
	double dMaxX = aX[aX.size() - 1];

	double dMinY, dMaxY;
	dMinY = DBL_MAX;
	dMaxY = -DBL_MAX;
	for (int i = 0; i < aY.size(); i++)
	{
		if(aY[i] < dMinY)
		{
			dMinY = aY[i];
		}
		if(aY[i] > dMaxY)
		{
			dMaxY = aY[i];
		}
	}

	double dDivVal = (dMaxX - dMinX) / nGlobalDiv;

	//// Global Div ////
	std::map<double, double> mapRstXToYGlobalDiv;
	double dXLocal;
	for (int i = 1; i < nGlobalDiv; i++)
	{
		dXLocal = dMinX + (dDivVal * i);
		mapRstXToYGlobalDiv[dXLocal] = CalcYLinear(aX, aY, dXLocal);
	}
	for (int i = 0; i < aX.size(); i++)
	{
		mapRstXToYGlobalDiv[aX[i]] = aY[i];
	}
	for(auto itrLoop : mapRstXToYGlobalDiv)
	{
		aXGlobalDiv.push_back(itrLoop.first);
		aYGlobalDiv.push_back(itrLoop.second);
		aColorGlobalDiv.push_back(GetColor(dMinY, dMaxY, itrLoop.second));
	}

	//// ValDiv ////
	std::map<double, double> mapRstXToYValDiv;  
	for (int i = 0; i < aX.size(); i++)
	{
		mapRstXToYValDiv[aX[i]] = aY[i];

		if(i == aX.size() - 1) break;

		for (int j = 0; j < nValDiv - 1; j++)
		{
			dXLocal = aX[i] + (aX[i + 1] - aX[i]) * ((double)(j + 1) / nValDiv);
			mapRstXToYValDiv[dXLocal] = CalcYLinear(aX, aY, dXLocal);
		}
	}
	for(auto itrLoop : mapRstXToYValDiv)
	{
		aXValDiv.push_back(itrLoop.first);
		aYValDiv.push_back(itrLoop.second);
		aColorValDiv.push_back(GetColor(dMinY, dMaxY, itrLoop.second));
	}

	return TRUE;
}

COLORREF CSeisCvl1DElemContourUtil::GetColor(double dMinY, double dMaxY, double dY)
{
	const int nRGBNum = 11;

	// 다른 색구성을 해보고 싶으면 T_ContourData::InitColorTable() 에 가서 퍼온다음에 코드를 고쳐서 쓰세요
	// R->G->B
	// R, B values extend through half range
	//static COLORREF aRGB[nRGBNum] = {
	//  RGB(  0,   0, 255),	// Blue
	//  RGB(  0, 255, 255),
	//  RGB(  0, 255,   0),	// Green
	//  RGB(170, 255,   0),
	//  RGB(255, 255,   0),	// Yellow
	//  RGB(255, 127,   0),
	//  RGB(255,   0,   0)};	// Red

	static COLORREF aRGB[nRGBNum] = {
		RGB(  0,   0, 255),
		RGB(  0, 255, 255),
		RGB(  0, 255, 102),
		RGB(  0, 255,   0),
		RGB(102, 255,   0),
		RGB(170, 255,   0),
		RGB(221, 255,   0),
		RGB(255, 255,   0),
		RGB(255, 177,   0),
		RGB(255, 127,   0),
		RGB(255,  50,   0)};

	if(dMaxY < dMinY) return aRGB[0];

	if(dY < dMinY)
	{
		return aRGB[0];
	}
	else if(dMaxY < dY)
	{
		return aRGB[nRGBNum - 1];
	}

	double dLen = dMaxY - dMinY;

	double aDivVal[nRGBNum];
	for (int i = 1; i < nRGBNum - 1; i++)
	{
		aDivVal[i] = dMinY + i * (dLen / nRGBNum);
	}
	aDivVal[0] = dMinY;
	aDivVal[nRGBNum - 1] = dMaxY;


	double dLenY, dYLocal;
	double dRatio;

	for (int i = 0; i < nRGBNum - 1; i++)
	{
		if(dY == aDivVal[i])
		{
			return aRGB[i];
		}
		else if(dY == aDivVal[i + 1])
		{
			return aRGB[i + 1];
		}
		else if(aDivVal[i] < dY && dY < aDivVal[i + 1])
		{
			dLenY = aDivVal[i + 1] - aDivVal[i];
			dLenY = aDivVal[i + 1] - aDivVal[i];
			dYLocal = dY - aDivVal[i];
			dRatio = (dYLocal / dLenY);

			//nR = GetRValue(aRGB[i]) + (GetRValue(aRGB[i + 1]) - GetRValue(aRGB[i])) * dRatio;
			//nG = GetGValue(aRGB[i]) + (GetGValue(aRGB[i + 1]) - GetGValue(aRGB[i])) * dRatio;
			//nB = GetBValue(aRGB[i]) + (GetBValue(aRGB[i + 1]) - GetBValue(aRGB[i])) * dRatio;

			if(dRatio < 0.5)
			{
				return aRGB[i];
			}
			else
			{
				return aRGB[i + 1];
			}
		}
	}

	return TRUE;
}

double CSeisCvl1DElemContourUtil::CalcYLinear(std::vector<double>& aX, std::vector<double>& aY, double dX)
{
	// aX는 오름차순 정렬되어 있다고 가정함
	if(dX < aX[0] || aX[aX.size() - 1] < dX)
	{
		return 0.0;
	}
	
	double dLenX, dLenY, dXLocal;
	double dRatio;
	for (int i = 0; i < aX.size() - 1; i++)
	{
		if(dX == aX[i])
		{
			return aY[i];
		}
		else if(dX == aX[i + 1])
		{
			return aY[i + 1];
		}
		else if(aX[i] < dX && dX < aX[i + 1])
		{
			dLenX = aX[i + 1] - aX[i];
			dLenY = aY[i + 1] - aY[i];
			dXLocal = dX - aX[i];
			dRatio = (dXLocal / dLenX);
			return aY[i] + dLenY * dRatio;
		}
	}
	ASSERT(0); // 여긴 안들어오는게 정상임
	return 0.0;
}

