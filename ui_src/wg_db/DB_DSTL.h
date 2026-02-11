#ifndef __DB_DSTL_DB_H__
#define __DB_DSTL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DSTL
{
public:
	CDB_DSTL();
	virtual ~CDB_DSTL();
	CDBDoc* m_pDoc;

public:
	void Add(T_DSTL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DSTL_D& rData);
		//{return m_dstl.Lookup(1,rData);}
	int GetCount();
		//{return m_dstl.GetCount();}

protected:
	CMap<T_DSTL_K,T_DSTL_K,T_DSTL_D,T_DSTL_D&>m_dstl;
};

#endif