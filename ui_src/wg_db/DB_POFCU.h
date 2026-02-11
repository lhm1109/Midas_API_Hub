#ifndef __DB_POFCU_DB_H__
#define __DB_POFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POFCU
{
public:
	CDB_POFCU()
	{
		m_pofc.InitHashTable(HASHSIZEPOFC);
	}
	virtual ~CDB_POFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_POFC_UDRD_D& rData)
		{m_pofc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pofc.RemoveKey(Key);}
	void DelAll()
		{m_pofc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POFC_UDRD_D& rData)
		{return m_pofc.Lookup(Key,rData);}
	int GetCount()
		{return m_pofc.GetCount();}
	POSITION GetStart()
		{return m_pofc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_POFC_UDRD_D& rData)
		{m_pofc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POFC_UDRD_D,T_POFC_UDRD_D&>m_pofc;
};

#endif