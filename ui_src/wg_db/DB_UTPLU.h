#ifndef __DB_UTPLU_DB_H__
#define __DB_UTPLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UTPLU
{
public:
	CDB_UTPLU()
	{
		m_utpl.InitHashTable(HASHSIZEUTPL);
	}
	virtual ~CDB_UTPLU(){};

public:
	void Add(T_UDRD_KEY Key,T_UTPL_UDRD_D& rData)
		{m_utpl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_utpl.RemoveKey(Key);}
	void DelAll()
		{m_utpl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_UTPL_UDRD_D& rData)
		{return m_utpl.Lookup(Key,rData);}
	int GetCount()
		{return m_utpl.GetCount();}
	POSITION GetStart()
		{return m_utpl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UTPL_UDRD_D& rData)
		{m_utpl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_UTPL_UDRD_D,T_UTPL_UDRD_D&>m_utpl;
};

#endif