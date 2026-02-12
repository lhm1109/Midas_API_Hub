#ifndef __DB_DOHVU_DB_H__
#define __DB_DOHVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DOHVU
{
public:
	CDB_DOHVU()
	{
		m_dohv.InitHashTable(HASHSIZEDOHV);
	}
	virtual ~CDB_DOHVU(){};

public:
	void Add(T_UDRD_KEY Key,T_DOHV_UDRD_D& rData)
		{m_dohv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dohv.RemoveKey(Key);}
	void DelAll()
		{m_dohv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DOHV_UDRD_D& rData)
		{return m_dohv.Lookup(Key,rData);}
	int GetCount()
		{return m_dohv.GetCount();}
	POSITION GetStart()
		{return m_dohv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DOHV_UDRD_D& rData)
		{m_dohv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DOHV_UDRD_D,T_DOHV_UDRD_D&>m_dohv;
};

#endif