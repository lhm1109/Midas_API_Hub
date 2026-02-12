#ifndef __DB_SBCTU_DB_H__
#define __DB_SBCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SBCTU
{
public:
	CDB_SBCTU()
	{
		m_sbct.InitHashTable(HASHSIZESBCT);
	}
	virtual ~CDB_SBCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_SBCT_UDRD_D& rData)
		{m_sbct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sbct.RemoveKey(Key);}
	void DelAll()
		{m_sbct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SBCT_UDRD_D& rData)
		{return m_sbct.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SBCT_UDRD_D,T_SBCT_UDRD_D&>m_sbct;
};

#endif