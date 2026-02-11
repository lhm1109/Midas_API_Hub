#ifndef __DB_CCTL_DB_H__
#define __DB_CCTL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CCTL
{
public:
	CDB_CCTL();
	virtual ~CDB_CCTL();

public:
	void Add(T_CCTL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_CCTL_D& rData)
		{return m_cctl.Lookup(1,rData);}
	int GetCount()
		{return m_cctl.GetCount();}

protected:
	CMap<T_CCTL_K,T_CCTL_K,T_CCTL_D,T_CCTL_D&>m_cctl;
};

#endif