#ifndef __DB_RDPO_DB_H__
#define __DB_RDPO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RDPO
{
public:
	CDB_RDPO();
	virtual ~CDB_RDPO();
	CDBDoc* m_pDoc;

public:
	void Add(T_RDPO_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RDPO_D& rData);
		//{return m_rdpo.Lookup(1,rData);}
	int GetCount();
		//{return m_rdpo.GetCount();}

protected:
	CMap<T_RDPO_K,T_RDPO_K,T_RDPO_D,T_RDPO_D&>m_rdpo;
};

#endif