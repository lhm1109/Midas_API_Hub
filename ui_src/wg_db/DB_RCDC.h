#ifndef __DB_RCDC_DB_H__
#define __DB_RCDC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RCDC
{
public:
	CDB_RCDC();
	virtual ~CDB_RCDC();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCDC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCDC_D& rData);
	//{return m_racd.Lookup(1,rData);}
	int GetCount();
	//{return m_racd.GetCount();}

protected:
	CMap<T_RCDC_K,T_RCDC_K,T_RCDC_D,T_RCDC_D&>m_rcdc;
};

#endif