#ifndef __DB_POGD_DB_H__
#define __DB_POGD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_POGD
{
public:
	CDB_POGD();
	virtual ~CDB_POGD();
	CDBDoc* m_pDoc;

public:
	void Add(T_POGD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_POGD_D& rData);
		//{return m_poct.Lookup(1,rData);}
	int GetCount();
		//{return m_poct.GetCount();}

protected:
	CMap<T_POGD_K,T_POGD_K,T_POGD_D,T_POGD_D&>m_pogd;
};

#endif