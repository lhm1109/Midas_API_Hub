#ifndef __DB_ISLY_DB_H__
#define __DB_ISLY_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_ISLY
{
public:
	CDB_ISLY();
	virtual ~CDB_ISLY();

public:
	void Add(T_ISLY_D& rData);
	BOOL Del();

public:
	BOOL Get(T_ISLY_D& rData)
	{
		return m_isly.Lookup(1, rData);
	}
	int GetCount()
	{
		return m_isly.GetCount();
	}

protected:
	CMap<T_ISLY_K, T_ISLY_K, T_ISLY_D, T_ISLY_D&>m_isly;
};

#endif
