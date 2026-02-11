#ifndef __DB_LCRS_DB_H__
#define __DB_LCRS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LCRS
{
public:
	CDB_LCRS();
	virtual ~CDB_LCRS();
	CDBDoc* m_pDoc;

public:
	void Add(T_LCRS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_LCRS_D& rData);
		//{return m_lcrs.Lookup(1,rData);}
	int GetCount();
		//{return m_lcrs.GetCount();}

protected:
	CMap<T_LCRS_K,T_LCRS_K,T_LCRS_D,T_LCRS_D&> m_lcrs;
};

#endif