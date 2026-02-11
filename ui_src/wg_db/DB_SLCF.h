#ifndef __DB_SLCF_DB_H__
#define __DB_SLCF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SLCF
{
public:
	CDB_SLCF();
	virtual ~CDB_SLCF();
	CDBDoc* m_pDoc;

public:
	void Add(T_SLCF_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SLCF_D& rData);
	int GetCount();

protected:
	CMap<T_SLCF_K,T_SLCF_K,T_SLCF_D,T_SLCF_D&>m_slcf;
};

#endif