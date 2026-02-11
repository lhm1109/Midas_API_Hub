#ifndef __DB_DCRBU_DB_H__
#define __DB_DCRBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DCRBU
{
public:
	CDB_DCRBU()
	{
		m_dcrb.InitHashTable(HASHSIZEDCRB);
	}
	virtual ~CDB_DCRBU(){};

public:
	void Add(T_UDRD_KEY Key,T_DCRB_UDRD_D& rData)
		{m_dcrb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dcrb.RemoveKey(Key);}
	void DelAll()
		{m_dcrb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DCRB_UDRD_D& rData)
		{return m_dcrb.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DCRB_UDRD_D,T_DCRB_UDRD_D&>m_dcrb;
};

#endif