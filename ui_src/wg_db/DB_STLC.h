#ifndef __DB_STLC_DB_H__
#define __DB_STLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_STLC
{
public:
	CDB_STLC();
	virtual ~CDB_STLC();
	//CDBDoc* m_pDoc;

public:
	void Add(T_STLC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_STLC_D& rData);
		//{return m_stlc.Lookup(1,rData);}
	int GetCount();
		//{return m_stlc.GetCount();}

protected:
	CMap<T_STLC_K,T_STLC_K,T_STLC_D,T_STLC_D&>m_stlc;
};

#endif