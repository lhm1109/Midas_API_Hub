#ifndef __DB_MVCTch_DB_H__
#define __DB_MVCTch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCTch
{
public:
	CDB_MVCTch();
	virtual ~CDB_MVCTch();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCTch_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCTch_D& rData);
		//{return m_mvctch.Lookup(1,rData);}
	int GetCount();
		//{return m_mvctch.GetCount();}

protected:
	CMap<T_MVCTch_K,T_MVCTch_K,T_MVCTch_D,T_MVCTch_D&>m_mvctch;
};

#endif