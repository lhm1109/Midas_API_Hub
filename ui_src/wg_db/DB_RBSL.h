#ifndef __DB_RBSL_DB_H__
#define __DB_RBSL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RBSL
{
public:
	CDB_RBSL();
	CDB_RBSL(CDB_RBSL& src);
	virtual ~CDB_RBSL();
	CDB_RBSL& operator=(CDB_RBSL& src);
	CDBDoc* m_pDoc;

public:
	T_RBSL_K m_nStartNum;
	T_RBSL_K m_nLastNum;

public:
	void Add(T_RBSL_K Key,T_RBSL_D& rData);
	BOOL Del(T_RBSL_K Key);

public:
	BOOL Get(T_RBSL_K Key,T_RBSL_D& rData);
		//{return m_rbsl.Lookup(Key,rData);}
	int GetCount();
		//{return m_rbsl.GetCount();}
	POSITION GetStart();
		//{return m_rbsl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RBSL_K& rKey,T_RBSL_D& rData);
		//{m_rbsl.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_RBSL_K Key);
	BOOL DelList(T_RBSL_K Key);
	void DelAllList();
	BOOL GetList(T_RBSL_K Key,RbslList*& rpList){return m_rbsllist.Lookup(Key,rpList);}
	int GetCountList(){return m_rbsllist.GetCount();}
	void AddListItem(T_RBSL_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_RBSL_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_RBSL_K,T_RBSL_K,T_RBSL_D,T_RBSL_D&>m_rbsl;
	CMap<T_RBSL_K,T_RBSL_K,RbslList*,RbslList*> m_rbsllist;
};

#endif

