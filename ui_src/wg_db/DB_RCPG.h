#ifndef __DB_RCPG_DB_H__
#define __DB_RCPG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_RCPG
{
public:
	CDB_RCPG();
	virtual ~CDB_RCPG();
	//CDBDoc* m_pDoc;

public:
	void Add(T_RCPG_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCPG_D& rData);
		//{return m_rcpg.Lookup(1,rData);}
	int GetCount();
		//{return m_rcpg.GetCount();}

protected:
	CMap<T_RCPG_K,T_RCPG_K,T_RCPG_D,T_RCPG_D&>m_rcpg;
};

#endif