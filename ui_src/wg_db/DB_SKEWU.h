#ifndef __DB_SKEWU_DB_H__
#define __DB_SKEWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SKEWU
{
public:
	CDB_SKEWU()
	{
		m_skew.InitHashTable(HASHSIZESKEW);
	}
	virtual ~CDB_SKEWU(){};

public:
	void Add(T_UDRD_KEY Key,T_SKEW_UDRD_D& rData)
		{m_skew.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_skew.RemoveKey(Key);}
	void DelAll()
		{m_skew.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SKEW_UDRD_D& rData)
		{return m_skew.Lookup(Key,rData);}
	int GetCount()
		{return m_skew.GetCount();}
	POSITION GetStart()
		{return m_skew.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SKEW_UDRD_D& rData)
		{m_skew.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SKEW_UDRD_D,T_SKEW_UDRD_D&>m_skew;
};

#endif