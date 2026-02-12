#ifndef __DB_THLCU_DB_H__
#define __DB_THLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THLCU
{
public:
	CDB_THLCU()
	{
		m_thlc.InitHashTable(HASHSIZETHLC);
	}
	virtual ~CDB_THLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_THLC_UDRD_D& rData)
		{m_thlc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thlc.RemoveKey(Key);}
	void DelAll()
		{m_thlc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THLC_UDRD_D& rData)
		{return m_thlc.Lookup(Key,rData);}
	int GetCount()
		{return m_thlc.GetCount();}
	POSITION GetStart()
		{return m_thlc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THLC_UDRD_D& rData)
		{m_thlc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THLC_UDRD_D,T_THLC_UDRD_D&>m_thlc;
};

#endif