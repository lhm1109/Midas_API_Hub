#ifndef __DB_SUFCU_DB_H__
#define __DB_SUFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SUFCU
{
public:
	CDB_SUFCU()
	{
		m_sufc.InitHashTable(HASHSIZESUFC);
	}
	virtual ~CDB_SUFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SUFC_UDRD_D& rData)
		{m_sufc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sufc.RemoveKey(Key);}
	void DelAll()
		{m_sufc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SUFC_UDRD_D& rData)
		{return m_sufc.Lookup(Key,rData);}
	int GetCount()
		{return m_sufc.GetCount();}
	POSITION GetStart()
		{return m_sufc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SUFC_UDRD_D& rData)
		{m_sufc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SUFC_UDRD_D,T_SUFC_UDRD_D&>m_sufc;
};

#endif