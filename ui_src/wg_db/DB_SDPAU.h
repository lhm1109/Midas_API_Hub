#ifndef __DB_SDPAU_DB_H__
#define __DB_SDPAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDPAU
{
public:
	CDB_SDPAU()
	{
		m_sdpa.InitHashTable(HASHSIZESDPA);
	}
	virtual ~CDB_SDPAU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDPA_UDRD_D& rData)
		{m_sdpa.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdpa.RemoveKey(Key);}
	void DelAll()
		{m_sdpa.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDPA_UDRD_D& rData)
		{return m_sdpa.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDPA_UDRD_D,T_SDPA_UDRD_D&>m_sdpa;
};

#endif