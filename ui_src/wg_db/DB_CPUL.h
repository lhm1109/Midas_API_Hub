#ifndef __DB_CPUL_DB_H__
#define __DB_CPUL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPUL
{
public:
	CDB_CPUL();
	virtual ~CDB_CPUL();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPUL_K Key,T_CPUL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPUL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPUL_K Key,T_CPUL_D& rData);
		//{return m_cpul.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpul.GetCount();}
	POSITION GetStart();
		//{return m_cpul.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPUL_K& rKey,T_CPUL_D& rData);
		//{m_cpul.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPUL_K,T_CPUL_K,T_CPUL_D,T_CPUL_D&>m_cpul;
};

#endif

