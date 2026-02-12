#ifndef __DB_INFLU_DB_H__
#define __DB_INFLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_INFLU
{
public:
	CDB_INFLU()
	{
		m_infl.InitHashTable(HASHSIZEINFL);
	}
	virtual ~CDB_INFLU(){};

public:
	void Add(T_UDRD_KEY Key,T_INFL_UDRD_D& rData)
		{m_infl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_infl.RemoveKey(Key);}
	void DelAll()
		{m_infl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_INFL_UDRD_D& rData)
		{return m_infl.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_INFL_UDRD_D,T_INFL_UDRD_D&>m_infl;
};

#endif