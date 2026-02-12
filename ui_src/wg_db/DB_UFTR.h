#ifndef __DB_UFTR_DB_H__
#define __DB_UFTR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UFTR
{
public:
	CDB_UFTR();
	virtual ~CDB_UFTR();

public:
	void Add(T_UFTR_D& rData);
	BOOL Del();

public:
	BOOL Get(T_UFTR_D& rData)
		{return m_uftr.Lookup(1,rData);}
	int GetCount()
		{return m_uftr.GetCount();}

protected:
	CMap<T_UFTR_K,T_UFTR_K,T_UFTR_D,T_UFTR_D&>m_uftr;
};

#endif