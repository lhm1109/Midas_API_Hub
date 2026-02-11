#ifndef __DB_SIGR_LOWER_DB_H__
#define __DB_SIGR_LOWER_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_SIGR_LOWER
{
public:
	CDB_SIGR_LOWER();
	virtual ~CDB_SIGR_LOWER();
	CDBDoc* m_pDoc;

public:
	T_SIGR_LOWER_K m_nStartNum;
	T_SIGR_LOWER_K m_nLastNum;

public:
	void Add(T_SIGR_LOWER_K Key, T_SIGR_LOWER_D& rData, CDB_NODE* pNode, CDB_ELEM* pElem);
	BOOL Del(T_SIGR_LOWER_K Key, CDB_NODE* pNode, CDB_ELEM* pElem);

public:
	BOOL Get(T_SIGR_LOWER_K Key, T_SIGR_LOWER_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_LOWER_K& rKey, T_SIGR_LOWER_D& rData);

protected:
	CMap<T_SIGR_LOWER_K, T_SIGR_LOWER_K, T_SIGR_LOWER_D, T_SIGR_LOWER_D&> m_sigrLower;
};

#endif