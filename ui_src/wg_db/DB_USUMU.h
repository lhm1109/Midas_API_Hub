#ifndef __DB_USUMU_DB_H__
#define __DB_USUMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_USUMU
{
public:
	CDB_USUMU()
	{
		m_usum.InitHashTable(HASHSIZEUSUM);
	}
	virtual ~CDB_USUMU(){};

public:
	void Add(T_UDRD_KEY Key,T_USUM_UDRD_D& rData)
		{m_usum.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_usum.RemoveKey(Key);}
	void DelAll()
		{m_usum.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_USUM_UDRD_D& rData)
		{return m_usum.Lookup(Key,rData);}
	int GetCount()
		{return m_usum.GetCount();}
	POSITION GetStart()
		{return m_usum.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_USUM_UDRD_D& rData)
		{m_usum.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_USUM_UDRD_D,T_USUM_UDRD_D&>m_usum;
};

#endif