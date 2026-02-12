#ifndef __DB_DMASU_DB_H__
#define __DB_DMASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DMASU
{
public:
	CDB_DMASU()
	{
		m_dmas.InitHashTable(HASHSIZEDMAS);
	}
	virtual ~CDB_DMASU(){};

public:
	void Add(T_UDRD_KEY Key,T_DMAS_UDRD_D& rData)
		{m_dmas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dmas.RemoveKey(Key);}
	void DelAll()
		{m_dmas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DMAS_UDRD_D& rData)
		{return m_dmas.Lookup(Key,rData);}
	int GetCount()
		{return m_dmas.GetCount();}
	POSITION GetStart()
		{return m_dmas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DMAS_UDRD_D& rData)
		{m_dmas.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DMAS_UDRD_D,T_DMAS_UDRD_D&>m_dmas;
};

#endif