#ifndef __DB_SIGR_UPPER_DB_H__
#define __DB_SIGR_UPPER_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_NODE;

class CDB_SIGR_UPPER
{
public:
	CDB_SIGR_UPPER();
	virtual ~CDB_SIGR_UPPER();
	CDBDoc* m_pDoc;

public:
	T_SIGR_UPPER_K m_nStartNum;
	T_SIGR_UPPER_K m_nLastNum;

public:
	void Add(T_SIGR_UPPER_K Key, T_SIGR_UPPER_D& rData, CDB_ELEM* pElem, CDB_NODE* pNode);
	BOOL Del(T_SIGR_UPPER_K Key, CDB_ELEM* pElem, CDB_NODE* pNode);

public:
	BOOL Get(T_SIGR_UPPER_K Key, T_SIGR_UPPER_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_UPPER_K& rKey, T_SIGR_UPPER_D& rData);

protected:
	CMap<T_SIGR_UPPER_K, T_SIGR_UPPER_K, T_SIGR_UPPER_D, T_SIGR_UPPER_D&>m_sigrUpper;
};

#endif