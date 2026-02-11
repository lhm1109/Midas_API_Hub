#ifndef __DB_MREB_DB_H__
#define __DB_MREB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_REBB;
class CDB_REBC;
class CDB_REBR;
class CDB_RBMW;
class CDB_RBST;

class CDB_MREB
{
public:
	CDB_MREB();
	virtual ~CDB_MREB();
	CDBDoc* m_pDoc;

public:
	void Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_REBB* pRebb);
	void Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_REBC* pRebc);
	void Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_REBR* pRebr);
	void Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_RBMW* pRbmw);
	void Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem);
	void Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_RBST* pRbst);

	BOOL Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_REBB* pRebb);
	BOOL Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_REBC* pRebc);
	BOOL Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_REBR* pRebr);
	BOOL Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_RBMW* pRbmw);
	BOOL Del(T_MREB_K Key,CDB_ELEM* pElem);
	BOOL Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_RBST* pRbst);

public:
	BOOL Get(T_MREB_K Key,T_MREB_D& rData);
		//{return m_mreb.Lookup(Key,rData);}
	int GetCount();
		//{return m_mreb.GetCount();}
	POSITION GetStart();
		//{return m_mreb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MREB_K& rKey,T_MREB_D& rData);
		//{m_mreb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MREB_K,T_MREB_K,T_MREB_D,T_MREB_D&>m_mreb;
};

#endif

