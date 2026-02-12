#ifndef __DB_MVCT_DB_H__
#define __DB_MVCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCT
{
public:
	CDB_MVCT();
	virtual ~CDB_MVCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCT_D& rData);
		//{return m_mvct.Lookup(1,rData);}
	int GetCount();
		//{return m_mvct.GetCount();}

protected:
	CMap<T_MVCT_K,T_MVCT_K,T_MVCT_D,T_MVCT_D&>m_mvct;
};

#endif