#ifndef __DB_DPAAU_DB_H__
#define __DB_DPAAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DPAAU
{
public:
	CDB_DPAAU()
	{
		m_dpaa.InitHashTable(HASHSIZEDPAA);
	}
	virtual ~CDB_DPAAU(){};

public:
	void Add(T_UDRD_KEY Key,T_DPAA_UDRD_D& rData)
		{m_dpaa.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dpaa.RemoveKey(Key);}
	void DelAll()
		{m_dpaa.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DPAA_UDRD_D& rData)
		{return m_dpaa.Lookup(Key,rData);}
	int GetCount()
		{return m_dpaa.GetCount();}
	POSITION GetStart()
		{return m_dpaa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DPAA_UDRD_D& rData)
		{m_dpaa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DPAA_UDRD_D,T_DPAA_UDRD_D&>m_dpaa;
};

#endif