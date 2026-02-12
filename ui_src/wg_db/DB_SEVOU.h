#ifndef __DB_SEVOU_DB_H__
#define __DB_SEVOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SEVOU
{
public:
	CDB_SEVOU()
	{
		m_sevo.InitHashTable(HASHSIZESEVO);
	}
	virtual ~CDB_SEVOU(){};

public:
	void Add(T_UDRD_KEY Key,T_SEVO_UDRD_D& rData)
		{m_sevo.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sevo.RemoveKey(Key);}
	void DelAll()
		{m_sevo.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SEVO_UDRD_D& rData)
		{return m_sevo.Lookup(Key,rData);}
	int GetCount()
		{return m_sevo.GetCount();}
	POSITION GetStart()
		{return m_sevo.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SEVO_UDRD_D& rData)
		{m_sevo.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SEVO_UDRD_D,T_SEVO_UDRD_D&>m_sevo;
};

#endif
