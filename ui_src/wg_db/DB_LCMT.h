#ifndef __DB_LCMT_DB_H__
#define __DB_LCMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LCMT
{
public:
	CDB_LCMT();
	virtual ~CDB_LCMT();

public:
	void Add(T_LCMT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_LCMT_D& rData)
		{return m_lcmt.Lookup(1,rData);}
	int GetCount()
		{return m_lcmt.GetCount();}

protected:
	CMap<T_LCMT_K,T_LCMT_K,T_LCMT_D,T_LCMT_D&>m_lcmt;
};

#endif