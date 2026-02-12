#ifndef __DB_NLCT_DB_H__
#define __DB_NLCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_NLCT
{
public:
	CDB_NLCT();
	virtual ~CDB_NLCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_NLCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_NLCT_D& rData);
	int GetCount();
	POSITION GetStart() { return m_nlct.GetStartPosition(); }
	void GetNext(POSITION& rNextPosition, T_NLCT_K& rKey, T_NLCT_D& rData) { m_nlct.GetNextAssoc(rNextPosition, rKey, rData); }

protected:
	CMap<T_NLCT_K, T_NLCT_K, T_NLCT_D, T_NLCT_D&>m_nlct;
};

#endif