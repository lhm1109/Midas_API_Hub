#ifndef __DB_MGDRU_DB_H__
#define __DB_MGDRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MGDRU
{
public:
	CDB_MGDRU()
	{
		m_mgdr.InitHashTable(HASHSIZEMGDR);
	}
	virtual ~CDB_MGDRU(){};

public:
	void Add(T_UDRD_KEY Key,T_MGDR_UDRD_D& rData)
		{m_mgdr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mgdr.RemoveKey(Key);}
	void DelAll()
		{m_mgdr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MGDR_UDRD_D& rData)
		{return m_mgdr.Lookup(Key,rData);}
	int GetCount()
		{return m_mgdr.GetCount();}
	POSITION GetStart()
		{return m_mgdr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MGDR_UDRD_D& rData)
		{m_mgdr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MGDR_UDRD_D,T_MGDR_UDRD_D&>m_mgdr;
};

#endif
