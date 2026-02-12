#ifndef __DB_SMCT_DB_H__
#define __DB_SMCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SMCT
{
public:
	CDB_SMCT();
	virtual ~CDB_SMCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_SMCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SMCT_D& rData);
		//{return m_smct.Lookup(1,rData);}
	int GetCount();
		//{return m_smct.GetCount();}

protected:
	CMap<T_SMCT_K,T_SMCT_K,T_SMCT_D,T_SMCT_D&>m_smct;
};

#endif