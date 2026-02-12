#ifndef __DB_PRNT_DB_H__
#define __DB_PRNT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PRNT
{
public:
	CDB_PRNT();
	virtual ~CDB_PRNT();
	CDBDoc* m_pDoc;

public:
	void Add(T_PRNT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_PRNT_D& rData);
		//{return m_prnt.Lookup(1,rData);}
	int GetCount();
		//{return m_prnt.GetCount();}

protected:
	CMap<T_PRNT_K,T_PRNT_K,T_PRNT_D,T_PRNT_D&>m_prnt;
};

#endif