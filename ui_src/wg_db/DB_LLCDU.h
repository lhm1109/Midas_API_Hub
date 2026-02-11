#ifndef __DB_LLCDU_DB_H__
#define __DB_LLCDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_LLCDU
{
public:
	CDB_LLCDU()
	{
		m_LLCD.InitHashTable(HASHSIZELLCD);
	}
	virtual ~CDB_LLCDU(){};

public:
	void Add(T_UDRD_KEY Key,T_LLCD_UDRD_D& rData)
		{m_LLCD.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_LLCD.RemoveKey(Key);}
	void DelAll()
		{m_LLCD.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLCD_UDRD_D& rData)
		{return m_LLCD.Lookup(Key,rData);}
	int GetCount()
		{return m_LLCD.GetCount();}
	POSITION GetStart()
		{return m_LLCD.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLCD_UDRD_D& rData)
		{m_LLCD.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLCD_UDRD_D,T_LLCD_UDRD_D&>m_LLCD;
};

#endif
