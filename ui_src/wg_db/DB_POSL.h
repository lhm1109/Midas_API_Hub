#ifndef __DB_POSL_DB_H__
#define __DB_POSL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_POSL
{
public:
	CDB_POSL();
	virtual ~CDB_POSL();
	CDBDoc* m_pDoc;

public:
	T_POSL_K m_nLastNum;
	T_POSL_K m_nStartNum;

public:
	void Add(T_POSL_K Key, T_POSL_D& rData);
	BOOL Del(T_POSL_K Key);

public:
	BOOL Get(T_POSL_K Key, T_POSL_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_POSL_K& rKey, T_POSL_D& rData);

protected:
	CMap<T_POSL_K, T_POSL_K, T_POSL_D, T_POSL_D&>m_posl;
};

#endif