#ifndef __DB_JDJO_DB_H__
#define __DB_JDJO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDJO
{
public:
	CDB_JDJO();
	virtual ~CDB_JDJO();

public:
	void Add(T_JDJO_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDJO_D& rData)
		{return m_JDJO.Lookup(1,rData);}
	int GetCount()
		{return m_JDJO.GetCount();}

protected:
	CMap<T_JDJO_K,T_JDJO_K,T_JDJO_D,T_JDJO_D&> m_JDJO;
};

#endif