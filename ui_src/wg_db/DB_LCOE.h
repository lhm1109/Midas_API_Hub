#ifndef __DB_LCOE_DB_H__
#define __DB_LCOE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LCOE
{
public:
	CDB_LCOE();
	virtual ~CDB_LCOE();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_LCOE_D& rData);
	BOOL Del();

public:
	BOOL Get(T_LCOE_D& rData);
	int GetCount();
	
protected:
	CMap<T_LCOE_K,T_LCOE_K,T_LCOE_D,T_LCOE_D&>m_lcoe;
};

#endif

