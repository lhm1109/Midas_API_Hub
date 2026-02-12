#ifndef __DB_FLBKU_DB_H__
#define __DB_FLBKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FLBKU
{
public:
	CDB_FLBKU()
	{
		m_flbk.InitHashTable(HASHSIZEFLBK);
	}
	virtual ~CDB_FLBKU(){};

public:
	void Add(T_UDRD_KEY Key,T_FLBK_UDRD_D& rData)
	{m_flbk.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_flbk.RemoveKey(Key);}
	void DelAll()
	{m_flbk.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FLBK_UDRD_D& rData)
	{return m_flbk.Lookup(Key,rData);}
	int GetCount()
	{return m_flbk.GetCount();}
	POSITION GetStart()
	{return m_flbk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FLBK_UDRD_D& rData)
	{m_flbk.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_FLBK_UDRD_D,T_FLBK_UDRD_D&>m_flbk;
};

#endif