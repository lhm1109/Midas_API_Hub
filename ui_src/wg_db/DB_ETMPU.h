#ifndef __DB_ETMPU_DB_H__
#define __DB_ETMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ETMPU
{
public:
	CDB_ETMPU()
	{
		m_etmp.InitHashTable(HASHSIZEETMP);
	}
	virtual ~CDB_ETMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_ETMP_UDRD_D& rData)
		{m_etmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_etmp.RemoveKey(Key);}
	void DelAll()
		{m_etmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ETMP_UDRD_D& rData)
		{return m_etmp.Lookup(Key,rData);}
	int GetCount()
		{return m_etmp.GetCount();}
	POSITION GetStart()
		{return m_etmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ETMP_UDRD_D& rData)
		{m_etmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ETMP_UDRD_D,T_ETMP_UDRD_D&>m_etmp;
};

#endif