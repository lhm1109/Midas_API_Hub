#ifndef __DB_HHNDU_DB_H__
#define __DB_HHNDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HHNDU
{
public:
	CDB_HHNDU()
	{
		m_hhnd.InitHashTable(HASHSIZEHHND);
	}
	virtual ~CDB_HHNDU(){};

public:
	void Add(T_UDRD_KEY Key,T_HHND_UDRD_D& rData)
		{m_hhnd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hhnd.RemoveKey(Key);}
	void DelAll()
		{m_hhnd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HHND_UDRD_D& rData)
		{return m_hhnd.Lookup(Key,rData);}
	int GetCount()
		{return m_hhnd.GetCount();}
	POSITION GetStart()
		{return m_hhnd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HHND_UDRD_D& rData)
		{m_hhnd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HHND_UDRD_D,T_HHND_UDRD_D&>m_hhnd;
};

#endif