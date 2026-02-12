#ifndef __DB_CESOU_DB_H__
#define __DB_CESOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CESOU
{
public:
	CDB_CESOU()
	{
		m_ceso.InitHashTable(HASHSIZECESO);
	}
	virtual ~CDB_CESOU(){};

public:
	void Add(T_UDRD_KEY Key,T_CESO_UDRD_D& rData)
		{m_ceso.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ceso.RemoveKey(Key);}
	void DelAll()
		{m_ceso.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CESO_UDRD_D& rData)
		{return m_ceso.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_CESO_UDRD_D,T_CESO_UDRD_D&>m_ceso;
};

#endif