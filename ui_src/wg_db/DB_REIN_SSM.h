#ifndef __DB_REIN_SSM_DB_H__
#define __DB_REIN_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_REIN_SSM
{
public:
	CDB_REIN_SSM();
	virtual ~CDB_REIN_SSM();
	CDBDoc* m_pDoc;

public:
	T_REIN_SSM_K m_nStartNum;
	T_REIN_SSM_K m_nLastNum;

public:
	void Add(T_REIN_SSM_K Key, T_REIN_SSM_D& rData);
	BOOL Del(T_REIN_SSM_K Key);

public:
	BOOL Get(T_REIN_SSM_K Key, T_REIN_SSM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_REIN_SSM_K& rKey, T_REIN_SSM_D& rData);

protected:
	CMap<T_REIN_SSM_K, T_REIN_SSM_K, T_REIN_SSM_D, T_REIN_SSM_D&> m_reinSsm;
};

#endif