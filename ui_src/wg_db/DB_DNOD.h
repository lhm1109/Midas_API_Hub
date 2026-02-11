#ifndef __DB_DNOD_DB_H__
#define __DB_DNOD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NODE;

class CDB_DNOD
{
public:
	CDB_DNOD();
	virtual ~CDB_DNOD();

public:
	void Add(T_DNOD_D& rData,CDB_NODE* pElem);
	BOOL Del(CDB_NODE* pElem);

public:
	BOOL Get(T_DNOD_D& rData)
		{return m_dnod.Lookup(1,rData);}
	int GetCount()
		{return m_dnod.GetCount();}

protected:
	CMap<T_DNOD_K,T_DNOD_K,T_DNOD_D,T_DNOD_D&>m_dnod;
};

#endif