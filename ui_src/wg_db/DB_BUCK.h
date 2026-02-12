#ifndef __DB_BUCK_DB_H__
#define __DB_BUCK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_BUCK
{
public:
	CDB_BUCK();
	virtual ~CDB_BUCK();
	CDBDoc* m_pDoc;

public:
	void Add(T_BUCK_D& rData);
	BOOL Del();

public:
	BOOL Get(T_BUCK_D& rData);
		//{return m_buck.Lookup(1,rData);}
	int GetCount();
		//{return m_buck.GetCount();}

protected:
	CMap<T_BUCK_K,T_BUCK_K,T_BUCK_D,T_BUCK_D&>m_buck;
};

#endif