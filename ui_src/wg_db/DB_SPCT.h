#ifndef __DB_SPCT_DB_H__
#define __DB_SPCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SPCT
{
public:
	CDB_SPCT();
	virtual ~CDB_SPCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SPCT_D& rData);
		//{return m_spct.Lookup(1,rData);}
	int GetCount();
		//{return m_spct.GetCount();}

protected:
	CMap<T_SPCT_K,T_SPCT_K,T_SPCT_D,T_SPCT_D&>m_spct;
};

#endif