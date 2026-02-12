#ifndef __DB_PORIU_DB_H__
#define __DB_PORIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PORIU
{
public:
	CDB_PORIU()
	{
		m_pori.InitHashTable(HASHSIZEPORI);
	}
	virtual ~CDB_PORIU(){};

public:
	void Add(T_UDRD_KEY Key,T_PORI_UDRD_D& rData)
		{m_pori.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pori.RemoveKey(Key);}
	void DelAll()
		{m_pori.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PORI_UDRD_D& rData)
		{return m_pori.Lookup(Key,rData);}
	int GetCount()
		{return m_pori.GetCount();}
	POSITION GetStart()
		{return m_pori.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PORI_UDRD_D& rData)
		{m_pori.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PORI_UDRD_D,T_PORI_UDRD_D&>m_pori;
};

#endif