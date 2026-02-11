#ifndef __DB_WDPR_DB_H__
#define __DB_WDPR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LDGR;
class CDB_WVEP;
class CDB_LAPL;
class CDB_ELEM;
class CDB_NODE;
class CDB_WDFC;
class CDB_WDPR
{
public:
	CDB_WDPR();
	virtual ~CDB_WDPR();
	CDBDoc* m_pDoc;

public:
	T_WDPR_K m_nLastNum;
	T_WDPR_K m_nStartNum;

public:
	void Add(T_WDPR_K Key, T_WDPR_D& rData, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_WVEP* pWvep, CDB_WDFC* pWdfc, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode);
	BOOL Del(T_WDPR_K Key,                  CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_WVEP* pWvep, CDB_WDFC* pWdfc, CDB_LAPL* pLapl, CDB_ELEM* pElem, CDB_NODE* pNode);

public:
	BOOL Get(T_WDPR_K Key,T_WDPR_D& rData)
		{return m_wdpr.Lookup(Key,rData);}
	int GetCount()
		{return m_wdpr.GetCount();}
	POSITION GetStart()
		{return m_wdpr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WDPR_K& rKey,T_WDPR_D& rData)
		{m_wdpr.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_WDPR_K Key);
	BOOL DelList(T_WDPR_K Key);
	void DelAllList();
	BOOL GetList(T_WDPR_K Key,WdprList*& rpList);
	BOOL GetListFromBaseDB(T_WDPR_K Key,WdprList*& rpList){return m_wdprlist.Lookup(Key,rpList);}
	int GetCountList(){return m_wdprlist.GetCount();}
	void AddListItem(T_WDPR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_WDPR_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_WDPR_K,T_WDPR_K,T_WDPR_D,T_WDPR_D&>m_wdpr;
	CMap<T_WDPR_K,T_WDPR_K,WdprList*,WdprList*>m_wdprlist;
};

#endif
