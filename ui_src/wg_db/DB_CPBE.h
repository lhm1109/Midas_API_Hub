#ifndef __DB_CPBE_DB_H__
#define __DB_CPBE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPBE
{
public:
	CDB_CPBE();
	virtual ~CDB_CPBE();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPBE_K Key,T_CPBE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPBE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPBE_K Key,T_CPBE_D& rData);
		//{return m_cpbe.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpbe.GetCount();}
	POSITION GetStart();
		//{return m_cpbe.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPBE_K& rKey,T_CPBE_D& rData);
		//{m_cpbe.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPBE_K,T_CPBE_K,T_CPBE_D,T_CPBE_D&>m_cpbe;
};

#endif

