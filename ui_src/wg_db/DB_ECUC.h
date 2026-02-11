#ifndef __DB_ECUC_DB_H__
#define __DB_ECUC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ECUC
{
public:
	CDB_ECUC();
	virtual ~CDB_ECUC();

public:
	void Add(T_ECUC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_ECUC_D& rData)
		{return m_ecuc.Lookup(1,rData);}
	int GetCount()
		{return m_ecuc.GetCount();}

protected:
	CMap<T_ECUC_K,T_ECUC_K,T_ECUC_D,T_ECUC_D&>m_ecuc;
};

#endif