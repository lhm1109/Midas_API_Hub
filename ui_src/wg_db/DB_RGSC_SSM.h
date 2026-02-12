#ifndef __DB_RGSC_SSM_DB_H__
#define __DB_RGSC_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RGSC_SSM
{
public:
	CDB_RGSC_SSM();
	virtual ~CDB_RGSC_SSM();
	CDBDoc* m_pDoc;

public:
	T_RGSC_SSM_K m_nStartNum;
	T_RGSC_SSM_K m_nLastNum;

public:
	void Add(T_RGSC_SSM_K Key, T_RGSC_SSM_D& rData);
	BOOL Del(T_RGSC_SSM_K Key);

public:
	BOOL Get(T_RGSC_SSM_K Key, T_RGSC_SSM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RGSC_SSM_K& rKey, T_RGSC_SSM_D& rData);

protected:
	CMap<T_RGSC_SSM_K, T_RGSC_SSM_K, T_RGSC_SSM_D, T_RGSC_SSM_D&> m_rgscSsm;
};

#endif