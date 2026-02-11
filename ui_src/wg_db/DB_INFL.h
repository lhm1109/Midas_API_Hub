#ifndef __DB_INFL_DB_H__
#define __DB_INFL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_INFL
{
public:
	CDB_INFL();
	virtual ~CDB_INFL();

public:
	void Add(T_INFL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_INFL_D& rData)
		{return m_infl.Lookup(1,rData);}
	int GetCount()
		{return m_infl.GetCount();}

protected:
	CMap<T_INFL_K,T_INFL_K,T_INFL_D,T_INFL_D&>m_infl;
};

#endif