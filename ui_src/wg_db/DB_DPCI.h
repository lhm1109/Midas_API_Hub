#pragma once

//#include <afxtempl.h>
//#include "DB_ST_DT.h"

class CDB_DPCI
{
public:
	CDB_DPCI();
	virtual ~CDB_DPCI();

public:
	void Add(T_DPCI_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DPCI_D& rData)
		{return m_mapDpci.Lookup(1,rData);}
	int GetCount()
		{return m_mapDpci.GetCount();}

protected:
	CMap<T_ISGB_K,T_ISGB_K,T_DPCI_D,T_DPCI_D&>m_mapDpci;
};

