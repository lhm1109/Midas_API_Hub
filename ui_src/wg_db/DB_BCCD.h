#ifndef __DB_BCCD_DB_H__
#define __DB_BCCD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
//class CDB_ELEM;

class CDB_BCCD
{
public:
	CDB_BCCD();
	virtual ~CDB_BCCD();
	CDBDoc* m_pDoc;

public:
	T_BCCD_K m_nLastNum;
	T_BCCD_K m_nStartNum;

public:
	void Add(T_BCCD_K Key, T_BCCD_D& rData);
	BOOL Del(T_BCCD_K Key);

public:
	BOOL Get(T_BCCD_K Key, T_BCCD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_BCCD_K& rKey, T_BCCD_D& rData);

protected:
	CMap<T_BCCD_K, T_BCCD_K, T_BCCD_D, T_BCCD_D&>m_bccd;
};

#endif