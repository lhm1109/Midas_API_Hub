#ifndef __DB_PPLC_DB_H__
#define __DB_PPLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PPLC
{
public:
	CDB_PPLC();
	virtual ~CDB_PPLC();
	CDBDoc* m_pDoc;

public:
	void Add(T_PPLC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_PPLC_D& rData);
		//{return m_pplc.Lookup(1,rData);}
	int GetCount();
		//{return m_pplc.GetCount();}

protected:
	CMap<T_PPLC_K,T_PPLC_K,T_PPLC_D,T_PPLC_D&>m_pplc;
};

#endif