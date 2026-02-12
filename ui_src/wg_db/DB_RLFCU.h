#ifndef __DB_RLFCU_DB_H__
#define __DB_RLFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RLFCU
{
public:
	CDB_RLFCU()
	{
		m_rlfc.InitHashTable(HASHSIZERLFC);
	}
	virtual ~CDB_RLFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_RLFC_UDRD_D& rData)
		{m_rlfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rlfc.RemoveKey(Key);}
	void DelAll()
		{m_rlfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RLFC_UDRD_D& rData)
		{return m_rlfc.Lookup(Key,rData);}
	int GetCount()
		{return m_rlfc.GetCount();}
	POSITION GetStart()
		{return m_rlfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RLFC_UDRD_D& rData)
		{m_rlfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RLFC_UDRD_D,T_RLFC_UDRD_D&>m_rlfc;
};

#endif