#ifndef __DB_PDCM_DB_H__
#define __DB_PDCM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif	// _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_PDCM
{
public:
	CDB_PDCM();
	virtual ~CDB_PDCM();

public:
	void Add(T_PDCM_K Key, T_PDCM_D& rData, CDB_MATL* pMatl);
	BOOL Del(T_PDCM_K Key, CDB_MATL* pMatl);

public:
	BOOL Get(T_PDCM_K Key, T_PDCM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PDCM_K& rKey, T_PDCM_D& rData);

protected:
	CMap<T_PDCM_K, T_PDCM_K, T_PDCM_D, T_PDCM_D&>m_pdcm;
};

#endif