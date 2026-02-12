#ifndef __DB_GSBGU_DB_H__
#define __DB_GSBGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GSBGU
{
public:
	CDB_GSBGU()
	{
		m_gsbg.InitHashTable(HASHSIZEGSBG);
	}
	virtual ~CDB_GSBGU(){};

public:
	void Add(T_UDRD_KEY Key,T_GSBG_UDRD_D& rData)
		{m_gsbg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gsbg.RemoveKey(Key);}
	void DelAll()
		{m_gsbg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GSBG_UDRD_D& rData)
		{return m_gsbg.Lookup(Key,rData);}
	int GetCount()
		{return m_gsbg.GetCount();}
	POSITION GetStart()
		{return m_gsbg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GSBG_UDRD_D& rData)
		{m_gsbg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GSBG_UDRD_D,T_GSBG_UDRD_D&>m_gsbg;
};

#endif