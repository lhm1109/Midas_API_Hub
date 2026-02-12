#ifndef __DB_SMLCU_DB_H__
#define __DB_SMLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SMLCU
{
public:
	CDB_SMLCU()
	{
		m_smlc.InitHashTable(HASHSIZESMLC);
	}
	virtual ~CDB_SMLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SMLC_UDRD_D& rData)
		{m_smlc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_smlc.RemoveKey(Key);}
	void DelAll()
		{m_smlc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SMLC_UDRD_D& rData)
		{return m_smlc.Lookup(Key,rData);}
	int GetCount()
		{return m_smlc.GetCount();}
	POSITION GetStart()
		{return m_smlc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SMLC_UDRD_D& rData)
		{m_smlc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SMLC_UDRD_D,T_SMLC_UDRD_D&>m_smlc;
};

#endif