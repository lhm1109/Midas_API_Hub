#ifndef __DB_ACOP_DB_H__
#define __DB_ACOP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ACOP
{
public:
	CDB_ACOP();
	virtual ~CDB_ACOP();

public:
	void Add(T_ACOP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_ACOP_D& rData)
		{return m_acop.Lookup(1,rData);}
	int GetCount()
		{return m_acop.GetCount();}

protected:
	CMap<T_ACOP_K,T_ACOP_K,T_ACOP_D,T_ACOP_D&>m_acop;
};

#endif