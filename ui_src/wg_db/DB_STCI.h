#ifndef __DB_STCI_DB_H__
#define __DB_STCI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STCI
{
public:
	CDB_STCI();
	virtual ~CDB_STCI();

public:
	void Add(T_STCI_D& rData);
	BOOL Del();

public:
	BOOL Get(T_STCI_D& rData)
		{return m_stci.Lookup(1,rData);}
	int GetCount()
		{return m_stci.GetCount();}

protected:
	CMap<T_STCI_K,T_STCI_K,T_STCI_D,T_STCI_D&>m_stci;
};

#endif