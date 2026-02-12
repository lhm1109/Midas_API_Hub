#ifndef __DB_DYFG_DB_H__
#define __DB_DYFG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DYFG
{
public:
	CDB_DYFG();
	virtual ~CDB_DYFG();

public:
	void Add(T_DYFG_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DYFG_D& rData)
		{return m_dyfg.Lookup(1,rData);}
	int GetCount()
		{return m_dyfg.GetCount();}

protected:
	CMap<T_DYFG_K,T_DYFG_K,T_DYFG_D,T_DYFG_D&>m_dyfg;
};

#endif