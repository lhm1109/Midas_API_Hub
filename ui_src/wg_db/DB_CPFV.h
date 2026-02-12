#ifndef __DB_CPFV_DB_H__
#define __DB_CPFV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CPFV
{
public:
	CDB_CPFV();
	virtual ~CDB_CPFV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPFV_K Key,T_CPFV_D& rData);
	BOOL Del(T_CPFV_K Key);

public:
	BOOL Get(T_CPFV_K Key,T_CPFV_D& rData);
		//{return m_cpfv.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpfv.GetCount();}
	POSITION GetStart();
		//{return m_cpfv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPFV_K& rKey,T_CPFV_D& rData);
		//{m_cpfv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPFV_K,T_CPFV_K,T_CPFV_D,T_CPFV_D&>m_cpfv;
};

#endif

