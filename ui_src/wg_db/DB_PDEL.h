#ifndef __DB_PDEL_DB_H__
#define __DB_PDEL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PDEL
{
public:
	CDB_PDEL();
	virtual ~CDB_PDEL();
	CDBDoc* m_pDoc;

public:
	void Add(T_PDEL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_PDEL_D& rData);
		//{return m_pdel.Lookup(1,rData);}
	int GetCount();
		//{return m_pdel.GetCount();}

protected:
	CMap<T_PDEL_K,T_PDEL_K,T_PDEL_D,T_PDEL_D&>m_pdel;
};

#endif