#ifndef __DB_LAPLU_DB_H__
#define __DB_LAPLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LAPLU
{
public:
	CDB_LAPLU()
	{
		m_lapl.InitHashTable(HASHSIZELAPL);
		m_laplitem.InitHashTable(HASHSIZELAPLITEM);
	}
	virtual ~CDB_LAPLU(){};

public:
	void Add(T_UDRD_KEY Key,T_LAPL_UDRD_D& rData)
		{m_lapl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lapl.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_LAPL_UDRD_D& rData)
		{return m_lapl.Lookup(Key,rData);}
	int GetCount()
		{return m_lapl.GetCount();}
	POSITION GetStart()
		{return m_lapl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LAPL_UDRD_D& rData)
		{m_lapl.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_LAPL_ITEM_UDRD_D& rData)
		{m_laplitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_laplitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_LAPL_ITEM_UDRD_D& rData)
		{return m_laplitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_laplitem.GetCount();}
	POSITION GetStartItem()
		{return m_laplitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LAPL_ITEM_UDRD_D& rData)
		{m_laplitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_lapl.RemoveAll();
			m_laplitem.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LAPL_UDRD_D,T_LAPL_UDRD_D&>m_lapl;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LAPL_ITEM_UDRD_D,T_LAPL_ITEM_UDRD_D&>m_laplitem;
};

#endif
