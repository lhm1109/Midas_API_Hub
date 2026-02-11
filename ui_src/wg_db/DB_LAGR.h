#ifndef __DB_LAGR_DB_H__
#define __DB_LAGR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
#include "DB_LDGR.h"

class CDB_ELEM;
class CDBDoc;

class CDB_LAGR
{
public:
	CDB_LAGR();
	virtual ~CDB_LAGR();
	CDBDoc* m_pDoc;

public:
	T_LAGR_K m_nLastNum;
	T_LAGR_K m_nStartNum;

public:
	void Add(T_LAGR_K Key,T_LAGR_D& rData);
	BOOL Del(T_LAGR_K Key);

public:
	BOOL Get(T_LAGR_K Key,T_LAGR_D& rData)
		{return m_lagr.Lookup(Key,rData);}
	int GetCount()
		{return m_lagr.GetCount();}
	POSITION GetStart()
		{return m_lagr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LAGR_K& rKey,T_LAGR_D& rData)
		{m_lagr.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_LAGR_K Key);
	BOOL DelList(T_LAGR_K Key);
	void DelAllList();
	BOOL GetList(T_LAGR_K Key,LagrList*& rpList);
	BOOL GetListFromBaseDB(T_LAGR_K Key,LagrList*& rpList){return m_lagrlist.Lookup(Key,rpList);}
	int GetCountList(){return m_lagrlist.GetCount();}
	void AddListItem(T_LAGR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_LAGR_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_LAGR_K,T_LAGR_K,T_LAGR_D,T_LAGR_D&>m_lagr;
	CMap<T_LAGR_K,T_LAGR_K,LagrList*,LagrList*>m_lagrlist;
};

#endif
