#ifndef __DB_JDSHU_DB_H__
#define __DB_JDSHU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDSHU
{
public:
	CDB_JDSHU()
	{
		m_JDSH.InitHashTable(HASHSIZEJDSH);
	}
	virtual ~CDB_JDSHU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDSH_UDRD_D& rData)
		{m_JDSH.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDSH.RemoveKey(Key);}
	void DelAll()
		{m_JDSH.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDSH_UDRD_D& rData)
		{return m_JDSH.Lookup(Key,rData);}
	int GetCount()
		{return m_JDSH.GetCount();}
	POSITION GetStart()
		{return m_JDSH.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_JDSH_UDRD_D& rData)
		{m_JDSH.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDSH_UDRD_D,T_JDSH_UDRD_D&>m_JDSH;
};

#endif
