#ifndef __DB_RBAR_SSM_DB_H__
#define __DB_RBAR_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RBAR_SSM
{
public:
	CDB_RBAR_SSM();
	virtual ~CDB_RBAR_SSM();
	CDBDoc* m_pDoc;

public:
	T_RBAR_SSM_K m_nStartNum;
	T_RBAR_SSM_K m_nLastNum;

public:
	void Add(T_RBAR_SSM_K Key, T_RBAR_SSM_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_RBAR_SSM_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_RBAR_SSM_K Key, T_RBAR_SSM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RBAR_SSM_K& rKey, T_RBAR_SSM_D& rData);

protected:
	CMap<T_RBAR_SSM_K, T_RBAR_SSM_K, T_RBAR_SSM_D, T_RBAR_SSM_D&> m_rbar;
};



#endif