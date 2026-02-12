#ifndef __DB_MCCS_DB_H__
#define __DB_MCCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MCCS
{
public:
	CDB_MCCS();
	virtual ~CDB_MCCS();

public:
	void Add(T_MCCS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MCCS_D& rData)
		{return m_mccs.Lookup(1,rData);}
	int GetCount()
		{return m_mccs.GetCount();}

protected:
	CMap<T_MCCS_K,T_MCCS_K,T_MCCS_D,T_MCCS_D&>m_mccs;
};

#endif