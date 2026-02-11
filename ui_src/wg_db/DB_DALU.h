#ifndef __DB_DALU_DB_H__
#define __DB_DALU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DALU
{
public:
	CDB_DALU();
	virtual ~CDB_DALU();
	CDBDoc* m_pDoc;

public:
	void Add(T_DALU_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DALU_D& rData);
		//{return m_dALU.Lookup(1,rData);}
	int GetCount();
		//{return m_dALU.GetCount();}

protected:
	CMap<T_DALU_K,T_DALU_K,T_DALU_D,T_DALU_D&>m_dalu;
};

#endif