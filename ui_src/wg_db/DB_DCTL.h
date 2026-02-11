#ifndef __DB_DCTL_DB_H__
#define __DB_DCTL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DCTL
{
public:
	CDB_DCTL();
	virtual ~CDB_DCTL();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCTL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DCTL_D& rData);
		//{return m_dctl.Lookup(1,rData);}
	int GetCount();
		//{return m_dctl.GetCount();}

protected:
	CMap<T_DCTL_K,T_DCTL_K,T_DCTL_D,T_DCTL_D&>m_dctl;
};

#endif