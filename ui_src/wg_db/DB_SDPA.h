#ifndef __DB_SDPA_DB_H__
#define __DB_SDPA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SDPA
{
public:
	CDB_SDPA();
	virtual ~CDB_SDPA();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDPA_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SDPA_D& rData);
		//{return m_sdpa.Lookup(1,rData);}
	int GetCount();
		//{return m_sdpa.GetCount();}

protected:
	CMap<T_SDPA_K,T_SDPA_K,T_SDPA_D,T_SDPA_D&>m_sdpa;
};

#endif