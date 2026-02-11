#ifndef __DB_JDBDU_DB_H__
#define __DB_JDBDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDBDU
{
public:
	CDB_JDBDU()
	{
		m_JDBD.InitHashTable(HASHSIZEJDBD);
	}
	virtual ~CDB_JDBDU(){};
	
public:
	void Add(T_UDRD_KEY Key,T_JDBD_UDRD_D& rData)
	{m_JDBD.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_JDBD.RemoveKey(Key);}
	void DelAll()
	{m_JDBD.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDBD_UDRD_D& rData)
	{return m_JDBD.Lookup(Key,rData);}
	
protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDBD_UDRD_D,T_JDBD_UDRD_D&> m_JDBD;
};

#endif