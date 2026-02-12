#ifndef __DB_JDRTU_DB_H__
#define __DB_JDRTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDRTU
{
public:
	CDB_JDRTU()
	{
		m_JDRT.InitHashTable(HASHSIZEJDRT);
	}
	virtual ~CDB_JDRTU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDRT_UDRD_D& rData)
		{m_JDRT.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDRT.RemoveKey(Key);}
	void DelAll()
		{m_JDRT.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDRT_UDRD_D& rData)
		{return m_JDRT.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDRT_UDRD_D,T_JDRT_UDRD_D&> m_JDRT;
};

#endif