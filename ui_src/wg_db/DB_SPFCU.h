#ifndef __DB_SPFCU_DB_H__
#define __DB_SPFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPFCU
{
public:
	CDB_SPFCU()
	{
		m_spfc.InitHashTable(HASHSIZESPFC);
	}
	virtual ~CDB_SPFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPFC_UDRD_D& rData)
		{m_spfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_spfc.RemoveKey(Key);}
	void DelAll()
		{m_spfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPFC_UDRD_D& rData)
		{return m_spfc.Lookup(Key,rData);}
	int GetCount()
		{return m_spfc.GetCount();}
	POSITION GetStart()
		{return m_spfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPFC_UDRD_D& rData)
		{m_spfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPFC_UDRD_D,T_SPFC_UDRD_D&>m_spfc;
};

#endif