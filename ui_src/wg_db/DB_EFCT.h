#ifndef __DB_EFCT_DB_H__
#define __DB_EFCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_EFCT
{
public:
	CDB_EFCT();
	virtual ~CDB_EFCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_EFCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_EFCT_D& rData);
		//{return m_efct.Lookup(1,rData);}
	int GetCount();
		//{return m_efct.GetCount();}

protected:
	CMap<T_EFCT_K,T_EFCT_K,T_EFCT_D,T_EFCT_D&>m_efct;
};

#endif