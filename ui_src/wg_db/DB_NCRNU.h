#ifndef __DB_NCRNU_DB_H__
#define __DB_NCRNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NCRNU
{
public:
	CDB_NCRNU()
	{
		m_ncrn.InitHashTable(HASHSIZENCRN);
	}
	virtual ~CDB_NCRNU(){};

public:
	void Add(T_UDRD_KEY Key,T_NCRN_UDRD_D& rData)
		{m_ncrn.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ncrn.RemoveKey(Key);}
	void DelAll()
		{m_ncrn.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NCRN_UDRD_D& rData)
		{return m_ncrn.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NCRN_UDRD_D,T_NCRN_UDRD_D&>m_ncrn;
};

#endif