#ifndef __DB_RCWG_DB_H__
#define __DB_RCWG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_RCWG
{
public:
	CDB_RCWG();
	virtual ~CDB_RCWG();
	//CDBDoc* m_pDoc;

public:
	void Add(T_RCWG_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCWG_D& rData);
		//{return m_rcwg.Lookup(1,rData);}
	int GetCount();
		//{return m_rcwg.GetCount();}

protected:
	CMap<T_RCWG_K,T_RCWG_K,T_RCWG_D,T_RCWG_D&>m_rcwg;
};

#endif