#ifndef __DB_RSDLU_DB_H__
#define __DB_RSDLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RSDLU
{
public:
	CDB_RSDLU()
	{
		m_rsdl.InitHashTable(HASHSIZERSDL);
	}
	virtual ~CDB_RSDLU(){};

public:
	void Add(T_UDRD_KEY Key,T_RSDL_UDRD_D& rData)
		{m_rsdl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rsdl.RemoveKey(Key);}
	void DelAll()
		{m_rsdl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RSDL_UDRD_D& rData)
		{return m_rsdl.Lookup(Key,rData);}
	int GetCount()
		{return m_rsdl.GetCount();}
	POSITION GetStart()
		{return m_rsdl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RSDL_UDRD_D& rData)
		{m_rsdl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RSDL_UDRD_D,T_RSDL_UDRD_D&>m_rsdl;
};

#endif