#ifndef __DB_BMLDU_DB_H__
#define __DB_BMLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BMLDU
{
public:
	CDB_BMLDU()
	{
		m_bmld.InitHashTable(HASHSIZEBMLD);
	}
	virtual ~CDB_BMLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_BMLD_UDRD_D& rData)
		{m_bmld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bmld.RemoveKey(Key);}
	void DelAll()
		{m_bmld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BMLD_UDRD_D& rData)
		{return m_bmld.Lookup(Key,rData);}
	int GetCount()
		{return m_bmld.GetCount();}
	POSITION GetStart()
		{return m_bmld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BMLD_UDRD_D& rData)
		{m_bmld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BMLD_UDRD_D,T_BMLD_UDRD_D&>m_bmld;
};

#endif