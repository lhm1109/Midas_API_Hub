#ifndef __DB_RCDCU_DB_H__
#define __DB_RCDCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCDCU
{
public:
	CDB_RCDCU()
	{
		m_rcdc.InitHashTable(HASHSIZERCDC);
	}
	virtual ~CDB_RCDCU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCDC_UDRD_D& rData)
	{m_rcdc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_rcdc.RemoveKey(Key);}
	void DelAll()
	{m_rcdc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCDC_UDRD_D& rData)
	{return m_rcdc.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCDC_UDRD_D,T_RCDC_UDRD_D&>m_rcdc;
};

#endif