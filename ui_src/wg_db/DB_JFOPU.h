#ifndef __DB_JFOPU_DB_H__
#define __DB_JFOPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_JFOPU
{
public:
	CDB_JFOPU()
	{
		m_JFOP.InitHashTable(HASHSIZEJFOP);
	}
	virtual ~CDB_JFOPU(){};
	
public:
	void Add(T_UDRD_KEY Key,T_JFOP_UDRD_D& rData)
	{m_JFOP.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_JFOP.RemoveKey(Key);}
	void DelAll()
	{m_JFOP.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JFOP_UDRD_D& rData)
	{return m_JFOP.Lookup(Key,rData);}
	
protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_JFOP_UDRD_D,T_JFOP_UDRD_D&> m_JFOP;
};

#endif