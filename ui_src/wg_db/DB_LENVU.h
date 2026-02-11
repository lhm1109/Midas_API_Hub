#ifndef __DB_LENVU_DB_H__
#define __DB_LENVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LENVU
{
public:
	CDB_LENVU()
	{
		m_lenv.InitHashTable(HASHSIZELENV);
	}
	virtual ~CDB_LENVU(){};

public:
	void Add(T_UDRD_KEY Key,T_LENV_UDRD_D& rData)
		{m_lenv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lenv.RemoveKey(Key);}
	void DelAll()
		{m_lenv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LENV_UDRD_D& rData)
		{return m_lenv.Lookup(Key,rData);}
	int GetCount()
		{return m_lenv.GetCount();}
	POSITION GetStart()
		{return m_lenv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LENV_UDRD_D& rData)
		{m_lenv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LENV_UDRD_D,T_LENV_UDRD_D&>m_lenv;
};

#endif