#ifndef __DB_NCRN_DB_H__
#define __DB_NCRN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NCRN
{
public:
	CDB_NCRN();
	virtual ~CDB_NCRN();

public:
	void Add(T_NCRN_D& rData);
	BOOL Del();

public:
	BOOL Get(T_NCRN_D& rData)
		{return m_ncrn.Lookup(1,rData);}
	int GetCount()
		{return m_ncrn.GetCount();}

protected:
	CMap<T_NCRN_K,T_NCRN_K,T_NCRN_D,T_NCRN_D&> m_ncrn;
};

#endif