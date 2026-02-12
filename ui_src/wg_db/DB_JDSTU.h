#ifndef __DB_JDSTU_DB_H__
#define __DB_JDSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDSTU
{
public:
	CDB_JDSTU()
	{
		m_JDST.InitHashTable(HASHSIZEJDST);
	}
	virtual ~CDB_JDSTU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDST_UDRD_D& rData)
		{m_JDST.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDST.RemoveKey(Key);}
	void DelAll()
		{m_JDST.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDST_UDRD_D& rData)
		{return m_JDST.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDST_UDRD_D,T_JDST_UDRD_D&> m_JDST;
};

#endif