#ifndef __DB_HPEA_DB_H__
#define __DB_HPEA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HPEA
{
public:
	CDB_HPEA();
	virtual ~CDB_HPEA();

public:
	void Add(T_HPEA_D& rData);
	BOOL Del();

public:
	BOOL Get(T_HPEA_D& rData)
		{return m_hpea.Lookup(1,rData);}
	int GetCount()
		{return m_hpea.GetCount();}

protected:
	CMap<T_HPEA_K,T_HPEA_K,T_HPEA_D,T_HPEA_D&>m_hpea;
};

#endif