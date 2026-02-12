#ifndef __DB_POLDU_DB_H__
#define __DB_POLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POLDU
{
public:
	CDB_POLDU()
	{
		m_pold.InitHashTable(HASHSIZEPOLD);
	}
	virtual ~CDB_POLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_POLD_UDRD_D& rData)
		{m_pold.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pold.RemoveKey(Key);}
	void DelAll()
		{m_pold.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POLD_UDRD_D& rData)
		{return m_pold.Lookup(Key,rData);}
	int GetCount()
		{return m_pold.GetCount();}
	POSITION GetStart()
		{return m_pold.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_POLD_UDRD_D& rData)
		{m_pold.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POLD_UDRD_D,T_POLD_UDRD_D&>m_pold;
};

#endif