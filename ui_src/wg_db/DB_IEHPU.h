#ifndef __DB_IEHPU_DB_H__
#define __DB_IEHPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IEHPU
{
public:
	CDB_IEHPU()
	{
		m_iehp.InitHashTable(HASHSIZEIEHP);
	}
	virtual ~CDB_IEHPU(){};

public:
	void Add(T_UDRD_KEY Key,T_IEHP_UDRD_D& rData)
		{m_iehp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_iehp.RemoveKey(Key);}
	void DelAll()
		{m_iehp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IEHP_UDRD_D& rData)
		{return m_iehp.Lookup(Key,rData);}
	int GetCount()
		{return m_iehp.GetCount();}
	POSITION GetStart()
		{return m_iehp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IEHP_UDRD_D& rData)
		{m_iehp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IEHP_UDRD_D,T_IEHP_UDRD_D&>m_iehp;
};

#endif