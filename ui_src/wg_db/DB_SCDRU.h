#ifndef __DB_SCDRU_DB_H__
#define __DB_SCDRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SCDRU
{
public:
	CDB_SCDRU()
	{
		m_SCDR.InitHashTable(HASHSIZESCDR);
	}
	virtual ~CDB_SCDRU(){};

public:
	void Add(T_UDRD_KEY Key,T_SCDR_UDRD_D& rData)
	{m_SCDR.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_SCDR.RemoveKey(Key);}
	void DelAll()
	{m_SCDR.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SCDR_UDRD_D& rData)
	{return m_SCDR.Lookup(Key,rData);}
	int GetCount()
	{return m_SCDR.GetCount();}
	POSITION GetStart()
	{return m_SCDR.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SCDR_UDRD_D& rData)
	{m_SCDR.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SCDR_UDRD_D,T_SCDR_UDRD_D&>m_SCDR;
};

#endif
