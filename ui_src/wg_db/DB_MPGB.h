#ifndef __DB_MPGB_DB_H__
#define __DB_MPGB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPGB
{
public:
	CDB_MPGB();
	virtual ~CDB_MPGB();

public:
	void Add(T_MPGB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MPGB_D& rData)
		{return m_mpgb.Lookup(1,rData);}
	int GetCount()
		{return m_mpgb.GetCount();}

protected:
	CMap<T_MPGB_K,T_MPGB_K,T_MPGB_D,T_MPGB_D&>m_mpgb;
};

#endif