#ifndef __DB_STCT_DB_H__
#define __DB_STCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STCT
{
public:
	CDB_STCT();
	virtual ~CDB_STCT();

public:
	void Add(T_STCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_STCT_D& rData)
		{return m_stct.Lookup(1,rData);}
	int GetCount()
		{return m_stct.GetCount();}

protected:
	CMap<T_STCT_K,T_STCT_K,T_STCT_D,T_STCT_D&>m_stct;
};

#endif