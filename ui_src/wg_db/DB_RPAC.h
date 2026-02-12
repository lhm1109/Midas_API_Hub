#ifndef __DB_RPAC_DB_H__
#define __DB_RPAC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RPAC
{
public:
	CDB_RPAC();
	virtual ~CDB_RPAC();
	CDBDoc* m_pDoc;

public:
	void Add(T_RPAC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RPAC_D& rData); 
	int GetCount();

protected:
	CMap<T_RPAC_K,T_RPAC_K,T_RPAC_D,T_RPAC_D&>m_rpac; 
};

#endif