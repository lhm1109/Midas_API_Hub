#ifndef __DB_DCONU_DB_H__
#define __DB_DCONU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DCONU
{
public:
	CDB_DCONU()
	{
		m_dcon.InitHashTable(HASHSIZEDCON);
	}
	virtual ~CDB_DCONU(){};

public:
	void Add(T_UDRD_KEY Key,T_DCON_UDRD_D& rData)
		{m_dcon.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dcon.RemoveKey(Key);}
	void DelAll()
		{m_dcon.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DCON_UDRD_D& rData)
		{return m_dcon.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DCON_UDRD_D,T_DCON_UDRD_D&>m_dcon;
};

#endif