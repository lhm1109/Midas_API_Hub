#ifndef __DB_BCCT_DB_H__
#define __DB_BCCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BCCT
{
public:
	CDB_BCCT();
	virtual ~CDB_BCCT();

public:
	void Add(T_BCCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_BCCT_D& rData)
		{return m_bcct.Lookup(1,rData);}
	int GetCount()
		{return m_bcct.GetCount();}

protected:
	CMap<T_BCCT_K,T_BCCT_K,T_BCCT_D,T_BCCT_D&>m_bcct;
};

#endif