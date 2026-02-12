#ifndef __DB_CESO_DB_H__
#define __DB_CESO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CESO
{
public:
	CDB_CESO();
	virtual ~CDB_CESO();

public:
	void Add(T_CESO_D& rData);
	BOOL Del();

public:
	BOOL Get(T_CESO_D& rData)
		{return m_ceso.Lookup(1,rData);}
	int GetCount()
		{return m_ceso.GetCount();}

protected:
	CMap<T_CESO_K,T_CESO_K,T_CESO_D,T_CESO_D&>m_ceso;
};

#endif