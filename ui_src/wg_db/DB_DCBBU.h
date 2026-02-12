#ifndef __DB_DCBBU_DB_H__
#define __DB_DCBBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DCBBU
{
public:
	CDB_DCBBU()
	{
		m_dcbb.InitHashTable(HASHSIZEDCBB);
	}
	virtual ~CDB_DCBBU(){};

public:
	void Add(T_UDRD_KEY Key,T_DCBB_UDRD_D& rData)
		{m_dcbb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dcbb.RemoveKey(Key);}
	void DelAll()
		{m_dcbb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DCBB_UDRD_D& rData)
		{return m_dcbb.Lookup(Key,rData);}
	int GetCount()
		{return m_dcbb.GetCount();}
	POSITION GetStart()
		{return m_dcbb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DCBB_UDRD_D& rData)
		{m_dcbb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DCBB_UDRD_D,T_DCBB_UDRD_D&>m_dcbb;
};

#endif