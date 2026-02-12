#ifndef __DB_STCIU_DB_H__
#define __DB_STCIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STCIU
{
public:
	CDB_STCIU()
	{
		m_stci.InitHashTable(HASHSIZESTCI);
	}
	virtual ~CDB_STCIU(){};

public:
	void Add(T_UDRD_KEY Key,T_STCI_UDRD_D& rData)
		{m_stci.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stci.RemoveKey(Key);}
	void DelAll()
		{m_stci.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STCI_UDRD_D& rData)
		{return m_stci.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STCI_UDRD_D,T_STCI_UDRD_D&>m_stci;
};

#endif