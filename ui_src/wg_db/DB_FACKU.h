#ifndef __DB_FACKU_DB_H__
#define __DB_FACKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FACKU
{
public:
	CDB_FACKU()
	{
		m_fack.InitHashTable(HASHSIZEFACK);
	}
	virtual ~CDB_FACKU(){};
	
public:
	void Add(T_UDRD_KEY Key,T_FACK_UDRD_D& rData)
	{m_fack.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_fack.RemoveKey(Key);}
	void DelAll()
	{m_fack.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FACK_UDRD_D& rData)
	{return m_fack.Lookup(Key,rData);}
	int GetCount()
	{return m_fack.GetCount();}
	POSITION GetStart()
	{return m_fack.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FACK_UDRD_D& rData)
	{m_fack.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_FACK_UDRD_D,T_FACK_UDRD_D&>m_fack;
};

#endif