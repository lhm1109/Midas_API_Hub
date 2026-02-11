#ifndef __DB_MODL_DB_H__
#define __DB_MODL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MODL
{
public:
	CDB_MODL();
	virtual ~CDB_MODL();
	CDBDoc* m_pDoc;

public:
	void Add(T_MODL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MODL_D& rData);
		//{return m_modl.Lookup(1,rData);}
	int GetCount();
		//{return m_modl.GetCount();}

protected:
	CMap<T_MODL_K,T_MODL_K,T_MODL_D,T_MODL_D&>m_modl;
};

#endif