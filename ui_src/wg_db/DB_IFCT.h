#ifndef __DB_IFCT_DB_H__
#define __DB_IFCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IFCT
{
public:
	CDB_IFCT();
	virtual ~CDB_IFCT();

public:
	void Add(T_IFCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_IFCT_D& rData)
		{return m_ifct.Lookup(1,rData);}
	int GetCount()
		{return m_ifct.GetCount();}

protected:
	CMap<T_IFCT_K,T_IFCT_K,T_IFCT_D,T_IFCT_D&>m_ifct;
};

#endif