#ifndef __DB_METH_DB_H__
#define __DB_METH_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_METH
{
public:
	CDB_METH();
	virtual ~CDB_METH();
	CDBDoc* m_pDoc;

public:
	void Add(T_METH_D& rData);
	BOOL Del();

public:
	BOOL Get(T_METH_D& rData);
		//{return m_meth.Lookup(1,rData);}
	int GetCount();
		//{return m_meth.GetCount();}

protected:
	CMap<T_METH_K,T_METH_K,T_METH_D,T_METH_D&>m_meth;
};

#endif