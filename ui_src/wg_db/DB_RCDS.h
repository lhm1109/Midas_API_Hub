#ifndef __DB_RCDS_DB_H__
#define __DB_RCDS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RCDS
{
public:
	CDB_RCDS();
	virtual ~CDB_RCDS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCDS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCDS_D& rData);
	BOOL Get(T_RCDS_K Key,T_RCDS_D& rData);
		//{return m_rcds.Lookup(1,rData);}
	int GetCount();
		//{return m_rcds.GetCount();}

protected:
	CMap<T_RCDS_K,T_RCDS_K,T_RCDS_D,T_RCDS_D&>m_rcds;
};

#endif