#ifndef __DB_BNGRU_DB_H__
#define __DB_BNGRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BNGRU
{
public:
	CDB_BNGRU()
	{
		m_bngr.InitHashTable(HASHSIZEBNGR);
	}
	virtual ~CDB_BNGRU(){};

public:
	void Add(T_UDRD_KEY Key,T_BNGR_UDRD_D& rData)
		{m_bngr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bngr.RemoveKey(Key);}
	void DelAll()
		{m_bngr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BNGR_UDRD_D& rData)
		{return m_bngr.Lookup(Key,rData);}
	int GetCount()
		{return m_bngr.GetCount();}
	POSITION GetStart()
		{return m_bngr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BNGR_UDRD_D& rData)
		{m_bngr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BNGR_UDRD_D,T_BNGR_UDRD_D&>m_bngr;
};

#endif