#ifndef __DB_HHCT_DB_H__
#define __DB_HHCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HHCT
{
public:
	CDB_HHCT();
	virtual ~CDB_HHCT();

public:
	void Add(T_HHCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_HHCT_D& rData)
		{return m_hhct.Lookup(1,rData);}
	int GetCount()
		{return m_hhct.GetCount();}

protected:
	CMap<T_HHCT_K,T_HHCT_K,T_HHCT_D,T_HHCT_D&>m_hhct;
};

#endif