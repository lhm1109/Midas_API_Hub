#ifndef __DB_MATD_REIN_DB_H__
#define __DB_MATD_REIN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_MATL;
class CDB_MATD_REIN
{
public:
	CDB_MATD_REIN();
	virtual ~CDB_MATD_REIN();
	CDBDoc* m_pDoc;

public:
	T_MATD_REIN_K m_nStartNum;
	T_MATD_REIN_K m_nLastNum;

public:
	void Add(T_MATD_REIN_K Key, T_MATD_REIN_D& rData, CDB_MATL* pMatl);
	BOOL Del(T_MATD_REIN_K Key, CDB_MATL* pMatl);

public:
	BOOL Get(T_MATD_REIN_K Key, T_MATD_REIN_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_MATD_REIN_K& rKey, T_MATD_REIN_D& rData);

protected:
	CMap<T_MATD_REIN_K, T_MATD_REIN_K, T_MATD_REIN_D, T_MATD_REIN_D&> m_dataMap;
};

#endif