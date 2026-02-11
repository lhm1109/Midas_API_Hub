#ifndef __DB_BCVTU_DB_H__
#define __DB_BCVTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BCVTU
{
public:
	CDB_BCVTU()
	{
		m_bcvt.InitHashTable(HASHSIZEBCVT);
	}
	virtual ~CDB_BCVTU(){};

public:
	void Add(T_UDRD_KEY Key,T_BCVT_UDRD_D& rData)
		{m_bcvt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bcvt.RemoveKey(Key);}
	void DelAll()
		{m_bcvt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BCVT_UDRD_D& rData)
		{return m_bcvt.Lookup(Key,rData);}
	int GetCount()
		{return m_bcvt.GetCount();}
	POSITION GetStart()
		{return m_bcvt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BCVT_UDRD_D& rData)
		{m_bcvt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BCVT_UDRD_D,T_BCVT_UDRD_D&>m_bcvt;
};

#endif