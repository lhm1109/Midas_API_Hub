#ifndef __DB_IFBZ_DB_H__
#define __DB_IFBZ_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IFBZ
{
public:
	CDB_IFBZ();
	virtual ~CDB_IFBZ();

public:
	void Add(T_IFBZ_D& rData);
	BOOL Del();

public:
	BOOL Get(T_IFBZ_D& rData)
	{
		return m_ifbz.Lookup(1, rData);
	}
	int GetCount()
	{
		return m_ifbz.GetCount();
	}

protected:
	CMap<T_IFBZ_K, T_IFBZ_K, T_IFBZ_D, T_IFBZ_D&> m_ifbz;
};

#endif