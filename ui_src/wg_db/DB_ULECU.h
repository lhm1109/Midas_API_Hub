#ifndef __DB_ULECU_DB_H__
#define __DB_ULECU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ULECU
{
public:
	CDB_ULECU()
	{
		m_ulec.InitHashTable(HASHSIZEULEC);
	}
	virtual ~CDB_ULECU(){};

public:
	void Add(T_UDRD_KEY Key,T_ULEC_UDRD_D& rData)
		{m_ulec.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ulec.RemoveKey(Key);}
	void DelAll()
		{m_ulec.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ULEC_UDRD_D& rData)
		{return m_ulec.Lookup(Key,rData);}
	int GetCount()
		{return m_ulec.GetCount();}
	POSITION GetStart()
		{return m_ulec.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ULEC_UDRD_D& rData)
		{m_ulec.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ULEC_UDRD_D,T_ULEC_UDRD_D&>m_ulec;
};

#endif