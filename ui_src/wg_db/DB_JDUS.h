#ifndef __DB_JDUS_DB_H__
#define __DB_JDUS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDUS
{
public:
	CDB_JDUS();
	virtual ~CDB_JDUS();

public:
	void Add(T_JDUS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDUS_D& rData)
		{return m_JDUS.Lookup(1,rData);}
	int GetCount()
		{return m_JDUS.GetCount();}

protected:
	CMap<T_JDUS_K,T_JDUS_K,T_JDUS_D,T_JDUS_D&> m_JDUS;
};

#endif