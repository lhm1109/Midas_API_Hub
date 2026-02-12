#ifndef __DB_HCRP_DB_H__
#define __DB_HCRP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_HCRP
{
public:
	CDB_HCRP();
	virtual ~CDB_HCRP();
	CDBDoc* m_pDoc;

public:
	void Add(T_HCRP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_HCRP_D& rData);
		//{return m_hcrp.Lookup(1,rData);}
	int GetCount();
		//{return m_hcrp.GetCount();}

protected:
	CMap<T_HCRP_K,T_HCRP_K,T_HCRP_D,T_HCRP_D&>m_hcrp;
};

#endif