#ifndef __DB_REXCU_DB_H__
#define __DB_REXCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_REXCU
{
public:
	CDB_REXCU()
	{
		m_rexc.InitHashTable(HASHSIZEREXC);
	}
	virtual ~CDB_REXCU(){};

public:
	void Add(T_UDRD_KEY Key,T_REXC_UDRD_D& rData)
		{m_rexc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rexc.RemoveKey(Key);}
	void DelAll()
		{m_rexc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_REXC_UDRD_D& rData)
		{return m_rexc.Lookup(Key,rData);}
	int GetCount()
		{return m_rexc.GetCount();}
	POSITION GetStart()
		{return m_rexc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_REXC_UDRD_D& rData)
		{m_rexc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_REXC_UDRD_D,T_REXC_UDRD_D&>m_rexc;
};

#endif