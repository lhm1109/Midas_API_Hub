#ifndef __DB_SEME_DB_H__
#define __DB_SEME_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SEME
{
public:
	CDB_SEME();
	virtual ~CDB_SEME();

public:
	void Add(T_SEME_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SEME_D& rData)
		{return m_seme.Lookup(1,rData);}
	int GetCount()
		{return m_seme.GetCount();}

protected:
	CMap<T_SEME_K,T_SEME_K,T_SEME_D,T_SEME_D&>m_seme;
};

#endif