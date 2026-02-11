#ifndef __DB_PESV_DB_H__
#define __DB_PESV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PESV
{
public:
	CDB_PESV();
	virtual ~CDB_PESV();
	CDBDoc* m_pDoc;

public:
	void Add(T_PESV_K Key, T_PESV_D& rData);
	BOOL Del(T_PESV_K Key);

public:
	BOOL Get(T_PESV_K Key, T_PESV_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_PESV_K& rKey, T_PESV_D& rData);

protected:
	CMap<T_PESV_K, T_PESV_K, T_PESV_D, T_PESV_D&>m_pesv;
};

#endif

