#ifndef __DB_DAGB_DB_H__
#define __DB_DAGB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DAGB
{
public:
	CDB_DAGB();
	virtual ~CDB_DAGB();

public:
	void Add(T_DAGB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DAGB_D& rData)
		{return m_Damping.Lookup(1,rData);}
	int GetCount()
		{return m_Damping.GetCount();}

protected:
	CMap<T_DAGB_K,T_DAGB_K,T_DAGB_D,T_DAGB_D&>m_Damping;
};

#endif

