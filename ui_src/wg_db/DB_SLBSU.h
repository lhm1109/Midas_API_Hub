#ifndef __DB_SLBSU_DB_H__
#define __DB_SLBSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SLBSU
{
public:
	CDB_SLBSU()
	{
		m_slbs.InitHashTable(HASHSIZESLBS);
		m_slbsitem.InitHashTable(HASHSIZESLBSITEM);
	}
	virtual ~CDB_SLBSU(){};

public:
	void Add(T_UDRD_KEY Key,T_SLBS_UDRD_D& rData)
		{m_slbs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_slbs.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_SLBS_UDRD_D& rData)
		{return m_slbs.Lookup(Key,rData);}
	int GetCount()
		{return m_slbs.GetCount();}
	POSITION GetStart()
		{return m_slbs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLBS_UDRD_D& rData)
		{m_slbs.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_SLBS_ITEM_UDRD_D& rData)
		{m_slbsitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_slbsitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_SLBS_ITEM_UDRD_D& rData)
		{return m_slbsitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_slbsitem.GetCount();}
	POSITION GetStartItem()
		{return m_slbsitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLBS_ITEM_UDRD_D& rData)
		{m_slbsitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_slbs.RemoveAll();
			m_slbsitem.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLBS_UDRD_D,T_SLBS_UDRD_D&>m_slbs;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLBS_ITEM_UDRD_D,T_SLBS_ITEM_UDRD_D&>m_slbsitem;
};

#endif
