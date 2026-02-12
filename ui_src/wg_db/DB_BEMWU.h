#ifndef __DB_BEMWU_DB_H__
#define __DB_BEMWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BEMWU
{
public:
	CDB_BEMWU()
	{
		m_bemw.InitHashTable(HASHSIZEBEMW);
	}
	virtual ~CDB_BEMWU(){};

public:
	void Add(T_UDRD_KEY Key,T_BEMW_UDRD_D& rData)
		{m_bemw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bemw.RemoveKey(Key);}
	void DelAll()
		{m_bemw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BEMW_UDRD_D& rData)
		{return m_bemw.Lookup(Key,rData);}
	int GetCount()
		{return m_bemw.GetCount();}
	POSITION GetStart()
		{return m_bemw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BEMW_UDRD_D& rData)
		{m_bemw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BEMW_UDRD_D,T_BEMW_UDRD_D&>m_bemw;
};

#endif