#ifndef __DB_SEMEU_DB_H__
#define __DB_SEMEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SEMEU
{
public:
	CDB_SEMEU()
	{
		m_seme.InitHashTable(HASHSIZESEME);
	}
	virtual ~CDB_SEMEU(){};

public:
	void Add(T_UDRD_KEY Key,T_SEME_UDRD_D& rData)
		{m_seme.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_seme.RemoveKey(Key);}
	void DelAll()
		{m_seme.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SEME_UDRD_D& rData)
		{return m_seme.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SEME_UDRD_D,T_SEME_UDRD_D&>m_seme;
};

#endif