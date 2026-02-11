#ifndef __DB_CHRP_DB_H__
#define __DB_CHRP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CHRP
{
public:
	CDB_CHRP();
	virtual ~CDB_CHRP();
	CDBDoc* m_pDoc;

public:
	void Add(T_CHRP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_CHRP_D& rData);
		//{return m_chrp.Lookup(1,rData);}
	int GetCount();
		//{return m_chrp.GetCount();}

protected:
	CMap<T_CHRP_K,T_CHRP_K,T_CHRP_D,T_CHRP_D&>m_chrp;
};

#endif