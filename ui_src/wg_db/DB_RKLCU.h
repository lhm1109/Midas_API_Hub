#ifndef __DB_RKLCU_DB_H__
#define __DB_RKLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RKLCU
{
public:
	CDB_RKLCU()
	{
		m_rklc.InitHashTable(HASHSIZERKLC);
	}
	virtual ~CDB_RKLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_RKLC_UDRD_D& rData)
		{m_rklc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rklc.RemoveKey(Key);}
	void DelAll()
		{m_rklc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RKLC_UDRD_D& rData)
		{return m_rklc.Lookup(Key,rData);}
	int GetCount()
		{return m_rklc.GetCount();}
	POSITION GetStart()
		{return m_rklc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RKLC_UDRD_D& rData)
		{m_rklc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RKLC_UDRD_D,T_RKLC_UDRD_D&>m_rklc;
};

#endif