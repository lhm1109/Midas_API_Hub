#ifndef __DB_UTCFU_DB_H__
#define __DB_UTCFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UTCFU
{
public:
	CDB_UTCFU()
	{
		m_utcf.InitHashTable(HASHSIZEUTCF);
	}
	virtual ~CDB_UTCFU(){};

public:
	void Add(T_UDRD_KEY Key,T_UTCF_UDRD_D& rData)
		{m_utcf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_utcf.RemoveKey(Key);}
	void DelAll()
		{m_utcf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_UTCF_UDRD_D& rData)
		{return m_utcf.Lookup(Key,rData);}
	int GetCount()
		{return m_utcf.GetCount();}
	POSITION GetStart()
		{return m_utcf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UTCF_UDRD_D& rData)
		{m_utcf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_UTCF_UDRD_D,T_UTCF_UDRD_D&>m_utcf;
};

#endif