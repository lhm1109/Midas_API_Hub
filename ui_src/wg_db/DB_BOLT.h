#ifndef __DB_BOLT_DB_H__
#define __DB_BOLT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BOLT
{
public:
	CDB_BOLT();
	virtual ~CDB_BOLT();
	
public:
	void Add(T_BOLT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_BOLT_D& rData)
		{return m_bolt.Lookup(1,rData);}
	int GetCount()
		{return m_bolt.GetCount();}

protected:
	CMap<T_BOLT_K,T_BOLT_K,T_BOLT_D,T_BOLT_D&>m_bolt;
};

#endif