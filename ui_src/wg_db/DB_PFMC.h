#ifndef __DB_PFMC_DB_H__
#define __DB_PFMC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PFMC
{
public:
	CDB_PFMC();
	virtual ~CDB_PFMC();
	CDBDoc* m_pDoc;

public:
	void Add(T_PFMC_K Key,T_PFMC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PFMC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PFMC_K Key,T_PFMC_D& rData);
		//{return m_PFMC.Lookup(Key,rData);}
	int GetCount();
		//{return m_PFMC.GetCount();}
	POSITION GetStart();
		//{return m_PFMC.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFMC_K& rKey,T_PFMC_D& rData);
		//{m_PFMC.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PFMC_K,T_PFMC_K,T_PFMC_D,T_PFMC_D&>m_PFMC;
};

#endif

