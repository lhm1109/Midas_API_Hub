#ifndef __DB_RBMS_DB_H__
#define __DB_RBMS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RBMS
{
public:
	CDB_RBMS();
	CDB_RBMS(CDB_RBMS& src);
	virtual ~CDB_RBMS();
	CDB_RBMS& operator=(CDB_RBMS& src);
	CDBDoc* m_pDoc;

public:
	T_RBMS_K m_nStartNum;
	T_RBMS_K m_nLastNum;

public:
	void Add(T_RBMS_K Key,T_RBMS_D& rData);
	BOOL Del(T_RBMS_K Key);

public:
	BOOL Get(T_RBMS_K Key,T_RBMS_D& rData);
		//{return m_rbms.Lookup(Key,rData);}
	int GetCount();
		//{return m_rbms.GetCount();}
	POSITION GetStart();
		//{return m_rbms.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RBMS_K& rKey,T_RBMS_D& rData);
		//{m_rbms.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_RBMS_K Key);
	BOOL DelList(T_RBMS_K Key);
	void DelAllList();
	BOOL GetList(T_RBMS_K Key,RbmsList*& rpList){return m_rbmslist.Lookup(Key,rpList);}
	int GetCountList(){return m_rbmslist.GetCount();}
	void AddListItem(T_RBMS_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_RBMS_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_RBMS_K,T_RBMS_K,T_RBMS_D,T_RBMS_D&>m_rbms;
	CMap<T_RBMS_K,T_RBMS_K,RbmsList*,RbmsList*> m_rbmslist;
};

#endif

