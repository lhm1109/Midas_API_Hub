#ifndef __DB_PSCS_DB_H__
#define __DB_PSCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PSCS
{
public:
	CDB_PSCS();
	virtual ~CDB_PSCS();
	CDBDoc* m_pDoc;

public:
	void Add(T_PSCS_K Key,T_PSCS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PSCS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PSCS_K Key,T_PSCS_D& rData);

	int GetCount();

	POSITION GetStart();

	void GetNext(POSITION& rNextPosition,T_PSCS_K& rKey,T_PSCS_D& rData);

protected:
	CMap<T_PSCS_K,T_PSCS_K,T_PSCS_D,T_PSCS_D&>m_pscs;
};

#endif

