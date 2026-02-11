#ifndef __DB_MLCB_DB_H__
#define __DB_MLCB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MLCB
{
public:
	CDB_MLCB();
	virtual ~CDB_MLCB();
	CDBDoc* m_pDoc;

public:
	void Add(T_MLCB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MLCB_D& rData);
		//{return m_mlcb.Lookup(1,rData);}
	int GetCount();
		//{return m_mlcb.GetCount();}

protected:
	CMap<T_MLCB_K,T_MLCB_K,T_MLCB_D,T_MLCB_D&>m_mlcb;
};

#endif