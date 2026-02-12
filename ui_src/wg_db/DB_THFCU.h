#ifndef __DB_THFCU_DB_H__
#define __DB_THFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THFCU
{
public:
	CDB_THFCU()
	{
		m_thfc.InitHashTable(HASHSIZETHFC);
	}
	virtual ~CDB_THFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_THFC_UDRD_D& rData)
		{m_thfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thfc.RemoveKey(Key);}
	void DelAll()
		{m_thfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THFC_UDRD_D& rData)
		{return m_thfc.Lookup(Key,rData);}
	int GetCount()
		{return m_thfc.GetCount();}
	POSITION GetStart()
		{return m_thfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THFC_UDRD_D& rData)
		{m_thfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THFC_UDRD_D,T_THFC_UDRD_D&>m_thfc;
};

#endif