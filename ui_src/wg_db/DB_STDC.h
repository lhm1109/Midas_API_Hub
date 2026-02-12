#ifndef __DB_STDC_DB_H__
#define __DB_STDC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STDC
{
public:
	CDB_STDC();
	virtual ~CDB_STDC();
	CDBDoc* m_pDoc;

public:
	void Add(T_STDC_K Key,T_STDC_D& rData,CDB_SECT* pSect);
	BOOL Del(T_STDC_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_STDC_K Key,T_STDC_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_STDC_K& rKey,T_STDC_D& rData);

protected:
	CMap<T_STDC_K,T_STDC_K,T_STDC_D,T_STDC_D&>m_stdc;
};

#endif

