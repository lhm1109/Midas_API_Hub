#ifndef __DB_IUHP_DB_H__
#define __DB_IUHP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IUHP
{
public:
	CDB_IUHP();
	virtual ~CDB_IUHP();

public:
	void Add(T_IUHP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_IUHP_D& rData)
		{return m_iuhp.Lookup(1,rData);}
	int GetCount()
		{return m_iuhp.GetCount();}

protected:
	CMap<T_IUHP_K,T_IUHP_K,T_IUHP_D,T_IUHP_D&>m_iuhp;
};

#endif