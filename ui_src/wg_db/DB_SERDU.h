#ifndef __DB_SERDU_DB_H__
#define __DB_SERDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SERDU
{
public:
	CDB_SERDU()
	{
		m_SERD.InitHashTable(HASHSIZESERD);
	}
	virtual ~CDB_SERDU(){};

public:
	void Add(T_UDRD_KEY Key,T_SERD_UDRD_D& rData)
	{m_SERD.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_SERD.RemoveKey(Key);}
	void DelAll()
	{m_SERD.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SERD_UDRD_D& rData)
	{return m_SERD.Lookup(Key,rData);}
	int GetCount()
	{return m_SERD.GetCount();}
	POSITION GetStart()
	{return m_SERD.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SERD_UDRD_D& rData)
	{m_SERD.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SERD_UDRD_D,T_SERD_UDRD_D&>m_SERD;
};

#endif
