#ifndef __DB_CAMB_DB_H__
#define __DB_CAMB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CAMB
{
public:
	CDB_CAMB();
	virtual ~CDB_CAMB();
	
public:
	void Add(T_CAMB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_CAMB_D& rData)
		{return m_camb.Lookup(1,rData);}
	int GetCount()
		{return m_camb.GetCount();}

protected:
	CMap<T_CAMB_K,T_CAMB_K,T_CAMB_D,T_CAMB_D&>m_camb;
};

#endif