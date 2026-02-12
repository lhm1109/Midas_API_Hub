#ifndef __DB_ANOP_DB_H__
#define __DB_ANOP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_ANOP
{
public:
	CDB_ANOP();
	virtual ~CDB_ANOP();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_ANOP_D& rData);
	BOOL Del();
	
public:
	BOOL Get(T_ANOP_D& rData);
	int GetCount();
	POSITION GetStart() { return m_anop.GetStartPosition(); }
	void GetNext(POSITION& rNextPosition, T_ANOP_K& rKey, T_ANOP_D& rData) { m_anop.GetNextAssoc(rNextPosition, rKey, rData); }

protected:
	CMap<T_ANOP_K,T_ANOP_K,T_ANOP_D,T_ANOP_D&>m_anop;
};

#endif
