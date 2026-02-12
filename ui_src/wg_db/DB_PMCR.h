#ifndef __DB_PMCR_DB_H__
#define __DB_PMCR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PMCR
{
public:
	CDB_PMCR();
	virtual ~CDB_PMCR();
	CDBDoc* m_pDoc;

public:
	void Add(T_PMCR_K Key,T_PMCR_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PMCR_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PMCR_K Key,T_PMCR_D& rData);
		//{return m_pmcr.Lookup(Key,rData);}
	int GetCount();
		//{return m_pmcr.GetCount();}
	POSITION GetStart();
		//{return m_pmcr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PMCR_K& rKey,T_PMCR_D& rData);
		//{m_pmcr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PMCR_K,T_PMCR_K,T_PMCR_D,T_PMCR_D&>m_pmcr;
};

#endif

