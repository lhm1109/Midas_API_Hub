#ifndef __DB_AETL_DB_H__
#define __DB_AETL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_AETL
{
public:
	CDB_AETL();
	virtual ~CDB_AETL();
	CDBDoc* m_pDoc;

public:
	void Add(T_AETL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_AETL_D& rData);
	int GetCount();
	BOOL GetFromBaseDB(T_AETL_D& rData);
	int GetCountFromBaseDB();
protected:
	CMap<T_AETL_K,T_AETL_K,T_AETL_D,T_AETL_D&>m_aetl;
};

#endif