#ifndef __DB_MDULU_DB_H__
#define __DB_MDULU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MDULU
{
public:
	CDB_MDULU()
	{
		m_mdul.InitHashTable(HASHSIZEMDUL);
	}
	virtual ~CDB_MDULU(){};

public:
	void Add(T_UDRD_KEY Key,T_MDUL_UDRD_D& rData)
		{m_mdul.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mdul.RemoveKey(Key);}
	void DelAll()
		{m_mdul.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MDUL_UDRD_D& rData)
		{return m_mdul.Lookup(Key,rData);}
	int GetCount()
		{return m_mdul.GetCount();}
	POSITION GetStart()
		{return m_mdul.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MDUL_UDRD_D& rData)
		{m_mdul.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MDUL_UDRD_D,T_MDUL_UDRD_D&>m_mdul;
};

#endif
