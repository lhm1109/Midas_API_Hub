#ifndef __DB_NLNKU_DB_H__
#define __DB_NLNKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NLNKU
{
public:
	CDB_NLNKU()
	{
		m_nlnk.InitHashTable(HASHSIZENLNK);
	}
	virtual ~CDB_NLNKU(){};

public:
	void Add(T_UDRD_KEY Key,T_NLNK_UDRD_D& rData)
		{m_nlnk.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nlnk.RemoveKey(Key);}
	void DelAll()
		{m_nlnk.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NLNK_UDRD_D& rData)
		{return m_nlnk.Lookup(Key,rData);}
	int GetCount()
		{return m_nlnk.GetCount();}
	POSITION GetStart()
		{return m_nlnk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NLNK_UDRD_D& rData)
		{m_nlnk.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NLNK_UDRD_D,T_NLNK_UDRD_D&>m_nlnk;
};

#endif