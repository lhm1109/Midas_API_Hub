#ifndef __DB_CLDRU_DB_H__
#define __DB_CLDRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CLDRU
{
public:
	CDB_CLDRU()
	{
		m_cldr.InitHashTable(HASHSIZECLDR);
	}
	virtual ~CDB_CLDRU(){};

public:
	void Add(T_UDRD_KEY Key,T_CLDR_UDRD_D& rData)
		{m_cldr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cldr.RemoveKey(Key);}
	void DelAll()
		{m_cldr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CLDR_UDRD_D& rData)
		{return m_cldr.Lookup(Key,rData);}
	int GetCount()
		{return m_cldr.GetCount();}
	POSITION GetStart()
		{return m_cldr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CLDR_UDRD_D& rData)
		{m_cldr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CLDR_UDRD_D,T_CLDR_UDRD_D&>m_cldr;
};

#endif