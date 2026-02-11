#ifndef __DB_RPAS_DB_H__
#define __DB_RPAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RPAS
{
public:
	CDB_RPAS();
	virtual ~CDB_RPAS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RPAS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RPAS_D& rData);
		//{return m_rpas.Lookup(1,rData);}
	int GetCount();
		//{return m_rpas.GetCount();}

protected:
	CMap<T_RPAS_K,T_RPAS_K,T_RPAS_D,T_RPAS_D&>m_rpas;
};

#endif