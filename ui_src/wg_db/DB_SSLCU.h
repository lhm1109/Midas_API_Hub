#ifndef __DB_SSLCU_DB_H__
#define __DB_SSLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SSLCU
{
public:
	CDB_SSLCU()
	{
		m_sslc.InitHashTable(HASHSIZESSLC);
	}
	virtual ~CDB_SSLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SSLC_UDRD_D& rData)
		{m_sslc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sslc.RemoveKey(Key);}
	void DelAll()
		{m_sslc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SSLC_UDRD_D& rData)
		{return m_sslc.Lookup(Key,rData);}
	int GetCount()
		{return m_sslc.GetCount();}
	POSITION GetStart()
		{return m_sslc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SSLC_UDRD_D& rData)
		{m_sslc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SSLC_UDRD_D,T_SSLC_UDRD_D&>m_sslc;
};

#endif