#ifndef __DB_PREBU_DB_H__
#define __DB_PREBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PREBU
{
public:
	CDB_PREBU()
	{
		m_preb.InitHashTable(HASHSIZEPREB);
	}
	virtual ~CDB_PREBU(){};

public:
	void Add(T_UDRD_KEY Key,T_PREB_UDRD_D& rData)
		{m_preb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_preb.RemoveKey(Key);}
	void DelAll()
		{m_preb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PREB_UDRD_D& rData)
		{return m_preb.Lookup(Key,rData);}
	int GetCount()
		{return m_preb.GetCount();}
	POSITION GetStart()
		{return m_preb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PREB_UDRD_D& rData)
		{m_preb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PREB_UDRD_D,T_PREB_UDRD_D&>m_preb;
};

#endif