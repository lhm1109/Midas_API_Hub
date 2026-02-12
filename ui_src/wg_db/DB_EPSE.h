#ifndef __DB_EPSE_DB_H__
#define __DB_EPSE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LDGR;
class CDB_POSP;
class CDB_LAPL;
class CDB_ELEM;
class CDB_NODE;

class CDB_EPSE
{
public:
	CDB_EPSE();
	virtual ~CDB_EPSE();
	CDBDoc* m_pDoc;

public:
	T_EPSE_K m_nLastNum;
	T_EPSE_K m_nStartNum;

public:
	void Add(T_EPSE_K Key, T_EPSE_D& rData, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode);
	BOOL Del(T_EPSE_K Key, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode);

public:
	BOOL Get(T_EPSE_K Key, T_EPSE_D& rData)
	{
		return m_epse.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_epse.GetCount();
	}
	POSITION GetStart()
	{
		return m_epse.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_EPSE_K& rKey, T_EPSE_D& rData)
	{
		m_epse.GetNextAssoc(rNextPosition, rKey, rData);
	}

	void AddList(T_EPSE_K Key);
	BOOL DelList(T_EPSE_K Key);
	void DelAllList();
	BOOL GetList(T_EPSE_K Key, EpseList*& rpList);
	BOOL GetListFromBaseDB(T_EPSE_K Key, EpseList*& rpList) { return m_epselist.Lookup(Key, rpList); }
	int GetCountList() { return m_epselist.GetCount(); }
	void AddListItem(T_EPSE_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_EPSE_K Key, int nCmd, int nKey);

protected:
	CMap<T_EPSE_K, T_EPSE_K, T_EPSE_D, T_EPSE_D&>m_epse;
	CMap<T_EPSE_K, T_EPSE_K, EpseList*, EpseList*>m_epselist;
};

#endif
