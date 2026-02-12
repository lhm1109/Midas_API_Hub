#ifndef __DB_SDSWU_DB_H__
#define __DB_SDSWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDSWU
{
public:
	CDB_SDSWU()
	{
		m_sdsw.InitHashTable(HASHSIZESDSW);
	}
	virtual ~CDB_SDSWU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDSW_UDRD_D& rData)
		{m_sdsw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdsw.RemoveKey(Key);}
	void DelAll()
		{m_sdsw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDSW_UDRD_D& rData)
		{return m_sdsw.Lookup(Key,rData);}
	int GetCount()
		{return m_sdsw.GetCount();}
	POSITION GetStart()
		{return m_sdsw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDSW_UDRD_D& rData)
		{m_sdsw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDSW_UDRD_D,T_SDSW_UDRD_D&>m_sdsw;
};

#endif