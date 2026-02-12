#ifndef __DB_DACT_DB_H__
#define __DB_DACT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DACT
{
public:
	CDB_DACT();
	virtual ~CDB_DACT();

public:
	void Add(T_DACT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DACT_D& rData)
	{
		return m_dact.Lookup(1, rData);
	}
	int GetCount()
	{
		return m_dact.GetCount();
	}

protected:
	CMap<T_DACT_K, T_DACT_K, T_DACT_D, T_DACT_D&>m_dact;
};

#endif