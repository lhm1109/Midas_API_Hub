#ifndef __DB_RBGC_DB_H__
#define __DB_RBGC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_RBGC
{
public:
	CDB_RBGC();
	virtual ~CDB_RBGC();
	CDBDoc* m_pDoc;

public:
	void Add(T_RBGC_K Key,T_RBGC_D& rData,CDB_SECT* pSect);
	BOOL Del(T_RBGC_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_RBGC_K Key,T_RBGC_D& rData);
		//{return m_rbgc.Lookup(Key,rData);}
	int GetCount();
		//{return m_rbgc.GetCount();}
	POSITION GetStart();
		//{return m_rbgc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RBGC_K& rKey,T_RBGC_D& rData);
		//{m_rbgc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RBGC_K,T_RBGC_K,T_RBGC_D,T_RBGC_D&>m_rbgc;
};

#endif

