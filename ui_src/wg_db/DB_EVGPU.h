#ifndef __DB_EVGPU_DB_H__
#define __DB_EVGPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EVGPU
{
public:
	CDB_EVGPU()
	{
		m_evgp.InitHashTable(HASHSIZEEVGP);
	}
	virtual ~CDB_EVGPU(){};

public:
	void Add(T_UDRD_KEY Key,T_EVGP_UDRD_D& rData)
	{m_evgp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_evgp.RemoveKey(Key);}
	void DelAll()
	{m_evgp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EVGP_UDRD_D& rData)
	{return m_evgp.Lookup(Key,rData);}
	int GetCount()
	{return m_evgp.GetCount();}
	POSITION GetStart()
	{return m_evgp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EVGP_UDRD_D& rData)
	{m_evgp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EVGP_UDRD_D,T_EVGP_UDRD_D&>m_evgp;
};

#endif

