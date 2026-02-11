#ifndef __DB_WLCI_DB_H__
#define __DB_WLCI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WLCI
{
public:
	CDB_WLCI();
	virtual ~CDB_WLCI();

public:
	void Add(T_WLCI_D& rData);
	BOOL Del();

public:
	BOOL Get(T_WLCI_D& rData)
		{return m_wlci.Lookup(1,rData);}
	int GetCount()
		{return m_wlci.GetCount();}

protected:
	CMap<T_WLCI_K,T_WLCI_K,T_WLCI_D,T_WLCI_D&>m_wlci;
};

#endif