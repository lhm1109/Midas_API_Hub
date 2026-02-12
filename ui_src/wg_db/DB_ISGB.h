#ifndef __DB_ISGB_DB_H__
#define __DB_ISGB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ISGB
{
public:
	CDB_ISGB();
	virtual ~CDB_ISGB();

public:
	void Add(T_ISGB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_ISGB_D& rData)
		{return m_Isolation.Lookup(1,rData);}
	int GetCount()
		{return m_Isolation.GetCount();}

protected:
	CMap<T_ISGB_K,T_ISGB_K,T_ISGB_D,T_ISGB_D&>m_Isolation;
};

#endif

