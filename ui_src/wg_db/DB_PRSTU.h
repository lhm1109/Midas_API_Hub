#ifndef __DB_PRSTU_DB_H__
#define __DB_PRSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PRSTU
{
public:
	CDB_PRSTU()
	{
		m_prst.InitHashTable(HASHSIZEPRST);
	}
	virtual ~CDB_PRSTU(){};

public:
	void Add(T_UDRD_KEY Key,T_PRST_UDRD_D& rData)
		{m_prst.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_prst.RemoveKey(Key);}
	void DelAll()
		{m_prst.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PRST_UDRD_D& rData)
		{return m_prst.Lookup(Key,rData);}
	int GetCount()
		{return m_prst.GetCount();}
	POSITION GetStart()
		{return m_prst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PRST_UDRD_D& rData)
		{m_prst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PRST_UDRD_D,T_PRST_UDRD_D&>m_prst;
};

#endif