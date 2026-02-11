#ifndef __DB_SEEWU_DB_H__
#define __DB_SEEWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SEEWU
{
public:
	CDB_SEEWU()
	{
		m_SEEW.InitHashTable(HASHSIZESEEW);
	}
	virtual ~CDB_SEEWU(){};

public:
	void Add(T_UDRD_KEY Key,T_SEEW_UDRD_D& rData)
	{m_SEEW.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_SEEW.RemoveKey(Key);}
	void DelAll()
	{m_SEEW.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SEEW_UDRD_D& rData)
	{return m_SEEW.Lookup(Key,rData);}
	int GetCount()
	{return m_SEEW.GetCount();}
	POSITION GetStart()
	{return m_SEEW.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SEEW_UDRD_D& rData)
	{m_SEEW.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SEEW_UDRD_D,T_SEEW_UDRD_D&>m_SEEW;
};

#endif
