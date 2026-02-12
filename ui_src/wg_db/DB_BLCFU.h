#ifndef __DB_BLCFU_DB_H__
#define __DB_BLCFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_BLCFU
{
public:
	CDB_BLCFU()
	{
		m_blcf.InitHashTable(HASHSIZEBLCF);    
		m_blcfitem.InitHashTable(101);
	}
	virtual ~CDB_BLCFU(){};

public:
	void Add(T_UDRD_KEY Key,T_BLCF_UDRD_D& rData)
		{m_blcf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_blcf.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_BLCF_UDRD_D& rData)
		{return m_blcf.Lookup(Key,rData);}
	int GetCount()
		{return m_blcf.GetCount();}
	POSITION GetStart()
		{return m_blcf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BLCF_UDRD_D& rData)
		{m_blcf.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_BLCF_ITEM_UDRD_D& rData)
		{m_blcfitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_blcfitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_BLCF_ITEM_UDRD_D& rData)
		{return m_blcfitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_blcfitem.GetCount();}
	POSITION GetStartItem()
		{return m_blcfitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BLCF_ITEM_UDRD_D& rData)
		{m_blcfitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_blcf.RemoveAll();      
			m_blcfitem.RemoveAll();
		}
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BLCF_UDRD_D,T_BLCF_UDRD_D&>m_blcf;   
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BLCF_ITEM_UDRD_D,T_BLCF_ITEM_UDRD_D&>m_blcfitem;
};

#endif
