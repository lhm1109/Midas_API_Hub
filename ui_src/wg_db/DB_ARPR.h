#ifndef __DB_ARPR_DB_H__
#define __DB_ARPR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LDGR;
class CDB_LAPL;
class CDB_ELEM;

class CDB_ARPR
{
public:
	CDB_ARPR();
	virtual ~CDB_ARPR();
	CDBDoc* m_pDoc;

public:
	T_ARPR_K m_nLastNum;
	T_ARPR_K m_nStartNum;

public:
	void Add(T_ARPR_K Key, T_ARPR_D& rData, CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem);
	BOOL Del(T_ARPR_K Key,                  CDB_STLD* pStld, CDB_LDGR* pLdgr, CDB_LAPL* pLapl, CDB_ELEM* pElem);

public:
	BOOL Get(T_ARPR_K Key, T_ARPR_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_ARPR_K& rKey, T_ARPR_D& rData);
	
protected:
	CMap<T_ARPR_K,T_ARPR_K,T_ARPR_D,T_ARPR_D&>m_arpr;
};

#endif
