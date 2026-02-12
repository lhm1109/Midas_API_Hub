#ifndef __DB_P1LT_DB_H__
#define __DB_P1LT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_P1LT
{
public:
	CDB_P1LT();
	virtual ~CDB_P1LT();
	CDBDoc* m_pDoc;

public:
	T_P1LT_K m_nStartNum;
	T_P1LT_K m_nLastNum;

public:
	void Add(T_P1LT_K Key,T_P1LT_D& rData);
	BOOL Del(T_P1LT_K Key);

public:
	BOOL GetListFromBaseDB(T_P1LT_K Key,P1ltList*& rpList){return m_p1ltlist.Lookup(Key,rpList);}

	/*
	BOOL Get(T_P1LT_K Key,T_P1LT_D& rData)
		{return m_p1lt.Lookup(Key,rData);}
	int GetCount()
		{return m_p1lt.GetCount();}
	POSITION GetStart()
		{return m_p1lt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_P1LT_K& rKey,T_P1LT_D& rData)
		{m_p1lt.GetNextAssoc(rNextPosition,rKey,rData);}
	*/
	BOOL Get(T_P1LT_K Key,T_P1LT_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_P1LT_K& rKey,T_P1LT_D& rData);

	void AddList(T_P1LT_K Key);
	BOOL DelList(T_P1LT_K Key);
	void DelAllList();
	BOOL GetList(T_P1LT_K Key,P1ltList*& rpList);//{return m_p1ltlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_p1ltlist.GetCount();}
	void AddListItem(T_P1LT_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_P1LT_K Key, int nCmd, int nKey);

protected:
	CMap<T_P1LT_K,T_P1LT_K,T_P1LT_D,T_P1LT_D&>m_p1lt;
	CMap<T_P1LT_K,T_P1LT_K,P1ltList*,P1ltList*>m_p1ltlist;
};

#endif
