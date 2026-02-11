#ifndef __DB_STOR_DB_H__
#define __DB_STOR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_STOR
{
public:
	CDB_STOR();
	virtual ~CDB_STOR();
	//CDBDoc* m_pDoc;

public:
	T_STOR_K m_nStartNum;
	T_STOR_K m_nLastNum;

public:
	void Add(T_STOR_K Key,T_STOR_D& rData);
	BOOL Del(T_STOR_K Key);

public:
	BOOL Get(T_STOR_K Key,T_STOR_D& rData)
		{return m_stor.Lookup(Key,rData);}
	int GetCount()
		{return m_stor.GetCount();}
	POSITION GetStart()
		{return m_stor.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STOR_K& rKey,T_STOR_D& rData)
		{m_stor.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_STOR_K Key);
	BOOL DelList(T_STOR_K Key);
	void DelAllList();
	BOOL GetList(T_STOR_K Key,StorList*& rpList){return m_storlist.Lookup(Key,rpList);}
	int GetCountList(){return m_storlist.GetCount();}
	void AddListItem(T_STOR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_STOR_K Key, int nCmd, int nKey);

protected:
	CMap<T_STOR_K,T_STOR_K,T_STOR_D,T_STOR_D&>m_stor;
	CMap<T_STOR_K,T_STOR_K,StorList*,StorList*>m_storlist;
};

#endif