#ifndef __DB_PHGTU_DB_H__
#define __DB_PHGTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PHGTU
{
public:
	CDB_PHGTU()
	{
		m_phgt.InitHashTable(HASHSIZEPHGT);
	}
	virtual ~CDB_PHGTU(){};

public:
	void Add(T_UDRD_KEY Key,T_PHGT_UDRD_D& rData)
		{m_phgt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_phgt.RemoveKey(Key);}
	void DelAll()
		{m_phgt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PHGT_UDRD_D& rData)
		{return m_phgt.Lookup(Key,rData);}
	int GetCount()
		{return m_phgt.GetCount();}
	POSITION GetStart()
		{return m_phgt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PHGT_UDRD_D& rData)
		{m_phgt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PHGT_UDRD_D,T_PHGT_UDRD_D&>m_phgt;
};

#endif