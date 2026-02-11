#ifndef __DB_THCR_DB_H__
#define __DB_THCR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THCR
{
public:
	CDB_THCR();
	virtual ~CDB_THCR();
	CDBDoc* m_pDoc;

public:
	T_THCR_K m_nStartNum;
	T_THCR_K m_nLastNum;

public:
	void Add(T_THCR_K Key,T_THCR_D& rData);
	BOOL Del(T_THCR_K Key);

public:
	BOOL Get(T_THCR_K Key,T_THCR_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_THCR_K& rKey,T_THCR_D& rData);

	void AddList(T_THCR_K Key);
	BOOL DelList(T_THCR_K Key);
	void DelAllList();
	void AddListItem(T_THCR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_THCR_K Key, int nCmd, int nKey);
	BOOL GetList(T_THCR_K Key, DBDataList*& rpList);

protected:
	CMap<T_THCR_K,T_THCR_K,T_THCR_D,T_THCR_D&>m_thcr;
	CMap<T_THCR_K, T_THCR_K, DBDataList*, DBDataList*>m_thcrlist;
};

#endif