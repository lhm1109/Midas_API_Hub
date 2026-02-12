#ifndef __DB_DCBC_DB_H__
#define __DB_DCBC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DCBC
{
public:
	CDB_DCBC();
	virtual ~CDB_DCBC();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCBC_K Key,T_DCBC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DCBC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DCBC_K Key,T_DCBC_D& rData);
		//{return m_dcbc.Lookup(Key,rData);}
	int GetCount();
		//{return m_dcbc.GetCount();}
	POSITION GetStart();
		//{return m_dcbc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DCBC_K& rKey,T_DCBC_D& rData);
		//{m_dcbc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DCBC_K,T_DCBC_K,T_DCBC_D,T_DCBC_D&>m_dcbc;
};

#endif

