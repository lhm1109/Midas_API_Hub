#ifndef __DB_MVLDtrtr_DB_H__
#define __DB_MVLDtrtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVLDtr
{
public:
	CDB_MVLDtr();
	virtual ~CDB_MVLDtr();
	CDBDoc* m_pDoc;

public:
	T_MVLDtr_K m_nStartNum;
	T_MVLDtr_K m_nLastNum;

public:
	void Add(T_MVLDtr_K Key,T_MVLDtr_D& rData);
	BOOL Del(T_MVLDtr_K Key);

public:
	BOOL Get(T_MVLDtr_K Key,T_MVLDtr_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_MVLDtr_K& rKey,T_MVLDtr_D& rData);

protected:
	CMap<T_MVLDtr_K,T_MVLDtr_K,T_MVLDtr_D,T_MVLDtr_D&>m_mvldtr;
};

#endif
