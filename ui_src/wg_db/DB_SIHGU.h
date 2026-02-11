#ifndef __DB_SIHGU_DB_H__
#define __DB_SIHGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIHGU
{
public:
	CDB_SIHGU()
	{
		m_sihg.InitHashTable(HASHSIZESIHG);
	}
	virtual ~CDB_SIHGU(){};

public:
	void Add(T_UDRD_KEY Key,T_SIHG_UDRD_D& rData)
	{m_sihg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_sihg.RemoveKey(Key);}
	void DelAll()
	{m_sihg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SIHG_UDRD_D& rData)
	{return m_sihg.Lookup(Key,rData);}
	int GetCount()
	{return m_sihg.GetCount();}
	POSITION GetStart()
	{return m_sihg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SIHG_UDRD_D& rData)
	{m_sihg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SIHG_UDRD_D,T_SIHG_UDRD_D&>m_sihg;
};

#endif