#ifndef __DB_BTMPU_DB_H__
#define __DB_BTMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BTMPU
{
public:
	CDB_BTMPU()
	{
		m_btmp.InitHashTable(HASHSIZEBTMP);
	}
	virtual ~CDB_BTMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_BTMP_UDRD_D& rData)
		{m_btmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_btmp.RemoveKey(Key);}
	void DelAll()
		{m_btmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BTMP_UDRD_D& rData)
		{return m_btmp.Lookup(Key,rData);}
	int GetCount()
		{return m_btmp.GetCount();}
	POSITION GetStart()
		{return m_btmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BTMP_UDRD_D& rData)
		{m_btmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BTMP_UDRD_D,T_BTMP_UDRD_D&>m_btmp;
};

#endif