#ifndef __DB_STAGU_DB_H__
#define __DB_STAGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STAGU
{
public:
	CDB_STAGU()
	{
		m_stag.InitHashTable(HASHSIZESTAG);
		m_stagitem.InitHashTable(HASHSIZESTAGITEM);
	}
	virtual ~CDB_STAGU(){};

public:
	void Add(T_UDRD_KEY Key,T_STAG_UDRD_D& rData)
		{m_stag.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stag.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_STAG_UDRD_D& rData)
		{return m_stag.Lookup(Key,rData);}
	int GetCount()
		{return m_stag.GetCount();}
	POSITION GetStart()
		{return m_stag.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STAG_UDRD_D& rData)
		{m_stag.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_STAG_ITEM_UDRD_D& rData)
		{m_stagitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_stagitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_STAG_ITEM_UDRD_D& rData)
		{return m_stagitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_stagitem.GetCount();}
	POSITION GetStartItem()
		{return m_stagitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STAG_ITEM_UDRD_D& rData)
		{m_stagitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_stag.RemoveAll();
			m_stagitem.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STAG_UDRD_D,T_STAG_UDRD_D&>m_stag;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STAG_ITEM_UDRD_D,T_STAG_ITEM_UDRD_D&>m_stagitem;
};

class CDB_STAG_CHGU
{
public:
	CDB_STAG_CHGU()
	{
		m_stag_chg.InitHashTable(HASHSIZESTAG);
	}
	virtual ~CDB_STAG_CHGU(){};

public:
	void Add(T_UDRD_KEY Key,T_STAG_K& rData)
		{m_stag_chg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stag_chg.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_STAG_K& rData)
		{return m_stag_chg.Lookup(Key,rData);}
	int GetCount()
		{return m_stag_chg.GetCount();}
	POSITION GetStart()
		{return m_stag_chg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STAG_K& rData)
		{m_stag_chg.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_stag_chg.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STAG_K,T_STAG_K>m_stag_chg;
};

#endif
