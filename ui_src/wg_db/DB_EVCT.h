#ifndef __DB_EVCT_DB_H__
#define __DB_EVCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_EVCT
{
public:
	CDB_EVCT();
	virtual ~CDB_EVCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_EVCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_EVCT_D& rData);
		//{return m_evct.Lookup(1,rData);}
	int GetCount();
		//{return m_evct.GetCount();}

protected:
	CMap<T_EVCT_K,T_EVCT_K,T_EVCT_D,T_EVCT_D&>m_evct;
};

#endif