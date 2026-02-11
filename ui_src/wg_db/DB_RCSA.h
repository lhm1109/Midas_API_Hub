#ifndef __DB_RCSA_DB_H__
#define __DB_RCSA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCSA
{
public:
	CDB_RCSA();
	virtual ~CDB_RCSA();

public:
	void Add(T_RCSA_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCSA_D& rData)
		{return m_rcsa.Lookup(1,rData);}
	int GetCount()
		{return m_rcsa.GetCount();}

protected:
	CMap<T_RCSA_K,T_RCSA_K,T_RCSA_D,T_RCSA_D&>m_rcsa;
};

#endif