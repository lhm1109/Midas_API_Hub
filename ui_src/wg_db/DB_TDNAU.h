#ifndef __DB_TDNAU_DB_H__
#define __DB_TDNAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDNAU
{
public:
	CDB_TDNAU()
	{
		m_tdna.InitHashTable(HASHSIZETDNA);
	}
	virtual ~CDB_TDNAU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDNA_UDRD_D& rData)
		{m_tdna.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdna.RemoveKey(Key);}
	void DelAll()
		{m_tdna.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDNA_UDRD_D& rData)
		{return m_tdna.Lookup(Key,rData);}
	int GetCount()
		{return m_tdna.GetCount();}
	POSITION GetStart()
		{return m_tdna.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDNA_UDRD_D& rData)
		{m_tdna.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDNA_UDRD_D,T_TDNA_UDRD_D&>m_tdna;
};

#endif