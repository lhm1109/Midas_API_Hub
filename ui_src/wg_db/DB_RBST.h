#ifndef __DB_RBST_DB_H__
#define __DB_RBST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RBST
{
public:
	CDB_RBST();
	CDB_RBST(CDB_RBST& src);
	virtual ~CDB_RBST();
	CDB_RBST& operator=(CDB_RBST& src);
	CDBDoc* m_pDoc;

public:
	T_RBST_K m_nStartNum;
	T_RBST_K m_nLastNum;

public:
	void Add(T_RBST_K Key,T_RBST_D& rData);
	BOOL Del(T_RBST_K Key);

public:
	BOOL Get(T_RBST_K Key,T_RBST_D& rData);
		//{return m_rbst.Lookup(Key,rData);}
	int GetCount();
		//{return m_rbst.GetCount();}
	POSITION GetStart();
		//{return m_rbst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RBST_K& rKey,T_RBST_D& rData);
		//{m_rbst.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_RBST_K Key);
	BOOL DelList(T_RBST_K Key);
	void DelAllList();
	BOOL GetList(T_RBST_K Key,RbstList*& rpList){return m_rbstlist.Lookup(Key,rpList);}
	int GetCountList(){return m_rbstlist.GetCount();}
	void AddListItem(T_RBST_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_RBST_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_RBST_K,T_RBST_K,T_RBST_D,T_RBST_D&>m_rbst;
	CMap<T_RBST_K,T_RBST_K,RbstList*,RbstList*> m_rbstlist;
};

#endif

