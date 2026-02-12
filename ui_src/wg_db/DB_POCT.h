#ifndef __DB_POCT_DB_H__
#define __DB_POCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_POCT
{
public:
	CDB_POCT();
	virtual ~CDB_POCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_POCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_POCT_D& rData);
		//{return m_poct.Lookup(1,rData);}
	int GetCount();
		//{return m_poct.GetCount();}

protected:
	CMap<T_POCT_K,T_POCT_K,T_POCT_D,T_POCT_D&>m_poct;
};

#endif