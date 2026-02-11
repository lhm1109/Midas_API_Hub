#ifndef __DB_STAN_DB_H__
#define __DB_STAN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STAN
{
public:
	CDB_STAN();
	virtual ~CDB_STAN();
	CDBDoc* m_pDoc;

public:
	void Add(T_STAN_K Key,T_STAN_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_STAN_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_STAN_K Key,T_STAN_D& rData);
		//{return m_stan.Lookup(Key,rData);}
	int GetCount();
		//{return m_stan.GetCount();}
	POSITION GetStart();
		//{return m_stan.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STAN_K& rKey,T_STAN_D& rData);
		//{m_stan.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STAN_K,T_STAN_K,T_STAN_D,T_STAN_D&>m_stan;
};

#endif

