#ifndef __DB_RPSS_DB_H__
#define __DB_RPSS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RPSS
{
public:
	CDB_RPSS();
	virtual ~CDB_RPSS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RPSS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RPSS_D& rData);
	//{return m_rpss.Lookup(1,rData);}
	int GetCount();
	//{return m_rpss.GetCount();}

protected:
	CMap<T_RPSS_K, T_RPSS_K, T_RPSS_D, T_RPSS_D&>m_rpss;
};

#endif