#ifndef __DB_MPCCU_DB_H__
#define __DB_MPCCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPCCU
{
public:
	CDB_MPCCU()
	{
		m_mpcc.InitHashTable(HASHSIZEMPCC);
	}
	virtual ~CDB_MPCCU(){};

public:
	void Add(T_UDRD_KEY Key,T_MPCC_UDRD_D& rData)
		{m_mpcc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mpcc.RemoveKey(Key);}
	void DelAll()
		{m_mpcc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MPCC_UDRD_D& rData)
		{return m_mpcc.Lookup(Key,rData);}
	int GetCount()
		{return m_mpcc.GetCount();}
	POSITION GetStart()
		{return m_mpcc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MPCC_UDRD_D& rData)
		{m_mpcc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MPCC_UDRD_D,T_MPCC_UDRD_D&>m_mpcc;
};

#endif
