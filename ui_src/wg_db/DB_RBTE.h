#ifndef __DB_RBTE_DB_H__
#define __DB_RBTE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_RBTE
{
public:
	CDB_RBTE();
	virtual ~CDB_RBTE();
	CDBDoc* m_pDoc;

public:
	void Add(T_RBTE_K Key,T_RBTE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RBTE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RBTE_K Key,T_RBTE_D& rData);
		//{return m_rbte.Lookup(Key,rData);}
	int GetCount();
		//{return m_rbte.GetCount();}
	POSITION GetStart();
		//{return m_rbte.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RBTE_K& rKey,T_RBTE_D& rData);
		//{m_rbte.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RBTE_K,T_RBTE_K,T_RBTE_D,T_RBTE_D&>m_rbte;
};

#endif

