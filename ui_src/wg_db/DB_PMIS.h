#ifndef __DB_PMIS_DB_H__
#define __DB_PMIS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_PMOS;

class CDB_PMIS
{
public:
	CDB_PMIS();
	virtual ~CDB_PMIS();
	CDBDoc* m_pDoc;

public:
	// 리스트 크기 조절 및 수정
	T_PMIS_K m_nStartNum;
	T_PMIS_K m_nLastNum;

public:
	void Add(T_PMIS_K Key, T_PMIS_D& rData);
	BOOL Del(T_PMIS_K Key);

public:
	BOOL Get(T_PMIS_K Key, T_PMIS_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PMIS_K& rKey, T_PMIS_D& rData);

protected:
	CMap<T_PMIS_K, T_PMIS_K, T_PMIS_D, T_PMIS_D&> m_pmis;
};



#endif