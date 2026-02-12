#ifndef __DB_SPVMU_DB_H__
#define __DB_SPVMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPVMU
{
public:
	CDB_SPVMU()
	{
		m_spvm.InitHashTable(HASHSIZEMBTP);
	}
	virtual ~CDB_SPVMU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPVM_UDRD_D& rData)
		{m_spvm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_spvm.RemoveKey(Key);}
	void DelAll()
		{m_spvm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPVM_UDRD_D& rData)
		{return m_spvm.Lookup(Key,rData);}
	int GetCount()
		{return m_spvm.GetCount();}
	POSITION GetStart()
		{return m_spvm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPVM_KEY& rKey,T_SPVM_UDRD_D& rData)
		{m_spvm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPVM_UDRD_D,T_SPVM_UDRD_D&>m_spvm;
};

#endif