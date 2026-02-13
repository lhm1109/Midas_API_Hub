// DGN_DgnPrint.cpp: implementation of the CDGN_DgnPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <math.h>
#include "DGN_DgnPrint.h"
#include "DGNStruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDGN_DgnPrint::CDGN_DgnPrint()
{

}

CDGN_DgnPrint::~CDGN_DgnPrint()
{

}

int CDGN_DgnPrint::GetInt(double dValue)
{
	// if value is larger than 123456.445, value is 123457.
	// else if value is smaller than 123456.444, value is 123456.
	int iValue = 0;
	//__int64 lRemnant = (__int64)floor(fabs(dValue*1000))%1000;
	//if(lRemnant >= 445)	iValue = (int)ceil(dValue);
	//else								iValue = (int)floor(dValue);
	// Coded by Seungjun MNET:No.2328 ('20060720)
	// Because negative value was calculated wrong. etc) -40000.00000000 -> -40001.
	if(dValue >= 0.0) iValue = (int)ceil(dValue-(0.445-cDgn_Zero));
	else              iValue = (int)floor(dValue+(0.445-cDgn_Zero));
	return iValue;
}

CString CDGN_DgnPrint::Change_S7_1e_S1(double dValue)
{
	// 10^Index, iCount는 출력 자릿수
	CString strNewValue = Get_ScientificForm(1, 7, dValue, TRUE);
	return strNewValue;
}

CString CDGN_DgnPrint::Change_7_1e_S2(double dValue)
{
	// 10^Index, iCount는 출력 자릿수
	CString strNewValue = Get_ScientificForm(2, 7, dValue);
	return strNewValue;
}

CString CDGN_DgnPrint::Change_6_1e_S1(double dValue)
{
	// 10^Index, iCount는 출력 자릿수
	CString strNewValue = Get_ScientificForm(1, 6, dValue);
	return strNewValue;
}

CString CDGN_DgnPrint::Change_ScientificForm(double dValue)
{
	CString strNewValue = _T("");
	strNewValue.Format(_T("%.3g"),dValue);
	return strNewValue;
}


CString CDGN_DgnPrint::Get_S7_1e_S1(double dValue, double dRatio)
{
	CString strValue=_T("");
	if(dRatio==0.0)	return strValue;
	double dRat = dValue / dRatio;
	
			 if(dRat <=-1.0E+6)	{strValue = Change_S7_1e_S1(dRat);}
	else if(dRat <=-1.0E+5)	{strValue.Format(_T("%6d"), GetInt(dRat));}
	else if(dRat <=-1.0E+4)	{strValue.Format(_T("%5d"), GetInt(dRat)); strValue = _T(" ")+strValue;}
	else if(dRat <=-1.0E+3)	{strValue.Format(_T("%6.1f"), dRat);}
	else if(dRat <=-1.0E+2)	{strValue.Format(_T("%6.2f"), dRat);}
	else if(dRat <=-1.0E+1)	{strValue.Format(_T("%6.3f"), dRat);}
	else if(dRat <  0.0)		{strValue.Format(_T("%6.4f"), dRat);}
	else if(dRat <  1.0E+1)	{strValue.Format(_T("%7.5f"), dRat);}
	else if(dRat <  1.0E+2)	{strValue.Format(_T("%7.4f"), dRat);}
	else if(dRat <  1.0E+3)	{strValue.Format(_T("%7.3f"), dRat);}
	else if(dRat <  1.0E+4)	{strValue.Format(_T("%7.2f"), dRat);}
	else if(dRat <  1.0E+5)	{strValue.Format(_T("%7.1f"), dRat);}
	else if(dRat <  1.0E+6)	{strValue.Format(_T("%6d"), GetInt(dRat)); strValue = _T(" ")+strValue;}
	else if(dRat <  1.0E+7)	{strValue.Format(_T("%7d"), GetInt(dRat));}
	else										{strValue = Change_S7_1e_S1(dRat);}
	
	return strValue;
}

CString CDGN_DgnPrint::Get_S7_1e_S1_RC(double dValue, double dRatio)
{
	CString strValue=_T("");
	if(dRatio==0.0)	return strValue;
	double dRat = dValue / dRatio;
	
			 if(dRat <=-1.0E+6)	{strValue = Change_ScientificForm(dRat);}
	else if(dRat <=-1.0E+5)	{strValue.Format(_T("%6d"), GetInt(dRat));}
	else if(dRat <=-1.0E+4)	{strValue.Format(_T("%5d"), GetInt(dRat)); strValue = _T(" ")+strValue;}
	else if(dRat <=-1.0E+3)	{strValue.Format(_T("%6.1f"), dRat);}
	else if(dRat <=-1.0E+2)	{strValue.Format(_T("%6.2f"), dRat);}
	else if(dRat <=-1.0E+1)	{strValue.Format(_T("%6.3f"), dRat);}
	else if(dRat <  0.0)		{strValue.Format(_T("%6.4f"), dRat);}
	else if(dRat <  1.0E+1)	{strValue.Format(_T("%7.5f"), dRat);}
	else if(dRat <  1.0E+2)	{strValue.Format(_T("%7.4f"), dRat);}
	else if(dRat <  1.0E+3)	{strValue.Format(_T("%7.3f"), dRat);}
	else if(dRat <  1.0E+4)	{strValue.Format(_T("%7.2f"), dRat);}
	else if(dRat <  1.0E+5)	{strValue.Format(_T("%7.1f"), dRat);}
	else if(dRat <  1.0E+6)	{strValue.Format(_T("%6d"), GetInt(dRat)); strValue = _T(" ")+strValue;}
	else if(dRat <  1.0E+7)	{strValue.Format(_T("%7d"), GetInt(dRat));}
	else										{strValue = Change_ScientificForm(dRat);}
	
	return strValue;
}

CString CDGN_DgnPrint::Get_7_1e_S2(double dValue, double dRatio)
{
	CString strValue=_T("");
	if(dRatio==0.0)	return strValue;
	double dRat = fabs(dValue) / dRatio;

			 if(dRat < 1.0E+1)	{strValue.Format(_T("%7.5f"), dRat);}
	else if(dRat < 1.0E+2)	{strValue.Format(_T("%7.4f"), dRat);}
	else if(dRat < 1.0E+3)	{strValue.Format(_T("%7.3f"), dRat);}
	else if(dRat < 1.0E+4)	{strValue.Format(_T("%7.2f"), dRat);}
	else if(dRat < 1.0E+5)	{strValue.Format(_T("%7.1f"), dRat);}
	else if(dRat < 1.0E+6)	{strValue.Format(_T("%6d"), GetInt(dRat)); strValue = _T(" ")+strValue;}
	else if(dRat < 1.0E+7)	{strValue.Format(_T("%7d"), GetInt(dRat));}
	else										{strValue = Change_7_1e_S2(dRat);}
	
	return strValue;
}

CString CDGN_DgnPrint::Get_6_1e_S1(double dValue, double dRatio)
{
	CString strValue=_T("");
	if(dRatio==0.0)	return strValue;
	double dRat = fabs(dValue) / dRatio;
	
			 if(dRat < 1.0E+1)	{strValue.Format(_T("%6.4f"), dRat);}
	else if(dRat < 1.0E+2)	{strValue.Format(_T("%6.3f"), dRat);}
	else if(dRat < 1.0E+3)	{strValue.Format(_T("%6.2f"), dRat);}
	else if(dRat < 1.0E+4)	{strValue.Format(_T("%6.1f"), dRat);}
	else if(dRat < 1.0E+5)	{strValue.Format(_T("%5d"), GetInt(dRat)); strValue = _T(" ")+strValue;}
	else if(dRat < 1.0E+6)	{strValue.Format(_T("%6d"), GetInt(dRat));}
	else										{strValue = Change_6_1e_S1(dRat);}
	
	return strValue;
}


CString CDGN_DgnPrint::Get_ScientificForm(int Index, int iCount, double dValue, BOOL bSign/*FALSE*/)
{
	// 10^Index, iCount는 출력 자릿수
	// PValue 또는 P-Value = Over Positive Limit value.
	// NValue 또는 N-Value = Over Negative Limit value.
	CString strNewValue=_T("");
	CString strValue=_T("");
	
	CString strSign=_T("");
	if(bSign==TRUE)
	{
		if(dValue < 0)	strSign = _T("-");
		else						strSign = _T("+");
	}
	else
	{
		if(dValue < 0)	
		{
			if(iCount==6)				return _T("NValue");
			else if(iCount==7)	return _T("N-Value");
		}
		else	strSign = _T("");
	}
	// 데이터를 읽자.(1.0e-12보다 작은값은 0으로 처리)
	if(dValue > -1 && dValue < 1)	strValue.Format(_T("%.12f"), dValue);
	else													strValue.Format(_T("%f"), dValue);
	
	int iLength = strValue.GetLength();
	BOOL bFlag=FALSE;
	int iChCount=0;
	int iPntCount=0;
	while(iChCount < iLength && bFlag==FALSE)
	{
		// 데이터를 하나씩 읽자.
		TCHAR chFirst = strValue.GetAt(0);
		if(chFirst=='0')
		{
			iChCount = 2; // 0. 다음부터 읽자.
			iPntCount = -1;
			CString strCh1=_T(""), strCh2=_T("");
			while(iChCount < (iLength-1) && bFlag==FALSE)
			{ 
				// strValue = 0.000000000012이면, 0이 아닌 부분에서 두 TCHAR까지만 읽어야 하므로
				// (iLength-1까지만 읽도록 하자.
				strCh1 = strValue.GetAt(iChCount);
				iChCount++;
				if(strCh1==_T("0"))	iPntCount--; // 1씩 감소
				else
				{
					/////////////////////////////////
					// 반올림 여부를 결정
					strCh1 = strValue.GetAt(iChCount-1);
					int iFig1 = _ttoi(strCh1);
					strCh2 = strValue.GetAt(iChCount);
					int iFig2 = _ttoi(strCh2);
					CString strCh3 = strValue.GetAt(iChCount+1);
					int iFig3 = _ttoi(strCh3);
					if(iFig3 >= 5)	iFig2 += 1;
					if(iFig2==10)  
					{
						iFig1 += 1;
						iFig2  = 0;
					}
					if(iFig1==10)
					{
						iFig1 = 1;
						iFig2 = 0;
						iPntCount += 1;
					}
					strCh1.Format(_T("%1d"), iFig1);
					strCh2.Format(_T("%1d"), iFig2);
					// 반올림 여부를 결정
					/////////////////////////////////
					strNewValue = strCh1 + _T(".") + strCh2;
					CString strPnt=_T("");
					if(iPntCount < -9)
					{
						if(Index==1 && iCount==6)				return _T("NValue");
						else if(Index==1 && iCount==7)	return _T("N-Value");
						else if(Index==2)
						{
							strPnt.Format(_T("%1d"), (-1)*iPntCount);
							strNewValue = strSign + strNewValue + _T("E-") + strPnt;
						}
					}
					else if(iPntCount < 0)
					{
						strPnt.Format(_T("%1d"), (-1)*iPntCount);
						if(Index==1)			strNewValue = strSign + strNewValue + _T("E-") + strPnt;
						else if(Index==2)	strNewValue = strSign + strNewValue + _T("E-0") + strPnt;
					}
					bFlag = TRUE;
				}
			}
		}
		else if(chFirst!='0')
		{
			iChCount=1; // 0. 다음부터 읽자.
			iPntCount=0;
			CString strCh1=_T(""), strCh2=_T("");
			while(iChCount < iLength && bFlag==FALSE)
			{
				strCh1 = strValue.GetAt(iChCount);
				iChCount++;
				if(strCh1!=_T("."))	iPntCount++; // 1씩 증가
				else
				{
					/////////////////////////////////
					// 반올림 여부를 결정
					CString strCh3=_T("");
					if(strSign==_T("-"))
					{ // (-)
						strCh1 = strValue.GetAt(1);
						strCh2 = strValue.GetAt(2);
						strCh3 = strValue.GetAt(3);
						iPntCount -= 1;
					}
					else
					{ // (+)
						strCh1 = strValue.GetAt(0);
						strCh2 = strValue.GetAt(1);
						strCh3 = strValue.GetAt(2);
					}
					int iFig1 = _ttoi(strCh1);
					int iFig2 = _ttoi(strCh2);
					int iFig3 = _ttoi(strCh3);
					if(iFig3 >= 5)	iFig2 += 1;
					if(iFig2==10)  
					{
						iFig1 += 1;
						iFig2  = 0;
					}
					if(iFig1==10)
					{
						iFig1 = 1;
						iFig2 = 0;
						iPntCount += 1;
					}
					strCh1.Format(_T("%1d"), iFig1);
					strCh2.Format(_T("%1d"), iFig2);
					// 반올림 여부를 결정
					/////////////////////////////////
					
					strNewValue = strCh1 + _T(".") + strCh2;
					CString strPnt=_T("");
					if(iPntCount > 9)
					{
						if(Index==1 && iCount==6)				return _T("PValue");
						else if(Index==1 && iCount==7)	return _T("P-Value");
						else if(Index==2)
						{
							strPnt.Format(_T("%1d"), iPntCount);
							strNewValue = strSign + strNewValue + _T("E+") + strPnt;
						}
					}
					else if(iPntCount > 0)
					{
						strPnt.Format(_T("%1d"), iPntCount);
						if(Index==1)			strNewValue = strSign + strNewValue + _T("E+") + strPnt;
						else if(Index==2)	strNewValue = strSign + strNewValue + _T("E+0") + strPnt;
					}
					bFlag = TRUE;
				}
			}
		}
	}

	if(bFlag==FALSE)
	{
		if(bSign==FALSE)
		{
			if(Index==1 && iCount==6)	strNewValue = _T("0.0E+0");
			if(Index==1 && iCount==7)	strNewValue = _T("0.0E+00");
		}
		else if(bSign==TRUE)
		{
			if(Index==1 && iCount==7)	strNewValue = _T("+0.0E+0");
		}
	}
	return strNewValue;
}

double CDGN_DgnPrint::Get_ScientificValue(int nDecimal, double dValue)
{
	double dMax = 0.0;
	double dMin = 0.0; 
	int nCount = 0;
	
	for(int i=0 ; i<100 ; i++)
	{
		double dMax = pow(10, nCount+1);
		double dMin = pow(10, nCount);

		if(fabs(dValue) < dMax && fabs(dValue) >= dMin)
			break;
		if(fabs(dValue) >= dMax)
			nCount++;
		else if(fabs(dValue) <= dMin)
			nCount--;
		else
			break;
	}
	double dScale = pow(10, nCount+1-nDecimal);

	double dSValue = (dScale==0.0) ? 0.0 : dValue/dScale;
	dSValue = double((int)(dSValue+0.5));

	return dSValue*dScale;
}

CString CDGN_DgnPrint::GetPrintString(int nLength, const CString& strValue)
{
	CStringA strValA(strValue);

	CStringA strValOut = "";
	// iLength (22=Member Name, 9=Material Name).
	if ( strValA.GetLength() > nLength )
	{
		strValOut = strValA.Left(nLength-1) + "~";
	}
	else
	{
		strValOut = strValA;
	}

	CStringA strForm = "";
	strForm.Format("%%-%ds", nLength);

	CStringA strString = "";
	strString.Format(strForm, strValOut);
	return CString(strString);
}