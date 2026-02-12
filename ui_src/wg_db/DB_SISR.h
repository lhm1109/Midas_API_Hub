#ifndef __DB_SISR_DB_H__
#define __DB_SISR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SIPA;
class CDB_SIGR;
class CDB_ELEM;

class CDB_SISR
{
public:
	CDB_SISR();
	virtual ~CDB_SISR();
	CDBDoc* m_pDoc;

public:
	T_SISR_K m_nStartNum;
	T_SISR_K m_nLastNum;

public:
	void Add(T_SISR_K Key, T_SISR_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_ELEM* pElem);
	BOOL Del(T_SISR_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_ELEM* pElem);

public:
	BOOL Get(T_SISR_K Key, T_SISR_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SISR_K& rKey, T_SISR_D& rData);

	void AddList(T_SISR_K Key);
	BOOL DelList(T_SISR_K Key);
	void DelAllList();
	void AddListItem(T_SISR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SISR_K Key, int nCmd, int nKey);
	BOOL GetList(T_SISR_K Key, DBDataList*& rpList);

protected:
	CMap<T_SISR_K, T_SISR_K, T_SISR_D, T_SISR_D&>m_sisr;
	CMap<T_SISR_K, T_SISR_K, DBDataList*, DBDataList*>m_sisrlist;
};



#endif