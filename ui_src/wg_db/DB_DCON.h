#ifndef __DB_DCON_DB_H__
#define __DB_DCON_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DCON
{
public:
	CDB_DCON();
	virtual ~CDB_DCON();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCON_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DCON_D& rData);
		//{return m_dcon.Lookup(1,rData);}
	int GetCount();
		//{return m_dcon.GetCount();}

protected:
	CMap<T_DCON_K,T_DCON_K,T_DCON_D,T_DCON_D&>m_dcon;
};

#endif