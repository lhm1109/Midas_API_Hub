#ifndef __DB_YSCO_DB_H__
#define __DB_YSCO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_YSCO
{
public:
	CDB_YSCO();
	virtual ~CDB_YSCO();
	CDBDoc* m_pDoc;

public:
	void Add(T_YSCO_D& rData);
	BOOL Del();

public:
	BOOL Get(T_YSCO_D& rData);
		//{return m_ysco.Lookup(1,rData);}
	int GetCount();
		//{return m_ysco.GetCount();}

protected:
	CMap<T_YSCO_K,T_YSCO_K,T_YSCO_D,T_YSCO_D&>m_ysco;
};

#endif