#ifndef __DB_TDCSU_DB_H__
#define __DB_TDCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDCSU
{
public:
	CDB_TDCSU()
	{
		m_tdcs.InitHashTable(HASHSIZETDCS);
	}
	virtual ~CDB_TDCSU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDCS_UDRD_D& rData)
	{m_tdcs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_tdcs.RemoveKey(Key);}
	void DelAll()
	{m_tdcs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDCS_UDRD_D& rData)
	{return m_tdcs.Lookup(Key,rData);}
	int GetCount()
	{return m_tdcs.GetCount();}
	POSITION GetStart()
	{return m_tdcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDCS_UDRD_D& rData)
	{m_tdcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDCS_UDRD_D,T_TDCS_UDRD_D&>m_tdcs;
};

#endif