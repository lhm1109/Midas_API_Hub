#ifndef __DB_EXLD_DB_H__
#define __DB_EXLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_EXLD
{
public:
	CDB_EXLD();
	virtual ~CDB_EXLD();
	CDBDoc* m_pDoc;

public:
	void Add(T_EXLD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_EXLD_D& rData);
		//{return m_exld.Lookup(1,rData);}
	int GetCount();
		//{return m_exld.GetCount();}

protected:
	CMap<T_EXLD_K,T_EXLD_K,T_EXLD_D,T_EXLD_D&>m_exld;
};

#endif