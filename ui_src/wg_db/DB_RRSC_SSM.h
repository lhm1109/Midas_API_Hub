#ifndef __DB_RRSC_SSM_DB_H__
#define __DB_RRSC_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RRSC_SSM
{
public:
	CDB_RRSC_SSM();
	virtual ~CDB_RRSC_SSM();
	CDBDoc* m_pDoc;

public:
	T_RRSC_SSM_K m_nStartNum;
	T_RRSC_SSM_K m_nLastNum;

public:
	void Add(T_RRSC_SSM_K Key, T_RRSC_SSM_D& rData);
	BOOL Del(T_RRSC_SSM_K Key);

public:
	BOOL Get(T_RRSC_SSM_K Key, T_RRSC_SSM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RRSC_SSM_K& rKey, T_RRSC_SSM_D& rData);

protected:
	CMap<T_RRSC_SSM_K, T_RRSC_SSM_K, T_RRSC_SSM_D, T_RRSC_SSM_D&> m_rrscSsm;
};

#endif