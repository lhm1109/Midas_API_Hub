#ifndef __DB_ASPM_DB_H__
#define __DB_ASPM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_ASPM
{
public:
	CDB_ASPM();
	virtual ~CDB_ASPM();
	CDBDoc* m_pDoc;

public:
	void Add(T_ASPM_D& rData);
	BOOL Del();

public:
	BOOL Get(T_ASPM_D& rData);
		//{return m_aspm.Lookup(1,rData);}
	int GetCount();
		//{return m_aspm.GetCount();}

protected:
	CMap<T_ASPM_K,T_ASPM_K,T_ASPM_D,T_ASPM_D&>m_aspm;
};

#endif