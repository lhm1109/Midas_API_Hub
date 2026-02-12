#ifndef __DB_CPSC_DB_H__
#define __DB_CPSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPSC
{
public:
	CDB_CPSC();
	virtual ~CDB_CPSC();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPSC_K Key,T_CPSC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPSC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPSC_K Key,T_CPSC_D& rData);
		//{return m_cpsc.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpsc.GetCount();}
	POSITION GetStart();
		//{return m_cpsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPSC_K& rKey,T_CPSC_D& rData);
		//{m_cpsc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPSC_K,T_CPSC_K,T_CPSC_D,T_CPSC_D&>m_cpsc;
};

#endif

