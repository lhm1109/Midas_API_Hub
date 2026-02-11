#ifndef __DB_NBOFU_DB_H__
#define __DB_NBOFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NBOFU
{
public:
	CDB_NBOFU()
	{
		m_nbof.InitHashTable(HASHSIZENBOF);
		m_nbofitem.InitHashTable(HASHSIZENBOF);
	}
	virtual ~CDB_NBOFU(){};

public:
	void Add(T_UDRD_KEY Key,T_NBOF_UDRD_D& rData)
		{m_nbof.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nbof.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_NBOF_UDRD_D& rData)
		{return m_nbof.Lookup(Key,rData);}
	int GetCount()
		{return m_nbof.GetCount();}
	POSITION GetStart()
		{return m_nbof.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NBOF_UDRD_D& rData)
		{m_nbof.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_NBOF_ITEM_UDRD_D& rData)
		{m_nbofitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_nbofitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_NBOF_ITEM_UDRD_D& rData)
		{return m_nbofitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_nbofitem.GetCount();}
	POSITION GetStartItem()
		{return m_nbofitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NBOF_ITEM_UDRD_D& rData)
		{m_nbofitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
	{
		m_nbof.RemoveAll();
		m_nbofitem.RemoveAll();
	}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NBOF_UDRD_D,T_NBOF_UDRD_D&>m_nbof;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NBOF_ITEM_UDRD_D,T_NBOF_ITEM_UDRD_D&>m_nbofitem;
};

#endif