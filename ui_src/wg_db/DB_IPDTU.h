#ifndef __DB_IPDTU_DB_H__
#define __DB_IPDTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_IPDTU
{
public:
	CDB_IPDTU()
	{
		m_IPDT.InitHashTable(HASHSIZEIPDT);
	}
	virtual ~CDB_IPDTU(){};

public:
	void Add(T_UDRD_KEY Key,T_IPDT_UDRD_D& rData)
	{m_IPDT.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_IPDT.RemoveKey(Key);}
	void DelAll()
	{m_IPDT.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IPDT_UDRD_D& rData)
	{return m_IPDT.Lookup(Key,rData);}
	int GetCount()
	{return m_IPDT.GetCount();}
	POSITION GetStart()
	{return m_IPDT.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IPDT_UDRD_D& rData)
	{m_IPDT.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_IPDT_UDRD_D,T_IPDT_UDRD_D&>m_IPDT;
};

#endif
