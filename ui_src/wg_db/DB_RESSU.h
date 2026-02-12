#ifndef __DB_RESSU_DB_H__
#define __DB_RESSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RESSU
{
public:
	CDB_RESSU()
	{
		m_ress.InitHashTable(HASHSIZERESS);
	}
	virtual ~CDB_RESSU(){};

public:
	void Add(T_UDRD_KEY Key,T_RESS_UDRD_D& rData)
		{m_ress.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ress.RemoveKey(Key);}
	void DelAll()
		{m_ress.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RESS_UDRD_D& rData)
		{return m_ress.Lookup(Key,rData);}
	int GetCount()
		{return m_ress.GetCount();}
	POSITION GetStart()
		{return m_ress.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RESS_UDRD_D& rData)
		{m_ress.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RESS_UDRD_D,T_RESS_UDRD_D&>m_ress;
};

#endif