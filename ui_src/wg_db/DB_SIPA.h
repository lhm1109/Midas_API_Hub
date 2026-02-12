#ifndef __DB_SIPA_DB_H__
#define __DB_SIPA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_SIPA
{
public:
	CDB_SIPA();
	virtual ~CDB_SIPA();
	CDBDoc* m_pDoc;

public:
	T_SIPA_K m_nStartNum;
	T_SIPA_K m_nLastNum;

public:
	void Add(T_SIPA_K Key, T_SIPA_D& rData, CDB_THIS* pThis);
	BOOL Del(T_SIPA_K Key, CDB_THIS* pThis);

public:
	BOOL Get(T_SIPA_K Key, T_SIPA_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIPA_K& rKey, T_SIPA_D& rData);

	void AddList(T_SIPA_K Key);
	BOOL DelList(T_SIPA_K Key);
	void DelAllList();
	void AddListItem(T_SIPA_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SIPA_K Key, int nCmd, int nKey);
	BOOL GetList(T_SIPA_K Key, DBDataList*& rpList);

protected:
	CMap<T_SIPA_K, T_SIPA_K, T_SIPA_D, T_SIPA_D&>m_sipa;
	CMap<T_SIPA_K, T_SIPA_K, DBDataList*, DBDataList*>m_sipalist;
};



#endif