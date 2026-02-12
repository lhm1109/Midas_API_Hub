#ifndef __DB_PILEU_DB_H__
#define __DB_PILEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PILEU
{
public:
	CDB_PILEU()
	{
		m_pile.InitHashTable(HASHSIZEPILE);
	}
	virtual ~CDB_PILEU(){};

public:
	void Add(T_UDRD_KEY Key,T_PILE_UDRD_D& rData)
		{m_pile.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pile.RemoveKey(Key);}
	void DelAll()
		{m_pile.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PILE_UDRD_D& rData)
		{return m_pile.Lookup(Key,rData);}
	int GetCount()
		{return m_pile.GetCount();}
	POSITION GetStart()
		{return m_pile.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PILE_UDRD_D& rData)
		{m_pile.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PILE_UDRD_D,T_PILE_UDRD_D&>m_pile;
};

#endif