
#include "stdafx.h"

#include "DgnTendonFileStream.h"
#include <float.h>
#include <math.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 1. Read Function  (******Concept : Oneline Read******) ////////////////////////////////////////////////////////////////////////////
BOOL CTendonFileStream::ReadOpen(LPCTSTR strDir) 
{
	CFileException e;   
	if(! file.Open(strDir, CFile::modeRead | CFile::typeText, &e)) return FALSE;
	return TRUE; 
}

void CTendonFileStream::ReadClose()
{
	if(file.m_pStream) file.Close();
}

// 2.1 Read One Line('\n')
BOOL CTendonFileStream::GetLine(TCHAR* str)
{	
	if(! file.ReadString(szLine)) return FALSE; 
	str = szLine.GetBuffer(szLine.GetLength()); 
	return TRUE;
}
BOOL CTendonFileStream::GetLine(CString& str)
{ 
	if(! file.ReadString(szLine)) return FALSE; 
	str = szLine; 
	return TRUE;
}
BOOL CTendonFileStream::GetLine()											  
{
	if(! file.ReadString(szLine)) return FALSE; 
	return TRUE; 
}
CString CTendonFileStream::GetLineString() 
{ 
	return szLine;
}
TCHAR* CTendonFileStream::GetLineStringC() 
{ 
	return szLine.GetBuffer(szLine.GetLength()); 
}
void CTendonFileStream::SetLineBack() 
{
	file.Seek(-szLine.GetLength(), CFile::current);
}
BOOL CTendonFileStream::IsEmpty()  
{
	CString str = szLine;
	str.TrimLeft();
	str.TrimRight();
	return str.IsEmpty();
}

// 2.2 Etc (File Field)	
void CTendonFileStream::SetStartPos()			
{ 
	file.Seek(0, CFile::begin); 
}
void CTendonFileStream::SetEndPos()  
{ 
	file.Seek(0, CFile::end);
}
LONGLONG CTendonFileStream::GetCurPos()								
{ 
	return file.GetPosition(); 
}
void CTendonFileStream::SetCurPos(long nLen) 				 
{ 
	file.Seek(nLen, CFile::begin);
}
void CTendonFileStream::SetBack(long nLen)        
{ 
	file.Seek(-nLen, CFile::current);
}
// 2.3 Get Variable (One Line Data Field(stringstream))
CString CTendonFileStream::GetString()                  
{ 
	return StrInLine(szLine);
} // Separate Space                               
void CTendonFileStream::GetString(CString& str) 				
{
	str = StrInLine(szLine);
}
void CTendonFileStream::GetString(TCHAR* str)		 				 
{ 
	str = szLine.GetBuffer(szLine.GetLength());
}
CString CTendonFileStream::GetString(int nLen)						
{ 
	return StrInLine(szLine, nLen);
} 
void CTendonFileStream::GetString(CString& str, int nLen) 
{ 
	str = StrInLine(szLine, nLen);
}
void CTendonFileStream::GetString(TCHAR* str, int nLen)	
{ 
	str = StrInLine(szLine, nLen).GetBuffer(szLine.GetLength());
}
int CTendonFileStream::GetInteger()	 		  					
{ 
	int n; 
	GetInteger(n);
	return n; 
}
void CTendonFileStream::GetInteger(int& nV)	
{ 
	nV = _ttoi(StrInLine(szLine));
}
double CTendonFileStream::GetDouble()	
{ 
	double d; 
	GetDouble(d);
	return d;
}
void CTendonFileStream::GetDouble(double& dV)			
{ 
	dV = _tstof(StrInLine(szLine).GetBuffer(StrInLine(szLine).GetLength()));
}	

CString CTendonFileStream::StrInLine(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	szTemp = str;		

	for(int no = 0; no <= str.GetLength(); no++)
	{
		if(str[no] == ' ' || str[no] == ',' || str[no] == ';' || str[no] == '\n' || str[no] == '\t' || str[no] == '=')
		{
			szTemp = szTemp.Left(no);
			str = str.Right(str.GetLength() - (no + 1));
			break;
		}			
		if(no == str.GetLength()) { str = _T(""); break; }
	}

	return szTemp;
}

CString CTendonFileStream::StrInLine(CString& str, unsigned int nLen)
{
	if(nLen > 0) 
	{
		szTemp = str;
		szTemp = szTemp.Left(nLen);
		str = str.Right(str.GetLength() - nLen);
		szTemp.TrimLeft();
		szTemp.TrimRight();
		return szTemp;
	}
	return _T("");
}

BOOL CTendonFileStream::CheckCommentLine()
{
	CString szT = szLine;
	szT.TrimLeft();
	szT.TrimRight();
	if(szT[0] == szComment) return TRUE;
	return FALSE;  
}

BOOL CTendonFileStream::CheckComment()
{
	CString szT = szTemp;
	szT.TrimLeft();
	szT.TrimRight();
	if(szT[0] == szComment) return TRUE;
	return FALSE;  
}

BOOL CTendonFileStream::CheckComment(CString& str, TCHAR szCom)
{
	CString szT = str;
	szT.TrimLeft();
	szT.TrimRight();
	if(szT[0] == szCom) return TRUE;
	return FALSE;  
}

BOOL CTendonFileStream::CheckComment(TCHAR* str, TCHAR szCom)
{
	CString szT = str;
	szT.TrimLeft();
	szT.TrimRight();
	if(szT[0] == szCom) return TRUE;
	return FALSE;  
}

BOOL CTendonFileStream::SkipCommentLine()
{
	for(;;) 
	{ 
		if(! GetLine()) return FALSE; 
		if(IsEmpty()) continue; 
		if(! CheckCommentLine()) break; 
	}

	return TRUE; 
}

CString CTendonFileStream::GetFormat(double dV)
{
	TCHAR szTemp[xWORDSIZE];

	if (fabs(dV) < DBL_EPSILON) lstrcpy(szTemp, _T("0"));
	else
	{
		_stprintf(szTemp, _T("%g"), dV);
		//sprintf_s(szTemp, sizeof(szTemp), _T("%g"), dV);
		//swprintf_s(szTemp, sizeof(szTemp), _T("%g"), dV);

		if(lstrlen(szTemp) > nLength) 
		{
			_stprintf(szTemp, _T("%.*g"), nPrecision, dV);
			//sprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);
			//swprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);
		}
		else return szTemp;

		if(lstrlen(szTemp) > nLength) 
		{
			_stprintf(szTemp, _T("%.*e"), nPrecision, dV);
			//sprintf_s(szTemp, sizeof(szTemp), _T("%.*e"), nPrecision, dV);
			//swprintf_s(szTemp, sizeof(szTemp), _T("%.*e"), nPrecision, dV);
		}
		else return szTemp;

		if(lstrlen(szTemp) > nLength)
		{
			_stprintf(szTemp, _T("%.*f"), nPrecision, dV);
			//sprintf_s(szTemp, sizeof(szTemp), _T("%f"), nPrecision, dV);
			//swprintf_s(szTemp, sizeof(szTemp), _T("%f"), nPrecision, dV);
			lstrcpyn(szTemp, szTemp, nLength);
		}
	}

	return szTemp;
}

void CTendonFileStream::GetFormat(double dV, TCHAR* szData)
{
	TCHAR szTemp[xWORDSIZE];

	if (fabs(dV) < DBL_EPSILON) lstrcpy(szTemp, _T("0"));
	else
	{
		_stprintf(szTemp, _T("%g"), dV);
		//sprintf_s(szTemp, sizeof(szTemp), _T("%g"), dV);
		//swprintf_s(szTemp, sizeof(szTemp), _T("%g"), dV);

		if(lstrlen(szTemp) > nLength) 
		{
			_stprintf(szTemp, _T("%.*g"), nPrecision, dV);
			//sprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);
			//swprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);
		}
		else { lstrcpy(szData, szTemp); return; }

		if(lstrlen(szTemp) > nLength) 
		{
			_stprintf(szTemp, _T("%.*e"), nPrecision, dV);
			//sprintf_s(szTemp, sizeof(szTemp), _T("%.*e"), nPrecision, dV);
			//swprintf_s(szTemp, sizeof(szTemp), _T("%.*e"), nPrecision, dV);
		}
		else { lstrcpy(szData, szTemp); return; }

		if(lstrlen(szTemp) > nLength)
		{
			_stprintf(szTemp, _T("%.*f"), nPrecision, dV);
			//sprintf_s(szTemp, sizeof(szTemp), _T("%f"), nPrecision, dV);
			//swprintf_s(szTemp, sizeof(szTemp), _T("%f"), nPrecision, dV);
			lstrcpyn(szTemp, szTemp, nLength);			
		}
	}

	lstrcpy(szData, szTemp);
}

CString CTendonFileStream::GetFormat(double dV, int nPrecision)
{
	TCHAR szTemp[xWORDSIZE];
	_stprintf(szTemp, _T("%.*g"), nPrecision, dV);
	//sprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);
	//swprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);
	return szTemp;
}

CString CTendonFileStream::GetFormat(double dV, int nPrecision, int nLength)
{
	TCHAR szTemp[xWORDSIZE];
	_stprintf(szTemp, _T("%.*g"), nPrecision, dV);
	//sprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);
	//swprintf_s(szTemp, sizeof(szTemp), _T("%.*g"), nPrecision, dV);

	if(lstrlen(szTemp) > nLength)
	{
		lstrcpyn(szTemp, szTemp, nLength);			
	}
	return szTemp;
}

