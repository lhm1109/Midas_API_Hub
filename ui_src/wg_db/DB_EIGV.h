#ifndef __DB_EIGV_DB_H__
#define __DB_EIGV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_EIGV
{
public:
	CDB_EIGV();
	virtual ~CDB_EIGV();
	CDBDoc* m_pDoc;

public:
	void Add(T_EIGV_D& rData);
	BOOL Del();

public:
	BOOL Get(T_EIGV_D& rData);
		//{return m_eigv.Lookup(1,rData);}
	int GetCount();
		//{return m_eigv.GetCount();}

protected:
	CMap<T_EIGV_K,T_EIGV_K,T_EIGV_D,T_EIGV_D&>m_eigv;
};

#endif