#ifndef __DB_BSEDU_DB_H__
#define __DB_BSEDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BSEDU
{
public:
	CDB_BSEDU()
	{
		m_bsed.InitHashTable(HASHSIZEBSED);
	}
	virtual ~CDB_BSEDU(){};

public:
	void Add(T_UDRD_KEY Key,T_BSED_UDRD_D& rData)
		{m_bsed.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bsed.RemoveKey(Key);}
	void DelAll()
		{m_bsed.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BSED_UDRD_D& rData)
		{return m_bsed.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BSED_UDRD_D,T_BSED_UDRD_D&>m_bsed;
};

#endif