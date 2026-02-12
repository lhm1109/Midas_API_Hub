#ifndef __DB_CPCO_DB_H__
#define __DB_CPCO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPCO
{
public:
	CDB_CPCO();
	virtual ~CDB_CPCO();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPCO_K Key,T_CPCO_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPCO_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPCO_K Key,T_CPCO_D& rData);
		//{return m_cpco.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpco.GetCount();}
	POSITION GetStart();
		//{return m_cpco.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPCO_K& rKey,T_CPCO_D& rData);
		//{m_cpco.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPCO_K,T_CPCO_K,T_CPCO_D,T_CPCO_D&>m_cpco;
};

#endif

