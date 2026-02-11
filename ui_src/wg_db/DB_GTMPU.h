#ifndef __DB_GTMPU_DB_H__
#define __DB_GTMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GTMPU
{
public:
	CDB_GTMPU()
	{
		m_gtmp.InitHashTable(HASHSIZEGTMP);
	}
	virtual ~CDB_GTMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_GTMP_UDRD_D& rData)
		{m_gtmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gtmp.RemoveKey(Key);}
	void DelAll()
		{m_gtmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GTMP_UDRD_D& rData)
		{return m_gtmp.Lookup(Key,rData);}
	int GetCount()
		{return m_gtmp.GetCount();}
	POSITION GetStart()
		{return m_gtmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GTMP_UDRD_D& rData)
		{m_gtmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GTMP_UDRD_D,T_GTMP_UDRD_D&>m_gtmp;
};

#endif