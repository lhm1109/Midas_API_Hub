// CRCFileBase.cpp: implementation of the CCRCFileBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CRCFileBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "DgnStruct.h"


#include <math.h>
#if _MSC_VER >= 1700  
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif
#include <Afxwin.h>		// for CTime
#if !defined(_STLP_NATIVE_CPP_RUNTIME_HEADER)
#if _MSC_VER >= 1700  
#include <strstream>	// for strstream
#else
#include <strstream.h>	// for strstream
#endif
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCRCFileBase::CCRCFileBase()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_strFileNa=_T("");	// 출력 파일명
	m_bFirstCommand = TRUE;
	m_bLineLimit = TRUE;
}

CCRCFileBase::~CCRCFileBase()
{

}

void CCRCFileBase::Set_FileName(CString strFileName)
{
	m_strFileNa = strFileName;
}

CString CCRCFileBase::Get_FileName()
{
	return m_strFileNa;
}

int CCRCFileBase::GetInt(double dValue)
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

BOOL CCRCFileBase::Write_Double(double dValue, BOOL bEmpty)
{
	CString strChgValue=_T("");

	if(m_bLineLimit && m_iRecordCount+16 > 256) return FALSE;

	// Add. Jaeoh. [6/16/2009], MQC,2678
	// Vista에서 0에 가까운 -0.000... 이면 자릿수를 17자리를 출력하기 때문에 0으로 처리. 
	//if(-1.0E-14<dValue && dValue<0.0) dValue=0.0;

	if(bEmpty)
		strChgValue.Format(_T("%-16s"), _T(" "));
	else
	{
		if(dValue <=-1.0E+15 + 1.0E-7)	  {strChgValue.Format(_T("%-16e"), dValue);}
		else if(dValue <=-1.0E+14 + 1.0E-7)	  {strChgValue.Format(_T("%-15.f"), dValue);}
		else if(dValue <=-1.0E+13 + 1.0E-7)	  {strChgValue.Format(_T("%-16.f"), dValue);}
		else if(dValue <=-1.0E+12 + 1.0E-7)	  {strChgValue.Format(_T("%-15.1f"), dValue);}
		else if(dValue <=-1.0E+11 + 1.0E-7)	  {strChgValue.Format(_T("%-15.2f"), dValue);}
		else if(dValue <=-1.0E+10 + 1.0E-7)	  {strChgValue.Format(_T("%-15.3f"), dValue);}
		else if(dValue <=-1.0E+9 + 1.0E-7)	  {strChgValue.Format(_T("%-15.4f"), dValue);}
		else if(dValue <=-1.0E+8 + 1.0E-7)	  {strChgValue.Format(_T("%-15.5f"), dValue);}
		else if(dValue <=-1.0E+7 + 1.0E-7)	  {strChgValue.Format(_T("%-15.6f"), dValue);}
		else if(dValue <=-1.0E+6 + 1.0E-7)	  {strChgValue.Format(_T("%-15.7f"), dValue);}
		else if(dValue <=-1.0E+5 + 1.0E-7)	  {strChgValue.Format(_T("%-15.8f"), dValue);}
		else if(dValue <=-1.0E+4 + 1.0E-7)	  {strChgValue.Format(_T("%-15.9f"), dValue);}
		else if(dValue <=-1.0E+3 + 1.0E-7)	  {strChgValue.Format(_T("%-15.10f"), dValue);}
		else if(dValue <=-1.0E+2 + 1.0E-7)	  {strChgValue.Format(_T("%-15.11f"), dValue);}
		else if(dValue <=-1.0E+1 + 1.0E-7)	  {strChgValue.Format(_T("%-15.12f"), dValue);}
		else if(dValue <  0.0)		  {strChgValue.Format(_T("%-15.13f"), dValue);}
		else if(dValue <  1.0E+1 - 1.0E-7)	  {strChgValue.Format(_T("%-16.14f"), dValue);}
		else if(dValue <  1.0E+2 - 1.0E-7)	  {strChgValue.Format(_T("%-16.13f"), dValue);}
		else if(dValue <  1.0E+3 - 1.0E-7)	  {strChgValue.Format(_T("%-16.12f"), dValue);}
		else if(dValue <  1.0E+4 - 1.0E-7)	  {strChgValue.Format(_T("%-16.11f"), dValue);}
		else if(dValue <  1.0E+5 - 1.0E-7)	  {strChgValue.Format(_T("%-16.10f"), dValue);}
		else if(dValue <  1.0E+6 - 1.0E-7)	  {strChgValue.Format(_T("%-16.9f"), dValue);}
		else if(dValue <  1.0E+7 - 1.0E-7)	  {strChgValue.Format(_T("%-16.8f"), dValue);}
		else if(dValue <  1.0E+8 - 1.0E-7)	  {strChgValue.Format(_T("%-16.7f"), dValue);}
		else if(dValue <  1.0E+9 - 1.0E-7)	  {strChgValue.Format(_T("%-16.6f"), dValue);}
		else if(dValue < 1.0E+10 - 1.0E-7)	  {strChgValue.Format(_T("%-16.5f"), dValue);}
		else if(dValue < 1.0E+11 - 1.0E-7)	  {strChgValue.Format(_T("%-16.4f"), dValue);}
		else if(dValue < 1.0E+12 - 1.0E-7)	  {strChgValue.Format(_T("%-16.3f"), dValue);}
		else if(dValue < 1.0E+13 - 1.0E-7)	  {strChgValue.Format(_T("%-16.2f"), dValue);}
		else if(dValue < 1.0E+14 - 1.0E-7)	  {strChgValue.Format(_T("%-16.1f"), dValue);}
		else if(dValue < 1.0E+15 - 1.0E-7)	  {strChgValue.Format(_T("%-16.f"), dValue);}
		else if(dValue < 1.0E+16 - 1.0E-7)	  {strChgValue.Format(_T("%-16.f"), dValue);}
		else										              {strChgValue.Format(_T("%-16e"), dValue);}
	}

	int nLen = strChgValue.GetLength();

	if ( nLen > 16 )
	{
		strChgValue.Delete(16, nLen-16);
	}

	m_cfile.WriteString(strChgValue);
	m_iRecordCount += 16;

	return TRUE;
}

BOOL CCRCFileBase::Write_NullDouble()
{
	return Write_Double(0.0, TRUE);
}

BOOL CCRCFileBase::Write_Int(int iValue, BOOL bEmpty)
{
	CString strChgValue=_T("");

	if(m_bLineLimit && m_iRecordCount+8 > 256) return FALSE;

	if(bEmpty)
		strChgValue.Format(_T("%-8s"), _T(" "));
	else
		strChgValue.Format(_T("%-8d"), iValue);
	int nLen = strChgValue.GetLength();

	m_cfile.WriteString(strChgValue);
	m_iRecordCount += 8;

	return TRUE;
}

BOOL CCRCFileBase::Write_NullInt()
{
	return Write_Int(0, TRUE);
}

BOOL CCRCFileBase::Write_String(CString strValue, BOOL bEmpty)
{
	CString strChgValue=_T("");

	if(m_bLineLimit && m_iRecordCount+16 > 256) return FALSE;

	if(bEmpty)
		strChgValue.Format(_T("%-16s"), _T(" "));
	else
	{
		if(strValue.GetLength() > 16) strChgValue.Format(_T("%-16s"), strValue.Mid(0,16));
		else                          strChgValue.Format(_T("%-16s"), strValue);
	}
	int nLen = strChgValue.GetLength();

	m_cfile.WriteString(strChgValue);
	m_iRecordCount += 16;

	return TRUE;
}

BOOL CCRCFileBase::Write_String8(unsigned char cValue, BOOL bEmpty)
{
	CString strValue = (TCHAR) cValue;
	return Write_String8(strValue, bEmpty);
}

BOOL CCRCFileBase::Write_String8(CString strValue, BOOL bEmpty)
{
	CString strChgValue=_T("");

	if(m_bLineLimit && m_iRecordCount+8 > 256) return FALSE;

	if(bEmpty)
		strChgValue.Format(_T("%-8s"), _T(" "));
	else
	{
		if(strValue.GetLength() > 8)  strChgValue.Format(_T("%-8s"), strValue.Mid(0,8));
		else                          strChgValue.Format(_T("%-8s"), strValue);
	}
	int nLen = strChgValue.GetLength();

	m_cfile.WriteString(strChgValue);
	m_iRecordCount += 8;

	return TRUE;
}

BOOL CCRCFileBase::Write_NullString()
{
	return Write_String(_T(" "), TRUE);
}

void CCRCFileBase::Write_Command(CString strValue, BOOL bContinue)
{
	CString strChgValue=_T("");

	if(bContinue)
		strChgValue.Format(_T("\n%-8s"), strValue+_T("+"));
	else
	{
		if(m_bFirstCommand)
		{
			if(strValue.GetLength() > 8)  strChgValue.Format(_T("%-8s"), strValue.Mid(0,8));
			else                          strChgValue.Format(_T("%-8s"), strValue);
			m_bFirstCommand = FALSE;
		}
		else
		{
			if(strValue.GetLength() > 8)  strChgValue.Format(_T("\n%-8s"), strValue.Mid(0,8));
			else                          strChgValue.Format(_T("\n%-8s"), strValue);
		}
	}
	int nLen = strChgValue.GetLength();

	m_cfile.WriteString(strChgValue);
	m_iRecordCount = 8;
}

void CCRCFileBase::Write_Project(CString strValue)
{
	CString strChgValue=_T("");

	if(strValue.GetLength() > 128)  strChgValue.Format(_T("%-128s"), strValue.Mid(0,128));
	else                            strChgValue.Format(_T("%-128s"), strValue);
	int nLen = strChgValue.GetLength();

	m_cfile.WriteString(strChgValue);
}

void CCRCFileBase::Write_Company(CString strValue)
{
	CString strChgValue=_T("");

	if(strValue.GetLength() > 120)  strChgValue.Format(_T("%-120s"), strValue.Mid(0,120));
	else                            strChgValue.Format(_T("%-120s"), strValue);
	int nLen = strChgValue.GetLength();

	m_cfile.WriteString(strChgValue);

}

BOOL CCRCFileBase::Write_Enable(int iNum)
{
	if(m_iRecordCount+iNum > 256) return FALSE;

	return TRUE;
}

void CCRCFileBase::Write_NullLine()
{
	CString strChgValue=_T("");
	strChgValue.Format(_T("\n\n"));
	int nLen = strChgValue.GetLength();

	m_cfile.WriteString(strChgValue);
}