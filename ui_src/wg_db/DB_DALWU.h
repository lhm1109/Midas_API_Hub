#ifndef __DB_DALWU_DB_H__
#define __DB_DALWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DALWU
{
public:
	CDB_DALWU()
	{
		m_dalw.InitHashTable(HASHSIZEDALW);
	}
	virtual ~CDB_DALWU(){};

public:
	void Add(T_UDRD_KEY Key,T_DALW_UDRD_D& rData)
		{m_dalw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dalw.RemoveKey(Key);}
	void DelAll()
		{m_dalw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DALW_UDRD_D& rData)
		{return m_dalw.Lookup(Key,rData);}
	int GetCount()
		{return m_dalw.GetCount();}
	POSITION GetStart()
		{return m_dalw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DALW_UDRD_D& rData)
		{m_dalw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DALW_UDRD_D,T_DALW_UDRD_D&>m_dalw;
};

#endif