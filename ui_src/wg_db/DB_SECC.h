#ifndef __DB_SECC_DB_H__
#define __DB_SECC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECC
{
public:
	CDB_SECC();
	virtual ~CDB_SECC();

public:
	void Add(T_SECC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_SECC_D& rData)
		{return m_secc.Lookup(1,rData);}
	int GetCount()
		{return m_secc.GetCount();}

protected:
	CMap<T_SECC_K,T_SECC_K,T_SECC_D,T_SECC_D&>m_secc;
};

#endif