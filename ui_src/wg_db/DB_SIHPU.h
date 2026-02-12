#ifndef __DB_SIHPU_DB_H__
#define __DB_SIHPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIHPU
{
public:
	CDB_SIHPU()
	{
		m_sihp.InitHashTable(HASHSIZESIHP);
	}
	virtual ~CDB_SIHPU(){};

public:
	void Add(T_UDRD_KEY Key,T_SIHP_UDRD_D& rData)
	{m_sihp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_sihp.RemoveKey(Key);}
	void DelAll()
	{m_sihp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SIHP_UDRD_D& rData)
	{return m_sihp.Lookup(Key,rData);}
	int GetCount()
	{return m_sihp.GetCount();}
	POSITION GetStart()
	{return m_sihp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SIHP_UDRD_D& rData)
	{m_sihp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SIHP_UDRD_D,T_SIHP_UDRD_D&>m_sihp;
};

#endif