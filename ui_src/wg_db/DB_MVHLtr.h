#ifndef __DB_MVHLtr_DB_H__
#define __DB_MVHLtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVHLtr
{
public:
	CDB_MVHLtr();
	virtual ~CDB_MVHLtr();
	CDBDoc* m_pDoc;

public:
	T_MVHLtr_K m_nStartNum;
	T_MVHLtr_K m_nLastNum;

public:
	void Add(T_MVHLtr_K Key,T_MVHLtr_D& rData);
	BOOL Del(T_MVHLtr_K Key);

public:
	BOOL Get(T_MVHLtr_K Key,T_MVHLtr_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_MVHLtr_K& rKey,T_MVHLtr_D& rData);

protected:
	CMap<T_MVHLtr_K,T_MVHLtr_K,T_MVHLtr_D,T_MVHLtr_D&>m_mvhltr;
};

#endif