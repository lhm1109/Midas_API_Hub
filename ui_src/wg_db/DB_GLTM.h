#ifndef __DB_GLTM_DB_H__
#define __DB_GLTM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GLTM
{
public:
	CDB_GLTM();
	virtual ~CDB_GLTM();

public:
	void Add(T_GLTM_D& rData);
	BOOL Del();

public:
	BOOL Get(T_GLTM_D& rData)
		{return m_gltm.Lookup(1,rData);}
	int GetCount()
		{return m_gltm.GetCount();}

protected:
	CMap<T_GLTM_K,T_GLTM_K,T_GLTM_D,T_GLTM_D&>m_gltm;
};

#endif