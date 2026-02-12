#ifndef __DB_HECBU_DB_H__
#define __DB_HECBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HECBU
{
public:
	CDB_HECBU()
	{
		m_hecb.InitHashTable(HASHSIZEHECB);
	}
	virtual ~CDB_HECBU(){};

public:
	void Add(T_UDRD_KEY Key,T_HECB_UDRD_D& rData)
		{m_hecb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hecb.RemoveKey(Key);}
	void DelAll()
		{m_hecb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HECB_UDRD_D& rData)
		{return m_hecb.Lookup(Key,rData);}
	int GetCount()
		{return m_hecb.GetCount();}
	POSITION GetStart()
		{return m_hecb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HECB_UDRD_D& rData)
		{m_hecb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HECB_UDRD_D,T_HECB_UDRD_D&>m_hecb;
};

#endif