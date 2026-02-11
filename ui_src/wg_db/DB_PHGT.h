#ifndef __DB_PHGT_DB_H__
#define __DB_PHGT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PHGT
{
public:
	CDB_PHGT();
	virtual ~CDB_PHGT();
	CDBDoc* m_pDoc;

public:
	T_PHGT_K m_nStartNum;
	T_PHGT_K m_nLastNum;
	int m_nLastSeq;

public:
	void Add(T_PHGT_K Key,T_PHGT_D& rData);
	BOOL Del(T_PHGT_K Key);

public:
	BOOL Get(T_PHGT_K Key,T_PHGT_D& rData);
	const T_PHGT_D* CDB_PHGT::GetConstPtr(T_PHGT_K Key);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_PHGT_K& rKey,T_PHGT_D& rData);

protected:
	CMap<T_PHGT_K,T_PHGT_K,T_PHGT_D,T_PHGT_D&>m_phgt;
};

#endif
