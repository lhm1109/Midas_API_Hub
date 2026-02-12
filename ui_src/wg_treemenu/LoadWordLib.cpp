#include "stdafx.h"

#include "LoadWordLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLoadWordLib* CLoadWordLib::GetInstance()
{
	static CLoadWordLib theCGP;

	return &theCGP;
}

CLoadWordLib::CLoadWordLib()
{
	m_hMod = NULL;
	m_nDocOpt = 0;
	m_strFileName = _T("");
}

CLoadWordLib::~CLoadWordLib()
{
	if(m_hMod!=NULL)
	{
		FreeLibrary(m_hMod);
		m_hMod = NULL;
	}
}

BOOL CLoadWordLib::LoadWordLibrary()
{
	TCHAR szModuleName[MAX_PATH * 4] = { '\0' };

	if(!*szModuleName)
	{
		::GetModuleFileName(NULL, szModuleName, MAX_PATH * 4);
		TCHAR *pLastBackslash = _tcsrchr(szModuleName, '\\');
		if(pLastBackslash) *(pLastBackslash) = '\0';
		//     TCHAR *pLastBackslash2 = _tcsrchr(szModuleName, '\\');
		//     if(pLastBackslash2) *(pLastBackslash2+1) = '\0';
	}
	CString tempPath;
	tempPath.Format(_T("%s"),szModuleName);
	tempPath = tempPath + _T("\\MIDAS_DyGen.dll");
	m_hMod = LoadLibrary(tempPath);	
	if(m_hMod==NULL)	
	{
		AfxMessageBox(_T("Mfc Dll failure !")) ;
		LPVOID lpMsgBuf=NULL;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);
		
		LPCTSTR str = (LPCTSTR)lpMsgBuf;
		LocalFree( lpMsgBuf );

		return FALSE;
	}

	return TRUE;
}

BOOL CLoadWordLib::IsLoadedWordLibrary()
{
	if(m_hMod!=NULL)
		return TRUE;

	return FALSE;
}

HINSTANCE CLoadWordLib::GetWordLibrary()
{
	return m_hMod;
}

void CLoadWordLib::FreeWordLibrary()
{
	if(m_hMod!=NULL)
	{
		FreeLibrary(m_hMod);

		m_hMod = NULL;
	}
}

void CLoadWordLib::SetDialogOption(int nDocOpt, CString strFileName)
{
	m_nDocOpt = nDocOpt;
	m_strFileName = strFileName;
}

void CLoadWordLib::GetDialogOption(int* nDocOpt, CString* strFileName)
{
	*nDocOpt = m_nDocOpt;
	*strFileName = m_strFileName;
}