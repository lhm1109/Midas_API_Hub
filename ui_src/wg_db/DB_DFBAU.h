#ifndef __DB_DFBAU_DB_H__
#define __DB_DFBAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DFBAU
{
public:
	CDB_DFBAU()
	{
		m_dfba.InitHashTable(HASHSIZEDFBA);
	}
	virtual ~CDB_DFBAU(){};

public:
	void Add(T_UDRD_KEY Key,T_DFBA_UDRD_D& rData)
		{m_dfba.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dfba.RemoveKey(Key);}
	void DelAll()
		{m_dfba.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DFBA_UDRD_D& rData)
		{return m_dfba.Lookup(Key,rData);}
	int GetCount()
		{return m_dfba.GetCount();}
	POSITION GetStart()
		{return m_dfba.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DFBA_UDRD_D& rData)
		{m_dfba.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DFBA_UDRD_D,T_DFBA_UDRD_D&>m_dfba;
};

#endif