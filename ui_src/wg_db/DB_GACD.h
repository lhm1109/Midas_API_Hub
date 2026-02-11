#ifndef __DB_GACD_DB_H__
#define __DB_GACD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_GACD
{
public:
	CDB_GACD();
	virtual ~CDB_GACD();
	CDBDoc* m_pDoc;

public:
	void Add(T_GACD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_GACD_D& rData);
	//{return m_GACD.Lookup(1,rData);}
	int GetCount();
	//{return m_GACD.GetCount();}

protected:
	CMap<T_GACD_K,T_GACD_K,T_GACD_D,T_GACD_D&>m_GACD;
};

#endif