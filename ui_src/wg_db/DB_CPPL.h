#ifndef __DB_CPPL_DB_H__
#define __DB_CPPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CPPL
{
public:
	CDB_CPPL();
	virtual ~CDB_CPPL();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPPL_K Key,T_CPPL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CPPL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CPPL_K Key,T_CPPL_D& rData);
		//{return m_cppl.Lookup(Key,rData);}
	int GetCount();
		//{return m_cppl.GetCount();}
	POSITION GetStart();
		//{return m_cppl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPPL_K& rKey,T_CPPL_D& rData);
		//{m_cppl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPPL_K,T_CPPL_K,T_CPPL_D,T_CPPL_D&>m_cppl;
};

#endif

