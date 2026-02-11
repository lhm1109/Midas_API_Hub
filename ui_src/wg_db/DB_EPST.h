#ifndef __DB_EPST_DB_H__
#define __DB_EPST_DB_H__

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

class CDB_EPST
{
public:
	CDB_EPST();
	virtual ~CDB_EPST();
	CDBDoc* m_pDoc;

public:
	T_EPST_K m_nLastNum;
	T_EPST_K m_nStartNum;

public:
	void Add(T_EPST_K Key, T_EPST_D& rData, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode);
	BOOL Del(T_EPST_K Key, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode);

public:
	BOOL Get(T_EPST_K Key, T_EPST_D& rData)
	{
		return m_epst.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_epst.GetCount();
	}
	POSITION GetStart()
	{
		return m_epst.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_EPST_K& rKey, T_EPST_D& rData)
	{
		m_epst.GetNextAssoc(rNextPosition, rKey, rData);
	}

	void AddList(T_EPST_K Key);
	BOOL DelList(T_EPST_K Key);
	void DelAllList();
	BOOL GetList(T_EPST_K Key, EpstList*& rpList);
	BOOL GetListFromBaseDB(T_EPST_K Key, EpstList*& rpList) { return m_epstlist.Lookup(Key, rpList); }
	int GetCountList() { return m_epstlist.GetCount(); }
	void AddListItem(T_EPST_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_EPST_K Key, int nCmd, int nKey);

protected:
	CMap<T_EPST_K, T_EPST_K, T_EPST_D, T_EPST_D&>m_epst;
	CMap<T_EPST_K, T_EPST_K, EpstList*, EpstList*>m_epstlist;
};

#endif
