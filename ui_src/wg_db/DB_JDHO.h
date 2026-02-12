#ifndef __DB_JDHO_DB_H__
#define __DB_JDHO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDHO
{
public:
	CDB_JDHO();
	virtual ~CDB_JDHO();

public:
	void Add(T_JDHO_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDHO_D& rData)
		{return m_JDHO.Lookup(1,rData);}
	int GetCount()
		{return m_JDHO.GetCount();}

protected:
	CMap<T_JDHO_K,T_JDHO_K,T_JDHO_D,T_JDHO_D&> m_JDHO;
};

#endif