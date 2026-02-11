#ifndef __DB_STYP_DB_H__
#define __DB_STYP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STYP
{
public:
	CDB_STYP();
	virtual ~CDB_STYP();

public:
	void Add(T_STYP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_STYP_D& rData)
		{return m_styp.Lookup(1,rData);}
	int GetCount()
		{return m_styp.GetCount();}

protected:
	CMap<T_STYP_K,T_STYP_K,T_STYP_D,T_STYP_D&>m_styp;
};

#endif