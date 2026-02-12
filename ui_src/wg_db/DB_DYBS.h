#ifndef __DB_DYBS_DB_H__
#define __DB_DYBS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DYBS
{
public:
	CDB_DYBS();
	virtual ~CDB_DYBS();

public:
	void Add(T_DYBS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DYBS_D& rData)
	{
		return m_dybs.Lookup(1, rData);
	}
	int GetCount()
	{
		return m_dybs.GetCount();
	}

protected:
	CMap<T_DYBS_K, T_DYBS_K, T_DYBS_D, T_DYBS_D&>m_dybs;
};

#endif