#ifndef __DB_SLDC_DB_H__
#define __DB_SLDC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SLDC
{
public:
	CDB_SLDC();
	virtual ~CDB_SLDC();
	CDBDoc* m_pDoc;

public:
	void Add(T_SLDC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SLDC_D& rData);
		//{return m_sldc.Lookup(1,rData);}
	int GetCount();
		//{return m_sldc.GetCount();}

protected:
	CMap<T_SLDC_K,T_SLDC_K,T_SLDC_D,T_SLDC_D&>m_sldc;
};

#endif