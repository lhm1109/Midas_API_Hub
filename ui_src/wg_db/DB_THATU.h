#ifndef __DB_THATU_DB_H__
#define __DB_THATU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THATU
{
public:
	CDB_THATU()
	{
		m_that.InitHashTable(HASHSIZETHAT);
	}
	virtual ~CDB_THATU(){};

public:
	void Add(T_UDRD_KEY Key,T_THAT_UDRD_D& rData)
		{m_that.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_that.RemoveKey(Key);}
	void DelAll()
		{m_that.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THAT_UDRD_D& rData)
		{return m_that.Lookup(Key,rData);}
	int GetCount()
		{return m_that.GetCount();}
	POSITION GetStart()
		{return m_that.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THAT_UDRD_D& rData)
		{m_that.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THAT_UDRD_D,T_THAT_UDRD_D&>m_that;
};

#endif