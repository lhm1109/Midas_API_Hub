#ifndef __DB_RIGDU_DB_H__
#define __DB_RIGDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RIGDU
{
public:
	CDB_RIGDU()
	{
		m_rigd.InitHashTable(HASHSIZERIGD);
		m_rigditem.InitHashTable(HASHSIZERIGDITEM);
	}
	virtual ~CDB_RIGDU(){};

public:
	void Add(T_UDRD_KEY Key,T_RIGD_UDRD_D& rData)
		{m_rigd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rigd.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_RIGD_UDRD_D& rData)
		{return m_rigd.Lookup(Key,rData);}
	int GetCount()
		{return m_rigd.GetCount();}
	POSITION GetStart()
		{return m_rigd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RIGD_UDRD_D& rData)
		{m_rigd.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_RIGD_ITEM_UDRD_D& rData)
		{m_rigditem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_rigditem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_RIGD_ITEM_UDRD_D& rData)
		{return m_rigditem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_rigditem.GetCount();}
	POSITION GetStartItem()
		{return m_rigditem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RIGD_ITEM_UDRD_D& rData)
		{m_rigditem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_rigd.RemoveAll();
			m_rigditem.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RIGD_UDRD_D,T_RIGD_UDRD_D&>m_rigd;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RIGD_ITEM_UDRD_D,T_RIGD_ITEM_UDRD_D&>m_rigditem;
};

#endif
