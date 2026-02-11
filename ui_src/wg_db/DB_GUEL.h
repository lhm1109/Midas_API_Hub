#ifndef __DB_GUEL_DB_H__
#define __DB_GUEL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_GUEL
{
public:
	CDB_GUEL();
	virtual ~CDB_GUEL();
	CDBDoc* m_pDoc;

public:
	T_GUEL_K m_nStartNum;
	T_GUEL_K m_nLastNum;

public:
	void Add(T_GUEL_K Key, T_GUEL_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_GUEL_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_GUEL_K Key, T_GUEL_D& rData);
	//{return m_guel.Lookup(Key,rData);}
	int GetCount();
	//{return m_guel.GetCount();}
	POSITION GetStart();
	//{return m_guel.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUEL_K& rKey, T_GUEL_D& rData);
	//{m_guel.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUEL_K,T_GUEL_K,T_GUEL_D,T_GUEL_D&>m_guel;
};

#endif