#ifndef __DB_IPDT_DB_H__
#define __DB_IPDT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_STOR;
class CDB_IPDT
{
public:
	CDB_IPDT();
	virtual ~CDB_IPDT();
	CDBDoc* m_pDoc;

public:
	void Add(T_IPDT_K Key,T_IPDT_D& rData,CDB_STOR* pStor);
	BOOL Del(T_IPDT_K Key,CDB_STOR* pStor);

public:
	BOOL Get(T_IPDT_K Key,T_IPDT_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_IPDT_K& rKey,T_IPDT_D& rData);

protected:
	CMap<T_IPDT_K,T_IPDT_K,T_IPDT_D,T_IPDT_D&>m_IPDT;
};

#endif