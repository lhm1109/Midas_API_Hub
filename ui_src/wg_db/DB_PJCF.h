#ifndef __DB_PJCF_DB_H__
#define __DB_PJCF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PJCF
{
public:
	CDB_PJCF();
	virtual ~CDB_PJCF();

public:
	void Add(T_PJCF_D& rData);
	BOOL Del();

public:
	BOOL Get(T_PJCF_D& rData)
		{return m_pjcf.Lookup(1,rData);}
	int GetCount()
		{return m_pjcf.GetCount();}

protected:
	CMap<T_PJCF_K,T_PJCF_K,T_PJCF_D,T_PJCF_D&>m_pjcf;
};

#endif