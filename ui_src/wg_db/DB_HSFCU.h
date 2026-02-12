#ifndef __DB_HSFCU_DB_H__
#define __DB_HSFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HSFCU
{
public:
	CDB_HSFCU()
	{
		m_hsfc.InitHashTable(HASHSIZEHSFC);
	}
	virtual ~CDB_HSFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_HSFC_UDRD_D& rData)
		{m_hsfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hsfc.RemoveKey(Key);}
	void DelAll()
		{m_hsfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HSFC_UDRD_D& rData)
		{return m_hsfc.Lookup(Key,rData);}
	int GetCount()
		{return m_hsfc.GetCount();}
	POSITION GetStart()
		{return m_hsfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HSFC_UDRD_D& rData)
		{m_hsfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HSFC_UDRD_D,T_HSFC_UDRD_D&>m_hsfc;
};

#endif