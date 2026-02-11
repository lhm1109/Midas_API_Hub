#ifndef __DB_ARIBU_DB_H__
#define __DB_ARIBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ARIBU
{
public:
	CDB_ARIBU()
	{
		m_arib.InitHashTable(HASHSIZEARIB);
	}
	virtual ~CDB_ARIBU(){};

public:
	void Add(T_UDRD_KEY Key,T_ARIB_UDRD_D& rData)
		{m_arib.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_arib.RemoveKey(Key);}
	void DelAll()
		{m_arib.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ARIB_UDRD_D& rData)
		{return m_arib.Lookup(Key,rData);}
	int GetCount()
		{return m_arib.GetCount();}
	POSITION GetStart()
		{return m_arib.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ARIB_UDRD_D& rData)
		{m_arib.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ARIB_UDRD_D,T_ARIB_UDRD_D&>m_arib;
};

#endif