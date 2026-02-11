#ifndef __DB_THIS_DB_H__
#define __DB_THIS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_THIS
{
public:
	CDB_THIS();
	virtual ~CDB_THIS();
	CDBDoc* m_pDoc;

public:
	T_THIS_K m_nStartNum;
	T_THIS_K m_nLastNum;

public:
	void Add(T_THIS_K Key,T_THIS_D& rData);
	BOOL Del(T_THIS_K Key);

public:
	BOOL Get(T_THIS_K Key,T_THIS_D& rData);
		//{return m_this.Lookup(Key,rData);}
	int GetCount();
		//{return m_this.GetCount();}
	POSITION GetStart();
		//{return m_this.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THIS_K& rKey,T_THIS_D& rData);
		//{m_this.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_THIS_K Key);
	BOOL DelList(T_THIS_K Key);
	void DelAllList();
	BOOL GetList(T_THIS_K Key,ThisList*& rpList);//{return m_thislist.Lookup(Key,rpList);}
	int GetCountList();//{return m_thislist.GetCount();}
	void AddListItem(T_THIS_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_THIS_K Key, int nCmd, int nKey);

protected:
	CMap<T_THIS_K,T_THIS_K,T_THIS_D,T_THIS_D&>m_this;
	CMap<T_THIS_K,T_THIS_K,ThisList*,ThisList*>m_thislist;
};

#endif
