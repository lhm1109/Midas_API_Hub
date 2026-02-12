#ifndef __DB_HSTGU_DB_H__
#define __DB_HSTGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HSTGU
{
public:
	CDB_HSTGU()
	{
		m_hstg.InitHashTable(HASHSIZEHSTG);
	}
	virtual ~CDB_HSTGU(){};

public:
	void Add(T_UDRD_KEY Key,T_HSTG_UDRD_D& rData)
		{m_hstg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hstg.RemoveKey(Key);}
	void DelAll()
		{m_hstg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HSTG_UDRD_D& rData)
		{return m_hstg.Lookup(Key,rData);}
	int GetCount()
		{return m_hstg.GetCount();}
	POSITION GetStart()
		{return m_hstg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HSTG_UDRD_D& rData)
		{m_hstg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HSTG_UDRD_D,T_HSTG_UDRD_D&>m_hstg;
};

#endif
