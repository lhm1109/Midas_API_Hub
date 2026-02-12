#ifndef __DB_LISTU_DB_H__
#define __DB_LISTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LISTU
{
public:
	CDB_LISTU()
	{
		m_list.InitHashTable(HASHSIZELISTCLASS);
	}
	virtual ~CDB_LISTU(){};

public:
	void Add(T_UDRD_KEY Key,T_LIST_UDRD_D& rData)
		{m_list.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_list.RemoveKey(Key);}
	void DelAll()
		{m_list.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LIST_UDRD_D& rData)
		{return m_list.Lookup(Key,rData);}
	int GetCount()
		{return m_list.GetCount();}
	POSITION GetStart()
		{return m_list.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LIST_UDRD_D& rData)
		{m_list.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LIST_UDRD_D,T_LIST_UDRD_D&>m_list;
};

#endif