#ifndef __DB_NLOP_DB_H__
#define __DB_NLOP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_NLOP
{
public:
	CDB_NLOP();
	virtual ~CDB_NLOP();
	CDBDoc* m_pDoc;

public:
	void Add(T_NLOP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_NLOP_D& rData);
		//{return m_nlop.Lookup(1,rData);}
	int GetCount();
		//{return m_nlop.GetCount();}

protected:
	CMap<T_NLOP_K,T_NLOP_K,T_NLOP_D,T_NLOP_D&>m_nlop;
};

#endif