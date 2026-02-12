#ifndef __DB_BLRGU_DB_H__
#define __DB_BLRGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BLRGU
{
public:
	CDB_BLRGU()
	{
		m_blrg.InitHashTable(HASHSIZEBLRG);
	}
	virtual ~CDB_BLRGU(){};

public:
	void Add(T_UDRD_KEY Key,T_BLRG_UDRD_D& rData)
		{m_blrg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_blrg.RemoveKey(Key);}
	void DelAll()
		{m_blrg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BLRG_UDRD_D& rData)
		{return m_blrg.Lookup(Key,rData);}
	int GetCount()
		{return m_blrg.GetCount();}
	POSITION GetStart()
		{return m_blrg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BLRG_UDRD_D& rData)
		{m_blrg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BLRG_UDRD_D,T_BLRG_UDRD_D&>m_blrg;
};

#endif