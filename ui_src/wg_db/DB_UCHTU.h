#ifndef __DB_UCHTU_DB_H__
#define __DB_UCHTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UCHTU
{
public:
	CDB_UCHTU()
	{
		m_ucht.InitHashTable(HASHSIZEUCHT);
	}
	virtual ~CDB_UCHTU(){};

public:
	void Add(T_UDRD_KEY Key,T_UCHT_UDRD_D& rData)
		{m_ucht.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ucht.RemoveKey(Key);}
	void DelAll()
		{m_ucht.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_UCHT_UDRD_D& rData)
		{return m_ucht.Lookup(Key,rData);}
	int GetCount()
		{return m_ucht.GetCount();}
	POSITION GetStart()
		{return m_ucht.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UCHT_UDRD_D& rData)
		{m_ucht.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_UCHT_UDRD_D,T_UCHT_UDRD_D&>m_ucht;
};

#endif