#ifndef __DB_BLDC_DB_H__
#define __DB_BLDC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_BLDC
{
public:
	CDB_BLDC();
	virtual ~CDB_BLDC();
	//CDBDoc* m_pDoc;

public:
	void Add(T_BLDC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_BLDC_D& rData)
		{return m_bldc.Lookup(1,rData);}
	int GetCount()
		{return m_bldc.GetCount();}

protected:
	CMap<T_BLDC_K,T_BLDC_K,T_BLDC_D,T_BLDC_D&>m_bldc;
};

#endif
