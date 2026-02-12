#ifndef __DB_AHTRU_DB_H__
#define __DB_AHTRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_AHTRU
{
public:
	CDB_AHTRU()
	{
		m_ahtr.InitHashTable(HASHSIZEAHTR);
	}
	virtual ~CDB_AHTRU(){};

public:
	void Add(T_UDRD_KEY Key,T_AHTR_UDRD_D& rData)
		{m_ahtr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ahtr.RemoveKey(Key);}
	void DelAll()
		{m_ahtr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_AHTR_UDRD_D& rData)
		{return m_ahtr.Lookup(Key,rData);}
	int GetCount()
		{return m_ahtr.GetCount();}
	POSITION GetStart()
		{return m_ahtr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_AHTR_UDRD_D& rData)
		{m_ahtr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_AHTR_UDRD_D,T_AHTR_UDRD_D&>m_ahtr;
};

#endif