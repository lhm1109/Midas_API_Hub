#ifndef __DB_SIGR_DB_H__
#define __DB_SIGR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SIGR
{
public:
	CDB_SIGR();
	virtual ~CDB_SIGR();
	CDBDoc* m_pDoc;

public:
	T_SIGR_K m_nStartNum;
	T_SIGR_K m_nLastNum;

public:
	void Add(T_SIGR_K Key, T_SIGR_D& rData);
	BOOL Del(T_SIGR_K Key);

public:
	BOOL Get(T_SIGR_K Key, T_SIGR_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_K& rKey, T_SIGR_D& rData);

	void AddList(T_SIGR_K Key);
	BOOL DelList(T_SIGR_K Key);
	void DelAllList();
	void AddListItem(T_SIGR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SIGR_K Key, int nCmd, int nKey);
	BOOL GetList(T_SIGR_K Key, DBDataList*& rpList);

protected:
	CMap<T_SIGR_K, T_SIGR_K, T_SIGR_D, T_SIGR_D&>m_sigr;
	CMap<T_SIGR_K, T_SIGR_K, DBDataList*, DBDataList*>m_sigrlist;
};

#endif