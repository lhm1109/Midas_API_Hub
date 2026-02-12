#ifndef __DB_VSECU_DB_H__
#define __DB_VSECU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_VSECU
{
public:
	CDB_VSECU()
	{
		m_vsec.InitHashTable(HASHSIZEVSEC);
		m_vsecitem.InitHashTable(HASHSIZEVSECITEM);
	}
	virtual ~CDB_VSECU(){};

public:
	void Add(T_UDRD_KEY Key,T_VSEC_UDRD_D& rData)
		{m_vsec.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_vsec.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_VSEC_UDRD_D& rData)
		{return m_vsec.Lookup(Key,rData);}
	int GetCount()
		{return m_vsec.GetCount();}
	POSITION GetStart()
		{return m_vsec.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_VSEC_UDRD_D& rData)
		{m_vsec.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_VSEC_ITEM_UDRD_D& rData)
		{m_vsecitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_vsecitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_VSEC_ITEM_UDRD_D& rData)
		{return m_vsecitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_vsecitem.GetCount();}
	POSITION GetStartItem()
		{return m_vsecitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_VSEC_ITEM_UDRD_D& rData)
		{m_vsecitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_vsec.RemoveAll();
			m_vsecitem.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_VSEC_UDRD_D,T_VSEC_UDRD_D&>m_vsec;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_VSEC_ITEM_UDRD_D,T_VSEC_ITEM_UDRD_D&>m_vsecitem;
};

#endif
