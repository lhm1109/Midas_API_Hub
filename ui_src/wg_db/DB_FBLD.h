#ifndef __DB_FBLD_DB_H__
#define __DB_FBLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_FBLD
{
public:
	CDB_FBLD();
	virtual ~CDB_FBLD();
	CDBDoc* m_pDoc;

public:
	T_FBLD_K m_nStartNum;
	T_FBLD_K m_nLastNum;

public:
	void Add(T_FBLD_K Key,T_FBLD_D& rData,CDB_STLD* pStld);
	BOOL Del(T_FBLD_K Key,CDB_STLD* pStld);

public:
	BOOL GetListFromBaseDB(T_FBLD_K Key,FbldList*& rpList){return m_fbldlist.Lookup(Key,rpList);}

	BOOL Get(T_FBLD_K Key,T_FBLD_D& rData)
		{return m_fbld.Lookup(Key,rData);}
	int GetCount()
		{return m_fbld.GetCount();}
	POSITION GetStart()
		{return m_fbld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FBLD_K& rKey,T_FBLD_D& rData)
		{m_fbld.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_FBLD_K Key);
	BOOL DelList(T_FBLD_K Key);
	void DelAllList();
	BOOL GetList(T_FBLD_K Key,FbldList*& rpList);//{return m_fbldlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_fbldlist.GetCount();}
	void AddListItem(T_FBLD_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_FBLD_K Key, int nCmd, int nKey);

protected:
	CMap<T_FBLD_K,T_FBLD_K,T_FBLD_D,T_FBLD_D&>m_fbld;
	CMap<T_FBLD_K,T_FBLD_K,FbldList*,FbldList*>m_fbldlist;
public:
	CMap<T_FBLD_K,T_FBLD_K,T_COLOR_D,T_COLOR_D&>m_fbldcolor;
};

#endif
