#ifndef __DB_VBEMU_DB_H__
#define __DB_VBEMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_VBEMU
{
public:
	CDB_VBEMU()
	{
		m_vbem.InitHashTable(HASHSIZEVBEM);
	}
	virtual ~CDB_VBEMU(){};

public:
	void Add(T_UDRD_KEY Key,T_VBEM_UDRD_D& rData)
		{m_vbem.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_vbem.RemoveKey(Key);}
	void DelAll()
		{m_vbem.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_VBEM_UDRD_D& rData)
		{return m_vbem.Lookup(Key,rData);}
	int GetCount()
		{return m_vbem.GetCount();}
	POSITION GetStart()
		{return m_vbem.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_VBEM_UDRD_D& rData)
		{m_vbem.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_VBEM_UDRD_D,T_VBEM_UDRD_D&>m_vbem;
};

#endif