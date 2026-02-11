#ifndef __DB_THGC_DB_H__
#define __DB_THGC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THGC
{
public:
	CDB_THGC();
	virtual ~CDB_THGC();
	
public:
	void Add(T_THGC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_THGC_D& rData)
		{return m_thgc.Lookup(1,rData);}
	int GetCount()
		{return m_thgc.GetCount();}

protected:
	CMap<T_THGC_K,T_THGC_K,T_THGC_D,T_THGC_D&>m_thgc;
};

#endif