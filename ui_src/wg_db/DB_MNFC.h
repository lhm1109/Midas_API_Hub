#ifndef __DB_MNFC_DB_H__
#define __DB_MNFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MNFC
{
public:
	CDB_MNFC();
	virtual ~CDB_MNFC();

public:
	void Add(T_MNFC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MNFC_D& rData)
	{
		return m_mnfc.Lookup(1, rData);
	}
	int GetCount()
	{
		return m_mnfc.GetCount();
	}

protected:
	CMap<T_MNFC_K, T_MNFC_K, T_MNFC_D, T_MNFC_D&>m_mnfc;
};

#endif
