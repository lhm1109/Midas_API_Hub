#ifndef __DB_ACTL_DB_H__
#define __DB_ACTL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ACTL
{
public:
	CDB_ACTL();
	virtual ~CDB_ACTL();

public:
	void Add(T_ACTL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_ACTL_D& rData)
		{return m_actl.Lookup(1,rData);}
	int GetCount()
		{return m_actl.GetCount();}

protected:
	CMap<T_ACTL_K,T_ACTL_K,T_ACTL_D,T_ACTL_D&>m_actl;
};

#endif