#ifndef __DB_TLGDU_DB_H__
#define __DB_TLGDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TLGDU
{
public:
	CDB_TLGDU()
	{
		m_tlgd.InitHashTable(HASHSIZETLGD);
	}
	virtual ~CDB_TLGDU(){};

public:
	void Add(T_UDRD_KEY Key,T_TLGD_UDRD_D& rData)
		{m_tlgd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tlgd.RemoveKey(Key);}
	void DelAll()
		{m_tlgd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TLGD_UDRD_D& rData)
		{return m_tlgd.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TLGD_UDRD_D,T_TLGD_UDRD_D&>m_tlgd;
};

#endif