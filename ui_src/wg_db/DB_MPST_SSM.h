#ifndef __DB_MPST_SSM_DB_H__
#define __DB_MPST_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPST_SSM
{
public:
	CDB_MPST_SSM();
	virtual ~CDB_MPST_SSM();

public:
	T_MPST_SSM_K m_nStartNum;
	T_MPST_SSM_K m_nLastNum;

public:
	void Add(T_MPST_SSM_K Key, T_MPST_SSM_D& rData);
	BOOL Del(T_MPST_SSM_K Key);

public:
	BOOL Get(T_MPST_SSM_K Key, T_MPST_SSM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_MPST_SSM_K& rKey, T_MPST_SSM_D& rData);

protected:
	CMap<T_MPST_SSM_K, T_MPST_SSM_K, T_MPST_SSM_D, T_MPST_SSM_D&> m_mpstSsm;
};

#endif