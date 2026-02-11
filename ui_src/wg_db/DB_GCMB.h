#ifndef __DB_GCMB_DB_H__
#define __DB_GCMB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GCMB
{
public:
	CDB_GCMB();
	virtual ~CDB_GCMB();
	
public:
	void Add(T_GCMB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_GCMB_D& rData)
		{return m_gcmb.Lookup(1,rData);}
	int GetCount()
		{return m_gcmb.GetCount();}

protected:
	CMap<T_GCMB_K,T_GCMB_K,T_GCMB_D,T_GCMB_D&>m_gcmb;
};

#endif