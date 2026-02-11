#ifndef __DB_JDBD_DB_H__
#define __DB_JDBD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDBD
{
public:
	CDB_JDBD();
	virtual ~CDB_JDBD();

public:
	void Add(T_JDBD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDBD_D& rData)
		{return m_JDBD.Lookup(1,rData);}
	int GetCount()
		{return m_JDBD.GetCount();}

protected:
	CMap<T_JDBD_K,T_JDBD_K,T_JDBD_D,T_JDBD_D&> m_JDBD;
};

#endif