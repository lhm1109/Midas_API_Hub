#ifndef __DB_SIDP_DB_H__
#define __DB_SIDP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SIPA;
class CDB_SIGR;
class CDB_SIGR_LOWER;

class CDB_SIDP
{
public:
	CDB_SIDP();
	virtual ~CDB_SIDP();
	CDBDoc* m_pDoc;

public:
	T_SIDP_K m_nStartNum;
	T_SIDP_K m_nLastNum;

public:
	void Add(T_SIDP_K Key, T_SIDP_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower);
	BOOL Del(T_SIDP_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower);

public:
	BOOL Get(T_SIDP_K Key, T_SIDP_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIDP_K& rKey, T_SIDP_D& rData);

protected:
	CMap<T_SIDP_K, T_SIDP_K, T_SIDP_D, T_SIDP_D&>m_sidp;
};



#endif