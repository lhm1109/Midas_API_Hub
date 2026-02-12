#ifndef __DB_MCONU_DB_H__
#define __DB_MCONU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MCONU
{
public:
	CDB_MCONU()
	{
		m_mcon.InitHashTable(HASHSIZEMCON);
		m_mconitem.InitHashTable(HASHSIZEMCONITEM);
	}
	virtual ~CDB_MCONU(){};

public:
	void Add(T_UDRD_KEY Key,T_MCON_UDRD_D& rData)
		{m_mcon.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mcon.RemoveKey(Key);}
	BOOL Get(T_UDRD_KEY Key,T_MCON_UDRD_D& rData)
		{return m_mcon.Lookup(Key,rData);}
	int GetCount()
		{return m_mcon.GetCount();}
	POSITION GetStart()
		{return m_mcon.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MCON_UDRD_D& rData)
		{m_mcon.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddItem(T_UDRD_KEY Key,T_MCON_ITEM_UDRD_D& rData)
		{m_mconitem.SetAt(Key,rData);}
	BOOL DelItem(T_UDRD_KEY Key)
		{return m_mconitem.RemoveKey(Key);}
	BOOL GetItem(T_UDRD_KEY Key,T_MCON_ITEM_UDRD_D& rData)
		{return m_mconitem.Lookup(Key,rData);}
	int GetCountItem()
		{return m_mconitem.GetCount();}
	POSITION GetStartItem()
		{return m_mconitem.GetStartPosition();}
	void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MCON_ITEM_UDRD_D& rData)
		{m_mconitem.GetNextAssoc(rNextPosition,rKey,rData);}

	void DelAll()
		{
			m_mcon.RemoveAll();
			m_mconitem.RemoveAll();
		}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MCON_UDRD_D,T_MCON_UDRD_D&>m_mcon;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MCON_ITEM_UDRD_D,T_MCON_ITEM_UDRD_D&>m_mconitem;
};

#endif
