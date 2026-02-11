#ifndef __DB_DSFC_DB_H__
#define __DB_DSFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DSFC
{
public:
	CDB_DSFC();
	virtual ~CDB_DSFC();

public:
	void Add(T_DSFC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DSFC_D& rData)
	{
		return m_dsfc.Lookup(1, rData);
	}
	int GetCount()
	{
		return m_dsfc.GetCount();
	}

protected:
	CMap<T_DSFC_K, T_DSFC_K, T_DSFC_D, T_DSFC_D&>m_dsfc;
};

#endif