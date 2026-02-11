#ifndef __DB_RCBG_DB_H__
#define __DB_RCBG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_RCBG
{
public:
	CDB_RCBG();
	virtual ~CDB_RCBG();
	//CDBDoc* m_pDoc;

public:
	void Add(T_RCBG_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCBG_D& rData);
		//{return m_rcbg.Lookup(1,rData);}
	int GetCount();
		//{return m_rcbg.GetCount();}

protected:
	CMap<T_RCBG_K,T_RCBG_K,T_RCBG_D,T_RCBG_D&>m_rcbg;
};

#endif