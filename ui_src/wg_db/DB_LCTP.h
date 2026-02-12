#ifndef __DB_LCTP_DB_H__
#define __DB_LCTP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LCTP
{
public:
	CDB_LCTP();
	virtual ~CDB_LCTP();
	CDBDoc* m_pDoc;

public:
	void Add(T_LCTP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_LCTP_D& rData);
		//{return m_lctp.Lookup(1,rData);}
	int GetCount();
		//{return m_lctp.GetCount();}

protected:
	CMap<T_LCTP_K,T_LCTP_K,T_LCTP_D,T_LCTP_D&>m_lctp;
};

#endif