#ifndef __DB_RCBEU_DB_H__
#define __DB_RCBEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCBEU
{
public:
	CDB_RCBEU()
	{
		m_rcbe.InitHashTable(HASHSIZERCBE);
	}
	virtual ~CDB_RCBEU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCBE_UDRD_D& rData)
		{m_rcbe.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rcbe.RemoveKey(Key);}
	void DelAll()
		{m_rcbe.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCBE_UDRD_D& rData)
		{return m_rcbe.Lookup(Key,rData);}
	int GetCount()
		{return m_rcbe.GetCount();}
	POSITION GetStart()
		{return m_rcbe.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RCBE_UDRD_D& rData)
		{m_rcbe.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCBE_UDRD_D,T_RCBE_UDRD_D&>m_rcbe;
};

#endif