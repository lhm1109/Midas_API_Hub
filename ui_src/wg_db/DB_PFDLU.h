#ifndef __DB_PFDLU_DB_H__
#define __DB_PFDLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PFDLU
{
public:
	CDB_PFDLU()
	{
		m_pfdl.InitHashTable(HASHSIZEPFDL);
	}
	virtual ~CDB_PFDLU(){};

public:
	void Add(T_UDRD_KEY Key,T_PFDL_UDRD_D& rData)
		{m_pfdl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pfdl.RemoveKey(Key);}
	void DelAll()
		{m_pfdl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PFDL_UDRD_D& rData)
		{return m_pfdl.Lookup(Key,rData);}
	int GetCount()
		{return m_pfdl.GetCount();}
	POSITION GetStart()
		{return m_pfdl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PFDL_UDRD_D& rData)
		{m_pfdl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PFDL_UDRD_D,T_PFDL_UDRD_D&>m_pfdl;
};

#endif