#ifndef __DB_SDVIU_DB_H__
#define __DB_SDVIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDVIU
{
public:
	CDB_SDVIU()
	{
		m_sdvi.InitHashTable(HASHSIZESDVI);
	}
	virtual ~CDB_SDVIU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDVI_UDRD_D& rData)
		{m_sdvi.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdvi.RemoveKey(Key);}
	void DelAll()
		{m_sdvi.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDVI_UDRD_D& rData)
		{return m_sdvi.Lookup(Key,rData);}
	int GetCount()
		{return m_sdvi.GetCount();}
	POSITION GetStart()
		{return m_sdvi.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDVI_UDRD_D& rData)
		{m_sdvi.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDVI_UDRD_D,T_SDVI_UDRD_D&>m_sdvi;
};

#endif