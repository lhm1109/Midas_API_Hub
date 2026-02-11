#ifndef __DB_CCTLU_DB_H__
#define __DB_CCTLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CCTLU
{
public:
	CDB_CCTLU()
	{
		m_cctl.InitHashTable(HASHSIZECCTL);
	}
	virtual ~CDB_CCTLU(){};

public:
	void Add(T_UDRD_KEY Key,T_CCTL_UDRD_D& rData)
		{m_cctl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cctl.RemoveKey(Key);}
	void DelAll()
		{m_cctl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CCTL_UDRD_D& rData)
		{return m_cctl.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CCTL_UDRD_D,T_CCTL_UDRD_D&>m_cctl;
};

#endif