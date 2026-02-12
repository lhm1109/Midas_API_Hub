#ifndef __DB_DSRC_DB_H__
#define __DB_DSRC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DSRC
{
public:
	CDB_DSRC();
	virtual ~CDB_DSRC();
	CDBDoc* m_pDoc;

public:
	void Add(T_DSRC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DSRC_D& rData);
		//{return m_dsrc.Lookup(1,rData);}
	int GetCount();
		//{return m_dsrc.GetCount();}

protected:
	CMap<T_DSRC_K,T_DSRC_K,T_DSRC_D,T_DSRC_D&>m_dsrc;
};

#endif