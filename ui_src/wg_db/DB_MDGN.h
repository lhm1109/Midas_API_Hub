#ifndef __DB_MDGN_DB_H__
#define __DB_MDGN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MDGN
{
public:
	CDB_MDGN();
	virtual ~CDB_MDGN();
	CDBDoc* m_pDoc;

public:
	void Add(T_MDGN_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MDGN_D& rData);
		//{return m_mdgn.Lookup(1,rData);}
	int GetCount();
		//{return m_mdgn.GetCount();}

protected:
	CMap<T_MDGN_K,T_MDGN_K,T_MDGN_D,T_MDGN_D&>m_mdgn;
};

#endif