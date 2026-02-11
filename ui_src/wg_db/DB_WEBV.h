#ifndef __DB_WEBV_DB_H__
#define __DB_WEBV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_WEBV
{
public:
	CDB_WEBV();
	virtual ~CDB_WEBV();
	CDBDoc* m_pDoc;

public:
	T_WEBV_K m_nStartNum;
	T_WEBV_K m_nLastNum;

public:
	void Add(T_WEBV_K Key, T_WEBV_D& rData);
	BOOL Del(T_WEBV_K Key);

public:
	BOOL Get(T_WEBV_K Key, T_WEBV_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_WEBV_K& rKey, T_WEBV_D& rData);
	BOOL GetWebvAssigned(T_VBEM_K KeyElem, T_WEBV_K& rKeyWebv);   // element가 속한 webv의 key를 찾아준다.


protected:
	CMap<T_WEBV_K, T_WEBV_K, T_WEBV_D, T_WEBV_D&> m_webv;
	CMap<T_VBEM_K, T_VBEM_K, T_WEBV_K, T_WEBV_K>  m_elemtowebv;
};

#endif

