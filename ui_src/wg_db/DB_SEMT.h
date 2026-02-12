#ifndef __DB_SEMT_DB_H__
#define __DB_SEMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SEMT
{
public:
	CDB_SEMT();
	virtual ~CDB_SEMT();

public:
	void Add(T_SEMT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SEMT_D& rData)
		{return m_semt.Lookup(1,rData);}
	int GetCount()
		{return m_semt.GetCount();}

protected:
	CMap<T_SEMT_K,T_SEMT_K,T_SEMT_D,T_SEMT_D&>m_semt;
};

#endif