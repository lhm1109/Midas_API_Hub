#ifndef __DB_DCRB_DB_H__
#define __DB_DCRB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DCRB
{
public:
	CDB_DCRB();
	virtual ~CDB_DCRB();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCRB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DCRB_D& rData);
		//{return m_dcrb.Lookup(1,rData);}
	int GetCount();
		//{return m_dcrb.GetCount();}

protected:
	CMap<T_DCRB_K,T_DCRB_K,T_DCRB_D,T_DCRB_D&>m_dcrb;
};

#endif