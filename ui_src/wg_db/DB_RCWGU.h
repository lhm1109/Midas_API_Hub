#ifndef __DB_RCWGU_DB_H__
#define __DB_RCWGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCWGU
{
public:
	CDB_RCWGU()
	{
		m_rcwg.InitHashTable(HASHSIZERCWG);
	}
	virtual ~CDB_RCWGU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCWG_UDRD_D& rData)
		{m_rcwg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rcwg.RemoveKey(Key);}
	void DelAll()
		{m_rcwg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCWG_UDRD_D& rData)
		{return m_rcwg.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCWG_UDRD_D,T_RCWG_UDRD_D&>m_rcwg;
};

#endif