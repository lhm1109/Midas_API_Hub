#ifndef __DB_SECT_SSM_DB_H__
#define __DB_SECT_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SECT_SSM
{
public:
	CDB_SECT_SSM();
	virtual ~CDB_SECT_SSM();
	CDBDoc* m_pDoc;

public:
	T_SECT_SSM_K m_nStartNum;
	T_SECT_SSM_K m_nLastNum;

public:
	void Add(T_SECT_SSM_K Key, T_SECT_SSM_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SECT_SSM_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SECT_SSM_K Key, T_SECT_SSM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SECT_SSM_K& rKey, T_SECT_SSM_D& rData);

	void AddList(T_SECT_SSM_K Key);
	BOOL DelList(T_SECT_SSM_K Key);
	void DelAllList();
	void AddListItem(T_SECT_SSM_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SECT_SSM_K Key, int nCmd, int nKey);
	BOOL GetList(T_SECT_SSM_K Key, DBDataList*& rpList);

protected:
	CMap<T_SECT_SSM_K, T_SECT_SSM_K, T_SECT_SSM_D, T_SECT_SSM_D&> m_sectSsm;
	CMap<T_SECT_SSM_K, T_SECT_SSM_K, DBDataList*, DBDataList*>m_sectSsmlist;
};



#endif