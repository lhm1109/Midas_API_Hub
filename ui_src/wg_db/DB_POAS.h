#ifndef __DB_POAS_DB_H__
#define __DB_POAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POAS
{
public:
	CDB_POAS();
	virtual ~CDB_POAS();

public:
	void Add(T_POAS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_POAS_D& rData)
		{return m_poas.Lookup(1,rData);}
	int GetCount()
		{return m_poas.GetCount();}

protected:
	CMap<T_POAS_K,T_POAS_K,T_POAS_D,T_POAS_D&>m_poas;
};

#endif
