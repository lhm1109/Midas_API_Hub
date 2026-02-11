#ifndef __DB_ULFCU_DB_H__
#define __DB_ULFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ULFCU
{
public:
	CDB_ULFCU()
	{
		m_ulfc.InitHashTable(HASHSIZEULFC);
	}
	virtual ~CDB_ULFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_ULFC_UDRD_D& rData)
		{m_ulfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ulfc.RemoveKey(Key);}
	void DelAll()
		{m_ulfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ULFC_UDRD_D& rData)
		{return m_ulfc.Lookup(Key,rData);}
	int GetCount()
		{return m_ulfc.GetCount();}
	POSITION GetStart()
		{return m_ulfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ULFC_UDRD_D& rData)
		{m_ulfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ULFC_UDRD_D,T_ULFC_UDRD_D&>m_ulfc;
};

#endif