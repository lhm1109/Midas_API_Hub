#ifndef __DB_CRGRU_DB_H__
#define __DB_CRGRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CRGRU
{
public:
	CDB_CRGRU()
	{
		m_crgr.InitHashTable(HASHSIZECRGR);
	}
	virtual ~CDB_CRGRU(){};

public:
	void Add(T_UDRD_KEY Key,T_CRGR_UDRD_D& rData)
		{m_crgr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_crgr.RemoveKey(Key);}
	void DelAll()
		{m_crgr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CRGR_UDRD_D& rData)
		{return m_crgr.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CRGR_UDRD_D,T_CRGR_UDRD_D&>m_crgr;
};

#endif