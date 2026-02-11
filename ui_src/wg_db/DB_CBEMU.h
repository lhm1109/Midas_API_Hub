#ifndef __DB_CBEMU_DB_H__
#define __DB_CBEMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CBEMU
{
public:
	CDB_CBEMU()
	{
		m_cbem.InitHashTable(HASHSIZECBEM);
	}
	virtual ~CDB_CBEMU(){};

public:
	void Add(T_UDRD_KEY Key,T_CBEM_UDRD_D& rData)
		{m_cbem.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cbem.RemoveKey(Key);}
	void DelAll()
		{m_cbem.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CBEM_UDRD_D& rData)
		{return m_cbem.Lookup(Key,rData);}
	int GetCount()
		{return m_cbem.GetCount();}
	POSITION GetStart()
		{return m_cbem.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CBEM_UDRD_D& rData)
		{m_cbem.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CBEM_UDRD_D,T_CBEM_UDRD_D&>m_cbem;
};

#endif
