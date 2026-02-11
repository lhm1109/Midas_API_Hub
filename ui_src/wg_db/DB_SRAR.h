#ifndef __DB_SRAR_DB_H__
#define __DB_SRAR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SRAR
{
public:
	CDB_SRAR();
	virtual ~CDB_SRAR();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRAR_K Key, T_SRAR_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SRAR_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SRAR_K Key, T_SRAR_D& rData);
	//{return m_srar.Lookup(Key,rData);}
	int GetCount();
	//{return m_srar.GetCount();}
	POSITION GetStart();
	//{return m_srar.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRAR_K& rKey, T_SRAR_D& rData);
	//{m_srar.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRAR_K, T_SRAR_K, T_SRAR_D, T_SRAR_D&>m_srar;
};

#endif

