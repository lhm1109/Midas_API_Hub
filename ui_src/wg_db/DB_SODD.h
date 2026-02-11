#ifndef __DB_SODD_DB_H__
#define __DB_SODD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SODD
{
public:
	CDB_SODD();
	virtual ~CDB_SODD();
	CDBDoc* m_pDoc;

public:
	void Add(T_SODD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SODD_D& rData);
	BOOL Get(T_SODD_K Key,T_SODD_D& rData);
		//{return m_sodd.Lookup(1,rData);}
	int GetCount();
		//{return m_sodd.GetCount();}

protected:
	CMap<T_SODD_K,T_SODD_K,T_SODD_D,T_SODD_D&>m_sodd;
};

#endif