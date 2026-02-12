#ifndef __DB_SBCT_DB_H__
#define __DB_SBCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SBCT
{
public:
	CDB_SBCT();
	virtual ~CDB_SBCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_SBCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SBCT_D& rData);
		//{return m_sbct.Lookup(1,rData);}
	int GetCount();
		//{return m_sbct.GetCount();}

protected:
	CMap<T_SBCT_K,T_SBCT_K,T_SBCT_D,T_SBCT_D&>m_sbct;
};

#endif