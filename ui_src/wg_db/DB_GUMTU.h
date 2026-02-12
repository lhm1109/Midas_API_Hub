#ifndef __DB_GUMTU_DB_H__
#define __DB_GUMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUMTU
{
public:
	CDB_GUMTU()
	{
		m_gumt.InitHashTable(HASHSIZEGUMT);
	}
	virtual ~CDB_GUMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUMT_UDRD_D& rData)
		{m_gumt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gumt.RemoveKey(Key);}
	void DelAll()
		{m_gumt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUMT_UDRD_D& rData)
		{return m_gumt.Lookup(Key,rData);}
	int GetCount()
		{return m_gumt.GetCount();}
	POSITION GetStart()
		{return m_gumt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUMT_UDRD_D& rData)
		{m_gumt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUMT_UDRD_D,T_GUMT_UDRD_D&>m_gumt;
};

#endif