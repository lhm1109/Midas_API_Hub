#ifndef __DB_GUMBU_DB_H__
#define __DB_GUMBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUMBU
{
public:
	CDB_GUMBU()
	{
		m_gumb.InitHashTable(HASHSIZEGUMB);
	}
	virtual ~CDB_GUMBU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUMB_UDRD_D& rData)
		{m_gumb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gumb.RemoveKey(Key);}
	void DelAll()
		{m_gumb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUMB_UDRD_D& rData)
		{return m_gumb.Lookup(Key,rData);}
	int GetCount()
		{return m_gumb.GetCount();}
	POSITION GetStart()
		{return m_gumb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUMB_UDRD_D& rData)
		{m_gumb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUMB_UDRD_D,T_GUMB_UDRD_D&>m_gumb;
};

#endif