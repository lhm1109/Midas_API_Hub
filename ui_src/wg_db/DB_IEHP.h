#ifndef __DB_IEHP_DB_H__
#define __DB_IEHP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_IEHP
{
public:
	CDB_IEHP();
	virtual ~CDB_IEHP();
	CDBDoc* m_pDoc;

public:
	T_IEHP_K m_nStartNum;
	T_IEHP_K m_nLastNum;

public:
	void Add(T_IEHP_K Key,T_IEHP_D& rData);
	BOOL Del(T_IEHP_K Key);

public:
	BOOL Get(T_IEHP_K Key,T_IEHP_D& rData);
	const T_IEHP_D* CDB_IEHP::GetConstPtr(T_IEHP_K Key);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_IEHP_K& rKey,T_IEHP_D& rData);

protected:
	CMap<T_IEHP_K,T_IEHP_K,T_IEHP_D,T_IEHP_D&>m_iehp;
};

#endif
