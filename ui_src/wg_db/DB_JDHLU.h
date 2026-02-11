#ifndef __DB_JDHLU_DB_H__
#define __DB_JDHLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDHLU
{
public:
	CDB_JDHLU()
	{
		m_JDHL.InitHashTable(HASHSIZEJDHL);
	}
	virtual ~CDB_JDHLU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDHL_UDRD_D& rData)
		{m_JDHL.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDHL.RemoveKey(Key);}
	void DelAll()
		{m_JDHL.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDHL_UDRD_D& rData)
		{return m_JDHL.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDHL_UDRD_D,T_JDHL_UDRD_D&> m_JDHL;
};

#endif