#ifndef __DB_IETHU_DB_H__
#define __DB_IETHU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IETHU
{
public:
	CDB_IETHU()
	{
		m_ieth.InitHashTable(HASHSIZEIETH);
	}
	virtual ~CDB_IETHU(){};

public:
	void Add(T_UDRD_KEY Key,T_IETH_UDRD_D& rData)
		{m_ieth.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ieth.RemoveKey(Key);}
	void DelAll()
		{m_ieth.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IETH_UDRD_D& rData)
		{return m_ieth.Lookup(Key,rData);}
	int GetCount()
		{return m_ieth.GetCount();}
	POSITION GetStart()
		{return m_ieth.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IETH_UDRD_D& rData)
		{m_ieth.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IETH_UDRD_D,T_IETH_UDRD_D&>m_ieth;
};

#endif