#ifndef __DB_RPRSU_DB_H__
#define __DB_RPRSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RPRSU
{
public:
	CDB_RPRSU()
	{
		m_rprs.InitHashTable(HASHSIZERPRS);
	}
	virtual ~CDB_RPRSU(){};

public:
	void Add(T_UDRD_KEY Key,T_RPRS_UDRD_D& rData)
		{m_rprs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rprs.RemoveKey(Key);}
	void DelAll()
		{m_rprs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RPRS_UDRD_D& rData)
		{return m_rprs.Lookup(Key,rData);}
	int GetCount()
		{return m_rprs.GetCount();}
	POSITION GetStart()
		{return m_rprs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RPRS_UDRD_D& rData)
		{m_rprs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPRS_UDRD_D,T_RPRS_UDRD_D&>m_rprs;
};

#endif