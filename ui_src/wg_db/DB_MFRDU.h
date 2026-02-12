#ifndef __DB_MFRDU_DB_H__
#define __DB_MFRDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MFRDU
{
public:
	CDB_MFRDU()
	{
		m_mfrd.InitHashTable(HASHSIZEMFRD);
	}
	virtual ~CDB_MFRDU(){};

public:
	void Add(T_UDRD_KEY Key,T_MFRD_UDRD_D& rData)
		{m_mfrd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mfrd.RemoveKey(Key);}
	void DelAll()
		{m_mfrd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MFRD_UDRD_D& rData)
		{return m_mfrd.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MFRD_UDRD_D,T_MFRD_UDRD_D&>m_mfrd;
};

#endif