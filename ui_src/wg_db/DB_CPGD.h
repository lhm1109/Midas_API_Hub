#ifndef __DB_CPGD_DB_H__
#define __DB_CPGD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_CPGD
{
public:
	CDB_CPGD();
	virtual ~CDB_CPGD();
	//CDBDoc* m_pDoc;

public:
	void Add(T_CPGD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_CPGD_D& rData);
		//{return m_cpgd.Lookup(1,rData);}
	int GetCount();
		//{return m_cpgd.GetCount();}

protected:
	CMap<T_CPGD_K,T_CPGD_K,T_CPGD_D,T_CPGD_D&>m_cpgd;
};

#endif