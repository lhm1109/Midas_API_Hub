#ifndef __DB_SRAS_DB_H__
#define __DB_SRAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SRAS
{
public:
	CDB_SRAS();
	virtual ~CDB_SRAS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRAS_K Key, T_SRAS_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SRAS_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SRAS_K Key, T_SRAS_D& rData);
	//{return m_sras.Lookup(Key,rData);}
	int GetCount();
	//{return m_sras.GetCount();}
	POSITION GetStart();
	//{return m_sras.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRAS_K& rKey, T_SRAS_D& rData);
	//{m_sras.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRAS_K, T_SRAS_K, T_SRAS_D, T_SRAS_D&>m_sras;
};

#endif