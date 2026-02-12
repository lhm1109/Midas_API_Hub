#ifndef __DB_LCWDU_DB_H__
#define __DB_LCWDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LCWDU
{
public:
	CDB_LCWDU()
	{
		m_lcwd.InitHashTable(HASHSIZELCWD);
	}
	virtual ~CDB_LCWDU(){};

public:
	void Add(T_UDRD_KEY Key,T_LCWD_UDRD_D& rData)
		{m_lcwd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lcwd.RemoveKey(Key);}
	void DelAll()
		{m_lcwd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LCWD_UDRD_D& rData)
		{return m_lcwd.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LCWD_UDRD_D,T_LCWD_UDRD_D&>m_lcwd;
};

#endif