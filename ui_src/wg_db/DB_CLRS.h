#ifndef __DB_CLRS_DB_H__
#define __DB_CLRS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_CLRS
{
public:
	CDB_CLRS();
	virtual ~CDB_CLRS();
	CDBDoc* m_pDoc;

public:
	void Add(T_CLRS_K Key,T_CLRS_D& rData,CDB_SECT* pSect);
	BOOL Del(T_CLRS_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_CLRS_K Key,T_CLRS_D& rData);
		//{return m_clrs.Lookup(Key,rData);}
	int GetCount();
		//{return m_clrs.GetCount();}
	POSITION GetStart();
		//{return m_clrs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CLRS_K& rKey,T_CLRS_D& rData);
		//{m_clrs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CLRS_K,T_CLRS_K,T_CLRS_D,T_CLRS_D&>m_clrs;
};

#endif

