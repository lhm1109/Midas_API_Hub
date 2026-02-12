#ifndef __DB_GRUPU_DB_H__
#define __DB_GRUPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GRUPU
{
public:
	CDB_GRUPU()
	{
		m_grup.InitHashTable(HASHSIZEGRUP);
		m_grupitem.InitHashTable(HASHSIZEGRUPITEM);
	}
	virtual ~CDB_GRUPU(){};

public:
	void Add(T_UDRD_KEY Key,T_GRUP_UDRD_D& rData)
		{m_grup.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_grup.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_GRUP_UDRD_D& rData)
		{return m_grup.Lookup(Key,rData);}
	int GetCount()
		{return m_grup.GetCount();}
	POSITION GetStart()
		{return m_grup.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GRUP_UDRD_D& rData)
		{m_grup.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_GRUP_ITEM_UDRD_D& rData)
		{m_grupitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_grupitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_GRUP_ITEM_UDRD_D& rData)
		{return m_grupitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_grupitem.GetCount();}
	POSITION GetStartItem()
		{return m_grupitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GRUP_ITEM_UDRD_D& rData)
		{m_grupitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_grup.RemoveAll();
			m_grupitem.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GRUP_UDRD_D,T_GRUP_UDRD_D&>m_grup;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GRUP_ITEM_UDRD_D,T_GRUP_ITEM_UDRD_D&>m_grupitem;
};

#endif
