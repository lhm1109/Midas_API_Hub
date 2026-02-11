#include "stdafx.h"
#include "StringDeepCopyCollector.h"

CStringDeepCopyCollector::CStringDeepCopyCollector()
{
	m_aDelete.clear();
}

CStringDeepCopyCollector::~CStringDeepCopyCollector()
{
	for (TCHAR* pDel : m_aDelete)
		_SAFE_DELETE(pDel);
}

TCHAR* CStringDeepCopyCollector::StringDeepCopy(CString& str)
{
	TCHAR* lpstr = new TCHAR[str.GetLength() + 1];
	memset(lpstr, 0, str.GetLength() + 1);
	lstrcpyn(lpstr, str, str.GetLength() + 1);
	m_aDelete.push_back(lpstr);
	return lpstr;
}
