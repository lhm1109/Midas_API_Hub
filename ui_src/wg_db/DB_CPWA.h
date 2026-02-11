#ifndef __DB_CPWA_DB_H__
#define __DB_CPWA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPWA
{
public:
	CDB_CPWA();
	virtual ~CDB_CPWA();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPWA_K Key,T_CPWA_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPWA_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPWA_K Key,T_CPWA_D& rData);
		//{return m_cpwa.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpwa.GetCount();}
	POSITION GetStart();
		//{return m_cpwa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPWA_K& rKey,T_CPWA_D& rData);
		//{m_cpwa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPWA_K,T_CPWA_K,T_CPWA_D,T_CPWA_D&>m_cpwa;
};

#endif

