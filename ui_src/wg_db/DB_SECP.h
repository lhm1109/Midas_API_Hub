#ifndef __DB_SECP_DB_H__
#define __DB_SECP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SECP
{
public:
	CDB_SECP();
	virtual ~CDB_SECP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SECP_K Key,T_SECP_D& rData,CDB_SECT* pSect);
	BOOL Del(T_SECP_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_SECP_K Key,T_SECP_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SECP_K& rKey,T_SECP_D& rData);

protected:
	CMap<T_SECP_K,T_SECP_K,T_SECP_D,T_SECP_D&>m_secp;
};

#endif

