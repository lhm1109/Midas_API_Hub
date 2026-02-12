#ifndef __DB_POSP_DB_H__
#define __DB_POSP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_POSP
{
public:
	CDB_POSP();
	virtual ~CDB_POSP();
	CDBDoc* m_pDoc;

public:
	T_POSP_K m_nLastNum;
	T_POSP_K m_nStartNum;

public:
	void Add(T_POSP_K Key, T_POSP_D& rData);
	BOOL Del(T_POSP_K Key);

public:
	BOOL Get(T_POSP_K Key, T_POSP_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_POSP_K& rKey, T_POSP_D& rData);

protected:
	CMap<T_POSP_K, T_POSP_K, T_POSP_D, T_POSP_D&>m_posp;
};

#endif