#ifndef __DB_PSAS_DB_H__
#define __DB_PSAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PSAS
{
public:
	CDB_PSAS();
	virtual ~CDB_PSAS();
	CDBDoc* m_pDoc;

public:
	void Add(T_PSAS_K Key,T_PSAS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PSAS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PSAS_K Key,T_PSAS_D& rData);
		//{return m_psas.Lookup(Key,rData);}
	int GetCount();
		//{return m_psas.GetCount();}
	POSITION GetStart();
		//{return m_psas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PSAS_K& rKey,T_PSAS_D& rData);
		//{m_psas.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PSAS_K,T_PSAS_K,T_PSAS_D,T_PSAS_D&>m_psas;
};

#endif

