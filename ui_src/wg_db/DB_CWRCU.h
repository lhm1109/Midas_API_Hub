#ifndef __DB_CWRCU_DB_H__
#define __DB_CWRCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CWRCU
{
public:
	CDB_CWRCU()
	{
		m_cwrc.InitHashTable(HASHSIZECWRC);
	}
	virtual ~CDB_CWRCU(){};

public:
	void Add(T_UDRD_KEY Key,T_CWRC_UDRD_D& rData)
		{m_cwrc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cwrc.RemoveKey(Key);}
	void DelAll()
		{m_cwrc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CWRC_UDRD_D& rData)
		{return m_cwrc.Lookup(Key,rData);}
	int GetCount()
		{return m_cwrc.GetCount();}
	POSITION GetStart()
		{return m_cwrc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CWRC_UDRD_D& rData)
		{m_cwrc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CWRC_UDRD_D,T_CWRC_UDRD_D&>m_cwrc;
};

#endif