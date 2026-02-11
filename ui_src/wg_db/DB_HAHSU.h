#ifndef __DB_HAHSU_DB_H__
#define __DB_HAHSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HAHSU
{
public:
	CDB_HAHSU()
	{
		m_hahs.InitHashTable(HASHSIZEHAHS);
	}
	virtual ~CDB_HAHSU(){};

public:
	void Add(T_UDRD_KEY Key,T_HAHS_UDRD_D& rData)
		{m_hahs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hahs.RemoveKey(Key);}
	void DelAll()
		{m_hahs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HAHS_UDRD_D& rData)
		{return m_hahs.Lookup(Key,rData);}
	int GetCount()
		{return m_hahs.GetCount();}
	POSITION GetStart()
		{return m_hahs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HAHS_UDRD_D& rData)
		{m_hahs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HAHS_UDRD_D,T_HAHS_UDRD_D&>m_hahs;
};

#endif
