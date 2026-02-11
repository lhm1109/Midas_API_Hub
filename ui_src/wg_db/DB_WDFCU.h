#ifndef __DB_WDFCU_DB_H__
#define __DB_WDFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WDFCU
{
public:
	CDB_WDFCU()
	{
		m_wdfc.InitHashTable(HASHSIZEWDFC);
	}
	virtual ~CDB_WDFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_WDFC_UDRD_D& rData)
		{m_wdfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wdfc.RemoveKey(Key);}
	void DelAll()
		{m_wdfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WDFC_UDRD_D& rData)
		{return m_wdfc.Lookup(Key,rData);}
	int GetCount()
		{return m_wdfc.GetCount();}
	POSITION GetStart()
		{return m_wdfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WDFC_UDRD_D& rData)
		{m_wdfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WDFC_UDRD_D,T_WDFC_UDRD_D&>m_wdfc;
};

#endif