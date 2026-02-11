#ifndef __DB_PBDCU_DB_H__
#define __DB_PBDCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PBDCU
{
public:
	CDB_PBDCU()
	{
		m_pbdc.InitHashTable(HASHSIZEPBDC);
	}
	virtual ~CDB_PBDCU(){};

public:
	void Add(T_UDRD_KEY Key,T_PBDC_UDRD_D& rData)
		{m_pbdc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pbdc.RemoveKey(Key);}
	void DelAll()
		{m_pbdc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PBDC_UDRD_D& rData)
		{return m_pbdc.Lookup(Key,rData);}
	int GetCount()
		{return m_pbdc.GetCount();}
	POSITION GetStart()
		{return m_pbdc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PBDC_UDRD_D& rData)
		{m_pbdc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PBDC_UDRD_D,T_PBDC_UDRD_D&>m_pbdc;
};

#endif