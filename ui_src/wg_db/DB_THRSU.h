#ifndef __DB_THRSU_DB_H__
#define __DB_THRSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THRSU
{
public:
	CDB_THRSU()
	{
		m_thrs.InitHashTable(HASHSIZETHRS);
	}
	virtual ~CDB_THRSU(){};

public:
	void Add(T_UDRD_KEY Key,T_THRS_UDRD_D& rData)
		{m_thrs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thrs.RemoveKey(Key);}
	void DelAll()
		{m_thrs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THRS_UDRD_D& rData)
		{return m_thrs.Lookup(Key,rData);}
	int GetCount()
		{return m_thrs.GetCount();}
	POSITION GetStart()
		{return m_thrs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THRS_UDRD_D& rData)
		{m_thrs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THRS_UDRD_D,T_THRS_UDRD_D&>m_thrs;
};

#endif