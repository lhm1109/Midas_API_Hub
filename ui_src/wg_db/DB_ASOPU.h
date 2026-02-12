#ifndef __DB_ASOPU_DB_H__
#define __DB_ASOPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ASOPU
{
public:
	CDB_ASOPU()
	{
		m_asop.InitHashTable(HASHSIZEASOP);
	}
	virtual ~CDB_ASOPU(){};

public:
	void Add(T_UDRD_KEY Key,T_ASOP_UDRD_D& rData)
		{m_asop.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_asop.RemoveKey(Key);}
	void DelAll()
		{m_asop.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ASOP_UDRD_D& rData)
		{return m_asop.Lookup(Key,rData);}
	int GetCount()
		{return m_asop.GetCount();}
	POSITION GetStart()
		{return m_asop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ASOP_UDRD_D& rData)
		{m_asop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ASOP_UDRD_D,T_ASOP_UDRD_D&>m_asop;
};

#endif