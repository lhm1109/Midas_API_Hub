#ifndef __DB_BPOP_DB_H__
#define __DB_BPOP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_BPOP
{
public:
	CDB_BPOP();
	virtual ~CDB_BPOP();

public:
	void Add(T_BPOP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_BPOP_D& rData)
		{return m_BPOP.Lookup(1,rData);}
	int GetCount()
		{return m_BPOP.GetCount();}

protected:
	CMap<T_BPOP_K,T_BPOP_K,T_BPOP_D,T_BPOP_D&> m_BPOP;
};

#endif