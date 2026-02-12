#ifndef __DB_SPHOU_DB_H__
#define __DB_SPHOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPHOU
{
public:
	CDB_SPHOU()
	{
		m_spho.InitHashTable(HASHSIZESPHO);
	}
	virtual ~CDB_SPHOU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPHO_UDRD_D& rData)
	{m_spho.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_spho.RemoveKey(Key);}
	void DelAll()
	{m_spho.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPHO_UDRD_D& rData)
	{return m_spho.Lookup(Key,rData);}
	int GetCount()
	{return m_spho.GetCount();}
	POSITION GetStart()
	{return m_spho.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPHO_UDRD_D& rData)
	{m_spho.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPHO_UDRD_D,T_SPHO_UDRD_D&>m_spho;
};

#endif