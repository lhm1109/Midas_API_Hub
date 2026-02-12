#ifndef __DB_CCFCU_DB_H__
#define __DB_CCFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CCFCU
{
public:
	CDB_CCFCU()
	{
		m_ccfc.InitHashTable(HASHSIZECCFC);
	}
	virtual ~CDB_CCFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_CCFC_UDRD_D& rData)
		{m_ccfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ccfc.RemoveKey(Key);}
	void DelAll()
		{m_ccfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CCFC_UDRD_D& rData)
		{return m_ccfc.Lookup(Key,rData);}
	int GetCount()
		{return m_ccfc.GetCount();}
	POSITION GetStart()
		{return m_ccfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CCFC_UDRD_D& rData)
		{m_ccfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CCFC_UDRD_D,T_CCFC_UDRD_D&>m_ccfc;
};

#endif