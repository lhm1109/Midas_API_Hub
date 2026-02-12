#ifndef __DB_WLCIU_DB_H__
#define __DB_WLCIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WLCIU
{
public:
	CDB_WLCIU()
	{
		m_wlci.InitHashTable(HASHSIZEWLCI);
	}
	virtual ~CDB_WLCIU(){};

public:
	void Add(T_UDRD_KEY Key,T_WLCI_UDRD_D& rData)
		{m_wlci.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wlci.RemoveKey(Key);}
	void DelAll()
		{m_wlci.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WLCI_UDRD_D& rData)
		{return m_wlci.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WLCI_UDRD_D,T_WLCI_UDRD_D&>m_wlci;
};

#endif