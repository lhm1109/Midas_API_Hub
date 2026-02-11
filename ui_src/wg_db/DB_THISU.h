#ifndef __DB_THISU_DB_H__
#define __DB_THISU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THISU
{
public:
	CDB_THISU()
	{
		m_this.InitHashTable(HASHSIZETHIS);
	}
	virtual ~CDB_THISU(){};

public:
	void Add(T_UDRD_KEY Key,T_THIS_UDRD_D& rData)
		{m_this.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_this.RemoveKey(Key);}
	void DelAll()
		{m_this.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THIS_UDRD_D& rData)
		{return m_this.Lookup(Key,rData);}
	int GetCount()
		{return m_this.GetCount();}
	POSITION GetStart()
		{return m_this.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THIS_UDRD_D& rData)
		{m_this.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THIS_UDRD_D,T_THIS_UDRD_D&>m_this;
};

#endif