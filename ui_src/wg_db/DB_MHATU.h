#ifndef __DB_MHATU_DB_H__
#define __DB_MHATU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MHATU
{
public:
	CDB_MHATU()
	{
		m_mhat.InitHashTable(HASHSIZEMHAT);
	}
	virtual ~CDB_MHATU(){};

public:
	void Add(T_UDRD_KEY Key,T_MHAT_UDRD_D& rData)
		{m_mhat.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mhat.RemoveKey(Key);}
	void DelAll()
		{m_mhat.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MHAT_UDRD_D& rData)
		{return m_mhat.Lookup(Key,rData);}
	int GetCount()
		{return m_mhat.GetCount();}
	POSITION GetStart()
		{return m_mhat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MHAT_UDRD_D& rData)
		{m_mhat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MHAT_UDRD_D,T_MHAT_UDRD_D&>m_mhat;
};

#endif