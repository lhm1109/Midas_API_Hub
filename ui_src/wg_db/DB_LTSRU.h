#ifndef __DB_LTSRU_DB_H__
#define __DB_LTSRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LTSRU
{
public:
	CDB_LTSRU()
	{
		m_ltsr.InitHashTable(HASHSIZELTSR);
	}
	virtual ~CDB_LTSRU(){};

public:
	void Add(T_UDRD_KEY Key,T_LTSR_UDRD_D& rData)
		{m_ltsr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ltsr.RemoveKey(Key);}
	void DelAll()
		{m_ltsr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LTSR_UDRD_D& rData)
		{return m_ltsr.Lookup(Key,rData);}
	int GetCount()
		{return m_ltsr.GetCount();}
	POSITION GetStart()
		{return m_ltsr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LTSR_UDRD_D& rData)
		{m_ltsr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LTSR_UDRD_D,T_LTSR_UDRD_D&>m_ltsr;
};

#endif