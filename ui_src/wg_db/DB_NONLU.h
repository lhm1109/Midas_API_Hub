#ifndef __DB_NONLU_DB_H__
#define __DB_NONLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NONLU
{
public:
	CDB_NONLU()
	{
		m_nonl.InitHashTable(HASHSIZENONL);
	}
	virtual ~CDB_NONLU(){};

public:
	void Add(T_UDRD_KEY Key,T_NONL_UDRD_D& rData)
		{m_nonl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nonl.RemoveKey(Key);}
	void DelAll()
		{m_nonl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NONL_UDRD_D& rData)
		{return m_nonl.Lookup(Key,rData);}
	int GetCount()
		{return m_nonl.GetCount();}
	POSITION GetStart()
		{return m_nonl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NONL_UDRD_D& rData)
		{m_nonl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NONL_UDRD_D,T_NONL_UDRD_D&>m_nonl;
};

#endif
