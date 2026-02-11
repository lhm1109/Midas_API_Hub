// CMWindItemChina2012.cpp : implementation file
//
// 2003. 09. 23  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "math.h"
#include "CMWindItemChina2012Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double CCMWindItemChina2012Tool::GetWindCoff_KW (const T_WIND_CH2012 &data)
{
	switch (data.nRoughCategory)
	{
	case 1:
		return 1.28;
	case 2:
		return    1;
	case 3:
		return 0.54;
	case 4:
		return 0.26;
	default:
		ASSERT(FALSE);
		return 1.0;
	}
}

void CCMWindItemChina2012Tool::CalWind_PulseMagnifyFactor (T_WIND_CH2012 &data)
{
	double dK = GetWindCoff_KW(data);
	//X
	if (fabs(data.dPeriod_X) >1.0E-7)
	{
		double dX1 = 30 * 1/data.dPeriod_X / sqrt(dK * data.dBasicWindPressure);
		dX1 = max(dX1, 5);
		double dVal1 = CMathFunc::m_pi/(6*data.dDampingRatio);
		double dVal2 = dX1*dX1 / (pow((1+dX1*dX1), 4.0/3.0));
		data.dPulseMagnifyFactorX = sqrt(dVal1*dVal2);
	}
	//Y
	if (fabs(data.dPeriod_Y) >1.0E-7)
	{
		double dX1 = 30 * 1/data.dPeriod_Y / sqrt(dK * data.dBasicWindPressure);
		dX1 = max(dX1, 5);
		double dVal1 = CMathFunc::m_pi/(6*data.dDampingRatio);
		double dVal2 = dX1*dX1 / (pow((1+dX1*dX1), 4.0/3.0));
		data.dPulseMagnifyFactorY = sqrt(dVal1*dVal2);
	}
}

