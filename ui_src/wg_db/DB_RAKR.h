#ifndef __DB_RAKR_DB_H__
#define __DB_RAKR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RAKR
{
public:
	CDB_RAKR();
	virtual ~CDB_RAKR();
	CDBDoc* m_pDoc;

public:
	void Add(T_RAKR_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RAKR_D& rData);
		//{return m_rakr.Lookup(1,rData);}
	int GetCount();
		//{return m_rakr.GetCount();}

protected:
	CMap<T_RAKR_K,T_RAKR_K,T_RAKR_D,T_RAKR_D&>m_rakr;
};

#endif