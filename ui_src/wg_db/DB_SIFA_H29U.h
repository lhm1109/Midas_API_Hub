#ifndef __DB_SIFA_H29U_DB_H__
#define __DB_SIFA_H29U_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIFA_H29U
{
public:
	CDB_SIFA_H29U()
	{
		m_dataMap.InitHashTable(HASHSIZESIFA_H29);
	}
	virtual ~CDB_SIFA_H29U() {};

public:
	void Add(T_UDRD_KEY Key, T_SIFA_H29_UDRD_D& rData)
	{
		m_dataMap.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_dataMap.RemoveKey(Key);
	}
	void DelAll()
	{
		m_dataMap.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIFA_H29_UDRD_D& rData)
	{
		return m_dataMap.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_dataMap.GetCount();
	}
	POSITION GetStart()
	{
		return m_dataMap.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIFA_H29_UDRD_D& rData)
	{
		m_dataMap.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIFA_H29_UDRD_D, T_SIFA_H29_UDRD_D&> m_dataMap;
};

#endif