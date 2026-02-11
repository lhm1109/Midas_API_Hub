#ifndef __DB_CGTEU_DB_H__
#define __DB_CGTEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGTEU
{
public:
	CDB_CGTEU()
	{
		m_cgte.InitHashTable(HASHSIZECGTE);
	}
	virtual ~CDB_CGTEU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGTE_UDRD_D& rData)
		{m_cgte.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgte.RemoveKey(Key);}
	void DelAll()
		{m_cgte.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGTE_UDRD_D& rData)
		{return m_cgte.Lookup(Key,rData);}
	int GetCount()
		{return m_cgte.GetCount();}
	POSITION GetStart()
		{return m_cgte.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGTE_UDRD_D& rData)
		{m_cgte.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGTE_UDRD_D,T_CGTE_UDRD_D&>m_cgte;
};

#endif