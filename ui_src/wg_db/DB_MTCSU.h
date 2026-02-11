#ifndef __DB_MTCSU_DB_H__
#define __DB_MTCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MTCSU
{
public:
	CDB_MTCSU()
	{
		m_mtcs.InitHashTable(HASHSIZEMTCS);
	}
	virtual ~CDB_MTCSU(){};

public:
	void Add(T_UDRD_KEY Key,T_MTCS_UDRD_D& rData)
		{m_mtcs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mtcs.RemoveKey(Key);}
	void DelAll()
		{m_mtcs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MTCS_UDRD_D& rData)
		{return m_mtcs.Lookup(Key,rData);}
	int GetCount()
		{return m_mtcs.GetCount();}
	POSITION GetStart()
		{return m_mtcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MTCS_UDRD_D& rData)
		{m_mtcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MTCS_UDRD_D,T_MTCS_UDRD_D&>m_mtcs;
};

#endif