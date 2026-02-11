#ifndef __DB_PMCM_DB_H__
#define __DB_PMCM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_PMCM
{
public:
	CDB_PMCM();
	virtual ~CDB_PMCM();
	CDBDoc* m_pDoc;

public:
	void Add(T_PMCM_K Key,T_PMCM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PMCM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PMCM_K Key,T_PMCM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_PMCM_K& rKey,T_PMCM_D& rData);

protected:
	CMap<T_PMCM_K,T_PMCM_K,T_PMCM_D,T_PMCM_D&>m_PMCM;
};

#endif