#ifndef __DB_SIHOU_DB_H__
#define __DB_SIHOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIHOU
{
public:
	CDB_SIHOU()
	{
		m_siho.InitHashTable(HASHSIZESIHO);
	}
	virtual ~CDB_SIHOU(){};

public:
	void Add(T_UDRD_KEY Key,T_SIHO_UDRD_D& rData)
	{m_siho.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_siho.RemoveKey(Key);}
	void DelAll()
	{m_siho.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SIHO_UDRD_D& rData)
	{return m_siho.Lookup(Key,rData);}
	int GetCount()
	{return m_siho.GetCount();}
	POSITION GetStart()
	{return m_siho.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SIHO_UDRD_D& rData)
	{m_siho.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SIHO_UDRD_D,T_SIHO_UDRD_D&>m_siho;
};

#endif