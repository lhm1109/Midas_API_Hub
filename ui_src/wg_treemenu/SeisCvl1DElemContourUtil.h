#if !defined(__SeisCvl1DElemContourUtil_H__)
#define __SeisCvl1DElemContourUtil_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvl1DElemContourUtil.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

class CSeisCvl1DElemContourUtil
{
public:  
	CSeisCvl1DElemContourUtil();
	static BOOL Get1DElemContour(std::vector<double>& aX, std::vector<double>& aY, int nGlobalDiv, int nValDiv, std::vector<double>& aXGlobalDiv, std::vector<double>& aYGlobalDiv, std::vector<COLORREF>& aColorGlobalDiv, std::vector<double>& aXValDiv, std::vector<double>& aYValDiv, std::vector<COLORREF>& aColorValDiv);
protected:
	static double CalcYLinear(std::vector<double>& aX, std::vector<double>& aY, double dX);
	static COLORREF GetColor(double dMinY, double dMaxY, double dY);
};

#endif // !defined(__SeisCvl1DElemContourUtil_H__)
