#ifndef __DB_JDUB_DB_H__
#define __DB_JDUB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDUB
{
public:
	CDB_JDUB();
	virtual ~CDB_JDUB();

public:
	void Add(T_JDUB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDUB_D& rData)
		{return m_JDUB.Lookup(1,rData);}
	int GetCount()
		{return m_JDUB.GetCount();}

protected:
	CMap<T_JDUB_K,T_JDUB_K,T_JDUB_D,T_JDUB_D&> m_JDUB;
};

#endif