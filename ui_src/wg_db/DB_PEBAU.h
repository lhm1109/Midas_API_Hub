#ifndef __DB_PEBAU_DB_H__
#define __DB_PEBAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PEBAU
{
public:
	CDB_PEBAU()
	{
		m_peba.InitHashTable(HASHSIZEPEBA);
	}
	virtual ~CDB_PEBAU(){};

public:
	void Add(T_UDRD_KEY Key,T_PEBA_UDRD_D& rData)
		{m_peba.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_peba.RemoveKey(Key);}
	void DelAll()
		{m_peba.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PEBA_UDRD_D& rData)
		{return m_peba.Lookup(Key,rData);}
	int GetCount()
		{return m_peba.GetCount();}
	POSITION GetStart()
		{return m_peba.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PEBA_UDRD_D& rData)
		{m_peba.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PEBA_UDRD_D,T_PEBA_UDRD_D&>m_peba;
};

#endif