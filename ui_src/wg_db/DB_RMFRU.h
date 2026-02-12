#ifndef __DB_RMFRU_DB_H__
#define __DB_RMFRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RMFRU
{
public:
	CDB_RMFRU()
	{
		m_rmfr.InitHashTable(HASHSIZERMFR);
	}
	virtual ~CDB_RMFRU(){};

public:
	void Add(T_UDRD_KEY Key,T_RMFR_UDRD_D& rData)
		{m_rmfr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rmfr.RemoveKey(Key);}
	void DelAll()
		{m_rmfr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RMFR_UDRD_D& rData)
		{return m_rmfr.Lookup(Key,rData);}
	int GetCount()
		{return m_rmfr.GetCount();}
	POSITION GetStart()
		{return m_rmfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RMFR_UDRD_D& rData)
		{m_rmfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RMFR_UDRD_D,T_RMFR_UDRD_D&>m_rmfr;
};

#endif