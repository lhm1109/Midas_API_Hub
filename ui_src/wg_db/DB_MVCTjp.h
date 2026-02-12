#ifndef __DB_MVCTjp_DB_H__
#define __DB_MVCTjp_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCTjp
{
public:
	CDB_MVCTjp();
	virtual ~CDB_MVCTjp();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCTjp_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCTjp_D& rData);
		//{return m_mvctjp.Lookup(1,rData);}
	int GetCount();
		//{return m_mvctjp.GetCount();}

protected:
	CMap<T_MVCTjp_K,T_MVCTjp_K,T_MVCTjp_D,T_MVCTjp_D&>m_mvctjp;
};

#endif