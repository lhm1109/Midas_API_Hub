#ifndef __DB_DAOPU_DB_H__
#define __DB_DAOPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DAOPU
{
public:
	CDB_DAOPU()
	{
		m_daop.InitHashTable(HASHSIZEDAOP);
	}
	virtual ~CDB_DAOPU(){};

public:
	void Add(T_UDRD_KEY Key,T_DAOP_UDRD_D& rData)
		{m_daop.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_daop.RemoveKey(Key);}
	void DelAll()
		{m_daop.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DAOP_UDRD_D& rData)
		{return m_daop.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DAOP_UDRD_D,T_DAOP_UDRD_D&>m_daop;
};

#endif