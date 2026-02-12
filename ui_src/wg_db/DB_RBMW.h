#ifndef __DB_RBMW_DB_H__
#define __DB_RBMW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RBMW
{
public:
	CDB_RBMW();
	CDB_RBMW(CDB_RBMW& src);
	virtual ~CDB_RBMW();
	CDB_RBMW& operator=(CDB_RBMW& src);
	CDBDoc* m_pDoc;

public:
	T_RBMW_K m_nStartNum;
	T_RBMW_K m_nLastNum;

public:
	void Add(T_RBMW_K Key,T_RBMW_D& rData);
	BOOL Del(T_RBMW_K Key);

public:
	BOOL Get(T_RBMW_K Key,T_RBMW_D& rData);
		//{return m_rbmw.Lookup(Key,rData);}
	int GetCount();
		//{return m_rbmw.GetCount();}
	POSITION GetStart();
		//{return m_rbmw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RBMW_K& rKey,T_RBMW_D& rData);
		//{m_rbmw.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_RBMW_K Key);
	BOOL DelList(T_RBMW_K Key);
	void DelAllList();
	BOOL GetList(T_RBMW_K Key,RbmwList*& rpList){return m_rbmwlist.Lookup(Key,rpList);}
	int GetCountList(){return m_rbmwlist.GetCount();}
	void AddListItem(T_RBMW_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_RBMW_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_RBMW_K,T_RBMW_K,T_RBMW_D,T_RBMW_D&>m_rbmw;
	CMap<T_RBMW_K,T_RBMW_K,RbmwList*,RbmwList*> m_rbmwlist;
};

#endif

