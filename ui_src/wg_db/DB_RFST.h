#pragma once
#include "DB_ST_DT.h"

// ReinForce Sect

class CDBDoc;
class CDB_ELEM;

class CDB_RFST
{
public:
	CDB_RFST();
	CDB_RFST(CDB_RFST& src);
	CDB_RFST& operator=(CDB_RFST& src);
	virtual ~CDB_RFST();

public:
	T_RFST_K m_nLastNum;
	T_RFST_K m_nStartNum;

public:
	void Add(T_RFST_K Key, T_RFST_D& rData);
	BOOL Del(T_RFST_K Key);

public:
	BOOL Get(T_RFST_K Key, T_RFST_D& rData)
	{
		return m_rfst.Lookup(Key, rData);
	}
	int GetCount()
	{
		return (int)m_rfst.GetCount();
	}
	POSITION GetStart()
	{
		return m_rfst.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_RFST_K& rKey, T_RFST_D& rData)
	{
		m_rfst.GetNextAssoc(rNextPosition, rKey, rData);
	}

	void AddList(T_RFST_K Key);
	BOOL DelList(T_RFST_K Key);
	void DelAllList();
	BOOL GetList(T_RFST_K Key, SectList*& rpList) { return m_rfstlist.Lookup(Key, rpList); }
	int GetCountList() { return (int)m_rfstlist.GetCount(); }
	void AddListItem(T_RFST_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_RFST_K Key, int nCmd, int nKey);

protected:
	CMap<T_RFST_K, T_RFST_K, T_RFST_D, T_RFST_D&>m_rfst;
	CMap<T_RFST_K, T_RFST_K, RfstList*, RfstList*>m_rfstlist;
};

