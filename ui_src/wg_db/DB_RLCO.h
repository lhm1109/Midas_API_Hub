#ifndef __DB_RLCO_DB_H__
#define __DB_RLCO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RLCO
{
public:
	CDB_RLCO();
	virtual ~CDB_RLCO();
	CDBDoc* m_pDoc;

public:
	void Add(T_RLCO_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RLCO_D& rData);
		//{return m_rlco.Lookup(1,rData);}
	int GetCount();
		//{return m_rlco.GetCount();}

protected:
	CMap<T_RLCO_K,T_RLCO_K,T_RLCO_D,T_RLCO_D&>m_rlco;
};

#endif