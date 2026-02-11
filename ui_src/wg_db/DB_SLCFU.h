#ifndef __DB_SLCFU_DB_H__
#define __DB_SLCFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLCFU
{
public:
	CDB_SLCFU()
	{
		m_slcf.InitHashTable(HASHSIZESLCF);
	}
	virtual ~CDB_SLCFU(){};

public:
	void Add(T_UDRD_KEY Key,T_SLCF_UDRD_D& rData)
		{m_slcf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_slcf.RemoveKey(Key);}
	void DelAll()
		{m_slcf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLCF_UDRD_D& rData)
		{return m_slcf.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLCF_UDRD_D,T_SLCF_UDRD_D&>m_slcf;
};

#endif