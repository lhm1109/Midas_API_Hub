#ifndef __DB_SBRI_DB_H__
#define __DB_SBRI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SBRI
{
public:
	CDB_SBRI();
	virtual ~CDB_SBRI();

public:
	void Add(T_SBRI_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SBRI_D& rData)
		{return m_sbri.Lookup(1,rData);}
	int GetCount()
		{return m_sbri.GetCount();}

protected:
	CMap<T_SBRI_K,T_SBRI_K,T_SBRI_D,T_SBRI_D&>m_sbri;
};

#endif