#ifndef __DB_MVCTbs_DB_H__
#define __DB_MVCTbs_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCTbs
{
public:
	CDB_MVCTbs();
	virtual ~CDB_MVCTbs();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCTbs_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCTbs_D& rData);
		//{return m_mvctbs.Lookup(1,rData);}
	int GetCount();
		//{return m_mvctbs.GetCount();}

protected:
	CMap<T_MVCTbs_K,T_MVCTbs_K,T_MVCTbs_D,T_MVCTbs_D&>m_mvctbs;
};

#endif