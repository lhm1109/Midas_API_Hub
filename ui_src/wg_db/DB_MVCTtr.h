#ifndef __DB_MVCTtrtr_DB_H__
#define __DB_MVCTtrtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCTtr
{
public:
	CDB_MVCTtr();
	virtual ~CDB_MVCTtr();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCTtr_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCTtr_D& rData);
		//{return m_mvcttr.Lookup(1,rData);}
	int GetCount();
		//{return m_mvcttr.GetCount();}

protected:
	CMap<T_MVCTtr_K,T_MVCTtr_K,T_MVCTtr_D,T_MVCTtr_D&>m_mvcttr;
};

#endif