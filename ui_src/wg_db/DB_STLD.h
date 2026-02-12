#ifndef __DB_STLD_DB_H__
#define __DB_STLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD
{
public:
	CDB_STLD();
	virtual ~CDB_STLD();
	CDBDoc* m_pDoc;

public:
	T_STLD_K m_nStartNum;
	T_STLD_K m_nLastNum;

public:
	void Add(T_STLD_K Key,T_STLD_D& rData);
	BOOL Del(T_STLD_K Key);
	void Initialize();

public:
	BOOL GetListFromBaseDB(T_STLD_K Key,StldList*& rpList){return m_stldlist.Lookup(Key,rpList);}

	BOOL Get(T_STLD_K Key,T_STLD_D& rData)
		{return m_stld.Lookup(Key,rData);}
	int GetCount()
		{return (int)m_stld.GetCount();}
	POSITION GetStart()
		{return m_stld.GetStartPosition();}

	void GetNext(POSITION& rNextPosition,T_STLD_K& rKey,T_STLD_D& rData)
		{m_stld.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_STLD_K Key);
	BOOL DelList(T_STLD_K Key);
	void DelAllList();
	BOOL GetList(T_STLD_K Key,StldList*& rpList);//{return m_stldlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_stldlist.GetCount();}
	//int GetCountListFromBaseDB();//{return m_stldlist.GetCount();}
	void AddListItem(T_STLD_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_STLD_K Key, int nCmd, int nKey);

protected:
	CMap<T_STLD_K,T_STLD_K,T_STLD_D,T_STLD_D&>m_stld;
	CMap<T_STLD_K,T_STLD_K,StldList*,StldList*>m_stldlist;
};

#endif
