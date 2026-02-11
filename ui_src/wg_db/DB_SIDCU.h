#ifndef __DB_SIDCU_DB_H__
#define __DB_SIDCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIDCU
{
public:
	CDB_SIDCU()
	{
		m_sidc.InitHashTable(HASHSIZESIDC);
	}
	virtual ~CDB_SIDCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SIDC_UDRD_D& rData)
		{m_sidc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sidc.RemoveKey(Key);}
	void DelAll()
		{m_sidc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SIDC_UDRD_D& rData)
		{return m_sidc.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SIDC_UDRD_D,T_SIDC_UDRD_D&>m_sidc;
};

#endif