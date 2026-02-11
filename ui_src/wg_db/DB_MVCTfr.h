#ifndef __DB_MVCTfr_DB_H__
#define __DB_MVCTfr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCTfr
{
public:
	CDB_MVCTfr();
	virtual ~CDB_MVCTfr();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCTfr_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCTfr_D& rData);
		
	int GetCount();
		
protected:
	CMap<T_MVCTfr_K,T_MVCTfr_K,T_MVCTfr_D,T_MVCTfr_D&>m_mvctfr;
};

#endif