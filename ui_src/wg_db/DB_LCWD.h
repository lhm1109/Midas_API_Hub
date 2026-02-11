#ifndef __DB_LCWD_DB_H__
#define __DB_LCWD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LCWD
{
public:
	CDB_LCWD();
	virtual ~CDB_LCWD();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_LCWD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_LCWD_D& rData);
	int GetCount();
	
protected:
	CMap<T_LCWD_K,T_LCWD_K,T_LCWD_D,T_LCWD_D&>m_lcwd;
};

#endif

