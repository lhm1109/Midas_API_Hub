#ifndef __DB_CJFG_DB_H__
#define __DB_CJFG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CJFG
{
public:
	CDB_CJFG();
	virtual ~CDB_CJFG();
	CDBDoc* m_pDoc;

public:
	void Add(T_CJFG_D& rData,CDB_GRUP* pGrup);
	BOOL Del(CDB_GRUP* pGrup);

public:
	BOOL Get(T_CJFG_D& rData);
	int GetCount();

protected:
	CMap<T_CJFG_K,T_CJFG_K,T_CJFG_D,T_CJFG_D&> m_cjfg;
};

#endif