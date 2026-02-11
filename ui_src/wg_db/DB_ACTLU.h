#ifndef __DB_ACTLU_DB_H__
#define __DB_ACTLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ACTLU
{
public:
	CDB_ACTLU()
	{
		m_actl.InitHashTable(HASHSIZEACTL);
	}
	virtual ~CDB_ACTLU(){};

public:
	void Add(T_UDRD_KEY Key,T_ACTL_UDRD_D& rData)
		{m_actl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_actl.RemoveKey(Key);}
	void DelAll()
		{m_actl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ACTL_UDRD_D& rData)
		{return m_actl.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ACTL_UDRD_D,T_ACTL_UDRD_D&>m_actl;
};

#endif